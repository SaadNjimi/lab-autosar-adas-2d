/**************************************************************************************************\
 *** Os.c
 ***
 *** OSEK API function definition.
 ***
 *** Copyright (c) 2009 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef OS_C
#define OS_C

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/
#include <setjmp.h>     /* setjmp, longjmp */

#include "Os_InternalTypes.h"
#include "Os_Kernel.h"
#include "Os_Native.h"
#include "Os_Frame.h"
#include "Os_SimulationCore.h"
#include "Os.h"

/*------------------------------------------------------------------------------------------------*\
  DEFINES
\*------------------------------------------------------------------------------------------------*/

#if defined( __GCC__) && defined(__x86_64__) 
   /*
   Note:
   setjmp and longjmp crashes when used GCC with optimization "on"
   Disable "-O0" avoids the problem
   */
  #pragma GCC push_options
  #pragma GCC optimize ("-O0")
#endif


/*------------------------------------------------------------------------------------------------*\
  FORWARD DECLARATIONS
\*------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

#if (DS_DAQ_AVAILABLE == STD_ON)

#endif /* DS_DAQ_AVAILABLE == STD_ON */

#if (NUMBER_OF_CORES > 0)
static void Os_ShutdownOneCoreOnMultiCore(StatusType Error);
#else
static void Os_ShutdownSingleCore(StatusType Error);
#endif /* (NUMBER_OF_CORES > 0) */

#ifdef __cplusplus
}
#endif

/*------------------------------------------------------------------------------------------------*\
 VARIABLE DEFINITIONS
\*------------------------------------------------------------------------------------------------*/
static boolean Os_initSetjmp = FALSE;
static jmp_buf Os_returnToFcnFramePrepareStartOs;

jmp_buf returnToTask[NUMBER_OF_TASKS];

/*------------------------------------------------------------------------------------------------*\
  OS FUNCTION DEFINITIONS
\*------------------------------------------------------------------------------------------------*/

AppModeType GetActiveApplicationMode(void)
{
    g_ErrorInfo.ServiceID = OSServiceId_GetActiveApplicationMode;

    return g_Kernel.AppMode;
}


void Os_SysScheduleSimple(void)
{
  Os_ScheduleInternal(OS_FALSE, OS_FALSE);
}

StatusType Schedule(void)
{
#ifdef OS_DBGMSG_ENABLED
    Os_SimulationCoreLogDbgMessage(__FUNCTION__, "Schedule(): Start of function.");
#endif

    g_ErrorInfo.ServiceID = OSServiceId_Schedule;

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

    Os_ScheduleInternal(OS_TRUE, OS_FALSE);

    return E_OK;
}

sint32 Os_FramePrepareStartOs(void)
{

    /* Save stack environment for return from Os_StartOs() */
    /*  - First time, setjmp returns a 0 */
    /*  - After a jump back from Os_StartOs(), setjmp returns a -1 */
    Os_initSetjmp = TRUE;
    return OS_SETJMP( Os_returnToFcnFramePrepareStartOs );
}

static void Os_FrmLockStartOs(void)
{
    if (Os_initSetjmp == TRUE)
    {
        /* jump directly to Os_FramePrepareStartOs() - > VEcuAppFrm_ChangeState() in VEcuAppFrm.c */
        OS_LONGJMP(Os_returnToFcnFramePrepareStartOs, 1);
    }
    else
    {
        /* return normally via call stack */
    }
}

