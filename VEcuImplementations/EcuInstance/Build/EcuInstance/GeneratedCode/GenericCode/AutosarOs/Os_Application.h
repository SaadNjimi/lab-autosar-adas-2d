/**************************************************************************************************\
 *** Os_Application.h
 ***
 *** OSEK API function definition for OS Application handling.
 ***
 *** Copyright (c) 2018 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef OS_APPLICATION_H
#define OS_APPLICATION_H

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/
#include "Os_Types.h"
#include "Os.h"

/*------------------------------------------------------------------------------------------------*\
  FORWARD DECLARATIONS
\*------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

extern StatusType Os_TerminateApplicationInternal( ApplicationType ApplicationID, RestartType RestartOption, StatusType Error );
extern void Os_ApplicationErrorHookFilter( StatusType Error );

#ifdef __cplusplus
}
#endif

#endif /* OS_APPLICATION_H */

/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/
