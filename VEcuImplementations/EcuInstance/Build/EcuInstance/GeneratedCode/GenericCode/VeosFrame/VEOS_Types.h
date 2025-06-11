/**************************************************************************************************\
 *** VEOS_Types.h
 ***
 *** VEOS frame global type definitions.
 ***
 *** Copyright (c) 2015 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/
#ifndef VEOS_TYPES_V54_H
#define VEOS_TYPES_V54_H

/*------------------------------------------------------------------------------------------------*\
  DEFINES (OPT)
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/
#include "VEOS_BaseTypes.h"

#ifdef VEOS_GUARD_SOURCECODEONLY_OK
#ifdef VEOS_GUARD_INTERNAL_API_OK
#include "stddef.h"
#endif /* VEOS_GUARD_INTERNAL_API_OK */
#endif /* VEOS_GUARD_SOURCECODEONLY_OK */

/* Unversioned version type */
typedef unsigned short int VEOS_VersionType[4];

#ifdef VEOS_FRAME_V54_NAMESPACE_BEGIN
VEOS_FRAME_V54_NAMESPACE_BEGIN
#endif /* VEOS_FRAME_V54_NAMESPACE_BEGIN */

/*------------------------------------------------------------------------------------------------*\
  DEFINES
\*------------------------------------------------------------------------------------------------*/

/*-----------------------------*
 * Calling conventions for x86 *
 *-----------------------------*/
#if defined(_WIN32) && !defined(_WIN64)
#  ifdef __GNUC__
/*   For ANSI compliance with GCC */
#    define VEOS_CDECL __attribute__((__cdecl__))
#    define VEOS_FASTCALL __attribute__((__fastcall__))
#  else
#    define VEOS_CDECL __cdecl
#    define VEOS_FASTCALL __fastcall
#  endif /* __GNUC__ */
#else
#  define VEOS_CDECL
#  define VEOS_FASTCALL
#endif /* _WIN32 */

/*------------*
* DLL export *
*------------*/
#if defined(_WIN32)
#  define VEOS_EXPORTS __declspec(dllexport)
#else
#  define VEOS_EXPORTS __attribute__((visibility("default")))
#endif /* defined(_WIN32) */

/*----------------*
 * Boolean values *
 *----------------*/
/* Feature disabled */
#define VEOS_OFF                        0x00
/* Feature enabled */
#define VEOS_ON                         0x01

/* Condition not fulfilled */
#define VEOS_FALSE                      0x00
/* Condition fulfilled */
#define VEOS_TRUE                       0x01

/*--------------*
 * Other values *
 *--------------*/
/* Definition of invalid pointer */
#define VEOS_NULL                       (void *)0

/* The invalid I/O event ID */
#define VEOS_IOEVENTID_INVALID          (VEOS_uint32)0


/*------------------------------------------------------------------------------------------------*\
  TYPEDEFS
\*------------------------------------------------------------------------------------------------*/

/* Boolean value type definition */
typedef VEOS_base_boolean VEOS_boolean;
/* Unsigned integer with 8 bit */
typedef VEOS_base_uint8 VEOS_uint8;
/* Signed integer with 8 bit */
typedef VEOS_base_sint8 VEOS_sint8;
/* Unsigned integer with 16 bit */
typedef VEOS_base_uint16 VEOS_uint16;
/* Signed integer with 16 bit */
typedef VEOS_base_sint16 VEOS_sint16;
/* Unsigned integer with 32 bit */
typedef VEOS_base_uint32 VEOS_uint32;
/* Signed integer with 32 bit */
typedef VEOS_base_sint32 VEOS_sint32;
/* Unsigned integer with 64 bit */
typedef VEOS_base_uint64 VEOS_uint64;
/* Signed integer with 64 bit */
typedef VEOS_base_sint64 VEOS_sint64;
/* Floating point with 32 bit */
typedef VEOS_base_float32 VEOS_float32;
/* Floating point with 64 bit */
typedef VEOS_base_float64 VEOS_float64;

