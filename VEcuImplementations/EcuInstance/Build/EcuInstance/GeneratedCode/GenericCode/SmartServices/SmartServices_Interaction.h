/****************************************************************************************************************************\
 *** SmartServices_Interaction.h
 ***
 *** This file provides the interaction types shared between the SmartServices and the SmartOfflineServicesAdapterRP.
 ***
 *** Copyright (c) 2020 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\****************************************************************************************************************************/

#ifndef SMARTSERVICES_INTERACTION_V54_H
#define SMARTSERVICES_INTERACTION_V54_H

#include "VEOS_BaseTypes.h"


#if defined(_WIN32) && !defined(_WIN64)
#  ifdef __GNUC__
#    define SMARTSERVICES_CDECL __attribute__((__cdecl__))
#  else
#    define SMARTSERVICES_CDECL __cdecl
#  endif
#else
#  define SMARTSERVICES_CDECL
#endif


#ifdef VEOS_FRAME_V54_NAMESPACE_BEGIN
VEOS_FRAME_V54_NAMESPACE_BEGIN
#endif

/*---------------*
 * BAM *
 *---------------*/
#define VEOS_V54_SMARTSERVICES_DSBUSACCESS_ABI_VERSION 0x0005000400000000

typedef VEOS_base_boolean(SMARTSERVICES_CDECL *SmartServices_DsBusAccess_EthernetReplayGetTimeInfoType)(VEOS_base_sint32 AdapterIdx, VEOS_base_uint64 *pStartTs_sec,
                                                                                                  VEOS_base_uint32 *pStartTs_ns, VEOS_base_uint64 *pStopTs_sec,
                                                                                                  VEOS_base_uint32 *pStopTs_ns);
typedef VEOS_base_boolean(SMARTSERVICES_CDECL *SmartServices_DsBusAccess_EthernetReplaySetStartTimeType)(VEOS_base_sint32 AdapterIdx, VEOS_base_uint64 StartTs_sec,
                                                                                                   VEOS_base_uint32 StartTs_ns);
typedef VEOS_base_boolean(SMARTSERVICES_CDECL *SmartServices_DsBusAccess_EthernetReplaySetStopTimeType)(VEOS_base_sint32 AdapterIdx, VEOS_base_uint64 StopTs_sec,
                                                                                                  VEOS_base_uint32 StopTs_ns);
typedef VEOS_base_boolean(SMARTSERVICES_CDECL *SmartServices_DsBusAccess_CANReplayGetTimeInfoType)(const void* DriverHandle, VEOS_base_uint64 *pStartTs_sec,
                                                                                             VEOS_base_uint32 *pStartTs_ns, VEOS_base_uint64 *pStopTs_sec,
                                                                                             VEOS_base_uint32 *pStopTs_ns);
typedef VEOS_base_boolean(SMARTSERVICES_CDECL *SmartServices_DsBusAccess_CANReplaySetStartTimeType)(const void* DriverHandle, VEOS_base_uint64 StartTs_sec,
                                                                                              VEOS_base_uint32 StartTs_ns);
typedef VEOS_base_boolean(SMARTSERVICES_CDECL *SmartServices_DsBusAccess_CANReplaySetStopTimeType)(const void* DriverHandle, VEOS_base_uint64 StopTs_sec,
                                                                                             VEOS_base_uint32 StopTs_ns);
typedef struct
{
    VEOS_base_uint64 ApiVersion;
    SmartServices_DsBusAccess_EthernetReplayGetTimeInfoType *SmartServices_DsBam_EthernetReplayGetTimeInfo;
    SmartServices_DsBusAccess_EthernetReplaySetStartTimeType *SmartServices_DsBam_EthernetReplaySetStartTime;
    SmartServices_DsBusAccess_EthernetReplaySetStopTimeType *SmartServices_DsBam_EthernetReplaySetStopTime;
    SmartServices_DsBusAccess_CANReplayGetTimeInfoType *SmartServices_DsBam_CANReplayGetTimeInfo;
    SmartServices_DsBusAccess_CANReplaySetStartTimeType *SmartServices_DsBam_CANReplaySetStartTime;
    SmartServices_DsBusAccess_CANReplaySetStopTimeType *SmartServices_DsBam_CANReplaySetStopTime;
} SmartServices_DsBusAccess_ApiType;

