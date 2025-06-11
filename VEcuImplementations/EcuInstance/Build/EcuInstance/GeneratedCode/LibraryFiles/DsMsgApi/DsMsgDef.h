/**************************************************************************************************\
*** DsMsgDef.h
***
*** Extract of dSPACE header file for message API for hardware platforms containing
*** required global definitions for message module.
***
*** Copyright (c) 2018 by dSPACE GmbH, Paderborn, Germany.
*** All Rights Reserved.
\**************************************************************************************************/

#ifndef _DSMSGDEF_H_
#define _DSMSGDEF_H_

#define MSG_SM_NONE 0 /* No Module */

enum DSERR_ClassEnum
{
    /* The message is only for support and development. It will be written
    to the log file, only*/
    DSERR_CLASS_LOG,
    /* The message is a state/text message. It will be written
    to the log file and the log viewer*/
    DSERR_CLASS_TEXT,
    /* The message is a warning message. It will be written
    to the log file and the log viewer. It is formated as warning*/
    DSERR_CLASS_WARN,
    /* The message is a error message. It will be written
    to the log file and the log viewer. It is formated as error*/
    DSERR_CLASS_ERROR,
    /* The message is a severe error message. It will be written
    to the log file and the log viewer. It is formated as severe error*/
    DSERR_CLASS_SEVERE,
    /* The message is a system error message. It will be written
    to the log file and the log viewer. It is formated as system error.*/
    DSERR_CLASS_SYSTEM
};

/* This enum lays down wether and how the message is dislayed in an messagebox.
there is no dependancy to DSERR_ClassEnum*/
enum DSERR_DialogEnum
{
    /*No message box will be displayed */
    DSERR_DLG_NOBOX,
    /*A message box with an OK button will be displayed */
    DSERR_DLG_OK,
    /*A message box with an OK and a Canncel button will be displayed */
    DSERR_DLG_OKCAN,
    /*A message box with a Yes and a No button will be displayed */
    DSERR_DLG_YESNO,
    /*A message box with a Yes, No and Canncel button will be displayed */
    DSERR_DLG_YESNOCAN,
    /*A message box with a Retry, Ignore and Abort button will be displayed */
    DSERR_DLG_RETIGNAB,
    /*A message box with a Close button will be displayed */
    DSERR_DLG_CLOSE,
    /*A message box with a Retry and a Cancel button will be displayed */
    DSERR_DLG_RETRYCANCEL
};

#endif /* _DSMSGDEF_H_ */
