/**************************************************************************************************\
 *** VEOS_AppFrm.h
 ***
 *** VEOS frame header file declaring the interaction with model-type specific application frames.
 ***
 *** Copyright 2019 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef VEOS_APPFRM_H
#define VEOS_APPFRM_H

/*------------------------------------------------------------------------------------------------*\
  DEFINES (OPT)
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/

#include "VEOS_Guard.h"
#include "VEOS_Types.h"
#include "VEOS_Cfg.h"
#include "VEOS_Internal.h"

/*------------------------------------------------------------------------------------------------*\
  PARAMETERIZED MACROS
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
  TYPEDEFS
\*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*\
  PARAMETERIZED MACROS
\*------------------------------------------------------------------------------------------------*/

#if (VEOS_CFG_APPFRM_IFMODE == VEOS_ON)

#ifdef VEOS_APPFRM_API_FUN
#ifdef VEOS_SIMAPI_DOWNLOADHANDLER_CFG_VALUE
#undef VEOS_SIMAPI_DOWNLOADHANDLER_CFG_VALUE
#endif /* VEOS_SIMAPI_DOWNLOADHANDLER_CFG_VALUE */
/* Function to handle the download event */
#define VEOS_SIMAPI_DOWNLOADHANDLER_CFG_VALUE               VEOS_APPFRM_API_FUN(Download)

#ifdef VEOS_SIMAPI_STARTHANDLER_CFG_VALUE
#undef VEOS_SIMAPI_STARTHANDLER_CFG_VALUE
#endif /* VEOS_SIMAPI_STARTHANDLER_CFG_VALUE */
/* Function to handle the start event */
#define VEOS_SIMAPI_STARTHANDLER_CFG_VALUE                  VEOS_APPFRM_API_FUN(Start)

#ifdef VEOS_SIMAPI_TRIGGERHANDLER_CFG_VALUE
#undef VEOS_SIMAPI_TRIGGERHANDLER_CFG_VALUE
#endif /* VEOS_SIMAPI_TRIGGERHANDLER_CFG_VALUE */
/* Function to handle the cyclic trigger event */
#define VEOS_SIMAPI_TRIGGERHANDLER_CFG_VALUE                VEOS_APPFRM_API_FUN(Trigger)

#ifdef VEOS_SIMAPI_STOPHANDLER_CFG_VALUE
#undef VEOS_SIMAPI_STOPHANDLER_CFG_VALUE
#endif /* VEOS_SIMAPI_STOPHANDLER_CFG_VALUE */
/* Function to handle the stop event */
#define VEOS_SIMAPI_STOPHANDLER_CFG_VALUE                   VEOS_APPFRM_API_FUN(Stop)

#ifdef VECU_CODE
#ifdef VEOS_SIMAPI_RESETHANDLER_CFG_VALUE
#undef VEOS_SIMAPI_RESETHANDLER_CFG_VALUE
#endif /* VEOS_SIMAPI_RESETHANDLER_CFG_VALUE */
/* Function to handle the reset event */
#define VEOS_SIMAPI_RESETHANDLER_CFG_VALUE                  VEOS_APPFRM_API_FUN(Reset)
#endif /* VECU_CODE */

#ifdef FMI_CODE
#ifdef VEOS_SIMAPI_UNLOADHANDLER_CFG_VALUE
#undef VEOS_SIMAPI_UNLOADHANDLER_CFG_VALUE
#endif /* VEOS_SIMAPI_UNLOADHANDLER_CFG_VALUE */
/* Function to handle the reset event */
#define VEOS_SIMAPI_UNLOADHANDLER_CFG_VALUE                 VEOS_APPFRM_API_FUN(Unload)
#endif /* FMI_CODE */

