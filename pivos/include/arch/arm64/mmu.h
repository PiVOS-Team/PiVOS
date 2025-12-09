#ifndef ARCH_ARM64_MMU_H_
#define ARCH_ARM64_MMU_H_

#define ARM64_MMU_GRANULE_SIZE_4KB

//------------------------------------------- MAIR_EL1 -------------------------------------------

#define MAIR_MEMORY_NORMAL_NO_CACHE 0
#define MAIR_MEMORY_DEVICE 1

#define MAIR_CREATE_ATTRIBUTE(upper, lower) (((upper) << 4) | (lower))
#define MAIR_CREATE_MEMORY_PROFILE(index, attribute) ((attribute) << ((index) * 8))
//-------------------------------------------
// Attr<n>[7:4]
// R - Outer Read-Allocate policy
// W - Outer Write-Allocate policy
//-------------------------------------------

#define MAIR_DEVICE_MEMORY 0b0000
#define MAIR_NORMAL_MEMORY_OUTER_WRITE_THROUGH_TRANSIENT(R, W) 0b00##R##W
#define MAIR_NORMAL_MEMORY_OUTER_NON_CACHEABLE 0b0100
#define MAIR_NORMAL_MEMORY_OUTER_WRITE_BACK_TRANSIENT(R, W) 0b01##R##W
#define MAIR_NORMAL_MEMORY_OUTER_WRITE_THROUGH_NON_TRANSIENT(R, W) 0b10##R##W
#define MAIR_NORMAL_MEMORY_OUTER_WRITE_BACK_NON_TRANSIENT(R, W) 0b11##R##W

//-------------------------------------------
// Attr<n>[3:0] when Attr<n>[7:4] is 0000
// (n)G - (non)-Gathering
// (n)R - (non)-Reordering
// (n)E - (no) Early Write Acknowledgement
//-------------------------------------------

#define MAIR_DEVICE_nGnRnE 0b0000
#define MAIR_DEVICE_nGnRE 0b0100
#define MAIR_DEVICE_nGRE 0b1000
#define MAIR_DEVICE_GRE 0b1100

//-------------------------------------------
// Attr<n>[3:0] when Attr<n>[7:4] is not 0000
// R - Inner Read-Allocate policy
// W - Inner Write-Allocate policy
//-------------------------------------------

#define MAIR_NORMAL_MEMORY_INNER_WRITE_THROUGH_TRANSIENT(R, W) 0b00##R##W
#define MAIR_NORMAL_MEMORY_INNER_NON_CACHEABLE 0b0100
#define MAIR_NORMAL_MEMORY_INNER_WRITE_BACK_TRANSIENT(R, W) 0b01##R##W
#define MAIR_NORMAL_MEMORY_INNER_WRITE_THROUGH_NON_TRANSIENT(R, W) 0b10##R##W
#define MAIR_NORMAL_MEMORY_INNER_WRITE_BACK_NON_TRANSIENT(R, W) 0b11##R##W

//------------------------------------------- TCR_EL1 -------------------------------------------

#define TCR_FIELD_T0SZ 0
#define TCR_FIELD_IRGN0 8
#define TCR_FIELD_ORGN0 10
#define TCR_FIELD_SH0 12
#define TCR_FIELD_TG0 14
#define TCR_FIELD_T1SZ 16
#define TCR_FIELD_A1 22
#define TCR_FIELD_IRGN1 24
#define TCR_FIELD_ORGN1 26
#define TCR_FIELD_SH1 28
#define TCR_FIELD_TG1 30
#define TCR_FIELD_IPS 32
#define TCR_FIELD_AS 36

#define TCR_IRGN_NORMAL_MEMORY_INNER_NON_CACHEABLE 0b00
#define TCR_IRGN_NORMAL_MEMORY_INNER_WRITE_BACK_READ_ALLOCATE_WRITE_ALLOCATE_CACHEABLE 0b01
#define TCR_IRGN_NORMAL_MEMORY_INNER_WRITE_THROUGH_READ_ALLOCATE_NO_WRITE_ALLOCATE_CACHEABLE 0b10
#define TCR_IRGN_NORMAL_MEMORY_INNER_WRITE_BACK_READ_ALLOCATE_NO_WRITE_ALLOCATE_CACHEABLE 0b11

#define TCR_ORGN_NORMAL_MEMORY_OUTER_NON_CACHEABLE 0b00
#define TCR_ORGN_NORMAL_MEMORY_OUTER_WRITE_BACK_READ_ALLOCATE_WRITE_ALLOCATE_CACHEABLE 0b01
#define TCR_ORGN_NORMAL_MEMORY_OUTER_WRITE_THROUGH_READ_ALLOCATE_NO_WRITE_ALLOCATE_CACHEABLE 0b10
#define TCR_ORGN_NORMAL_MEMORY_OUTER_WRITE_BACK_READ_ALLOCATE_NO_WRITE_ALLOCATE_CACHEABLE 0b11

