/**************************************************************************************************\
 *** Sab.h
 ***
 *** Generic Simulator Abstraction configuration header file.
 ***
 *** Copyright 2019 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef SAB_CAN_H
#define SAB_CAN_H

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/

#include "Sab_Cfg.h"
#include "Sab_Types.h"
#include "Std_Types.h"

/*------------------------------------------------------------------------------------------------*\
  DEFINES
\*------------------------------------------------------------------------------------------------*/

#define SAB_CAN_E_PARAM_POINTER         0x10
#define SAB_CAN_E_PARAM_VALUE           0x11

#define SAB_CAN_BOARD_FUNCTION          0x90
#define SAB_CAN_CHANNEL_FUNCTION        0x91

#define SAB_CAN_MAX_HWHANDLE            0xFFFF

/*------------------------------------------------------------------------------------------------*\
  FUNCTION DECLARATIONS
\*------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif
#if 0
}
#endif

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_CanGetBoardAddress
 ***
 *** DESCRIPTION:
 ***     Tries to retrieve the CAN board address for the specified hardware handle.
 ***
 *** PARAMETERS:
 ***     Type                   Name            Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     Sab_HwHandleType       HardwareHandle  The hardware handle to get the address for.
 ***     Sab_BoardAddressType * pBoardAddress   The variable to store the board address in.
 ***
 *** RETURNS:
 ***     Std_ReturnType
 ***            E_OK            If the address was determined successfully.
 ***            E_NOT_OK        If the address could either not be determined or not be stored.
\**************************************************************************************************/
extern Std_ReturnType Sab_CanGetBoardAddress(Sab_HwHandleType HardwareHandle, Sab_BoardAddressType *pBoardAddress);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_CanGetChannelAddress
 ***
 *** DESCRIPTION:
 ***     Tries to retrieve the CAN channel address for the specified hardware handle.
 ***
 *** PARAMETERS:
 ***     Type                     Name            Description
 ***     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***     Sab_HwHandleType         HardwareHandle  The hardware handle to get the address for.
 ***     Sab_ChannelAddressType * pChannelAddress The variable to store the channel address in.
 ***
 *** RETURNS:
 ***     Std_ReturnType
 ***            E_OK            If the address was determined successfully.
 ***            E_NOT_OK        If the address could either not be determined or not be stored.
\**************************************************************************************************/
extern Std_ReturnType Sab_CanGetChannelAddress(Sab_HwHandleType HardwareHandle, Sab_ChannelAddressType *pChannelAddress);

#if 0
{
#endif
#ifdef __cplusplus
};
#endif

#endif /* SAB_CFG_H */

/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/
