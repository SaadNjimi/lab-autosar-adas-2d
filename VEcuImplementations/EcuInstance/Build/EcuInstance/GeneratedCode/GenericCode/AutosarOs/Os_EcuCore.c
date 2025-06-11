/**************************************************************************************************\
 *** Os_EcuCore.c
 ***
 *** OSEK API function definition for OS core handling.
 ***
 *** Copyright (c) 2018 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef OS_ECU_CORE_C
#define OS_ECU_CORE_C

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/
#include "Os_Native.h"
#include "Os_Kernel.h"
#include "Os_SimulationCore.h"
#include "Os_EcuCore.h"
#include "Os_Task.h"

/*------------------------------------------------------------------------------------------------*\
  FUNCTION DEFINITIONS
\*------------------------------------------------------------------------------------------------*/

/**************************************************************************************************\
 *** FUNCTION:
 ***     Os_GetCoreID
 ***
 *** DESCRIPTION:
 ***    The function returns a unique core identifier.
 ***    [SRS_Os_80001]
 ***
 *** PARAMETERS:
 ***     Type             Name     Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***
 ***
 *** RETURNS: CoreIdType
 ***
\**************************************************************************************************/
CoreIdType GetCoreID(void)
{
#if NUMBER_OF_OS_APPLICATIONS > 0
    ApplicationType idxApplication;

    /* Limitation:  GetCoreID() in ISRs and AlarmCallbacks are not supported! */

    /* [SWS_Os_00625] The AUTOSAR Operating System API function GetCoreID shall be callable before StartOS
       Limitation: ==> GetCoreID is callable after StartOS */
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
                    /* [SWS_Os_00675] The function GetCoreID shall return the unique logical CoreID of the core on which the function is called. The mapping of physical cores to logical CoreIDs is implementation specific. */
                    return g_ApplicationCb[idxApplication].CoreId;
                }
            }
        break;
        case OS_LEVEL:
            return OS_CORE_ID_MASTER;
        break;
        case ISR_LEVEL:
            return OS_CORE_ID_MASTER;
        break;
        case STARTUPHOOK_LEVEL:
        case SHUTDOWNHOOK_LEVEL:
            /* Note: A GetCoreID() in STARTUPHOOK or STARTUPHOOK will returns the correct core ID */
            if(g_Kernel.ActCoreIdForHookFunction != OS_UNKNOWN_CORE_ID)
            {
                /* [SWS_Os_00582] The OS-Application-specific StartupHooks shall be called after the global StartupHook but only on the cores to which the OS-Application is bound. */
                /* [SWS_Os_00586] During the shutdown, the OS-Application specific ShutdownHook shall be called on the core on which the corresponding OS-Application is bound. */
                return g_Kernel.ActCoreIdForHookFunction;
            }
            else
            {
                return OS_CORE_ID_MASTER;
            }
        break;
        default:
        break;
    }
    return OS_CORE_ID_MASTER;

#else
    return OS_CORE_ID_MASTER;
#endif /* NUMBER_OF_OS_APPLICATIONS > 0 */
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     Os_GetNumberOfActivatedCores
 ***
 *** DESCRIPTION:
 ***    The function returns the number of cores activated by the StartCore function.
 ***    [SWS_Os_00672]
 ***
 *** PARAMETERS:
 ***     Type             Name     Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***
 ***
 *** RETURNS:
 ***     uint32   Number of cores activated by the StartCore function
 ***
\**************************************************************************************************/
uint32 GetNumberOfActivatedCores(void)
{
#if (NUMBER_OF_CORES > 0)
    CoreIdType idxCore;

    uint32 numberOfActivatedCores = 0;
    /* [SWS_Os_00673] The return value of GetNumberOfActivatedCores shall be less or equal to the configured value of OsNumberOfCores */
    for(idxCore = 0; idxCore < NUMBER_OF_CORES; idxCore++)
    {
        if( (g_CoreCb[idxCore].IsStarted == OS_TRUE) && (g_CoreCb[idxCore].IsNonAutosarCore == OS_FALSE) )
        {
            numberOfActivatedCores++;
        }
    }
    /* [SWS_Os_00626] An implementation shall offer a function GetNumberOfActivatedCores that returns the number of cores running the AUTOSAR OS. */
    return numberOfActivatedCores;
#else
    return 0;
#endif /* (NUMBER_OF_CORES > 0) */
}

