#ifndef KERNEL_DEVICE_BUS_H_
#define KERNEL_DEVICE_BUS_H_

#include <kernel/device/dev.h>

#define DEV_bus_CONFIG_FIELDS \
    DEV_dev_CONFIG_FIELDS

#define DEV_bus_API_FIELDS \
    DEV_dev_API_FIELDS void (*search)(struct dev_bus * ctx);

#define DEV_bus_DATA_FIELDS \
    DEV_dev_DATA_FIELDS

DEV_DEFINE_NEW(bus)

#ifdef BUS_PROTECTED_INTERFACE

int32_t protected_bus_init(struct dev_bus* ctx);

#endif

void bus_search(struct dev_bus* ctx);

#endif