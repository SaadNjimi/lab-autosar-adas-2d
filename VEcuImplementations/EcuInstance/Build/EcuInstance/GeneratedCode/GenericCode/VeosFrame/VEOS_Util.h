/**************************************************************************************************\
 *** VEOS_Util.h
 ***
 *** VEOS Utilities.
 ***
 *** Copyright 2019 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef VEOS_UTIL_H
#define VEOS_UTIL_H

/*------------------------------------------------------------------------------------------------*\
  DEFINES (OPT)
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/

#include "VEOS_Types.h"
#include "VEOS_Guard.h"
#include "VEOS_Cfg.h"
#include "VEOS_DbgEh.h"
#include "VEOS_Compiler.h"

#ifdef VEOS_GUARD_SOURCECODEONLY_OK
#ifndef VEOS_USE_MEMCPY_OWN
/* Required for memcopy-Function */
#include "string.h"
#endif /* !VEOS_USE_MEMCPY_OWN */
#endif /* VEOS_GUARD_SOURCECODEONLY_OK */

/*------------------------------------------------------------------------------------------------*\
  DEFINES
\*------------------------------------------------------------------------------------------------*/

/* The definition for Calibration variables */
#ifndef VEOS_CAL
#define VEOS_CAL                                  volatile
#endif /* !VEOS_CAL */

/* The data type check shall be ignored. */
#define VEOS_DTCHECK_NOSIGNCHECKREQUIRED		  (signed char)-1

#define VEOS_DTCHECK_UNSIGNEDEXPECTATIONMASK	  (signed char)0x40

#define VEOS_DTCHECK_SIGNEDEXPECTATIONMASK	      (signed char)0x20

#define VEOS_DTCHECK_SIGNCHECK_SUCCEEDED          (signed char)0x00

#define VEOS_DTCHECK_SIGNCHECK_FAILED_SIGNED      (signed char)(0x01 | VEOS_DTCHECK_SIGNEDEXPECTATIONMASK)

#define VEOS_DTCHECK_SIGNCHECK_FAILED_UNSIGNED    (signed char)(0x01 | VEOS_DTCHECK_UNSIGNEDEXPECTATIONMASK)

/*------------------------------------------------------------------------------------------------*\
  PARAMETERIZED MACROS
\*------------------------------------------------------------------------------------------------*/

#ifdef VEOS_GUARD_SOURCECODEONLY_OK
#ifdef VEOS_GUARD_INTERNAL_API_OK
#ifndef VEOS_USE_MEMCPY_OWN
/*
 * Parameterized macro copying the content (delimited by the size) of the source array to the destination array assuming
 * it is of the specified data type.
 */
#define VEOS_COPY_ARRAY(DataType, Src, Dest, Size)		memcpy(Dest, Src, (Size * sizeof(VEOS_##DataType)))
#define VEOS_COPY_ARRAY_APIRESULT(DataType, Src, Dest, Size)        \
                                                        (VEOS_COPY_ARRAY(DataType, Src, Dest, Size)!= VEOS_NULL ? VEOS_E_OK : VEOS_E_NOT_OK)
#else
/*
 * Parameterized macro copying the content (delimited by the size) of the source array to the destination array assuming
 * it is of the specified data type.
 */
#define VEOS_COPY_ARRAY(DataType, Src, Dest, Size)		VEOS_CopyArray_##DataType(Src, Size, Dest, Size)
#define VEOS_COPY_ARRAY_APIRESULT                       VEOS_COPY_ARRAY
#endif /* !VEOS_USE_MEMCPY_OWN */
#endif /* !VEOS_GUARD_INTERNAL_API_OK */
#endif /* VEOS_GUARD_SOURCECODEONLY_OK */

#ifdef VEOS_GUARD_PRIVATE_API_OK
/*
 * Parameterized macro assigning the value of a local scalar parameter to a variable.
 */
#define VEOS_COPY_IN_PARAM_SCALAR(GlobalParam, LocalParam)		GlobalParam = LocalParam;
/*
 * Parameterized macro assigning the value of a variable to a local scalar parameter assuming it is a pointer.
 */
#define VEOS_COPY_OUT_PARAM_SCALAR(LocalParam, GlobalParam)		* LocalParam = GlobalParam;

