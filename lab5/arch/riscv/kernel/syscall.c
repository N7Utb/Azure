#include "syscall.h"
#include "sbi.h"
#include "proc.h"
extern struct task_struct *current;
uint64 write(unsigned int fd, const char* buf, size_t count)
{
    for(uint64 i=0 ; i<count ; i++)
    {
        sbi_ecall(SBI_PUTCHAR,0,buf[i],0,0,0,0,0);
    }
    return count;
}
uint64 getpid()
{
    return current->pid;
}