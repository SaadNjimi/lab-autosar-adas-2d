/**************************************************************************************************\
 *** Os_SimulationCore.c
 ***
 *** The veos os simulation core interface.
 ***
 *** Copyright (c) 2017 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef OS_SIMULATIONCORE_C
#define OS_SIMULATIONCORE_C

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/

/* Include simulation frame dependencies. */
#include "VEOS.h"
#include "VEcuAppFrm_Os.h"
#include "Os_SimulationCore.h"
#include "stdarg.h"

#include "Os_SimulationCore.h"
#include "Os_Native.h"
#include "Os_Kernel.h"

/*------------------------------------------------------------------------------------------------*\
  FORWARD DECLARATIONS
\*------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

/*------------------------------------------------------------------------------------------------*\
  VARIABLE DEFINITIONS
\*------------------------------------------------------------------------------------------------*/
static Os_ApplicationLevelType tmpApplicationLevel;
static Os_ApplicationLevelType tmpCallLevel;

/*------------------------------------------------------------------------------------------------*\
  FUNCTION DEFINITIONS
\*------------------------------------------------------------------------------------------------*/

/**************************************************************************************************\
 *** FUNCTION:
 ***      Os_SetInterruptLevel
 ***
 *** DESCRIPTION: This function set ApplicationLevel and CallLevel to 'ISR_LEVEL'
 ***
 *** PARAMETERS:
 ***     Type             Name     Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***
 ***
 *** RETURNS:
 ***
\**************************************************************************************************/
void Os_SetInterruptLevel(void)
{
    tmpApplicationLevel = g_Kernel.ApplicationLevel;
    tmpCallLevel = g_Kernel.CallLevel;
    g_Kernel.ApplicationLevel = ISR_LEVEL;
    g_Kernel.CallLevel        = ISR_LEVEL;
}

/**************************************************************************************************\
 *** FUNCTION:
 ***      Os_ResetInterruptLevel
 ***
 *** DESCRIPTION: This function restore ApplicationLevel and CallLevel
 ***
 *** PARAMETERS:
 ***     Type             Name     Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***
 ***
 *** RETURNS:
 ***
\**************************************************************************************************/
void Os_ResetInterruptLevel(void)
{
    g_Kernel.ApplicationLevel = tmpApplicationLevel;
    g_Kernel.CallLevel        = tmpCallLevel;
}

/**************************************************************************************************\
 *** FUNCTION:
 ***      Os_ScheduleInterruptActivatedTask
 ***
 *** DESCRIPTION: If the callback function activate tasks, start scheduling here
 ***
 *** PARAMETERS:
 ***     Type             Name     Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***
 ***
 *** RETURNS:
 ***
\**************************************************************************************************/
void Os_ScheduleInterruptActivatedTask(void)
{
    /* Set basic rate task to idle loop. */
    g_Kernel.ActiveBasicRateTask = IDLE_LOOP;
    Os_TaskStart(IDLE_LOOP);
    /* Set basic rate task to idle loop. */
    g_Kernel.ActiveBasicRateTask = IDLE_LOOP;
}

/**************************************************************************************************\
 *** FUNCTION:
 ***      Os_SimulationCoreOnOsStarted
 ***
 *** DESCRIPTION: This function is used to signal the start of the OS.
 ***
 *** PARAMETERS:
 ***     Type             Name     Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***
 ***
 *** RETURNS:
 ***
\**************************************************************************************************/
void Os_SimulationCoreOnOsStarted(void)
{
    VEcuAppFrm_OnOsStarted();
}

/**************************************************************************************************\
 *** FUNCTION:
 ***      Os_SimulationCoreOnOsShutdown
 ***
 *** DESCRIPTION: This function is used to signal the shutdown of the OS.
 ***
 *** PARAMETERS:
 ***     Type             Name     Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***
 ***
 *** RETURNS:
 ***
\**************************************************************************************************/
void Os_SimulationCoreOnOsShutdown(void)
{
    VEcuAppFrm_OnOsShutdown();
}


/**************************************************************************************************\
 *** FUNCTION:
 ***      Os_SimulationCoreOnOsWakeup
 ***
 *** DESCRIPTION: This function is used to signal the wakeup of the OS.
 ***
 *** PARAMETERS:
 ***     Type             Name     Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***
 ***
 *** RETURNS:
 ***
\**************************************************************************************************/
void Os_SimulationCoreOnOsWakeup(void)
{
    VEcuAppFrm_OnOsWakeup();
}


