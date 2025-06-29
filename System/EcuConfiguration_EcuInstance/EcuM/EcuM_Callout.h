/**************************************************************************************************\
 *** 
 *** System              : System
 *** ECU                 : EcuInstance
 *** Configuration       : EcuConfiguration_EcuInstance
 *** Codefile            : EcuM_Callout.h
 ***
 *** Generated by SystemDesk 5.6.0.13023
 *** Generation date: 2025-06-11 10:13:54
 ***
 *** Copyright (c) 2018 dSPACE GmbH
\**************************************************************************************************/

#ifndef ECUM_CALLOUT_H
#define ECUM_CALLOUT_H

/*------------------------------------------------------------------------------------------------*\
  INCLUDES
\*------------------------------------------------------------------------------------------------*/
#include "Std_Types.h"
#include "EcuM_Cfg.h"
#include "Os.h"

/*------------------------------------------------------------------------------------------------*\
  PARAMETERIZED MACROS
\*------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */ 

/*------------------------------------------------------------------------------------------------*\
  FUNCTION PROTOTYPES
\*------------------------------------------------------------------------------------------------*/
/**************************************************************************************************\
 *** FUNCTION:
 ***     EcuM_AL_DriverInitOne
 *** 
 *** DESCRIPTION:
 ***     This callout shall provide driver initialization and other hardware-related startup
 ***     activities in case of a power on reset.
 ***     
 ***     ServiceID  : 0x33
 ***     Reentrant  : false
 ***
 *** PARAMETERS:
 ***    Type           Name   Description
 ***    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***	const EcuM_ConfigType* ConfigPtr Pointer to the EcuM post-build configuration which contains pointers to all other BSW
 ***     module post-build configurations. 
 *** 
 *** RETURNS:
 ***    void
\**************************************************************************************************/
extern void EcuM_AL_DriverInitOne(const EcuM_ConfigType* ConfigPtr);

/**************************************************************************************************\
 *** FUNCTION:
 ***     EcuM_AL_DriverInitThree
 *** 
 *** DESCRIPTION:
 ***     This callout shall provide driver initialization of drivers which need OS and need to
 ***     wait for the NvM_ReadAll job to finish.
 ***     
 ***     ServiceID  : 0x35
 ***     Reentrant  : false
 ***
 *** PARAMETERS:
 ***    Type           Name   Description
 ***    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***	const EcuM_ConfigType* ConfigPtr Pointer to the EcuM post-build configuration which contains pointers to all other BSW
 ***     module post-build configurations. 
 *** 
 *** RETURNS:
 ***    void
\**************************************************************************************************/
extern void EcuM_AL_DriverInitThree(const EcuM_ConfigType* ConfigPtr);

/**************************************************************************************************\
 *** FUNCTION:
 ***     EcuM_AL_DriverInitTwo
 *** 
 *** DESCRIPTION:
 ***     This callout shall provide driver initialization of drivers which need OS and do not
 ***     need to wait for the NvM_ReadAll job to finish.
 ***     
 ***     ServiceID  : 0x34
 ***     Reentrant  : false
 ***
 *** PARAMETERS:
 ***    Type           Name   Description
 ***    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***	const EcuM_ConfigType* ConfigPtr Pointer to the EcuM post-build configuration which contains pointers to all other BSW
 ***     module post-build configurations. 
 *** 
 *** RETURNS:
 ***    void
\**************************************************************************************************/
extern void EcuM_AL_DriverInitTwo(const EcuM_ConfigType* ConfigPtr);