#ifdef VECU_CODE
#ifdef VEOS_SIMAPI_ASYNCSIMEVENT_HANDLER_CFG_VALUE
#undef VEOS_SIMAPI_ASYNCSIMEVENT_HANDLER_CFG_VALUE
#endif /* VEOS_SIMAPI_ASYNCSIMEVENT_HANDLER_CFG_VALUE */
/* Function to handle asynchronous simulation events */
#define VEOS_SIMAPI_ASYNCSIMEVENT_HANDLER_CFG_VALUE         VEOS_APPFRM_API_FUN(HandleAsyncSimEvent)
#endif /* VECU_CODE */

#ifdef VEOS_SIMAPI_IOHANDLER_CFG_VALUE
#undef VEOS_SIMAPI_IOHANDLER_CFG_VALUE
#endif /* VEOS_SIMAPI_IOHANDLER_CFG_VALUE */
#if (VEOS_APPFRM_IOSUPPORT_CFG_VALUE == VEOS_ON)
/* Function to handle the model specific I/O events */
#define VEOS_SIMAPI_IOHANDLER_CFG_VALUE                     VEOS_APPFRM_API_FUN(HandleIoEvent)
#endif /* VEOS_APPFRM_IOSUPPORT_CFG_VALUE == VEOS_ON */

#ifdef VEOS_SIMAPI_INTERRUPTHANDLER_CFG_VALUE
#undef VEOS_SIMAPI_INTERRUPTHANDLER_CFG_VALUE
#endif /* VEOS_SIMAPI_INTERRUPTHANDLER_CFG_VALUE */
#if (VEOS_NOTIAPI_NOTIFICATION_CFG_VALUE == VEOS_ON || VEOS_BUSAPI_INTERRUPT_CFG_VALUE == VEOS_ON)
/* Function to handle the asynchronous interrupt events */
#define VEOS_SIMAPI_INTERRUPTHANDLER_CFG_VALUE              VEOS_APPFRM_API_FUN(InterruptEvent)
#endif /* VEOS_NOTIAPI_NOTIFICATION_CFG_VALUE == VEOS_ON || VEOS_BUSAPI_INTERRUPT_CFG_VALUE == VEOS_ON */

#else
#error Application Frame interface not set
#endif /* VEOS_APPFRM_API_FUN */

#endif /* VEOS_CFG_APPFRM_IFMODE == VEOS_ON */

#if (VEOS_BUSAPI_CANSUPPORT_CFG_VALUE == VEOS_ON)
#if (VEOS_BUSAPI_CANSUPPORT_IFMODE == VEOS_ON)
#ifdef VEOS_CAN_API_FUN
#ifdef VEOS_BUSAPI_CANMSGHANDLER_CFG_VALUE
#undef VEOS_BUSAPI_CANMSGHANDLER_CFG_VALUE
#endif /* VEOS_BUSAPI_CANMSGHANDLER_CFG_VALUE */
/* Function to handle CAN specific message events */
#define VEOS_BUSAPI_CANMSGHANDLER_CFG_VALUE                 VEOS_CAN_API_FUN(HandleMsgEvent)

#ifdef VEOS_BUSAPI_CANCHHANDLER_CFG_VALUE
#undef VEOS_BUSAPI_CANCHHANDLER_CFG_VALUE
#endif /* VEOS_BUSAPI_CANCHHANDLER_CFG_VALUE */
/* Function to handle CAN specific channel events */
#define VEOS_BUSAPI_CANCHHANDLER_CFG_VALUE                  VEOS_CAN_API_FUN(HandleChEvent)

#else
#error CAN simulation handler interface interface not set
#endif /* VEOS_CAN_API_FUN */
#endif /* VEOS_BUSAPI_CANSUPPORT_IFMODE_CFG_VALUE == VEOS_ON */
#endif /* VEOS_BUSAPI_CANSUPPORT == VEOS_ON */

