/**************************************************************************************************\
 *** Os_Application.c
 ***
 *** OSEK API function definition for OS Application handling.
 ***
 *** Copyright (c) 2018 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef OS_APPLICATION_C
#define OS_APPLICATION_C

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/
#include "Os_SimulationCore.h"
#include "Os_Kernel.h"
#include "Os_Native.h"
#include "Os.h"

#include "Os_Application.h"
#include "Os_Task.h"
#include "Os_Spinlock.h"
#include "Os_ResInt.h"
#include "Os_Frame.h"

/*------------------------------------------------------------------------------------------------*\
  FUNCTION DEFINITIONS
\*------------------------------------------------------------------------------------------------*/

#if (NUMBER_OF_OS_APPLICATIONS > 0)
static void Os_DeactivateAllSpinlocks(ApplicationType ApplicationID);
static void Os_DeactivateAllResources(ApplicationType ApplicationID);
static void Os_CancelAlarmAndScheduleTable(ApplicationType ApplicationID);
static void Os_DeactivateAllTasks(ApplicationType ApplicationID, RestartType RestartOption);
#endif /* (NUMBER_OF_OS_APPLICATIONS > 0) */

/**************************************************************************************************\
 *** FUNCTION:
 ***   GetApplicationID
 ***
 *** DESCRIPTION:
 ***   This service determines the OS-Application (a unique identifier has to be allocated to each application)
 ***   where the caller originally belongs to (was configured to).
 ***    [SWS_Os_00016]
 ***
 *** PARAMETERS:
 ***     Type             Name     Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***
 ***
 *** RETURNS: Os_ApplicationType  <identifier of running OS-Application> or INVALID_OSAPPLICATION
 ***
\**************************************************************************************************/
ApplicationType GetApplicationID(void)
{
    g_ErrorInfo.ServiceID = OSServiceId_GetApplicationID;

#if (NUMBER_OF_OS_APPLICATIONS > 0)
    {
        ApplicationType idxApplication;

        switch(g_Kernel.CallLevel)
        {
            case TASK_LEVEL:
            case PRETASKHOOK_LEVEL:
            case POSTTASKHOOK_LEVEL:
            case ERRORHOOK_LEVEL:
                for(idxApplication = 0; idxApplication < NUMBER_OF_OS_APPLICATIONS; idxApplication++)
                {
                    if( g_ApplicationCb[idxApplication].ConnectedTasks[g_Kernel.ActTaskId] == OS_TRUE)
                    {
                        if( g_ApplicationCb[idxApplication].IsStarted == OS_FALSE)
                        {
                            /* [SWS_Os_00262] If no OS-Application is running, GetApplicationID() shall return INVALID_OSAPPLICATION. */
                            return INVALID_OSAPPLICATION;
                        }
                        return idxApplication;
                    }
                }
            break;
            case OS_LEVEL:
                return INVALID_OSAPPLICATION;
            break;
            case ISR_LEVEL:
                return INVALID_OSAPPLICATION;
            break;
            case STARTUPHOOK_LEVEL:
            case SHUTDOWNHOOK_LEVEL:
            /* Note: A GetApplicationID() in STARTUPHOOK or STARTUPHOOK will returns the correct application ID */
            if(g_Kernel.ActApplicationIdForHookFunction != OS_UNKNOWN_APPLICATION_ID)
            {
                return g_Kernel.ActApplicationIdForHookFunction;
            }
            else
            {
                return INVALID_OSAPPLICATION;
            }
            break;
            default:
            break;
        }
    }
    return INVALID_OSAPPLICATION;
#else
    return INVALID_OSAPPLICATION;
#endif /* (NUMBER_OF_OS_APPLICATIONS > 0) */
}

