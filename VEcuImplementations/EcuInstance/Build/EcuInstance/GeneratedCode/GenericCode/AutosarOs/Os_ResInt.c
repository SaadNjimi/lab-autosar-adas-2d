/**************************************************************************************************\
 *** Os_ResInt.c
 ***
 *** OSEK API function definition for OS Resources and interrupt handling.
 ***
 *** Copyright (c) 2014 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef OS_RESINT_C
#define OS_RESINT_C

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/

#include "Os_ResInt.h"

#include "Os_Kernel.h"
#include "Os_Types.h"
#include "Os_Cfg.h"


/*------------------------------------------------------------------------------------------------*\
  FUNCTION DEFINITIONS
\*------------------------------------------------------------------------------------------------*/

static Os_BoolType Os_DecIsrDisableCounter(void)
{
    if(g_Kernel.IsrDisableCnt != 0)
    {
        g_Kernel.IsrDisableCnt--;
        return OS_TRUE;
    }
    return OS_FALSE;
}

static Os_BoolType Os_IncIsrDisableCounter(void)
{
    if(g_Kernel.IsrDisableCnt < 0xFFFFu)
    {
        g_Kernel.IsrDisableCnt++;
        return OS_TRUE;
    }
    return OS_FALSE;
}

void EnableAllInterrupts(void)
{
    g_ErrorInfo.ServiceID = OSServiceId_EnableAllInterrupts;

    if ( Os_DecIsrDisableCounter() )
    {
        if( (g_Kernel.IsrDisableCnt == 0) && (g_Kernel.IsSleeping == OS_VECU_IS_NOT_SLEEPING) )
        {
            Os_ScheduleInternal(OS_TRUE, OS_FALSE);
        }
    }
}

void DisableAllInterrupts(void)
{
    g_ErrorInfo.ServiceID = OSServiceId_DisableAllInterrupts;
    (void)Os_IncIsrDisableCounter();
}

void ResumeAllInterrupts(void)
{
    g_ErrorInfo.ServiceID = OSServiceId_ResumeAllInterrupts;

    if ( Os_DecIsrDisableCounter() )
    {
        if(g_Kernel.IsrDisableCnt == 0)
        {
            Os_ScheduleInternal(OS_TRUE, OS_FALSE);
        }
    }
}

void SuspendAllInterrupts(void)
{
    g_ErrorInfo.ServiceID = OSServiceId_SuspendAllInterrupts;
    (void)Os_IncIsrDisableCounter();
}

void ResumeOSInterrupts(void)
{
    g_ErrorInfo.ServiceID = OSServiceId_ResumeOSInterrupts;

    if ( Os_DecIsrDisableCounter() )
    {
        if( (g_Kernel.IsrDisableCnt == 0) && (g_Kernel.IsSleeping == OS_VECU_IS_NOT_SLEEPING) )
        {
            Os_ScheduleInternal(OS_TRUE, OS_FALSE);
        }
    }
}

void SuspendOSInterrupts(void)
{
    g_ErrorInfo.ServiceID = OSServiceId_SuspendOSInterrupts;
    (void)Os_IncIsrDisableCounter();
}

void Os_EnterISR(void)
{
    g_ErrorInfo.ServiceID = OSServiceId_EnterISR;
    Os_EnterIsrHook();
}

void Os_LeaveISR(void)
{
    g_ErrorInfo.ServiceID = OSServiceId_LeaveISR;
    Os_LeaveIsrHook();
}

StatusType Os_EnableInterrupt(Os_IntDescriptorType Descriptor)
{
    g_ErrorInfo.ServiceID = OSServiceId_EnableInterrupt;
    g_ErrorInfo.Service.SvEnableInterrupt.Descriptor = Descriptor;

    if ( Os_DecIsrDisableCounter() )
    {
        if( (g_Kernel.IsrDisableCnt == 0) && (g_Kernel.IsSleeping == OS_VECU_IS_NOT_SLEEPING) )
        {
            Os_ScheduleInternal(OS_TRUE, OS_FALSE);
        }
    }

    g_Kernel.IntDescriptor |= Descriptor;

    return E_OK;
}

StatusType Os_DisableInterrupt(Os_IntDescriptorType Descriptor)
{
    g_ErrorInfo.ServiceID = OSServiceId_DisableInterrupt;
    g_ErrorInfo.Service.SvDisableInterrupt.Descriptor = Descriptor;

    (void)Os_IncIsrDisableCounter();
    g_Kernel.IntDescriptor &= (~Descriptor);

    return E_OK;
}

