/**************************************************************************************************\
 *** VEcuAppFrm_Exp.c
 ***
 *** State handling functions for the application frame implementation for V-ECUs.
 ***
 *** Copyright 2019 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef VECUAPPFRM_STATES_C
#define VECUAPPFRM_STATES_C

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
#include "VEOS.h"

#include "Platform_Types.h"

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
#endif /* VECUAPPFRM_DET_UL_AVAILABLE == VEOS_ON */

/*------------------------------------------------------------------------------------------------*\
 DEFINES
\*------------------------------------------------------------------------------------------------*/

#if !defined(CPU_TYPE) || !((CPU_TYPE == CPU_TYPE_32) || (CPU_TYPE == CPU_TYPE_64))
#error "CPU type of platform is not defined properly. This code is designed to run on 32 or 64 bit platforms only."
#endif /* !CPU_TYPE */

#if !defined(CPU_BIT_ORDER) || (CPU_BIT_ORDER != LSB_FIRST)
#error "CPU bit order of platform is not defined properly. This code is designed to run on LSB first platforms only."
#endif /* !CPU_BIT_ORDER */

#if !defined(CPU_BYTE_ORDER) || (CPU_BYTE_ORDER != LOW_BYTE_FIRST)
#error "CPU bit order of platform is not defined properly. This code is designed to run on low byte first platforms only."
#endif /* !CPU_BYTE_ORDER */

#define BITS_PER_BYTE                    8
#define NUMBER_OF_DATATYPES_TO_CHECK    (1 + 1 + 4 + 4 + 2)     /* base type, boolean, signed data types, unsigned data types, floating point data types */

/*------------------------------------------------------------------------------------------------*\
 TYPEDEFS
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
 PARAMETERIZED MACROS
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
 VARIABLE DECLARATIONS
\*------------------------------------------------------------------------------------------------*/

/* The current state */
VEcuAppFrm_AppStateType g_VEcuAppFrm_State;
/* The number of delayed cycles */
uint32 g_VEcuAppFrm_NumberOfDelayCycles;
/* The flag to indicate that the simulator is shutting down the kernel */
boolean g_VEcuAppFrm_SimulatorShuttingDown;

#if !defined(VEOS_NO_DTSIZE_CHECK)
static VEOS_DataTypeCheckInfoType ga_VEcuAppFrm_DownloadDataCheck[NUMBER_OF_DATATYPES_TO_CHECK] = {
    /* basic type reference */
    { "platform CPU type", (CPU_TYPE / BITS_PER_BYTE), sizeof(void *), VEOS_DTCHECK_NOSIGNCHECKREQUIRED },
    { "boolean", ((CPU_TYPE_32/BITS_PER_BYTE)/4), sizeof(boolean), VEOS_DTCHECK_NOSIGNCHECKREQUIRED },
    { "sint8", ((CPU_TYPE_32/BITS_PER_BYTE)/4), sizeof(sint8), VEOS_DTCHECK_ISSIGNEDDATATYPE(sint8) },
    { "sint16", ((CPU_TYPE_32/BITS_PER_BYTE)/2), sizeof(sint16), VEOS_DTCHECK_ISSIGNEDDATATYPE(sint16) },
    { "sint32", ((CPU_TYPE_32/BITS_PER_BYTE)/1), sizeof(sint32), VEOS_DTCHECK_ISSIGNEDDATATYPE(sint32) },
    { "sint64", ((CPU_TYPE_32/BITS_PER_BYTE)*2), sizeof(sint64), VEOS_DTCHECK_ISSIGNEDDATATYPE(sint64) },
    { "uint8", ((CPU_TYPE_32/BITS_PER_BYTE)/4), sizeof(uint8), VEOS_DTCHECK_ISUNSIGNEDDATATYPE(uint8) },
    { "uint16", ((CPU_TYPE_32/BITS_PER_BYTE)/2), sizeof(uint16), VEOS_DTCHECK_ISUNSIGNEDDATATYPE(uint16) },
    { "uint32", ((CPU_TYPE_32/BITS_PER_BYTE)/1), sizeof(uint32), VEOS_DTCHECK_ISUNSIGNEDDATATYPE(uint32) },
    { "uint64", ((CPU_TYPE_32/BITS_PER_BYTE)*2), sizeof(uint64), VEOS_DTCHECK_ISUNSIGNEDDATATYPE(uint64) },
    { "float32", ((CPU_TYPE_32/BITS_PER_BYTE)/1), sizeof(float32), VEOS_DTCHECK_NOSIGNCHECKREQUIRED },
    { "float64", ((CPU_TYPE_32/BITS_PER_BYTE)*2), sizeof(float64), VEOS_DTCHECK_NOSIGNCHECKREQUIRED },
};
#endif /* !VEOS_NO_DTSIZE_CHECK */

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
 *** FUNCTION:
 ***     VEOS_Impl_PerformDownloadDataTypeCheck
 ***
 *** DESCRIPTION:
 ***     Checks the correct initialization of the downloaded core data types.
 ***
 *** PARAMETERS:
 ***     -/-
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any error
 ***                            occurs, an error code will be the result. See VEOS_Types.h for
 ***                            details.