/**************************************************************************************************\
 *** FUNCTION:
 ***    GetCurrentApplicationID
 ***
 *** DESCRIPTION:
 ***    This service determines the OS-Application where the caller of the service is currently executing.
 ***    Note that if the caller is not within a CallTrustedFunction() call the value is equal to the result of GetApplicationID().
 ***    [SWS_Os_00797]
 ***
 *** PARAMETERS:
 ***     Type             Name     Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***
 ***
 *** RETURNS: ApplicationType  <identifier of running OS-Application> or INVALID_OSAPPLICATION
 ***
\**************************************************************************************************/
ApplicationType GetCurrentApplicationID(void)
{
   return GetApplicationID();
}

/**************************************************************************************************\
 *** FUNCTION:
 ***    GetApplicationState
 ***
 *** DESCRIPTION:
 ***    This service returns the current state of an OS-Application
 ***    [SWS_Os_00499]
 ***
 ***
 *** PARAMETERS:
 ***     Type                        Name              Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     ApplicationType          ApplicationID     In:  The OS-Application from which the state is requested
 ***     ApplicationStateRefType  Value             Out: The current state of the application
 ***
 *** RETURNS: StatusType  E_OK: No errors
 ***                      E_OS_ID: <Application> is not valid
 ***
\**************************************************************************************************/
StatusType GetApplicationState( ApplicationType ApplicationID, ApplicationStateRefType Value )
{
    g_ErrorInfo.ServiceID = OSServiceId_GetApplicationState;
    g_ErrorInfo.Service.SvGetApplicationState.ApplicationID = ApplicationID;

#if (NUMBER_OF_OS_APPLICATIONS > 0)

    /* [SWS_Os_00495] If the <Application> in a call of GetApplicationState() is not valid GetApplicationState() shall return E_OS_ID. */
    if(ApplicationID >= NUMBER_OF_OS_APPLICATIONS)
    {
        ERRORHOOK(E_OS_ID);
        return E_OS_ID;
    }

    /* [SWS_Os_00496] If the parameters in a call of GetApplicationState() are valid, GetApplicationState() shall return the state of OS-Application <Application> in <Value>. */
    *Value = g_ApplicationCb[ApplicationID].State;
    return E_OK;

#else
    /* to avoid the compiler warning C4100 */
    (void)(Value);

    ERRORHOOK(E_OS_ID);
    return E_OS_ID;
#endif /* (NUMBER_OF_OS_APPLICATIONS > 0) */
}

/**************************************************************************************************\
 *** FUNCTION:
 ***    AllowAccess
 ***
 *** DESCRIPTION:
 ***    This service sets the own state of an OS-Application from APPLICATION_RESTARTING to APPLICATION_ACCESSIBLE
 ***    [SWS_Os_00501]
 ***
 ***
 *** PARAMETERS:
 ***     Type                        Name              Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***
 *** RETURNS: StatusType  E_OK: No errors
 ***                      E_OS_STATE:The OS-Application of the caller is in the wrong state
 ***
\**************************************************************************************************/
StatusType AllowAccess(void)
{
    g_ErrorInfo.ServiceID = OSServiceId_AllowAccess;

#if (NUMBER_OF_OS_APPLICATIONS > 0)

    /* is this task belong to an os application ? */
    if (g_TaskCb[g_Kernel.ActTaskId].ConnectApplicationId != OS_NOT_CONNECTED_TO_ANY_APPLICATION)
    {
        if (g_ApplicationCb[g_TaskCb[g_Kernel.ActTaskId].ConnectApplicationId].State == APPLICATION_RESTARTING)
        {
            /* [SWS_Os_00498] If the state of the OS-Application of the caller of AllowAccess() is APPLICATION_RESTARTING, AllowAccess() shall set the state to APPLICATION_ACCESSIBLE and allow other OS-Applications to access the configured objects of the callers OS-Application. */
            g_ApplicationCb[g_TaskCb[g_Kernel.ActTaskId].ConnectApplicationId].State = APPLICATION_ACCESSIBLE;
            return E_OK;
        }
    }
    /* [SWS_Os_00497] If the state of the OS-Application of the caller of AllowAccess() is not APPLICATION_RESTARTING AllowAccess() shall return E_OS_STATE. */
    ERRORHOOK(E_OS_STATE);
    return E_OS_STATE;

#else
    ERRORHOOK(E_OS_STATE);
    return E_OS_STATE;
#endif /* (NUMBER_OF_OS_APPLICATIONS > 0) */
}

