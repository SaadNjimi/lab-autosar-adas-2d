/**************************************************************************************************\
 *** Os_Spinlock.c
 ***
 *** OSEK API function definition for OS spinlock handling.
 ***
 *** Copyright (c) 2018 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef OS_SPINLOCK_C
#define OS_SPINLOCK_C

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/
#include "Os_SimulationCore.h"
#include "Os_Kernel.h"

#include "Os_EcuCore.h"
#include "Os_Application.h"
#include "Os_Task.h"
#include "Os_ResInt.h"
#include "Os_Spinlock.h"

/*------------------------------------------------------------------------------------------------*\
  FUNCTION DEFINITIONS
\*------------------------------------------------------------------------------------------------*/

#if ((NUMBER_OF_SPINLOCKS > 0) && (NUMBER_OF_CORES > 0))
static Os_BoolType Os_SpinlockIsInAnAllowedNestedOrder( SpinlockIdType currenSpinlockID, SpinlockIdType nextSpinlockID );
static void Os_PushSpinlockId(CoreIdType currentCoreID, SpinlockIdType SpinlockID);
#endif

/**************************************************************************************************\
 *** FUNCTION:
 ***    GetSpinlock
 ***
 *** DESCRIPTION:
 ***    GetSpinlock tries to occupy a spin-lock variable. If the function returns,
 ***    either the lock is successfully taken or an error has occurred. The spinlock mechanism
 ***    is an active polling mechanism. The function does not cause a de-scheduling.
 ***    [SWS_Os_00686]
 ***
 *** PARAMETERS:
 ***     Type                 Name          Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     SpinlockIdType    SpinlockID     The ID of the spinlock
 ***
 *** RETURNS:
 ***    E_OK                          In standard and extended status : No Error
 ***    E_OS_ID                       The SpinlockId is invalid
 ***    E_OS_INTERFERENCE_DEADLOCK    A TASK tries to occupy the spinlock while the lock is already occupied by a TASK on the same core. This would cause a deadlock.
 ***    E_OS_NESTING_DEADLOCK         A TASK tries to occupy the spinlock while a TASK on the same core is holding a different spinlock in a way that may cause a deadlock.
 ***    E_OS_ACCESS                   The spinlock cannot be accessed.
 ***
\**************************************************************************************************/
StatusType GetSpinlock( SpinlockIdType SpinlockID )
{
    g_ErrorInfo.ServiceID = OSServiceId_GetSpinlock;
    g_ErrorInfo.Service.SvGetSpinlock.SpinlockID = SpinlockID;

#if ((NUMBER_OF_SPINLOCKS > 0) && (NUMBER_OF_CORES > 0))
    {
        ApplicationType currentApplicationID;
        CoreIdType currentCoreID;

        /* [SWS_Os_00689] The function GetSpinlock shall return E_OS_ID if the parameter SpinlockID refers to a spinlock that does not exist. */
        if(SpinlockID >= NUMBER_OF_SPINLOCKS)
        {
            ERRORHOOK(E_OS_ID);
            return E_OS_ID;
        }

        currentApplicationID = g_TaskCb[g_Kernel.ActTaskId].ConnectApplicationId;
        if(currentApplicationID != OS_NOT_CONNECTED_TO_ANY_APPLICATION)
        {
            /* [SWS_Os_00692] The function GetSpinlock shall return E_OS_ACCESS if the accessing OS-Application was not listed in the configuration (OsSpinlock). */
            if (g_SpinlockCb[SpinlockID].ApplicationHasAccess[currentApplicationID] == OS_FALSE)
            {
                ERRORHOOK(E_OS_ACCESS);
                return E_OS_ACCESS;
            }
        }

        currentCoreID = GetCoreID();
        if(g_SpinlockCb[SpinlockID].IsLockedByCore == OS_NOT_LOCKED_BY_ANY_CORE)  /* true, if spinlock is not already occupied? */
        {
            /* has this core already occupied a spinlock? */
            if( Os_GetCurentSpinlockId(currentCoreID) != OS_NOT_LOCKED_BY_ANY_SPINLOCK )
            {
                /* [SWS_Os_00660] A unique order in which multiple spinlocks can be occupied by a TASK/ISR2 on one core should be configurable in the AUTOSAR Operating System. This might be realized by the configuration item (OsSpinlockSuccessor{NEXT_SPINLOCK}) where NEXT_SPINLOCK refers to the consecutive spinlock.*/
                if (Os_SpinlockIsInAnAllowedNestedOrder(Os_GetCurentSpinlockId(currentCoreID), SpinlockID) == OS_TRUE)
                {
                    /* spinlock can be occupied */
                }
                else
                {
                    /* Error, to avoid deadlocks it is not allowed to nest different spinlocks.
                       Optionally if spinlocks shall be nested, a unique order has to be defined.
                       Spinlocks can only be taken in this order whereas it is allowed to skip individual spinlocks.  */
                    /* [SWS_Os_00661] The AUTOSAR Operating System shall generate an error if a TASK/ISR2 on a core, where the same or a different TASK/ISR already holds a spinlock, tries to seize another spinlock that has not been configured as a direct or indirect successor of the latest acquired spinlock (by means of the OsSpinlockSuccessor configuration parameter) or if no successor is configured. */
                    ERRORHOOK(E_OS_NESTING_DEADLOCK);
                    return E_OS_NESTING_DEADLOCK;
                }
            }

            /* [SWS_Os_00688] The function GetSpinlock shall return E_OK if no error was detected. The spinlock is now occupied by the calling TASK/ISR2 on the calling core. */
            switch(g_SpinlockCb[SpinlockID].LockMethode)
            {
                case LOCK_ALL_INTERRUPTS:
                    SuspendAllInterrupts();
                break;
                case LOCK_CAT2_INTERRUPTS:
                    SuspendOSInterrupts();
                break;
                case LOCK_WITH_RES_SCHEDULER:
                    GetResource(RES_SCHEDULER);
                break;
                case LOCK_NOTHING:
                break;
            }
            g_SpinlockCb[SpinlockID].IsLockedByCore = currentCoreID;
            g_SpinlockCb[SpinlockID].OccupiedByTaskId = g_Kernel.ActTaskId;
            Os_PushSpinlockId(currentCoreID, SpinlockID);

            return E_OK;
        }
        else
        {   /*--- the spinlock is already occupied? ---*/

            /* [SWS_Os_00658] The AUTOSAR Operating System shall generate an error if a TASK tries to occupy a spinlock that is assigned to a TASK/ISR2 on the same core. */
            /* [SWS_Os_00690] The function GetSpinlock shall return E_OS_INTERFERENCE_DEADLOCK if the spinlock referred by the parameter SpinlockID is already occupied by a TASK/ISR2 on the same core. */
            if(g_SpinlockCb[SpinlockID].IsLockedByCore == currentCoreID)
            {
                /* same core and same spinlock ID */
                ERRORHOOK(E_OS_INTERFERENCE_DEADLOCK);
                return E_OS_INTERFERENCE_DEADLOCK;
            }
            else
            {
                /* has this core already occupied a spinlock? */
                if(Os_GetCurentSpinlockId(currentCoreID) != OS_NOT_LOCKED_BY_ANY_SPINLOCK)
                {
                    /* [SWS_Os_00691] The function GetSpinlock shall return E_OS_NESTING_DEADLOCK if the sequence by which multiple spinlocks are occupied at the same time on one core do not comply with the configured order. */
                    if(Os_SpinlockIsInAnAllowedNestedOrder(Os_GetCurentSpinlockId(currentCoreID), SpinlockID) == OS_FALSE)
                    {
                        /* error, if the SuccessorId is not already occupied  */
                        ERRORHOOK(E_OS_NESTING_DEADLOCK);
                        return E_OS_NESTING_DEADLOCK;
                    }
                }
            }

            /* [SWS_Os_00649] The AUTOSAR Operating System shall provide a GetSpinlock function which occupies a spinlock. If the spinlock is already occupied, GetSpinlock shall keep on trying to occupy the spinlock until it succeeds. */
            /* [SWS_Os_00687] The function GetSpinlock shall occupy a spinlock. If the spinlock is already occupied the function shall busy wait until the spinlock becomes available. */
            while(g_SpinlockCb[SpinlockID].IsLockedByCore != OS_NOT_LOCKED_BY_ANY_CORE)
            {
                /* This may occurs only during a VEOS simulation */
                Os_SimulationCoreShowWarningMessage("The spinlock with the ID=%d is already occupied. The Spinlock is ignored, which may lead to a different simulation behavior.\n", SpinlockID);
                return E_OK;
            }
        }

        return E_OK;
    }
#else

    ERRORHOOK(E_OS_ID);
    return E_OS_ID;
#endif /* (NUMBER_OF_SPINLOCKS > 0) */
}

