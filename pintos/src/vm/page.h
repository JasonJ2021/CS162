#ifndef PAGE_HDER
#define PAGE_HDER
#include <stdint.h>
#include <stdbool.h>
#include <hash.h>

enum vm_type{
  ELF_EXEC,
  GEN_FILE,  
  SWAP_AREA
};
struct vm_entry{
    uintptr_t pg_number; // Virtual Page number: pd_no : pt_no , get from pg_no(*va)
    bool writable ;      // write = 1 表示可以读写; write = 0 表示只能读 . 这里和pte中PTE_W一致
    struct file *file;   // file-backup
    int32_t offset;      // 当前vm page 映射到file *file 的位置
    enum vm_type page_type; // 记录当前page的种类，elf执行文件/文件/对应交换区
    int32_t data_aside;  // 在page中数据的大小, 读取100bytes 页中其他的字节要清零
    // swap area part2 中完成
    bool in_memory;      // 是否在memory中
    struct hash_elem elem; // hash elem
    bool is_stack;
};

void vm_init(struct hash *h);
struct vm_entry *find_vme(struct hash *vm, void *vaddr);
bool insert_vme(struct hash *vm , struct vm_entry *entry);
void vm_destroy(struct hash *vm);


#endif