/**************************************************************************************************\
 *** 
 *** System              : System
 *** ECU                 : EcuInstance
 *** Configuration       : EcuConfiguration_EcuInstance
 *** Codefile            : EcuM.h
 ***
 *** Generated by SystemDesk 5.6.0.13023
 *** Generation date: 2025-06-11 10:13:54
 ***
 *** Copyright (c) 2018 dSPACE GmbH
\**************************************************************************************************/

#ifndef ECUM_H
#define ECUM_H

/*------------------------------------------------------------------------------------------------*\
  DEFINES (OPT)
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/
#include "Std_Types.h"
#include "EcuM_Cfg.h"
#include "Os.h"

/*------------------------------------------------------------------------------------------------*\
    Default Errors                                                                         
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
    Development Errors                                                                         
\*------------------------------------------------------------------------------------------------*/
#define ECUM_E_NOT_INITIALIZED                  (0x10)
#define ECUM_E_SERVICE_DISABLED                 (0x11)
#define ECUM_E_NULL_POINTER                     (0x12)
#define ECUM_E_INVALID_PAR                      (0x13)
#define ECUM_E_MULTIPLE_RUN_REQUESTS            (0x14)
#define ECUM_E_MISMATCHED_RUN_RELEASE           (0x15)
#define ECUM_E_STATE_PAR_OUT_OF_RANGE           (0x16)
#define ECUM_E_UNKNOWN_WAKEUP_SOURCE            (0x17)
#define ECUM_E_RAM_CHECK_FAILED                 (0x18)
#define ECUM_E_ALL_RUN_REQUESTS_KILLED          (0x19)
#define ECUM_E_CONFIGURATION_DATA_INCONSISTENT  (0x1a)
#define ECUM_E_TIMEOUT                          (0x1b)

/*------------------------------------------------------------------------------------------------*\
    Runtime Errors                                                                         
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
    Transient Faults                                                                         
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
  PARAMETERIZED MACROS
\*------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */ 

/*------------------------------------------------------------------------------------------------*\
  TYPEDEFS
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
  VARIABLE DECLARATIONS
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
  FUNCTION PROTOTYPES
\*------------------------------------------------------------------------------------------------*/
/**************************************************************************************************\
 *** FUNCTION:
 ***     EcuM_MainFunction
 *** 
 *** DESCRIPTION:
 ***     The purpose of this service is to implement all activities of the ECU State Manager
 ***     while the OS is up and running.
 ***     
 ***     ServiceID  : 0x18
 ***     Reentrant  : false
 ***
 *** PARAMETERS:
 ***     -/-
 *** 
 *** RETURNS:
 ***    void
\**************************************************************************************************/
extern void EcuM_MainFunction(void);


/**************************************************************************************************\
 *** FUNCTION:
 ***     EcuM_ClearWakeupEvent
 *** 
 *** DESCRIPTION:
 ***     Clears wakeup events.
 ***     
 ***     ServiceID  : 0x16
 ***     Reentrant  : false
 ***
 *** PARAMETERS:
 ***    Type           Name   Description
 ***    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***	EcuM_WakeupSourceType sources Events to be cleared 
 *** 
 *** RETURNS:
 ***    void
\**************************************************************************************************/
extern void EcuM_ClearWakeupEvent(EcuM_WakeupSourceType sources);

/**************************************************************************************************\
 *** FUNCTION:
 ***     EcuM_GetBootTarget
 *** 
 *** DESCRIPTION:
 ***     EcuM_GetBootTarget returns the current boot target - see EcuM_SelectBootTarget.
 ***     EcuM_GetBootTarget is part of the ECU Manager Module port interface.
 ***     
 ***     ServiceID  : 0x13
 ***     Reentrant  : true
 ***
 *** PARAMETERS:
 ***    Type           Name   Description
 ***    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***	EcuM_BootTargetType* target The currently selected boot target. 
 *** 
 *** RETURNS:
 ***    Std_ReturnType
\**************************************************************************************************/
extern Std_ReturnType EcuM_GetBootTarget(EcuM_BootTargetType* target);

