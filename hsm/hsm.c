/**
 * @file hsm.c
 * @brief 层级状态机(Hierarchical State Machine)实现
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* 状态机相关定义 */
#define HSM_MAX_STATES 20      /* 最大状态数 */
#define HSM_MAX_EVENTS 30      /* 最大事件数 */
#define HSM_MAX_TRANSITIONS 50 /* 最大转换数 */
#define HSM_MAX_NESTING 10     /* 最大嵌套层级 */

/* 事件类型定义 */
typedef uint16_t EventType;

/* 事件结构体 */
typedef struct
{
    EventType type; /* 事件类型 */
    void *data;     /* 事件数据 */
} Event;

/* 前向声明 */
typedef struct State State;
typedef struct HSM HSM;

/* 状态处理函数类型 */
typedef bool (*StateHandler)(HSM *hsm, const Event *event);

/* 状态结构体 */
struct State
{
    const char *name;     /* 状态名称 */
    StateHandler handler; /* 状态处理函数 */
    State *parent;        /* 父状态 */
};

/* 状态转换结构体 */
typedef struct
{
    State *source;                                /* 源状态 */
    EventType eventType;                          /* 触发事件类型 */
    State *target;                                /* 目标状态 */
    bool (*guard)(HSM *hsm, const Event *event);  /* 转换条件 */
    void (*action)(HSM *hsm, const Event *event); /* 转换动作 */
} Transition;

/* 层级状态机结构体 */
struct HSM
{
    State *currentState;                         /* 当前状态 */
    State *states[HSM_MAX_STATES];               /* 状态列表 */
    uint8_t stateCount;                          /* 状态数量 */
    Transition transitions[HSM_MAX_TRANSITIONS]; /* 转换列表 */
    uint8_t transitionCount;                     /* 转换数量 */
    void *context;                               /* 用户上下文 */
};
void hsm_transition_to(HSM *hsm, State *target);
/* 系统事件定义 */
#define HSM_EVENT_ENTRY 0xFFF0 /* 进入状态事件 */
#define HSM_EVENT_EXIT 0xFFF1  /* 退出状态事件 */
#define HSM_EVENT_INIT 0xFFF2  /* 初始化事件 */

/* 创建事件 */
static Event createEvent(EventType type, void *data)
{
    Event event;
    event.type = type;
    event.data = data;
    return event;
}

/* 初始化状态机 */
void hsm_init(HSM *hsm, State *initialState, void *context)
{
    if (hsm == NULL || initialState == NULL)
    {
        return;
    }

    /* 初始化状态机结构 */
    hsm->currentState = NULL;
    hsm->stateCount = 0;
    hsm->transitionCount = 0;
    hsm->context = context;

    /* 设置初始状态 */
    hsm_transition_to(hsm, initialState);
}

/* 添加状态 */
bool hsm_add_state(HSM *hsm, State *state, State *parent)
{
    if (hsm == NULL || state == NULL || hsm->stateCount >= HSM_MAX_STATES)
    {
        return false;
    }

    state->parent = parent;
    hsm->states[hsm->stateCount++] = state;
    return true;
}

/* 添加转换 */
bool hsm_add_transition(HSM *hsm, State *source, EventType eventType,
                        State *target, bool (*guard)(HSM *, const Event *),
                        void (*action)(HSM *, const Event *))
{
    if (hsm == NULL || source == NULL || target == NULL ||
        hsm->transitionCount >= HSM_MAX_TRANSITIONS)
    {
        return false;
    }

    Transition *transition = &hsm->transitions[hsm->transitionCount++];
    transition->source = source;
    transition->eventType = eventType;
    transition->target = target;
    transition->guard = guard;
    transition->action = action;

    return true;
}

static Transition *hsm_find_transition(HSM *hsm, State *state, EventType eventType)
{
    for (uint8_t i = 0; i < hsm->transitionCount; i++)
    {
        Transition *transition = &hsm->transitions[i];
        if (transition->source == state && transition->eventType == eventType)
        {
            /* 检查转换条件 */
            if (transition->guard == NULL)
            {
                return transition;
            }
            else
            {
                Event tempEvent = createEvent(eventType, NULL);
                if (transition->guard(hsm, &tempEvent))
                {
                    return transition;
                }
            }
        }
    }
    return NULL;
}

