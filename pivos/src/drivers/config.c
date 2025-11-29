#include <kernel/config.h>
#include <kernel/mem.h>
#include <kernel/device.h>
#include <kernel/utils.h>

#if defined(TARGET_RPI_4B)

#include <drivers/irq_controller/gic/impl.h>
#include <drivers/uart/pl011/impl.h>
#include <drivers/timer/arm_generic_timer/impl.h>

static struct mem_reg mem_reg_dram0 = {
    .base = 0,
    .size = 0xfc000000,
    .mem_type = MEM_TYPE_NORMAL_NO_CACHE
};

static struct mem_reg mem_reg_dram1 = {
    .base = 0xfc000000,
    .size = 0x100000000 - 0xfc000000,
    .mem_type = MEM_TYPE_DEVICE
};

static struct mem_reg* mem_regs[] = {
    &mem_reg_dram0,
    &mem_reg_dram1,
};

static struct dev_gic gic = {
    .name = "gic",
    .type = DEV_TYPE_IRQ_CONTROLLER,
    .bus_connection = BUS_TYPE_MMIO,
    .status = DEV_STATUS_NEW,
    
    .api = NULL,
    .data = &(struct dev_gic_data) {},
    
    .config = &(struct dev_gic_config) {
        .irq_controller = NULL,
        .version = 2,
        .mem = &(struct dev_mem_reg){
            .base = 0xFF840000 + 0xFFFF000000000000ULL,
            .size = 0x0
        }
    }
};

static struct dev_pl011 uart0 = {
    .name = "pl011",
    .type = DEV_TYPE_UART,
    .bus_connection = BUS_TYPE_MMIO,
    .status = DEV_STATUS_NEW,

    .api = NULL,
    .data = &(struct dev_pl011_data) {},

    .config = &(struct dev_pl011_config) {
        .irq_controller = (struct dev_irq_controller*)&gic,
        .irq_number = 153,
        .mem = &(struct dev_mem_reg) {
            .base = 0xfe201000UL + 0xFFFF000000000000ULL,
            .size = 0x0
        }
    }
};

static struct dev_generic_timer arm_generic_timer = {
    .name = "generic_timer",
    .type = DEV_TYPE_TIMER,
    .bus_connection = BUS_TYPE_MMIO,
    .status = DEV_STATUS_NEW,

    .api = NULL,
    .data = &(struct dev_generic_timer_data) {},

    .config = &(struct dev_generic_timer_config) {
        .irq_controller = (struct dev_irq_controller*)&gic,
        .irq_number = 30,
        .mem = NULL
    }
};

static struct dev* devs[] = {
    (struct dev*)&gic,
    (struct dev*)&uart0,
    (struct dev*)&arm_generic_timer
};

static struct config_selected_dev selected_devs = {
    .console = (struct dev_uart*)&uart0,
    .main_timer = (struct dev_timer*)&arm_generic_timer,
    .main_controller = (struct dev_irq_controller*)&gic
};

static uint16_t number_of_devices = sizeof(devs) / sizeof(struct dev*);
static uint16_t number_of_mem_regions = sizeof(mem_regs) / sizeof(struct mem_reg*);

#elif defined(TARGET_STANDALONE)

#error "Standalone targer is not supported yet"

#else

#error "Undefined target"

#endif


struct config_selected_dev* config_get_selected_dev() {
    return &selected_devs;
}

struct dev** config_get_all_dev(uint16_t* dev_number) {
    *dev_number = number_of_devices;
    return devs;
}

struct mem_reg** config_get_global_mem_regs(uint16_t* reg_num) {
    *reg_num = number_of_mem_regions;
    return mem_regs;
}