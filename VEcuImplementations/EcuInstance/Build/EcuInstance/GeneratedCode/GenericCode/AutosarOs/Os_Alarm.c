/**************************************************************************************************\
 *** Os_Alarm.c
 ***
 *** OSEK API function definition for OS Alarms.
 ***
 *** Copyright (c) 2014 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef OS_ALARM_C
#define OS_ALARM_C

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/

#include "Os_Alarm.h"

#include "Os_Kernel.h"
#include "Os_InternalTypes.h"
#include "Os_Cfg.h"
#include "Os_SimulationCore.h"

/*------------------------------------------------------------------------------------------------*\
  FUNCTION DEFINITIONS
\*------------------------------------------------------------------------------------------------*/

#if (NUMBER_OF_ALARMS > 0)

StatusType GetAlarmBase(AlarmType AlarmID, AlarmBaseRefType Info)
{
#ifdef OS_DBGMSG_ENABLED
    Os_SimulationCoreLogDbgMessage(__FUNCTION__, "GetAlarmBase(): Start of function. Alarm ID is %d.", AlarmID);
#endif

    g_ErrorInfo.ServiceID = OSServiceId_GetAlarmBase;
    g_ErrorInfo.Service.SvGetAlarmBase.AlarmID = AlarmID;

    if(AlarmID >= NUMBER_OF_ALARMS)
    {
        ERRORHOOK(E_OS_ID);
        return E_OS_ID;
    }

    Info->maxallowedvalue = g_CtrCb[g_AlarmCb[AlarmID].Os_Counter].MaxAllowedValue;
    Info->mincycle        = g_CtrCb[g_AlarmCb[AlarmID].Os_Counter].MinCycle;
    Info->ticksperbase    = g_CtrCb[g_AlarmCb[AlarmID].Os_Counter].TicksPerBase;

    return E_OK;
}

StatusType GetAlarm(AlarmType AlarmID, TickRefType Tick)
{
#ifdef OS_DBGMSG_ENABLED
    Os_SimulationCoreLogDbgMessage(__FUNCTION__, "GetAlarm(): Start of function. Alarm ID is %d.", AlarmID);
#endif

    g_ErrorInfo.ServiceID = OSServiceId_GetAlarm;
    g_ErrorInfo.Service.SvGetAlarm.AlarmID = AlarmID;

    *Tick = (TickType)0u;

    if(AlarmID >= NUMBER_OF_ALARMS)
    {
        ERRORHOOK(E_OS_ID);
        return E_OS_ID;
    }

    if(g_AlarmCb[AlarmID].IsStarted == OS_FALSE)
    {
        ERRORHOOK(E_OS_NOFUNC);
        return E_OS_NOFUNC;
    }

    if(g_AlarmCb[AlarmID].NextOcourence >=
        g_CtrCb[g_AlarmCb[AlarmID].Os_Counter].CtrValue)
    {
        *Tick = g_AlarmCb[AlarmID].NextOcourence -
            g_CtrCb[g_AlarmCb[AlarmID].Os_Counter].CtrValue;
    }
    else /* Counter runs over MAXALLOWEDVALUE before alarm expires */
    {
        /* stay in range of uint32 */
        /* no + max - cv  ==>  (max - cv) + no */
        *Tick = (g_CtrCb[g_AlarmCb[AlarmID].Os_Counter].MaxAllowedValue
            - g_CtrCb[g_AlarmCb[AlarmID].Os_Counter].CtrValue)
            + g_AlarmCb[AlarmID].NextOcourence;

    }

    return E_OK;
}

