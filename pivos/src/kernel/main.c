#include <kernel/arch/cpu.h>
#include <kernel/device.h>
#include <kernel/mem.h>
#include <kernel/panic.h>
#include <kernel/config.h>
#include <kernel/irq.h>
#include <kernel/userspace.h>
#include <kernel/io_data.h>

static int32_t kernel_init_mem() {
    uint16_t number_of_regs = 0;
    uint64_t max_allocatable_addr = 0;
    struct mem_reg** regs = config_get_global_mem_regs(&number_of_regs);

    for(uint16_t i = 0; i < number_of_regs; i++) {
        struct mem_reg* curr_mem_reg = regs[i];
        uint64_t curr_mem_reg_max_addr = curr_mem_reg->base + curr_mem_reg->size;

        if (curr_mem_reg->mem_type != MEM_TYPE_DEVICE && 
            curr_mem_reg_max_addr > max_allocatable_addr) {
            max_allocatable_addr = curr_mem_reg_max_addr;
        }
    }

    struct mem_ctx kernel_mem_ctx; 
    mem_init(max_allocatable_addr);
    mem_get_kernel_ctx(&kernel_mem_ctx);

    for(uint16_t i = 0; i < number_of_regs; i++) {
        struct mem_reg* curr_mem_reg = regs[i];

        uint64_t curr_mem_reg_max_addr = curr_mem_reg->base + curr_mem_reg->size;
        uint32_t map_type = MEM_MAP_ONE_TO_ONE;

        // Some devices can be mapped very high, so if 
        // their address is bigger than max allocatable addr
        // we can't allocate it, but we still have to map it in mmu
        if(curr_mem_reg->mem_type == MEM_TYPE_DEVICE && curr_mem_reg_max_addr <= max_allocatable_addr) {
            map_type = MEM_MAP_ONE_TO_ONE_AND_ALLOC;
        }

        mem_map_reg(&kernel_mem_ctx, curr_mem_reg, map_type);
    }

    return 1;
}

static int32_t kernel_init_dev() {
    arch_cpu_disable_int();
    arch_cpu_init_int();

    struct config_selected_dev* selected_devs = config_get_selected_dev();
    irq_init(selected_devs->main_controller);

    uint16_t devs_number = 0;
    struct dev** devs = config_get_all_dev(&devs_number);

    io_data_init();
    device_init();

    for(uint16_t i = 0; i < devs_number; i++) {
        struct dev* curr_dev = devs[i];
        dev_event_post(curr_dev, DEV_EVENT_NEW_DEVICE_FOUND);
    }

    arch_cpu_enable_int();
    
    return 1;
}

static int32_t kernel_init_userspace() {
    struct config_selected_dev* selected_devs = config_get_selected_dev();
    userspace_init(selected_devs->console, selected_devs->main_timer);

    return 1;
}

void kernel_main() {
    if(!kernel_init_mem()) {
        kernel_panic("Memory init failed");
    }

    if(!kernel_init_dev()) {
        kernel_panic("Device init failed");
    }

    if(!kernel_init_userspace()) {
        kernel_panic("Userspace init failed");
    }

    // Infinite
    userspace_start();
    kernel_panic("Kernel undefined state");
}
