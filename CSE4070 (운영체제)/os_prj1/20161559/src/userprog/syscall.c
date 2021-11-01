#include "userprog/syscall.h"
#include "userprog/pagedir.h"
#include "userprog/process.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "devices/shutdown.h"
#include "devices/input.h"


static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

void check_address(void *addr, int count);
void get_arguments(void *esp, uint32_t *argv, int count);

static tid_t exec(const char* file);
static int wait(tid_t child_tid);
static int read(int, void*, uint32_t);
static int write(int, const void*, uint32_t);
int fibonacci(int num);
int max_of_four_int(int num1, int num2, int num3, int num4);

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
	uint32_t argv[4];

	//Check validity of address (check arguments in system call)
	check_address(f->esp, 0);

	switch(*(uint32_t*)(f->esp)) {
		case SYS_HALT:
			halt();
			break;
		
		case SYS_EXIT:
			check_address(f->esp, 1);
			get_arguments(f->esp, argv, 1);
			exit(argv[0]);
			break;

		case SYS_EXEC:
			check_address(f->esp, 1);
			get_arguments(f->esp, argv, 1);
			f->eax = exec((const char*)argv[0]);
			break;

		case SYS_WAIT:
			check_address(f->esp, 1);
			get_arguments(f->esp, argv, 1);
			f->eax = wait((tid_t)argv[0]);
			break;

		case SYS_CREATE:
			check_address(f->esp, 2);
			break;

		case SYS_REMOVE:
			check_address(f->esp, 1);
			break;

		case SYS_OPEN:
			check_address(f->esp, 1);
			break;

		case SYS_FILESIZE:
			check_address(f->esp, 1);
			break;

		case SYS_READ:
			check_address(f->esp, 3);
			get_arguments(f->esp, argv, 3);
			f->eax = read(argv[0], (void*)argv[1], argv[2]);
			break;

		case SYS_WRITE:
			check_address(f->esp, 3);
			get_arguments(f->esp, argv, 3);
			f->eax = write(argv[0], (const void*)argv[1], argv[2]);
			break;

		case SYS_SEEK:
			check_address(f->esp, 2);
			break;

		case SYS_TELL:
			check_address(f->esp, 1);
			break;

		case SYS_CLOSE:
			check_address(f->esp, 1);
			break;

		case SYS_FIBONACCI:
			check_address(f->esp, 1);
			get_arguments(f->esp, argv, 1);
			f->eax = fibonacci(argv[0]);
			break;

		case SYS_MAX_OF_FOUR_INT:
			check_address(f->esp, 4);
			get_arguments(f->esp, argv, 4);
			f->eax = max_of_four_int(argv[0], argv[1], argv[2], argv[3]);
			break;
	}
}

void check_address(void* addr, int count) {
	for(int i=0; i<=count; i++) {
		if((addr+4*i)==NULL || !is_user_vaddr(addr+4*i) || (uint32_t)(addr+4*i)<0x08048000 || pagedir_get_page(thread_current()->pagedir, (addr+4*i))==NULL) {
			exit(-1);
			break;
		}
	}
}

void get_arguments(void *esp, uint32_t *argv, int count) {
	for(int i=1; i<=count; i++) 
		argv[i-1] = *(uint32_t*)(esp+4*i);
}

void halt(void) {
	shutdown_power_off();
}

void exit(int status) {
	printf("%s: exit(%d)\n", thread_name(), status);
	thread_current()->exit_status = status;
	thread_exit();
}

tid_t exec(const char* filename) {
	tid_t child_tid =  process_execute(filename);
	if(child_tid==TID_ERROR)
		return TID_ERROR;

	struct thread* t = get_child_thread(child_tid);
	if(t==NULL)
		return TID_ERROR;

	sema_down(&(t->load_lock));
	if(t->load_succeded==false)
		return TID_ERROR;

	return child_tid;
}

int wait(tid_t child_tid) {
	return process_wait(child_tid);
}

int read(int fd, void *buffer, uint32_t size) {
	uint8_t *read_buffer = (uint8_t*) buffer;

	if(fd==0) {
		int i;
		for(i=0; i<(int)size; i++)
			read_buffer[i] = input_getc();
		return i;
	}

	return -1;
}

int write(int fd, const void *buffer, uint32_t size) {
	if(fd==1) {
		putbuf(buffer, size);
		return size;
	}
	return -1;
}

int fibonacci(int num) {
	int n1=0;
	int n2=1;

	for(int i=0; i<num; i++) {
		n2 = n2 + n1;
		n1 = n2 - n1;
	}

	return n1;
}

int max_of_four_int(int num1, int num2, int num3, int num4) {
	int max = num1;
	
	if(num2>max)
		max=num2;
	if(num3>max)
		max=num3;
	if(num4>max)
		max=num4;

	return max;
}
