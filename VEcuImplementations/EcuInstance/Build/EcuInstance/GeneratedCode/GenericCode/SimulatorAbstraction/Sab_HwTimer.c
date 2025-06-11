/**************************************************************************************************\
 *** Sab_HwTimer.c
 ***
 *** Implementation of the Simulator Abstraction layer function concerning the low level hardware timer for VEOS.
 ***
 *** Copyright 2019 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef SAB_HWTIMER_C
#define SAB_HWTIMER_C

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/

#include "Platform_Types.h"
#include "Std_Types.h"

#include "Sab.h"
#include "Sab_Veos.h"
#include "Sab_Types.h"
#include "Sab_Cfg.h"
#include "VEOS.h"

#if (SAB_DEV_ERROR_DETECT == STD_ON)
#include "Det.h"
#endif

/*------------------------------------------------------------------------------------------------*\
  DEFINES
\*------------------------------------------------------------------------------------------------*/

#define SAB_UINT32_MAXVALUE     0xffffffff
#define SAB_UINT64_MAXVALUE     0xffffffffffffffffull

/*------------------------------------------------------------------------------------------------*\
  FORWARD DECLARATIONS
\*------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif
#if 0
}
#endif


/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_Gpt_GetCurrentHwCounter1ns
 ***
 *** DESCRIPTION:
 ***     Gets the current hardware timer. The underlying timer uses a resolution of 1 nano second and a
 ***     precision of 64 bit. Thus this timer will overflow after 18446744073,709551616 seconds.
 ***
 *** PARAMETERS:
 ***     Type                               Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     Sab_PredefTimer1ns64bitType        currentValue The value to set
 ***
 *** RETURNS:
 ***     boolean
 ***            TRUE    if the value has been obtained successfully.
 ***            FALSE   if an error has occurred.
\**************************************************************************************************/
extern boolean Sab_Gpt_GetCurrentHwCounter1ns(Sab_PredefTimer1ns64bitType *currentValue);

#if 0
{
#endif
#ifdef __cplusplus
}
#endif

