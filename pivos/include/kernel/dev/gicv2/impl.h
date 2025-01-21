#ifndef KERNEL_DEV_GICV2_IMPL_H_
#define KERNEL_DEV_GICV2_IMPL_H_

#include <stdint.h>

void gicv2_set_int_priority(uint32_t id, uint8_t priority);
void gicv2_set_int_target(uint32_t id, uint8_t cpu);
void gicv2_set_int_group(uint32_t id, uint8_t group);
void gicv2_set_int_trigger(uint32_t id, uint8_t trigger);

void gicv2_dispatch(int64_t *_rs);
int32_t gicv2_get_irq_id();
void gicv2_end_isr(int32_t irq_id);

#endif  // KERNEL_DEV_GICV2_IMPL_H_