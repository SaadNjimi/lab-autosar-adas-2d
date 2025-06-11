/**************************************************************************************************\
 *** Sab_Callbacks.h
 ***
 *** Generic Simulator Abstraction configuration header file for callbacks.
 ***
 *** Copyright 2019 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef SAB_CALLBACKS_H
#define SAB_CALLBACKS_H

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/

#include "Sab_Cfg.h"
#include "Sab_Types.h"
#include "Std_Types.h"

/*------------------------------------------------------------------------------------------------*\
  DEFINES
\*------------------------------------------------------------------------------------------------*/

#define SAB_NOTIFICATION_TICKS_PER_SECOND  1000000000U

/*------------------------------------------------------------------------------------------------*\
  FUNCTION DECLARATIONS
\*------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif
#if 0
}
#endif

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_CreateNotification
 ***
 *** DESCRIPTION:
 ***     Registers a callback method and returns a unique notification ID pointer.
 ***
 *** PARAMETERS:
 ***     Type                           Name            Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     Sab_NotificationFunctionType   pFunction       Callback method to call after the RelAlarmTime
 ***                                                    has expired.
 ***     const char*                    pName           Used only for an error or warning message.
 ***                                                    The name does not have to be unique, can also
 ***                                                    be a nullpointer or empty string.
 ***                                                    The string buffer must be constant and persistent. 
 ***     Sab_NotificationIdType*        pId             Pointer that contains a unique notification ID.
 ***                                                    Maximum number of notification PID is limited to 65535 (2^16)-1
 *** RETURNS:
 ***     Sab_ReturnType                 SAB_E_OK or error code.
\**************************************************************************************************/
extern Sab_ReturnType Sab_CreateNotification(Sab_NotificationFunctionType pFunction, const char* pName, Sab_NotificationIdType* pId);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_StartRelTimeNotification
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
 ***     Sab_NotificationIdType   id              ID of the notification to be started.
 ***     Sab_NotificationTicksType offsetInTicks  Offset in ticks (ns) relative to the current time,
 ***                                              which indicates when the callback method is called.
 ***                                              Value range from 1 to (2^64)-1. A value of 0 is not allowed.
 ***     Sab_NotificationTicksType periodInTicks  Period in ticks (ns) relative to current time + offsetInTicks,
 ***                                              which specifies periodic rhythm the callback method is called.
 ***                                              Range of values from 0 to (2^64)-1.
 ***                                              A value of 0 means "one shot", greater than 0 is cyclic events.
 ***
 *** RETURNS:
 ***     Sab_ReturnType                           SAB_E_OK or error code.
\**************************************************************************************************/
extern Sab_ReturnType Sab_StartRelTimeNotification( Sab_NotificationIdType id, Sab_NotificationTicksType offsetInTicks, Sab_NotificationTicksType periodInTicks);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_CancelNotification
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
 ***     Sab_NotificationIdType       id           ID of the notification to be terminated.
 ***
 *** RETURNS:
 ***     Sab_ReturnType                            SAB_E_OK or error code.
\**************************************************************************************************/
extern Sab_ReturnType Sab_CancelNotification (Sab_NotificationIdType id);

#if 0
{
#endif
#ifdef __cplusplus
};
#endif

#endif /* SAB_CALLBACKS_H */

/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/
