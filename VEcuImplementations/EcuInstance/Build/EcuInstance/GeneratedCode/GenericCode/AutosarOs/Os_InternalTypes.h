/**************************************************************************************************\
 *** Os_InternalTypes.h
 ***
 *** This file contains all types specified from the Os.
 ***
 *** Copyright (c) 2008 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef OS_INTERNALTYPES_H
#define OS_INTERNALTYPES_H

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/
#include "Os_Types.h"
#include "Platform_Types.h"
#include "Std_Types.h"


/* Include generated part of OS configuration. */
#include "Os_Cfg_generated.h"

/*------------------------------------------------------------------------------------------------*\
  DEFINES
\*------------------------------------------------------------------------------------------------*/

#define TASK_LEVEL                          (Os_ApplicationLevelType)0x01u
#define OS_LEVEL                            (Os_ApplicationLevelType)0x02u
#define ISR_LEVEL                           (Os_ApplicationLevelType)0x03u
#define STARTUPHOOK_LEVEL                   (Os_ApplicationLevelType)0x04u
#define SHUTDOWNHOOK_LEVEL                  (Os_ApplicationLevelType)0x05u
#define ERRORHOOK_LEVEL                     (Os_ApplicationLevelType)0x06u
#define PRETASKHOOK_LEVEL                   (Os_ApplicationLevelType)0x07u
#define POSTTASKHOOK_LEVEL                  (Os_ApplicationLevelType)0x08u

/* Defines for the priority order. */
#define OS_SYS_PRIO_ORDERING_ASCENDING      0
#define OS_SYS_PRIO_ORDERING_DESCENDING     1

#define INITIAL_INTERRUPT_DESCRIPTOR        (Os_IntDescriptorType)0x00u

#define OS_TASKPREEMPTION_NONE              0x00u
#define OS_TASKPREEMPTION_FULL              0x01u

#define OS_TASKPRIORITY_MAX                 (Os_PrioType)2147483647
#define OS_TASKPRIORITY_MIN                 (Os_PrioType)-2147483647


/* Synchronisation ScheduleTables */
#define OS_EXPLICIT_SYNCHRONISATION         (Os_ScheduleTableSyncStrategy)0u
#define OS_IMPLICIT_SYNCHRONISATION         (Os_ScheduleTableSyncStrategy)1u
#define OS_NONE_SYNCHRONISATION             (Os_ScheduleTableSyncStrategy)2u

/* Counter driving */
#define OS_COUNTER_SOFTWARE                 (Os_CounterDriverType)0u
#define OS_COUNTER_HARDWARE                 (Os_CounterDriverType)1u

/* Schedule table initialization */
#define OS_SCHEDULE_TABLE_TICK_DEFAULT      (Os_ScheduleTableTickType)0

#define OS_NULL                             (void*)(0)

#define E_OS_OK                             0



#define OS_TRUE                                        (Os_BoolType)0xffu
#define OS_FALSE                                       (Os_BoolType)0x00u

#define OS_ALARMACTION_NONE                            (Os_ActionType)0u
#define OS_ALARMACTION_ACTIVATETASK                    (Os_ActionType)1u
#define OS_ALARMACTION_SETEVENT                        (Os_ActionType)2u
#define OS_ALARMACTION_CALLBACK                        (Os_ActionType)3u
#define OS_ALARMACTION_FLAG                            (Os_ActionType)4u
#define OS_ALARMACION_INCREMENTCOUNTER                 (Os_ActionType)5u

#define OS_RESOURCE_STANDARD                           (Os_ResourcePropertyType)0x01u
#define OS_RESOURCE_LINKED                             (Os_ResourcePropertyType)0x02u
#define OS_RESOURCE_INTERNAL                           (Os_ResourcePropertyType)0x03u

#define OS_NOT_CONNECTED_TO_ANY_APPLICATION            (ApplicationType)0xffffu
#define OS_NOT_LOCKED_BY_ANY_CORE                      (CoreIdType)0xffffu
#define OS_NOT_LOCKED_BY_ANY_TASKS                     (TaskType)0xffffu
#define OS_NOT_LOCKED_BY_ANY_SPINLOCK                  (SpinlockIdType)0xffffu
#define OS_UNKNOWN_CORE_ID                             (CoreIdType)0xffffu
#define OS_UNKNOWN_APPLICATION_ID                      (ApplicationType)0xffffu
#define OS_INVALID_SPINLOCK                            (SpinlockIdType)0xffffu
#define OS_RESTART_TASK_NOT_DEFINED                    (TaskType)0xffffu
#define OS_READYTASKS_INVALID_IDX                      0xffffffffu


