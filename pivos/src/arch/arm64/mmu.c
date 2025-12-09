#include <arch/arm64/mmu.h>
#include <kernel/arch/mmu.h>
#include <kernel/utils.h>

typedef void (*mmu_create_mem_entry)(uint64_t* mmu_table, uint32_t idx, uint8_t level, uint64_t addr, uint8_t mem_type);

struct mmu_entry_info {
    uint64_t* table;
    uint32_t idx;
    uint8_t level;
};

static void* (*s_pages_alloc)(uint32_t pages, uint32_t align);
static void (*s_pages_dealloc)(void* addr, uint32_t number);

static inline uint8_t mmu_is_same_mem_type(uint64_t entry, uint8_t mem_type) {
    union mmu_mem_entry_common common = (union mmu_mem_entry_common)entry;
    return ((union mmu_lower_attributes)(uint16_t)common.fields.lower_attributes).fields_stage_1.AttrIndx == mem_type;
}

static inline void mmu_override_mem_type(uint64_t* mmu_table, uint32_t idx, uint8_t mem_type) {
    union mmu_mem_entry_common* entries = (union mmu_mem_entry_common*)mmu_table;
    union mmu_lower_attributes lower_attr = (union mmu_lower_attributes)(uint16_t)entries[idx].fields.lower_attributes;

    lower_attr.fields_stage_1.AttrIndx = mem_type;
    entries[idx].fields.lower_attributes = (uint64_t)lower_attr.bits;
}

static inline uint64_t mmu_get_addr_from_block_entry(uint64_t entry, uint8_t level) {
    if (level == 1) {
        return (uint64_t)((union mmu_block_entry)entry).fields_level_1.output_address << MMU_BLOCK_ENTRY_LEVEL1_OUTPUT_ADDRESS;
    } else if (level == 2) {
        return (uint64_t)((union mmu_block_entry)entry).fields_level_2.output_address << MMU_BLOCK_ENTRY_LEVEL2_OUTPUT_ADDRESS;
    }

    return 0;
}

static inline uint64_t mmu_get_addr_from_page_entry(uint64_t entry) {
    return ((union mmu_page_entry)entry).fields.output_address << MMU_PAGE_ENTRY_OUTPUT_ADDRESS;
}

static inline uint64_t mmu_get_next_level_table_from_table_entry(uint64_t entry) {
    return ((union mmu_table_entry)entry).fields.next_level_table_address << MMU_TABLE_ENTRY_NEXT_LEVEL_TABLE_ADDRESS;
}

static inline void mmu_destroy_page_entry(uint64_t entry) {
    s_pages_dealloc((void*)mmu_get_addr_from_page_entry(entry), 1);
}

static inline void mmu_destroy_block_entry(uint64_t entry, uint8_t level) {
    s_pages_dealloc((void*)mmu_get_addr_from_block_entry(entry, level), mmu_size_of_level_region[level] / MMU_PAGE_SIZE);
}

static inline void mmu_dealloc_table(uint64_t* mmu_table) {
    s_pages_dealloc((void*)mmu_table, 1);
}

static void mmu_create_page_entry(uint64_t* mmu_table, uint32_t idx, uint8_t level, uint64_t addr, uint8_t mem_type) {
    ASSERT(HIGH, level == 3, "Only level 3 can have page entries");

    union mmu_lower_attributes lower_attributes = {.fields_stage_1 = {.AttrIndx = mem_type, .AF = 1, .AP = 0b01}};

    union mmu_page_entry new_page_entry = {
        .fields = {
            .valid = 1,
            .descriptor = 1,
            .lower_attributes = lower_attributes.bits,
            .output_address = addr >> MMU_PAGE_ENTRY_OUTPUT_ADDRESS,
        }};

    mmu_table[idx] = new_page_entry.bits;
}