StatusType SetRelAlarm(AlarmType AlarmID, TickType increment, TickType cycle)
{
#ifdef OS_DBGMSG_ENABLED
    Os_SimulationCoreLogDbgMessage(__FUNCTION__, "SetRelAlarm(): Start of function. Alarm ID is %d.", AlarmID);
#endif

    g_ErrorInfo.ServiceID = OSServiceId_SetRelAlarm;
    g_ErrorInfo.Service.SvSetRelAlarm.AlarmID = AlarmID;
    g_ErrorInfo.Service.SvSetRelAlarm.increment = increment;
    g_ErrorInfo.Service.SvSetRelAlarm.cycle = cycle;

    if(AlarmID >= NUMBER_OF_ALARMS)
    {
        ERRORHOOK(E_OS_ID);
        return E_OS_ID;
    }

    if(g_AlarmCb[AlarmID].IsStarted == OS_TRUE)
    {
        ERRORHOOK(E_OS_STATE);
        return E_OS_STATE;
    }

    if ( (increment > g_CtrCb[g_AlarmCb[AlarmID].Os_Counter].MaxAllowedValue) ||
        (cycle     > g_CtrCb[g_AlarmCb[AlarmID].Os_Counter].MaxAllowedValue) ||
        ((cycle != 0) && (cycle     < g_CtrCb[g_AlarmCb[AlarmID].Os_Counter].MinCycle))  )
    {
        ERRORHOOK(E_OS_VALUE);
        return E_OS_VALUE;
    }

#if NUMBER_OF_OS_APPLICATIONS > 0
    /* is the OS-Application, which belongs to the alarm, started? */
    if (g_AlarmCb[AlarmID].ConnectApplicationId != OS_NOT_CONNECTED_TO_ANY_APPLICATION)
    {
        if( (g_ApplicationCb[g_AlarmCb[AlarmID].ConnectApplicationId].State == APPLICATION_RESTARTING) ||
            (g_ApplicationCb[g_AlarmCb[AlarmID].ConnectApplicationId].State == APPLICATION_TERMINATED) )
        {
            /* [SWS_Os_00509] If a service call is made on an Operating System object that is owned by another OS-Application without state APPLICATION_ACCESSIBLE, then the Operating System module shall return E_OS_ACCESS. */
            ERRORHOOK(E_OS_ACCESS);
            return E_OS_ACCESS;
        }
    }

    /* is this task belong to an os application ? */
    if (g_TaskCb[g_Kernel.ActTaskId].ConnectApplicationId != OS_NOT_CONNECTED_TO_ANY_APPLICATION)
    {
        /* belong the actual task and the to activated alarm to the different os application ? */
        if(g_TaskCb[g_Kernel.ActTaskId].ConnectApplicationId != g_AlarmCb[AlarmID].ConnectApplicationId )
        {
            /* has the current os application the access rights for the 'AlarmID' ? */
            if ( g_AlarmCb[AlarmID].ApplicationHasAccess[g_TaskCb[g_Kernel.ActTaskId].ConnectApplicationId] == OS_FALSE)
            {
                /* [SWS_Os_00589] All functions that are not allowed to operate cross core shall return E_OS_CORE in extended status if called with parameters that require a cross core operation. */
                ERRORHOOK(E_OS_CORE);
                return E_OS_CORE;
            }
        }
    }
#endif

    if (OS_FALSE == g_CtrCb[g_AlarmCb[AlarmID].Os_Counter].IsInitialized)
    {
        /* increment is always >= GeneralOffset, since offset is added to increment by CG */
        increment -= g_AlarmCb[AlarmID].GeneralOffset;
        increment += 1; /* adjust for sys time = -1 */
    }

    if(increment == 0)
    {
        increment = 1;
    }

    g_AlarmCb[AlarmID].IsStarted = OS_TRUE;

    /* stay in range of uint32 with calculations */
    /* counter value + increment > max  ==>  cv > max - inc */
    if(g_CtrCb[g_AlarmCb[AlarmID].Os_Counter].CtrValue >
        g_CtrCb[g_AlarmCb[AlarmID].Os_Counter].MaxAllowedValue - increment
        && OS_TRUE == g_CtrCb[g_AlarmCb[AlarmID].Os_Counter].IsInitialized) /* -1 = FFFFFFFF => so would be >, but not wanted */
    {
        /* cv + inc - max - 1  ==>  inc - (max - cv) - 1 */
        g_AlarmCb[AlarmID].NextOcourence =
            increment -
            (g_CtrCb[g_AlarmCb[AlarmID].Os_Counter].MaxAllowedValue -
            g_CtrCb[g_AlarmCb[AlarmID].Os_Counter].CtrValue) -
            1;
    }
    else
    {
        g_AlarmCb[AlarmID].NextOcourence =
            g_CtrCb[g_AlarmCb[AlarmID].Os_Counter].CtrValue + increment;
    }

    g_AlarmCb[AlarmID].Cycle = cycle;

    return E_OK;
}

