/**************************************************************************************************\
 *** Std_Types.h
 ***
 *** AUTOSAR standard types.
 ***
 *** Copyright (c) 2008 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef STD_TYPES_H
#define STD_TYPES_H

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/

#include "Platform_Types.h" /* Platform specific type definitions. */
#include "Compiler.h"       /* Compiler abstraction layer */

/*------------------------------------------------------------------------------------------------*\
  DEFINES
\*------------------------------------------------------------------------------------------------*/

/* The standard common published information */
#define STD_VENDOR_ID                       0x0023u
#define STD_MODULE_ID                       0x00C5u
#define STD_AR_RELEASE_MAJOR_VERSION        0x04u
#define STD_AR_RELEASE_MINOR_VERSION        0x03u
#define STD_AR_RELEASE_REVISION_VERSION     0x00u
#define STD_SW_MAJOR_VERSION                0x02u
#define STD_SW_MINOR_VERSION                0x00u
#define STD_SW_PATCH_VERSION                0x00u

/* For OSEK compliance this define has been added. */
#ifndef STATUSTYPEDEFINED
    #define STATUSTYPEDEFINED
    #define E_OK    0x00u

    typedef unsigned char StatusType; /* OSEK compliance. */
#endif

#define E_NOT_OK    0x01u

#define STD_HIGH    0x01u    /* Physical state 5V or 3.3V. */
#define STD_LOW     0x00u    /* Physical state 0V. */

#define STD_ACTIVE  0x01u    /* Logical state active. */
#define STD_IDLE    0x00u    /* Logical state idle. */

#define STD_ON      0x01u
#define STD_OFF     0x00u


/* Transformer classes */
#define STD_TRANSFORMER_UNSPECIFIED 0x00u
#define STD_TRANSFORMER_SERIALIZER  0x01u
#define STD_TRANSFORMER_SAFETY      0x02u
#define STD_TRANSFORMER_SECURITY    0x03u
#define STD_TRANSFORMER_CUSTOM      0xffu

/*------------------------------------------------------------------------------------------------*\
  TYPEDEFS
\*------------------------------------------------------------------------------------------------*/

/* This type is used as standard API return type which is shared between the RTE and the BSW
 * modules. The Std_ReturnType is normally used with value E_OK or E_NOT_OK. If those return values
 * are not sufficient user specific values can be defined by using the 6 least specific bits.
 * Bit 7 and bit 8 are reserved and defined by the RTE specification. */
typedef uint8 Std_ReturnType;

/* Version information. This type is be used to request the version of a BSW module using the
 * <ModuleName>_GetVersionInfo()function. */
typedef struct
{
   uint16 vendorID;
   uint16 moduleID;
   /*
    * instanceID has been removed in AR 3.1.4/4.X. It still remains in this struct for compatibility reasons.
    * It is not possible to use a struct initializer this way.
    * This limitation should be acceptable.
    * If the user has to use this, the define STD_TYPES_NO_INSTANCE_IN_VERSION must be set manually.
    */
#ifndef STD_TYPES_NO_INSTANCE_IN_VERSION
   uint8 instanceID;
#endif /* !STD_TYPES_NO_INSTANCE_IN_VERSION */
   uint8 sw_major_version;
   uint8 sw_minor_version;
   uint8 sw_patch_version;
} Std_VersionInfoType;


/* The data type Std_TransformerError is a struct which contains the error code and the transformer class to which the error belongs */
typedef uint8 Std_TransformerErrorCode;
typedef uint8 Std_TransformerClass;

typedef struct
{
    Std_TransformerErrorCode errorCode;
    Std_TransformerClass transformerClass;
}Std_TransformerError;


#endif  /* STD_TYPES_H */

/*----------------------------------------------------------------------------*\
  END OF FILE
\*----------------------------------------------------------------------------*/
