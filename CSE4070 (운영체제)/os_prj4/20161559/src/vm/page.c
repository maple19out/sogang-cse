#include <string.h>
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "threads/malloc.h"
#include "filesys/file.h"
#include "userprog/syscall.h"
#include "vm/page.h"
#include "vm/frame.h"

static unsigned vm_hash_func(const struct hash_elem *e, void *aux UNUSED) {
	struct vm_entry *v = hash_entry(e, struct vm_entry, elem);

	return hash_int((int)v->vaddr);
}

static bool vm_less_func(const struct hash_elem *a, const struct hash_elem *b, void *aux UNUSED) {
	struct vm_entry *v1 = hash_entry(a, struct vm_entry, elem);
	struct vm_entry *v2 = hash_entry(b, struct vm_entry, elem);

	return v1->vaddr < v2->vaddr;
}

static void vm_destroy_func(struct hash_elem *e UNUSED, void *aux UNUSED) {
}

void vm_init(struct hash *vm) {
	hash_init(vm, vm_hash_func, vm_less_func, NULL);
}

bool insert_vme(struct hash *vm, struct vm_entry *vme) {
	struct hash_elem *result =	hash_insert(vm, &vme->elem);

	if(!result)
		return true;
	else
		return false;
}

bool delete_vme(struct hash *vm, struct vm_entry *vme) {
	struct hash_elem *result = hash_delete(vm, &vme->elem);

	if(result) {
		free(vme);
		return true;
	}
	else
		return false;
}

struct vm_entry* find_vme(const void *vaddr) {
	struct hash_elem *elem;
	struct vm_entry vme;

	vme.vaddr = pg_round_down(vaddr);

	elem = hash_find(&thread_current()->vm, &vme.elem);

	if(!elem)
		return NULL;
	else 
		return hash_entry(elem, struct vm_entry, elem);
}

void vm_destroy(struct hash *vm) {
	hash_destroy(vm, vm_destroy_func); 
}

bool load_file(void *kaddr, struct vm_entry *vme) {
	if(file_read_at(vme->file, kaddr, vme->read_bytes, vme->offset)!=(int)vme->read_bytes)
		return false;
	memset(kaddr+vme->read_bytes, 0, vme->zero_bytes);
	vme->is_loaded = true;

	return true;
}

struct page* alloc_page(enum palloc_flags flags) {
	struct page *new_page = (struct page*)malloc(sizeof(struct page));

	void *kaddr = palloc_get_page(flags);
	if(kaddr==NULL) {
		try_to_free_pages();
		kaddr = palloc_get_page(flags);
	}

	if(kaddr==NULL) 
		return NULL;

	new_page->kaddr = kaddr;
	new_page->thread = thread_current();
	add_page_to_lru_list(new_page);

	return new_page;
}

void free_page(void *kaddr) {
	struct list_elem *e;
	struct page *page;
	lock_acquire(&lru_list_lock);

	for(e=list_begin(&lru_list); e!=list_end(&lru_list); e=list_next(e)) {
		page = list_entry(e, struct page, lru);
		if(page->kaddr == kaddr) {
			__free_page(page);
			break;
		}
	}

	lock_release(&lru_list_lock);
}

void __free_page(struct page *page) {
	palloc_free_page(page->kaddr);
	del_page_from_lru_list(page);
	free(page);
}
