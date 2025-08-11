#include "message_box.h"

message_list head;

void register_message_list(void)
{
    head.data_from = 0;
    head.len = 0;
    head.idx = 0;
    head.data = NULL;
    head.next = NULL;
}