/**************************************************************************************************\
 *** FUNCTION:
 ***    ReleaseSpinlock
 ***
 *** DESCRIPTION:
 ***    ReleaseSpinlock releases a spinlock variable that was occupied before.
 ***    Before terminating a TASK all spinlock variables that have been occupied with
 ***    GetSpinlock() shall be released. Before calling WaitEVENT all Spinlocks shall be released.
 ***    [SWS_Os_00695]
 ***
 *** PARAMETERS:
 ***     Type                 Name          Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     SpinlockIdType    SpinlockID     The ID of the spinlock
 ***
 *** RETURNS:
 ***    E_OK            In standard and extended status : No Error
 ***    E_OS_ID         The SpinlockId is invalid
 ***    E_OS_STATE      The Spinlock is not occupied by the TASK
 ***    E_OS_ACCESS     The Spinlock cannot be accessed.
 ***    E_OS_NOFUNC     Attempt to release a spinlock while another spinlock has to be released before.
 ***
\**************************************************************************************************/
StatusType ReleaseSpinlock( SpinlockIdType SpinlockID )
{
    g_ErrorInfo.ServiceID = OSServiceId_ReleaseSpinlock;
    g_ErrorInfo.Service.SvReleaseSpinlock.SpinlockID = SpinlockID;

#if ((NUMBER_OF_SPINLOCKS > 0) && (NUMBER_OF_CORES > 0))
    {
        ApplicationType currentApplicationID;

        /* [SWS_Os_00698] The function ReleaseSpinlock shall return E_OS_ID if the parameter SpinlockID refers to a spinlock that does not exist. */
        if(SpinlockID >= NUMBER_OF_SPINLOCKS)
        {
            ERRORHOOK(E_OS_ID);
            return E_OS_ID;
        }

        currentApplicationID = g_TaskCb[g_Kernel.ActTaskId].ConnectApplicationId;
        if(currentApplicationID != OS_NOT_CONNECTED_TO_ANY_APPLICATION)
        {
            /* [SWS_Os_00700] The function ReleaseSpinlock shall return E_OS_ACCESS if the TASK has no access to the spinlock referred by the parameter SpinlockID. */
            if (g_SpinlockCb[SpinlockID].ApplicationHasAccess[currentApplicationID] == OS_FALSE)
            {
                ERRORHOOK(E_OS_ACCESS);
                return E_OS_ACCESS;
            }
        }

        /* [SWS_Os_00655] The AUTOSAR Operating System shall provide a ReleaseSpinlock function which releases an occupied spinlock. If the spinlock is not occupied an error shall be returned. */
        if(g_SpinlockCb[SpinlockID].IsLockedByCore == OS_NOT_LOCKED_BY_ANY_CORE)
        {
            ERRORHOOK(E_OS_STATE);
            return E_OS_STATE;
        }

        /* [SWS_Os_00699] The function ReleaseSpinlock shall return E_OS_STATE if the parameter SpinlockID refers to a spinlock that is not occupied by the calling TASK. */
        if(g_SpinlockCb[SpinlockID].OccupiedByTaskId != g_Kernel.ActTaskId)
        {
            ERRORHOOK(E_OS_STATE);
            return E_OS_STATE;
        }
        else
        {
            CoreIdType currentCoreID;
            currentCoreID = GetCoreID();

            /* [SWS_Os_00701] The function ReleaseSpinlock shall return E_OS_NOFUNC if the TASK tries to release a spinlock while another spinlock has to be released before. No functionality shall be performed. */
            if( Os_GetCurentSpinlockId(currentCoreID) != SpinlockID)
            {
                /*  ERRORHOOK() is not called, because "No functionality shall be performed" */
                return E_OS_NOFUNC;
            }

            /* [SWS_Os_00696] The function ReleaseSpinlock shall release a spinlock that has been occupied by the same (calling) TASK. If the related GetSpinlock call used configured locks the function shall also perform the undo of the used lock. */
            /* [SWS_Os_00697] The function ReleaseSpinlock shall return E_OK if no error was detected. The spinlock is now free and can be occupied by the same or other TASKs. */
            switch(g_SpinlockCb[SpinlockID].LockMethode)
            {
                case LOCK_ALL_INTERRUPTS:
                    ResumeAllInterrupts();
                break;
                case LOCK_CAT2_INTERRUPTS:
                    ResumeOSInterrupts();
                break;
                case LOCK_WITH_RES_SCHEDULER:
                    ReleaseResource(RES_SCHEDULER);
                break;
                case LOCK_NOTHING:
                break;
            }
            g_SpinlockCb[SpinlockID].IsLockedByCore = OS_NOT_LOCKED_BY_ANY_CORE;
            g_SpinlockCb[SpinlockID].OccupiedByTaskId = OS_NOT_LOCKED_BY_ANY_TASKS;
            Os_PopSpinlockId(currentCoreID);
        }

        return E_OK;
    }
#else

    ERRORHOOK(E_OS_ID);
    return E_OS_ID;
#endif /* (NUMBER_OF_SPINLOCKS > 0) */
}

