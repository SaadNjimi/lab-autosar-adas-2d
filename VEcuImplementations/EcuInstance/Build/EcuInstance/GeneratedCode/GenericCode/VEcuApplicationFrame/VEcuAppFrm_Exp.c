/**************************************************************************************************\
 *** VEcuAppFrm_Exp.c
 ***
 *** Experimentation functions for the application frame implementation for V-ECUs.
 ***
 *** Copyright 2019 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef VECUAPPFRM_EXP_C
#define VECUAPPFRM_EXP_C

/*------------------------------------------------------------------------------------------------*\
 DEFINES (OPT)
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
 INCLUDES
\*------------------------------------------------------------------------------------------------*/

#include "VEcuAppFrm.h"
#include "VEcuAppFrm_Cfg.h"
#include "VEcuAppFrm_Internal.h"

#if (VECUAPPFRM_OS_AVAILABLE == VEOS_ON)
#include "Os.h"
#endif /* VECUAPPFRM_OS_AVAILABLE == VEOS_ON */


#include "Platform_Types.h"
#include "Std_Types.h"
#include "VEOS.h"

/*------------------------------------------------------------------------------------------------*\
 DEFINES
\*------------------------------------------------------------------------------------------------*/

#if (VECUAPPFRM_OS_AVAILABLE == VEOS_ON)
#if defined(OS_TASK_STIM_OFFSET) && (OS_TASK_STIM_OFFSET > 1)
#define VECUAPPFRM_STIM_OFFSET_VALUE                                OS_TASK_STIM_OFFSET
#endif /* defined(OS_TASK_STIM_OFFSET) && (OS_TASK_STIM_OFFSET > 1) */
#endif /* VECUAPPFRM_OS_AVAILABLE == VEOS_ON */

#ifndef VECUAPPFRM_STIM_OFFSET_VALUE
#define VECUAPPFRM_STIM_OFFSET_VALUE                                1
#endif

/*------------------------------------------------------------------------------------------------*\
 TYPEDEFS
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
 PARAMETERIZED MACROS
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
 VARIABLE DECLARATIONS
\*------------------------------------------------------------------------------------------------*/

static VEOS_uint16 g_VEcuAppFrm_ExpectedPostCallRasterId = 0;
static VEOS_boolean g_VEcuAppFrm_StimRasterTriggered = VEOS_FALSE;
static VEOS_boolean g_VEcuAppFrm_RasterServiceActive = VEOS_FALSE;
/* The number of basic task steps to perform before a stimulation can take place. */
uint32 g_VEcuAppFrm_StimCycles = 0;
/* The RealTimeTesting lock. */
boolean g_VEcuAppFrm_RttLock = FALSE;

VEOS_boolean g_VEcuAppFrm_TriggerStep;

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
 ***     VEcuAppFrm_ResetStimService
 ***
 *** DESCRIPTION:
 ***     Function called to reset the stimulation service.
 ***
 *** PARAMETERS:
 ***     Type                    Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