#if (VEOS_BUSAPI_LINSUPPORT_CFG_VALUE == VEOS_ON)
#if (VEOS_BUSAPI_LINSUPPORT_IFMODE == VEOS_ON)
#ifdef VEOS_LIN_API_FUN
#ifdef VEOS_BUSAPI_LINMSGHANDLER_CFG_VALUE
#undef VEOS_BUSAPI_LINMSGHANDLER_CFG_VALUE
#endif /* VEOS_BUSAPI_LINMSGHANDLER_CFG_VALUE */
/* Function to handle LIN specific message events */
#define VEOS_BUSAPI_LINMSGHANDLER_CFG_VALUE                 VEOS_LIN_API_FUN(HandleMsgEvent)

#ifdef VEOS_BUSAPI_LINCHHANDLER_CFG_VALUE
#undef VEOS_BUSAPI_LINCHHANDLER_CFG_VALUE
#endif /* VEOS_BUSAPI_LINCHHANDLER_CFG_VALUE */
/* Function to handle LIN specific channel events */
#define VEOS_BUSAPI_LINCHHANDLER_CFG_VALUE                  VEOS_LIN_API_FUN(HandleChEvent)

#ifdef VEOS_BUSAPI_LINHDRHANDLER_CFG_VALUE
#undef VEOS_BUSAPI_LINHDRHANDLER_CFG_VALUE
#endif /* VEOS_BUSAPI_LINHDRHANDLER_CFG_VALUE */
/* Function to handle LIN specific channel events */
#define VEOS_BUSAPI_LINHDRHANDLER_CFG_VALUE                 VEOS_LIN_API_FUN(HandleHdrEvent)

#else
#error LIN simulation handler interface interface not set
#endif /* VEOS_LIN_API_FUN */
#endif /* VEOS_BUSAPI_LINSUPPORT_IFMODE_CFG_VALUE == VEOS_ON */
#endif /* VEOS_BUSAPI_LINSUPPORT == VEOS_ON */

#if (VEOS_BUSAPI_IDIFSUPPORT_CFG_VALUE == VEOS_ON)
#if (VEOS_BUSAPI_IDIFSUPPORT_IFMODE == VEOS_ON)
#ifdef VEOS_IDIF_API_FUN
#ifdef VEOS_BUSAPI_IDIFMSGHANDLER_CFG_VALUE
#undef  VEOS_BUSAPI_IDIFMSGHANDLER_CFG_VALUE
#endif /* VEOS_BUSAPI_IDIFMSGHANDLER_CFG_VALUE */
/* Function to handle idealized bus simulation specific message events */
#define VEOS_BUSAPI_IDIFMSGHANDLER_CFG_VALUE                VEOS_IDIF_API_FUN(HandleMsgEvent)

#ifdef VEOS_BUSAPI_IDIFCHHANDLER_CFG_VALUE
#undef  VEOS_BUSAPI_IDIFCHHANDLER_CFG_VALUE
#endif /* VEOS_BUSAPI_IDIFCHHANDLER_CFG_VALUE */
/* Function to handle idealized bus simulation specific channel events */
#define VEOS_BUSAPI_IDIFCHHANDLER_CFG_VALUE                 VEOS_IDIF_API_FUN(HandleChEvent)

#else
#error Idealized bus simulation handler interface interface not set
#endif /* VEOS_IDIF_API_FUN */
#endif /* VEOS_BUSAPI_IDIFSUPPORT_IFMODE_CFG_VALUE == VEOS_ON */
#endif /* VEOS_BUSAPI_IDIFSUPPORT == VEOS_ON */

#if (VEOS_BUSAPI_SIMULATION == VEOS_ON)
#define VEOS_BUSAPI_MSG_EVENT_HANDLER_DECLARATION(FunctionName) \
    VEOS_ApiSuccessType FunctionName(const VEOS_BusApi_MessageHandleType MsgHandle, const VEOS_uint32 MsgId, const VEOS_uint32 CbkArg)

