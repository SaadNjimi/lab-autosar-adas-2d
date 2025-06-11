/**************************************************************************************************\
 *** Os_Kernel.h
 ***
 *** Definitions for operating system core functions and data addresses
 ***
 *** Copyright (c) 2009 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef OS_KERNEL_H
#define OS_KERNEL_H

/*------------------------------------------------------------------------------------------------*\
  DEFINES (OPT)
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/
#include <setjmp.h>     /* setjmp, longjmp */

#include "Os_Types.h"
#include "Os.h"

#include "Platform_Types.h"
#include "Os_Cfg.h"
#include "Os_InternalTypes.h"
#include "Os_ErrorHandler.h"
#include "Os_Application.h"
#include "Os_EcuCore.h"
#include "Os_Counter.h"
#include "Os_ResInt.h"

/*------------------------------------------------------------------------------------------------*\
  DEFINES
\*------------------------------------------------------------------------------------------------*/

#ifndef OSDEFAULTAPPMODE
#error Configuration did not provide a default application mode
#endif

/*
Note:
setjmp and longjmp crashes when used in a 64bit windows environment and GCC
Using __builtin_setjump avoids the problem
*/
#if defined( __GCC__) && defined(__x86_64__) && defined(__USING_SJLJ_EXCEPTIONS__) && defined(NDEBUG)
   #define OS_SETJMP(jmpBuf)  __builtin_setjmp(jmpBuf)
   #define OS_LONGJMP(jmpBuf, value) __builtin_longjmp(jmpBuf, value)
#else
   #define OS_SETJMP(jmpBuf)  setjmp(jmpBuf)
   #define OS_LONGJMP(jmpBuf, value)  longjmp(jmpBuf, value)
#endif


#if defined(__GNUC__) || defined(__clang__)
#define OS_ATTR_FORCEINLINE inline __attribute__((always_inline))
#elif defined(_MSC_VER)
#define OS_ATTR_FORCEINLINE __forceinline
#endif


#ifdef STARTUP_HOOK_ENABLED
    #define STARTUPHOOK()                           \
    {                                               \
       g_Kernel.CallLevel = STARTUPHOOK_LEVEL;      \
       StartupHook();                               \
       g_Kernel.CallLevel = OS_LEVEL;               \
    }
#else
    #define STARTUPHOOK()
#endif

#ifdef SHUTDOWN_HOOK_ENABLED
    #define SHUTDOWNHOOK(Error)                      \
    {                                                \
       g_Kernel.PreviousCallLevel = g_Kernel.CallLevel; \
       g_Kernel.CallLevel = SHUTDOWNHOOK_LEVEL;      \
       g_ErrorInfo.Error = Error;                    \
       ShutdownHook(Error);                          \
       g_Kernel.CallLevel = g_Kernel.PreviousCallLevel; \
    }
#else
    #define SHUTDOWNHOOK(Error)
#endif

#ifdef PRETASK_HOOK_ENABLED
    #define PRETASKHOOK()                                  \
    {                                                      \
       Os_PreTaskHook();                                   \
       if(g_Kernel.ActTaskId != IDLE_LOOP)                 \
       {                                                   \
          g_Kernel.PreviousCallLevel = g_Kernel.CallLevel; \
          g_Kernel.CallLevel = PRETASKHOOK_LEVEL;          \
          PreTaskHook();                                   \
          g_Kernel.CallLevel = g_Kernel.PreviousCallLevel; \
       }                                                   \
    }
#else
    #define PRETASKHOOK()  Os_PreTaskHook()
#endif

#ifdef POSTTASK_HOOK_ENABLED
  #if (NUMBER_OF_CORES > 0)
   #define POSTTASKHOOK()                                    \
    {                                                        \
      Os_PostTaskHook();                                     \
      if( g_Kernel.ActTaskId != IDLE_LOOP )                  \
      {                                                      \
        /* [SWS_Os_00071] */                                 \
        if( g_CoreCb[ GetCoreID() ].IsStarted == OS_TRUE) \
        {                                                    \
          g_Kernel.PreviousCallLevel = g_Kernel.CallLevel;   \
          g_Kernel.CallLevel = POSTTASKHOOK_LEVEL;           \
          PostTaskHook();                                    \
          g_Kernel.CallLevel = g_Kernel.PreviousCallLevel;   \
        }                                                    \
      }                                                      \
    }
  #else
    #define POSTTASKHOOK()                               \
    {                                                    \
      Os_PostTaskHook();                                 \
      if( g_Kernel.ActTaskId != IDLE_LOOP )              \
      {                                                  \
        g_Kernel.PreviousCallLevel = g_Kernel.CallLevel; \
        g_Kernel.CallLevel = POSTTASKHOOK_LEVEL;         \
        PostTaskHook();                                  \
        g_Kernel.CallLevel = g_Kernel.PreviousCallLevel; \
      }                                                  \
    }
  #endif /* (NUMBER_OF_CORES > 0) */
#else
    #define POSTTASKHOOK()   Os_PostTaskHook()
#endif

#ifdef ERROR_HOOK_ENABLED
    #define ERRORHOOK(Error)                           \
    {                                                  \
      Os_ErrorHook(Error);                             \
                                                       \
      g_Kernel.PreviousCallLevel = g_Kernel.CallLevel; \
      g_Kernel.CallLevel = ERRORHOOK_LEVEL;            \
      ErrorHook(Error);                                \
      g_Kernel.CallLevel = g_Kernel.PreviousCallLevel; \
                                                       \
      Os_ApplicationErrorHookFilter(Error);            \
    }
