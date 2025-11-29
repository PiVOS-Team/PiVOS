#ifndef KERNEL_ARCH_CPU_H_
#define KERNEL_ARCH_CPU_H_

#include <stdint.h>

uint8_t arch_cpu_get_id();

void arch_cpu_init_int();

void arch_cpu_enable_int();

void arch_cpu_disable_int();

void arch_cpu_halt();

void arch_cpu_prepare_context(void** k_sp, void* p_sp, void* p_pc);

void arch_cpu_switch_context(void** curr_sp, void** next_sp);

#endif