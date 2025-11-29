#include <kernel/io_data.h>
#include <kernel/buffer/ring.h>

#define BUFFER_DATA_SIZE 4096

static struct kbuffer_ring input_buffer;
static uint8_t input_buffer_data[BUFFER_DATA_SIZE];

static struct kbuffer_ring output_buffer;
static uint8_t output_buffer_data[BUFFER_DATA_SIZE];

int32_t io_data_init() {
    input_buffer = KBUFFER_RING_INIT(BUFFER_DATA_SIZE, input_buffer_data);
    output_buffer = KBUFFER_RING_INIT(BUFFER_DATA_SIZE, output_buffer_data);

    return 1;
}

struct kbuffer_ring* io_data_get_text_input_buffer() {
    return &input_buffer;
}

struct kbuffer_ring* io_data_get_text_output_buffer() {
    return &output_buffer;
}