/**************************************************************************************************\
 *** FUNCTION:
 ***    TerminateApplication
 ***
 *** DESCRIPTION:
 ***    This service terminates the OS-Application to which the calling Task/Category 2 ISR/application specific error hook belongs
 ***    [SWS_Os_00258]
 ***
 ***
 *** PARAMETERS:
 ***     Type                        Name              Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     ApplicationType          ApplicationID     In: The OS-Application from which the state is requested
 ***     RestartType              RestartOption     In: Either RESTART for doing a restart of the OS-Application
 ***                                                       or NO_RESTART if OS-Application shall not be restarted.
 ***
 *** RETURNS: StatusType
 ***             E_OK: No errors
 ***             E_OS_ID: <Application> was not valid (only in EXTENDED status)
 ***             E_OS_VALUE: <RestartOption> was neither RESTART nor NO_RESTART (only in EXTENDED status)
 ***             E_OS_ACCESS: The caller does not have the right to terminate <Application> (only in EXTENDED status)
 ***             E_OS_STATE: The state of <Application> does not allow terminating <Application>
 ***
\**************************************************************************************************/
StatusType TerminateApplication( ApplicationType ApplicationID, RestartType RestartOption )
{
    g_ErrorInfo.ServiceID = OSServiceId_TerminateApplication;
    g_ErrorInfo.Service.SvTerminateApplication.ApplicationID = ApplicationID;

#if (NUMBER_OF_OS_APPLICATIONS > 0)

    return Os_TerminateApplicationInternal( ApplicationID, RestartOption, E_OK );

#else
    /* [SWS_Os_00535] Caveats of TerminateApplication(): If no applications are configured the implementation shall make sure that this service is not available. */

    /* to avoid the compiler warning C4100 */
    (void)(RestartOption);

    ERRORHOOK(E_OS_ID);
    return E_OS_ID;
#endif /* (NUMBER_OF_OS_APPLICATIONS > 0) */
}

#if (NUMBER_OF_OS_APPLICATIONS > 0)

