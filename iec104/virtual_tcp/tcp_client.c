#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>


#include "tcp_define.h"
#include "tcp_client_type.h"

virtual_tcp_client_info tcp_client_info;
virtual_tcp_fsm  tcp_client_fsm[TCP_CLIENT_STATE_MANAGE];

void set_client_work_state(uint8_t next_state)
{
    tcp_client_info.next_client_state = next_state;
    tcp_client_info.work_time = 0;
}



void virtual_tcp_client_creat(void* arg)
{
    // 1. 创建套接字 (AF_INET=IPv4, SOCK_STREAM=TCP)
    tcp_client_info.client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp_client_info.client_fd < 0) {
        perror("failed to creat socket\n");
        exit(EXIT_FAILURE);
    }
    set_client_work_state(TCP_CLIENT_CONFIG);

}

void virtual_tcp_client_config(void* arg)
{
    while(get_server_runnint_state() < TCP_SERVER_BIND);
    // 1. 配置本地 IP 和端口
    memset(&tcp_client_info.client_addr, 0, sizeof(tcp_client_info.client_addr));
    tcp_client_info.client_addr.sin_family = AF_INET;
    tcp_client_info.client_addr.sin_port = htons(0);//htons(PORT); // 0 表示让系统自动分配端口
    tcp_client_info.client_addr.sin_addr.s_addr = INADDR_ANY;//inet_addr(CLIENT_IP); // 客户端要使用的本地IP

    if (bind(tcp_client_info.client_fd, (struct sockaddr*)&tcp_client_info.client_addr, sizeof(tcp_client_info.client_addr)) < 0) {
        perror("client addr has binded failed");
        close(tcp_client_info.client_fd);
        printf("%d\n",tcp_client_info.client_fd);
        exit(EXIT_FAILURE);
    }

    tcp_client_info.server_addr = get_tcp_server_sock();
    printf("server: %s has running\n",inet_ntoa(tcp_client_info.server_addr.sin_addr));
    // 2. 配置服务器信息
    // tcp_client_info.server_addr.sin_family = AF_INET;           // IPv4
    // tcp_client_info.server_addr.sin_port = htons(PORT);          // 目标端口（例：502）
    // tcp_client_info.server_addr.sin_addr.s_addr = inet_addr(SERVER_IP); // 目标IP

    set_client_work_state(TCP_CLIENT_CONNECT);
}


void virtual_tcp_client_connect(void* arg)
{

    while(get_server_runnint_state() < TCP_SERVER_LISTING);
    // 3. 连接服务器
    if (connect(tcp_client_info.client_fd, (struct sockaddr *)&tcp_client_info.server_addr, sizeof(tcp_client_info.server_addr)) < 0) {
        perror("连接失败");
        close(tcp_client_info.client_fd);
        exit(EXIT_FAILURE);
    }
    printf("connect success\n");

    //发送握手信息
    send(tcp_client_info.client_fd, CLIENT_HAND_MESSAGE, strlen(CLIENT_HAND_MESSAGE), 0);
    
    set_client_work_state(TCP_CLIENT_WAIT_HAND);
}

void virtual_tcp_client_wait_hand(void* arg)
{
    char buffer[1024];
    // 5. 接收数据
    int len = recv(tcp_client_info.client_fd, buffer, sizeof(buffer) - 1, 0);
    if (len > 0) {
        if(strcmp(SERVER_HAN_MESSAGE,buffer) == 0)
        {
            
            set_client_work_state(TCP_CLIENT_IDLE);
            printf("hand success , message from server is : %s\n",buffer);
        }
        
    }
}



void virtual_tcp_client_idle(void* arg)
{
    char buffer[1024];
    
    if(tcp_client_info.cur_client_state != TCP_CLIENT_IDLE)
    {
        tcp_client_info.cur_client_state = TCP_CLIENT_IDLE;
        
    }
    // 5. 接收数据
    int len = recv(tcp_client_info.client_fd, buffer, sizeof(buffer) - 1, 0);
    if (len > 0) {
        printf("%s\n",buffer);
    }
}



void virtual_tcp_client_close(void* arg)
{
    // 6. 关闭连接
    if(tcp_client_info.cur_client_state != TCP_CLIENT_CLOSE)
    {
        close(tcp_client_info.client_fd);
        tcp_client_info.cur_client_state = TCP_CLIENT_CLOSE;
    }
    
}




void virtual_tcp_client_fsm_register(void)
{
    
    tcp_client_fsm[TCP_CLIENT_CREAT].fun = virtual_tcp_client_creat;
    tcp_client_fsm[TCP_CLIENT_CONFIG].fun = virtual_tcp_client_config;
    tcp_client_fsm[TCP_CLIENT_CONNECT].fun = virtual_tcp_client_connect;
    tcp_client_fsm[TCP_CLIENT_WAIT_HAND].fun = virtual_tcp_client_wait_hand;
    tcp_client_fsm[TCP_CLIENT_IDLE].fun = virtual_tcp_client_idle;
    tcp_client_fsm[TCP_CLIENT_CLOSE].fun = virtual_tcp_client_close;
    

}


void* virtual_tcp_client_run_fsm(void* arg)
{
    virtual_tcp_client_fsm_register();
    for(;;)
    {
        tcp_client_fsm[tcp_client_info.next_client_state].fun(arg);
    }
    return NULL;
}

