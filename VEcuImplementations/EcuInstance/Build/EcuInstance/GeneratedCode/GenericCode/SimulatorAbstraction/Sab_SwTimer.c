/**************************************************************************************************\
 *** Sab_SwTimer.c
 ***
 *** Implementation of the Simulator Abstraction layer function concerning the higher level software timer for VEOS.
 ***
 *** Copyright 2019 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef SAB_SWTIMER_C
#define SAB_SWTIMER_C

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/

#include "Sab.h"
#include "Sab_Veos.h"
#include "Std_Types.h"

#include "VEOS.h"


/*------------------------------------------------------------------------------------------------*\
  FORWARD DECLARATIONS
\*------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif
#if 0
}
#endif

extern Sab_ApplicationState Sab_CurrentState;

extern Sab_TickType g_Sab_TimeResolution;

#if 0
{
#endif
#ifdef __cplusplus
}
#endif

/*------------------------------------------------------------------------------------------------*\
  MACRO DECLARATIONS
\*------------------------------------------------------------------------------------------------*/

#if (SAB_VARIADIC_TIMESTAMP == STD_OFF)
/* Resolution of the real time */
#define SAB_REAL_TIME_RESOLUTION()      g_Sab_TimeResolution;

/* Resolution of the virtual time */
#define SAB_VIRTUAL_TIME_RESOLUTION()   g_Sab_TimeResolution;
#endif /* SAB_VARIADIC_TIMESTAMP == STD_ON */


/*------------------------------------------------------------------------------------------------*\
  VARIABLE DECLARATIONS
\*------------------------------------------------------------------------------------------------*/

/* Flag indicating that the OS has started */
boolean Sab_OsRunning = FALSE;

/* Global timer instances for measuring of the target time */
Sab_PredefTimer1ns64bitType Sab_PredefTargetTimer_StartSimulation_1ns64bit;
Sab_PredefTimer1ns64bitType Sab_PredefTargetTimer_StartOs_1ns64bit;

/* The time when the application has entered the running state. */
float64 Sab_VirtualStartTime = 0.0;

/* The time when the operating system has started. */
float64 Sab_RealStartTime = 0.0;

float64 Sab_RecentVirtualTime = 0.0;
float64 Sab_VirtualTimeElapsedSinceLastTrigger = 0.0;

float64 Sab_RecentRealTime = 0.0;
float64 Sab_RealTimeElapsedSinceLastTrigger = 0.0;

/* The ticks since the application has entered the running state */
Sab_TickType Sab_VirtualTicks = 0;

/* The ticks since the operating system has started */
Sab_TickType Sab_RealTicks = 0;

#ifndef SAB_TIME_OFFSET
#define SAB_TIME_OFFSET (Sab_TickType) 1000
#else
#if SAB_TIME_OFFSET <= 0
#undef  SAB_TIME_OFFSET;
#define SAB_TIME_OFFSET (Sab_TickType) 1000
#endif
#endif

/*------------------------------------------------------------------------------------------------*\
  FORWARD DECLARATIONS
\*------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C"
{
#endif
#if 0
}
#endif
    extern void Sab_StopModel(void);
#if 0
{
#endif
#ifdef __cplusplus
}
#endif

/*------------------------------------------------------------------------------------------------*\
  PARAMETRIZED MACROS
\*------------------------------------------------------------------------------------------------*/

#define SAB_UINT64_MAXVALUE     0xffffffffffffffffull

#define SAB_ROUND_FP(_p) \
        (((_p) == 0.0) ? (0.0) : (((_p) > 0.0) ? ((_p) + 0.5) : ((_p) - 0.5)))

#define SAB_CALCULATEELAPSEDVALUE(dataType, dtVoidFunction, referenceValue)     \
    dataType result = 0;                                                \
    dataType currentValue = 0;                                          \
                                                                        \
    if (NULL == referenceValue)                                         \
    {                                                                   \
        return result;                                                  \
    }                                                                   \
                                                                        \
    currentValue = dtVoidFunction();                                    \
                                                                        \
    result = currentValue - (* referenceValue);                         \
                                                                        \
    (* referenceValue) = currentValue;                                  \
                                                                        \
    return result;                                                      \




