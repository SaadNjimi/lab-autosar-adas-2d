/**************************************************************************************************\
 *** tl_types.h
 ***
 *** By definition, after inclusion of tl_types.h all original TargetLink base types
 ***     Bool, Int8/16/32, UInt8/16/32, Float32/64, Void
 *** are defined.
 ***
 *** For an AUTOSAR ECU, the types UInt8/16/32 are defined in Rte_Type.h and tl_basetypes.h defines
 *** the remaining types Bool, Int8/16/32, Float32/64, Void.
 ***
 *** For a Non-AUTOSAR ECU, Rte_Type.h does not exist. Therefore all TargetLink base types are
 *** defined in tl_basetypes.h.
 ***
 *** tl_types.h is included by dsfxp.h, i.e. it is needed to build dsfxp.lib for a given target.
 ***
 *** Copyright 2019 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

/*
 * NOTE:
 * If a non-AUTOSAR V-ECU comes with an own tl_types.h, it must be on the include path before this file.
 *
 * In TargetLink 3.3 the header shield has changed from _TL_TYPES_H_ to TL_TYPES_H.
 * The extra checks are for compatibility reasons.
 */
#if !defined(TL_TYPES_H) && !defined(_TL_TYPES_H_)
/* Avoid double inclusion of tl_types.h */
#define _TL_TYPES_H_
#define TL_TYPES_H

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/

#include "tl_basetypes.h"

/*------------------------------------------------------------------------------------------------*\
  TYPEDEFS
\*------------------------------------------------------------------------------------------------*/

/*
 * If Rte_Type.h was found on the file system, include it.
 * The flag USE_AUTOSAR_RTE_TYPES must be defined as an compiler option (makefile).
 * The determination is done by the build process.
 */
#if defined(USE_AUTOSAR_RTE_TYPES) || defined(VEOSTLTYPESUSERTE)

#include "Rte_Type.h"

#else /* !USE_AUTOSAR_RTE_TYPES */

    /*
     * All original TargetLink base types (Bool, Int8/16/32, UInt8/16/32, Float32/64, Void) are
     * defined in tl_basetypes.h.
     *
     * If the TargetLink base types have been renamed in the Data Dictionary, then the generated
     * tl_types.h and tl_basetypes.h have to be delivered with the TargetLink code and must be on
     * the include path before this file, i.e. this file will NOT be used.
     */

#endif /* !USE_AUTOSAR_RTE_TYPES */

#endif /* TL_TYPES_H */

/*----------------------------------------------------------------------------*\
  END OF FILE
\*----------------------------------------------------------------------------*/
