#include "vm/frame.h"
#include "threads/synch.h"
#include "threads/palloc.h"
#include "userprog/pagedir.h"
#include "threads/vaddr.h"
#include "vm/swap.h"
#include "threads/thread.h"
#include <stdio.h>
static struct list frame_list; // 在init.c中初始化
static struct lock frame_lock;
static struct list_elem *clock_ptr;
void frame_list_init()
{
	lock_init(&frame_lock);
	list_init(&frame_list);
}

// 为entry分配一个frame
uint8_t *frame_alloc(struct vm_entry *entry)
{
	// 分配并设置frame
	struct frame *frame_ = (struct frame *)malloc(sizeof(struct frame));
	frame_->entry = entry;
	frame_->owner_thread = thread_current();
	frame_->kpage = palloc_get_page(PAL_USER);
	if(frame_->kpage == NULL){
		free(frame_);
		return evict_frame(entry);
	}
	entry->frame_ = frame_;
	// 把frame加入到frame_list中
	lock_acquire(&frame_lock);
	list_push_back(&frame_list, &frame_->elem);
	lock_release(&frame_lock);
	// 最终返回获得的kpage
	return frame_->kpage;
}

// free entry指向的frame & physical page
void *frame_free(struct vm_entry *entry)
{
	lock_acquire(&frame_lock);
	if(entry == NULL || entry->frame_ == NULL){
		lock_release(&frame_lock);
		return;
	}
	list_remove(&entry->frame_->elem);
	palloc_free_page(entry->frame_->kpage);
	free(entry->frame_);
	entry->frame_ = NULL;
	lock_release(&frame_lock);
}

/**
 * @brief 驱逐一个frame，把frame写回到磁盘，并且设置相应entry值，返回frame kpage
 *
 * @param entry
 * @return uint8_t*
 */
uint8_t *evict_frame(struct vm_entry *entry)
{
	struct list_elem *ptr;
	struct frame *frame_;
	uint32_t *pd;
	void *upage;
	lock_acquire(&frame_lock);
	for (ptr = list_begin(&frame_list); ptr != list_end(&frame_list);
			 ptr = list_next(ptr))
	{
		frame_ = list_entry(ptr, struct frame, elem);
		pd = frame_->owner_thread->pagedir;
		upage = frame_->entry->pg_number << PGBITS;
		if (pagedir_is_accessed(pd, upage))
		{
			pagedir_set_accessed(pd, upage, false);
		}
		else
		{
			list_remove(ptr);
			break;
		}
	}
	if (ptr == list_end(&frame_list))
	{
		frame_ = list_entry(list_begin(&frame_list), struct frame, elem);
		pd = frame_->owner_thread->pagedir;
		upage = frame_->entry->pg_number << PGBITS;
		list_remove(list_begin(&frame_list));
	}
	lock_release(&frame_lock);
	// 到现在找到了要驱逐的frame，并且把它移出了frame_list
	if (frame_->entry->page_type == ELF_EXEC)
	{
		if (pagedir_is_dirty(pd, upage))
		{
			pagedir_set_dirty(pd, upage, false);
			// ELF_EXEC 被修改过，应该写回到磁盘中
			frame_->entry->block_index = write_to_block(frame_->kpage, false, 0);
			frame_->entry->page_type = SWAP_AREA;
		}
	}
	else if (frame_->entry->page_type == STACK)
	{
		pagedir_set_dirty(pd, upage, false);
		// 如果当前是stack page , 把页写到swap空间中
		frame_->entry->block_index = write_to_block(frame_->kpage, 0, 0);
		frame_->entry->page_type = SWAP_AREA;
	}else if(frame_->entry->page_type == SWAP_AREA){
		if(pagedir_is_dirty(pd , upage)){
			pagedir_set_dirty(pd, upage, false);
			// back page 被修改过，需要重新被写回到磁盘中
			write_to_block(frame_->kpage, true, frame_->entry->block_index);
		}
	}
	// 设置相应页表和frame，并且把frame加到List中
	frame_->entry->frame_ = NULL;
	pagedir_clear_page(pd,upage);
	frame_->entry = entry;
	frame_->owner_thread = thread_current();
	lock_acquire(&frame_lock);
	list_push_back(&frame_list , &frame_->elem);
	lock_release(&frame_lock);
	return frame_->kpage;
}