.SECTION .boot, "ax"
.GLOBAL enable_mmu

#include <arch/arm64/mmu.h>

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