/*
 * Parameterized macro copying the content (delimited by the size) of the source array to the destination array assuming
 * it is of the specified data type.
 */
#define VEOS_COPY_PARAM_ARRAY                                   VEOS_COPY_ARRAY_APIRESULT

/*
 * Redefinition of VEOS_COPY_IN_PARAM_SCALAR for Call-outs.
 */
#define VEOS_COPY_CO_IN_PARAM_SCALAR							VEOS_COPY_IN_PARAM_SCALAR
/*
 * Redefinition of VEOS_COPY_OUT_PARAM_SCALAR for Call-outs.
 */
#define VEOS_COPY_CO_OUT_PARAM_SCALAR							VEOS_COPY_IN_PARAM_SCALAR

/*
 * Redefinition of VEOS_COPY_IN_PARAM_SCALAR for Callbacks.
 */
#define VEOS_COPY_CBK_IN_PARAM_SCALAR							VEOS_COPY_IN_PARAM_SCALAR

/*
 * Redefinition of VEOS_COPY_OUT_PARAM_SCALAR for Callbacks.
 */
#define VEOS_COPY_CBK_OUT_PARAM_SCALAR							VEOS_COPY_OUT_PARAM_SCALAR

/*
 * Parameterized macro to find the minimum of to primitive values a and b.
 */
#define VEOS_MIN_PARAM(a, b)									((a) < (b) ? (a) : (b))

/*
 * Parameterized macro to find the maximum of to primitive values a and b.
 */
#define VEOS_MAX_PARAM(a, b)									((a) > (b) ? (a) : (b))
#endif /* VEOS_GUARD_PRIVATE_API_OK */

/*
 * Parameterized macro performing a data type check
 * assuming that the data type passed is signed.
 */
#define VEOS_DTCHECK_ISSIGNEDDATATYPE(DataType)			(signed char)(((DataType)-1 < 0) ? VEOS_DTCHECK_SIGNCHECK_SUCCEEDED : VEOS_DTCHECK_SIGNCHECK_FAILED_SIGNED)

/*
 * Parameterized macro performing a data type check
 * assuming that the data type passed is unsigned.
 */
#define VEOS_DTCHECK_ISUNSIGNEDDATATYPE(DataType)		(signed char)((((DataType)-1) > 0) ? VEOS_DTCHECK_SIGNCHECK_SUCCEEDED : VEOS_DTCHECK_SIGNCHECK_FAILED_UNSIGNED)

/*------------------------------------------------------------------------------------------------*\
  VARIABLE DECLARATIONS
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
  FUNCTION PROTOTYPES
\*------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef VEOS_GUARD_INTERNAL_API_OK

#ifdef VEOS_USE_MEMCPY_OWN
/**************************************************************************************************\
 *** FUNCTION:
 ***     VEOS_CopyArray_uint8
 ***
 *** DESCRIPTION:
 ***     Copies the specified number of uint8 values from the specified source buffer to the
 ***     specified outgoing data buffer.
 ***
 ***     This function is marked as internal and may only be called from VEOS frame and directly
 ***     related modules.
 ***
 *** PARAMETERS:
 ***     Type                    Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     const VEOS_uint8 *      pSrc         Incoming argument: The source buffer to copy from.
 ***     const VEOS_uint32       srcLength    Incoming argument: The number of items to copy.
 ***     VEOS_uint8 *            pDst         Incoming and outgoing argument: The buffer to copy data to.
 ***     const VEOS_uint32       dstMaxLength Incoming argument: The maximum size of the destination buffer.
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any error
 ***                            occurs, an error code will be the result. See VEOS_Types.h for
 ***                            details.
\**************************************************************************************************/
extern VEOS_ApiSuccessType VEOS_CopyArray_uint8(const VEOS_uint8 *pSrc, const VEOS_uint32 srcLength, VEOS_uint8 * pDst, const VEOS_uint32 dstMaxLength);

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEOS_CopyArray_sint8
 ***
 *** DESCRIPTION:
 ***     Copies the specified number of sint8 values from the specified source buffer to the
 ***     specified outgoing data buffer.
 ***
 ***     This function is marked as internal and may only be called from VEOS frame and directly
 ***     related modules.
 ***
 *** PARAMETERS:
 ***     Type                    Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     const VEOS_sint8 *      pSrc         Incoming argument: The source buffer to copy from.
 ***     const VEOS_uint32       srcLength    Incoming argument: The number of items to copy.
 ***     VEOS_sint8 *            pDst         Incoming and outgoing argument: The buffer to copy data to.
 ***     const VEOS_uint32       dstMaxLength Incoming argument: The maximum size of the destination buffer.
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any error
 ***                            occurs, an error code will be the result. See VEOS_Types.h for
 ***                            details.
