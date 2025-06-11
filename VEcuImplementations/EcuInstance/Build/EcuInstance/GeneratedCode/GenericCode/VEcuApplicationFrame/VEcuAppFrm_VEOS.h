/**************************************************************************************************\
 *** VEcuAppFrm_Modules.h
 ***
 *** Header file resolving the dependencies to VEOS Frame for the application frame implementation.
 ***
 *** Copyright 2019 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef VECUAPPFRM_VEOS_H
#define VECUAPPFRM_VEOS_H

/*------------------------------------------------------------------------------------------------*\
 DEFINES (OPT)
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
 INCLUDES
\*------------------------------------------------------------------------------------------------*/

#include "VEOS.h"
#include "VEcuAppFrm_Cfg.h"

/*------------------------------------------------------------------------------------------------*\
 DEFINES
\*------------------------------------------------------------------------------------------------*/

/* Definition of a delayed start-up: 0xFE00 is a specific identified for V-ECU application frame. 0xA1 is the error code. */
#define VECUAPPFRM_E_DELAYED                        (VEOS_ApiSuccessType)0xFEA1

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
#endif

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_Download
 ***
 *** DESCRIPTION:
 ***     Event handler function called right after the V-ECU has been downloaded to the simulator.
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
extern VEOS_ApiSuccessType VEcuAppFrm_Download(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_Start
 ***
 *** DESCRIPTION:
 ***     Event handler function called right after the simulator has started the simulation.
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
extern VEOS_ApiSuccessType VEcuAppFrm_Start(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_Trigger
 ***
 *** DESCRIPTION:
 ***     Event handler function called on time-cycle base by the simulator to indicate a
 ***     new simulation step.
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
extern VEOS_ApiSuccessType VEcuAppFrm_Trigger(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_Stop
 ***
 *** DESCRIPTION:
 ***     Event handler function called when the simulation is ending on time base or by explicit
 ***     user request via VEOS API or simulation clients.
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
extern VEOS_ApiSuccessType VEcuAppFrm_Stop(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_Reset
 ***
 *** DESCRIPTION:
 ***     Event handler function called when the simulation is ending and 
 ***     after VEOS_ApiSuccessType VEcuAppFrm_Stop() is called on time base or by explicit
 ***     user request via VEOS API or simulation clients.
 ***
 *** PARAMETERS:
 ***     Type                    Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     void * const     VpuData           Incoming argument: The pointer to the VPU data.
 ***
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any error
 ***                            occurs, an error code will be the result. See VEOS_Types.h for
 ***                            details.
\**************************************************************************************************/
extern VEOS_ApiSuccessType VEcuAppFrm_Reset(void * const VpuData);

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_HandleIoEvent
 ***
 *** DESCRIPTION:
 ***     Event handler function called when the model has encountered an I/O event. This event is
 ***     routed from VEOS Frame to the handler.
 ***
 *** PARAMETERS:
 ***     Type                    Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     const VEOS_uint32       ioEventId    Unique ID of the I/O event to handle.
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any error
 ***                            occurs, an error code will be the result. See VEOS_Types.h for
 ***                            details.
\**************************************************************************************************/
extern VEOS_ApiSuccessType VEcuAppFrm_HandleIoEvent(const VEOS_uint32 ioEventId);


#ifdef __cplusplus
}
#endif

#endif /* VECUAPPFRM_VEOS_H */

/*------------------------------------------------------------------------------------------------*\
 END OF FILE
\*------------------------------------------------------------------------------------------------*/