/*------------------------------------------------------------------------------------------------*\
  FUNCTION DEFINITIONS
\*------------------------------------------------------------------------------------------------*/


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
Sab_ReturnType Sab_ResetTargetTimer1ns64bit(Sab_PredefTimer1ns64bitType* TimerPtr)
{
    Sab_CheckInitialized(SAB_FUNCTION_RESETTIMER1NS64BIT);

    if (NULL == TimerPtr)
    {
#if (SAB_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(SAB_MODULE_ID, 0, SAB_LOWLEVEL_TIMER_API, SAB_E_PARAM_POINTER);
#endif
        return SAB_E_NOT_OK;
    }

    TimerPtr->ReferenceTime = (Sab_UInt64)g_VEOS_CurrentSimulationCounter_1ns;
    return SAB_E_OK;
}


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
Sab_ReturnType Sab_GetTargetTimeSpan1ns64bit(const Sab_PredefTimer1ns64bitType* TimerPtr, Sab_TickType1ns64bit* TimeSpanPtr)
{

    Sab_CheckInitialized(SAB_FUNCTION_GETTIMESPAN1NS64BIT);

    if (NULL == TimerPtr)
    {
#if (SAB_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(SAB_MODULE_ID, 0, SAB_LOWLEVEL_TIMER_API, SAB_E_PARAM_POINTER);
#endif
        return SAB_E_NOT_OK;
    }

    if (NULL == TimeSpanPtr)
    {
#if (SAB_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(SAB_MODULE_ID, 0, SAB_LOWLEVEL_TIMER_API, SAB_E_PARAM_POINTER);
#endif
        return SAB_E_NOT_OK;
    }

    Sab_UInt64 currentTimeValue = (Sab_UInt64)g_VEOS_CurrentSimulationCounter_1ns;
    if (currentTimeValue >= TimerPtr->ReferenceTime)
    {
        (*TimeSpanPtr) = currentTimeValue - TimerPtr->ReferenceTime;
    }
    else
    {
        (*TimeSpanPtr) = (SAB_UINT64_MAXVALUE - TimerPtr->ReferenceTime) + currentTimeValue;
    }
    return SAB_E_OK;
}

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
void Sab_ShiftTargetTimer1ns64bit(Sab_PredefTimer1ns64bitType* TimerPtr, Sab_TickType1ns64bit TimeValue)
{
    Sab_TickType1ns64bit referenceValue;

    Sab_CheckInitialized(SAB_FUNCTION_SHIFTTIMER1NS64BIT);

    if (NULL == TimerPtr)
    {
#if (SAB_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(SAB_MODULE_ID, 0, SAB_LOWLEVEL_TIMER_API, SAB_E_PARAM_POINTER);
#endif
        return;
    }

    referenceValue = TimerPtr->ReferenceTime;
    TimerPtr->ReferenceTime = referenceValue + TimeValue;
}


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
void Sab_SyncTargetTimer1ns64bit(Sab_PredefTimer1ns64bitType* TimerDstPtr, const Sab_PredefTimer1ns64bitType* TimerSrcPtr)
{
    Sab_CheckInitialized(SAB_FUNCTION_SYNCTIMER1NS64BIT);

    if (NULL == TimerDstPtr)
    {
#if (SAB_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(SAB_MODULE_ID, 0, SAB_LOWLEVEL_TIMER_API, SAB_E_PARAM_POINTER);
#endif
        return;
    }
    if (NULL == TimerSrcPtr)
    {
#if (SAB_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(SAB_MODULE_ID, 0, SAB_LOWLEVEL_TIMER_API, SAB_E_PARAM_POINTER);
#endif
        return;
    }

    TimerDstPtr->ReferenceTime = TimerSrcPtr->ReferenceTime;
}



/**************************************************************************************************\
 *** internal Sab functions
\**************************************************************************************************/