void StartOS(AppModeType Mode)
{
    TaskType Tid;
    static Os_BoolType firstRun = OS_TRUE;
    static Os_BoolType lastRun  = OS_FALSE;

#ifdef OS_DBGMSG_ENABLED
    Os_SimulationCoreLogDbgMessage(__FUNCTION__, "StartOS(): Start of function. Application mode is %d.", Mode);
#endif

    g_ErrorInfo.ServiceID = OSServiceId_StartOS;

    g_ErrorInfo.Service.SvStartOS.Mode = Mode;
    g_ErrorInfo.Error = E_OK;

    if ( (Mode >= NUMBER_OF_APPMODES) &&  (Mode != DONOTCARE) )
    {
        ERRORHOOK(E_OS_ID);
    }
    if (Mode != DONOTCARE)
    {
        /* do not save OS_DONOTCARE */
        g_Kernel.AppMode = Mode;
    }

    /* [SWS_Os_00579] 
        The Os is running and it is not allowed to call StartOS*/
    if( g_Kernel.IsStarted == OS_TRUE)
    {
        CoreIdType coreID;
        coreID = GetCoreID();
        TaskType taskID;
        (void)GetTaskID(&taskID);
        Os_SimulationCoreShowWarningMessage("All cores that belong to the AUTOSAR system shall be synchronized within the StartOS function before the scheduling is started. StartOs has just been called, but scheduling has already started. Core ID: '%d'  Task ID: '%d'\n", coreID, taskID);
        return;
    }

    if (firstRun == OS_TRUE)
    {
        /* initialzes os */
        Os_InitTaskContextSwitch();

        for(Tid=0;Tid<NUMBER_OF_TASKS;Tid++)
        {
            g_NumberOfTaskInvocations[Tid] = 0;
            g_NumberOfTaskOverruns[Tid] = 0;
        }

        /* Create here all tasks, which belong to no application */
        /* Note: In single core mode, all task are created here */
        for(Tid=0;Tid<NUMBER_OF_TASKS;Tid++)
        {
            if(g_TaskCb[Tid].ConnectApplicationId == OS_NOT_CONNECTED_TO_ANY_APPLICATION)
            {
                Os_TaskCreate(Tid, &g_TaskCb[Tid] );
            }
        }

        /* Start alarms for cyclic actions, which belong to no application */
        Os_Autostart(Mode, OS_NOT_CONNECTED_TO_ANY_APPLICATION);

        firstRun = OS_FALSE;
    }

#if NUMBER_OF_OS_APPLICATIONS > 0
    /*  starting the os applications  */
    {
        ApplicationType    idxApplication;
        Os_BoolType        allCoresAreStarted;
#if (NUMBER_OF_CORES > 0)
        CoreIdType         idxCore;
#endif /* (NUMBER_OF_CORES > 0) */

        /* Call the system-specific startup hook before the application-specific startup hook(s). */
        if(g_Kernel.IsStarted == OS_FALSE)
        {
            /* [SWS_Os_00236] If both a system-specific and one (or more) application specific startup hook(s) are configured, the Operating System module shall call the system-specific startup hook before the application-specific startup hook(s). */
            /*  Limitierung: In STARTUPHOOK() a function call of getCoreID() will return with OS_CORE_ID_MASTER */
#if (NUMBER_OF_CORES > 0)
            for(idxCore = 0; idxCore < NUMBER_OF_CORES; idxCore++)
            {
                /* STARTUPHOOK() is not called for a non autosar core */
                if (g_CoreCb[idxCore].IsNonAutosarCore == OS_FALSE)
                {
                    if( (g_CoreCb[idxCore].IsStarted == OS_TRUE) && (g_CoreCb[idxCore].GblHookIsCalled == OS_FALSE))
                    {
                        g_CoreCb[idxCore].GblHookIsCalled = OS_TRUE;
                        g_Kernel.ActCoreIdForHookFunction = idxCore;
                        STARTUPHOOK();
                        g_Kernel.ActCoreIdForHookFunction = OS_UNKNOWN_CORE_ID;
                    }
                }
            }
#else
            STARTUPHOOK();
#endif /* (NUMBER_OF_CORES > 0) */
        }

        for(idxApplication = 0; idxApplication < NUMBER_OF_OS_APPLICATIONS; idxApplication++)
        {
#if (NUMBER_OF_CORES > 0)
            if(g_CoreCb[g_ApplicationCb[idxApplication].CoreId].IsStarted == OS_TRUE)
#endif /* (NUMBER_OF_CORES > 0) */
            {
               if(g_ApplicationCb[idxApplication].IsStarted == OS_FALSE)
               {
                    for(Tid=0;Tid<NUMBER_OF_TASKS;Tid++)
                    {
                        if(g_ApplicationCb[idxApplication].ConnectedTasks[Tid] == OS_TRUE)
                        {
                            if(g_TaskCb[Tid].ConnectApplicationId == OS_NOT_CONNECTED_TO_ANY_APPLICATION)
                            {
                                // internal error
                            }
                            else
                            {
                                Os_TaskCreate(Tid, &g_TaskCb[Tid] );
                            }
                        }
                    }
                    /* Start alarms for cyclic actions, which belong to an Os application with an 'autostart' flag */
                    Os_Autostart(Mode, idxApplication);

                    /* [SWS_Os_00500] The Operating System module shall set the state of all OS-Applications after the call of StartOS() and before any StartupHook is called to APPLICATION_ACCESSIBE. */
                    g_ApplicationCb[idxApplication].IsStarted = OS_TRUE;
                    g_ApplicationCb[idxApplication].State = APPLICATION_ACCESSIBLE;

                    /* Call the StartUpHooks of the OS-Applications */
                    if(g_ApplicationCb[idxApplication].pStartupHook != OS_NULL)
                    {
                        /* [SWS_Os_00543] Availability of StartupHook_<App>() */
                        /* [SWS_Os_00060] If an application-specific startup hook is configured for an OS-Application <App>, the Operating System module shall call StartupHook_<App> on startup of the Operating System module. */
                        /* [SWS_Os_00582] The OS-Application-specific StartupHooks shall be called after the global StartupHook but only on the cores to which the OS-Application is bound. */
                        g_Kernel.CallLevel = STARTUPHOOK_LEVEL;
                        g_Kernel.ActCoreIdForHookFunction = g_ApplicationCb[idxApplication].CoreId;
                        g_Kernel.ActApplicationIdForHookFunction = idxApplication;
                        (*(g_ApplicationCb[idxApplication].pStartupHook))();
                        g_Kernel.ActCoreIdForHookFunction = OS_UNKNOWN_CORE_ID;
                        g_Kernel.ActApplicationIdForHookFunction = OS_UNKNOWN_APPLICATION_ID;
                        g_Kernel.CallLevel = OS_LEVEL;
                    }
               }
            }
        }

        /* If all autosar cores are started, set g_Kernel.IsStarted */
        allCoresAreStarted = OS_TRUE;
#if (NUMBER_OF_CORES > 0)
        for(idxCore = 0; idxCore < NUMBER_OF_CORES; idxCore++)
        {
            if( (g_CoreCb[idxCore].IsStarted == OS_FALSE) && (g_CoreCb[idxCore].IsNonAutosarCore == OS_FALSE) )
            {
                allCoresAreStarted = OS_FALSE;
            }
        }
#endif /* (NUMBER_OF_CORES > 0) */
        if (allCoresAreStarted == OS_TRUE)
        {
            if(g_Kernel.IsStarted == OS_FALSE)
            {
                g_Kernel.IsStarted = OS_TRUE;
                lastRun = OS_TRUE;
            }
        }
    }
#else
    /* No application specified */
    if(g_Kernel.IsStarted == OS_FALSE)
    {
        g_Kernel.IsStarted = OS_TRUE;

        STARTUPHOOK();

        lastRun = OS_TRUE;
    }
#endif /*  NUMBER_OF_OS_APPLICATIONS > 0 */

    if (lastRun == OS_TRUE)
    {
#if (NUMBER_OF_IOCS > 0)
        /* [SWS_Os_00611] If the IOC is configured, StartOS shall initialize the data structures of the IOC. */
        Os_InitDataStructsOfIoc();
#endif /*  NUMBER_OF_IOCS > 0 */

        Os_SimulationCoreOnOsStarted();

        /* Implemented differently, last call does not return */
        /* [SWS_Os_00424] The first call to StartOS() (for starting the Operating System) shall not return. */
        Os_FrmLockStartOs(); /* never return */
    }
}

