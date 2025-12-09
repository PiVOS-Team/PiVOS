#include <kernel/sched.h>
#include <kernel/utils.h>

#include "kernel/arch/cpu.h"
#include "kernel/proc.h"

static uint16_t s_quantum;
static uint16_t s_tick_left;

static struct proc* s_current_proc;
static struct proc* s_ready_head;
static struct proc* s_ready_tail;

static struct proc* sched_pick_next() {
    if (!s_ready_head) {
        return NULL;
    }

    struct proc* proc = s_ready_head;
    s_ready_head = s_ready_head->next;

    if (!s_ready_head) {
        s_ready_tail = NULL;
    }

    return proc;
}

int32_t sched_init(uint16_t quantum, struct proc* first_proc) {
    s_quantum = quantum;
    s_tick_left = s_quantum;
    s_current_proc = first_proc;

    return 1;
}

void sched_add_ready(struct proc* proc) {
    proc->state = PROC_STATE_READY;
    proc->next = NULL;

    if (!s_ready_head) {
        s_ready_head = proc;
        s_ready_tail = proc;
    } else {
        s_ready_tail->next = proc;
        s_ready_tail = proc;
    }
}

void sched_remove_ready(struct proc* proc) {
}

void sched_reschedule() {
    struct proc* curr = s_current_proc;
    struct proc* next = NULL;

    if (curr->state == PROC_STATE_RUNNING) {
        sched_add_ready(curr);
    }

    next = sched_pick_next();

    while (!next) {
        // Wait for any ready process
        arch_cpu_halt();
        next = sched_pick_next();
    }

    // If there is only one proc in queue
    if (curr == next) {
        curr->state = PROC_STATE_RUNNING;
        s_tick_left = s_quantum;
        return;
    }

    next->state = PROC_STATE_RUNNING;
    s_current_proc = next;
    s_tick_left = s_quantum;
    proc_switch(curr, next);
}

struct proc* sched_get_current() {
    return s_current_proc;
}

uint8_t sched_tick() {
    s_tick_left--;

    return s_tick_left == 0 ? 1 : 0;
}
