#include <kernel/memory.h>
#include <kernel/memory/allocator.h>
#include <kernel/memory/mmu.h>
#include <kernel/memory/mmu/reg.h>
#include <kernel/utils.h>

#define ALIGN_UP_PAGE(addr) ALIGN_UP(addr, MMU_PAGE_SIZE)
#define ALIGN_DOWN_PAGE(addr) ALIGN_DOWN(addr, MMU_PAGE_SIZE)

extern char mmu_space_start;
extern char kernel_end;
static struct memory_mmu_context s_kernel_memory_context;

int32_t memory_init(uint64_t size) {
    uint64_t number_of_pages = size / MMU_PAGE_SIZE;
    uint64_t peripherials_start = 0xF0000000ULL;
    uint64_t peripherials_end = 0x100000000ULL;
    uint64_t image_size = ((uint64_t)&kernel_end) & ~KERNEL_SPACE_MASK;

    ASSERT(LOW, &mmu_space_start > 0, "Check mmu_space_start in .ld script");
    ASSERT(LOW, &kernel_end > 0, "Check kernel_end symbol in .ld script");

    // Prepare allocator

    memory_allocator_init((void *)image_size, number_of_pages, MMU_PAGE_SIZE);
    memory_allocator_claim_range((void*)0, (void*)ALIGN_UP_PAGE(image_size));
    memory_allocator_claim_range((void*)ALIGN_DOWN_PAGE(peripherials_start), (void*)ALIGN_UP_PAGE(peripherials_end));

    memory_mmu_init(memory_allocator_claim_pages, memory_allocator_free_pages);

    // Prepare kernel regions

    s_kernel_memory_context = (struct memory_mmu_context){
        .level0_table = (uint64_t*)&mmu_space_start
    };

    struct memory_mmu_region normal_memory_region_1 = {
        .memory_type = MMU_MEMORY_TYPE_NORMAL_NO_CACHE, 
        .va = 0x0,
        .size = peripherials_start
    };

    struct memory_mmu_region peripherals_region = {
        .memory_type = MMU_MEMORY_TYPE_DEVICE,
        .va = peripherials_start,
        .size = peripherials_end - peripherials_start
    };

    struct memory_mmu_region normal_memory_region_2 = {
        .memory_type = MMU_MEMORY_TYPE_NORMAL_NO_CACHE,
        .va = peripherials_end,
        .size = size - peripherials_end
    };

    memory_mmu_map_region(&s_kernel_memory_context, &normal_memory_region_1, MMU_MEMORY_MAP_ONE_TO_ONE);
        
    memory_mmu_map_region(&s_kernel_memory_context, &peripherals_region, MMU_MEMORY_MAP_ONE_TO_ONE);

    memory_mmu_map_region(&s_kernel_memory_context, &normal_memory_region_2, MMU_MEMORY_MAP_ONE_TO_ONE);

    return 0;
}