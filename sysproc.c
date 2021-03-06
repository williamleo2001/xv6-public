#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "pstat.h"

int
sys_fork(void) {
    return fork();
}

int
sys_exit(void) {
    exit();
    return 0;  // not reached
}

int
sys_wait(void) {
    return wait();
}

int
sys_kill(void) {
    int pid;

    if (argint(0, &pid) < 0)
        return -1;
    return kill(pid);
}

int
sys_getpid(void) {
    return myproc()->pid;
}

int
sys_sbrk(void) {
    int addr;
    int n;

    if (argint(0, &n) < 0)
        return -1;
    addr = myproc()->sz;
    if (growproc(n) < 0)
        return -1;
    return addr;
}

int
sys_sleep(void) {
    int n;
    uint ticks0;

    if (argint(0, &n) < 0)
        return -1;
    acquire(&tickslock);
    ticks0 = ticks;
    while (ticks - ticks0 < n) {
        if (myproc()->killed) {
            release(&tickslock);
            return -1;
        }
        sleep(&ticks, &tickslock);
    }
    release(&tickslock);
    return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void) {
    uint xticks;

    acquire(&tickslock);
    xticks = ticks;
    release(&tickslock);
    return xticks;
}

// return the year of which the Unix version 6 was released
int
sys_getyear(void) {
    return 1975;
}

struct rtcdate* sys_gettime(struct rtcdate *r) {
    cmostime(r);
    cprintf("%d-%d-%d %d:%d:%d\n", r->year, r->month, r->day, r->hour, r->minute, r->second);
    return r;
}

// Reference:
// https://github.com/wkatsak/xv6
// https://github.com/t3rm1n4l/pintos/blob/master/devices/shutdown.c
// FIXME
int
sys_halt(void) {
    char *p = "Shutdown";

    for (; *p; p++) {
        outw(0xB004, 0x2000);
        outb(0xF, *p);
//      outb(0x8900, *p);
    }
    outb(0x70, 0xF);  // offset 0xF is shutdown code
    outb(0x71+1, 0x0A);
    return 0;
}

void sys_getproc(void) {
    struct rtcdate r;
    cmostime(&r);

    cprintf("==%d-%d-%d %d:%d:%d  pid:%d priority: %d\n", r.year, r.month, r.day, r.hour, r.minute, r.second,
            myproc()->pid, myproc()->priority);
    consoleproc();
    for(int i=0; i< 10000;i++){
     cmostime(&r);
    }
    cmostime(&r);
    cprintf("==%d-%d-%d %d:%d:%d  pid:%d priority: %d\n", r.year, r.month, r.day, r.hour, r.minute, r.second,
            myproc()->pid, myproc()->priority);
    return;
}

void sys_prio1(void) {

    struct rtcdate r;
    cmostime(&r);

    cprintf("======Process %s Start %d-%d-%d %d:%d:%d priority: %d\n", myproc()->name , r.year, r.month, r.day, r.hour,
            r.minute, r.second, myproc()->priority);

    consoleproc();

    cprintf("======Process %s End %d-%d-%d %d:%d:%d priority: %d\n", myproc()->name , r.year, r.month, r.day, r.hour,
            r.minute, r.second, myproc()->priority);
    return;
}

void sys_sleepprio(void) {

    struct rtcdate r;
    cmostime(&r);

    cprintf("======Process %s Start %d-%d-%d %d:%d:%d priority: %d\n", myproc()->name , r.year, r.month, r.day, r.hour,
            r.minute, r.second, myproc()->priority);
    for(int i=0; i< 80000;i++){
        cmostime(&r);
    }
    consoleproc();
    cprintf("======Process %s End %d-%d-%d %d:%d:%d priority: %d\n", myproc()->name , r.year, r.month, r.day, r.hour,
            r.minute, r.second, myproc()->priority);
    return;
}



