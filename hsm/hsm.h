/**
 * @file hsm.h
 * @brief 层级状态机(Hierarchical State Machine)头文件
 */

 #ifndef HSM_H
 #define HSM_H
 
 #include <stdint.h>
 #include <stdbool.h>
 
 /* 状态机相关定义 */
 #define HSM_MAX_STATES      20   /* 最大状态数 */
 #define HSM_MAX_EVENTS      30   /* 最大事件数 */
 #define HSM_MAX_TRANSITIONS 50   /* 最大转换数 */
 #define HSM_MAX_NESTING     10   /* 最大嵌套层级 */
 
 /* 系统事件定义 */
 #define HSM_EVENT_ENTRY    0xFFF0  /* 进入状态事件 */
 #define HSM_EVENT_EXIT     0xFFF1  /* 退出状态事件 */
 #define HSM_EVENT_INIT     0xFFF2  /* 初始化事件 */
 
 /* 事件类型定义 */
 typedef uint16_t EventType;
 
 /* 事件结构体 */
 typedef struct {
     EventType type;        /* 事件类型 */
     void*     data;        /* 事件数据 */
 } Event;
 
 /* 前向声明 */
 typedef struct State State;
 typedef struct HSM HSM;
 
 /* 状态处理函数类型 */
 typedef bool (*StateHandler)(HSM* hsm, const Event* event);
 
 /* 状态结构体 */
 struct State {
     const char*  name;        /* 状态名称 */
     StateHandler handler;     /* 状态处理函数 */
     State*       parent;      /* 父状态 */
 };
 
 /* 状态转换结构体 */
 typedef struct {
     State*    source;      /* 源状态 */
     EventType eventType;   /* 触发事件类型 */
     State*    target;      /* 目标状态 */
     bool      (*guard)(HSM* hsm, const Event* event);  /* 转换条件 */
     void      (*action)(HSM* hsm, const Event* event); /* 转换动作 */
 } Transition;
 
 /* 层级状态机结构体 */
 struct HSM {
     State*      currentState;  /* 当前状态 */
     State*      states[HSM_MAX_STATES];           /* 状态列表 */
     uint8_t     stateCount;                       /* 状态数量 */
     Transition  transitions[HSM_MAX_TRANSITIONS]; /* 转换列表 */
     uint8_t     transitionCount;                  /* 转换数量 */
     void*       context;                          /* 用户上下文 */
 };
 
 /**
  * @brief 初始化状态机
  * @param hsm 状态机指针
  * @param initialState 初始状态
  * @param context 用户上下文
  */
 void hsm_init(HSM* hsm, State* initialState, void* context);
 
 /**
  * @brief 添加状态
  * @param hsm 状态机指针
  * @param state 要添加的状态
  * @param parent 父状态（如果没有父状态，设为NULL）
  * @return 成功返回true，失败返回false
  */
 bool hsm_add_state(HSM* hsm, State* state, State* parent);
 
 /**
  * @brief 添加转换
  * @param hsm 状态机指针
  * @param source 源状态
  * @param eventType 事件类型
  * @param target 目标状态
  * @param guard 转换条件（可为NULL）
  * @param action 转换动作（可为NULL）
  * @return 成功返回true，失败返回false
  */
 bool hsm_add_transition(HSM* hsm, State* source, EventType eventType, 
                        State* target, bool (*guard)(HSM*, const Event*), 
                        void (*action)(HSM*, const Event*));
 
 /**
  * @brief 状态转换
  * @param hsm 状态机指针
  * @param target 目标状态
  */
 void hsm_transition_to(HSM* hsm, State* target);
 
 /**
  * @brief 处理事件
  * @param hsm 状态机指针
  * @param event 事件指针
  * @return 事件被处理返回true，否则返回false
  */
 bool hsm_dispatch(HSM* hsm, const Event* event);
 
 /**
  * @brief 获取当前状态
  * @param hsm 状态机指针
  * @return 当前状态指针
  */
 State* hsm_get_current_state(HSM* hsm);
 
 /**
  * @brief 获取用户上下文
  * @param hsm 状态机指针
  * @return 用户上下文指针
  */
 void* hsm_get_context(HSM* hsm);
 
 /**
  * @brief 设置用户上下文
  * @param hsm 状态机指针
  * @param context 用户上下文
  */
 void hsm_set_context(HSM* hsm, void* context);
 
 #endif /* HSM_H */