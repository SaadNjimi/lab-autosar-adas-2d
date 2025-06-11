/**************************************************************************************************\
 *** VEOS_DbgEh.c
 ***
 *** VEOS frame debugging support and error handler definition file.
 ***
 *** Copyright 2019 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef VEOS_DBGEH_H
#define VEOS_DBGEH_H

/*------------------------------------------------------------------------------------------------*\
  DEFINES (OPT)
\*------------------------------------------------------------------------------------------------*/

/* FIXME: Optional speed up by defining check macros as empty. */

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/

#include "VEOS_Types.h"
#include "VEOS_Util.h"

/*------------------------------------------------------------------------------------------------*\
  DEFINES
\*------------------------------------------------------------------------------------------------*/
#ifndef VEOS_DEBUG
#define VEOS_SIGNALERROR_INIT                               VEOS_FALSE
#else
#define VEOS_SIGNALERROR_INIT                               VEOS_TRUE
#endif /* VEOS_DEBUG */

/*------------------------------------------------------------------------------------------------*\
  PARAMETERIZED MACROS
\*------------------------------------------------------------------------------------------------*/

/*
 * Parameterized macro to consume a parameter which is not used anywhere in the function.
 * Param: The current parameter value.
 */
#define VEOS_DBG_UNUSED_PARAM(Param)												(void)(Param)

#ifdef VEOS_DEBUG
/*
 * Parameterized macro to track the start of a VEOS frame function.
 * ApiName: Name of the API currently used
 * MethodName: Name of the method
 */
#define VEOS_FRM_DBG_METHOD_START(ApiName, MethodName)                              (void)VEOS_MsgApi_LogMessage(ApiName, MethodName, "Start of function.")
/*
 * Parameterized macro to track the end of a VEOS frame function.
 * ApiName: Name of the API currently used
 * MethodName: Name of the method
 */
#define VEOS_FRM_DBG_METHOD_END(ApiName, MethodName)                                (void)VEOS_MsgApi_LogMessage(ApiName, MethodName, "End of function.")
/*
 * Parameterized macro to track the value of a parameter passed to a VEOS frame function.
 * ApiName: Name of the API currently used
 * MethodName: Name of the method
 * Param: The current parameter value.
 */
#define VEOS_FRM_DBG_METHOD_PARAM(ApiName, MethodName, Param)                       (void)VEOS_MsgApi_LogMessage(ApiName, MethodName, "Parameter "#Param" set to %x.", Param)
/*
 * Parameterized macro to track the result of a VEOS frame function.
 * ApiName: Name of the API currently used
 * MethodName: Name of the method
 * Result: The current function result.
 */
#define VEOS_FRM_DBG_METHOD_RESULT(ApiName, MethodName, Result)                     (void)VEOS_MsgApi_LogMessage(ApiName, MethodName, "Function result set to %x.", Result)
#else
/*
 * Parameterized macro to track the start of a VEOS frame function.
 * ApiName: Name of the API currently used
 * MethodName: Name of the method
 */
#define VEOS_FRM_DBG_METHOD_START(ApiName, MethodName)                              (void)0
/*
 * Parameterized macro to track the end of a VEOS frame function.
 * ApiName: Name of the API currently used
 * MethodName: Name of the method
 */
#define VEOS_FRM_DBG_METHOD_END(ApiName, MethodName)                                (void)0
/*
 * Parameterized macro to track the value of a parameter passed to a VEOS frame function.
 * ApiName: Name of the API currently used
 * MethodName: Name of the method
 * Param: The current parameter value.
 */
#define VEOS_FRM_DBG_METHOD_PARAM(ApiName, MethodName, Param)                       VEOS_DBG_UNUSED_PARAM(Param)
/*
 * Parameterized macro to track the result of a VEOS frame function.
 * ApiName: Name of the API currently used
 * MethodName: Name of the method
 * Result: The current function result.
 */
#define VEOS_FRM_DBG_METHOD_RESULT(ApiName, MethodName, Result)                     (void)0
#endif /* VEOS_DEBUG */

/*
 * Parameterized macro to end the current call-out, if an error has occurred yet.
 * If so, the function returns immediately using the error code.
 * Param: The current result of the call-out
 */
#define VEOS_CHECK_LOCAL_CALLOUT_RESULT(Param)              \
    if (VEOS_E_OK != Param) {                               \
    {                                                       \
        VEOS_REPORT_ERROR(Param);                           \
        return Param;                                       \
    }                                                       \
}

/*
 * Parameterized macro to end the current callback, if an error has occurred yet.
 * If so, the function returns immediately using the error code.
 * Param: The current result of the callback
 */
#define VEOS_CHECK_LOCAL_CALLBACK_RESULT(LocalParam)        \
    if (VEOS_E_OK != LocalParam) {                          \
    {                                                       \
        VEOS_CALLBACK_ERROR();                              \
        return LocalParam;                                  \
    }                                                       \
}

/*
 * Parameterized macro to check, if the array size currently passed is not exceeding the maximum value.
 * LocalSize:   The array size to check.
 * MaximumSize: The maximum size of the array.
 */
#define CHECK_PARAM_OVERFLOW(LocalSize, MaximumSize)        \
     if (MaximumSize < LocalSize)                           \
     {                                                      \
         VEOS_REPORT_ERROR(VEOS_E_BUFFER_OVERFLOW);         \
         return VEOS_E_BUFFER_OVERFLOW;                     \
     }                                                      \

/*
 * Parameterized macro to check, if the parameter currently passed is not a pointer to a valid memory position.
 * LocalParam: The parameter to check.
 */
