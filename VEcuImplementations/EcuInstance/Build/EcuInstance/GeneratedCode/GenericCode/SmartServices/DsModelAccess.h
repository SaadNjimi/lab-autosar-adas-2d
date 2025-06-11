/*
 * dSPACE Model access service interface.
 *
 * Copyright 2020, dSPACE GmbH. All rights reserved.
 */

#ifndef VEOS_DSMODELACCESSRP_H
#define VEOS_DSMODELACCESSRP_H

#include "SmartServices_Interaction.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const volatile VEOS_base_uint32* g_SmartServices_DsModelAccessEnabled;
extern SmartServices_DsModelAccessInitType gp_SmartServices_DsModelAccessInit;
extern SmartServices_DsModelAccessReadType gp_SmartServices_DsModelAccessRead;
extern SmartServices_DsModelAccessWriteType gp_SmartServices_DsModelAccessWrite;

#ifdef __cplusplus
}
#endif

/*
 * The hook shall be called in the application's onInit initialization phase.
 * It identifies applications which do support the model data access feature.
 */
inline static void DsModelAccessInit()
{
    gp_SmartServices_DsModelAccessInit();
}

/*
 * The hook should be called at the end of the associated task.
 * It executes pending read operations accessing the task's variables.
 */
inline static void DsModelAccessRead(VEOS_base_uint32 serviceId)
{
    if (*g_SmartServices_DsModelAccessEnabled != 0) {
        gp_SmartServices_DsModelAccessRead(serviceId);
    }
}

/*
 * The hook should be called at the beginning of the associated task.
 * It executes pending write operations to the task's variables.
 */
inline static void DsModelAccessWrite(VEOS_base_uint32 serviceId)
{
    if (*g_SmartServices_DsModelAccessEnabled != 0) {
        gp_SmartServices_DsModelAccessWrite(serviceId);
    }
}

#endif
