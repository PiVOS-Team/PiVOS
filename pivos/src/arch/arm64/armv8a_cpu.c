#include <arch/arm64/int.h>
#include <kernel/arch/cpu.h>

extern void vector_table();

static uint64_t last_state;

uint8_t arch_cpu_get_id() {
    return 0;
}

void arch_cpu_init_int() {
    __asm__ volatile("msr vbar_el1, %[tab]" : : [tab] "r"(vector_table) :);
}

void arch_cpu_enable_int() {
    __asm__ volatile("msr daif, %[state]" : : [state] "r"(last_state));
}

void arch_cpu_disable_int() {
    __asm__ volatile(
        "mrs %[state], DAIF;"
        "msr daifset, #2;"
        : [state] "=r"(last_state)
        :);
}

void arch_cpu_halt() {
    __asm__ volatile("wfe");
}
