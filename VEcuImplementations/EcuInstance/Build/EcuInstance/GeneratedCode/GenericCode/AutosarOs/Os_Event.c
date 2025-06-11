/**************************************************************************************************\
 *** Os_Event.c
 ***
 *** OSEK API function definition for OS Events.
 ***
 *** Copyright (c) 2014 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef OS_EVENT_C
#define OS_EVENT_C

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/

#include "Os_Event.h"

#include "Os_Types.h"
#include "Os_Kernel.h"
#include "Os_Cfg.h"
#include "Os_SimulationCore.h"

/*------------------------------------------------------------------------------------------------*\
  FUNCTION DEFINITIONS
\*------------------------------------------------------------------------------------------------*/

StatusType SetEvent(TaskType TaskID, EventMaskType Mask)
{
#ifdef OS_DBGMSG_ENABLED
    Os_SimulationCoreLogDbgMessage(__FUNCTION__, "SetEvent(): Start of function. Task ID is %d, event mask is %llu.", TaskID, Mask);
#endif

    g_ErrorInfo.ServiceID = OSServiceId_SetEvent;
    g_ErrorInfo.Service.SvSetEvent.TaskID = TaskID;
    g_ErrorInfo.Service.SvSetEvent.Mask = Mask;

    if(TaskID >= NUMBER_OF_TASKS)
    {
        ERRORHOOK(E_OS_ID);
        return E_OS_ID;
    }

    if(g_TaskCb[TaskID].ExtendedTask != OS_TRUE)
    {
        ERRORHOOK(E_OS_ACCESS);
        return E_OS_ACCESS;
    }

    if(g_TaskCb[TaskID].State == SUSPENDED)
    {
        ERRORHOOK(E_OS_STATE);
        return E_OS_STATE;
    }

#if NUMBER_OF_OS_APPLICATIONS > 0
    /* is the OS-Application, which belongs to the task/event, started? */
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
            /* [SWS_Os_00602] It shall be possible to set an EVENT that is part of an OS-Application located on another core, as long as the assigned access rights allow it.  */
            if ( g_TaskCb[TaskID].ApplicationHasAccess[g_TaskCb[g_Kernel.ActTaskId].ConnectApplicationId] == OS_FALSE)
            {
                /* [SWS_Os_00589] All functions that are not allowed to operate cross core shall return E_OS_CORE in extended status if called with parameters that require a cross core operation. */
                /* [SWS_Os_00605] In case of an error when calling SetEvent across cores, the error handler shall be called on the core on which SetEvent was originally called. */
                ERRORHOOK(E_OS_CORE);
                return E_OS_CORE;
            }
        }
    }
#endif

    g_TaskCb[TaskID].Events |= Mask;

    if (g_TaskCb[TaskID].State == WAITING)
    {
        /* Set task to READY state only if events occurr for which the task is waiting */
        if ((g_TaskCb[TaskID].EventsWaiting & Mask) == Mask)
        {
#ifdef OS_DBGMSG_ENABLED
            Os_SimulationCoreLogDbgMessage(__FUNCTION__, "SetEvent(): Task with ID %d switches to state READY.", TaskID);
#endif
            Os_ChangeTaskState(TaskID, READY);
        }
    }

    Os_ScheduleInternal(OS_FALSE, OS_FALSE);

    return E_OK;
}

StatusType ClearEvent(EventMaskType Mask)
{
#ifdef OS_DBGMSG_ENABLED
    Os_SimulationCoreLogDbgMessage(__FUNCTION__, "ClearEvent(): Start of function. Event mask is %llu.", Mask);
#endif

    g_ErrorInfo.ServiceID = OSServiceId_ClearEvent;
    g_ErrorInfo.Service.SvClearEvent.Mask = Mask;

    if(g_TaskCb[g_Kernel.ActTaskId].ExtendedTask != OS_TRUE)
    {
        ERRORHOOK(E_OS_ACCESS);
        return E_OS_ACCESS;
    }

    if(g_Kernel.CallLevel != TASK_LEVEL)
    {
        ERRORHOOK(E_OS_CALLEVEL);
        return E_OS_CALLEVEL ;
    }

    g_TaskCb[g_Kernel.ActTaskId].Events &= (~Mask);

    return E_OK;
}