void Os_SendWarningOsKernelIsNotRunning(void)
{
#if (NUMBER_OF_CORES > 0)
    /* multo core behavior  */
    CoreIdType idxCore;

    for(idxCore = 0; idxCore < NUMBER_OF_CORES; idxCore++)
    {
        if( (g_CoreCb[idxCore].IsStarted == OS_FALSE) && (g_CoreCb[idxCore].IsNonAutosarCore == OS_FALSE) )
        {
            /* This may occurs only during a VEOS simulation and if not all autosar cores have been started. */
            Os_SimulationCoreShowWarningMessage("The OS startup has finished and not all cores have been started. This may lead to a different simulation behavior.\n" );
            g_Kernel.IsStarted = OS_TRUE;
            return;
        }
    }
#endif /* (NUMBER_OF_CORES > 0) */
}

void Os_SendWarningAppModeIsNotDefined(void)
{
    /* [SWS_Os_00610] At least one core shall define an AppMode other than 'DONOTCARE'. */
    Os_SimulationCoreShowWarningMessage("The OS startup has finished and no AppMode other than DONOTCARE has been defined.\n" );
    g_Kernel.AppMode = OSDEFAULTAPPMODE;
}

void ShutdownOS(StatusType Error)
{
#if (NUMBER_OF_CORES > 0)
    Os_ShutdownOneCoreOnMultiCore(Error);
#else
    Os_ShutdownSingleCore(Error);
#endif /* (NUMBER_OF_CORES > 0) */
}

