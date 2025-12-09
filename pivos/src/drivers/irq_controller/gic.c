#define IRQ_CONTROLLER_PROTECTED_INTERFACE

#include <drivers/irq_controller/gic/impl.h>
#include <drivers/irq_controller/gic/reg.h>
#include <kernel/device/driver.h>

static inline struct gicv2* gicv2_get_regs(struct dev_irq_controller* ctx) {
    return (struct gicv2*)ctx->config->mem->base;
}

static inline int32_t gicv2_get_irq_id(struct gicv2* gicv2) {
    union gicc_iar iar = {.bits = gicv2->GICC.IAR.bits};
    return iar.fields.InterruptID;
}

static inline void gicv2_end_isr(struct gicv2* gicv2, uint16_t irq_id) {
    union gicc_eoir eoir = {.bits = gicv2->GICC.EOIR.bits};
    eoir.fields.EOIINTID = irq_id;
    gicv2->GICC.EOIR.bits = eoir.bits;
}

static inline void gicv2_set_int_priority(struct gicv2* gicv2, uint32_t id, uint8_t priority) {
    uint32_t reg = id / 4;
    uint32_t shift = (id % 4) * 8;
    uint32_t new_val = (gicv2->GICD.IPRIORITYR[reg].bits & ~(0xFF << shift)) | (priority << shift);

    gicv2->GICD.IPRIORITYR[reg].bits = new_val;
}

static inline void gicv2_set_int_target(struct gicv2* gicv2, uint32_t id, uint8_t cpu) {
    uint32_t reg = id / 4;
    uint32_t shift = (id % 4) * 8;
    uint32_t new_val = (gicv2->GICD.ITARGETSR[reg].bits & ~(0xFF << shift)) | (cpu << shift);

    gicv2->GICD.ITARGETSR[reg].bits = new_val;
}

static inline void gicv2_set_int_group(struct gicv2* gicv2, uint32_t id, uint8_t group) {
    uint32_t reg = id / 32;
    uint32_t shift = id % 32;
    uint32_t new_val = (gicv2->GICD.IGROUPR[reg].bits & ~(1 << shift)) | (group << shift);

    gicv2->GICD.IGROUPR[reg].bits = new_val;
}

static inline void gicv2_set_int_trigger(struct gicv2* gicv2, uint32_t id, uint8_t trigger) {
    uint32_t reg = id / 16;
    uint32_t shift = (id % 16) * 2;
    uint32_t new_val = (gicv2->GICD.ICFGR[reg].bits & ~(0x3 << shift)) | (trigger << shift);

    gicv2->GICD.ICFGR[reg].bits = new_val;
}

static int32_t api_gic_init(struct dev* ctx) {
    // Call base init
    protected_irq_controller_init((struct dev_irq_controller*)ctx);

    struct gicv2* gicv2 = gicv2_get_regs((struct dev_irq_controller*)ctx);

    union gicd_ctlr gicd_ctlr = {.bits = gicv2->GICD.CTLR.bits};
    union gicc_ctlr gicc_ctlr = {.bits = gicv2->GICC.CTLR.bits};
    union gicc_pmr gicc_pmr = {.bits = gicv2->GICC.PMR.bits};

    gicd_ctlr.fields.ENABLE = 1;
    gicc_ctlr.fields_gicv2_with_SE_NS_copy.EnableGrp1 = 1;
    gicc_pmr.fields.Priority = 0xFF;

    gicv2->GICD.CTLR.bits = gicd_ctlr.bits;
    gicv2->GICC.CTLR.bits = gicc_ctlr.bits;
    gicv2->GICC.PMR.bits = gicc_pmr.bits;

    return 1;
}

static int32_t api_gic_destroy(struct dev* ctx) {
    // GIC irq controller is part of ARM architecture - so it cannot be removed
    return 0;
}

static void api_gic_setup_irq(struct dev_irq_controller* ctx, uint16_t irq_number, struct irq_params* params) {
    struct gicv2* gicv2 = gicv2_get_regs(ctx);

    gicv2_set_int_trigger(gicv2, irq_number, params->trigger_type);
    gicv2_set_int_priority(gicv2, irq_number, params->priority);
    gicv2_set_int_target(gicv2, irq_number, params->target_cpu);
}

static void api_gic_toggle_irq(struct dev_irq_controller* ctx, uint16_t irq_number, uint8_t state) {
    struct gicv2* gicv2 = gicv2_get_regs(ctx);

    uint32_t reg = irq_number / 32;
    uint32_t shift = irq_number % 32;
    uint32_t new_val = 1 << shift;

    if (state == 1) {
        gicv2->GICD.ISENABLER[reg].bits = new_val;
    } else {
        gicv2->GICD.ICENABLER[reg].bits = new_val;
    }
}

static void api_gic_end_of_interrupt(struct dev_irq_controller* ctx, uint16_t irq_number) {
    struct gicv2* gicv2 = gicv2_get_regs(ctx);
    gicv2_end_isr(gicv2, irq_number);
}

static uint16_t api_gic_get_pending_irq(struct dev_irq_controller* ctx) {
    struct gicv2* gicv2 = gicv2_get_regs(ctx);
    return gicv2_get_irq_id(gicv2);
}

static uint16_t api_gic_get_max_irq(struct dev_irq_controller* ctx) {
    struct gicv2* gicv2 = gicv2_get_regs(ctx);
    uint16_t N = gicv2->GICD.TYPER.fields.ITLinesNumber;
    return 32 * (N + 1);
}

static struct dev_gic_api gic_api = {
    .init = api_gic_init,
    .destroy = api_gic_destroy,
    .setup_irq = api_gic_setup_irq,
    .toggle_irq = api_gic_toggle_irq,
    .end_of_interrupt = api_gic_end_of_interrupt,
    .get_pending_irq = api_gic_get_pending_irq,
    .get_max_irq = api_gic_get_max_irq,
};

static void drv_gic_update_ctx(struct dev* ctx) {
    struct dev_gic* gic = (struct dev_gic*)ctx;
    gic->api = &gic_api;
}

DRV_REGISTER_NEW(gic, BUS_TYPE_MMIO, DEV_TYPE_IRQ_CONTROLLER, drv_gic_update_ctx)
