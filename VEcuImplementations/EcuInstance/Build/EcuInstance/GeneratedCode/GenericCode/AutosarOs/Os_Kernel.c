/**************************************************************************************************\
 *** Os_Kernel.c
 ***
 *** Definitions for operating system core functions and data addresses
 ***
 *** Copyright (c) 2014 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef OS_KERNEL_C
#define OS_KERNEL_C

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/

#include "Os_Types.h"
#include "Os_Kernel.h"
#include "Os_Cfg.h"
#include "Os_Native.h"
#include "Os_InternalTypes.h"
#include "Os_SimulationCore.h"
#include <stddef.h>

/*------------------------------------------------------------------------------------------------*\
  VARIABLE DEFINITIONS
\*------------------------------------------------------------------------------------------------*/

#if (NUMBER_OF_TASKS > 0)
/* AUTOSAR Os does not support measurement of task turnarounds. */
/* MEASUREMENT in A2LPreAdressUpdate.a2l  */
const uint32 g_MockTaskTurnAround = 0x0;

uint32 g_NumberOfTaskOverruns[NUMBER_OF_TASKS];
uint32 g_NumberOfTaskInvocations[NUMBER_OF_TASKS];
#endif /* NUMBER_OF_TASKS > 0 */

Os_KernelParamType g_Kernel =
{
    /* IsStarted                       = */ OS_FALSE,
    /* SimTime                         = */ 0u,
    /* TlOsAppMode                     = */ DONOTCARE,
    /* ActTaskId                       = */ IDLE_LOOP,
    /* ActIsrId                        = */ DUMMY_ISR,
    /* ActCoreIdForHookFunction        = */ OS_UNKNOWN_CORE_ID,
    /* ActApplicationIdForHookFunction = */ OS_UNKNOWN_APPLICATION_ID,
    /* ApplicationLevel                = */ OS_LEVEL,
    /* CallLevel                       = */ OS_LEVEL,
    /* PreviousCallLevel               = */ OS_LEVEL,
    /* LastInterruptedTask             = */ IDLE_LOOP,
    /* ActivationOrder                 = */ 0u,
#ifdef ERROR_HOOK_ENABLED
    /* ErrorHookCnt                    = */ 0u,
#endif
    /* IsrDisableCnt                   = */ 0u,
    /* IntDescriptor                   = */ INITIAL_INTERRUPT_DESCRIPTOR,
    /* ActiveBasicRateTask             = */ IDLE_LOOP,
    /* ShutdownOsCalled                = */ OS_FALSE,
    /* SleepingTaskId                  = */ OS_NO_TASK_IS_SLEEPING,
    /* IsSleeping                      = */ OS_VECU_IS_NOT_SLEEPING,
    /* IsSleepingLast                  = */ OS_VECU_IS_NOT_SLEEPING,
    /* InfLoopOfSleepingTaskMustBeCalled     = */ OS_FALSE
};


EventMaskType g_Os_RecentOsEventMaskResult = 0;

static Os_BoolType scheduleCallFromIdleLoop = 0;

/* Priority queue (binary heap) of all tasks, which are in state READY or RUNNING.
   The task with the highest priority is at element 0. */
static Os_TaskControlBlockType* readyTasksHeap[NUMBER_OF_TASKS];
static uint32                   readyTasksHeapSize = 0;

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

void Os_EnterIsrHook(void)
{
}

void Os_LeaveIsrHook(void)
{
}

void Os_PreTaskHook(void)
{
}

void Os_PostTaskHook(void)
{
}

void Os_TaskInternal_IDLE_LOOP(void)
{
    for(;;)
    {
        /*  V-ECU is sleeping */
        if (g_Kernel.SleepingTaskId != OS_NO_TASK_IS_SLEEPING)
        {
            g_Kernel.InfLoopOfSleepingTaskMustBeCalled = OS_TRUE;
        }
        
        scheduleCallFromIdleLoop = 1;
        Os_SysScheduleSimple();
        scheduleCallFromIdleLoop = 0;

        Os_Leave(0);
    }
    return;
}