#define VEOS_BUSAPI_CH_EVENT_HANDLER_DECLARATION(FunctionName) \
    VEOS_ApiSuccessType FunctionName(const VEOS_BusApi_ConnectorHandleType Channel, const VEOS_BusApi_ConnectorStatusType Status, const VEOS_uint32 CbkArg)

#if (VEOS_BUSAPI_LINSUPPORT == VEOS_ON)
#define VEOS_BUSAPI_LINHDR_EVENT_HANDLER_DECLARATION(FunctionName) \
    VEOS_ApiSuccessType FunctionName(const VEOS_BusApi_MessageHandleType MsgHandle, const VEOS_uint32 MsgId,  const VEOS_uint32 CbkArg)
#endif /* VEOS_BUSAPI_LINSUPPORT == VEOS_ON */
#endif /* VEOS_BUSAPI_SIMULATION == VEOS_ON */

/*------------------------------------------------------------------------------------------------*\
  FUNCTION PROTOTYPES
\*------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C"{
#endif

#ifdef VEOS_SIMAPI_DOWNLOADHANDLER_CFG_VALUE
extern VEOS_ApiSuccessType VEOS_SIMAPI_DOWNLOADHANDLER_CFG_VALUE(void);
#endif /* VEOS_SIMAPI_DOWNLOADHANDLER_CFG_VALUE */

#ifdef VEOS_SIMAPI_STARTHANDLER_CFG_VALUE
extern VEOS_ApiSuccessType VEOS_SIMAPI_STARTHANDLER_CFG_VALUE(void);
#endif /* VEOS_SIMAPI_STARTHANDLER_CFG_VALUE */

#ifdef VEOS_SIMAPI_TRIGGERHANDLER_CFG_VALUE
extern VEOS_ApiSuccessType VEOS_SIMAPI_TRIGGERHANDLER_CFG_VALUE(void);
#endif /* VEOS_SIMAPI_TRIGGERHANDLER_CFG_VALUE */

#ifdef VEOS_SIMAPI_STOPHANDLER_CFG_VALUE
extern VEOS_ApiSuccessType VEOS_SIMAPI_STOPHANDLER_CFG_VALUE(void);
#endif /* VEOS_SIMAPI_STOPHANDLER_CFG_VALUE */

#ifdef VEOS_SIMAPI_RESETHANDLER_CFG_VALUE
extern VEOS_ApiSuccessType VEOS_SIMAPI_RESETHANDLER_CFG_VALUE(void * const VpuData);
#endif /* VEOS_SIMAPI_RESETHANDLER_CFG_VALUE */

#ifdef VEOS_SIMAPI_UNLOADHANDLER_CFG_VALUE
extern VEOS_ApiSuccessType VEOS_SIMAPI_UNLOADHANDLER_CFG_VALUE(void);
#endif /* VEOS_SIMAPI_UNLOADHANDLER_CFG_VALUE */

#ifdef VEOS_SIMAPI_ASYNCSIMEVENT_HANDLER_CFG_VALUE
extern VEOS_ApiSuccessType VEOS_SIMAPI_ASYNCSIMEVENT_HANDLER_CFG_VALUE(void);
#endif /* VEOS_SIMAPI_ASYNCSIMEVENT_HANDLER_CFG_VALUE */

#ifdef VEOS_SIMAPI_IOHANDLER_CFG_VALUE
extern VEOS_ApiSuccessType VEOS_SIMAPI_IOHANDLER_CFG_VALUE(const VEOS_uint32 ioEventId);
#endif /* VEOS_SIMAPI_IOHANDLER_CFG_VALUE */

#ifdef VEOS_SIMAPI_INTERRUPTHANDLER_CFG_VALUE
extern VEOS_ApiSuccessType VEOS_SIMAPI_INTERRUPTHANDLER_CFG_VALUE(const VEOS_SimApi_InterruptCallbackFunctionType pFunction);
#endif /* VEOS_SIMAPI_IOHANDLER_CFG_VALUE */

