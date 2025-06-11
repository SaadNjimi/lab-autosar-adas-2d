/**************************************************************************************************\
 *** Os_Spinlock.h
 ***
 *** OSEK API function definition for OS spinlock handling.
 ***
 *** Copyright (c) 2018 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef OS_SPINLOCK_H
#define OS_SPINLOCK_H

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/
#include "Os_Types.h"
#include "Os.h"


/*------------------------------------------------------------------------------------------------*\
  FORWARD DECLARATIONS
\*------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

#if ((NUMBER_OF_SPINLOCKS > 0) && (NUMBER_OF_CORES > 0))
extern SpinlockIdType Os_GetCurentSpinlockId(CoreIdType currentCoreID);
extern void Os_PopSpinlockId(CoreIdType currentCoreID);
#endif

#ifdef __cplusplus
}
#endif

#endif /* OS_SPINLOCK_H */

/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/
