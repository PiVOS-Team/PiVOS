#define TIMER_PROTECTED_INTERFACE

#include <drivers/timer/arm_generic_timer/impl.h>
#include <drivers/timer/arm_generic_timer/reg.h>
#include <kernel/device/dev_event.h>
#include <kernel/device/driver.h>
#include <kernel/device/irq_controller.h>

static inline void generic_timer_set_interval(uint16_t interval) {
    union cntp_tval_el0_t tval;
    union cntfrq_el0_t frq;

    __asm__ volatile("mrs %[dst], cntfrq_el0" : [dst] "=r"(frq));

    uint32_t freq = frq.fields.ClockFrequency;
    tval.fields.TimerValue = (freq * interval) / 1000;

    __asm__ volatile("msr cntp_tval_el0, %[src]" : : [src] "r"(tval.bits));
}

static inline void generic_timer_toggle(uint8_t state) {
    union cntp_ctl_el0_t ctl;
    ctl.fields.ISTATUS = 0;
    ctl.fields.ENABLE = state;

    __asm__ volatile("msr cntp_ctl_el0, %[src]" : : [src] "r"(ctl.bits));
}

static enum irq_handle_status generic_timer_irq_handler(struct dev* ctx) {
    struct dev_timer* timer_ctx = (struct dev_timer*)ctx;

    protected_timer_tick(timer_ctx);

    generic_timer_set_interval(timer_ctx->data->interval);
    generic_timer_toggle(1);

    return IRQ_HANDLED;
}

static int32_t api_generic_timer_init(struct dev* ctx) {
    struct dev_generic_timer* generic_timer_ctx = (struct dev_generic_timer*)ctx;
    protected_timer_init((struct dev_timer*)ctx);

    struct dev_irq_controller* irq_ctrl = ctx->config->irq_controller;
    dev_event_post((struct dev*)irq_ctrl, DEV_EVENT_ENSURE_DEVICE_INITIALIZED);

    struct irq_params params = {
        .target_cpu = IRQ_DEFAULT_CPU,
        .priority = IRQ_PRIORITY_REGULAR,
        .trigger_type = IRQ_TRIGGER_EDGE};

    uint16_t irq_number = generic_timer_ctx->config->irq_number;
    irq_controller_register(irq_ctrl, irq_number, &params, generic_timer_irq_handler, ctx);
    irq_controller_toggle(irq_ctrl, irq_number, 1);

    return 1;
}

static int32_t api_generic_timer_destroy(struct dev* ctx) {
    return 0;
}

static void api_generic_timer_start(struct dev_timer* ctx) {
    generic_timer_toggle(1);
}

static void api_generic_timer_stop(struct dev_timer* ctx) {
    generic_timer_toggle(0);
}

static void api_generic_timer_update_interval(struct dev_timer* ctx) {
    generic_timer_set_interval(ctx->data->interval);
}

static struct dev_generic_timer_api generic_timer_api = {
    .init = api_generic_timer_init,
    .destroy = api_generic_timer_destroy,
    .start = api_generic_timer_start,
    .stop = api_generic_timer_stop,
    .update_interval = api_generic_timer_update_interval,
};

static void generic_timer_update_ctx(struct dev* ctx) {
    struct dev_generic_timer* timer = (struct dev_generic_timer*)ctx;
    timer->api = &generic_timer_api;
}

DRV_REGISTER_NEW(generic_timer, BUS_TYPE_MMIO, DEV_TYPE_TIMER, generic_timer_update_ctx)