#ifdef VEOS_BUSAPI_CANMSGHANDLER_CFG_VALUE
extern VEOS_BUSAPI_MSG_EVENT_HANDLER_DECLARATION(VEOS_BUSAPI_CANMSGHANDLER_CFG_VALUE);
#endif /* VEOS_BUSAPI_CANMSGHANDLER_CFG_VALUE */

#ifdef VEOS_BUSAPI_CANCHHANDLER_CFG_VALUE
extern VEOS_BUSAPI_CH_EVENT_HANDLER_DECLARATION(VEOS_BUSAPI_CANCHHANDLER_CFG_VALUE);
#endif /* VEOS_BUSAPI_CANCHHANDLER_CFG_VALUE */

#ifdef VEOS_BUSAPI_LINMSGHANDLER_CFG_VALUE
extern VEOS_BUSAPI_MSG_EVENT_HANDLER_DECLARATION(VEOS_BUSAPI_LINMSGHANDLER_CFG_VALUE);
#endif /* VEOS_BUSAPI_LINMSGHANDLER_CFG_VALUE */

#ifdef VEOS_BUSAPI_LINCHHANDLER_CFG_VALUE
extern VEOS_BUSAPI_CH_EVENT_HANDLER_DECLARATION(VEOS_BUSAPI_LINCHHANDLER_CFG_VALUE);
#endif /* VEOS_BUSAPI_LINCHHANDLER_CFG_VALUE */

#ifdef VEOS_BUSAPI_LINHDRHANDLER_CFG_VALUE
extern VEOS_BUSAPI_LINHDR_EVENT_HANDLER_DECLARATION(VEOS_BUSAPI_LINHDRHANDLER_CFG_VALUE);
#endif /* VEOS_BUSAPI_LINHDRHANDLER_CFG_VALUE */

#ifdef VEOS_BUSAPI_IDIFMSGHANDLER_CFG_VALUE
extern VEOS_BUSAPI_MSG_EVENT_HANDLER_DECLARATION(VEOS_BUSAPI_IDIFMSGHANDLER_CFG_VALUE);
#endif /* VEOS_BUSAPI_IDIFMSGHANDLER_CFG_VALUE */

#ifdef VEOS_BUSAPI_IDIFCHHANDLER_CFG_VALUE
extern VEOS_BUSAPI_CH_EVENT_HANDLER_DECLARATION(VEOS_BUSAPI_IDIFCHHANDLER_CFG_VALUE);
#endif /* VEOS_BUSAPI_IDIFCHHANDLER_CFG_VALUE */

#ifdef __cplusplus
}
#endif

/*------------------------------------------------------------------------------------------------*\
  DEFINES
\*------------------------------------------------------------------------------------------------*/

#ifdef VEOS_SIMAPI_DOWNLOADHANDLER_CFG_VALUE
/* The function called on download of the application */
#define VEOS_SIMAPI_DOWNLOADHANDLER                                     VEOS_SIMAPI_DOWNLOADHANDLER_CFG_VALUE
#else
/* No implementation for this function. */
#define VEOS_SIMAPI_DOWNLOADHANDLER()                                   VEOS_E_NO_IMPLEMENTATION
#endif /* VEOS_SIMAPI_DOWNLOADHANDLER_CFG_VALUE */

#ifdef VEOS_SIMAPI_STARTHANDLER_CFG_VALUE
/* The function called on start of the application */
#define VEOS_SIMAPI_STARTHANDLER                                        VEOS_SIMAPI_STARTHANDLER_CFG_VALUE
#else
/* No implementation for this function. */
#define VEOS_SIMAPI_STARTHANDLER()                                      VEOS_E_NO_IMPLEMENTATION
#endif /* VEOS_SIMAPI_STARTHANDLER_CFG_VALUE */