static void VEcuAppFrm_ResetStimService(void)
{
    g_VEcuAppFrm_ExpectedPostCallRasterId = 0;
    g_VEcuAppFrm_StimRasterTriggered = VEOS_FALSE;
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_RttSetAsync
 ***
 *** DESCRIPTION:
 ***     Function called when the operating system must use Real-Time-Testing and runs asynchronously.
 ***
 *** PARAMETERS:
 ***     Type                    Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
void VEcuAppFrm_RttSetAsync(void)
{
    if (VEcuAppFrm_State_Off == g_VEcuAppFrm_State)
    {
        return;
    }

    if (FALSE == g_VEcuAppFrm_RttLock)
    {
        (void)VEOS_ExpApi_StimSvcAsyncMode();
    }
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_RttSetAsync
 ***
 *** DESCRIPTION:
 ***     Function called when the operating system must use Real-Time-Testing and runs synchronously.
 ***
 *** PARAMETERS:
 ***     Type                    Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
void VEcuAppFrm_RttSetSync(void)
{
    if (VEcuAppFrm_State_Off == g_VEcuAppFrm_State)
    {
        return;
    }

    if (FALSE == g_VEcuAppFrm_RttLock)
    {
        (void)VEOS_ExpApi_StimSvcSyncMode();
    }
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_TriggerStimulation
 ***
 *** DESCRIPTION:
 ***     Function called to trigger stimulation in the current trigger step.
 ***     If stimulation is not required, the next stimulation step might be prepared.
 ***
 *** PARAMETERS:
 ***     Type                    Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
void VEcuAppFrm_TriggerStimulation(void)
{
    /*
     * The raster service is not active. That indicates, that the operating system might be active,
     * but no task has run yet.
     */
    if (VEOS_FALSE == g_VEcuAppFrm_RasterServiceActive)
    {
        return;
    }

    /*
     * If the method is called from the basic rate task currently
     * active for this timing period, reduce the number of stimulus cycles.
     * Reduce this value after the tasks have been triggered.
     */
    if(g_VEcuAppFrm_StimCycles != 0)
    {
        g_VEcuAppFrm_StimCycles--;
    }
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_ResetStimulation
 ***
 *** DESCRIPTION:
 ***     Function called to reset stimulation services to initial values.
 ***
 *** PARAMETERS:
 ***     Type                    Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
void VEcuAppFrm_ResetStimulation(void)
{
    g_VEcuAppFrm_RasterServiceActive = VEOS_FALSE;
    g_VEcuAppFrm_StimCycles = VECUAPPFRM_STIM_OFFSET_VALUE;
    VEcuAppFrm_ResetStimService();
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_ExpSvcPreTaskCall
 ***
 *** DESCRIPTION:
 ***     Function called when the operating system starts a task and must trigger rasters.
 ***
 *** PARAMETERS:
 ***     Type            Name                 Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     const uint16    RasterId             The experimentation service raster to invoke.
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
void VEcuAppFrm_ExpSvcPreTaskCall(const uint16 RasterId)
{
    /* Local result of API function */
    VEOS_ApiSuccessType success = VEOS_E_PENDING;
    VEOS_ExpApi_ServiceType tmpService = VEOS_ExpApi_ServiceType_None;
    boolean stimulationRasterActive = (0 == g_VEcuAppFrm_StimCycles);

    /* Do not run during acyclic callbacks */
    if (VEOS_FALSE == g_VEcuAppFrm_TriggerStep)
    {
       return;
    }

    if (VEcuAppFrm_State_Off == g_VEcuAppFrm_State)
    {
        return;
    }

    if (0 == RasterId)
    {
        return;
    }

    g_VEcuAppFrm_RasterServiceActive = VEOS_TRUE;

    success = VEOS_ExpApi_GetRasterService(RasterId, &tmpService);

    if (success == VEOS_E_OK)
    {
        success = VEOS_ExpApi_DsModelAccessWrite(RasterId);
    }

    if ((FALSE != stimulationRasterActive) && (success == VEOS_E_OK) && ((VEOS_ExpApi_ServiceType_Stimulation & tmpService) == VEOS_ExpApi_ServiceType_Stimulation))
    {
        if (VEOS_FALSE == g_VEcuAppFrm_StimRasterTriggered)
        {
            success = VEOS_ExpApi_CallStimServicePreStep();
            g_VEcuAppFrm_StimRasterTriggered = VEOS_TRUE;
        }
    }

    if ((success == VEOS_E_OK) && ((VEOS_ExpApi_ServiceType_Daq & tmpService) == VEOS_ExpApi_ServiceType_Daq))
    {
        success = VEOS_ExpApi_CallDaqService(RasterId);
    }

    if ((success == VEOS_E_OK) && ((VEOS_ExpApi_ServiceType_Bypass & tmpService) == VEOS_ExpApi_ServiceType_Bypass))
    {
        success = VEOS_ExpApi_CallBypassingService(RasterId);
    }

    if (VEOS_E_OK != success)
    {
        (void)VEOS_SimApi_ReportError(VECUAPPFRM_MODULE_NAME, __FUNCTION__, success);
    }
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_ExpSvcPostTaskCall
 ***
 *** DESCRIPTION:
 ***     Function called when the operating system terminates a task and must trigger rasters.
 ***
 *** PARAMETERS:
 ***     Type            Name                 Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     const uint16    RasterId             The experimentation service raster to invoke.
 ***     const boolean   ActiveBasicRateTask  A flag to indicate whether the current task is the active
 ***                                          basic rate task.
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
void VEcuAppFrm_ExpSvcPostTaskCall(const uint16 RasterId, const boolean ActiveBasicRateTask)
{
    /* Local result of API function */
    VEOS_ApiSuccessType success = VEOS_E_PENDING;
    VEOS_ExpApi_ServiceType tmpService = VEOS_ExpApi_ServiceType_None;
    VEOS_boolean stimulationRasterActive = (0 == g_VEcuAppFrm_StimCycles);

     /* Do not run during acyclic callbacks */
    if (VEOS_FALSE == g_VEcuAppFrm_TriggerStep)
    {
       return;
    }

    if (VEcuAppFrm_State_Off == g_VEcuAppFrm_State)
    {
        return;
    }

    if (0 == RasterId)
    {
        return;
    }

    if (TRUE == ActiveBasicRateTask)
    {
        if ((0 != RasterId) && (0 == g_VEcuAppFrm_ExpectedPostCallRasterId)) /* First call in this time step */
        {
            g_VEcuAppFrm_ExpectedPostCallRasterId = RasterId;
        }
        else if ((0 == RasterId) && (0 != g_VEcuAppFrm_ExpectedPostCallRasterId)) /* Reset raster expectations */
        {
            VEcuAppFrm_ResetStimService();
        }
    }

    success = VEOS_ExpApi_GetRasterService(RasterId, &tmpService);

    if (success == VEOS_E_OK)
    {
        success = VEOS_ExpApi_DsModelAccessRead(RasterId);
    }

    if ((success == VEOS_E_OK) && ((VEOS_ExpApi_ServiceType_Bypass & tmpService) == VEOS_ExpApi_ServiceType_Bypass))
    {
        success = VEOS_ExpApi_CallBypassingService(RasterId);
    }

    if ((success == VEOS_E_OK) && ((VEOS_ExpApi_ServiceType_Daq & tmpService) == VEOS_ExpApi_ServiceType_Daq))
    {
        success = VEOS_ExpApi_CallDaqService(RasterId);
    }

    if ((VEOS_FALSE != stimulationRasterActive) && (success == VEOS_E_OK) && ((VEOS_ExpApi_ServiceType_Stimulation & tmpService) == VEOS_ExpApi_ServiceType_Stimulation))
    {
        if ((VEOS_TRUE == g_VEcuAppFrm_StimRasterTriggered) && (RasterId == g_VEcuAppFrm_ExpectedPostCallRasterId))
        {
            success = VEOS_ExpApi_CallStimServicePostStep();
        }
    }

    if (stimulationRasterActive)
    {
        g_VEcuAppFrm_StimCycles = VECUAPPFRM_STIM_OFFSET_VALUE;
        VEcuAppFrm_ResetStimService();
    }

    if (VEOS_E_OK != success)
    {
        (void)VEOS_SimApi_ReportError(VECUAPPFRM_MODULE_NAME, __FUNCTION__, success);
    }
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_CallBypassingService
 ***
 *** DESCRIPTION:
 ***     Calls the bypassing service with error checks.
 ***
 *** PARAMETERS:
 ***     Type                 Name        Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     uint16               ServiceId   ID of the service raster to forward to the service.
 ***
 *** RETURNS:
 ***     Std_ReturnType      E_OK, if the service call has been forwarded.
 ***                         E_NOT_OK, if the service call resulted in an error.
\**************************************************************************************************/
Std_ReturnType VEcuAppFrm_CallBypassingService(uint16 ServiceId)
{
    /* Local result of API function */
    VEOS_ApiSuccessType success = VEOS_E_PENDING;
    VEOS_ExpApi_ServiceType tmpService = VEOS_ExpApi_ServiceType_None;

    success = VEOS_ExpApi_GetRasterService(ServiceId, &tmpService);

    if ((success == VEOS_E_OK) && ((VEOS_ExpApi_ServiceType_Bypass & tmpService) == VEOS_ExpApi_ServiceType_Bypass))
    {
        success = VEOS_ExpApi_CallBypassingService(ServiceId);
    }

    if (VEOS_E_OK != success)
    {
        return E_NOT_OK;
    }

    return E_OK;
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEcuAppFrm_EnsureInfLoopExperimentation
 ***
 *** DESCRIPTION:
 ***     During a sleep ccenarios real-time testing is called by the V-ECU ApplicationFrame 
 ***     and is running in the regular time steps.
 ***
 *** PARAMETERS:
 ***     Type           Name               Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     uint8               justWakeup   V_ECU is just wakeup.
 *** RETURNS:
 ***     void
\**************************************************************************************************/
void VEcuAppFrm_EnsureInfLoopExperimentation(uint8 justWakeup)
{
    /* Do not run during acyclic callbacks */
    if (VEOS_FALSE == g_VEcuAppFrm_TriggerStep)
    {
        if (justWakeup) 
        {
            /* Trigger stimulation */
            g_VEcuAppFrm_RasterServiceActive = VEOS_TRUE;
            VEcuAppFrm_TriggerStimulation();
            g_VEcuAppFrm_RasterServiceActive = VEOS_FALSE;
        }
       return;
    }
    else
    {
       /* Trigger stimulation */
       g_VEcuAppFrm_RasterServiceActive = VEOS_TRUE;
       VEcuAppFrm_TriggerStimulation();
       g_VEcuAppFrm_RasterServiceActive = VEOS_FALSE;
    }

    /* Timeslot for RTT? */
    if (0 == g_VEcuAppFrm_StimCycles)
    {
      /* has the pre-task-hook send? */
      if (VEOS_FALSE == g_VEcuAppFrm_StimRasterTriggered)
      {
         (void)VEOS_ExpApi_CallStimServicePreStep();
      }
      /* Run the stimulation service */
      (void) VEOS_ExpApi_CallStimServicePostStep();
      /* Reset counters */
       VEcuAppFrm_ResetStimulation();
    }
}



#endif  /* VECUAPPFRM_EXP_C */

/*------------------------------------------------------------------------------------------------*\
 END OF FILE
\*------------------------------------------------------------------------------------------------*/
