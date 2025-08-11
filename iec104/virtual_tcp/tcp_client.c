#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "tcp_define.h"

int main() {
    int sock;
    struct sockaddr_in local_addr, server_addr;
    char buffer[1024];

    // 1. 创建套接字 (AF_INET=IPv4, SOCK_STREAM=TCP)
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket 创建失败");
        return -1;
    }

    // 1. 配置本地 IP 和端口
    memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(PORT); // 0 表示让系统自动分配端口
    local_addr.sin_addr.s_addr = inet_addr(CLIENT_IP); // 客户端要使用的本地IP

    if (bind(sock, (struct sockaddr*)&local_addr, sizeof(local_addr)) < 0) {
        perror("本地IP绑定失败");
        close(sock);
        exit(EXIT_FAILURE);
    }

    // 2. 配置服务器信息
    server_addr.sin_family = AF_INET;           // IPv4
    server_addr.sin_port = htons(PORT);          // 目标端口（例：502）
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP); // 目标IP

    // 3. 连接服务器
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("连接失败");
        close(sock);
        return -1;
    }
    printf("连接成功！\n");

    // 4. 发送数据
    const char *msg = "Hello TCP";
    send(sock, msg, strlen(msg), 0);

    // 5. 接收数据
    int len = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (len > 0) {
        buffer[len] = '\0';
        printf("收到: %s\n", buffer);
    }

    // 6. 关闭连接
    close(sock);
    return 0;
}