/**************************************************************************************************\
 *** FUNCTION:
 ***    TryToGetSpinlock
 ***
 *** DESCRIPTION:
 ***    TryToGetSpinlock has the same functionality as GetSpinlock with the difference that
 ***    if the spinlock is already occupied by a TASK on a different core the function sets
 ***    the OUT parameter "Success" and returns with E_OK.
 ***    [SWS_Os_00703]
 ***
 *** PARAMETERS:
 ***     Type                     Name          Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     SpinlockIdType        SpinlockID     The ID of the spinlock
 ***     TryToGetSpinlockType  Success        Returns if the lock has been occupied or not
 ***                                               TRYTOGETSPINLOCK_SUCCESS or TRYTOGETSPINLOCK_NOSUCCESS
 ***
 *** RETURNS:
 ***    E_OK                          In standard and extended status : No Error
 ***    E_OS_ID                       The SpinlockId is invalid
 ***    E_OS_INTERFERENCE_DEADLOCK    A TASK tries to occupy the spinlock while the lock is already occupied by a TASK on the same core. This would cause a deadlock.
 ***    E_OS_NESTING_DEADLOCK         A TASK tries to occupy the spinlock while a TASK on the same core is holding a different spinlock in a way that may cause a deadlock.
 ***    E_OS_ACCESS                   The spinlock cannot be accessed.
 ***
\**************************************************************************************************/
StatusType TryToGetSpinlock( SpinlockIdType SpinlockID, TryToGetSpinlockType* Success )
{
    g_ErrorInfo.ServiceID = OSServiceId_TryToGetSpinlock;
    g_ErrorInfo.Service.SvTryToGetSpinlock.SpinlockID = SpinlockID;

#if ((NUMBER_OF_SPINLOCKS > 0) && (NUMBER_OF_CORES > 0))
    {
        ApplicationType currentApplicationID;
        CoreIdType currentCoreID;

        /* [SWS_Os_00706] If the function TryToGetSpinlock does not return E_OK, the OUT parameter "Success" shall be undefined. */
        *Success = TRYTOGETSPINLOCK_NOSUCCESS;  /* Note: different from [SWS_Os_00706] the 'Success'is always set */

        /* [SWS_Os_00707] The function TryToGetSpinlock shall return E_OS_ID if the parameter SpinlockID refers to a spinlock that does not exist. */
        if(SpinlockID >= NUMBER_OF_SPINLOCKS)
        {
            ERRORHOOK(E_OS_ID);
            return E_OS_ID;
        }

        currentApplicationID = g_TaskCb[g_Kernel.ActTaskId].ConnectApplicationId;
        if(currentApplicationID != OS_NOT_CONNECTED_TO_ANY_APPLICATION)
        {
            /* [SWS_Os_00710] The function TryToGetSpinlock shall return E_OS_ACCESS if the TASK has no access to the spinlock referred by the parameter SpinlockID */
            if (g_SpinlockCb[SpinlockID].ApplicationHasAccess[currentApplicationID] == OS_FALSE)
            {
                ERRORHOOK(E_OS_ACCESS);
                return E_OS_ACCESS;
            }
        }

        currentCoreID = GetCoreID();
        if(g_SpinlockCb[SpinlockID].IsLockedByCore == OS_NOT_LOCKED_BY_ANY_CORE)   /* true, if spinlock is not already occupied? */
        {
            /* has this core already occupied a spinlock? */
            if(Os_GetCurentSpinlockId(currentCoreID) != OS_NOT_LOCKED_BY_ANY_SPINLOCK)
            {
                /* [SWS_Os_00660] A unique order in which multiple spinlocks can be occupied by a TASK/ISR2 on one core should be configurable in the AUTOSAR Operating System. This might be realized by the configuration item (OsSpinlockSuccessor{NEXT_SPINLOCK}) where NEXT_SPINLOCK refers to the consecutive spinlock. (See page 195) (SRS_Os_80018, SRS_Os_80021) */
                if (Os_SpinlockIsInAnAllowedNestedOrder(Os_GetCurentSpinlockId(currentCoreID), SpinlockID) == OS_TRUE)
                {
                    /* spinlock can be occupied */
                }
                else
                {
                    /* error, if the SuccessorId is not already occupied  */
                    /* [SWS_Os_00661] The AUTOSAR Operating System shall generate an error if a TASK/ISR2 on a core, where the same or a different TASK/ISR already holds a spinlock, tries to seize another spinlock that has not been configured as a direct or indirect successor of the latest acquired spinlock (by means of the OsSpinlockSuccessor configuration parameter) or if no successor is configured. (SRS_Os_80018, SRS_Os_80021) */
                    ERRORHOOK(E_OS_NESTING_DEADLOCK);
                    return E_OS_NESTING_DEADLOCK;
                }
            }

            /* [SWS_Os_00704] The function TryToGetSpinlock shall atomically test the availability of the spinlock and if available occupy it. The result of success is returned. */
            switch(g_SpinlockCb[SpinlockID].LockMethode)
            {
                case LOCK_ALL_INTERRUPTS:
                    SuspendAllInterrupts();
                break;
                case LOCK_CAT2_INTERRUPTS:
                    SuspendOSInterrupts();
                break;
                case LOCK_WITH_RES_SCHEDULER:
                    GetResource(RES_SCHEDULER);
                break;
                case LOCK_NOTHING:
                break;
            }
            g_SpinlockCb[SpinlockID].IsLockedByCore = currentCoreID;
            g_SpinlockCb[SpinlockID].OccupiedByTaskId = g_Kernel.ActTaskId;
            Os_PushSpinlockId(currentCoreID, SpinlockID);

            *Success = TRYTOGETSPINLOCK_SUCCESS;
            return E_OK;
        }
        else
        {   /*--- the spinlock is already occupied? ---*/

            /* [SWS_Os_00658] The AUTOSAR Operating System shall generate an error if a TASK tries to occupy a spinlock that is assigned to a TASK/ISR2 on the same core. */
            /* [SWS_Os_00708] The function TryToGetSpinlock shall return E_OS_INTERFERENCE_DEADLOCK if the spinlock referred by the parameter SpinlockID is already occupied by a TASK on the same core. */
            if(g_SpinlockCb[SpinlockID].IsLockedByCore == currentCoreID)
            {
                /* same core and same spinlock ID */
                ERRORHOOK(E_OS_INTERFERENCE_DEADLOCK);
                return E_OS_INTERFERENCE_DEADLOCK;
            }
            else
            {
                /* has this core already occupied a spinlock? */
                if(Os_GetCurentSpinlockId(currentCoreID) != OS_NOT_LOCKED_BY_ANY_SPINLOCK)
                {
                    /* [SWS_Os_00709] The function TryToGetSpinlock shall return E_OS_NESTING_DEADLOCK if a TASK tries to occupy a spinlock while holding a different spinlock in a way that may cause a deadlock. */
                     if(Os_SpinlockIsInAnAllowedNestedOrder(Os_GetCurentSpinlockId(currentCoreID), SpinlockID) == OS_FALSE)
                    {
                        /* error, if the SuccessorId is not already occupied  */
                        ERRORHOOK(E_OS_NESTING_DEADLOCK);
                        return E_OS_NESTING_DEADLOCK;
                    }
                }
            }

            /* [SWS_Os_00652] The AUTOSAR Operating System shall provide a TryToGetSpinlock function which occupies a spinlock. If the spinlock is already occupied by a TASK, TryToGetSpinlock shall return. */
            /* [SWS_Os_00705] The function TryToGetSpinlock shall set the OUT parameter Success to TRYTOGETSPINLOCK_SUCCESS if the spinlock was successfully occupied, and TRYTOGETSPINLOCK_NOSUCCESS if not. In both cases E_OK shall be returned. */
            *Success = TRYTOGETSPINLOCK_NOSUCCESS;
        }

        return E_OK;
    }
#else
    *Success = TRYTOGETSPINLOCK_NOSUCCESS;

    ERRORHOOK(E_OS_ID);
    return E_OS_ID;
#endif /* (NUMBER_OF_SPINLOCKS > 0) */
}

