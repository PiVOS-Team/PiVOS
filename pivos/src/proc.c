#include <kernel/proc.h>

#include <kernel/buffer/stack.h>
#include <kernel/utils.h>

#define KPROC_ALIGN_SIZE ((sizeof(struct kproc) + 0xf) & ~0xf)
#define KPROC_MAX_COUNT 16

static uint8_t __attribute__((aligned(16))) kproc_buffer[KPROC_MAX_COUNT * KPROC_ALIGN_SIZE];
static struct kbuffer_stack kproc_stack =
    KBUFFER_STACK_INIT(KPROC_MAX_COUNT, kproc_buffer, KPROC_ALIGN_SIZE);

// add error statuses

int32_t kproc_start(uint8_t *text, uint32_t size, int64_t *rs) {
    struct kproc *prev_proc = (struct kproc *)kbuffer_stack_peek(&kproc_stack);
    __asm__ volatile("mrs %[stack], sp_el0"
        : [stack] "=r"(prev_proc->suspend.sp)
        :
        :);
    __asm__ volatile("mrs %[elr], elr_el1"
        : [elr] "=r"(prev_proc->suspend.elr)
        :
        :);
    bytecpy((uint8_t *)prev_proc->suspend.general_regs, (uint8_t *)rs,
        sizeof(prev_proc->suspend.general_regs));

    struct kproc *proc = (struct kproc *)kbuffer_stack_alloc(&kproc_stack);
    bytecpy(proc->text, text, size);
    __asm__ volatile("msr sp_el0, %[stack]"
        :
        : [stack] "r"(proc->stack + KPROC_STACK_SIZE - 0x10)
        :);
    __asm__ volatile("msr elr_el1, %[text]" : : [text] "r"(proc->text) :);
    return 0;
}

int32_t kproc_exec(uint8_t *text, uint32_t size) {
    struct kproc *proc = (struct kproc *)kbuffer_stack_alloc(&kproc_stack);
    bytecpy(proc->text, text, size);

    __asm__ volatile("msr sp_el0, %[stack]"
        :
        : [stack] "r"(proc->stack + KPROC_STACK_SIZE - 0x10)
        :);
    __asm__ volatile("msr elr_el1, %[text]" : : [text] "r"(proc->text) :);
    __asm__ volatile("eret");
}

int32_t kproc_exit(int64_t *rs) {
    // handle exiting the first process
    kbuffer_stack_free(&kproc_stack);

    struct kproc *proc = (struct kproc *)kbuffer_stack_peek(&kproc_stack);
    __asm__ volatile("msr sp_el0, %[stack]"
        :
        : [stack] "r"(proc->suspend.sp)
        :);
    __asm__ volatile("msr elr_el1, %[text]"
        :
        : [text] "r"(proc->suspend.elr)
        :);
    bytecpy((uint8_t *)rs, (uint8_t *)proc->suspend.general_regs,
        sizeof(proc->suspend.general_regs));
    return 0;
}