#ifndef _RING_BUFFER_H_
#define _RING_BUFFER_H_

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif
typedef struct
{
    // size_t head_drop;
    // size_t tail_drop;
    // size_t farm_drop;
    size_t drop_len;
    size_t remine_len;
    uint8_t drop_type;
}Drop_data;

typedef struct 
{
    uint16_t len_queue[128];
    uint16_t num;
}Queue_len;


/* ring buffer */
typedef struct 
{
    uint8_t *buffer_ptr;
    /* use the msb of the {read,write}_index as mirror bit. You can see this as
     * if the buffer adds a virtual mirror and the pointers point either to the
     * normal or to the mirrored buffer. If the write_index has the same value
     * with the read_index, but in a different mirror, the buffer is full.
     * While if the write_index and the read_index are the same and within the
     * same mirror, the buffer is empty. The ASCII art of the ringbuffer is:
     *
     *          mirror = 0                    mirror = 1
     * +---+---+---+---+---+---+---+|+~~~+~~~+~~~+~~~+~~~+~~~+~~~+
     * | 0 | 1 | 2 | 3 | 4 | 5 | 6 ||| 0 | 1 | 2 | 3 | 4 | 5 | 6 | Full
     * +---+---+---+---+---+---+---+|+~~~+~~~+~~~+~~~+~~~+~~~+~~~+
     *  read_idx-^                   write_idx-^
     *
     * +---+---+---+---+---+---+---+|+~~~+~~~+~~~+~~~+~~~+~~~+~~~+
     * | 0 | 1 | 2 | 3 | 4 | 5 | 6 ||| 0 | 1 | 2 | 3 | 4 | 5 | 6 | Empty
     * +---+---+---+---+---+---+---+|+~~~+~~~+~~~+~~~+~~~+~~~+~~~+
     * read_idx-^ ^-write_idx
     */

    uint32_t read_mirror : 1;
    uint32_t read_index : 31;
    uint32_t write_mirror : 1;
    uint32_t write_index : 31;
    /* as we use msb of index as mirror bit, the size should be signed and
     * could only be positive. */
    int32_t buffer_size;
    Queue_len len_que;
    Drop_data drop_data;
}Ringbuffer;

enum ringbuffer_state
{
    RINGBUFFER_EMPTY,
    RINGBUFFER_FULL,
    /* half full is neither full nor empty */
    RINGBUFFER_HALFFULL,
};

/**
 * RingBuffer for DeviceDriver
 *
 * Please note that the ring buffer implementation of RT-Thread
 * has no thread wait or resume feature.
 */
void ringbuffer_init(Ringbuffer *rb, uint8_t *pool, int32_t size);
void ringbuffer_reset(Ringbuffer *rb);
size_t ringbuffer_put(Ringbuffer *rb, const uint8_t *ptr, uint32_t length);
size_t ringbuffer_put_force(Ringbuffer *rb, const uint8_t *ptr, uint32_t length);
size_t ringbuffer_putchar(Ringbuffer *rb, const uint8_t ch);
size_t ringbuffer_putchar_force(Ringbuffer *rb, const uint8_t ch);
size_t ringbuffer_get(Ringbuffer *rb, uint8_t *ptr, uint32_t length);
size_t ringbuffer_peek(Ringbuffer *rb, uint8_t **ptr);
size_t ringbuffer_getchar(Ringbuffer *rb, uint8_t *ch);
size_t ringbuffer_data_len(Ringbuffer *rb);
size_t ringbuffer_view_data(Ringbuffer *rb , uint8_t *ptr, size_t length);
void ringbuffer_update_rw_state(Ringbuffer *rb);
void put_in_que(Ringbuffer *rb, uint16_t data);
uint16_t get_que(Ringbuffer *rb);

/**
 * @brief Get the buffer size of the ring buffer object.
 *
 * @param rb        A pointer to the ring buffer object.
 *
 * @return  Buffer size.
 */
inline uint32_t ringbuffer_get_size(Ringbuffer *rb)
{
    return rb->buffer_size;
}

/** return the size of empty space in rb */
#define ringbuffer_space_len(rb) ((rb)->buffer_size - ringbuffer_data_len(rb))




#ifdef __cplusplus
}
#endif

#endif