#if ((NUMBER_OF_SPINLOCKS > 0) && (NUMBER_OF_CORES > 0))

/*
  Returns the spinlock ID at the top of the stack or
  or 'OS_NOT_LOCKED_BY_ANY_SPINLOCK' if stack is empty
*/
SpinlockIdType Os_GetCurentSpinlockId(CoreIdType currentCoreID)
{
    return g_CoreCb[currentCoreID].OccupiedSpinlockStack[g_CoreCb[currentCoreID].actSpinlockIdx];
}

/*
  Returns OS_TRUE, the spinlock is in a defined unique order.
  Spinlocks can only be taken in this order whereas it is allowed to skip individual spinlocks.
  otherwise returns OS_FALSE
*/
static Os_BoolType Os_SpinlockIsInAnAllowedNestedOrder( SpinlockIdType currentSpinlockID, SpinlockIdType nextSpinlockID )
{
    SpinlockIdType tmpSpinlockID;
    if (nextSpinlockID == OS_INVALID_SPINLOCK)
    {
        return OS_FALSE;
    }
    else
    {
        tmpSpinlockID = currentSpinlockID;
        while( (tmpSpinlockID < NUMBER_OF_SPINLOCKS) && (g_SpinlockCb[tmpSpinlockID].SuccessorId != OS_INVALID_SPINLOCK) )
        {
            if (nextSpinlockID == g_SpinlockCb[tmpSpinlockID].SuccessorId)
            {
                return OS_TRUE;
            }
            tmpSpinlockID = g_SpinlockCb[tmpSpinlockID].SuccessorId;
        }
        return OS_FALSE;
    }
}