static void mmu_create_block_entry(uint64_t* mmu_table, uint32_t idx, uint8_t level, uint64_t addr, uint8_t mem_type) {
    ASSERT(HIGH, MMU_ARE_BLOCK_ENTRIES_SUPPORTED(level), "Given level cannot have block entries");

    union mmu_block_entry new_block_entry;
    union mmu_lower_attributes lower_attributes = {.fields_stage_1 = {.AttrIndx = mem_type, .AF = 1, .AP = 0b01}};

    // This part is specific for 4KB granule size
    // other granule types may allow different levels to have block entries
    // see MMU_ARE_BLOCK_ENTRIES_SUPPORTED macro

    if (level == 1) {
        new_block_entry = (union mmu_block_entry){
            .fields_level_1 = {
                .valid = 1,
                .descriptor = 0,
                .lower_block_attributes = lower_attributes.bits,
                .output_address = addr >> MMU_BLOCK_ENTRY_LEVEL1_OUTPUT_ADDRESS,
            }};
    } else {
        new_block_entry = (union mmu_block_entry){
            .fields_level_2 = {
                .valid = 1,
                .descriptor = 0,
                .lower_block_attributes = lower_attributes.bits,
                .output_address = addr >> MMU_BLOCK_ENTRY_LEVEL2_OUTPUT_ADDRESS,
            }};
    }

    mmu_table[idx] = new_block_entry.bits;
}

static void mmu_create_table_entry(uint64_t* mmu_table, uint32_t idx, uint8_t level, uint64_t* next_level_table) {
    ASSERT(HIGH, level != 3, "Level 3 cannot have table entries");

    union mmu_table_entry new_table_entry = {
        .fields = {
            .valid = 1,
            .descriptor = 1,
            .next_level_table_address = (uint64_t)next_level_table >> MMU_TABLE_ENTRY_NEXT_LEVEL_TABLE_ADDRESS,
        }};

    mmu_table[idx] = new_table_entry.bits;
}

static uint64_t* mmu_get_subtable(uint64_t* mmu_table, uint32_t idx, uint8_t level) {
    ASSERT(HIGH, level != 3, "Level 3 cannot have table entries");
    uint64_t entry = mmu_table[idx];
    ASSERT(HIGH, (entry & MMU_TYPE_MASK) == MMU_TABLE_ENTRY, "Given entry is not a table entry");

    // Address is | with KERNEL_SPACE_MASK to make it accessible from kernel
    return (uint64_t*)(mmu_get_next_level_table_from_table_entry(entry) | KERNEL_SPACE_MASK);
}

static uint64_t* mmu_split_block_entry(uint64_t* mmu_table, uint32_t idx, uint8_t level) {
    uint64_t* temp_table = arch_mmu_create_table();

    union mmu_block_entry entry = (union mmu_block_entry)mmu_table[idx];
    uint8_t next_level = level + 1;
    uint8_t mem_type = 0;
    uint64_t new_table_base_addr = 0;
    uint64_t unit_size = mmu_size_of_level_region[next_level];

    if (level == 1) {
        mem_type = ((union mmu_lower_attributes)(uint16_t)entry.fields_level_1.lower_block_attributes).fields_stage_1.AttrIndx;
        new_table_base_addr = entry.fields_level_1.output_address << MMU_BLOCK_ENTRY_LEVEL1_OUTPUT_ADDRESS;
    } else if (level == 2) {
        mem_type = ((union mmu_lower_attributes)(uint16_t)entry.fields_level_2.lower_block_attributes).fields_stage_1.AttrIndx;
        new_table_base_addr = entry.fields_level_2.output_address << MMU_BLOCK_ENTRY_LEVEL2_OUTPUT_ADDRESS;
    }

    mmu_create_table_entry(mmu_table, idx, level, temp_table);

    if (MMU_ARE_BLOCK_ENTRIES_SUPPORTED(next_level)) {
        for (uint32_t unit_number = 0; unit_number < mmu_number_of_level_entries[next_level]; unit_number++) {
            mmu_create_block_entry(temp_table, unit_number, next_level, new_table_base_addr + unit_number * unit_size, mem_type);
        }
    } else {
        for (uint32_t unit_number = 0; unit_number < mmu_number_of_level_entries[next_level]; unit_number++) {
            mmu_create_page_entry(temp_table, unit_number, next_level, new_table_base_addr + unit_number * unit_size, mem_type);
        }
    }

    return temp_table;
}

static struct mmu_entry_info mmu_traverse_and_alloc_tables(uint64_t* mmu_table, uint8_t level, uint64_t addr, uint8_t destination_level) {
    uint64_t* temp_table = mmu_table;

