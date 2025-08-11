#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>         // close()
#include <arpa/inet.h>      // inet_addr()
#include <sys/socket.h>     // socket()
#include <netinet/in.h>     // sockaddr_in

#include "tcp_define.h"


int server_fd, client_fd;

struct sockaddr_in server_addr, client_addr;

virtual_tcp_fsm  tcp_server_fsm[TCP_SERVER_STATE_MANAGE];


void virtual_tcp_server_creat(void* arg)
{
    // 1. 创建套接字 (IPv4, TCP)
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("failed to creat socket\n");
        exit(EXIT_FAILURE);
    }
}


void virtual_tcp_server_config(void* arg)
{
    // 2. 设置服务器地址信息
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(CLIENT_IP); // 监听本机所有 IP
    server_addr.sin_port = htons(PORT);
}

void virtual_tcp_server_bind(void* arg)
{
    // 3. 绑定 IP 和端口
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("failed to bind port\n");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
}


void virtual_tcp_server_linsting(void* arg)
{
    // 4. 开始监听
    if (listen(server_fd, 5) < 0) {
        perror("failed to linsting ip");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("TCP server has started , bind port %d...\n", PORT);
}


void virtual_tcp_server_wait_connect(void* arg)
{
    socklen_t addr_len = sizeof(client_addr);
    // 5. 等待客户端连接
    client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
    if (client_fd < 0) {
        perror("error occured when client connect to server\n");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("connect success: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
}


//TODO 需要在这里将消息处理之消息盒子中
void virtual_tcp_server_idle(void* arg)
{
    char buffer[1024];
    // 6. 接收客户端数据
    int len = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (len > 0) {
        // buffer[len] = '\0';
        // printf("收到: %s\n", buffer);

        // // 7. 回复数据
        // send(client_fd, "你好，客户端！", strlen("你好，客户端！"), 0);
    }
}


void virtual_tcp_server_close(void* arg)
{
    // 8. 关闭连接
    close(client_fd);
    close(server_fd);
}



void virtual_tcp_fsm_register(void)
{
    tcp_server_fsm[TCP_SERVER_CREAT].fun = virtual_tcp_server_creat;
    tcp_server_fsm[TCP_SERVER_CONFIG].fun = virtual_tcp_server_config;
    tcp_server_fsm[TCP_SERVER_BIND].fun = virtual_tcp_server_bind;
    tcp_server_fsm[TCP_SERVER_LISTING].fun = virtual_tcp_server_linsting;
    tcp_server_fsm[TCP_SERVER_WAIT_LINK].fun = virtual_tcp_server_wait_connect;
    tcp_server_fsm[TCP_SERVER_IDLE].fun = virtual_tcp_server_idle;
    tcp_server_fsm[TCP_SERVER_CLOSE].fun = virtual_tcp_server_close;
}



