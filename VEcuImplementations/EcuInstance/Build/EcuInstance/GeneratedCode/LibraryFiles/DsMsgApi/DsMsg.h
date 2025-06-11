/**************************************************************************************************\
*** DsMsg.h
***
*** dSPACE Standard message API for hardware platforms.
***
*** Copyright (c) 2018 by dSPACE GmbH, Paderborn, Germany.
*** All Rights Reserved.
\**************************************************************************************************/

#ifndef _DSMSG_H_
#define _DSMSG_H_

 /*------------------------------------------------------------------------------------------------*\
   INCLUDES
 \*------------------------------------------------------------------------------------------------*/

#include "Dstypes.h"
#include "DsMsgDef.h"

 /*------------------------------------------------------------------------------------------------*\
   TYPE DEFINITIONS
 \*------------------------------------------------------------------------------------------------*/

typedef enum
{
    MSG_MC_LOG = DSERR_CLASS_LOG,
    MSG_MC_INFO = DSERR_CLASS_TEXT,
    MSG_MC_WARNING = DSERR_CLASS_WARN,
    MSG_MC_ERROR = DSERR_CLASS_ERROR
} msg_class_type;

typedef enum
{
    MSG_DLG_NONE = DSERR_DLG_NOBOX,
    MSG_DLG_OKCANCEL = DSERR_DLG_OKCAN,
    MSG_DLG_DEFAULT = 0xFFFF
} msg_dialog_type;

typedef Int32 msg_no_type;
typedef UInt32 msg_submodule_type;
typedef UInt32 DsTError;

/*------------------------------------------------------------------------------------------------*\
  FUNCTION DECLARATIONS
\*------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

    /* message printf */
    extern int msg_printf(msg_class_type msg_class, msg_dialog_type msg_dialog, msg_submodule_type submodule, msg_no_type no, const char* format, ...);

    /* write an error message using the printf format */
    extern int msg_error_printf(msg_submodule_type submodule, msg_no_type no, const char* format, ...);

    /* write an info message using the printf format */
    extern int msg_info_printf(msg_submodule_type submodule, msg_no_type no, const char* format, ...);

    /* write a warning message using the printf format */
    extern int msg_warning_printf(msg_submodule_type submodule, msg_no_type no, const char* format, ...);

    /* change the default dialog of a message class */
    extern void msg_default_dialog_set(msg_class_type msg_class, msg_dialog_type msg_dialog);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* !_DSMSG_H_ */

/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/
