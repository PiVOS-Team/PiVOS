#include <kernel/panic.h>
#include <kernel/irq.h>
#include <kernel/syscall.h>

#include <arch/arm64/int.h>

#include <stddef.h>

static void int_not_implemented() {
    kernel_panic("Not implemented type of interrupt");
}

static inline void int_call_kernel_irq_handler(int64_t* args) {
    irq_dispatch();
}

static inline void int_call_kernel_syscall_handler(int64_t* args) {
    uint64_t esr_el1 = 0;
    __asm__ volatile("mrs %[esr_el1], esr_el1" : [esr_el1] "=r"(esr_el1));

    syscall_dispatch(esr_el1, args);
}

void int_cur_el_sp0_sync_isr(int64_t* args) {
    int_not_implemented();
}

void int_cur_el_sp0_irq_isr(int64_t* args) {
    int_not_implemented();
}

void int_cur_el_sp0_fiq_isr(int64_t* args) {
    int_not_implemented();
}

void int_cur_el_sp0_serror_isr(int64_t* args) {
    int_not_implemented();
}

void int_cur_el_spn_sync_isr(int64_t* args) {
    int_not_implemented();
}

void int_cur_el_spn_irq_isr(int64_t* args) {
    int_call_kernel_irq_handler(args);
}

void int_cur_el_spn_fiq_isr(int64_t* args) {
    int_not_implemented();
}

void int_cur_el_spn_serror_isr(int64_t* args) {
    int_not_implemented();
}

void int_low_el_aarch64_sync_isr(int64_t* args) {
    int_call_kernel_syscall_handler(args);
}

void int_low_el_aarch64_irq_isr(int64_t* args) {
    int_call_kernel_irq_handler(args);
}

void int_low_el_aarch64_fiq_isr(int64_t* args) {
    int_not_implemented();
}

void int_low_el_aarch64_serror_isr(int64_t* args) {
    int_not_implemented();
}

void int_low_el_aarch32_sync_isr(int64_t* args) {
    int_not_implemented();
}

void int_low_el_aarch32_irq_isr(int64_t* args) {
    int_not_implemented();
}

void int_low_el_aarch32_fiq_isr(int64_t* args) {
    int_not_implemented();
}

void int_low_el_aarch32_serror_isr(int64_t* args) {
    int_not_implemented();
}