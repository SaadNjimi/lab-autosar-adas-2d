/**************************************************************************************************\
 *** VEcuAppFrm_Os.h
 ***
 *** Application frame declarations for OSEK and AUTOSAR OS implementations.
 ***
 *** Copyright 2019 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef VECUAPPFRM_OS_H
#define VECUAPPFRM_OS_H

/*------------------------------------------------------------------------------------------------*\
  DEFINES (OPT)
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/

#include "Platform_Types.h"

/*------------------------------------------------------------------------------------------------*\
  DEFINES
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
  PARAMETERIZED MACROS
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
  VARIABLE DECLARATIONS
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
  FUNCTION PROTOTYPES
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
  VARIABLE DEFINITIONS
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
  INLINE FUNCTIONS
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
  FUNCTION DEFINITIONS
\*------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_RttSetAsync
 ***
 *** DESCRIPTION:
 ***     Function called when the operating system must use Real-Time-Testing and runs asynchronously.
 ***
 *** PARAMETERS:
 ***     Type                    Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
extern void VEcuAppFrm_RttSetAsync(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_RttSetAsync
 ***
 *** DESCRIPTION:
 ***     Function called when the operating system must use Real-Time-Testing and runs synchronously.
 ***
 *** PARAMETERS:
 ***     Type                    Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
extern void VEcuAppFrm_RttSetSync(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_ExpSvcPreTaskCall
 ***
 *** DESCRIPTION:
 ***     Function called when the operating system starts a task and must trigger rasters.
 ***
 *** PARAMETERS:
 ***     Type            Name                 Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     const uint16    RasterId             The experimentation service raster to invoke.
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
extern void VEcuAppFrm_ExpSvcPreTaskCall(const uint16 RasterId);

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_ExpSvcPostTaskCall
 ***
 *** DESCRIPTION:
 ***     Function called when the operating system terminates a task and must trigger rasters.
 ***
 *** PARAMETERS:
 ***     Type            Name                 Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     const uint16    RasterId             The experimentation service raster to invoke.
 ***     const boolean   ActiveBasicRateTask  A flag to indicate whether the current task is the active
 ***                                          basic rate task.
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
extern void VEcuAppFrm_ExpSvcPostTaskCall(const uint16 RasterId, const boolean ActiveBasicRateTask);

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_TerminateSimulation
 ***
 *** DESCRIPTION:
 ***     Function is called when terminating the simulation from within the model.
 ***
 *** PARAMETERS:
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
extern void VEcuAppFrm_TerminateSimulation(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_OnOsStarted
 ***
 *** DESCRIPTION:
 ***     Callout for the implementation of the operating system to signal the start of the OS.
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
extern void VEcuAppFrm_OnOsStarted(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_OnOsShutdown
 ***
 *** DESCRIPTION:
 ***     Callout for the implementation of the operating system to signal the shutdown of the OS .
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
extern void VEcuAppFrm_OnOsShutdown(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_OnOsWakeup
 ***
 *** DESCRIPTION:
 ***     Callout for the implementation of the operating system to signal the wakeup of the OS .
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
extern void VEcuAppFrm_OnOsWakeup(void);


#ifdef __cplusplus
}
#endif

#endif /* VECUAPPFRM_OS_H */

/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/
