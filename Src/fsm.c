/**
  ******************************************************************************
  * @file fsm.c
  * @author lx
  * @version
  * @date 2026-06-05
  * @brief FSM 框架核心实现
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

  /* Includes ------------------------------------------------------------------*/
#include "fsm.h"

/**
  * @brief 初始化 FSM 实例，设置初始状态并自动派发 FSM_EVENT_ENTRY
  * @note 初始化完成后会对初始状态派发 FSM_EVENT_ENTRY 事件
  * @param pFSM FSM 实例指针
  * @param initialState 初始状态函数指针
  * @retval void
  */
void FSMInit(FSM_T * pFSM, FSM_STATE_T initialState)
{
    if (pFSM == NULL || initialState == NULL) {
        return;
    }

    pFSM->lastState = initialState;
    pFSM->currentState = initialState;
    pFSM->transitionAction = NULL;

    FSMDispatch(pFSM, FSM_EVENT_ENTRY);
}

/**
  * @brief 请求状态转换，记录目标状态和转换动作
  * @note 仅设置 pFSM->currentState 和 pFSM->transitionAction，不派发 EXIT/ENTRY 事件。实际转换序列由 FSMDispatch 检测并执行。不要在 EXIT 事件处理中调用此函数
  * @param pFSM FSM 实例指针
  * @param newState 目标状态函数指针
  * @param transitionAction 状态转换时执行的一次性动作函数指针，可为 NULL
  * @retval void
  */
void FSMTransform(FSM_T * pFSM, FSM_STATE_T newState, FSM_ACTION_T transitionAction)
{
    if (pFSM == NULL || newState == NULL) {
        return;
    }

    pFSM->currentState = newState;
    pFSM->transitionAction = transitionAction;
}

/**
  * @brief 派发事件到当前状态，若触发状态转换则自动处理 EXIT/ENTRY 序列
  * @note 若状态函数内部调用了 FSMTransform，则会自动执行 EXIT → transitionAction → ENTRY 的转换序列
  * @param pFSM FSM 实例指针
  * @param event 事件值
  * @retval void
  */
void FSMDispatch(FSM_T * pFSM, int event)
{
    if (pFSM == NULL) {
        return;
    }

    pFSM->currentState(pFSM, event);

    if (pFSM->currentState != pFSM->lastState) {
        pFSM->lastState(pFSM, FSM_EVENT_EXIT);
        if (pFSM->transitionAction != NULL) {
            pFSM->transitionAction();
            pFSM->transitionAction = NULL;
        }
        pFSM->currentState(pFSM, FSM_EVENT_ENTRY);
        pFSM->lastState = pFSM->currentState;
    }
}

/**
  * @brief 便利函数，向当前状态派发 DO 事件
  * @note 等价于 FSMDispatch(pFSM, FSM_EVENT_DO)
  * @param pFSM FSM 实例指针
  * @retval void
  */
void FSMDo(FSM_T * pFSM)
{
    if (pFSM == NULL) {
        return;
    }

    FSMDispatch(pFSM, FSM_EVENT_DO);
}


