#ifndef KERNEL_DEVICE_DEV_EVENT_H_
#define KERNEL_DEVICE_DEV_EVENT_H_

#include <kernel/device/dev.h>

enum dev_event {
    DEV_EVENT_NEW_DEVICE_FOUND,
    DEV_EVENT_ENSURE_DEVICE_INITIALIZED,
    DEV_EVENT_TIMER_TICK,
    DEV_EVENT_DATA_RECEIVED,

    DEV_EVENT__N
};

typedef void (*dev_event_handler)(struct dev* ctx);

void dev_event_init();

void dev_event_post(struct dev* ctx, enum dev_event event);

void dev_event_register_handler(enum dev_event event, dev_event_handler callback);

#endif
