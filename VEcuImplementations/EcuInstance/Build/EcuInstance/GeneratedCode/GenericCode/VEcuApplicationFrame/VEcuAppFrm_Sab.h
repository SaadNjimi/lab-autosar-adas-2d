/**************************************************************************************************\
 *** VEcuAppFrm_Sab.h
 ***
 *** Application frame declarations for Simulator abstraction module.
 ***
 *** Copyright 2019 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef VECUAPPFRM_SAB_H
#define VECUAPPFRM_SAB_H

/*------------------------------------------------------------------------------------------------*\
  DEFINES (OPT)
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
  DEFINES
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
 TYPEDEFS
\*------------------------------------------------------------------------------------------------*/
typedef void (* FunctionType)(void);

/*------------------------------------------------------------------------------------------------*\
  PARAMETERIZED MACROS
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
  VARIABLE DECLARATIONS
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
  FUNCTION PROTOTYPES
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
  VARIABLE DEFINITIONS
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
  INLINE FUNCTIONS
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
  FUNCTION DEFINITIONS
\*------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_EcuM_Init
 ***
 *** DESCRIPTION:
 ***     Function called when the simulator abstraction has no EcuM_Init function defined.
 ***
 *** PARAMETERS:
 ***     Type                    Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
extern void VEcuAppFrm_EcuM_Init(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_EcuM_ShutdownAll
 ***
 *** DESCRIPTION:
 ***     Function called when the simulator abstraction has no shutdown function defined.
 ***
 *** PARAMETERS:
 ***     Type                    Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
extern void VEcuAppFrm_EcuM_ShutdownAll(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_ModelStateEventBoot
 ***
 *** DESCRIPTION:
 ***     Event handler function called when the simulator abstraction is instructed by the model to
 ***     announce that the model is performing a start-up.
 ***
 *** PARAMETERS:
 ***     Type                    Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
extern void VEcuAppFrm_ModelStateEventBoot(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_ModelStateEventRun
 ***
 *** DESCRIPTION:
 ***     Event handler function called when the simulator abstraction is instructed by the model to
 ***     announce that the model is initialized completely and running.
 ***
 *** PARAMETERS:
 ***     Type                    Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
extern void VEcuAppFrm_ModelStateEventRun(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_ModelStateEventTerminate
 ***
 *** DESCRIPTION:
 ***     Event handler function called when the simulator abstraction is instructed by the model to
 ***     initiate the V-ECU shutdown.
 ***
 *** PARAMETERS:
 ***     Type                    Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
extern void VEcuAppFrm_ModelStateEventTerminate(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_ModelStateEventOff
 ***
 *** DESCRIPTION:
 ***     Event handler function called when the simulator abstraction is instructed by the model to
 ***     turn off the current V-ECU.
 ***
 *** PARAMETERS:
 ***     Type                    Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
extern void VEcuAppFrm_ModelStateEventOff(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_CreateNotification
 ***
 *** DESCRIPTION:
 ***     Registers a callback method and returns a unique notification ID pointer.
 ***
 *** PARAMETERS:
 ***     Type                           Name            Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     FunctionType                  pFunction        Callback method to call after the RelAlarmTime
 ***                                                    has expired.
 ***     const char*                   pName            Used only for an error or warning message.
 ***                                                    The name does not have to be unique, can also
 ***                                                    be a nullpointer or empty string.
 ***     uint32*                       pId              Pointer that contains a unique notification ID.
 ***                                                    Maximum number of notification PID is limited to 65535 (2^16)-1
 *** RETURNS:
 ***     Std_ReturnType                 SAB_E_OK or error code.
\**************************************************************************************************/
extern Std_ReturnType VEcuAppFrm_CreateNotification( FunctionType pFunction, const char* pName, uint32 * pId);

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_StartRelTimeNotification
 ***
 *** DESCRIPTION:
 ***     Specifies the ticks and starts the time after which a callback method should be called.
 ***     If the API is called a second time within the period, then the time is quasi restarted.
 ***     This applies to cyclic as well as to "one shot" events.
 ***
 ***     For conversion from a time to NotificationTicks, the SAB module provides
 ***     the define SAB_NOTIFICATION_TICKS_PER_SECOND.
 ***
 *** PARAMETERS:
 ***     Type                     Name            Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     uint32                  id              ID of the notification to be started.
 ***     uint64                  offsetInTicks   Offset in ticks (ns) relative to the current time,
 ***                                             which indicates when the callback method is called.
 ***                                             Value range from 1 to (2^64)-1. A value of 0 is not allowed.
 ***     uint64                  periodInTicks   Period in ticks (ns) relative to current time + offsetInTicks,
 ***                                             which specifies periodic rhythm the callback method is called.
 ***                                             Range of values from 0 to (2^64)-1.
 ***                                             A value of 0 means "one shot", greater than 0 is cyclic events.
 ***
 *** RETURNS:
 ***     Std_ReturnType                          SAB_E_OK or error code.
