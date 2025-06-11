/**************************************************************************************************\
 *** Os_TaskHandlingViaCallbacks.c
 ***
 *** The VEOS implementaion of task handling via callbacks.
 ***
 *** Copyright (c) 2017 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/


#ifndef OS_TASKHANDLINGVIACALLBACKS_C
#define OS_TASKHANDLINGVIACALLBACKS_C

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/
#include "Os_TaskHandlingViaCallbacks.h"
#include "Os_Kernel.h"
#include "Os_Types.h"
#include "Os_ErrorHandler.h"
#include "Os_SimulationCore.h"


#include "VEOS.h"

/*------------------------------------------------------------------------------------------------*\
  DEFINES
\*------------------------------------------------------------------------------------------------*/
#define THREAD_ID_NOT_DEFINED  -1

/* one extra thread for Os */
#define NUMBER_OF_MANAGED_THREADS (NUMBER_OF_TASKS+1)

#define OS_NULL_POINTER   0


/*------------------------------------------------------------------------------------------------*\
  TYPEDEFS
\*------------------------------------------------------------------------------------------------*/
typedef struct CallBackData_tag 
{
    VEOS_OsTskApi_TaskIdType  restartTaskId;
    VEOS_OsTskApi_TaskIdType  callingThreadId;
    boolean callBackPending;

} CallBackData;


/*------------------------------------------------------------------------------------------------*\
  VARIABLE DEFINITIONS
\*------------------------------------------------------------------------------------------------*/
static const TaskType           OS_MAIN_THREAD = (NUMBER_OF_MANAGED_THREADS-1);
static VEOS_OsTskApi_TaskIdType Os_ThreadIds[NUMBER_OF_MANAGED_THREADS];
static CallBackData             callBackData;


/*------------------------------------------------------------------------------------------------*\
  FORWARD DECLARATIONS
\*------------------------------------------------------------------------------------------------*/
extern void OsTerminateSimulation(void);


/*------------------------------------------------------------------------------------------------*\
  FUNCTION DEFINITIONS
\*------------------------------------------------------------------------------------------------*/

/**************************************************************************************************\
 *** FUNCTION:
 ***      Os_ThreadInit
 *** 
 *** DESCRIPTION: This function initialzes the OS_MAIN_THREAD.
 ***
 *** PARAMETERS:
 ***     Type             Name     Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***
 *** 
 *** RETURNS:
 ***
\**************************************************************************************************/
uint16 Os_ThreadInit(void)
{
    TaskType Tid;

    callBackData.callBackPending = FALSE;
    VEOS_OsTskApi_GetCurrentThreadId(&Os_ThreadIds[OS_MAIN_THREAD]);
 
#ifdef OS_DBGMSG_ENABLED
    Os_SimulationCoreLogDbgMessage(__FUNCTION__, "Os_ThreadInit: ThreadId: %d", Os_ThreadIds[OS_MAIN_THREAD]);
#endif /* OS_DBGMSG_ENABLED */

    for(Tid=0;Tid<NUMBER_OF_TASKS;Tid++)
    {
       Os_ThreadIds[Tid] = THREAD_ID_NOT_DEFINED;
    }
    
    return E_OK;
}


/**************************************************************************************************\
 *** FUNCTION:
 ***      Os_ThreadDeInit
 *** 
 *** DESCRIPTION: This function deinitialzes the OS_MAIN_THREAD.
 ***
 *** PARAMETERS:
 ***     Type             Name     Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***
 *** 
 *** RETURNS:
 ***
\**************************************************************************************************/
void Os_ThreadDeInit(void)
{
    VEOS_OsTskApi_TaskIdType threadId;
    VEOS_OsTskApi_GetCurrentThreadId(&threadId);
    if ( threadId == Os_ThreadIds[OS_MAIN_THREAD] )
    {
        Os_ThreadIds[OS_MAIN_THREAD] = THREAD_ID_NOT_DEFINED;

#ifdef OS_DBGMSG_ENABLED
        Os_SimulationCoreLogDbgMessage(__FUNCTION__, "Os_ThreadDeInit: ThreadId: %d", Os_ThreadIds[OS_MAIN_THREAD]);
#endif /* OS_DBGMSG_ENABLED */

    }
}