/*------------------------------------------------------------------------------------------------*\
  FUNCTION DEFINITIONS
\*------------------------------------------------------------------------------------------------*/

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_ResetTimer1ns64bit
 ***
 *** DESCRIPTION:
 ***     Resets the current timer. This means, that the timer instance shall be set to the current
 ***     hardware timer value. The underlying timer uses a resolution of 1 nano second and a
 ***     precision of 64 bit. Thus this timer will overflow after 18446744073,709551616 seconds.
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
Sab_ReturnType Sab_ResetTimer1ns64bit(Sab_PredefTimer1ns64bitType* TimerPtr)
{

#if (SAB_ONE_NANOSECONDPRECISION == STD_ON)
    Sab_PredefTimer1ns64bitType currentValue;
    Sab_CheckInitialized(SAB_FUNCTION_RESETTIMER1NS64BIT);

    if (NULL == TimerPtr)
    {
#if (SAB_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(SAB_MODULE_ID, 0, SAB_LOWLEVEL_TIMER_API, SAB_E_PARAM_POINTER);
#endif
        return SAB_E_NOT_OK;
    }

    if (TRUE == Sab_Gpt_GetCurrentHwCounter1ns(&currentValue))
    {
#ifdef SAB_UINT64_ATOMIC
        TimerPtr->ReferenceTime = currentValue.ReferenceTime;
#else
        TimerPtr->ReferenceTime.low = currentValue.ReferenceTime.high;
        TimerPtr->ReferenceTime.high = currentValue.ReferenceTime.high;
#endif
        return E_OK;
    }
    else
    {
#if (SAB_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(SAB_MODULE_ID, 0, SAB_LOWLEVEL_TIMER_API, SAB_E_HARDWARE_TIMER);
#endif
        return SAB_E_NOT_OK;
    }
#else
#if (SAB_DEV_ERROR_DETECT == STD_ON)
    Det_ReportError(SAB_MODULE_ID, 0, SAB_LOWLEVEL_TIMER_API, SAB_E_NO_SERVICE);
#endif

    return SAB_E_NOT_OK;
#endif /* (SAB_ONE_NANOSECONDPRECISION == STD_ON) */
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_GetTimeSpan1ns64bit
 ***
 *** DESCRIPTION:
 ***     Calculates the difference between the current hardware timer value and the given counter.
 ***     The underlying timer uses a resolution of 1 nano second and a
 ***     precision of 64 bit. Thus this timer will overflow after 18446744073,709551616 seconds.
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
Sab_ReturnType Sab_GetTimeSpan1ns64bit(const Sab_PredefTimer1ns64bitType* TimerPtr, Sab_TickType1ns64bit* TimeSpanPtr)
{
#if (SAB_ONE_NANOSECONDPRECISION == STD_ON)
    Sab_PredefTimer1ns64bitType currentValue;
    Sab_UInt64 referenceTimeValue;

#ifndef SAB_UINT64_ATOMIC
    uint32 lowValueReference;
#endif

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

    referenceTimeValue = TimerPtr->ReferenceTime;
    if (TRUE == Sab_Gpt_GetCurrentHwCounter1ns(&currentValue))
    {
        Sab_UInt64 currentTimeValue = currentValue.ReferenceTime;
#ifdef SAB_UINT64_ATOMIC
        if (currentTimeValue >= referenceTimeValue)
        {
            (*TimeSpanPtr) = currentTimeValue - referenceTimeValue;
            return SAB_E_OK;
        }
        else
        {
            (*TimeSpanPtr) = (SAB_UINT64_MAXVALUE - referenceTimeValue) + currentTimeValue;
            return SAB_E_OK;
        }
#else
        lowValueReference = TimeSpanPtr->low;
        TimeSpanPtr->high = currentTimeValue.high - TimerPtr->ReferenceTime.high;
        TimeSpanPtr->low = currentTimeValue.low - TimerPtr->ReferenceTime.low;
        if (TimeSpanPtr->low > lowValueReference) /* underflow detected */
        {
            TimeSpanPtr->high = TimeSpanPtr->high - 1;
        }

        return SAB_E_OK;
#endif
    }
    else
    {
#if (SAB_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(SAB_MODULE_ID, 0, SAB_LOWLEVEL_TIMER_API, SAB_E_HARDWARE_TIMER);
#endif
        return SAB_E_NOT_OK;
    }
#else
#if (SAB_DEV_ERROR_DETECT == STD_ON)
    Det_ReportError(SAB_MODULE_ID, 0, SAB_LOWLEVEL_TIMER_API, SAB_E_NO_SERVICE);
#endif
    return SAB_E_NOT_OK;
#endif /* (SAB_ONE_NANOSECONDPRECISION == STD_ON) */
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_ShiftTimer1ns64bit
 ***
 *** DESCRIPTION:
 ***     Calculates the difference between the current hardware timer value and the given counter.
 ***     The underlying timer uses a resolution of 1 nano second and a
 ***     precision of 64 bit. Thus this timer will overflow after 18446744073,709551616 seconds.
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
void Sab_ShiftTimer1ns64bit(Sab_PredefTimer1ns64bitType* TimerPtr, Sab_TickType1ns64bit TimeValue)
{
#if (SAB_ONE_NANOSECONDPRECISION == STD_ON)
    Sab_TickType1ns64bit referenceValue;
#ifndef SAB_UINT64_ATOMIC
    uint32 lowValueReference;
#endif

    Sab_CheckInitialized(SAB_FUNCTION_SHIFTTIMER1NS64BIT);

    if (NULL == TimerPtr)
    {
#if (SAB_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(SAB_MODULE_ID, 0, SAB_LOWLEVEL_TIMER_API, SAB_E_PARAM_POINTER);
#endif
        return;
    }

    referenceValue = TimerPtr->ReferenceTime;
#ifdef SAB_UINT64_ATOMIC
    TimerPtr->ReferenceTime = referenceValue + TimeValue;
#else
    lowValueReference = referenceValue.low;
    TimerPtr->ReferenceTime.low = referenceValue.low + TimeValue.low;
    TimerPtr->ReferenceTime.high = referenceValue.high + TimeValue.high;
    if (lowValueReference < referenceValue.low) /* detect overflow */
    {
        TimerPtr->ReferenceTime.high = TimerPtr->ReferenceTime.high + 1;
    }
#endif
#else
#if (SAB_DEV_ERROR_DETECT == STD_ON)
    Det_ReportError(SAB_MODULE_ID, 0, SAB_LOWLEVEL_TIMER_API, SAB_E_NO_SERVICE);
#endif
#endif /*  (SAB_ONE_NANOSECONDPRECISION == STD_ON) */
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_SyncTimer1ns64bit
 ***
 *** DESCRIPTION:
 ***     Synchronizes two timers. This means, that the value of the destination timer is set to
 ***     to the value of the source timer. The underlying timer uses a resolution of 1 nano second and a
 ***     precision of 64 bit. Thus this timer will overflow after 18446744073,709551616 seconds.
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
void Sab_SyncTimer1ns64bit(Sab_PredefTimer1ns64bitType* TimerDstPtr, const Sab_PredefTimer1ns64bitType* TimerSrcPtr)
{
#if (SAB_ONE_NANOSECONDPRECISION == STD_ON)
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

#ifdef SAB_UINT64_ATOMIC
    TimerDstPtr->ReferenceTime = TimerSrcPtr->ReferenceTime;
#else
    TimerDstPtr->ReferenceTime.high = TimerSrcPtr->ReferenceTime.high;
    TimerDstPtr->ReferenceTime.low = TimerSrcPtr->ReferenceTime.low;
#endif
#else
#if (SAB_DEV_ERROR_DETECT == STD_ON)
    Det_ReportError(SAB_MODULE_ID, 0, SAB_LOWLEVEL_TIMER_API, SAB_E_NO_SERVICE);
#endif
#endif /* (SAB_ONE_NANOSECONDPRECISION == STD_ON) */
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_SyncTimer1ns64bit
 ***
 *** DESCRIPTION:
 ***     Causes the current process to wait busy for the desired time span in timer ticks.
 ***     The underlying timer uses a resolution of 1 nano second and a
 ***     precision of 64 bit. Thus this timer will overflow after 18446744073,709551616 seconds.
 ***
 *** PARAMETERS:
 ***     Type           Name               Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     Sab_TickType1ns32bit         WaitingTimeMin     The number of nano seconds to wait (In-Value).
 ***
 *** RETURNS:
 ***     Sab_ReturnType
 ***            SAB_E_OK            If the busy waiting has succeeded and the application has
 ***                            been waiting for at least the specified time.
 ***            SAB_E_NOT_OK        If an error has occurred during the time evaluation.
\**************************************************************************************************/
Sab_ReturnType Sab_BusyWait1ns64bit(Sab_TickType1ns32bit WaitingTimeMin)
{
#if (SAB_ONE_NANOSECONDPRECISION == STD_ON)
    Sab_PredefTimer1ns64bitType entranceValue;
    Sab_TickType1ns64bit timeElapsed;
    Sab_ReturnType callReturnValue;

    Sab_CheckInitialized(SAB_FUNCTION_BUSYWAIT1NS64BIT);
    if (TRUE != Sab_Gpt_GetCurrentHwCounter1ns(&entranceValue))
    {
#if (SAB_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(SAB_MODULE_ID, 0, SAB_LOWLEVEL_TIMER_API, SAB_E_HARDWARE_TIMER);
#endif
        return SAB_E_NOT_OK;
    }

    callReturnValue = SAB_E_OK;
    for (;;)
    {
        if (SAB_E_NOT_OK == Sab_GetTimeSpan1ns64bit(&entranceValue, &timeElapsed))
        {
            callReturnValue = SAB_E_NOT_OK;
            break;
        }

#ifdef SAB_UINT64_ATOMIC
        if (WaitingTimeMin < timeElapsed)
#else
        if (WaitingTimeMin < timeElapsed.low)
#endif
        {
            break;
        }
    }

    return callReturnValue;
#else
#if (SAB_DEV_ERROR_DETECT == STD_ON)
    Det_ReportError(SAB_MODULE_ID, 0, SAB_LOWLEVEL_TIMER_API, SAB_E_NO_SERVICE);
#endif
    return SAB_E_NOT_OK;
#endif /* (SAB_ONE_NANOSECONDPRECISION == STD_ON) */
}






/**************************************************************************************************\
 *** obsolete Sab functions
\**************************************************************************************************/

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_Gpt_GetCurrentHwCounter100ns
 ***
 *** DESCRIPTION:
 ***     Gets the current hardware timer. The underlying timer uses a resolution of 100 nano second and a
 ***     precision of 32 bit. Thus this timer will overflow after 429,4967296 seconds.
 ***
 *** PARAMETERS:
 ***     Type                               Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     Sab_PredefTimer100ns32bitType      currentValue The value to set
 ***
 *** RETURNS:
 ***     boolean
 ***            TRUE    if the value has been obtained successfully.
 ***            FALSE   if an error has occurred.
\**************************************************************************************************/
extern boolean Sab_Gpt_GetCurrentHwCounter100ns(Sab_PredefTimer100ns32bitType *currentValue);


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
Std_ReturnType Sab_ResetTimer100ns32bit(Sab_PredefTimer100ns32bitType* TimerPtr)
{
#if (SAB_HUNDRED_NANOSECONDPRECISION == STD_ON)
    Sab_PredefTimer100ns32bitType currentValue;

    Sab_CheckInitialized(SAB_FUNCTION_RESETTIMER100NS32BIT);
    if (NULL == TimerPtr)
    {
#if (SAB_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(SAB_MODULE_ID, 0, SAB_LOWLEVEL_TIMER_API, SAB_E_PARAM_POINTER);
#endif
        return E_NOT_OK;
    }

    if (TRUE == Sab_Gpt_GetCurrentHwCounter100ns(&currentValue))
    {
        TimerPtr->ReferenceTime = currentValue.ReferenceTime;
        return E_OK;
    }
    else
    {
#if (SAB_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(SAB_MODULE_ID, 0, SAB_LOWLEVEL_TIMER_API, SAB_E_HARDWARE_TIMER);
#endif
        return E_NOT_OK;
    }

#else
#if (SAB_DEV_ERROR_DETECT == STD_ON)
    Det_ReportError(SAB_MODULE_ID, 0, SAB_LOWLEVEL_TIMER_API, SAB_E_NO_SERVICE);
#endif
    return E_NOT_OK;
#endif /* (SAB_HUNDRED_NANOSECONDPRECISION == STD_ON) */
}

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
Std_ReturnType Sab_GetTimeSpan100ns32bit(const Sab_PredefTimer100ns32bitType* TimerPtr, uint32* TimeSpanPtr)
{
#if (SAB_HUNDRED_NANOSECONDPRECISION == STD_ON)
    Sab_PredefTimer100ns32bitType currentValue;
    uint32 referenceTimeValue;

    Sab_CheckInitialized(SAB_FUNCTION_GETTIMESPAN100NS32BIT);
    if (NULL == TimerPtr)
    {
#if (SAB_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(SAB_MODULE_ID, 0, SAB_LOWLEVEL_TIMER_API, SAB_E_PARAM_POINTER);
#endif
        return E_NOT_OK;
    }

    if (NULL == TimeSpanPtr)
    {
#if (SAB_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(SAB_MODULE_ID, 0, SAB_LOWLEVEL_TIMER_API, SAB_E_PARAM_POINTER);
#endif
        return E_NOT_OK;
    }

    referenceTimeValue = TimerPtr->ReferenceTime;
    if (TRUE == Sab_Gpt_GetCurrentHwCounter100ns(&currentValue))
    {
        uint32 currentTimeValue = currentValue.ReferenceTime;
        if (currentTimeValue >= referenceTimeValue)
        {
            (*TimeSpanPtr) = currentTimeValue - referenceTimeValue;
            return E_OK;
        }
        else
        {
            (*TimeSpanPtr) = (SAB_UINT32_MAXVALUE - referenceTimeValue) + currentTimeValue;
            return E_OK;
        }
    }
    else
    {
#if (SAB_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(SAB_MODULE_ID, 0, SAB_LOWLEVEL_TIMER_API, SAB_E_HARDWARE_TIMER);
#endif
        return E_NOT_OK;
    }
#else
#if (SAB_DEV_ERROR_DETECT == STD_ON)
    Det_ReportError(SAB_MODULE_ID, 0, SAB_LOWLEVEL_TIMER_API, SAB_E_NO_SERVICE);
#endif
    return E_NOT_OK;
#endif /* (SAB_HUNDRED_NANOSECONDPRECISION == STD_ON) */
}

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
void Sab_ShiftTimer100ns32bit(Sab_PredefTimer100ns32bitType* TimerPtr, uint32 TimeValue)
{
#if (SAB_HUNDRED_NANOSECONDPRECISION == STD_ON)
    uint32 referenceValue;

    Sab_CheckInitialized(SAB_FUNCTION_SHIFTTIMER100NS32BIT);
    if (NULL == TimerPtr)
    {
#if (SAB_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(SAB_MODULE_ID, 0, SAB_LOWLEVEL_TIMER_API, SAB_E_PARAM_POINTER);
#endif
        return;
    }

    referenceValue = TimerPtr->ReferenceTime;
    TimerPtr->ReferenceTime = referenceValue + TimeValue;
#else
#if (SAB_DEV_ERROR_DETECT == STD_ON)
    Det_ReportError(SAB_MODULE_ID, 0, SAB_LOWLEVEL_TIMER_API, SAB_E_NO_SERVICE);
#endif
#endif /* #if (SAB_HUNDRED_NANOSECONDPRECISION == STD_ON) */
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_SyncTimer100ns32bit
 ***
 *** DESCRIPTION:
 ***     Synchronizes two timers. This means, that the value of the destination timer is set to
 ***     to the value of the source timer. The underlying timer uses a resolution of 100 nano second and a
 ***     precision of 32 bit. Thus this timer will overflow after 429,4967296 seconds.
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
void Sab_SyncTimer100ns32bit(Sab_PredefTimer100ns32bitType* TimerDstPtr, const Sab_PredefTimer100ns32bitType* TimerSrcPtr)
{
#if (SAB_HUNDRED_NANOSECONDPRECISION == STD_ON)

    Sab_CheckInitialized(SAB_FUNCTION_SYNCTIMER100NS32BIT);
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
#else
#if (SAB_DEV_ERROR_DETECT == STD_ON)
    Det_ReportError(SAB_MODULE_ID, 0, SAB_LOWLEVEL_TIMER_API, SAB_E_NO_SERVICE);
#endif
#endif /* SAB_HUNDRED_NANOSECONDPRECISION == STD_ON */
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_BusyWait100ns32bit
 ***
 *** DESCRIPTION:
 ***     Causes the current process to wait busy for the desired time span in timer ticks.
 ***     The underlying timer uses a resolution of 100 nano second and a
 ***     precision of 32 bit. Thus this timer will overflow after 429,4967296 seconds.
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
Std_ReturnType Sab_BusyWait100ns32bit(uint16 WaitingTimeMin)
{
#if (SAB_HUNDRED_NANOSECONDPRECISION == STD_ON)
    Sab_PredefTimer100ns32bitType entranceValue;
    uint32 timeElapsed;
    Std_ReturnType callReturnValue;

    Sab_CheckInitialized(SAB_FUNCTION_BUSYWAIT100NS32BIT);
    if (TRUE != Sab_Gpt_GetCurrentHwCounter100ns(&entranceValue))
    {
#if (SAB_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(SAB_MODULE_ID, 0, SAB_LOWLEVEL_TIMER_API, SAB_E_HARDWARE_TIMER);
#endif
        return E_NOT_OK;
    }

    callReturnValue = E_OK;
    for (;;)
    {
        if (E_NOT_OK == Sab_GetTimeSpan100ns32bit(&entranceValue, &timeElapsed))
        {
            callReturnValue = E_NOT_OK;
            break;
        }

        if (WaitingTimeMin < timeElapsed)
        {
            break;
        }
    }

    return callReturnValue;
#else
#if (SAB_DEV_ERROR_DETECT == STD_ON)
    Det_ReportError(SAB_MODULE_ID, 0, SAB_LOWLEVEL_TIMER_API, SAB_E_NO_SERVICE);
#endif
    return E_NOT_OK;
#endif /* SAB_HUNDRED_NANOSECONDPRECISION == STD_ON */
}

#endif /* SAB_HWTIMER_C */

/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/
