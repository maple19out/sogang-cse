#ifndef VM_PAGE_H
#define VM_PAGE_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <debug.h>
#include <list.h>
#include <hash.h>
#include "threads/palloc.h"

#define VM_BIN 0
#define VM_FILE 1
#define VM_ANON 2

struct vm_entry {
	uint8_t type;
	void *vaddr;
	bool writable;

	bool is_loaded;
	struct file* file;

	size_t offset;
	size_t read_bytes;
	size_t zero_bytes;

	size_t swap_slot;

	struct hash_elem elem;
};

struct page {
	void *kaddr;
	struct vm_entry *vme;
	struct thread *thread;
	struct list_elem lru;
};

void vm_init(struct hash*);
bool insert_vme(struct hash *vm, struct vm_entry *vme);
bool delete_vme(struct hash *vm, struct vm_entry *vme);
struct vm_entry *find_vme(const void *vaddr);
void vm_destroy(struct hash *vm);

bool load_file(void* kaddr, struct vm_entry *vme);

struct page* alloc_page(enum palloc_flags flags);
void free_page(void *kaddr);
void __free_page(struct page *page);

#endif
