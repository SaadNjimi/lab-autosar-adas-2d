/**************************************************************************************************\
 *** Sab_Obsolete.h
 ***
 *** Simulator Abstraction interface header file for obsolete Sab functions and types
 ***
 *** Copyright 2020 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef SAB_OBSOLETE_H
#define SAB_OBSOLETE_H

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------------------------*\
  DEFINES
\*------------------------------------------------------------------------------------------------*/

#define SAB_VENDOR_ID           35
#define SAB_MODULE_ID           9000
#define SAB_MODULE_NAME         "Simulator Abstraction"
#define SAB_SW_MAJOR_VERSION    2
#define SAB_SW_MINOR_VERSION    0
#define SAB_SW_PATCH_VERSION    0

typedef uint8 Sab_State;
#define SAB_UNINIT  (Sab_State)0
#define SAB_INIT    (Sab_State)1

#define SAB_FUNCTION_GETCURRENTHWCOUNTER100NS   (uint8)0
#define SAB_FUNCTION_GETCURRENTHWCOUNTER1NS     (uint8)1
#define SAB_FUNCTION_RESETTIMER1NS64BIT         (uint8)2
#define SAB_FUNCTION_GETTIMESPAN1NS64BIT        (uint8)3
#define SAB_FUNCTION_SHIFTTIMER1NS64BIT         (uint8)4
#define SAB_FUNCTION_SYNCTIMER1NS64BIT          (uint8)5
#define SAB_FUNCTION_BUSYWAIT1NS64BIT           (uint8)6
#define SAB_FUNCTION_RESETTIMER100NS32BIT       (uint8)7
#define SAB_FUNCTION_GETTIMESPAN100NS32BIT      (uint8)8
#define SAB_FUNCTION_SHIFTTIMER100NS32BIT       (uint8)9
#define SAB_FUNCTION_SYNCTIMER100NS32BIT        (uint8)10
#define SAB_FUNCTION_BUSYWAIT100NS32BIT         (uint8)11
#define SAB_FUNCTION_VPUTRIGGERED               (uint8)12
#define SAB_FUNCTION_EVENTTRIGGERED             (uint8)13

#define SAB_E_UNINT (uint8)1

/* Alias for obsolete API */
#define Sab_MsgPrintf Sab_SubmitMsg

#if (SAB_DEV_ERROR_DETECT == STD_ON)
#define SAB_E_NO_SERVICE        0x01
#define SAB_E_PARAM_POINTER     0x02
#define SAB_E_PARAM_VALUE       0x03
#define SAB_E_HARDWARE_TIMER    0x04
#define SAB_E_SOFTWARE_TIMER    0x05
#define SAB_E_PRECISION_100NS   0x06
#define SAB_E_PRECISION_1NS     0x07
#define SAB_E_INIT              0x08

#define SAB_MESSAGE_API         0x01
#define SAB_HIGHLEVEL_TIMER_API 0x02
#define SAB_LOWLEVEL_TIMER_API  0x03
#define SAB_STATETRANSITION_API 0x04
#endif

/*------------------------------------------------------------------------------------------------*\
  FUNCTION PROTOTYPES
\*------------------------------------------------------------------------------------------------*/



#ifdef __cplusplus
extern "C" {
#endif
#if 0
}
#endif



/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_SubmitMsg
 ***
 *** DESCRIPTION:
 ***     Submits a formatted message with severity Error (without opening a dialog).
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     Sab_MsgSeverity       severity     The message severity. This will specify the importance
 ***                                        of the submitted message (from log file up to error message)
 ***     uint32                moduleNumber The number of the basic software module calling this.
 ***                                        (Application software components should use zero.)
 ***     sint32                msgNumber    A message code (optional).
 ***     const char *          format       The formatted message text.
 ***
 *** RETURNS:
 ***     Std_ReturnType
 ***            E_OK            If the message has been submitted successfully.
 ***            E_NOT_OK        If the message has not been submitted or not submitted as desired.
\**************************************************************************************************/
extern Std_ReturnType Sab_SubmitMsg(Sab_MsgSeverity severity, uint32 moduleNumber, sint32 messageNumber, const char* format);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_GetVirtualTime
 ***
 *** DESCRIPTION:
 ***    Returns the virtual time. May be used for time stamps. For long time measurements.
 ***    The virtual time starts when the V-ECU OS starts.
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     float64
 ***        The virtual time of the Virtual ECU.
