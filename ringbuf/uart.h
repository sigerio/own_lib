#ifndef _UART_H
#define _UART_H
#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
int open_port(void);
int uart_set(int fd, int baudrate, int bits, char parity, int stop, char flow);
void uart_task_init(void);
void sendtask(int fd);
void receivetask(int fd);
void test_check(void);
void test_task_init(void);
#endif
