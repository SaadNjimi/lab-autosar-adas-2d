/**************************************************************************************************\
 *** Dap_Limits.h
 ***
 *** Data access point limits for data types
 ***
 *** Copyright (c) 2015 by dSPACE GmbH, Paderborn, Germany.
 *** All Rights Reserved.
\**************************************************************************************************/

#ifndef DAP_LIMIITS_H
#define DAP_LIMIITS_H

#include "limits.h"
#include "float.h"

#define DAP_LIMITS_INT8_MAX             SCHAR_MAX
#define DAP_LIMITS_INT8_MIN             SCHAR_MIN
#define DAP_LIMITS_INT16_MAX            SHRT_MAX
#define DAP_LIMITS_INT16_MIN            SHRT_MIN
#define DAP_LIMITS_INT32_MAX            INT_MAX
#define DAP_LIMITS_INT32_MIN            INT_MIN
#define DAP_LIMITS_INT64_MAX            LLONG_MAX
#define DAP_LIMITS_INT64_MIN            LLONG_MIN

#define DAP_LIMITS_UINT8_MAX            UCHAR_MAX
#define DAP_LIMITS_UINT8_MIN            0
#define DAP_LIMITS_UINT16_MAX           USHRT_MAX
#define DAP_LIMITS_UINT16_MIN           0
#define DAP_LIMITS_UINT32_MAX           UINT_MAX
#define DAP_LIMITS_UINT32_MIN           0
#define DAP_LIMITS_UINT64_MAX           ULLONG_MAX
#define DAP_LIMITS_UINT64_MIN           0

#define DAP_LIMITS_FLOAT32_MAX          FLT_MAX
#define DAP_LIMITS_FLOAT32_MIN          -FLT_MAX
#define DAP_LIMITS_FLOAT64_MAX          DBL_MAX
#define DAP_LIMITS_FLOAT64_MIN          -DBL_MAX

#endif
