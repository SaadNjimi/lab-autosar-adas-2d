/*
 * dSPACE real time testing service interface.
 *
 * Copyright 2020, dSPACE GmbH. All rights reserved.
 */

#ifndef DSRTT_H
#define DSRTT_H

#include "SmartServices_Interaction.h"

#ifdef __cplusplus
extern "C"
{
#endif

extern SmartServices_DsRtt_ScriptServicePostComputationType gp_SmartServices_DsRtt_ScriptServicePostComputation;
extern SmartServices_DsRtt_ScriptServicePreComputationType gp_SmartServices_DsRtt_ScriptServicePreComputation;
extern SmartServices_DsRtt_SetAsyncType gp_SmartServices_DsRtt_SetAsync;

#ifdef __cplusplus
}
#endif

#define dll_dsrtt_script_service_pre_computation gp_SmartServices_DsRtt_ScriptServicePreComputation
#define dll_dsrtt_script_service_post_computation gp_SmartServices_DsRtt_ScriptServicePostComputation
#define dll_dsrtt_set_async(flag) gp_SmartServices_DsRtt_SetAsync(flag)

#endif
