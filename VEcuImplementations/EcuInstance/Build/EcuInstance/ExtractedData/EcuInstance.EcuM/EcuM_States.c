/**************************************************************************************************\
 *** EcuM_States.c
 ***
 *** This file contains the ECU State Manager internal implementation of state changes.
 ***
 *** Copyright (c) 2017 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef ECUM_STATES_C
#define ECUM_STATES_C

/*------------------------------------------------------------------------------------------------*\
  DEFINES (OPT)
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/
#include "EcuM.h"
#include "EcuM_Callout.h"
#include "EcuM_Internal.h"
#include "Rte_Main.h"
#include "Rte_EcuM.h"
#include "SchM.h"
#include "Os.h"
#include "Sab.h"
#if (ECUM_INCLUDE_BSWM == STD_ON)
#include "BswM.h"
#endif
#if (ECUM_INCLUDE_MCU == STD_ON)
#include "Mcu.h"
#endif
#if (ECUM_INCLUDE_COMM == STD_ON)
#include "ComM.h"
#endif
#if (ECUM_INCLUDE_DEM == STD_ON)
#include "Dem.h"
#endif
#if (ECUM_INCLUDE_NVM == STD_ON)
#include "NvM.h"
#endif
#if (ECUM_INCLUDE_NMS == STD_ON)
#include "DsNMS.h"
#endif


/*------------------------------------------------------------------------------------------------*\
  DEFINES
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
  PARAMETERIZED MACROS
\*------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
 extern "C" {
#endif

/*------------------------------------------------------------------------------------------------*\
  TYPEDEFS
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
  VARIABLE DECLARATIONS
\*------------------------------------------------------------------------------------------------*/

/* Post-build configuration structure. */
const EcuM_ConfigType* EcuM_PostBuildConfiguration;

#if (ECUM_INCLUDE_NVM == STD_ON)
/* Indicates whether the EcuM is waiting for the NvM to finish the ReadAll/WriteAll-Request.  */
boolean EcuM_WaitingForNvM = FALSE;
volatile boolean EcuM_NvMCbReceived = FALSE;
EcuM_TimeoutType EcuM_NvMTimeout;
#endif

#if (ECUM_CURRENT_MODE_PORT_AVAILABLE == STD_ON)
Rte_ModeType_EcuM_Mode EcuM_CurrentApplicationState = RTE_MODE_EcuM_Mode_SHUTDOWN;
#endif

boolean EcuM_Initialized = FALSE;
boolean EcuM_UseMinimumMode = ECUM_MINIMUM_MODE;
EcuM_TimeoutType EcuM_RunRequestTimeout;

volatile boolean EcuM_InWakeup = FALSE;
volatile EcuM_WakeupSourceType EcuM_CurrentWakeupEvent;
EcuM_WakeupSourceStatusType EcuM_WakeupSourceStatus;
boolean EcuM_WaitingForWakeupValidation;

/*------------------------------------------------------------------------------------------------*\
  FUNCTION PROTOTYPES
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
  FUNCTION DEFINITIONS
\*------------------------------------------------------------------------------------------------*/