\**************************************************************************************************/
extern VEOS_ApiSuccessType VEOS_CopyArray_sint8(const VEOS_sint8 *pSrc, const VEOS_uint32 srcLength, VEOS_sint8 * pDst, const VEOS_uint32 dstMaxLength);

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEOS_CopyArray_uint16
 ***
 *** DESCRIPTION:
 ***     Copies the specified number of uint16 values from the specified source buffer to the
 ***     specified outgoing data buffer.
 ***
 ***     This function is marked as internal and may only be called from VEOS frame and directly
 ***     related modules.
 ***
 *** PARAMETERS:
 ***     Type                    Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     const VEOS_uint16 *     pSrc         Incoming argument: The source buffer to copy from.
 ***     const VEOS_uint32       srcLength    Incoming argument: The number of items to copy.
 ***     VEOS_uint16 *           pDst         Incoming and outgoing argument: The buffer to copy data to.
 ***     const VEOS_uint32       dstMaxLength Incoming argument: The maximum size of the destination buffer.
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any error
 ***                            occurs, an error code will be the result. See VEOS_Types.h for
 ***                            details.
\**************************************************************************************************/
extern VEOS_ApiSuccessType VEOS_CopyArray_uint16(const VEOS_uint16 *pSrc, const VEOS_uint32 srcLength, VEOS_uint16 * pDst, const VEOS_uint32 dstMaxLength);

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEOS_CopyArray_sint16
 ***
 *** DESCRIPTION:
 ***     Copies the specified number of sint16 values from the specified source buffer to the
 ***     specified outgoing data buffer.
 ***
 ***     This function is marked as internal and may only be called from VEOS frame and directly
 ***     related modules.
 ***
 *** PARAMETERS:
 ***     Type                    Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     const VEOS_sint16 *     pSrc         Incoming argument: The source buffer to copy from.
 ***     const VEOS_uint32       srcLength    Incoming argument: The number of items to copy.
 ***     VEOS_sint16 *           pDst         Incoming and outgoing argument: The buffer to copy data to.
 ***     const VEOS_uint32       dstMaxLength Incoming argument: The maximum size of the destination buffer.
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any error
 ***                            occurs, an error code will be the result. See VEOS_Types.h for
 ***                            details.
\**************************************************************************************************/
extern VEOS_ApiSuccessType VEOS_CopyArray_sint16(const VEOS_sint16 *pSrc, const VEOS_uint32 srcLength, VEOS_sint16 * pDst, const VEOS_uint32 dstMaxLength);

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEOS_CopyArray_uint32
 ***
 *** DESCRIPTION:
 ***     Copies the specified number of uint32 values from the specified source buffer to the
 ***     specified outgoing data buffer.
 ***
 ***     This function is marked as internal and may only be called from VEOS frame and directly
 ***     related modules.
 ***
 *** PARAMETERS:
 ***     Type                    Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     const VEOS_uint32 *     pSrc         Incoming argument: The source buffer to copy from.
 ***     const VEOS_uint32       srcLength    Incoming argument: The number of items to copy.
 ***     VEOS_uint32 *           pDst         Incoming and outgoing argument: The buffer to copy data to.
 ***     const VEOS_uint32       dstMaxLength Incoming argument: The maximum size of the destination buffer.
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any error
 ***                            occurs, an error code will be the result. See VEOS_Types.h for
 ***                            details.
