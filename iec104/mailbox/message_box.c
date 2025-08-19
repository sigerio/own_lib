#include "message_box.h"
#include "../my_def.h"






list_manage list_info;

void update_manage_info(list_manage* list_belong_to, uint8_t info_type, message_list* info)
{
    if(info_type == UPDATE_HEAD)
    {
        list_belong_to->head = info;
    }
    else if(info_type == UPDATE_TAIL)
    {
        list_belong_to->tail = info;
    }
    else if(info_type == UPDATE_ALL)
    {
        list_belong_to->head = info;
        list_belong_to->tail = info;
    }
    
    
}

int list_err(list_manage list_belong_to)
{

    if((list_belong_to.head->next == MY_NULL)&&(list_belong_to.tail == MY_NULL))
        return -1;
    else   
        return 0;
}


message_list* get_list_tail(list_manage list_belong_to)
{
    return list_belong_to.tail;
}


message_list* get_list_head(list_manage list_belong_to)
{

    return list_belong_to.head;
}

message_list* buy_list_node(message_list src_node)
{
    
    uint16_t node_len = (uint16_t)sizeof(message_list);
    message_list* node = (message_list*)malloc(node_len);

    memcpy(node, &src_node, node_len);

    return node;
}



void sell_list_node(message_list* src_node)
{
    
    if(src_node->data != MY_NULL)
        free(src_node->data);
    if(src_node != MY_NULL)
        free(src_node);
}


void register_message_list(list_manage* node_belongs_to)
{
    message_list loc_head;
    loc_head.data_from = 0;
    loc_head.len = 0;
    loc_head.idx = 0;
    loc_head.data = MY_NULL;
    loc_head.next = MY_NULL;

    message_list* dst_node = buy_list_node(loc_head);

    update_manage_info(node_belongs_to,UPDATE_ALL, dst_node);
}



void write_in_message_list(list_manage node_belongs_to, message_list write_node)
{
    // message_list src_node = (message_list)write_node;
    message_list* tail = get_list_tail(node_belongs_to);
    message_list* node = buy_list_node(write_node);
    if(tail == MY_NULL)
        return;
    
    tail->next = node;

    update_manage_info(&node_belongs_to, UPDATE_TAIL, node);    
    
}

//是否增加对空节点的严格检测?
message_list* read_out_message_list(list_manage list_belong_to)
{
    message_list* head = get_list_head(list_belong_to);

    message_list* node = head->next;
    head->next = node->next;
    
    return node;
}




// void send_message(uint8_t data_from, uint8_t* data, uint8_t len)
// {
//     message_list src_node;
//     src_node.data_from = data_from;
//     src_node.idx++;
//     src_node.len = len;
//     src_node.data = (uint8_t*)malloc(len);
//     memcpy(src_node.data,data,len);

//     write_in_message_list(src_node);

// }



// uint8_t* read_message(void)
// {
//     message_list* data_node = read_out_message_list();
// }


#if 0
//测试读写接口是否正常
uint8_t buf[2][2] = {{1,2},{3,4}};

message_list loc_node;
for(int i = 0; i<2; i++)
{
    loc_node.data_from = 1;
    loc_node.idx++;
    loc_node.len = 2;
    loc_node.data = buf[i];

    write_in_message_list(loc_node);
    printf("write %d\n",loc_node.idx);
    
}
message_list* read_node;
for(int k = 0; k<2; k++)
{
    read_node = read_out_message_list();
    printf("%d\n",read_node->data[0]);
    sell_list_node(read_node);
}

#endif

