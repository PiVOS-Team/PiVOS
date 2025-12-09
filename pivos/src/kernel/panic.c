#include <kernel/arch/cpu.h>
#include <kernel/panic.h>

void kernel_panic(const char* reason) {
    while (1) {
        arch_cpu_halt();
    }
}