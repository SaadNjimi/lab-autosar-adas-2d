/**************************************************************************************************\
 *** Os_SimulationCore.h
 ***
 *** This file contains the os simulation core interface.
 ***
 *** Copyright (c) 2017 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef OS_SIMULATION_CORE_H
#define OS_SIMULATION_CORE_H

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/
#include "Os_Types.h"
#include "Os.h"

#include "Os_ErrorHandler.h"
#include "Os_Frame.h"

/*------------------------------------------------------------------------------------------------*\
  DEFINES
\*------------------------------------------------------------------------------------------------*/

#if defined(__GNUC__) || defined(__clang__)
#define OS_ATTR_FORMAT_PRINTF(fmtidx, argsidx) __attribute__((format(printf, fmtidx, argsidx)))
#else
#define OS_ATTR_FORMAT_PRINTF(fmtidx, argsidx)
#endif

/*------------------------------------------------------------------------------------------------*\
  TYPEDEFS
\*------------------------------------------------------------------------------------------------*/

typedef const char *                Os_ServiceServiceDescType;
typedef const char *                Os_ErrorLevelDescType;

/*------------------------------------------------------------------------------------------------*\
  FUNCTION PROTOTYPES
\*------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

extern void Os_SetInterruptLevel(void);
extern void Os_ResetInterruptLevel(void);
extern void Os_ScheduleInterruptActivatedTask(void);

extern void Os_SimulationCoreOnOsStarted(void);
extern void Os_SimulationCoreOnOsShutdown(void);
extern void Os_SimulationCoreOnOsWakeup(void);

extern void Os_SimulationCoreExpSvcPreTaskCall(const uint16 RasterId);
extern void Os_SimulationCoreExpSvcPostTaskCall(const uint16 RasterId, const boolean ActiveBasicRateTask);

void Os_SimulationCoreErrorHook(StatusType error, Os_ServiceServiceDescType serviceIdentifier, Os_ErrorLevelDescType errorType);

#ifdef OS_DBGMSG_ENABLED
extern void Os_SimulationCoreLogDbgMessage(const char* FunctionName, const char* pFormat, ...) OS_ATTR_FORMAT_PRINTF(2, 3);
#endif /* OS_DBGMSG_ENABLED */
extern void Os_SimulationCoreShowErrorMessage(const char * pFormat,  ...) OS_ATTR_FORMAT_PRINTF(1, 2);
extern void Os_SimulationCoreShowFatalErrorMessage(const char * pFormat,  ...) OS_ATTR_FORMAT_PRINTF(1, 2);
extern void Os_SimulationCoreShowWarningMessage(const char * pFormat,  ...)  OS_ATTR_FORMAT_PRINTF(1, 2);

extern void Os_SimlationTerminateSimulation(void);

extern void Os_DisableAllLowerPrioTasks(void);
extern void Os_WaitForTaskResumption(void);
extern void Os_ResumeResumableTask(void);

#ifdef __cplusplus
}
#endif

#endif /* OS_SIMULATION_CORE_H */

/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/
