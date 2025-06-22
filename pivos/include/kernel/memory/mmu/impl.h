#ifndef KERNEL_MEMORY_MMU_IMPL_H_
#define KERNEL_MEMORY_MMU_IMPL_H_

#include <kernel/memory/mmu.h>
#include <kernel/memory/mmu/reg.h>
#include <kernel/utils.h>

typedef void (*mmu_create_memory_entry)(uint64_t* mmu_table, uint32_t idx,
                                        uint8_t level, uint64_t address,
                                        uint8_t memory_type);

struct mmu_entry_info {
    uint64_t* table;
    uint32_t idx;
    uint8_t level;
};

void mmu_dealloc_table(uint64_t* mmu_table);

void mmu_create_table_entry(uint64_t* mmu_table, uint32_t idx, uint8_t level,
                            uint64_t* next_level_table);

void mmu_create_block_entry(uint64_t* mmu_table, uint32_t idx, uint8_t level,
                            uint64_t address, uint8_t memory_type);

void mmu_create_page_entry(uint64_t* mmu_table, uint32_t idx, uint8_t level,
                           uint64_t address, uint8_t memory_type);

uint64_t* mmu_get_subtable(uint64_t* mmu_table, uint32_t idx, uint8_t level);

uint64_t* mmu_split_block_entry(uint64_t* mmu_table, uint32_t idx,
                                uint8_t level);

struct mmu_entry_info mmu_traverse_and_alloc_tables(uint64_t* mmu_table,
                                                    uint8_t level,
                                                    uint64_t address,
                                                    uint8_t destination_level);

struct mmu_entry_info mmu_find(uint64_t* mmu_table, uint8_t level,
                               uint64_t address);

void mmu_destroy_entry(uint64_t* mmu_table, uint32_t idx, uint8_t level);

void mmu_map_units(uint64_t* mmu_table, uint8_t level, uint64_t address,
                   uint8_t memory_type, uint32_t count,
                   uint8_t destination_level,
                   mmu_create_memory_entry create_memory_entry,
                   memory_mmu_get_phys_space_callback get_phys_space);

#endif  // KERNEL_MEMORY_MMU_IMPL_H_