/**************************************************************************************************\
 *** FUNCTION:
 ***     EcuM_DoStartupI
 *** 
 *** DESCRIPTION:
 ***     Implementation of EcuM STARTUP_I state.
 ***     
 *** PARAMETERS:
 ***     -/-
 *** 
 *** RETURNS:
 ***    void
\**************************************************************************************************/
void EcuM_DoStartupI(void)
{
    uint8 mode = 0;

#if (ECUM_INCLUDE_MCU == STD_ON)
    Mcu_ResetType resetReason;
#endif

    /* Callout for pre-OS, low level initialization code. */
    EcuM_AL_DriverInitZero();

    /* Determine the post-build configuration settings. */
    EcuM_PostBuildConfiguration = EcuM_DeterminePbConfiguration();

    /* skip consistency check of configuration data */
    
    /* Callout for further driver initializes before OS start. */
    EcuM_AL_DriverInitOne(EcuM_PostBuildConfiguration);

#if (ECUM_INCLUDE_MCU == STD_ON)
    {
        int i;

        /* Determination of reset reason. */
        resetReason = Mcu_GetResetReason();

        /* map reset reason to wakeup source */
        for (i = 0; i < EcuM_PostBuildConfiguration->NumWakeupSources; i++)
        {
            int j;

            EcuM_WakeupSourceConfig wakeupSource = EcuM_PostBuildConfiguration->WakeupSources[i];
            for (j = 0; j < wakeupSource.NumMcuResets; j++)
            {
                if (wakeupSource.McuResets[j] == resetReason)
                {
                    EcuM_InWakeup = TRUE;
                    EcuM_CurrentWakeupEvent = j;
                }
            }
        }
    }
#endif

    if (EcuM_PostBuildConfiguration->DefaultShutdownTarget == ECUM_STATE_SLEEP)
    {
        mode = EcuM_PostBuildConfiguration->DefaultSleepMode;
    }
    else
    {
        if (EcuM_PostBuildConfiguration->DefaultShutdownTarget == ECUM_STATE_RESET)
        {
            mode = EcuM_PostBuildConfiguration->DefaultResetMode;
        }
    }

    EcuM_ShutdownTarget.state = EcuM_PostBuildConfiguration->DefaultShutdownTarget;
    EcuM_ShutdownTarget.mode = mode;

    EcuM_Initialized = TRUE;
    EcuM_SwitchState(ECUM_STATE_STARTUP_TWO, FALSE);

    StartOS(EcuM_PostBuildConfiguration->DefaultAppMode);
}

/**************************************************************************************************\
*** FUNCTION:
***     EcuM_DoStartupII
***
*** DESCRIPTION:
***     Implementation of EcuM STARTUP_II state.
***
*** PARAMETERS:
***     -/-
***
*** RETURNS:
***    void
\**************************************************************************************************/
void EcuM_DoStartupII(void)
{
#if (ECUM_INCLUDE_NVM == STD_ON)
    /* in the first call handle the first part of the StartupII state */
    if (EcuM_WaitingForNvM == FALSE)
    {
        EcuM_WaitingForNvM = TRUE;
        EcuM_NvMTimeout = EcuM_PostBuildConfiguration->NvM_ReadAllTimeout;
        EcuM_NvMCbReceived = FALSE;
#endif

        /* Call Basic Software Scheduler init. */
        SchM_Start();
        SchM_Init();
        SchM_StartTiming();

        /* Callout for further driver initializes after OS start. */
        /* NvM_ReadAll is started here */
        EcuM_AL_DriverInitTwo(EcuM_PostBuildConfiguration);

#if (ECUM_INCLUDE_NMS == STD_ON)
        DsNMS_Init();
#endif

        /* Callout stub for RTE Startup. */
        EcuM_OnRTEStartup();

        /* Start RTE. */
        Rte_Start();

#if (ECUM_INCLUDE_NVM == STD_ON)
        /* skip remaining part of STARTUP_II, if NvM_ReadAll has not finished yet */
        if (EcuM_NvMCbReceived == FALSE)
        {
            return;
        }
    }
    else
    {
        if (EcuM_NvMCbReceived == FALSE)
        {
            if (EcuM_NvMTimeout == 0)
            {
                EcuM_Det_ReportError(ECUM_MAIN_FUNCTION_SERVICE_ID, ECUM_E_TIMEOUT);
            }
            else
            {
                EcuM_NvMTimeout--;
                return;
            }
        }
    }

    EcuM_WaitingForNvM = FALSE;
#endif

    /* callout for further driver initialization after NvM_ReadAll has finished */
    EcuM_AL_DriverInitThree(EcuM_PostBuildConfiguration);

    /* set new state */
    EcuM_SwitchState(ECUM_STATE_RUN, TRUE);

    /* check wakeup reason */
    if (EcuM_InWakeup == TRUE &&
        EcuM_PostBuildConfiguration->WakeupSources[EcuM_CurrentWakeupEvent].RequiresPolling == FALSE)
    {
        /* continue with wakeup sequence if wakeup source uses integrated power control */
        EcuM_DoWakeupValidation();
    }
    else
    {
        EcuM_DoEnterRun();
    }
}

