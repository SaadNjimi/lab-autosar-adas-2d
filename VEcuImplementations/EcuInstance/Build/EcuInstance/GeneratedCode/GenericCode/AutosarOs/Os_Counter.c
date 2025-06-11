/**************************************************************************************************\
 *** Os_Counter.c
 ***
 *** OSEK API function definition for OS Counters.
 ***
 *** Copyright (c) 2014 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef OS_COUNTER_C
#define OS_COUNTER_C

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/

#include "Os_Alarm.h"
#include "Os_SchedTbl.h"
#include "Os_Counter.h"

#include "Os_Kernel.h"
#include "Os_InternalTypes.h"
#include "Os_Cfg.h"
#include "Os_SimulationCore.h"

/*------------------------------------------------------------------------------------------------*\
  FUNCTION DEFINITIONS
\*------------------------------------------------------------------------------------------------*/

#if NUMBER_OF_COUNTERS > 0

StatusType Os_InitCounter(CounterType CounterID, TickType Ticks)
{
    g_ErrorInfo.ServiceID = OSServiceId_InitCounter;

    if(CounterID >= NUMBER_OF_COUNTERS)
    {
        ERRORHOOK(E_OS_ID);
        return E_OS_ID;
    }

    if (0 == g_CtrCb[CounterID].MaxAllowedValue)
    {
        ERRORHOOK(E_OS_SYS_INTERNAL);
        return E_OS_SYS_INTERNAL;
    }

    if(Ticks > g_CtrCb[CounterID].MaxAllowedValue)
    {
        ERRORHOOK(E_OS_VALUE);
        return E_OS_VALUE;
    }

    if(Ticks == (TickType)0)
    {
        Ticks = (TickType)-1;
    }

    g_CtrCb[CounterID].CtrValue = Ticks;

    return E_OK;
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     Os_CounterTriggerSolely
 ***
 *** DESCRIPTION:
 ***    Trigger the counter with ID CounterID. If the counter drives an alarm or schedule table the function
 ***    will activate the tasks or set events of these alarms or schedule tables.
 ***
 ***
 *** PARAMETERS:
 ***     Type             Name     Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     CounterType      CounterID The ID of the counter
 ***
 ***
 *** RETURNS:
 ***     StatusType
\**************************************************************************************************/
StatusType Os_CounterTriggerSolely(CounterType CounterID)
{
    StatusType returnValue = E_OK;

    /* no need to adjust alarms anymore after first counter trigger,
       because first counter trigger will step system time from -1 to 0 */
    if (OS_FALSE == g_CtrCb[CounterID].IsInitialized)
    {
        g_CtrCb[CounterID].IsInitialized = OS_TRUE;
    }

    g_ErrorInfo.ServiceID = OSServiceId_CounterTrigger;

    if (CounterID >= NUMBER_OF_COUNTERS)
    {
        ERRORHOOK(E_OS_ID);
        return E_OS_ID;
    }

    g_CtrCb[CounterID].OldSimTime++;
    if (g_CtrCb[CounterID].CtrValue == g_CtrCb[CounterID].MaxAllowedValue)
    {
        g_CtrCb[CounterID].CtrValue = (TickType)0u;
    }
    else
    {
        g_CtrCb[CounterID].CtrValue++;
    }

#if NUMBER_OF_ALARMS > 0

   /* If the counter drives an alarm 'Os_AlarmHandler()' will activate the tasks or set events of these alarms. */
   returnValue = Os_AlarmHandler(CounterID);

#endif /* NUMBER_OF_ALARMS > 0 */

#if NUMBER_OF_SCHEDULE_TABLES > 0

   /* If the counter drives a schedule table 'Os_ScheduleTableHandler()' will activate the tasks or set events of these schedule table. */
   returnValue = Os_ScheduleTableHandler(CounterID);

#endif

    return returnValue;
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     GetCounterValue
 ***
 *** DESCRIPTION:
 ***    Get for a counter with ID 'CounterID' the counter value
 ***
 *** PARAMETERS:
 ***     Type             Name     Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     CounterType      CounterID    Id of the counter
 ***     TickRefType      TicksRef     Value of the counter
 ***
 *** RETURNS:
 ***     StatusType
\**************************************************************************************************/
StatusType GetCounterValue(CounterType CounterID, TickRefType TicksRef)
{
    g_ErrorInfo.ServiceID = OSServiceId_GetCounterValue1;

    if(CounterID >= NUMBER_OF_COUNTERS)
    {
        ERRORHOOK(E_OS_ID);
        return E_OS_ID;
    }

    *TicksRef = g_CtrCb[CounterID].CtrValue;

    return E_OK;
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     GetElapsedCounterValue
 ***
 *** DESCRIPTION:
 ***    Get for a counter with the ID 'CounterID' the elapsed counter value
 ***
 *** PARAMETERS:
 ***     Type             Name     Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     CounterType      CounterID     Id of the counter
 ***     TickRefType      Value         Value of the counter
 ***     TickRefType      ElapsedValue  Value of the counter
 ***
 *** RETURNS:
 ***     StatusType
\**************************************************************************************************/
StatusType GetElapsedCounterValue(CounterType CounterID, TickRefType Value, TickRefType ElapsedValue)
{
    TickType lValue;

    if (CounterID > NUMBER_OF_COUNTERS)
    {
        ERRORHOOK(E_OS_ID);
        return E_OS_ID;
    }

    if (OS_NULL == Value)
    {
        ERRORHOOK(E_OS_ILLEGAL_ADDRESS);
        return E_OS_ILLEGAL_ADDRESS;
    }

    if (OS_NULL == ElapsedValue)
    {
        ERRORHOOK(E_OS_ILLEGAL_ADDRESS);
        return E_OS_ILLEGAL_ADDRESS;
    }

    lValue = *(Value);
    if (lValue > g_CtrCb[CounterID].MaxAllowedValue)
    {
        ERRORHOOK(E_OS_VALUE);
        return E_OS_VALUE;
    }

    lValue = lValue + g_CtrCb[CounterID].CtrValue;
    lValue = lValue % g_CtrCb[CounterID].MaxAllowedValue;

    *(ElapsedValue) = lValue;

    return E_OK;
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     Os_CounterTrigger
 ***
 *** DESCRIPTION:
 ***    Check modulo of 'MultipleOfSimTimeBase' and 'SimTime' for a counter
 ***       with the ID 'CounterID' and call the function 'Os_CounterTriggerSolely()'
 ***
 *** PARAMETERS:
 ***     Type             Name     Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     CounterType      CounterID    Id of the counter
 ***
 *** RETURNS:
 ***     StatusType
\**************************************************************************************************/
StatusType Os_CounterTrigger(CounterType CounterID)
{
    StatusType Status;

    if( !(g_Kernel.SimTime % g_CtrCb[CounterID].MultipleOfSimTimeBase) )
    {
        Status = Os_CounterTriggerSolely(CounterID);
        if (Status != E_OK)
        {
            return Status;
        }
    }

    return E_OK;
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     IncrementCounter
 ***
 *** DESCRIPTION:
 ***    IncrementCounter() shall increment the counter <CounterID> by one (if any alarm connected
 ***    to this counter expires, the given action, e.g. task activation, is done) and shall return E_OK.
 ***
 *** PARAMETERS:
 ***     Type             Name     Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ***     CounterType      CounterID    Id of the counter
 ***
 *** RETURNS:
 ***     StatusType
\**************************************************************************************************/
StatusType IncrementCounter(CounterType CounterID)
{
#ifdef OS_DBGMSG_ENABLED
    Os_SimulationCoreLogDbgMessage(__FUNCTION__, "IncrementCounter(): Start of function. Counter ID is %d.", CounterID);
#endif

    if( CounterID >= NUMBER_OF_COUNTERS)
    {
        ERRORHOOK(E_OS_ID);
        return E_OS_ID;
    }

    if (g_CtrCb[CounterID].counterType == OS_COUNTER_HARDWARE)
    {
        ERRORHOOK(E_OS_VALUE);
        return E_OS_VALUE;
    }

    Os_CounterTrigger(CounterID);

    return E_OK;
}

#endif /* NUMBER_OF_COUNTERS > 0 */

#endif /* OS_COUNTER_C */

/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/