/* Returns true if 'l' has a higher scheduling priority than 'r'. */
static OS_ATTR_FORCEINLINE boolean Os_FirstIsHigherPrioTask(const Os_TaskControlBlockType* l, const Os_TaskControlBlockType* r)
{
    if (l->ActPrio == r->ActPrio)
    {
        return (l->ActivationOrder < r->ActivationOrder);
    }

    return Os_FirstIsHigherPrio(l->ActPrio, r->ActPrio);
}

TaskType Os_FindHighPriorTask(void)
{
    const Os_TaskControlBlockType* highPrioTask = &g_TaskCb[IDLE_LOOP];

    if (readyTasksHeapSize > 0) {
        if (Os_FirstIsHigherPrioTask(readyTasksHeap[0], highPrioTask)) {
            highPrioTask = readyTasksHeap[0];
        }
    }

    return highPrioTask->TaskId;
}

/*-------------------------------------------------------------------
|
| Check and handle the V-ECU sleep and wakeup scenario
|
-------------------------------------------------------------------*/
TaskType Os_HandleSleepWakeupScenario(TaskType TlOsNewTask)
{
    if ( (g_Kernel.IsSleeping == OS_VECU_IS_SLEEPING) && (g_Kernel.IsSleepingLast == OS_VECU_IS_NOT_SLEEPING))
    {
        /*  V-ECU is just enter the sleep mode */
        if (g_Kernel.InfLoopOfSleepingTaskMustBeCalled == OS_TRUE)
        {
            /* During a sleep scenario the infinite loop of the sleeping task is called one time per time step */
            g_Kernel.ApplicationLevel = TASK_LEVEL;
            g_Kernel.CallLevel = TASK_LEVEL;
            g_Kernel.InfLoopOfSleepingTaskMustBeCalled = OS_FALSE;
        }
        else
        { 
            /* finish this simulation step */
            TlOsNewTask = IDLE_LOOP;
        }
        g_Kernel.IsSleepingLast = OS_VECU_IS_SLEEPING;
    }
    else if ( (g_Kernel.IsSleeping == OS_VECU_IS_SLEEPING) && (g_Kernel.IsSleepingLast == OS_VECU_IS_SLEEPING))
    {
        /*   V-ECU is still sleeping */
        if(Os_FirstIsHigherPrio(g_TaskCb[TlOsNewTask].ActPrio, g_TaskCb[g_Kernel.SleepingTaskId].ActPrio))
        {
            /*  V-ECU is just wakeup sleeping due a higher prio task is activated */
            g_Kernel.ApplicationLevel = TASK_LEVEL;
            g_Kernel.CallLevel = TASK_LEVEL;
            g_Kernel.InfLoopOfSleepingTaskMustBeCalled = OS_FALSE;
        
            g_Kernel.SleepingTaskId = OS_NO_TASK_IS_SLEEPING;
            g_Kernel.IsSleeping = OS_VECU_IS_NOT_SLEEPING;
            g_Kernel.IsSleepingLast = OS_VECU_IS_NOT_SLEEPING;

            Os_SimulationCoreOnOsWakeup();
        }
        else
        {
            if (g_Kernel.InfLoopOfSleepingTaskMustBeCalled == OS_TRUE)
            {
                /* During a sleep scenario the infinite loop of the sleeping task is called one time per time step */
                g_Kernel.ApplicationLevel = TASK_LEVEL;
                g_Kernel.CallLevel = TASK_LEVEL;
                g_Kernel.InfLoopOfSleepingTaskMustBeCalled = OS_FALSE;
            }
            else
            { 
                /* finish this simulation step */
                TlOsNewTask = IDLE_LOOP;
            }
        }
    }
    else if ( (g_Kernel.IsSleeping == OS_VECU_IS_NOT_SLEEPING) && (g_Kernel.IsSleepingLast == OS_VECU_IS_SLEEPING))
    {
        /*  V-ECU is just enter the wakeup mode */
        g_Kernel.ApplicationLevel = TASK_LEVEL;
        g_Kernel.CallLevel = TASK_LEVEL;
        g_Kernel.InfLoopOfSleepingTaskMustBeCalled = OS_FALSE;
        
        g_Kernel.SleepingTaskId = OS_NO_TASK_IS_SLEEPING;
        g_Kernel.IsSleepingLast = OS_VECU_IS_NOT_SLEEPING;
    }
    else
    {
        /*  V-ECU is normal running */
    }
    return TlOsNewTask;
}


