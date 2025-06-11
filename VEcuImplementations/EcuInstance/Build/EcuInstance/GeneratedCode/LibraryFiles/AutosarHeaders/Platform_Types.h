/**************************************************************************************************\
 *** Platform_Types.h
 ***
 *** Target platform specific configurations.
 ***
 *** Selected Platform:         HostPC
 ***          Microcontroller:  i86
 ***
 *** Copyright (c) 2008 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef PLATFORM_TYPES_H
#define PLATFORM_TYPES_H

/*----------------------------------------------------------------------------*\
  DEFINES (OPT)
\*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*\
  INCLUDES
\*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*\
  DEFINES
\*----------------------------------------------------------------------------*/

/* The standard common published information */
#define PLATFORM_VENDOR_ID                       0x0023u
#define PLATFORM_MODULE_ID                       0x00C7u
#define PLATFORM_AR_RELEASE_MAJOR_VERSION        0x04u
#define PLATFORM_AR_RELEASE_MINOR_VERSION        0x03u
#define PLATFORM_AR_RELEASE_REVISION_VERSION     0x00u
#define PLATFORM_SW_MAJOR_VERSION                0x02u
#define PLATFORM_SW_MINOR_VERSION                0x00u
#define PLATFORM_SW_PATCH_VERSION                0x00u

/* CPU types. */
#define CPU_TYPE_8  8
#define CPU_TYPE_16 16
#define CPU_TYPE_32 32
#define CPU_TYPE_64 64

/* Bit ordering. */
#define MSB_FIRST 0
#define LSB_FIRST 1

/* Byte ordering. */
#define HIGH_BYTE_FIRST 0
#define LOW_BYTE_FIRST 1

/* CPU. */
#define CPU_TYPE CPU_TYPE_64
#define CPU_BIT_ORDER LSB_FIRST
#define CPU_BYTE_ORDER LOW_BYTE_FIRST

/* Definitions for TRUE and FALSE. */
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

/*----------------------------------------------------------------------------*\
  TYPEDEFS
\*----------------------------------------------------------------------------*/

/* AUTOSAR type used together with the definitions TRUE and FALSE. */
typedef unsigned char boolean;

/* Integer data types. */
typedef unsigned char uint8;                /* The standard AUTOSAR type for 8 bit unsigned integer */
typedef unsigned short int uint16;          /* The standard AUTOSAR type for 16 bit unsigned integer */
typedef unsigned int uint32;                /* The standard AUTOSAR type for 32 bit unsigned integer */
typedef unsigned long long uint64;          /* The standard AUTOSAR type for 64 bit unsigned integer */
typedef signed char sint8;                  /* The standard AUTOSAR type for 8 bit signed integer */
typedef signed short int sint16;            /* The standard AUTOSAR type for 16 bit signed integer */
typedef signed int sint32;                  /* The standard AUTOSAR type for 32 bit signed integer */
typedef signed long long sint64;            /* The standard AUTOSAR type for 64 bit signed integer */

/* Optimized integer data types. */
typedef unsigned long int uint8_least;      /* The optimized AUTOSAR type for 8 bit unsigned integer */
typedef unsigned long int uint16_least;     /* The optimized AUTOSAR type for 16 bit unsigned integer */
typedef unsigned long int uint32_least;     /* The optimized AUTOSAR type for 32 bit unsigned integer */
typedef signed long int sint8_least;        /* The optimized AUTOSAR type for 8 bit signed integer */
typedef signed long int sint16_least;       /* The optimized AUTOSAR type for 16 bit signed integer */
typedef signed long int sint32_least;       /* The optimized AUTOSAR type for 32 bit signed integer */

/* Floating point types. */
typedef float float32;                      /* The standard AUTOSAR type for 32 bit floating-point number */
typedef double float64;                     /* The standard AUTOSAR type for 64 bit floating-point number */
/*----------------------------------------------------------------------------*\
  ENUMS
\*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*\
  VARIABLES
\*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*\
  PARAMETERIZED MACROS
\*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*\
  FUNCTION PROTOTYPES
\*----------------------------------------------------------------------------*/

#endif/* PLATFORM_TYPES_H */
/*----------------------------------------------------------------------------*\
 END OF FILE
\*----------------------------------------------------------------------------*/