/* 获取状态路径 */
static uint8_t hsm_get_state_path(State *state, State *path[], uint8_t maxDepth)
{
    uint8_t depth = 0;

    while (state != NULL && depth < maxDepth)
    {
        path[depth++] = state;
        state = state->parent;
    }

    return depth;
}

/* 查找最近公共祖先 */
static State *hsm_find_lca(State *s1, State *s2)
{
    State *path1[HSM_MAX_NESTING];
    State *path2[HSM_MAX_NESTING];

    uint8_t depth1 = hsm_get_state_path(s1, path1, HSM_MAX_NESTING);
    uint8_t depth2 = hsm_get_state_path(s2, path2, HSM_MAX_NESTING);

    State *lca = NULL;

    /* 从根状态开始比较 */
    for (int i = depth1 - 1, j = depth2 - 1; i >= 0 && j >= 0; i--, j--)
    {
        if (path1[i] == path2[j])
        {
            lca = path1[i];
        }
        else
        {
            break;
        }
    }

    return lca;
}

/* 状态转换 */
void hsm_transition_to(HSM *hsm, State *target)
{
    if (hsm == NULL || target == NULL)
    {
        return;
    }

    State *source = hsm->currentState;

    /* 如果当前没有状态，直接进入目标状态 */
    if (source == NULL)
    {
        hsm->currentState = target;

        /* 发送进入事件 */
        Event entryEvent = createEvent(HSM_EVENT_ENTRY, NULL);
        target->handler(hsm, &entryEvent);

        /* 发送初始化事件 */
        Event initEvent = createEvent(HSM_EVENT_INIT, NULL);
        target->handler(hsm, &initEvent);

        return;
    }

    /* 找到最近公共祖先 */
    State *lca = hsm_find_lca(source, target);

    /* 退出当前状态直到LCA */
    State *state = source;
    Event exitEvent = createEvent(HSM_EVENT_EXIT, NULL);

    while (state != lca)
    {
        state->handler(hsm, &exitEvent);
        state = state->parent;
    }

    /* 更新当前状态 */
    hsm->currentState = target;

    /* 从LCA进入到目标状态 */
    State *entryPath[HSM_MAX_NESTING];
    uint8_t entryDepth = 0;

    state = target;
    while (state != lca)
    {
        entryPath[entryDepth++] = state;
        state = state->parent;
    }

    /* 按照从父到子的顺序发送进入事件 */
    Event entryEvent = createEvent(HSM_EVENT_ENTRY, NULL);
    for (int i = entryDepth - 1; i >= 0; i--)
    {
        entryPath[i]->handler(hsm, &entryEvent);
    }

    /* 发送初始化事件给目标状态 */
    Event initEvent = createEvent(HSM_EVENT_INIT, NULL);
    target->handler(hsm, &initEvent);
}

/* 处理事件 */
bool hsm_dispatch(HSM *hsm, const Event *event)
{
    if (hsm == NULL || event == NULL || hsm->currentState == NULL)
    {
        return false;
    }

    /* 从当前状态开始向上查找能处理事件的状态 */
    State *state = hsm->currentState;
    while (state != NULL)
    {
        /* 查找转换 */
        Transition *transition = hsm_find_transition(hsm, state, event->type);
        if (transition != NULL)
        {
            /* 执行转换动作 */
            if (transition->action != NULL)
            {
                transition->action(hsm, event);
            }

            /* 执行状态转换 */
            hsm_transition_to(hsm, transition->target);
            return true;
        }

        /* 让当前状态处理事件 */
        if (state->handler(hsm, event))
        {
            return true; /* 事件已处理 */
        }

        /* 向父状态传递事件 */
        state = state->parent;
    }

    return false; /* 事件未处理 */
}

/* 获取当前状态 */
State *hsm_get_current_state(HSM *hsm)
{
    return (hsm != NULL) ? hsm->currentState : NULL;
}

