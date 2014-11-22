#include "types.h"
#include "x86.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"
#include "sysfunc.h"

int
sys_fork(void)
{
  return fork();
}

//// Added for p4b ////

//int clone(void *stack)
int sys_clone(void)
{

// Fetch the nth word-sized system call argument as a pointer
// to a block of memory of size n bytes.  Check that the pointer
// lies within the process address space.
//int argptr(int n, char **pp, int size)

  void *stack;

  if(argptr(0, (void*)&stack, 2*sizeof(stack[0])) < 0){ return -1; }

  return clone(stack);
}

//int join()
int sys_join(void)
{
  return join();
}

//int lock(int *l)
int sys_lock(void)
{
  int *l;

  if(argptr(0, (void*)&l, 2*sizeof(l[0])) < 0){ return -1; }

  return lock(l);
}

//int unlock(int *l)
int sys_unlock(void)
{
  int *l;

  if(argptr(0, (void*)&l, 2*sizeof(l[0])) < 0){ return -1; }

  return unlock(l);
}

///////////////////////


int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;
  
  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since boot.
int
sys_uptime(void)
{
  uint xticks;
  
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
