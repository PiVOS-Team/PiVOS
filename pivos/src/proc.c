#include <kernel/proc.h>

int32_t kproc_exec(struct kproc *proc) {
    __asm__ volatile("msr sp_el0, %[stack]" : : [stack] "r"(proc->stack) :);
    __asm__ volatile("msr elr_el1, %[text]" : : [text] "r"(proc->text) :);
    __asm__ volatile("eret");
}

int32_t kproc_exit() {
}