/**************************************************************************************************\
 *** FUNCTION:
 ***     Os_TerminateApplicationInternal
 ***
 *** DESCRIPTION:
 **      Os_TerminateApplication() functionality with an addtional parameter 'StatusType Error'
 ***
\**************************************************************************************************/
StatusType Os_TerminateApplicationInternal( ApplicationType ApplicationID, RestartType RestartOption, StatusType Error )
{
    g_ErrorInfo.ServiceID = OSServiceId_TerminateApplication;
    g_ErrorInfo.Service.SvTerminateApplication.ApplicationID = ApplicationID;

    /* [SWS_Os_00493] If the input parameter <Application> in a call of TerminateApplication() is not valid TerminateApplication() shall return E_OS_ID. */
    if(ApplicationID >= NUMBER_OF_OS_APPLICATIONS)
    {
        ERRORHOOK(E_OS_ID);
        return E_OS_ID;
    }

    if( (RestartOption == RESTART) || (RestartOption == NO_RESTART) )
    {

        ApplicationType currentApplicationID;

        if(g_ApplicationCb[ApplicationID].State == APPLICATION_TERMINATED)
        {
            /* [SWS_Os_00507] If the state of <Application> in a call of TerminateApplication() is APPLICATION_TERMINATED TerminateApplication() shall return E_OS_STATE. */
            ERRORHOOK(E_OS_STATE);
            return E_OS_STATE;
        }

        currentApplicationID = g_TaskCb[g_Kernel.ActTaskId].ConnectApplicationId;
        if(currentApplicationID == OS_NOT_CONNECTED_TO_ANY_APPLICATION)
        {
            /*return, if the task belongs to no OS-Application */
            ERRORHOOK(E_OS_ACCESS);
            return E_OS_ACCESS;
        }

        if ( (!g_ApplicationCb[currentApplicationID].IsTrusted) && (currentApplicationID != ApplicationID) )
        {
            /* [SWS_Os_00494] If the input parameter <Application> in a call of TerminateApplication() is valid
                AND the caller belongs to a non-trusted OS-Application AND the caller does not belong to <Application> TerminateApplication() shall return E_OS_ACCESS. */
            ERRORHOOK(E_OS_ACCESS);
            return E_OS_ACCESS;
        }

        /* ShutdownHook_<App> is called */
        if(g_ApplicationCb[ApplicationID].pShutdownHook != OS_NULL)
        {
            g_Kernel.CallLevel = SHUTDOWNHOOK_LEVEL;
            g_Kernel.ActCoreIdForHookFunction = g_ApplicationCb[ApplicationID].CoreId;
            g_Kernel.ActApplicationIdForHookFunction = ApplicationID;
            (*(g_ApplicationCb[ApplicationID].pShutdownHook))(Error);
            g_Kernel.ActCoreIdForHookFunction = OS_UNKNOWN_CORE_ID;
            g_Kernel.ActApplicationIdForHookFunction = OS_UNKNOWN_APPLICATION_ID;
            g_Kernel.CallLevel = TASK_LEVEL;
        }

        /* [SWS_Os_00287] If the parameters in a call of TerminateApplication() are valid and the above criteria are met TerminateApplication()
              shall terminate <Application> (i.e. to kill all tasks, disable the interrupt sources of those ISRs which belong to the OS-Application
              and free all other OS resources associated with the application) AND shall activate the configured OsRestartTask of <Application>
              if <RestartOption> equals RESTART. If the <Application> is restarted, its state is set to APPLICATION_RESTARTING otherwise to APPLICATION_TERMINATED.
              If the caller belongs to <Application> TerminateApplication() shall not return, otherwise it shall return E_OK. */
        /* [SWS_Os_00447] if the Operating System module terminates an OS-Application, then it shall:
             * terminate all running, ready and waiting Tasks/ISRs of the OS-Application AND
             * disable all interrupts of the OS-Application AND
             * stop all active alarms of the OS-Applications AND
             * stop all schedule tables of the OS-Application. */

        /* deactivate all spinlock(s) */
        Os_DeactivateAllSpinlocks(ApplicationID);

        /* deactivate all resource(s) */
        Os_DeactivateAllResources(ApplicationID);

        /* cancel alarm(s) and scheduleTable(s) */
        Os_CancelAlarmAndScheduleTable(ApplicationID);

        /* deactivate all task(s) */
        Os_DeactivateAllTasks(ApplicationID, RestartOption);

        if(RestartOption == NO_RESTART)
        {
            /* [SWS_Os_00502] If an OS-Application is terminated  and no restart is requested, then the Operating System module shall set the state of this OS-Application to APPLICATION_TERMINATED. */
            g_ApplicationCb[ApplicationID].State = APPLICATION_TERMINATED;
        }
        else
        {
            /* Start alarms for cyclic actions, which belong to an Os application with an 'autostart' flag */
            Os_Autostart(g_Kernel.AppMode, ApplicationID);

            /* [SWS_Os_00503] If an OS-Application is terminated (e.g. through a service call or via protection hook) and a restart is requested, then the Operating System module shall set the state of this OS-Application to APPLICATION_RESTARTING. */
            g_ApplicationCb[ApplicationID].State = APPLICATION_RESTARTING;

            if(g_ApplicationCb[ApplicationID].pStartupHook != OS_NULL)
            {
                g_Kernel.CallLevel = STARTUPHOOK_LEVEL;
                g_Kernel.ActCoreIdForHookFunction = g_ApplicationCb[ApplicationID].CoreId;
                g_Kernel.ActApplicationIdForHookFunction = ApplicationID;
                (*(g_ApplicationCb[ApplicationID].pStartupHook))();
                g_Kernel.ActCoreIdForHookFunction = OS_UNKNOWN_CORE_ID;
                g_Kernel.ActApplicationIdForHookFunction = OS_UNKNOWN_APPLICATION_ID;
                g_Kernel.CallLevel = TASK_LEVEL;
            }

            /* [SWS_Os_00500] The Operating System module shall set the state of all OS-Applications after the call of StartOS() and before any StartupHook is called to APPLICATION_ACCESSIBE. */
            g_ApplicationCb[ApplicationID].IsStarted = OS_TRUE;

            /* [SWS_Os_00111] When the Operating System module restarts an OS-Application, it shall activate the configured OsRestartTask. */
            if(g_ApplicationCb[ApplicationID].RestartTaskId != OS_RESTART_TASK_NOT_DEFINED)
            {
                Os_SetTaskReady(g_ApplicationCb[ApplicationID].RestartTaskId);
            }
        }

        /* If the caller belongs to <Application> TerminateApplication() shall not return, otherwise it shall return E_OK. */
        if(currentApplicationID != ApplicationID)
        {
            return E_OK;
        }
        else
        {
            g_TaskCb[g_Kernel.ActTaskId].NumberOfAct = 1;

            TerminateTask();
            OS_LONGJMP(returnToTask[g_Kernel.ActTaskId], 1);
        }
    }
    else
    {
        /* [SWS_Os_00459] If the <RestartOption> in a call of TerminateApplication() is invalid, TerminateApplication() shall return E_OS_VALUE. */
        ERRORHOOK(E_OS_VALUE);
        return E_OS_VALUE;
    }

}

