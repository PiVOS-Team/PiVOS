#ifndef KERNEL_MEMORY_MMU_IMPL_H_
#define KERNEL_MEMORY_MMU_IMPL_H_

#include <kernel/memory/mmu.h>
#include <kernel/memory/mmu/reg.h>
#include <kernel/utils.h>

struct mmu_entry_info {
    uint64_t* table;
    uint32_t idx;
    uint8_t level;
};

struct mmu_phys_space_request {
    uint64_t virt_addr;
    uint32_t page_count;
    uint32_t align;
};

typedef void* (*mmu_get_phys_space_callback)(struct mmu_phys_space_request* request);
typedef void (*mmu_create_memory_entry)(uint64_t* mmu_table, uint32_t idx, uint8_t level, uint64_t address, uint8_t memory_type);


uint64_t* mmu_create_table();

void mmu_destroy_table(uint64_t* mmu_table);

void mmu_create_table_entry(uint64_t* mmu_table, uint32_t idx, uint8_t level, uint64_t* next_level_table);

void mmu_create_block_entry(uint64_t* mmu_table, uint32_t idx, uint8_t level, uint64_t address, uint8_t memory_type);

void mmu_create_page_entry(uint64_t* mmu_table, uint32_t idx, uint8_t level, uint64_t address, uint8_t memory_type);

uint64_t* mmu_get_subtable(uint64_t* mmu_table, uint32_t idx, uint8_t level);

uint64_t* mmu_split_block_entry(uint64_t* mmu_table, uint32_t idx, uint8_t level);

struct mmu_entry_info mmu_traverse_and_alloc_tables(uint64_t* mmu_table, uint8_t level, uint64_t address, uint8_t destination_level);

struct mmu_entry_info mmu_find(uint64_t* mmu_table, uint8_t level, uint64_t address);

void mmu_remove(uint64_t* mmu_table, uint32_t idx, uint8_t level);

void mmu_insert(uint64_t* mmu_table, uint8_t level, uint64_t address, uint64_t size, uint8_t memory_type, mmu_get_phys_space_callback get_phys_space);

void mmu_map_units(uint64_t* mmu_table, uint8_t level, uint64_t address, uint8_t memory_type, uint32_t count, uint8_t destination_level, mmu_create_memory_entry create_memory_entry, mmu_get_phys_space_callback get_phys_space);


// Phys space claiming

void* mmu_get_phys_space_alloc(struct mmu_phys_space_request* request);

void* mmu_get_phys_space_one_to_one(struct mmu_phys_space_request* request);

static const mmu_get_phys_space_callback map_type_to_callback[2] = {
    mmu_get_phys_space_alloc,
    mmu_get_phys_space_one_to_one
};

static_assert(MMU_MEMORY_MAP_ALLOC == 0, "Enum values must match callbacks is array");
static_assert(MMU_MEMORY_MAP_ONE_TO_ONE == 1, "Enum values must match callbacks is array");

#endif  // KERNEL_MEMORY_MMU_IMPL_H_