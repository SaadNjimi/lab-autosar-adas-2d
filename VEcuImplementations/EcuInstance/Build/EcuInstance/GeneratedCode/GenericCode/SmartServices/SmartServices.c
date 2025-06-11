/*
 * Smart services wrapper.
 *
 * Copyright 2020, dSPACE GmbH. All rights reserved.
 */

#define SMARTSERVICES_C
#include "SmartServices.h"
#include "SmartServices_Interaction.h"
#include "VEOS.h"

#if SMARTSERVICES_BAM == VEOS_ON
SmartServices_DsBusAccess_EthernetReplayGetTimeInfoType gp_SmartServices_DsBam_EthernetReplayGetTimeInfo;
SmartServices_DsBusAccess_EthernetReplaySetStartTimeType gp_SmartServices_DsBam_EthernetReplaySetStartTime;
SmartServices_DsBusAccess_EthernetReplaySetStopTimeType gp_SmartServices_DsBam_EthernetReplaySetStopTime;
SmartServices_DsBusAccess_CANReplayGetTimeInfoType gp_SmartServices_DsBam_CANReplayGetTimeInfo;
SmartServices_DsBusAccess_CANReplaySetStartTimeType gp_SmartServices_DsBam_CANReplaySetStartTime;
SmartServices_DsBusAccess_CANReplaySetStopTimeType gp_SmartServices_DsBam_CANReplaySetStopTime;

static const SmartServices_DsBusAccess_ApiType g_SmartServices_DsBusAccess_Api = {
    VEOS_V54_SMARTSERVICES_DSBUSACCESS_ABI_VERSION,    &gp_SmartServices_DsBam_EthernetReplayGetTimeInfo, &gp_SmartServices_DsBam_EthernetReplaySetStartTime,
    &gp_SmartServices_DsBam_EthernetReplaySetStopTime, &gp_SmartServices_DsBam_CANReplayGetTimeInfo,      &gp_SmartServices_DsBam_CANReplaySetStartTime,
    &gp_SmartServices_DsBam_CANReplaySetStopTime};
#endif

//
// ECU Bypassing
//
#if SMARTSERVICES_BYP == VEOS_ON
SmartServices_DsEcuIntByp_GetAddressPtrType gp_SmartServices_DsEcuIntByp_GetAddressPtr;
SmartServices_DsEcuIntByp_ServiceType gp_SmartServices_DsEcuIntByp_Service;

static const SmartServices_DsEcuIntByp_ApiType g_SmartServices_DsEcuIntByp_Api = {
    VEOS_V54_SMARTSERVICES_DSECUINTBYP_ABI_VERSION, &gp_SmartServices_DsEcuIntByp_GetAddressPtr, &gp_SmartServices_DsEcuIntByp_Service};

// This function is required for the older (pre VEOS 5.1) Bypassing interface.
VEOS_base_uint16 dsEcuInt_service(VEOS_base_uint16 svcId)
{
    return dsECUIntByp_service(svcId);
}
#endif

//
// DsDaq Service
//
#if SMARTSERVICES_DSDAQ == VEOS_ON
SmartServices_DsDaq_ServiceType gp_SmartServices_DsDaq_Service;

static const SmartServices_DsDaq_ApiType g_SmartServices_DsDaq_Api = {VEOS_V54_SMARTSERVICES_DSDAQ_ABI_VERSION, &gp_SmartServices_DsDaq_Service};
#endif

//
// MotionDesk
//
#if SMARTSERVICES_MDBS == VEOS_ON
SmartServices_DsMd_InitInstrumentsType gp_SmartServices_DsMd_InitInstruments;
SmartServices_DsMd_InitInterfaceType gp_SmartServices_DsMd_InitInterface;
SmartServices_DsMd_ServiceType gp_SmartServices_DsMd_Service;
SmartServices_DsMd_SetBroadcastType gp_SmartServices_DsMd_SetBroadcast;
SmartServices_DsMd_SetInstrumentDataType gp_SmartServices_DsMd_SetInstrumentData;
SmartServices_DsMd_SetInstrumentPropertiesType gp_SmartServices_DsMd_SetInstrumentProperties;
SmartServices_DsMd_SetManeuverStartTimeType gp_SmartServices_DsMd_SetManeuverStartTime;
SmartServices_DsMd_SetMemberNameType gp_SmartServices_DsMd_SetMemberName;
SmartServices_DsMd_SetMemberPositionType gp_SmartServices_DsMd_SetMemberPosition;

