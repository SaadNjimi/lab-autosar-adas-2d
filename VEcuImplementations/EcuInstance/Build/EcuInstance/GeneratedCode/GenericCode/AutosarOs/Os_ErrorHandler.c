/**************************************************************************************************\
 *** Os_ErrorHandler.c
 ***
 *** OSEK error handling
 ***
 *** Copyright (c) 2009 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef OS_ERRORHANDLER_C
#define OS_ERRORHANDLER_C

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/

#include "Os_Kernel.h"
#include "Os_SimulationCore.h"
#include "Os_Application.h"

#include "stdarg.h"

/*------------------------------------------------------------------------------------------------*\
  DEFINES
\*------------------------------------------------------------------------------------------------*/
    
#define OS_ERRORHANDLER_MAX_SERVICE_ID              (OSServiceId_CheckObjectOwnerShip) 
#define OS_ERRORHANDLER_MAX_ERROR_LEVEL             (E_OS_SYS_INTERNAL + 1)  /* last is E_OS_PARAM_POINTER=0x17u + 1 */ 


/*------------------------------------------------------------------------------------------------*\
  TYPEDEFS
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
  VARIABLE DEFINITIONS
\*------------------------------------------------------------------------------------------------*/

Os_SErrorInfoType g_ErrorInfo;
Os_ErrorLevelDescType ga_OsErrorLevelIdentifiers[OS_ERRORHANDLER_MAX_ERROR_LEVEL + 5]; /* + 5=reserve */
Os_ServiceServiceDescType ga_OsServiceIdentifiers[OS_ERRORHANDLER_MAX_SERVICE_ID + 5]; /* + 5=reserve */


/*------------------------------------------------------------------------------------------------*\
  FUNCTION DEFINITIONS
\*------------------------------------------------------------------------------------------------*/

