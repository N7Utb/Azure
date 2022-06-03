#include "print.h"
#include "sbi.h"

extern void test();

int start_kernel() {
    sbi_ecall(0x1, 0x0, 0x30, 0, 0, 0, 0, 0); 
    puts("\n");
    puti(2021);
    puts("\n");
    puts(" Hello RISC-V 3190102214\n");

    test(); // DO NOT DELETE !!!

	return 0;
}