/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_ExpSvcPreTaskCall
 ***
 *** DESCRIPTION:
 ***     Function called when the operating system starts a task and must trigger rasters.
 ***
 *** PARAMETERS:
 ***     Type            Name                 Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     const uint16    RasterId             The experimentation service raster to invoke.
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
void Os_SimulationCoreExpSvcPreTaskCall(const uint16 RasterId)
{
    VEcuAppFrm_ExpSvcPreTaskCall(RasterId);
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     Os_SimlationExpSvcPostTaskCall
 ***
 *** DESCRIPTION:
 ***     Function called when the operating system terminates a task and must trigger rasters.
 ***
 *** PARAMETERS:
 ***     Type            Name                 Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     const uint16    RasterId             The experimentation service raster to invoke.
 ***     const boolean   ActiveBasicRateTask  A flag to indicate whether the current task is the active
 ***                                          basic rate task.
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
void Os_SimulationCoreExpSvcPostTaskCall(const uint16 RasterId, const boolean ActiveBasicRateTask)
{
    VEcuAppFrm_ExpSvcPostTaskCall(RasterId, ActiveBasicRateTask);
}

/************************************************************************\
 *  Function to determine the message severity.                          *
\************************************************************************/
VEOS_MsgApi_SeverityType Os_GetMessageLevel(StatusType Error, boolean* const fatal)
{
    switch(Error)
    {
    case E_OS_ACCESS:
    case E_OS_CALLEVEL:
    case E_OS_ID:
    case E_OS_VALUE:
    case E_OS_RESOURCE:
    case E_OS_NESTING_DEADLOCK:
    case E_OS_INTERFERENCE_DEADLOCK:
        return VEOS_MsgApi_SeverityType_Error;
        /*break;*/
    case E_OS_LIMIT:
    case E_OS_NOFUNC:
    case E_OS_STATE:
    case E_OS_CORE:
        return VEOS_MsgApi_SeverityType_Warning;
        /*break;*/
    default:
        *fatal = VEOS_TRUE;
        return VEOS_MsgApi_SeverityType_Error;
        /*break;*/
    }
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     Os_SimulationCoreErrorHook
 ***
 *** DESCRIPTION:
 ***     Transfers the specified message to the simulator backplane where it processed further on.
 ***
 *** PARAMETERS:
 ***     Type                            Name      Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     StatusType                   error              number of status
 ***
 ***     Os_ServiceServiceDescType    serviceIdentifier  name of service for the error message.
 ***
 ***     Os_ErrorLevelDescType        errorType          name of the error
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any error
 ***                            occurs, an error code will be the result. See VEOS_Types.h for
 ***                            details.
\**************************************************************************************************/
void Os_SimulationCoreErrorHook(StatusType error, Os_ServiceServiceDescType serviceIdentifier, Os_ErrorLevelDescType errorType)
{
    boolean fatal = VEOS_FALSE;
    VEOS_MsgApi_SeverityType msgLevel = Os_GetMessageLevel(error, &fatal);

    if (((void *)0) != errorType)
    {
        VEOS_MsgApi_ShowMessageFormat(msgLevel, VEOS_TRUE, "AUTOSAR OS error of type %s occurred in OS service function %s.", errorType, serviceIdentifier);
        if (fatal)
        {
            VEOS_SimApi_TerminateSimulationFatal();
        }
        if (msgLevel == VEOS_MsgApi_SeverityType_Error)
        {
            (void)VEOS_SimApi_TerminateSimulation(VEOS_SimApi_TerminationKindType_StopSimulationAndFail);
        }
    }
    else
    {
        VEOS_MsgApi_ShowMessageFormat(VEOS_MsgApi_SeverityType_Error, VEOS_TRUE, "Internal fatal error occurred in OS module in function with service ID %s.", serviceIdentifier);
        VEOS_SimApi_TerminateSimulationFatal();
    }
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     Os_SimulationCoreLogDbgMessage
 ***
 *** DESCRIPTION:
 ***     Transfers logging debug messages.
 ***
 *** PARAMETERS:
 ***     Type                            Name      Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     const VEOS_char *               pFormat   Incoming argument: The formatted message.
 ***
 *** RETURNS:
 ***
\**************************************************************************************************/
#ifdef OS_DBGMSG_ENABLED
void Os_SimulationCoreLogDbgMessage(const VEOS_char* FunctionName, const char * pFormat, ...)
{
    va_list args;
    va_start(args, pFormat);
    (void)VEOS_MsgApi_LogMessageVarArgs("AUTOSAR OS", FunctionName, pFormat, args);
    va_end(args);
}
#endif /* OS_DBGMSG_ENABLED */

/**************************************************************************************************\
 *** FUNCTION:
 ***     Os_SimulationCoreShowErrorMessage
 ***
 *** DESCRIPTION:
 ***     Transfers the specified message to the simulator backplane where it processed further on.
 ***
 *** PARAMETERS:
 ***     Type                            Name      Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     const VEOS_char *               pFormat   Incoming argument: The formatted message.
 ***
 *** RETURNS:
 ***
\**************************************************************************************************/
void Os_SimulationCoreShowErrorMessage(const char * pFormat,  ...)
{
    va_list args;
    va_start(args, pFormat);
    (void)VEOS_MsgApi_ShowMessageVarArgs(VEOS_MsgApi_SeverityType_Error, VEOS_TRUE, pFormat, args);
    (void)VEOS_SimApi_TerminateSimulation(VEOS_SimApi_TerminationKindType_StopSimulationAndFail);
    va_end(args);
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     Os_SimulationCoreShowFatalErrorMessage
 ***
 *** DESCRIPTION:
 ***     Transfers the specified message to the simulator backplane where it processed further on.
 ***
 *** PARAMETERS:
 ***     Type                            Name      Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     const VEOS_char *               pFormat   Incoming argument: The formatted message.
 ***
 *** RETURNS:
 ***
\**************************************************************************************************/
void Os_SimulationCoreShowFatalErrorMessage(const char * pFormat,  ...)
{
    va_list args;
    va_start(args, pFormat);
    (void)VEOS_MsgApi_ShowMessageVarArgs(VEOS_MsgApi_SeverityType_Error, VEOS_TRUE, pFormat, args);
    va_end(args);
    VEOS_SimApi_TerminateSimulationFatal();
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     Os_SimulationCoreShowWarningMessage
 ***
 *** DESCRIPTION:
 ***     Transfers the specified message to the simulator backplane where it processed further on.
 ***
 *** PARAMETERS:
 ***     Type                            Name      Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     const VEOS_char *               pFormat   Incoming argument: The formatted message.
 ***
 *** RETURNS:
 ***
\**************************************************************************************************/
void Os_SimulationCoreShowWarningMessage(const char * pFormat,  ...)
{
    va_list args;
    va_start(args, pFormat);
    (void)VEOS_MsgApi_ShowMessageVarArgs(VEOS_MsgApi_SeverityType_Warning, VEOS_TRUE, pFormat, args);
    va_end(args);
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     Os_SimlationTerminateSimulation
 ***
 *** DESCRIPTION:
 ***     Access point for terminating the simulation from within the model.
 ***     Note: This part is only execute if a user task is called ShutdownOS(E_OK)
 ***
 *** PARAMETERS:
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
void Os_SimlationTerminateSimulation(void)
{
    /* Sends the terminate simulation request. */
    VEcuAppFrm_TerminateSimulation();
}





/**************************************************************************************************\
 *** FUNCTION:
 ***     Os_DisableAllLowerPrioTasks
 ***
 *** DESCRIPTION:
 ***     The function Os_DisableAllLowerPrioTasks is called during a sleep scenario
 ***     and save the sleeping task Id.
 ***
 *** PARAMETERS:
 ***     Type           Name               Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
void Os_DisableAllLowerPrioTasks(void)
{
    g_Kernel.IsSleeping = OS_VECU_IS_SLEEPING;
    g_Kernel.SleepingTaskId = g_Kernel.ActTaskId;
}


/**************************************************************************************************\
 *** FUNCTION:
 ***     Os_WaitForTaskResumption
 ***
 *** DESCRIPTION:
 ***     The function Os_WaitForTaskResumption is called during a sleep scenario
 ***      - Lower prio task are still in task state "READY", but are not running
 ***      - Jump to IDLE_LOOP and finish simulation step
 ***
 *** PARAMETERS:
 ***     Type           Name               Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
void Os_WaitForTaskResumption(void)
{
    Os_ScheduleInternal(OS_TRUE, OS_FALSE);
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     Os_ResumeResumableTask
 ***
 *** DESCRIPTION:
 ***     The Os_ResumeResumableTask function is called during a sleep scenario to wake up the V-ECU.
 ***
 *** PARAMETERS:
 ***     Type           Name               Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
void Os_ResumeResumableTask(void)
{
    g_Kernel.IsSleeping = OS_VECU_IS_NOT_SLEEPING;
}

#endif /* OS_SIMULATION_CORE_C */

/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/
