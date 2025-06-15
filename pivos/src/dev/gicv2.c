#include <kernel/dev.h>

#include <kernel/dev/gicv2.h>
#include <kernel/dev/gicv2/impl.h>
#include <kernel/dev/gicv2/reg.h>

static struct gicv2 *gicv2 = (struct gicv2 *)GICV2_ADDRESS;
static int32_t irq_devices[1024];
static struct kdev_gic gic_kdev;

int32_t dev_gicv2_kdev_register_irq(
    void *ctx, uint32_t irq_id, int32_t irq_dev_id, void *params) {
    struct dev_gicv2_params *p = params;
    gicv2_set_int_priority(irq_id, p->priority);
    gicv2_set_int_target(irq_id, p->cpu);
    gicv2_set_int_trigger(irq_id, p->trigger);

    // handle maximum irq_id better
    irq_devices[irq_id] = irq_dev_id;

    return KSTATUS_DEV_GIC_OK;
}

int32_t dev_gicv2_kdev_enable_irq(void *ctx, uint32_t irq_id) {
    uint32_t reg     = irq_id / 32;
    uint32_t shift   = irq_id % 32;
    uint32_t new_val = 1 << shift;

    gicv2->GICD.ISENABLER[reg].bits = new_val;
    return KSTATUS_DEV_GIC_OK;
}

int_dispatch_fn dev_gicv2_kdev_gic_get_dispatch(void *ctx) {
    return gicv2_dispatch;
}

void gicv2_dispatch(int64_t *_rs, uint64_t _esr) {
    int32_t irq_id     = gicv2_get_irq_id();
    int32_t irq_device = irq_devices[irq_id];

    kdev_generic_isr(irq_device);

    gicv2_end_isr(irq_id);
}

int32_t gicv2_get_irq_id() {
    union gicc_iar iar = {.bits = gicv2->GICC.IAR.bits};
    return iar.fields.InterruptID;
}

void gicv2_end_isr(int32_t irq_id) {
    union gicc_eoir eoir  = {.bits = gicv2->GICC.EOIR.bits};
    eoir.fields.EOIINTID  = irq_id;
    gicv2->GICC.EOIR.bits = eoir.bits;
}

void gicv2_set_int_priority(uint32_t id, uint8_t priority) {
    uint32_t reg     = id / 4;
    uint32_t shift   = (id % 4) * 8;
    uint32_t new_val = (gicv2->GICD.IPRIORITYR[reg].bits & ~(0xFF << shift)) |
        (priority << shift);

    gicv2->GICD.IPRIORITYR[reg].bits = new_val;
}

void gicv2_set_int_target(uint32_t id, uint8_t cpu) {
    uint32_t reg   = id / 4;
    uint32_t shift = (id % 4) * 8;
    uint32_t new_val =
        (gicv2->GICD.ITARGETSR[reg].bits & ~(0xFF << shift)) | (cpu << shift);

    gicv2->GICD.ITARGETSR[reg].bits = new_val;
}

void gicv2_set_int_group(uint32_t id, uint8_t group) {
    uint32_t reg   = id / 32;
    uint32_t shift = id % 32;
    uint32_t new_val = (gicv2->GICD.IGROUPR[reg].bits & ~(1 << shift)) | (group << shift);

    gicv2->GICD.IGROUPR[reg].bits = new_val;
}

void gicv2_set_int_trigger(uint32_t id, uint8_t trigger) {
    uint32_t reg   = id / 16;
    uint32_t shift = (id % 16) * 2;
    uint32_t new_val =
        (gicv2->GICD.ICFGR[reg].bits & ~(0x3 << shift)) | (trigger << shift);

    gicv2->GICD.ICFGR[reg].bits = new_val;
}

int32_t dev_gicv2_init() {
    union gicd_ctlr gicd_ctlr = {.bits = gicv2->GICD.CTLR.bits};
    union gicc_ctlr gicc_ctlr = {.bits = gicv2->GICC.CTLR.bits};
    union gicc_pmr gicc_pmr   = {.bits = gicv2->GICC.PMR.bits};

    gicd_ctlr.fields.ENABLE                           = 1;
    gicc_ctlr.fields_gicv2_with_SE_NS_copy.EnableGrp1 = 1;
    gicc_pmr.fields.Priority                          = 0xFF;

    gicv2->GICD.CTLR.bits = gicd_ctlr.bits;
    gicv2->GICC.CTLR.bits = gicc_ctlr.bits;
    gicv2->GICC.PMR.bits  = gicc_pmr.bits;

    gic_kdev = (struct kdev_gic){
        .type         = KDEV_GIC,
        .ctx          = 0,
        .isr          = 0,
        .register_irq = dev_gicv2_kdev_register_irq,
        .enable_irq   = dev_gicv2_kdev_enable_irq,
        .get_dispatch = dev_gicv2_kdev_gic_get_dispatch,
    };

    return 0;
}

struct kdev_gic *dev_gicv2_get() {
    return &gic_kdev;
}