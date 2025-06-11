/**************************************************************************************************\
 *** Os_Native.h
 ***
 *** This file contains the native interface.
 ***
 *** Copyright (c) 2008 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef OS_NATIVE_H
#define OS_NATIVE_H

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/

#include "Os_Types.h"
#include "Os_InternalTypes.h"

/*------------------------------------------------------------------------------------------------*\
  FUNCTION PROTOTYPES
\*------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

extern void     Os_InitTaskContextSwitch(void);
extern void     Os_DeInitTaskContextSwitch(void);
extern void     Os_TaskStart(TaskType id);
extern boolean  Os_TaskSwitch(TaskType id);
extern void     Os_TaskCreate(TaskType id, Os_TaskControlBlockType* pTaskControlBlock);
extern void     Os_TaskRestart(TaskType id);
extern void     Os_TaskDelete(TaskType id);
extern void     Os_Leave(TaskType id);

#ifdef __cplusplus
}
#endif

#endif /* OS_NATIVE_H */

/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/
