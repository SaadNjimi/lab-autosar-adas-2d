/**************************************************************************************************\
 *** Sab_MsgApi.c
 ***
 *** Implementation of the Simulator Abstraction layer with the Message service functions for VEOS.
 ***
 *** Copyright 2019 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef SAB_MSGAPI_C
#define SAB_MSGAPI_C

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/

#include "Sab.h"
#include "VEOS.h"
#include "VEcuAppFrm.h"

#if (SAB_DEV_ERROR_DETECT == STD_ON)
#include "Det.h"
#endif

#include <stdarg.h>
#include <stdio.h>

/*------------------------------------------------------------------------------------------------*\
  FORWARD DECLARATIONS
\*------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif
#if 0
}
#endif

static Sab_ReturnType Sab_Submit_Internal(Sab_MsgSeverity severity, const char* pFormat, va_list arguments);

#if 0
{
#endif
#ifdef __cplusplus
}
#endif


/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_Submit
 ***
 *** DESCRIPTION:
 ***     Submits a formatted message with severity Error (without opening a dialog).
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     Sab_MsgSeverity       severity     The message severity. This will specify the importance
 ***                                        of the submitted message (from log file up to error message)
 ***     const char *          format       The formatted message text - may contain simple format strings.
 ***     var args              ...          Optional formatter arguments.
 ***
 *** RETURNS:
 ***     Sab_ReturnType
 ***            SAB_E_OK            If the message has been submitted successfully.
 ***            SAB_E_NOT_OK        If the message has not been submitted or not submitted as desired.
\**************************************************************************************************/
Sab_ReturnType Sab_Submit(Sab_MsgSeverity severity, const char* pFormat, ...)
{
    va_list arguments;
    Sab_ReturnType success;
    va_start(arguments, pFormat);
    success = Sab_Submit_Internal(severity, pFormat, arguments);
    va_end(arguments);
    return success;
}


/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_SubmitLog
 ***
 *** DESCRIPTION:
 ***     Submits a formatted message to the simulator log file.
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     const char *          format       The formatted message text.
 ***     var args              ...          Optional formatter arguments.
 ***
 *** RETURNS:
 ***     Sab_ReturnType
 ***            SAB_E_OK            If the message has been submitted successfully.
 ***            SAB_E_NOT_OK        If the message has not been submitted or not submitted as desired.
\**************************************************************************************************/
Sab_ReturnType Sab_SubmitLog(const char* pFormat, ...)
{
    va_list arguments;
    Sab_ReturnType success;
    va_start(arguments, pFormat);
    success = Sab_Submit_Internal(Sab_MsgSeverity_Log, pFormat, arguments);
    va_end(arguments);
    return success;
}


/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_SubmitInfo
 ***
 *** DESCRIPTION:
 ***     Submits a formatted message with severity Info.
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     const char *          format       The formatted message text.
 ***     var args              ...          Optional formatter arguments.
 ***
 *** RETURNS:
 ***     Sab_ReturnType
 ***            SAB_E_OK            If the message has been submitted successfully.
 ***            SAB_E_NOT_OK        If the message has not been submitted or not submitted as desired.