#define OS_INVALID_TASKID                              (TaskType)-1

#define OS_NO_TASK_IS_SLEEPING                         (TaskType)IDLE_LOOP
#define OS_VECU_IS_NOT_SLEEPING                        (uint8)0x0u
#define OS_VECU_IS_SLEEPING                            (uint8)0x1u



/*------------------------------------------------------------------------------------------------*\
  TYPEDEFS
\*------------------------------------------------------------------------------------------------*/

typedef uint8               Os_CounterDriverType;
typedef uint8               Os_BoolType;
typedef uint8               Os_ScheduleTableSyncStrategy;
typedef TickType            Os_ScheduleTableTickType;
typedef uint16              Os_ActionType;
typedef uint16              Os_ApplicationLevelType;
typedef uint16              Os_IntDescriptorType;
typedef Os_IntDescriptorType*  Os_IntDescriptorRefType;
typedef sint32              Os_PrioType;
typedef uint16              Os_ResourcePropertyType;
typedef uint16              Os_ServiceIdType;
typedef uint16              Os_SymbolicName;
typedef void*               Os_AccessNameRef;


typedef struct
{
    Os_BoolType             IsStarted;
    TickType                SimTime;
    AppModeType             AppMode;
    TaskType                ActTaskId;
    ISRType                 ActIsrId;
    CoreIdType              ActCoreIdForHookFunction;
    ApplicationType         ActApplicationIdForHookFunction;
    Os_ApplicationLevelType    ApplicationLevel;
    /* possible: TASK_LEVEL, OS_LEVEL, ISR_LEVEL */

    Os_ApplicationLevelType    CallLevel;
    Os_ApplicationLevelType    PreviousCallLevel;
    /* possible: TASK_LEVEL,         OS_LEVEL,           ISR_LEVEL,
                 STARTUPHOOK_LEVEL,  SHUTDOWNHOOK_LEVEL, ERRORHOOK_LEVEL,
                 PRETASKHOOK_LEVEL,  POSTTASKHOOK_LEVEL                   */

    TaskType                LastInterruptedTask;
    uint32                  ActivationOrder;

#ifdef ERROR_HOOK_ENABLED
    uint16      ErrorHookCnt;
#endif

    uint16                  IsrDisableCnt;
    Os_IntDescriptorType    IntDescriptor;
    TaskType                ActiveBasicRateTask;
    Os_BoolType             ShutdownOsCalled;
    TaskType                SleepingTaskId;
    Os_BoolType             IsSleeping;
    Os_BoolType             IsSleepingLast;
    Os_BoolType             InfLoopOfSleepingTaskMustBeCalled;
} Os_KernelParamType;

typedef struct
{
    const uint32                        stackSize;
    Os_PrioType                         ActPrio;
    TaskStateType                       State;
    const Os_BoolType                   ExtendedTask;
    EventMaskType                       Events;
    EventMaskType                       EventsWaiting;
    uint32                              ActivationOrder;
    TaskType                            OsInterruptedTask;
    uint16                              ResourceLevelCnt;
    uint16                              NumberOfAct;
    Os_BoolType                         InAppMode[NUMBER_OF_APPMODES]; /* not used */
    /* AUTOSTART == false ==> no 'true' in this array */
#if (NUMBER_OF_RESOURCES > 0)
    Os_BoolType                         InternalRes[NUMBER_OF_RESOURCES]; /* not used */
#endif /* (NUMBER_OF_RESOURCES > 0) */
    const Os_PrioType                   InitialPrio;
    const Os_BoolType                   Preemptive;
    const uint16                        MaxAct;
    Os_BoolType                         wasWaiting; /* not used */
    void                                (*pTaskFcn)(void);
    const uint16                        PreCallRasterId;
    const uint16                        PostCallRasterId;
    uint32 *                            pOverrun;
    Os_BoolType                         Terminated;
    const char *                        TaskName;
    const TaskType                      TaskId;
    uint32                              ReadyTasksHeapIdx;
    const ApplicationType               ConnectApplicationId;
#if (NUMBER_OF_OS_APPLICATIONS > 0)
    const Os_BoolType                   ApplicationHasAccess[NUMBER_OF_OS_APPLICATIONS];
#endif /* (NUMBER_OF_OS_APPLICATIONS > 0) */
} Os_TaskControlBlockType;

