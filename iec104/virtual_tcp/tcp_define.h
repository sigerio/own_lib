#ifndef __TCP_DEFINE_H__
#define __TCP_DEFINE_H__


#include <netinet/in.h>     // sockaddr_in

#define PORT 8888   // 监听端口

#define SERVER_IP ("127.0.0.4")
#define CLIENT_IP ("127.0.0.222")


#define CLIENT_HAND_MESSAGE  "I'M COMMING SERVER"
#define SERVER_HAN_MESSAGE   "HELLO CLIENT"


typedef enum
{
    TCP_SERVER_CREAT = 0,
    TCP_SERVER_CONFIG,
    TCP_SERVER_BIND,
    TCP_SERVER_LISTING,
    TCP_SERVER_WAIT_LINK,
    TCP_SERVER_WAIT_HAND,
    TCP_SERVER_IDLE,
    TCP_SERVER_CLOSE,
    TCP_SERVER_STATE_MANAGE,
}VIRTUAL_TCP_SERVER;



typedef enum
{
    TCP_CLIENT_CREAT = 0,
    TCP_CLIENT_CONFIG,
    TCP_CLIENT_CONNECT,
    TCP_CLIENT_WAIT_HAND,
    TCP_CLIENT_IDLE,
    TCP_CLIENT_CLOSE,
    TCP_CLIENT_STATE_MANAGE,
}VIRTUAL_TCP_CLIENT;




struct sockaddr_in get_tcp_server_sock(void);


uint8_t get_server_runnint_state(void);


typedef struct 
{
    void (*fun)(void*);
}virtual_tcp_fsm;


#endif