/**************************************************************************************************\
*** FUNCTION:
***     EcuM_DoEnterRun
***
*** DESCRIPTION:
***     Enter the EcuM RUN_II state.
***
*** PARAMETERS:
***     -/-
***
*** RETURNS:
***    void
\**************************************************************************************************/
void EcuM_DoEnterRun(void)
{
    /* Callout for "just after startup". */
    EcuM_OnEnterRun();

#if (ECUM_INCLUDE_COMM == STD_ON)
    {
        int i;

        /* switch on communication for all configured ComM channels */
        for (i = 0; i < EcuM_PostBuildConfiguration->ComMData.NumEntries; i++)
        {
            ComM_CommunicationAllowed(EcuM_PostBuildConfiguration->ComMData.CommunicationAllowedEntries[i], TRUE);
        }
    }
#endif

    /* The EcuStateManager RUN self request must be released after a configurable period
     * of time (via define ECUM_RUN_SELF_REQUEST_PERIOD), i.e. an OSEK alarm must be set
     * here which calls EcuM_ReleaseRUN(userType) after that time, i.e.
     * SetRelAlarm(0, ECUM_RUN_SELF_REQUEST_PERIOD, 0);
     * In this implementation the minimum duration is a multiple of the main
     * function cycle time. */
    EcuM_RunRequestTimeout = EcuM_PostBuildConfiguration->RunMinimumDuration;

    /* Set new state appropriately. */
    EcuM_SwitchState(ECUM_STATE_APP_RUN, FALSE);
}

/**************************************************************************************************\
*** FUNCTION:
***     EcuM_DoRunII
***
*** DESCRIPTION:
***     Implementation of EcuM RUN_II state.
***
*** PARAMETERS:
***     -/-
***
*** RETURNS:
***    void
\**************************************************************************************************/
void EcuM_DoRunII(void)
{
    boolean keepAlive = FALSE;
    EcuM_WakeupSourceType wakeupSource = EcuM_GetPendingWakeupEvents();

    EcuM_StartWakeupSources(wakeupSource);
    EcuM_CheckValidation(wakeupSource);
    EcuM_StopWakeupSources(wakeupSource);

    /* check for timeout and run requests */
    if (EcuM_CurrentNumberOfRunRequests == 0)
    {
        if (EcuM_RunRequestTimeout > 0)
        {
            EcuM_RunRequestTimeout--;

            /* timeout is not yet expired */
            keepAlive = TRUE;
        }
    }
    else
    {
        keepAlive = TRUE;
        EcuM_RunRequestTimeout = 0;
    }

#if (ECUM_INCLUDE_COMM == STD_ON)
    {
        int i;

        /* check current state of all ComM channels */
        for (i = 0; i < EcuM_PostBuildConfiguration->ComMData.NumEntries; i++)
        {
            ComM_StateType state = COMM_NO_COM_NO_PENDING_REQUEST;
            ComM_GetState(EcuM_PostBuildConfiguration->ComMData.CommunicationAllowedEntries[i], &state);

            if (state != COMM_NO_COM_NO_PENDING_REQUEST)
            {
                keepAlive = TRUE;
                break;
            }
        }
    }
#endif

    if ((EcuM_UseMinimumMode == STD_OFF && !keepAlive) || EcuM_AllRunRequestsKilled)
    {
#if (ECUM_INCLUDE_COMM == STD_ON)
        int i;

        /* switch off communication for all configured ComM channels */
        for (i = 0; i < EcuM_PostBuildConfiguration->ComMData.NumEntries; i++)
        {
            ComM_CommunicationAllowed(EcuM_PostBuildConfiguration->ComMData.CommunicationAllowedEntries[i], FALSE);
        }
#endif

        /* Callout for exit RUN state. */
        EcuM_OnExitRun();

        /* Set new state appropriately. */
        EcuM_SwitchState(ECUM_STATE_APP_POST_RUN, TRUE);
    }
}

