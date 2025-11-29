#include <kernel/panic.h>
#include <kernel/arch/cpu.h>

void kernel_panic(const char* reason) {
    while(1) {
        arch_cpu_halt();
    }
}