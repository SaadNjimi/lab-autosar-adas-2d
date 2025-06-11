/**************************************************************************************************\
 *** Os_Stubs.c
 ***
 *** AUTOSAR API function stubs for elements not implemented by this OS.
 ***
 *** Copyright (c) 2014 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/
#include "Os_Stubs.h"
#include "Os_ErrorHandler.h"
#include "Os_Kernel.h"
#include "Os_InternalTypes.h"

/*------------------------------------------------------------------------------------------------*\
  DEFINES
\*------------------------------------------------------------------------------------------------*/
#define ACCESS_GRANTED                              ((AccessType)0x1)
#define ACCESS_DENIED                               ((AccessType)0x0)

#define INVALID_ISR                                 ((ISRType)-1)


/*------------------------------------------------------------------------------------------------*\
  FUNCTION DEFINITIONS
\*------------------------------------------------------------------------------------------------*/

#if !defined(OS_EXCLUDE_GETISRID)
/* [SWS_Os_00511] */
ISRType GetISRID(void)
{
    g_ErrorInfo.ServiceID = OSServiceId_GetISRID;
    ERRORHOOK(E_OS_NOFUNC);
    return INVALID_ISR;
}
#endif

#if !defined(OS_EXCLUDE_CALLTRUSTEDFUNCTION)
/* [SWS_Os_00097] */
StatusType CallTrustedFunction(TrustedFunctionIndexType FunctionIndex, TrustedFunctionParameterRefType FunctionParams)
{
    /* to avoid the compiler warning C4100 */
    (void)(FunctionIndex);
    (void)(FunctionParams);

    g_ErrorInfo.ServiceID = OSServiceId_CallTrustedFunction;
    ERRORHOOK(E_OS_NOFUNC);
    return E_OS_NOFUNC;
}
#endif

#if !defined(OS_EXCLUDE_CHECKOBJECTACCESS)
/* [SWS_Os_00256] */
StatusType CheckObjectAccess(ApplicationType ApplID, ObjectTypeType ObjectType, void *ReferencedObject)
{
    /* to avoid the compiler warning C4100 */
    (void)(ApplID);
    (void)(ObjectType);
    (void)(ReferencedObject);

#ifdef RES_SCHEDULER
    if ((OBJECT_RESOURCE == ObjectType) && (RES_SCHEDULER == (*((ResourceType*) ReferencedObject))))
    {
        return ACCESS;
    }
#endif /* RES_SCHEDULER */

    g_ErrorInfo.ServiceID = OSServiceId_CheckObjectAccess;
    ERRORHOOK(E_OS_NOFUNC);
    return NO_ACCESS;
}
#endif

#if !defined(OS_EXCLUDE_CHECKOBJECTOWNERSHIP)
/* [SWS_Os_00017] */
ApplicationType CheckObjectOwnerShip(ObjectTypeType ObjectType, void *ReferencedObject)
{
    /* to avoid the compiler warning C4100 */
    (void)(ObjectType);
    (void)(ReferencedObject);

    g_ErrorInfo.ServiceID = OSServiceId_CheckObjectOwnerShip;
    ERRORHOOK(E_OS_NOFUNC);
    return INVALID_OSAPPLICATION;
}
#endif

#if !defined(OS_EXCLUDE_CHECKISRMEMORYACCESS)
/* [SWS_Os_00512] */
AccessType CheckISRMemoryAccess(ISRType ISRID, MemoryStartAddressType Address, MemorySizeType Size)
{
    /* to avoid the compiler warning C4100 */
    (void)(ISRID);
    (void)(Address);
    (void)(Size);

    g_ErrorInfo.ServiceID = OSServiceId_CheckXMemoryAccess;
    ERRORHOOK(E_OS_NOFUNC);
    return ACCESS_DENIED;
}
#endif

