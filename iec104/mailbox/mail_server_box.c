#include "message_box.h"
#include "mail_box_define.h"
#include "../my_def.h"
#include <pthread.h>
#include <stdio.h>

list_manage server_mail_box;
Mail_Server_Box_Info mail_server_box_info = {0};

void register_server_mail_box(void)
{
    register_message_list(&server_mail_box);
}

void server_box_send(uint8_t* data, uint8_t len)
{

    message_list src_node;
    src_node.data_from = FROM_SERVER; //服务器需要向外发送数据
    src_node.idx = mail_server_box_info.server_send_idx;
    src_node.len = len;
    src_node.data = (uint8_t*)malloc(len+1);
    src_node.next = NULL;
    memcpy(src_node.data,data,len);

    write_in_message_list(server_mail_box,src_node);

    mail_server_box_info.server_send_idx++;
}


void server_box_recv(uint8_t* data, uint8_t len)
{
    
        
    message_list src_node;
    src_node.data_from = FROM_CLIENT; //服务器需要解析数据
    src_node.idx = mail_server_box_info.server_recv_idx;
    src_node.len = len;
    src_node.data = (uint8_t*)malloc(len+1);
    src_node.next = NULL;
    memcpy(src_node.data,data,len);

    write_in_message_list(server_mail_box,src_node);
    mail_server_box_info.server_recv_idx++;
}

void* server_mail_box_analyze_transform(void* arg)
{
    while (1)
    {
        if(list_err(server_mail_box) == 0)
        {
            message_list* read_node = read_out_message_list(server_mail_box);
            if(read_node != MY_NULL)
            {
                if(read_node->data_from == FROM_CLIENT)
                {

                }
                else if(read_node->data_from == FROM_SERVER)
                {

                }
                else
                {
                    sell_list_node(read_node);
                }
            }

        }
    }
    return NULL;
    
}



void* server_mail_box_signl_handle(void* arg)
{
    while (1)
    {
        /* code */
    }
    return NULL;
    
}



void* server_mail_box_run_fsm(void* arg)
{

    //TODO 在此线程下创建两个个线程，其一：用于解析/直接发送，其二 用于周期或者信号事件的处理
    pthread_t analyze_transform, signl_handle;
    int ret;
    ret = pthread_create(&analyze_transform, NULL, server_mail_box_analyze_transform, NULL);
    if(ret != 0) {
        printf("Create 104 client thread error!\n");
        return NULL;
    }

    ret = pthread_create(&signl_handle, NULL, server_mail_box_signl_handle, NULL);
    if(ret != 0) {
        printf("Create 104 server thread error!\n");
        return NULL;
    }

    pthread_join(analyze_transform, NULL);
    pthread_join(signl_handle, NULL);
    return NULL;
}




