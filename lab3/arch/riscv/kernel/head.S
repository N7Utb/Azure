.extern start_kernel

    .section .text.init
    .globl _start
_start:
    # ------------------
    # - your code here -
    # ------------------

    # set stvec
    la a0, _traps
    csrw stvec, a0

    # set sstatus[sie]
    csrsi sstatus,2

    # set the first time interrupt
    lui a1,0x0400
    rdtime a0
    add a0,a0,a1
    addi a7,zero,0
    addi a6,zero,0
    addi a1,zero,0
    addi a2,zero,0
    addi a3,zero,0
    addi a4,zero,0
    addi a5,zero,0
    ecall

    # set sie[STIE]
    addi a0,zero,32
    csrs sie,a0
    la sp, boot_stack_top
    j start_kernel
    # j _traps
    .section .bss.stack
    .globl boot_stack
boot_stack:
    .space 4096 # <-- change to your stack size

    .globl boot_stack_top
boot_stack_top: