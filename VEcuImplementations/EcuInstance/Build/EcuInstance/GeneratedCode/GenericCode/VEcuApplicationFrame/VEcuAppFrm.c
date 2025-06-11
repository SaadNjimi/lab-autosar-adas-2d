/**************************************************************************************************\
 *** VEcuAppFrm.c
 ***
 *** Application frame implementation for V-ECUs.
 ***
 *** Copyright 2019 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef VECUAPPFRM_C
#define VECUAPPFRM_C

/*------------------------------------------------------------------------------------------------*\
 DEFINES (OPT)
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
 INCLUDES
\*------------------------------------------------------------------------------------------------*/

#include "VEcuAppFrm.h"
#include "VEcuAppFrm_Cfg.h"
#include "VEcuAppFrm_VEOS.h"
#include "VEcuAppFrm_Modules.h"
#include "VEcuAppFrm_Internal.h"
#include "VEcuAppFrm_Sab.h"
#include "VEOS.h"

#if (VECUAPPFRM_DAP_AVAILABLE == VEOS_ON)
#include "Dap.h"
#endif /* VECUAPPFRM_DAP_AVAILABLE == VEOS_ON */

#if (VECUAPPFRM_SAB_AVAILABLE == VEOS_ON)
#include "Sab_Veos.h"
#endif /* VECUAPPFRM_SAB_AVAILABLE == VEOS_ON */

#if (VECUAPPFRM_OS_AVAILABLE == VEOS_ON)
#include "Os.h"
#include "Os_SimulationCore.h"
#endif /* VECUAPPFRM_OS_AVAILABLE == VEOS_ON */

#if (VECUAPPFRM_IDIF_AVAILABLE == VEOS_ON)
#include "IdealizedBusIf.h"
#endif /* VECUAPPFRM_IDIF_AVAILABLE == VEOS_ON */

#if (VECUAPPFRM_DET_UL_AVAILABLE == VEOS_ON)
#include "Det.h"

#if defined(DET_AR_RELEASE_MAJOR_VERSION) && defined(DET_AR_RELEASE_MINOR_VERSION) && defined(DET_AR_RELEASE_REVISION_VERSION)
#if (DET_AR_RELEASE_MAJOR_VERSION >= 0x04u) && (DET_AR_RELEASE_MINOR_VERSION >= 2)
/* Default error tracer functions must be available. */
#define VECUAPPFRM_TRANSIENT_AND_RUNTIMEERROR_AVAILABLE
#endif /* DET_AR_RELEASE_MAJOR_VERSION >= 4 & DET_AR_RELEASE_MINOR_VERSION >= 2*/
#endif /* DET_AR_RELEASE_MAJOR_VERSION & DET_AR_RELEASE_MINOR_VERSION & DET_AR_RELEASE_REVISION_VERSION */
#endif /* VECUAPPFRM_DET_UL_AVAILABLE == VEOS_ON */



/*------------------------------------------------------------------------------------------------*\
  FUNCTION PROTOTYPES
\*------------------------------------------------------------------------------------------------*/
#ifdef CTC_CODE_COVERAGE
    extern void ctc_append_all(void);
#endif /* CTC_CODE_COVERAGE */

/*------------------------------------------------------------------------------------------------*\
 DEFINES
\*------------------------------------------------------------------------------------------------*/
#ifdef CTC_CODE_COVERAGE
    #define SAVE_CODECOVERAGE_DATA() (void)ctc_append_all()
#else
    #define SAVE_CODECOVERAGE_DATA() (void)
#endif /* CTC_CODE_COVERAGE */


/*------------------------------------------------------------------------------------------------*\
 TYPEDEFS
\*------------------------------------------------------------------------------------------------*/

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

/**************************************************************************************************\
 * Event handling functions for VEOS Frame layer.                                                 *
\**************************************************************************************************/

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_Download
 ***
 *** DESCRIPTION:
 ***     Event handler function called right after the V-ECU has been downloaded to the simulator.
 ***
 *** PARAMETERS:
 ***     Type                    Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any error
 ***                            occurs, an error code will be the result. See VEOS_Types.h for
 ***                            details.
