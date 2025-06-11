/**************************************************************************************************\
 *** Os_TaskHandlingViaCallbacks.h
 ***
 *** The VEOS implementaion of task handling via callbacks.
 *** 
 *** Copyright (c) 2017 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef OS_TASKHANDLINGVIACALLBACKS_H
#define OS_TASKHANDLINGVIACALLBACKS_H


#include "Os_Cfg.h"
#include "Os_Types.h"
#include "Os_InternalTypes.h"
#include "Os_Cfg_generated.h"
#include "Platform_Types.h"


uint16 Os_ThreadInit(void);
void Os_ThreadDeInit(void);
void Os_ThreadTaskCreate(TaskType id, Os_TaskControlBlockType* pTaskControlBlock);
void Os_ThreadTaskRestart(TaskType id);
void Os_ThreadTaskDelete(TaskType id);
void Os_ThreadTaskStart(TaskType id);
void Os_ThreadLeave(TaskType id);
boolean Os_ThreadTaskSwitch(TaskType id);


#endif /* OS_TASKHANDLINGVIACALLBACKS_H */
