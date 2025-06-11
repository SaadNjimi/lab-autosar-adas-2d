/**************************************************************************************************\
 *** VEcuAppFrm_Cfg.h
 ***
 *** Configuration header file for the V-ECU application frame. This file represents the static part.
 ***
 *** Copyright 2019 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef VECUAPPFRM_CFG_H
#define VECUAPPFRM_CFG_H

/*------------------------------------------------------------------------------------------------*\
 DEFINES (OPT)
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
 INCLUDES
\*------------------------------------------------------------------------------------------------*/

#include "VEcuAppFrm_Cfg_generated.h"

/*------------------------------------------------------------------------------------------------*\
 DEFINES
\*------------------------------------------------------------------------------------------------*/

/* Check for legacy code dependent hooks */
#ifdef OSIM_TRIGGERHOOKS
#ifndef VECUAPPFRM_HOOKS_AVAILABLE
#define VECUAPPFRM_USE_HOOKS
#endif /* !VECUAPPFRM_HOOKS_AVAILABLE */
#define VECUAPPFRM_HOOKS_USE_OLD_NAMES
#endif /* OSIM_TRIGGERHOOKS */

/* Evaluate user specific hooks */
#ifdef VECUAPPFRM_HOOKS_AVAILABLE
#ifndef VECUAPPFRM_PREHOOK
#ifndef VECUAPPFRM_HOOKS_USE_OLD_NAMES
#define VECUAPPFRM_PREHOOK                  VEcuAppFrm_PreHook      /* Use new naming styles */
#else
#define VECUAPPFRM_PREHOOK                  Osim_PreTriggerHook     /* Use legacy naming styles */
#endif /* VECUAPPFRM_HOOKS_USE_OLD_NAMES */
#endif /* VECUAPPFRM_PREHOOK */

#ifndef VECUAPPFRM_POSTHOOK
#ifndef VECUAPPFRM_HOOKS_USE_OLD_NAMES
#define VECUAPPFRM_POSTHOOK                 VEcuAppFrm_PostHook     /* Use new naming styles */
#else
#define VECUAPPFRM_POSTHOOK                 Osim_PostTriggerHook    /* Use legacy naming styles */
#endif /* VECUAPPFRM_HOOKS_USE_OLD_NAMES */
#endif /* VECUAPPFRM_POSTHOOK */
#endif /* VECUAPPFRM_HOOKS_AVAILABLE */

/*------------------------------------------------------------------------------------------------*\
 TYPEDEFS
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
#endif /* __cplusplus */

#if (VECUAPPFRM_SAB_AVAILABLE == VEOS_ON)
/**************************************************************************************************\
 *** FUNCTION:
 ***     VECUAPPFRM_STARTUP_FUNCTION
 ***
 *** DESCRIPTION:
 ***     Configured function for starting the V-ECU.
 ***
 *** PARAMETERS:
 ***     Type                    Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
extern void VECUAPPFRM_STARTUP_FUNCTION(void);
#endif /* VECUAPPFRM_SAB_AVAILABLE == VEOS_ON */

#ifdef VECUAPPFRM_PREHOOK
/**************************************************************************************************\
 *** FUNCTION:
 ***     VECUAPPFRM_PREHOOK
 ***
 *** DESCRIPTION:
 ***     Configured function to call before start or before each trigger.
 ***
 *** PARAMETERS:
 ***     Type                    Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
extern void VECUAPPFRM_PREHOOK(void);
#endif /* VECUAPPFRM_PREHOOK */

#ifdef VECUAPPFRM_POSTHOOK
/**************************************************************************************************\
 *** FUNCTION:
 ***     VECUAPPFRM_PREHOOK
 ***
 *** DESCRIPTION:
 ***     Configured function to call before start or after each trigger.
 ***
 *** PARAMETERS:
 ***     Type                    Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
extern void VECUAPPFRM_POSTHOOK(void);
#endif /* VECUAPPFRM_POSTHOOK */

#if (VECUAPPFRM_IO_AVAILABLE == VEOS_ON)
/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_CallIoEventHandler
 ***
 *** DESCRIPTION:
 ***     Generated implementation of the I/O handling function.
 ***
 *** PARAMETERS:
 ***     Type                    Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     const VEOS_uint32       eventId      Unique ID of the I/O event to handle.
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any error
 ***                            occurs, an error code will be the result. See VEOS_Types.h for
 ***                            details.
\**************************************************************************************************/
extern VEOS_ApiSuccessType VEcuAppFrm_CallIoEventHandler(const VEOS_uint32 eventId);
#endif /* VECUAPPFRM_IO_AVAILABLE == VEOS_ON */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* VECUAPPFRM_CFG_H */

/*------------------------------------------------------------------------------------------------*\
 END OF FILE
\*------------------------------------------------------------------------------------------------*/
