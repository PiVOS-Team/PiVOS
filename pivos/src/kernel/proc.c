#include <kernel/proc.h>
#include "kernel/arch/cpu.h"
#include "kernel/mem.h"

void proc_create_new(struct proc* ctx, uint16_t pid) {
    ctx->PID = pid;
    ctx->k_sp = ctx->kernel_stack + PROC_KERNEL_STACK_SIZE;
    ctx->state = PROC_STATE_NEW;
    mem_create_ctx(&ctx->mem_ctx);
}

void proc_destroy(struct proc* ctx) {
    mem_destroy_ctx(&ctx->mem_ctx);
    ctx->state = PROC_STATE_FREE;
}

void proc_init_ctx(struct proc* ctx, void* sp, void* pc) {
    arch_cpu_prepare_context(&ctx->k_sp, sp, pc);
}

void proc_switch(struct proc* curr, struct proc* next) {
    mem_set_usr_ctx(&next->mem_ctx, next->PID);
    arch_cpu_switch_context(&curr->k_sp, &next->k_sp);
}