\**************************************************************************************************/
Sab_ReturnType Sab_SubmitInfo(const char* pFormat, ...)
{
    va_list arguments;
    Sab_ReturnType success;
    va_start(arguments, pFormat);
    success = Sab_Submit_Internal(Sab_MsgSeverity_Info, pFormat, arguments);
    va_end(arguments);
    return success;
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_SubmitWarning
 ***
 *** DESCRIPTION:
 ***     Submits a formatted message with severity Warning (without opening a dialog).
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     const char *          format       The formatted message text.
 ***     var args              ...          Optional formatter arguments.
 ***
 *** RETURNS:
 ***     Sab_ReturnType
 ***            SAB_E_OK            If the message has been submitted successfully.
 ***            SAB_E_NOT_OK        If the message has not been submitted or not submitted as desired.
\**************************************************************************************************/
Sab_ReturnType Sab_SubmitWarning(const char* pFormat, ...)
{
    va_list arguments;
    Sab_ReturnType success;
    va_start(arguments, pFormat);
    success = Sab_Submit_Internal(Sab_MsgSeverity_Warning, pFormat, arguments);
    va_end(arguments);
    return success;
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_SubmitError
 ***
 *** DESCRIPTION:
 ***     Submits a formatted message with severity Error (without opening a dialog).
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     const char *          format       The formatted message text.
 ***     var args              ...          Optional formatter arguments.
 ***
 *** RETURNS:
 ***     Sab_ReturnType
 ***            SAB_E_OK            If the message has been submitted successfully.
 ***            SAB_E_NOT_OK        If the message has not been submitted or not submitted as desired.
\**************************************************************************************************/
Sab_ReturnType Sab_SubmitError(const char* pFormat, ...)
{
    va_list arguments;
    Sab_ReturnType success;
    va_start(arguments, pFormat);
    success = Sab_Submit_Internal(Sab_MsgSeverity_Error, pFormat, arguments);
    va_end(arguments);
    return success;
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_Submit
 ***
 *** DESCRIPTION:
 ***     Submits a formatted message with the given severity (without opening a dialog).
 ***
 *** PARAMETERS:
 ***     Type                       Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     VEOS_MsgApi_SeverityType   severity     The message severity. This will specify the importance
 ***                                             of the submitted message (from log file up to error message)
 ***     const char *               pFormat      The formatted message text - may contain simple format strings.
 ***     va_list                    arguments    Optional formatter arguments.
 ***
 *** RETURNS:
 ***     Sab_ReturnType
 ***            SAB_E_OK            If the message has been submitted successfully.
 ***            SAB_E_NOT_OK        If the message has not been submitted or not submitted as desired.
\**************************************************************************************************/
static Sab_ReturnType Sab_Submit_Internal(Sab_MsgSeverity severity, const char* pFormat, va_list arguments)
{
    VEOS_MsgApi_SeverityType messageType;
    Sab_ReturnType success = SAB_E_OK;
    VEOS_ApiSuccessType result;

    VEOS_boolean display = VEOS_TRUE;

    switch (severity)
    {
    case Sab_MsgSeverity_Warning:
        {
            messageType = VEOS_MsgApi_SeverityType_Warning;
        }

        break;
    case Sab_MsgSeverity_Error:
        {
            messageType = VEOS_MsgApi_SeverityType_Error;
        }

        break;
    case Sab_MsgSeverity_Info:
        {
            messageType = VEOS_MsgApi_SeverityType_Info;
        }

        break;
    case Sab_MsgSeverity_Log:
        {
            messageType = VEOS_MsgApi_SeverityType_Info;
            display = VEOS_FALSE;
        }

        break;
    default:
        {
            return SAB_E_NOT_OK;
        }
    }

    result = VEOS_MsgApi_ShowMessageVarArgs(messageType, display, pFormat, arguments);
    if (VEOS_E_OK != result)
    {
        success = SAB_E_NOT_OK;
    }
    if (messageType == VEOS_MsgApi_SeverityType_Error)
    {
        VEOS_SimApi_TerminateSimulation(VEOS_SimApi_TerminationKindType_StopSimulationAndFail);
    }

    return success;
}





/**************************************************************************************************\
 *** obsolete Sab functions
\**************************************************************************************************/

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_SubmitMsg
 ***
 *** DESCRIPTION:
 ***     Submits a formatted message with the given severity (without opening a dialog).
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     Sab_MsgSeverity       severity     The message severity. This will specify the importance
 ***                                        of the submitted message (from log file up to error message)
 ***     uint32                moduleNumber The number of the basic software module calling this.
 ***                                        (Application software components should use zero.)
 ***     sint32                msgNumber    A message code (optional).
 ***     const char *          format       The formatted message text.
 ***
 *** RETURNS:
 ***     Std_ReturnType
 ***            E_OK            If the message has been submitted successfully.
 ***            E_NOT_OK        If the message has not been submitted or not submitted as desired.
\**************************************************************************************************/
Std_ReturnType Sab_SubmitMsg(Sab_MsgSeverity severity, uint32 moduleNumber, sint32 messageNumber, const char *format)
{
    VEOS_MsgApi_SeverityType messageType = VEOS_MsgApi_SeverityType_Info;
    Std_ReturnType success = E_OK;
    VEOS_ApiSuccessType result;
    /* TODO consider message number and module number */

    /* to avoid the compiler warning C4100 */
    (void)(moduleNumber);
    (void)(messageNumber);

    VEOS_boolean display = VEOS_TRUE;

    switch (severity)
    {
        case Sab_MsgSeverity_Log:
        {
            messageType = VEOS_MsgApi_SeverityType_Info;
            display = VEOS_FALSE;
        }

        break;
        case Sab_MsgSeverity_Info:
        {
            messageType = VEOS_MsgApi_SeverityType_Info;
        }

        break;
        case Sab_MsgSeverity_Warning:
        {
            messageType = VEOS_MsgApi_SeverityType_Warning;
        }

        break;
        case Sab_MsgSeverity_Error:
        {
            messageType = VEOS_MsgApi_SeverityType_Error;
        }

        break;
    }

    result = VEOS_MsgApi_ShowMessage(messageType, display, format);
    if (VEOS_E_OK != result)
    {
        success = E_NOT_OK;
    }
    if (messageType == VEOS_MsgApi_SeverityType_Error)
    {
        VEOS_SimApi_TerminateSimulation(VEOS_SimApi_TerminationKindType_StopSimulationAndFail);
    }

    return success;
}



#endif /* SAB_MSGAPI_C */

/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/
