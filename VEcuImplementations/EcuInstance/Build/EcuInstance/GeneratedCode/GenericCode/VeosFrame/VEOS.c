/**************************************************************************************************\
 *** VEOS.c
 ***
 *** VEOS frame data source and utility function definition file.
 ***
 *** Copyright 2019 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef VEOS_C
#define VEOS_C

/*------------------------------------------------------------------------------------------------*\
  DEFINES (OPT)
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/

#include "VEOS.h"

/*------------------------------------------------------------------------------------------------*\
  PARAMETERIZED MACROS
\*------------------------------------------------------------------------------------------------*/

#ifdef VEOS_USE_MEMCPY_OWN
/**************************************************************************************************\
 *** MACRO:
 ***     VEOS_COPY_ARRAY_CASTED
 ***
 *** DESCRIPTION:
 ***     Copies the specified number of primitive values from the specified source buffer to the
 ***     specified outgoing data buffer assuming that the source and destination buffer
 ***     are of the specific type.
 ***
 ***
 *** PARAMETERS:
 ***     Type                    Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     const void *            pSrc         Incoming argument: The source buffer to copy from.
 ***     const VEOS_uint32       srcLength    Incoming argument: The number of items to copy.
 ***     void *                  pDst         Incoming and outgoing argument: The buffer to copy data to.
 ***     const VEOS_uint32       dstMaxLength Incoming argument: The maximum size of the destination buffer.
 ***     const DataTypeType      dataType     Incoming argument: The primitive data type of the buffer.
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any error
 ***                            occurs, an error code will be the result. See VEOS_Types.h for
 ***                            details.
\**************************************************************************************************/
#define VEOS_COPY_ARRAY_CASTED(pSrc, srcLength, pDst, dstMaxLength, dataType)               \
{                                                                                           \
    VEOS_uint32 i;                                                                          \
                                                                                            \
    VEOS_CHECK_PARAM_PTR(pSrc);                                                             \
    VEOS_CHECK_PARAM_PTR(pDst);                                                             \
                                                                                            \
    for (i = 0; i < srcLength; i++)                                                         \
    {                                                                                       \
        ((dataType *)pDst)[i] = (i <= dstMaxLength) ? ((dataType *)pSrc)[i] : 0;            \
    }                                                                                       \
}
#endif /* VEOS_USE_MEMCPY_OWN */

/*------------------------------------------------------------------------------------------------*\
  TYPEDEFS
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
  VARIABLE DECLARATIONS
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
  FUNCTION PROTOTYPES
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
  VARIABLE DEFINITIONS
\*------------------------------------------------------------------------------------------------*/

/* Model hash to check consistency from simulator */
VEOS_uint8 ga_VEOS_ModelHash[16] = VEOS_CFG_MODEL_HASH;

/*------------------------------------------------------------------------------------------------*\
  INLINE FUNCTIONS
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
  FUNCTION DEFINITIONS
\*------------------------------------------------------------------------------------------------*/

