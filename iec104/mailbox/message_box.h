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

enum{
    NO_SOURCE = 0,
    FROM_CLIENT,
    FROM_SERVER,
};




typedef struct message_node
{
    uint8_t data_from; // 0 no source/ 1 from clint / 2 from server
    uint16_t idx;
    uint16_t len;
    uint8_t* data;
    struct message_node* next;
    
}message_list;



typedef struct 
{
    message_list* head;
    message_list* tail;

}list_manage;



void sell_list_node(message_list* src_node);
void register_message_list(list_manage* node_belongs_to);
void write_in_message_list(list_manage node_belongs_to, message_list write_node);
int list_err(list_manage list_belong_to);
message_list* read_out_message_list(list_manage list_belong_to);



#endif
