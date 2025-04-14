
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include "uart.h"
#include "ringbuffer.h"
#include "check.h"
#include <stdint.h>
#include <time.h>

/*
 * 'open_port()' - Open serial port 1.
 *
 * Returns the file descriptor on success or -1 on error.
 */

int open_port(void)
{
    int fd; /* File descriptor for the port */

    fd = open("/dev/ttyS2", O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1)
    {
        printf("open port fail\n");
        return -1;
    }

    printf("open port ok fd is %d\n", fd);
    return (fd);
}

int uart_set(int fd, int baudrate, int bits, char parity, int stop, char flow)
{
    struct termios termios_uart;
    int ret = 0;
    speed_t uart_speed = 0;

    /* 获取串口属性 */
    memset(&termios_uart, 0, sizeof(termios_uart));
    ret = tcgetattr(fd, &termios_uart);
    if (ret == -1)
    {
        printf("tcgetattr failed\n");
        return -1;
    }

    //__print_termios(&termios_uart);

    /* 设置波特率 */
    switch (baudrate)
    {
    case 0:
        uart_speed = B0;
        break;
    case 50:
        uart_speed = B50;
        break;
    case 75:
        uart_speed = B75;
        break;
    case 110:
        uart_speed = B110;
        break;
    case 134:
        uart_speed = B134;
        break;
    case 150:
        uart_speed = B150;
        break;
    case 200:
        uart_speed = B200;
        break;
    case 300:
        uart_speed = B300;
        break;
    case 600:
        uart_speed = B600;
        break;
    case 1200:
        uart_speed = B1200;
        break;
    case 1800:
        uart_speed = B1800;
        break;
    case 2400:
        uart_speed = B2400;
        break;
    case 4800:
        uart_speed = B4800;
        break;
    case 9600:
        uart_speed = B9600;
        break;
    case 19200:
        uart_speed = B19200;
        break;
    case 38400:
        uart_speed = B38400;
        break;
    case 57600:
        uart_speed = B57600;
        break;
    case 115200:
        uart_speed = B115200;
        break;
    case 230400:
        uart_speed = B230400;
        break;
    default:
        printf("Baud rate not supported\n");
        return -1;
    }
    cfsetspeed(&termios_uart, uart_speed);

    /* 设置数据位 */
    switch (bits)
    {
    case 5: /* 数据位5 */
        termios_uart.c_cflag &= ~CSIZE;
        termios_uart.c_cflag |= CS5;
        break;

    case 6: /* 数据位6 */
        termios_uart.c_cflag &= ~CSIZE;
        termios_uart.c_cflag |= CS6;
        break;

    case 7: /* 数据位7 */
        termios_uart.c_cflag &= ~CSIZE;
        termios_uart.c_cflag |= CS7;
        break;

    case 8: /* 数据位8 */
        termios_uart.c_cflag &= ~CSIZE;
        termios_uart.c_cflag |= CS8;
        break;

    default:
        printf("Data bits not supported\n");
        return -1;
    }

    /* 设置校验位 */
    switch (parity)
    {
    case 'n': /* 无校验 */
    case 'N':
        termios_uart.c_cflag &= ~PARENB;
        termios_uart.c_iflag &= ~INPCK; /* 禁能输入奇偶校验 */
        break;

    case 'o': /* 奇校验 */
    case 'O':
        termios_uart.c_cflag |= PARENB;
        termios_uart.c_cflag |= PARODD;
        termios_uart.c_iflag |= INPCK; /* 使能输入奇偶校验 */
        // termios_uart.c_iflag |= ISTRIP;     /* 除去第八个位（奇偶校验位），如果这么写的话，最高位永远为0 */
        termios_uart.c_iflag &= ~ISTRIP;
        break;

    case 'e': /* 偶校验 */
    case 'E':
        termios_uart.c_cflag |= PARENB;
        termios_uart.c_cflag &= ~PARODD;
        termios_uart.c_iflag |= INPCK; /* 使能输入奇偶校验 */
        // termios_uart.c_iflag |= ISTRIP;     /* 除去第八个位（奇偶校验位），如果这么写的话，最高位永远为0 */
        termios_uart.c_iflag &= ~ISTRIP;
        break;

    default:
        printf("Parity not supported\n");
        return -1;
    }

    /* 设置停止位 */
    switch (stop)
    {
    case 1:
        termios_uart.c_cflag &= ~CSTOPB;
        break; /* 1个停止位 */
    case 2:
        termios_uart.c_cflag |= CSTOPB;
        break; /* 2个停止位 */
    default:
        printf("Stop bits not supported\n");
    }

    /* 设置流控 */
    switch (flow)
    {
    case 'n':
    case 'N': /* 无流控 */
        termios_uart.c_cflag &= ~CRTSCTS;
        termios_uart.c_iflag &= ~(IXON | IXOFF | IXANY);
        break;

    case 'h':
    case 'H': /* 硬件流控 */
        termios_uart.c_cflag |= CRTSCTS;
        termios_uart.c_iflag &= ~(IXON | IXOFF | IXANY);
        break;

    case 's':
    case 'S': /* 软件流控 */
        termios_uart.c_cflag &= ~CRTSCTS;
        termios_uart.c_iflag |= (IXON | IXOFF | IXANY);
        break;

    default:
        printf("Flow control parameter error\n");
        return -1;
    }

    /* 其他设置 */
    termios_uart.c_cflag |= CLOCAL; /* 忽略modem（调制解调器）控制线 */
    termios_uart.c_cflag |= CREAD;  /* 使能接收 */

    termios_uart.c_iflag &= ~ICRNL; /* 禁止将输入的CR转换为NL，如果不加这一行的话，串口接收到的0D会变成0A */

    /* 禁能执行定义（implementation-defined）输出处理，意思就是输出的某些特殊数
       据会作特殊处理，如果禁能的话那么就按原始数据输出 */
    termios_uart.c_oflag &= ~OPOST;

    /**
     *  设置本地模式位原始模式
     *  ICANON：规范输入模式，如果设置了那么退格等特殊字符会产生实际动作
     *  ECHO：则将输入字符回送到终端设备
     *  ECHOE：如果ICANON也设置了，那么收到ERASE字符后会从显示字符中擦除一个字符
     *         通俗点理解就是收到退格键后显示内容会往回删一个字符
     *  ISIG：使终端产生的信号起作用。（比如按ctrl+c可以使程序退出）
     */
    termios_uart.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    /**
     * 设置等待时间和最小接收字符
     * 这两个值只有在阻塞模式下有意义，也就是说open的时候不能传入O_NONBLOCK，
     * 如果经过了c_cc[VTIME]这么长时间，缓冲区内有数据，但是还没达到c_cc[VMIN]个
     * 数据，read也会返回。而如果当缓冲区内有了c_cc[VMIN]个数据时，无论等待时间
     * 是否到了c_cc[VTIME]，read都会返回，但返回值可能比c_cc[VMIN]还大。如果将
     * c_cc[VMIN]的值设置为0，那么当经过c_cc[VTIME]时间后read也会返回，返回值
     * 为0。如果将c_cc[VTIME]和c_cc[VMIN]都设置为0，那么程序运行的效果与设置
     * O_NONBLOCK类似，不同的是如果设置了O_NONBLOCK，那么在没有数据时read返回-1，
     * 而如果没有设置O_NONBLOCK，那么在没有数据时read返回的是0。
     */
    termios_uart.c_cc[VTIME] = 1; /* 设置等待时间，单位1/10秒 */
    termios_uart.c_cc[VMIN] = 1;  /* 最少读取一个字符 */

    tcflush(fd, TCIFLUSH); /* 清空读缓冲区 */

    /* 写入配置 */
    ret = tcsetattr(fd, TCSANOW, &termios_uart);
    if (ret == -1)
    {
        printf("tcsetattr failed\n");
    }

    return ret;
}

