#ifndef KERNEL_DEVICE_TIMER_H_
#define KERNEL_DEVICE_TIMER_H_

#include <kernel/device/dev.h>

#define DEV_timer_CONFIG_FIELDS \
    DEV_dev_CONFIG_FIELDS

#define DEV_timer_API_FIELDS                                    \
    DEV_dev_API_FIELDS                                          \
    void (*start)(struct dev_timer* ctx);                       \
    void (*stop)(struct dev_timer* ctx);                        \
    void (*update_interval)(struct dev_timer* ctx);

#define DEV_timer_DATA_FIELDS   \
    DEV_dev_DATA_FIELDS         \
    uint16_t interval;          \
    uint16_t total_time;        \
    volatile uint8_t is_running;\

DEV_DEFINE_NEW(timer)

#ifdef TIMER_PROTECTED_INTERFACE

int32_t protected_timer_init(struct dev_timer* ctx);

void protected_timer_tick(struct dev_timer* ctx);

#endif

void timer_start(struct dev_timer* ctx);

void timer_stop(struct dev_timer* ctx);

void timer_set_interval(struct dev_timer* ctx, uint16_t count);

uint16_t timer_get_interval(struct dev_timer* ctx);

uint16_t timer_get_total_time(struct dev_timer* ctx);

#endif