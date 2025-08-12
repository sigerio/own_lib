#ifndef __TCP_DEFINE_H__
#define __TCP_DEFINE_H__




#define PORT 8888   // 监听端口

#define SERVER_IP ("127.0.0.4")
#define CLIENT_IP ("127.0.0.222")










typedef struct 
{
    void (*fun)(void*);
}virtual_tcp_fsm;


#endif


