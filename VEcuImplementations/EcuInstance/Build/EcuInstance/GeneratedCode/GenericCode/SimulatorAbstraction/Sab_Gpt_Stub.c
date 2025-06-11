/**************************************************************************************************\
 *** Sab_Gpt_Stub.c
 ***
 *** Implementation the general purpose timer for VEOS using the Windows API.
 ***
 *** Copyright 2019 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef SAB_GPT_STUB_C
#define SAB_GPT_STUB_C

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/

#include "Windows.h"

#include "Platform_Types.h"
#include "Std_Types.h"
#include "VEOS.h"

#include "Sab_Types.h"
#include "Sab_Cfg.h"
#include "Sab.h"
#include "Sab_Veos.h"

#if (SAB_DEV_ERROR_DETECT == STD_ON)
#include "Det.h"
#endif

/*------------------------------------------------------------------------------------------------*\
  PARAMETRIZED MACROS
\*------------------------------------------------------------------------------------------------*/

/* 4294967296 = 2^32 = 32 left shifts */
#define SAB_LARGEINT_TO_ULONGLONG(value) (((uint64)value.HighPart) * 4294967296) + value.LowPart
#define SAB_LONGLONG_TO_UINT32(value) (uint32)(value & 0xFFFFFFFF)

/*------------------------------------------------------------------------------------------------*\
  VARIABLE DECLARATIONS
\*------------------------------------------------------------------------------------------------*/
uint64 Sab_TimerResolution_1s = 0;