#if (NUMBER_OF_CORES > 0)
static void Os_ShutdownOneCoreOnMultiCore(StatusType Error)
{
    CoreIdType CoreID;

    if (OS_FALSE == g_Kernel.IsStarted)
    {
        return;
    }

    /* is this function is called from a task ? */
    if( g_Kernel.ApplicationLevel == TASK_LEVEL)
    {
        /* is this task belong to an os application ? */
        if (g_TaskCb[g_Kernel.ActTaskId].ConnectApplicationId != OS_NOT_CONNECTED_TO_ANY_APPLICATION)
        {
            /* is this os application not trusted ?  */
#if (NUMBER_OF_OS_APPLICATIONS > 0)
            if ( g_ApplicationCb[g_TaskCb[g_Kernel.ActTaskId].ConnectApplicationId].IsTrusted == OS_FALSE)
#endif
            {
                /* [SWS_Os_00054] The Operating System module shall ignore calls to ShutdownOS() from non-trusted OS-Applications. */
                return;
            }
        }
        else
        {
            /* [SWS_Os_00054] The Operating System module shall ignore calls to ShutdownOS() from non-trusted OS-Applications. */
            return;
        }
    }
    else
    {
        /* Limitation:  ShutdownOS() from Error- or Shutdownhook are not supported. */
        return;
    }

    CoreID = GetCoreID();

    /* The global ShutdownHook is called.  */
    if( g_CoreCb[CoreID].IsStarted == OS_TRUE)
    {
#ifdef SHUTDOWN_HOOK_ENABLED
        g_Kernel.ActCoreIdForHookFunction = CoreID;
        SHUTDOWNHOOK(Error); /* In SHUTDOWNHOOK() GetCoreID() will be returns the correct ID */
        g_Kernel.ActCoreIdForHookFunction = OS_UNKNOWN_CORE_ID;
#else
        /* to avoid the compiler warning C4100 */
        (void)(Error);
#endif
        /* [SWS_Os_00617] ShutdownOS shall shutdown the core on which it was called. */
        g_CoreCb[CoreID].IsStarted = OS_FALSE;
    }

#if (NUMBER_OF_OS_APPLICATIONS > 0)
    {
        ApplicationType idxApplication;
        for(idxApplication = 0; idxApplication < NUMBER_OF_OS_APPLICATIONS; idxApplication++)
        {
            if( (g_ApplicationCb[idxApplication].CoreId == CoreID) &&
                (g_TaskCb[g_Kernel.ActTaskId].ConnectApplicationId != idxApplication) )
            {
                /* release all spinlocks is handled in TerminateApplication() */
                /* [SWS_Os_00620] ShutdownOS shall release all spinlocks which are occupied by the calling core. */
                Os_TerminateApplicationInternal( idxApplication, NO_RESTART, Error);
            }
        }
        /*  call the self terminated application at the end */
        if ( g_TaskCb[g_Kernel.ActTaskId].ConnectApplicationId != OS_NOT_CONNECTED_TO_ANY_APPLICATION)
        {
            Os_TerminateApplicationInternal( g_TaskCb[g_Kernel.ActTaskId].ConnectApplicationId, NO_RESTART, Error);
        }
    }
#endif /* (NUMBER_OF_OS_APPLICATIONS > 0) */

}
#else

