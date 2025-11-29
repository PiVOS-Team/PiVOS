.GLOBAL arch_cpu_prepare_context
.GLOBAL arch_cpu_switch_context

arch_instant_return:
    ldp x0, x1,     [sp, #(16 * 0)]
    add sp, sp,     #(2 * 8)

    msr sp_el0, x0
    msr elr_el1, x1
    mov x0, 0
    msr spsr_el1, x0

    mov x0, 0
    mov x1, 0
    mov x2, 0
    mov x3, 0
    mov x4, 0
    mov x5, 0
    mov x6, 0
    mov x7, 0
    mov x8, 0
    mov x9, 0
    mov x10, 0
    mov x11, 0
    mov x12, 0
    mov x13, 0
    mov x14, 0
    mov x15, 0
    mov x16, 0
    mov x17, 0
    mov x18, 0

    eret

arch_cpu_prepare_context:
    mov x3, 0
    mov x4, 0
    ldr x5, [x0]

    sub x5, x5,     #(14 * 8)
    stp x3, x4,     [x5, #(16 * 0)]
    stp x3, x4,     [x5, #(16 * 1)]
    stp x3, x4,     [x5, #(16 * 2)]
    stp x3, x4,     [x5, #(16 * 3)]
    stp x3, x4,     [x5, #(16 * 4)]

    // later this addr will be read as x30
    adr x4, arch_instant_return
    stp x3, x4,     [x5, #(16 * 5)]

    // Save EL0 PC and SP
    stp x1, x2,     [x5, #(16 * 6)]
    str x5, [x0]

    ret


arch_cpu_switch_context:
    sub sp, sp,     #(12 * 8)
    stp x19, x20,   [sp, #(16 * 0)]
    stp x21, x22,   [sp, #(16 * 1)]
    stp x23, x24,   [sp, #(16 * 2)]
    stp x25, x26,   [sp, #(16 * 3)]
    stp x27, x28,   [sp, #(16 * 4)]
    stp x29, x30,   [sp, #(16 * 5)]

    mov x2, sp
    str x2, [x0]

    ldr x2, [x1]
    mov sp, x2

    ldp x19, x20,   [sp, #(16 * 0)]
    ldp x21, x22,   [sp, #(16 * 1)]
    ldp x23, x24,   [sp, #(16 * 2)]
    ldp x25, x26,   [sp, #(16 * 3)]
    ldp x27, x28,   [sp, #(16 * 4)]
    ldp x29, x30,   [sp, #(16 * 5)]
    add sp, sp,     #(12 * 8)

    ret