/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_VpuTriggered
 ***
 *** DESCRIPTION:
 ***     Called when the VPU has been triggered and the timestamp has been incremented.
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
void Sab_VpuTriggered(void)
{
    static boolean Sab_FirstVpuTrigger = VEOS_TRUE;

    float64 currentTime = g_VEOS_CurrentSimulationTime;
    Sab_CheckInitialized(SAB_FUNCTION_VPUTRIGGERED);
    
    if (Sab_FirstVpuTrigger)
    {
        /* First trigger of the VPU. If delayed start is configured, the V-ECU is not started, yet. */
        if ( SAB_E_NOT_OK == Sab_ResetTargetTimer1ns64bit(&Sab_PredefTargetTimer_StartSimulation_1ns64bit))
        {
             VEOS_MsgApi_ShowMessage(VEOS_MsgApi_SeverityType_Error, VEOS_TRUE, "The global predefined timer instance 'Sab_PredefTargetTimer_StartSimulation_1ns64bit' cannot be initialized.");
             VEOS_SimApi_TerminateSimulationFatal();
        }
        Sab_FirstVpuTrigger = VEOS_FALSE;
    }

    if ((Sab_Run == Sab_CurrentState) || (Sab_Sleep == Sab_CurrentState))
    {
        /* Incrementation, which should work even if Sab_VirtualTicks is not a multiple of SAB_TIME_OFFSET */
        Sab_VirtualTicks += (Sab_TickType)SAB_TIME_OFFSET;
        Sab_RecentVirtualTime = currentTime;
        Sab_VirtualTimeElapsedSinceLastTrigger = 0.0;
    }

    if (TRUE == Sab_OsRunning)
    {
        Sab_RealTicks += (Sab_TickType)SAB_TIME_OFFSET;
        Sab_RecentRealTime = currentTime;
        Sab_RealTimeElapsedSinceLastTrigger = 0.0;
    }
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_EventTriggered
 ***
 *** DESCRIPTION:
 ***     Called when the VPU has received an event which is not associated with a cyclic timer
 ***     and the timestamp has been incremented.
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
void Sab_EventTriggered(void)
{
    float64 currentTime = g_VEOS_CurrentSimulationTime;
    Sab_CheckInitialized(SAB_FUNCTION_EVENTTRIGGERED);
    Sab_VirtualTimeElapsedSinceLastTrigger = currentTime - Sab_RecentVirtualTime;
    Sab_RealTimeElapsedSinceLastTrigger = currentTime - Sab_RecentRealTime;
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_EventFinished
 ***
 *** DESCRIPTION:
 ***     Called when the VPU received event is finished.
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
void Sab_EventFinished(void)
{
    Sab_VirtualTimeElapsedSinceLastTrigger = 0.0;
    Sab_RealTimeElapsedSinceLastTrigger = 0.0;
}



/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_OnOsStarted
 ***
 *** DESCRIPTION:
 ***     Callout for the implementation of the operating system to signal the start of the OS.
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
void Sab_OnOsStarted(void)
{
    if (Sab_CurrentState != Sab_Run)
    {
        Sab_ApplicationStateExit(Sab_CurrentState);
        Sab_ApplicationStateEntry(Sab_Run);
    }

    if ( SAB_E_NOT_OK == Sab_ResetTargetTimer1ns64bit(&Sab_PredefTargetTimer_StartOs_1ns64bit))
    {
        VEOS_MsgApi_ShowMessage(VEOS_MsgApi_SeverityType_Error, VEOS_TRUE, "The global predefined timer instance 'Sab_PredefTargetTimer_StartOs_1ns64bit' cannot be initialized.");
        VEOS_SimApi_TerminateSimulationFatal();
    }
    
    Sab_OsRunning = TRUE;
    
    Sab_RealStartTime = g_VEOS_CurrentSimulationTime;
}



/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_OnOsShutdown
 ***
 *** DESCRIPTION:
 ***     Callout for the implementation of the operating system to signal the shutdown of the OS .
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
void Sab_OnOsShutdown(void)
{
    if (Sab_CurrentState != Sab_Off)
    {
         Sab_ApplicationStateExit(Sab_CurrentState);
         Sab_ApplicationStateEntry(Sab_Off);
    }
    Sab_RealStartTime = 0.0;
    Sab_OsRunning = FALSE;

    Sab_StopModel();
}


/**************************************************************************************************\
 *** obsolete Sab functions
\**************************************************************************************************/


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
float64 Sab_GetVirtualTime(void)
{
    return g_VEOS_CurrentSimulationTime - Sab_VirtualStartTime;
}

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
float64 Sab_GetVirtualTimeElapsed(float64 * refTime)
{
    SAB_CALCULATEELAPSEDVALUE(float64, Sab_GetVirtualTime, refTime)
}

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
Sab_TickType Sab_GetVirtualTimeTicks(void)
{
    if (Sab_VirtualTicks > (Sab_TickType)(SAB_ROUND_FP(Sab_GetVirtualTime() * Sab_GetRealTimeTicksPerSecond())) )
    {
        /* this function is called in a 'MCAL Notification Event' */
        Sab_TickType DeltaTicks = (Sab_TickType)(SAB_ROUND_FP(Sab_VirtualTimeElapsedSinceLastTrigger * Sab_GetVirtualTimeTicksPerSecond()));
        return Sab_VirtualTicks + DeltaTicks - SAB_TIME_OFFSET;
    }
    else
    {
        return Sab_VirtualTicks;
    }
}

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
Sab_TickType Sab_GetVirtualTimeTicksElapsed(Sab_TickType * refTimeTicks)
{
    SAB_CALCULATEELAPSEDVALUE(Sab_TickType, Sab_GetVirtualTimeTicks, refTimeTicks)
}

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
Sab_TickType Sab_GetVirtualTimeTicksPerSecond(void)
{
#if (SAB_VARIADIC_TIMESTAMP == STD_OFF)
    return SAB_TIME_OFFSET * SAB_VIRTUAL_TIME_RESOLUTION();
#else
    float64 virtualTime = Sab_GetVirtualTime();

    if (virtualTime > 0.0d)
    {
        return Sab_VirtualTicks / virtualTime;
    }

#if (SAB_DEV_ERROR_DETECT == STD_ON)
    Det_ReportError(SAB_MODULE_ID, 0, SAB_HIGHLEVEL_TIMER_API, SAB_E_SOFTWARE_TIMER);
#endif

    VEOS_MsgApi_ShowMessage(VEOS_MsgApi_SeverityType_Error, VEOS_TRUE, "Virtual time cannot be determined. This happens, if the resolution is taken in the first simulation step.");
    VEOS_SimApi_TerminateSimulationFatal();
    return 0;
#endif /* SAB_VARIADIC_TIMESTAMP == STD_OFF */
}

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
float64 Sab_GetRealTime(void)
{
    return g_VEOS_CurrentSimulationTime - Sab_RealStartTime;
}

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
float64 Sab_GetRealTimeElapsed(float64 * refTime)
{
    SAB_CALCULATEELAPSEDVALUE(float64, Sab_GetRealTime, refTime)
}

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
Sab_TickType Sab_GetRealTimeTicks(void)
{
    if (Sab_RealTicks > (Sab_TickType)(SAB_ROUND_FP(Sab_GetRealTime() * Sab_GetRealTimeTicksPerSecond())) )
    {
        /* this function is called in a 'MCAL Notification Event' */
        Sab_TickType DeltaTicks = (Sab_TickType)(SAB_ROUND_FP(Sab_VirtualTimeElapsedSinceLastTrigger * Sab_GetRealTimeTicksPerSecond()));
        return Sab_RealTicks + DeltaTicks - SAB_TIME_OFFSET;
    }
    else
    {
        return Sab_RealTicks;
    }
}

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
Sab_TickType Sab_GetRealTimeTicksElapsed(Sab_TickType * refTimeTicks)
{
    SAB_CALCULATEELAPSEDVALUE(Sab_TickType, Sab_GetRealTimeTicks, refTimeTicks);
}

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
Sab_TickType Sab_GetRealTimeTicksPerSecond(void)
{
#if (SAB_VARIADIC_TIMESTAMP == STD_OFF)
    return SAB_TIME_OFFSET * SAB_REAL_TIME_RESOLUTION();
#else
    float64 realTime = Sab_GetRealTime();

    if (realTime > 0.0d)
    {
        return Sab_RealTicks / realTime;
    }

#if (SAB_DEV_ERROR_DETECT == STD_ON)
    Det_ReportError(SAB_MODULE_ID, 0, SAB_HIGHLEVEL_TIMER_API, SAB_E_SOFTWARE_TIMER);
#endif

    VEOS_MsgApi_ShowMessage(VEOS_MsgApi_SeverityType_Error, VEOS_TRUE, "Real time cannot be determined. This happens, if the resolution is taken in the first simulation step.");
    VEOS_SimApi_TerminateSimulationFatal();
    return 0;
#endif /* SAB_VARIADIC_TIMESTAMP == STD_OFF */
}

#endif /* SAB_SWTIMER_C */

/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/