\**************************************************************************************************/
static VEOS_ApiSuccessType VEcuAppFrm_DownloadDataCheck(void)
{

    VEOS_ApiSuccessType result = VEOS_E_OK;

#if !defined(VEOS_NO_DTSIZE_CHECK)
    int i;
    VEOS_ApiSuccessType dtCheckResult;

#if !defined(VEOS_NO_DTSIZE_CHECK_PTR)
    /* Check Platform CPU Type */
    if (ga_VEcuAppFrm_DownloadDataCheck[0].ExpectedSize != ga_VEcuAppFrm_DownloadDataCheck[0].RealSize)
    {
        if (ga_VEcuAppFrm_DownloadDataCheck[0].ExpectedSize == 4)
        {
            (void)VEOS_MsgApi_ShowMessageFormat(
                VEOS_MsgApi_SeverityType_Warning,
                VEOS_TRUE,
                "The implementation of Platform_Types.h expects the size of the %s to be %u, but its real size is %u. This might cause compatibility problems. Repeat the import and select the HostPC32 simulation target platform or specify the VEOS_NO_DTSIZE_CHECK_PTR preprocessor define to disable the compatibility check.",
                ga_VEcuAppFrm_DownloadDataCheck[0].DataTypeName,
                ga_VEcuAppFrm_DownloadDataCheck[0].ExpectedSize,
                ga_VEcuAppFrm_DownloadDataCheck[0].RealSize);
        }
        else
        {
            (void)VEOS_MsgApi_ShowMessageFormat(
                VEOS_MsgApi_SeverityType_Warning,
                VEOS_TRUE,
                "The implementation of Platform_Types.h expects the size of the %s to be %u, but its real size is %u. This might cause compatibility problems. Repeat the import and select the HostPC64 simulation target platform or specify the VEOS_NO_DTSIZE_CHECK_PTR preprocessor define to disable the compatibility check.",
                ga_VEcuAppFrm_DownloadDataCheck[0].DataTypeName,
                ga_VEcuAppFrm_DownloadDataCheck[0].ExpectedSize,
                ga_VEcuAppFrm_DownloadDataCheck[0].RealSize);
        }
    }
#endif /* !VEOS_NO_DTSIZE_CHECK_PTR */

    for (i = 1; i < NUMBER_OF_DATATYPES_TO_CHECK; i++)
    {
        dtCheckResult = VEOS_CheckDataType(&ga_VEcuAppFrm_DownloadDataCheck[i]);
        if (VEOS_E_OK != dtCheckResult)
        {
            result = dtCheckResult;
        }
    }

    if (VEOS_E_OK != result)
    {
        (void)VEOS_MsgApi_ShowMessageFormat(VEOS_MsgApi_SeverityType_Error, VEOS_TRUE, "The check of data type sizes has failed. Simulation will terminate.");
        VEOS_SimApi_TerminateSimulation(VEOS_SimApi_TerminationKindType_StopSimulationAndFail);
    }
#endif /* !VEOS_NO_DTSIZE_CHECK */

    return result;
}