/**************************************************************************************************\
 *** FUNCTION:
 ***     EcuM_GetExpiredWakeupEvents
 *** 
 *** DESCRIPTION:
 ***     Gets expired wakeup events.
 ***     
 ***     ServiceID  : 0x19
 ***     Reentrant  : false
 ***
 *** PARAMETERS:
 ***     -/-
 *** 
 *** RETURNS:
 ***    EcuM_WakeupSourceType
\**************************************************************************************************/
extern EcuM_WakeupSourceType EcuM_GetExpiredWakeupEvents(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     EcuM_GetLastShutdownTarget
 *** 
 *** DESCRIPTION:
 ***     EcuM_GetLastShutdownTarget returns the shutdown target of the previous shutdown
 ***     process. EcuM_GetLastShutdownTarget is part of the ECU Manager Module port
 ***     interface.
 ***     
 ***     ServiceID  : 0x08
 ***     Reentrant  : true
 ***
 *** PARAMETERS:
 ***    Type           Name   Description
 ***    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***	EcuM_StateType* shutdownTarget One of these values is returned: *      ECUM_STATE_SLEEP *      ECUM_STATE_RESET *
 ***     ECUM_STATE_OFF
 ***	uint8* sleepMode If the out parameter \"shutdownTarget\" is ECUM_STATE_SLEEP, sleepMode tells which of
 ***     the configured sleep modes was actually chosen. If \"shutdownTarget\" is
 ***     ECUM_STATE_RESET, sleepMode tells which of the configured reset modes was
 ***     actually chosen. 
 *** 
 *** RETURNS:
 ***    Std_ReturnType
\**************************************************************************************************/
extern Std_ReturnType EcuM_GetLastShutdownTarget(EcuM_StateType* shutdownTarget, uint8* sleepMode);

/**************************************************************************************************\
 *** FUNCTION:
 ***     EcuM_GetPendingWakeupEvents
 *** 
 *** DESCRIPTION:
 ***     Gets pending wakeup events.
 ***     
 ***     ServiceID  : 0x0d
 ***     Reentrant  : false
 ***
 *** PARAMETERS:
 ***     -/-
 *** 
 *** RETURNS:
 ***    EcuM_WakeupSourceType
\**************************************************************************************************/
extern EcuM_WakeupSourceType EcuM_GetPendingWakeupEvents(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     EcuM_GetShutdownTarget
 *** 
 *** DESCRIPTION:
 ***     EcuM_GetShutdownTarget returns the currently selected shutdown target as set by
 ***     EcuM_SelectShutdownTarget. EcuM_GetShutdownTarget is part of the ECU Manager
 ***     Module port interface.
 ***     
 ***     ServiceID  : 0x09
 ***     Reentrant  : true
 ***
 *** PARAMETERS:
 ***    Type           Name   Description
 ***    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***	EcuM_StateType* shutdownTarget One of these values is returned: *      ECUM_STATE_SLEEP *      ECUM_STATE_RESET *
 ***     ECUM_STATE_OFF
 ***	uint8* sleepMode If the out parameter \"shutdownTarget\" is ECUM_STATE_SLEEP, sleepMode tells which of
 ***     the configured sleep modes was actually chosen. If \"shutdownTarget\" is
 ***     ECUM_STATE_RESET, sleepMode tells which of the configured reset modes was
 ***     actually chosen. 
 *** 
 *** RETURNS:
 ***    Std_ReturnType
\**************************************************************************************************/
extern Std_ReturnType EcuM_GetShutdownTarget(EcuM_StateType* shutdownTarget, uint8* sleepMode);

/**************************************************************************************************\
 *** FUNCTION:
 ***     EcuM_GetState
 *** 
 *** DESCRIPTION:
 ***     Gets a state.
 ***     
 ***     ServiceID  : 0x07
 ***     Reentrant  : true
 ***
 *** PARAMETERS:
 ***    Type           Name   Description
 ***    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***	EcuM_StateType* state The value of the internal state variable. 
 *** 
 *** RETURNS:
 ***    Std_ReturnType
\**************************************************************************************************/
extern Std_ReturnType EcuM_GetState(EcuM_StateType* state);

/**************************************************************************************************\
 *** FUNCTION:
 ***     EcuM_GetStatusOfWakeupSource
 *** 
 *** DESCRIPTION:
 ***     The sum status shall be computed according to the following algorithm: If
 ***     (EcuM_GetValidatedWakeupEvents() AND sources) is not 0 then return
 ***     ECUM_WKSTATUS_VALIDATED. If (EcuM_GetPendingWakeupEvents() AND sources) is
 ***     not 0 then return ECUM_WKSTATUS_PENDING. If (EcuM_GetExpiredWakeupEvents()
 ***     AND sources) is not 0 then return ECUM_WKSTATUS_EXPIRED. Otherwise, return
 ***     ECUM_WKSTATUS_NONE.
 ***     
 ***     ServiceID  : 0x17
 ***     Reentrant  : true
 ***
 *** PARAMETERS:
 ***    Type           Name   Description
 ***    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***	EcuM_WakeupSourceType sources The sources for which the status is returned 
 *** 
 *** RETURNS:
 ***    EcuM_WakeupStatusType
\**************************************************************************************************/
extern EcuM_WakeupStatusType EcuM_GetStatusOfWakeupSource(EcuM_WakeupSourceType sources);

/**************************************************************************************************\
 *** FUNCTION:
 ***     EcuM_GetValidatedWakeupEvents
 *** 
 *** DESCRIPTION:
 ***     Gets validated wakeup events.
 ***     
 ***     ServiceID  : 0x15
 ***     Reentrant  : false
 ***
 *** PARAMETERS:
 ***     -/-
 *** 
 *** RETURNS:
 ***    EcuM_WakeupSourceType
\**************************************************************************************************/
extern EcuM_WakeupSourceType EcuM_GetValidatedWakeupEvents(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     EcuM_Init
 *** 
 *** DESCRIPTION:
 ***     Initializes the ECU state manager and carries out the startup procedure. The function
 ***     will never return (it calls StartOS)
 ***     
 ***     ServiceID  : 0x01
 ***     Reentrant  : true
 ***
 *** PARAMETERS:
 ***     -/-
 *** 
 *** RETURNS:
 ***    void
\**************************************************************************************************/
extern void EcuM_Init(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     EcuM_KillAllPostRUNRequests
 *** 
 *** DESCRIPTION:
 ***     This function unconditionally releases all pending requests to PostRUN.
 ***     
 ***     ServiceID  : 0x2a
 ***     Reentrant  : true
 ***
 *** PARAMETERS:
 ***     -/-
 *** 
 *** RETURNS:
 ***    void
\**************************************************************************************************/
extern void EcuM_KillAllPostRUNRequests(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     EcuM_KillAllRUNRequests
 *** 
 *** DESCRIPTION:
 ***     The benefit of this function over an ECU reset is that the shutdown sequence is
 ***     executed, which e.g. takes care of writing back NV memory contents.
 ***     
 ***     ServiceID  : 0x05
 ***     Reentrant  : true
 ***
 *** PARAMETERS:
 ***     -/-
 *** 
 *** RETURNS:
 ***    void
\**************************************************************************************************/
extern void EcuM_KillAllRUNRequests(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     EcuM_ReleasePOST_RUN
 *** 
 *** DESCRIPTION:
 ***     Releases a POST RUN request previously done with a call to EcuM_RequestPOST_RUN. The
 ***     service is intended for implementing AUTOSAR ports.
 ***     
 ***     ServiceID  : 0x0b
 ***     Reentrant  : true
 ***
 *** PARAMETERS:
 ***    Type           Name   Description
 ***    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***	EcuM_UserType user ID of the entity releasing the POST RUN state. 
 *** 
 *** RETURNS:
 ***    Std_ReturnType
\**************************************************************************************************/
extern Std_ReturnType EcuM_ReleasePOST_RUN(EcuM_UserType user);

/**************************************************************************************************\
 *** FUNCTION:
 ***     EcuM_ReleaseRUN
 *** 
 *** DESCRIPTION:
 ***     Releases a RUN request previously done with a call to EcuM_RequestRUN. The service is
 ***     intended for implementing AUTOSAR ports.
 ***     
 ***     ServiceID  : 0x04
 ***     Reentrant  : true
 ***
 *** PARAMETERS:
 ***    Type           Name   Description
 ***    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***	EcuM_UserType user ID of the entity releasing the RUN state. 
 *** 
 *** RETURNS:
 ***    Std_ReturnType
\**************************************************************************************************/
extern Std_ReturnType EcuM_ReleaseRUN(EcuM_UserType user);

/**************************************************************************************************\
 *** FUNCTION:
 ***     EcuM_RequestPOST_RUN
 *** 
 *** DESCRIPTION:
 ***     Places a request for the POST RUN state. Requests can be placed by every user made
 ***     known to the state manager at configuration time. Requests for RUN and POST
 ***     RUN must be tracked independently (in other words: two independent
 ***     variables). The service is intended for implementing AUTOSAR ports.
 ***     
 ***     ServiceID  : 0x0a
 ***     Reentrant  : true
 ***
 *** PARAMETERS:
 ***    Type           Name   Description
 ***    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***	EcuM_UserType user ID of the entity requesting the POST RUN state. 
 *** 
 *** RETURNS:
 ***    Std_ReturnType
\**************************************************************************************************/
extern Std_ReturnType EcuM_RequestPOST_RUN(EcuM_UserType user);

/**************************************************************************************************\
 *** FUNCTION:
 ***     EcuM_RequestRUN
 *** 
 *** DESCRIPTION:
 ***     Places a request for the RUN state. Requests can be placed by every user made known
 ***     to the state manager at configuration time.
 ***     
 ***     ServiceID  : 0x03
 ***     Reentrant  : true
 ***
 *** PARAMETERS:
 ***    Type           Name   Description
 ***    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***	EcuM_UserType user ID of the entity requesting the RUN state. 
 *** 
 *** RETURNS:
 ***    Std_ReturnType
\**************************************************************************************************/
extern Std_ReturnType EcuM_RequestRUN(EcuM_UserType user);

/**************************************************************************************************\
 *** FUNCTION:
 ***     EcuM_SelectBootTarget
 *** 
 *** DESCRIPTION:
 ***     EcuM_SelectBootTarget selects a boot target. EcuM_SelectBootTarget is part of the ECU
 ***     Manager Module port interface.
 ***     
 ***     ServiceID  : 0x12
 ***     Reentrant  : true
 ***
 *** PARAMETERS:
 ***    Type           Name   Description
 ***    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***	EcuM_BootTargetType target The selected boot target. 
 *** 
 *** RETURNS:
 ***    Std_ReturnType
\**************************************************************************************************/
extern Std_ReturnType EcuM_SelectBootTarget(EcuM_BootTargetType target);

/**************************************************************************************************\
 *** FUNCTION:
 ***     EcuM_SelectShutdownTarget
 *** 
 *** DESCRIPTION:
 ***     EcuM_SelectShutdownTarget selects the shutdown target. EcuM_SelectShutdownTarget is
 ***     part of the ECU Manager Module port interface.
 ***     
 ***     ServiceID  : 0x06
 ***     Reentrant  : true
 ***
 *** PARAMETERS:
 ***    Type           Name   Description
 ***    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***	EcuM_StateType target The selected shutdown target.
 ***	uint8 mode The identfier of a sleep mode (if target is ECUM_STATE_SLEEP) or a reset mechanism
 ***     (if target is ECUM_STATE_RESET) as defined by configuration. 
 *** 
 *** RETURNS:
 ***    Std_ReturnType
\**************************************************************************************************/
extern Std_ReturnType EcuM_SelectShutdownTarget(EcuM_StateType target, uint8 mode);

/**************************************************************************************************\
 *** FUNCTION:
 ***     EcuM_Shutdown
 *** 
 *** DESCRIPTION:
 ***     Typically called from the shutdown hook, this function takes over execution control
 ***     and will carry out GO OFF II activities.
 ***     
 ***     ServiceID  : 0x02
 ***     Reentrant  : true
 ***
 *** PARAMETERS:
 ***     -/-
 *** 
 *** RETURNS:
 ***    void
\**************************************************************************************************/
extern void EcuM_Shutdown(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     EcuM_StartupTwo
 *** 
 *** DESCRIPTION:
 ***     This function implements the STARTUP II state.
 ***     
 ***     ServiceID  : 0x1a
 ***     Reentrant  : false
 ***
 *** PARAMETERS:
 ***     -/-
 *** 
 *** RETURNS:
 ***    void
\**************************************************************************************************/
extern void EcuM_StartupTwo(void);


#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */ 

#endif /* ECUM_H */
/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/
