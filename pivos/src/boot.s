.SECTION .boot
.GLOBAL start

start:

check_core:
    mrs x0, mpidr_el1
    and x0, x0, #3
    cmp x0, #0
    bne halt
    
zero_bss:
    ldr x0, =bss_start
    ldr x1, =bss_end

zero_bss_loop:
    cmp x0, x1
    b.ge zero_bss_done

    str wzr, [X0], #4
    b zero_bss_loop

zero_bss_done:
    nop

ensure_el2:
    mrs x0, CurrentEL
    and x0, x0, 0xc
    cmp x0, 0x8
    bne halt 

transition_to_el1:
    mrs x0, hcr_el2
    // RW (31) to 1
    mov x0, xzr
    orr x0, x0, 0x80000000
    msr hcr_el2, x0

    mrs x0, spsr_el2
    mov x0, xzr
    // M (3:0) to 0101
    ldr x1, =0x5
    orr x0, x0, x1
    // F, I (6, 7) to 0
    and x0, x0, 0xffffffffffffff3f
    // A, D (8, 9) to 1
    orr x0, x0, 0x300
    msr spsr_el2, x0

    // set Return address to enter_kernel
    ldr x0, =enter_kernel
    msr elr_el2, x0
    eret

    // prevent kernel entry in EL2
    b halt

enter_kernel:
    mov sp, #0x80000
    bl kernel_main

halt:
    wfe
    b halt