StatusType SetAbsAlarm(AlarmType AlarmID, TickType start, TickType cycle)
{
#ifdef OS_DBGMSG_ENABLED
    Os_SimulationCoreLogDbgMessage(__FUNCTION__, "SetAbsAlarm(): Start of function. Alarm ID is %d.", AlarmID);
#endif

    g_ErrorInfo.ServiceID = OSServiceId_SetAbsAlarm;
    g_ErrorInfo.Service.SvSetAbsAlarm.AlarmID = AlarmID;
    g_ErrorInfo.Service.SvSetAbsAlarm.start = start;
    g_ErrorInfo.Service.SvSetAbsAlarm.cycle = cycle;

    if(AlarmID >= NUMBER_OF_ALARMS)
    {
        ERRORHOOK(E_OS_ID);
        return E_OS_ID;
    }

    if(g_AlarmCb[AlarmID].IsStarted == OS_TRUE)
    {
        ERRORHOOK(E_OS_STATE);
        return E_OS_STATE;
    }

#if NUMBER_OF_OS_APPLICATIONS > 0
    /* is the OS-Application, which belongs to the alarm, started? */
    if (g_AlarmCb[AlarmID].ConnectApplicationId != OS_NOT_CONNECTED_TO_ANY_APPLICATION)
    {
        if( (g_ApplicationCb[g_AlarmCb[AlarmID].ConnectApplicationId].State == APPLICATION_RESTARTING) ||
            (g_ApplicationCb[g_AlarmCb[AlarmID].ConnectApplicationId].State == APPLICATION_TERMINATED) )
        {
            /* [SWS_Os_00509] If a service call is made on an Operating System object that is owned by another OS-Application without state APPLICATION_ACCESSIBLE, then the Operating System module shall return E_OS_ACCESS. */
            ERRORHOOK(E_OS_ACCESS);
            return E_OS_ACCESS;
        }
    }

    /* is this task belong to an os application ? */
    if (g_TaskCb[g_Kernel.ActTaskId].ConnectApplicationId != OS_NOT_CONNECTED_TO_ANY_APPLICATION)
    {
        /* belong the actual task and the to activated alarm to the different os application ? */
        if(g_TaskCb[g_Kernel.ActTaskId].ConnectApplicationId != g_AlarmCb[AlarmID].ConnectApplicationId )
        {
            /* has the current os application the access rights for the 'AlarmID' ? */
            if ( g_AlarmCb[AlarmID].ApplicationHasAccess[g_TaskCb[g_Kernel.ActTaskId].ConnectApplicationId] == OS_FALSE)
            {
                /* [SWS_Os_00589] All functions that are not allowed to operate cross core shall return E_OS_CORE in extended status if called with parameters that require a cross core operation. */
                ERRORHOOK(E_OS_CORE);
                return E_OS_CORE;
            }
        }
    }
#endif

    if (OS_FALSE == g_CtrCb[g_AlarmCb[AlarmID].Os_Counter].IsInitialized)
    {
        start -= g_AlarmCb[AlarmID].GeneralOffset;
    }

    if ( (start > g_CtrCb[g_AlarmCb[AlarmID].Os_Counter].MaxAllowedValue) ||
        (cycle > g_CtrCb[g_AlarmCb[AlarmID].Os_Counter].MaxAllowedValue) ||
        ((cycle != 0) && (cycle < g_CtrCb[g_AlarmCb[AlarmID].Os_Counter].MinCycle))  )
    {
        ERRORHOOK(E_OS_VALUE);
        return E_OS_VALUE;
    }

    g_AlarmCb[AlarmID].IsStarted = OS_TRUE;
    g_AlarmCb[AlarmID].NextOcourence = start;
    g_AlarmCb[AlarmID].Cycle = cycle;

    return E_OK;
}