/* 获取用户上下文 */
void *hsm_get_context(HSM *hsm)
{
    return (hsm != NULL) ? hsm->context : NULL;
}

/* 设置用户上下文 */
void hsm_set_context(HSM *hsm, void *context)
{
    if (hsm != NULL)
    {
        hsm->context = context;
    }
}

/* 示例使用方法 */
#if 0
 /* 定义事件类型 */
 enum {
     EVENT_BUTTON_PRESS = 1,
     EVENT_TIMEOUT,
     EVENT_ERROR
 };
 
 /* 定义状态处理函数 */
 bool state_off_handler(HSM* hsm, const Event* event);
 bool state_on_handler(HSM* hsm, const Event* event);
 bool state_error_handler(HSM* hsm, const Event* event);
 
 /* 定义状态 */
 State stateOff = {"OFF", state_off_handler, NULL};
 State stateOn = {"ON", state_on_handler, NULL};
 State stateError = {"ERROR", state_error_handler, NULL};
 
 /* 状态处理函数实现 */
 bool state_off_handler(HSM* hsm, const Event* event) {
     switch (event->type) {
         case HSM_EVENT_ENTRY:
             printf("进入OFF状态\n");
             return true;
             
         case HSM_EVENT_EXIT:
             printf("退出OFF状态\n");
             return true;
             
         case EVENT_BUTTON_PRESS:
             printf("按钮按下，从OFF转到ON\n");
             hsm_transition_to(hsm, &stateOn);
             return true;
     }
     return false;
 }
 
 bool state_on_handler(HSM* hsm, const Event* event) {
     switch (event->type) {
         case HSM_EVENT_ENTRY:
             printf("进入ON状态\n");
             return true;
             
         case HSM_EVENT_EXIT:
             printf("退出ON状态\n");
             return true;
             
         case EVENT_BUTTON_PRESS:
             printf("按钮按下，从ON转到OFF\n");
             hsm_transition_to(hsm, &stateOff);
             return true;
             
         case EVENT_ERROR:
             printf("错误发生，从ON转到ERROR\n");
             hsm_transition_to(hsm, &stateError);
             return true;
     }
     return false;
 }
 
 bool state_error_handler(HSM* hsm, const Event* event) {
     switch (event->type) {
         case HSM_EVENT_ENTRY:
             printf("进入ERROR状态\n");
             return true;
             
         case HSM_EVENT_EXIT:
             printf("退出ERROR状态\n");
             return true;
             
         case EVENT_BUTTON_PRESS:
             printf("按钮按下，从ERROR转到OFF\n");
             hsm_transition_to(hsm, &stateOff);
             return true;
     }
     return false;
 }
 
 /* 主函数示例 */
 int main(void) {
     HSM hsm;
     
     /* 初始化状态机 */
     hsm_init(&hsm, &stateOff, NULL);
     
     /* 添加状态 */
     hsm_add_state(&hsm, &stateOff, NULL);
     hsm_add_state(&hsm, &stateOn, NULL);
     hsm_add_state(&hsm, &stateError, NULL);
     
     /* 添加转换 */
     hsm_add_transition(&hsm, &stateOff, EVENT_BUTTON_PRESS, &stateOn, NULL, NULL);
     hsm_add_transition(&hsm, &stateOn, EVENT_BUTTON_PRESS, &stateOff, NULL, NULL);
     hsm_add_transition(&hsm, &stateOn, EVENT_ERROR, &stateError, NULL, NULL);
     hsm_add_transition(&hsm, &stateError, EVENT_BUTTON_PRESS, &stateOff, NULL, NULL);
     
     /* 处理事件 */
     Event buttonEvent = createEvent(EVENT_BUTTON_PRESS, NULL);
     hsm_dispatch(&hsm, &buttonEvent); /* OFF -> ON */
     
     Event errorEvent = createEvent(EVENT_ERROR, NULL);
     hsm_dispatch(&hsm, &errorEvent);  /* ON -> ERROR */
     
     hsm_dispatch(&hsm, &buttonEvent); /* ERROR -> OFF */
     
     return 0;
 }
#endif