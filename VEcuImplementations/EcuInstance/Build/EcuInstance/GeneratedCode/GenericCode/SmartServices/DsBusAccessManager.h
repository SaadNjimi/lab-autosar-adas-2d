/*
 * dSPACE bus access manager service interface.
 *
 * Copyright 2021, dSPACE GmbH. All rights reserved.
 */

#ifndef DSBUSACCESSMANAGER_H
#define DSBUSACCESSMANAGER_H

#include "SmartServices_Interaction.h"

#ifdef __cplusplus
extern "C"
{
#endif

extern SmartServices_DsBusAccess_EthernetReplayGetTimeInfoType gp_SmartServices_DsBam_EthernetReplayGetTimeInfo;
extern SmartServices_DsBusAccess_EthernetReplaySetStartTimeType gp_SmartServices_DsBam_EthernetReplaySetStartTime;
extern SmartServices_DsBusAccess_EthernetReplaySetStopTimeType gp_SmartServices_DsBam_EthernetReplaySetStopTime;
extern SmartServices_DsBusAccess_CANReplayGetTimeInfoType gp_SmartServices_DsBam_CANReplayGetTimeInfo;
extern SmartServices_DsBusAccess_CANReplaySetStartTimeType gp_SmartServices_DsBam_CANReplaySetStartTime;
extern SmartServices_DsBusAccess_CANReplaySetStopTimeType gp_SmartServices_DsBam_CANReplaySetStopTime;

#define DsBusAccessRP_EthernetReplayGetTimeInfo(AdapterIdx, pStartTs_sec, pStartTs_ns, pStopTs_sec, pStopTs_ns) \
    gp_SmartServices_DsBam_EthernetReplayGetTimeInfo(AdapterIdx, pStartTs_sec, pStartTs_ns, pStopTs_sec, pStopTs_ns)

#define DsBusAccessRP_EthernetReplaySetStartTime(AdapterIdx, StartTs_sec, UStartTs_ns) \
    gp_SmartServices_DsBam_EthernetReplaySetStartTime(AdapterIdx, StartTs_sec, UStartTs_ns)

#define DsBusAccessRP_EthernetReplaySetStopTime(AdapterIdx, StopTs_sec, UStopTs_ns) \
    gp_SmartServices_DsBam_EthernetReplaySetStopTime(AdapterIdx, StopTs_sec, UStopTs_ns)

#define DsBusAccessRP_CANReplayGetTimeInfo(DriverHandle, pStartTs_sec, pStartTs_ns, pStopTs_sec, pStopTs_ns) \
    gp_SmartServices_DsBam_CANReplayGetTimeInfo(DriverHandle, pStartTs_sec, pStartTs_ns, pStopTs_sec, pStopTs_ns)

#define DsBusAccessRP_CANReplaySetStartTime(DriverHandle, StartTs_sec, UStartTs_ns) \
    gp_SmartServices_DsBam_CANReplaySetStartTime(DriverHandle, StartTs_sec, UStartTs_ns)

#define DsBusAccessRP_CANReplaySetStopTime(DriverHandle, StopTs_sec, UStopTs_ns) \
    gp_SmartServices_DsBam_CANReplaySetStopTime(DriverHandle, StopTs_sec, UStopTs_ns)

#ifdef __cplusplus
}
#endif

#endif
