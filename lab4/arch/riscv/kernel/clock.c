#include"printk.h"
// QEMU中时钟的频率是10MHz, 也就是1秒钟相当于10000000个时钟周期。
unsigned long TIMECLOCK = 5000000;
// unsigned long TIMECLOCK = 10000000;

unsigned long get_cycles()
{
    // 使⽤ rdtime 编写内联汇编，获取 time 寄存器中 (也就是mtime 寄存器 )的值并返回
    unsigned long res; 
    __asm__ volatile(
		"rdtime %[res]\n"
		: [res] "=r"(res)				// output
		:                               // input 
        :                               // modification
	);
	return res; 
}

void clock_set_next_event()
{
    // 下⼀次 时钟中断 的时间点
    unsigned long next = get_cycles() + TIMECLOCK;
    // 使⽤ sbi_ecall 来完成对下⼀次时钟中断的设置
	// printk("%x\n", next); 

    __asm__ volatile(
		"addi a0, %[next], 0\n"
		"addi a1, zero, 0\n"
		"addi a2, zero, 0\n"
		"addi a3, zero, 0\n"
		"addi a4, zero, 0\n"
		"addi a5, zero, 0\n"
		"addi a6, zero, 0\n"
		"addi a7, zero, 0\n"
		"ecall\n"
		: 			// output
		: [next] "r" (next)	// input
        : "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7"
	);
}