/**************************************************************************************************\
*** FUNCTION:
***     EcuM_DoRunIII
***
*** DESCRIPTION:
***     Implementation of EcuM RUN_III state.
***
*** PARAMETERS:
***     -/-
***
*** RETURNS:
***    void
\**************************************************************************************************/
void EcuM_DoRunIII(void)
{
#if (ECUM_INCLUDE_COMM == STD_ON)
    int i;
    boolean returnToRun = FALSE;

    /* poll ComM channels */
    for (i = 0; i < EcuM_PostBuildConfiguration->ComMData.NumEntries; i++)
    {
        ComM_StateType commState;
        ComM_GetState(EcuM_PostBuildConfiguration->ComMData.CommunicationAllowedEntries[i], &commState);
        if (commState == COMM_NO_COM_REQUEST_PENDING)
        {
            returnToRun = TRUE;
            break;
        }
    }

    if (EcuM_CurrentNumberOfRunRequests > 0 || returnToRun == TRUE)
#else
    if (EcuM_CurrentNumberOfRunRequests > 0)
#endif

    {
        EcuM_SwitchState(ECUM_STATE_APP_RUN, FALSE);

    }
    else if (EcuM_CurrentNumberOfPostRunRequests == 0)
    {
#if (ECUM_INCLUDE_COMM == STD_ON)
        /* switch off communication for all configured ComM channels */
        for (i = 0; i < EcuM_PostBuildConfiguration->ComMData.NumEntries; i++)
        {
            ComM_CommunicationAllowed(EcuM_PostBuildConfiguration->ComMData.CommunicationAllowedEntries[i], FALSE);
        }
#endif

        EcuM_OnExitPostRun();

        EcuM_SwitchState(ECUM_STATE_PREP_SHUTDOWN, FALSE);
    }
}

/**************************************************************************************************\
*** FUNCTION:
***     EcuM_DoPrepShutdown
***
*** DESCRIPTION:
***     Implementation of EcuM PREP SHUTDOWN state.
***
*** PARAMETERS:
***     -/-
***
*** RETURNS:
***    void
\**************************************************************************************************/
void EcuM_DoPrepShutdown(void)
{
    /* Callout for preparation of shutdown state. */
    EcuM_OnPrepShutdown();

#if (ECUM_INCLUDE_DEM == STD_ON)
    /* Shutdown of module Diagnostic Event Manager (DEM). */
    Dem_Shutdown();
#endif

    /* Switch depending on the shutdown target. */
    if (EcuM_ShutdownTarget.state == ECUM_STATE_SLEEP)
    {
        /* Set new state appropriately. */
        EcuM_SwitchState(ECUM_STATE_GO_SLEEP, TRUE);
    }
    else /* Shutdown target is STD_OFF or RESET, see req. EcuM2288. */
    {
        /* Set new state appropriately. */
        EcuM_SwitchState(ECUM_STATE_GO_OFF_ONE, TRUE);
    }
}

/**************************************************************************************************\
*** FUNCTION:
***     EcuM_DoGoOffI
***
*** DESCRIPTION:
***     Implementation of EcuM GO OFF I state.
***
*** PARAMETERS:
***     -/-
***
*** RETURNS:
***    void
\**************************************************************************************************/
void EcuM_DoGoOffI(void)
{
    EcuM_WakeupSourceType pendingWakeupEvents;

#if (ECUM_INCLUDE_NVM == STD_ON)
    if (EcuM_WaitingForNvM == FALSE)
    {
#endif
        EcuM_OnGoOffOne();
        Rte_Stop();

#if (ECUM_INCLUDE_COMM == STD_ON)
        ComM_DeInit();
#endif

#if (ECUM_INCLUDE_NVM == STD_ON)
        EcuM_WaitingForNvM = TRUE;
        EcuM_NvMTimeout = EcuM_PostBuildConfiguration->NvM_WriteAllTimeout;
        EcuM_NvMCbReceived = FALSE;

        NvM_WriteAll();

        /* skip remaining part of GO_OFF_I, NvM_WriteAll has not finished yet */
        if (EcuM_NvMCbReceived == FALSE)
        {
            return;
        }
    }
    else
    {
        if (EcuM_NvMCbReceived == FALSE)
        {
            if (EcuM_NvMTimeout == 0)
            {
                EcuM_Det_ReportError(ECUM_MAIN_FUNCTION_SERVICE_ID, ECUM_E_TIMEOUT);
            }
            else
            {
                EcuM_NvMTimeout--;
                return;
            }
        }
    }

    EcuM_WaitingForNvM = FALSE;
#endif

#if (ECUM_INCLUDE_BSWM == STD_ON)
    BswM_Deinit();
#endif
    SchM_Deinit();

    pendingWakeupEvents = EcuM_GetPendingWakeupEvents();
    if (pendingWakeupEvents != 0)
    {
        /* wakeup events occurred during shutdown, re-start immediately */
        EcuM_SelectShutdownTarget(ECUM_STATE_RESET, EcuM_PostBuildConfiguration->DefaultResetMode);
    }

    EcuM_SwitchState(ECUM_STATE_GO_OFF_TWO, FALSE);

    ShutdownOS(E_OK);
}

