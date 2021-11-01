#include "vm/frame.h"
#include "threads/thread.h"
#include "vm/swap.h"
#include "userprog/pagedir.h"

void lru_list_init(void) {
	list_init(&lru_list);
	lock_init(&lru_list_lock);
	lru_clock = NULL;
}

void add_page_to_lru_list(struct page *page) {
	lock_acquire(&lru_list_lock);
	list_push_back(&lru_list, &page->lru);
	lock_release(&lru_list_lock);
}

void del_page_from_lru_list(struct page *page) {
	if(lru_clock==&page->lru)
		lru_clock = list_remove(&page->lru);
	else
		list_remove(&page->lru);
}

static struct list_elem* get_next_lru_clock(void) {
	if(list_empty(&lru_list))
		return NULL;
	else if(lru_clock==NULL)
		lru_clock = list_begin(&lru_list);
	else {
		lru_clock = list_next(lru_clock);
		if(lru_clock==list_end(&lru_list))
			lru_clock = list_begin(&lru_list);
	}

	return lru_clock;
}

void try_to_free_pages(void) {
	lock_acquire(&lru_list_lock);

	struct thread *page_thread;
	struct page *lru_page;

	if(list_empty(&lru_list)) {
		lock_release(&lru_list_lock);
		return;
	}

	//traversing the clock until the victim is found.
	while(true) {
		
		lru_clock = get_next_lru_clock();
		if(!lru_clock) {
			lock_release(&lru_list_lock);
			return;
		}
		lru_page = list_entry(lru_clock, struct page, lru);
		page_thread = lru_page->thread;

		//giving a second chance
		if(pagedir_is_accessed(page_thread->pagedir, lru_page->vme->vaddr)) {
			pagedir_set_accessed(page_thread->pagedir, lru_page->vme->vaddr, false);
			continue;
		}

		//swap out if the page is dirty(modified)
		if(pagedir_is_dirty(page_thread->pagedir, lru_page->vme->vaddr) || lru_page->vme->type==VM_ANON) {
			lru_page->vme->type = VM_ANON;
			lru_page->vme->swap_slot = swap_out(lru_page->kaddr);
		}

		lru_page->vme->is_loaded = false;
		pagedir_clear_page(page_thread->pagedir, lru_page->vme->vaddr);
		__free_page(lru_page);
		break;
	}

	lru_clock = get_next_lru_clock();
	lock_release(&lru_list_lock);
	return;
}