#ifdef VEOS_SIMAPI_TRIGGERHANDLER_CFG_VALUE
/* The function called on cyclic trigger of the application */
#define VEOS_SIMAPI_TRIGGERHANDLER                                      VEOS_SIMAPI_TRIGGERHANDLER_CFG_VALUE
#else
/* No implementation for this function. */
#define VEOS_SIMAPI_TRIGGERHANDLER()                                    VEOS_E_NO_IMPLEMENTATION
#endif /* VEOS_SIMAPI_TRIGGERHANDLER_CFG_VALUE */

#ifdef VEOS_SIMAPI_STOPHANDLER_CFG_VALUE
/* The function called on stop of the application */
#define VEOS_SIMAPI_STOPHANDLER                                         VEOS_SIMAPI_STOPHANDLER_CFG_VALUE
#else
/* No implementation for this function. */
#define VEOS_SIMAPI_STOPHANDLER()                                       VEOS_E_NO_IMPLEMENTATION
#endif /* VEOS_SIMAPI_STOPHANDLER_CFG_VALUE */

#ifdef VEOS_SIMAPI_RESETHANDLER_CFG_VALUE
/* The function called on reset of the application */
#define VEOS_SIMAPI_RESETHANDLER                                        VEOS_SIMAPI_RESETHANDLER_CFG_VALUE
#else
#define VEOS_SIMAPI_RESETHANDLER(arg)                                   VEOS_E_OK; (void)arg
#endif /* VEOS_SIMAPI_RESETHANDLER_CFG_VALUE */

#ifdef VEOS_SIMAPI_UNLOADHANDLER_CFG_VALUE
/* The function called on unload of the application */
#define VEOS_SIMAPI_UNLOADHANDLER                                       VEOS_SIMAPI_UNLOADHANDLER_CFG_VALUE
#else
#define VEOS_SIMAPI_UNLOADHANDLER()                                     VEOS_E_OK
#endif /* VEOS_SIMAPI_UNLOADHANDLER_CFG_VALUE */

#ifdef VEOS_SIMAPI_ASYNCSIMEVENT_HANDLER_CFG_VALUE
/* The function called on asynchronous simulation events */
#define VEOS_SIMAPI_ASYNCSIMEVENT_HANDLER                               VEOS_SIMAPI_ASYNCSIMEVENT_HANDLER_CFG_VALUE
#else
#define VEOS_SIMAPI_ASYNCSIMEVENT_HANDLER()                             VEOS_E_OK
#endif /* VEOS_SIMAPI_ASYNCSIMEVENT_HANDLER_CFG_VALUE */

#ifdef VEOS_SIMAPI_IOHANDLER_CFG_VALUE
/* The function called during an internal I/O event */
#define VEOS_SIMAPI_IOHANDLER                                           VEOS_SIMAPI_IOHANDLER_CFG_VALUE
#else
/* No implementation for this function. */
#define VEOS_SIMAPI_IOHANDLER(Event)                                    VEOS_E_NO_IMPLEMENTATION
#endif /* VEOS_SIMAPI_IOHANDLER_CFG_VALUE */

#if (VEOS_BUSAPI_CANSUPPORT_CFG_VALUE == VEOS_ON)
#ifdef VEOS_BUSAPI_CANMSGHANDLER_CFG_VALUE
/* The function handling CAN bus message events */
#define VEOS_BUSAPI_CANMSGHANDLER                                       VEOS_BUSAPI_CANMSGHANDLER_CFG_VALUE
#else
/* No implementation for this function. */
#define VEOS_BUSAPI_CANMSGHANDLER(Message, MsgId, CbkArg)               VEOS_E_NO_IMPLEMENTATION
#endif /* VEOS_BUSAPI_CANMSGHANDLER_CFG_VALUE */

