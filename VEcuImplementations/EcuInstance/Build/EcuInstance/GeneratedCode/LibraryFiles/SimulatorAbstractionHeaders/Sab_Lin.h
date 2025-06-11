/**************************************************************************************************\
 *** Sab_Lin.h
 ***
 *** Generic Simulator Abstraction configuration header file for LIN.
 ***
 *** Copyright 2019 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef SAB_LIN_H
#define SAB_LIN_H

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/

#include "Sab_Cfg.h"
#include "Sab_Types.h"
#include "Std_Types.h"

/*------------------------------------------------------------------------------------------------*\
  DEFINES
\*------------------------------------------------------------------------------------------------*/

#define SAB_LIN_E_PARAM_POINTER         0x10
#define SAB_LIN_E_PARAM_VALUE           0x11

#define SAB_LIN_BOARD_FUNCTION          0x90
#define SAB_LIN_CHANNEL_FUNCTION        0x91

#define SAB_LIN_MAX_HWHANDLE            0xFFFF

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
 ***     Sab_LinGetBoardAddress
 ***
 *** DESCRIPTION:
 ***     Tries to retrieve the LIN board address for the specified hardware handle.
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
extern Std_ReturnType Sab_LinGetBoardAddress(Sab_HwHandleType HardwareHandle, Sab_BoardAddressType *pBoardAddress);

/**************************************************************************************************\
 *** FUNCTION:
 ***     Sab_LinGetChannelAddress
 ***
 *** DESCRIPTION:
 ***     Tries to retrieve the LIN channel address for the specified hardware handle.
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
extern Std_ReturnType Sab_LinGetChannelAddress(Sab_HwHandleType HardwareHandle, Sab_ChannelAddressType *pChannelAddress);

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
