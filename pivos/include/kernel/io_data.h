#ifndef KERNEL_IO_DATA_H_
#define KERNEL_IO_DATA_H_

#include <kernel/buffer/ring.h>

int32_t io_data_init();

struct kbuffer_ring* io_data_get_text_output_buffer();

struct kbuffer_ring* io_data_get_text_input_buffer();

#endif