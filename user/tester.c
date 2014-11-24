// Do not modify this file. It will be replaced by the grading scripts
// when checking your project.

#include "types.h"
#include "user.h"
#include "fcntl.h"
#include "x86.h"

#undef NULL
#define NULL ((void*)0)

#define PGSIZE (4096)

int ppid0;
volatile int global0 = 1;

int ppid1;
volatile int arg = 55;
volatile int global1 = 1;

int ppid2;
volatile uint newfd = 0;
int global2 = 0;

#define assert0(x) if (x) {} else { \
   printf(1, "%s: %d ", __FILE__, __LINE__); \
   printf(1, "assert failed (%s)\n", # x); \
   printf(1, "TEST FAILED\n"); \
   kill(ppid0); \
   exit(); \
}

#define assert1(x) if (x) {} else { \
   printf(1, "%s: %d ", __FILE__, __LINE__); \
   printf(1, "assert failed (%s)\n", # x); \
   printf(1, "TEST FAILED\n"); \
   kill(ppid1); \
   exit(); \
}

#define assert2(x) if (x) {} else { \
   printf(1, "%s: %d ", __FILE__, __LINE__); \
   printf(1, "assert failed (%s)\n", # x); \
   printf(1, "TEST FAILED\n"); \
   kill(ppid2); \
   exit(); \
}

void worker0(void *arg_ptr);
void worker1(void *arg_ptr);
void worker2(void *arg_ptr);

int
main(int argc, char *argv[])
{
  
 //first test
  	printf(1, "Clone and verify that address space is shared\n");
	ppid0 = getpid();
	void *stack0 = malloc(PGSIZE*2);
	assert0(stack0 != NULL);
	if((uint)stack0 % PGSIZE)
	 stack0 = stack0 + (4096 - (uint)stack0 % PGSIZE);

	int clone_pid0 = clone(stack0);
	if (clone_pid0 == 0) {
	 worker0(0);
	}
	assert0(clone_pid0 > 0);
	while(global0 != 5);
	printf(1, "TEST PASSED\n");

//second test
	printf(1, "Clone and play with the argument\n");
	ppid1 = getpid();
   void *stack1 = malloc(PGSIZE*2);
   assert1(stack1 != NULL);
   if((uint)stack1 % PGSIZE)
     stack1 = stack1 + (4096 - (uint)stack1 % PGSIZE);

   int clone_pid1 = clone(stack1);
   if (clone_pid1 == 0) {
     worker1((void*)&arg);
   }
   assert1(clone_pid1 > 0);
   while(global1 != 55);
   assert1(arg == 1);
   printf(1, "TEST PASSED\n");

//Final Test
   printf(1,"Clone copies file descriptors, but doesn't share\n");
   ppid2 = getpid();
   void *stack2 = malloc(PGSIZE*2);
   assert2(stack2 != NULL);
   if((uint)stack2 % PGSIZE)
     stack2 = stack2 + (4096 - (uint)stack2 % PGSIZE);

   int fd = open("tmp", O_WRONLY|O_CREATE);
   assert2(fd == 3);
   int clone_pid2 = clone(stack2);
   if (clone_pid2 == 0) {
     worker2(0);	
   }
   assert2(clone_pid2 > 0);
   while (global2 != 50);
   printf(1, "TEST PASSED\n");
  exit();
}


void
worker0(void *arg_ptr) {
   assert0(global0 == 1);
   global0 = 5;
   exit();
}

void
worker1(void *arg_ptr) {
   int tmp = *(int*)arg_ptr;
   *(int*)arg_ptr = 1;
   assert1(global1 == 1);
   global1 = tmp;
   exit();
}

void
worker2(void *arg_ptr) {
   assert2(write(3, "hello\n", 6) == 6);
   global2 = 50;
   exit();
}