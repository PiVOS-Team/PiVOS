#include <kernel/config.h>
#include <kernel/device.h>

int32_t device_init() {
    dev_event_init();
    dev_registry_init();

    return 1;
}
