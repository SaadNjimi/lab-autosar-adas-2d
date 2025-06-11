/**************************************************************************************************\
 *** VEOS_SmartServices_Cfg.h
 ***
 *** VEOS frame configuration header file for the configuration of smart services frame functions.
 ***
 *** Copyright 2019 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef VEOS_SMARTSERVICES_CFG_H
#define VEOS_SMARTSERVICES_CFG_H

/*------------------------------------------------------------------------------------------------*\
  DEFINES (OPT)
\*------------------------------------------------------------------------------------------------*/

#ifdef VEOS_DAQ_SERVICE_DSDAQ_CFG_VALUE
#undef  VEOS_DAQ_SERVICE_DSDAQ_CFG_VALUE
#endif /* !VEOS_DAQ_SERVICE_DSDAQ_CFG_VALUE */

#ifdef VEOS_STIM_SERVICE_RTT_CFG_VALUE
#undef  VEOS_STIM_SERVICE_RTT_CFG_VALUE
#endif /* !VEOS_STIM_SERVICE_RTT_CFG_VALUE */

#ifdef VEOS_EXP_SERVICE_BYP_CFG_VALUE
#undef  VEOS_EXP_SERVICE_BYP_CFG_VALUE
#endif /* !VEOS_EXP_SERVICE_BYP_CFG_VALUE */

#ifdef VEOS_EXP_SERVICE_MDBS_CFG_VALUE
#undef  VEOS_EXP_SERVICE_MDBS_CFG_VALUE
#endif /* !VEOS_EXP_SERVICE_MDBS_CFG_VALUE */

#ifdef VEOS_EXP_SERVICE_DSMODELACCESS_CFG_VALUE
#undef VEOS_EXP_SERVICE_DSMODELACCESS_CFG_VALUE
#endif /* !VEOS_EXP_SERVICE_DSMODELACCESS_CFG_VALUE */

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/

#include "SmartServices.h"

/*------------------------------------------------------------------------------------------------*\
  CONFIGURATION CHECKS
\*------------------------------------------------------------------------------------------------*/

#ifndef SMARTSERVICES_DSDAQ
#error Required definition of 'SMARTSERVICES_DSDAQ' not found. The configuration is invalid.
#endif /* !SMARTSERVICES_DSDAQ */

#ifndef SMARTSERVICES_RTT
#error Required definition of 'SMARTSERVICES_RTT' not found. The configuration is invalid.
#endif /* !SMARTSERVICES_RTT */

#ifndef SMARTSERVICES_BYP
#error Required definition of 'SMARTSERVICES_BYP' not found. The configuration is invalid.
#endif /* !SMARTSERVICES_BYP */

#ifndef SMARTSERVICES_MDBS
#error Required definition of 'SMARTSERVICES_MDBS' not found. The configuration is invalid.
#endif /* !SMARTSERVICES_MDBS */

#ifndef SMARTSERVICES_MODELACCESS
#error Required definition of 'SMARTSERVICES_MODELACCESS' not found. The configuration is invalid.
#endif /* !SMARTSERVICES_MODELACCESS */

/*------------------------------------------------------------------------------------------------*\
  DEFINES
\*------------------------------------------------------------------------------------------------*/

#define VEOS_DAQ_SERVICE_DSDAQ_CFG_VALUE                        SMARTSERVICES_DSDAQ
#define VEOS_STIM_SERVICE_RTT_CFG_VALUE                         SMARTSERVICES_RTT
#define VEOS_EXP_SERVICE_BYP_CFG_VALUE                          SMARTSERVICES_BYP
#define VEOS_EXP_SERVICE_MDBS_CFG_VALUE                         SMARTSERVICES_MDBS
#define VEOS_EXP_SERVICE_DSMODELACCESS_CFG_VALUE                SMARTSERVICES_MODELACCESS

#endif /* VEOS_SMARTSERVICES_CFG_H */

/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/