/*------------------------------------------------------------------------------------------------*\
  FUNCTION DEFINITIONS
\*------------------------------------------------------------------------------------------------*/

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_Gpt_Init
 ***
 *** DESCRIPTION:
 ***     Starts the general purpose timer for the simulator abstraction hardware timer.
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     Sab_ApplicationState  state        Value of the unsupported application state entered.
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
void Sab_Gpt_Init(void)
{
    LARGE_INTEGER frequency;
    BOOL apiResult = QueryPerformanceFrequency(&frequency);
    if (0 != apiResult)
    {
        /* The QueryPerformanceFrequency does not change during the simulation. */
        Sab_TimerResolution_1s = SAB_LARGEINT_TO_ULONGLONG(frequency);
    }
    else
    {
        DWORD lastError = GetLastError();
        VEOS_MsgApi_ShowMessageFormat(VEOS_MsgApi_SeverityType_Error, VEOS_TRUE, "Unable to initialize performance counter. API returned with error code %x", lastError);
        VEOS_SimApi_TerminateSimulation(VEOS_SimApi_TerminationKindType_StopSimulationAndFail);
    }
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_Gpt_DeInit
 ***
 *** DESCRIPTION:
 ***     Stops the general purpose timer for the simulator abstraction hardware timer.
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
void Sab_Gpt_DeInit(void)
{
}

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
boolean Sab_Gpt_GetCurrentHwCounter100ns(Sab_PredefTimer100ns32bitType *currentValue)
{
#if (SAB_HUNDRED_NANOSECONDPRECISION == STD_ON)
    LARGE_INTEGER hardwareCounter;
    BOOL apiResult;
    Sab_CheckInitialized(SAB_FUNCTION_GETCURRENTHWCOUNTER100NS);
#endif
    if (NULL == currentValue)
    {
#if (SAB_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(SAB_MODULE_ID, 0, SAB_LOWLEVEL_TIMER_API, SAB_E_PARAM_POINTER);
#endif
        return FALSE;
    }

    currentValue->ReferenceTime = 0;

#if (SAB_HUNDRED_NANOSECONDPRECISION == STD_ON)
    if (0 == Sab_TimerResolution_1s)
    {
#if (SAB_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(SAB_MODULE_ID, 0, SAB_LOWLEVEL_TIMER_API, SAB_E_INIT);
#endif
        return FALSE;
    }

    apiResult = QueryPerformanceCounter(&hardwareCounter);

    if (0 != apiResult)
    {
        /* The QueryPerformanceCounter (QPC) has to be multiplied with the SAB_GPT_RESOLUTION and
         * divided by the Sab_TimerResolution. The chosen order has big influence on the result:
         * - Division before multiplication can cause a overflow of the hwCounterValue
         * - Multiplication before division leads to a loss in accuracy
         * If the overflow leads to problems, there are several possible solutions:
         * - choose a larger data type for the interim result
         * - extend the Sab_PredefTimer100ns32bitType to store the original QPC-value. Thus timespans
         *   can be be calculated by subtracting the QPC-value. The result will be small enough to
         *   prevent overflows. */

        uint64 hwCounterValue = SAB_LARGEINT_TO_ULONGLONG(hardwareCounter);
        hwCounterValue *= (SAB_GPT_RESOLUTION_100NS / Sab_TimerResolution_1s);
        currentValue->ReferenceTime = SAB_LONGLONG_TO_UINT32(hwCounterValue);
    }
    else
    {
#if (SAB_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(SAB_MODULE_ID, 0, SAB_LOWLEVEL_TIMER_API, SAB_E_HARDWARE_TIMER);
#endif
        DWORD lastError = GetLastError();
        VEOS_MsgApi_ShowMessageFormat(VEOS_MsgApi_SeverityType_Error, VEOS_TRUE, "Unable to query performance counter. API returned with error code %x", lastError);
        VEOS_SimApi_TerminateSimulation(VEOS_SimApi_TerminationKindType_StopSimulationAndFail);
        return FALSE;
    }

    return TRUE;

#else
#if (SAB_DEV_ERROR_DETECT == STD_ON)
    Det_ReportError(SAB_MODULE_ID, 0, SAB_LOWLEVEL_TIMER_API, SAB_E_NO_SERVICE);
#endif
    return FALSE;
#endif /* SAB_HUNDRED_NANOSECONDPRECISION == STD_ON */
}

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
boolean Sab_Gpt_GetCurrentHwCounter1ns(Sab_PredefTimer1ns64bitType *currentValue)
{
#if (SAB_HUNDRED_NANOSECONDPRECISION == STD_ON)
    LARGE_INTEGER hardwareCounter;
    BOOL apiResult;
#endif

    Sab_CheckInitialized(SAB_FUNCTION_GETCURRENTHWCOUNTER1NS);
    if (NULL == currentValue)
    {
#if (SAB_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(SAB_MODULE_ID, 0, SAB_LOWLEVEL_TIMER_API, SAB_E_PARAM_POINTER);
#endif
        return FALSE;
    }
#ifdef SAB_UINT64_ATOMIC
    currentValue->ReferenceTime = SAB_UINT64_INITVALUE;
#else
    currentValue->ReferenceTime.high = SAB_UINT64_INITVALUE_HIGH;
    currentValue->ReferenceTime.low = SAB_UINT64_INITVALUE_LOW;
#endif

#if (SAB_HUNDRED_NANOSECONDPRECISION == STD_ON)
    if (0 == Sab_TimerResolution_1s)
    {
#if (SAB_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(SAB_MODULE_ID, 0, SAB_LOWLEVEL_TIMER_API, SAB_E_INIT);
#endif
        return FALSE;
    }

    apiResult = QueryPerformanceCounter(&hardwareCounter);

    if (0 != apiResult)
    {
        /* The QueryPerformanceCounter (QPC) has to be multiplied with the SAB_GPT_RESOLUTION and
         * divided by the Sab_TimerResolution. The chosen order has big influence on the result:
         * - Division before multiplication can cause a overflow of the hwCounterValue
         * - Multiplication before division leads to a loss in accuracy
         * If the overflow leads to problems, there are several possible solutions:
         * - choose a larger data type for the interim result
         * - extend the Sab_PredefTimer1ns64bitType to store the original QPC-value. Thus timespans
         *   can be be calculated by subtracting the QPC-value. The result will be small enough to
         *   prevent overflows. */

        uint64 hwCounterValue = SAB_LARGEINT_TO_ULONGLONG(hardwareCounter);
        hwCounterValue *= (SAB_GPT_RESOLUTION_1NS / Sab_TimerResolution_1s);
#ifdef SAB_UINT64_ATOMIC
        currentValue->ReferenceTime = hwCounterValue;
#else /* SAB_UINT64_ATOMIC */
        /* 2^32 = 4294967296.
        * ==> hwCounterValue / 4294967296 gives the 32 high bits.
        * ==> hwCounterValue % 4294967296 gives the 32 low bits. */
        currentValue->ReferenceTime.high = (uint32)(hwCounterValue / 4294967296);
        currentValue->ReferenceTime.low = hwCounterValue % 4294967296;
#endif /* SAB_UINT64_ATOMIC */
    }
    else
    {
        DWORD lastError;
#if (SAB_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(SAB_MODULE_ID, 0, SAB_LOWLEVEL_TIMER_API, SAB_E_HARDWARE_TIMER);
#endif
        lastError = GetLastError();
        VEOS_MsgApi_ShowMessageFormat(VEOS_MsgApi_SeverityType_Error, VEOS_TRUE, "Unable to query performance counter. API returned with error code %x", lastError);
        VEOS_SimApi_TerminateSimulation(VEOS_SimApi_TerminationKindType_StopSimulationAndFail);
        return FALSE;
    }

    return TRUE;
#else
#if (SAB_DEV_ERROR_DETECT == STD_ON)
    Det_ReportError(SAB_MODULE_ID, 0, SAB_LOWLEVEL_TIMER_API, SAB_E_NO_SERVICE);
#endif
    return FALSE;
#endif /* SAB_ONENANOSECONDPRECISION == STD_ON */
}

#endif /* SAB_GPT_STUB_C */

/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/
