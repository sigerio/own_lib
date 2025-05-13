/**
 * @file hsm_example.c
 * @brief 层级状态机使用示例
 */

 #include <stdio.h>
 #include "hsm.h"
 
 /* 定义事件类型 */
 enum {
     EVENT_BUTTON_PRESS = 1,
     EVENT_TIMEOUT,
     EVENT_ERROR
 };
 
 /* 定义应用上下文 */
 typedef struct {
     uint32_t counter;
     uint32_t errorCode;
 } AppContext;
 
 /* 定义状态处理函数 */
 bool state_idle_handler(HSM* hsm, const Event* event);
 bool state_active_handler(HSM* hsm, const Event* event);
 bool state_running_handler(HSM* hsm, const Event* event);
 bool state_paused_handler(HSM* hsm, const Event* event);
 bool state_error_handler(HSM* hsm, const Event* event);
 
 /* 定义状态 */
 State stateIdle = {"IDLE", state_idle_handler, NULL};
 State stateActive = {"ACTIVE", state_active_handler, NULL};
 State stateRunning = {"RUNNING", state_running_handler, &stateActive};
 State statePaused = {"PAUSED", state_paused_handler, &stateActive};
 State stateError = {"ERROR", state_error_handler, NULL};
 
 /* 转换条件示例 */
 bool check_error_condition(HSM* hsm, const Event* event) {
     AppContext* context = (AppContext*)hsm_get_context(hsm);
     return (context->errorCode != 0);
 }
 
 /* 转换动作示例 */
 void reset_counter_action(HSM* hsm, const Event* event) {
     AppContext* context = (AppContext*)hsm_get_context(hsm);
     context->counter = 0;
     printf("计数器已重置\n");
 }
 
 /* 状态处理函数实现 */
 bool state_idle_handler(HSM* hsm, const Event* event) {
     AppContext* context = (AppContext*)hsm_get_context(hsm);
     
     switch (event->type) {
         case HSM_EVENT_ENTRY:
             printf("进入IDLE状态\n");
             return true;
             
         case HSM_EVENT_EXIT:
             printf("退出IDLE状态\n");
             return true;
             
         case EVENT_BUTTON_PRESS:
             printf("按钮按下，从IDLE转到RUNNING\n");
             return false; /* 让状态机处理转换 */
     }
     return false;
 }
 
 bool state_active_handler(HSM* hsm, const Event* event) {
     AppContext* context = (AppContext*)hsm_get_context(hsm);
     
     switch (event->type) {
         case HSM_EVENT_ENTRY:
             printf("进入ACTIVE状态（父状态）\n");
             return true;
             
         case HSM_EVENT_EXIT:
             printf("退出ACTIVE状态（父状态）\n");
             return true;
             
         case EVENT_ERROR:
             printf("错误发生，从ACTIVE转到ERROR\n");
             context->errorCode = 1;
             return false; /* 让状态机处理转换 */
     }
     return false;
 }
 
 bool state_running_handler(HSM* hsm, const Event* event) {
     AppContext* context = (AppContext*)hsm_get_context(hsm);
     
     switch (event->type) {
         case HSM_EVENT_ENTRY:
             printf("进入RUNNING状态\n");
             return true;
             
         case HSM_EVENT_EXIT:
             printf("退出RUNNING状态\n");
             return true;
             
         case EVENT_BUTTON_PRESS:
             printf("按钮按下，从RUNNING转到PAUSED\n");
             return false; /* 让状态机处理转换 */
             
         case EVENT_TIMEOUT:
             context->counter++;
             printf("超时事件，计数器增加: %lu\n", (unsigned long)context->counter);
             return true;
     }
     return false;
 }
 
 bool state_paused_handler(HSM* hsm, const Event* event) {
     AppContext* context = (AppContext*)hsm_get_context(hsm);
     
     switch (event->type) {
         case HSM_EVENT_ENTRY:
             printf("进入PAUSED状态\n");
             return true;
             
         case HSM_EVENT_EXIT:
             printf("退出PAUSED状态\n");
             return true;
             
         case EVENT_BUTTON_PRESS:
             printf("按钮按下，从PAUSED转到RUNNING\n");
             return false; /* 让状态机处理转换 */
     }
     return false;
 }
 
 bool state_error_handler(HSM* hsm, const Event* event) {
     AppContext* context = (AppContext*)hsm_get_context(hsm);
     
     switch (event->type) {
         case HSM_EVENT_ENTRY:
             printf("进入ERROR状态\n");
             return true;
             
         case HSM_EVENT_EXIT:
             printf("退出ERROR状态\n");
             return true;
             
         case EVENT_BUTTON_PRESS:
             printf("按钮按下，从ERROR转到IDLE\n");
             context->errorCode = 0; /* 清除错误 */
             return false; /* 让状态机处理转换 */
     }
     return false;
 }
 
 /* 示例函数 */
 void hsm_example(void) {
     HSM hsm;
     AppContext context = {0, 0};
     
     /* 初始化状态机 */
     hsm_init(&hsm, &stateIdle, &context);
     
     /* 添加状态 */
     hsm_add_state(&hsm, &stateIdle, NULL);
     hsm_add_state(&hsm, &stateActive, NULL);
     hsm_add_state(&hsm, &stateRunning, &stateActive);
     hsm_add_state(&hsm, &statePaused, &stateActive);
     hsm_add_state(&hsm, &stateError, NULL);
     
     /* 添加转换 */
     hsm_add_transition(&hsm, &stateIdle, EVENT_BUTTON_PRESS, &stateRunning, NULL, NULL);
     hsm_add_transition(&hsm, &stateRunning, EVENT_BUTTON_PRESS, &statePaused, NULL, NULL);
     hsm_add_transition(&hsm, &statePaused, EVENT_BUTTON_PRESS, &stateRunning, NULL, NULL);
     hsm_add_transition(&hsm, &stateActive, EVENT_ERROR, &stateError, check_error_condition, NULL);
     hsm_add_transition(&hsm, &stateError, EVENT_BUTTON_PRESS, &stateIdle, NULL, reset_counter_action);
     
     /* 创建事件 */
     Event buttonEvent = {EVENT_BUTTON_PRESS, NULL};
     Event timeoutEvent = {EVENT_TIMEOUT, NULL};
     Event errorEvent = {EVENT_ERROR, NULL};
     
     /* 模拟事件序列 */
     printf("\n===== 开始状态机示例 =====\n\n");
     
     /* IDLE -> RUNNING */
     hsm_dispatch(&hsm, &buttonEvent);
     printf("当前状态: %s\n\n", hsm_get_current_state(&hsm)->name);
     
     /* 处理几个超时事件 */
     hsm_dispatch(&hsm, &timeoutEvent);
     hsm_dispatch(&hsm, &timeoutEvent);
     printf("当前状态: %s\n\n", hsm_get_current_state(&hsm)->name);
     
     /* RUNNING -> PAUSED */
     hsm_dispatch(&hsm, &buttonEvent);
     printf("当前状态: %s\n\n", hsm_get_current_state(&hsm)->name);
     
     /* PAUSED -> RUNNING */
     hsm_dispatch(&hsm, &buttonEvent);
     printf("当前状态: %s\n\n", hsm_get_current_state(&hsm)->name);
     
     /* 触发错误 */
     hsm_dispatch(&hsm, &errorEvent);
     printf("当前状态: %s\n\n", hsm_get_current_state(&hsm)->name);
     
     /* ERROR -> IDLE */
     hsm_dispatch(&hsm, &buttonEvent);
     printf("当前状态: %s\n\n", hsm_get_current_state(&hsm)->name);
     
     printf("===== 状态机示例结束 =====\n");
 }
 
 #if 1
 /* 主函数 */
 int main(void) {
     hsm_example();
     return 0;
 }
 #endif