/**************************************************************************************************\
*** FUNCTION:
***     EcuM_DoGoOffII
***
*** DESCRIPTION:
***     Implementation of EcuM GO OFF II state.
***
*** PARAMETERS:
***     -/-
***
*** RETURNS:
***    void
\**************************************************************************************************/
void EcuM_DoGoOffII(void)
{
    EcuM_OnGoOffTwo();

#if (ECUM_INCLUDE_NMS == STD_ON)
    DsNMS_DeInit();
#endif

    if (EcuM_ShutdownTarget.state == ECUM_STATE_RESET)
    {
        EcuM_SwitchState(ECUM_STATE_RESET, FALSE);

#if (ECUM_INCLUDE_MCU == STD_ON)
        Mcu_PerformReset();
#endif
    }

    if (EcuM_ShutdownTarget.state == ECUM_STATE_OFF)
    {
        EcuM_SwitchState(ECUM_STATE_OFF, FALSE);

        EcuM_AL_SwitchOff();
    }
}

/**************************************************************************************************\
*** FUNCTION:
***     EcuM_DoGoSleep
***
*** DESCRIPTION:
***     Implementation of EcuM GO SLEEP state.
***
*** PARAMETERS:
***     -/-
***
*** RETURNS:
***    void
\**************************************************************************************************/
void EcuM_DoGoSleep(void)
{
    EcuM_WakeupSourceType pendingWakeups = 0;

#if (ECUM_INCLUDE_NVM == STD_ON)
    if (EcuM_WaitingForNvM == FALSE)
    {
#endif
        EcuM_OnGoSleep();

#if (ECUM_INCLUDE_NMS == STD_ON)
        DsNMS_Suspend();
#endif

#if (ECUM_INCLUDE_NVM == STD_ON)
        EcuM_WaitingForNvM = TRUE;
        EcuM_NvMCbReceived = FALSE;

        NvM_WriteAll();

        EcuM_NvMTimeout = EcuM_PostBuildConfiguration->NvM_WriteAllTimeout;

#if (ECUM_INCLUDE_NVM == STD_ON)
        /* skip remaining part of STARTUP_II, if NvM_ReadAll has not finished yet */
        if (EcuM_NvMCbReceived == FALSE)
        {
            return;
        }
#endif
    }
    else
    {
        pendingWakeups = EcuM_GetPendingWakeupEvents();
        if (EcuM_NvMCbReceived == FALSE && pendingWakeups == 0)
        {
            if (EcuM_NvMTimeout == 0)
            {
                EcuM_Det_ReportError(ECUM_MAIN_FUNCTION_SERVICE_ID, ECUM_E_TIMEOUT);
            }
            else
            {
                EcuM_NvMTimeout--;
                return;
            }
        }
    }

    EcuM_WaitingForNvM = FALSE;
#else
    pendingWakeups = EcuM_GetPendingWakeupEvents();
#endif

    if (pendingWakeups != 0)
    {
#if (ECUM_INCLUDE_NVM == STD_ON)
        NvM_CancelWriteAll();
#endif

        EcuM_SwitchState(ECUM_STATE_WAKEUP_VALIDATION, FALSE);
    }
    else
    {
        EcuM_SleepModeConfig sleepModeConfig = EcuM_PostBuildConfiguration->SleepModes[EcuM_ShutdownTarget.mode];

        EcuM_EnableWakeupSources(sleepModeConfig.WakeupSourceMask);
        GetResource(RES_SCHEDULER);

        EcuM_SwitchState(ECUM_STATE_SLEEP, FALSE);

        if (sleepModeConfig.McuSuspended == TRUE)
        {
            EcuM_DoSleepI();
        }
        else
        {
            EcuM_DoSleepII();
            EcuM_DoWakeupI();

            EcuM_SwitchState(ECUM_STATE_WAKEUP_VALIDATION, FALSE);
        }
    }
}


