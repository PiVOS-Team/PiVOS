#ifndef KERNEL_DEVICE_H_
#define KERNEL_DEVICE_H_

#include <kernel/device/bus.h>
#include <kernel/device/dev.h>
#include <kernel/device/dev_event.h>
#include <kernel/device/dev_registry.h>
#include <kernel/device/driver.h>
#include <kernel/device/irq_controller.h>
#include <kernel/device/timer.h>
#include <kernel/device/uart.h>

int32_t device_init();

#endif