#else
    #define ERRORHOOK(Error)                           \
    {                                                  \
      Os_ErrorHook(Error);                             \
      Os_ApplicationErrorHookFilter(Error);            \
    }
#endif

/*------------------------------------------------------------------------------------------------*\
  VARIABLE DECLARATIONS
\*------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern uint32 g_NumberOfTaskOverruns[NUMBER_OF_TASKS];
extern uint32 g_NumberOfTaskInvocations[NUMBER_OF_TASKS];

extern jmp_buf returnToTask[NUMBER_OF_TASKS];

#ifdef __cplusplus
}
#endif /* __cplusplus */

/*------------------------------------------------------------------------------------------------*\
  FORWARD DECLARATIONS
\*------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern Os_KernelParamType          g_Kernel;
extern Os_TaskControlBlockType     g_TaskCb[NUMBER_OF_TASKS];
extern Os_PrioType                 g_TaskPriorities[NUMBER_OF_TASKS];

#if NUMBER_OF_RESOURCES >= 1
extern Os_ResourceControlBlockType g_ResCb[NUMBER_OF_RESOURCES];
#endif

#if NUMBER_OF_ISRS >= 1
extern Os_IsrControlBlockType      g_IsrCb[NUMBER_OF_ISRS];
#endif

#if NUMBER_OF_COUNTERS >= 1
extern Os_CounterControlBlockType  g_CtrCb[NUMBER_OF_COUNTERS];
#endif

#if NUMBER_OF_ALARMS >= 1
extern Os_AlarmControlBlockType    g_AlarmCb[NUMBER_OF_ALARMS];
#endif

#if NUMBER_OF_SCHEDULE_TABLES >= 1
#if NUMBER_OF_ALL_SCHED_TBL_EVENTS > 0
extern EventMaskType                        g_eventTbl[NUMBER_OF_ALL_SCHED_TBL_EVENTS];
extern Os_TaskControlBlockRefType           g_taskEventTbl[NUMBER_OF_ALL_SCHED_TBL_EVENTS];
#endif
#if NUMBER_OF_ALL_SCHED_TBL_TASKS > 0
extern Os_TaskControlBlockRefType           g_taskTbl[NUMBER_OF_ALL_SCHED_TBL_TASKS];
#endif
#if NUMBER_OF_EXPIRY_POINTS > 0
extern Os_ExpiryPointControlBlockType       g_ExpPnts[NUMBER_OF_EXPIRY_POINTS];
#endif
extern Os_ScheduleTableControlBlockType     g_SchedTbl[NUMBER_OF_SCHEDULE_TABLES];
extern Os_ScheduleTableControlBlockExecType g_SchedTblExec[NUMBER_OF_SCHEDULE_TABLES];
#endif

#if NUMBER_OF_SPINLOCKS > 0
extern Os_SpinlockBlockType g_SpinlockCb[NUMBER_OF_SPINLOCKS];
#endif

#if NUMBER_OF_CORES > 0
extern Os_CoreControlBlockType g_CoreCb[NUMBER_OF_CORES];
#endif

#if NUMBER_OF_OS_APPLICATIONS > 0
extern Os_ApplicationControlBlockType g_ApplicationCb[NUMBER_OF_OS_APPLICATIONS];
#endif

extern EventMaskType                     g_Os_RecentOsEventMaskResult;

#ifdef __cplusplus
}
#endif /* __cplusplus */

/*------------------------------------------------------------------------------------------------*\
  FUNCTION PROTOTYPES
\*------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


extern TaskType Os_FindHighPriorTask(void);
extern void Os_ScheduleInternal(Os_BoolType ForceScheduling, Os_BoolType Termination);
extern void Os_SysScheduleSimple(void);
extern void Os_SetTaskReady(TaskType id);
extern void Os_ChangeTaskStateByPtr(Os_TaskControlBlockType* task, TaskStateType state);

extern void Os_ErrorHook(StatusType Error);
extern void Os_EnterIsrHook(void);
extern void Os_LeaveIsrHook(void);

extern void Os_ReadyTasks_Add(Os_TaskControlBlockType* task);
extern void Os_ReadyTasks_Remove(Os_TaskControlBlockType* task);
extern void Os_ReadyTasks_Update(Os_TaskControlBlockType* task);

static OS_ATTR_FORCEINLINE void Os_ChangeTaskState(TaskType taskId, TaskStateType state)
{
    Os_ChangeTaskStateByPtr(&g_TaskCb[taskId], state);
}

static inline void Os_SetTaskActPrio(TaskType taskId, Os_PrioType prio)
{
    g_TaskPriorities[taskId] = prio;
    g_TaskCb[taskId].ActPrio = prio;

    if (g_TaskCb[taskId].ReadyTasksHeapIdx != OS_READYTASKS_INVALID_IDX)
    {
        Os_ReadyTasks_Update(&g_TaskCb[taskId]);
    }
}

/* Returns true if 'first' has a higher scheduling priority than 'second'. */
static OS_ATTR_FORCEINLINE boolean Os_FirstIsHigherPrio(Os_PrioType first, Os_PrioType second)
{
#if OS_PRIO_ORDER == OS_SYS_PRIO_ORDERING_DESCENDING
    return (first < second);
#else
    return (first > second);
#endif
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OS_KERNEL_H */

/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/
