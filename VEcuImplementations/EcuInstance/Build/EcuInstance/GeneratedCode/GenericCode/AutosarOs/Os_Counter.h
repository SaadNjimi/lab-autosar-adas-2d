/**************************************************************************************************\
 *** Os_Counter.h
 ***
 *** OSEK API function definition for OS Counters.
 ***
 *** Copyright (c) 2014 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef OS_COUNTER_H
#define OS_COUNTER_H

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/
#include "Os_Types.h"
#include "Os.h"


/*------------------------------------------------------------------------------------------------*\
  FUNCTION DECLARATIONS
\*------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

extern StatusType Os_InitCounter(CounterType CounterID, TickType Ticks);
extern StatusType Os_CounterTriggerSolely(CounterType CounterID);
extern StatusType Os_CounterTrigger(CounterType CounterID);

#ifdef __cplusplus
}
#endif

#endif /* OS_COUNTER_H */

/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/