/*-------------------------------------------------------------------
|
| ForceScheduling == OS_TRUE if called from:
|      Schedule, TerminateTask, ChainTask, WaitEvent, StartOS
| Termination == OS_TRUE if called from
|      TerminateTask, ChainTask
|
-------------------------------------------------------------------*/
void Os_ScheduleInternal(Os_BoolType ForceScheduling, Os_BoolType Termination)
{
    TaskType TlOsNewTask;

    /* to avoid the compiler warning C4100 */
   (void)(Termination);

    /* If g_Kernel.ShutdownOsCalled is OS_TRUE, do not return here. */
    if (OS_FALSE == g_Kernel.ShutdownOsCalled)
    {
        /* If the function StartOs() have not been running, return here      */
        /* This is possible if ResumeAllInterrupts() called before StartOs() */
        if(g_Kernel.IsStarted == OS_FALSE)
        {
            return;
        }
    }

    if(g_Kernel.ApplicationLevel == ISR_LEVEL)
    {
        return;
    }

    /* The call must come from the idle loop, otherwise the stack point is not ready */
    /* For example if the busmanager is calling a function on the target maybe "Com_RxIndication_2", and the RTE is starting a task, than the task scheduling will not come from the idle loop"*/
    if(scheduleCallFromIdleLoop == 0)
    {
        Os_TaskStart(IDLE_LOOP);
        return;
    }

    /* Actual running entity is a task. */
    TlOsNewTask = Os_FindHighPriorTask();

    /*  check and handle the V-ECU sleep and wakeup scenario */
    TlOsNewTask = Os_HandleSleepWakeupScenario(TlOsNewTask);
      
    if (TlOsNewTask == IDLE_LOOP)
    {
        /* Simulation step worked off */
        if(g_Kernel.ApplicationLevel == TASK_LEVEL)
        {
            POSTTASKHOOK();
        }

        g_Kernel.ApplicationLevel = OS_LEVEL;
        g_Kernel.CallLevel = OS_LEVEL;
        g_Kernel.ActTaskId = IDLE_LOOP;
        g_Kernel.ActIsrId = DUMMY_ISR;
        g_Kernel.LastInterruptedTask = IDLE_LOOP;
    }

    if ((g_TaskCb[g_Kernel.ActTaskId].Preemptive == OS_TASKPREEMPTION_NONE) && (ForceScheduling == OS_FALSE))
    {
        /* actual running task is not preemptive  ==> it remains in running state */
        return;
    }

    if(TlOsNewTask != g_Kernel.ActTaskId)
    {
        if(g_Kernel.ApplicationLevel == TASK_LEVEL)
        {
            /* New task interrupts actual task */
            POSTTASKHOOK();
        }

        if (TlOsNewTask != IDLE_LOOP)
        {
#ifdef OS_DBGMSG_ENABLED
            Os_SimulationCoreLogDbgMessage(__FUNCTION__, "Os_Schedule(): Task with ID %d switches to state RUNNING.", TlOsNewTask);
#endif
            Os_ChangeTaskState(TlOsNewTask, RUNNING);
            g_TaskCb[TlOsNewTask].OsInterruptedTask = g_Kernel.ActTaskId;
            g_Kernel.LastInterruptedTask = g_Kernel.ActTaskId;
            g_Kernel.ActTaskId = TlOsNewTask;

            PRETASKHOOK();
        }
    }

    if(g_TaskCb[TlOsNewTask].pTaskFcn != OS_NULL)
    {
#ifdef OS_DBGMSG_ENABLED
            if (TlOsNewTask != 0)
            {
                Os_SimulationCoreLogDbgMessage(__FUNCTION__, "Os_Schedule(): Starting task with ID %d. Control is given to user code.", TlOsNewTask);
            }
            else
            {
                Os_SimulationCoreLogDbgMessage(__FUNCTION__, "Os_Schedule(): Starting task with ID 0 (IDLE task).");
            }
#endif

        Os_TaskSwitch(TlOsNewTask);
    }
    else
    {
#ifdef OS_DBGMSG_ENABLED
        Os_SimulationCoreLogDbgMessage(__FUNCTION__, "Os_Schedule(): Task with ID %d switches to state SUSPENDED.", g_Kernel.ActTaskId);
#endif
        Os_ChangeTaskState(g_Kernel.ActTaskId, SUSPENDED);
        g_TaskCb[g_Kernel.ActTaskId].NumberOfAct = 0;
    }

    return;

    /* ERRORHOOK(E_OS_SYS_SCHED); */
    /* This line must not be reached in correct operation */
}