#define TCR_A1_TTBR0_DEFINE_ASID 0
#define TCR_A1_TTBR1_DEFINE_ASID 1

#define TCR_SH_NON_SHAREABLE 0b00
#define TCR_SH_OUTER_SHAREABLE 0b10
#define TCR_SH_INNER_SHAREABLE 0b11

#define TCR_TG0_GRANULE_4KB 0b00
#define TCR_TG0_GRANULE_16KB 0b10
#define TCR_TG0_GRANULE_64KB 0b01

#define TCR_TG1_GRANULE_4KB 0b10
#define TCR_TG1_GRANULE_16KB 0b01
#define TCR_TG1_GRANULE_64KB 0b11

#define TCR_T0SZ 16
#define TCR_T1SZ 16

#define TCR_IPS_32bit_4GB 0b000
#define TCR_IPS_36bit_64GB 0b001
#define TCR_IPS_40bit_1TB 0b010
#define TCR_IPS_42bit_4TB 0b011
#define TCR_IPS_44bit_16TB 0b100
#define TCR_IPS_48bit_256TB 0b101
#define TCR_IPS_52bit_4PB 0b110

#define TCR_AS_8bit 0
#define TCR_AS_16bit 1

#define TCR_VALUE (                                                 \
    TCR_T0SZ << TCR_FIELD_T0SZ |                                    \
    TCR_IRGN_NORMAL_MEMORY_INNER_NON_CACHEABLE << TCR_FIELD_IRGN0 | \
    TCR_ORGN_NORMAL_MEMORY_OUTER_NON_CACHEABLE << TCR_FIELD_ORGN0 | \
    TCR_SH_NON_SHAREABLE << TCR_FIELD_SH0 |                         \
    TCR_TG0_GRANULE_4KB << TCR_FIELD_TG0 |                          \
    TCR_T1SZ << TCR_FIELD_T1SZ |                                    \
    TCR_A1_TTBR0_DEFINE_ASID << TCR_FIELD_A1 |                      \
    TCR_IRGN_NORMAL_MEMORY_INNER_NON_CACHEABLE << TCR_FIELD_IRGN1 | \
    TCR_ORGN_NORMAL_MEMORY_OUTER_NON_CACHEABLE << TCR_FIELD_ORGN1 | \
    TCR_SH_NON_SHAREABLE << TCR_FIELD_SH1 |                         \
    TCR_TG1_GRANULE_4KB << TCR_FIELD_TG1 |                          \
    TCR_IPS_32bit_4GB << TCR_FIELD_IPS |                            \
    TCR_AS_16bit << TCR_FIELD_AS)

#define MAIR_VALUE (                                                                                             \
    MAIR_CREATE_MEMORY_PROFILE(                                                                                  \
        MAIR_MEMORY_NORMAL_NO_CACHE,                                                                             \
        MAIR_CREATE_ATTRIBUTE(MAIR_NORMAL_MEMORY_OUTER_NON_CACHEABLE, MAIR_NORMAL_MEMORY_INNER_NON_CACHEABLE)) | \
    MAIR_CREATE_MEMORY_PROFILE(                                                                                  \
        MAIR_MEMORY_DEVICE,                                                                                      \
        MAIR_CREATE_ATTRIBUTE(MAIR_DEVICE_MEMORY, MAIR_DEVICE_nGnRnE)))

#define MMU_TABLE_VALID_FLAG 0
#define MMU_TABLE_ACCESS_FLAG 10

#define MMU_TABLE_ATTR (        \
    1 << MMU_TABLE_VALID_FLAG | \
    1 << MMU_TABLE_ACCESS_FLAG)

#define MMU_TYPE_MASK 0b11
#define MMU_ENTRY_VALID_MASK 0b01
#define MMU_ENTRY_DESCRIPTOR_MASK 0b10

#define MMU_EMPTY_ENTRY 0b00
#define MMU_BLOCK_ENTRY 0b01
#define MMU_TABLE_ENTRY 0b11
#define MMU_PAGE_ENTRY 0b11

#ifndef __ASSEMBLER__

#include <kernel/arch/mmu.h>
#include <kernel/utils.h>
#include <stdint.h>

#if MMU_PAGE_SIZE == (KB_IN_B * 4)

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

union mmu_mem_entry_common {
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

static const uint64_t mmu_size_of_level_region[4] = {GB_IN_B * 512, GB_IN_B * 1, MB_IN_B * 2, KB_IN_B * 4};

#elif MMU_PAGE_SIZE == (KB_IN_B * 16)
#error "MMU: 16KB page size is currently unsupported"
#elif MMU_PAGE_SIZE == (KB_IN_B * 64)
#error "MMU: 64KB page size is currently unsupported"
#else
#error "MMU: Specified page size is unsupported by ARM64 architecture"
#endif
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

#endif

#endif