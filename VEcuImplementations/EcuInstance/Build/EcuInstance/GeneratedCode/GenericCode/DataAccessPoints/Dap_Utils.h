/**************************************************************************************************\
 *** Dap_Utils.h
 ***
 *** Utility functions for the Data Access Points.
 ***
 *** Copyright (c) 2012 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef DAP_UTILS_H
#define DAP_UTILS_H

/*------------------------------------------------------------------------------------------------*\
  PARAMETRIZED MACROS
\*------------------------------------------------------------------------------------------------*/
#include <string.h>
#include "Dap.h"

#define Dap_CopyValues_UInt8    Dap_CopyValues_uint8
#define Dap_CopyValues_UInt16   Dap_CopyValues_uint16
#define Dap_CopyValues_UInt32   Dap_CopyValues_uint32
#define Dap_CopyValues_UInt64   Dap_CopyValues_uint64
#define Dap_CopyValues_SInt8    Dap_CopyValues_sint8
#define Dap_CopyValues_SInt16   Dap_CopyValues_sint16
#define Dap_CopyValues_SInt32   Dap_CopyValues_sint32
#define Dap_CopyValues_SInt64   Dap_CopyValues_sint64
#define Dap_CopyValues_Int16    Dap_CopyValues_sint16
#define Dap_CopyValues_Int32    Dap_CopyValues_sint32
#define Dap_CopyValues_Bool     Dap_CopyValues_boolean
#define Dap_CopyValues_bool     Dap_CopyValues_boolean
#define Dap_CopyValues_Float32  Dap_CopyValues_float32
#define Dap_CopyValues_Float64  Dap_CopyValues_float64

typedef uint8 Dap_State;
#define DAP_UNINIT    (Dap_State) 0
#define DAP_INIT      (Dap_State) 1

#define DAP_FUNCTION_READ    (uint8) 0
#define DAP_FUNCTION_WRITE   (uint8) 1
#define DAP_FUNCTION_ENTER   (uint8) 2
#define DAP_FUNCTION_EXIT    (uint8) 3

#define DAP_E_UNINT (uint8) 1


/* To avoid code duplication, the Dap_CopyValues_* functions are implemented as a macro. */
#define DAP_SCALE_ARRAY(datatype, outvalue, buffervalue, factor, offset, length, minValue, maxValue) \
{                                                                                        \
    if((factor == 1.0) && (offset == 0.0))                                               \
    {                                                                                    \
        (void)memcpy((void*)outvalue, (const void *)buffervalue, (size_t)(length * sizeof(datatype))); \
    }                                                                                    \
    else                                                                                 \
    {                                                                                    \
        uint32 j;                                                                        \
        /* factor and offset as "non volatile" increases the loop speed */               \
        float64 tmpFactor = factor;                                                      \
        float64 tmpOffset = offset;                                                      \
        for (j = 0; j < length; j++)                                                     \
        {                                                                                \
            float64 value = ((((float64)(((datatype *)(buffervalue))[j])) * (float64)tmpFactor) + (float64)tmpOffset); \
            if(value < (float64)minValue)                                                \
            {                                                                            \
                ((datatype *) outvalue)[j] = minValue;                                   \
            }                                                                            \
            else if(value > (float64)maxValue)                                           \
            {                                                                            \
                ((datatype *) outvalue)[j] = maxValue;                                   \
            }                                                                            \
            else                                                                         \
            {                                                                            \
                ((datatype *) outvalue)[j] = (datatype) value;                           \
            }                                                                            \
        }                                                                                \
    }                                                                                    \
}                                                                                        \