/**************************************************************************************************\
 *** FUNCTION:
 ***     Os_DeactivateAllSpinlocks
 ***
 *** DESCRIPTION:
 **      All spinlocks which belong to the OS application are deactivated
 ***
\**************************************************************************************************/
static void Os_DeactivateAllSpinlocks(ApplicationType ApplicationID)
{

#if (NUMBER_OF_SPINLOCKS > 0)
    SpinlockIdType topSpinlockID;

    /* [SWS_Os_00614] TerminateApplication shall check if any of the TASKs in the OS-Application have occupied a spinlock. If so, the spinlocks shall be released. */
    topSpinlockID = Os_GetCurentSpinlockId(g_ApplicationCb[ApplicationID].CoreId);
    while ( topSpinlockID != OS_NOT_LOCKED_BY_ANY_SPINLOCK)
    {
        if ( (g_SpinlockCb[topSpinlockID].OccupiedByTaskId != OS_NOT_LOCKED_BY_ANY_TASKS) &&
              (g_ApplicationCb[ApplicationID].ConnectedTasks[g_SpinlockCb[topSpinlockID].OccupiedByTaskId] == OS_TRUE) )
        /*  is this spinlock occupied by a task that belong to the Os application? */
        {
            /* ok, the spinlock can unlocked in strict LIFO order */
        }
        else
        {
            /* nok, the spinlock can not unlocked in strict LIFO order */
            Os_SimulationCoreShowWarningMessage("A TerminateApplication() can not unlocked the spinlock ID=%d in strict LIFO order. The nesting spinlock ID=%d is occupied by a task that belong to other Os application.\n", topSpinlockID, topSpinlockID);
        }
        switch(g_SpinlockCb[topSpinlockID].LockMethode)
        {
            case LOCK_ALL_INTERRUPTS:
                ResumeAllInterrupts();
            break;
            case LOCK_CAT2_INTERRUPTS:
                 ResumeOSInterrupts();
            break;
            case LOCK_WITH_RES_SCHEDULER:
                 ReleaseResource(RES_SCHEDULER);
            break;
            case LOCK_NOTHING:
            break;
        }
        Os_PopSpinlockId(g_SpinlockCb[topSpinlockID].IsLockedByCore);
        g_SpinlockCb[topSpinlockID].IsLockedByCore = OS_NOT_LOCKED_BY_ANY_CORE;
        g_SpinlockCb[topSpinlockID].OccupiedByTaskId = OS_NOT_LOCKED_BY_ANY_TASKS;

        topSpinlockID = Os_GetCurentSpinlockId(g_ApplicationCb[ApplicationID].CoreId);
    }
#else
        /* to avoid the compiler warning C4100 */
        (void)(ApplicationID);
#endif /* (NUMBER_OF_SPINLOCKS > 0) */

}

