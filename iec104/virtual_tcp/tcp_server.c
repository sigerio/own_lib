#include <unistd.h>         // close()
#include <arpa/inet.h>      // inet_addr()
#include <sys/socket.h>     // socket()


#include "tcp_define.h"
#include "tcp_server_type.h"

virtual_tcp_server_info tcp_server_info;

virtual_tcp_fsm  tcp_server_fsm[TCP_SERVER_STATE_MANAGE];



void set_server_work_state(uint8_t next_state)
{
    tcp_server_info.next_server_state = next_state;
    tcp_server_info.work_time = 0;
}

void virtual_tcp_server_creat(void* arg)
{
    // 1. 创建套接字 (IPv4, TCP)
    tcp_server_info.server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp_server_info.server_fd < 0) {
        perror("failed to creat socket\n");
        exit(EXIT_FAILURE);
    }
    set_server_work_state(TCP_SERVER_CONFIG);

}


void virtual_tcp_server_config(void* arg)
{
    // 2. 设置服务器地址信息
    tcp_server_info.server_addr.sin_family = AF_INET;
    tcp_server_info.server_addr.sin_addr.s_addr = INADDR_ANY;//inet_addr(CLIENT_IP); // 监听本机所有 IP -> INADDR_ANY; 
    tcp_server_info.server_addr.sin_port = htons(PORT);

    set_server_work_state(TCP_SERVER_BIND);
}

void virtual_tcp_server_bind(void* arg)
{
    // 3. 绑定 IP 和端口
    if (bind(tcp_server_info.server_fd, (struct sockaddr*)&tcp_server_info.server_addr, sizeof(tcp_server_info.server_addr)) < 0) {
        perror("failed to bind port\n");
        close(tcp_server_info.server_fd);
        exit(EXIT_FAILURE);
    }

    set_server_work_state(TCP_SERVER_LISTING);
}


void virtual_tcp_server_linsting(void* arg)
{
    // 4. 开始监听
    if (listen(tcp_server_info.server_fd, 5) < 0) {
        perror("failed to linsting ip");
        close(tcp_server_info.server_fd);
        exit(EXIT_FAILURE);
    }
    printf("TCP server has started , bind port %d...\n", PORT);

    set_server_work_state(TCP_SERVER_WAIT_LINK);
}


void virtual_tcp_server_wait_connect(void* arg)
{
    socklen_t addr_len = sizeof(tcp_server_info.client_addr);
    // 5. 等待客户端连接
    tcp_server_info.client_fd = accept(tcp_server_info.server_fd, (struct sockaddr*)&tcp_server_info.client_addr, &addr_len);
    if (tcp_server_info.client_fd < 0) {
        perror("error occured when client connect to server\n");
        close(tcp_server_info.server_fd);
        exit(EXIT_FAILURE);
    }
    printf("connect success: %s:%d\n", inet_ntoa(tcp_server_info.client_addr.sin_addr), ntohs(tcp_server_info.client_addr.sin_port));

    
    set_server_work_state(TCP_SERVER_IDLE);
}


//TODO 需要在这里将消息处理之消息盒子中
void virtual_tcp_server_idle(void* arg)
{
    char buffer[1024];
    // 6. 接收客户端数据
    int len = recv(tcp_server_info.client_fd, buffer, sizeof(buffer) - 1, 0);
    if (len > 0) {
        // buffer[len] = '\0';
        // printf("收到: %s\n", buffer);

        // // 7. 回复数据
        // send(client_fd, "你好，客户端！", strlen("你好，客户端！"), 0);
        printf("server recv some messages\n");
    }

    
}


void virtual_tcp_server_close(void* arg)
{
    // 8. 关闭连接
    if(tcp_server_info.cur_server_state != TCP_SERVER_CLOSE)
    {
        close(tcp_server_info.client_fd);
        close(tcp_server_info.server_fd);
        tcp_server_info.cur_server_state = TCP_SERVER_CLOSE;
    }
}



void virtual_tcp_server_fsm_register(void)
{
    tcp_server_fsm[TCP_SERVER_CREAT].fun = virtual_tcp_server_creat;
    tcp_server_fsm[TCP_SERVER_CONFIG].fun = virtual_tcp_server_config;
    tcp_server_fsm[TCP_SERVER_BIND].fun = virtual_tcp_server_bind;
    tcp_server_fsm[TCP_SERVER_LISTING].fun = virtual_tcp_server_linsting;
    tcp_server_fsm[TCP_SERVER_WAIT_LINK].fun = virtual_tcp_server_wait_connect;
    tcp_server_fsm[TCP_SERVER_IDLE].fun = virtual_tcp_server_idle;
    tcp_server_fsm[TCP_SERVER_CLOSE].fun = virtual_tcp_server_close;
}



void* virtual_tcp_server_run_fsm(void* arg)
{
    virtual_tcp_server_fsm_register();
    for(;;)
    {
        tcp_server_fsm[tcp_server_info.next_server_state].fun(arg);
    }
    return NULL;
}


