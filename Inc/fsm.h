/**
 ******************************************************************************
 * @file simpleamp.h
 * @author lx
 * @version
 * @date 2026-03-14
 * @brief FSM - 面向异构多核场景的有限状态机框架头文件
 ******************************************************************************
 * @attention
 * 该框架专为异构多核系统设计，支持不同核心间的高效状态机管理
 ******************************************************************************
 */

#ifndef _FSM_H_
#define _FSM_H_

 /* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdint.h>

typedef enum {
    FSM_EVENT_NONE = 0,
    FSM_EVENT_ENTRY = 1,
    FSM_EVENT_DO = 2,
    FSM_EVENT_EXIT = 3,
    FSM_EVENT_USER_START = 4,
} FSM_EVENT_T;

typedef struct fsm_t {
    void (*lastState)(struct fsm_t *pFSM, int event);
    void (*currentState)(struct fsm_t *pFSM, int event);
    void (*transitionAction)(void);
} FSM_T;

typedef void (*FSM_STATE_T)(FSM_T *pFSM, int event);

typedef void (*FSM_ACTION_T)(void);

void FSMInit(FSM_T *pFSM, FSM_STATE_T initialState);

void FSMDispatch(FSM_T *pFSM, int event);

void FSMDo(FSM_T *pFSM);

void FSMTrasform(FSM_T *pFSM, FSM_STATE_T newState, FSM_ACTION_T transitionAction);

#endif // _FSM_H_
