/**************************************************************************************************\
 *** Os_ResInt.h
 ***
 *** OSEK API function definition for OS Resources and interrupt handling.
 ***
 *** Copyright (c) 2014 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef OS_RESINT_H
#define OS_RESINT_H

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/

#include "Os_Types.h"
#include "Os_InternalTypes.h"
#include "Os.h"


/* Section 13.3  Interrupt handling - 13.3.3  Naming convention */
/*
#define ISR(ISRIdentifier)                \
    void Func##ISRIdentifier(void);       \
    void ISR_##ISRIdentifier(void)        \
    {                                     \
    Os_EnterISR();                        \
    Func##ISRIdentifier();                \
    Os_LeaveISR();                        \
    }                                     \
    void Func##ISRIdentifier(void)
*/



/*------------------------------------------------------------------------------------------------*\
  FORWARD DECLARATIONS
\*------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

extern StatusType Os_EnableInterrupt(Os_IntDescriptorType Descriptor);
extern StatusType Os_DisableInterrupt(Os_IntDescriptorType Descriptor);
extern StatusType Os_GetInterruptDescriptor(Os_IntDescriptorRefType Descriptor);

extern void Os_EnterISR(void);
extern void Os_LeaveISR(void);
extern StatusType Os_ActivateIsr(ISRType IsrId);

#ifdef __cplusplus
}
#endif

#endif /* OS_RESINT_H */

/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/
