#ifndef KERNEL_DEVICE_DRIVER_H_
#define KERNEL_DEVICE_DRIVER_H_

#include <kernel/device/dev.h>

#define DRV_REGISTER_NEW(drv_name, drv_bus_type, drv_dev_type, drv_update_ctx_callback) \
    static struct drv _driver_##drv_name = {                                            \
        .name = #drv_name,                                                              \
        .bus_type = drv_bus_type,                                                       \
        .dev_type = drv_dev_type,                                                       \
        .update_ctx = drv_update_ctx_callback                                           \
    };                                                                                  \
    static struct drv* __driver_##drv_name __attribute__((section(".drivers"), used)) = &_driver_##drv_name;

struct drv {
    const char* name;
    enum bus_type bus_type;
    enum dev_type dev_type;

    void (*update_ctx)(struct dev* ctx);
};

int32_t drv_attach(struct dev* ctx);

#endif
