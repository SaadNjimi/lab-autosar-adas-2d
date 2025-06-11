/*
 * Smart services header file.
 *
 * Copyright 2020, dSPACE GmbH. All rights reserved.
 */

#ifndef VEOS_SMARTSERVICES_H
#define VEOS_SMARTSERVICES_H

#include "VEOS_BaseTypes.h"
#include "SmartServices_Cfg.h"

#if SMARTSERVICES_BAM == VEOS_ON
#include "DsBusAccessManager.h"
#endif
#if SMARTSERVICES_BYP == VEOS_ON
#include "DsBypassingService.h"
#endif
#if SMARTSERVICES_DSDAQ == VEOS_ON
#include "DsDaq.h"
#endif
#if SMARTSERVICES_MDBS == VEOS_ON
#include "DsMotionService.h"
#endif
#if SMARTSERVICES_RTT == VEOS_ON
#include "DsRTT.h"
#endif
#if SMARTSERVICES_MODELACCESS == VEOS_ON
#include "DsModelAccess.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Verifies that the smart services interfaces are properly initialized.
 * Returns either VEOS_E_OK on success or VEOS_E_API_DISCONNECTED otherwise.
 */
VEOS_base_uint16 SmartServices_Initialize();

#ifdef __cplusplus
}
#endif

#endif
