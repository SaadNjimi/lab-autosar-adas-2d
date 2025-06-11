/**************************************************************************************************\
 *** Sab.h
 ***
 *** Simulator Abstraction interface header file.
 ***
 *** Copyright 2019 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef SAB_H
#define SAB_H

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/

#include "Sab_Cfg.h"
#include "Sab_Types.h"
#include "Sab_Callbacks.h"

#include "Sab_Obsolete.h"



/*------------------------------------------------------------------------------------------------*\
  DEFINES
\*------------------------------------------------------------------------------------------------*/
/* Error code: No error occurred */
#define SAB_E_OK            E_OK
/* Error code: Unspecified error occurred */
#define SAB_E_NOT_OK        E_NOT_OK
/* Error code: Invalid Id */
#define SAB_E_INVALID_ID    (Std_ReturnType)0x02
/* Error code: Invalid Id */
#define SAB_E_INVALID_TIME  (Std_ReturnType)0x03




#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#if 0
}
#endif



/*------------------------------------------------------------------------------------------------*\
  VARIABLE DEFINITIONS
\*------------------------------------------------------------------------------------------------*/

/* The value of Sab_PredefTimer_StartSimulation_1ns64bit is defined from start to stop of the simulation */
/* Only the GetTimeSpan() method may be called for this timer instance, but not Reset(), etc. otherwise the reference time would be moved. */
extern Sab_PredefTimer1ns64bitType Sab_PredefTargetTimer_StartSimulation_1ns64bit;

/* The value of Sab_PredefTimer_StartOs_1ns64bit is undefined until StartOs() has been called */
/* Only the GetTimeSpan() method may be called for this timer instance, but not Reset(), etc. otherwise the reference time would be moved. */
extern Sab_PredefTimer1ns64bitType Sab_PredefTargetTimer_StartOs_1ns64bit;



/*------------------------------------------------------------------------------------------------*\
  FUNCTION PROTOTYPES
\*------------------------------------------------------------------------------------------------*/