typedef Os_TaskControlBlockType* Os_TaskControlBlockRefType;


typedef const struct
{
    const TickType                      offset;

    const TickType                      maxAdvance;
    const TickType                      maxRetard;

    const EventMaskRefType              eventTbl;
    const Os_TaskControlBlockRefType*   taskEventTbl;
    const Os_TaskControlBlockRefType*   taskTbl;

    const uint32                        numberOfEvents;
    const uint32                        numberOfTasks;
} Os_ExpiryPointControlBlockType;

typedef Os_ExpiryPointControlBlockType* Os_ExpiryPointControlBlockTypeRef;

typedef const struct
{
    const Os_ExpiryPointControlBlockTypeRef    expiryPointTbl;
    const CounterType                   counterId;
    const AppModeType                   appModeId;

    const uint32                        duration;
    const uint32                        numberOfExpiryPoints;

#ifdef OS_SCHEDULETABLE_EXPLICIT_SYNCHRONISATION
    const uint32                        precision;
#endif

    const Os_BoolType                   repeating;
    const Os_ScheduleTableSyncStrategy  syncStrategy;
    const ApplicationType               ConnectApplicationId;
#if (NUMBER_OF_OS_APPLICATIONS > 0)
    const Os_BoolType             ApplicationHasAccess[NUMBER_OF_OS_APPLICATIONS];
#endif /* (NUMBER_OF_OS_APPLICATIONS > 0) */
} Os_ScheduleTableControlBlockType;

typedef Os_ScheduleTableControlBlockType* Os_ScheduleTableControlBlockTypeRef;


typedef struct Os_tagScheduleTableControlBlockExecType
{
    Os_ScheduleTableTickType      ticks;
    TickType                      match;

    struct Os_tagScheduleTableControlBlockExecType* nextSchedTbl;
    Os_ScheduleTableControlBlockTypeRef      schedTblConst;

#ifdef OS_SCHEDULETABLE_EXPLICIT_SYNCHRONISATION
    sint32                        deviation;
    Os_BoolType                   actEpCanMoved;
#endif

    ScheduleTableStatusType    status;
    /* possible:  SCHEDULETABLEMODE_STOPPED, SCHEDULETABLEMODE_NEXT, SCHEDULETABLEMODE_WAITING, SCHEDULETABLEMODE_RUNNING, SCHEDULETABLEMODE_RUNNING_AND_SYNCHRONOUS */
    uint16                        expiryPointNext;
} Os_ScheduleTableControlBlockExecType;

typedef Os_ScheduleTableControlBlockExecType* Os_ScheduleTableControlBlockTypeExecRef;

typedef struct
{
    TickType                      CtrValue;
    TickType                      OldSimTime;
    const TickType                MaxAllowedValue;
    const TickType                MinCycle;
    const TickType                TicksPerBase;
#if NUMBER_OF_ALARMS > 0
    const Os_BoolType             ConnectedAlarms[NUMBER_OF_ALARMS_FOR_CTRCB];
#endif /* NUMBER_OF_ALARMS > 0 */
    Os_BoolType                   IsInitialized;
    const Os_ScheduleTableControlBlockTypeExecRef SchedTblControlBlock;
#if NUMBER_OF_SCHEDULE_TABLES > 0
    const Os_BoolType             ConnectedScheduleTables[NUMBER_OF_SCHEDULE_TABLES];
#endif /* NUMBER_OF_SCHEDULE_TABLES > 0 */
    const Os_CounterDriverType    counterType;
    const uint32                  MultipleOfSimTimeBase;
#if (NUMBER_OF_OS_APPLICATIONS > 0)
    const Os_BoolType             ApplicationHasAccess[NUMBER_OF_OS_APPLICATIONS];
#endif /* (NUMBER_OF_OS_APPLICATIONS > 0) */
} Os_CounterControlBlockType;

