/**************************************************************************************************\
 *** Os_Osek.h
 ***
 *** Backward compatibility of older V-ECUs.
 ***
 *** Copyright (c) 2020 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef OS_OSEK_H
#define OS_OSEK_H

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/
#include "Platform_Types.h"
#include "Os_Types.h"

/* Include generated part of OS configuration. */
#include "Os_Cfg_generated.h"
#include "Os_InternalTypes.h"


/*------------------------------------------------------------------------------------------------*\
  DEFINES
\*------------------------------------------------------------------------------------------------*/


/* Section 13.1 Common data types */

#define OS_E_OK                     E_OK

#define OS_E_ACCESS                 E_OS_ACCESS
#define OS_E_CALLEVEL               E_OS_CALLEVEL
#define OS_E_ID                     E_OS_ID
#define OS_E_LIMIT                  E_OS_LIMIT
#define OS_E_NOFUNC                 E_OS_NOFUNC
#define OS_E_RESOURCE               E_OS_RESOURCE
#define OS_E_STATE                  E_OS_STATE
#define OS_E_VALUE                  E_OS_VALUE
#define OS_E_NESTING_DEADLOCK       E_OS_NESTING_DEADLOCK
#define OS_E_INTERFERENCE_DEADLOCK  E_OS_INTERFERENCE_DEADLOCK
#define OS_E_CORE                   E_OS_CORE

#define OS_IOC_E_OK                 IOC_E_OK
#define OS_IOC_E_LENGTH             IOC_E_LENGTH
#define OS_IOC_E_LIMIT              IOC_E_LIMIT
#define OS_IOC_E_LOST_DATA          IOC_E_LOST_DATA
#define OS_IOC_E_NO_DATA            IOC_E_NO_DATA

#define OS_E_SYS_SCHED              E_OS_SYS_SCHED
#define OS_E_SYS_CALLEVEL           E_OS_SYS_CALLEVEL
#define OS_E_SYS_INTERNAL           E_OS_SYS_INTERNAL

/* Section 13.2 Task management - 13.2.4  Constants */
#define OS_TASKSTATE_RUNNING        RUNNING
#define OS_TASKSTATE_WAITING        WAITING
#define OS_TASKSTATE_READY          READY
#define OS_TASKSTATE_SUSPENDED      SUSPENDED

#define OS_TASKID_INVALID           INVALID_TASK

#define OS_SCHEDULETABLEMODE_STOPPED  SCHEDULETABLE_STOPPED
#define OS_SCHEDULETABLEMODE_NEXT     SCHEDULETABLE_NEXT
#define OS_SCHEDULETABLEMODE_WAITING  SCHEDULETABLE_WAITING
#define OS_SCHEDULETABLEMODE_RUNNING  SCHEDULETABLE_RUNNING
#define OS_SCHEDULETABLEMODE_RUNNING_AND_SYNCHRONOUS  SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS

#define OS_APPLICATION_ACCESSIBLE  APPLICATION_ACCESSIBLE
#define OS_APPLICATION_RESTARTING  APPLICATION_RESTARTING
#define OS_APPLICATION_TERMINATED  APPLICATION_TERMINATED

/* Section 13.8 Hook routines - 13.8.1 Data Types */
//#define Os_ServiceIdType            OSServiceIdType


/*------------------------------------------------------------------------------------------------*\
  TYPEDEFS
\*------------------------------------------------------------------------------------------------*/

/* Section 13.1 Common data types */

#define Os_StatusType                  StatusType

/* Section 13.2 Task management - 13.2.1 Data types */

/* This data type identifies a task.  */
#define Os_TaskType                    TaskType

/* This data type points to a variable of TaskType.  */
#define Os_TaskRefType                 TaskRefType

/* This data type identifies the state of a task.  */
#define Os_TaskStateType               TaskStateType

/* This data type points to a variable of the data type TaskStateType */
#define Os_TaskStateRefType            TaskStateRefType

/* Section 13.4  Resource management - 13.4.1  Data types */

/* Data type for a resource. */
#define Os_ResourceType                ResourceType

/* Section 13.5  Event control - 13.5.1  Data types */

/* Data type of the event mask.  */
#define Os_EventMaskType               EventMaskType

/* Reference to an event mask. */
#define Os_EventMaskRefType            EventMaskRefType

/* Section 13.6 Alarms - 13.6.1  Data types */

/* This data type represents count values in ticks */
#define Os_TickType                    TickType