StatusType CancelAlarm(AlarmType AlarmID)
{
#ifdef OS_DBGMSG_ENABLED
    Os_SimulationCoreLogDbgMessage(__FUNCTION__, "CancelAlarm(): Start of function. Alarm ID is %d.", AlarmID);
#endif

    g_ErrorInfo.ServiceID = OSServiceId_CancelAlarm;
    g_ErrorInfo.Service.SvCancelAlarm.AlarmID = AlarmID;

    if(AlarmID >= NUMBER_OF_ALARMS)
    {
        ERRORHOOK(E_OS_ID);
        return E_OS_ID;
    }

    if(g_AlarmCb[AlarmID].IsStarted == OS_FALSE)
    {
        ERRORHOOK(E_OS_NOFUNC);
        return E_OS_NOFUNC;
    }

#if NUMBER_OF_OS_APPLICATIONS > 0
    /* is the OS-Application, which belongs to the alarm, started? */
    if (g_AlarmCb[AlarmID].ConnectApplicationId != OS_NOT_CONNECTED_TO_ANY_APPLICATION)
    {
        if( (g_ApplicationCb[g_AlarmCb[AlarmID].ConnectApplicationId].State == APPLICATION_RESTARTING) ||
            (g_ApplicationCb[g_AlarmCb[AlarmID].ConnectApplicationId].State == APPLICATION_TERMINATED) )

        {
            /* [SWS_Os_00509] If a service call is made on an Operating System object that is owned by another OS-Application without state APPLICATION_ACCESSIBLE, then the Operating System module shall return E_OS_ACCESS. */
            ERRORHOOK(E_OS_ACCESS);
            return E_OS_ACCESS;
        }
    }

    /* is this task belong to an os application ? */
    if (g_TaskCb[g_Kernel.ActTaskId].ConnectApplicationId != OS_NOT_CONNECTED_TO_ANY_APPLICATION)
    {
        /* belong the actual task and the to activated alarm to the different os application ? */
        if(g_TaskCb[g_Kernel.ActTaskId].ConnectApplicationId != g_AlarmCb[AlarmID].ConnectApplicationId )
        {
            /* has the current os application the access rights for the 'AlarmID' ? */
            if ( g_AlarmCb[AlarmID].ApplicationHasAccess[g_TaskCb[g_Kernel.ActTaskId].ConnectApplicationId] == OS_FALSE)
            {
                /* [SWS_Os_00589] All functions that are not allowed to operate cross core shall return E_OS_CORE in extended status if called with parameters that require a cross core operation. */
                ERRORHOOK(E_OS_CORE);
                return E_OS_CORE;
            }
        }
    }
#endif

    g_AlarmCb[AlarmID].IsStarted = OS_FALSE;
    g_AlarmCb[AlarmID].Cycle = (TickType)0u;
    g_AlarmCb[AlarmID].NextOcourence = (TickType)0u;

    return E_OK;
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     Os_AlarmHandler
 ***
 *** DESCRIPTION:
 ***     If the counter drives an alarm this function
 ***     will activate the tasks or set events of these alarms.
 ***
 *** PARAMETERS:
 ***     Type             Name     Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     CounterType      CounterID The ID of the counter
 ***
 *** RETURNS:
 ***     StatusType
\**************************************************************************************************/
StatusType Os_AlarmHandler(CounterType CounterID)
{
    StatusType returnValue = E_OK;

    AlarmType AlId;

    for(AlId = 0; AlId < NUMBER_OF_ALARMS; AlId++)
    {
        if (g_CtrCb[CounterID].ConnectedAlarms[AlId] == OS_TRUE)
        { /* This Alarm is connected to this counter and it belongs to the actual TL-SS*/
            if (g_AlarmCb[AlId].IsStarted == OS_TRUE)
            { /* This alarm is active */
                if (g_CtrCb[CounterID].CtrValue == g_AlarmCb[AlId].NextOcourence)
                { /* This alarm has to trigger its action */
                    if (g_AlarmCb[AlId].Cycle == (TickType)0u)
                    {
                        g_AlarmCb[AlId].IsStarted = OS_FALSE;
                    }
                    else /* Alarm is cyclic and therefor it remains active */
                    {
                        /* stay in uint32 range with all calculations */
                        /* no + cyc <= max  ==>  no <= max - cyc */
                        if (g_AlarmCb[AlId].NextOcourence <=
                            g_CtrCb[CounterID].MaxAllowedValue - g_AlarmCb[AlId].Cycle)
                        {
                            g_AlarmCb[AlId].NextOcourence =
                                g_AlarmCb[AlId].NextOcourence + g_AlarmCb[AlId].Cycle;
                        }
                        else
                        {
                            /* no + cyc - max - 1  ==>  cyc - (max - no) - 1 */
                            g_AlarmCb[AlId].NextOcourence =
                                g_AlarmCb[AlId].Cycle -
                                (g_CtrCb[CounterID].MaxAllowedValue - g_AlarmCb[AlId].NextOcourence)
                                - 1;
                        }
                    }

                    switch (g_AlarmCb[AlId].Action)
                    {
                    case OS_ALARMACTION_ACTIVATETASK:
#ifdef OS_DBGMSG_ENABLED
                        Os_SimulationCoreLogDbgMessage(__FUNCTION__, "Os_CounterTriggerSolely(): Alarm with ID %d executes action ACTIVATETASK for task with ID %d.", AlId, g_AlarmCb[AlId].Os_Task);
#endif
                        if (g_AlarmCb[AlId].Os_Task >= NUMBER_OF_TASKS)
                        {
                            ERRORHOOK(E_OS_SYS_INTERNAL);
                            returnValue = (E_OS_SYS_INTERNAL);
                            continue; /* Go to next loop element */
                        }

                        if (g_TaskCb[g_AlarmCb[AlId].Os_Task].NumberOfAct >= g_TaskCb[g_AlarmCb[AlId].Os_Task].MaxAct)
                        {
                            if (g_Kernel.IsSleeping == OS_VECU_IS_SLEEPING)
                            {
                                /* avoids the OS_E_LIMIT warnings during V-ECU is sleeping */
                            }
                            else
                            {
                                ERRORHOOK(E_OS_LIMIT);
                                g_NumberOfTaskOverruns[g_AlarmCb[AlId].Os_Task]++;
                                returnValue = (E_OS_LIMIT);
                                continue; /* Go to next loop element */
                            }
                        }

                        if (g_Kernel.IsSleeping == OS_VECU_IS_SLEEPING)
                        {
                            /* during V-ECU is sleeping increase the 'NumberOfAct' only until max activations are reached. */
                            if (g_TaskCb[g_AlarmCb[AlId].Os_Task].NumberOfAct < g_TaskCb[g_AlarmCb[AlId].Os_Task].MaxAct)
                            {
                                g_TaskCb[g_AlarmCb[AlId].Os_Task].NumberOfAct++;
                            }
                        }
                        else
                        {
                            g_TaskCb[g_AlarmCb[AlId].Os_Task].NumberOfAct++;
                        }
                        
#ifdef OS_DBGMSG_ENABLED
                        Os_SimulationCoreLogDbgMessage(__FUNCTION__, "Os_CounterTriggerSolely(): Task with ID %d switches to state READY.", g_AlarmCb[AlId].Os_Task);
#endif
                        Os_ChangeTaskState(g_AlarmCb[AlId].Os_Task, READY);
                        break;

                    case OS_ALARMACTION_SETEVENT:
#ifdef OS_DBGMSG_ENABLED
                        Os_SimulationCoreLogDbgMessage(__FUNCTION__, "Os_CounterTriggerSolely(): Alarm with ID %d executes action SETEVENT.", AlId);
#endif
                        if (g_AlarmCb[AlId].Os_Task > NUMBER_OF_TASKS)
                        {
                            ERRORHOOK(E_OS_SYS_INTERNAL);
                            returnValue = (E_OS_SYS_INTERNAL);
                            continue; /* Go to next loop element */
                        }

                        if (g_TaskCb[g_AlarmCb[AlId].Os_Task].ExtendedTask != OS_TRUE)
                        {
                            ERRORHOOK(E_OS_ACCESS);
                            returnValue = (E_OS_ACCESS);
                            continue; /* Go to next loop element */
                        }

                        if (g_TaskCb[g_AlarmCb[AlId].Os_Task].State == SUSPENDED)
                        {
                            ERRORHOOK(E_OS_STATE);
                            returnValue = (E_OS_STATE);
                            continue; /* Go to next loop element */
                        }
                    
                        if (g_Kernel.IsSleeping == OS_VECU_IS_SLEEPING)
                        {
                            /* During V-ECU is sleeping the cyclic events, called from an alarms, are not set. */
                        }
                        else
                        {
                            g_TaskCb[g_AlarmCb[AlId].Os_Task].Events |= g_AlarmCb[AlId].Os_Event;
                        }

                        if (g_TaskCb[g_AlarmCb[AlId].Os_Task].State == WAITING)
                        {
                            /* Set task to READY state only if events occurr for which the task is waiting */
                            if ((g_TaskCb[g_AlarmCb[AlId].Os_Task].EventsWaiting & g_AlarmCb[AlId].Os_Event) == g_AlarmCb[AlId].Os_Event)
                            {
#ifdef OS_DBGMSG_ENABLED
                                Os_SimulationCoreLogDbgMessage(__FUNCTION__, "Os_CounterTriggerSolely(): Task with ID %d switches to state READY.", g_AlarmCb[AlId].Os_Task);
#endif
                                Os_ChangeTaskState(g_AlarmCb[AlId].Os_Task, READY);
                            }
                        }
                        break;

                    case OS_ALARMACTION_CALLBACK:
#ifdef OS_DBGMSG_ENABLED
                        Os_SimulationCoreLogDbgMessage(__FUNCTION__, "Os_CounterTriggerSolely(): Alarm with ID %d executes action CALLBACK.", AlId);
#endif
                        (*(g_AlarmCb[AlId].pCallbackFcn))();
                        break;

                    case OS_ALARMACION_INCREMENTCOUNTER:
#ifdef OS_DBGMSG_ENABLED
                        Os_SimulationCoreLogDbgMessage(__FUNCTION__, "Os_CounterTriggerSolely(): Alarm with ID %d executes action INCREMENTCOUNTER.", AlId);
#endif
                        Os_CounterTriggerSolely(g_AlarmCb[AlId].IncCounter);
                        break;

                    case OS_ALARMACTION_NONE:
                        break;

                    default:
                        ERRORHOOK(E_OS_SYS_INTERNAL);
                        returnValue = (E_OS_SYS_INTERNAL);
                        continue; /* Go to next loop element */
                    } /* switch(g_AlarmCb[AlId].Action) */
                } /* if(g_CtrCb[CounterID]...*/
            } /* if(g_AlarmCb[AlId].IsStarted == OS_TRUE) */
        } /* if( (g_CtrCb[CounterID]... */
    } /* for(AlId = 0; AlId < NUMBER_OF_ALARMS; AlId++) */

    return returnValue;
}

#endif  /* NUMBER_OF_ALARMS > 0 */

#endif /* OS_ALARM_C */

/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/