\**************************************************************************************************/
extern VEOS_ApiSuccessType VEOS_CopyArray_uint32(const VEOS_uint32 *pSrc, const VEOS_uint32 srcLength, VEOS_uint32 * pDst, const VEOS_uint32 dstMaxLength);

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEOS_CopyArray_sint32
 ***
 *** DESCRIPTION:
 ***     Copies the specified number of sint32 values from the specified source buffer to the
 ***     specified outgoing data buffer.
 ***
 ***     This function is marked as internal and may only be called from VEOS frame and directly
 ***     related modules.
 ***
 *** PARAMETERS:
 ***     Type                    Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     const VEOS_sint32 *     pSrc         Incoming argument: The source buffer to copy from.
 ***     const VEOS_uint32       srcLength    Incoming argument: The number of items to copy.
 ***     VEOS_sint32 *           pDst         Incoming and outgoing argument: The buffer to copy data to.
 ***     const VEOS_uint32       dstMaxLength Incoming argument: The maximum size of the destination buffer.
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any error
 ***                            occurs, an error code will be the result. See VEOS_Types.h for
 ***                            details.
\**************************************************************************************************/
extern VEOS_ApiSuccessType VEOS_CopyArray_sint32(const VEOS_sint32 *pSrc, const VEOS_uint32 srcLength, VEOS_sint32 * pDst, const VEOS_uint32 dstMaxLength);

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEOS_CopyArray_float32
 ***
 *** DESCRIPTION:
 ***     Copies the specified number of float32 values from the specified source buffer to the
 ***     specified outgoing data buffer.
 ***
 ***     This function is marked as internal and may only be called from VEOS frame and directly
 ***     related modules.
 ***
 *** PARAMETERS:
 ***     Type                    Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     const VEOS_float32 *    pSrc         Incoming argument: The source buffer to copy from.
 ***     const VEOS_uint32       srcLength    Incoming argument: The number of items to copy.
 ***     VEOS_float32 *          pDst         Incoming and outgoing argument: The buffer to copy data to.
 ***     const VEOS_uint32       dstMaxLength Incoming argument: The maximum size of the destination buffer.
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any error
 ***                            occurs, an error code will be the result. See VEOS_Types.h for
 ***                            details.
\**************************************************************************************************/
extern VEOS_ApiSuccessType VEOS_CopyArray_float32(const VEOS_float32 *pSrc, const VEOS_uint32 srcLength, VEOS_float32 * pDst, const VEOS_uint32 dstMaxLength);

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEOS_CopyArray_float64
 ***
 *** DESCRIPTION:
 ***     Copies the specified number of float64 values from the specified source buffer to the
 ***     specified outgoing data buffer.
 ***
 ***     This function is marked as internal and may only be called from VEOS frame and directly
 ***     related modules.
 ***
 *** PARAMETERS:
 ***     Type                    Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     const VEOS_float64 *    pSrc         Incoming argument: The source buffer to copy from.
 ***     const VEOS_uint32       srcLength    Incoming argument: The number of items to copy.
 ***     VEOS_float64 *          pDst         Incoming and outgoing argument: The buffer to copy data to.
 ***     const VEOS_uint32       dstMaxLength Incoming argument: The maximum size of the destination buffer.
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any error
 ***                            occurs, an error code will be the result. See VEOS_Types.h for
 ***                            details.
\**************************************************************************************************/
extern VEOS_ApiSuccessType VEOS_CopyArray_float64(const VEOS_float64 *pSrc, const VEOS_uint32 srcLength, VEOS_float64 * pDst, const VEOS_uint32 dstMaxLength);

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEOS_CopyArray_char
 ***
 *** DESCRIPTION:
 ***     Copies the specified number of char values from the specified source buffer to the
 ***     specified outgoing data buffer.
 ***
 ***     This function is marked as internal and may only be called from VEOS frame and directly
 ***     related modules.
 ***
 *** PARAMETERS:
 ***     Type                    Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     const VEOS_char *       pSrc         Incoming argument: The source buffer to copy from.
 ***     const VEOS_uint32       srcLength    Incoming argument: The number of items to copy.
 ***     VEOS_char *             pDst         Incoming and outgoing argument: The buffer to copy data to.
 ***     const VEOS_uint32       dstMaxLength Incoming argument: The maximum size of the destination buffer.
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any error
 ***                            occurs, an error code will be the result. See VEOS_Types.h for
 ***                            details.
