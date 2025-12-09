#ifndef DRIVERS_TIMER_GENERIC_TIMER_IMPL_H_
#define DRIVERS_TIMER_GENERIC_TIMER_IMPL_H_

#include <kernel/device/timer.h>

#define DEV_generic_timer_CONFIG_FIELDS \
    DEV_timer_CONFIG_FIELDS             \
        uint16_t irq_number;

#define DEV_generic_timer_API_FIELDS \
    DEV_timer_API_FIELDS

#define DEV_generic_timer_DATA_FIELDS \
    DEV_timer_DATA_FIELDS

DEV_DEFINE_NEW(generic_timer)

#endif