#ifndef KERNEL_ALLOC_BITMAP_H_
#define KERNEL_ALLOC_BITMAP_H_

#include <stdint.h>

void alloc_bitmap_init(void* addr, uint32_t number_of_pages, uint32_t page_size);

void* alloc_bitmap_claim_page();

void alloc_bitmap_free_page(void* addr);

void* alloc_bitmap_claim_pages(uint32_t number, uint32_t align);

void alloc_bitmap_free_pages(void* addr, uint32_t number);

void alloc_bitmap_claim_range(void* from, void* to);

void alloc_bitmap_free_range(void* from, void* to);

#endif