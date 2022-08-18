#ifndef PAGE_HDER
#define PAGE_HDER
#include <stdint.h>
#include <stdbool.h>
#include <hash.h>
#include "vm/frame.h"
#include "devices/block.h"
#include "list.h"
#define mapid_t int32_t
enum vm_type{
  ELF_EXEC,
  GEN_FILE,  
  SWAP_AREA,
  STACK
};
struct vm_entry{
    uintptr_t pg_number; // Virtual Page number: pd_no : pt_no , get from pg_no(*va)
    bool writable ;      // write = 1 表示可以读写; write = 0 表示只能读 . 这里和pte中PTE_W一致
    struct file *file;   // file-backup
    int32_t offset;      // 当前ELF page 映射到file *file 的位置
    int32_t file_offset;  // mmap file 映射的位置
    enum vm_type page_type; // 记录当前page的种类，elf执行文件/文件/对应交换区
    int32_t data_aside;  // 在page中数据的大小, 读取100bytes 页中其他的字节要清零
    // swap area part2 中完成
    struct hash_elem elem; // hash elem
    struct frame *frame_; // 对应的物理页
    block_sector_t block_index; // 物理页在swap中的back对应扇区的开始位置
    struct list_elem mmap_elem;
};
struct mmap_file{
  mapid_t mmap_id;
  struct file *mapping_file;
  struct list_elem elem;
  struct list file_owned_vms;
};

void vm_init(struct hash *h);
struct vm_entry *find_vme(struct hash *vm, void *vaddr);
bool insert_vme(struct hash *vm , struct vm_entry *entry);
void vm_destroy(struct hash *vm);
void mmap_init();
mapid_t mmap(int fd, void *addr);
void munmap (mapid_t mapping);
void munmap_all();
#endif