/**************************************************************************************************\
* Local functions for realization.                                                               *
\**************************************************************************************************/

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_StartOs
 ***
 *** DESCRIPTION:
 ***     Function to start OSEK OS implementation, if AUTOSAR ECU State Manager (EcuM) is not available.
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
static void VEcuAppFrm_StartOs(void)
{
#if (VECUAPPFRM_OS_AVAILABLE == VEOS_ON)
    StartOS(OSDEFAULTAPPMODE);
#endif /* VECUAPPFRM_OS_AVAILABLE == VEOS_ON */
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_HandlePowerOnEvent
 ***
 *** DESCRIPTION:
 ***     Function to initialize the V-ECU before the power has been plugged in.
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
static void VEcuAppFrm_HandlePowerOnEvent(void)
{
#if (VECUAPPFRM_DAP_AVAILABLE == VEOS_ON)
    /* Initialize data access points, if available */
    Dap_Init();
#endif /* VECUAPPFRM_DAP_AVAILABLE == VEOS_ON */

#if (VECUAPPFRM_IDIF_AVAILABLE == VEOS_ON)
    /* Initialize idealized bus simulation interface, if available */
    IdealizedBusIf_Init();
#endif /* VECUAPPFRM_IDIF_AVAILABLE == VEOS_ON */

    /* Call configured start-up function. Regularly, this will initialize AUTOSAR ECU State Manager (EcuM_Init) */
#if (VECUAPPFRM_SAB_AVAILABLE == VEOS_ON)
    Sab_StartModel();
#else
    VEcuAppFrm_StartOs();
#endif /* VECUAPPFRM_SAB_AVAILABLE == VEOS_ON */
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_HandlePowerOffEvent
 ***
 *** DESCRIPTION:
 ***     Function to initialize the V-ECU before the power has been plugged off.
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
static void VEcuAppFrm_HandlePowerOffEvent(void)
{
#if (VECUAPPFRM_DAP_AVAILABLE == VEOS_ON)
    Dap_DeInit();
#endif /* VECUAPPFRM_DAP_AVAILABLE == VEOS_ON */

    /* Idealized bus simulation interface does not provide a deinitialization function. No need to perform a call here. */

#if (VECUAPPFRM_SAB_AVAILABLE == VEOS_ON)
    Sab_Call_PostShutDown_Functions();
    Sab_DeInit();
#endif /* VECUAPPFRM_SAB_AVAILABLE == VEOS_ON */

#if (VECUAPPFRM_OS_AVAILABLE == VEOS_ON)
#if (NUMBER_OF_CORES > 0)
    ShutdownAllCores(E_OK);
#else
    ShutdownOS(E_OK);
#endif /* (NUMBER_OF_CORES > 0) */
#endif /* VECUAPPFRM_OS_AVAILABLE == VEOS_ON */
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_ChangeState
 ***
 *** DESCRIPTION:
 ***     Implements a simple state machine with four states, where each state can reach any other
 ***     state except itself.
 ***
 *** PARAMETERS:
 ***     Type                    Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     VEcuAppFrm_AppStateType newState     The new state to enter.
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any error
 ***                            occurs, an error code will be the result. See VEOS_Types.h for
 ***                            details.
\**************************************************************************************************/
VEOS_ApiSuccessType VEcuAppFrm_ChangeState(const VEcuAppFrm_AppStateType newState)
{
    static VEOS_ApiSuccessType result = VEOS_E_OK; /* Note: Do not remove 'static'! Workaround for a frame pointer optimization problem */

    result = VEOS_E_OK;

    /* No action must be performed, if the next state to enter has already been reached. */
    if (newState == g_VEcuAppFrm_State)
    {
        return result;
    }

    /* Set new state. */
    g_VEcuAppFrm_State = newState;

    /* Lock real time testing mode switch */
    g_VEcuAppFrm_RttLock = FALSE;

    switch (newState)
    {
    case VEcuAppFrm_State_Off:
        /* No action to take place. */
        break;
    case VEcuAppFrm_State_Booting:
#if (VECUAPPFRM_OS_AVAILABLE == VEOS_ON)
        /* Check data types of application */
        result = VEcuAppFrm_DownloadDataCheck();

        if (VEOS_E_OK == result)
        {
            /* If OSEK operating system is present, setup all initial values. */
            Os_PreInit();

            if (0 == Os_FramePrepareStartOs())
    #endif /* VECUAPPFRM_OS_AVAILABLE == VEOS_ON */
            {
                /* Setup V-ECU specific events. */
                VEcuAppFrm_HandlePowerOnEvent();
            }
        }
        break;
    case VEcuAppFrm_State_Running:
        /* Set RTT mode switch to locked. */
        g_VEcuAppFrm_RttLock = TRUE;
        break;
    case VEcuAppFrm_State_ShuttingDown:
        /* Shut down all ECU related function. */
        VEcuAppFrm_HandlePowerOffEvent();
        break;
    default:
        /* No action to take place. */
        break;
    }

    return result;
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_HandleStartEvent
 ***
 *** DESCRIPTION:
 ***     Function called to handle delayed V-ECU start-up.
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
VEOS_ApiSuccessType VEcuAppFrm_HandleStartEvent(void)
{
#ifdef VECUAPPFRM_PREHOOK
    /* Call hook */
    VECUAPPFRM_PREHOOK();
#endif /* VECUAPPFRM_PREHOOK */

#if (VECUAPPFRM_SAB_AVAILABLE == VEOS_ON)
    /* If delayed start is configured, do not turn on V-ECU */
    if (!Sab_CanStart()) /* Sab_CanStart function is defined to work without call to init. */
    {
        g_VEcuAppFrm_NumberOfDelayCycles++;

        return VECUAPPFRM_E_DELAYED;
    }
#endif

    return VEOS_E_OK;
}

/**************************************************************************************************\
* Routed events from Simulator abstraction (Sab).                                                *
\**************************************************************************************************/

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
void VEcuAppFrm_ModelStateEventBoot(void)
{
}

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
void VEcuAppFrm_ModelStateEventRun(void)
{
    (void)VEcuAppFrm_ChangeState(VEcuAppFrm_State_Running);
}

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
void VEcuAppFrm_ModelStateEventTerminate(void)
{
}

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
void VEcuAppFrm_ModelStateEventOff(void)
{
    if (FALSE == g_VEcuAppFrm_SimulatorShuttingDown)
    {
        (void)VEOS_SimApi_TerminateSimulation(VEOS_SimApi_TerminationKindType_StopProcess);
    }
}

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
void VEcuAppFrm_EcuM_Init(void)
{
    VEcuAppFrm_StartOs();
}

#if defined(_MSC_VER) && defined(_M_IX86)
#pragma optimize("", on)
#endif

#endif /* VECUAPPFRM_STATES_C */

/*------------------------------------------------------------------------------------------------*\
 END OF FILE
\*------------------------------------------------------------------------------------------------*/
