#ifndef KERNEL_DEVICE_DEV_REGISTRY_H_
#define KERNEL_DEVICE_DEV_REGISTRY_H_

#include <kernel/device/dev.h>

void dev_registry_init();

uint16_t dev_to_id(struct dev* ctx);

struct dev* id_to_dev(uint16_t id);

#endif