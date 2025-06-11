/**************************************************************************************************\
 *** VEOS_Data.h
 ***
 *** VEOS frame data access header file.
 ***
 *** Copyright 2019 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef VEOS_DATA_H
#define VEOS_DATA_H

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/

#include "VEOS.h"
#ifdef VEOS_GUARD_PORTAPIUSERS_OK
#if (VEOS_PORTAPI_NUMBEROFINPORTS > 0) || (VEOS_PORTAPI_NUMBEROFOUTPORTS > 0)
#include "VEOS_Data_generated.h"
#endif
#endif /* VEOS_GUARD_PORTAPIUSERS_OK */

/*------------------------------------------------------------------------------------------------*\
  PARAMETERIZED MACROS
\*------------------------------------------------------------------------------------------------*/

/**************************************************************************************************\
 *** MACRO:
 ***     VEOS_CURRENT_SIMULATIONTIME
 ***
 *** DESCRIPTION:
 ***     Parameterized macro to access the current simulation time.
 ***
 *** PARAMETERS:
 ***     Type                           Name            Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     VEOS_float64
 ***                                    The current simulation time.
\**************************************************************************************************/
#define VEOS_CURRENT_SIMULATIONTIME()                           g_VEOS_CurrentSimulationTime

/**************************************************************************************************\
 *** MACRO:
 ***     VEOS_CURRENT_SIMULATIONTIMETICKS_NS
 ***
 *** DESCRIPTION:
 ***     Parameterized macro to access the current simulation time ticks in nano seconds.
 ***
 *** PARAMETERS:
 ***     Type                           Name            Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     VEOS_float64
 ***                                    The current simulation time.
\**************************************************************************************************/
#define VEOS_CURRENT_SIMULATIONCOUNTER_1NS()                   g_VEOS_CurrentSimulationCounter_1ns

/**************************************************************************************************\
 *** MACRO:
 ***     VEOS_CURRENT_APPLICATIONID
 ***
 *** DESCRIPTION:
 ***     Parameterized macro to get the current local application id.
 ***
 *** PARAMETERS:
 ***     Type                           Name            Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     VEOS_uint32
 ***                                    The current application id.
\**************************************************************************************************/
#define VEOS_CURRENT_APPLICATIONID()                            g_VEOS_ApplicationId

/**************************************************************************************************\
 *** MACRO:
 ***     VEOS_CURRENT_APPLICATIONBASEADDRESS
 ***
 *** DESCRIPTION:
 ***     Parameterized macro to get the current local memory basic address.
 ***
 *** PARAMETERS:
 ***     Type                           Name            Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     void*
 ***                                    The current application id.
\**************************************************************************************************/
#define VEOS_CURRENT_APPLICATIONBASEADDRESS()                   gp_VEOS_TargetAddressBase

#endif /* VEOS_DATA_H */

/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/
