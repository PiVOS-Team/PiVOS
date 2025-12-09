#ifndef DRIVERS_IRQ_CONTROLLER_GIC_IMPL_H_
#define DRIVERS_IRQ_CONTROLLER_GIC_IMPL_H_

#include <kernel/device/irq_controller.h>

#define DEV_gic_CONFIG_FIELDS        \
    DEV_irq_controller_CONFIG_FIELDS \
        uint8_t version;

#define DEV_gic_API_FIELDS \
    DEV_irq_controller_API_FIELDS

#define DEV_gic_DATA_FIELDS \
    DEV_irq_controller_DATA_FIELDS

DEV_DEFINE_NEW(gic)

#endif