#ifndef _TCP_CLIENT_TYPE_H_
#define _TCP_CLIENT_TYPE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netinet/in.h>


typedef enum
{
    TCP_CLIENT_CREAT = 0,
    TCP_CLIENT_CONFIG,
    TCP_CLIENT_CONNECT,
    TCP_CLIENT_IDLE,
    TCP_CLIENT_CLOSE,
    TCP_CLIENT_STATE_MANAGE,
}VIRTUAL_TCP_CLIENT;



typedef struct 
{
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;

    int client_fd;
    uint8_t next_client_state;//客户端将切换的工作状态
    uint8_t cur_client_state;//客户端此刻的工作状态
    uint64_t work_time;
    /* data */
}virtual_tcp_client_info;



void* virtual_tcp_client_run_fsm(void* arg);


#endif

