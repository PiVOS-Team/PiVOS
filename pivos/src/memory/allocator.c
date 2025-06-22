#include <kernel/memory/allocator.h>
#include <kernel/utils.h>
#include <stddef.h>

#define PAGE_FULL (~0ULL)
#define PAGES_PER_ENTRY 64  // NOLINT

static uint64_t* s_bitmap;
static uint64_t s_bitmap_entry_count;
static uint64_t s_page_size;

static inline uint64_t memory_allocator_convert_to_mask(uint32_t number,
                                                        uint32_t offset) {
    return (~0ULL << (PAGES_PER_ENTRY - number)) >> offset;  // NOLINT
}

static inline uint32_t memory_allocator_get_page_number(uint64_t addr) {
    return (addr & ~KERNEL_SPACE_MASK) / s_page_size;
}

static inline uint64_t memory_allocator_get_addres(uint32_t page_number) {
    return page_number * s_page_size;
}

static inline void memory_allocator_claim_specific_page(uint32_t page_number) {
    uint64_t mask =
        memory_allocator_convert_to_mask(1, page_number % PAGES_PER_ENTRY);
    s_bitmap[page_number / PAGES_PER_ENTRY] |= mask;
}

static inline void memory_allocator_free_specific_page(uint32_t page_number) {
    uint64_t mask =
        ~memory_allocator_convert_to_mask(1, page_number % PAGES_PER_ENTRY);
    s_bitmap[page_number / PAGES_PER_ENTRY] &= mask;
}

static void memory_allocator_claim_multiple_pages(uint32_t page_number,
                                                  uint32_t count) {
    uint32_t entry = page_number / PAGES_PER_ENTRY;
    uint32_t offset = page_number % PAGES_PER_ENTRY;
    uint32_t pages_to_claim = MIN((PAGES_PER_ENTRY - offset), count);

    while (count > 0) {
        uint64_t mask =
            memory_allocator_convert_to_mask(pages_to_claim, offset);
        s_bitmap[entry] |= mask;
        entry++;
        count -= pages_to_claim;
        pages_to_claim = MIN(PAGES_PER_ENTRY, count);
        offset = 0;
    }
}

static void memory_allocator_free_multiple_pages(uint32_t page_number,
                                                 uint32_t count) {
    uint32_t entry = page_number / PAGES_PER_ENTRY;
    uint32_t offset = page_number % PAGES_PER_ENTRY;
    uint32_t pages_to_free = MIN((PAGES_PER_ENTRY - offset), count);

    while (count > 0) {
        uint64_t mask =
            ~memory_allocator_convert_to_mask(pages_to_free, offset);
        s_bitmap[entry] &= mask;
        entry++;
        count -= pages_to_free;
        pages_to_free = MIN(PAGES_PER_ENTRY, count);
    }
}

// allocator.h

void memory_allocator_init(void* addr, uint32_t number_of_pages,
                           uint32_t page_size) {
    ASSERT(LOW, (uint64_t)addr % page_size == 0,
           "Bitmap addr must be aligned to page size");

    s_page_size = page_size;
    s_bitmap = (uint64_t*)addr;
    s_bitmap_entry_count = number_of_pages / PAGES_PER_ENTRY;
    byteset((uint8_t*)s_bitmap, 0, s_bitmap_entry_count * sizeof(uint64_t));

    // Claim bitmap region in bitmap
    uint64_t bitmap_end =
        (uint64_t)s_bitmap + s_bitmap_entry_count * sizeof(uint64_t);
    memory_allocator_claim_range(s_bitmap,
                                 (void*)ALIGN_UP(bitmap_end, s_page_size));
}

