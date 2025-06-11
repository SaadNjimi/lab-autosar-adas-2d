/**************************************************************************************************\
 *** Sab.c
 ***
 *** Implementation of the Simulator Abstraction layer for VEOS.
 ***
 *** Copyright 2019 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef SAB_C
#define SAB_C

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/
#include "Sab_Can.h"
#include "Sab_Lin.h"
#include "Sab_Callbacks.h"
#include "Sab.h"
#include "Sab_Veos.h"
#include "Std_Types.h"
#include "VEOS.h"
#include "VEcuAppFrm_Sab.h"
#include "VEcuAppFrm.h"

#if (SAB_DEV_ERROR_DETECT == STD_ON)
#include "Det.h"
#endif

#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>

#include "Os.h"
#include "Os_SimulationCore.h"

/*------------------------------------------------------------------------------------------------*\
  DEFINES
\*------------------------------------------------------------------------------------------------*/

#define SAB_SUBSERVICE_ECUOFF 0x00000001

/*------------------------------------------------------------------------------------------------*\
  VARIABLE DEFINITIONS
\*------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C"
{
#endif
#if 0
}
#endif
extern boolean g_Sab_Autostart;
#if 0
{
#endif
#ifdef __cplusplus
}
#endif

/* Equivalent for Pin 30 */
extern volatile boolean Sab_StartupVpu;

/* Equivalent for PIN 15 */
extern volatile boolean Sab_ResetVpu;

Sab_State g_SabState = SAB_UNINIT;
/*------------------------------------------------------------------------------------------------*\
  FORWARD DECLARATIONS
\*------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif
#if 0
}
#endif

/* External declarations from Sab_SwTimer.c */

