#ifndef KERNEL_DEV_GENERIC_TIMER_H_
#define KERNEL_DEV_GENERIC_TIMER_H_

#include <stdint.h>

int32_t dev_generic_timer_init(uint16_t interval);
struct kdev_timer *dev_generic_timer_get();

void generic_timer_reset();

void dev_generic_timer_kdev_isr(void *ctx);

void dev_generic_timer_kdev_set_action(void *ctx, void (*action)(void));

#endif  // KERNEL_DEV_GENERIC_TIMER_H_