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
// #include "crc32.h"
#include "uart.h"
// #include "ringbuffer.h"

#define THREAD 


void* input_thread(void* arg) {
    while(1) {
        test_in_data();
        usleep(1000*80);
    }
    return NULL;
}

void* check_thread(void* arg) {
    while(1) {
        test_check_data();
        usleep(1000*100);
    }
    return NULL;
}


int main(void)
{
    pthread_t tid1, tid2;
    int ret = 0;

    uart_task_init();
#ifdef THREAD
    ret = pthread_create(&tid1, NULL, input_thread, NULL);
    if(ret != 0) {
        printf("Create input thread error!\n");
        return -1;
    }

    ret = pthread_create(&tid2, NULL, check_thread, NULL);
    if(ret != 0) {
        printf("Create check thread error!\n");
        return -1;
    }

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

#endif

#ifdef LUO_JI
    while (1)
    {
        /* code */
        test_check();
        // usleep(1000*10);
        sleep(1);
    }
#endif    
    return 0;
}