/************************************************************************\
 *  Function to determine name of the service for the error message.    *
\************************************************************************/
Os_ServiceServiceDescType Os_GetServiceIdentifier(Os_ServiceIdType serviceId)
{

    ga_OsServiceIdentifiers[OSServiceId_Invalid] = (const char*)"OSServiceId_Invalid"; /* No service specified */
    ga_OsServiceIdentifiers[OSServiceId_ActivateTask] = (const char*)"ActivateTask"; /* OSServiceId_ActivateTask */
    ga_OsServiceIdentifiers[OSServiceId_TerminateTask] = (const char*)"TerminateTask"; /* OSServiceId_TerminateTask */
    ga_OsServiceIdentifiers[OSServiceId_ChainTask] = (const char*)"ChainTask"; /* OSServiceId_ChainTask */
    ga_OsServiceIdentifiers[OSServiceId_Schedule] = (const char*)"Schedule"; /* OSServiceId_Schedule */
    ga_OsServiceIdentifiers[OSServiceId_GetTaskID] = (const char*)"GetTaskID"; /* OSServiceId_GetTaskID */
    ga_OsServiceIdentifiers[OSServiceId_GetTaskState] = (const char*)"GetTaskState"; /* OSServiceId_GetTaskState */
    ga_OsServiceIdentifiers[OSServiceId_EnableAllInterrupts] = (const char*)"EnableAllInterrupts"; /* OSServiceId_EnableAllInterrupts */
    ga_OsServiceIdentifiers[OSServiceId_DisableAllInterrupts] = (const char*)"DisableAllInterrupts"; /* OSServiceId_DisableAllInterrupts */
    ga_OsServiceIdentifiers[OSServiceId_ResumeAllInterrupts] = (const char*)"ResumeAllInterrupts"; /* OSServiceId_ResumeAllInterrupts */
    ga_OsServiceIdentifiers[OSServiceId_SuspendAllInterrupts] = (const char*)"SuspendAllInterrupts"; /* OSServiceId_SuspendAllInterrupts */
    ga_OsServiceIdentifiers[OSServiceId_ResumeOSInterrupts] = (const char*)"ResumeOSInterrupts"; /* OSServiceId_ResumeOSInterrupts */
    ga_OsServiceIdentifiers[OSServiceId_SuspendOSInterrupts] = (const char*)"SuspendOSInterrupts"; /* OSServiceId_SuspendOSInterrupts */
    ga_OsServiceIdentifiers[OSServiceId_GetResource] = (const char*)"GetResource"; /* OSServiceId_GetResource */
    ga_OsServiceIdentifiers[OSServiceId_ReleaseResource] = (const char*)"ReleaseResource"; /* OSServiceId_ReleaseResource */
    ga_OsServiceIdentifiers[OSServiceId_SetEvent] = (const char*)"SetEvent"; /* OSServiceId_SetEvent */
    ga_OsServiceIdentifiers[OSServiceId_ClearEvent] = (const char*)"ClearEvent"; /* OSServiceId_ClearEvent */
    ga_OsServiceIdentifiers[OSServiceId_GetEvent] = (const char*)"GetEvent"; /* OSServiceId_GetEvent */
    ga_OsServiceIdentifiers[OSServiceId_WaitEvent] = (const char*)"WaitEvent"; /* OSServiceId_WaitEvent */
    ga_OsServiceIdentifiers[OSServiceId_GetAlarmBase] = (const char*)"GetAlarmBase"; /* OSServiceId_GetAlarmBase */
    ga_OsServiceIdentifiers[OSServiceId_GetAlarm] = (const char*)"GetAlarm"; /* OSServiceId_GetAlarm */
    ga_OsServiceIdentifiers[OSServiceId_SetRelAlarm] = (const char*)"SetRelAlarm"; /* OSServiceId_SetRelAlarm */
    ga_OsServiceIdentifiers[OSServiceId_SetAbsAlarm] = (const char*)"SetAbsAlarm"; /* OSServiceId_SetAbsAlarm */
    ga_OsServiceIdentifiers[OSServiceId_CancelAlarm] = (const char*)"CancelAlarm"; /* OSServiceId_CancelAlarm */
    ga_OsServiceIdentifiers[OSServiceId_GetActiveApplicationMode] = (const char*)"GetActiveApplicationMode"; /* OSServiceId_GetActiveApplicationMode */
    ga_OsServiceIdentifiers[OSServiceId_StartOS] = (const char*)"StartOS"; /* OSServiceId_StartOS */
    ga_OsServiceIdentifiers[OSServiceId_ShutdownOS] = (const char*)"ShutdownOS"; /* OSServiceId_ShutdownOS */
    ga_OsServiceIdentifiers[OSServiceId_StartScheduleTableRel] = (const char*)"StartScheduleTableRel"; /* OSServiceId_StartScheduleTableRel */
    ga_OsServiceIdentifiers[OSServiceId_StartScheduleTableAbs] = (const char*)"StartScheduleTableAbs"; /* OSServiceId_StartScheduleTableAbs */
    ga_OsServiceIdentifiers[OSServiceId_StopScheduleTable] = (const char*)"StopScheduleTable"; /* OSServiceId_StopScheduleTable */
    ga_OsServiceIdentifiers[OSServiceId_NextScheduleTable] = (const char*)"NextScheduleTable"; /* OSServiceId_NextScheduleTable */
    ga_OsServiceIdentifiers[OSServiceId_StartScheduleTableSynchron] = (const char*)"StartScheduleTableSynchron"; /* OSServiceId_StartScheduleTableSynchron */
    ga_OsServiceIdentifiers[OSServiceId_SyncScheduleTable] = (const char*)"SyncScheduleTable"; /* OSServiceId_SyncScheduleTable */
    ga_OsServiceIdentifiers[OSServiceId_SetScheduleTableAsync] = (const char*)"SetScheduleTableAsync"; /* OSServiceId_SetScheduleTableAsync */
    ga_OsServiceIdentifiers[OSServiceId_GetScheduleTableStatus] = (const char*)"GetScheduleTableStatus"; /* OSServiceId_GetScheduleTableStatus */
    ga_OsServiceIdentifiers[OSServiceId_StartCore] = (const char*)"StartCore"; /* OSServiceId_StartCore */
    ga_OsServiceIdentifiers[OSServiceId_StartNonAutosarCore] = (const char*)"StartNonAutosarCore"; /* OSServiceId_StartNonAutosarCore */
    ga_OsServiceIdentifiers[OSServiceId_ShutdownAllCores] = (const char*)"ShutdownAllCores"; /* OSServiceId_ShutdownAllCores */
    ga_OsServiceIdentifiers[OSServiceId_GetApplicationID] = (const char*)"GetApplicationID"; /* OSServiceId_GetApplicationID */
    ga_OsServiceIdentifiers[OSServiceId_GetApplicationState] = (const char*)"GetApplicationState"; /* OSServiceId_GetApplicationState */
    ga_OsServiceIdentifiers[OSServiceId_AllowAccess] = (const char*)"AllowAccess"; /* OSServiceId_AllowAccess */
    ga_OsServiceIdentifiers[OSServiceId_TerminateApplication] = (const char*)"TerminateApplication"; /* OSServiceId_TerminateApplication */
    ga_OsServiceIdentifiers[OSServiceId_GetSpinlock] = (const char*)"GetSpinlock"; /* OSServiceId_GetSpinlock */
    ga_OsServiceIdentifiers[OSServiceId_ReleaseSpinlock] = (const char*)"ReleaseSpinlock"; /* OSServiceId_ReleaseSpinlock */
    ga_OsServiceIdentifiers[OSServiceId_TryToGetSpinlock] = (const char*)"TryToGetSpinlock"; /* OSServiceId_TryToGetSpinlock */
    ga_OsServiceIdentifiers[OSServiceId_GetISRID] = (const char*)"GetISRID"; /* OSServiceId_GetISRID */
    ga_OsServiceIdentifiers[OSServiceId_CallTrustedFunction] = (const char*)"CallTrustedFunction"; /* OSServiceId_CallTrustedFunction */
    ga_OsServiceIdentifiers[OSServiceId_CheckObjectAccess] = (const char*)"CheckObjectAccess"; /* OSServiceId_CheckObjectAccess */


    /*
     *  Only for VEOS
     */
    ga_OsServiceIdentifiers[OSServiceId_EnterISR] = (const char*)"EnterISR"; /* OSServiceId_EnterISR */
    ga_OsServiceIdentifiers[OSServiceId_LeaveISR] = (const char*)"LeaveISR"; /* OSServiceId_LeaveISR */
    ga_OsServiceIdentifiers[OSServiceId_EnableInterrupt] = (const char*)"EnableInterrupt"; /* OSServiceId_EnableInterrupt */
    ga_OsServiceIdentifiers[OSServiceId_DisableInterrupt] = (const char*)"DisableInterrupt"; /* OSServiceId_DisableInterrupt */
    ga_OsServiceIdentifiers[OSServiceId_GetInterruptDescriptor] = (const char*)"GetInterruptDescriptor"; /* OSServiceId_GetInterruptDescriptor */
    ga_OsServiceIdentifiers[OSServiceId_SendMessage] = (const char*)"SendMessage"; /* OSServiceId_SendMessage */
    ga_OsServiceIdentifiers[OSServiceId_ReceiveMessage] = (const char*)"ReceiveMessage"; /* OSServiceId_ReceiveMessage */
    ga_OsServiceIdentifiers[OSServiceId_ActivateISR] = (const char*)"ActivateISR"; /* OSServiceId_ActivateISR */
    ga_OsServiceIdentifiers[OSServiceId_InitCounter] = (const char*)"TlInitCounter"; /* OSServiceId_InitCounter */
    ga_OsServiceIdentifiers[OSServiceId_CounterTrigger] = (const char*)"TlCounterTrigger"; /* OSServiceId_CounterTrigger */
    ga_OsServiceIdentifiers[OSServiceId_GetCounterValue1] = (const char*)"TlGetCounterValue1"; /* OSServiceId_GetCounterValue1 */
    ga_OsServiceIdentifiers[OSServiceId_GetCounterValue2] = (const char*)"TlGetCounterValue2"; /* OSServiceId_GetCounterValue2 */
    ga_OsServiceIdentifiers[OSServiceId_XPeripheral] = (const char*)"Read|Write|Modify|Peripheral|8|16|32"; /* OSServiceId_XPeripheral */
    ga_OsServiceIdentifiers[OSServiceId_CheckXMemoryAccess] = (const char*)"Check|ISR|Task|MemoryAccess"; /* OSServiceId_CheckXMemoryAccess */
    ga_OsServiceIdentifiers[OSServiceId_CheckObjectOwnerShip] = (const char*)"CheckObjectOwnerShip"; /* OSServiceId_CheckObjectOwnerShip */ 


    if (serviceId > OS_ERRORHANDLER_MAX_SERVICE_ID)
    {
        return ga_OsServiceIdentifiers[0];
    }

    return ga_OsServiceIdentifiers[serviceId];
}

