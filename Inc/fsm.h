/**
 ******************************************************************************
 * @file fsm.h
 * @author lx
 * @version
 * @date 2026-06-05
 * @brief FSM - 基于函数指针的有限状态机框架
 ******************************************************************************
 * @attention
 *
 ******************************************************************************
 */

#ifndef _FSM_H_
#define _FSM_H_

 /* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdint.h>

/**
  * @brief FSM 事件类型枚举，定义框架保留事件及用户事件起始值
  */
typedef enum {
    FSM_EVENT_NONE = 0,      /*!< 无事件 */
    FSM_EVENT_ENTRY = 1,     /*!< 状态入口事件 */
    FSM_EVENT_DO = 2,        /*!< 状态执行事件 */
    FSM_EVENT_EXIT = 3,      /*!< 状态出口事件 */
    FSM_EVENT_USER_START = 4 /*!< 用户自定义事件起始值 */
} FSM_EVENT_T;

/**
  * @brief FSM 实例结构体，维护状态机当前状态、上一状态及转换动作
  */
typedef struct fsm_t {
    void (*lastState)(struct fsm_t * pFSM, int event);      /*!< 上一状态函数指针 */
    void (*currentState)(struct fsm_t * pFSM, int event);    /*!< 当前状态函数指针 */
    void (*transitionAction)(void);                         /*!< 状态转换时的一次性动作函数指针 */
} FSM_T;

/**
  * @brief 状态函数指针类型，签名为 void(FSM_T*, int)
  */
typedef void (*FSM_STATE_T)(FSM_T * pFSM, int event);

/**
  * @brief 转换动作函数指针类型，签名为 void(void)
  */
typedef void (*FSM_ACTION_T)(void);

/**
  * @brief 初始化 FSM 实例，设置初始状态并自动派发 FSM_EVENT_ENTRY
  * @note 初始化完成后会对初始状态派发 FSM_EVENT_ENTRY 事件
  * @param pFSM FSM 实例指针
  * @param initialState 初始状态函数指针
  * @retval void
  */
void FSMInit(FSM_T * pFSM, FSM_STATE_T initialState);

/**
  * @brief 派发事件到当前状态，若触发状态转换则自动处理 EXIT/ENTRY 序列
  * @note 若状态函数内部调用了 FSMTransform，则会自动执行 EXIT → transitionAction → ENTRY 的转换序列
  * @param pFSM FSM 实例指针
  * @param event 事件值
  * @retval void
  */
void FSMDispatch(FSM_T * pFSM, int event);

/**
  * @brief 便利函数，向当前状态派发 DO 事件
  * @note 等价于 FSMDispatch(pFSM, FSM_EVENT_DO)
  * @param pFSM FSM 实例指针
  * @retval void
  */
void FSMDo(FSM_T * pFSM);

/**
  * @brief 请求状态转换，记录目标状态和转换动作
  * @note 仅设置 pFSM->currentState 和 pFSM->transitionAction，不派发 EXIT/ENTRY 事件。实际转换序列由 FSMDispatch 检测并执行。不要在 EXIT 事件处理中调用此函数
  * @param pFSM FSM 实例指针
  * @param newState 目标状态函数指针
  * @param transitionAction 状态转换时执行的一次性动作函数指针，可为 NULL
  * @retval void
  */
void FSMTransform(FSM_T * pFSM, FSM_STATE_T newState, FSM_ACTION_T transitionAction);

#endif // _FSM_H_