/**************************************************************************************************\
*** FUNCTION:
***     EcuM_DoSleepI
***
*** DESCRIPTION:
***     Implementation of EcuM SLEEP I state.
***
*** PARAMETERS:
***     -/-
***
*** RETURNS:
***    void
\**************************************************************************************************/
void EcuM_DoSleepI(void)
{
    DisableAllInterrupts();
    EcuM_GenerateRamHash();

#if (ECUM_INCLUDE_MCU == STD_ON)
    /* the processor is halted in this function and will continue after an interrupt of a wakeup source occured */
    Mcu_SetMode(EcuM_PostBuildConfiguration->SleepModes[EcuM_ShutdownTarget.mode].McuMode);
#endif

    EnableAllInterrupts();

    if (EcuM_CheckRamHash() == 0)
    {
        /* RAM check failed */
        EcuM_ErrorHook(ECUM_E_RAM_CHECK_FAILED);
    }

    EcuM_DoWakeupI();

    EcuM_SwitchState(ECUM_STATE_WAKEUP_VALIDATION, FALSE);
}

/**************************************************************************************************\
*** FUNCTION:
***     EcuM_DoSleepII
***
*** DESCRIPTION:
***     Implementation of EcuM SLEEP II state.
***
*** PARAMETERS:
***     -/-
***
*** RETURNS:
***    void
\**************************************************************************************************/
void EcuM_DoSleepII(void)
{
    EcuM_SleepModeConfig sleepModeConfig = EcuM_PostBuildConfiguration->SleepModes[EcuM_ShutdownTarget.mode];

#if (ECUM_INCLUDE_MCU == STD_ON)
    DisableAllInterrupts();

    Mcu_SetMode(sleepModeConfig.McuMode);

    EnableAllInterrupts();
#endif

    while (1)
    {
        int i;

        EcuM_SleepActivity();

        for (i = 0; i < EcuM_PostBuildConfiguration->NumWakeupSources; i++)
        {
            EcuM_WakeupSourceType source = sleepModeConfig.WakeupSourceMask & (1 << i);
            if (source > 0)
            {
                if (EcuM_PostBuildConfiguration->WakeupSources[i].RequiresPolling == TRUE)
                {
                    EcuM_CheckWakeup(source);
                }
            }
        }

        if (EcuM_GetPendingWakeupEvents() != 0)
        {
#if defined(VEOS_PLATFORM) || defined(FMU_VECU)
            Sab_WakeupActivity();
#endif
            break;
        }

#if defined(VEOS_PLATFORM) || defined(FMU_VECU)
        Sab_SleepActivity();
#endif
    }

    EcuM_SwitchState(ECUM_STATE_WAKEUP_ONE, FALSE);
}

/**************************************************************************************************\
*** FUNCTION:
***     EcuM_DoWakeupI
***
*** DESCRIPTION:
***     Implementation of EcuM WAKEUP I state.
***
*** PARAMETERS:
***     -/-
***
*** RETURNS:
***    void
\**************************************************************************************************/
void EcuM_DoWakeupI(void)
{
    EcuM_WakeupSourceType pendingWakeups;

    DisableAllInterrupts();

#if (ECUM_INCLUDE_MCU == STD_ON)
    Mcu_SetMode(EcuM_PostBuildConfiguration->NormalMcuMode);
#endif

    EnableAllInterrupts();

    pendingWakeups = EcuM_GetPendingWakeupEvents();
    EcuM_DisableWakeupSources(pendingWakeups);

    EcuM_AL_DriverRestart(EcuM_PostBuildConfiguration);

#if (ECUM_INCLUDE_NMS == STD_ON)
    DsNMS_Resume();
#endif

    ReleaseResource(RES_SCHEDULER);
}

