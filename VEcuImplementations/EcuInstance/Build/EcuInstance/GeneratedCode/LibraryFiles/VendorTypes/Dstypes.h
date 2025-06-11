/**************************************************************************************************\
 *** DsTypes.h
 ***
 *** Defines the dSPACE real-time and run-time library base types.
 ***
 *** NOTE:
 ***
 ***  - The definition is that after inclusion all original dSPACE base types are defined:
 ***      Bool, Int8/16/32, UInt8/16/32, Float32/64
 ***
 ***  - For Bus Simulation Containers, the dSPACE base types are renamed to be identical with the
 ***    corresponding AUTOSAR platform types (boolean, sint8/16/32, uint8/16/32, float32/64).
 ***
 ***  - On the VEOS platform, all dSPACE type definitions (tl_types, DsTypes) refer to the same,
 ***    unified definitions.
 ***
 *** Copyright 2020 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef DSTYPES_H
#define DSTYPES_H

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/

#include "VEOS_BaseTypes.h"

/*------------------------------------------------------------------------------------------------*\
  TYPE DEFINTIIONS
\*------------------------------------------------------------------------------------------------*/

/* Check for upper case type definitions. */
#ifndef VEOS_UC_TYPES_DEFINED
#define VEOS_UC_TYPES_DEFINED

typedef VEOS_base_boolean       Bool;
typedef VEOS_base_sint8         Int8;
typedef VEOS_base_sint16        Int16;
typedef VEOS_base_sint32        Int32;
typedef VEOS_base_float32       Float32;
typedef VEOS_base_float64       Float64;
#endif /* !VEOS_UC_TYPES_DEFINED */

/* Check for unsigned upper case type definitions. */
#ifndef VEOS_UUC_TYPES_DEFINED
#define VEOS_UUC_TYPES_DEFINED
typedef VEOS_base_uint8         UInt8;
typedef VEOS_base_uint16        UInt16;
typedef VEOS_base_uint32        UInt32;
#endif /* !VEOS_UUC_TYPES_DEFINED */

typedef Bool *                  BoolPtr;
typedef Int8 *                  Int8Ptr;
typedef UInt8 *                 UInt8Ptr;
typedef Int16 *                 Int16Ptr;
typedef UInt16 *                UInt16Ptr;
typedef Int32 *                 Int32Ptr;
typedef UInt32 *                UInt32Ptr;

/* Int64 / UInt64 are still left as structured data types. */
typedef struct { UInt32 low; Int32 high;  } Int64;
typedef struct { UInt32 low; UInt32 high; } UInt64;

typedef Int64 *                 Int64Ptr;
typedef UInt64 *                UInt64Ptr;
typedef Float32 *               Float32Ptr;
typedef Float64 *               Float64Ptr;

typedef VEOS_base_sint64        Long64;
typedef VEOS_base_uint64        ULong64;

typedef union { UInt64 ui64; ULong64 ul64; } DsULong64;
typedef Long64 *                Long64Ptr;
typedef ULong64 *               ULong64Ptr;
typedef VEOS_base_float64       dsfloat;

#endif /* DSTYPES_H */

/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/
