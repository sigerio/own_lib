#ifndef __IEC_104_H__
#define __IEC_104_H__
#include <stdint.h>

/******************************************************
I帧:  06 00 02 00   -> 0110 0000 0010 0000 -> 实际上发送序列和接收序列最低位固定是0来标记是I帧，所以发送序号应为0011 即3，接收序列应为0001，即1
S帧： 01 00 06 00   -> 0001 0000 0110 0000 -> 不包含发送序列，所以01 00仅用于标识该帧为S帧，接收序列最低位预留，所以接受序列实际为0011，即3；
U帧： 43 00 00 00   -> 0011 0100 0000 0000 -> U帧没有发送序列和 接收序列，所有字节用于编写功能码，其特殊点在于第一字节的最低两位始终为1，即其格式应为xx11 xxxx 0000 0000


S帧不包含发送序列，只用来确认接收序列
U帧只作为命令帧，不包含ASDU


U帧只能由U帧来确认
I帧信息可以由I帧或S帧来确认，当一方接收到I帧且没有I帧需要发送时，即可发送S帧
同时，假设A发送3帧，但是只收到一帧S帧，帧中只确认了第1帧被接收到，意味着第2第3帧丢失，此时A站需立刻停止发送，等待超时后主动关闭链接，随后再主动打开


为避免确认帧S帧频繁响应，协议中规定了未被确认的I帧APDU最大数目，从0开始至N-1，接下来有w < 2/3k;  k < N；发送方未被确认的帧到达k个时，主动停止发送，接收方每收到w个帧才发送一次S帧，

*****************************************************/


#define PACT_104_ACPI_U_FORM_START_ACT      0x0007  //启动数据传输-服务端控制客户端开始传输数据
#define PACT_104_ACPI_U_FORM_START_CON      0x000B  //客户端回复服务端，已确认开启数据传输

#define PACT_104_ACPI_U_FORM_STOP_ACT       0x0013  //主站 控制从站停止数据传输
#define PACT_104_ACPI_U_FORM_STOP_CON       0x0023  //从站回复主站确认停止数据传输



/*在规定时间内，链路上没有消息传递，应周期性发送测试帧，当收到任意帧后重置超时时长*/
#define PACT_104_ACPI_U_FORM_TESTER_ACT     0x0043  // 主/从确认链路是否正常
#define PACT_104_ACPI_U_FORM_TESTER_CON     0x0083  // 回复确认帧


#define MAX_NOT_CONFIRM_N                   0x0100  //当发送方未被确认的帧到达该值后，断开连接
#define MAX_NEED_TO_CONFIRM_NO              (MAX_NOT_CONFIRM_N-1) // 当发送方未被确认的帧到达该值后，断开连接
#define ACK_S_EVERY_I_NO                    (MAX_NEED_TO_CONFIRM_NO/2) // 接收方每接收到该数目的I帧，立刻发送S帧进行回复


typedef union 
{
    struct 
    {
        uint16_t lowest_bit:1;//I form = 00//if lowest_bit = 1; send_no = 1;
        uint16_t send_no:15;
    }control_I_form_send_no;

    struct 
    {
        uint16_t lowest_bit:2;// S form = 01
        uint16_t send_no:15;
    }control_S_form_send_no;

    struct 
    {
        uint16_t lowest_bit:2;// U form = 11
        uint16_t send_no:14;
    }control_U_form_send_no;    //U帧的填写不区分位数，都有固定的功能码，可以忽略，此处只是用于加强记忆

    
    uint16_t send_no;
}pact_104_APCI_control_send_no;


typedef union 
{
    struct 
    {
        uint16_t lowest_bit:1;//form = 00
        uint16_t recv_no:15;
    }control_form_recv_no;

    
    uint16_t recv_no;
}pact_104_APCI_control_recv_no;


typedef struct 
{
    uint8_t start;  //0x68H
    uint8_t length; //指从length之后，所有字节
    pact_104_APCI_control_send_no control_send_no; //控制域共4字节，前两字节存储发送序号，后两字节存储接收序号，以防止数据丢失
    pact_104_APCI_control_recv_no control_recv_no; //发送序列号和接收序列号初始都为0

    /*
    此处可增添私有扩展协议，标准格式内APCI一共6字节，length字段可扩充
    */
   uint32_t dst_addr;
   uint32_t src_addr;

}pact_104_APCI; //


typedef union 
{
    struct 
    {
        uint8_t n:7;
        uint8_t sq:1; //SQ位用于指示后续信息体IOA是否连续，0->不连续  1->连续且依次递增
    }vsq_bit;
    uint8_t vsq;
    
}pact_104_ASDU_vsq;


typedef union 
{
    struct 
    {
        uint16_t n:13;
        uint16_t PN:2;//当从站接收到无法识别或着无法操作的控制指令时，回复帧中应将该位置1；表示消极确认，意味着无法识别或操作，反之置0；
        uint16_t T:1; //测试模式，1->仅用于消息测试，不执行控制等命令

    }cot_bit;
    uint16_t cot;
    
}pact_104_ASDU_cot;



typedef struct 
{
    uint8_t IOA[3]; //三字节的IOA，即信息对象地址，例如0x000a01，则IOA[0] = 0x01;IOA[1] = 0x0A; IOA[2] = 0x00;
    uint8_t* info;//信息对象数据格式，其格式和长度均有type_id决定
}pact_104_ASDU_info_obj;



typedef struct 
{
    uint8_t type_id;    //帧类型号，比如对时，总召等,同时帧类型也决定了消息体的长度，同类型的消息体其长度是一致的，不同类型的消息体需要通过不同的消息帧来发送
    pact_104_ASDU_vsq VSQ; //消息体长度,也叫做可变结构限定词
    pact_104_ASDU_cot COT;   //消息传送原因
    uint8_t ORG;    //识别地址，一般指消息发送方地址，通常忽略不计
    uint16_t COA;    //公共地址，从设备发出去的消息需要用此来标记设备编号，站控下发需要此来标记消息去向， 0禁止使用，65535标记为广播，广播出去的消息在被回复时，必须带上回复方的地址，1~65534
    pact_104_ASDU_info_obj info_obj;
}pact_104_ASDU;

typedef struct 
{
    pact_104_APCI apci;
    pact_104_ASDU asdu;
    /* data */
}pact_104_APDU;




#endif