#define BUFLEN1 2
#define BUFLEN2 4
#define BUFLEN3 11
#define BUFLEN4 6
#define BUFLEN5 11
#define BUFLEN6 10
#define BUFLEN7 7
#define BUFLEN8 17
#define BUFLEN9 23

char mem_pool[512] = {0};

Ringbuffer ring = {0};

#define LEN(x) BUFLEN##x
// 分三包发送，整包无误，但是字节位于第二包
uint8_t buf1_1[23] = {
    0xAA,
    0x55,
};
uint8_t buf1_2[23] = {
    0x00,
    0x11,
    0x04,
    0x21,
};
uint8_t buf1_3[23] = {0x00, 0x00, 0x01, 0x4E, 0x20, 0x00, 0x04, 0x29, 0xDC, 0x0D, 0x0A};
// 分2包发送，整包无误
uint8_t buf2_1[23] = {
    0xAA,
    0x55,
    0x00,
    0x11,
    0x04,
    0x20,
};
uint8_t buf2_2[23] = {0x00, 0x00, 0x01, 0x4E, 0x20, 0x00, 0x02, 0xE7, 0x9D, 0x0D, 0x0A};

// 分两包，但是包校验错误
uint8_t buf3_1[23] = {
    0xAA,
    0x55,
    0x00,
    0x11,
    0x04,
    0x21,
    0x00,
    0x00,
    0x01,
    0x4E,
};
uint8_t buf3_2[23] = {0x20, 0x00, 0x04, 0x30, 0xDC, 0x0D, 0x0A}; // 29dc
// 整包无误
uint8_t buf4[23] = {0xAA, 0x55, 0x00, 0x11, 0x04, 0x21, 0x00, 0x00, 0x01, 0x4E, 0x20, 0x00, 0x04, 0x29, 0xDC, 0x0D, 0x0A};
// 整包头尾存在错乱数据
uint8_t buf5[23] = {0x12, 0x23, 0x34, 0xAA, 0x55, 0x00, 0x11, 0x04, 0x21, 0x00, 0x00, 0x01, 0x4E, 0x20, 0x00, 0x04, 0x29, 0xDC, 0x0D, 0x0A, 0x02, 0x03, 0x05};

