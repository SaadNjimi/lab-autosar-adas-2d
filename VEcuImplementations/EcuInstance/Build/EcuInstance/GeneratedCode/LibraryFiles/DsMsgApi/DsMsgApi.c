/**************************************************************************************************\
*** DsMsgApi.c
***
*** Implementation of message API
***
*** Copyright 2019 by dSPACE GmbH, Paderborn, Germany.
*** All Rights Reserved.
\**************************************************************************************************/

#ifndef DSMSGAPI_C
#define DSMSGAPI_C

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/

#include "DsMsg.h"
#include "VEOS.h"
#include "stdarg.h"

/*------------------------------------------------------------------------------------------------*\
  PARAMETERIZED MACROS
\*------------------------------------------------------------------------------------------------*/

#define MSG_API_UNUSED_PARAM(Param) (void)(Param)

/*------------------------------------------------------------------------------------------------*\
  FUNCTION DEFINITIONS
\*------------------------------------------------------------------------------------------------*/

/* message printf */
int msg_printf(msg_class_type msg_class, msg_dialog_type msg_dialog, msg_submodule_type submodule, msg_no_type no, const char *format, ...)
{
    VEOS_MsgApi_SeverityType severity;
    va_list list;

    MSG_API_UNUSED_PARAM(msg_dialog);
    MSG_API_UNUSED_PARAM(submodule);
    MSG_API_UNUSED_PARAM(no);

    VEOS_boolean display = VEOS_TRUE;

    switch (msg_class)
    {
        case MSG_MC_ERROR: {
            severity = VEOS_MsgApi_SeverityType_Error;
            break;
        }
        case MSG_MC_INFO: {
            severity = VEOS_MsgApi_SeverityType_Info;
            break;
        }
        case MSG_MC_LOG: {
            severity = VEOS_MsgApi_SeverityType_Info;
            display = VEOS_FALSE;
            break;
        }
        case MSG_MC_WARNING: {
            severity = VEOS_MsgApi_SeverityType_Warning;
            break;
        }
        default:
        {
            severity = VEOS_MsgApi_SeverityType_Info;
        }
    }

    va_start(list, format);
    (void)VEOS_MsgApi_ShowMessageVarArgs(severity, display, format, list);
    va_end(list);

    return 0;
}

/* write an error message using the printf format */
int msg_error_printf(msg_submodule_type submodule, msg_no_type no, const char *format, ...)
{
    va_list list;

    MSG_API_UNUSED_PARAM(submodule);
    MSG_API_UNUSED_PARAM(no);

    va_start(list, format);
    (void)VEOS_MsgApi_ShowMessageVarArgs(VEOS_MsgApi_SeverityType_Error, VEOS_TRUE, format, list);
    va_end(list);

    return 0;
}

/* write an info message using the printf format */
int msg_info_printf(msg_submodule_type submodule, msg_no_type no, const char *format, ...)
{
    va_list list;

    MSG_API_UNUSED_PARAM(submodule);
    MSG_API_UNUSED_PARAM(no);

    va_start(list, format);
    (void)VEOS_MsgApi_ShowMessageVarArgs(VEOS_MsgApi_SeverityType_Info, VEOS_TRUE, format, list);
    va_end(list);

    return 0;
}

/* write a warning message using the printf format */
int msg_warning_printf(msg_submodule_type submodule, msg_no_type no, const char *format, ...)
{
    va_list list;

    MSG_API_UNUSED_PARAM(submodule);
    MSG_API_UNUSED_PARAM(no);

    va_start(list, format);
    (void)VEOS_MsgApi_ShowMessageVarArgs(VEOS_MsgApi_SeverityType_Warning, VEOS_TRUE, format, list);
    va_end(list);

    return 0;
}

/* change the default dialog of a message class */
void msg_default_dialog_set(msg_class_type msg_class, msg_dialog_type msg_dialog)
{
    MSG_API_UNUSED_PARAM(msg_class);
    MSG_API_UNUSED_PARAM(msg_dialog);
}

#endif /* DSMSGAPI_C */

/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/
