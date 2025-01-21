#include <kernel/int.h>

.section .text
.global vector_table


.macro ISR_PROLOGUE
    sub sp, sp,     #(32 * 8)
    stp x0, x1,     [sp]
    stp x2, x3,     [sp, #(16 * 1)]
    stp x4, x5,     [sp, #(16 * 2)]
    stp	x6, x7,     [sp, #(16 * 3)]
	stp	x8, x9,     [sp, #(16 * 4)]
	stp	x10, x11,   [sp, #(16 * 5)]
	stp	x12, x13,   [sp, #(16 * 6)]
	stp	x14, x15,   [sp, #(16 * 7)]
	stp	x16, x17,   [sp, #(16 * 8)]
	stp	x18, x19,   [sp, #(16 * 9)]
	stp	x20, x21,   [sp, #(16 * 10)]
	stp	x22, x23,   [sp, #(16 * 11)]
	stp	x24, x25,   [sp, #(16 * 12)]
	stp	x26, x27,   [sp, #(16 * 13)]
	stp	x28, x29,   [sp, #(16 * 14)]
    STR x30,        [sp, #(16 * 15)]
.endm

.macro ISR_EPILOGUE
    ldp x0, x1, 	[sp]
    ldp x2, x3, 	[sp, #(16 * 1)]
    ldp x4, x5, 	[sp, #(16 * 2)]
    ldp	x6, x7, 	[sp, #(16 * 3)]
	ldp	x8, x9, 	[sp, #(16 * 4)]
	ldp	x10, x11, 	[sp, #(16 * 5)]
	ldp	x12, x13, 	[sp, #(16 * 6)]
	ldp	x14, x15, 	[sp, #(16 * 7)]
	ldp	x16, x17, 	[sp, #(16 * 8)]
	ldp	x18, x19, 	[sp, #(16 * 9)]
	ldp	x20, x21, 	[sp, #(16 * 10)]
	ldp	x22, x23, 	[sp, #(16 * 11)]
	ldp	x24, x25, 	[sp, #(16 * 12)]
	ldp	x26, x27, 	[sp, #(16 * 13)]
	ldp	x28, x29, 	[sp, #(16 * 14)]
    ldr x30, 		[sp, #(16 * 15)]
    add sp, sp, 	#(32 * 8)
.endm

.macro ISR name
    \name\()_isr_wrp:
    ISR_PROLOGUE
    mov x0, sp
    bl int_\name\()_isr
    ISR_EPILOGUE
    eret
.endm

.balign 0x800
vector_table:

.balign 0x80
cur_el_sp0_sync:
    b cur_el_sp0_sync_isr_wrp

.balign 0x80
cur_el_sp0_irq:
    b cur_el_sp0_irq_isr_wrp

.balign 0x80
cur_el_sp0_fiq:
    b cur_el_sp0_fiq_isr_wrp

.balign 0x80
cur_el_sp0_serror:
    b cur_el_sp0_serror_isr_wrp

.balign 0x80
cur_el_spn_sync:
    b cur_el_spn_sync_isr_wrp

.balign 0x80
cur_el_spn_irq:
    b cur_el_spn_irq_isr_wrp

.balign 0x80
cur_el_spn_fiq:
    b cur_el_spn_fiq_isr_wrp

.balign 0x80
cur_el_spn_serror:
    b cur_el_spn_serror_isr_wrp

.balign 0x80
low_el_aarch64_sync:
    b low_el_aarch64_sync_isr_wrp

.balign 0x80
low_el_aarch64_irq:
    b low_el_aarch64_irq_isr_wrp

.balign 0x80
low_el_aarch64_fiq:
    b low_el_aarch64_fiq_isr_wrp

.balign 0x80
low_el_aarch64_serror:
    b low_el_aarch64_serror_isr_wrp

.balign 0x80
low_el_aarch32_sync:
    b low_el_aarch32_sync_isr_wrp

.balign 0x80
low_el_aarch32_irq:
    b low_el_aarch32_irq_isr_wrp

.balign 0x80
low_el_aarch32_fiq:
    b low_el_aarch32_fiq_isr_wrp

.balign 0x80
low_el_aarch32_serror:
    b low_el_aarch32_serror_isr_wrp

.balign 0x80

ISR cur_el_sp0_sync
ISR cur_el_sp0_irq
ISR cur_el_sp0_fiq
ISR cur_el_sp0_serror

ISR cur_el_spn_sync
ISR cur_el_spn_irq
ISR cur_el_spn_fiq
ISR cur_el_spn_serror

ISR low_el_aarch64_sync
ISR low_el_aarch64_irq
ISR low_el_aarch64_fiq
ISR low_el_aarch64_serror

ISR low_el_aarch32_sync
ISR low_el_aarch32_irq
ISR low_el_aarch32_fiq
ISR low_el_aarch32_serror
