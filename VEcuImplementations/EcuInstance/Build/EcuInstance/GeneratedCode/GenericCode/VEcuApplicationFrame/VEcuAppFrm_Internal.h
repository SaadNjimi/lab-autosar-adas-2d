/**************************************************************************************************\
 *** VEcuAppFrm_Internal.h
 ***
 *** Internal definitions for application frame for V-ECUs.
 ***
 *** Copyright 2019 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef VECUAPPFRM_INTERNAL_H
#define VECUAPPFRM_INTERNAL_H

/*------------------------------------------------------------------------------------------------*\
 DEFINES (OPT)
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
 INCLUDES
\*------------------------------------------------------------------------------------------------*/

#include "Platform_Types.h"
#include "VEOS.h"

/*------------------------------------------------------------------------------------------------*\
 DEFINES
\*------------------------------------------------------------------------------------------------*/

#define VECUAPPFRM_MODULE_NAME				"VEcuApplicationFrame"
#define VECUAPPFRM_MODULE_ID				(VEOS_uint32)0xFE

/*------------------------------------------------------------------------------------------------*\
 TYPEDEFS
\*------------------------------------------------------------------------------------------------*/

/*
 * Definition of the internal states of the V-ECU application frame.
 */
typedef enum _tag_VEcuAppFrm_AppStateType
{
    /* V-ECU is currently turned off. */
    VEcuAppFrm_State_Off,
    /* V-ECU is currently booting. */
    VEcuAppFrm_State_Booting,
    /* V-ECU is currently running. */
    VEcuAppFrm_State_Running,
    /* V-ECU is currently terminating. */
    VEcuAppFrm_State_ShuttingDown
} VEcuAppFrm_AppStateType;

/*------------------------------------------------------------------------------------------------*\
 PARAMETERIZED MACROS
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
 VARIABLE DECLARATIONS
\*------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* The current state */
extern VEcuAppFrm_AppStateType g_VEcuAppFrm_State;
/* The number of basic task steps to perform before a stimulation can take place. */
extern uint32 g_VEcuAppFrm_StimCycles;
/* The number of delayed cycles */
extern  uint32 g_VEcuAppFrm_NumberOfDelayCycles;
/* The RealTimeTesting lock. */
extern boolean g_VEcuAppFrm_RttLock;
/* The flag to indicate that the simulator is shutting down the kernel */
extern boolean g_VEcuAppFrm_SimulatorShuttingDown;

extern VEOS_boolean g_VEcuAppFrm_TriggerStep;

#ifdef __cplusplus
};
#endif /* __cplusplus */

/*------------------------------------------------------------------------------------------------*\
 FUNCTION DECLARATIONS
\*------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_ChangeState
 ***
 *** DESCRIPTION:
 ***     Implements a simple state machine with four states, where each state can reach any other
 ***     state except itself.
 ***
 *** PARAMETERS:
 ***     Type                    Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     VEcuAppFrm_AppStateType newState     The new state to enter.
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any error
 ***                            occurs, an error code will be the result. See VEOS_Types.h for
 ***                            details.
\**************************************************************************************************/
extern VEOS_ApiSuccessType VEcuAppFrm_ChangeState(const VEcuAppFrm_AppStateType newState);

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_HandleStartEvent
 ***
 *** DESCRIPTION:
 ***     Function called to handle delayed V-ECU start-up.
 ***
 *** PARAMETERS:
 ***     Type                    Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any error
 ***                            occurs, an error code will be the result. See VEOS_Types.h for
 ***                            details.
\**************************************************************************************************/
extern VEOS_ApiSuccessType VEcuAppFrm_HandleStartEvent(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_TriggerStimulation
 ***
 *** DESCRIPTION:
 ***     Function called to trigger stimulation in the current trigger step.
 ***     If stimulation is not required, the next stimulation step might be prepared.
 ***
 *** PARAMETERS:
 ***     Type                    Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
extern void VEcuAppFrm_TriggerStimulation(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_ResetStimulation
 ***
 *** DESCRIPTION:
 ***     Function called to reset stimulation services to initial values.
 ***
 *** PARAMETERS:
 ***     Type                    Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
extern void VEcuAppFrm_ResetStimulation(void);

#ifdef __cplusplus
};
#endif /* __cplusplus */

#endif /* VECUAPPFRM_INTERNAL_H */

/*------------------------------------------------------------------------------------------------*\
 END OF FILE
\*------------------------------------------------------------------------------------------------*/