\**************************************************************************************************/
VEOS_ApiSuccessType VEcuAppFrm_Download(void)
{
    /* Just reset state management variable */
    g_VEcuAppFrm_State = VEcuAppFrm_State_Off;
    g_VEcuAppFrm_NumberOfDelayCycles = 0;
    /* This value must be zero to trigger stimulation at first trigger */
    g_VEcuAppFrm_StimCycles = 0;

    g_VEcuAppFrm_SimulatorShuttingDown = FALSE;

#if (VECUAPPFRM_SAB_AVAILABLE == VEOS_ON)
    /* Initialize simulator abstraction, if available */
    /* Sab_CanStart is called afterwards. Hence Simulator abstraction needs to be initialized. */
    Sab_Init();
    Sab_Call_PreStartUp_Functions();
#endif /* VECUAPPFRM_SAB_AVAILABLE == VEOS_ON */

    return VEOS_E_OK;
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_Start
 ***
 *** DESCRIPTION:
 ***     Event handler function called right after the simulator has started the simulation.
 ***
 *** PARAMETERS:
 ***     Type                    Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any error
 ***                            occurs, an error code will be the result. See VEOS_Types.h for
 ***                            details.
\**************************************************************************************************/
VEOS_ApiSuccessType VEcuAppFrm_Start(void)
{
    g_VEcuAppFrm_SimulatorShuttingDown = FALSE;
    return VEOS_E_OK;
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_Trigger
 ***
 *** DESCRIPTION:
 ***     Event handler function called on time-cycle base by the simulator to indicate a
 ***     new simulation step.
 ***
 *** PARAMETERS:
 ***     Type                    Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any error
 ***                            occurs, an error code will be the result. See VEOS_Types.h for
 ***                            details.
\**************************************************************************************************/
VEOS_ApiSuccessType VEcuAppFrm_Trigger(void)
{
    VEOS_ApiSuccessType success = VEOS_E_OK;

#ifdef VECUAPPFRM_PREHOOK
    /* Call hook */
    VECUAPPFRM_PREHOOK();
#endif /* VECUAPPFRM_PREHOOK */

    /* In delayed start-up scenarios, the application frame has not run its start method to completion. */
    if (VEcuAppFrm_State_Off == g_VEcuAppFrm_State)
    {
        /* Try to start V-ECU. If not, this is no error. */
        success = VEcuAppFrm_HandleStartEvent();

        /* Delayed start of V-ECU. */
        if (VECUAPPFRM_E_DELAYED == success)
        {
#if (VECUAPPFRM_SAB_AVAILABLE == VEOS_ON)
            /* Indicate a cyclic trigger to simulator abstraction. */
            Sab_VpuTriggered();
#endif /* (VECUAPPFRM_SAB_AVAILABLE == VEOS_ON) */

            /*
             * V-ECU startup was not done, e.g. due to missing Pin15/Pin30 signal.
             * Retry later, and report success to simulator.
             */
            return VEOS_E_OK;
        }

        if (VEOS_E_OK == success)
        {
            /* Call V-ECU startup procedure */
            success = VEcuAppFrm_ChangeState(VEcuAppFrm_State_Booting);
        }

        if (VEOS_E_OK != success)
        {
            return success;
        }
    }

    g_VEcuAppFrm_TriggerStep = VEOS_TRUE;
#if (VEOS_STIM_SERVICE_RTT == VEOS_ON)
    VEOS_ExpApi_StimRasterExpected(0);

#endif /* VEOS_STIM_SERVICE_RTT == VEOS_ON */

#if (VECUAPPFRM_OS_AVAILABLE == VEOS_ON)
    /* Trigger operating system's hardware counters. */
    Os_IsrSystemTimer();
#endif /* (VECUAPPFRM_OS_AVAILABLE == VEOS_ON) */

    VEcuAppFrm_TriggerStimulation();

#if (VECUAPPFRM_SAB_AVAILABLE == VEOS_ON)
    /* Indicate a cyclic trigger to simulator abstraction. */
    Sab_VpuTriggered();
#endif /* (VECUAPPFRM_SAB_AVAILABLE == VEOS_ON) */
    g_VEcuAppFrm_TriggerStep = VEOS_FALSE;

#ifdef VECUAPPFRM_POSTHOOK
    /* Call hook */
    VECUAPPFRM_POSTHOOK();
#endif /* VECUAPPFRM_POSTHOOK */

    return success;
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_Stop
 ***
 *** DESCRIPTION:
 ***     Event handler function called when the simulation is ending on time base or by explicit
 ***     user request via VEOS API or simulation clients.
 ***
 *** PARAMETERS:
 ***     Type                    Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any error
 ***                            occurs, an error code will be the result. See VEOS_Types.h for
 ***                            details.
\**************************************************************************************************/
VEOS_ApiSuccessType VEcuAppFrm_Stop(void)
{
    g_VEcuAppFrm_StimCycles = 0;

    g_VEcuAppFrm_SimulatorShuttingDown = TRUE;

#if (VECUAPPFRM_SAB_AVAILABLE == VEOS_ON)
    Sab_OnSimulationStopped();
#endif

    VEcuAppFrm_ResetStimulation();

#ifdef CTC_CODE_COVERAGE
    SAVE_CODECOVERAGE_DATA();
#endif /* CTC_CODE_COVERAGE */


    return VEcuAppFrm_ChangeState(VEcuAppFrm_State_ShuttingDown);
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_Reset
 ***
 *** DESCRIPTION:
 ***     Event handler function called when the simulation is ending and 
 ***     after VEOS_ApiSuccessType VEcuAppFrm_Stop() is called on time base or by explicit
 ***     user request via VEOS API or simulation clients.
 ***
 *** PARAMETERS:
 ***     Type                    Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     void * const     VpuData           Incoming argument: The pointer to the VPU data.
 ***
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any error
 ***                            occurs, an error code will be the result. See VEOS_Types.h for
 ***                            details.
\**************************************************************************************************/
VEOS_ApiSuccessType VEcuAppFrm_Reset(void * const VpuData)
{
    /* to avoid the compiler warning C4100 */
    (void)(VpuData);
    return VEcuAppFrm_Download();
}



/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_HandleIoEvent
 ***
 *** DESCRIPTION:
 ***     Event handler function called when the model has encountered an I/O event. This event is
 ***     routed from VEOS Frame to the handler.
 ***
 *** PARAMETERS:
 ***     Type                    Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     const VEOS_uint32       ioEventId    Unique ID of the I/O event to handle.
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any error
 ***                            occurs, an error code will be the result. See VEOS_Types.h for
 ***                            details.
\**************************************************************************************************/
VEOS_ApiSuccessType VEcuAppFrm_HandleIoEvent(const VEOS_uint32 ioEventId)
{
#if (VECUAPPFRM_IO_AVAILABLE == VEOS_ON)
    if (VEcuAppFrm_State_Off == g_VEcuAppFrm_State)
    {
        return VEOS_E_NOT_OK;
    }

    /* This can only be performed, if the handler has events to fulfill. */
    return VEcuAppFrm_CallIoEventHandler(ioEventId);
#else
    VEOS_DBG_UNUSED_PARAM(ioEventId);

    return VEOS_E_NO_IMPLEMENTATION;
#endif /* VECUAPPFRM_IO_AVAILABLE == VEOS_ON */
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_HandleAsyncSimEvent
 ***
 *** DESCRIPTION:
 ***     Event handler function called when the simulator has triggered an event from outside its
 ***     regular timing intervals.
 ***
 *** PARAMETERS:
 ***     Type                    Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any error
 ***                            occurs, an error code will be the result. See VEOS_Types.h for
 ***                            details.
\**************************************************************************************************/
VEOS_ApiSuccessType VEcuAppFrm_HandleAsyncSimEvent(void)
{
    if (VEcuAppFrm_State_Off == g_VEcuAppFrm_State)
    {
        return VEOS_E_NOT_OK;
    }

#if (VECUAPPFRM_SAB_AVAILABLE == VEOS_ON)
    Sab_EventTriggered();
#endif /* VECUAPPFRM_SAB_AVAILABLE == VEOS_ON */

    return VEOS_E_OK;
}

/**************************************************************************************************\
 * Special callback methods.                                                                      *
\**************************************************************************************************/

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_ReportError
 ***
 *** DESCRIPTION:
 ***     Service to report development, runtime or transient errors.
 ***
 *** PARAMETERS:
 ***     Type                 Name        Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     VEcuAppFrm_ErrorType Error       Specifies the type of error occured.
 ***     uint16               ModuleId    Module ID of calling module.
 ***     uint8                InstanceId  The identifier of the index based instance of a module, starting
 ***                                      from 0. If the module is a single instance module it shall pass 0 as
 ***                                      the InstanceId.
 ***     uint8   ApiId                    ID of API service in which error is detected (defined in SWS of
 ***                                      calling module).
 ***     uint8   ErrorId                  ID of detected development error (defined in SWS of calling module).
 ***
 *** RETURNS:
 ***     Std_ReturnType      E_OK, if the error has been handled.
 ***                         E_NOT_OK, if the error has not been handled.
 ***                         If Det is used, any other implementation specific errors might be returned.
\**************************************************************************************************/
Std_ReturnType VEcuAppFrm_ReportError(VEcuAppFrm_ErrorType Error, uint16 ModuleId, uint8 InstanceId, uint8 ApiId, uint8 ErrorId)
{
    Std_ReturnType result;

    if (VEcuAppFrm_State_Off == g_VEcuAppFrm_State)
    {
        return E_NOT_OK;
    }

    switch (Error)
    {
        case VEcuAppFrm_DevelopmentError: {
#if (VECUAPPFRM_DET_UL_AVAILABLE == VEOS_ON)
            result = Det_ReportError(ModuleId, InstanceId, ApiId, ErrorId);
#else
            (void)VEOS_MsgApi_ShowMessageFormat(VEOS_MsgApi_SeverityType_Warning, VEOS_TRUE, "Development error during simulation of AUTOSAR BSW module with ID %u in instance %u. API %u reported error %u.", ModuleId, InstanceId, ApiId, ErrorId);
            result = E_OK;
#endif /* VECUAPPFRM_DET_UL_AVAILABLE == VEOS_ON */
            break;
        }
        case VEcuAppFrm_TransientError: {
#if (VECUAPPFRM_DET_UL_AVAILABLE == VEOS_ON)
#ifdef VECUAPPFRM_TRANSIENT_AND_RUNTIMEERROR_AVAILABLE
            result = Det_ReportTransientFault(ModuleId, InstanceId, ApiId, ErrorId);
#else
            result = Det_ReportError(ModuleId, InstanceId, ApiId, ErrorId);
#endif /* VECUAPPFRM_TRANSIENT_AND_RUNTIMEERROR_AVAILABLE */
#else
            (void)VEOS_MsgApi_ShowMessageFormat(VEOS_MsgApi_SeverityType_Warning, VEOS_TRUE, "Transient fault during simulation of AUTOSAR BSW module with ID %u in instance %u. API %u reported error %u.", ModuleId, InstanceId, ApiId, ErrorId);
            result = E_OK;
#endif  /* VECUAPPFRM_DET_UL_AVAILABLE == VEOS_ON */
            break;
        }
        case VEcuAppFrm_RuntimeError: {
#if (VECUAPPFRM_DET_UL_AVAILABLE == VEOS_ON)
#ifdef VECUAPPFRM_TRANSIENT_AND_RUNTIMEERROR_AVAILABLE
            result = Det_ReportRuntimeError(ModuleId, InstanceId, ApiId, ErrorId);
#else
            result = Det_ReportError(ModuleId, InstanceId, ApiId, ErrorId);
#endif /* VECUAPPFRM_TRANSIENT_AND_RUNTIMEERROR_AVAILABLE */
#else
            (void)VEOS_MsgApi_ShowMessageFormat(VEOS_MsgApi_SeverityType_Warning, VEOS_TRUE, "Runtime error during simulation of AUTOSAR BSW module with ID %u in instance %u. API %u reported error %u.", ModuleId, InstanceId, ApiId, ErrorId);
            result = E_OK;
#endif /* VECUAPPFRM_DET_UL_AVAILABLE == VEOS_ON */
            break;
        }
        case VEcuAppFrm_Fault:
        {
            (void)VEOS_MsgApi_ShowMessageFormat(VEOS_MsgApi_SeverityType_Error, VEOS_TRUE, "Error during simulation of AUTOSAR BSW module with ID %u in instance %u. API %u reported error %u.", ModuleId, InstanceId, ApiId, ErrorId);
            VEOS_SimApi_TerminateSimulationFatal();
            result = E_NOT_OK;
            break;
        }

        default:
        {
            result = E_NOT_OK;
            break;
        }
    }

    return result;
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_TerminateSimulation
 ***
 *** DESCRIPTION:
 ***     Function is called when terminating the simulation from within the model.
 ***
 *** PARAMETERS:
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
void VEcuAppFrm_TerminateSimulation(void)
{
    /* Sends the terminate simulation request. */
    (void)VEOS_SimApi_TerminateSimulation(VEOS_SimApi_TerminationKindType_StopProcess);
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_OnOsStarted
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
void VEcuAppFrm_OnOsStarted(void)
{
#if (VECUAPPFRM_SAB_AVAILABLE == STD_ON)
   Sab_OnOsStarted();
#endif /* VECUAPPFRM_SAB_AVAILABLE == STD_ON */
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_OnOsShutdown
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
void VEcuAppFrm_OnOsShutdown(void)
{
#if (VECUAPPFRM_SAB_AVAILABLE == STD_ON)
   Sab_OnOsShutdown();
#endif /* VECUAPPFRM_SAB_AVAILABLE == STD_ON */
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_OnOsWakeup
 ***
 *** DESCRIPTION:
 ***     Callout for the implementation of the operating system to signal the wakeup of the OS .
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
void VEcuAppFrm_OnOsWakeup(void)
{
#if (VECUAPPFRM_SAB_AVAILABLE == STD_ON)
    Sab_WakeupActivity();
#endif /* VECUAPPFRM_SAB_AVAILABLE == STD_ON */
}


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
 ***     FunctionType                   pFunction       Callback method to call after the RelAlarmTime
 ***                                                    has expired.
 ***     const char*                    pName           Used only for an error or warning message.
 ***                                                    The name does not have to be unique, can also
 ***                                                    be a nullpointer or empty string.
 ***     uint32*                        pId             Pointer that contains a unique notification ID.
 ***                                                    Maximum number of notification PID is limited to 65535 (2^16)-1
 *** RETURNS:
 ***     Std_ReturnType                 SAB_E_OK or error code.
\**************************************************************************************************/
Std_ReturnType VEcuAppFrm_CreateNotification(FunctionType pFunction, const char* pName, uint32 * pId)
{
    return (Std_ReturnType)VEOS_NotiApi_CreateNotification( pFunction, pName, (VEOS_uint32*)(pId) );
}

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
Std_ReturnType VEcuAppFrm_StartRelTimeNotification( uint32 id, uint64 offsetInTicks, uint64 periodInTicks)
{
    return (Std_ReturnType)VEOS_NotiApi_StartNotification( id, offsetInTicks, periodInTicks );
}

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
Std_ReturnType VEcuAppFrm_CancelNotification(uint32 id)
{
    return (Std_ReturnType)VEOS_NotiApi_CancelNotification( id );
}

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
VEOS_ApiSuccessType VEcuAppFrm_NotificationEvent(const VEOS_SimApi_InterruptCallbackFunctionType pFunction)
{

    if (VEOS_NULL == pFunction)
    {
        return VEOS_E_NULL_PARAM;
    }

    Sab_EventTriggered();

#if (VECUAPPFRM_OS_AVAILABLE == VEOS_ON)
    /* Handle a callback function similar an interrupt function */
    Os_SetInterruptLevel();
#endif

    /* callback function */
    (*pFunction)();

#if (VECUAPPFRM_OS_AVAILABLE == VEOS_ON)

    /* Reset interrupt level and restore old level */
    Os_ResetInterruptLevel();

    /* if the callback function activate tasks, start scheduling here */
    Os_ScheduleInterruptActivatedTask();

#endif

    Sab_EventFinished();

return E_OK;
}




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
VEOS_ApiSuccessType VEcuAppFrm_InterruptEvent(const VEOS_SimApi_InterruptCallbackFunctionType pFunction)
{

    if (VEOS_NULL == pFunction)
    {
        return VEOS_E_NULL_PARAM;
    }

    Sab_EventTriggered();

#if (VECUAPPFRM_OS_AVAILABLE == VEOS_ON)
    /* Handle a callback function similar an interrupt function */
    Os_SetInterruptLevel();
#endif

    /* callback function */
    (*pFunction)();

#if (VECUAPPFRM_OS_AVAILABLE == VEOS_ON)

    /* Reset interrupt level and restore old level */
    Os_ResetInterruptLevel();

    /* if the callback function activate tasks, start scheduling here */
    Os_ScheduleInterruptActivatedTask();

#endif

    Sab_EventFinished();

    return E_OK;
}


#endif /* VECUAPPFRM_C */

/*------------------------------------------------------------------------------------------------*\
 END OF FILE
\*------------------------------------------------------------------------------------------------*/