/**************************************************************************************************\
 *** FUNCTION:
 ***    StartCore
 ***
 *** DESCRIPTION:
 ***    It is not supported to call this function after StartOS().
 ***    The function starts the core specified by the parameter CoreID.
 ***    The OUT parameter allows the caller to check whether the operation was successful or not.
 ***    If a core is started by means of this function StartOS shall be called on the core.
 ***    [SWS_Os_00676]
 ***
 ***
 *** PARAMETERS:
 ***     Type             Name     Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     CoreIdType       CoreID     The ID of the core
 ***     StatusType*      Status     The status []
 ***                                    E_OK: No Error
 ***                                    E_OS_ID: Core ID is invalid.
 ***                                    E_OS_ACCESS: The function was called after starting the OS.
 ***                                    E_OS_STATE: The Core is already activated
 ***
\**************************************************************************************************/
void StartCore( CoreIdType CoreID, StatusType* Status )
{
#if (NUMBER_OF_CORES > 0)
#else
    /* to avoid the compiler warning C4100 */
    (void)(CoreID);
#endif /* (NUMBER_OF_CORES > 0) */

#ifdef OS_DBGMSG_ENABLED
    Os_SimulationCoreLogDbgMessage(__FUNCTION__, "StartCore(): Start of function. Core is %d.", CoreID);
#endif

#if (NUMBER_OF_CORES > 0)
    /* [SWS_Os_00681] There is no call to the ErrorHook() if an error occurs during StartCore(); */
    if(CoreID >= NUMBER_OF_CORES)
    {
        *Status = E_OS_ID;
        return;
    }

    /* [SWS_Os_00606] The AUTOSAR specification does not support the activation of AUTOSAR cores after calling StartOS on that core. If StartCore is called after StartOS it shall return with E_OS_ACCESS in extended status. */
    /* [SWS_Os_00678] Calls to the StartCore function after StartOS() shall return with E_OS_ACCESS and the core shall not be started. */
    if(g_Kernel.IsStarted == OS_TRUE)
    {
        /* The function StartCore() was called after starting the OS */
        *Status = E_OS_ACCESS;
        return;
    }

    /* [SWS_Os_00679] If the parameter CoreIDs refers to a core that was already started by the function StartCore the related core is ignored and E_OS_STATE shall be returned. */
    if( g_CoreCb[CoreID].IsStarted == OS_TRUE)
    {
        /* The Core is already activated */
        *Status = E_OS_STATE;
        return;
    }
    else
    {
        /* [SWS_Os_00677] The function StartCore shall start one core that shall run under the control of the AUTOSAR OS. */
        g_CoreCb[CoreID].IsStarted = OS_TRUE;
        *Status = E_OK;
    }
#else
    *Status = E_OS_ID;
#endif /* (NUMBER_OF_CORES > 0) */
}

/**************************************************************************************************\
 *** FUNCTION:
 ***    StartNonAutosarCore
 ***
 *** DESCRIPTION:
 ***    It is allowed to call this function after StartOS().
 ***    The function starts the core specified by the parameter CoreID.
 ***    The OUT parameter allows the caller to check whether the operation was successful or not.
 ***    It is not allowed to call StartOS on cores activated by StartNonAutosarCore.
 ***    [SWS_Os_00682]
 ***
 ***
 *** PARAMETERS:
 ***     Type             Name     Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     Os_CoreIdType    CoreID     The ID of the core
 ***     StatusType*      Status     The status []
 ***                                    E_OK: No Error
 ***                                    E_OS_ID: Core ID is invalid.
 ***                                    E_OS_STATE: The Core is already activated
 ***
\**************************************************************************************************/
void StartNonAutosarCore( CoreIdType CoreID, StatusType* Status )
{
#if (NUMBER_OF_CORES > 0)
#else
    /* to avoid the compiler warning C4100 */
    (void)(CoreID);
#endif /* (NUMBER_OF_CORES > 0) */

#ifdef OS_DBGMSG_ENABLED
    Os_SimulationCoreLogDbgMessage(__FUNCTION__, "StartNonAutosarCore(): Start of function. Core is %d.", CoreID);
#endif

#if (NUMBER_OF_CORES > 0)
    /* [SWS_Os_00685] If the parameter CoreID refers to an unknown core the function StartNonAutosarCore has no effect and sets Status to E_OS_ID. */
    if(CoreID >= NUMBER_OF_CORES)
    {
        *Status = E_OS_ID;
        return;
    }

    /* [SWS_Os_00684] If the parameter CoreID refers to a core that was already started by the function StartNonAutosarCore has no effect and sets Status to E_OS_STATE. */
    if( g_CoreCb[CoreID].IsStarted == OS_TRUE)
    {
        *Status = E_OS_STATE;
        return;
    }
    else
    {
        /* [SWS_Os_00683] The function StartNonAutosarCore shall start a core that is not controlled by the AUTOSAR OS. */
        g_CoreCb[CoreID].IsStarted = OS_TRUE;
        *Status = E_OK;
    }
#else
    *Status = E_OS_ID;
#endif /* (NUMBER_OF_CORES > 0) */
}