/* Returns the node index of the left child. */
static OS_ATTR_FORCEINLINE size_t Os_ReadyTasks_Left(size_t parentidx)
{
    return 2 * parentidx + 1;
}

/* Returns the node index of the right child. */
static OS_ATTR_FORCEINLINE size_t Os_ReadyTasks_Right(size_t parentidx)
{
    return 2 * parentidx + 2;
}

/* Returns the node index of the parent for any inner node. */
static OS_ATTR_FORCEINLINE size_t Os_ReadyTasks_Parent(size_t childidx)
{
    return (childidx - 1) / 2;
}

/* Returns true if the node at 'idx' is a leaf node. */
static OS_ATTR_FORCEINLINE boolean Os_ReadyTasks_IsLeaf(size_t idx)
{
    return (idx >= (readyTasksHeapSize / 2));
}

/* Returns true if 'first' task has a higher scheduling priority than 'second' task in the heap. */
static OS_ATTR_FORCEINLINE boolean Os_ReadyTasks_FirstIsHigher(size_t first, size_t second)
{
    return Os_FirstIsHigherPrioTask(readyTasksHeap[first], readyTasksHeap[second]);
}

#ifdef VEOS_DEBUG
static inline boolean Os_ReadyTasks_IsValidHeapEntry(size_t idx)
{
    return (idx < readyTasksHeapSize) && (readyTasksHeap[idx] != OS_NULL) && (readyTasksHeap[idx]->ReadyTasksHeapIdx == idx);
}

static boolean Os_ReadyTasks_IsValidHeap()
{
    if (readyTasksHeapSize > 0)
    {
        if (!Os_ReadyTasks_IsValidHeapEntry(0))
        {
            return FALSE;
        }

        /* Check all inner nodes for valid heap condition. */
        for (size_t idx = 1; idx < readyTasksHeapSize; ++idx)
        {
            size_t parentidx = Os_ReadyTasks_Parent(idx);
            if (!Os_ReadyTasks_IsValidHeapEntry(idx) || Os_ReadyTasks_FirstIsHigher(idx, parentidx))
            {
                return FALSE;
            }
        }
    }

    return TRUE;
}
#endif

static inline void Os_ReadyTasks_Swap(size_t a, size_t b)
{
    Os_TaskControlBlockType* tmp = readyTasksHeap[a];

    readyTasksHeap[a] = readyTasksHeap[b];
    readyTasksHeap[a]->ReadyTasksHeapIdx = (uint32)a;

    readyTasksHeap[b] = tmp;
    readyTasksHeap[b]->ReadyTasksHeapIdx = (uint32)b;
}

/* Compare and possibly swap the node with its parent, until the heap property is restored again. */
static void Os_ReadyTasks_BubbleUp(size_t idx)
{
    while (idx != 0)
    {
        size_t parentidx = Os_ReadyTasks_Parent(idx);
        if (Os_ReadyTasks_FirstIsHigher(idx, parentidx))
        {
            Os_ReadyTasks_Swap(idx, parentidx);
            idx = parentidx;
            continue;
        }

        break; /* valid heap condition reached */
    }
}

/* Compare and possibly swap the node with one of its children, until the heap property is restored again. */
static void Os_ReadyTasks_BubbleDown(size_t idx)
{
    size_t highest = idx;

    do
    {
        size_t left = Os_ReadyTasks_Left(idx);
        size_t right = Os_ReadyTasks_Right(idx);

        if ((left < readyTasksHeapSize) && Os_ReadyTasks_FirstIsHigher(left, highest))
        {
            highest = left;
        }

        if ((right < readyTasksHeapSize) && Os_ReadyTasks_FirstIsHigher(right, highest))
        {
            highest = right;
        }

        if (highest == idx)
        {
            break;
        }

        Os_ReadyTasks_Swap(idx, highest);
        idx = highest;
    } while (TRUE);
}

