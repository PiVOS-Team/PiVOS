#ifndef KERNEL_SCHED_H_
#define KERNEL_SCHED_H_

#include <kernel/proc.h>

int32_t sched_init(uint16_t quantum, struct proc* first_proc);

void sched_add_ready(struct proc* proc);

void sched_remove_ready(struct proc* proc);

void sched_reschedule();

struct proc* sched_get_current();

uint8_t sched_tick();

#endif