    uint8_t current_level = level;
    uint64_t current_addr = addr;
    uint32_t current_level_idx = 0;

    // This loop traverse through tables to reach destination level
    for (; current_level < destination_level; current_level++) {
        current_level_idx = current_addr / mmu_size_of_level_region[current_level];
        current_addr %= mmu_size_of_level_region[current_level];
        uint64_t entry = temp_table[current_level_idx];
        uint8_t type = entry & MMU_TYPE_MASK;

        if (!(type & MMU_ENTRY_VALID_MASK)) {
            // Empty or invalid - we need to alloc new table

            uint64_t* new_table = arch_mmu_create_table();

            mmu_create_table_entry(temp_table, current_level_idx, current_level, new_table);
            temp_table = new_table;
        } else {
            if (type & MMU_ENTRY_DESCRIPTOR_MASK) {
                // Table entry - we need to get subtable
                // This will never be page entry

                temp_table = mmu_get_subtable(temp_table, current_level_idx, current_level);
            } else {
                // Block entry
                // This can appear only in case if we want to create unit in a
                // higher level table So to handle this, we have to split this
                // block into smaller units

                temp_table = mmu_split_block_entry(temp_table, current_level_idx, current_level);
            }
        }
    }

    return (struct mmu_entry_info){
        .table = temp_table,
        .idx = current_addr / mmu_size_of_level_region[current_level],
        .level = current_level};
}

struct mmu_entry_info mmu_find(uint64_t* mmu_table, uint8_t level, uint64_t addr) {
    uint64_t* temp_table = mmu_table;

    uint8_t current_level = level;
    uint64_t current_addr = addr;
    uint32_t current_level_idx = 0;

    // This loop traverse through tables to reach destination level
    for (; current_level < 3; current_level++) {
        current_level_idx = current_addr / mmu_size_of_level_region[current_level];
        current_addr %= mmu_size_of_level_region[current_level];
        uint64_t entry = temp_table[current_level_idx];

        if ((entry & MMU_TYPE_MASK) == MMU_TABLE_ENTRY) {
            temp_table = mmu_get_subtable(temp_table, current_level_idx, current_level);
        } else {
            // If current entry is empty / block / page - stop
            break;
        }
    }

    return (struct mmu_entry_info){
        .table = temp_table,
        .idx = current_addr / mmu_size_of_level_region[current_level],
        .level = current_level,
    };
}

static void mmu_destroy_entry(uint64_t* mmu_table, uint32_t idx, uint8_t level) {
    // If given node is located in level 3 table or is a block entry
    // we can simply destroy it without any tree traversing

    if (level == 3) {
        mmu_destroy_page_entry(mmu_table[idx]);
        mmu_table[idx] = MMU_EMPTY_ENTRY;
        return;
    } else if (MMU_ARE_BLOCK_ENTRIES_SUPPORTED(level) && ((mmu_table[idx] & MMU_TYPE_MASK) == MMU_BLOCK_ENTRY)) {
        mmu_destroy_block_entry(mmu_table[idx], level);
        mmu_table[idx] = MMU_EMPTY_ENTRY;
        return;
    }

    static struct mmu_entry_info table_stack[4];

    uint8_t table_stack_depth = 1;
    table_stack[0] = (struct mmu_entry_info){
        .table = mmu_get_subtable(mmu_table, idx, level),
        .idx = 0,
        .level = level + 1,
    };

    struct mmu_entry_info* current_entry = &table_stack[0];
    while (table_stack_depth > 0) {
        uint64_t entry = current_entry->table[current_entry->idx];
        uint8_t valid = entry & MMU_ENTRY_VALID_MASK;
        uint8_t descriptor = entry & MMU_ENTRY_DESCRIPTOR_MASK;

        if (!valid) {
            // This entry is empty or invalid - skip
            current_entry->idx++;
        } else {
            if (descriptor) {
                if (current_entry->level == 3) {
                    // Page entry
                    mmu_destroy_page_entry(entry);
                    current_entry->idx++;
                } else {
                    // Table entry
                    uint64_t* subtable = mmu_get_subtable(current_entry->table, current_entry->idx, current_entry->level);

                    table_stack[table_stack_depth] = (struct mmu_entry_info){
                        .table = subtable,
                        .idx = 0,
                        .level = current_entry->level + 1,
                    };

                    table_stack_depth++;
                    current_entry->idx++;
                    current_entry = &table_stack[table_stack_depth - 1];
                }
            } else {
                // Block entry
                mmu_destroy_block_entry(entry, current_entry->level);
                current_entry->idx++;
            }
        }

        if (current_entry->idx == mmu_number_of_level_entries[current_entry->level]) {
            mmu_dealloc_table(current_entry->table);
            table_stack_depth--;
            current_entry = &table_stack[table_stack_depth - 1];
        }
    }

    mmu_table[idx] = MMU_EMPTY_ENTRY;
}

