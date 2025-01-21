#ifndef KERNEL_DEV_GICV2_H_
#define KERNEL_DEV_GICV2_H_

#include <stdint.h>

int32_t dev_gicv2_init();
struct kdev_gic *dev_gicv2_get();

struct dev_gicv2_params {
    uint8_t priority;
    uint8_t cpu;
    uint8_t group;
    uint8_t trigger;
};

#endif  // KERNEL_DEV_GICV2_H_