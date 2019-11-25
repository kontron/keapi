/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron AG */

/* KEAPI library initialization functions. */

#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "keapi_inc.h"
#include "PlatformDependence.h"
#if PLATFORM(Windows)
#ifndef KEAPI_MOCK
#include "KeapiDriver.h"
#endif
#endif /* PLATFORM (Windows) */
#if PLATFORM(WindowsCE)
#include "log.h"
#endif

#if PLATFORM(Linux)
#include <stdio.h>
#include "globals.h"
#endif

#if PLATFORM(Windows)
#include "WmiHelper.h"
#endif /* PLATFORM (Windows) */

#if PLATFORM(VxWorks)
#include "versionK.h"
KONTRON_VERSION(libinit_c, 01e) /* version identifier */
#endif /* VxWorks */

#if PLATFORM(Linux)
int keapiLocalInitialized = 1; /* Linux: always initialized */
#endif

KEAPI_CALLTYPE KEAPI_RETVAL KEApiLibInitialize()
{
	uint32_t retval = KEAPI_RET_ERROR;

#if PLATFORM(Linux)
#ifndef KEAPI_MOCK
	pthread_mutex_init(&gpio_mutex, NULL);
	pthread_mutex_init(&i2c_mutex, NULL);
	pthread_mutex_init(&smbus_mutex, NULL);
	pthread_mutex_init(&eep_mutex, NULL);
	pthread_mutex_init(&wdt_mutex, NULL);
#endif
#endif

	retval = KEAPI_RET_SUCCESS;

#if PLATFORM(Windows)
	if (retval == KEAPI_RET_SUCCESS) {
		MyCoInitialize();
#ifndef KEAPI_MOCK
		COMMON_DRIVER_CALL(retval, DRV_KEApiInit, ())
#endif
	}
#endif /* PLATFORM (Windows) */

#if PLATFORM(WindowsCE)
	if (retval == KEAPI_RET_SUCCESS) {
		retval = CeInitialize();
	}
#endif

#ifdef KEAPI_MOCK
	if (retval == KEAPI_RET_SUCCESS) {
		extern int MockInitialize(void);
		retval = MockInitialize();
	}
#endif

	return retval;
}

/******************************************************************************/
KEAPI_CALLTYPE KEAPI_RETVAL KEApiLibUnInitialize()
{
	uint32_t retval = KEAPI_RET_SUCCESS;

#if PLATFORM(Windows)
	MyCoUninitialize();
#ifndef KEAPI_MOCK
	COMMON_DRIVER_CALL(retval, DRV_KEApiDeInit, ());
#endif
#endif /* PLATFORM (Windows) */

#if PLATFORM(Linux)
#ifndef KEAPI_MOCK
	pthread_mutex_destroy(&gpio_mutex);
	pthread_mutex_destroy(&smbus_mutex);
	pthread_mutex_destroy(&i2c_mutex);
	pthread_mutex_destroy(&eep_mutex);
	pthread_mutex_destroy(&wdt_mutex);
#endif
#endif

#if PLATFORM(WindowsCE)
	CeUninitialize();
#endif

#ifdef KEAPI_MOCK
	{
		extern void MockUninitialize(void);
		MockUninitialize();
	}
#endif

	return retval;
}

#ifndef KEAPI_LOCAL_ACCESS_DISABLED

/****************************************************************************
 * Local stub for Passwd - not supported for KEAPI Local */
KEAPI_RETVAL KEApiL_Passwd(const char *pNewPassword, const char *pOldPassword)
{
	UNUSED(pNewPassword);
	UNUSED(pOldPassword);

	return KEAPI_RET_FUNCTION_NOT_SUPPORTED;
}

#endif /* KEAPI_LOCAL_ACCESS_DISABLED */