static void Os_ShutdownSingleCore(StatusType Error)
{
    /* -------- single core  ---------- */
    TaskType Tid, CalledFromTid;

    if (OS_FALSE == g_Kernel.IsStarted)
    {
        return;
    }

    g_Kernel.ShutdownOsCalled = OS_TRUE;

    g_ErrorInfo.ServiceID = OSServiceId_ShutdownOS;
    g_ErrorInfo.Service.SvShutdownOS.Error = Error;

    g_Kernel.IsStarted = OS_FALSE;

#if (NUMBER_OF_OS_APPLICATIONS > 0)
    {
        ApplicationType idxApplication;
        for(idxApplication = 0; idxApplication < NUMBER_OF_OS_APPLICATIONS; idxApplication++)
        {
            if( (g_ApplicationCb[idxApplication].pShutdownHook != OS_NULL) && (g_ApplicationCb[idxApplication].State == APPLICATION_ACCESSIBLE) )
            {
                g_Kernel.CallLevel = SHUTDOWNHOOK_LEVEL;
                g_Kernel.ActCoreIdForHookFunction = g_ApplicationCb[idxApplication].CoreId;
                g_Kernel.ActApplicationIdForHookFunction = idxApplication;
                (*(g_ApplicationCb[idxApplication].pShutdownHook))(Error);
                g_Kernel.ActCoreIdForHookFunction = OS_UNKNOWN_CORE_ID;
                g_Kernel.ActApplicationIdForHookFunction = OS_UNKNOWN_APPLICATION_ID;
                g_Kernel.CallLevel = OS_LEVEL;
            }
        }
    }
#endif /* (NUMBER_OF_OS_APPLICATIONS > 0) */

    SHUTDOWNHOOK(Error);

    Os_SimulationCoreOnOsShutdown();

    for(Tid = 0; Tid < NUMBER_OF_TASKS; Tid++)
    {
#ifdef OS_DBGMSG_ENABLED
        Os_SimulationCoreLogDbgMessage(__FUNCTION__, "Os_ShutdownOS(): Task with ID %d switches to state SUSPENDED.", Tid);
#endif
        /* If a running task shuts down the operating system */
        if ((g_Kernel.ActTaskId == Tid) && (RUNNING == g_TaskCb[Tid].State) && (g_TaskCb[g_Kernel.ActTaskId].PostCallRasterId > 0))
        {
            Os_SimulationCoreExpSvcPostTaskCall(g_TaskCb[g_Kernel.ActTaskId].PostCallRasterId, (g_Kernel.ActiveBasicRateTask == g_Kernel.ActTaskId) ? TRUE : FALSE);
        }

        Os_ChangeTaskState(Tid, SUSPENDED);
        Os_SetTaskActPrio(Tid, g_TaskCb[Tid].InitialPrio);
        g_TaskCb[Tid].NumberOfAct = 0;
        g_TaskCb[Tid].OsInterruptedTask = IDLE_LOOP;
        g_TaskCb[Tid].ResourceLevelCnt = 0;

        Os_TaskDelete(Tid);
    }

    CalledFromTid = g_Kernel.ActTaskId;
    g_Kernel.ActTaskId = IDLE_LOOP;
    g_Kernel.LastInterruptedTask = IDLE_LOOP;
    g_Kernel.ApplicationLevel = OS_LEVEL;
    g_Kernel.CallLevel = OS_LEVEL;
    g_Kernel.IsrDisableCnt = 0;

    g_ErrorInfo.Error = Error;

    /* deinitialzes os */
    Os_DeInitTaskContextSwitch();

    /* Note: This part is only execute if a user calls ShutdownOS(E_OK) in extendend tasks */
    if ( g_TaskCb[CalledFromTid].ExtendedTask )
    {
        /* To break the endless loop of an extended task, call here TerminateTask() and jump back into the scheduler */
        TerminateTask();
    }


    return;

}
#endif /* (NUMBER_OF_CORES > 0) */

#endif /* OS_C */

/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/
