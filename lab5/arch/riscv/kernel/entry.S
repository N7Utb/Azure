.extern trap_handler
.extern dummy
.section .text.entry
.align 2
.globl _traps
_traps:
    # obviously, the context should be saved on the kernel stack instead of the user stack
    # judge whether sscratch == 0
    # the entry is kernel thread, not swap the sscratch and sp
    csrrw sp, sscratch, sp
    bne sp, zero, __is_kernel
    csrrw sp, sscratch, sp
    # -----------
    # 1. save 32 registers and sepc to stack
__is_kernel:    
    addi sp, sp, -272
    sd x0,0(sp)
    sd x1,8(sp)
    sd x2,16(sp)
    sd x3,24(sp)
    sd x4,32(sp)
    sd x5,40(sp)
    sd x6,48(sp)
    sd x7,56(sp)
    sd x8,64(sp)
    sd x9,72(sp)
    sd x10,80(sp)
    sd x11,88(sp)
    sd x12,96(sp)
    sd x13,104(sp)
    sd x14,112(sp)
    sd x15,120(sp)
    sd x16,128(sp)
    sd x17,136(sp)
    sd x18,144(sp)
    sd x19,152(sp)
    sd x20,160(sp)
    sd x21,168(sp)
    sd x22,176(sp)
    sd x23,184(sp)
    sd x24,192(sp)
    sd x25,200(sp)
    sd x26, 208(sp)
    sd x27, 216(sp)
    sd x28, 224(sp)
    sd x29, 232(sp)
    sd x30, 240(sp)
    sd x31, 248(sp)
    csrr a1, sepc
    sd a1, 256(sp)
    csrr a0, scause
    sd a0, 264(sp)
    addi a2, sp, 0
    call trap_handler  # trap_handler

    # -----------
    # 3. restore sepc and 32 registers (x2(sp) should be restore last) from stack
    ld t0, 256(sp)
    csrw sepc, t0
    ld t0, 264(sp)
    csrw scause, t0
    ld x1,8(sp)
    ld x3,24(sp)
    ld x4,32(sp)
    ld x5,40(sp)
    ld x6,48(sp)
    ld x7,56(sp)
    ld x8,64(sp)
    ld x9,72(sp)
    ld x10,80(sp)
    ld x11,88(sp)
    ld x12,96(sp)
    ld x13,104(sp)
    ld x14,112(sp)
    ld x15,120(sp)
    ld x16,128(sp)
    ld x17,136(sp)
    ld x18,144(sp)
    ld x19,152(sp)
    ld x20,160(sp)
    ld x21,168(sp)
    ld x22,176(sp)
    ld x23,184(sp)
    ld x24,192(sp)
    ld x25,200(sp)
    ld x26, 208(sp)
    ld x27, 216(sp)
    ld x28, 224(sp)
    ld x29, 232(sp)
    ld x30, 240(sp)
    ld x31, 248(sp)
    addi sp,sp,272
    # judge whether sscratch == 0
    # the entry is kernel thread, not swap the sscratch and sp
    csrrw sp, sscratch, sp
    bne sp, zero, __exit
    csrrw sp, sscratch, sp
__exit:
    sret
    .global __dummy
__dummy:
    # TODO: 4.3修改交换sp
    # la t0, dummy
    # csrw sepc, t0
    csrrw sp, sscratch, sp
    sret
    .global __switch_to
__switch_to:
    sd ra, 40(a0)
    sd sp, 48(a0)
    sd s0, 56(a0)
    sd s1, 64(a0)
    sd s2, 72(a0)
    sd s3, 80(a0)
    sd s4, 88(a0)
    sd s5, 96(a0)
    sd s6, 104(a0)
    sd s7, 112(a0)
    sd s8, 120(a0)
    sd s9, 128(a0)
    sd s10, 136(a0)
    sd s11, 144(a0)
    # save sepc, sscratch, satp
    csrr t0, sepc
    sd t0, 152(a0)
    csrr t0, sstatus
    sd t0, 160(a0)
    csrr t0, sscratch
    sd t0, 168(a0)
    csrr t0, satp
    slli t0,t0,12
    li t1, 0xffffffdf80000000
    add t0, t0, t1
    sd t0,176(a0)


    ld ra, 40(a1)
    ld sp, 48(a1)
    ld s0, 56(a1)
    ld s1, 64(a1)
    ld s2, 72(a1)
    ld s3, 80(a1)
    ld s4, 88(a1)
    ld s5, 96(a1)
    ld s6, 104(a1)
    ld s7, 112(a1)
    ld s8, 120(a1)
    ld s9, 128(a1)
    ld s10, 136(a1)
    ld s11, 144(a1)
    # set sepc
    ld t0, 152(a1)
    csrw sepc, t0
    # set sstatus
    ld t0, 160(a1)
    csrw sstatus, t0
    # set sscratch
    ld t0, 168(a1)
    csrw sscratch, t0
    # set satp
    ld t0, 176(a1)
    li t1, 0xffffffdf80000000
    sub t0, t0, t1
    srli t0, t0, 12
    li t1, 0x8
    slli t1, t1, 60
    add t0, t0, t1
    csrw satp, t0
    sfence.vma zero, zero

    ret