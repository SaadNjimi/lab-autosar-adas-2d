/*
 * dSPACE DAQ service interface.
 *
 * Copyright 2020, dSPACE GmbH. All rights reserved.
 */

#ifndef DSDAQ_H
#define DSDAQ_H

#include "SmartServices_Interaction.h"

#ifdef __cplusplus
extern "C" {
#endif

extern SmartServices_DsDaq_ServiceType gp_SmartServices_DsDaq_Service;

#ifdef __cplusplus
}
#endif

#define DsDaq_Service(applicationId, serviceInstanceNo, simulationTime) gp_SmartServices_DsDaq_Service(applicationId, serviceInstanceNo, simulationTime)

#endif