/**************************************************************************************************\
 *** Converter functions.
\**************************************************************************************************/

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
const VEOS_char * VEOS_ErrorCodeToMessageConverter(VEOS_ApiSuccessType ErrorCode)
{
    switch (ErrorCode) {
        /* VEOS frame error codes */
        case VEOS_E_NOT_OK:
            return "An unspecified error occurred.";
        case VEOS_E_NO_RESULT:
            return "The execution of the function suspended or delayed.";
        case VEOS_E_PENDING:
            return "The function has not run to end yet.";
        case VEOS_E_CALLBACK_ERROR:
            return "Error during execution of the host function.";
        case VEOS_E_API_INITIALIZATION:
            return "Error during API initialization.";
        case VEOS_E_NULL_PARAM:
            return "Parameter value NULL was passed instead of a pointer or array.";
        case VEOS_E_ENUM_VALUE:
            return "Invalid Enumeration value passed.";
        case VEOS_E_BAD_PARAMETER:
            return "Parameter value was invalid or unexpected.";
        case VEOS_E_BUFFER_OVERFLOW:
            return "Buffer overflow.";
        case VEOS_E_BUFFER_UNDERFLOW:
            return "No items to process in a buffer.";
        case VEOS_E_API_DISCONNECTED:
            return "The simulator did not provide an implementation for a specific API.";
        case VEOS_E_NO_IMPLEMENTATION:
            return "The function called does not provide an implementation at the moment (not implemented or switched off by configuration).";

        /* AUTOSAR CAN Interface 3.2.1 error codes */
        case 0x3C0A:	/* CANIF_E_PARAM_CANID */
            return "API service called with invalid parameter (Error code: 0x0A).";
        case 0x3C0B:	/* CANIF_E_PARAM_DLC */
            return "API service called with invalid parameter (Error code: 0x0B).";
        case 0x3C0C:	/* CANIF_E_PARAM_HRH */
            return "API service called with invalid parameter (Error code: 0x0C).";
        case 0x3C0D:	/* CANIF_E_PARAM_LPDU */
            return "API service called with invalid parameter (Error code: 0x0D).";
        case 0x3C0E:	/* CANIF_E_PARAM_CONTROLLER */
            return "API service called with invalid parameter (Error code: 0x0E).";
        case 0x3C0F:	/* CANIF_E_PARAM_WAKEUPSOURCE */
            return "API service called with invalid parameter (Error code: 0x0F).";
        case 0x3C12:	/* CANIF_E_PARAM_TRCVMODE */
            return "API service called with invalid parameter (Error code: 0x12).";
        case 0x3C14:	/* CANIF_E_PARAM_POINTER */
            return "API service called with invalid pointer (Error code: 0x14).";
        case 0x3C1E:	/* CANIF_E_UNINIT */
            return "API service used without module initialization (Error code: 0x1E).";
        case 0x3C28:	/* CANIF_E_NOK_NOSUPPORT */
            return "Requested API operation is not supported (Error code: 0x28).";
        case 0x3C32:	/* CANIF_TRCV_E_TRANSCEIVER */
            return "API service called with invalid transceiver parameter (Error code: 0x32).";
        case 0x3C3C:	/* CANIF_TRCV_E_TRCV_NOT_STANDBY */
            return "API service called with invalid parameter for CAN transceiver operation mode. (Error code: 0x3C)";
        case 0x3C46:	/* CANIF_TRCV_E_TRCV_NOT_NORMAL */
            return "API service called with invalid parameter for CAN transceiver operation mode (Error code: 0x46).";
        case 0x3C50:	/* CANIF_E_INVALID_TXPDUID */
            return "Transmit PDU ID invalid. (Error code: 0x50)";
        case 0x3C5A:	/* CANIF_E_INVALID_RXPDUID */
            return "Receive PDU ID invalid. (Error code: 0x5A)";
        case 0x3C5B:	/* CANIF_E_INVALID_DLC */
            return "Failed DLC Check. (Error code: 0x5B)";
        case 0x3C1F:	/* CANIF_E_STOPPED */
            return "CAN Interface is in STOPPED mode. (Error code: 0x1F)";
        case 0x3C20:	/* CANIF_E_FULL_TX_BUFFER */
            return "Transmit buffers full. (Error code: 0x20)";

        /* AUTOSAR LIN Interface 3.2.1 error codes */
        case 0x3E00:    /* LINIF_E_UNINIT */
            return "API called without initialization of LIN Interface (Error code: 0x00).";
        case 0x3E10:    /* LINIF_E_ALREADY_INITIALIZED */
            return "Initialization API is used when already initialized (Error code: 0x10).";
        case 0x3E20:    /* LINIF_E_NONEXISTENT_CHANNEL */
            return "Referenced channel does not exist (identification is out of range) (Error code: 0x20).";
        case 0x3E30:    /* LINIF_E_PARAMETER */
            return "API service called with wrong parameter (Error code: 0x30).";
        case 0x3E40:    /* LINIF_E_PARAMETER_POINTER */
            return "API service called with invalid pointer (Error code: 0x40).";
        case 0x3E50:    /* LINIF_E_SCHEDULE_OVERFLOW */
            return "Schedule request queue overflow (Error code: 0x50).";
        case 0x3E51:    /* LINIF_E_SCHEDULE_REQUEST_ERROR */
            return "Schedule request made in channel sleep (Error code: 0x51).";
        case 0x3E60:    /* LINIF_E_RESPONSE */
            return "LIN frame error detected (Error code: 0x60).";
        case 0x3E61:    /* LINIF_E_NC_NO_RESPONSE */
            return "Slave did not answer on a node configuration request (Error code: 0x61).";

        /* Unknown error */
        default:
            return "Unknown error.";
    }
}


