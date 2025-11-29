#ifndef KERNEL_MEM_MMU_H_
#define KERNEL_MEM_MMU_H_

#ifndef MMU_PAGE_SIZE
#define MMU_PAGE_SIZE 0
#error "MMU page size was not defined"
#endif

#include <stdint.h>

struct arch_mmu_space_req {
    uint64_t virt_addr;
    uint32_t page_count;
    uint32_t align;
};

void arch_mmu_init(void* (*page_alloc)(uint32_t number, uint32_t allign), void (*page_dealloc)(void* addr, uint32_t number));

void arch_mmu_insert(void* table, uint64_t addr, uint64_t size, uint8_t memory_type, void* (*get_phys_space)(struct arch_mmu_space_req* req));

void* arch_mmu_va_to_pa(void* table, void* va);

void* arch_mmu_create_table();

void arch_mmu_destroy_table(void* table);

void arch_mmu_change_usr_ctx(void* table, uint16_t id);

void* arch_mmu_get_usr_ctx();

#endif