/**************************************************************************************************\
 *** VEOS_Impl.c
 ***
 *** VEOS frame hand-coded implementation part.
 ***
 *** Copyright 2021 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef VEOS_IMPL_C
#define VEOS_IMPL_C

/*------------------------------------------------------------------------------------------------*\
  DEFINES (OPT)
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/

#include "VEOS.h"
#include "VEOS_Version.h"
#include "VEOS_AppFrm.h"

#include "stdio.h"
#include "stdarg.h"
#include "stddef.h"

/*------------------------------------------------------------------------------------------------*\
  DEFINES
\*------------------------------------------------------------------------------------------------*/

#ifndef VEOS_GUARD_PRIVATE_API_OK
#error Cannot guarantee correct implementation: No access to private implementation.
#endif /* VEOS_GUARD_PRIVATE_API_OK */

#ifndef VEOS_GUARD_BUSAPIUSERS_OK
#error Cannot guarantee correct implementation: No access to bus API implementation.
#endif /* VEOS_GUARD_BUSAPIUSERS_OK */

#if (VEOS_NUMBER_OF_CACHED_IO_EVENTS > 0)
#define VEOS_IO_EVENT_CACHE_ENABLED
#endif /* (VEOS_NUMBER_OF_CACHED_IO_EVENTS > 0) */

#define VEOS_IMPL_SERVICE_FCN_FORMAT_MESSAGE                0x30

/*------------------------------------------------------------------------------------------------*\
  PARAMETERIZED MACROS
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
  TYPEDEFS
\*------------------------------------------------------------------------------------------------*/

#ifndef VEOS_NO_DTSIZE_CHECK

#if defined __GNUC__ && !defined __cplusplus
#undef static_assert
#define static_assert _Static_assert
#endif

static_assert(sizeof(VEOS_boolean) == 1, "sizeof(VEOS_boolean) must be 1");
static_assert(sizeof(VEOS_uint8) == 1, "sizeof(VEOS_uint8) must be 1");
static_assert(sizeof(VEOS_sint8) == 1, "sizeof(VEOS_sint8) must be 1");
static_assert(sizeof(VEOS_uint16) == 2, "sizeof(VEOS_uint16) must be 2");
static_assert(sizeof(VEOS_sint16) == 2, "sizeof(VEOS_sint16) must be 2");
static_assert(sizeof(VEOS_uint32) == 4, "sizeof(VEOS_uint32) must be 4");
static_assert(sizeof(VEOS_sint32) == 4, "sizeof(VEOS_sint32) must be 4");
static_assert(sizeof(VEOS_uint64) == 8, "sizeof(VEOS_uint64) must be 8");
static_assert(sizeof(VEOS_sint64) == 8, "sizeof(VEOS_sint64) must be 8");
static_assert(sizeof(VEOS_float32) == 4, "sizeof(VEOS_float32) must be 4");
static_assert(sizeof(VEOS_float64) == 8, "sizeof(VEOS_float64) must be 8");

#ifndef VEOS_NO_DTSIZE_CHECK_PTR
#if (VEOS_CPU_TYPE == VEOS_CPU_TYPE_32)
static_assert(sizeof(void*) == 4, "size of pointer must be 4 bytes");
#elif (VEOS_CPU_TYPE == VEOS_CPU_TYPE_64)
static_assert(sizeof(void*) == 8, "size of pointer must be 8 bytes");
#else
#error "Unknown VEOS_CPU_TYPE"
#endif /* VEOS_CPU_TYPE */
#endif /* VEOS_NO_DTSIZE_CHECK_PTR */

#ifndef VEOS_NO_DTSIZE_CHECK_ENUM
typedef enum {
    VEOS_DataTypeCheck_SignedByteEnum_Low = -1,
    VEOS_DataTypeCheck_SignedByteEnum_High = 1
} VEOS_DataTypeCheck_SignedByteEnum;

static_assert(sizeof(VEOS_DataTypeCheck_SignedByteEnum) == sizeof(VEOS_sint32), "8 bit signed enum must have the same size as int32");

typedef enum {
    VEOS_DataTypeCheck_SignedShortEnum_Low = -129,
    VEOS_DataTypeCheck_SignedShortEnum_High = 130
} VEOS_DataTypeCheck_SignedShortEnum;

static_assert(sizeof(VEOS_DataTypeCheck_SignedShortEnum) == sizeof(VEOS_sint32), "16 bit signed enum must have the same size as int32");

typedef enum {
    VEOS_DataTypeCheck_SignedIntEnum_Low = -32770,
    VEOS_DataTypeCheck_SignedIntEnum_High = 32770
} VEOS_DataTypeCheck_SignedIntEnum;

static_assert(sizeof(VEOS_DataTypeCheck_SignedShortEnum) == sizeof(VEOS_sint32), "32 bit signed enum must have the same size as int32");

typedef enum {
    VEOS_DataTypeCheck_UnsignedByteEnum_Low = 0,
    VEOS_DataTypeCheck_UnsignedByteEnum_High = 1
} VEOS_DataTypeCheck_UnsignedByteEnum;

static_assert(sizeof(VEOS_DataTypeCheck_UnsignedByteEnum) == sizeof(VEOS_uint32), "8 bit unsigned enum must have the same size as uint32");

typedef enum {
    VEOS_DataTypeCheck_UnsignedShortEnum_Low = 0,
    VEOS_DataTypeCheck_UnsignedShortEnum_High = 270
} VEOS_DataTypeCheck_UnsignedShortEnum;

static_assert(sizeof(VEOS_DataTypeCheck_UnsignedShortEnum) == sizeof(VEOS_uint32), "16 bit unsigned enum must have the same size as uint32");

typedef enum {
    VEOS_DataTypeCheck_UnsignedIntEnum_Low = 0,
    VEOS_DataTypeCheck_UnsignedIntEnum_High = 65580
} VEOS_DataTypeCheck_UnsignedIntEnum;

static_assert(sizeof(VEOS_DataTypeCheck_UnsignedIntEnum) == sizeof(VEOS_uint32), "32 bit unsigned enum must have the same size as uint32");

#endif /* !VEOS_NO_DTSIZE_CHECK_ENUM */