#define VEOS_CHECK_PARAM_PTR(LocalParam)                    \
    if (VEOS_NULL == LocalParam)                            \
    {                                                       \
        VEOS_REPORT_ERROR(VEOS_E_NULL_PARAM);               \
        return VEOS_E_NULL_PARAM;                           \
    }

/*
 * Parameterized macro to check, if the parameter currently passed is not a pointer to a valid memory position.
 * LocalParam: The parameter to check.
 */
#define VEOS_CHECK_PARAM_PTR_VOID(LocalParam)               \
    if (VEOS_NULL == LocalParam) {                          \
    {                                                       \
        VEOS_REPORT_ERROR(VEOS_E_NULL_PARAM);               \
        return;                                             \
    }                                                       \
}

/*
 * Parameterized macro to report an error within the callback.
 */
#define VEOS_CALLBACK_ERROR()                               VEOS_REPORT_ERROR(VEOS_E_CALLBACK_ERROR)

/*
 * Parameterized macro to report an error within the current function.
 * Error: The error code.
 */
#ifndef VEOS_SIGNAL_API_ERROR
#define VEOS_REPORT_ERROR(Error)                            \
    {                                                       \
        g_VEOS_LastError = (VEOS_ApiSuccessType)Error;      \
    }
#else
#define VEOS_REPORT_ERROR(Error)                            \
    {                                                       \
        g_VEOS_LastError = (VEOS_ApiSuccessType)Error;      \
        if (VEOS_TRUE == g_VEOS_SimApi_SignalErrorOn)       \
        {                                                   \
            (void)VEOS_SimApi_SignalApiError();             \
        }                                                   \
    }
#endif /* !VEOS_SIGNAL_API_ERROR */

/*
 * Parameterized macro to mark a callback as unhandled.
 * ApiId: ID of the API
 * CallbackId: ID of the callback.
 */
#define VEOS_CALLBACK_UNHANDLED(ApiId, CallbackId)                          \
    {                                                                       \
         VEOS_LOG_API_FUNC(ApiId, CallbackId, VEOS_E_PENDING);              \
    }

 /*
 * Parameterized macro to mark a call-out as unhandled.
 * ApiId: ID of the API
 * CalloutId: ID of the call-out.
 */
#define VEOS_CALLOUT_UNHANDLED(ApiId, CalloutId)                            \
    {                                                                       \
        VEOS_LOG_API_FUNC(ApiId, CalloutId, VEOS_E_PENDING);                \
    }                                                                       \

/*
 * Parameterized macro to initialize / reset the error handling variables.
 * ApiId: ID of the API
 * FuncId: ID of the function
 * Result: The current result.
 */
#define VEOS_LOG_API_FUNC(ApiId, FuncId, Result)            \
    {                                                       \
        g_VEOS_LastApiCalled   = (VEOS_char *)ApiId;        \
        g_VEOS_LastFuncCalled  = (VEOS_char *)FuncId;       \
        g_VEOS_LastError = (VEOS_ApiSuccessType)Result;     \
    }

/*
 * Parameterized macro to mark a callback as handled.
 * ApiId: ID of the API
 * CallbackId: ID of the callback
 * Result: Recent result of the callback
 */
#define VEOS_CALLBACK_HANDLED(ApiId, CallbackId, Result)    \
    {                                                       \
         VEOS_LOG_API_FUNC(ApiId, CallbackId, Result);      \
    }

/*
 * Parameterized macro to mark a call-out as handled.
 * ApiId: ID of the API
 * CallbackId: ID of the call-out
 * Result: Recent result of the call-out
 */
#define VEOS_CALLOUT_HANDLED(ApiId, CalloutId, Result)      \
    {                                                       \
        VEOS_LOG_API_FUNC(ApiId, CalloutId, Result);        \
    }

/*------------------------------------------------------------------------------------------------*\
  VARIABLE DECLARATIONS
\*------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif
    /* Global API error buffer: Last error code. */
    extern VEOS_ApiSuccessType g_VEOS_LastError;
    /* Global API error buffer: Last API id. */
    extern VEOS_char * g_VEOS_LastApiCalled;
    /* Global API error buffer: Last API function id. */
    extern VEOS_char * g_VEOS_LastFuncCalled;
    /* Set to true to turn on error handling. */
    extern VEOS_boolean g_VEOS_SimApi_SignalErrorOn;
#ifdef __cplusplus
};
#endif

/*------------------------------------------------------------------------------------------------*\
  FUNCTION PROTOTYPES
\*------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif
/**************************************************************************************************\
 *** FUNCTION:
 ***     VEOS_SimApi_SignalApiError
 ***
 *** DESCRIPTION:
 ***     Signals an error to the simulator.
 ***
 *** PARAMETERS:
 ***     -/-
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any error
 ***                            occurs, an error code will be the result. See VEOS_Types.h for
 ***                            details.
\**************************************************************************************************/
extern VEOS_ApiSuccessType VEOS_SimApi_SignalApiError(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEOS_ErrorCodeToMessageConverter
 ***
 *** DESCRIPTION:
 ***     Converts an error ID to an error message.
 ***
 *** PARAMETERS:
 ***     VEOS_ApiSuccessType    The error ID.
 ***
 *** RETURNS:
 ***     const VEOS_char *      The error message.
\**************************************************************************************************/
extern const VEOS_char * VEOS_ErrorCodeToMessageConverter(VEOS_ApiSuccessType);
#ifdef __cplusplus
};
#endif

/*------------------------------------------------------------------------------------------------*\
  VARIABLE DEFINITIONS
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
  INLINE FUNCTIONS
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
  FUNCTION DEFINITIONS
\*------------------------------------------------------------------------------------------------*/

#endif /* VEOS_DBGEH_H */

/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/
