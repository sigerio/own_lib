#include <stdbool.h>
#include <stdint.h>





typedef struct message_node
{
    uint8_t data_from; // 0 no source/ 1 from clint / 2 from server
    uint16_t idx;
    uint8_t len;
    uint8_t* data;
    message_node* next;
    
}message_list;


void register_message_list(void);

