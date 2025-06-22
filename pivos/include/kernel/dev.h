#ifndef KERNEL_DEVICE_H_
#define KERNEL_DEVICE_H_

#include <kernel/int.h>
#include <kernel/log.h>
#include <stdint.h>

#ifndef KDEV_BUFFER_SIZE
#define KDEV_BUFFER_SIZE 1024
#endif  // KDEV_BUFFER_SIZE

enum kdev_type {
    KDEV_GENERIC,
    KDEV_IO,
    KDEV_GIC,
    KDEV_TIMER,
};

struct kdev_generic {
    enum kdev_type type;
    void *ctx;
    void (*isr)(void *);
};

struct kdev_io {
    enum kdev_type type;
    void *ctx;
    int32_t (*isr)(void *);
    int32_t (*write)(void *, uint8_t *, uint32_t);
    int32_t (*read)(void *, uint8_t *, uint32_t);
    int32_t (*pool_out)(void *, uint8_t);
    int32_t (*pool_in)(void *, uint8_t *);
};

struct kdev_gic {
    enum kdev_type type;
    void *ctx;
    void (*isr)(void *);
    int32_t (*register_irq)(void *, uint32_t, int32_t, void *);
    int32_t (*enable_irq)(void *, uint32_t);
    int_dispatch_fn (*get_dispatch)(void *);
};

struct kdev_timer {
    enum kdev_type type;
    void *ctx;
    void (*isr)(void *);
    void (*set_action)(void *, void (*)(void));
};

int32_t kdev_register(int32_t dev_id, struct kdev_generic *dev);

int32_t kdev_generic_isr(int32_t dev_id);

int32_t kdev_io_write(int32_t dev_id, uint8_t *src, uint32_t n);

int32_t kdev_io_read(int32_t dev_id, uint8_t *dst, uint32_t n);

int32_t kdev_io_sync(int32_t dev_id);

int32_t kdev_io_pool_out(int32_t dev_id, uint8_t c);

int32_t kdev_io_pool_in(int32_t dev_id, uint8_t *c);

int32_t kdev_gic_register_irq(int32_t dev_id, uint32_t irq_id,
                              int32_t irq_dev_id, void *params);

int32_t kdev_gic_enable_irq(int32_t dev_id, uint32_t irq_id);

int_dispatch_fn kdev_gic_get_dispatch(int32_t dev_id);

int32_t kdev_timer_set_action(int32_t dev_id, void (*action)(void));

#endif  // KERNEL_DEVICE_H_