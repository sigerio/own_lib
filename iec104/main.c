#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>




#include "mailbox/message_box.h"
#include "virtual_tcp/tcp_server_type.h"
#include "virtual_tcp/tcp_client_type.h"


void* client_104(void* arg) {
    while(1) {
        
        
    }
    return NULL;
}

void* server_104(void* arg) {
    while(1) {
        
        
    }
    return NULL;
}


int main(void)
{
    pthread_t tid1, tid2, tid3, tid4;
    int ret = 0;

    // register_message_list();
    printf("register\n");
    

    ret = pthread_create(&tid1, NULL, client_104, NULL);
    if(ret != 0) {
        printf("Create 104 client thread error!\n");
        return -1;
    }

    ret = pthread_create(&tid2, NULL, server_104, NULL);
    if(ret != 0) {
        printf("Create 104 server thread error!\n");
        return -1;
    }

    ret = pthread_create(&tid3, NULL, virtual_tcp_server_run_fsm, NULL);
    if(ret != 0) {
        printf("Create tcp server thread error!\n");
        return -1;
    }

    ret = pthread_create(&tid4, NULL, virtual_tcp_client_run_fsm, NULL);
    if(ret != 0) {
        printf("Create tcp server thread error!\n");
        return -1;
    }

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);
    pthread_join(tid4, NULL);
    return 0;
}