#if !defined(OS_EXCLUDE_CHECKTASKMEMORYACCESS)
/* [SWS_Os_00513] */
AccessType CheckTaskMemoryAccess(TaskType TaskID, MemoryStartAddressType Address, MemorySizeType Size)
{
    /* to avoid the compiler warning C4100 */
    (void)(TaskID);
    (void)(Address);
    (void)(Size);

    g_ErrorInfo.ServiceID = OSServiceId_CheckXMemoryAccess;
    ERRORHOOK(E_OS_NOFUNC);
    return ACCESS_GRANTED;
}
#endif

#if !defined(OS_EXCLUDE_READPERIPHERAL8)
/* [SWS_Os_91013] */
StatusType ReadPeripheral8( AreaIdType Area, const uint8* Address, uint8* ReadValue )
{
    /* to avoid the compiler warning C4100 */
    (void)(Area);
    (void)(Address);
    (void)(ReadValue);

    g_ErrorInfo.ServiceID = OSServiceId_XPeripheral;
    ERRORHOOK(E_OS_NOFUNC);
    return NO_ACCESS;
}
#endif

#if !defined(OS_EXCLUDE_READPERIPHERAL16)
/* [SWS_Os_91015] */
StatusType ReadPeripheral16( AreaIdType Area, const uint16* Address, uint16* ReadValue )
{
    /* to avoid the compiler warning C4100 */
    (void)(Area);
    (void)(Address);
    (void)(ReadValue);

    ERRORHOOK(E_OS_NOFUNC);
    return NO_ACCESS;
}
#endif

#if !defined(OS_EXCLUDE_READPERIPHERAL32)
/* [SWS_Os_91014] */
StatusType ReadPeripheral32( AreaIdType Area, const uint32* Address, uint32* ReadValue )
{
    /* to avoid the compiler warning C4100 */
    (void)(Area);
    (void)(Address);
    (void)(ReadValue);

    g_ErrorInfo.ServiceID = OSServiceId_XPeripheral;
    ERRORHOOK(E_OS_NOFUNC);
    return NO_ACCESS;
}
#endif

#if !defined(OS_EXCLUDE_WRITEPERIPHERAL8)
/* [SWS_Os_91010] */
StatusType WritePeripheral8( AreaIdType Area, uint8* Address, uint8 WriteValue )
{
    /* to avoid the compiler warning C4100 */
    (void)(Area);
    (void)(Address);
    (void)(WriteValue);

    g_ErrorInfo.ServiceID = OSServiceId_XPeripheral;
    ERRORHOOK(E_OS_NOFUNC);
    return NO_ACCESS;
}
#endif

#if !defined(OS_EXCLUDE_WRITEPERIPHERAL16)
/* [SWS_Os_91012] */
StatusType WritePeripheral16( AreaIdType Area, uint16* Address, uint16 WriteValue )
{
    /* to avoid the compiler warning C4100 */
    (void)(Area);
    (void)(Address);
    (void)(WriteValue);

    g_ErrorInfo.ServiceID = OSServiceId_XPeripheral;
    ERRORHOOK(E_OS_NOFUNC);
    return NO_ACCESS;
}
#endif

#if !defined(OS_EXCLUDE_WRITEPERIPHERAL32)
/* [SWS_Os_91011] */
StatusType WritePeripheral32( AreaIdType Area, uint32* Address, uint32 WriteValue )
{
    /* to avoid the compiler warning C4100 */
    (void)(Area);
    (void)(Address);
    (void)(WriteValue);

    g_ErrorInfo.ServiceID = OSServiceId_XPeripheral;
    ERRORHOOK(E_OS_NOFUNC);
    return NO_ACCESS;
}
#endif

#if !defined(OS_EXCLUDE_MODIFYPERIPHERAL8)
/* [SWS_Os_91016] */
StatusType ModifyPeripheral8( AreaIdType Area, uint8* Address, uint8 Clearmask, uint8 Setmask )
{
    /* to avoid the compiler warning C4100 */
    (void)(Area);
    (void)(Address);
    (void)(Clearmask);
    (void)(Setmask);

    g_ErrorInfo.ServiceID = OSServiceId_XPeripheral;
    ERRORHOOK(E_OS_NOFUNC);
    return NO_ACCESS;
}
#endif

