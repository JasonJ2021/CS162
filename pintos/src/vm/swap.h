#ifndef SWAP_H
#define SWAP_H
#include "threads/vaddr.h"
#include "devices/block.h"
#define SECTOR_SIZE 512
#define PAGE_SECTORS PGSIZE / SECTOR_SIZE

void swap_init();
void read_from_block(uint8_t *kpage, block_sector_t sector_index);
block_sector_t write_to_block(uint8_t *kpage, bool flag, block_sector_t sector_index);

#endif
