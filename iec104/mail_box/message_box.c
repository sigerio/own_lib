#include "message_box.h"
#include "my_def.h"




typedef struct 
{
    message_list* head;
    message_list* tail;

}list_manage;

list_manage list_info_;

void update_manage_info(void* list_belong_to, uint8_t info_type, void* info)
{
    list_manage* list_info = (list_manage*)list_belong_to;
    message_list* add_info = (message_list*)info;
    if(info_type == UPDATE_HEAD)
    {
        list_info->head = add_info;
    }
    else if(info_type == UPDATE_TAIL)
    {
        list_info->tail = add_info;
    }
    else if(info_type == UPDATE_ALL)
    {
        list_info->head = add_info;
        list_info->tail = add_info;
    }
    
    
}

int list_err(void list_belong_to)
{
    list_manage list_info = (list_manage)list_belong_to;
    if((list_info.head == MY_NULL)||(list_info.tail == MY_NULL))
        return -1;
    else   
        return 0;
}


void* get_list_tail(void list_belong_to)
{
    list_manage list_info = (list_manage)list_belong_to;
    return (void*)list_info.tail;
}


void* get_list_head(void list_belong_to)
{
    list_manage list_info = (list_manage)list_belong_to;
    return (void*)list_info.head;
}

void* buy_list_node(void src_node)
{
    message_list node = (message_list)src_node;
    uint16_t node_len = (uint16_t)sizeof(message_list);
    message_list* node = (message_list*)malloc(node_len);

    memcpy(node, &src_node, node_len);

    return (void*)node;
}



void sell_list_node(void* src_node)
{
    message_list node = (message_list*)src_node;
    if(node->data != MY_NULL)
        free(node->data);
    if(node != MY_NULL)
        free(node);
}


void register_message_list(void* node_belongs_to)
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



void write_in_message_list(void* node_belongs_to, void write_node)
{
    message_list src_node = (message_list)write_node;
    message_list* tail = get_list_tail();
    message_list* node = buy_list_node(src_node);
    if(tail == MY_NULL)
        return;
    
    tail->next = node;

    update_manage_info(node_belongs_to, UPDATE_TAIL, node);    
    
}

//是否增加对空节点的严格检测?
void* read_out_message_list(void list_belong_to)
{
    message_list* head = get_list_head(list_belong_to);

    message_list* node = head->next;
    head->next = node->next;
    
    return (void)node;
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

