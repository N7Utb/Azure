#include "clock.h"
#include "printk.h"
#include "proc.h"
#include "syscall.h"
#define ECALL_FROM_U_MODE 0x8
#define ECALL_FROM_S_MODE 0x9

static int i=0; 

struct pt_regs{
    uint64 reg[32];
    uint64 sepc; 
    uint64 sstatus;  
}; 

void trap_handler(unsigned long long scause, unsigned long long sepc, struct pt_regs*regs) { 
    // 通过 `scause` 判断trap类型 
    // 如果是interrupt 判断是否是timer interrupt 
    // 如果是timer interrupt 则打印输出相关信息, 并通过 `clock_set_next_event()` 设置下 ⼀次时钟中断 
    // `clock_set_next_event()` ⻅ 4.5 节 
    // 其他interrupt / exception 可以直接忽略 # YOUR CODE HERE  
    if (scause == 0x8000000000000005){
        // printk("[%d] Supervisor Mode Timer Interrupt ", i=(i+1)%3600); 
        // printk("Next time: "); 
        clock_set_next_event(); 
        do_timer();
        
    }
    else if(scause == ECALL_FROM_U_MODE){
        switch (regs->reg[17])
        {
        case SYS_GETPID:
            regs->reg[10] = getpid();
            break;
        case SYS_WRITE:
            regs->reg[10] = write(regs->reg[10],(char *)regs->reg[11],regs->reg[12]);
            break;
        default:
            break;
        }
        regs->sepc = regs->sepc + 4;
    }
    else {
        // printk("123\n\n\n\n\n"); 
    }
    return ;
}