/**************************************************************************************************\
*** FUNCTION:
***     EcuM_DoWakeupValidation
***
*** DESCRIPTION:
***     Implementation of EcuM wakeup validation.
***
*** PARAMETERS:
***     -/-
***
*** RETURNS:
***    void
\**************************************************************************************************/
void EcuM_DoWakeupValidation(void)
{
    EcuM_WakeupSourceType pendingWakeups = 0;

    /* in the first call start wakeup sources and timeouts */
    if (EcuM_WaitingForWakeupValidation == FALSE)
    {
        int i = 0;
        EcuM_WakeupSourceType wakeups;

        EcuM_WaitingForWakeupValidation = TRUE;

        pendingWakeups = EcuM_GetPendingWakeupEvents();
        EcuM_StartWakeupSources(pendingWakeups);

        /* start validation timeouts */
        for (wakeups = pendingWakeups; i < EcuMConf_EcuMWakeupSource_LENGTH && wakeups > 0; i++, wakeups >>= 1)
        {
            if ((wakeups & 0x1) != 0)
            {
                EcuM_WakeupSourceStatus.CurrentTimeouts[i] = EcuM_PostBuildConfiguration->WakeupSources[i].ValidationTimeout;
            }
        }
    }
    else
    {
        pendingWakeups = EcuM_GetPendingWakeupEvents();
    }

    EcuM_CheckValidation(pendingWakeups);

    if (EcuM_WakeupSourceStatus.Valid == 0)
    {
        int i = 0;
        EcuM_WakeupSourceType wakeups;
        boolean allSourcesTimedOut = TRUE;

        /* check timeouts */
        for (wakeups = pendingWakeups; i < EcuMConf_EcuMWakeupSource_LENGTH && wakeups > 0; i++, wakeups >>= 1)
        {
            if ((wakeups & 0x1) != 0)
            {
                if (EcuM_WakeupSourceStatus.CurrentTimeouts[i] > 0)
                {
                    allSourcesTimedOut = FALSE;
                    EcuM_WakeupSourceStatus.CurrentTimeouts[i]--;
                }
            }
        }

        if (allSourcesTimedOut == TRUE)
        {
            EcuM_Det_ReportError(ECUM_MAIN_FUNCTION_SERVICE_ID, ECUM_E_TIMEOUT);
        }
        else
        {
            return;
        }
    }

    if (EcuM_WakeupSourceStatus.Valid == 0)
    {
        EcuM_StopWakeupSources(pendingWakeups);
    }

    EcuM_SwitchState(ECUM_STATE_WAKEUP_REACTION, FALSE);
}

/**************************************************************************************************\
*** FUNCTION:
***     EcuM_DoWakeupReaction
***
*** DESCRIPTION:
***     Implementation of EcuM wakeup reaction.
***
*** PARAMETERS:
***     -/-
***
*** RETURNS:
***    void
\**************************************************************************************************/
void EcuM_DoWakeupReaction(void)
{
    EcuM_WakeupSourceType wakeupEvents = EcuM_GetValidatedWakeupEvents();
    EcuM_ClearWakeupEvent(wakeupEvents);
    EcuM_WakeupReactionType reaction;

    /* check whether only the TTII wakeup is requested */
    if ((EcuM_PostBuildConfiguration->TTII_Enabled == TRUE) &&
        (wakeupEvents & EcuM_PostBuildConfiguration->TTII_WakeupSourceMask) &&
        (wakeupEvents | ~EcuM_PostBuildConfiguration->TTII_WakeupSourceMask))
    {
        reaction = ECUM_WKACT_TTII;
    }
    else if (wakeupEvents > 0)
    {
        reaction = ECUM_WKACT_RUN;
    }
    else
    {
        reaction = ECUM_WKACT_SHUTDOWN;
    }

    /* the wakeup reaction may be overidden in a callout */
    reaction = EcuM_OnWakeupReaction(reaction);

    if (reaction == ECUM_WKACT_RUN)
    {
        EcuM_SwitchState(ECUM_STATE_WAKEUP_TWO, FALSE);
        EcuM_DoWakeupII();
    }
    else if (reaction == ECUM_WKACT_TTII)
    {
        /* TTII not implement */
        EcuM_SwitchState(ECUM_STATE_GO_SLEEP, TRUE);
    }
    else
    {
        /* Switch depending on the shutdown target. */
        if (EcuM_ShutdownTarget.state == ECUM_STATE_SLEEP)
        {
            /* Set new state appropriately. */
            EcuM_SwitchState(ECUM_STATE_GO_SLEEP, TRUE);
        }
        else
        {
            /* Set new state appropriately. */
            EcuM_SwitchState(ECUM_STATE_GO_OFF_ONE, TRUE);
        }
    }
}

