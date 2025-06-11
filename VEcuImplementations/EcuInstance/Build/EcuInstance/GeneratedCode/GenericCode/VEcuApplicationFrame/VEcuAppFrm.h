/**************************************************************************************************\
 *** VEcuAppFrm.h
 ***
 *** Application frame declarations for V-ECUs.
 ***
 *** Copyright 2019 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef VECUAPPFRM_H
#define VECUAPPFRM_H

/*------------------------------------------------------------------------------------------------*\
  DEFINES (OPT)
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/

#include "Platform_Types.h"
#include "Std_Types.h"

/*------------------------------------------------------------------------------------------------*\
  DEFINES
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
  TYPEDEFS
\*------------------------------------------------------------------------------------------------*/

/* Definition of an error. */
typedef enum _tag_VEcuAppFrm_ErrorType
{
    /* This is a non-critical runtime error. */
    VEcuAppFrm_RuntimeError = 0x0,

    /* This is a non-critical development error. */
    VEcuAppFrm_DevelopmentError = 0x1,

    /* This is a transient, non-faulting error. */
    VEcuAppFrm_TransientError = 0x2,

    /* This is a critical error. */
    VEcuAppFrm_Fault = 0xF,
} VEcuAppFrm_ErrorType;

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
 ***     VEcuAppFrm_ReportError
 ***
 *** DESCRIPTION:
 ***     Service to report development, runtime or transient errors.
 ***
 *** PARAMETERS:
 ***     Type                 Name        Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     VEcuAppFrm_ErrorType Error       Specifies the type of error occured.
 ***     uint16               ModuleId    Module ID of calling module.
 ***     uint8                InstanceId  The identifier of the index based instance of a module, starting
 ***                                      from 0. If the module is a single instance module it shall pass 0 as
 ***                                      the InstanceId.
 ***     uint8   ApiId                    ID of API service in which error is detected (defined in SWS of
 ***                                      calling module).
 ***     uint8   ErrorId                  ID of detected development error (defined in SWS of calling module).
 ***
 *** RETURNS:
 ***     Std_ReturnType      E_OK, if the error has been handled.
 ***                         E_NOT_OK, if the error has not been handled.
 ***                         If Det is used, any other implementation specific errors might be returned.
\**************************************************************************************************/
Std_ReturnType VEcuAppFrm_ReportError(VEcuAppFrm_ErrorType Error, uint16 ModuleId, uint8 InstanceId, uint8 ApiId, uint8 ErrorId);

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_CallBypassingService
 ***
 *** DESCRIPTION:
 ***     Calls the bypassing service with error checks.
 ***
 *** PARAMETERS:
 ***     Type                 Name        Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     uint16               ServiceId   ID of the service raster to forward to the service.
 ***
 *** RETURNS:
 ***     Std_ReturnType      E_OK, if the service call has been forwarded.
 ***                         E_NOT_OK, if the service call resulted in an error.
\**************************************************************************************************/
Std_ReturnType VEcuAppFrm_CallBypassingService(uint16 ServiceId);

#ifdef __cplusplus
}
#endif

#endif /* VECUAPPFRM_H */

/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/