\**************************************************************************************************/
extern VEOS_ApiSuccessType VEOS_CopyArray_char(const VEOS_char *pSrc, const VEOS_uint32 srcLength, VEOS_char * pDst, const VEOS_uint32 dstMaxLength);

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEOS_CopyArray_boolean
 ***
 *** DESCRIPTION:
 ***     Copies the specified number of boolean values from the specified source buffer to the
 ***     specified outgoing data buffer.
 ***
 ***     This function is marked as internal and may only be called from VEOS frame and directly
 ***     related modules.
 ***
 *** PARAMETERS:
 ***     Type                    Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     const VEOS_boolean *    pSrc         Incoming argument: The source buffer to copy from.
 ***     const VEOS_uint32       srcLength    Incoming argument: The number of items to copy.
 ***     VEOS_boolean *          pDst         Incoming and outgoing argument: The buffer to copy data to.
 ***     const VEOS_uint32       dstMaxLength Incoming argument: The maximum size of the destination buffer.
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any error
 ***                            occurs, an error code will be the result. See VEOS_Types.h for
 ***                            details.
\**************************************************************************************************/
extern VEOS_ApiSuccessType VEOS_CopyArray_boolean(const VEOS_boolean *pSrc, const VEOS_uint32 srcLength, VEOS_boolean * pDst, const VEOS_uint32 dstMaxLength);