extern boolean Sab_OsRunning;
extern float64 Sab_VirtualStartTime;
extern float64 Sab_RealStartTime;
extern Sab_TickType Sab_VirtualTicks;
extern Sab_TickType Sab_RealTicks;

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_SetVariablesToInitialValues
 ***
 *** DESCRIPTION:
 ***     Sets the variables to their initial values. Earlier this was done in the functions Sab_Init()
 ***     and Sab_DeInit(). This new function will be called whenever the Sab_CurrentState becomes Sab_Off.
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
void Sab_SetVariablesToInitialValues(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_ReportIllegalApplicationState
 ***
 *** DESCRIPTION:
 ***     Will cause a fatal simulation error because an application state change has taken place,
 ***     which is not supported by the simulator backplane.
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     Sab_ApplicationState  state        Value of the unsupported application state entered.
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
void Sab_ReportIllegalApplicationState(Sab_ApplicationState state);

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
extern void Sab_Gpt_Init(void);

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
extern void Sab_Gpt_DeInit(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_StartModel
 ***
 *** DESCRIPTION:
 ***     Starts the model using the configured start-up function.
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
extern void Sab_StartModel(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_GetResourceLocationFullPathName
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
extern char* Sab_GetResourceLocationFullPathName(void);

#if 0
{
#endif
#ifdef __cplusplus
}
#endif

/*------------------------------------------------------------------------------------------------*\
  VARIABLE DECLARATIONS
\*------------------------------------------------------------------------------------------------*/

Sab_ApplicationState Sab_CurrentState;

/*------------------------------------------------------------------------------------------------*\
  FUNCTION DEFINITIONS
\*------------------------------------------------------------------------------------------------*/

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_Init
 ***
 *** DESCRIPTION:
 ***     Initialization function for the simulator abstraction layer.
 ***
 *** PARAMETERS:
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
void Sab_Init(void)
{
    Sab_CurrentState = Sab_Off;
    Sab_SetVariablesToInitialValues();

#ifdef SAB_GPT_DRIVERSUPPORTED
    Sab_Gpt_Init();
#endif /* SAB_GPT_DRIVERSUPPORTED */
    g_SabState = SAB_INIT;
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_DeInit
 ***
 *** DESCRIPTION:
 ***     De-Initialization function for the simulator abstraction layer.
 ***
 *** PARAMETERS:
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
void Sab_DeInit(void)
{
    Sab_ApplicationStateEntry(Sab_Off);

#ifdef SAB_GPT_DRIVERSUPPORTED
    Sab_Gpt_DeInit();
#endif /* SAB_GPT_DRIVERSUPPORTED */
    g_SabState = SAB_UNINIT;
}




/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_CanGetBoardAddress
 ***
 *** DESCRIPTION:
 ***     Tries to retrieve the CAN board address for the specified hardware handle.
 ***
 *** PARAMETERS:
 ***     Type                   Name            Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     Sab_HwHandleType       HardwareHandle  The hardware handle to get the address for.
 ***     Sab_BoardAddressType * pBoardAddress   The variable to store the board address in.
 ***
 *** RETURNS:
 ***     Std_ReturnType
 ***            E_OK            If the address was determined successfully.
 ***            E_NOT_OK        If the address could either not be determined or not be stored.
\**************************************************************************************************/
Std_ReturnType Sab_CanGetBoardAddress(Sab_HwHandleType HardwareHandle, Sab_BoardAddressType* pBoardAddress)
{
    if (NULL == pBoardAddress)
    {
#if (SAB_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(SAB_MODULE_ID, 0, SAB_CAN_BOARD_FUNCTION, SAB_CAN_E_PARAM_POINTER);
#else
        VEcuAppFrm_ReportError(VEcuAppFrm_DevelopmentError, SAB_MODULE_ID, 0, SAB_CAN_BOARD_FUNCTION, SAB_CAN_E_PARAM_POINTER);
#endif /* SAB_DEV_ERROR_DETECT == STD_ON */
    }

    if (SAB_CAN_MAX_HWHANDLE <= HardwareHandle)
    {
#if (SAB_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(SAB_MODULE_ID, 0, SAB_CAN_BOARD_FUNCTION, SAB_CAN_E_PARAM_VALUE);
#else
        VEcuAppFrm_ReportError(VEcuAppFrm_DevelopmentError, SAB_MODULE_ID, 0, SAB_CAN_BOARD_FUNCTION, SAB_CAN_E_PARAM_VALUE);
#endif /* SAB_DEV_ERROR_DETECT == STD_ON */
    }

    *pBoardAddress = HardwareHandle;
    return E_OK;
}

/**************************************************************************************************\
*** FUNCTION:
***     Sab_CanGetChannelAddress
***
*** DESCRIPTION:
***     Tries to retrieve the CAN channel address for the specified hardware handle.
***
*** PARAMETERS:
***     Type                     Name            Description
***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
***     Sab_HwHandleType         HardwareHandle  The hardware handle to get the address for.
***     Sab_ChannelAddressType * pChannelAddress The variable to store the channel address in.
***
*** RETURNS:
***     Std_ReturnType
***            E_OK            If the address was determined successfully.
***            E_NOT_OK        If the address could either not be determined or not be stored.
\**************************************************************************************************/
Std_ReturnType Sab_CanGetChannelAddress(Sab_HwHandleType HardwareHandle, Sab_ChannelAddressType* pChannelAddress)
{
    if (NULL == pChannelAddress)
    {
#if (SAB_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(SAB_MODULE_ID, 0, SAB_CAN_CHANNEL_FUNCTION, SAB_CAN_E_PARAM_POINTER);
#else
        VEcuAppFrm_ReportError(VEcuAppFrm_DevelopmentError, SAB_MODULE_ID, 0, SAB_CAN_CHANNEL_FUNCTION, SAB_CAN_E_PARAM_POINTER);
#endif /* SAB_DEV_ERROR_DETECT == STD_ON */
    }

    if (SAB_CAN_MAX_HWHANDLE <= HardwareHandle)
    {
#if (SAB_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(SAB_MODULE_ID, 0, SAB_CAN_CHANNEL_FUNCTION, SAB_CAN_E_PARAM_VALUE);
#else
        VEcuAppFrm_ReportError(VEcuAppFrm_DevelopmentError, SAB_MODULE_ID, 0, SAB_CAN_CHANNEL_FUNCTION, SAB_CAN_E_PARAM_VALUE);
#endif /* SAB_DEV_ERROR_DETECT == STD_ON */
    }

    *pChannelAddress = HardwareHandle;
    return E_OK;
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_LinGetBoardAddress
 ***
 *** DESCRIPTION:
 ***     Tries to retrieve the LIN board address for the specified hardware handle.
 ***
 *** PARAMETERS:
 ***     Type                   Name            Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     Sab_HwHandleType       HardwareHandle  The hardware handle to get the address for.
 ***     Sab_BoardAddressType * pBoardAddress   The variable to store the board address in.
 ***
 *** RETURNS:
 ***     Std_ReturnType
 ***            E_OK            If the address was determined successfully.
 ***            E_NOT_OK        If the address could either not be determined or not be stored.
\**************************************************************************************************/
Std_ReturnType Sab_LinGetBoardAddress(Sab_HwHandleType HardwareHandle, Sab_BoardAddressType* pBoardAddress)
{
    if (NULL == pBoardAddress)
    {
#if (SAB_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(SAB_MODULE_ID, 0, SAB_LIN_BOARD_FUNCTION, SAB_LIN_E_PARAM_POINTER);
#else
        VEcuAppFrm_ReportError(VEcuAppFrm_DevelopmentError, SAB_MODULE_ID, 0, SAB_LIN_BOARD_FUNCTION, SAB_LIN_E_PARAM_POINTER);
#endif /* SAB_DEV_ERROR_DETECT == STD_ON */
    }

    if (SAB_LIN_MAX_HWHANDLE <= HardwareHandle)
    {
#if (SAB_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(SAB_MODULE_ID, 0, SAB_LIN_BOARD_FUNCTION, SAB_LIN_E_PARAM_VALUE);
#else
        VEcuAppFrm_ReportError(VEcuAppFrm_DevelopmentError, SAB_MODULE_ID, 0, SAB_LIN_BOARD_FUNCTION, SAB_LIN_E_PARAM_VALUE);
#endif /* SAB_DEV_ERROR_DETECT == STD_ON */
    }

    *pBoardAddress = HardwareHandle;
    return E_OK;
}

/**************************************************************************************************\
*** FUNCTION:
***     Sab_LinGetChannelAddress
***
*** DESCRIPTION:
***     Tries to retrieve the LIN channel address for the specified hardware handle.
***
*** PARAMETERS:
***     Type                     Name            Description
***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
***     Sab_HwHandleType         HardwareHandle  The hardware handle to get the address for.
***     Sab_ChannelAddressType * pChannelAddress The variable to store the channel address in.
***
*** RETURNS:
***     Std_ReturnType
***            E_OK            If the address was determined successfully.
***            E_NOT_OK        If the address could either not be determined or not be stored.
\**************************************************************************************************/
Std_ReturnType Sab_LinGetChannelAddress(Sab_HwHandleType HardwareHandle, Sab_ChannelAddressType* pChannelAddress)
{
    if (NULL == pChannelAddress)
    {
#if (SAB_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(SAB_MODULE_ID, 0, SAB_LIN_CHANNEL_FUNCTION, SAB_LIN_E_PARAM_POINTER);
#else
        VEcuAppFrm_ReportError(VEcuAppFrm_DevelopmentError, SAB_MODULE_ID, 0, SAB_LIN_CHANNEL_FUNCTION, SAB_LIN_E_PARAM_POINTER);
#endif /* SAB_DEV_ERROR_DETECT == STD_ON */
    }

    if (SAB_LIN_MAX_HWHANDLE <= HardwareHandle)
    {
#if (SAB_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(SAB_MODULE_ID, 0, SAB_LIN_CHANNEL_FUNCTION, SAB_LIN_E_PARAM_VALUE);
#else
        VEcuAppFrm_ReportError(VEcuAppFrm_DevelopmentError, SAB_MODULE_ID, 0, SAB_LIN_CHANNEL_FUNCTION, SAB_LIN_E_PARAM_VALUE);
#endif /* SAB_DEV_ERROR_DETECT == STD_ON */
    }

    *pChannelAddress = HardwareHandle;
    return E_OK;
}

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
 ***     Sab_NotificationIdType*        pId             Pointer that contains a unique notification ID.
 ***                                                    Maximum number of notification PID is limited to 65535 (2^16)-1
 *** RETURNS:
 ***     Std_ReturnType                 SAB_E_OK or error code.
\**************************************************************************************************/
Std_ReturnType Sab_CreateNotification(Sab_NotificationFunctionType pFunction, const char* pName, Sab_NotificationIdType* pId)
{
    return VEcuAppFrm_CreateNotification( pFunction, pName, pId );
}

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
 ***     Std_ReturnType                           SAB_E_OK or error code.
\**************************************************************************************************/
Std_ReturnType Sab_StartRelTimeNotification( Sab_NotificationIdType id, Sab_NotificationTicksType offsetInTicks, Sab_NotificationTicksType periodInTicks)
{
    return VEcuAppFrm_StartRelTimeNotification( id, offsetInTicks, periodInTicks );
}

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
 ***     Sab_NotificationIdType       id              ID of the notification to be terminated.
 ***
 *** RETURNS:
 ***     Std_ReturnType                           SAB_E_OK or error code.
\**************************************************************************************************/
Std_ReturnType Sab_CancelNotification (Sab_NotificationIdType id)
{
    return VEcuAppFrm_CancelNotification( id );
}

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
void Sab_SleepActivity(void)
{
    if (Sab_CurrentState != Sab_Sleep)
    {
        Sab_ApplicationStateExit(Sab_CurrentState);
        Sab_ApplicationStateEntry(Sab_Sleep);
    }
    Sab_BreakInfiniteLoop();
}

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
void Sab_WakeupActivity(void)
{
    Os_ResumeResumableTask();
    if (Sab_CurrentState != Sab_Run)
    {
        Sab_ApplicationStateExit(Sab_CurrentState);
        Sab_ApplicationStateEntry(Sab_Wakeup);

        /* Prepare Real-time testing from sleep to running */
        VEcuAppFrm_EnsureInfLoopExperimentation(VEOS_TRUE);

        Sab_ApplicationStateExit(Sab_CurrentState);
        Sab_ApplicationStateEntry(Sab_Run);
    }
}

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
void Sab_BreakInfiniteLoop(void)
{
    /* Real-time testing is called by the V-ECU ApplicationFrame and is running in the regular time steps */
    VEcuAppFrm_EnsureInfLoopExperimentation(VEOS_FALSE);

    /* Lower prio task are still in task state "READY", but are not running during a sleep scenario */
    Os_DisableAllLowerPrioTasks();

    /* Jump to IDLE_LOOP and finish simulation step   */
    Os_WaitForTaskResumption();
}



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
Sab_UInt64 Sab_GetBaseAddress(void)
{
    return (Sab_UInt64)((uintptr_t)VEOS_CURRENT_APPLICATIONBASEADDRESS());
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_GetTargetAddress
 ***
 *** DESCRIPTION:
 ***     Returns the address under which an A2L variable can be read or write.
 ***
 *** PARAMETERS:
 ***     Type           Name               Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     Sab_UInt64   relativeAddress    The relative addresses of an A2L symbol (In-Value).
 ***
 *** RETURNS:
 ***     Sab_UInt64   Target physical address
 \**************************************************************************************************/
Sab_UInt64 Sab_GetTargetAddress(Sab_UInt64 relativeAddress)
{
    return ((Sab_UInt64)((uintptr_t)VEOS_CURRENT_APPLICATIONBASEADDRESS())) + relativeAddress;
}


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
char* Sab_GetResourceLocation(void)
{
    return Sab_GetResourceLocationFullPathName();
}




/**************************************************************************************************\
 *** internal Sab functions
\**************************************************************************************************/

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_CanStart
 ***
 *** DESCRIPTION:
 ***     Checks whether the Sab-module can start.
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     boolean
 ***            TRUE         If the Sab-module is ready to start.
 ***            FALSE        Else.
\**************************************************************************************************/
boolean Sab_CanStart(void)
{
    return Sab_StartupVpu && Sab_ResetVpu;
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_SetVariablesToInitialValues
 ***
 *** DESCRIPTION:
 ***     Sets the variables to their initial values. Earlier this was done in the functions Sab_Init()
 ***     and Sab_DeInit(). This new function will be called whenever the Sab_CurrentState becomes Sab_Off.
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
void Sab_SetVariablesToInitialValues(void)
{
    /* Set variables to initial values */
    Sab_OsRunning = FALSE;
    Sab_VirtualStartTime = 0.0;
    Sab_RealStartTime = 0.0;
    Sab_VirtualTicks = 0;
    Sab_RealTicks = 0;
    Sab_StartupVpu = g_Sab_Autostart;
    Sab_ResetVpu = g_Sab_Autostart;
}


/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_ApplicationStateEntry
 ***
 *** DESCRIPTION:
 ***     Callout for the ECU State manager or other application state managing BSW modules to
 ***     indicate that new application state has been entered.
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     Sab_ApplicationState  state        The new application state.
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
void Sab_ApplicationStateEntry(Sab_ApplicationState state)
{
    if (Sab_CurrentState == state)
    {
        return;
    }

    switch (Sab_CurrentState)
    {
        case Sab_Off:
        {
            switch (state)
            {
                case Sab_Startup:
                {
#ifdef VEOS_DEBUG
                    VEOS_MsgApi_LogMessage(SAB_MODULE_NAME, __FUNCTION__, "VPU entered Startup state.");
#endif /* VEOS_DEBUG */
                    /*  Invoke backplane specific activities to start the run */
                    VEcuAppFrm_ModelStateEventBoot();
                }
                break;
                case Sab_Run:
                {
#ifdef VEOS_DEBUG
                    VEOS_MsgApi_LogMessage(SAB_MODULE_NAME, __FUNCTION__, "VPU entered Startup state.");
#endif /* VEOS_DEBUG */
                    /*  Invoke backplane specific activities to start the run */
                    VEcuAppFrm_ModelStateEventBoot();

#ifdef VEOS_DEBUG
                    VEOS_MsgApi_LogMessage(SAB_MODULE_NAME, __FUNCTION__, "VPU entered Run state.");
#endif /* VEOS_DEBUG */
                    /*  Invoke backplane specific activities to start the run */
                    VEcuAppFrm_ModelStateEventRun();

                    Sab_VirtualStartTime = g_VEOS_CurrentSimulationTime;
                }
                break;
                default:
                {
                    Sab_ReportIllegalApplicationState(state);
                    return;
                }
                break;
            }
        }
        break;
        case Sab_Wakeup:
        {
            switch (state)
            {
                case Sab_Run:
                {
#ifdef VEOS_DEBUG
                    VEOS_MsgApi_LogMessage(SAB_MODULE_NAME, __FUNCTION__, "VPU entered Run state.");
#endif /* VEOS_DEBUG */
                }
                break;
                default:
                {
                    Sab_ReportIllegalApplicationState(state);
                    return;
                }
                break;
            }
        }
        break;
        case Sab_Sleep:
        {
            switch (state)
            {
                case Sab_ShutDown:
                {
#ifdef VEOS_DEBUG
                    VEOS_MsgApi_LogMessage(SAB_MODULE_NAME, __FUNCTION__, "VPU entered ShutDown state");
#endif /* VEOS_DEBUG */
                    /*  Invoke backplane specific activities to start the shutdown */
                    VEcuAppFrm_ModelStateEventTerminate();
                }
                break;
                case Sab_Off:
                {
#ifdef VEOS_DEBUG
                    VEOS_MsgApi_LogMessage(SAB_MODULE_NAME, __FUNCTION__, "VPU entered ShutDown state");
#endif /* VEOS_DEBUG */
                    /*  Invoke backplane specific activities to start the shutdown */
                    VEcuAppFrm_ModelStateEventTerminate();

#ifdef VEOS_DEBUG
                    VEOS_MsgApi_LogMessage(SAB_MODULE_NAME, __FUNCTION__, "VPU entered Off state");
#endif /* VEOS_DEBUG */
                    /* Set variables to initial values */
                    Sab_SetVariablesToInitialValues();

                    /*  Invoke backplane specific activities to stop the VPU */
                    VEcuAppFrm_ModelStateEventOff();

                    Sab_VirtualStartTime = 0.0;
                }
                break;
                case Sab_Wakeup:
                {
#ifdef VEOS_DEBUG
                    VEOS_MsgApi_LogMessage(SAB_MODULE_NAME, __FUNCTION__, "VPU entered Wakeup state.");
#endif /* VEOS_DEBUG */
                }
                break;
                default:
                {
                    Sab_ReportIllegalApplicationState(state);
                    return;
                }
                break;
            }
        }
        break;
        case Sab_Startup:
        {
            switch (state)
            {
                case Sab_Run:
                {
#ifdef VEOS_DEBUG
                    VEOS_MsgApi_LogMessage(SAB_MODULE_NAME, __FUNCTION__, "VPU entered Run state.");
#endif /* VEOS_DEBUG */

                    /*  Invoke backplane specific activities to start the run */
                    VEcuAppFrm_ModelStateEventRun();

                    if ( SAB_E_NOT_OK == Sab_ResetTargetTimer1ns64bit(&Sab_PredefTargetTimer_StartSimulation_1ns64bit))
                    {
                        VEOS_MsgApi_ShowMessage(VEOS_MsgApi_SeverityType_Error, VEOS_TRUE, "The global predefined timer instance 'Sab_PredefTargetTimer_StartSimulation_1ns64bit' cannot be initialized.");
                        VEOS_SimApi_TerminateSimulationFatal();
                    }

                    Sab_VirtualStartTime = g_VEOS_CurrentSimulationTime;
                }
                break;
                default:
                {
                    Sab_ReportIllegalApplicationState(state);
                    return;
                }
                break;
            }
        }
        break;
        case Sab_Run:
        {
            switch (state)
            {
                case Sab_Sleep:
                {
#ifdef VEOS_DEBUG
                    VEOS_MsgApi_LogMessage(SAB_MODULE_NAME, __FUNCTION__, "VPU entered Sleep state");
#endif /* VEOS_DEBUG */
                }
                break;
                case Sab_ShutDown:
                {
#ifdef VEOS_DEBUG
                    VEOS_MsgApi_LogMessage(SAB_MODULE_NAME, __FUNCTION__, "VPU entered ShutDown state");
#endif /* VEOS_DEBUG */
                    /*  Invoke backplane specific activities to start the shutdown */
                    VEcuAppFrm_ModelStateEventTerminate();
                }
                break;
                case Sab_Off:
                {
#ifdef VEOS_DEBUG
                    VEOS_MsgApi_LogMessage(SAB_MODULE_NAME, __FUNCTION__, "VPU entered ShutDown state");
#endif /* VEOS_DEBUG */
                    /*  Invoke backplane specific activities to start the shutdown */
                    VEcuAppFrm_ModelStateEventTerminate();

#ifdef VEOS_DEBUG
                    VEOS_MsgApi_LogMessage(SAB_MODULE_NAME, __FUNCTION__, "VPU entered Off state");
#endif /* VEOS_DEBUG */
                    /* Set variables to initial values */
                    Sab_SetVariablesToInitialValues();

                    /*  Invoke backplane specific activities to stop the VPU */
                    VEcuAppFrm_ModelStateEventOff();

                    Sab_VirtualStartTime = 0.0;
                }
                break;
                default:
                {
                    Sab_ReportIllegalApplicationState(state);
                    return;
                }
                break;
            }
        }
        break;
        case Sab_ShutDown:
        {
            switch (state)
            {
                case Sab_Off:
                {
#ifdef VEOS_DEBUG
                    VEOS_MsgApi_LogMessage(SAB_MODULE_NAME, __FUNCTION__, "VPU entered Off state");
#endif /* VEOS_DEBUG */
                    /* Set variables to initial values */
                    Sab_SetVariablesToInitialValues();

                    /*  Invoke backplane specific activities to stop the VPU */
                    VEcuAppFrm_ModelStateEventOff();

                    Sab_VirtualStartTime = 0.0;
                }
                break;
                case Sab_Startup:
                {
#ifdef VEOS_DEBUG
                    VEOS_MsgApi_LogMessage(SAB_MODULE_NAME, __FUNCTION__, "VPU entered Startup state.");
#endif /* VEOS_DEBUG */
                    /*  Invoke backplane specific activities to start the run */
                    VEcuAppFrm_ModelStateEventBoot();
                }
                break;
                default:
                {
                    Sab_ReportIllegalApplicationState(state);
                    return;
                }
                break;
            }
        }
        break;
        /* Sab_Sleep: */
        /* Sab_WakeUp: */
        default:
        {
#if (SAB_DEV_ERROR_DETECT == STD_ON)
            Det_ReportError(SAB_MODULE_ID, 0, SAB_STATETRANSITION_API, SAB_E_PARAM_VALUE);
#endif
            VEOS_MsgApi_ShowMessageFormat(VEOS_MsgApi_SeverityType_Warning, VEOS_TRUE, "Unsupported simulation state '%d'.", Sab_CurrentState);
        }
    }
    Sab_CurrentState = state;
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_ApplicationStateExit
 ***
 *** DESCRIPTION:
 ***     Callout for the ECU State manager or other application state managing BSW modules to
 ***     indicate that the current application state has been left.
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     Sab_ApplicationState  state        The old application state.
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
void Sab_ApplicationStateExit(Sab_ApplicationState state)
{
    switch (state)
    {
        case Sab_Startup:
        {
#ifdef VEOS_DEBUG
            VEOS_MsgApi_LogMessage(SAB_MODULE_NAME, __FUNCTION__, "VPU will try to leave Startup state.");
#endif /* VEOS_DEBUG */
        }
        break;
        case Sab_Run:
        {
#ifdef VEOS_DEBUG
            VEOS_MsgApi_LogMessage(SAB_MODULE_NAME, __FUNCTION__, "VPU will try to leave Run state.");
#endif /* VEOS_DEBUG */
        }
        break;
        case Sab_ShutDown:
        {
#ifdef VEOS_DEBUG
            VEOS_MsgApi_LogMessage(SAB_MODULE_NAME, __FUNCTION__, "VPU will try to leave ShutDown state");
#endif /* VEOS_DEBUG */
        }
        break;
        case Sab_Sleep:
        {
#ifdef VEOS_DEBUG
            VEOS_MsgApi_LogMessage(SAB_MODULE_NAME, __FUNCTION__, "VPU will try to leave Sleep state");
#endif /* VEOS_DEBUG */
        }
        break;
        case Sab_Wakeup:
        {
#ifdef VEOS_DEBUG
            VEOS_MsgApi_LogMessage(SAB_MODULE_NAME, __FUNCTION__, "VPU will try to leave Wakeup state");
#endif /* VEOS_DEBUG */
        }
        break;
        case Sab_Off:
        {
#ifdef VEOS_DEBUG
            VEOS_MsgApi_LogMessage(SAB_MODULE_NAME, __FUNCTION__, "VPU will try to leave Off state");
#endif /* VEOS_DEBUG */
        }
        break;
        default:
        {
        }
        break;
    }
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_ReportIllegalApplicationState
 ***
 *** DESCRIPTION:
 ***     Will cause a fatal simulation error because an application state change has taken place,
 ***     which is not supported by the simulator backplane.
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     Sab_ApplicationState  state        Value of the unsupported application state entered.
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
void Sab_ReportIllegalApplicationState(Sab_ApplicationState state)
{
#if (SAB_DEV_ERROR_DETECT == STD_ON)
    Det_ReportError(SAB_MODULE_ID, 0, SAB_STATETRANSITION_API, SAB_E_PARAM_VALUE);
#endif
    VEOS_MsgApi_ShowMessageFormat(VEOS_MsgApi_SeverityType_Warning, VEOS_TRUE, "Unsupported change of simulation state from '%d' to '%d'.", Sab_CurrentState, state);
}


/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_OnSimulationStopped
 ***
 *** DESCRIPTION:
 ***     Called when the VPU has received a simulation stopped event.
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
void Sab_OnSimulationStopped(void)
{
    if (Sab_Run == Sab_CurrentState)
    {
        Sab_ApplicationStateExit(Sab_Run);
        Sab_ApplicationStateEntry(Sab_ShutDown);
    }
    else
    {
        Sab_CurrentState = Sab_ShutDown;
    }
}


/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_CheckInitialized
 ***
 *** DESCRIPTION:
 ***     Checkes whether the SAB module was initialized yet.
 ***
 *** PARAMETERS:
 ***     Type           Name               Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     uint8         functionType        The function type.
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
void Sab_CheckInitialized(uint8 functionType)
{
    if (g_SabState != SAB_INIT)
    {
        VEcuAppFrm_ReportError(VEcuAppFrm_DevelopmentError, SAB_MODULE_ID, 0, functionType, SAB_E_UNINT);
    }
}




/**************************************************************************************************\
 *** obsolete Sab functions
\**************************************************************************************************/



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
void Sab_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
#ifdef VEOS_DEBUG
    VEOS_MsgApi_LogMessage(SAB_MODULE_NAME, __FUNCTION__, "Sab_GetVersionInfo() called.");
#endif

    /* Error check. */
    if (NULL == versioninfo)
    {
        return;
    }

    versioninfo->vendorID = SAB_VENDOR_ID;
    versioninfo->moduleID = SAB_MODULE_ID;
    versioninfo->sw_major_version = SAB_SW_MAJOR_VERSION;
    versioninfo->sw_minor_version = SAB_SW_MINOR_VERSION;
    versioninfo->sw_patch_version = SAB_SW_PATCH_VERSION;
}



#endif /* SAB_C */

/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/