static const SmartServices_DsMd_ApiType g_SmartServices_DsMd_Api = {VEOS_V54_SMARTSERVICES_DSMD_ABI_VERSION,
                                                                    &gp_SmartServices_DsMd_InitInstruments,
                                                                    &gp_SmartServices_DsMd_InitInterface,
                                                                    &gp_SmartServices_DsMd_Service,
                                                                    &gp_SmartServices_DsMd_SetBroadcast,
                                                                    &gp_SmartServices_DsMd_SetInstrumentData,
                                                                    &gp_SmartServices_DsMd_SetInstrumentProperties,
                                                                    &gp_SmartServices_DsMd_SetManeuverStartTime,
                                                                    &gp_SmartServices_DsMd_SetMemberName,
                                                                    &gp_SmartServices_DsMd_SetMemberPosition};
#endif

//
// Real-Time Testing
//
#if SMARTSERVICES_RTT == VEOS_ON
SmartServices_DsRtt_ScriptServicePostComputationType gp_SmartServices_DsRtt_ScriptServicePostComputation;
SmartServices_DsRtt_ScriptServicePreComputationType gp_SmartServices_DsRtt_ScriptServicePreComputation;
SmartServices_DsRtt_SetAsyncType gp_SmartServices_DsRtt_SetAsync;

static const SmartServices_DsRtt_ApiType g_SmartServices_DsRtt_Api = {VEOS_V54_SMARTSERVICES_DSRTT_ABI_VERSION,
                                                                      &gp_SmartServices_DsRtt_ScriptServicePostComputation,
                                                                      &gp_SmartServices_DsRtt_ScriptServicePreComputation, &gp_SmartServices_DsRtt_SetAsync};
#endif

//
// DsModelAccess
//
#if SMARTSERVICES_MODELACCESS == VEOS_ON
static const volatile VEOS_base_uint32 g_SmartServices_DsModelAccessEnabledDefault = 0;
const volatile VEOS_base_uint32* g_SmartServices_DsModelAccessEnabled = &g_SmartServices_DsModelAccessEnabledDefault;
SmartServices_DsModelAccessInitType gp_SmartServices_DsModelAccessInit;
SmartServices_DsModelAccessReadType gp_SmartServices_DsModelAccessRead;
SmartServices_DsModelAccessWriteType gp_SmartServices_DsModelAccessWrite;

static const SmartServices_DsModelAccess_ApiType g_SmartServices_DsModelAccess_Api = {
    VEOS_V54_SMARTSERVICES_DSMODELACCESS_ABI_VERSION, &g_SmartServices_DsModelAccessEnabled, &gp_SmartServices_DsModelAccessInit,
    &gp_SmartServices_DsModelAccessRead, &gp_SmartServices_DsModelAccessWrite};
#endif

//
// API initialization.
//
const SmartServices_ApiType g_SmartServices_Api = {
    VEOS_V54_SMARTSERVICES_ABI_VERSION,
#if SMARTSERVICES_BAM == VEOS_ON
   &g_SmartServices_DsBusAccess_Api,
#else
   VEOS_NULL,
#endif

#if SMARTSERVICES_BYP == VEOS_ON
   &g_SmartServices_DsEcuIntByp_Api,
#else
   VEOS_NULL,
#endif

#if SMARTSERVICES_DSDAQ == VEOS_ON
   &g_SmartServices_DsDaq_Api,
#else
   VEOS_NULL,
#endif

#if SMARTSERVICES_MDBS == VEOS_ON
   &g_SmartServices_DsMd_Api,
#else
   VEOS_NULL,
#endif

#if SMARTSERVICES_RTT == VEOS_ON
   &g_SmartServices_DsRtt_Api,
#else
   VEOS_NULL,
#endif

#if SMARTSERVICES_MODELACCESS == VEOS_ON
   &g_SmartServices_DsModelAccess_Api
#else
   VEOS_NULL
#endif
};