/**************************************************************************************************\
*** FUNCTION:
***     EcuM_DoWakeupII
***
*** DESCRIPTION:
***     Implementation of EcuM WAKEUP II state.
***
*** PARAMETERS:
***     -/-
***
*** RETURNS:
***    void
\**************************************************************************************************/
void EcuM_DoWakeupII()
{
    /* Dem_Init is contained in init list three if Dem module is present */
    EcuM_AL_DriverInitThree(EcuM_PostBuildConfiguration);

    EcuM_RunRequestTimeout = EcuM_PostBuildConfiguration->RunMinimumDuration;
    EcuM_SwitchState(ECUM_STATE_RUN, TRUE);
}

/**************************************************************************************************\
 * helper methods
\**************************************************************************************************/
void EcuM_SwitchState(EcuM_StateType nextState, boolean switchApplicationState)
{
    if (nextState == EcuM_CurrentState)
    {
        /* State does not change. No transition has to be reported. */
        return;
    }

#if (ECUM_CURRENT_MODE_PORT_AVAILABLE == STD_ON)
    if (switchApplicationState == TRUE)
    {
        Rte_ModeType_EcuM_Mode nextApplicationState = EcuM_CurrentApplicationState;

        switch (nextState)
        {
        case ECUM_STATE_STARTUP_ONE:
        case ECUM_STATE_STARTUP_TWO:
            nextApplicationState = RTE_MODE_EcuM_Mode_STARTUP;
            break;
        case ECUM_STATE_RUN:
        case ECUM_STATE_APP_RUN:
            nextApplicationState = RTE_MODE_EcuM_Mode_RUN;
            break;
        case ECUM_STATE_APP_POST_RUN:
            nextApplicationState = RTE_MODE_EcuM_Mode_POST_RUN;
            break;
        case ECUM_STATE_GO_OFF_ONE:
        case ECUM_STATE_GO_OFF_TWO:
            nextApplicationState = RTE_MODE_EcuM_Mode_SHUTDOWN;
            break;
        case ECUM_STATE_GO_SLEEP:
            nextApplicationState = RTE_MODE_EcuM_Mode_SLEEP;
            break;
        }

        if (nextApplicationState != EcuM_CurrentApplicationState)
        {
            /* Send notification to runtime environment and application */
            if (Rte_Switch_currentMode_currentMode(nextApplicationState) == E_OK)
            {
                EcuM_CurrentApplicationState = nextApplicationState;
            }
        }
    }
#else
    ECUM_UNUSED_PARAMETER(switchApplicationState);
#endif

    /* Set new state. */
    EcuM_CurrentState = nextState;
}

void EcuM_InitializeList(const EcuM_InitListType* initList)
{
    uint8 i;

    for (i = 0; i<initList->NumEntries; i++)
    {
        if (initList->Entries[i].ParamType == ECUM_INIT_PARAM_VOID &&
            initList->Entries[i].InitFuncVoid != NULL)
        {
            initList->Entries[i].InitFuncVoid();
        }
        else if (initList->Entries[i].InitFuncParam != NULL)
        {
            initList->Entries[i].InitFuncParam(initList->Entries[i].InitParam);
        }
    }
}

Std_ReturnType EcuM_CheckInitialized(uint8 ServiceId)
{
    Std_ReturnType result = E_OK;

    /* Check if EcuM was initialized. */
    if (EcuM_Initialized != TRUE)
    {
        EcuM_Det_ReportError(ServiceId, ECUM_E_NOT_INITIALIZED);
        result = E_NOT_OK;
    }

    return result;
}

Std_ReturnType EcuM_CheckValidShutdownTarget(uint8 ServiceId, EcuM_StateType target)
{
    Std_ReturnType result = E_OK;

    /* Check if EcuM was initialized. */
    if (target != ECUM_STATE_SLEEP && target != ECUM_STATE_RESET && target != ECUM_STATE_OFF)
    {
        EcuM_Det_ReportError(ServiceId, ECUM_E_STATE_PAR_OUT_OF_RANGE);
        result = E_NOT_OK;
    }

    return result;
}

Std_ReturnType EcuM_CheckNullPtr(uint8 ServiceId, void* Ptr)
{
    Std_ReturnType result = E_OK;

    /* Check for NULL pointer. */
    if (Ptr == NULL)
    {
        EcuM_Det_ReportError(ServiceId, ECUM_E_NULL_POINTER);
        result = E_NOT_OK;
    }

    return result;
}

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ECUM_STATES_C */
/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/
