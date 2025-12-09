#ifndef KERNEL_CONFIG_H_
#define KERNEL_CONFIG_H_

#include <kernel/device/dev.h>
#include <kernel/device/irq_controller.h>
#include <kernel/device/timer.h>
#include <kernel/device/uart.h>
#include <kernel/mem.h>

struct config_selected_dev {
    struct dev_uart* console;
    struct dev_timer* main_timer;
    struct dev_irq_controller* main_controller;
};

struct config_selected_dev* config_get_selected_dev();

struct dev** config_get_all_dev(uint16_t* dev_number);

struct mem_reg** config_get_global_mem_regs(uint16_t* reg_number);

#endif