/* Void data type for consistency */
typedef void  VEOS_void;

/* Character type */
typedef char VEOS_char;

/* Return value for VEOS Frame API functions, callbacks and call-outs. See related error codes. */
typedef VEOS_uint16 VEOS_ApiSuccessType;

/* Error code type */
typedef enum
{
    /* Error code: No error occurred */
    VEOS_E_OK = (VEOS_ApiSuccessType)0x00,
    /* Error code: Unspecified error occurred */
    VEOS_E_NOT_OK = (VEOS_ApiSuccessType)0x01,
    /* Error code: Execution of function suspended or delayed */
    VEOS_E_NO_RESULT = (VEOS_ApiSuccessType)0x02,
    /* Error code: Function has not run to end yet */
    VEOS_E_PENDING = (VEOS_ApiSuccessType)0x03,

    /* Error code: Error during execution of callback */
    VEOS_E_CALLBACK_ERROR = (VEOS_ApiSuccessType)0x20,
    /* Error code: Error during initialization */
    VEOS_E_API_INITIALIZATION = (VEOS_ApiSuccessType)0x21,

    /* Error code: Parameter value NULL was passed instead of a pointer or array */
    VEOS_E_NULL_PARAM = (VEOS_ApiSuccessType)0x30,
    /* Error code: Invalid Enumeration value passed */
    VEOS_E_ENUM_VALUE = (VEOS_ApiSuccessType)0x31,
    /* Error code: Parameter value was invalid or unexpected */
    VEOS_E_BAD_PARAMETER = (VEOS_ApiSuccessType)0x32,

    /* Error code: A buffer has no space left */
    VEOS_E_BUFFER_OVERFLOW = (VEOS_ApiSuccessType)0x40,
    /* Error code: No items to process in a buffer */
    VEOS_E_BUFFER_UNDERFLOW = (VEOS_ApiSuccessType)0x41,

    /* Error code: Simulator did not provide an implementation for a specific API */
    VEOS_E_API_DISCONNECTED = (VEOS_ApiSuccessType)0x50,

    /* Error code: The function called does not provide an implementation at the moment (not implemented or switched off by configuration) */
    VEOS_E_NO_IMPLEMENTATION = (VEOS_ApiSuccessType)0xFF
} VEOS_ErrorCodeType;

/* Definition of application states */
typedef enum
{
    /* Application is not loaded */
    VEOS_State_Off,
    /* Application is currently loaded */
    VEOS_State_Downloaded,
    /* Application is started */
    VEOS_State_Started,
    /* Application was stopped */
    VEOS_State_Stopped
} VEOS_RuntimeStateType;

/* Definition of the runtime configuration */
typedef struct _tag_VEOS_RuntimeConfigType
{
    /* State of handling I/O events */
    VEOS_boolean ioEventsEnabled;
    /* Current state of application */
    VEOS_RuntimeStateType state;
} VEOS_RuntimeConfigType;

#ifdef VEOS_GUARD_SOURCECODEONLY_OK
#ifdef VEOS_GUARD_INTERNAL_API_OK
/* The data type check instructions */
typedef struct tag_VEOS_DataTypeCheckInfoType
{
    /* The name of the data type to check. */
    const char * DataTypeName;
    /* The expected size of the data type. */
    const size_t ExpectedSize;
    /* The real size in the binary. */
    const size_t RealSize;
    /* The result of the check for the algebraic sign of the data type. The result can be skipped by setting the value to 0xFF. */
    const signed char SignumCheckResult;
} VEOS_DataTypeCheckInfoType;

#define VEOS_CHECKDTDEFINED
#endif /* VEOS_GUARD_INTERNAL_API_OK */
#endif /* VEOS_GUARD_SOURCECODEONLY_OK */


#ifdef VEOS_FRAME_V54_NAMESPACE_END
VEOS_FRAME_V54_NAMESPACE_END
#endif /* VEOS_FRAME_V54_NAMESPACE_END */

#endif /* VEOS_TYPES_V54_H */

/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/
