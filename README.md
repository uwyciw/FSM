# FSM

轻量级 C 语言有限状态机框架，专为嵌入式系统设计。

## 特性

- **函数指针驱动** — 每个状态是一个函数，事件驱动状态切换
- **自动转换序列** — 状态切换时自动执行 EXIT → transition action → ENTRY
- **事件模型** — 内置 ENTRY / DO / EXIT 事件，支持自定义用户事件
- **极简 API** — 仅 4 个核心函数，零依赖
- **可移植** — 纯 C 标准库，无平台相关代码
- **Tiny** — 代码量极小，适合资源受限的 MCU

## API 概览

| 函数 | 说明 |
|------|------|
| `FSMInit` | 初始化状态机，设置初始状态并派发 ENTRY 事件 |
| `FSMDispatch` | 向当前状态派发事件，检测并处理状态转换 |
| `FSMDo` | 快捷派发 DO 事件 |
| `FSMTransform` | 请求切换到新状态，记录转换动作 |

## 目录结构

- `Inc/fsm.h` — 头文件（类型定义 + API 声明）
- `Src/fsm.c` — 核心实现
- `_Example/` — STM32L053 示例工程（IAR EWARM）

## 使用示例

```c
#include "fsm.h"

// 状态函数声明
void StateIdle(FSM_T * pFSM, int event);
void StateRun(FSM_T * pFSM, int event);

// 状态机实例
FSM_T fsm;

int main(void)
{
    FSMInit(&fsm, StateIdle);

    while (1) {
        FSMDispatch(&fsm, FSM_EVENT_DO);
    }
}

void StateIdle(FSM_T * pFSM, int event)
{
    switch (event) {
    case FSM_EVENT_ENTRY:
        // 进入 Idle 状态
        break;
    case FSM_EVENT_DO:
        // 触发转换到 Run 状态
        FSMTransform(pFSM, StateRun, NULL);
        break;
    case FSM_EVENT_EXIT:
        // 离开 Idle 状态
        break;
    }
}

void StateRun(FSM_T * pFSM, int event)
{
    switch (event) {
    case FSM_EVENT_ENTRY:
        // 进入 Run 状态
        break;
    case FSM_EVENT_DO:
        // ...
        break;
    case FSM_EVENT_EXIT:
        // 离开 Run 状态
        break;
    }
}
```

## 转换机制

当状态函数中调用 `FSMTransform` 后：

1. 当前状态收到 `EXIT` 事件
2. 执行 `transitionAction`（如有）
3. 新状态收到 `ENTRY` 事件


