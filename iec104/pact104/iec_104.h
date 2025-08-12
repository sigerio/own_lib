#ifndef __IEC_104_H__
#define __IEC_104_H__


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
/******************************************************
I帧:  06 00 02 00   -> 0110 0000 0010 0000 -> 实际上发送序列和接收序列最低位固定是0来标记是I帧，所以发送序号应为0011 即3，接收序列应为0001，即1
S帧： 01 00 06 00   -> 0001 0000 0110 0000 -> 不包含发送序列，所以01 00仅用于标识该帧为S帧，接收序列最低位预留，所以接受序列实际为0011，即3；
U帧： 43 00 00 00   -> 0011 0100 0000 0000 -> U帧没有发送序列和 接收序列，所有字节用于编写功能码，其特殊点在于第一字节的最低两位始终为1，即其格式应为xx11 xxxx 0000 0000

*****************************************************/

typedef struct 
{
    uint8_t start;  //0x68H
    uint8_t length; //指从length之后，直到校验码之前的字节总数, 
    pact_104_APCI_control_send_no control_send_no; //控制域共4字节，前两字节存储发送序号，后两字节存储接收序号，以防止数据丢失
    pact_104_APCI_control_recv_no control_recv_no;

    /*
    此处可增添私有扩展协议，标准格式内APCI一共6字节，length字段可扩充
    */

}pact_104_APCI; //



#endif


