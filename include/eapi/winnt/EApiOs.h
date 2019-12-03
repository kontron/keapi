/*
 *<KHeader>
 *+=========================================================================
 *I  Project Name: EApiDK Embedded Application Development Kit
 *+=========================================================================
 *I  $HeadURL: https://sourceforge.net/p/eapidk/code/HEAD/tree/trunk/include/winnt/EApiOs.h $
 *+=========================================================================
 *I  SPDX-License-Identifier: BSD-3-Clause
 *I  Copyright (C) 2019 Kontron Europe GmbH
 *I  Author: John Kearney, John.Kearney@kontron.com
 *I
 *I Description: Auto Created for EApiOs.h
 *I
 *+-------------------------------------------------------------------------
 *I
 *I  File Name            : EApiOs.h
 *I  File Location        : include\winnt
 *I
 *+=========================================================================
 *</KHeader>
 */
/* Windows NT Common Header */
#ifndef _EAPIOS_H_        
#define _EAPIOS_H_

#include <windows.h>
#include <assert.h>
#ifdef _MSC_VER 
#include "inttypes.h"

#ifndef __IN
#  define __IN
#endif

//#define EAPI_CALLTYPE WINAPI
#define EAPI_CALLTYPE __declspec (dllexport)
#ifdef __cplusplus
extern "C" {
#endif
void EApiSleepus(unsigned long ms);
#ifdef __cplusplus
}
#endif

#define EAPI_PRINTF_ARG(Format, Arg) 
#define EAPI_EMUL_DELAY_NS(x) EApiSleepus(x/1000)

#ifndef EApiSleep
#  define EApiSleep(x) Sleep(x)
#endif


#define inline __inline

#ifdef UNICODE
#  define EAPI_strlen         wcslen
#  define EAPI_strnlen        wcsnlen
#  define EAPI_strncpy        wcsncpy
#  define EAPI_vsnprintf      _vsnwprintf
#  define EAPI_fprintf        fwprintf
#  define EAPI_printf         wprintf
#  define EAPI_fopen          _wfopen
#else
#  define EAPI_strlen         strlen
#  define EAPI_strnlen        strnlen
#  define EAPI_strncpy        strncpy
#  define EAPI_vsnprintf      _vsnprintf
#  define EAPI_fprintf        fprintf
#  define EAPI_fopen          fopen
#  define EAPI_printf         printf
#endif

#define EAPI_vsnprintfA     _vsnprintf
#define EAPI_strdup         _strdup
#define EAPI_strncpyA       strncpy
#define EAPI_strnlenA       strnlen
#define EAPI_strlenA        strlen

#define         MSVC_1_5         800
#define         MSVC_98         1200
#define         MSVC_2003       1300 /* .NET */
#define         MSVC_2005       1400
#define         MSVC_2008       1500
#define         MSVC_2010       1600

/* unreferenced formal parameter */
#  if ( _MSC_VER >= MSVC_1_5 )
#    pragma warning(disable:4127) /* conditional expression is constant */
#  endif /* if ( _MSC_VER >= MSVC_1_5 ) */
#endif /* _MSC_VER */

#endif /* _EAPIOS_H_ */


