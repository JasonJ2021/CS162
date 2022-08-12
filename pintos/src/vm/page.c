#include "vm/page.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include <hash.h>


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

void vm_destroy(struct hash *vm){
    hash_destroy(vm , vme_destroy_func);
}

void vm_init(struct hash *h)
{
    hash_init(&thread_current()->vm, vm_hash_func, vm_less_func, NULL);
}