/*
  Add a spinlock ID onto the top of the stack.
*/
static void Os_PushSpinlockId(CoreIdType currentCoreID, SpinlockIdType spinlockID)
{
    if ( (g_CoreCb[currentCoreID].actSpinlockIdx == 0) &&
         (g_CoreCb[currentCoreID].OccupiedSpinlockStack[0]) == OS_NOT_LOCKED_BY_ANY_SPINLOCK )
    {
        g_CoreCb[currentCoreID].OccupiedSpinlockStack[0] = spinlockID;
    }
    else
    {
        if ( g_CoreCb[currentCoreID].actSpinlockIdx < (NUMBER_OF_SPINLOCKS-1) )
        {
            g_CoreCb[currentCoreID].actSpinlockIdx++;
            g_CoreCb[currentCoreID].OccupiedSpinlockStack[g_CoreCb[currentCoreID].actSpinlockIdx] = spinlockID;
        }
    }
}

/*
  Remove spinlock ID from the top of the stack.
*/
void Os_PopSpinlockId(CoreIdType currentCoreID)
{
    if (g_CoreCb[currentCoreID].actSpinlockIdx == 0)
    {
        g_CoreCb[currentCoreID].OccupiedSpinlockStack[0] = OS_NOT_LOCKED_BY_ANY_SPINLOCK;
    }
    else
    {
        if ( g_CoreCb[currentCoreID].actSpinlockIdx < (NUMBER_OF_SPINLOCKS) )
        {
            g_CoreCb[currentCoreID].OccupiedSpinlockStack[g_CoreCb[currentCoreID].actSpinlockIdx] = OS_NOT_LOCKED_BY_ANY_SPINLOCK;
            g_CoreCb[currentCoreID].actSpinlockIdx--;
        }
    }
}

#endif

#endif /* OS_SPINLOCK_C */

/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/
