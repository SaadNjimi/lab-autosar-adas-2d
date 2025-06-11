/**************************************************************************************************\
 *** Dap.c
 ***
 *** Generic implementation for Data Access Points.
 ***
 *** Copyright (c) 2012 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef DAP_C
#define DAP_C

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/

#include "Platform_Types.h"
#include "Std_Types.h"

#include "VEcuAppFrm.h"
#include "VEOS.h"

#include "Dap.h"
#include "Dap_Utils.h"

/*------------------------------------------------------------------------------------------------*\
  PARAMETRIZED MACROS
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
  FORWARD DECLARATIONS
\*------------------------------------------------------------------------------------------------*/

Dap_State g_DapState = DAP_UNINIT;

#ifdef __cplusplus
extern "C" {
#endif

extern void Dap_InitInportSignalBuffers(void);
extern void Dap_InitOutportSignalBuffers(void);

#ifdef __cplusplus
}
#endif

/*------------------------------------------------------------------------------------------------*\
  FUNCTION DEFINITIONS
\*------------------------------------------------------------------------------------------------*/

/**************************************************************************************************\
 *** FUNCTION:
 ***     Dap_Init
 ***
 *** DESCRIPTION:
 ***     Service to initialize the Data Access Points.
 ***
 *** PARAMETERS:
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
void Dap_Init(void)
{
#ifdef VEOS_DEBUG
    VEOS_MsgApi_LogMessage(DAP_MODULE_NAME, __FUNCTION__, "Dap_Init() was called.");
#endif

    g_DapState = DAP_INIT;

    Dap_InitInportSignalBuffers();
    Dap_InitOutportSignalBuffers();
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     Dap_DeInit
 ***
 *** DESCRIPTION:
 ***     Service to deinitialize the Data Access Points.
 ***
 *** PARAMETERS:
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
void Dap_DeInit(void)
{
#ifdef VEOS_DEBUG
    VEOS_MsgApi_LogMessage(DAP_MODULE_NAME, __FUNCTION__, "Dap_DeInit() was called.");
#endif

    g_DapState = DAP_UNINIT;
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     Dap_GetVersionInfo
 ***
 *** DESCRIPTION:
 ***     This service returns the version information of this module. The version information
 ***     includes the module Id, the vendor Id, and a vendor specific version numbers.
 ***
 *** PARAMETERS:
 ***     Type                  Name         Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     Std_VersionInfoType*  versioninfo  Pointer to where to store the version information of
 ***                                        this module.
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
void Dap_GetVersionInfo(Std_VersionInfoType *versioninfo)
{
#ifdef VEOS_DEBUG
    VEOS_MsgApi_LogMessage(DAP_MODULE_NAME, __FUNCTION__, "Dap_GetVersionInfo() was called.");
#endif

    versioninfo->vendorID = DAP_VENDOR_ID;
    versioninfo->moduleID = DAP_MODULE_ID;
    versioninfo->sw_major_version = DAP_SW_MAJOR_VERSION;
    versioninfo->sw_minor_version = DAP_SW_MINOR_VERSION;
    versioninfo->sw_patch_version = DAP_SW_PATCH_VERSION;
}


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
void DAP_SCALE_ARRAY_boolean(boolean *outvalue, boolean *buffervalue, float64 factor, float64 offset, uint32 length)
{
    if((factor == 1.0) && (offset == 0.0))
    {
        (void)memcpy((void*)outvalue, (const void *)buffervalue, (size_t)(length));
    }
    else
    {
        uint32 j;
        /* factor and offset as "non volatile" increases the loop speed */ 
        float64 tmpFactor = factor;
        float64 tmpOffset = offset;
        for (j = 0; j < length; j++)
        {
            float64 value = ((((float64)(((boolean *)(buffervalue))[j])) * (float64)tmpFactor) + (float64)tmpOffset);
            if(value != 0.0) 
            { 
                ((boolean *)outvalue)[j] = 1; 
            }
            else 
            { 
                ((boolean *)outvalue)[j] = 0;
            }
        }
    }            
}

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
void Dap_CopyValues_uint8(const uint8 *srcPtr, uint8 *destPtr, uint32 length)
{
    (void)memcpy(destPtr, srcPtr, (size_t)(length * sizeof(uint8)));
}

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
void Dap_CopyValues_sint8(const sint8 *srcPtr, sint8 *destPtr, uint32 length)
{
    (void)memcpy(destPtr, srcPtr, (size_t)(length * sizeof(sint8)));
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     Dap_CopyValues_uint16
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
void Dap_CopyValues_uint16(const uint16 *srcPtr, uint16 *destPtr, uint32 length)
{
    (void)memcpy(destPtr, srcPtr, (size_t)(length * sizeof(uint16)));
}

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
void Dap_CopyValues_sint16(const sint16 *srcPtr, sint16 *destPtr, uint32 length)
{
    (void)memcpy(destPtr, srcPtr, (size_t)(length * sizeof(sint16)));
}

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
void Dap_CopyValues_uint32(const uint32 *srcPtr, uint32 *destPtr, uint32 length)
{
    (void)memcpy(destPtr, srcPtr, (size_t)(length * sizeof(uint32)));
}

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
void Dap_CopyValues_uint64(const uint64 *srcPtr, uint64 *destPtr, uint32 length)
{
    (void)memcpy(destPtr, srcPtr, (size_t)(length * sizeof(uint64)));
}

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
 ***     uint32                 length       Number of items to copy from srcPtr to destPtr.
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
void Dap_CopyValues_sint32(const sint32 *srcPtr, sint32 *destPtr, uint32 length)
{
    (void)memcpy(destPtr, srcPtr, (size_t)(length * sizeof(sint32)));
}

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
void Dap_CopyValues_sint64(const sint64 *srcPtr, sint64 *destPtr, uint32 length)
{
    (void)memcpy(destPtr, srcPtr, (size_t)(length * sizeof(sint64)));
}

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
void Dap_CopyValues_float32(const float32 *srcPtr, float32 *destPtr, uint32 length)
{
    (void)memcpy(destPtr, srcPtr, (size_t)(length * sizeof(float32)));
}

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
void Dap_CopyValues_float64(const float64 *srcPtr, float64 *destPtr, uint32 length)
{
    (void)memcpy(destPtr, srcPtr, (size_t)(length * sizeof(float64)));
}

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
void Dap_CopyValues_boolean(const boolean *srcPtr, boolean *destPtr, uint32 length)
{
    (void)memcpy(destPtr, srcPtr, (size_t)(length * sizeof(boolean)));
}

void Dap_CheckInitialized(uint8 functionType)
{
    if (g_DapState != DAP_INIT)
    {
        VEcuAppFrm_ReportError(VEcuAppFrm_DevelopmentError, DAP_MODULE_ID, 0, functionType, DAP_E_UNINT);
    }
}

#endif /* DAP_C */

/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/