/**************************************************************************************************\
 *** FUNCTION:
 ***      Os_ThreadTaskCreate
 *** 
 *** DESCRIPTION: This function creates a thread including the stacks of a new os task.
 ***
 *** PARAMETERS:
 ***     Type             Name     Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***
 *** 
 *** RETURNS:
 ***
\**************************************************************************************************/
void Os_ThreadTaskCreate(TaskType id, Os_TaskControlBlockType* pTaskControlBlock)
{
    if (Os_ThreadIds[id] == THREAD_ID_NOT_DEFINED)
    {
        VEOS_OsTskApi_CreateThread(pTaskControlBlock->TaskName, pTaskControlBlock->stackSize, pTaskControlBlock->pTaskFcn, &Os_ThreadIds[id]);

#ifdef OS_DBGMSG_ENABLED
        Os_SimulationCoreLogDbgMessage(__FUNCTION__, "Os_ThreadTaskCreate: TaskId: %d, ThreadId: %d, %d kbyte", id, Os_ThreadIds[id], pTaskControlBlock->stackSize/1024);
#endif /* OS_DBGMSG_ENABLED */

    }
}



/**************************************************************************************************\
 *** FUNCTION:
 ***      Os_ThreadTaskRestart
 *** 
 *** DESCRIPTION: This function creates a thread including the stacks of a new os task.
 ***
 *** PARAMETERS:
 ***     Type             Name     Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***
 *** 
 *** RETURNS:
 ***
\**************************************************************************************************/
void Os_ThreadTaskRestart(TaskType id)
{
    if (id >= NUMBER_OF_MANAGED_THREADS)
    {
        ERRORHOOK(E_OS_ID);
        return;
    }

    if (Os_ThreadIds[id] < 0)
    {
        ERRORHOOK(E_OS_ID);
        return;
    }
    
    VEOS_OsTskApi_TaskIdType threadId;
    VEOS_OsTskApi_GetCurrentThreadId(&threadId);
    if ( threadId != Os_ThreadIds[id] )
    {
        
        callBackData.restartTaskId = id;
        VEOS_OsTskApi_GetCurrentThreadId(&callBackData.callingThreadId);
        callBackData.callBackPending = TRUE;
        
        /* Deleting a thread is only in main thread possible */
        Os_ThreadTaskSwitch(OS_MAIN_THREAD);

#ifdef OS_DBGMSG_ENABLED
        Os_SimulationCoreLogDbgMessage(__FUNCTION__, "Os_ThreadTaskRestart: TaskId: %d, ThreadId: %d", id, Os_ThreadIds[id]);
#endif /* OS_DBGMSG_ENABLED */

    }
}


/**************************************************************************************************\
 *** FUNCTION:
 ***      Os_ThreadTaskDelete
 *** 
 *** DESCRIPTION: This function delete a thread.
 ***
 *** PARAMETERS:
 ***     Type             Name     Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***
 *** 
 *** RETURNS:
 ***
\**************************************************************************************************/
void Os_ThreadTaskDelete(TaskType id)
{
    VEOS_OsTskApi_TaskIdType threadId;
    VEOS_OsTskApi_GetCurrentThreadId(&threadId);
    if ( threadId == Os_ThreadIds[OS_MAIN_THREAD] )
    {
        if (id >= NUMBER_OF_MANAGED_THREADS)
        {
            ERRORHOOK(E_OS_ID);
            return;
        }

        if (Os_ThreadIds[id] < 0)
        {
            /* not delete a thread, if it is already deleted! */
            /* this is possible in multi core Os with ShutdownOS() and afterward ShutdownAllCores() */
            return;
        }

        /* The VEOS_OsTskApi_DeleteThread() function deletes all data associated with the thread. This data includes the stack, 
           a subset of the registers, and the thread data.*/
        VEOS_OsTskApi_DeleteThread(Os_ThreadIds[id]);
        Os_ThreadIds[id] = THREAD_ID_NOT_DEFINED;

#ifdef OS_DBGMSG_ENABLED
            Os_SimulationCoreLogDbgMessage(__FUNCTION__, "Os_ThreadTaskDelete: %d", id);
#endif /* OS_DBGMSG_ENABLED */
    }
}


/**************************************************************************************************\
 *** FUNCTION:
 ***      Os_ThreadTaskStart
 *** 
 *** DESCRIPTION: Starts the multitasking system. It is called from an Timer-ISR.
 ***              Note that the TIMER-ISR is only  a function call in sil mode. Therefore the function
 ***              is differnent to a task start function for real interrupts.
 ***
 *** PARAMETERS:
 ***     Type             Name     Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***
 *** 
 *** RETURNS:
 ***
\**************************************************************************************************/
void Os_ThreadTaskStart(TaskType id)
{

#ifdef OS_DBGMSG_ENABLED
    Os_SimulationCoreLogDbgMessage(__FUNCTION__, "Os_StartTimeStep: %d", id);
#endif /* OS_DBGMSG_ENABLED */

    Os_ThreadTaskSwitch(id);
    
    /* Note: This part is only execute if a user task is called ShutdownOS(E_OK) */
    if (g_Kernel.ShutdownOsCalled == OS_TRUE)
    {
        TaskType Tid;
        for(Tid=0;Tid<NUMBER_OF_TASKS;Tid++)
        {
           Os_ThreadTaskDelete(Tid);
        }
        Os_ThreadDeInit();
        
        OsTerminateSimulation();
    }
}


