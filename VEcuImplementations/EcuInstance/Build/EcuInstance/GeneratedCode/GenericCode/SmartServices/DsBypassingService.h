/*
 * dSPACE bypassing service interface.
 *
 * Copyright 2020, dSPACE GmbH. All rights reserved.
 */

#ifndef DSBYPASSINGSERVICE_H
#define DSBYPASSINGSERVICE_H

#include "SmartServices_Interaction.h"

#ifdef __cplusplus
extern "C" {
#endif

extern SmartServices_DsEcuIntByp_GetAddressPtrType gp_SmartServices_DsEcuIntByp_GetAddressPtr;
extern SmartServices_DsEcuIntByp_ServiceType gp_SmartServices_DsEcuIntByp_Service;

#ifdef __cplusplus
}
#endif

#define dsECUIntByp_service(id) gp_SmartServices_DsEcuIntByp_Service(id)
#define dsECUIntByp_getAddressPtr(address) gp_SmartServices_DsEcuIntByp_GetAddressPtr(address)

#endif
