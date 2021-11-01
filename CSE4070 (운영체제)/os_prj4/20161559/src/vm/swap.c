#include <bitmap.h>
#include "vm/swap.h"
#include "devices/block.h"
#include "threads/synch.h"

#define SECTORS_PER_PAGE 8
#define SWAP_FREE 0
#define SWAP_USED 1

struct lock swap_lock;
struct bitmap *swap_bitmap;
struct block *swap_block;


void swap_init(void) {
	lock_init(&swap_lock);
	swap_block = block_get_role(BLOCK_SWAP);
	swap_bitmap = bitmap_create(block_size(swap_block) / SECTORS_PER_PAGE);
}

void swap_in(size_t used_index, void *kaddr) {
	lock_acquire(&swap_lock);

	for(size_t i=0; i<SECTORS_PER_PAGE; i++)
		block_read(swap_block, used_index*SECTORS_PER_PAGE+i, kaddr + i*BLOCK_SECTOR_SIZE);
	bitmap_flip(swap_bitmap, used_index);

	lock_release(&swap_lock);
}

size_t swap_out(void *kaddr) {
	lock_acquire(&swap_lock);

	size_t free_index = bitmap_scan_and_flip(swap_bitmap, 0, 1, SWAP_FREE);
	for(int i=0; i<SECTORS_PER_PAGE; i++)
		block_write(swap_block, free_index*SECTORS_PER_PAGE+i, kaddr + i*BLOCK_SECTOR_SIZE);

	lock_release(&swap_lock);

	return free_index;
}
