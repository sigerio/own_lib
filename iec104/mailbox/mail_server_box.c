#include "message_box.h"

list_manage server_mail_box;
uint32_t server_send_idx = 0;

void register_server_mail_box(void)
{
    register_message_list(&server_mail_box);
}

void server_box_send(uint8_t* data, uint8_t len)
{

    message_list src_node;
    src_node.data_from = FROM_SERVER; //服务器需要向外发送数据
    src_node.idx = server_send_idx;
    src_node.len = len;
    src_node.data = (uint8_t*)malloc(len+1);
    src_node.next = NULL;
    memcpy(src_node.data,data,len);

    write_in_message_list(server_mail_box,src_node);

    server_send_idx++;
}


void server_box_recv(uint8_t* data, uint8_t len)
{
    if(server_send_idx > 0)
        server_send_idx--;
    message_list src_node;
    src_node.data_from = FROM_CLIENT; //服务器需要解析数据
    src_node.idx = server_send_idx;
    src_node.len = len;
    src_node.data = (uint8_t*)malloc(len+1);
    src_node.next = NULL;
    memcpy(src_node.data,data,len);

    write_in_message_list(server_mail_box,src_node);

}





void server_mail_box_run_fsm(void* arg)
{
    if(list_err(server_mail_box) == 0)
    {

    }
}