//
// State management functions for the SmartServices.
//
VEOS_base_uint16 SmartServices_Initialize()
{
    /* Check if API is correctly connected to the SmartOfflineServicesAdapterRP */
#if SMARTSERVICES_BAM == VEOS_ON
    if (VEOS_NULL == gp_SmartServices_DsBam_EthernetReplayGetTimeInfo)
    {
        return VEOS_E_API_DISCONNECTED;
    }
    if (VEOS_NULL == gp_SmartServices_DsBam_EthernetReplaySetStartTime)
    {
        return VEOS_E_API_DISCONNECTED;
    }
    if (VEOS_NULL == gp_SmartServices_DsBam_EthernetReplaySetStopTime)
    {
        return VEOS_E_API_DISCONNECTED;
    }
    if (VEOS_NULL == gp_SmartServices_DsBam_CANReplayGetTimeInfo)
    {
        return VEOS_E_API_DISCONNECTED;
    }
    if (VEOS_NULL == gp_SmartServices_DsBam_CANReplaySetStartTime)
    {
        return VEOS_E_API_DISCONNECTED;
    }
    if (VEOS_NULL == gp_SmartServices_DsBam_CANReplaySetStopTime)
    {
        return VEOS_E_API_DISCONNECTED;
    }
#endif

#if SMARTSERVICES_BYP == VEOS_ON
    if (VEOS_NULL == gp_SmartServices_DsEcuIntByp_GetAddressPtr)
    {
        return VEOS_E_API_DISCONNECTED;
    }
    if (VEOS_NULL == gp_SmartServices_DsEcuIntByp_Service)
    {
        return VEOS_E_API_DISCONNECTED;
    }
#endif

#if SMARTSERVICES_DSDAQ == VEOS_ON
    if (VEOS_NULL == gp_SmartServices_DsDaq_Service)
    {
        return VEOS_E_API_DISCONNECTED;
    }
#endif

#if SMARTSERVICES_MDBS == VEOS_ON
    if (VEOS_NULL == gp_SmartServices_DsMd_InitInstruments)
    {
        return VEOS_E_API_DISCONNECTED;
    }
    if (VEOS_NULL == gp_SmartServices_DsMd_InitInterface)
    {
        return VEOS_E_API_DISCONNECTED;
    }
    if (VEOS_NULL == gp_SmartServices_DsMd_Service)
    {
        return VEOS_E_API_DISCONNECTED;
    }
    if (VEOS_NULL == gp_SmartServices_DsMd_SetBroadcast)
    {
        return VEOS_E_API_DISCONNECTED;
    }
    if (VEOS_NULL == gp_SmartServices_DsMd_SetInstrumentData)
    {
        return VEOS_E_API_DISCONNECTED;
    }
    if (VEOS_NULL == gp_SmartServices_DsMd_SetInstrumentProperties)
    {
        return VEOS_E_API_DISCONNECTED;
    }
    if (VEOS_NULL == gp_SmartServices_DsMd_SetManeuverStartTime)
    {
        return VEOS_E_API_DISCONNECTED;
    }
    if (VEOS_NULL == gp_SmartServices_DsMd_SetMemberName)
    {
        return VEOS_E_API_DISCONNECTED;
    }
    if (VEOS_NULL == gp_SmartServices_DsMd_SetMemberPosition)
    {
        return VEOS_E_API_DISCONNECTED;
    }
#endif

#if SMARTSERVICES_RTT == VEOS_ON
    if (VEOS_NULL == gp_SmartServices_DsRtt_ScriptServicePostComputation)
    {
        return VEOS_E_API_DISCONNECTED;
    }
    if (VEOS_NULL == gp_SmartServices_DsRtt_ScriptServicePreComputation)
    {
        return VEOS_E_API_DISCONNECTED;
    }
    if (VEOS_NULL == gp_SmartServices_DsRtt_SetAsync)
    {
        return VEOS_E_API_DISCONNECTED;
    }
#endif

#if SMARTSERVICES_MODELACCESS == VEOS_ON
    if (gp_SmartServices_DsModelAccessInit == VEOS_NULL)
    {
        return VEOS_E_API_DISCONNECTED;
    }
#endif

    return VEOS_E_OK;
}
