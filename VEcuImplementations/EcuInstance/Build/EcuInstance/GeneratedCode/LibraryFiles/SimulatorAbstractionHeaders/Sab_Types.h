/**************************************************************************************************\
 *** Sab_Types.h
 ***
 *** Generic Simulator Abstraction type definition header file.
 ***
 *** Copyright 2019 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef SAB_TYPES_H
#define SAB_TYPES_H

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/

#include "Std_Types.h"

/*------------------------------------------------------------------------------------------------*\
  DEFINES
\*------------------------------------------------------------------------------------------------*/

#ifdef DS_PLATFORM_64BIT_INTTYPE
#define SAB_GPT_64BITNATIVE_TYPE
#endif /* DS_PLATFORM_64BIT_INTTYPE */

#define SAB_GPT_DRIVERSUPPORTED
#define SAB_HUNDRED_NANOSECONDPRECISION     STD_ON
#define SAB_ONE_NANOSECONDPRECISION         STD_ON

#ifdef SAB_GPT_DRIVERSUPPORTED
#define SAB_GPT_RESOLUTION_100NS    10000000
 #if (SAB_ONE_NANOSECONDPRECISION == STD_ON)
  #define SAB_GPT_RESOLUTION_1NS      1000000000
 #endif
#endif

#ifndef NULL
#define NULL        (void *)0
#endif

/*------------------------------------------------------------------------------------------------*\
  TYPEDEFS
\*------------------------------------------------------------------------------------------------*/


typedef Std_ReturnType Sab_ReturnType;

/* Sab types mapping to autosar platform types */
typedef uint8 Sab_Bool;
typedef uint8 Sab_UInt8;
typedef uint16 Sab_UInt16;
typedef uint32 Sab_UInt32;
typedef uint64 Sab_UInt64;
typedef sint8 Sab_Int8;
typedef sint16 Sab_Int16;
typedef sint32 Sab_Int32;
typedef sint64 Sab_Int64;
typedef float32 Sab_Float32;
typedef float64 Sab_Float64;


/* Defines the severity of the message. */
typedef enum
{
    Sab_MsgSeverity_Log,        /* Submit message to the log file. */
    Sab_MsgSeverity_Info,       /* Submit info message. */
    Sab_MsgSeverity_Warning,    /* Submit warning message. */
    Sab_MsgSeverity_Error       /* Submit error message. */
} Sab_MsgSeverity;


typedef struct tag_Sab_PredefTimer1ns64bitType {
    Sab_UInt64 ReferenceTime;
} Sab_PredefTimer1ns64bitType;


typedef Sab_UInt64 Sab_TickType1ns64bit; 
typedef Sab_UInt32 Sab_TickType1ns32bit; 

typedef Sab_UInt32 Sab_NotificationIdType;
typedef void (* Sab_NotificationFunctionType)(void);


/* Application states for the simulator abstraction */
typedef enum
{
    Sab_Startup = 0,
    Sab_Run,
    Sab_ShutDown,
    Sab_Sleep,
    Sab_Wakeup,
    Sab_Off,
} Sab_ApplicationState;

/* Initialization for the measurement of ticks */
typedef uint32 Sab_TickType;


#ifdef SAB_GPT_64BITNATIVE_TYPE
typedef uint64 Sab_UInt64;

#define SAB_UINT64_ATOMIC
#define SAB_UINT64_INITVALUE 0
#else
typedef struct {
    uint32 high;
    uint32 low;
} Sab_UInt64;

#define SAB_UINT64_INITVALUE_HIGH           0
#define SAB_UINT64_INITVALUE_LOW            0
#define SAB_UINT64_INITVALUE { SAB_UINT64_INITVALUE_HIGH , SAB_UINT64_INITVALUE_LOW }

#endif /* SAB_GPT_64BITNATIVE_TYPE */


typedef struct tag_Sab_PredefTimer100ns32bitType {
    uint32 ReferenceTime;
} Sab_PredefTimer100ns32bitType;

typedef uint32 Sab_HwHandleType;

typedef uint32 Sab_BoardAddressType;

typedef uint32 Sab_ChannelAddressType;


typedef uint64 Sab_NotificationTicksType;

#endif /* SAB_TYPES_H */

/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/