static void mmu_map_units(uint64_t* mmu_table, uint8_t level, uint64_t addr, uint8_t mem_type, uint32_t count, uint8_t destination_level, mmu_create_mem_entry create_mem_entry, void* (*get_phys_space)(struct arch_mmu_space_req* req)) {
    uint64_t offset = 0;
    uint64_t unit_size = mmu_size_of_level_region[destination_level];
    uint32_t pages_for_unit = unit_size / MMU_PAGE_SIZE;
    uint64_t entry = 0;

    // align = page_count because if we alloc block, phys addr
    // must be also aligned to it's size
    struct arch_mmu_space_req phys_space_request = {.page_count = pages_for_unit, .align = pages_for_unit};

    while (count > 0) {
        struct mmu_entry_info table_data = mmu_traverse_and_alloc_tables(mmu_table, level, addr + offset, destination_level);
        uint32_t unit_count = MIN(mmu_number_of_level_entries[destination_level] - table_data.idx, count);
        uint64_t requested_addr = addr + offset;

        for (uint32_t unit_number = table_data.idx; unit_number < unit_count + table_data.idx; unit_number++) {
            entry = table_data.table[unit_number];

            if (entry != MMU_EMPTY_ENTRY) {
                // We are trying to alloc block entry but this entry already
                // points to level 3 table there are two possible solutions:
                // 1. merge entries of lvl3 table and create block (only if
                // output addres are continous)
                // 2. insted of creating block entry -> create multiple page
                // entries
                if (table_data.level != 3 && (entry & MMU_TYPE_MASK) == MMU_TABLE_ENTRY) {
                    uint64_t* pages = mmu_get_subtable(table_data.table, table_data.idx, table_data.level);

                    struct arch_mmu_space_req request_page = {
                        .page_count = 1,
                        .align = 1,
                        .virt_addr = requested_addr,
                    };

                    uint64_t l3_entry = 0;
                    for (uint32_t l3_idx = 0; l3_idx < mmu_number_of_level_entries[3]; l3_idx++) {
                        l3_entry = pages[l3_idx];
                        if ((l3_entry & MMU_TYPE_MASK) == MMU_EMPTY_ENTRY) {
                            mmu_create_page_entry(pages, l3_idx, 3, (uint64_t)get_phys_space(&request_page), mem_type);
                        } else if (!mmu_is_same_mem_type(l3_entry, mem_type)) {
                            mmu_override_mem_type(pages, l3_idx, mem_type);
                        }

                        request_page.virt_addr += MMU_PAGE_SIZE;
                    }
                } else if (!mmu_is_same_mem_type(entry, mem_type)) {
                    // If entry already exists and has different mem type -
                    // override it
                    mmu_override_mem_type(table_data.table, unit_number, mem_type);
                }
            } else {
                phys_space_request.virt_addr = requested_addr;
                create_mem_entry(table_data.table, unit_number, destination_level, (uint64_t)get_phys_space(&phys_space_request), mem_type);
            }

            requested_addr += unit_size;
        }

        offset += unit_count * unit_size;
        count -= unit_count;
    }
}