/**************************************************************************************************\
 *** FUNCTION:
 ***     Os_DeactivateAllResources
 ***
 *** DESCRIPTION:
 **      All resources which belong to the OS application are deactivated
 ***
\**************************************************************************************************/
static void Os_DeactivateAllResources(ApplicationType ApplicationID)
{
#if (NUMBER_OF_RESOURCES > 0)
    ResourceType idxResource;

    /* [SWS_Os_00614] TerminateApplication shall check if any of the TASKs in the OS-Application have occupied a spinlock. If so, the spinlocks shall be released. */
    for(idxResource = 0; idxResource < NUMBER_OF_RESOURCES; idxResource++)
    {
        /*  is this resource is occupied by a task that belong to the Os application? */
        if(g_ResCb[idxResource].Occupied == OS_TRUE)
        {
            /* belong the resource 'OwnerTask' to the Os application? */
            if (g_ApplicationCb[ApplicationID].ConnectedTasks[g_ResCb[idxResource].OwnerTask] == OS_TRUE)
            {
                /* ReleaseResource() for task define in 'g_ResCb[idxResource].OwnerTask' */
                Os_SetTaskActPrio(g_ResCb[idxResource].OwnerTask, g_ResCb[idxResource].ReplacedPrio);
                g_TaskCb[g_ResCb[idxResource].OwnerTask].ResourceLevelCnt--;

                g_ResCb[idxResource].Occupied = OS_FALSE;
                g_ResCb[idxResource].ReplacedPrio = (Os_PrioType)0;
                g_ResCb[idxResource].OwnerTask = IDLE_LOOP;
            }
        }
    }
#else
        /* to avoid the compiler warning C4100 */
        (void)(ApplicationID);
#endif /* (NUMBER_OF_SPINLOCKS > 0) */

}

