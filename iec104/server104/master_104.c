#include "master_104_define.h"
#include "../pact104/iec_104.h"
#include "../virtual_tcp/tcp_define.h"

/*this file will manage the fsm of 104 server  , we called this 104A*/

master_104_manage master_104_info;


void set_master_104_work_state(uint8_t next_state)
{
    master_104_info.master_next_state = next_state;

}

void master_104_run_init(void* arg)
{
    while(get_server_runnint_state() != TCP_CLIENT_IDLE);

    set_master_104_work_state(MASTER_104_TESTER);

}



void master_104_run_tester(void* arg)
{
    
}