/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_Init
 ***
 *** DESCRIPTION:
 ***     Initialization function for the simulator abstraction layer.
 ***     Sab_Init() is called automatically by the simulation platform before the 
 ***     application code is executed.
 ***
 *** PARAMETERS:
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
extern void Sab_Init(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_DeInit
 ***
 *** DESCRIPTION:
 ***     De-Initialization function for the simulator abstraction layer.
 ***     Sab_DeInit() is called automatically by the simulation platform after the 
 ***     application code is finished.
 ***
 *** PARAMETERS:
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
extern void Sab_DeInit(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_Submit
 ***
 *** DESCRIPTION:
 ***     Submits a formatted message with severity Error (without opening a dialog).
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     Sab_MsgSeverity       severity     The message severity. This will specify the importance
 ***                                        of the submitted message (from log file up to error message)
 ***     const char *          format       The formatted message text - may contain simple format strings.
 ***     var args              ...          Optional formatter arguments.
 ***
 *** RETURNS:
 ***     Sab_ReturnType
 ***            SAB_E_OK            If the message has been submitted successfully.
 ***            SAB_E_NOT_OK        If the message has not been submitted or not submitted as desired.
\**************************************************************************************************/
extern Sab_ReturnType Sab_Submit(Sab_MsgSeverity Severity, const char* pFormat, ...);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_SubmitLog
 ***
 *** DESCRIPTION:
 ***     Submits a formatted message to the simulator log file.
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     const char *          format       The formatted message text.
 ***     var args              ...          Optional formatter arguments.
 ***
 *** RETURNS:
 ***     Sab_ReturnType
 ***            SAB_E_OK            If the message has been submitted successfully.
 ***            SAB_E_NOT_OK        If the message has not been submitted or not submitted as desired.
\**************************************************************************************************/
extern Sab_ReturnType Sab_SubmitLog(const char* pFormat, ...);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_SubmitInfo
 ***
 *** DESCRIPTION:
 ***     Submits a formatted message with severity Info.
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     const char *          format       The formatted message text.
 ***     var args              ...          Optional formatter arguments.
 ***
 *** RETURNS:
 ***     Sab_ReturnType
 ***            SAB_E_OK            If the message has been submitted successfully.
 ***            SAB_E_NOT_OK        If the message has not been submitted or not submitted as desired.
\**************************************************************************************************/
extern Sab_ReturnType Sab_SubmitInfo(const char* pFormat, ...);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_SubmitWarning
 ***
 *** DESCRIPTION:
 ***     Submits a formatted message with severity Warning (without opening a dialog).
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     const char *          format       The formatted message text.
 ***     var args              ...          Optional formatter arguments.
 ***
 *** RETURNS:
 ***     Sab_ReturnType
 ***            SAB_E_OK            If the message has been submitted successfully.
 ***            SAB_E_NOT_OK        If the message has not been submitted or not submitted as desired.
\**************************************************************************************************/
extern Sab_ReturnType Sab_SubmitWarning(const char* pFormat, ...);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_SubmitError
 ***
 *** DESCRIPTION:
 ***     Submits a formatted message with severity Error (without opening a dialog).
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     const char *          format       The formatted message text.
 ***     var args              ...          Optional formatter arguments.
 ***
 *** RETURNS:
 ***     Sab_ReturnType
 ***            SAB_E_OK            If the message has been submitted successfully.
 ***            SAB_E_NOT_OK        If the message has not been submitted or not submitted as desired.
\**************************************************************************************************/
extern Sab_ReturnType Sab_SubmitError(const char* pFormat, ...);


/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_ResetTimer1ns64bit
 ***
 *** DESCRIPTION:
 ***     Resets the given timer instance. This means, that the timer instance is set 
 ***     to the current hardware timer value. The time value is scaled to 1 nano second 
 ***     and a precision of 64 bit. Thus, this timer will overflow after 18446744073,709551616 seconds (~583 years). 
 ***     The actual resolution of the time value depends on the underlying hardware timer and 
 ***     may be significantly greater than 1 ns.
 ***
 *** PARAMETERS:
 ***     Type                               Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     Sab_PredefTimer1ns64bitType *      TimerPtr     The pointer to the timer to reset (In-Out value).
 ***
 *** RETURNS:
 ***     Sab_ReturnType
 ***            SAB_E_OK            If the timer has been reset successfully.
 ***            SAB_E_NOT_OK        If an error has occurred during the time evaluation.
\**************************************************************************************************/
extern Sab_ReturnType Sab_ResetTimer1ns64bit(Sab_PredefTimer1ns64bitType* TimerPtr);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_GetTimeSpan1ns64bit
 ***
 *** DESCRIPTION:
 ***     Calculates the difference between the current time and the reference time stored 
 ***     at the given timer instance. The time value is scaled to 1 nano second and a precision 
 ***     of 64 bit. Thus, this timer will overflow after 18446744073,709551616 seconds (~583 years). 
 ***     The actual resolution of the time value depends on the underlying hardware timer 
 ***     and may be significantly greater than 1 ns.
 ***
 *** PARAMETERS:
 ***     Type                               Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     Sab_PredefTimer1ns64bitType *      TimerPtr     The pointer to the timer to use as basic (In-Value).
 ***     Sab_TickType1ns64bit *             TimeSpanPtr  The time span calculated (Out-Parameter)
 ***
 *** RETURNS:
 ***     Sab_ReturnType
 ***            SAB_E_OK            If the timespan has been been calculated successfully.
 ***            SAB_E_NOT_OK        If an error has occurred during the derivation of the time.
\**************************************************************************************************/
extern Sab_ReturnType Sab_GetTimeSpan1ns64bit(const Sab_PredefTimer1ns64bitType* TimerPtr, Sab_TickType1ns64bit* TimeSpanPtr);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_ShiftTimer1ns64bit
 ***
 *** DESCRIPTION:
 ***     The ShiftTimer functions shall perform proper wrap-around handling at adding (reference time + TimeValue), 
 ***     if the sum is greater than the maximum value of the timer. Shifts the reference time stored at the given 
 ***     timer instance by the given time value. The time value is scaled to 1 nano second and a precision of 64 bit. 
 ***     Thus, this timer will overflow after 18446744073,709551616 seconds (~583 years).
 ***
 *** PARAMETERS:
 ***     Type                               Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     Sab_PredefTimer1ns64bitType *      TimerPtr     The pointer to the timer to shift (InOut-Value).
 ***     Sab_TickType1ns64bit               TimeValue    The time span to shift the timer with. (In-Value)
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
extern void Sab_ShiftTimer1ns64bit(Sab_PredefTimer1ns64bitType* TimerPtr, Sab_TickType1ns64bit TimeValue);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_SyncTimer1ns64bit
 ***
 *** DESCRIPTION:
 ***     Synchronizes two timer instances. This means, that the reference time of the destination 
 ***     timer instance is set to the reference time of the source timer instance. The time values are scaled 
 ***     to 1 nano second and a precision of 64 bit. Thus, this timer will overflow 
 ***     after 18446744073,709551616 seconds (~583 years).
 ***
 *** PARAMETERS:
 ***     Type                               Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     Sab_PredefTimer1ns64bitType *      TimerDstPtr    The pointer to the destination timer (InOut-Value).
 ***     Sab_PredefTimer1ns64bitType *      TimerSrcPtr    The pointer to the source timer (In-Value).
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
extern void Sab_SyncTimer1ns64bit(Sab_PredefTimer1ns64bitType* TimerDstPtr, const Sab_PredefTimer1ns64bitType* TimerSrcPtr);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_BusyWait1ns64bit
 ***
 *** DESCRIPTION:
 ***     Performs busy waiting by polling with a guaranteed minimum waiting time. 
 ***     The waiting time is specified with a resolution of 1 nano second and a precision of 64 bit. 
 ***     Thus, the maximum waiting time is 18446744073,709551616 seconds (~583 years). Due to the polling approach, 
 ***     the actual waiting time may be greater than the desired waiting time.
 ***
 *** PARAMETERS:
 ***     Type           Name               Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     Sab_TickType1ns32bit       WaitingTimeMin     The number of nano seconds to wait (In-Value).
 ***
 *** RETURNS:
 ***     Sab_ReturnType
 ***            SAB_E_OK            If the busy waiting has succeeded and the application has
 ***                                been waiting for at least the specified time.
 ***            SAB_E_NOT_OK        If an error has occurred during the time evaluation.
\**************************************************************************************************/
extern Sab_ReturnType Sab_BusyWait1ns64bit(Sab_TickType1ns32bit WaitingTimeMin);


/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_ResetTargetTimer1ns64bit
 ***
 *** DESCRIPTION:
 ***     Resets the given timer instance. This means, that the timer instance is set 
 ***     to the current timer value. The time value is scaled to 1 nano second 
 ***     and a precision of 64 bit. Thus, this timer will overflow after 18446744073,709551616 seconds (~583 years). 
 ***     The actual resolution of the time value depends on the underlying timer and 
 ***     may be significantly greater than 1 ns.
 ***
 *** PARAMETERS:
 ***     Type                               Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     Sab_PredefTimer1ns64bitType *      TimerPtr     The pointer to the timer to reset (In-Out value).
 ***
 *** RETURNS:
 ***     Sab_ReturnType
 ***            SAB_E_OK            If the timer has been reset successfully.
 ***            SAB_E_NOT_OK        If an error has occurred during the time evaluation.
\**************************************************************************************************/
extern Sab_ReturnType Sab_ResetTargetTimer1ns64bit(Sab_PredefTimer1ns64bitType* TimerPtr);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_GetTargetTimeSpan1ns64bit
 ***
 *** DESCRIPTION:
 ***     Calculates the difference between the current time and the reference time stored 
 ***     at the given timer instance. The time value is scaled to 1 nano second and a precision 
 ***     of 64 bit. Thus, this timer will overflow after 18446744073,709551616 seconds (~583 years). 
 ***     The actual resolution of the time value depends on the underlying timer 
 ***     and may be significantly greater than 1 ns.
 ***
 *** PARAMETERS:
 ***     Type                               Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     Sab_PredefTimer1ns64bitType *      TimerPtr     The pointer to the timer to use as basic (In-Value).
 ***     Sab_TickType1ns64bit *             TimeSpanPtr  The time span calculated (Out-Parameter)
 ***
 *** RETURNS:
 ***     Sab_ReturnType
 ***            SAB_E_OK            If the timespan has been been calculated successfully.
 ***            SAB_E_NOT_OK        If an error has occurred during the derivation of the time.
\**************************************************************************************************/
extern Sab_ReturnType Sab_GetTargetTimeSpan1ns64bit(const Sab_PredefTimer1ns64bitType* TimerPtr, Sab_TickType1ns64bit* TimeSpanPtr);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_ShiftTargetTimer1ns64bit
 ***
 *** DESCRIPTION:
 ***     The ShiftTimer functions shall perform proper wrap-around handling at adding (reference time + TimeValue), 
 ***     if the sum is greater than the maximum value of the timer. Shifts the reference time stored at the given 
 ***     timer instance by the given time value. The time value is scaled to 1 nano second and a precision of 64 bit. 
 ***     Thus, this timer will overflow after 18446744073,709551616 seconds (~583 years).
 ***
 *** PARAMETERS:
 ***     Type                               Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     Sab_PredefTimer1ns64bitType *      TimerPtr     The pointer to the timer to shift (InOut-Value).
 ***     Sab_TickType1ns64bit               TimeValue    The time span to shift the timer with. (In-Value)
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
extern void Sab_ShiftTargetTimer1ns64bit(Sab_PredefTimer1ns64bitType* TimerPtr, Sab_TickType1ns64bit TimeValue);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_SyncTargetTimer1ns64bit
 ***
 *** DESCRIPTION:
 ***     Synchronizes two timer instances. This means, that the reference time of the destination 
 ***     timer instance is set to the reference time of the source timer instance. The time values are scaled 
 ***     to 1 nano second and a precision of 64 bit. Thus, this timer will overflow 
 ***     after 18446744073,709551616 seconds (~583 years).
 ***
 *** PARAMETERS:
 ***     Type                               Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     Sab_PredefTimer1ns64bitType *      TimerDstPtr    The pointer to the destination timer (InOut-Value).
 ***     Sab_PredefTimer1ns64bitType *      TimerSrcPtr    The pointer to the source timer (In-Value).
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
extern void Sab_SyncTargetTimer1ns64bit(Sab_PredefTimer1ns64bitType* TimerDstPtr, const Sab_PredefTimer1ns64bitType* TimerSrcPtr);


/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_SleepActivity
 ***
 *** DESCRIPTION:
 ***     The Sab_SleepActivity function is called during an infinite loop during the sleep scenario.
 ***
 *** PARAMETERS:
 ***     Type           Name               Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
extern void Sab_SleepActivity(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_WakeupActivity
 ***
 *** DESCRIPTION:
 ***     The Sab_WakeupActivity function is called during a sleep scenario to wake up the V-ECU.
 ***
 *** PARAMETERS:
 ***     Type           Name               Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
extern void Sab_WakeupActivity(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_BreakInfiniteLoop
 ***
 *** DESCRIPTION:
 ***     The function Sab_BreakInfiniteLoop breaks an infinite loop.
 ***      - Real-time testing is running in the regular time steps 
 ***      - Lower prio task are still in task state "READY", but are not running
 ***      - Jump to IDLE_LOOP and finish simulation step 
 ***
 *** PARAMETERS:
 ***     Type           Name               Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
extern void Sab_BreakInfiniteLoop(void);


/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_GetBaseAddress
 ***
 *** DESCRIPTION:
 ***     Returns the base address to which the addresses in the A2L file refer.
 ***
 *** PARAMETERS:
 ***     Type           Name               Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***
 *** RETURNS:
 ***     Sab_UInt64   The base address to be used for calculation of effective addresses 
 ***                  for given relative addresses
\**************************************************************************************************/
extern Sab_UInt64 Sab_GetBaseAddress(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_GetTargetAddress
 ***
 *** DESCRIPTION:
 ***     Returns the address under which an A2L variable can be read or write..
 ***
 *** PARAMETERS:
 ***     Type           Name               Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     Sab_UInt64   relativeAddress    The relative addresses of an A2L symbol (In-Value).
 ***
 *** RETURNS:
 ***     Sab_UInt64   Target physical address
 \**************************************************************************************************/
extern Sab_UInt64 Sab_GetTargetAddress(Sab_UInt64 relativeAddress);


/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_GetResourceLocation
 ***
 *** DESCRIPTION:
 ***     Returns the root directory on the target platform where the resource files of the V-ECU are stored. 
 ***
 *** PARAMETERS:
 ***     Type           Name               Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     
 ***
 *** RETURNS:
 ***     char*   The root directory where the resource files of the V-ECU application process are located.
 \**************************************************************************************************/
extern char* Sab_GetResourceLocation(void);


#if 0
{
#endif
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SAB_H */

/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/