#endif /* VEOS_USE_MEMCPY_OWN */

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEOS_EnableInterruptHandlers
 ***
 *** DESCRIPTION:
 ***     Sets the interrupt handlers to enabled. If the interrupt cache is configured, the
 ***     the interrupts are stored interrupts enabled and handlers are called within this function.
 ***
 ***     This function is marked as internal and may only be called from VEOS frame and directly
 ***     related modules.
 ***
 *** PARAMETERS:
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
extern void VEOS_EnableInterruptHandlers(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEOS_DisableInterruptHandlers
 ***
 *** DESCRIPTION:
 ***     Sets the interrupt handlers to disabled. If the interrupt cache is configured, the
 ***     the interrupts are restored and handled, if the interrupt is enabled afterwards.
 ***
 ***     This function is marked as internal and may only be called from VEOS frame and directly
 ***     related modules.
 ***
 *** PARAMETERS:
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
extern void VEOS_DisableInterruptHandlers(void);

#ifdef VEOS_CHECKDTDEFINED
/**************************************************************************************************\
 *** FUNCTION:
 ***     VEOS_CheckDataType
 ***
 *** DESCRIPTION:
 ***     Clears the I/O event cache without calling the I/O event handler.
 ***
 *** PARAMETERS:
 ***     Type                             Name           Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     const VEOS_DataTypeCheckInfoType pCheckinfo     Pointer to the data type to be checked.
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any error
 ***                            occurs, an error code will be the result. See VEOS_Types.h for
 ***                            details.
\**************************************************************************************************/
extern VEOS_ApiSuccessType VEOS_CheckDataType(const VEOS_DataTypeCheckInfoType *pCheckinfo);
#endif /* VEOS_CHECKDTDEFINED */

#endif /* VEOS_GUARD_INTERNAL_API_OK */

#ifdef VEOS_GUARD_PRIVATE_API_OK
/**************************************************************************************************\
 *** FUNCTION:
 ***     VEOS_API_Start
 ***
 *** DESCRIPTION:
 ***     Performs an initialization procedure on the whole VEOS frame implementation.
 ***
 *** PARAMETERS:
 ***     -/-
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any error
 ***                            occurs, an error code will be the result. See VEOS_Types.h for
 ***                            details.
\**************************************************************************************************/
extern VEOS_ApiSuccessType VEOS_API_Start(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEOS_API_Stop
 ***
 *** DESCRIPTION:
 ***     Performs a clean shutdown on the whole VEOS frame implementation.
 ***
 *** PARAMETERS:
 ***     -/-
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any error
 ***                            occurs, an error code will be the result. See VEOS_Types.h for
 ***                            details.
\**************************************************************************************************/
extern VEOS_ApiSuccessType VEOS_API_Stop(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEOS_API_Download
 ***
 *** DESCRIPTION:
 ***     Performs special operations when the application has been downloaded.
 ***
 *** PARAMETERS:
 ***     -/-
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any error
 ***                            occurs, an error code will be the result. See VEOS_Types.h for
 ***                            details.
\**************************************************************************************************/
extern VEOS_ApiSuccessType VEOS_API_Download(void);

/****************************************************************************************************************************************************\
 *** FUNCTION:
 ***     VEOS_SimApiImpl_InterruptEvent
 ***
 *** DESCRIPTION:
 ***     Hand-coded implementation of API function InterruptEvent for SimApi of VEOS Frame
 ***
 *** PARAMETERS:
 ***     Type                                             Name           Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     const VEOS_SimApi_InterruptCallbackFunctionType  pInterruptFct  Incoming argument: The pointer to the interrupt callback function to call.
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any error occurs, an error code will be the result. See
 ***                            VEOS_Types.h for details.
\****************************************************************************************************************************************************/
#if defined(VEOS_GUARD_INTERRUPTHANDLING_OK)
extern VEOS_ApiSuccessType VEOS_SimApiImpl_InterruptEvent(VEOS_SimApi_InterruptCallbackFunctionType const pInterruptFct);
#endif /* defined(VEOS_GUARD_INTERRUPTHANDLING_OK) */

/**************************************************************************************************************************************************************************************************************************\
 *** FUNCTION:
 ***     VEOS_MdlInitApiImpl_CheckModel
 ***
 *** DESCRIPTION:
 ***     Frame function: Method to check model integrity.
 ***
 *** PARAMETERS:
 ***     Type                 Name                Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     const VEOS_uint32    ModelConfiguration  Incoming argument: The model id to transfer.
 ***     VEOS_uint32 * const  ModelCheckResult    Outgoing argument: The result of the model configuration check.
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any error occurs, an error code will be the result. See VEOS_Types.h for details.
\**************************************************************************************************************************************************************************************************************************/
extern VEOS_ApiSuccessType VEOS_MdlInitApiImpl_CheckModel(const VEOS_uint32 ModelConfiguration, VEOS_uint32 * const ModelCheckResult);

/**************************************************************************************************************************************************************************************************************************\
 *** FUNCTION:
 ***     DsCanLib_Initialize
 ***
 *** DESCRIPTION:
 ***     State management function for the DsCanLib.
 ***
 *** PARAMETERS:
 ***     -/-
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any error occurs, an error code will be the result. See VEOS_Types.h for details.
\**************************************************************************************************************************************************************************************************************************/
extern VEOS_ApiSuccessType DsCanLib_Initialize(void);

/**************************************************************************************************************************************************************************************************************************\
 *** FUNCTION:
 ***     DsLinLib_Initialize
 ***
 *** DESCRIPTION:
 ***     State management function for the DsLinLib.
 ***
 *** PARAMETERS:
 ***     -/-
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any error occurs, an error code will be the result. See VEOS_Types.h for details.
\**************************************************************************************************************************************************************************************************************************/
extern VEOS_ApiSuccessType DsLinLib_Initialize(void);

/**************************************************************************************************************************************************************************************************************************\
 *** FUNCTION:
 ***     DsEthLib_Initialize
 ***
 *** DESCRIPTION:
 ***     State management function for the DsEthLib.
 ***
 *** PARAMETERS:
 ***     -/-
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any error occurs, an error code will be the result. See VEOS_Types.h for details.
\**************************************************************************************************************************************************************************************************************************/
extern VEOS_ApiSuccessType DsEthLib_Initialize(void);

/**************************************************************************************************************************************************************************************************************************\
 *** FUNCTION:
 ***     DsSocketLib_Initialize
 ***
 *** DESCRIPTION:
 ***     State management function for the DsSocketLib.
 ***
 *** PARAMETERS:
 ***     -/-
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any error occurs, an error code will be the result. See VEOS_Types.h for details.
\**************************************************************************************************************************************************************************************************************************/
extern VEOS_ApiSuccessType DsSocketLib_Initialize(void);

/**************************************************************************************************************************************************************************************************************************\
 *** FUNCTION:
 ***     DsPcapLib_Initialize
 ***
 *** DESCRIPTION:
 ***     State management function for the DsPcapLib.
 ***
 *** PARAMETERS:
 ***     -/-
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any error occurs, an error code will be the result. See VEOS_Types.h for details.
\**************************************************************************************************************************************************************************************************************************/
extern VEOS_ApiSuccessType DsPcapLib_Initialize(void);

#endif /* VEOS_GUARD_INTERNAL_API_OK */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* VEOS_UTIL_H */

/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/
