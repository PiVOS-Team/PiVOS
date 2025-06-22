#ifndef KERNEL_MEMORY_MMU_H_
#define KERNEL_MEMORY_MMU_H_

#include <stdint.h>
struct mmu_phys_space_request {
    uint64_t virt_addr;
    uint32_t page_count;
    uint32_t align;
};

typedef void* (*memory_mmu_get_phys_space_callback)(struct mmu_phys_space_request* request);

void memory_mmu_init(void* (*pages_alloc)(uint32_t number, uint32_t align), void (*pages_dealloc)(void *addr, uint32_t number));

uint64_t* memory_mmu_create_table();

void memory_mmu_destroy_table(uint64_t* level0_table);

void* memory_mmu_va_to_pa(uint64_t* level0_table, void* va);

void memory_mmu_insert(uint64_t* mmu_table, uint8_t level, uint64_t address, uint64_t size, uint8_t memory_type, memory_mmu_get_phys_space_callback get_phys_space);

#endif  // KERNEL_MEMORY_MMU_H_