//
// ECU Bypassing
//
typedef VEOS_base_uint32* (SMARTSERVICES_CDECL *SmartServices_DsEcuIntByp_GetAddressPtrType)(VEOS_base_uint32 Address);
typedef VEOS_base_uint16 (SMARTSERVICES_CDECL *SmartServices_DsEcuIntByp_ServiceType)(VEOS_base_uint16 Id);

#define VEOS_V54_SMARTSERVICES_DSECUINTBYP_ABI_VERSION 0x0000000000000005

typedef struct {
    VEOS_base_uint64 AbiVersion;
    SmartServices_DsEcuIntByp_GetAddressPtrType* SmartServices_DsEcuIntByp_GetAddressPtr;
    SmartServices_DsEcuIntByp_ServiceType* SmartServices_DsEcuIntByp_Service;
} SmartServices_DsEcuIntByp_ApiType;


//
// DsDaq
//
typedef VEOS_base_sint32 (SMARTSERVICES_CDECL *SmartServices_DsDaq_ServiceType)(VEOS_base_uint32 ApplicationId, VEOS_base_uint32 ServiceInstanceNo, VEOS_base_float64 SimulationTime);

#define VEOS_V54_SMARTSERVICES_DSDAQ_ABI_VERSION 0x0000000000000005

typedef struct {
    VEOS_base_uint64 AbiVersion;
    SmartServices_DsDaq_ServiceType * SmartServices_DsDaq_Service;
} SmartServices_DsDaq_ApiType;


//
// MotionDesk
//
#define DS_MD_HOMOG_DIMENSION 4
typedef double DsMDHomogPositionData[DS_MD_HOMOG_DIMENSION][DS_MD_HOMOG_DIMENSION];

typedef VEOS_base_sint32 (SMARTSERVICES_CDECL *SmartServices_DsMd_InitInstrumentsType)(VEOS_base_uint32 NumberOfInstruments);
typedef VEOS_base_sint32 (SMARTSERVICES_CDECL *SmartServices_DsMd_InitInterfaceType)(VEOS_base_uint32 NumberOfMembers);
typedef VEOS_base_sint32 (SMARTSERVICES_CDECL *SmartServices_DsMd_ServiceType)(VEOS_base_float64 SimulationTime);
typedef VEOS_base_sint32 (SMARTSERVICES_CDECL *SmartServices_DsMd_SetBroadcastType)(VEOS_base_uint32 State);
typedef VEOS_base_sint32 (SMARTSERVICES_CDECL *SmartServices_DsMd_SetInstrumentDataType)(VEOS_base_uint32 MemberIdx, void* pValue);
typedef VEOS_base_sint32 (SMARTSERVICES_CDECL *SmartServices_DsMd_SetInstrumentPropertiesType)(VEOS_base_uint32 MemberIdx, VEOS_base_uint32 ObjectId, const char* pInstrumentName, const char* pUnit, VEOS_base_uint32 DataType);
typedef VEOS_base_sint32 (SMARTSERVICES_CDECL *SmartServices_DsMd_SetManeuverStartTimeType)(VEOS_base_float64 ManeuverStartTime);
typedef VEOS_base_sint32 (SMARTSERVICES_CDECL *SmartServices_DsMd_SetMemberNameType)(VEOS_base_uint32 MemberIdx, VEOS_base_uint32 ObjectId, const char* pMemberName, VEOS_base_uint32 MemberType);
typedef VEOS_base_sint32 (SMARTSERVICES_CDECL *SmartServices_DsMd_SetMemberPositionType)(VEOS_base_uint32 MemberIdx, const DsMDHomogPositionData* pPositionData);

#define VEOS_V54_SMARTSERVICES_DSMD_ABI_VERSION 0x0000000000000005

