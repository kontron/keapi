/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron Europe GmbH */

/* Include files for library compilation. */

#ifndef _KEAPI_INC_H
#define _KEAPI_INC_H

#include <assert.h>

#ifdef KEAPI_REMOTE
#include "keapi_r.h"
#else
#include "keapi.h"
#endif

#ifdef __VXWORKS__
#include "versionK.h"
KONTRON_VERSION(keapi_inc_h, 01b) /* version identifier */
#endif /* __VXWORKS__ */

#define PLATFORM(x) PLATFORM_##x

#ifdef _WIN32
#	ifdef _WIN32_WCE
#		define PLATFORM_WindowsCE 1
#	else
#		define PLATFORM_Windows 1
#	endif
#elif defined(__linux__) || defined(__CYGWIN__)
#	define PLATFORM_Linux 1
#	ifndef KEAPI_MOCK
#		include "linux_support.h"
#	endif
#elif defined(__VXWORKS__)
#	define PLATFORM_VxWorks 1
#	include <vxWorks.h>
#else
#	error "unsupported platform"
#endif

#include "keapi_defs.h"

enum LogLevel {
	LV_ERROR = 0x01,
	LV_WARN = 0x02,
	LV_INFO = 0x04,
	LV_DEBUG = 0x08,
	LV_TRACE = 0x10,
	LV_ALLTIME = 0xFF
};

#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#define MAX_KEAPI_COUNTER 512
#define MAX_KEAPI_BINARY (256 * 1024)

#if PLATFORM(WindowsCE)
typedef wchar_t KEAPI_TCHAR;
typedef wchar_t *KEAPI_PTCHAR;
#endif

#if PLATFORM(Linux)
#include <stdlib.h>
#endif /* PLATFORM (Linux) */

#if PLATFORM(VxWorks)
#include <stdlib.h>
#endif /* PLATFORM (VxWorks) */

extern int keapiLocalInitialized;

#define UNUSED(x) (void)(x)

#endif /* _KEAPI_INC_H */
