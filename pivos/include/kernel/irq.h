#ifndef KERNEL_IRQ_H_
#define KERNEL_IRQ_H_

#include <kernel/device/irq_controller.h>

typedef void (*irq_post_handler)();

void irq_init(struct dev_irq_controller* main_controller);

void irq_register_post_handler(irq_post_handler callback);

void irq_dispatch();

#endif