StatusType Os_GetInterruptDescriptor(Os_IntDescriptorRefType Descriptor)
{
    g_ErrorInfo.ServiceID = OSServiceId_GetInterruptDescriptor;

    *Descriptor = g_Kernel.IntDescriptor;

    return E_OK;
}

StatusType GetResource(ResourceType ResID)
{
    g_ErrorInfo.ServiceID = OSServiceId_GetResource;
    g_ErrorInfo.Service.SvGetResource.ResID = ResID;

    if(ResID >= NUMBER_OF_RESOURCES)
    {
        ERRORHOOK(E_OS_ID);
        return E_OS_ID;
    }
#if (NUMBER_OF_RESOURCES > 0)
    if(g_ResCb[ResID].Occupied == OS_TRUE)
    {

        ERRORHOOK(E_OS_ACCESS);
        return E_OS_ACCESS;
    }

  if(Os_FirstIsHigherPrio(g_TaskCb[g_Kernel.ActTaskId].InitialPrio, g_ResCb[ResID].Prio))
  {
     ERRORHOOK(E_OS_ACCESS);
     return E_OS_ACCESS;
  }

  g_TaskCb[g_Kernel.ActTaskId].ResourceLevelCnt++;

  g_ResCb[ResID].OwnerTask = g_Kernel.ActTaskId;
  g_ResCb[ResID].Occupied = OS_TRUE;
  g_ResCb[ResID].ReplacedPrio = g_TaskCb[g_Kernel.ActTaskId].ActPrio;
  g_ResCb[ResID].ResourceLevelCnt = g_TaskCb[g_Kernel.ActTaskId].ResourceLevelCnt;

  if(Os_FirstIsHigherPrio(g_ResCb[ResID].Prio, g_TaskCb[g_Kernel.ActTaskId].ActPrio))
  {
     Os_SetTaskActPrio(g_Kernel.ActTaskId, g_ResCb[ResID].Prio);
  }

  return E_OK;
#else
  ERRORHOOK(E_OS_ID);
  return E_OS_ID;
#endif /* NUMBER_OF_RESOURCES > 0 */
}

StatusType ReleaseResource(ResourceType ResID)
{
  g_ErrorInfo.ServiceID = OSServiceId_ReleaseResource;
  g_ErrorInfo.Service.SvReleaseResource.ResID = ResID;

  if(ResID >= NUMBER_OF_RESOURCES)
  {
     ERRORHOOK(E_OS_ID);
     return E_OS_ID;
  }
#if (NUMBER_OF_RESOURCES > 0)
  if ( (g_ResCb[ResID].OwnerTask != g_Kernel.ActTaskId)                             ||
       (g_TaskCb[g_Kernel.ActTaskId].ResourceLevelCnt != g_ResCb[ResID].ResourceLevelCnt)  )
  {
     ERRORHOOK(E_OS_NOFUNC);
     return E_OS_NOFUNC;
  }

  if(Os_FirstIsHigherPrio(g_TaskCb[g_Kernel.ActTaskId].InitialPrio, g_ResCb[ResID].Prio))
  {
     ERRORHOOK(E_OS_ACCESS);
     return E_OS_ACCESS;
  }

  Os_SetTaskActPrio(g_Kernel.ActTaskId, g_ResCb[ResID].ReplacedPrio);
  g_TaskCb[g_Kernel.ActTaskId].ResourceLevelCnt--;

  g_ResCb[ResID].Occupied = OS_FALSE;
  g_ResCb[ResID].ReplacedPrio = (Os_PrioType)0;
  g_ResCb[ResID].OwnerTask = IDLE_LOOP;

  Os_ScheduleInternal(OS_FALSE, OS_FALSE);

  return E_OK;
#else
  ERRORHOOK(E_OS_ID);
  return E_OS_ID;
#endif /* NUMBER_OF_RESOURCES > 0 */
}

StatusType Os_ActivateIsr(ISRType IsrId)
{
    g_ErrorInfo.ServiceID = OSServiceId_ActivateISR;

    if(IsrId >= NUMBER_OF_ISRS)
    {
        ERRORHOOK(E_OS_ID);
        return E_OS_ID;
    }

    g_Kernel.CallLevel        = ISR_LEVEL;
    g_Kernel.ApplicationLevel = ISR_LEVEL;
    g_Kernel.ActIsrId         = IsrId;

    (*(g_IsrCb[IsrId].pIsrFcn))();

    g_Kernel.ActIsrId         = DUMMY_ISR;
    g_Kernel.CallLevel        = OS_LEVEL;
    g_Kernel.ApplicationLevel = OS_LEVEL;

    Os_ScheduleInternal(OS_TRUE, OS_TRUE);

    return E_OK;
}

#endif /* OS_RESINT_C */

/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/
