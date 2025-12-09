#include <kernel/device/bus.h>
#include <kernel/device/dev_event.h>
#include <kernel/device/dev_registry.h>
#include <kernel/device/driver.h>
#include <kernel/panic.h>

#define MAX_DEV_NUMBER 64

static struct dev* s_devs[MAX_DEV_NUMBER];
static uint16_t s_dev_number;

static void register_new_dev(struct dev* ctx) {
    if (s_dev_number + 1 >= MAX_DEV_NUMBER) {
        kernel_panic("Exceded max number of devices");
    }

    s_devs[s_dev_number++] = ctx;
}

static void try_init_device(struct dev* ctx) {
    if (ctx->status == DEV_STATUS_NEW) {
        register_new_dev(ctx);
        drv_attach(ctx);
    }

    if (ctx->status == DEV_STATUS_DRIVER_ATTACHED) {
        dev_init(ctx);
    }

    if (ctx->type == DEV_TYPE_BUS) {
        struct dev_bus* bus = (struct dev_bus*)ctx;
        bus_search(bus);
    }
}

void dev_registry_init() {
    s_dev_number = 0;

    dev_event_register_handler(DEV_EVENT_NEW_DEVICE_FOUND, try_init_device);
    dev_event_register_handler(DEV_EVENT_ENSURE_DEVICE_INITIALIZED, try_init_device);
}

uint16_t dev_to_id(struct dev* ctx) {
    for (uint16_t i = 0; i < MAX_DEV_NUMBER; i++) {
        if (s_devs[i] == ctx) {
            return i;
        }
    }

    return (uint16_t)-1;
}

struct dev* id_to_dev(uint16_t id) {
    if (id >= MAX_DEV_NUMBER) {
        return 0;
    }

    return s_devs[id];
}