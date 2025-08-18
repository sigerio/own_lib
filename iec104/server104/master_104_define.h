#ifndef __MASTER_104_DEFINE_H__
#define __MASTER_104_DEFINE_H__

#include <stdint.h>



typedef enum
{
    MASTER_104_INIT = 0, //所谓初始化即主从站建立通讯连接的过程，
    MASTER_104_TESTER, //该过程中必须完成U帧的TERSET的下发与确认
    MASTER_104_STARTDT,//完成STARTDT帧的下发，消息确认及超时等待机制

    MASTER_104_STOPDT,//完成STOPDT帧的下发，消息确认及超时等待机制     /*任意超时信息，在主站主动关闭连接后，主站需要重新启用该连接，防止通讯断裂无法恢复*/
    

}MASTER_104_FSM;





typedef struct 
{
    uint8_t master_cur_state;
    uint8_t master_next_state;

    uint64_t master_wait_time;
}master_104_manage;











#endif


