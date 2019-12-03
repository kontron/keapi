/*
 *<KHeader>
 *+=========================================================================
 *I  Project Name: EApiDK Embedded Application Development Kit
 *+=========================================================================
 *I  $HeadURL: https://sourceforge.net/p/eapidk/code/HEAD/tree/trunk/include/linux/EApiOs.h $
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
 *I  File Location        : include\linux
 *I
 *+=========================================================================
 *</KHeader>
 */
/* Windows NT Common Header */
#ifndef _EAPIOS_H_        
#define _EAPIOS_H_

#ifndef __IN
#  define __IN
#endif
#include <stdlib.h>
#include <assert.h>
#include <inttypes.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>
#include <wchar.h>
#include <ctype.h>


#if defined _WIN32 || defined __CYGWIN__
  #ifdef EAPI_BUILDING_LIB
    #ifdef __GNUC__
      #define EAPI_CALLTYPE __attribute__((dllexport))
    #else
      #define EAPI_CALLTYPE __declspec(dllexport) /* Note: actually gcc seems to also supports this syntax. */
    #endif
  #else
    #ifdef __GNUC__
      #define EAPI_CALLTYPE __attribute__((dllimport))
    #else
      #define EAPI_CALLTYPE __declspec(dllimport) /* Note: actually gcc seems to also supports this syntax. */
    #endif
  #endif
#else
  #if __GNUC__ >= 4
    #define EAPI_CALLTYPE __attribute__ ((visibility("default")))
  #else
    #define EAPI_CALLTYPE
  #endif
#endif
#ifdef __GNUC__ 
#ifdef __i386__
#    ifndef __fastcall
#      define __fastcall __attribute__((fastcall))
#    endif
#    ifndef __stdcall
#      define __stdcall __attribute__((stdcall))
#    endif
#    ifndef __cdecl
#      define __cdecl __attribute__((cdecl))
#    endif
#else
#    ifndef __fastcall
#      define __fastcall
#    endif
#    ifndef __stdcall
#      define __stdcall
#    endif
#    ifndef __cdecl
#      define __cdecl
#    endif
#endif
#  ifndef _fastcall
#    define _fastcall __fastcall
#  endif
#  ifndef _stdcall
#    define _stdcall __stdcall
#  endif
#  ifndef _cdecl
#    define _cdecl __cdecl
#  endif

#endif

#define EAPI_PRINTF_ARG(Format, Arg) __attribute__((format(printf, Format, Arg)))
void EApiSleepns(unsigned long ns);
#define EAPI_EMUL_DELAY_NS(x) EApiSleepns(x)

#ifndef EApiSleep
#  define EApiSleep(x) usleep(x*1000)
#endif



#ifdef UNICODE
#  define TEXT(x) Lx
#  define EAPI_strlen         wcslen
#  define EAPI_strnlen        wcsnlen
#  define EAPI_strncpy        wcsncpy
#  define EAPI_vsnprintf      vsnwprintf
#  define EAPI_fprintf        fwprintf
#  define EAPI_printf         wprintf
#  define EAPI_fopen          _wfopen
#else
#  define TEXT(x) x
#  define EAPI_strlen         strlen
#  define EAPI_strnlen        strnlen
#  define EAPI_strncpy        strncpy
#  define EAPI_vsnprintf      vsnprintf
#  define EAPI_fprintf        fprintf
#  define EAPI_fopen          fopen
#  define EAPI_printf         printf
#endif

#define EAPI_vsnprintfA     vsnprintf
#define EAPI_strdup         strdup
#define EAPI_strncpyA       strncpy

#define _strdup strdup

#endif /* _EAPIOS_H_ */


