/**************************************************************************************************\
 *** Os_ErrorHandler.h
 ***
 *** Definitions for OSEK error hook function.
 ***
 *** Copyright (c) 2009 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef OS_ERROR_HANDLER_H
#define OS_ERROR_HANDLER_H

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/

#include "Os_InternalTypes.h"

/*------------------------------------------------------------------------------------------------*\
  DEFINES
\*------------------------------------------------------------------------------------------------*/

#ifdef OS_DBGMSG_ENABLED
#undef OS_DBGMSG_ENABLED
#endif

#ifdef VEOS_DEBUG
#define OS_DBGMSG_ENABLED
#else
#define Os_SimulationCoreLogDbgMessage                            (void)0
#endif



/*-------- Only for VEOS ---------*/
#define E_OS_SYS_INTERNAL (E_OS_PARAM_POINTER + 1) 

#define OSServiceId_EnterISR (OSServiceId_ShutdownAllCores + 1)
#define OSServiceId_LeaveISR (OSServiceId_ShutdownAllCores + 2)
#define OSServiceId_EnableInterrupt (OSServiceId_ShutdownAllCores + 3)
#define OSServiceId_DisableInterrupt (OSServiceId_ShutdownAllCores + 4)
#define OSServiceId_GetInterruptDescriptor (OSServiceId_ShutdownAllCores + 5)
#define OSServiceId_SendMessage (OSServiceId_ShutdownAllCores + 6)
#define OSServiceId_ReceiveMessage (OSServiceId_ShutdownAllCores + 7)
#define OSServiceId_ActivateISR (OSServiceId_ShutdownAllCores + 8)
#define OSServiceId_InitCounter (OSServiceId_ShutdownAllCores + 9)
#define OSServiceId_CounterTrigger (OSServiceId_ShutdownAllCores + 10)
#define OSServiceId_GetCounterValue1 (OSServiceId_ShutdownAllCores + 11)
#define OSServiceId_GetCounterValue2 (OSServiceId_ShutdownAllCores + 12)
#define OSServiceId_XPeripheral (OSServiceId_ShutdownAllCores + 13)
#define OSServiceId_CheckXMemoryAccess (OSServiceId_ShutdownAllCores + 14)
#define OSServiceId_CheckObjectOwnerShip (OSServiceId_ShutdownAllCores + 15)



/*------------------------------------------------------------------------------------------------*\
  TYPEDEFS
\*------------------------------------------------------------------------------------------------*/

typedef struct tagOsSActivateTaskParam
{
    TaskType TaskID;
} Os_SActivateTaskParam;

typedef struct tagOsSChainTaskParam
{
    TaskType TaskID;
} Os_SChainTaskParam;

typedef struct tagOsSGetTaskStateParam
{
    TaskType TaskID;
} Os_SGetTaskStateParam;

typedef struct tagOsSEnableInterruptParam
{
    Os_IntDescriptorType  Descriptor;
} Os_SEnableInterruptParam;

typedef struct tagOsSDisableInterruptParam
{
    Os_IntDescriptorType  Descriptor;
} Os_SDisableInterruptParam;

typedef struct tagOsSGetResourceParam
{
    ResourceType ResID;
} Os_SGetResourceParam;

typedef struct tagOsSReleaseResourceParam
{
    ResourceType ResID;
} Os_SReleaseResourceParam;

typedef struct tagOsSSetEventParam
{
    TaskType TaskID;
    EventMaskType Mask;
} Os_SSetEventParam;

typedef struct tagOsSClearEventParam
{
    EventMaskType Mask;
} Os_SClearEventParam;

typedef struct tagTlSGetEventParam
{
    TaskType TaskID;
} Os_SGetEventParam;

typedef struct tagOsSWaitEventParam
{
    EventMaskType Mask;
} Os_SWaitEventParam;

typedef struct tagOsSGetAlarmBaseParam
{
    AlarmType AlarmID;
} Os_SGetAlarmBaseParam;

typedef struct tagOsSGetAlarmParam
{
    AlarmType AlarmID;
} Os_SGetAlarmParam;

