#include <kernel/buffer/stack.h>
#include <kernel/log.h>
#include <kernel/memory.h>
#include <kernel/proc.h>
#include <kernel/utils.h>

static struct kproc kproc_buffer[KPROC_MAX_COUNT];
static struct kbuffer_stack kproc_stack = KBUFFER_STACK_INIT(
    KPROC_MAX_COUNT, (uint8_t *)kproc_buffer, sizeof(struct kproc));

static struct memory_region kproc_region = {
    .memory_type = MEMORY_TYPE_NORMAL_NO_CACHE,
    .va = 0x0,
    .size = ALIGN_UP(sizeof(struct kproc), 2 * MB_IN_B),
};

#include <kernel/example.h>
static struct kproc_prog_entry kproc_prog_tab[] = {
    {.text = a_img, .size = a_img_len},
    {.text = b_img, .size = b_img_len},
    {.text = c_img, .size = c_img_len}};

uint64_t kproc_count() {
    return kbuffer_stack_size(&kproc_stack);
}

struct kproc *kproc_alloc() {
    struct kproc *kproc = (struct kproc *)kbuffer_stack_alloc(&kproc_stack);
    if (kproc == 0) {
        return 0;
    }

    memory_create_context(&kproc->ctx);
    memory_map_region(&kproc->ctx, &kproc_region, MEMORY_MAP_ALLOC);

    return kproc;
}

struct kproc *kproc_free() {
    struct kproc *kproc = (struct kproc *)kbuffer_stack_peek(&kproc_stack);

    memory_destroy_context(&kproc->ctx);

    kbuffer_stack_free(&kproc_stack);

    return (struct kproc *)kbuffer_stack_peek(&kproc_stack);
}

struct kproc *kproc_get() {
    return (struct kproc *)kbuffer_stack_peek(&kproc_stack);
}

int32_t kproc_mem_switch(struct kproc *proc) {
    struct memory_context *ctx = &proc->ctx;

    const uint64_t proc_id = kbuffer_stack_size(&kproc_stack);
    const uint64_t asid_size = 16;
    const uint64_t asid_mask = proc_id << (64 - asid_size);
    const uint64_t phy_tab = (uint64_t)ctx->level0_table & ~KERNEL_SPACE_MASK;

    void *tab = (void *)(phy_tab | asid_mask);

    __asm__ volatile(
        "msr ttbr0_el1, %[tab]\n"
        "tlbi vmalle1\n"
        "dsb ish\n"
        "isb\n"
        :
        : [tab] "r"(tab));

    return KSTATUS_PROC_OK;
}

int32_t kproc_init(struct kproc_prog_entry *prog) {
    bytecpy((uint8_t *)KPROC_PROG_OFFSET, prog->text, prog->size);
    __asm__ volatile("msr sp_el0, %[stack]" : : [stack] "r"(KPROC_PROG_OFFSET));
    __asm__ volatile("msr elr_el1, %[text]" : : [text] "r"(KPROC_PROG_OFFSET));
    return KSTATUS_PROC_OK;
}

int32_t kproc_suspend(struct kproc *proc, int64_t *rs) {
    __asm__ volatile("mrs %[stack], sp_el0" : [stack] "=r"(proc->suspend.sp));
    __asm__ volatile("mrs %[elr], elr_el1" : [elr] "=r"(proc->suspend.elr));
    bytecpy((uint8_t *)proc->suspend.general_regs, (uint8_t *)rs,
            sizeof(proc->suspend.general_regs));
    return KSTATUS_PROC_OK;
}

int32_t kproc_restore(struct kproc *proc, int64_t *rs) {
    __asm__ volatile("msr sp_el0, %[stack]" : : [stack] "r"(proc->suspend.sp));
    __asm__ volatile("msr elr_el1, %[text]" : : [text] "r"(proc->suspend.elr));
    bytecpy((uint8_t *)rs, (uint8_t *)proc->suspend.general_regs,
            sizeof(proc->suspend.general_regs));
    return KSTATUS_PROC_OK;
}

int32_t kproc_start(uint32_t prog_id, int64_t *rs) {
    struct kproc_prog_entry *prog = &kproc_prog_tab[prog_id];
    struct kproc *proc = (void *)0;

    if (kbuffer_stack_full(&kproc_stack)) {
        return KSTATUS_PROC_STACK_FULL;
    }

    proc = kproc_get();
    kproc_mem_switch(proc);
    kproc_suspend(proc, rs);

    proc = kproc_alloc();
    kproc_mem_switch(proc);
    kproc_init(prog);
    return KSTATUS_PROC_OK;
}

void kproc_exec(uint32_t prog_id) {
    struct kproc_prog_entry *prog = &kproc_prog_tab[prog_id];
    struct kproc *proc = (void *)0;

    proc = kproc_alloc();
    kproc_mem_switch(proc);
    kproc_init(prog);

    __asm__ volatile("eret");
}

int32_t kproc_exit(int64_t *rs) {
    struct kproc *proc = (void *)0;

    if (kproc_count() == 1) {
        return KSTATUS_PROC_LAST;
    }

    proc = kproc_free();
    kproc_mem_switch(proc);
    kproc_restore(proc, rs);
    return 0;
}
