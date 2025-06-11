/**************************************************************************************************\
 *** Sab_Veos.h
 ***
 *** Simulator Abstraction interface header file for VEOS internal purposes only.
 ***
 *** Copyright 2019 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef SAB_VEOS_H
#define SAB_VEOS_H

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/

#include "Sab.h"

/*------------------------------------------------------------------------------------------------*\
  FUNCTION PROTOTYPES
\*------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif
#if 0
}
#endif

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_VpuTriggered
 ***
 *** DESCRIPTION:
 ***     Called when the VPU has been triggered and the timestamp has been incremented.
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
extern void Sab_VpuTriggered(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_EventTriggered
 ***
 *** DESCRIPTION:
 ***     Called when the VPU has received an event which is not associated with a cyclic timer
 ***     and the timestamp has been incremented.
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
extern void Sab_EventTriggered(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_EventFinished
 ***
 *** DESCRIPTION:
 ***     Called when the VPU received event is finished.
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
extern void Sab_EventFinished(void);

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
extern void Sab_OnSimulationStopped(void);

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
extern boolean Sab_CanStart(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_OnOsStarted
 ***
 *** DESCRIPTION:
 ***     Callout for the implementation of the operating system to signal the start of the OS .
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
extern void Sab_OnOsStarted(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_OnOsShutdown
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
extern void Sab_OnOsShutdown(void);

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
extern void Sab_ApplicationStateEntry(Sab_ApplicationState state);

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
extern void Sab_ApplicationStateExit(Sab_ApplicationState state);


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
extern void Sab_CheckInitialized(uint8 functionType);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_Call_PreStartUp_Functions
 ***
 *** DESCRIPTION:
 ***     Call Pre-Start-up functions for the virtual ECUs.
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
extern void Sab_Call_PreStartUp_Functions(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_Call_PostShutDown_Functions
 ***
 *** DESCRIPTION:
 ***     Call Post-Shut-Down functions for the virtual ECUs.
 ***
 *** PARAMETERS:
***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
extern void Sab_Call_PostShutDown_Functions(void);


#if 0
{
#endif
#ifdef __cplusplus
}
#endif

#endif /* SAB_VEOS_H */

/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/