static void mmu_insert(uint64_t* mmu_table, uint8_t level, uint64_t addr, uint64_t size, uint8_t mem_type, void* (*get_phys_space)(struct arch_mmu_space_req* req)) {
    // Every type of granulation allows block entries at level 2
    // Other levels of block entries are currently unsupported (for example
    // level 1 for 4KB granule)

    uint64_t block_size = mmu_size_of_level_region[2];
    uint64_t block_addr = ALIGN_UP(addr, block_size);

    if (size >= block_addr - addr + block_size) {
        uint64_t offset = 0;

        // Alloc pages until reach desired size or allign to alloc blocks
        uint32_t count_temp = MIN(size, block_addr - addr);
        if (count_temp > 0) {
            mmu_map_units(mmu_table, level, addr, mem_type, count_temp, 3, mmu_create_page_entry, get_phys_space);
            offset += count_temp * MMU_PAGE_SIZE;
        }

        // Alloc as many as possible blocks
        count_temp = (size - offset) / block_size;
        if (count_temp > 0) {
            mmu_map_units(mmu_table, level, addr + offset, mem_type, count_temp, 2, mmu_create_block_entry, get_phys_space);
            offset += count_temp * block_size;
        }

        // Alloc another pages to fill the gap after blocks
        count_temp = (size - offset) / MMU_PAGE_SIZE;
        if (count_temp > 0) {
            mmu_map_units(mmu_table, level, addr + offset, mem_type, count_temp, 3, mmu_create_page_entry, get_phys_space);
        }
    } else {
        mmu_map_units(mmu_table, level, addr, mem_type, size / MMU_PAGE_SIZE, 3, mmu_create_page_entry, get_phys_space);
    }
}

void arch_mmu_init(void* (*page_alloc)(uint32_t number, uint32_t allign), void (*page_dealloc)(void* addr, uint32_t number)) {
    s_pages_alloc = page_alloc;
    s_pages_dealloc = page_dealloc;
}

void arch_mmu_insert(void* table, uint64_t addr, uint64_t size, uint8_t mem_type, void* (*get_phys_space)(struct arch_mmu_space_req* req)) {
    mmu_insert((uint64_t*)table, 0, addr, size, mem_type, get_phys_space);
}

void* arch_mmu_va_to_pa(void* table, void* va) {
    uint64_t* level0_table = (uint64_t*)table;

    struct mmu_entry_info search_result = mmu_find(level0_table, 0, (uint64_t)va);
    uint64_t entry = search_result.table[search_result.idx];
    uint8_t type = entry & MMU_TYPE_MASK;

    if (type & MMU_PAGE_ENTRY) {
        return (void*)((mmu_get_addr_from_page_entry(entry) + ((uint64_t)va % MMU_PAGE_SIZE)) | KERNEL_SPACE_MASK);
    } else if (type & MMU_BLOCK_ENTRY) {
        return (void*)((mmu_get_addr_from_block_entry(entry, search_result.level) + ((uint64_t)va % mmu_size_of_level_region[search_result.level])) | KERNEL_SPACE_MASK);
    }

    return NULL;
}

void* arch_mmu_create_table() {
    // This addr is | with KERNEL_SPACE_MASK to make it accessible from kernel
    // 16 most significant bits are ignored when creating table entries

    uint64_t* new_table_addr = (uint64_t*)((uint64_t)s_pages_alloc(1, 1) | KERNEL_SPACE_MASK);
    byteset((uint8_t*)new_table_addr, 0, MMU_PAGE_SIZE);
    return new_table_addr;
}

void arch_mmu_destroy_table(void* table) {
    uint64_t* level0_table = (uint64_t*)table;

    for (uint32_t idx = 0; idx < mmu_number_of_level_entries[0]; idx++) {
        if (level0_table[idx] != MMU_EMPTY_ENTRY) {
            mmu_destroy_entry(level0_table, idx, 0);
        }
    }

    s_pages_dealloc(level0_table, 1);
}

void arch_mmu_change_usr_ctx(void* table, uint16_t id) {
    uint64_t mask = (uint64_t)id << (64 - 16);  // 16 - ASID SIZE
    uint64_t addr = ((uint64_t)table & ADDRESS_MASK) | mask;

    __asm__ volatile(
        "msr ttbr0_el1, %[tab]\n"
        "tlbi vmalle1\n"
        "dsb ish\n"
        "isb\n"
        :
        : [tab] "r"(addr));
}

void* arch_mmu_get_usr_ctx() {
    void* addr = NULL;

    __asm__ volatile("mrs %[tab], ttbr0_el1" : [tab] "=r"(addr));
    return addr;
}