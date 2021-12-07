#include "clock.h"
#include "printk.h"
#include "proc.h"
static int i=0; 

void trap_handler(unsigned long long scause, unsigned long long sepc) { 
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
    else {
        ; 
    }
    return ;
}