typedef struct tagOsSSetRelAlarmParam
{
    AlarmType AlarmID;
    TickType increment;
    TickType cycle;
} Os_SSetRelAlarmParam;

typedef struct tagOsSSetAbsAlarmParam
{
    AlarmType AlarmID;
    TickType start;
    TickType cycle;
} Os_SSetAbsAlarmParam;

typedef struct tagOsSCancelAlarmParam
{
    AlarmType AlarmID;
} Os_SCancelAlarmParam;

typedef struct tagOsSStartOSParam
{
    AppModeType Mode;
} Os_SStartOSParam;

typedef struct tagOsSShutdownOSParam
{
    StatusType Error;
} Os_SShutdownOSParam;

typedef struct tagOsSSendMessageParam
{
    Os_SymbolicName Message;
    Os_AccessNameRef Data;
} Os_SSendMessageParam;

typedef struct tagOsSReceiveMessageParam
{
    Os_SymbolicName Message;
    Os_AccessNameRef Data;
} Os_SReceiveMessageParam;

typedef struct tagOsSStartCoreParam
{
    CoreIdType CoreID;
} Os_SStartCoreParam;

typedef struct tagOsSGetApplicationStateParam
{
    ApplicationType ApplicationID;
} Os_SGetApplicationStateParam;

typedef struct tagOsSTerminateApplicationParam
{
    ApplicationType ApplicationID;
} Os_STerminateApplicationParam;

typedef struct tagOsSGetSpinlockParam
{
    SpinlockIdType SpinlockID;
} Os_SGetSpinlockParam;

typedef struct tagOsSReleaseSpinlockParam
{
    SpinlockIdType SpinlockID;
} Os_SReleaseSpinlockParam;

typedef struct tagOsSTryToGetSpinlockParam
{
    SpinlockIdType SpinlockID;
} Os_STryToGetSpinlockParam;

typedef union tagOsUServices
{
    Os_SActivateTaskParam          SvActivateTask;
    Os_SChainTaskParam             SvChainTask;
    Os_SGetTaskStateParam          SvGetTaskState;
    Os_SEnableInterruptParam       SvEnableInterrupt;
    Os_SDisableInterruptParam      SvDisableInterrupt;
    Os_SGetResourceParam           SvGetResource;
    Os_SReleaseResourceParam       SvReleaseResource;
    Os_SSetEventParam              SvSetEvent;
    Os_SClearEventParam            SvClearEvent;
    Os_SGetEventParam              SvGetEvent;
    Os_SWaitEventParam             SvWaitEvent;
    Os_SGetAlarmBaseParam          SvGetAlarmBase;
    Os_SGetAlarmParam              SvGetAlarm;
    Os_SSetRelAlarmParam           SvSetRelAlarm;
    Os_SSetAbsAlarmParam           SvSetAbsAlarm;
    Os_SCancelAlarmParam           SvCancelAlarm;
    Os_SStartOSParam               SvStartOS;
    Os_SShutdownOSParam            SvShutdownOS;
    Os_SSendMessageParam           SvSendMessage;
    Os_SReceiveMessageParam        SvReceiveMessage;
    Os_SStartCoreParam             SvStartCore;
    Os_SGetApplicationStateParam   SvGetApplicationState;
    Os_STerminateApplicationParam  SvTerminateApplication;
    Os_SGetSpinlockParam           SvGetSpinlock;
    Os_SReleaseSpinlockParam       SvReleaseSpinlock;
    Os_STryToGetSpinlockParam      SvTryToGetSpinlock;
} Os_UServices;

typedef struct tagOsSErrorInfoType
{
    Os_ServiceIdType   ServiceID;
    Os_UServices       Service;
    StatusType         Error;
} Os_SErrorInfoType;

/*------------------------------------------------------------------------------------------------*\
  FORWARD DECLARATIONS
\*------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

extern Os_SErrorInfoType g_ErrorInfo;

#ifdef __cplusplus
}
#endif

#endif /* OS_ERROR_HANDLER_H */
