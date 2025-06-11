/**************************************************************************************************\
 *** Os_Frame.h
 ***
 *** Operating system frame for the dSPACE Offline Simulator.
 ***
 *** Copyright (c) 2008 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef OS_FRAME_H
#define OS_FRAME_H

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/

#include "Os_Types.h"
#include "Os_Kernel.h"

/*------------------------------------------------------------------------------------------------*\
  FUNCTION PROTOTYPES
\*------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

extern void Os_PreInit(void);
extern void Os_Autostart(AppModeType AppMode, ApplicationType OsApplicationId);
extern void Os_IsrSystemTimer(void);

/* define in Os.c  */
extern void   Os_SendWarningOsKernelIsNotRunning(void);
extern void   Os_SendWarningAppModeIsNotDefined(void);
extern sint32 Os_FramePrepareStartOs(void);



#ifdef __cplusplus
}
#endif

#endif /* OS_FRAME_H */

/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/