#ifdef VEOS_BUSAPI_CANCHHANDLER_CFG_VALUE
/* The function handling CAN bus channel events */
#define VEOS_BUSAPI_CANCHHANDLER                                        VEOS_BUSAPI_CANCHHANDLER_CFG_VALUE
#else
/* No implementation for this function. */
#define VEOS_BUSAPI_CANCHHANDLER(Channel, Status, CbkArg)               VEOS_E_NO_IMPLEMENTATION
#endif /* VEOS_BUSAPI_CANCHHANDLER_CFG_VALUE */
#endif /* VEOS_BUSAPI_CANSUPPORT_CFG_VALUE == VEOS_ON */

#if (VEOS_BUSAPI_LINSUPPORT_CFG_VALUE == VEOS_ON)
#ifdef VEOS_BUSAPI_LINMSGHANDLER_CFG_VALUE
/* The function handling LIN bus message events */
#define VEOS_BUSAPI_LINMSGHANDLER                                       VEOS_BUSAPI_LINMSGHANDLER_CFG_VALUE
#else
/* No implementation for this function. */
#define VEOS_BUSAPI_LINMSGHANDLER(Message, MsgId, CbkArg)               VEOS_E_NO_IMPLEMENTATION
#endif /* VEOS_BUSAPI_LINMSGHANDLER_CFG_VALUE */

#ifdef VEOS_BUSAPI_LINCHHANDLER_CFG_VALUE
/* The function handling LIN bus channel events */
#define VEOS_BUSAPI_LINCHHANDLER                                        VEOS_BUSAPI_LINCHHANDLER_CFG_VALUE
#else
/* No implementation for this function. */
#define VEOS_BUSAPI_LINCHHANDLER(Channel, Status, CbkArg)               VEOS_E_NO_IMPLEMENTATION
#endif /* VEOS_BUSAPI_LINCHHANDLER_CFG_VALUE */

#ifdef VEOS_BUSAPI_LINCHHANDLER_CFG_VALUE
/* The function handling LIN bus header received events */
#define VEOS_BUSAPI_LINHDRHANDLER                                       VEOS_BUSAPI_LINHDRHANDLER_CFG_VALUE
#else
/* No implementation for this function. */
#define VEOS_BUSAPI_LINHDRHANDLER(Message, MsgId, CbkArg)               VEOS_E_NO_IMPLEMENTATION
#endif /* VEOS_BUSAPI_LINCHHANDLER_CFG_VALUE */
#endif /* VEOS_BUSAPI_LINSUPPORT_CFG_VALUE == VEOS_ON */

#if (VEOS_BUSAPI_IDIFSUPPORT_CFG_VALUE == VEOS_ON)
#ifdef VEOS_BUSAPI_IDIFMSGHANDLER_CFG_VALUE
/* The function handling IDIF bus message events */
#define VEOS_BUSAPI_IDIFMSGHANDLER                                      VEOS_BUSAPI_IDIFMSGHANDLER_CFG_VALUE
#else
/* No implementation for this function. */
#define VEOS_BUSAPI_IDIFMSGHANDLER(Message, MsgId,  CbkArg)             VEOS_E_NO_IMPLEMENTATION
#endif /* VEOS_BUSAPI_IDIFMSGHANDLER_CFG_VALUE */

#ifdef VEOS_BUSAPI_IDIFCHHANDLER_CFG_VALUE
/* The function handling IDIF bus channel events */
#define VEOS_BUSAPI_IDIFCHHANDLER                                       VEOS_BUSAPI_IDIFCHHANDLER_CFG_VALUE
#else
/* No implementation for this function. */
#define VEOS_BUSAPI_IDIFCHHANDLER(Channel, Status, CbkArg)              VEOS_E_NO_IMPLEMENTATION
#endif /* VEOS_BUSAPI_IDIFCHHANDLER_CFG_VALUE */
#endif /* VEOS_BUSAPI_IDIFSUPPORT_CFG_VALUE == VEOS_ON */

#endif /* VEOS_APPFRM_H */

/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/
