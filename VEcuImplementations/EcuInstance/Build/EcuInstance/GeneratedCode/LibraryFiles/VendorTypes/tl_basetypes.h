/**************************************************************************************************\
 *** tl_basetypes.h
 ***
 *** Defines the TargetLink base types.
 ***
 *** NOTE:
 ***
 ***  - This file is included by tl_types.h, which defines all original TargetLink base types
 ***    which are renamed in tl_basetypes.h. The definition is that after inclusion of
 ***    tl_types.h all original TargetLink base types are defined:
 ***      Bool, Int8/16/32, UInt8/16/32, Float32/64, Void
 ***
 ***  - For AUTOSAR V-ECUs, the TargetLink base types are renamed to be identical with the
 ***    corresponding AUTOSAR platform types (boolean, sint8/16/32, uint8/16/32, float32/64).
 ***    In that case the application code just includes Rte_Type.h which includes Platform_Types.h.
 ***    tl_basetypes.h does not appear in AUTOSAR application code directly. Thus it is not part
 ***    of the V-ECU implementation code.
 ***
 ***  - For Non-AUTOSAR-V-ECUs, the user may arbitrarily rename the TargetLink base types.
 ***    In that case this default version of tl_basetypes.h should NOT be included.
 ***    Instead an own version of tl_basetypes.h must be provided with the V-ECU implementation code.
 ***    The version provided with the V-ECU must be on the include path before this file.
 ***
 *** Copyright 2019 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

/*
 * NOTE:
 * If a non-AUTOSAR V-ECU comes with an own tl_basetypes.h, it must be on the include path before this file.
 *
 * In TargetLink 3.3 the header shield has changed from _TL_BASETYPES_H_ to TL_BASETYPES_H.
 * The extra checks are for compatibility reasons.
 */
#if !defined(TL_BASETYPES_H) && !defined(_TL_BASETYPES_H_)
/* Avoid double inclusion of tl_basetypes.h */
#define _TL_BASETYPES_H_
#define TL_BASETYPES_H

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/

#ifdef VECU_CODE
#include "Platform_Types.h"
#else
#include "VEOS_BaseTypes.h"
#endif /* VECU_CODE */

#if defined(USE_AUTOSAR_RTE_TYPES) || defined(TL_TYPES_DEFINED_IN_RTE_TYPE)

/* It is assumed that the types UInt8, UInt16, UInt32 are defined in Rte_Type.h
and that Rte_Type.h is included in tl_types.h */
#include "Rte_Type.h"

#endif /* USE_AUTOSAR_RTE_TYPES || TL_TYPES_DEFINED_IN_RTE_TYPE */

/*------------------------------------------------------------------------------------------------*\
  TYPEDEFS
\*------------------------------------------------------------------------------------------------*/

#ifdef VECU_CODE
#ifndef TL_TYPES_DEFINED_IN_RTE_TYPE

/* Check for upper case type definitions. */
#ifndef VEOS_UC_TYPES_DEFINED
    #define VEOS_UC_TYPES_DEFINED
    /* Need to define original TargetLink base types, which are different from AUTOSAR standard types. */
    typedef boolean Bool;              /* Boolean basetype. */
    typedef sint8 Int8;                /* 8 bit signed integer basetype. */
    typedef sint16 Int16;              /* 16 bit signed integer basetype. */
    typedef sint32 Int32;              /* 32 bit signed integer basetype. */
    typedef float32 Float32;           /* 32 bit floating-point basetype. */
    typedef float64 Float64;           /* 64 bit floating-point basetype. */
#endif /* !VEOS_UC_TYPES_DEFINED */
#endif /* TL_TYPES_DEFINED_IN_RTE_TYPE */

#ifndef USE_AUTOSAR_RTE_TYPES

/* Check for unsigned upper case type definitions. */
#ifndef VEOS_UUC_TYPES_DEFINED
    #define VEOS_UUC_TYPES_DEFINED
                                          /* On non-AUTOSAR ECUs we need to define remaining original TargetLink base types. */
    typedef uint8 UInt8;        /* 8 bit unsigned integer basetype. */
    typedef uint16 UInt16;      /* 16 bit unsigned integer basetype. */
    typedef uint32 UInt32;      /* 32 bit unsigned integer basetype. */
#endif /* !VEOS_UUC_TYPES_DEFINED */

#endif /* !USE_AUTOSAR_RTE_TYPES */

#ifndef VEOS_UC_VOID_TYPE_DEFINED
    #define VEOS_UC_VOID_TYPE_DEFINED
    typedef void Void;                 /* Void type. */
#endif /* !VEOS_UC_VOID_TYPE_DEFINED */
#else
/* Check for upper case type definitions. */
#ifndef VEOS_UC_TYPES_DEFINED
    #define VEOS_UC_TYPES_DEFINED
    /* Need to define original TargetLink base types, which are different from AUTOSAR standard types. */
    typedef VEOS_base_boolean Bool;              /* Boolean basetype. */
    typedef VEOS_base_sint8 Int8;                /* 8 bit signed integer basetype. */
    typedef VEOS_base_sint16 Int16;              /* 16 bit signed integer basetype. */
    typedef VEOS_base_sint32 Int32;              /* 32 bit signed integer basetype. */
    typedef VEOS_base_float32 Float32;           /* 32 bit floating-point basetype. */
    typedef VEOS_base_float64 Float64;           /* 64 bit floating-point basetype. */
#endif /* !VEOS_UC_TYPES_DEFINED */

/* Check for unsigned upper case type definitions. */
#ifndef VEOS_UUC_TYPES_DEFINED
    #define VEOS_UUC_TYPES_DEFINED
                                             /* On non-AUTOSAR ECUs we need to define remaining original TargetLink base types. */
    typedef VEOS_base_uint8 UInt8;        /* 8 bit unsigned integer basetype. */
    typedef VEOS_base_uint16 UInt16;      /* 16 bit unsigned integer basetype. */
    typedef VEOS_base_uint32 UInt32;      /* 32 bit unsigned integer basetype. */
#endif /* !VEOS_UUC_TYPES_DEFINED */

#ifndef VEOS_UC_VOID_TYPE_DEFINED
    #define VEOS_UC_VOID_TYPE_DEFINED
    typedef void Void;                 /* Void type. */
    #endif /* !VEOS_UC_VOID_TYPE_DEFINED */
#endif /* VECU_CODE */

#endif /* TL_BASETYPES_H */

/*----------------------------------------------------------------------------*\
  END OF FILE
\*----------------------------------------------------------------------------*/