typedef struct
{
    const CounterType             Os_Counter;
    const TickType                GeneralOffset;
    Os_BoolType                   IsStarted;
    TickType                      NextOcourence;
    TickType                      Cycle;
    const Os_ActionType           Action;
    const TaskType                Os_Task;
    const EventMaskType           Os_Event;
    const CounterType             IncCounter;
    void                          (*pCallbackFcn)(void);
    const ApplicationType         ConnectApplicationId;
#if (NUMBER_OF_OS_APPLICATIONS > 0)
    const Os_BoolType             ApplicationHasAccess[NUMBER_OF_OS_APPLICATIONS];
#endif /* (NUMBER_OF_OS_APPLICATIONS > 0) */
} Os_AlarmControlBlockType;

typedef struct
{
    const Os_PrioType             Prio;
    Os_PrioType                   ReplacedPrio;
    const Os_ResourcePropertyType Property;
    Os_BoolType                   Occupied;
    TaskType                      OwnerTask;
    uint16                        ResourceLevelCnt;
#if (NUMBER_OF_OS_APPLICATIONS > 0)
    const Os_BoolType             ApplicationHasAccess[NUMBER_OF_OS_APPLICATIONS];
#endif /* (NUMBER_OF_OS_APPLICATIONS > 0) */
} Os_ResourceControlBlockType;

typedef struct
{
   void                  (*pIsrFcn)(void);
} Os_IsrControlBlockType;

#if (NUMBER_OF_CORES > 0)
typedef struct
{
    const CoreIdType              CoreId;
    const Os_BoolType             IsMasterCore;
    Os_BoolType                   IsStarted;
    const Os_BoolType             IsNonAutosarCore;
    Os_BoolType                   GblHookIsCalled;
#if (NUMBER_OF_SPINLOCKS > 0)
    SpinlockIdType                OccupiedSpinlockStack[NUMBER_OF_SPINLOCKS];
#else
    SpinlockIdType                OccupiedSpinlockStack[1];
#endif /* (NUMBER_OF_SPINLOCKS > 0) */
    SpinlockIdType                actSpinlockIdx;
} Os_CoreControlBlockType;
#endif /* (NUMBER_OF_CORES > 0) */

#if (NUMBER_OF_SPINLOCKS > 0)
typedef struct
{
    const SpinlockIdType          SpinlockId;
    const SpinlockIdType          SuccessorId;
    const uint8                   LockMethode;
    CoreIdType                    IsLockedByCore;
    TaskType                      OccupiedByTaskId;
#if (NUMBER_OF_OS_APPLICATIONS > 0)
    const Os_BoolType             ApplicationHasAccess[NUMBER_OF_OS_APPLICATIONS];
#endif /* (NUMBER_OF_OS_APPLICATIONS > 0) */
} Os_SpinlockBlockType;
#endif /* (NUMBER_OF_SPINLOCKS > 0) */

#if (NUMBER_OF_OS_APPLICATIONS > 0)
typedef struct
{
    const Os_BoolType         IsTrusted;
    const CoreIdType          CoreId;
    const TaskType            RestartTaskId;
#if NUMBER_OF_ALARMS > 0
    const Os_BoolType         ConnectedAlarms[NUMBER_OF_ALARMS_FOR_CTRCB];
#endif /* NUMBER_OF_ALARMS > 0 */
#if NUMBER_OF_COUNTERS > 0
    const Os_BoolType         ConnectedCounters[NUMBER_OF_COUNTERS];
#endif /* NUMBER_OF_COUNTERS > 0 */
#if NUMBER_OF_TASKS > 0
    const Os_BoolType         ConnectedTasks[NUMBER_OF_TASKS];
#endif /* NUMBER_OF_TASKS > 0 */
#if NUMBER_OF_SCHEDULE_TABLES > 0
    const Os_BoolType         ConnectedSchedTables[NUMBER_OF_SCHEDULE_TABLES];
#endif /* NUMBER_OF_SCHEDULE_TABLES > 0 */
    void                     (*pStartupHook)(void);
    void                     (*pShutdownHook)(StatusType Error);
    void                     (*pErrorHook)(StatusType Error);
    Os_BoolType              IsStarted;
    ApplicationStateType     State;
    /* possible: APPLICATION_ACCESSIBLE, APPLICATION_RESTARTING, APPLICATION_TERMINATED  */
} Os_ApplicationControlBlockType;
#endif /* (NUMBER_OF_OS_APPLICATIONS > 0) */

#endif /* OS_INTERNALTYPES_H */

/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/
