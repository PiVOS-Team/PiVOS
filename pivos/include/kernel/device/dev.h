#ifndef KERNEL_DEVICE_DEV_H_
#define KERNEL_DEVICE_DEV_H_

#include <stdint.h>

#define _DEV_BASE_FIELDS                        \
    const char* name;                           \
    enum dev_type type;                         \
    enum bus_type bus_connection;               \
    enum dev_status status;

#define _DEV_FIELDS(name)                       \
    struct dev_##name##_config* config;         \
    struct dev_##name##_api* api;               \
    struct dev_##name##_data* data;

#define _DEV_DEFINE_STRUCT(name, fields)        \
    struct dev_##name {                         \
        fields                                  \
    };

#define _DEV_DEFINE_STRUCT_AND_INHERIT(name, fields, inherit_fields)\
    struct dev_##name {                                             \
        inherit_fields                                              \
        fields                                                      \
    };

#define _DEV_DEFINE_CONFIG(name, fields)        \
    _DEV_DEFINE_STRUCT(name##_config, fields)

#define _DEV_DEFINE_API(name, fields)           \
    _DEV_DEFINE_STRUCT(name##_api, fields)

#define _DEV_DEFINE_DATA(name, fields)          \
    _DEV_DEFINE_STRUCT(name##_data, fields)

#define _DEV_DEFINE_HEADER(name) \
    struct dev_##name;

#define DEV_DEFINE_NEW(name)                                    \
    struct dev_##name;                                          \
    _DEV_DEFINE_CONFIG(name, DEV_##name##_CONFIG_FIELDS)        \
    _DEV_DEFINE_API(name, DEV_##name##_API_FIELDS)              \
    _DEV_DEFINE_DATA(name, DEV_##name##_DATA_FIELDS)            \
    _DEV_DEFINE_STRUCT_AND_INHERIT(name, _DEV_FIELDS(name), _DEV_BASE_FIELDS)


#define DEV_dev_CONFIG_FIELDS                   \
     struct dev_irq_controller* irq_controller; \
     struct dev_mem_reg* mem;

#define DEV_dev_API_FIELDS                      \
    int32_t (*init)(struct dev* ctx);           \
    int32_t (*destroy)(struct dev* ctx);        

#define DEV_dev_DATA_FIELDS

enum dev_status {
    DEV_STATUS_NEW,
    DEV_STATUS_DRIVER_ATTACHED,
    DEV_STATUS_INITIALIZED,
};

enum dev_type {
    DEV_TYPE_IRQ_CONTROLLER,
    DEV_TYPE_TIMER,
    DEV_TYPE_UART,
    DEV_TYPE_BUS,
    DEV_TYPE__N
};

enum bus_type {
    BUS_TYPE_MMIO,
    BUS_TYPE_PCI,
    BUS_TYPE_I2C,
    BUS_TYPE_SPI,
    BUS_TYPE_MMC,
    BUS_TYPE__N
};

struct dev_mem_reg {
    uint64_t base;
    uint64_t size;
};

struct dev;

struct dev_config {
    DEV_dev_CONFIG_FIELDS
};

struct dev_api {
    DEV_dev_API_FIELDS
};

struct dev_data {
    DEV_dev_DATA_FIELDS
};

struct dev {
    _DEV_BASE_FIELDS

    struct dev_config* config;
    struct dev_api* api;
    struct dev_data* data;
};

#ifdef DEV_PROTECTED_INTERFACE

int32_t protected_dev_init(struct dev* ctx);

#endif


int32_t dev_init(struct dev* ctx);

#endif
