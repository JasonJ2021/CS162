#include "vm/swap.h"
#include "devices/block.h"
#include <bitmap.h>
#include "threads/synch.h"

static struct lock bitmap_lock;
static struct block *swap_block;
static struct bitmap *swap_bitmap;
void swap_init()
{
	swap_block = block_get_role(BLOCK_SWAP);
	lock_init(&bitmap_lock);
	swap_bitmap = bitmap_create(block_size(swap_block));
	bitmap_set_all(swap_bitmap, false);
}

// 把sector_index开始的8个扇区内容读到kpage中
void read_from_block(uint8_t *kpage, block_sector_t sector_index)
{
	for (uint32_t i = 0; i < PAGE_SECTORS; i++)
	{
		block_read(swap_block, sector_index + i, kpage + SECTOR_SIZE * i);
	}
}

// 把kpage中的4096字节写到连续的8个扇区中
// flag = 1 写到sector_index开始的8个扇区中
// flag = 0 由bitmap分配8个扇区
// 返回写入扇区开头的index
block_sector_t write_to_block(uint8_t *kpage, bool flag, block_sector_t sector_index)
{
	if (flag)
	{
		for (uint32_t i = 0; i < PAGE_SECTORS; i++)
		{
			block_write(swap_block, sector_index + i, kpage + SECTOR_SIZE * i);
		}
		return sector_index;
	}
	else
	{
		lock_acquire(&bitmap_lock);
		sector_index = bitmap_scan_and_flip(swap_bitmap , 0,PAGE_SECTORS,false);
		lock_release(&bitmap_lock);
		for (uint32_t i = 0; i < PAGE_SECTORS; i++)
		{
			block_write(swap_block, sector_index + i, kpage + SECTOR_SIZE * i);
		}
		return sector_index;
	}
}