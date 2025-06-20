.SECTION .boot, "ax"
.GLOBAL enable_mmu

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

#define TCR_VALUE (                                                     \
    TCR_T0SZ                                    << TCR_FIELD_T0SZ |     \
    TCR_IRGN_NORMAL_MEMORY_INNER_NON_CACHEABLE  << TCR_FIELD_IRGN0 |    \
    TCR_ORGN_NORMAL_MEMORY_OUTER_NON_CACHEABLE  << TCR_FIELD_ORGN0 |    \
    TCR_SH_NON_SHAREABLE                        << TCR_FIELD_SH0 |      \
    TCR_TG0_GRANULE_4KB                         << TCR_FIELD_TG0 |      \
    TCR_T1SZ                                    << TCR_FIELD_T1SZ |     \
    TCR_A1_TTBR0_DEFINE_ASID                    << TCR_FIELD_A1 |       \
    TCR_IRGN_NORMAL_MEMORY_INNER_NON_CACHEABLE  << TCR_FIELD_IRGN1 |    \
    TCR_ORGN_NORMAL_MEMORY_OUTER_NON_CACHEABLE  << TCR_FIELD_ORGN1 |    \
    TCR_SH_NON_SHAREABLE                        << TCR_FIELD_SH1 |      \
    TCR_TG1_GRANULE_4KB                         << TCR_FIELD_TG1 |      \
    TCR_IPS_32bit_4GB                           << TCR_FIELD_IPS |      \
    TCR_AS_8bit                                 << TCR_FIELD_AS         \
)

#define MAIR_VALUE (                                                                                            \
    MAIR_CREATE_MEMORY_PROFILE(                                                                                 \
        MAIR_MEMORY_NORMAL_NO_CACHE,                                                                            \
        MAIR_CREATE_ATTRIBUTE(MAIR_NORMAL_MEMORY_OUTER_NON_CACHEABLE, MAIR_NORMAL_MEMORY_INNER_NON_CACHEABLE)   \
    ) |                                                                                                         \
    MAIR_CREATE_MEMORY_PROFILE(                                                                                 \
        MAIR_MEMORY_DEVICE,                                                                                     \
        MAIR_CREATE_ATTRIBUTE(MAIR_DEVICE_MEMORY, MAIR_DEVICE_nGnRnE)                                           \
    )                                                                                                           \
)

#define MMU_TABLE_VALID_FLAG 0
#define MMU_TABLE_ACCESS_FLAG 10

#define MMU_TABLE_ATTR (            \
    1 << MMU_TABLE_VALID_FLAG |     \
    1 << MMU_TABLE_ACCESS_FLAG      \
)

enable_mmu:

prepare:
    ldr x25, =0x1000                    // page size
    ldr x26, =mmu_space_start
    and x26, x26, 0x0000FFFFFFFFFFFF    // x26 - kernel_level0_table
    add x27, x26, x25                   // x27 - kernel_level1_table
    add x28, x27, x25                   // x28 - kernel_level2_table
    add x29, x28, x25                   // x29 - kernel_level3_table

setup_tables:
    mov x0, x26
    mov x1, x27
    orr x1, x1, #3                      // valid = 1 and descriptor = 1 => table entry
    str x1, [x0]                        // level0_table[0] = level1_table

    mov x0, x27
    mov x1, x28
    orr x1, x1, #3                      // valid = 1 and descriptor = 1 => table entry
    str x1, [x0]                        // level1_table[0] = level2_table

    mov x0, 0                           // current block address
    ldr x1, =0x40000000                 // destination address (1GB)
    ldr x2, =0x200000                   // block size
    mov x3, x28                         // write address
    ldr x5, =MMU_TABLE_ATTR             

create_entries_loop:
    mov x4, x0                          // set output address
    orr x4, x4, x5                      // set lower block attributes and valid flag
    str x4, [x3], #8                    // level2_table[n] = x4
    add x0, x0, x2                      // next block address
    cmp x0, x1
    blo create_entries_loop

setup_registers:
    msr ttbr1_el1, x26                  // set kernel level0_table
    msr ttbr0_el1, x26                  // set user space level0_table to kernel level0_table

    ldr x0, =MAIR_VALUE                 // create memory profiles
    ldr x1, =TCR_VALUE                  // set mmu settings

    msr mair_el1, x0
    msr tcr_el1, x1

    mrs x0, sctlr_el1
    orr x0, x0, #1
    msr sctlr_el1, x0
    dsb ish
    isb

    ret