/**************************************************************************************************\
 *** FUNCTION:
 ***    ShutdownAllCores
 ***
 *** DESCRIPTION:
 ***    After this service the OS on all AUTOSAR cores is shut down.
 ***    Allowed at TASK level and ISR level and also internally by the OS.
 ***    The function will never return. The function will force other cores into a shutdown.
 ***    [SRS_Os_80007]
 ***
 ***
 *** PARAMETERS:
 ***     Type             Name     Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     StatusType      Status     <Error> needs to be a valid error code supported by the AUTOSAR OS.
 ***
\**************************************************************************************************/
void ShutdownAllCores( StatusType Error )
{
#if (NUMBER_OF_CORES > 0)
    Os_ApplicationLevelType IsCalledFrom = g_Kernel.CallLevel;
#endif /* (NUMBER_OF_CORES > 0) */

#ifdef OS_DBGMSG_ENABLED
    Os_SimulationCoreLogDbgMessage(__FUNCTION__, "ShutdownAllCores(): Start of function.");
#endif

    g_ErrorInfo.ServiceID = OSServiceId_ShutdownAllCores;
    g_ErrorInfo.Service.SvShutdownOS.Error = Error;

#if (NUMBER_OF_CORES > 0)

    if (OS_FALSE == g_Kernel.IsStarted)
    {
        return;
    }

    /* return, if kernal is already in shutdown mode */
    if (OS_TRUE == g_Kernel.ShutdownOsCalled)
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
#endif /* (NUMBER_OF_OS_APPLICATIONS > 0) */
            {
                /* [SWS_Os_00716] If ShutdownAllCores is called from non trusted code the call shall be ignored */
                return;
            }
        }
    }

    g_Kernel.ShutdownOsCalled = OS_TRUE;

