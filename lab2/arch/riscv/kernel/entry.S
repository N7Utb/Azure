.extern trap_handler
.section .text.entry
.align 2
.globl _traps
_traps:
    # YOUR CODE HERE
    # -----------
    # 1. save 32 registers and sepc to stack
    addi sp, sp, -256
    sd ra,0(sp)
    sd gp,8(sp)
    sd tp,16(sp)
    sd t0,24(sp)
    sd t1,32(sp)
    sd t2,40(sp)
    sd s0,48(sp)
    sd s1,56(sp)
    sd a0,64(sp)
    sd a1,72(sp)
    sd a2,80(sp)
    sd a3,88(sp)
    sd a4,96(sp)
    sd a5,104(sp)
    sd a6,112(sp)
    sd a7,120(sp)
    sd s2,128(sp)
    sd s3,136(sp)
    sd s4,144(sp)
    sd s5,152(sp)
    sd s6,160(sp)
    sd s7,168(sp)
    sd s8,176(sp)
    sd s9,184(sp)
    sd s10,192(sp)
    sd s11,200(sp)
    sd t3, 208(sp)
    sd t4, 216(sp)
    sd t5, 224(sp)
    sd t6, 232(sp)
    csrr t6, sepc
    sd t6, 240(sp)
    sd sp, 248(sp)
    # -----------
    # 2. call trap_handler
    csrr a0, scause
    csrr a1, sepc
    call trap_handler  # trap_handler

    # -----------
    # 3. restore sepc and 32 registers (x2(sp) should be restore last) from stack
    ld ra,0(sp)
    ld gp,8(sp)
    ld tp,16(sp)
    ld t0,24(sp)
    ld t1,32(sp)
    ld t2,40(sp)
    ld s0,48(sp)
    ld s1,56(sp)
    ld a0,64(sp)
    ld a1,72(sp)
    ld a2,80(sp)
    ld a3,88(sp)
    ld a4,96(sp)
    ld a5,104(sp)
    ld a6,112(sp)
    ld a7,120(sp)
    ld s2,128(sp)
    ld s3,136(sp)
    ld s4,144(sp)
    ld s5,152(sp)
    ld s6,160(sp)
    ld s7,168(sp)
    ld s8,176(sp)
    ld s9,184(sp)
    ld s10,192(sp)
    ld s11,200(sp)
    ld t3, 208(sp)
    ld t4, 216(sp)
    ld t5, 224(sp)
    ld t6, 240(sp)
    csrw sepc, t6
    ld t6, 232(sp)
    ld sp, 248(sp)
    addi sp,sp,256
    # -----------
    # 4. return from trap
    # -----------
    sret