#if !defined(OS_EXCLUDE_MODIFYPERIPHERAL16)
/* [SWS_Os_91018] */
StatusType ModifyPeripheral16( AreaIdType Area, uint16* Address, uint16 Clearmask, uint16 Setmask )
{
    /* to avoid the compiler warning C4100 */
    (void)(Area);
    (void)(Address);
    (void)(Clearmask);
    (void)(Setmask);

    g_ErrorInfo.ServiceID = OSServiceId_XPeripheral;
    ERRORHOOK(E_OS_NOFUNC);
    return NO_ACCESS;
}
#endif

#if !defined(OS_EXCLUDE_MODIFYPERIPHERAL32)
/* [SWS_Os_91017] */
StatusType ModifyPeripheral32( AreaIdType Area, uint32* Address, uint32 Clearmask, uint32 Setmask )
{
     /* to avoid the compiler warning C4100 */
    (void)(Area);
    (void)(Address);
    (void)(Clearmask);
    (void)(Setmask);

    g_ErrorInfo.ServiceID = OSServiceId_XPeripheral;
    ERRORHOOK(E_OS_NOFUNC);
    return NO_ACCESS;
}
#endif


#ifdef ENABLE_OS_FUNCTION_BACKWARD_COMPATIBILITY

/* Start and Shutdown */
#undef Os_StartOs
void Os_StartOs(AppModeType appMode)
{
    StartOS(appMode);
}

#undef Os_ShutdownOS
void Os_ShutdownOS(StatusType error)
{
    ShutdownOS(error);
}


/* Task */
#undef Os_ActivateTask
StatusType Os_ActivateTask(TaskType taskId)
{
    return ActivateTask(taskId);
}

#undef Os_ChainTask
StatusType Os_ChainTask(TaskType taskId)
{
    return ChainTask(taskId);
}

#undef Os_GetTaskID
StatusType Os_GetTaskID(TaskRefType taskId)
{
    return GetTaskID(taskId);
}

#undef Os_GetTaskState
StatusType Os_GetTaskState(TaskType taskId, TaskStateType* state)
{
    return GetTaskState(taskId, state);
}

#undef Os_Schedule
StatusType Os_Schedule(void)
{
    return Schedule();
}


/* Resource */
#undef Os_GetResource
StatusType Os_GetResource(ResourceType resourceId)
{
    return GetResource(resourceId);
}

#undef Os_ReleaseResource
StatusType Os_ReleaseResource(ResourceType resourceId)
{
    return ReleaseResource(resourceId);
}


/* Event */
#undef Os_SetEvent
StatusType Os_SetEvent(TaskType taskId, EventMaskType    eventMask)
{
    return SetEvent(taskId, eventMask);
}

#undef Os_GetEvent
StatusType Os_GetEvent(TaskType taskId, EventMaskRefType pEventMask)
{
    return GetEvent(taskId, pEventMask);
}

#undef Os_WaitEvent
StatusType Os_WaitEvent(EventMaskType eventMask)
{
    return WaitEvent(eventMask);
}

#undef Os_ClearEvent
StatusType Os_ClearEvent(EventMaskType eventMask)
{
    return ClearEvent(eventMask);
}
 

/* Counter */
#undef Os_IncrementCounter
StatusType Os_IncrementCounter(CounterType counterId)
{
    return IncrementCounter(counterId);
}
 
#undef Os_GetCounterValue
StatusType Os_GetCounterValue(CounterType counterId, TickRefType tick)
{
    return GetCounterValue(counterId, tick);
}
 
#undef Os_GetElapsedCounterValue
StatusType Os_GetElapsedCounterValue(CounterType counterId, TickRefType value, TickRefType elapsedValue)
{
    return GetElapsedCounterValue(counterId, value, elapsedValue);
}
 

#if (NUMBER_OF_ALARMS > 0)
/* Alarm */
#undef Os_GetAlarmBase
StatusType Os_GetAlarmBase(AlarmType alarmId, AlarmBaseRefType alarmBase)
{
    return GetAlarmBase(alarmId, alarmBase);
}