uint8_t *buf_list[9] = {buf5, buf1_1, buf1_2, buf1_3, buf2_1, buf2_2, buf3_1, buf3_2,buf4,};
int idx = 0;
char buflen[9] = {LEN(9), LEN(1), LEN(2), LEN(3), LEN(4), LEN(5), LEN(6), LEN(7), LEN(8), };

void uart_task_init(void)
{
    ringbuffer_init(&ring, mem_pool, 512);
}

static uint16_t data_out_sum = 0;
static uint16_t data_in_sum = 0;
void test_check(void)
{
    int len = 0;
    int i = 0;

    uint8_t buf[128] = {0};
    

    static int in_data = 0;
    int n = 0;

    len = ringbuffer_put_force(&ring, buf_list[in_data], buflen[in_data]);
    printf("in_data = %d put in:  ",in_data);
    for(int q = 0; q<buflen[in_data];q++)
    {
        printf(" %02x",buf_list[in_data][q]);
    }
    printf("\n");
    put_in_que(&ring, len);
    // printf("in len:%d\n", len);
    data_in_sum+=buflen[in_data];
    in_data++;
    if (in_data >= 9)
        in_data = 0;
        
    
    len = get_que(&ring);
    len += ring.drop_data.remine_len;
    // printf("out len:%d\n", len);
    ringbuffer_view_data(&ring, buf, len);
    printf("len = %d  check data:",len);
    for(int j = 0; j<len;j++)
    {
        
        printf(" 0x%02x", buf[j]);
    }
    printf("\n");
    int res = check_farm(len,buf,&ring);
    if(res == 0)
    {
        printf("right: ");
        for (i = 0; i < len - ring.drop_data.remine_len; i++)
        {
            printf(" 0x%02x", buf[i]); 
        }
        printf("\n");
    }
    else
    {

        // printf("res = %d  len = %d,remine = %d drop = %d\n",res,len,ring.drop_data.remine_len,ring.drop_data.drop_len);
    }
    ringbuffer_update_rw_state(&ring);
    // printf("read index = %d\n",ring.read_index);

    data_out_sum+=ring.drop_data.drop_len;
    
    printf("----------------in %d ------out %d -------\n",data_in_sum,data_out_sum);
    printf("+++++++ wirte:  %d ++++++++++++ read  %d ++++++\n",ring.write_index,ring.read_index);
}

void test_in_data()
{
    static int in_data = 0;
    int n = 0;
    int len = 0;

    len = ringbuffer_put_force(&ring, buf_list[in_data], buflen[in_data]);
    printf("in_data = %d put in:  ",in_data);
    for(int q = 0; q<buflen[in_data];q++)
    {
        printf(" %02x",buf_list[in_data][q]);
    }
    printf("\n");
    put_in_que(&ring, len);
    // printf("in len:%d\n", len);
    data_in_sum+=buflen[in_data];
    in_data++;
    if (in_data > 9)
        in_data = 0;
        
}


void test_check_data()
{
    int len = 0;
    int i = 0;

    uint8_t buf[128] = {0};

    len = get_que(&ring);
    len += ring.drop_data.remine_len;
    // printf("out len:%d\n", len);
    ringbuffer_view_data(&ring, buf, len);
    printf("len = %d  check data:",len);
    for(int j = 0; j<len;j++)
    {
        
        printf(" 0x%02x", buf[j]);
    }
    printf("\n");
    int res = check_farm(len,buf,&ring);
    if(res == 0)
    {
        printf("right: ");
        for (i = 0; i < len - ring.drop_data.remine_len; i++)
        {
            printf(" 0x%02x", buf[i]); 
        }
        printf("\n");
    }
    else
    {

        // printf("res = %d  len = %d,remine = %d drop = %d\n",res,len,ring.drop_data.remine_len,ring.drop_data.drop_len);
    }
    ringbuffer_update_rw_state(&ring);
    // printf("read index = %d\n",ring.read_index);

    data_out_sum+=ring.drop_data.drop_len;
    
    printf("----------------in %d ------out %d -------\n",data_in_sum,data_out_sum);
    printf("+++++++ wirte:  %d ++++++++++++ read  %d ++++++\n",ring.write_index,ring.read_index);


}
