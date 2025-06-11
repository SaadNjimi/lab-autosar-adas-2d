/**************************************************************************************************\
 *** Os_Task.c
 ***
 *** OSEK API function definition for OS Tasks.
 ***
 *** Copyright (c) 2014 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef OS_TASK_C
#define OS_TASK_C

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/

#include "Os.h"
#include "Os_Task.h"

#include "Os_Frame.h"
#include "Os_Kernel.h"
#include "Os_Cfg.h"

#include "Os_SimulationCore.h"

/*------------------------------------------------------------------------------------------------*\
  FUNCTION DEFINITIONS
\*------------------------------------------------------------------------------------------------*/

void Os_PreActivateTaskHook(void)
{
    g_Kernel.ApplicationLevel = TASK_LEVEL;
    g_Kernel.CallLevel = TASK_LEVEL;
    /* Increment number of task invocations for newly activated task - must only be called when the task is activated */
    g_NumberOfTaskInvocations[g_Kernel.ActTaskId] += 1;

    /* For this turn, this is the first basic rate task running. */
    if ((IDLE_LOOP == g_Kernel.ActiveBasicRateTask) && (OS_FALSE == g_TaskCb[g_Kernel.ActTaskId].ExtendedTask) && (g_TaskCb[g_Kernel.ActTaskId].PostCallRasterId > 0))
    {
        /* Set service to run */
        g_Kernel.ActiveBasicRateTask = g_Kernel.ActTaskId;
    }

    if (0 < g_TaskCb[g_Kernel.ActTaskId].PreCallRasterId)
    {
        Os_SimulationCoreExpSvcPreTaskCall(g_TaskCb[g_Kernel.ActTaskId].PreCallRasterId);
    }

    g_TaskCb[g_Kernel.ActTaskId].Terminated = OS_FALSE;
}

StatusType Os_PostDeActivateTaskHook(TaskType TaskID)
{
    if(TaskID >= NUMBER_OF_TASKS)
    {
        ERRORHOOK(E_OS_ID);
        return E_OS_ID;
    }

    if (g_TaskCb[TaskID].Terminated == OS_FALSE)
    {
        return E_OS_STATE;
    }

    return E_OK;
}

StatusType ActivateTask(TaskType TaskID)
{
#ifdef OS_DBGMSG_ENABLED
    Os_SimulationCoreLogDbgMessage(__FUNCTION__, "ActivateTask(): Start of function. Task ID is %d.", TaskID);
#endif

    g_ErrorInfo.ServiceID = OSServiceId_ActivateTask;
    g_ErrorInfo.Service.SvActivateTask.TaskID = TaskID;

    if(g_Kernel.ApplicationLevel > ISR_LEVEL)
    {
        ERRORHOOK(E_OS_CALLEVEL);
        return E_OS_CALLEVEL;
    }

    if(TaskID >= NUMBER_OF_TASKS)
    {
        ERRORHOOK(E_OS_ID);
        return E_OS_ID;
    }

#if NUMBER_OF_OS_APPLICATIONS > 0
    /* is the OS-Application, which belongs to the task, started? */
    if (g_TaskCb[TaskID].ConnectApplicationId != OS_NOT_CONNECTED_TO_ANY_APPLICATION)
    {
        if( (g_TaskCb[TaskID].ConnectApplicationId != g_TaskCb[g_Kernel.ActTaskId].ConnectApplicationId) &&
            (g_ApplicationCb[g_TaskCb[TaskID].ConnectApplicationId].State != APPLICATION_ACCESSIBLE) )
        {
            /* [SWS_Os_00509] If a service call is made on an Operating System object that is owned by another OS-Application without state APPLICATION_ACCESSIBLE, then the Operating System module shall return E_OS_ACCESS. */
            ERRORHOOK(E_OS_ACCESS);
            return E_OS_ACCESS;
        }
    }

    /* is this task belong to an os application ? */
    if (g_TaskCb[g_Kernel.ActTaskId].ConnectApplicationId != OS_NOT_CONNECTED_TO_ANY_APPLICATION)
    {
        /* belong the actual and the to activated task  to the different os application ? */
        if(g_TaskCb[g_Kernel.ActTaskId].ConnectApplicationId != g_TaskCb[TaskID].ConnectApplicationId )
        {
            /* [SWS_Os_00596] It shall be possible to activate a TASK that is part of an OS-Application located on another core, as long as the assigned access rights allow it.  */
            if ( g_TaskCb[TaskID].ApplicationHasAccess[g_TaskCb[g_Kernel.ActTaskId].ConnectApplicationId] == OS_FALSE)
            {
                /* [SWS_Os_00589] All functions that are not allowed to operate cross core shall return E_OS_CORE in extended status if called with parameters that require a cross core operation. */
                /* [SWS_Os_00599] In case of an error when calling ActivateTask across cores, the error handler shall be called on the core on which ActivateTask was originally called. */
                ERRORHOOK(E_OS_CORE);
                return E_OS_CORE;
            }
        }
    }
#endif

    g_TaskCb[TaskID].NumberOfAct++;

    if(g_TaskCb[TaskID].NumberOfAct > g_TaskCb[TaskID].MaxAct)
    {
        g_TaskCb[TaskID].NumberOfAct--;
        g_NumberOfTaskOverruns[TaskID]++;
        ERRORHOOK(E_OS_LIMIT);
        return E_OS_LIMIT;
    }

#ifdef OS_DBGMSG_ENABLED
    Os_SimulationCoreLogDbgMessage(__FUNCTION__, "ActivateTask(): Task with ID %d switches to state READY.", TaskID);
#endif
    Os_ChangeTaskState(TaskID, READY);

    Os_ScheduleInternal(OS_FALSE, OS_FALSE);

    return E_OK;
}

