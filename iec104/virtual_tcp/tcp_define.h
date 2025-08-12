#ifndef __TCP_DEFINE_H__
#define __TCP_DEFINE_H__


#include <netinet/in.h>     // sockaddr_in

#define PORT 8888   // 监听端口

#define SERVER_IP ("127.0.0.4")
#define CLIENT_IP ("127.0.0.222")




struct sockaddr_in get_tcp_server_sock(void);





typedef struct 
{
    void (*fun)(void*);
}virtual_tcp_fsm;


#endif