/**************************************************************************************************\
 *** FUNCTION:
 ***     Os_CancelAlarmAndScheduleTable
 ***
 *** DESCRIPTION:
 **      All Alarms and ScheduleTables which belong to the OS application are canceled
 ***
\**************************************************************************************************/
static void Os_CancelAlarmAndScheduleTable(ApplicationType ApplicationID)
{
#if NUMBER_OF_OS_APPLICATIONS > 0
    if(g_ApplicationCb[ApplicationID].State == APPLICATION_ACCESSIBLE)
    {
#if NUMBER_OF_ALARMS > 0
        AlarmType idxConnectedAlarm;
#endif /* NUMBER_OF_ALARMS > 0 */
#if NUMBER_OF_SCHEDULE_TABLES > 0
        ScheduleTableType idxConnectedSchedTable;
#endif /* NUMBER_OF_SCHEDULE_TABLES > 0 */

#if NUMBER_OF_ALARMS > 0
        for(idxConnectedAlarm=0; idxConnectedAlarm<NUMBER_OF_ALARMS; idxConnectedAlarm++)
        {
            if(g_ApplicationCb[ApplicationID].ConnectedAlarms[idxConnectedAlarm] == OS_TRUE)
            {
                if(g_AlarmCb[idxConnectedAlarm].IsStarted == OS_TRUE)
                {
                    CancelAlarm(idxConnectedAlarm);
                }
            }
        }
#endif /* NUMBER_OF_ALARMS > 0 */

#if NUMBER_OF_SCHEDULE_TABLES > 0
        for(idxConnectedSchedTable=0; idxConnectedSchedTable<NUMBER_OF_SCHEDULE_TABLES; idxConnectedSchedTable++)
        {
            if(g_ApplicationCb[ApplicationID].ConnectedSchedTables[idxConnectedSchedTable] == OS_TRUE)
            {
                if(g_SchedTblExec[idxConnectedSchedTable].status != SCHEDULETABLE_STOPPED)
                {
                    StopScheduleTable(idxConnectedSchedTable);
                }
            }
        }
#endif /* NUMBER_OF_SCHEDULE_TABLES > 0 */
    }
#else
    /* to avoid the compiler warning C4100 */
    (void)(ApplicationID);

#endif /* (NUMBER_OF_OS_APPLICATIONS > 0) */
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     Os_DeactivateAllTask
 ***
 *** DESCRIPTION:
 **      All tasks which belong to the OS application are deactivated
 ***
\**************************************************************************************************/
static void Os_DeactivateAllTasks(ApplicationType ApplicationID, RestartType RestartOption)
{
    TaskType idxTask;

    /* start with idxTask=1 ==> never ever delete IDLE_LOOP() */
    for(idxTask=1; idxTask<NUMBER_OF_TASKS; idxTask++)
    {
        if(g_TaskCb[idxTask].ConnectApplicationId == ApplicationID)
        {
            Os_ChangeTaskState(idxTask, SUSPENDED);
            Os_SetTaskActPrio(idxTask, g_TaskCb[idxTask].InitialPrio);
            g_TaskCb[idxTask].NumberOfAct = 0;
            g_TaskCb[idxTask].OsInterruptedTask = IDLE_LOOP;
            g_TaskCb[idxTask].ResourceLevelCnt = 0;

            if(RestartOption == RESTART)
            {
                Os_TaskRestart(idxTask);
            }
        }
    }
}
#endif /* (NUMBER_OF_OS_APPLICATIONS > 0) */

/**************************************************************************************************\
 *** FUNCTION:
 ***     Os_ApplicationErrorHookFilter
 ***
 *** DESCRIPTION:
 **      If the current task is an ownership of an Os application
 ***     then the Os application error hook function will be called
 ***
\**************************************************************************************************/
void Os_ApplicationErrorHookFilter( StatusType Error )
{
#if (NUMBER_OF_OS_APPLICATIONS > 0)
    ApplicationType idxApplication;
    if ( g_Kernel.IsStarted == OS_TRUE)
    {
        for(idxApplication = 0; idxApplication < NUMBER_OF_OS_APPLICATIONS; idxApplication++)
        {
            if(g_ApplicationCb[idxApplication].pErrorHook != OS_NULL)
            {
                if( g_ApplicationCb[idxApplication].ConnectedTasks[g_Kernel.ActTaskId] == OS_TRUE)
                {
                    g_Kernel.PreviousCallLevel = g_Kernel.CallLevel;
                    g_Kernel.CallLevel = ERRORHOOK_LEVEL;
                    /* [SWS_Os_00544] Availability of ErrorHook_<App>() */
                    (*(g_ApplicationCb[idxApplication].pErrorHook))(Error);
                    g_Kernel.CallLevel = g_Kernel.PreviousCallLevel;
                    break;
                }
            }
        }
    }
#else
    /* to avoid the compiler warning C4100 */
    (void)(Error);
#endif /* (NUMBER_OF_OS_APPLICATIONS > 0) */
}

#endif /* OS_APPLICATION_C */

/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/
