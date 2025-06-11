/**************************************************************************************************\
 *** Dap.h
 ***
 *** Generic implementation for Data Access Points.
 ***
 *** Copyright (c) 2012 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef DAP_H
#define DAP_H

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/

#include "Std_Types.h"
#include "Dap_Cfg.h"

/*------------------------------------------------------------------------------------------------*\
  DEFINES
\*------------------------------------------------------------------------------------------------*/

/* Vendor ID. */
#define DAP_VENDOR_ID           35

/* Module ID of Data Access Points. */
#define DAP_MODULE_ID           9010

/* Module name of Data Access Points. */
#define DAP_MODULE_NAME         "Data Access Points"

/* AUTOSAR specification where the appropriate implementation is based on. */
#define DAP_AR_MAJOR_VERSION    3
#define DAP_AR_MINOR_VERSION    2
#define DAP_AR_PATCH_VERSION    1

/* Version number of dSPACE specific implementation of the module. */
#define DAP_SW_MAJOR_VERSION    1
#define DAP_SW_MINOR_VERSION    0
#define DAP_SW_PATCH_VERSION    0

/*------------------------------------------------------------------------------------------------*\
  FUNCTION PROTOTYPES
\*------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/* The DAP API functions. */
extern void Dap_Init(void);
extern void Dap_DeInit(void);
extern void Dap_CheckInitialized(uint8 functionType);
#ifdef __cplusplus
}
#endif

#endif /* DAP_H */

/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/
