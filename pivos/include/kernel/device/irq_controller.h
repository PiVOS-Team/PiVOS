#ifndef KERNEL_DEVICE_IRQ_CONTROLLER_H_
#define KERNEL_DEVICE_IRQ_CONTROLLER_H_

#include <kernel/device/dev.h>

enum irq_cpu {
    IRQ_DEFAULT_CPU = 1
};

enum irq_priority {
    IRQ_PRIORITY_LOW = 0,
    IRQ_PRIORITY_REGULAR = 0,
    IRQ_PRIORITY_HIGH = 0
};

enum irq_trigger_type {
    IRQ_TRIGGER_LEVEL,
    IRQ_TRIGGER_EDGE = 2
};

enum irq_handle_status {
    IRQ_HANDLED,
    IRQ_UNHANDLED
};

typedef enum irq_handle_status (*irq_handler)(struct dev* ctx);

struct irq_params {
    uint8_t target_cpu;
    enum irq_priority priority;
    enum irq_trigger_type trigger_type;
};

struct irq_dispatch_entry {
    irq_handler handler;
    struct dev* sender;
};


#define DEV_irq_controller_CONFIG_FIELDS    \
    DEV_dev_CONFIG_FIELDS

#define DEV_irq_controller_API_FIELDS                                                                   \
    DEV_dev_API_FIELDS                                                                                  \
    void (*setup_irq)(struct dev_irq_controller* ctx, uint16_t irq_number, struct irq_params* params);   \
    void (*toggle_irq)(struct dev_irq_controller* ctx, uint16_t irq_number, uint8_t state);             \
    void (*end_of_interrupt)(struct dev_irq_controller* ctx, uint16_t irq_number);                      \
    uint16_t (*get_pending_irq)(struct dev_irq_controller* ctx);                                        \
    uint16_t (*get_max_irq)(struct dev_irq_controller* ctx);

#define DEV_irq_controller_DATA_FIELDS      \
    DEV_dev_DATA_FIELDS                     \
    struct irq_dispatch_entry* dispatchers; \
    uint16_t dispatchers_count;             \

DEV_DEFINE_NEW(irq_controller)

#ifdef IRQ_CONTROLLER_PROTECTED_INTERFACE

int32_t protected_irq_controller_init(struct dev_irq_controller* ctx);

#endif

int32_t irq_controller_register(struct dev_irq_controller* ctx, uint16_t irq_number, struct irq_params* params, irq_handler handler, struct dev* sender);

int32_t irq_controller_toggle(struct dev_irq_controller* ctx, uint16_t irq_number, uint8_t state);

int32_t irq_controller_dispatch(struct dev_irq_controller* ctx);

#endif