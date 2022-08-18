#include "vm/page.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include <hash.h>
#include "threads/synch.h"
#include "threads/interrupt.h"
#include <debug.h>
#include "userprog/pagedir.h"

// typedef unsigned hash_hash_func (const struct hash_elem *e, void *aux);
static unsigned vm_hash_func(const struct hash_elem *e, void *aux)
{
	const struct vm_entry *vm_entry_ = hash_entry(e, struct vm_entry, elem);
	return hash_bytes(&vm_entry_->pg_number, sizeof vm_entry_->pg_number);
}

static bool vm_less_func(const struct hash_elem *a, const struct hash_elem *b, void *aux UNUSED)
{
	const struct vm_entry *vm_entry_1 = hash_entry(a, struct vm_entry, elem);
	const struct vm_entry *vm_entry_2 = hash_entry(b, struct vm_entry, elem);
	return vm_entry_1->pg_number < vm_entry_2->pg_number;
}

struct vm_entry *find_vme(struct hash *vm, void *vaddr)
{
	// 这里寻找vme很奇怪...
	struct vm_entry entry_temp;
	int32_t pg_number = pg_no(vaddr);
	entry_temp.pg_number = pg_number;
	struct hash_elem *elem_find = hash_find(vm, &entry_temp.elem);
	if (elem_find != NULL)
	{
		struct vm_entry *vm_entry_find = hash_entry(elem_find, struct vm_entry, elem);
		return vm_entry_find;
	}
	return NULL;
}

bool insert_vme(struct hash *vm, struct vm_entry *entry)
{
	hash_insert(vm, &entry->elem);
}

static void vme_destroy_func(struct hash_elem *e, void *aux UNUSED)
{
	struct vm_entry *entry = hash_entry(e, struct vm_entry, elem);
	free(entry);
}

void vm_destroy(struct hash *vm)
{
	hash_destroy(vm, vme_destroy_func);
}

void vm_init(struct hash *h)
{
	hash_init(&thread_current()->vm, vm_hash_func, vm_less_func, NULL);
}

mapid_t mmap(int fd, void *addr)
{
	struct mmap_file *mmap_file_ = (struct mmap_file *)malloc(sizeof(struct mmap_file));
	mmap_file_->mmap_id = thread_current()->next_mapid++;
	list_init(&mmap_file_->file_owned_vms);
	mmap_file_->mapping_file = file_reopen(thread_current()->fdt[fd]);
	off_t file_length_ = file_length(mmap_file_->mapping_file);
	// if file_length = 0 , fails
	if (file_length_ == 0)
	{
		file_close(mmap_file_->mapping_file);
		free(mmap_file_);
		return -1;
	}
	// if addr is not Page aligned or addr == 0, fails
	if ((uint32_t)addr % PGSIZE != 0 || addr == 0)
	{
		file_close(mmap_file_->mapping_file);
		free(mmap_file_);
		return -1;
	}

	// bad fd , fails
	if (fd < 2 || fd >= FDT_SIZE || thread_current()->fdt[fd] == NULL)
	{
		file_close(mmap_file_->mapping_file);
		free(mmap_file_);
		return -1;
	}
	// validate addr is used early
	bool valid_addr = true;
	{
		int32_t pages = (file_length_ - 1) / PGSIZE;
		for (int i = 0; i <= pages; i++)
		{
			if (find_vme(&thread_current()->vm, addr + PGSIZE * i) != NULL)
			{
				valid_addr = false;
				break;
			}
		}
	}
	if (!valid_addr)
	{
		file_close(mmap_file_->mapping_file);
		free(mmap_file_);
		return -1;
	}

	uint32_t read_bytes = file_length_;
	int32_t offset = 0;
	while (read_bytes > 0)
	{
		size_t page_read_bytes = read_bytes < PGSIZE ? read_bytes : PGSIZE;
		struct vm_entry *entry = (struct vm_entry *)malloc(sizeof(struct vm_entry));
		entry->pg_number = pg_no(addr + offset);
		entry->writable = true;
		entry->file = mmap_file_->mapping_file;
		entry->file_offset = offset;
		entry->data_aside = page_read_bytes;
		entry->frame_ = NULL;
		entry->page_type = GEN_FILE;
		insert_vme(&thread_current()->vm, entry);
		list_push_back(&mmap_file_->file_owned_vms, &entry->mmap_elem);

		/* Advance */
		read_bytes -= page_read_bytes;
		offset += PGSIZE;
	}

	list_push_back(&thread_current()->mmap_list, &mmap_file_->elem);
	return mmap_file_->mmap_id;
}

void munmap(mapid_t mapping)
{
	struct list_elem *e;
	struct mmap_file *mmap_file_ = NULL;
	for (e = list_begin(&thread_current()->mmap_list); e != list_end(&thread_current()->mmap_list); e = list_next(e))
	{
		mmap_file_ = list_entry(e, struct mmap_file, elem);
		if (mmap_file_->mmap_id == mapping)
		{
			break;
		}
	}
	if (e == list_end(&thread_current()->mmap_list))
	{
		return;
	}
	else
	{
		list_remove(e);
	}
	for (e = list_begin(&mmap_file_->file_owned_vms); e != list_end(&mmap_file_->file_owned_vms); e = list_next(e))
	{
		struct vm_entry *entry = list_entry(e, struct vm_entry, mmap_elem);
		uint32_t *upage = entry->pg_number << PGBITS;
		if (pagedir_is_dirty(thread_current()->pagedir, upage))
		{
			file_write_at(entry->file, upage, PGSIZE, entry->file_offset);
		}
		frame_free(entry);
		// 标记当前page不存在
		pagedir_clear_page(thread_current()->pagedir, upage);
		hash_delete(&thread_current()->vm, &entry->elem);
	}
	file_close(mmap_file_->mapping_file);
	free(mmap_file_);
}

void munmap_all()
{
	for(mapid_t i = 1 ; i < thread_current()->next_mapid ; i++){
		munmap(i);
	}
}
