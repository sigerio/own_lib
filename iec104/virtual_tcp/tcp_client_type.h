#ifndef _TCP_CLIENT_TYPE_H_
#define _TCP_CLIENT_TYPE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>





typedef struct 
{
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;

    int client_fd;
    uint8_t next_client_state;//客户端将切换的工作状态
    uint8_t cur_client_state;//客户端此刻的工作状态
    uint8_t link_state;
    uint64_t work_time;

    /* data */
}virtual_tcp_client_info;



void* virtual_tcp_client_run_fsm(void* arg);


#endif