#if (NUMBER_OF_OS_APPLICATIONS > 0)
    {
        ApplicationType idxApplication;
        for(idxApplication = 0; idxApplication < NUMBER_OF_OS_APPLICATIONS; idxApplication++)
        {
            /* [SWS_Os_00545] Availability of ShutdownHook_<App>() */
            /* [SWS_Os_00112] If an application-specific shutdown hook is configured for an OS-Application <App>, the Operating System module shall call ShutdownHook_<App> on shutdown of the OS. */
            /* [SWS_Os_00237] If both a system-specific and one (or more) application specific shutdown hook(s) are configured, the Operating System module shall call the system-specific shutdown hook after the application-specific shutdown hook(s). */
            /* [SWS_Os_00586] During the shutdown, the OS-Application specific ShutdownHook shall be called on the core on which the corresponding OS-Application is bound. */
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

    {
        /* [SWS_Os_00588] The global ShutdownHook shall be called on all core.  */
        CoreIdType idxCore;
        for(idxCore = 0; idxCore < NUMBER_OF_CORES; idxCore++)
        {
            /* SHUTDOWNHOOK() is not called for a non autosar core */
            if (g_CoreCb[idxCore].IsNonAutosarCore == OS_FALSE)
            {
                if( g_CoreCb[idxCore].IsStarted == OS_TRUE)
                {
                    g_Kernel.ActCoreIdForHookFunction = idxCore;
                    SHUTDOWNHOOK(Error);
                    g_CoreCb[idxCore].IsStarted = OS_FALSE;
                    g_Kernel.ActCoreIdForHookFunction = OS_UNKNOWN_CORE_ID;
                }
            }
        }
    }

    Os_SimulationCoreOnOsShutdown();

    {
        TaskType  idxTask;
        for(idxTask = 0; idxTask < NUMBER_OF_TASKS; idxTask++)
        {
#ifdef OS_DBGMSG_ENABLED
            Os_SimulationCoreLogDbgMessage(__FUNCTION__, "ShutdownAllCores(): Task with ID %d switches to state SUSPENDED.", idxTask);
#endif
            /* If a running task shuts down the operating system */
            if ((g_Kernel.ActTaskId == idxTask) && (RUNNING == g_TaskCb[idxTask].State) && (g_TaskCb[g_Kernel.ActTaskId].PostCallRasterId > 0))
            {
                Os_SimulationCoreExpSvcPostTaskCall(g_TaskCb[g_Kernel.ActTaskId].PostCallRasterId, (g_Kernel.ActiveBasicRateTask == g_Kernel.ActTaskId) ? TRUE : FALSE);
            }

            Os_ChangeTaskState(idxTask, SUSPENDED);
            Os_SetTaskActPrio(idxTask, g_TaskCb[idxTask].InitialPrio);
            g_TaskCb[idxTask].NumberOfAct = 0;
            g_TaskCb[idxTask].OsInterruptedTask = IDLE_LOOP;
            g_TaskCb[idxTask].ResourceLevelCnt = 0;

            Os_TaskDelete(idxTask);
        }
    }

    g_ErrorInfo.Error = Error;

    /* deinitialzes os */
    Os_DeInitTaskContextSwitch();

    if ( (IsCalledFrom == TASK_LEVEL) || (IsCalledFrom == ERRORHOOK_LEVEL)  )
    {
        /* ShutdownAllCores() is called from an OS task */
        /*  ==> no function return */
        g_Kernel.CallLevel = TASK_LEVEL;
        g_TaskCb[g_Kernel.ActTaskId].NumberOfAct = 1;

        TerminateTask();
    }
    else
    {
        /* ShutdownAllCores() is called from VEcuAppFrm_HandlePowerOffEvent() */
        g_Kernel.ActTaskId = IDLE_LOOP;
        g_Kernel.LastInterruptedTask = IDLE_LOOP;
        g_Kernel.ApplicationLevel = OS_LEVEL;
        g_Kernel.CallLevel = OS_LEVEL;
        g_Kernel.IsrDisableCnt = 0;
    }

#else
    /* to avoid the compiler warning C4100 */
    (void)(Error);

    ERRORHOOK(E_OS_NOFUNC);
#endif /* (NUMBER_OF_CORES > 0) */
}

/**************************************************************************************************\
 *** FUNCTION:
 ***    ControlIdle
 ***
 *** DESCRIPTION:
 ***    This API allows the caller to select the idle mode action which is performed
 ***    during idle time of the OS (e.g. if no Task/ISR is active). It can be used
 ***    to implement energy savings. The real idle modes are hardware dependent and
 ***    not standardized. The default idle mode on each core is IDLE_NO_HALT.
 ***    [SWS_Os_00769]
 ***
 ***
 *** PARAMETERS:
 ***     Type               Name       Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     CoreIdType      CoreID     Selects the core which idle mode is set.
 ***     IdleModeType    IdleMode   The mode which shall be performed during idle time
 ***
 *** RETURNS: StatusType
 ***             E_OK: No errors
 ***
\**************************************************************************************************/
StatusType ControlIdle( CoreIdType CoreID, IdleModeType IdleMode )
{
    /* to avoid the compiler warning C4100 */
    (void)(IdleMode);

#if (NUMBER_OF_CORES > 0)
    /* [SWS_Os_00771] The function ControlIdle shall return E_OS_ID if the parameter CoreID or IdleMode is invalid (e.g. refered core does not exist; idlemode is not known).
                      In single core systems the check of CoreID shall be omitted. */
    if( (CoreID >= NUMBER_OF_CORES) || (IdleMode != IDLE_NO_HALT) )
    {
        ERRORHOOK(E_OS_ID);
        return E_OS_ID;
    }
#else
    /* to avoid the compiler warning C4100 */
    (void)(CoreID);
#endif /* (NUMBER_OF_CORES > 0) */

    return E_OK;
}

#endif /* OS_ECU_CORE_C */

/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/