/**************************************************************************************************\
 *** FUNCTION:
 ***     EcuM_AL_DriverInitZero
 *** 
 *** DESCRIPTION:
 ***     This callout shall provide driver initialization and other hardware-related startup
 ***     activities for loading the post-build configuration data. Beware: Here only
 ***     pre-compile and link-time configurable modules may be used.
 ***     
 ***     ServiceID  : 0x31
 ***     Reentrant  : false
 ***
 *** PARAMETERS:
 ***     -/-
 *** 
 *** RETURNS:
 ***    void
\**************************************************************************************************/
extern void EcuM_AL_DriverInitZero(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     EcuM_AL_DriverRestart
 *** 
 *** DESCRIPTION:
 ***     This callout shall provide driver initialization and other hardware-related startup
 ***     activities in the wakeup case.
 ***     
 ***     ServiceID  : 0x45
 ***     Reentrant  : false
 ***
 *** PARAMETERS:
 ***    Type           Name   Description
 ***    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***	const EcuM_ConfigType* ConfigPtr Pointer to the EcuM post-build configuration which contains pointers to all other BSW
 ***     module post-build configurations. 
 *** 
 *** RETURNS:
 ***    void
\**************************************************************************************************/
extern void EcuM_AL_DriverRestart(const EcuM_ConfigType* ConfigPtr);

/**************************************************************************************************\
 *** FUNCTION:
 ***     EcuM_AL_SwitchOff
 *** 
 *** DESCRIPTION:
 ***     This callout shall take the code for shutting off the power supply of the ECU. If the
 ***     ECU cannot unpower itself, a reset may be an adequate reaction.
 ***     
 ***     ServiceID  : 0x3e
 ***     Reentrant  : false
 ***
 *** PARAMETERS:
 ***     -/-
 *** 
 *** RETURNS:
 ***    void
\**************************************************************************************************/
extern void EcuM_AL_SwitchOff(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     EcuM_CheckRamHash
 *** 
 *** DESCRIPTION:
 ***     This callout is intended to provide a RAM integrity test. The goal of this test is to
 ***     ensure that after a long SLEEP duration, RAM contents is still consistent.
 ***     The check does not need to be exhaustive since this would consume quite some
 ***     processing time during wakeups. A well designed check will execute quickly
 ***     and detect RAM integrity defects with a sufficient probability. This
 ***     specification does not make any assumption about the algorithm chosen for a
 ***     particular ECU. The areas of RAM which will be checked have to be chosen
 ***     carefully. It depends on the check algorithm itself and the task structure.
 ***     Stack contents of the task executing the RAM check e.g. very likely cannot
 ***     be checked. It is good practice to have the hash generation and checking in
 ***     the same task and that this task is not preemptible and that there is only
 ***     little activity between hash generation and hash check. The RAM check itself
 ***     is provided by the system designer. In case of applied multi core and
 ***     existence of Satellite-EcuM(s): this API will be called by the Master-EcuM
 ***     only.
 ***     
 ***     ServiceID  : 0x43
 ***     Reentrant  : false
 ***
 *** PARAMETERS:
 ***     -/-
 *** 
 *** RETURNS:
 ***    uint8
\**************************************************************************************************/
extern uint8 EcuM_CheckRamHash(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     EcuM_CheckValidation
 *** 
 *** DESCRIPTION:
 ***     This callout is called by the EcuM to validate a wakeup source. If a valid wakeup has
 ***     been detected, it shall be reported to EcuM via EcuM_ValidateWakeupEvent().
 ***     
 ***     ServiceID  : 0x47
 ***     Reentrant  : false
 ***
 *** PARAMETERS:
 ***    Type           Name   Description
 ***    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***	EcuM_WakeupSourceType wakeupSource  
 *** 
 *** RETURNS:
 ***    void
\**************************************************************************************************/
extern void EcuM_CheckValidation(EcuM_WakeupSourceType wakeupSource);

/**************************************************************************************************\
 *** FUNCTION:
 ***     EcuM_CheckWakeup
 *** 
 *** DESCRIPTION:
 ***     This callout is called by the EcuM to poll a wakeup source. It shall also be called
 ***     by the ISR of a wakeup source to set up the PLL and check other wakeup
 ***     sources that may be connected to the same interrupt.
 ***     
 ***     ServiceID  : 0x42
 ***     Reentrant  : false
 ***
 *** PARAMETERS:
 ***    Type           Name   Description
 ***    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***	EcuM_WakeupSourceType wakeupSource  
 *** 
 *** RETURNS:
 ***    void
\**************************************************************************************************/
extern void EcuM_CheckWakeup(EcuM_WakeupSourceType wakeupSource);

/**************************************************************************************************\
 *** FUNCTION:
 ***     EcuM_DeterminePbConfiguration
 *** 
 *** DESCRIPTION:
 ***     This callout should evaluate some condition, like port pin or NVRAM value, to
 ***     determine which post-build configuration shall be used in the remainder of
 ***     the startup process. It shall load this configuration data into a piece of
 ***     memory that is accessible by all BSW modules and shall return a pointer to
 ***     the EcuM post-build configuration as a base for all BSW module post-build
 ***     configrations.
 ***     
 ***     ServiceID  : 0x32
 ***     Reentrant  : false
 ***
 *** PARAMETERS:
 ***     -/-
 *** 
 *** RETURNS:
 ***    const EcuM_ConfigType*
\**************************************************************************************************/
extern const EcuM_ConfigType* EcuM_DeterminePbConfiguration(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     EcuM_DisableWakeupSources
 *** 
 *** DESCRIPTION:
 ***     The ECU Manager Module calls EcuM_DisableWakeupSources to set the wakeup source(s)
 ***     defined in the wakeupSource bitfield so that they are not able to wake the
 ***     ECU up.
 ***     
 ***     ServiceID  : 0x44
 ***     Reentrant  : false
 ***
 *** PARAMETERS:
 ***    Type           Name   Description
 ***    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***	EcuM_WakeupSourceType wakeupSource  
 *** 
 *** RETURNS:
 ***    void
\**************************************************************************************************/
extern void EcuM_DisableWakeupSources(EcuM_WakeupSourceType wakeupSource);

/**************************************************************************************************\
 *** FUNCTION:
 ***     EcuM_EnableWakeupSources
 *** 
 *** DESCRIPTION:
 ***     The ECU Manager Module calls EcuM_EnableWakeupSource to allow the system designer to
 ***     notify wakeup sources defined in the wakeupSource bitfield that SLEEP will
 ***     be entered and to adjust their source accordingly.
 ***     
 ***     ServiceID  : 0x3f
 ***     Reentrant  : false
 ***
 *** PARAMETERS:
 ***    Type           Name   Description
 ***    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***	EcuM_WakeupSourceType wakeupSource  
 *** 
 *** RETURNS:
 ***    void
\**************************************************************************************************/
extern void EcuM_EnableWakeupSources(EcuM_WakeupSourceType wakeupSource);

/**************************************************************************************************\
 *** FUNCTION:
 ***     EcuM_EndCheckWakeup
 *** 
 *** DESCRIPTION:
 ***     This API is called by any SW Module whose wakeup source is checked asynchronously
 ***     (e.g. asynchronous Can Trcv Driver) and the Check of the Wakeup returns a
 ***     negative Result (no Wakeup by this Source). The API cancels the
 ***     CheckWakeupTimer for the WakeupSource. If the correponding CheckWakeupTimer
 ***     is canceled the check of this wakeup source is finished.
 ***     
 ***     ServiceID  : 0x00
 ***     Reentrant  : false
 ***
 *** PARAMETERS:
 ***    Type           Name   Description
 ***    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***	EcuM_WakeupSourceType WakeupSource For this wakeup source the corresponding CheckWakeupTimer shall be canceled. 
 *** 
 *** RETURNS:
 ***    void
\**************************************************************************************************/
extern void EcuM_EndCheckWakeup(EcuM_WakeupSourceType WakeupSource);

/**************************************************************************************************\
 *** FUNCTION:
 ***     EcuM_ErrorHook
 *** 
 *** DESCRIPTION:
 ***     The ECU State Manager will call the error hook if the error codes
 ***     \"ECUM_E_RAM_CHECK_FAILED\" or \"ECUM_E_CONFIGURATION_DATA_INCONSISTENT\"
 ***     occur. In this situation it is not possible to continue processing and the
 ***     ECU must be stopped. The integrator may choose the modality how the ECU is
 ***     stopped, i.e. reset, halt, restart, safe state etc.
 ***     
 ***     ServiceID  : 0x30
 ***     Reentrant  : false
 ***
 *** PARAMETERS:
 ***    Type           Name   Description
 ***    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***	uint16 reason Reason for calling the error hook 
 *** 
 *** RETURNS:
 ***    void
\**************************************************************************************************/
extern void EcuM_ErrorHook(uint16 reason);

/**************************************************************************************************\
 *** FUNCTION:
 ***     EcuM_GenerateRamHash
 *** 
 *** DESCRIPTION:
 ***     see EcuM_CheckRamHash
 ***     
 ***     ServiceID  : 0x40
 ***     Reentrant  : false
 ***
 *** PARAMETERS:
 ***     -/-
 *** 
 *** RETURNS:
 ***    void
\**************************************************************************************************/
extern void EcuM_GenerateRamHash(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     EcuM_OnEnterRun
 *** 
 *** DESCRIPTION:
 ***     On entry of RUN state is very similar to \"just after startup\". This call allows the
 ***     system designer to notify that RUN state has been reached.
 ***     
 ***     ServiceID  : 0x37
 ***     Reentrant  : false
 ***
 *** PARAMETERS:
 ***     -/-
 *** 
 *** RETURNS:
 ***    void
\**************************************************************************************************/
extern void EcuM_OnEnterRun(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     EcuM_OnExitPostRun
 *** 
 *** DESCRIPTION:
 ***     This call allows the system designer to notify that the APP POST RUN state is about
 ***     to be left.
 ***     
 ***     ServiceID  : 0x39
 ***     Reentrant  : false
 ***
 *** PARAMETERS:
 ***     -/-
 *** 
 *** RETURNS:
 ***    void
\**************************************************************************************************/
extern void EcuM_OnExitPostRun(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     EcuM_OnExitRun
 *** 
 *** DESCRIPTION:
 ***     This call allows the system designer to notify that the APP RUN state is about to be
 ***     left.
 ***     
 ***     ServiceID  : 0x38
 ***     Reentrant  : false
 ***
 *** PARAMETERS:
 ***     -/-
 *** 
 *** RETURNS:
 ***    void
\**************************************************************************************************/
extern void EcuM_OnExitRun(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     EcuM_OnGoOffOne
 *** 
 *** DESCRIPTION:
 ***     This call allows the system designer to notify that the GO OFF I state is about to be
 ***     entered.
 ***     
 ***     ServiceID  : 0x3c
 ***     Reentrant  : false
 ***
 *** PARAMETERS:
 ***     -/-
 *** 
 *** RETURNS:
 ***    void
\**************************************************************************************************/
extern void EcuM_OnGoOffOne(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     EcuM_OnGoOffTwo
 *** 
 *** DESCRIPTION:
 ***     This call allows the system designer to notify that the GO OFF II state is about to
 ***     be entered.
 ***     
 ***     ServiceID  : 0x3d
 ***     Reentrant  : false
 ***
 *** PARAMETERS:
 ***     -/-
 *** 
 *** RETURNS:
 ***    void
\**************************************************************************************************/
extern void EcuM_OnGoOffTwo(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     EcuM_OnGoSleep
 *** 
 *** DESCRIPTION:
 ***     This call allows the system designer to notify that the GO SLEEP state is about to be
 ***     entered.
 ***     
 ***     ServiceID  : 0x3b
 ***     Reentrant  : false
 ***
 *** PARAMETERS:
 ***     -/-
 *** 
 *** RETURNS:
 ***    void
\**************************************************************************************************/
extern void EcuM_OnGoSleep(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     EcuM_OnPrepShutdown
 *** 
 *** DESCRIPTION:
 ***     This call allows the system designer to notify that the PREP SHUTDOWN state is about
 ***     to be entered.
 ***     
 ***     ServiceID  : 0x3a
 ***     Reentrant  : false
 ***
 *** PARAMETERS:
 ***     -/-
 *** 
 *** RETURNS:
 ***    void
\**************************************************************************************************/
extern void EcuM_OnPrepShutdown(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     EcuM_OnRTEStartup
 *** 
 *** DESCRIPTION:
 ***     
 ***     
 ***     ServiceID  : 0x36
 ***     Reentrant  : false
 ***
 *** PARAMETERS:
 ***     -/-
 *** 
 *** RETURNS:
 ***    void
\**************************************************************************************************/
extern void EcuM_OnRTEStartup(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     EcuM_OnWakeupReaction
 *** 
 *** DESCRIPTION:
 ***     This callout gives the system designer the chance to intercept the automatic boot
 ***     behavior and to override the wakeup reaction computed from wakeup source.
 ***     
 ***     ServiceID  : 0x49
 ***     Reentrant  : false
 ***
 *** PARAMETERS:
 ***    Type           Name   Description
 ***    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***	EcuM_WakeupReactionType wact The wakeup reaction computed by ECU State Manager 
 *** 
 *** RETURNS:
 ***    EcuM_WakeupReactionType
\**************************************************************************************************/
extern EcuM_WakeupReactionType EcuM_OnWakeupReaction(EcuM_WakeupReactionType wact);

/**************************************************************************************************\
 *** FUNCTION:
 ***     EcuM_SleepActivity
 *** 
 *** DESCRIPTION:
 ***     This callout is invoked periodically in all reduced clock sleep modes. It is
 ***     explicitely allowed to poll wakeup sources from this callout and to call
 ***     wakeup notification functions to indicate the end of the sleep state to the
 ***     ECU State Manager.
 ***     
 ***     ServiceID  : 0x41
 ***     Reentrant  : false
 ***
 *** PARAMETERS:
 ***     -/-
 *** 
 *** RETURNS:
 ***    void
\**************************************************************************************************/
extern void EcuM_SleepActivity(void);

/**************************************************************************************************\
 *** FUNCTION:
 ***     EcuM_StartCheckWakeup
 *** 
 *** DESCRIPTION:
 ***     This API is called by the ECU Firmware to start the CheckWakeupTimer for the
 ***     corresponding WakeupSource. If EcuMCheckWakeupTimeout ' 0 the
 ***     CheckWakeupTimer for the WakeupSource is started. If EcuMCheckWakeupTimeout
 ***     '= 0 the API call is ignored by the EcuM.
 ***     
 ***     ServiceID  : 0x00
 ***     Reentrant  : false
 ***
 *** PARAMETERS:
 ***    Type           Name   Description
 ***    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***	EcuM_WakeupSourceType WakeupSource For this wakeup source the corresponding CheckWakeupTimer shall be started. 
 *** 
 *** RETURNS:
 ***    void
\**************************************************************************************************/
extern void EcuM_StartCheckWakeup(EcuM_WakeupSourceType WakeupSource);

/**************************************************************************************************\
 *** FUNCTION:
 ***     EcuM_StartWakeupSources
 *** 
 *** DESCRIPTION:
 ***     The callout shall start the given wakeup source(s) so that they are ready to perform
 ***     wakeup validation.
 ***     
 ***     ServiceID  : 0x46
 ***     Reentrant  : false
 ***
 *** PARAMETERS:
 ***    Type           Name   Description
 ***    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***	EcuM_WakeupSourceType wakeupSource  
 *** 
 *** RETURNS:
 ***    void
\**************************************************************************************************/
extern void EcuM_StartWakeupSources(EcuM_WakeupSourceType wakeupSource);

/**************************************************************************************************\
 *** FUNCTION:
 ***     EcuM_StopWakeupSources
 *** 
 *** DESCRIPTION:
 ***     The callout shall stop the given wakeup source(s) after unsuccessful wakeup
 ***     validation.
 ***     
 ***     ServiceID  : 0x48
 ***     Reentrant  : false
 ***
 *** PARAMETERS:
 ***    Type           Name   Description
 ***    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ***	EcuM_WakeupSourceType wakeupSource  
 *** 
 *** RETURNS:
 ***    void
\**************************************************************************************************/
extern void EcuM_StopWakeupSources(EcuM_WakeupSourceType wakeupSource);


#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */ 

#endif /* ECUM_CALLOUT_H */
/*------------------------------------------------------------------------------------------------*\
  END OF FILE
\*------------------------------------------------------------------------------------------------*/