StatusType GetTaskID(TaskRefType TaskID)
{
#ifdef OS_DBGMSG_ENABLED
    Os_SimulationCoreLogDbgMessage(__FUNCTION__, "GetTaskID(): Start of function. Task ID is %d.", g_Kernel.ActTaskId);
#endif

    g_ErrorInfo.ServiceID = OSServiceId_GetTaskID;

    if(g_Kernel.ActTaskId == IDLE_LOOP)
    {
        *TaskID = OS_INVALID_TASKID;
    }
    else
    {
        *TaskID = g_Kernel.ActTaskId;
    }

    return E_OK;
}

StatusType GetTaskState(TaskType TaskID, TaskStateRefType State)
{
#ifdef OS_DBGMSG_ENABLED
    Os_SimulationCoreLogDbgMessage(__FUNCTION__, "GetTaskState(): Start of function. Task ID is %d.", TaskID);
#endif

    g_ErrorInfo.ServiceID = OSServiceId_GetTaskState;
    g_ErrorInfo.Service.SvGetTaskState.TaskID = TaskID;

    if(TaskID >= NUMBER_OF_TASKS)
    {
        ERRORHOOK(E_OS_ID);
        return E_OS_ID;
    }

    *State = g_TaskCb[TaskID].State;

    return E_OK;
}

