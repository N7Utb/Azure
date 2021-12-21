#include "printk.h"
#include "sbi.h"
#include "proc.h"
extern void test();

int start_kernel() {
    // puti(2021);
    // puts(" Hello RISC-V 3190103058\n");
    // sbi_ecall(0x30,0,0,0,0,0,0,1);
    printk("%d",2021);
    printk(" Hello RISC-V 3190103058 3190102214\n");
    schedule();
    test(); // DO NOT DELETE !!!

	return 0;
}
