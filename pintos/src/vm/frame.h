#ifndef FRAME_H
#define FRAME_H
#include <list.h>
#include "vm/page.h"
#include "threads/malloc.h"
#include "threads/thread.h"



struct frame{
    struct vm_entry*entry; // frame映射的虚拟页entry
    struct thread *owner_thread;    // 拥有该物理页的线程，方便之后查找
    uint8_t *kpage;
    struct list_elem elem;
};

void frame_list_init();
uint8_t *frame_alloc(struct vm_entry *entry);
void *frame_free(struct vm_entry *entry);
uint8_t *evict_frame(struct vm_entry *entry);

#endif