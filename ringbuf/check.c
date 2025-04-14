#include <string.h>
#include <stdint.h>
#include "check.h"
#include "crc32.h"
#include "ringbuffer.h"



uint8_t check_farm(uint16_t len, uint8_t *src, Ringbuffer *rb)
{
    uint8_t src_data_len = 0;
    uint16_t crc = 0;
    // len += rb->drop_data.remine_len;
    int check_len = 1;
    if (len > 8)
    {
        if ((src[0] == 0xAA) && (src[1] == 0x55)) // 数据包头正确
        {
            uint16_t packLen;
            src_data_len = (uint16_t)(src[2] << 8) + src[3];
            if (src_data_len <= len)
            {
                // crc and tail , then check tail_drop, if tail drop = 0,mines that will remain some byte to be checked in next turn

                if ((src[src_data_len - 2] == 0x0D) && (src[src_data_len - 1] == 0x0A))
                {
                    crc = crc16(src, src_data_len - 4);
                    if ((src[src_data_len - 3] == (uint8_t)crc) && (src[src_data_len - 4] == (uint8_t)(crc >> 8)))
                    {
                        rb->drop_data.drop_type = f_succ;
                    }
                    else
                    {
                        rb->drop_data.drop_type = f_crc_e;
                    }
                    rb->drop_data.drop_len = src_data_len;
                    rb->drop_data.remine_len = len - src_data_len;
                }
            }

            if (src_data_len > len)
            {
                rb->drop_data.drop_type = 4;//f_no_tail;
                rb->drop_data.drop_len = 0;
                rb->drop_data.remine_len = len;
            }
        }
        else
        {
            while(check_len < len)
            {
                if(src[check_len] == 0xAA)
                {
                    break;
                }
                check_len++;
            }
            rb->drop_data.drop_len = check_len;
            rb->drop_data.remine_len = len - check_len;
            rb->drop_data.drop_type = 5;//f_no_tail;//f_no_head;
            
        }
    }
    else
    {
        
        rb->drop_data.drop_len = 0;
        rb->drop_data.remine_len = len;
        rb->drop_data.drop_type = 6;//f_no_head;
    }
    return rb->drop_data.drop_type;
}
