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




#include "message_box.h"




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
    pthread_t tid1, tid2;
    int ret = 0;
    
    register_message_list();

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

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
  
    return 0;
}