#define VEOS_STATIC_ASSERT_STRUCT_ALIGN(dt,align)                                                                                  \
    typedef struct                                                                                                                 \
    {                                                                                                                              \
        VEOS_uint8 pad;                                                                                                            \
        dt value;                                                                                                                  \
    } VEOS_DataTypeCheck_Align_##dt;                                                                                               \
                                                                                                                                   \
    static_assert(offsetof(VEOS_DataTypeCheck_Align_##dt, value) == (align), "Struct alignment of " #dt "member must be " #align);

VEOS_STATIC_ASSERT_STRUCT_ALIGN(VEOS_boolean, 1)
VEOS_STATIC_ASSERT_STRUCT_ALIGN(VEOS_sint8, 1)
VEOS_STATIC_ASSERT_STRUCT_ALIGN(VEOS_uint8, 1)
VEOS_STATIC_ASSERT_STRUCT_ALIGN(VEOS_sint16, 2)
VEOS_STATIC_ASSERT_STRUCT_ALIGN(VEOS_uint16, 2)
VEOS_STATIC_ASSERT_STRUCT_ALIGN(VEOS_sint32, 4)
VEOS_STATIC_ASSERT_STRUCT_ALIGN(VEOS_uint32, 4)
VEOS_STATIC_ASSERT_STRUCT_ALIGN(VEOS_float32, 4)

#if !defined(__linux__) || (defined(__linux__) && defined(__x86_64__))
VEOS_STATIC_ASSERT_STRUCT_ALIGN(VEOS_sint64, 8)
VEOS_STATIC_ASSERT_STRUCT_ALIGN(VEOS_uint64, 8)
VEOS_STATIC_ASSERT_STRUCT_ALIGN(VEOS_float64, 8)
#else
VEOS_STATIC_ASSERT_STRUCT_ALIGN(VEOS_sint64, 4)
VEOS_STATIC_ASSERT_STRUCT_ALIGN(VEOS_uint64, 4)
VEOS_STATIC_ASSERT_STRUCT_ALIGN(VEOS_float64, 4)
#endif

#endif /* !VEOS_NO_DTSIZE_CHECK */

/*------------------------------------------------------------------------------------------------*\
  VARIABLE DECLARATIONS
\*------------------------------------------------------------------------------------------------*/

#ifdef VEOS_IO_EVENT_CACHE_ENABLED
/* Cache for I/O events */
static struct
{
    /* The number of cached I/O events */
    VEOS_uint8 CachedEventsCount;
    /* The I/O events currently cached */
    VEOS_uint32 CachedEvents[VEOS_NUMBER_OF_CACHED_IO_EVENTS];
} g_VEOS_Impl_IoEventCache;
#endif /* VEOS_IO_EVENT_CACHE_ENABLED */

/*------------------------------------------------------------------------------------------------*\
  FUNCTION PROTOTYPES
\*------------------------------------------------------------------------------------------------*/

#if (VEOS_ON == VEOS_APPFRM_IOSUPPORT)
/**************************************************************************************************\
 *** FUNCTION:
 ***     VEOS_Impl_HandleCachedIoEvents
 ***
 *** DESCRIPTION:
 ***     Calls the I/O handler for all cached I/O events.
 ***
 *** PARAMETERS:
 ***     -/-
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any error
 ***                            occurs, an error code will be the result. See VEOS_Types.h for
 ***                            details.
\**************************************************************************************************/
static VEOS_ApiSuccessType VEOS_Impl_AppendCachedIoEvent(const VEOS_uint32 eventId);

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEOS_Impl_HandleCachedIoEvents
 ***
 *** DESCRIPTION:
 ***     Calls the I/O handler for all cached I/O events.
 ***
 *** PARAMETERS:
 ***     -/-
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any error
 ***                            occurs, an error code will be the result. See VEOS_Types.h for
 ***                            details.
\**************************************************************************************************/
static VEOS_ApiSuccessType VEOS_Impl_HandleCachedIoEvents(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEOS_Impl_ClearCachedIoEvents
 ***
 *** DESCRIPTION:
 ***     Clears the I/O event cache without calling the I/O event handler.
 ***
 *** PARAMETERS:
 ***     -/-
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any error
 ***                            occurs, an error code will be the result. See VEOS_Types.h for
 ***                            details.
\**************************************************************************************************/
static VEOS_ApiSuccessType VEOS_Impl_ClearCachedIoEvents(void);
#endif /* VEOS_ON == VEOS_APPFRM_IOSUPPORT */

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEOS_Impl_Start
 ***
 *** DESCRIPTION:
 ***     Performs a start-up on the VEOS Frame hand-coded part.
 ***
 *** PARAMETERS:
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
static void VEOS_Impl_Start(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEOS_Impl_Stop
 ***
 *** DESCRIPTION:
 ***     Performs a shutdown on the VEOS Frame hand-coded part.
 ***
 *** PARAMETERS:
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
static void VEOS_Impl_Stop(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEOS_Impl_CheckEventStatus
 ***
 *** DESCRIPTION:
 ***     Checks and updates the status of the current event.
 ***
 *** PARAMETERS:
 ***     Type                     Name               Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     VEOS_ApiSuccessType *    pSuccess           The status variable to update.
 ***     const VEOS_boolean       AllowNoImplResult  If set to VEOS_TRUE, the result code VEOS_E_NO_IMPLEMENTATION
 ***                                                 will be allowed as return code as well.
 ***
 *** RETURNS:
 ***     VEOS_boolean
 ***            VEOS_TRUE:   If the current result allows to continue execution.
 ***            VEOS_FALSE:  If the current result does not allow execution.
\**************************************************************************************************/
static VEOS_boolean VEOS_Impl_CheckEventStatus(VEOS_ApiSuccessType* pSuccess, const VEOS_boolean AllowNoImplResult);

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEOS_Target_HandleDownload
 ***
 *** DESCRIPTION:
 ***     Target specific function for VEOS Frame to initialize the target or its run-time during
 ***     download phase.
 ***
 *** PARAMETERS:
 ***     Type            Name               Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     -/-
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any error
 ***                            occurs, an error code will be the result. See VEOS_Types.h for
 ***                            details.
\**************************************************************************************************/
extern VEOS_ApiSuccessType VEOS_Target_HandleDownload(void);

#ifdef __cplusplus
}
#endif

/*------------------------------------------------------------------------------------------------*\
  VARIABLE DEFINITIONS
\*------------------------------------------------------------------------------------------------*/

/* The runtime configuration type for this instance. */
static VEOS_RuntimeConfigType g_VEOS_Impl_RuntimeConfig =
{
    VEOS_TRUE, /* Handling I/O events is currently set to ON */
    VEOS_State_Off, /* The application is currently switched off. */
};

/*------------------------------------------------------------------------------------------------*\
  INLINE FUNCTIONS
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
  FUNCTION DEFINITIONS
\*------------------------------------------------------------------------------------------------*/

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEOS_BusApiImpl_MessageReceivedEvent
 ***
 *** DESCRIPTION:
 ***     Hand-coded implementation of API function MessageReceivedEvent for BusApi of VEOS Frame
 ***
 *** PARAMETERS:
 ***     Type                                 Name      Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     const VEOS_BusApi_MessageHandleType  Msg       Incoming argument: The handle of the
 ***                                                    message.
 ***     const VEOS_BusApi_InterfaceType      Protocol  Incoming argument: The simulated hardware
 ***                                                    interface type.
 ***     const VEOS_uint32                    MsgId     Incoming argument: The id of the message
 ***                                                    transferred.
 ***     const VEOS_uint32                    CbkArg    Incoming argument: The custom callback
 ***                                                    argument for this event.
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any
 ***                                      error occurs, an error code will be the result. See
 ***                                      VEOS_Types.h for details.
\**************************************************************************************************/
VEOS_ApiSuccessType VEOS_BusApiImpl_MessageReceivedEvent(
    const VEOS_BusApi_MessageHandleType Msg,
    const VEOS_BusApi_InterfaceType Protocol,
    const VEOS_uint32 MsgId,
    const VEOS_uint32 CbkArg
)
{
#if (VEOS_BUSAPI_SIMULATION == VEOS_ON)
    VEOS_ApiSuccessType result = VEOS_SIMAPI_ASYNCSIMEVENT_HANDLER();

    VEOS_FRM_DBG_METHOD_START("BusApi", __FUNCTION__);
#endif /* VEOS_BUSAPI_SIMULATION == VEOS_ON */

    VEOS_FRM_DBG_METHOD_PARAM("BusApi", __FUNCTION__, Msg);
    VEOS_FRM_DBG_METHOD_PARAM("BusApi", __FUNCTION__, Protocol);
    VEOS_FRM_DBG_METHOD_PARAM("BusApi", __FUNCTION__, MsgId);
    VEOS_FRM_DBG_METHOD_PARAM("BusApi", __FUNCTION__, CbkArg);

    (void)VEOS_SimApi_ResetCurrentEventStatus();

#if (VEOS_BUSAPI_SIMULATION == VEOS_ON)
    switch (Protocol)
    {
#if (VEOS_BUSAPI_CANSUPPORT == VEOS_ON)
    case VEOS_BusApi_InterfaceType_CAN:
        result = VEOS_BUSAPI_CANMSGHANDLER(Msg, MsgId, CbkArg);
        break;
#endif /* VEOS_BUSAPI_CANSUPPORT == VEOS_ON */
#if (VEOS_BUSAPI_LINSUPPORT == VEOS_ON)
    case VEOS_BusApi_InterfaceType_LIN:
        result = VEOS_BUSAPI_LINMSGHANDLER(Msg, MsgId, CbkArg);
        break;
#endif /* VEOS_BUSAPI_LINSUPPORT == VEOS_ON */
#if (VEOS_BUSAPI_IDIFSUPPORT == VEOS_ON)
    case VEOS_BusApi_InterfaceType_Idealized:
        result = VEOS_BUSAPI_IDIFMSGHANDLER(Msg, MsgId, CbkArg);
        break;
#endif /* VEOS_BUSAPI_IDIFSUPPORT == VEOS_ON */
    default:
        result = VEOS_E_NO_IMPLEMENTATION;
        break;
    }

    (void)VEOS_Impl_CheckEventStatus(&result, VEOS_FALSE);

    VEOS_FRM_DBG_METHOD_END("BusApi", __FUNCTION__);

#ifdef VEOS_DEBUG
    VEOS_FRM_DBG_METHOD_RESULT("BusApi", __FUNCTION__, result);
#endif

    return result;
#else
    return VEOS_E_NO_IMPLEMENTATION;
#endif /* VEOS_BUSAPI_SIMULATION == VEOS_ON */
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEOS_BusApiImpl_LinHeaderReceivedEvent
 ***
 *** DESCRIPTION:
 ***     Hand-coded implementation of API function LinHeaderReceivedEvent for BusApi of VEOS Frame
 ***
 *** PARAMETERS:
 ***     Type                                 Name       Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     const VEOS_BusApi_MessageHandleType  MsgHandle  Incoming argument: The handle of the
 ***                                                     message.
 ***     const VEOS_uint32                    MsgId      Incoming argument: The id of the message
 ***                                                     transferred.
 ***     const VEOS_uint32                    CbkArg     Incoming argument: The argument for the
 ***                                                     callback handler function.
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any
 ***                                      error occurs, an error code will be the result. See
 ***                                      VEOS_Types.h for details.
\**************************************************************************************************/
VEOS_ApiSuccessType VEOS_BusApiImpl_LinHeaderReceivedEvent(
    const VEOS_BusApi_MessageHandleType MsgHandle,
    const VEOS_uint32 MsgId,
    const VEOS_uint32 CbkArg
)
{
#if (VEOS_BUSAPI_LINSUPPORT == VEOS_ON)
    VEOS_ApiSuccessType result = VEOS_SIMAPI_ASYNCSIMEVENT_HANDLER();

    VEOS_FRM_DBG_METHOD_START("BusApi", __FUNCTION__);
#endif /* VEOS_BUSAPI_LINSUPPORT == VEOS_ON */

    VEOS_FRM_DBG_METHOD_PARAM("BusApi", __FUNCTION__, MsgHandle);
    VEOS_FRM_DBG_METHOD_PARAM("BusApi", __FUNCTION__, MsgId);
    VEOS_FRM_DBG_METHOD_PARAM("BusApi", __FUNCTION__, CbkArg);

    (void)VEOS_SimApi_ResetCurrentEventStatus();

#if (VEOS_BUSAPI_LINSUPPORT == VEOS_ON)
    result = VEOS_BUSAPI_LINHDRHANDLER(MsgHandle, MsgId, CbkArg);

    (void)VEOS_Impl_CheckEventStatus(&result, VEOS_FALSE);

    VEOS_FRM_DBG_METHOD_END("BusApi", __FUNCTION__);

#ifdef VEOS_DEBUG
    VEOS_FRM_DBG_METHOD_RESULT("BusApi", __FUNCTION__, result);
#endif

    return result;
#else
    return VEOS_E_NO_IMPLEMENTATION;
#endif /* VEOS_BUSAPI_LINSUPPORT == VEOS_ON */
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEOS_BusApiImpl_ChannelStatusEvent
 ***
 *** DESCRIPTION:
 ***     Hand-coded implementation of API function ChannelStatusEvent for BusApi of VEOS Frame
 ***
 *** PARAMETERS:
 ***     Type                                   Name      Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     const VEOS_BusApi_ConnectorHandleType  Channel   Incoming argument: The handle of the
 ***                                                      channel.
 ***     const VEOS_BusApi_InterfaceType        Protocol  Incoming argument: The simulated hardware
 ***                                                      interface.
 ***     const VEOS_BusApi_ConnectorStatusType  Status    Incoming argument: The status of the
 ***                                                      monitor.
 ***     const VEOS_uint32                      CbkArg    Incoming argument: The custom callback
 ***                                                      argument for this event.
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any
 ***                                      error occurs, an error code will be the result. See
 ***                                      VEOS_Types.h for details.
\**************************************************************************************************/
VEOS_ApiSuccessType VEOS_BusApiImpl_ChannelStatusEvent(
    const VEOS_BusApi_ConnectorHandleType Channel,
    const VEOS_BusApi_InterfaceType Protocol,
    const VEOS_BusApi_ConnectorStatusType Status,
    const VEOS_uint32 CbkArg
)
{
#if (VEOS_BUSAPI_SIMULATION == VEOS_ON)
    VEOS_ApiSuccessType result = VEOS_SIMAPI_ASYNCSIMEVENT_HANDLER();

    VEOS_FRM_DBG_METHOD_START("BusApi", __FUNCTION__);
#endif /* VEOS_BUSAPI_SIMULATION == VEOS_ON */

    VEOS_FRM_DBG_METHOD_PARAM("BusApi", __FUNCTION__, Channel);
    VEOS_FRM_DBG_METHOD_PARAM("BusApi", __FUNCTION__, Protocol);
    VEOS_FRM_DBG_METHOD_PARAM("BusApi", __FUNCTION__, Status);
    VEOS_FRM_DBG_METHOD_PARAM("BusApi", __FUNCTION__, CbkArg);

    (void)VEOS_SimApi_ResetCurrentEventStatus();

#if (VEOS_BUSAPI_SIMULATION == VEOS_ON)
    switch (Protocol)
    {
#if (VEOS_BUSAPI_CANSUPPORT == VEOS_ON)
    case VEOS_BusApi_InterfaceType_CAN:
        result = VEOS_BUSAPI_CANCHHANDLER(Channel, Status, CbkArg);
        break;
#endif /* VEOS_BUSAPI_CANSUPPORT == VEOS_ON */
#if (VEOS_BUSAPI_LINSUPPORT == VEOS_ON)
    case VEOS_BusApi_InterfaceType_LIN:
        result = VEOS_BUSAPI_LINCHHANDLER(Channel, Status, CbkArg);
        break;
#endif /* VEOS_BUSAPI_LINSUPPORT == VEOS_ON */
#if (VEOS_BUSAPI_IDIFSUPPORT == VEOS_ON)
    case VEOS_BusApi_InterfaceType_Idealized:
        result = VEOS_BUSAPI_IDIFCHHANDLER(Channel, Status, CbkArg);
        break;
#endif /* VEOS_BUSAPI_IDIFSUPPORT == VEOS_ON) */
    default:
        result = VEOS_E_NO_IMPLEMENTATION;
        break;
    }

    (void)VEOS_Impl_CheckEventStatus(&result, VEOS_FALSE);

    VEOS_FRM_DBG_METHOD_END("BusApi", __FUNCTION__);

#ifdef VEOS_DEBUG
    VEOS_FRM_DBG_METHOD_RESULT("BusApi", __FUNCTION__, result);
#endif

    return result;
#else
    return VEOS_E_NO_IMPLEMENTATION;
#endif /* VEOS_BUSAPI_SIMULATION == VEOS_ON */
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEOS_SimApiImpl_CallIoHandler
 ***
 *** DESCRIPTION:
 ***     Hand-coded implementation of API function CallIoHandler for SimApi of VEOS Frame
 ***
 *** PARAMETERS:
 ***     Type               Name       Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     const VEOS_uint32  ioEventId  Incoming argument:
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any error
 ***                            occurs, an error code will be the result. See VEOS_Types.h for
 ***                            details.
\**************************************************************************************************/
VEOS_ApiSuccessType VEOS_SimApiImpl_CallIoHandler(const VEOS_uint32 ioEventId)
{
#if (VEOS_ON == VEOS_APPFRM_IOSUPPORT)
    VEOS_ApiSuccessType result = VEOS_E_PENDING;

    VEOS_FRM_DBG_METHOD_START("SimApi", __FUNCTION__);

    VEOS_FRM_DBG_METHOD_PARAM("SimApi", __FUNCTION__, ioEventId);

    if (VEOS_IOEVENTID_INVALID == ioEventId)
    {
        return VEOS_E_BAD_PARAMETER;
    }

    if (VEOS_TRUE == g_VEOS_Impl_RuntimeConfig.ioEventsEnabled)
    {
        result = VEOS_SIMAPI_IOHANDLER(ioEventId);
    }
    else
    {
        result = VEOS_Impl_AppendCachedIoEvent(ioEventId);
    }

    VEOS_FRM_DBG_METHOD_END("SimApi", __FUNCTION__);

#ifdef VEOS_DEBUG
    VEOS_FRM_DBG_METHOD_RESULT("SimApi", __FUNCTION__, result);
#endif

    return result;
#else
    VEOS_DBG_UNUSED_PARAM(ioEventId);

    return VEOS_E_NO_IMPLEMENTATION;
#endif  /* VEOS_ON == VEOS_APPFRM_IOSUPPORT */
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEOS_SimApiImpl_DownloadEvent
 ***
 *** DESCRIPTION:
 ***     Hand-coded implementation of API function Initialize for SimApi of VEOS Frame
 ***
 *** PARAMETERS:
 ***     -/-
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any error
 ***                            occurs, an error code will be the result. See VEOS_Types.h for
 ***                            details.
\**************************************************************************************************/
VEOS_ApiSuccessType VEOS_SimApiImpl_DownloadEvent(
    const VEOS_uint32 ApplicationId,
    const void* const TargetAddressBase,
    const VEOS_uint32 ModelConfiguration,
    VEOS_uint32* const ModelCheckResult)
{
    VEOS_ApiSuccessType result = VEOS_E_PENDING;

    VEOS_FRM_DBG_METHOD_START("SimApi", __FUNCTION__);

    g_VEOS_ApplicationId = ApplicationId;
    gp_VEOS_TargetAddressBase = (void*)TargetAddressBase;

    VEOS_MdlInitApiImpl_CheckModel(ModelConfiguration, ModelCheckResult);

    if (VEOS_State_Off == g_VEOS_Impl_RuntimeConfig.state)
    {
        (void)VEOS_SimApi_ResetCurrentEventStatus();
        result = VEOS_API_Download();

#if DSCAN_API == VEOS_ON
        if (VEOS_E_OK == result)
        {
            result = DsCanLib_Initialize();
        }
#endif /* DSCAN_API == VEOS_ON */

#if DSLIN_API == VEOS_ON
        if (VEOS_E_OK == result)
        {
            result = DsLinLib_Initialize();
        }
#endif /* DSLIN_API == VEOS_ON */

#if DSETH_API == VEOS_ON
        if (VEOS_E_OK == result)
        {
            result = DsEthLib_Initialize();
        }
#endif /* DSETH_API == VEOS_ON */

#if DSSOCKET_API == VEOS_ON
        if (VEOS_E_OK == result)
        {
            result = DsSocketLib_Initialize();
        }
#endif /* DSSOCKET_API == VEOS_ON */

#if DSPCAP_API == VEOS_ON
        if (VEOS_E_OK == result)
        {
            result = DsPcapLib_Initialize();
        }
#endif /* DSPCAP_API == VEOS_ON */

        if (VEOS_E_OK == result)
        {
            result = VEOS_SIMAPI_DOWNLOADHANDLER();

#if defined(VEOS_TARGET_HOSTPC32_MSVC) || defined(VEOS_TARGET_HOSTPC64_MSVC)
            if (VEOS_E_OK == result)
            {
                result = VEOS_Target_HandleDownload();
            }
#endif /* VEOS_TARGET_HOSTPC32_MSVC */

            if (VEOS_TRUE == VEOS_Impl_CheckEventStatus(&result, VEOS_TRUE))
            {
                g_VEOS_Impl_RuntimeConfig.state = VEOS_State_Downloaded;
            }
        }
    }
    else
    {
        result = VEOS_E_NOT_OK;
    }

    VEOS_FRM_DBG_METHOD_END("SimApi", __FUNCTION__);

#ifdef VEOS_DEBUG
    VEOS_FRM_DBG_METHOD_RESULT("SimApi", __FUNCTION__, result);
#endif

    return result;
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEOS_SimApiImpl_StartEvent
 ***
 *** DESCRIPTION:
 ***     Hand-coded implementation of API function Start for SimApi of VEOS Frame
 ***
 *** PARAMETERS:
 ***     -/-
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any error
 ***                            occurs, an error code will be the result. See VEOS_Types.h for
 ***                            details.
\**************************************************************************************************/
VEOS_ApiSuccessType VEOS_SimApiImpl_StartEvent(void)
{
    VEOS_ApiSuccessType result = VEOS_E_PENDING;

    VEOS_FRM_DBG_METHOD_START("SimApi", __FUNCTION__);

    if ((VEOS_State_Downloaded == g_VEOS_Impl_RuntimeConfig.state) || (VEOS_State_Stopped == g_VEOS_Impl_RuntimeConfig.state))
    {
        (void)VEOS_SimApi_ResetCurrentEventStatus();
        VEOS_Impl_Start();

        result = VEOS_API_Start();

        if (VEOS_E_OK == result)
        {
            result = VEOS_SIMAPI_STARTHANDLER();

            if (VEOS_TRUE == VEOS_Impl_CheckEventStatus(&result, VEOS_TRUE))
            {
                (void)VEOS_MsgApi_ShowMessageFormat(VEOS_MsgApi_SeverityType_Info, VEOS_FALSE, "Starting application process with runtime revision %u.%u.%u.%u of module VEOS frame.", VEOS_FRAME_VERSION_MAJOR, VEOS_FRAME_VERSION_MINOR, VEOS_FRAME_VERSION_MAINTENANCE, VEOS_FRAME_VERSION_REVISION);
                g_VEOS_Impl_RuntimeConfig.state = VEOS_State_Started;
            }
        }
    }
    else
    {
        result = VEOS_E_NOT_OK;
    }

    VEOS_FRM_DBG_METHOD_END("SimApi", __FUNCTION__);

#ifdef VEOS_DEBUG
    VEOS_FRM_DBG_METHOD_RESULT("SimApi", __FUNCTION__, result);
#endif

    return result;
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEOS_SimApiImpl_TriggerEvent
 ***
 *** DESCRIPTION:
 ***     Hand-coded implementation of API function Trigger for SimApi of VEOS Frame
 ***
 *** PARAMETERS:
 ***     -/-
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any error
 ***                            occurs, an error code will be the result. See VEOS_Types.h for
 ***                            details.
\**************************************************************************************************/
VEOS_ApiSuccessType VEOS_SimApiImpl_TriggerEvent(void)
{
    VEOS_ApiSuccessType result = VEOS_E_PENDING;

    VEOS_FRM_DBG_METHOD_START("SimApi", __FUNCTION__);

    if (VEOS_State_Started == g_VEOS_Impl_RuntimeConfig.state)
    {
        (void)VEOS_SimApi_ResetCurrentEventStatus();
        result = VEOS_SIMAPI_TRIGGERHANDLER();
        (void)VEOS_Impl_CheckEventStatus(&result, VEOS_FALSE);
    }
    else
    {
        result = VEOS_E_NOT_OK;
    }

    VEOS_FRM_DBG_METHOD_END("SimApi", __FUNCTION__);

#ifdef VEOS_DEBUG
    VEOS_FRM_DBG_METHOD_RESULT("SimApi", __FUNCTION__, result);
#endif

    return result;
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEOS_SimApiImpl_StopEvent
 ***
 *** DESCRIPTION:
 ***     Hand-coded implementation of API function Stop for SimApi of VEOS Frame
 ***
 *** PARAMETERS:
 ***     -/-
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any error
 ***                            occurs, an error code will be the result. See VEOS_Types.h for
 ***                            details.
\**************************************************************************************************/
VEOS_ApiSuccessType VEOS_SimApiImpl_StopEvent(void)
{
    VEOS_ApiSuccessType result = VEOS_E_PENDING;

    VEOS_FRM_DBG_METHOD_START("SimApi", __FUNCTION__);

    if (VEOS_State_Started == g_VEOS_Impl_RuntimeConfig.state)
    {
        (void)VEOS_SimApi_ResetCurrentEventStatus();
        VEOS_Impl_Stop();

        result = VEOS_API_Stop();

        if (VEOS_E_OK == result)
        {
            result = VEOS_SIMAPI_STOPHANDLER();

            if (VEOS_TRUE == VEOS_Impl_CheckEventStatus(&result, VEOS_TRUE))
            {
                g_VEOS_Impl_RuntimeConfig.state = VEOS_State_Stopped;
            }
        }
    }
    else
    {
        result = VEOS_E_NOT_OK;
    }

    VEOS_FRM_DBG_METHOD_END("SimApi", __FUNCTION__);

#ifdef VEOS_DEBUG
    VEOS_FRM_DBG_METHOD_RESULT("SimApi", __FUNCTION__, result);
#endif

    return result;
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEOS_SimApiImpl_ResetEvent
 ***
 *** DESCRIPTION:
 ***     Hand-coded implementation of API function Reset for SimApi of VEOS Frame
 ***
 *** PARAMETERS:
 ***     Type                   Name              Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     void * const           VpuData           Incoming argument: The pointer to the VPU data.
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any error
 ***                            occurs, an error code will be the result. See VEOS_Types.h for
 ***                            details.
\**************************************************************************************************/
VEOS_ApiSuccessType VEOS_SimApiImpl_ResetEvent(void * const VpuData)
{
    VEOS_ApiSuccessType result = VEOS_E_PENDING;

    VEOS_FRM_DBG_METHOD_START("SimApi", __FUNCTION__);

    if (VEOS_State_Downloaded == g_VEOS_Impl_RuntimeConfig.state || VEOS_State_Stopped == g_VEOS_Impl_RuntimeConfig.state)
    {
        (void)VEOS_SimApi_ResetCurrentEventStatus();

        result = VEOS_SIMAPI_RESETHANDLER(VpuData);
    }
    else
    {
        result = VEOS_E_NOT_OK;
    }

    VEOS_FRM_DBG_METHOD_END("SimApi", __FUNCTION__);

#ifdef VEOS_DEBUG
    VEOS_FRM_DBG_METHOD_RESULT("SimApi", __FUNCTION__, result);
#endif

    return result;
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEOS_SimApiImpl_UnloadEvent
 ***
 *** DESCRIPTION:
 ***     Hand-coded implementation of API function Unload for SimApi of VEOS Frame
 ***
 *** PARAMETERS:
 ***     -/-
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any error
 ***                            occurs, an error code will be the result. See VEOS_Types.h for
 ***                            details.
\**************************************************************************************************/
VEOS_ApiSuccessType VEOS_SimApiImpl_UnloadEvent(void)
{
    VEOS_ApiSuccessType result = VEOS_E_PENDING;

    VEOS_FRM_DBG_METHOD_START("SimApi", __FUNCTION__);

    if (VEOS_State_Off != g_VEOS_Impl_RuntimeConfig.state)
    {
        (void)VEOS_SimApi_ResetCurrentEventStatus();

        result = VEOS_SIMAPI_UNLOADHANDLER();

        if (VEOS_TRUE == VEOS_Impl_CheckEventStatus(&result, VEOS_TRUE))
        {
            g_VEOS_Impl_RuntimeConfig.state = VEOS_State_Off;
        }
    }
    else
    {
        result = VEOS_E_NOT_OK;
    }

    VEOS_FRM_DBG_METHOD_END("SimApi", __FUNCTION__);

#ifdef VEOS_DEBUG
    VEOS_FRM_DBG_METHOD_RESULT("SimApi", __FUNCTION__, result);
#endif

    return result;
}

/**************************************************************************************************\
  *** FUNCTION:
  ***     VEOS_SimApiImpl_NotificationEvent
  ***
  *** DESCRIPTION:
  ***     Hand-coded implementation of API function NotificationEvent for SimApi of VEOS Frame
  ***
  *** PARAMETERS:
  ***     Type                                        Name              Description
  ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ***     VEOS_SimApi_InterruptCallbackFunctionType   pInterruptFct     Incoming argument: The
  ***                                                                   pointer to the notification
  ***                                                                   function to call.
  ***
  *** RETURNS:
  ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any error
  ***                            occurs, an error code will be the result. See VEOS_Types.h for
  ***                            details.
\**************************************************************************************************/
VEOS_ApiSuccessType VEOS_SimApiImpl_InterruptEvent(VEOS_SimApi_InterruptCallbackFunctionType const pInterruptFct)
{
    VEOS_CHECK_PARAM_PTR(pInterruptFct);

#ifdef VEOS_SIMAPI_INTERRUPTHANDLER_CFG_VALUE
    return VEOS_SIMAPI_INTERRUPTHANDLER_CFG_VALUE(pInterruptFct);
#else
    return VEOS_E_NO_IMPLEMENTATION;
#endif /* VEOS_SIMAPI_INTERRUPTHANDLER_CFG_VALUE */
}

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
void VEOS_EnableInterruptHandlers(void)
{
    VEOS_FRM_DBG_METHOD_START("FrameImplementation", __FUNCTION__);

#if (VEOS_ON == VEOS_APPFRM_IOSUPPORT)
    g_VEOS_Impl_RuntimeConfig.ioEventsEnabled = VEOS_TRUE;

    VEOS_Impl_HandleCachedIoEvents();
#endif /* VEOS_ON == VEOS_APPFRM_IOSUPPORT */

    VEOS_FRM_DBG_METHOD_END("FrameImplementation", __FUNCTION__);
}

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
void VEOS_DisableInterruptHandlers(void)
{
    VEOS_FRM_DBG_METHOD_START("FrameImplementation", __FUNCTION__);

#if (VEOS_ON == VEOS_APPFRM_IOSUPPORT)
    g_VEOS_Impl_RuntimeConfig.ioEventsEnabled = VEOS_FALSE;
#endif /* VEOS_ON == VEOS_APPFRM_IOSUPPORT */

    VEOS_FRM_DBG_METHOD_END("FrameImplementation", __FUNCTION__);
}

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
 ***     const VEOS_DataTypeCheckInfo     pCheckinfo     Pointer to the data type to be checked.
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any error
 ***                            occurs, an error code will be the result. See VEOS_Types.h for
 ***                            details.
\**************************************************************************************************/
VEOS_ApiSuccessType VEOS_CheckDataType(const VEOS_DataTypeCheckInfoType* pCheckinfo)
{
    if (((VEOS_DataTypeCheckInfoType*)0) == pCheckinfo)
    {
        return VEOS_E_NULL_PARAM;
    }

    if (pCheckinfo->ExpectedSize != pCheckinfo->RealSize)
    {
        (void)VEOS_MsgApi_ShowMessageFormat(
            VEOS_MsgApi_SeverityType_Warning,
            VEOS_TRUE,
            "The expected size of the data type '%s' was %u, but its real size is %u. The simulation cannot continue, since the expectations of the variable descriptions and simulator cannot be met.",
            pCheckinfo->DataTypeName,
            pCheckinfo->ExpectedSize,
            pCheckinfo->RealSize);
        return VEOS_E_NOT_OK;
    }

    if (VEOS_DTCHECK_NOSIGNCHECKREQUIRED != pCheckinfo->SignumCheckResult)
    {
        if (VEOS_DTCHECK_SIGNCHECK_SUCCEEDED != pCheckinfo->SignumCheckResult)
        {
            if (VEOS_DTCHECK_SIGNCHECK_FAILED_UNSIGNED == pCheckinfo->SignumCheckResult)
            {
                (void)VEOS_MsgApi_ShowMessageFormat(
                    VEOS_MsgApi_SeverityType_Warning,
                    VEOS_TRUE,
                    "The data type '%s' was expected to be unsigned, but seems to be signed. The simulation cannot continue, since the expectations of the variable descriptions and simulator cannot be met.",
                    pCheckinfo->DataTypeName);
            }
            else if (VEOS_DTCHECK_SIGNCHECK_FAILED_SIGNED == pCheckinfo->SignumCheckResult)
            {
                (void)VEOS_MsgApi_ShowMessageFormat(
                    VEOS_MsgApi_SeverityType_Warning,
                    VEOS_TRUE,
                    "The data type '%s' was expected to be signed, but seems to be unsigned. The simulation cannot continue, since the expectations of the variable descriptions and simulator cannot be met.",
                    pCheckinfo->DataTypeName);
            }
            else
            {
                (void)VEOS_MsgApi_ShowMessageFormat(
                    VEOS_MsgApi_SeverityType_Warning,
                    VEOS_TRUE,
                    "The data type '%s' does not meet the expectations of the signum check. The simulation cannot continue, since the expectations of the variable descriptions and simulator cannot be met.",
                    pCheckinfo->DataTypeName);
            }

            return VEOS_E_NOT_OK;
        }
    }

    return VEOS_E_OK;
}

#if (VEOS_ON == VEOS_APPFRM_IOSUPPORT)
/**************************************************************************************************\
 *** FUNCTION:
 ***     VEOS_Impl_AppendCachedIoEvent
 ***
 *** DESCRIPTION:
 ***     Appends an unhandled I/O event to the cache, if I/O events are currently disabled.
 ***
 *** PARAMETERS:
 ***     Type                             Name           Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     const VEOS_uint32                eventId        ID of the event which could not be handled.
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any error
 ***                            occurs, an error code will be the result. See VEOS_Types.h for
 ***                            details.
\**************************************************************************************************/
static VEOS_ApiSuccessType VEOS_Impl_AppendCachedIoEvent(const VEOS_uint32 eventId)
{
#ifdef VEOS_IO_EVENT_CACHE_ENABLED
    if (g_VEOS_Impl_IoEventCache.CachedEventsCount < VEOS_NUMBER_OF_CACHED_IO_EVENTS)
    {
        g_VEOS_Impl_IoEventCache.CachedEvents[g_VEOS_Impl_IoEventCache.CachedEventsCount] = eventId;
        g_VEOS_Impl_IoEventCache.CachedEventsCount++;

        return VEOS_E_OK;
    }
    else
    {
#ifdef VEOS_DEBUG
        VEOS_FRM_DBG_METHOD_RESULT("FrameImplementation", __FUNCTION__, VEOS_E_BUFFER_OVERFLOW);
#endif
        return VEOS_E_BUFFER_OVERFLOW;
    }
#else
    VEOS_DBG_UNUSED_PARAM(eventId);
    return VEOS_E_NO_IMPLEMENTATION;
#endif /* VEOS_IO_EVENT_CACHE_ENABLED */
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEOS_Impl_HandleCachedIoEvents
 ***
 *** DESCRIPTION:
 ***     Calls the I/O handler for all cached I/O events.
 ***
 *** PARAMETERS:
 ***     -/-
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any error
 ***                            occurs, an error code will be the result. See VEOS_Types.h for
 ***                            details.
\**************************************************************************************************/
static VEOS_ApiSuccessType VEOS_Impl_HandleCachedIoEvents(void)
{
#ifdef VEOS_IO_EVENT_CACHE_ENABLED
#if defined(VEOS_IO_EVENTCACHE_LIFO) || defined(VEOS_IO_EVENTCACHE_FIFO)
    VEOS_uint32 i;
    VEOS_ApiSuccessType result = VEOS_E_OK;

    VEOS_FRM_DBG_METHOD_START("FrameImplementation", __FUNCTION__);

#if defined(VEOS_IO_EVENTCACHE_LIFO)
    for (i = g_VEOS_Impl_IoEventCache.CachedEventsCount - 1; i >= 0; i--)
#elif defined(VEOS_IO_EVENTCACHE_FIFO)
    for (i = 0; i < g_VEOS_Impl_IoEventCache.CachedEventsCount; i++)
#endif
    {
        VEOS_ApiSuccessType localResult = VEOS_SIMAPI_IOHANDLER(g_VEOS_Impl_IoEventCache.CachedEvents[i]);
        g_VEOS_Impl_IoEventCache.CachedEvents[i] = VEOS_IOEVENTID_INVALID;

        if (VEOS_E_OK == result)
        {
            result = localResult;
        }
    }

    g_VEOS_Impl_IoEventCache.CachedEventsCount = 0;

    VEOS_FRM_DBG_METHOD_END("FrameImplementation", __FUNCTION__);
#ifdef VEOS_DEBUG
    VEOS_FRM_DBG_METHOD_RESULT("FrameImplementation", __FUNCTION__, result);
#endif

    return result;
#else
    return VEOS_Impl_ClearCachedIoEvents();
#endif
#else
    return VEOS_E_NO_IMPLEMENTATION;
#endif /* VEOS_IO_EVENT_CACHE_ENABLED */
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEOS_Impl_ClearCachedIoEvents
 ***
 *** DESCRIPTION:
 ***     Clears the I/O event cache without calling the I/O event handler.
 ***
 *** PARAMETERS:
 ***     -/-
 ***
 *** RETURNS:
 ***     VEOS_ApiSuccessType    If no error occurs, VEOS_E_OK will be returned. If any error
 ***                            occurs, an error code will be the result. See VEOS_Types.h for
 ***                            details.
\**************************************************************************************************/
static VEOS_ApiSuccessType VEOS_Impl_ClearCachedIoEvents(void)
{
#ifdef VEOS_IO_EVENT_CACHE_ENABLED
    int i;

    VEOS_FRM_DBG_METHOD_START("FrameImplementation", __FUNCTION__);

    for (i = g_VEOS_Impl_IoEventCache.CachedEventsCount; i >= 0; i--)
    {
        g_VEOS_Impl_IoEventCache.CachedEvents[i] = VEOS_IOEVENTID_INVALID;
    }

    g_VEOS_Impl_IoEventCache.CachedEventsCount = 0;

    VEOS_FRM_DBG_METHOD_END("FrameImplementation", __FUNCTION__);

    return VEOS_E_OK;
#else
    return VEOS_E_NO_IMPLEMENTATION;
#endif /* VEOS_IO_EVENT_CACHE_ENABLED */
}
#endif  /* VEOS_ON == VEOS_APPFRM_IOSUPPORT */

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEOS_Impl_Start
 ***
 *** DESCRIPTION:
 ***     Performs a start-up on the VEOS Frame hand-coded part.
 ***
 *** PARAMETERS:
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
static void VEOS_Impl_Start(void)
{
    g_VEOS_Impl_RuntimeConfig.ioEventsEnabled = VEOS_TRUE;
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEOS_Impl_Stop
 ***
 *** DESCRIPTION:
 ***     Performs a shutdown on the VEOS Frame hand-coded part.
 ***
 *** PARAMETERS:
 ***     -/-
 ***
 *** RETURNS:
 ***     void
\**************************************************************************************************/
static void VEOS_Impl_Stop(void)
{
    g_VEOS_Impl_RuntimeConfig.ioEventsEnabled = VEOS_FALSE;

#if (VEOS_ON == VEOS_APPFRM_IOSUPPORT && defined(VEOS_IO_EVENT_CACHE_ENABLED))
    (void)VEOS_Impl_ClearCachedIoEvents();
#endif /* VEOS_ON == VEOS_APPFRM_IOSUPPORT && defined(VEOS_IO_EVENT_CACHE_ENABLED) */
}

/**************************************************************************************************\
 *** FUNCTION:
 ***     VEOS_Impl_CheckEventStatus
 ***
 *** DESCRIPTION:
 ***     Checks and updates the status of the current event.
 ***
 *** PARAMETERS:
 ***     Type                     Name               Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     VEOS_ApiSuccessType *    pSuccess           The status variable to update.
 ***     const VEOS_boolean       AllowNoImplResult  If set to VEOS_TRUE, the result code VEOS_E_NO_IMPLEMENTATION
 ***                                                 will be allowed as return code as well.
 ***
 *** RETURNS:
 ***     VEOS_boolean
 ***            VEOS_TRUE:   If the current result allows to continue execution.
 ***            VEOS_FALSE:  If the current result does not allow execution.
\**************************************************************************************************/
static VEOS_boolean VEOS_Impl_CheckEventStatus(VEOS_ApiSuccessType* pSuccess, const VEOS_boolean AllowNoImplResult)
{
    if (NULL != pSuccess)
    {
        if (*pSuccess != VEOS_E_OK)
        {
            if (AllowNoImplResult && (*pSuccess != VEOS_E_NO_IMPLEMENTATION))
            {
                return VEOS_FALSE;
            }
        }

        *pSuccess = VEOS_SimApi_GetCurrentEventStatus();
        if (*pSuccess == VEOS_E_OK)
        {
            return VEOS_TRUE;
        }
    }

    return VEOS_FALSE;
}

#endif /* VEOS_IMPL_C */

/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/