StatusType ChainTask(TaskType TaskID)
{
    g_ErrorInfo.ServiceID = OSServiceId_ChainTask;
    g_ErrorInfo.Service.SvChainTask.TaskID = TaskID;

    if(g_Kernel.CallLevel != TASK_LEVEL)
    {
        ERRORHOOK(E_OS_CALLEVEL);
        return E_OS_CALLEVEL;
    }

    if(g_TaskCb[g_Kernel.ActTaskId].ResourceLevelCnt > 0)
    {
        ERRORHOOK(E_OS_RESOURCE);
        return E_OS_RESOURCE;
    }

    if(TaskID >= NUMBER_OF_TASKS)
    {
        ERRORHOOK(E_OS_ID);
        return E_OS_ID;
    }

#if NUMBER_OF_OS_APPLICATIONS > 0
    /* is the OS-Application, which belongs to the task, started? */
    if (g_TaskCb[TaskID].ConnectApplicationId != OS_NOT_CONNECTED_TO_ANY_APPLICATION)
    {
        if( (g_TaskCb[TaskID].ConnectApplicationId != g_TaskCb[g_Kernel.ActTaskId].ConnectApplicationId) &&
            (g_ApplicationCb[g_TaskCb[TaskID].ConnectApplicationId].State != APPLICATION_ACCESSIBLE) )
        {
            /* [SWS_Os_00509] If a service call is made on an Operating System object that is owned by another OS-Application without state APPLICATION_ACCESSIBLE, then the Operating System module shall return E_OS_ACCESS. */
            ERRORHOOK(E_OS_ACCESS);
            return E_OS_ACCESS;
        }
    }

    /* is this task belong to an os application ? */
    if (g_TaskCb[g_Kernel.ActTaskId].ConnectApplicationId != OS_NOT_CONNECTED_TO_ANY_APPLICATION)
    {
        /* belong the actual and the to activated task  to the different os application ? */
        if(g_TaskCb[g_Kernel.ActTaskId].ConnectApplicationId != g_TaskCb[TaskID].ConnectApplicationId )
        {
            /* [SWS_Os_00596] It shall be possible to activate a TASK that is part of an OS-Application located on another core, as long as the assigned access rights allow it.  */
            if ( g_TaskCb[TaskID].ApplicationHasAccess[g_TaskCb[g_Kernel.ActTaskId].ConnectApplicationId] == OS_FALSE)
            {
                /* [SWS_Os_00589] All functions that are not allowed to operate cross core shall return E_OS_CORE in extended status if called with parameters that require a cross core operation. */
                /* [SWS_Os_00599] In case of an error when calling ActivateTask across cores, the error handler shall be called on the core on which ActivateTask was originally called. */
                ERRORHOOK(E_OS_CORE);
                return E_OS_CORE;
            }
        }
    }
#endif

    if(g_TaskCb[TaskID].NumberOfAct >= g_TaskCb[TaskID].MaxAct)
    {
        ERRORHOOK(E_OS_LIMIT);
        g_NumberOfTaskOverruns[TaskID]++;
        return E_OS_LIMIT;
    }

    g_TaskCb[TaskID].NumberOfAct++;

    g_TaskCb[g_Kernel.ActTaskId].NumberOfAct--;
    Os_SetTaskActPrio(g_Kernel.ActTaskId, g_TaskCb[g_Kernel.ActTaskId].InitialPrio);
    g_Kernel.LastInterruptedTask = g_TaskCb[g_Kernel.ActTaskId].OsInterruptedTask;

    if(g_TaskCb[g_Kernel.ActTaskId].NumberOfAct == 0)
    {
#ifdef OS_DBGMSG_ENABLED
        Os_SimulationCoreLogDbgMessage(__FUNCTION__, "Os_ChainTask(): Task with ID %d switches to state SUSPENDED.", g_Kernel.ActTaskId);
#endif
        Os_ChangeTaskState(g_Kernel.ActTaskId, SUSPENDED);
        g_TaskCb[g_Kernel.ActTaskId].OsInterruptedTask = IDLE_LOOP;
    }
    else
    {
#ifdef OS_DBGMSG_ENABLED
        Os_SimulationCoreLogDbgMessage(__FUNCTION__, "Os_ChainTask(): Task with ID %d switches to state READY.", g_Kernel.ActTaskId);
#endif
        Os_ChangeTaskState(g_Kernel.ActTaskId, READY);
    }

    g_TaskCb[g_Kernel.ActTaskId].Terminated = OS_TRUE;

#ifdef OS_DBGMSG_ENABLED
    Os_SimulationCoreLogDbgMessage(__FUNCTION__, "Os_ChainTask(): Task with ID %d switches to state READY.", TaskID);
#endif
    Os_ChangeTaskState(TaskID, READY);

    if (g_TaskCb[g_Kernel.ActTaskId].PostCallRasterId > 0)
    {
        Os_SimulationCoreExpSvcPostTaskCall(g_TaskCb[g_Kernel.ActTaskId].PostCallRasterId, (g_Kernel.ActiveBasicRateTask == g_Kernel.ActTaskId) ? TRUE : FALSE);
    }

    Os_ScheduleInternal(OS_TRUE, OS_TRUE);

    OS_LONGJMP(returnToTask[g_Kernel.ActTaskId], 1);

}

