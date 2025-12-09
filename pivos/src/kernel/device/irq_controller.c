#define DEV_PROTECTED_INTERFACE
#define IRQ_CONTROLLER_PROTECTED_INTERFACE

#include <kernel/device/irq_controller.h>
#include <kernel/utils.h>

static struct irq_dispatch_entry dispatchers[256];

int32_t protected_irq_controller_init(struct dev_irq_controller* ctx) {
    // Call base init
    protected_dev_init((struct dev*)ctx);
    ctx->data->dispatchers = dispatchers;
    ctx->data->dispatchers_count = 256;

    return 1;
}

int32_t irq_controller_register(struct dev_irq_controller* ctx, uint16_t irq_number, struct irq_params* params, irq_handler handler, struct dev* sender) {
    uint16_t supported_irqs = ctx->data->dispatchers_count;

    if (irq_number > supported_irqs) {
        return 0;
    }

    struct irq_dispatch_entry* entry = &ctx->data->dispatchers[irq_number];

    if (entry->handler != NULL) {
        return 0;
    }

    entry->handler = handler;
    entry->sender = sender;

    ctx->api->setup_irq(ctx, irq_number, params);

    return 1;
}

int32_t irq_controller_toggle(struct dev_irq_controller* ctx, uint16_t irq_number, uint8_t state) {
    ctx->api->toggle_irq(ctx, irq_number, state);
    return 1;
}

int32_t irq_controller_dispatch(struct dev_irq_controller* ctx) {
    uint16_t pending_irq = ctx->api->get_pending_irq(ctx);

    struct irq_dispatch_entry* entry = &ctx->data->dispatchers[pending_irq];
    entry->handler(entry->sender);

    ctx->api->end_of_interrupt(ctx, pending_irq);

    return 1;
}