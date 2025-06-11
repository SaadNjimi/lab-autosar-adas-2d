/**************************************************************************************************\
 *** Compiler.h
 ***
 *** Compiler specific configurations.
 ***
 *** Copyright (c) 2012 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef COMPILER_H
#define COMPILER_H

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/

/* Module specific memory and pointer classes. */
#include "Compiler_Cfg.h"

/*------------------------------------------------------------------------------------------------*\
  DEFINES
\*------------------------------------------------------------------------------------------------*/

/* The standard common published information */
#define COMPILER_VENDOR_ID                      0x0023u
#define COMPILER_MODULE_ID                      0x00C6u
#define COMPILER_AR_RELEASE_MAJOR_VERSION       0x04u
#define COMPILER_AR_RELEASE_MINOR_VERSION       0x03u
#define COMPILER_AR_RELEASE_REVISION_VERSION    0x00u
#define COMPILER_SW_MAJOR_VERSION               0x02u
#define COMPILER_SW_MINOR_VERSION               0x00u
#define COMPILER_SW_PATCH_VERSION               0x00u

/* Symbol for target compiler. */
#define _GCC_C_HOSTPC32_

/* AUTOMATIC used for the declaration of local pointers. */
#define AUTOMATIC

/* TYPEDEF used within type definitions, where no memory qualifier can be specified. */
#define TYPEDEF

/* Void pointer to zero. */
#ifndef NULL_PTR
    #define NULL_PTR ((void *)0)
#endif

/* Violation of SWS_COMPILER_00050, but required to compile AR 3.x compliant code. */
#define STATIC static

/* Abstraction of compiler keyword inline. */
#define INLINE inline
        /* The following must be omitted, since some implementations are using extern in combination with INLINE */
        /* __attribute__((always_inline)) */

/* Abstraction of compiler keyword inline for static functions. */
#define LOCAL_INLINE static INLINE

/* The compiler abstraction shall define the FUNC macro for the declaration and definition of
functions, that ensures correct syntax of function declarations as required by a specific compiler.
- used for API functions
   rettype     return type of the function
   memclass    classification of the function itself
               (not used on 32bit platforms)
*/
#define FUNC(rettype, memclass) rettype

/* FUNC_P2CONST macro for the declaration and definition of functions returning a pointer to a
constant. This shall ensure the correct syntax of function declarations as required
by a specific compiler.
    rettype          return type of the function
    ptrclass         defines the classification of the pointer's distance
    memclass         classification of the function itself
                     (not used on 32bit platforms)
*/
#define FUNC_P2CONST(rettype, ptrclass, memclass) const /*ptrclass*/ rettype *

/* FUNC_P2VAR macro
for the declaration and definition of functions returning a pointer to a variable. This
shall ensure the correct syntax of function declarations as required by a specific
compiler.
    rettype          return type of the function
    ptrclass         defines the classification of the pointer's distance
    memclass         classification of the function itself
                     (not used on 32bit platforms)
*/
#define FUNC_P2VAR(rettype, ptrclass, memclass) /*ptrclass*/  rettype *

/* Pointer to variable data
   ptrtype     type of the referenced data
   memclass    classification of the pointer's variable itself
               (not used on 32bit platforms)
   ptrclass    defines the classification of the pointer's distance
               (not used on 32bit platforms)
 */
#define P2VAR(ptrtype, memclass, ptrclass) ptrtype *

/* Pointer to constant data
   ptrtype     type of the referenced data
   memclass    classification of the pointer's variable itself
               (not used on 32bit platforms)
   ptrclass    defines the classification of the pointer's distance
               (not used on 32bit platforms)
*/
#define P2CONST(ptrtype, memclass, ptrclass) const ptrtype *

/* Const pointer to variable data
   ptrtype     type of the referenced data
   memclass    classification of the pointer's variable itself
               (not used on 32bit platforms)
   ptrclass    defines the classification of the pointer's distance
               (not used on 32bit platforms)
*/
#define CONSTP2VAR(ptrtype, memclass, ptrclass) ptrtype * const

/* Const pointer to constant data
   ptrtype     type of the referenced data
   memclass    classification of the pointer's variable itself
               (not used on 32bit platforms)
   ptrclass    defines the classification of the pointer's distance
               (not used on 32bit platforms)
*/
#define CONSTP2CONST(ptrtype, memclass, ptrclass) const ptrtype * const

/* Type definition of pointers to functions
   rettype     return type of the function
   ptrclass    defines the classification of the pointer's distance
               (not used on 32bit platforms)
   fctname     function name respectivly name of the defined type
*/
#define P2FUNC(rettype, ptrclass, fctname) rettype (*fctname)

/* Type definition of constant pointers to functions
   rettype     return type of the function
   ptrclass    defines the classification of the pointer's distance
               (not used on 32bit platforms)
   fctname     function name respectivly name of the defined type
*/
#define CONSTP2FUNC(rettype, ptrclass, fctname) rettype (* const fctname)

/* ROM constant
   type        type of the constant
   memclass    classification of the constant
               (not used on 32bit platforms)
*/
#ifdef CONST
#undef CONST
#endif
#define CONST(type, memclass) const type

/* RAM variables
   type        type of the variable
   memclass    classification of the variable
               (not used on 32bit platforms)
*/
#define VAR(type, memclass) type

#endif /* COMPILER_H */

/**************************************************************************************************\
 *** END OF FILE                                                                                  *
\**************************************************************************************************/