/* This data type points to the data type TickType */
#define Os_TickRefType                 TickRefType

/* This  data  type  represents  a  structure  for  storage  of  counter  characteristics. All elements of the structure are of data type TickType.  */
#define Os_AlarmBaseType               AlarmBaseType

/* This data type points to the data type AlarmBaseType */
#define Os_AlarmBaseRefType            AlarmBaseRefType

/* This data type represents an alarm object. */
#define Os_AlarmType                   AlarmType

/* Section 13.7 Operating system execution control - 13.7.1 Data types */
#define Os_AppModeType                 AppModeType

/* This data type represents a CoreIdType SWS_Os_00790 */
#define Os_CoreIdType                  CoreIdType

/* This data type represents a SpinlockIdType  SWS_Os_00791  */
#define Os_SpinlockIdType              SpinlockIdType

/* This data type represents a SpinlockIdType  SWS_Os_00792  */
#define Os_TryToGetSpinlockType        TryToGetSpinlockType

/* This data type represents a SpinlockIdType  SWS_Os_00793  */
#define Os_IdleModeType                IdleModeType

/* This data type identifies the state of an OS-Application  SWS_Os_00772  */
#define Os_ApplicationType             ApplicationType

/* This data type points to location where a ApplicationStateType can be stored.  SWS_Os_00774  */
#define Os_ApplicationStateRefType     ApplicationStateRefType

/* This data type identifies the state of an OS-Application  SWS_Os_00773  */
#define Os_ApplicationStateType        ApplicationStateType

#define Os_RestartType                 RestartType
#define Os_ScheduleTableType           ScheduleTableType

/* This data type identifies a peripheral area  SWS_Os_91000  */
#define Os_AreaIdType                  AreaIdType


/*------------------------------------------------------------------------------------------------*\
  PARAMETRIZED MACROS
\*------------------------------------------------------------------------------------------------*/

/* Section 13.5 Event control - 13.5.3  System services */
#define Os_WaitEvent(Mask)     WaitEvent(Mask)

/* Section 13.7  Operating system execution control - 13.7.2  System services */
#define Os_ShutdownOS(Error)   ShutdownOS(Error); 


/*------------------------------------------------------------------------------------------------*\
  FUNCTION REDIRECTIONS
\*------------------------------------------------------------------------------------------------*/

/* Section 13.2 Task management - 13.2.3  System services  */
#define Os_ActivateTask                ActivateTask
#define Os_Schedule                    Schedule
#define Os_GetTaskID                   GetTaskID
#define Os_GetTaskState                GetTaskState

/* Section 13.3 Interrupt handling - 13.3.2  System services  */
#define Os_EnableAllInterrupts         EnableAllInterrupts
#define Os_DisableAllInterrupts        DisableAllInterrupts
#define Os_ResumeAllInterrupts         ResumeAllInterrupts
#define Os_SuspendAllInterrupts        SuspendAllInterrupts
#define Os_ResumeOSInterrupts          ResumeOSInterrupts
#define Os_SuspendOSInterrupts         SuspendOSInterrupts

/* Section 13.4 Resource management - 13.4.3  System services */
#define Os_GetResource                 GetResource
#define Os_ReleaseResource             ReleaseResource

/* Section 13.5 Event control - 13.5.3  System services */
#define Os_SetEvent                    SetEvent
#define Os_ClearEvent                  ClearEvent
#define Os_GetEvent                    GetEvent

/* Section 13.6 Alarms - 13.6.3 System services */
#define Os_GetAlarmBase                GetAlarmBase
#define Os_GetAlarm                    GetAlarm
#define Os_SetRelAlarm                 SetRelAlarm
#define Os_SetAbsAlarm                 SetAbsAlarm
#define Os_CancelAlarm                 CancelAlarm

/* 13.7  Operating system execution control - 13.7.2  System services */
#define Os_GetActiveApplicationMode    GetActiveApplicationMode
#define Os_StartOS                     StartOs

/* AUTOSAR CP Release 4.3.0  8.4.24 GetCoreID */
#define Os_GetCoreID                   GetCoreID



#ifdef  __GCC__
  /* disable warning */
  /* needed for backward compatibility of older V-ECUs */
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wunused-value"

  /* #pragma GCC diagnostic pop  */
#endif



/*------------------------------------------------------------------------------------------------*\
  FUNCTION PROTOTYPES
\*------------------------------------------------------------------------------------------------*/
extern Os_KernelParamType          g_Kernel;


#endif /* OS_OSEK_H */

/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/
