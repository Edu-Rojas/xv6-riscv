#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "vm.h"
#include "syscall.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  kexit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return kfork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return kwait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int t;
  int n;

  argint(0, &n);
  argint(1, &t);
  addr = myproc()->sz;

  if(t == SBRK_EAGER || n < 0) {
    if(growproc(n) < 0) {
      return -1;
    }
  } else {
    // Lazily allocate memory for this process: increase its memory
    // size but don't allocate memory. If the processes uses the
    // memory, vmfault() will allocate it.
    if(addr + n < addr)
      return -1;
    myproc()->sz += n;
  }
  return addr;
}

uint64
sys_pause(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kkill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
<<<<<<< Updated upstream
=======

uint64
sys_settickets(void)
{
  int n;
  struct proc *p = myproc();
  argint(0, &n);
  if (n < 1) {
    n = 1; // check for valid number of tickets and set minimum value to 1 as work says
  }
  acquire(&p->lock);
  p->tickets = n; // set the number of tickets for the process made in a lock to avoid race conditions
  release(&p->lock);

  return 0;

}

//It's necessary to declare the function prototype for change_read_protection, also declare extern 
//as it's defined in another file (vm.c)

extern int change_read_protection(uint64 addr, int len, int enable_read);

uint64
sys_mrdprotect(void)
{
  uint64 addr;
  int len;

  if(argaddr(0, &addr) < 0 || argint(1, &len) < 0)
    return -1;

  return change_read_protection(addr, len, 0); //disable read protection
}
>>>>>>> Stashed changes
