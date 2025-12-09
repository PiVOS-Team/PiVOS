#include <kernel/device/driver.h>
#include <kernel/utils.h>

extern char drivers_space_start;
extern char drivers_space_end;

static inline struct drv* drv_find_match(struct dev* ctx) {
    uint16_t entry_size = sizeof(struct drv*);
    uint16_t number_of_entries = (uint16_t)((&drivers_space_end - &drivers_space_start) / entry_size);

    struct drv** entries = (struct drv**)&drivers_space_start;

    for (uint16_t i = 0; i < number_of_entries; i++) {
        struct drv* entry = entries[i];

        if (entry->bus_type == ctx->bus_connection && strcmp(entry->name, ctx->name) == 0) {
            return entry;
        }
    }

    return NULL;
}

int32_t drv_attach(struct dev* ctx) {
    struct drv* match_drv = drv_find_match(ctx);

    if (match_drv == NULL) {
        return 0;
    }

    match_drv->update_ctx(ctx);
    ctx->status = DEV_STATUS_DRIVER_ATTACHED;

    return 1;
}