#undef Os_GetAlarm
StatusType Os_GetAlarm(AlarmType alarmId, TickRefType ticks)
{
    return GetAlarm(alarmId, ticks);
}

#undef Os_SetRelAlarm
StatusType Os_SetRelAlarm(AlarmType alarmId, TickType increment, TickType cycle)
{
    return SetRelAlarm(alarmId, increment, cycle);
}
#undef Os_SetAbsAlarm
StatusType Os_SetAbsAlarm(AlarmType alarmId, TickType start, TickType cycle)
{
    return SetAbsAlarm(alarmId, start, cycle);
}

#undef Os_CancelAlarm
StatusType Os_CancelAlarm(AlarmType alarmId)
{
    return CancelAlarm(alarmId);
}
#endif  /* NUMBER_OF_ALARMS > 0 */


/* Interrupt */
#undef Os_SuspendAllInterrupts
void Os_SuspendAllInterrupts(void)
{
    SuspendAllInterrupts();
}

#undef Os_ResumeAllInterrupts
void Os_ResumeAllInterrupts(void)
{
    ResumeAllInterrupts();
}

#undef Os_DisableAllInterrupts
void Os_DisableAllInterrupts(void)
{
    DisableAllInterrupts();
}

#undef Os_EnableAllInterrupts
void Os_EnableAllInterrupts(void)
{
    EnableAllInterrupts();
}

#undef Os_ResumeOSInterrupts
void Os_ResumeOSInterrupts(void)
{
    ResumeOSInterrupts();
}

#undef Os_SuspendOSInterrupts
void Os_SuspendOSInterrupts(void)
{
    SuspendOSInterrupts();
}


/* ScheduleTable */
#if NUMBER_OF_SCHEDULE_TABLES > 0
#undef Os_StartScheduleTableRel
StatusType Os_StartScheduleTableRel(ScheduleTableType ScheduleTableID, TickType Offset)
{
    return StartScheduleTableRel(ScheduleTableID, Offset);
}

#undef Os_StartScheduleTableAbs
StatusType Os_StartScheduleTableAbs(ScheduleTableType ScheduleTableID, TickType Start)
{
    return StartScheduleTableAbs(ScheduleTableID, Start);
}

#undef Os_StartScheduleTableSynchron
StatusType Os_StartScheduleTableSynchron(ScheduleTableType scheduleTableId)
{
    return StartScheduleTableSynchron(scheduleTableId);
}

#undef Os_StopScheduleTable
StatusType Os_StopScheduleTable(ScheduleTableType scheduleTableId)
{
    return StopScheduleTable(scheduleTableId);
}

#undef Os_extScheduleTable
StatusType Os_NextScheduleTable(ScheduleTableType scheduleTableId_From, ScheduleTableType scheduleTableId_To)
{
    return NextScheduleTable(scheduleTableId_From, scheduleTableId_To);
}

#undef Os_SyncScheduleTable
StatusType Os_SyncScheduleTable(ScheduleTableType scheduleTableId, TickType Value)
{
    return SyncScheduleTable(scheduleTableId, Value);
}

#undef Os_SetScheduleTableAsync
StatusType Os_SetScheduleTableAsync(ScheduleTableType scheduleTableId)
{
    return SetScheduleTableAsync(scheduleTableId);
}

#undef Os_GetScheduleTableStatus
StatusType Os_GetScheduleTableStatus(ScheduleTableType scheduleTableId, ScheduleTableStatusRefType ScheduleStatus)
{
    return GetScheduleTableStatus(scheduleTableId, ScheduleStatus);
}
#endif  /* NUMBER_OF_SCHEDULE_TABLES > 0 */


#endif /* ENABLE_OS_FUNCTION_BACKWARD_COMPATIBILITY */



/*------------------------------------------------------------------------------------------------*\
  END OD FILE
\*------------------------------------------------------------------------------------------------*/