void* memory_allocator_claim_page() {
    uint64_t current_entry = 0;
    for (uint64_t entry_idx = 0; entry_idx < s_bitmap_entry_count;
         entry_idx++) {
        current_entry = s_bitmap[entry_idx];

        // This is necessary because result
        // of __builtin_clzll(0) is undefined

        if (current_entry == PAGE_FULL) {
            continue;
        }

        // We are looking for the first one from left - decrease segmentation.
        // After negation all claimed pages are represented by 0
        // __builtin_clzll return number of leading zeros in nubmer
        // so the number will be index of first free page

        uint32_t pos = __builtin_clzll(~current_entry);
        uint32_t page_number = entry_idx * PAGES_PER_ENTRY + pos;

        memory_allocator_claim_specific_page(page_number);
        return (void*)(page_number * s_page_size);
    }

    return NULL;
}

void memory_allocator_claim_range(void* from, void* to) {
    ASSERT(HIGH,
           (uint64_t)from % s_page_size == 0 && (uint64_t)to % s_page_size == 0,
           "Addres must be aligned");

    uint32_t from_page = memory_allocator_get_page_number((uint64_t)from);
    uint32_t to_page = memory_allocator_get_page_number((uint64_t)to);
    uint32_t pages_to_claim = to_page - from_page;

    if (pages_to_claim == 0) {
        pages_to_claim++;
    }

    memory_allocator_claim_multiple_pages(from_page, pages_to_claim);
}

void* memory_allocator_claim_pages(uint32_t number, uint32_t align) {
    if (number == 1 && align == 1) {
        return memory_allocator_claim_page();
    }

    uint32_t current_entry = 0;
    uint64_t current_mask = 0;

    uint32_t current_page = 0;
    uint32_t offset = 0;
    uint32_t pages_left = number;
    uint32_t entry_idx = 0;

    while (pages_left > 0 && entry_idx < s_bitmap_entry_count) {
        uint32_t pages_to_claim = MIN(PAGES_PER_ENTRY - offset, pages_left);

        current_entry = s_bitmap[entry_idx];
        current_mask = memory_allocator_convert_to_mask(pages_to_claim, offset);

        if ((current_entry & current_mask) == 0) {
            pages_left -= pages_to_claim;
            entry_idx++;
            offset = 0;
        } else {
            // If number of checked pages is greater than align, it
            // means there is no sense to check from current_page + align,
            // because we already now that part of these pages are claimed
            uint32_t checked_pages = number - pages_left;
            current_page +=
                checked_pages > align ? ALIGN_UP(checked_pages, align) : align;

            offset = current_page % PAGES_PER_ENTRY;
            entry_idx = current_page / PAGES_PER_ENTRY;
            pages_left = number;
        }
    }

    if (pages_left == 0) {
        memory_allocator_claim_multiple_pages(current_page, number);
        return (void*)memory_allocator_get_addres(current_page);
    }

    return NULL;
}

void memory_allocator_free_page(void* addr) {
    ASSERT(HIGH, (uint64_t)addr % s_page_size == 0, "Addres must be aligned");

    uint32_t page_number = memory_allocator_get_page_number((uint64_t)addr);
    memory_allocator_free_specific_page(page_number);
}

void memory_allocator_free_range(void* from, void* to) {
    ASSERT(HIGH,
           (uint64_t)from % s_page_size == 0 && (uint64_t)to % s_page_size == 0,
           "Addres must be aligned");

    uint32_t from_page = memory_allocator_get_page_number((uint64_t)from);
    uint32_t to_page = memory_allocator_get_page_number((uint64_t)to);
    uint32_t pages_to_free = to_page - from_page;

    if (pages_to_free == 0) {
        pages_to_free++;
    }

    memory_allocator_free_multiple_pages(from_page, pages_to_free);
}

void memory_allocator_free_pages(void* addr, uint32_t count) {
    ASSERT(HIGH, (uint64_t)addr % s_page_size == 0, "Addres must be aligned");

    uint32_t from_page = memory_allocator_get_page_number((uint64_t)addr);
    memory_allocator_free_multiple_pages(from_page, count);
}