\**************************************************************************************************/
extern Std_ReturnType VEcuAppFrm_StartRelTimeNotification( uint32 id, uint64 offsetInTicks, uint64 periodInTicks);

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_CancelNotification
 ***
 *** DESCRIPTION:
 ***     If the API Sab_CancelNotification() is called within the time period, the notification is stopped.
 ***     If the time period in the combination "one Shot" (offsetInTicks + 0 * periodInTicks) has already
 ***     expired, the function returns the error "SAB_E_NOT_OK". The 'id' is not invalidated
 ***     with the call of Sab_CancelNotification(). With Sab_StartRelTimeNotification() the callback can
 ***     be activated again.
 ***
 *** PARAMETERS:
 ***     Type                     Name            Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     uint32                   id              ID of the notification to be terminated.
 ***
 *** RETURNS:
 ***     Std_ReturnType                           SAB_E_OK or error code.
\**************************************************************************************************/
extern Std_ReturnType VEcuAppFrm_CancelNotification (uint32 id);

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_NotificationEvent
 ***
 *** DESCRIPTION:
 ***     Calls a callback method.
 ***
 *** PARAMETERS:
 ***     Type                     Name            Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     FunctionType*                  pFunction       Callback method to call after the RelAlarmTime
 ***                                                    has expired.
 *** RETURNS:
 ***     Std_ReturnType                           SAB_E_OK or error code.
\**************************************************************************************************/
extern VEOS_ApiSuccessType VEcuAppFrm_NotificationEvent(const VEOS_SimApi_InterruptCallbackFunctionType pFunction);

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_EnsureInfLoopExperimentation
 ***
 *** DESCRIPTION:
 ***     During a sleep ccenarios real-time testing is called by the V-ECU ApplicationFrame
 ***     and is running in the regular time steps.
 ***
 *** PARAMETERS:
 ***     Type           Name               Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     uint8               justWakeup   V_ECU is just wakeup.
 *** RETURNS:
 ***     void
\**************************************************************************************************/
extern void VEcuAppFrm_EnsureInfLoopExperimentation(uint8 justWakeup);


/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_InterruptEvent
 ***
 *** DESCRIPTION:
 ***     Calls a callback interrupt function.
 ***
 *** PARAMETERS:
 ***     Type                     Name            Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     FunctionType*            pFunction       Pointer of an interrupt function
 ***
 *** RETURNS:
 ***     Std_ReturnType                           SAB_E_OK or error code.
\**************************************************************************************************/
extern VEOS_ApiSuccessType VEcuAppFrm_InterruptEvent(const VEOS_SimApi_InterruptCallbackFunctionType pFunction);


#ifdef __cplusplus
}
#endif

#endif /* VECUAPPFRM_SAB_H */

/*------------------------------------------------------------------------------------------------*\
 END OF FILE
\*------------------------------------------------------------------------------------------------*/