StatusType GetEvent(TaskType TaskID, EventMaskRefType Event)
{
#ifdef OS_DBGMSG_ENABLED
    Os_SimulationCoreLogDbgMessage(__FUNCTION__, "GetEvent(): Start of function. Task ID is %d.", TaskID);
#endif

    g_ErrorInfo.ServiceID = OSServiceId_GetEvent;
    g_ErrorInfo.Service.SvGetEvent.TaskID = TaskID;

    if(TaskID >= NUMBER_OF_TASKS)
    {
        ERRORHOOK(E_OS_ID);
        return E_OS_ID;
    }

    if(g_TaskCb[TaskID].ExtendedTask != OS_TRUE)
    {
        ERRORHOOK(E_OS_ACCESS);
        return E_OS_ACCESS;
    }

#if NUMBER_OF_OS_APPLICATIONS > 0
    /* is this task belong to an os application ? */
    if (g_TaskCb[g_Kernel.ActTaskId].ConnectApplicationId != OS_NOT_CONNECTED_TO_ANY_APPLICATION)
    {
        /* belong the actual and the to activated task  to the different os application ? */
        if(g_TaskCb[g_Kernel.ActTaskId].ConnectApplicationId != g_TaskCb[TaskID].ConnectApplicationId )
        {
            /* [SWS_Os_00602] It shall be possible to set an EVENT that is part of an OS-Application located on another core, as long as the assigned access rights allow it.  */
            if ( g_TaskCb[TaskID].ApplicationHasAccess[g_TaskCb[g_Kernel.ActTaskId].ConnectApplicationId] == OS_FALSE)
            {
                /* [SWS_Os_00589] All functions that are not allowed to operate cross core shall return E_OS_CORE in extended status if called with parameters that require a cross core operation. */
                /* [SWS_Os_00605] In case of an error when calling SetEvent across cores, the error handler shall be called on the core on which SetEvent was originally called. */
                ERRORHOOK(E_OS_CORE);
                return E_OS_CORE;
            }
        }
    }
#endif

    if(g_TaskCb[TaskID].State == SUSPENDED)
    {
        ERRORHOOK(E_OS_STATE);
        return E_OS_STATE;
    }

    *Event = g_TaskCb[TaskID].Events;

    return E_OK;
}

StatusType WaitEvent(EventMaskType Mask)
{
    g_ErrorInfo.ServiceID = OSServiceId_WaitEvent;
    g_ErrorInfo.Service.SvWaitEvent.Mask = Mask;

    g_Os_RecentOsEventMaskResult = (EventMaskType)0;

    if(g_TaskCb[g_Kernel.ActTaskId].ExtendedTask != OS_TRUE)
    {
        ERRORHOOK(E_OS_ACCESS);
        return E_OS_ACCESS;
    }

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

    if((g_TaskCb[g_Kernel.ActTaskId].Events & Mask ) == 0)
    {
#ifdef OS_DBGMSG_ENABLED
        Os_SimulationCoreLogDbgMessage(__FUNCTION__, "Os_WaitEvent(): Task with ID %d switches to state WAITING.", g_Kernel.ActTaskId);
#endif
        Os_ChangeTaskState(g_Kernel.ActTaskId, WAITING);
        g_TaskCb[g_Kernel.ActTaskId].EventsWaiting = Mask;
    }
    else
    {
        g_Os_RecentOsEventMaskResult = (g_TaskCb[g_Kernel.ActTaskId].Events & Mask);
        return E_OK;
    }

    Os_ScheduleInternal(OS_TRUE, OS_FALSE);

    g_Kernel.ApplicationLevel = TASK_LEVEL;
    g_Kernel.CallLevel = TASK_LEVEL;

    return E_OK;
}

#endif /* OS_EVENT_C */

/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/
