#ifndef KERNEL_MEMORY_MMU_REG_H_
#define KERNEL_MEMORY_MMU_REG_H_

#include <kernel/utils.h>
#include <stdint.h>

#define MMU_TYPE_MASK 0b11
#define MMU_ENTRY_VALID_MASK 0b01
#define MMU_ENTRY_DESCRIPTOR_MASK 0b10

#define MMU_EMPTY_ENTRY 0b00
#define MMU_BLOCK_ENTRY 0b01
#define MMU_TABLE_ENTRY 0b11
#define MMU_PAGE_ENTRY 0b11

#if defined(ARM_MMU_GRANULE_SIZE_4KB)

#define MMU_GRANULE_SIZE (KB_IN_B * 4)

#define MMU_BLOCK_ENTRY_LEVEL1_OUTPUT_ADDRESS (48U - 18U)
#define MMU_BLOCK_ENTRY_LEVEL2_OUTPUT_ADDRESS (48U - 27U)
#define MMU_TABLE_ENTRY_NEXT_LEVEL_TABLE_ADDRESS (48U - 36U)
#define MMU_PAGE_ENTRY_OUTPUT_ADDRESS (48U - 36U)

#define MMU_ARE_BLOCK_ENTRIES_SUPPORTED(level) ((level) == 1 || (level) == 2)
union mmu_block_entry {
    uint64_t bits;
    struct __attribute__((packed)) {
        uint64_t valid                  : 1;
        uint64_t descriptor             : 1;
        uint64_t lower_block_attributes : 10;
        uint64_t RES0                   : 18;
        uint64_t output_address         : 18;
        uint64_t RES1                   : 4;
        uint64_t upper_block_attributes : 12;
    } fields_level_1;
    struct __attribute__((packed)) {
        uint64_t valid                  : 1;
        uint64_t descriptor             : 1;
        uint64_t lower_block_attributes : 10;
        uint64_t RES0                   : 9;
        uint64_t output_address         : 27;
        uint64_t RES1                   : 4;
        uint64_t upper_block_attributes : 12;
    } fields_level_2;
};
union mmu_page_entry {
    uint64_t bits;
    struct __attribute__((packed)) {
        uint64_t valid            : 1;
        uint64_t descriptor       : 1;
        uint64_t lower_attributes : 10;
        uint64_t output_address   : 36;
        uint64_t RES0             : 3;
        uint64_t upper_attributes : 13;
    } fields;
};

union mmu_memory_entry_common {
    uint64_t bits;
    struct __attribute__((packed)) {
        uint64_t valid            : 1;
        uint64_t descriptor       : 1;
        uint64_t lower_attributes : 10;
        uint64_t RES              : 52;
    } fields;
};

union mmu_table_entry {
    uint64_t bits;
    struct __attribute__((packed)) {
        uint64_t valid                    : 1;
        uint64_t descriptor               : 1;
        uint64_t IGNORED0                 : 10;
        uint64_t next_level_table_address : 36;
        uint64_t RES1                     : 4;
        uint64_t IGNORED1                 : 7;
        uint64_t RES2                     : 5;
    } fields;
};

static const uint64_t mmu_number_of_level_entries[4] = {512, 512, 512, 512};

static const uint64_t mmu_size_of_level_region[4] = {GB_IN_B * 512, GB_IN_B * 1,
                                                     MB_IN_B * 2, KB_IN_B * 4};

#elif defined(ARM_MMU_GRANULE_SIZE_16KB)
#define MMU_GRANULE_SIZE (KB_IN_B * 16)
#error "MMU: 16KB granule size is currently unsupported"
#elif defined(ARM_MMU_GRANULE_SIZE_64KB)
#define MMU_GRANULE_SIZE (KB_IN_B * 64)
#error "MMU: 64KB granule size is currently unsupported"
#else
#error "MMU: You must specify granule size"
#endif

#define MMU_PAGE_SIZE MMU_GRANULE_SIZE

union mmu_lower_attributes {
    uint16_t bits;
    struct __attribute__((packed)) {
        uint16_t AttrIndx : 3;
        uint16_t NS       : 1;
        uint16_t AP       : 2;
        uint16_t SH       : 2;
        uint16_t AF       : 1;
        uint16_t nG       : 1;
    } fields_stage_1;
};

union mmu_upper_attributes {
    uint16_t bits;
    struct __attribute__((packed)) {
        uint16_t DBM        : 1;
        uint16_t Contiguous : 1;
        uint16_t XN         : 2;
        uint16_t IGNORED    : 4;
        uint16_t PBHA       : 4;
    } fields_stage_1;
};

#endif  // KERNEL_MEMORY_MMU_REG_H_