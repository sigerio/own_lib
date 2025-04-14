#ifndef _CHECK_H
#define _CHECK_H

#include <string.h>
#include <stdint.h>
#include "ringbuffer.h"
typedef enum
{
    f_succ = 0,
    f_crc_e,
    f_no_head,
    f_no_tail,
    
} farm_type;



uint8_t check_farm(uint16_t len, uint8_t *src, Ringbuffer *rb);












#endif