/*------------------------------------------------------------------------------------------------*\
  FUNCTION PROTOTYPES
\*------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************\
 *** FUNCTION:
 ***     DAP_SCALE_ARRAY_boolean
 ***
 *** DESCRIPTION:
 ***     Service function to copy an uint8 array into another array of the same size.
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     boolean*              outvalue     Pointer to the scaled value.
 ***     boolean*              buffervalue  Pointer to the modified destiniation array.
 ***     float64               factor       Factor that should be used.
 ***     float64               offset       Offset that should be used.
 ***     uint32                length       Length of destination array.
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
extern void DAP_SCALE_ARRAY_boolean(boolean *outvalue, boolean *buffervalue, float64 factor, float64 offset, uint32 length);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Dap_CopyValues_uint8
 ***
 *** DESCRIPTION:
 ***     Service function to copy an uint8 array into another array of the same size.
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     uint8*                srcPtr       Pointer to the unmodified source array.
 ***     uint8*                destPtr      Pointer to the modified destiniation array.
 ***     uint32                length       Number of items to copy from srcPtr to destPtr.
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
extern void Dap_CopyValues_uint8(const uint8 *srcPtr, uint8 *destPtr, uint32 length);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Dap_CopyValues_sint8
 ***
 *** DESCRIPTION:
 ***     Service function to copy an sint8 array into another array of the same size.
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     sint8*                srcPtr       Pointer to the unmodified source array.
 ***     sint8*                destPtr      Pointer to the modified destiniation array.
 ***     sint32                length       Number of items to copy from srcPtr to destPtr.
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
extern void Dap_CopyValues_sint8(const sint8 *srcPtr, sint8 *destPtr, uint32 length);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Dap_CopyValues_unit16
 ***
 *** DESCRIPTION:
 ***     Service function to copy an unit16 array into another array of the same size.
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     unit16*               srcPtr       Pointer to the unmodified source array.
 ***     unit16*               destPtr      Pointer to the modified destiniation array.
 ***     uint32                length       Number of items to copy from srcPtr to destPtr.
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
extern void Dap_CopyValues_uint16(const uint16 *srcPtr, uint16 *destPtr, uint32 length);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Dap_CopyValues_sint16
 ***
 *** DESCRIPTION:
 ***     Service function to copy an sint16 array into another array of the same size.
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     sint16*               srcPtr       Pointer to the unmodified source array.
 ***     sint16*               destPtr      Pointer to the modified destiniation array.
 ***     uint32                length       Number of items to copy from srcPtr to destPtr.
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
extern void Dap_CopyValues_sint16(const sint16 *srcPtr, sint16 *destPtr, uint32 length);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Dap_CopyValues_uint32
 ***
 *** DESCRIPTION:
 ***     Service function to copy an uint32 array into another array of the same size.
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     uint32*               srcPtr       Pointer to the unmodified source array.
 ***     uint32*               destPtr      Pointer to the modified destiniation array.
 ***     uint32                length       Number of items to copy from srcPtr to destPtr.
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
extern void Dap_CopyValues_uint32(const uint32 *srcPtr, uint32 *destPtr, uint32 length);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Dap_CopyValues_uint64
 ***
 *** DESCRIPTION:
 ***     Service function to copy an uint64 array into another array of the same size.
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     uint64*               srcPtr       Pointer to the unmodified source array.
 ***     uint64*               destPtr      Pointer to the modified destiniation array.
 ***     uint32                length       Number of items to copy from srcPtr to destPtr.
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
extern void Dap_CopyValues_uint64(const uint64 *srcPtr, uint64 *destPtr, uint32 length);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Dap_CopyValues_sint32
 ***
 *** DESCRIPTION:
 ***     Service function to copy an sint32 array into another array of the same size.
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     sint32*                srcPtr       Pointer to the unmodified source array.
 ***     sint32*                destPtr      Pointer to the modified destiniation array.
 ***     uint32                length       Number of items to copy from srcPtr to destPtr.
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
extern void Dap_CopyValues_sint32(const sint32 *srcPtr, sint32 *destPtr, uint32 length);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Dap_CopyValues_sint64
 ***
 *** DESCRIPTION:
 ***     Service function to copy an sint32 array into another array of the same size.
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     sint64*                srcPtr       Pointer to the unmodified source array.
 ***     sint64*                destPtr      Pointer to the modified destiniation array.
 ***     uint32                 length       Number of items to copy from srcPtr to destPtr.
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
extern void Dap_CopyValues_sint64(const sint64 *srcPtr, sint64 *destPtr, uint32 length);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Dap_CopyValues_float32
 ***
 *** DESCRIPTION:
 ***     Service function to copy an float32 array into another array of the same size.
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     float32*                srcPtr       Pointer to the unmodified source array.
 ***     float32*                destPtr      Pointer to the modified destiniation array.
 ***     uint32                length       Number of items to copy from srcPtr to destPtr.
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
extern void Dap_CopyValues_float32(const float32 *srcPtr, float32 *destPtr, uint32 length);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Dap_CopyValues_float64
 ***
 *** DESCRIPTION:
 ***     Service function to copy an float64 array into another array of the same size.
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     float64*                srcPtr       Pointer to the unmodified source array.
 ***     float64*                destPtr      Pointer to the modified destiniation array.
 ***     uint32                length       Number of items to copy from srcPtr to destPtr.
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
extern void Dap_CopyValues_float64(const float64 *srcPtr, float64 *destPtr, uint32 length);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Dap_CopyValues_boolean
 ***
 *** DESCRIPTION:
 ***     Service function to copy an boolean array into another array of the same size.
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     boolean*                srcPtr       Pointer to the unmodified source array.
 ***     boolean*                destPtr      Pointer to the modified destiniation array.
 ***     uint32                length       Number of items to copy from srcPtr to destPtr.
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
extern void Dap_CopyValues_boolean(const boolean *srcPtr, boolean *destPtr, uint32 length);

#ifdef __cplusplus
}
#endif

#endif /* DAP_UTILS_H */

/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/
