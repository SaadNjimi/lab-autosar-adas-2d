/**************************************************************************************************\
 *** Os_Native.c
 ***
 *** The x86-32 implementaion of the OS native interface.
 ***
 *** Copyright (c) 2008 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef OS_NATIVE_C
#define OS_NATIVE_C

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/

#include "Os_Native.h"
#include "Os_Cfg.h"
#include "Os_InternalTypes.h"

#include "Os_SimulationCore.h"

/*------------------------------------------------------------------------------------------------*\
  FORWARD DECLARATIONS
\*------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

#include "Os_TaskHandlingViaCallbacks.h"


#ifdef __cplusplus
}
#endif

/*------------------------------------------------------------------------------------------------*\
  VARIABLE DEFINITIONS
\*------------------------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------------------------*\
  FUNCTION DEFINITIONS
\*------------------------------------------------------------------------------------------------*/

/**************************************************************************************************\
 *** FUNCTION:
 ***      Os_InitTaskContextSwitch
 *** 
 *** DESCRIPTION: This function initialzes the operation system.
 ***
 *** PARAMETERS:
 ***     Type             Name     Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***
 *** 
 *** RETURNS:
 ***
\**************************************************************************************************/
void Os_InitTaskContextSwitch(void)
{
    (void)Os_ThreadInit();
}

/**************************************************************************************************\
 *** FUNCTION:
 ***      Os_DeInitTaskContextSwitch
 *** 
 *** DESCRIPTION: This function releases the operation system.
 ***
 *** PARAMETERS:
 ***     Type             Name     Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***
 *** 
 *** RETURNS:
 ***
\**************************************************************************************************/
void Os_DeInitTaskContextSwitch(void)
{
    Os_ThreadDeInit();
}

/**************************************************************************************************\
 *** FUNCTION:
 ***      Os_TaskCreate
 *** 
 *** DESCRIPTION: This function creates and initialize the stacks of a new task.
 ***
 *** PARAMETERS:
 ***     Type                Name       Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     TaskType            id          Task ID
 ***     Os_TaskControlBlockType         Pointer to task control block
 *** 
 *** RETURNS:
 ***
\**************************************************************************************************/
void Os_TaskCreate(TaskType id, Os_TaskControlBlockType* pTaskControlBlock)
{
    Os_ThreadTaskCreate(id, pTaskControlBlock);
}




/**************************************************************************************************\
 *** FUNCTION:
 ***      Os_TaskRestart
 *** 
 *** DESCRIPTION: This function restarting and initialize the stacks of a task.
 ***
 *** PARAMETERS:
 ***     Type                Name       Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     TaskType            id          Task ID
 ***     Os_TaskControlBlockType         Pointer to task control block
 *** 
 *** RETURNS:
 ***
\**************************************************************************************************/
void Os_TaskRestart(TaskType id)
{
    Os_ThreadTaskRestart(id);
}




/**************************************************************************************************\
 *** FUNCTION:
 ***      Os_TaskDelete
 *** 
 *** DESCRIPTION: This function delete the stacks of a task.
 ***
 *** PARAMETERS:
 ***     Type                Name       Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     TaskType            id          Task ID
 *** 
 *** RETURNS:
 ***
\**************************************************************************************************/
void Os_TaskDelete(TaskType id)
{
    Os_ThreadTaskDelete(id);
}



/**************************************************************************************************\
 *** FUNCTION:
 ***      Os_TaskStart
 *** 
 *** DESCRIPTION: Starts the multitasking system. It is called from an Timer-ISR.
 ***              Note that the TIMER-ISR is only a function call in sil mode. Therefore the function
 ***              is different to a task start function for real interrupts.
 ***
 *** PARAMETERS:
 ***     Type             Name     Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     TaskType          id       Task ID
 *** 
 *** RETURNS:
 ***
\**************************************************************************************************/
void Os_TaskStart(TaskType id)
{
    Os_ThreadTaskStart(id);
}

/**************************************************************************************************\
 *** FUNCTION:
 ***      Os_Leave
 *** 
 *** DESCRIPTION: Return to the simulated ISR and restore the "main" stack. This is the stack 
 ***              of caller of the Timer-ISR. This function exists only in sil mode.
 ***
 *** PARAMETERS:
 ***     Type             Name     Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     TaskType          id       Task ID
 *** 
 *** RETURNS:
 ***
\**************************************************************************************************/
void Os_Leave(TaskType id)
{
    Os_ThreadLeave(id);
}

/**************************************************************************************************\
 *** FUNCTION:
 ***      Os_TaskSwitch
 *** 
 *** DESCRIPTION: This function provides the context switch. It switches from the actual task
 ***              to the task with id 'id'.
 ***
 *** PARAMETERS:
 ***     Type             Name     Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     TaskType          id       Task ID
 *** 
 *** RETURNS:
 ***
\**************************************************************************************************/
boolean Os_TaskSwitch(TaskType id)
{
    return Os_ThreadTaskSwitch(id);
}



/**************************************************************************************************\
 *** FUNCTION:
 ***     OsTerminateSimulation
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
void OsTerminateSimulation(void)
{
    /* Sends the terminate simulation request. */
    Os_SimlationTerminateSimulation();
}




#endif /* OS_NATIVE_C */

/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/
