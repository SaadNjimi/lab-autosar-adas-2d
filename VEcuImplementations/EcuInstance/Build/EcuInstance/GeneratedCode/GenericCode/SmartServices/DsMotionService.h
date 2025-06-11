/*
 * MotionService real time interface.
 *
 * Copyright 2020, dSPACE GmbH. All rights reserved.
 */

#ifndef DSCMOTIONSERVICE_H
#define DSCMOTIONSERVICE_H

#include "SmartServices_Interaction.h"


#define DSCNMD_NO_ERROR                   0
#define DSCNMD_OUT_OF_RANGE              -1101
#define DSCNMD_OUT_OF_MEMORY             -1102
#define DSCNMD_NOT_INITIALIZED           -1104
#define DSCNMD_WRONG_MEMBER_TYPE         -1105
#define DSCNMD_UDP_SOCKET_ERROR          -1106
#define DSCNMD_NO_HOST_ADDRESS_AVAILABLE -1107
#define DSCNMD_SHARED_MEMORY_ERROR       -1108
#define DSCNMD_ANIMATION_NOT_STARTED     -1111
#define DSCNMD_TOO_MANY_MEMBERS          -1112
#define DSCNMD_MEMBERS_INIT_ERROR        -1113
#define DSCNMD_INVALID_BC_STATE          -1114
#define DSCNMD_INVALID_DATA              -1115
#define DSCNMD_INVALID_DATA_TYPE         -1116
#define DSCNMD_INSTR_PROPERTIES_ERROR    -1117

/* Stream Types */
#define DS_MD_STREAM_TYPE_BODY        0
#define DS_MD_STREAM_TYPE_INSTRUMENT  1

/* Instrument Data Types */
#define DS_MD_INSTR_DATA_TYPE_INT8      1
#define DS_MD_INSTR_DATA_TYPE_INT16     2
#define DS_MD_INSTR_DATA_TYPE_INT32     3
#define DS_MD_INSTR_DATA_TYPE_INT64     4
#define DS_MD_INSTR_DATA_TYPE_LONG64    5
#define DS_MD_INSTR_DATA_TYPE_UINT8     6
#define DS_MD_INSTR_DATA_TYPE_UINT16    7
#define DS_MD_INSTR_DATA_TYPE_UINT32    8
#define DS_MD_INSTR_DATA_TYPE_UINT64    9
#define DS_MD_INSTR_DATA_TYPE_ULONG64   10
#define DS_MD_INSTR_DATA_TYPE_FLT32     11
#define DS_MD_INSTR_DATA_TYPE_FLT64     12
#define DS_MD_INSTR_DATA_TYPE_BOOL      13

/* Broadcast modes */
#define DS_MD_BROADCAST_SUPPORTED       1
#define DS_MD_BROADCAST_NOT_SUPPORTED   0


#ifdef __cplusplus
extern "C" {
#endif

extern SmartServices_DsMd_InitInstrumentsType gp_SmartServices_DsMd_InitInstruments;
extern SmartServices_DsMd_InitInterfaceType gp_SmartServices_DsMd_InitInterface;
extern SmartServices_DsMd_ServiceType gp_SmartServices_DsMd_Service;
extern SmartServices_DsMd_SetBroadcastType gp_SmartServices_DsMd_SetBroadcast;
extern SmartServices_DsMd_SetInstrumentDataType gp_SmartServices_DsMd_SetInstrumentData;
extern SmartServices_DsMd_SetInstrumentPropertiesType gp_SmartServices_DsMd_SetInstrumentProperties;
extern SmartServices_DsMd_SetManeuverStartTimeType gp_SmartServices_DsMd_SetManeuverStartTime;
extern SmartServices_DsMd_SetMemberNameType gp_SmartServices_DsMd_SetMemberName;
extern SmartServices_DsMd_SetMemberPositionType gp_SmartServices_DsMd_SetMemberPosition;

#ifdef __cplusplus
}
#endif

#define md_init_interface(numberOfMembers) gp_SmartServices_DsMd_InitInterface(numberOfMembers)
#define md_init_instruments(numberOfInstruments) gp_SmartServices_DsMd_InitInstruments(numberOfInstruments)
#define md_service(simTime) gp_SmartServices_DsMd_Service(simTime)
#define md_set_member_name(memberIdx, objId, memberName, memberType) gp_SmartServices_DsMd_SetMemberName(memberIdx, objId, memberName, memberType)
#define md_set_member_position(memberIdx, positionData) gp_SmartServices_DsMd_SetMemberPosition(memberIdx, positionData)
#define md_set_instrument_properties(memIdx, objId, inst, unit, dt) gp_SmartServices_DsMd_SetInstrumentProperties(memIdx, objId, inst, unit, dt)
#define md_set_instrument_data(memberIdx, value) gp_SmartServices_DsMd_SetInstrumentData(memberIdx, value)
#define md_set_maneuver_start_time(maneuverStartTime) gp_SmartServices_DsMd_SetManeuverStartTime(maneuverStartTime)
#define md_set_broadcast(state) gp_SmartServices_DsMd_SetBroadcast(state)

#endif