/**************************************************************************************************\
 *** FUNCTION:
 ***      Os_ThreadLeave
 *** 
 *** DESCRIPTION: Return to the simulated ISR and restore the "main" stack. This is the stack 
 ***              of caller of the Timer-ISR, which is assumed as the OS_MAIN_THREAD. This function
 ***              exists only in sil mode.
 ***
 *** PARAMETERS:
 ***     Type             Name     Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***
 *** 
 *** RETURNS:
 ***
\**************************************************************************************************/
void Os_ThreadLeave(TaskType id)
{
   /* to avoid the compiler warning C4100 */ 
   (void)(id);

#ifdef OS_DBGMSG_ENABLED
    Os_SimulationCoreLogDbgMessage(__FUNCTION__, "Os_LeaveTimeStep: %d", id);
#endif /* OS_DBGMSG_ENABLED */

    /* check the current id and switch only if differ */
    VEOS_OsTskApi_TaskIdType threadId;
    VEOS_OsTskApi_GetCurrentThreadId(&threadId);
    if ( threadId != Os_ThreadIds[OS_MAIN_THREAD] )
    {
        /* thread context switch */
        VEOS_OsTskApi_SwitchToThread(Os_ThreadIds[OS_MAIN_THREAD]);
    }
}


/**************************************************************************************************\
 *** FUNCTION:
 ***      Os_ThreadTaskSwitch
 *** 
 *** DESCRIPTION: This function provides the context switch. It switchs from the actual task
 ***              to the task with id 'id'.
 ***
 *** PARAMETERS:
 ***     Type             Name     Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***
 *** 
 *** RETURNS:
 ***
\**************************************************************************************************/
boolean Os_ThreadTaskSwitch(TaskType id)
{

#ifdef OS_DBGMSG_ENABLED
    Os_SimulationCoreLogDbgMessage(__FUNCTION__, "Os_TaskSwitch(Thread): %d", id);
#endif /* OS_DBGMSG_ENABLED */

    if (id >= NUMBER_OF_MANAGED_THREADS)
    {
        ERRORHOOK(E_OS_ID);
        return 0;
    }

    if (Os_ThreadIds[id] < 0)
    {
        ERRORHOOK(E_OS_ID);
        return 0;
    }

    /* check the current id and switch only if differ */
    VEOS_OsTskApi_TaskIdType threadId;
    VEOS_OsTskApi_GetCurrentThreadId(&threadId);
    if ( threadId != Os_ThreadIds[id] )
    {
        /* thread context switch */
        VEOS_OsTskApi_SwitchToThread(Os_ThreadIds[id]);
    }
    
    /* if a callback shall be executed ... */
    while ( callBackData.callBackPending == TRUE )
    {
        callBackData.callBackPending = FALSE;
        VEOS_OsTskApi_DeleteThread(Os_ThreadIds[callBackData.restartTaskId]);
        Os_ThreadIds[callBackData.restartTaskId] = THREAD_ID_NOT_DEFINED;
        VEOS_OsTskApi_CreateThread(g_TaskCb[callBackData.restartTaskId].TaskName, g_TaskCb[callBackData.restartTaskId].stackSize, g_TaskCb[callBackData.restartTaskId].pTaskFcn, &Os_ThreadIds[callBackData.restartTaskId]);

        /* ... and switch back to the invoking thread */
        VEOS_OsTskApi_SwitchToThread(callBackData.callingThreadId);
    }

   return 1;
}


/**************************************************************************************************\
 *** FUNCTION:
 ***      Os_CurrentThreadIsInternal
 *** 
 *** DESCRIPTION: This function decides if the calling thread is an internally created one.
 ***
 *** PARAMETERS:
 ***     Type             Name     Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***
 *** 
 *** RETURNS:
 ***     TRUE  if calling thread is an internally one.
 ***     FALSE if calling thread is not an internally one. It is the main thread.
\**************************************************************************************************/
boolean Os_CurrentThreadIsInternal()
{
    sint32 i=0;
    if (Os_ThreadIds[OS_MAIN_THREAD] == THREAD_ID_NOT_DEFINED)
    {
        return FALSE;  /* not started or already finished  */
    }
    else
    {
        VEOS_OsTskApi_TaskIdType threadId;
        VEOS_OsTskApi_GetCurrentThreadId(&threadId);
        for(i=0; i<OS_MAIN_THREAD; i++)
        {
            if ( threadId == Os_ThreadIds[i] )
                return TRUE;
        }
        return FALSE;
    }
}

#endif /* #ifdef OS_TASKHANDLINGVIACALLBACKS_C */