/**************************************************************************************************\
 *** Copy functions.
\**************************************************************************************************/
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
VEOS_ApiSuccessType VEOS_CopyArray_uint8(const VEOS_uint8 *pSrc, const VEOS_uint32 srcLength, VEOS_uint8 * pDst, const VEOS_uint32 dstMaxLength)
{
    VEOS_COPY_ARRAY_CASTED(pSrc, srcLength, pDst, dstMaxLength, VEOS_uint8);
    return VEOS_E_OK;
}

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
VEOS_ApiSuccessType VEOS_CopyArray_uint16(const VEOS_uint16 *pSrc, const VEOS_uint32 srcLength, VEOS_uint16 * pDst, const VEOS_uint32 dstMaxLength)
{
    VEOS_COPY_ARRAY_CASTED(pSrc, srcLength, pDst, dstMaxLength, VEOS_uint16);
    return VEOS_E_OK;
}

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
VEOS_ApiSuccessType VEOS_CopyArray_uint32(const VEOS_uint32 *pSrc, const VEOS_uint32 srcLength, VEOS_uint32 * pDst, const VEOS_uint32 dstMaxLength)
{
    VEOS_COPY_ARRAY_CASTED(pSrc, srcLength, pDst, dstMaxLength, VEOS_uint32);
    return VEOS_E_OK;
}

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
VEOS_ApiSuccessType VEOS_CopyArray_sint8(const VEOS_sint8 *pSrc, const VEOS_uint32 srcLength, VEOS_sint8 * pDst, const VEOS_uint32 dstMaxLength)
{
    VEOS_COPY_ARRAY_CASTED(pSrc, srcLength, pDst, dstMaxLength, VEOS_sint8);
    return VEOS_E_OK;
}

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
VEOS_ApiSuccessType VEOS_CopyArray_sint16(const VEOS_sint16 *pSrc, const VEOS_uint32 srcLength, VEOS_sint16 * pDst, const VEOS_uint32 dstMaxLength)
{
    VEOS_COPY_ARRAY_CASTED(pSrc, srcLength, pDst, dstMaxLength, VEOS_sint16);
    return VEOS_E_OK;
}

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
VEOS_ApiSuccessType VEOS_CopyArray_sint32(const VEOS_sint32 *pSrc, const VEOS_uint32 srcLength, VEOS_sint32 * pDst, const VEOS_uint32 dstMaxLength)
{
    VEOS_COPY_ARRAY_CASTED(pSrc, srcLength, pDst, dstMaxLength, VEOS_sint32);
    return VEOS_E_OK;
}

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
VEOS_ApiSuccessType VEOS_CopyArray_float32(const VEOS_float32 *pSrc, const VEOS_uint32 srcLength, VEOS_float32 * pDst, const VEOS_uint32 dstMaxLength)
{
    VEOS_COPY_ARRAY_CASTED(pSrc, srcLength, pDst, dstMaxLength, VEOS_float32);
    return VEOS_E_OK;
}

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
VEOS_ApiSuccessType VEOS_CopyArray_float64(const VEOS_float64 *pSrc, const VEOS_uint32 srcLength, VEOS_float64 * pDst, const VEOS_uint32 dstMaxLength)
{
    VEOS_COPY_ARRAY_CASTED(pSrc, srcLength, pDst, dstMaxLength, VEOS_float64);
    return VEOS_E_OK;
}

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
VEOS_ApiSuccessType VEOS_CopyArray_char(const VEOS_char *pSrc, const VEOS_uint32 srcLength, VEOS_char * pDst, const VEOS_uint32 dstMaxLength)
{
    VEOS_COPY_ARRAY_CASTED(pSrc, srcLength, pDst, dstMaxLength, VEOS_char);
    return VEOS_E_OK;
}

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
VEOS_ApiSuccessType VEOS_CopyArray_boolean(const VEOS_boolean *pSrc, const VEOS_uint32 srcLength, VEOS_boolean * pDst, const VEOS_uint32 dstMaxLength)
{
    VEOS_COPY_ARRAY_CASTED(pSrc, srcLength, pDst, dstMaxLength, VEOS_boolean);
    return VEOS_E_OK;
}
#endif /* VEOS_USE_MEMCPY_OWN */

#endif /* VEOS_C */

/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/
