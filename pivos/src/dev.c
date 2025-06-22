#include <kernel/dev.h>
#include <kernel/log.h>

static struct kdev_generic *kdevs[KDEV_BUFFER_SIZE];

int32_t kdev_register(int32_t dev_id, struct kdev_generic *dev) {
    if (dev_id >= KDEV_BUFFER_SIZE)
        return KSTATUS_DEV_OUT_OF_BOUNDS;
    if (kdevs[dev_id])
        return KSTATUS_DEV_ALREADY_EXISTS;

    kdevs[dev_id] = dev;
    return dev_id;
}

int32_t kdev_generic_isr(int32_t dev_id) {
    struct kdev_generic *kdev = kdevs[dev_id];
    if (!kdev->isr) {
        return KSTATUS_DEV_GENERIC_NO_ISR;
    }
    kdev->isr(kdev->ctx);
    return KSTATUS_DEV_GENERIC_OK;
}

int32_t kdev_io_write(int32_t dev_id, uint8_t *src, uint32_t n) {
    struct kdev_io *kdev = (struct kdev_io *)kdevs[dev_id];
    if (kdev->type != KDEV_IO) {
        return KSTATUS_DEV_INVALID;
    }

    return kdev->write(kdev->ctx, src, n);
}

int32_t kdev_io_read(int32_t dev_id, uint8_t *dst, uint32_t n) {
    struct kdev_io *kdev = (struct kdev_io *)kdevs[dev_id];
    if (kdev->type != KDEV_IO) {
        return KSTATUS_DEV_INVALID;
    }

    return kdev->read(kdev->ctx, dst, n);
}

int32_t kdev_io_pool_out(int32_t dev_id, uint8_t c) {
    struct kdev_io *kdev = (struct kdev_io *)kdevs[dev_id];
    if (kdev->type != KDEV_IO) {
        return KSTATUS_DEV_INVALID;
    }

    return kdev->pool_out(kdev->ctx, c);
}

int32_t kdev_io_pool_in(int32_t dev_id, uint8_t *c) {
    struct kdev_io *kdev = (struct kdev_io *)kdevs[dev_id];
    if (kdev->type != KDEV_IO) {
        return KSTATUS_DEV_INVALID;
    }

    return kdev->pool_in(kdev->ctx, c);
}

int32_t kdev_gic_register_irq(int32_t dev_id, uint32_t irq_id,
                              int32_t irq_dev_id, void *params) {
    struct kdev_gic *kdev = (struct kdev_gic *)kdevs[dev_id];
    if (kdev->type != KDEV_GIC) {
        return KSTATUS_DEV_INVALID;
    }

    return kdev->register_irq(kdev->ctx, irq_id, irq_dev_id, params);
}

int32_t kdev_gic_enable_irq(int32_t dev_id, uint32_t irq_id) {
    struct kdev_gic *kdev = (struct kdev_gic *)kdevs[dev_id];
    if (kdev->type != KDEV_GIC) {
        return KSTATUS_DEV_INVALID;
    }

    return kdev->enable_irq(kdev->ctx, irq_id);
}

int_dispatch_fn kdev_gic_get_dispatch(int32_t dev_id) {
    struct kdev_gic *kdev = (struct kdev_gic *)kdevs[dev_id];
    if (kdev->type != KDEV_GIC) {
        return (int_dispatch_fn)0;
    }

    return kdev->get_dispatch(kdev->ctx);
}

int32_t kdev_timer_set_action(int32_t dev_id, void (*action)(void)) {
    struct kdev_timer *kdev = (struct kdev_timer *)kdevs[dev_id];
    if (kdev->type != KDEV_TIMER) {
        return KSTATUS_DEV_INVALID;
    }

    kdev->set_action(kdev->ctx, action);
    return KSTATUS_DEV_TIMER_OK;
}
