/**************************************************************************************************\
 *** Os_SchedTbl.c
 ***
 *** AUTOSAR API function definition for OS Schedule tables.
 ***
 *** Copyright (c) 2014 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef OS_SCHEDTBL_C
#define OS_SCHEDTBL_C

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/

#include "Os_Types.h"
#include "Os_Cfg.h"
#include "Os_SchedTbl.h"
#include "Os_Kernel.h"

/*------------------------------------------------------------------------------------------------*\
  FUNCTION DECLARATIONS
\*------------------------------------------------------------------------------------------------*/

#if NUMBER_OF_SCHEDULE_TABLES > 0

/**************************************************************************************************\
 *** FUNCTION:
 ***     StartScheduleTableRel
 ***
 *** DESCRIPTION:
 ***     StartScheduleTableRel() shall start the processing of a schedule table <ScheduleTableID>.
 ***     The Initial Expiry Point shall be processed after <Offset> + Initial Offset ticks have
 ***     elapsed on the underlying counter. The state of <ScheduleTableID> to SCHEDULETABLE_RUNNING.
 ***
 *** PARAMETERS:
 ***     Type             Name     Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     ScheduleTableType    ScheduleTableID     Id of the schedule table.
 ***     TickType             Offset              Start schedule Table from now + Offset.
 ***
 *** RETURNS:
 ***     StatusType
\**************************************************************************************************/
StatusType StartScheduleTableRel(ScheduleTableType ScheduleTableID, TickType Offset)
{
    g_ErrorInfo.ServiceID = OSServiceId_StartScheduleTableRel;

    if( ScheduleTableID >= NUMBER_OF_SCHEDULE_TABLES)
    {
        ERRORHOOK(E_OS_ID);
        return E_OS_ID;
    }

    if (g_CtrCb[g_SchedTbl[ScheduleTableID].counterId].IsInitialized == OS_TRUE )
    {
        if( Offset > (g_CtrCb[g_SchedTbl[ScheduleTableID].counterId].MaxAllowedValue - g_CtrCb[g_SchedTbl[ScheduleTableID].counterId].CtrValue) \
            || Offset == 0)
       {
            ERRORHOOK(E_OS_VALUE);
            return E_OS_VALUE;
       }
    }

    if( g_SchedTblExec[ScheduleTableID].status != SCHEDULETABLE_STOPPED )
    {
        ERRORHOOK(E_OS_STATE);
        return E_OS_STATE;
    }

    if( g_SchedTbl[ScheduleTableID].syncStrategy == OS_IMPLICIT_SYNCHRONISATION )
    {
        ERRORHOOK(E_OS_ID);
        return E_OS_ID;
    }

#if NUMBER_OF_OS_APPLICATIONS > 0
    /* is the OS-Application, which belongs to the schedTable, started? */
    if (g_SchedTbl[ScheduleTableID].ConnectApplicationId != OS_NOT_CONNECTED_TO_ANY_APPLICATION)
    {
        if( (g_ApplicationCb[g_SchedTbl[ScheduleTableID].ConnectApplicationId].State == APPLICATION_RESTARTING) ||
            (g_ApplicationCb[g_SchedTbl[ScheduleTableID].ConnectApplicationId].State == APPLICATION_TERMINATED) )
        {
            /* [SWS_Os_00509] If a service call is made on an Operating System object that is owned by another OS-Application without state APPLICATION_ACCESSIBLE, then the Operating System module shall return E_OS_ACCESS. */
            ERRORHOOK(E_OS_ACCESS);
            return E_OS_ACCESS;
        }
    }

    /* is this task belong to an os application ? */
    if (g_TaskCb[g_Kernel.ActTaskId].ConnectApplicationId != OS_NOT_CONNECTED_TO_ANY_APPLICATION)
    {
        /* belong the actual task and the to activated SchedTbl to the different os application ? */
        if(g_TaskCb[g_Kernel.ActTaskId].ConnectApplicationId != g_SchedTbl[ScheduleTableID].ConnectApplicationId )
        {
            /* has the current os application the access rights for the 'ScheduleTableID' ? */
            if ( g_SchedTbl[ScheduleTableID].ApplicationHasAccess[g_TaskCb[g_Kernel.ActTaskId].ConnectApplicationId] == OS_FALSE)
            {
                /* [SWS_Os_00589] All functions that are not allowed to operate cross core shall return E_OS_CORE in extended status if called with parameters that require a cross core operation. */
                ERRORHOOK(E_OS_CORE);
                return E_OS_CORE;
            }
        }
    }
#endif

    if (g_CtrCb[g_SchedTbl[ScheduleTableID].counterId].IsInitialized == OS_FALSE )
    {
        /* OS is in initialization and the timestep t=0 was not yet calculated. */
        g_SchedTblExec[ScheduleTableID].match  = Offset;
        g_SchedTblExec[ScheduleTableID].status = SCHEDULETABLE_RUNNING;
    }
    else
    {
        g_SchedTblExec[ScheduleTableID].match  = g_CtrCb[g_SchedTbl[ScheduleTableID].counterId].CtrValue + Offset;
        g_SchedTblExec[ScheduleTableID].status = SCHEDULETABLE_RUNNING;
    }
    return E_OK;
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     StartScheduleTableAbs
 ***
 *** DESCRIPTION:
 ***     StartScheduleTableAbs() shall start the processing of schedule table <ScheduleTableID> at
 ***     count value <Start> and shall set the state of <ScheduleTableID> to SCHEDULETABLE_RUNNING or
 ***     SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS. The Initial Expiry Point will be processed when the
 ***     underlying counter equals <Start>+Initial Offset.
 ***
 *** PARAMETERS:
 ***     Type             Name     Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     ScheduleTableType    ScheduleTableID     Id of the schedule table.
 ***     TickType             Offset              Start schedule Table at Tick Start.
 *** RETURNS:
 ***     StatusType
\**************************************************************************************************/
StatusType StartScheduleTableAbs(ScheduleTableType ScheduleTableID, TickType Start)
{
    g_ErrorInfo.ServiceID = OSServiceId_StartScheduleTableAbs;

    if(ScheduleTableID >= NUMBER_OF_SCHEDULE_TABLES)
    {
        ERRORHOOK(E_OS_ID);
        return E_OS_ID;
    }

    if(Start > g_CtrCb[g_SchedTbl[ScheduleTableID].counterId].MaxAllowedValue)
    {
        ERRORHOOK(E_OS_VALUE);
        return E_OS_VALUE;
    }

    if( g_SchedTblExec[ScheduleTableID].status != SCHEDULETABLE_STOPPED )
    {
        ERRORHOOK(E_OS_STATE);
        return E_OS_STATE;
    }

#if NUMBER_OF_OS_APPLICATIONS > 0
    /* is the OS-Application, which belongs to the schedTable, started? */
    if (g_SchedTbl[ScheduleTableID].ConnectApplicationId != OS_NOT_CONNECTED_TO_ANY_APPLICATION)
    {
        if( (g_ApplicationCb[g_SchedTbl[ScheduleTableID].ConnectApplicationId].State == APPLICATION_RESTARTING) ||
            (g_ApplicationCb[g_SchedTbl[ScheduleTableID].ConnectApplicationId].State == APPLICATION_TERMINATED) )
        {
            /* [SWS_Os_00509] If a service call is made on an Operating System object that is owned by another OS-Application without state APPLICATION_ACCESSIBLE, then the Operating System module shall return E_OS_ACCESS. */
            ERRORHOOK(E_OS_ACCESS);
            return E_OS_ACCESS;
        }
    }

    /* is this task belong to an os application ? */
    if (g_TaskCb[g_Kernel.ActTaskId].ConnectApplicationId != OS_NOT_CONNECTED_TO_ANY_APPLICATION)
    {
        /* belong the actual task and the to activated SchedTbl to the different os application ? */
        if(g_TaskCb[g_Kernel.ActTaskId].ConnectApplicationId != g_SchedTbl[ScheduleTableID].ConnectApplicationId )
        {
            /* has the current os application the access rights for the 'ScheduleTableID' ? */
            if ( g_SchedTbl[ScheduleTableID].ApplicationHasAccess[g_TaskCb[g_Kernel.ActTaskId].ConnectApplicationId] == OS_FALSE)
            {
                /* [SWS_Os_00589] All functions that are not allowed to operate cross core shall return E_OS_CORE in extended status if called with parameters that require a cross core operation. */
                ERRORHOOK(E_OS_CORE);
                return E_OS_CORE;
            }
        }
    }
#endif

    if( g_SchedTbl[ScheduleTableID].syncStrategy == OS_IMPLICIT_SYNCHRONISATION && Start == 0 )
    {
        if( (g_CtrCb[g_SchedTbl[ScheduleTableID].counterId].MaxAllowedValue + 1) == g_SchedTbl[ScheduleTableID].duration)
        {
            g_SchedTblExec[ScheduleTableID].status = SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS;
        }
        else
        {
            g_SchedTblExec[ScheduleTableID].status = SCHEDULETABLE_RUNNING;
        }
    }
    else
    {
        g_SchedTblExec[ScheduleTableID].status = SCHEDULETABLE_RUNNING;

    }

    g_SchedTblExec[ScheduleTableID].match = Start;

    return E_OK;
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     StopScheduleTable
 ***
 *** DESCRIPTION:
 ***     StopScheduleTable()shall set the state of <ScheduleTableID> to SCHEDULETABLE_STOPPED and
 ***     (stop the schedule table <ScheduleTableID> from processing any further expiry points and)
 ***     shall return E_OK.
 ***
 *** PARAMETERS:
 ***     Type             Name     Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ***     ScheduleTableType    ScheduleTableID     Id of the schedule table.
 ***
 *** RETURNS:
 ***     StatusType
\**************************************************************************************************/
StatusType StopScheduleTable(ScheduleTableType ScheduleTableID)
{
    g_ErrorInfo.ServiceID = OSServiceId_StopScheduleTable;

    if(ScheduleTableID >= NUMBER_OF_SCHEDULE_TABLES)
    {
        ERRORHOOK(E_OS_ID);
        return E_OS_ID;
    }

    if( g_SchedTblExec[ScheduleTableID].status == SCHEDULETABLE_STOPPED )
    {
        ERRORHOOK(E_OS_NOFUNC);
        return E_OS_NOFUNC;
    }

#if NUMBER_OF_OS_APPLICATIONS > 0
    /* is the OS-Application, which belongs to the schedTable, started? */
    if (g_SchedTbl[ScheduleTableID].ConnectApplicationId != OS_NOT_CONNECTED_TO_ANY_APPLICATION)
    {
        if( (g_ApplicationCb[g_SchedTbl[ScheduleTableID].ConnectApplicationId].State == APPLICATION_RESTARTING) ||
            (g_ApplicationCb[g_SchedTbl[ScheduleTableID].ConnectApplicationId].State == APPLICATION_TERMINATED) )
        {
            /* [SWS_Os_00509] If a service call is made on an Operating System object that is owned by another OS-Application without state APPLICATION_ACCESSIBLE, then the Operating System module shall return E_OS_ACCESS. */
            ERRORHOOK(E_OS_ACCESS);
            return E_OS_ACCESS;
        }
    }

    /* is this task belong to an os application ? */
    if (g_TaskCb[g_Kernel.ActTaskId].ConnectApplicationId != OS_NOT_CONNECTED_TO_ANY_APPLICATION)
    {
        /* belong the actual task and the to activated SchedTbl to the different os application ? */
        if(g_TaskCb[g_Kernel.ActTaskId].ConnectApplicationId != g_SchedTbl[ScheduleTableID].ConnectApplicationId )
        {
            /* has the current os application the access rights for the 'ScheduleTableID' ? */
            if ( g_SchedTbl[ScheduleTableID].ApplicationHasAccess[g_TaskCb[g_Kernel.ActTaskId].ConnectApplicationId] == OS_FALSE)
            {
                /* [SWS_Os_00589] All functions that are not allowed to operate cross core shall return E_OS_CORE in extended status if called with parameters that require a cross core operation. */
                ERRORHOOK(E_OS_CORE);
                return E_OS_CORE;
            }
        }
    }
#endif

    g_SchedTblExec[ScheduleTableID].status = SCHEDULETABLE_STOPPED;
    g_SchedTblExec[ScheduleTableID].ticks = 0;

    g_SchedTblExec[ScheduleTableID].expiryPointNext = 0;

#ifdef OS_SCHEDULETABLE_EXPLICIT_SYNCHRONISATION
    g_SchedTblExec[ScheduleTableID].actEpCanMoved = OS_TRUE;
    g_SchedTblExec[ScheduleTableID].deviation = 0;
#endif

    return E_OK;
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     NextScheduleTable
 ***
 *** DESCRIPTION:
 ***     NextScheduleTable() shall start the processing of schedule table <ScheduleTableID_To>
 ***     <ScheduleTableID_From>.FinalDelay ticks after the Final Expiry Point on
 ***     <ScheduleTableID_From> is processed and shall return E_OK. The Initial Expiry Point on
 ***     <ScheduleTableID_To> shall be processed at <ScheduleTableID_From>.Final Delay +
 ***     <ScheduleTable_To>.Initial Offset ticks after the Final Expiry Point on
 ***     <ScheduleTableID_From> is processed .
 ***     If <ScheduleTableID_From> already has a "next" schedule table then the <ScheduleTableID_To>
 ***     shall replace the previous "next" schedule table and the old "next" schedule table state
 ***     becomes SCHEDULETABLE_STOPPED.
 ***
 *** PARAMETERS:
 ***     Type             Name     Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     ScheduleTableType ScheduleTableID_From     .
 ***     ScheduleTableType ScheduleTableID_To
 ***
 *** RETURNS:
 ***     StatusType
\**************************************************************************************************/
StatusType NextScheduleTable(ScheduleTableType ScheduleTableID_From, ScheduleTableType ScheduleTableID_To)
{
    Os_ScheduleTableControlBlockTypeExecRef nextSchedTbl;

    g_ErrorInfo.ServiceID = OSServiceId_NextScheduleTable;

    /*
    if( g_SchedTbl[ScheduleTableID_From].syncStrategy == EXPLICIT_SYNCHRONISATION &&  g_SchedTbl[ScheduleTableID_To].syncStrategy == IMPLICIT_SYNCHRONISATION)
    {
        ERRORHOOK(E_OS_NOFUNC);
        return E_OS_NOFUNC;
    }
    */

    if(ScheduleTableID_From >= NUMBER_OF_SCHEDULE_TABLES|| ScheduleTableID_To >= NUMBER_OF_SCHEDULE_TABLES)
    {
        ERRORHOOK(E_OS_ID);
        return E_OS_ID;
    }

    if( g_SchedTbl[ScheduleTableID_From].counterId != g_SchedTbl[ScheduleTableID_To].counterId )
    {
        ERRORHOOK(E_OS_ID);
        return E_OS_ID;
    }

    if( g_SchedTblExec[ScheduleTableID_From].status == SCHEDULETABLE_STOPPED || g_SchedTblExec[ScheduleTableID_From].status == SCHEDULETABLE_NEXT  )
    {
        ERRORHOOK(E_OS_NOFUNC);
        return E_OS_NOFUNC;
    }

    if( g_SchedTblExec[ScheduleTableID_To].status != SCHEDULETABLE_STOPPED)
    {
        ERRORHOOK(E_OS_STATE);
        return E_OS_STATE;
    }

    g_SchedTblExec[ScheduleTableID_To].status = SCHEDULETABLE_NEXT;

    if(g_SchedTblExec[ScheduleTableID_From].nextSchedTbl != 0)
    {
        nextSchedTbl = g_SchedTblExec[ScheduleTableID_From].nextSchedTbl;
        nextSchedTbl->status = SCHEDULETABLE_STOPPED;
    }

    g_SchedTblExec[ScheduleTableID_From].nextSchedTbl =  &g_SchedTblExec[ScheduleTableID_To];

    return E_OK;
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     StartScheduleTableSynchron
 ***
 *** DESCRIPTION:
 ***     StartScheduleTableSynchron() shall set the state of <ScheduleTableID> to SCHEDULETABLE_WAITING
 ***     and start the processing of schedule table <ScheduleTableID> after the synchronization count
 ***     of the schedule table is set via SyncScheduleTable(). The Initial Expiry Point shall
 ***     processed when (Duration-SyncValue)+InitialOffset ticks have elapsed on the synchronization
 ***     counter where:
 ***         Duration is <ScheduleTableID>.OsScheduleTableDuration
 ***         SyncValue is the <Value> parameter passed to the SyncScheduleTable()
 ***         InitialOffset is the shortest expiry point offset in <ScheduleTableID>
 ***
 *** PARAMETERS:
 ***     Type             Name     Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***      ScheduleTableType    ScheduleTableID     Id of the schedule table.
 ***
 *** RETURNS:
 ***     StatusType
\**************************************************************************************************/
StatusType StartScheduleTableSynchron(ScheduleTableType ScheduleTableID )
{
    g_ErrorInfo.ServiceID = OSServiceId_StartScheduleTableSynchron;

    if(ScheduleTableID >= NUMBER_OF_SCHEDULE_TABLES )
    {
        ERRORHOOK(E_OS_ID);
        return E_OS_ID;
    }

    if( g_SchedTbl[ScheduleTableID].syncStrategy != OS_EXPLICIT_SYNCHRONISATION )
    {
        ERRORHOOK(E_OS_ID);
        return E_OS_ID;
    }

    if( g_SchedTblExec[ScheduleTableID].status != SCHEDULETABLE_STOPPED  )
    {
        ERRORHOOK(E_OS_STATE);
        return E_OS_STATE;
    }

    g_SchedTblExec[ScheduleTableID].status = SCHEDULETABLE_WAITING;

    return E_OK;
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     SyncScheduleTable
 ***
 *** DESCRIPTION:
 ***     The service provides the operating system with the current synchronization count for the
 ***     given schedule table. (It is used to synchronize the processing of the schedule table to
 ***     the synchronization counter.)
 ***
 *** PARAMETERS:
 ***     Type             Name     Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     ScheduleTableType    ScheduleTableID      Id of the schedule table.
 ***     TickType             Value                Value of the sychronisation counter.
 ***
 *** RETURNS:
 ***     StatusType
\**************************************************************************************************/
StatusType SyncScheduleTable(ScheduleTableType ScheduleTableID, TickType Value)
{
    g_ErrorInfo.ServiceID = OSServiceId_SyncScheduleTable;

    if(ScheduleTableID >= NUMBER_OF_SCHEDULE_TABLES )
    {
        ERRORHOOK(E_OS_ID);
        return E_OS_ID;
    }

    if( g_SchedTbl[ScheduleTableID].syncStrategy != OS_EXPLICIT_SYNCHRONISATION )
    {
       ERRORHOOK(E_OS_ID);
       return E_OS_ID;
    }

    if( Value > g_SchedTbl[ScheduleTableID].duration - 1 )
    {
        ERRORHOOK(E_OS_VALUE);
        return E_OS_VALUE;
    }

    if( g_SchedTblExec[ScheduleTableID].status == SCHEDULETABLE_STOPPED || g_SchedTblExec[ScheduleTableID].status == SCHEDULETABLE_NEXT)
    {
        ERRORHOOK(E_OS_STATE);
        return E_OS_STATE;
    }

#ifdef OS_SCHEDULETABLE_EXPLICIT_SYNCHRONISATION
    if( g_SchedTblExec[ScheduleTableID].status == SCHEDULETABLE_WAITING)
    {
        g_SchedTblExec[ScheduleTableID].match = g_CtrCb[g_SchedTbl[ScheduleTableID].counterId].CtrValue + (g_SchedTbl[ScheduleTableID].duration - Value); /* + (g_SchedTbl[ScheduleTableID].expiryPointTbl+ g_SchedTblExec[ScheduleTableID].expiryPointNext)->offset; */
        if( g_SchedTblExec[ScheduleTableID].match >= g_CtrCb[g_SchedTbl[ScheduleTableID].counterId].MaxAllowedValue)
        {
            g_SchedTblExec[ScheduleTableID].match = g_SchedTblExec[ScheduleTableID].match % (g_CtrCb[g_SchedTbl[ScheduleTableID].counterId].MaxAllowedValue + 1);
        }
        g_SchedTblExec[ScheduleTableID].status = SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS;
    }
    else
    {
        sint32 deviation;

        /* Maybe -1 has to be subtracted */
        deviation = g_SchedTblExec[ScheduleTableID].ticks - Value;
        g_SchedTblExec[ScheduleTableID].deviation = deviation;

        if(deviation == 0)
        {
            g_SchedTblExec[ScheduleTableID].status = SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS;
        }
    }
#endif

    return E_OK;
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     SetScheduleTableAsync
 ***
 *** DESCRIPTION:
 ***     If OsScheduleTblSyncStrategy of <ScheduleTableID> equals to EXPLICIT_SYNCHRONISATION then
 ***     SetScheduleTableAsync() shall set the status of <ScheduleTableID> to SCHEDULETABLE_RUNNING.
 ***
 *** PARAMETERS:
 ***     Type             Name     Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     ScheduleTableType    ScheduleTableID      Id of the schedule table.
 ***
 *** RETURNS:
 ***     StatusType
\**************************************************************************************************/
StatusType SetScheduleTableAsync( ScheduleTableType ScheduleTableID )
{
#ifdef OS_SCHEDULETABLE_EXPLICIT_SYNCHRONISATION
    g_ErrorInfo.ServiceID = OSServiceId_SetScheduleTableAsync;

    if (g_SchedTbl[ScheduleTableID].syncStrategy == OS_EXPLICIT_SYNCHRONISATION )
    {
        if (g_SchedTblExec[ScheduleTableID].status == SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS )
        {
            g_SchedTblExec[ScheduleTableID].status = SCHEDULETABLE_RUNNING;
            g_SchedTblExec[ScheduleTableID].deviation = 0;
        }
    }
    else
    {
        return E_OS_ID;
    }

    return E_OK;
#else
    /* to avoid the compiler warning C4100 */
    (void)(ScheduleTableID);

    return E_OS_ID;
#endif
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     GetScheduleTableStatus
 ***
 *** DESCRIPTION:
 ***     Pass back schedule table status via the reference parameter ScheduleStatus AND shall return E_OK.
 ***
 *** PARAMETERS:
 ***     Type             Name     Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     ScheduleTableType    ScheduleTableID      Id of the schedule table.
 ***     ScheduleTableStatusRefType ScheduleStatus returns status <ScheduleTableID>
 ***
 *** RETURNS:
 ***     StatusType
\**************************************************************************************************/
StatusType GetScheduleTableStatus( ScheduleTableType ScheduleTableID, ScheduleTableStatusRefType ScheduleStatus)
{
    g_ErrorInfo.ServiceID = OSServiceId_GetScheduleTableStatus;

    if(ScheduleTableID >= NUMBER_OF_SCHEDULE_TABLES )
    {
        ERRORHOOK(E_OS_ID);
        return E_OS_ID;
    }

#if NUMBER_OF_OS_APPLICATIONS > 0
    /* is this task belong to an os application ? */
    if (g_TaskCb[g_Kernel.ActTaskId].ConnectApplicationId != OS_NOT_CONNECTED_TO_ANY_APPLICATION)
    {
        /* belong the actual task and the to activated SchedTbl to the different os application ? */
        if(g_TaskCb[g_Kernel.ActTaskId].ConnectApplicationId != g_SchedTbl[ScheduleTableID].ConnectApplicationId )
        {
            /* has the current os application the access rights for the 'ScheduleTableID' ? */
            if ( g_SchedTbl[ScheduleTableID].ApplicationHasAccess[g_TaskCb[g_Kernel.ActTaskId].ConnectApplicationId] == OS_FALSE)
            {
                /* [SWS_Os_00589] All functions that are not allowed to operate cross core shall return E_OS_CORE in extended status if called with parameters that require a cross core operation. */
                ERRORHOOK(E_OS_CORE);
                return E_OS_CORE;
            }
        }
    }
#endif

   *ScheduleStatus =  g_SchedTblExec[ScheduleTableID].status;

    return E_OK;
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     Os_ScheduleTableHandler
 ***
 *** DESCRIPTION:
 ***     If the counter drives a schedule table this function
 ***     will activate the tasks or set events of these schedule tables.
 ***
 *** PARAMETERS:
 ***     Type             Name     Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     CounterType      CounterID The ID of the counter
 ***
 *** RETURNS:
 ***     StatusType
\**************************************************************************************************/

StatusType Os_ScheduleTableHandler(CounterType CounterID)
{
    StatusType returnValue = E_OK;

    uint32 i;
    ScheduleTableType schedTblId;
    Os_ScheduleTableControlBlockTypeExecRef schedTbl;
    Os_ExpiryPointControlBlockTypeRef ep;
    Os_ScheduleTableControlBlockTypeExecRef nextSchedTbl;

    /* check all schedule tables which connect to the counter if they should executec */
    for (schedTblId = 0; schedTblId <  NUMBER_OF_SCHEDULE_TABLES; schedTblId++)
    {
        if (g_CtrCb[CounterID].ConnectedScheduleTables[schedTblId] == OS_TRUE)
        {
            /* Only schedule tables with status SCHEDULETABLE_RUNNING or SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS must be considerd */
            if (( g_CtrCb[CounterID].SchedTblControlBlock[schedTblId].status == SCHEDULETABLE_RUNNING) || ( g_CtrCb[CounterID].SchedTblControlBlock[schedTblId].status == SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS))
            {
                /* get the schedule table from "connected" table */
                schedTbl = &(g_CtrCb[CounterID].SchedTblControlBlock[schedTblId]);

                if (g_CtrCb[CounterID].CtrValue == schedTbl->match  )
                {
                    schedTbl->match++;
                    if (g_CtrCb[CounterID].MaxAllowedValue + 1 == schedTbl->match)
                    {
                        schedTbl->match = 0;
                    }

                    /* increment the schedule table counter */
                    schedTbl->ticks++;

                    /* if all eps was executed we must reset the schedule table  */
                    if (schedTbl->expiryPointNext < schedTbl->schedTblConst->numberOfExpiryPoints)
                    {
#ifdef OS_SCHEDULETABLE_EXPLICIT_SYNCHRONISATION
                        /* If sync stragtegy is  EXPLICIT_SYNCHRONISATION  we have to consider the devation from the sync counter */
                        if ((schedTbl->schedTblConst->syncStrategy == OS_EXPLICIT_SYNCHRONISATION))
                        {
                            /* if the devation is positive then we have to move back the schedule table counter */
                            if (schedTbl->deviation > 0)
                            {
                                /* the flag signs if the next expiry point was moved back (OS_TRUE). Then we must wait up to the expiry point is executed
                                   and move back the next expiry point dependable of his retard*/
                                if (schedTbl->actEpCanMoved == OS_TRUE)
                                {
                                    /* It's assured, that the deviation is greater than 0. The type cast is safe. */
                                    if (((uint32)(schedTbl->deviation)) <= schedTbl->schedTblConst->expiryPointTbl[schedTbl->expiryPointNext].maxRetard)
                                    {
                                        /* subtract the deviation form schedule table counter NOTE: in this case the deviation is positive*/
                                        schedTbl->ticks -= schedTbl->deviation;
                                        schedTbl->deviation = 0;
                                    }
                                    else
                                    {
                                        /* subtract the deviation form schedule table counter NOTE: in this case the deviation is positive*/
                                        schedTbl->ticks -= schedTbl->schedTblConst->expiryPointTbl[schedTbl->expiryPointNext].maxRetard;
                                        /* the deviation is bigger than the max retard then we must divide the deviation to other expiry point*/
                                        schedTbl->deviation -= schedTbl->schedTblConst->expiryPointTbl[schedTbl->expiryPointNext].maxRetard;
                                    }

                                    /* the ep was moved with max retard the next eps get the rest of the deviation */
                                    schedTbl->actEpCanMoved = OS_FALSE;
                                }

                                /* check in which status the schedule table will run at the next tick */
                                /* It's assured, that the deviation is greater than 0. The type cast is safe. */
                                if (((uint32)(schedTbl->deviation)) > schedTbl->schedTblConst->precision)
                                {
                                    schedTbl->status = OS_SCHEDULETABLEMODE_RUNNING;
                                }
                                else
                                {
                                    schedTbl->status = OS_SCHEDULETABLEMODE_RUNNING_AND_SYNCHRONOUS;
                                }
                            }

                            /* the same with a NEGATIVE deviation*/
                            if (schedTbl->deviation < 0)
                            {
                                if (schedTbl->actEpCanMoved == OS_TRUE)
                                {
                                    /* It's assured, that the deviation is less than 0. The type cast is safe. */
                                    if (((uint32)(-schedTbl->deviation)) <= schedTbl->schedTblConst->expiryPointTbl[schedTbl->expiryPointNext].maxAdvance)
                                    {
                                        schedTbl->ticks += -schedTbl->deviation;
                                        schedTbl->deviation = 0;
                                    }
                                    else
                                    {
                                        schedTbl->ticks += schedTbl->schedTblConst->expiryPointTbl[schedTbl->expiryPointNext].maxAdvance;
                                        schedTbl->deviation +=schedTbl->schedTblConst->expiryPointTbl[schedTbl->expiryPointNext].maxAdvance;
                                    }

                                    schedTbl->actEpCanMoved = OS_FALSE;
                                }

                                /* It's assured, that the deviation is less than 0. The type cast is safe. */
                                if (((uint32)(-schedTbl->deviation)) > schedTbl->schedTblConst->precision)
                                {
                                    schedTbl->status = OS_SCHEDULETABLEMODE_RUNNING;
                                }
                                else
                                {
                                    schedTbl->status = OS_SCHEDULETABLEMODE_RUNNING_AND_SYNCHRONOUS;
                                }
                            }
                        }
#endif
                        /* get the next ep */
                        ep = &schedTbl->schedTblConst->expiryPointTbl[schedTbl->expiryPointNext];

                        /* if the ep is reached then execute him */
                        if (schedTbl->ticks == ep->offset + 1)
                        {
                            for (i = 0; i < ep->numberOfEvents; i++)
                            {
                                if (ep->taskEventTbl[i]->ExtendedTask != OS_TRUE)
                                {
                                    ERRORHOOK(E_OS_ACCESS);
                                    returnValue = (E_OS_ACCESS);
                                    continue; /* Go to next loop element */
                                }

                                if (ep->taskEventTbl[i]->State == SUSPENDED)
                                {
                                    ERRORHOOK(E_OS_STATE);
                                    returnValue = (E_OS_STATE);
                                    continue; /* Go to next loop element */
                                }

                                ep->taskEventTbl[i]->Events |= ep->eventTbl[i];

                                if (ep->taskEventTbl[i]->State == WAITING)
                                {
                                    Os_ChangeTaskStateByPtr(ep->taskEventTbl[i], READY);
                                }
                            }

                            for (i = 0; i < ep->numberOfTasks; i++)
                            {
                                if (ep->taskTbl[i]->NumberOfAct >= ep->taskTbl[i]->MaxAct)
                                {
                                    ERRORHOOK(E_OS_LIMIT);
                                    (*(ep->taskTbl[i]->pOverrun))++;
                                    returnValue = (E_OS_LIMIT);
                                    continue; /* Go to next loop element */
                                }

                                ep->taskTbl[i]->NumberOfAct++;

                                Os_ChangeTaskStateByPtr(ep->taskTbl[i], READY);
                            }

                            schedTbl->expiryPointNext++;
#ifdef OS_SCHEDULETABLE_EXPLICIT_SYNCHRONISATION
                            schedTbl->actEpCanMoved = OS_TRUE;
#endif
                        }
                    }

                    if (schedTbl->expiryPointNext >= schedTbl->schedTblConst->numberOfExpiryPoints)
                    {
                        /* all eps was executed */
                        /* the schedule table is executed completely */
                        if (schedTbl->ticks >= schedTbl->schedTblConst->duration )
                        {
                            /* reset all execution variables of schedule table */
                            schedTbl->ticks = OS_SCHEDULE_TABLE_TICK_DEFAULT;
                            schedTbl->expiryPointNext = 0;

                            /* if the schedule table is executed and repeating is false then stop the schedule table */
                            if (schedTbl->schedTblConst->repeating == OS_FALSE)
                            {
                                schedTbl->status = SCHEDULETABLE_STOPPED;

                                /* activate the next schedule table if possible */
                                if (schedTbl->nextSchedTbl != 0)
                                {
                                    nextSchedTbl = schedTbl->nextSchedTbl;
                                    schedTbl->nextSchedTbl = 0;

                                    nextSchedTbl->match =  g_CtrCb[CounterID].CtrValue + 1 ;
                                    nextSchedTbl->match =   nextSchedTbl->match % (g_CtrCb[CounterID].MaxAllowedValue + 1);

                                    /* check if the schedule table is implicit sychronised */
                                    if ((nextSchedTbl->schedTblConst->syncStrategy == OS_IMPLICIT_SYNCHRONISATION) && ( nextSchedTbl->match == 0) )
                                    {
                                        if ((g_CtrCb[CounterID].MaxAllowedValue + 1) == nextSchedTbl->schedTblConst->duration)
                                        {
                                            nextSchedTbl->status = SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS;
                                        }
                                    }
                                    else
                                    {
                                        nextSchedTbl->status = SCHEDULETABLE_RUNNING;
                                    }
                                }
                            }

#ifdef OS_SCHEDULETABLE_EXPLICIT_SYNCHRONISATION
                            schedTbl->deviation = 0;
                            schedTbl->actEpCanMoved = OS_TRUE;
#endif
                        }
                    }
                }
            }
        }
    }

    return returnValue;
}

#endif  /* NUMBER_OF_SCHEDULE_TABLES > 0 */

#endif /* OS_SCHEDTBL_C */

/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/