\**************************************************************************************************/
extern float64 Sab_GetVirtualTime(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_GetVirtualTimeElapsed
 ***
 *** DESCRIPTION:
 ***    Returns the elapsed virtual time. May be used to determine the difference time between
 ***    two asynchronous events, e.g. for an integrator in an asynchronous task.
 ***    The refTime value is updated with each call.
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     float64 *             refTime      The reference time.
 ***
 *** RETURNS:
 ***     float64
 ***        The virtual time of the Virtual ECU.
\**************************************************************************************************/
extern float64 Sab_GetVirtualTimeElapsed(float64* refTime);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_GetVirtualTimeTicks
 ***
 *** DESCRIPTION:
 ***    Returns the virtual time as an integer value (counter). The value may overflow.
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     Sab_TickType
 ***        The counter value.
\**************************************************************************************************/
extern Sab_TickType Sab_GetVirtualTimeTicks(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_GetVirtualTimeTicksElapsed
 ***
 *** DESCRIPTION:
 ***     Returns the elapsed virtual time as an integer value (difference of counter values).
 ***     The refTimeTicks value is updated with each call.
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     Sab_TickType *        refTimeTicks
 ***
 *** RETURNS:
 ***     Sab_TickType
 ***        The elapsed counter value.
\**************************************************************************************************/
extern Sab_TickType Sab_GetVirtualTimeTicksElapsed(Sab_TickType* refTimeTicks);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_GetVirtualTimeTicksPerSecond
 ***
 *** DESCRIPTION:
 ***     Returns the number of real time ticks per second (counter resolution).
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     Sab_TickType
 ***        The time resolution.
\**************************************************************************************************/
extern Sab_TickType Sab_GetVirtualTimeTicksPerSecond(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_GetRealTime
 ***
 *** DESCRIPTION:
 ***    Returns the real time. May be used for time stamps. For long time measurements.
 ***    The real time starts when the simulator enters the Running state.
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     float64
 ***        The real time of the Virtual ECU.
\**************************************************************************************************/
extern float64 Sab_GetRealTime(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_GetRealTimeElapsed
 ***
 *** DESCRIPTION:
 *** Returns the elapsed real time. May be used for execution time measurements.
 *** The refTime value is updated with each call.
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     float64 *             refTime      The reference time.
 ***
 *** RETURNS:
 ***     float64
 ***        The real time of the Virtual ECU.
\**************************************************************************************************/
extern float64 Sab_GetRealTimeElapsed(float64* refTime);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_GetRealTimeElapsed
 ***
 *** DESCRIPTION:
 ***    Returns the real time as an integer value (counter). The value may overflow.
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     Sab_TickType
 ***        The counter value.
\**************************************************************************************************/
extern Sab_TickType Sab_GetRealTimeTicks(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_GetRealTimeTicksElapsed
 ***
 *** DESCRIPTION:
 ***     Returns the elapsed real time as an integer value (difference of counter values).
 ***     The refTimeTicks value is updated with each call.
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     Sab_TickType *        refTimeTicks
 ***
 *** RETURNS:
 ***     Sab_TickType
 ***        The elapsed counter value.
\**************************************************************************************************/
extern Sab_TickType Sab_GetRealTimeTicksElapsed(Sab_TickType* refTimeTicks);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_GetRealTimeTicksPerSecond
 ***
 *** DESCRIPTION:
 ***     Returns the number of real time ticks per second (counter resolution).
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     Sab_TickType
 ***        The time resolution.
\**************************************************************************************************/
extern Sab_TickType Sab_GetRealTimeTicksPerSecond(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_GetVersionInfo
 ***
 *** DESCRIPTION:
 ***     This service returns the version information of this module. The version information
 ***     includes the module Id, the vendor Id, and a vendor specific version numbers. This
 ***     function is always enabled and available.
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     Std_VersionInfoType*  versioninfo  Pointer to where to store the version information of
 ***                                        this module.
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
extern void Sab_GetVersionInfo(Std_VersionInfoType* versioninfo);


/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_ResetTimer100ns32bit
 ***
 *** DESCRIPTION:
 ***     Resets the current timer. This means, that the timer instance shall be set to the current
 ***     hardware timer value. The underlying timer uses a resolution of 100 nano second and a
 ***     precision of 32 bit. Thus this timer will overflow after 429,4967296 seconds.
 ***
 *** PARAMETERS:
 ***     Type                               Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     Sab_PredefTimer100ns32bitType *    TimerPtr     The pointer to the timer to reset (In-Out value).
 ***
 *** RETURNS:
 ***     Std_ReturnType
 ***            E_OK            If the timer has been reset successfully.
 ***            E_NOT_OK        If an error has occurred during the time evaluation.
\**************************************************************************************************/
extern Std_ReturnType Sab_ResetTimer100ns32bit(Sab_PredefTimer100ns32bitType* TimerPtr);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_GetTimeSpan100ns32bit
 ***
 *** DESCRIPTION:
 ***     Calculates the difference between the current hardware timer value and the given counter.
 ***     The underlying timer uses a resolution of 100 nano second and a
 ***     precision of 32 bit. Thus this timer will overflow after 429,4967296 seconds.
 ***
 *** PARAMETERS:
 ***     Type                               Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     Sab_PredefTimer100ns32bitType *    TimerPtr     The pointer to the timer to use as basic (In-Value).
 ***     uint32                             TimeSpanPtr  The time span calculated (Out-Parameter)
 ***
 *** RETURNS:
 ***     Std_ReturnType
 ***            E_OK            If the timespan has been been calculated successfully.
 ***            E_NOT_OK        If an error has occurred during the derivation of the time.
\**************************************************************************************************/
extern Std_ReturnType Sab_GetTimeSpan100ns32bit(const Sab_PredefTimer100ns32bitType* TimerPtr, uint32* TimeSpanPtr);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_ShiftTimer1ns64bit
 ***
 *** DESCRIPTION:
 ***     Calculates the difference between the current hardware timer value and the given counter.
 ***     The underlying timer uses a resolution of 100 nano second and a
 ***     precision of 32 bit. Thus this timer will overflow after 429,4967296 seconds.
 ***
 *** PARAMETERS:
 ***     Type                               Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     Sab_PredefTimer100ns32bitType *    TimerPtr     The pointer to the timer to shift (InOut-Value).
 ***     uint32                             TimeValue    The time span to shift the timer with. (In-Value)
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
extern void Sab_ShiftTimer100ns32bit(Sab_PredefTimer100ns32bitType* TimerPtr, uint32 TimeValue);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_SyncTimer100ns32bit
 ***
 *** DESCRIPTION:
 ***     Synchronizes two timers. This means, that the value of the destination timer is set to
 ***     to the value of the source timer. The underlying timer uses a resolution of 1 nano second and a
 ***     precision of 64 bit. Thus this timer will overflow after 18446744073,709551616 seconds.
 ***
 *** PARAMETERS:
 ***     Type                               Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     Sab_PredefTimer100ns32bitType *    TimerDstPtr    The pointer to the destination timer (InOut-Value).
 ***     Sab_PredefTimer100ns32bitType *    TimerSrcPtr    The pointer to the source timer (In-Value).
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
extern void Sab_SyncTimer100ns32bit(Sab_PredefTimer100ns32bitType* TimerDstPtr, const Sab_PredefTimer100ns32bitType* TimerSrcPtr);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_BusyWait100ns32bit
 ***
 *** DESCRIPTION:
 ***     Causes the current process to wait busy for the desired time span in timer ticks.
 ***     The underlying timer uses a resolution of 1 nano second and a
 ***     precision of 64 bit. Thus this timer will overflow after 18446744073,709551616 seconds.
 ***
 *** PARAMETERS:
 ***     Type           Name               Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     uint16         WaitingTimeMin     The number of nano seconds to wait (In-Value).
 ***
 *** RETURNS:
 ***     Std_ReturnType
 ***            E_OK            If the busy waiting has succeeded and the application has
 ***                            been waiting for at least the specified time.
 ***            E_NOT_OK        If an error has occurred during the time evaluation.
\**************************************************************************************************/
extern Std_ReturnType Sab_BusyWait100ns32bit(uint16 WaitingTimeMin);





#if 0
{
#endif
#ifdef __cplusplus
}
#endif

#endif /* SAB_OBSOLETE_H */

/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/
