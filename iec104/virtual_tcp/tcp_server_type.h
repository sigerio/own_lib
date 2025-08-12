#ifndef _TCP_SERVER_TYPE_H_
#define _TCP_SERVER_TYPE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <netinet/in.h>     // sockaddr_in





typedef struct 
{
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    int server_fd;  
    int client_fd;
    uint8_t next_server_state;//服务端将切换的工作状态
    uint8_t cur_server_state;//服务端此刻的工作状态
    uint8_t link_state;
    uint64_t work_time;
    /* data */
}virtual_tcp_server_info;



void* virtual_tcp_server_run_fsm(void* arg);


#endif