/* Adds the new 'task' to the binary heap. */
void Os_ReadyTasks_Add(Os_TaskControlBlockType* task)
{
#ifdef VEOS_DEBUG
    if (task == OS_NULL)
    {
        Os_SimulationCoreLogDbgMessage(__FUNCTION__, "Invalid parameter.");
        return;
    }

    if (task->ReadyTasksHeapIdx != OS_READYTASKS_INVALID_IDX)
    {
        Os_SimulationCoreLogDbgMessage(__FUNCTION__, "Internal error. Task is already in heap.");
        return;
    }

    if (readyTasksHeapSize >= NUMBER_OF_TASKS)
    {
        Os_SimulationCoreLogDbgMessage(__FUNCTION__, "Internal error. Heap is full.");
        return;
    }

    if (!Os_ReadyTasks_IsValidHeap())
    {
        Os_SimulationCoreLogDbgMessage(__FUNCTION__, "Internal error. Heap is invalid.");
        return;
    }
#endif

    if (task->ActivationOrder == 0)
    {
        task->ActivationOrder = ++g_Kernel.ActivationOrder;
    }

    task->ReadyTasksHeapIdx = readyTasksHeapSize;
    readyTasksHeap[readyTasksHeapSize++] = task;

    if (task->ReadyTasksHeapIdx != 0)
    {
        Os_ReadyTasks_BubbleUp(task->ReadyTasksHeapIdx);
    }
}

/* Removes 'task' from the binary heap. */
void Os_ReadyTasks_Remove(Os_TaskControlBlockType* task)
{
#ifdef VEOS_DEBUG
    if (task == OS_NULL)
    {
        Os_SimulationCoreLogDbgMessage(__FUNCTION__, "Invalid parameter.");
        return;
    }

    if (task->ReadyTasksHeapIdx >= readyTasksHeapSize)
    {
        Os_SimulationCoreLogDbgMessage(__FUNCTION__, "Invalid parameter. Task is not in heap.");
        return;
    }

    if (!Os_ReadyTasks_IsValidHeap())
    {
        Os_SimulationCoreLogDbgMessage(__FUNCTION__, "Internal error. Heap is invalid.");
        return;
    }
#endif

    /* Replace the element with the last one in the tree and then bubble it down. */
    size_t delidx = task->ReadyTasksHeapIdx;
    task->ReadyTasksHeapIdx = OS_READYTASKS_INVALID_IDX;

    readyTasksHeapSize--;

    if (delidx < readyTasksHeapSize)
    {
        readyTasksHeap[delidx] = readyTasksHeap[readyTasksHeapSize];
        readyTasksHeap[delidx]->ReadyTasksHeapIdx = (uint32)delidx;
    }

#ifndef NDEBUG
    /* Make debugging easier: overwrite unused entries with NULL. */
    readyTasksHeap[readyTasksHeapSize] = OS_NULL;
#endif

    if (!Os_ReadyTasks_IsLeaf(delidx))
    {
        Os_ReadyTasks_BubbleDown(delidx);
    }
}

/* Restores the heap property by moving 'task' to its correct position. */
void Os_ReadyTasks_Update(Os_TaskControlBlockType* task)
{
#ifdef VEOS_DEBUG
    if (task == OS_NULL)
    {
        Os_SimulationCoreLogDbgMessage(__FUNCTION__, "Invalid parameter.");
        return;
    }

    if (task->ReadyTasksHeapIdx >= readyTasksHeapSize)
    {
        Os_SimulationCoreLogDbgMessage(__FUNCTION__, "Invalid parameter. Task is not in heap.");
        return;
    }
#endif

    size_t idx = task->ReadyTasksHeapIdx;
    size_t parentidx = Os_ReadyTasks_Parent(idx);

    if ((idx > 0) && Os_ReadyTasks_FirstIsHigher(idx, parentidx))
    {
        Os_ReadyTasks_Swap(idx, parentidx);
        Os_ReadyTasks_BubbleUp(parentidx);
    }
    else if (!Os_ReadyTasks_IsLeaf(idx))
    {
        Os_ReadyTasks_BubbleDown(idx);
    }
}

#endif/* OS_KERNEL_C */

/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/