/************************************************************************\
 *  Function to determine name of the error for the error message.      *
\************************************************************************/
Os_ErrorLevelDescType Os_GetErrorType(StatusType Error)
{

    ga_OsErrorLevelIdentifiers[E_OK] = (const char *)"E_OK"; /* E_OK */
    ga_OsErrorLevelIdentifiers[E_OS_ACCESS] = (const char *)"E_OS_ACCESS"; /* E_OS_ACCESS */
    ga_OsErrorLevelIdentifiers[E_OS_CALLEVEL] = (const char *)"E_OS_CALLEVEL"; /* E_OS_CALLEVEL */
    ga_OsErrorLevelIdentifiers[E_OS_ID] = (const char *)"E_OS_ID"; /* E_OS_ID */
    ga_OsErrorLevelIdentifiers[E_OS_LIMIT] = (const char *)"E_OS_LIMIT"; /* E_OS_LIMIT */
    ga_OsErrorLevelIdentifiers[E_OS_NOFUNC] = (const char *)"E_OS_NOFUNC"; /* E_OS_NOFUNC */
    ga_OsErrorLevelIdentifiers[E_OS_RESOURCE] = (const char *)"E_OS_RESOURCE"; /* E_OS_RESOURCE */
    ga_OsErrorLevelIdentifiers[E_OS_STATE] = (const char *)"E_OS_STATE"; /* E_OS_STATE */
    ga_OsErrorLevelIdentifiers[E_OS_VALUE] = (const char *)"E_OS_VALUE"; /* E_OS_VALUE */
    ga_OsErrorLevelIdentifiers[E_OS_MISSINGEND] = (const char *)"E_OS_MISSINGEND"; /* E_OS_MISSINGEND */
    ga_OsErrorLevelIdentifiers[E_OS_DISABLEDINT] = (const char *)"E_OS_DISABLEDINT"; /* E_OS_DISABLEDINT */
    ga_OsErrorLevelIdentifiers[E_OS_SERVICEID] = (const char *)"E_OS_SERVICEID"; /* E_OS_SERVICEID */
    ga_OsErrorLevelIdentifiers[E_OS_ILLEGAL_ADDRESS] = (const char *)"E_OS_ILLEGAL_ADDRESS"; /* E_OS_ILLEGAL_ADDRESS */
    ga_OsErrorLevelIdentifiers[E_OS_STACKFAULT] = (const char *)"E_OS_STACKFAULT"; /* E_OS_STACKFAULT */
    ga_OsErrorLevelIdentifiers[E_OS_PROTECTION_MEMORY] = (const char *)"E_OS_PROTECTION_MEMORY"; /* E_OS_PROTECTION_MEMORY */
    ga_OsErrorLevelIdentifiers[E_OS_PROTECTION_TIME] = (const char *)"E_OS_PROTECTION_TIME"; /* E_OS_PROTECTION_TIME */
    ga_OsErrorLevelIdentifiers[E_OS_PROTECTION_ARRIVAL] = (const char *)"E_OS_PROTECTION_ARRIVAL"; /* E_OS_PROTECTION_ARRIVAL */
    ga_OsErrorLevelIdentifiers[E_OS_PROTECTION_LOCKED] = (const char *)"E_OS_PROTECTION_LOCKED"; /* E_OS_PROTECTION_LOCKED */
    ga_OsErrorLevelIdentifiers[E_OS_PROTECTION_EXCEPTION] = (const char *)"E_OS_PROTECTION_EXCEPTION"; /* E_OS_PROTECTION_EXCEPTION */
    ga_OsErrorLevelIdentifiers[E_OS_CORE] = (const char *)"E_OS_CORE"; /* E_OS_CORE */
    ga_OsErrorLevelIdentifiers[E_OS_SPINLOCK] = (const char *)"E_OS_SPINLOCK"; /* E_OS_SPINLOCK */
    ga_OsErrorLevelIdentifiers[E_OS_INTERFERENCE_DEADLOCK] = (const char *)"E_OS_INTERFERENCE_DEADLOCK"; /* E_OS_INTERFERENCE_DEADLOCK */
    ga_OsErrorLevelIdentifiers[E_OS_NESTING_DEADLOCK] = (const char *)"E_OS_NESTING_DEADLOCK"; /* E_OS_NESTING_DEADLOCK */
    ga_OsErrorLevelIdentifiers[E_OS_PARAM_POINTER] = (const char *)"E_OS_PARAM_POINTER"; /* E_OS_PARAM_POINTER */


    /*-------- Only for VEOS ---------*/
    ga_OsErrorLevelIdentifiers[E_OS_SYS_INTERNAL] = (const char *)"E_OS_SYS_INTERNAL"; /* E_OS_PARAM_POINTER */

    if (Error > OS_ERRORHANDLER_MAX_ERROR_LEVEL)
    {
        return (char *)0;
    }

    return ga_OsErrorLevelIdentifiers[Error];
}

/************************************************************************\
 *  Function to transfers the error message to the simulator backplane  *
\************************************************************************/
void Os_ErrorHook(StatusType Error)
{
    Os_ServiceServiceDescType serviceIdentifier = Os_GetServiceIdentifier(g_ErrorInfo.ServiceID);
    Os_ErrorLevelDescType errorType = Os_GetErrorType(Error);
    Os_SimulationCoreErrorHook(Error, serviceIdentifier, errorType);
}

#endif /* OS_ERRORHANDLER_C */