typedef struct {
    VEOS_base_uint64 AbiVersion;
    SmartServices_DsMd_InitInstrumentsType* SmartServices_DsMd_InitInstruments;
    SmartServices_DsMd_InitInterfaceType* SmartServices_DsMd_InitInterface;
    SmartServices_DsMd_ServiceType* SmartServices_DsMd_Service;
    SmartServices_DsMd_SetBroadcastType* SmartServices_DsMd_SetBroadcast;
    SmartServices_DsMd_SetInstrumentDataType* SmartServices_DsMd_SetInstrumentData;
    SmartServices_DsMd_SetInstrumentPropertiesType* SmartServices_DsMd_SetInstrumentProperties;
    SmartServices_DsMd_SetManeuverStartTimeType* SmartServices_DsMd_SetManeuverStartTime;
    SmartServices_DsMd_SetMemberNameType* SmartServices_DsMd_SetMemberName;
    SmartServices_DsMd_SetMemberPositionType* SmartServices_DsMd_SetMemberPosition;
} SmartServices_DsMd_ApiType;


//
// Real-Time Testing
//
typedef void (SMARTSERVICES_CDECL *SmartServices_DsRtt_ScriptServicePostComputationType)(void);
typedef void (SMARTSERVICES_CDECL *SmartServices_DsRtt_ScriptServicePreComputationType)(void);
typedef void (SMARTSERVICES_CDECL *SmartServices_DsRtt_SetAsyncType)(VEOS_base_sint32 Flag);

#define VEOS_V54_SMARTSERVICES_DSRTT_ABI_VERSION 0x0000000000000005

typedef struct {
    VEOS_base_uint64 AbiVersion;
    SmartServices_DsRtt_ScriptServicePostComputationType* SmartServices_DsRtt_ScriptServicePostComputation;
    SmartServices_DsRtt_ScriptServicePreComputationType* SmartServices_DsRtt_ScriptServicePreComputation;
    SmartServices_DsRtt_SetAsyncType* SmartServices_DsRtt_SetAsync;
} SmartServices_DsRtt_ApiType;


//
// DsModelAccess
//
typedef void (SMARTSERVICES_CDECL *SmartServices_DsModelAccessInitType)();
typedef void (SMARTSERVICES_CDECL *SmartServices_DsModelAccessReadType)(VEOS_base_uint32);
typedef void (SMARTSERVICES_CDECL *SmartServices_DsModelAccessWriteType)(VEOS_base_uint32);

#define VEOS_V54_SMARTSERVICES_DSMODELACCESS_ABI_VERSION 0x0005000100000001

typedef struct {
    VEOS_base_uint64 AbiVersion;
    const volatile VEOS_base_uint32** SmartServices_DsModelAccessEnabled;
    SmartServices_DsModelAccessInitType* SmartServices_DsModelAccessInit;
    SmartServices_DsModelAccessReadType* SmartServices_DsModelAccessRead;
    SmartServices_DsModelAccessWriteType* SmartServices_DsModelAccessWrite;
} SmartServices_DsModelAccess_ApiType;


//
// SmartServices API type definition.
//
#define VEOS_V54_SMARTSERVICES_ABI_VERSION 0x0005000400000000

typedef struct SmartServices_Api_s {
    VEOS_base_uint64 AbiVersion;
    const SmartServices_DsBusAccess_ApiType* DsBusAccessApi;
    const SmartServices_DsEcuIntByp_ApiType* DsEcuIntBypApi;
    const SmartServices_DsDaq_ApiType* DsDaqApi;
    const SmartServices_DsMd_ApiType* DsMdApi;
    const SmartServices_DsRtt_ApiType* DsRttApi;
    const SmartServices_DsModelAccess_ApiType* DsModelAccess;
} SmartServices_ApiType;


#ifdef VEOS_FRAME_V54_NAMESPACE_END
VEOS_FRAME_V54_NAMESPACE_END
#endif

#undef SMARTSERVICES_CDECL

#endif
