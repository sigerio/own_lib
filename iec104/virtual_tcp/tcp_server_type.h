#ifndef _TCP_SERVER_TYPE_H_
#define _TCP_SERVER_TYPE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


#include <netinet/in.h>     // sockaddr_in

typedef enum
{
    TCP_SERVER_CREAT = 0,
    TCP_SERVER_CONFIG,
    TCP_SERVER_BIND,
    TCP_SERVER_LISTING,
    TCP_SERVER_WAIT_LINK,
    TCP_SERVER_IDLE,
    TCP_SERVER_CLOSE,
    TCP_SERVER_STATE_MANAGE,
}VIRTUAL_TCP_SERVER;



typedef struct 
{
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    int server_fd;  
    int client_fd;
    uint8_t next_server_state;//服务端将切换的工作状态
    uint8_t cur_server_state;//服务端此刻的工作状态
    uint64_t work_time;
    /* data */
}virtual_tcp_server_info;



void* virtual_tcp_server_run_fsm(void* arg);


#endif

