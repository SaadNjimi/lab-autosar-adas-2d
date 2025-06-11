/**************************************************************************************************\
 *** Os_ErrorIds.h
 ***
 *** Definition of OSEK error codes and parameters.
 ***
 *** Copyright (c) 2009 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef OS_ERROR_IDS_H
#define OS_ERROR_IDS_H

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/

#include "Platform_Types.h"

/*------------------------------------------------------------------------------------------------*\
  DEFINES
\*------------------------------------------------------------------------------------------------*/

#define NOT_KNOWN 0xFFFFu

/* The OSEK error consists of two parts:
   The highest 4 bits describe the calling level.
   The lowest 12 bits describe the error number.
  */
#define IN_TASK         0x1000u
#define IN_OS           0x2000u
#define IN_ISR          0x3000u
#define IN_STARTUPHOOK  0x4000u
#define IN_SHUTDOWNHOOK 0x5000u
#define IN_ERRORHOOK    0x6000u
#define IN_PRETASKHOOK  0x7000u
#define IN_POSTTASKHOOK 0x8000u

#define NO_OSEK_ERROR             0x0000u
        /* 0x0000u is reserved for OK response */

#define INTERNAL_FATAL_OSEK_ERROR 0x0001u
        /* An internal error occured in the OSEK scheduler.
           - The file number, the error occured in is <par1>
           - The location of error occurance is <par2>
           - The OSServiceId is <par3>*/

#define ACTIVATETASK_FAILED       0x0002u
        /* ActivateTask caused an error.
           - The task to be activated is <par1>*/

#define TERMINATETASK_FAILED      0x0003u
        /* TerminateTask caused an error.
           - no additional parameter */

#define CHAINTASK_FAILED          0x0004u
        /* ChainTask caused an error.
           - The task to be activated is <par1>*/

#define SCHEDULE_FAILED           0x0005u
        /* Schedule caused an error.
           - no additional parameter */

#define GETTASKSTATE_FAILED       0x0006u
        /* GetTaskState caused an error.
           - The task, which state should be read is <par1> */

#define GETRESOURCE_FAILED        0x0007u
        /* GetResource caused an error.
           - The resource, which should be locked is <par1> */

#define RELEASERERSOURCE_FAILED   0x0008u
        /* ReleaseResource caused an error.
           - The resource, which should be released is <par1> */

#define SETEVENT_FAILED           0x0009u
        /* SetEvent caused an error.
           - The event mask, which should be set is <par1>
           - The task, which owns the event is <par2> */

#define CLEAREVENT_FAILED         0x000Au
        /* ClearEvent caused an error.
           - The event mask, which should be cleared is <par1> */

#define GETEVENT_FAILED           0x000Bu
        /* GetEvent caused an error.
           - The task, which events should be read is <par1> */

#define WAITEVENT_FAILED          0x000Cu
        /* WaitEvent caused an error.
           - The event mask, the calling task should wait for is <par1> */

#define GETALARMBASE_FAILED       0x000Du
        /* GetAlarmBase caused an error.
           - The alarm, which alarm base should be read is <par1> */

#define GETALARM_FAILED           0x000Eu
        /* GetAlarm caused an error.
           - The alarm, which remaining ticks should be read is <par1> */

#define SETRELALARM_FAILED        0x000Fu
        /* SetRelAlarm caused an error.
           - The alarm, which should be started is <par1>
           - The cycle value used to start the alarm is <par2>
           - The increment value used to start the alarm is <par3> */

#define SETABSALARM_FAILED        0x0010u
        /* SetAbsAlarm caused an error.
           - The alarm, which should be started is <par1>
           - The cycle value used to start the alarm is <par2>
           - The start value used to start the alarm is <par3> */

#define CANCELALARM_FAILED        0x0011u
        /* CancelAlarm caused an error.
           - The alarm, which should be canceled is <par1> */

#define SENDMESSAGE_FAILED        0x0012u
        /* SendMessage caused an error.
           - The message which should be sent is <par1> */

#define RECEIVEMESSAGE_FAILED     0x0013u
        /* ReceiveMessage caused an error.
           - The message which should be received is <par1> */

#define INITCOUNTER_FAILED        0x0014u
#define COUNTERTRIGGER_FAILED     0x0015u
#define GETCOUNTERVALUE_FAILED    0x0016u

/*------------------------------------------------------------------------------------------------*\
  TYPEDEFS
\*------------------------------------------------------------------------------------------------*/

#endif /* OS_ERROR_IDS_H */

/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/

