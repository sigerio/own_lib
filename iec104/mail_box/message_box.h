#ifndef __MESSAGE_BOX_H__
#define __MESSAGE_BOX_H__

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>


enum
{
    UPDATE_HEAD = 0,
    UPDATE_TAIL,
    UPDATE_ALL,

};


typedef struct message_node
{
    uint8_t data_from; // 0 no source/ 1 from clint / 2 from server
    uint16_t idx;
    uint16_t len;
    uint8_t* data;
    struct message_node* next;
    
}message_list;

void sell_list_node(message_list* node);
void register_message_list(void);

void write_in_message_list(message_list src_node);

message_list* read_out_message_list(void);


#endif