StatusType TerminateTask(void)
{
    g_ErrorInfo.ServiceID = OSServiceId_TerminateTask;

#ifdef OS_DBGMSG_ENABLED
    Os_SimulationCoreLogDbgMessage(__FUNCTION__, "TerminateTask(): Terminating task with ID %d. Control has left user code.", g_Kernel.ActTaskId);
#endif

    if ((OS_FALSE == g_Kernel.IsStarted) && (OS_TRUE == g_Kernel.ShutdownOsCalled))
    {
        /*
         * The schedule manager might have shutdown the operating system already.
         * In this case, terminate task is invoked after ShutdownOs was called.
         * Since the internal kernel structure is reset, the operating system must
         * not execute terminate task.
         */
        Os_ScheduleInternal(OS_FALSE, OS_TRUE);
        return E_OS_SYS_INTERNAL;
    }

    if(g_Kernel.CallLevel != TASK_LEVEL)
    {
        ERRORHOOK(E_OS_CALLEVEL);
        return E_OS_CALLEVEL ;
    }

    if(g_TaskCb[g_Kernel.ActTaskId].ResourceLevelCnt > 0)
    {
        ERRORHOOK(E_OS_RESOURCE);
        return E_OS_RESOURCE;
    }

    g_TaskCb[g_Kernel.ActTaskId].NumberOfAct--;
    Os_SetTaskActPrio(g_Kernel.ActTaskId, g_TaskCb[g_Kernel.ActTaskId].InitialPrio);
    g_Kernel.LastInterruptedTask = g_TaskCb[g_Kernel.ActTaskId].OsInterruptedTask;

    if(g_TaskCb[g_Kernel.ActTaskId].NumberOfAct == 0)
    {
#ifdef OS_DBGMSG_ENABLED
        Os_SimulationCoreLogDbgMessage(__FUNCTION__, "TerminateTask(): Task with ID %d switches to state SUSPENDED.", g_Kernel.ActTaskId);
#endif
        Os_ChangeTaskState(g_Kernel.ActTaskId, SUSPENDED);
        g_TaskCb[g_Kernel.ActTaskId].OsInterruptedTask = IDLE_LOOP;
    }
    else
    {
#ifdef OS_DBGMSG_ENABLED
        Os_SimulationCoreLogDbgMessage(__FUNCTION__, "TerminateTask(): Task with ID %d switches to state READY.", g_Kernel.ActTaskId);
#endif
        Os_ChangeTaskState(g_Kernel.ActTaskId, READY);
    }

    g_TaskCb[g_Kernel.ActTaskId].Terminated = OS_TRUE;

    if (g_TaskCb[g_Kernel.ActTaskId].PostCallRasterId > 0)
    {
        Os_SimulationCoreExpSvcPostTaskCall(g_TaskCb[g_Kernel.ActTaskId].PostCallRasterId, (g_Kernel.ActiveBasicRateTask == g_Kernel.ActTaskId) ? TRUE : FALSE);
    }

    Os_PostDeActivateTaskHook(g_Kernel.ActTaskId);

    Os_ScheduleInternal(OS_TRUE, OS_TRUE);

    OS_LONGJMP(returnToTask[g_Kernel.ActTaskId], 1);

    /* return E_OK; */
}

void Os_SetTaskReady(TaskType Tid)
{
    if (Tid >= NUMBER_OF_TASKS)
    {
#ifdef OS_DBGMSG_ENABLED
        Os_SimulationCoreLogDbgMessage(__FUNCTION__, "Os_SetTaskReady(): No Task with ID %d. Task id invalid.", Tid);
#endif
        ERRORHOOK(E_OS_ID);
        return;
    }

    if ((g_TaskCb[Tid].pTaskFcn != OS_NULL) )
    {
#ifdef OS_DBGMSG_ENABLED
        Os_SimulationCoreLogDbgMessage(__FUNCTION__, "Os_SetTaskReady(): Task with ID %d switches to state READY.", Tid);
#endif
        Os_ChangeTaskState(Tid, READY);
        g_TaskCb[Tid].NumberOfAct = 1;
    }
}

void Os_ChangeTaskStateByPtr(Os_TaskControlBlockType* task, TaskStateType state)
{
#ifdef VEOS_DEBUG
    if (task == OS_NULL)
    {
        Os_SimulationCoreLogDbgMessage(__FUNCTION__, "Invalid parameter.");
    }
#endif

    if (task->State != state)
    {
        task->State = state;

        if ((state == READY) || (state == RUNNING))
        {
            if (task->ReadyTasksHeapIdx == OS_READYTASKS_INVALID_IDX)
            {
                Os_ReadyTasks_Add(task);
            }
        }
        else
        {
            if (task->ReadyTasksHeapIdx != OS_READYTASKS_INVALID_IDX)
            {
                Os_ReadyTasks_Remove(task);
            }
        }
    }
}

#endif /* OS_TASK_C */

/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/
