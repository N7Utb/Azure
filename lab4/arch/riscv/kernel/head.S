.extern start_kernel
.extern task_init
.extern mm_init
.extern setup_vm_final
.extern early_pgtbl
# .extern PA2VA_OFFSET
    .section .text.init
    .globl _start
_start:

    addi a0,zero,32
    csrs sie,a0

    la sp, boot_stack_top

    


    call setup_vm
    call relocate
    call mm_init
    

    # set sstatus[sie]

    la a0, _traps
    csrw stvec, a0
    csrsi sstatus,2
    # set the first time interrupt
    # lui a1,0x0800
    lui a1,0x0800
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


    call setup_vm_final
    call task_init

    j start_kernel
relocate:
    # set ra = ra + PA2VA_OFFSET
    # set sp = sp + PA2VA_OFFSET (If you have set the sp before)
    # ld t0, PA2VA_OFFSET
    li t0, 0xffffffdf80000000
    add ra,ra,t0
    add sp,sp,t0

    la t1, aaa
    add t1,t1,t0
    csrw stvec,t1
    # set satp
    # mode
    li t0, 0x8
    slli t0,t0,60
    # PPN
    la t1, early_pgtbl
    srli t1,t1,12
    add t0,t0,t1
    sfence.vma zero,zero
    csrw satp,t0
    # flush tlb
aaa:
    sfence.vma zero,zero
    ret
    .section .bss.stack
    .globl boot_stack
boot_stack:
    .space 4096 # <-- change to your stack size

    .globl boot_stack_top
boot_stack_top: