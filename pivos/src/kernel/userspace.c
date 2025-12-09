#include <kernel/arch/cpu.h>
#include <kernel/buffer/ring.h>
#include <kernel/config.h>
#include <kernel/device.h>
#include <kernel/io_data.h>
#include <kernel/irq.h>
#include <kernel/loader.h>
#include <kernel/mem.h>
#include <kernel/proc.h>
#include <kernel/sched.h>
#include <kernel/syscall.h>
#include <kernel/userspace.h>
#include <kernel/utils.h>

#define MAX_PROC 128

static struct dev_uart* s_console;
static struct dev_timer* s_sys_timer;
static struct kbuffer_ring* s_out_buffer;
static struct proc s_proc[MAX_PROC];
static uint8_t s_need_reschedule;
static uint32_t ticks;

static struct proc* get_new_proc() {
    struct proc* curr_proc = NULL;

    for (uint16_t i = 0; i < MAX_PROC; i++) {
        curr_proc = &s_proc[i];
        if (curr_proc->state == PROC_STATE_FREE) {
            return curr_proc;
        }
    }

    return NULL;
}

static void userspace_irq_post_handler() {
    if (s_need_reschedule) {
        s_need_reschedule = 0;
        sched_reschedule();
    }
}

static void userspace_timer_tick_event_handler(struct dev* ctx) {
    struct dev_timer* timer_ctx = (struct dev_timer*)ctx;
    if (timer_ctx != s_sys_timer) {
        return;
    }

    s_need_reschedule = sched_tick();
    ticks++;
}

static void syscall_write(int64_t* args) {
    uint8_t* ptr = (uint8_t*)args[0];
    uint16_t len = args[1];

    if (kbuffer_ring_available(s_out_buffer) < len) {
        return;
    }

    for (uint64_t i = 0; i < len; i++) {
        kbuffer_ring_write(s_out_buffer, ptr[i]);
    }

    if (!uart_get_tx_state(s_console)) {
        uart_toggle_tx(s_console, 1);
    }

    args[0] = 1;
}

static void syscall_yield(int64_t* args) {
    sched_reschedule();
}

static void userspace_register_syscalls() {
    syscall_register_handler(SYSCALL_WRITE, syscall_write);
    syscall_register_handler(SYSCALL_YIELD, syscall_yield);
}

int32_t userspace_init(struct dev_uart* console, struct dev_timer* sys_timer) {
    if (!syscall_init()) {
        return 0;
    }

    userspace_register_syscalls();

    s_console = console;
    s_sys_timer = sys_timer;
    s_out_buffer = io_data_get_text_output_buffer();

    dev_event_register_handler(DEV_EVENT_TIMER_TICK, userspace_timer_tick_event_handler);
    irq_register_post_handler(userspace_irq_post_handler);
    timer_set_interval(s_sys_timer, 1);

    ticks = 0;
    return 1;
}

void userspace_start() {
    void* temp = NULL;

    // Create first process

    struct proc* p1 = get_new_proc();
    proc_create_new(p1, 1);
    loader_prepare_proc(p1, 0);

    // Create second process

    struct proc* p2 = get_new_proc();
    proc_create_new(p2, 2);
    loader_prepare_proc(p2, 1);
    sched_add_ready(p2);

    // Create third process

    struct proc* p3 = get_new_proc();
    proc_create_new(p3, 3);
    loader_prepare_proc(p3, 2);
    sched_add_ready(p3);

    // Start timer and jump to PID 1
    mem_set_usr_ctx(&p1->mem_ctx, 1);

    p1->state = PROC_STATE_RUNNING;
    sched_init(10, p1);
    timer_start(s_sys_timer);

    arch_cpu_switch_context(&temp, &p1->k_sp);

    while (1) {
        arch_cpu_halt();
    }
}
