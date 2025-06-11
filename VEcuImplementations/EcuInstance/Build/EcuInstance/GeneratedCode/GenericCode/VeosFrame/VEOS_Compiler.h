/**************************************************************************************************\
 *** VEOS_Compiler.h
 ***
 *** VEOS frame header file for compiler specific definitions for the GCC compiler.
 ***
 *** Copyright (c) 2018 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef VEOS_COMPILER_H
#define VEOS_COMPILER_H

/*------------------------------------------------------------------------------------------------*\
  DEFINES (OPT)
\*------------------------------------------------------------------------------------------------*/

#define VEOS_CAL    __attribute__((nocommon, section("VEOS_CAL"), visibility("default"))) volatile

#endif /* VEOS_COMPILER_H */
