#ifndef KERNEL_MEMORY_MMU_H_
#define KERNEL_MEMORY_MMU_H_

#include <stdint.h>

struct memory_mmu_context {
    uint64_t* level0_table;
};


// !!! CAUTION !!!
// DO NOT CHANGE THIS ORDER
// These values are set in MAIR_EL1 register during boot stage
enum memory_mmu_region_type { 
    MMU_MEMORY_TYPE_NORMAL_NO_CACHE = 0, 
    MMU_MEMORY_TYPE_DEVICE = 1 
};

enum memory_mmu_map_type {
    MMU_MEMORY_MAP_ALLOC,
    MMU_MEMORY_MAP_ONE_TO_ONE
};

struct memory_mmu_region {
    enum memory_mmu_region_type memory_type;
    uint64_t va;
    uint64_t pa;
    uint64_t size;
};

void memory_mmu_init(void* (*pages_alloc)(uint32_t number, uint32_t align), void (*pages_dealloc)(void *addr, uint32_t number));

void memory_mmu_create_context(struct memory_mmu_context* context);

void memory_mmu_destroy_context(struct memory_mmu_context* context);

void* memory_mmu_va_to_pa(struct memory_mmu_context* context, void* va);

void memory_mmu_map_region(struct memory_mmu_context* context, struct memory_mmu_region* region, enum memory_mmu_map_type map_type);

#endif  // KERNEL_MEMORY_MMU_H_