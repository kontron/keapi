/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron Europe GmbH */

/* KEAPI watchdog functions. */

#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <linux/watchdog.h>
#include <signal.h>
#include <jansson.h>

#include "globals.h"
#include "keapi_inc.h"

#define WDT_TMP_PATH "/tmp/keapi.tmp"

#ifndef WDIO_GS_OPT_IRQ /* for KONTRON_LIKE_KOM */
#define WDIOC_OC_SETTIME	0x0000
#define WDIOC_OC_ENABLE		0x0100
#define WDIOC_OC_SETROUTING	0x0200
#define WDIO_GS_OPT_IRQ		0x0001 /* Supports Routing to IRQ */
#define WDIO_GS_OPT_NMI		0x0002 /* Supports Routing to NMI */
#define WDIO_GS_OPT_RES		0x0004 /* Supports Routing to RES */
#define WDIO_GS_OPT_NMI_RES	0x0008 /* Supports Cascade Mode NMI/Reset */
#define WDIO_GS_OPT_TIMEOUT	0x0020 /* Supports Timeout */
#define WDIO_GS_OPT_IRQ_RES	0x0040 /* Supports Cascade Mode IRQ/Reset */
#define MAX_WDT_TIME_VALS	20 /* Maximum support different time values */
#define WDIOC_TIMEVALUES _IOR(WATCHDOG_IOCTL_BASE, 51, int[MAX_WDT_TIME_VALS])
#endif

struct WdtInfo {
	int32_t maxTimeout; /* maximum possible timeout */

	int32_t minTimeout; /* minimum possible timeout */

	char path[KEAPI_MAX_STR]; /* path to watchdog need only when wanna enable/trigger/disable it,
				   * and watchdog have not standard path (/dev/watchdog)*/

	enum FeatureStyle wdtStyle; /* watchdog style. support one of 2 cases:
				     * LINUX_LIKE - only MaxTimeout (mandatory) and Path (optional)
				     * are used as available options. I.e. this is simple classic Linux watchdog.
				     * KONTRON_LIKE_KEM - TODO
				     * KONTRON_LIKE_KOM - TODO
				     * KEAPI_FULL - TODO
				     */
};
static struct WdtInfo __wdtInfo;
static struct WdtInfo *wdtInfo = NULL;

/* Structure and functions for storing stages for
 * LINUX_LIKE and KONTRON_LIKE_KEM wdts */
typedef struct wdt_stage {
	int32_t stageNr;
	int32_t timeout;
	int32_t mode;
} WDT_STAGE, *PWDT_STAGE;

int32_t readWdtSetup(int fd, PWDT_STAGE *setup);
void writeWdtSetup(int fd, PWDT_STAGE setup, int32_t setup_count);
void resetWdtSetup(int fd);
int32_t updateWdtSetup(int32_t stage, int32_t timeout, int32_t mode, PWDT_STAGE *setup, int32_t stage_count);

/* Watchdog: KONTRON_LIKE_KOM */
static int keapiEvtconfig;
static int getWdTimeIndex(int fd, int time);
void wd_signal_handler(int sig);

/*
 * GetWatchgdogConfig
 *
 * Function-helper reads watchdog json configuration file.
 * Return values:
 *  - KEAPI_RET_ERROR - unable to find config or it is incorrect;
 *  - KEAPI_RET_SUCCESS - if into the file we have found and timeout
 *  and any path to wdt;
 */
int32_t GetWatchgdogConfig()
{
	int32_t ret;
	json_t *root, *data;
	json_error_t error;
	char bufstr[KEAPI_MAX_STR];

	wdtInfo = NULL;

	if ((ret = checkRAccess(WDTCONF_PATH)) != KEAPI_RET_SUCCESS)
		return ret;

	root = json_load_file(WDTCONF_PATH, JSON_DECODE_ANY, &error);
	if (!root)
		return KEAPI_RET_ERROR;

	data = json_object_get(root, "maxTimeout");
	if (!json_is_integer(data)) {
		json_decref(root);
		return KEAPI_RET_ERROR;
	}
	__wdtInfo.maxTimeout = json_integer_value(data);

	data = json_object_get(root, "minTimeout");
	if (!json_is_integer(data)) {
		json_decref(root);
		return KEAPI_RET_ERROR;
	}
	__wdtInfo.minTimeout = json_integer_value(data);

	data = json_object_get(root, "wdtPath");
	if (json_is_string(data)) {
		strcpy(__wdtInfo.path, json_string_value(data));
	} else
		strcpy(__wdtInfo.path, WATCHDOG_PATH);

	data = json_object_get(root, "wdtStyle");
	if (!json_is_string(data)) {
		__wdtInfo.wdtStyle = LINUX_LIKE;
		json_decref(root);
		wdtInfo = &__wdtInfo;
		return KEAPI_RET_SUCCESS;
	}
	strcpy(bufstr, json_string_value(data));

	if (strcasecmp(bufstr, "linux-like") == 0)
		__wdtInfo.wdtStyle = LINUX_LIKE;
	else if (strcasecmp(bufstr, "keapi-full") == 0)
		__wdtInfo.wdtStyle = KEAPI_FULL;
	else if (strcasecmp(bufstr, "kontron-like-kem") == 0)
		__wdtInfo.wdtStyle = KONTRON_LIKE_KEM;
	else if (strcasecmp(bufstr, "kontron-like-kom") == 0)
		__wdtInfo.wdtStyle = KONTRON_LIKE_KOM;
	else {
		json_decref(root);
		return KEAPI_RET_ERROR;
	}

	json_decref(root);

	wdtInfo = &__wdtInfo;

	return KEAPI_RET_SUCCESS;
}

/*******************************************************************************/

KEAPI_RETVAL KEApiL_WatchdogGetCaps(int32_t *pMaxTimeout, int32_t *pMinTimeout, int32_t *pStagesNr)
{
	int32_t ret;

	if (pMaxTimeout == NULL || pMinTimeout == NULL || pStagesNr == NULL)
		return KEAPI_RET_PARAM_NULL;

	if (wdtInfo == NULL)
		if ((ret = GetWatchgdogConfig()) != KEAPI_RET_SUCCESS)
			return ret;

	*pMaxTimeout = wdtInfo->maxTimeout;
	*pMinTimeout = wdtInfo->minTimeout;

	switch (wdtInfo->wdtStyle) {
	case LINUX_LIKE:
		*pStagesNr = 1;
		break;
	case KONTRON_LIKE_KOM: //TODO
		*pStagesNr = 2;
		break;
	case KONTRON_LIKE_KEM:
	case KEAPI_FULL:
		return KEAPI_RET_FUNCTION_NOT_IMPLEMENTED;
	default:
		return KEAPI_RET_ERROR;
	}

	return KEAPI_RET_SUCCESS;
}

/*******************************************************************************/

KEAPI_RETVAL KEApiL_WatchdogSetup(int32_t stage, int32_t timeout, int32_t mode)
{
	int32_t fd, fdtmp, ret = 0;
	int32_t stage_count = 0;
	PWDT_STAGE wdtSets = NULL;

	if (wdtInfo == NULL)
		if ((ret = GetWatchgdogConfig()) != KEAPI_RET_SUCCESS)
			return ret;

	if (stage < 0 ||
	    (mode != KEAPI_WD_MODE_DISABLE && (timeout < wdtInfo->minTimeout || timeout > wdtInfo->maxTimeout)))
		return KEAPI_RET_PARAM_ERROR;

	switch (wdtInfo->wdtStyle) {
	case LINUX_LIKE:

		if ((mode != KEAPI_WD_MODE_RESET && mode != KEAPI_WD_MODE_DISABLE) || stage > 0)
			return KEAPI_RET_PARAM_ERROR;

		fdtmp = openLock(WDT_TMP_PATH, &wdt_mutex);
		stage_count = readWdtSetup(fdtmp, &wdtSets);
		stage_count = updateWdtSetup(stage, timeout, mode, &wdtSets, stage_count);
		writeWdtSetup(fdtmp, wdtSets, stage_count);
		closeUnlock(fdtmp, &wdt_mutex);

		if (wdtSets)
			free(wdtSets);

		break;

	case KONTRON_LIKE_KOM: // TODO: detailed check
	{
		int wdtIdx = -1;
		int32_t rc;

		if (stage > 1 || mode > 10)
			return KEAPI_RET_PARAM_ERROR;

		/* Check if watchdog file exists */
		if ((ret = checkRWAccess(wdtInfo->path)) != KEAPI_RET_SUCCESS) {
			if (ret == KEAPI_RET_RETRIEVAL_ERROR)
				return KEAPI_RET_FUNCTION_NOT_SUPPORTED;
			else
				return ret;
		}

		/* Open watchdog file /dev/watchdog */
		if ((fd = open(wdtInfo->path, O_WRONLY)) < 0)
			return KEAPI_RET_ERROR;

		fdtmp = openLock(WDT_TMP_PATH, &wdt_mutex);

		if (stage == 0) {
			keapiEvtconfig = 0;
			rc = write(fdtmp, &keapiEvtconfig, sizeof(keapiEvtconfig));
			if (rc == -1)
				return failIoctlCheckErrno(&fdtmp);
		} else if ((stage == 1) && (mode == KEAPI_WD_MODE_RESET)) {
			rc = read(fdtmp, &keapiEvtconfig, sizeof(keapiEvtconfig));
			if (rc == -1)
				return failIoctlCheckErrno(&fdtmp);

			if (keapiEvtconfig == WDIO_GS_OPT_IRQ) {
				keapiEvtconfig = WDIO_GS_OPT_IRQ_RES;
				lseek(fdtmp, 0, SEEK_SET);
				rc = write(fdtmp, &keapiEvtconfig, sizeof(keapiEvtconfig));
				if (rc == -1)
					return failIoctlCheckErrno(&fdtmp);

				closeUnlock(fdtmp, &wdt_mutex);
				close(fd);
				return KEAPI_RET_SUCCESS;
			} else {
				closeUnlock(fdtmp, &wdt_mutex);
				close(fd);
				return KEAPI_RET_PARAM_ERROR;
			}
		} else {
			closeUnlock(fdtmp, &wdt_mutex);
			close(fd);
			return KEAPI_RET_PARAM_ERROR;
		}

		// todo: should we check return value ??
		wdtIdx = getWdTimeIndex(fd, timeout);

		errno = 0;
		ret = ioctl(fd, WDIOC_SETOPTIONS, WDIOC_OC_SETTIME | (0xff & wdtIdx));
		if (ret < 0)
			return failIoctlCheckErrno(&fd);

		switch (mode) {
		case KEAPI_WD_MODE_RESET:
			keapiEvtconfig = WDIO_GS_OPT_RES;
			break;
		case KEAPI_WD_MODE_INTERRUPT:
			keapiEvtconfig = WDIO_GS_OPT_IRQ;
			break;
		case KEAPI_WD_MODE_TIMER_ONLY:
			keapiEvtconfig = WDIO_GS_OPT_TIMEOUT;
			break;
		case KEAPI_WD_MODE_DISABLE:
		default:
			close(fd);
			return KEAPI_RET_PARAM_ERROR;
		}

		lseek(fdtmp, 0, SEEK_SET);
		rc = write(fdtmp, &keapiEvtconfig, sizeof(keapiEvtconfig));
		if (rc == -1)
			return failIoctlCheckErrno(&fdtmp);

		closeUnlock(fdtmp, &wdt_mutex);
		close(fd);
		break;
	}

	case KONTRON_LIKE_KEM:
	case KEAPI_FULL:
		return KEAPI_RET_FUNCTION_NOT_IMPLEMENTED;
	default:
		return KEAPI_RET_ERROR;
	}

	return KEAPI_RET_SUCCESS;
}

/*******************************************************************************/

KEAPI_RETVAL KEApiL_WatchdogDisable()
{
	int32_t fd, options, ret;

	if (wdtInfo == NULL)
		if ((ret = GetWatchgdogConfig()) != KEAPI_RET_SUCCESS)
			return ret;

	switch (wdtInfo->wdtStyle) {
	case LINUX_LIKE:
	case KONTRON_LIKE_KOM:
		break;
	case KONTRON_LIKE_KEM:
	case KEAPI_FULL:
		return KEAPI_RET_FUNCTION_NOT_IMPLEMENTED;
	default:
		return KEAPI_RET_ERROR;
	}

	/* Check if wachdog file exists */
	if ((ret = checkRWAccess(wdtInfo->path)) != KEAPI_RET_SUCCESS) {
		if (ret == KEAPI_RET_RETRIEVAL_ERROR)
			return KEAPI_RET_FUNCTION_NOT_SUPPORTED;
		else
			return ret;
	}

	/* Open watchdog file /dev/watchdog */
	if ((fd = open(wdtInfo->path, O_WRONLY)) < 0)
		return KEAPI_RET_ERROR;

	/* Stop watchdog */
	options = WDIOS_DISABLECARD;
	ret = ioctl(fd, WDIOC_SETOPTIONS, &options);
	if (ret < 0)
		return failIoctlCheckErrno(&fd);

	// FIXME: in current implementation of wdt core in the kernel,
	// correct disabling of wdt via ioctl is enough.

	// /* send magic close char, to make sure watchdog will stop on close */
	//ret = write(fd, "V", 1);
	//if (ret < 0) {
	//	/* TODO: should we handle this situation ?*/
	//}

	close(fd);
	return KEAPI_RET_SUCCESS;
}

/*******************************************************************************/

KEAPI_RETVAL KEApiL_WatchdogEnable()
{
	int32_t fd, fdtmp, stage_count = 0;
	int32_t test_timeout, timeout, rc, ret;
	PWDT_STAGE wdtSets = NULL;

	ret = KEApiL_WatchdogDisable();
	if (ret != KEAPI_RET_SUCCESS)
		return ret;

	switch (wdtInfo->wdtStyle) {
	case LINUX_LIKE:

		fdtmp = openLock(WDT_TMP_PATH, &wdt_mutex);
		stage_count = readWdtSetup(fdtmp, &wdtSets);
		if (stage_count != 1 || wdtSets[0].stageNr != 0) {
			resetWdtSetup(fdtmp);
			closeUnlock(fdtmp, &wdt_mutex);
			if (wdtSets)
				free(wdtSets);
			return KEAPI_RET_ERROR;
		}

		if (wdtSets[0].mode == KEAPI_WD_MODE_DISABLE) {
			closeUnlock(fdtmp, &wdt_mutex);
			if (wdtSets)
				free(wdtSets);
			return KEAPI_RET_SUCCESS;
		}

		/* Set timeout (convert to seconds) and check if it was set correctly */
		test_timeout = timeout = wdtSets[0].timeout / 1000;

		/* Open watchdog file /dev/watchdog */
		if ((fd = open(wdtInfo->path, O_WRONLY)) < 0) {
			closeUnlock(fdtmp, &wdt_mutex);
			if (wdtSets)
				free(wdtSets);
			return KEAPI_RET_ERROR;
		}

		errno = 0;
		ret = ioctl(fd, WDIOC_SETTIMEOUT, &timeout);
		if (ret < 0 || (test_timeout != timeout)) {
			closeUnlock(fdtmp, &wdt_mutex);
			if (wdtSets)
				free(wdtSets);
			return failIoctlCheckErrno(&fd);
		}

		/* FIXME: maybe separate ENABLECARD is unnecessary,
		 * because WDIOC_SETTIMEOUT enables wdt too.*/
		///* Start watchdog */
		//options = WDIOS_ENABLECARD;
		//ret = ioctl(fd, WDIOC_SETOPTIONS, &options);
		//if (ret < 0) {
		//	resetWdtSetup(fdtmp);
		//	closeUnlock(fdtmp, &wdt_mutex);
		//	if (wdtSets)
		//		free(wdtSets);
		//	return failIoctlCheckErrno(&fd);
		//}

		close(fd);
		closeUnlock(fdtmp, &wdt_mutex);
		if (wdtSets)
			free(wdtSets);

		break;

	case KONTRON_LIKE_KOM: {
		fdtmp = openLock(WDT_TMP_PATH, &wdt_mutex);

		rc = read(fdtmp, &keapiEvtconfig, sizeof(keapiEvtconfig));
		if (rc == -1)
			return failIoctlCheckErrno(&fdtmp);

		closeUnlock(fdtmp, &wdt_mutex);

		/* Open watchdog file /dev/watchdog */
		if ((fd = open(wdtInfo->path, O_WRONLY)) < 0)
			return KEAPI_RET_ERROR;

		errno = 0;
		ret = ioctl(fd, WDIOC_SETOPTIONS, WDIOC_OC_SETROUTING | keapiEvtconfig);
		if (ret < 0)
			return failIoctlCheckErrno(&fd);

		ret = ioctl(fd, WDIOC_SETOPTIONS, WDIOC_OC_ENABLE);

		close(fd);
		break;
	}
	case KONTRON_LIKE_KEM:
	case KEAPI_FULL:
		return KEAPI_RET_FUNCTION_NOT_IMPLEMENTED;
	default:
		return KEAPI_RET_ERROR;
	}

	if (ret)
		return KEAPI_RET_ERROR;

	return KEAPI_RET_SUCCESS;
}

/*******************************************************************************/

KEAPI_RETVAL KEApiL_WatchdogTrigger()
{
	int32_t fd, ret;

	if (wdtInfo == NULL)
		if ((ret = GetWatchgdogConfig()) != KEAPI_RET_SUCCESS)
			return ret;

	switch (wdtInfo->wdtStyle) {
	case LINUX_LIKE:
	case KONTRON_LIKE_KOM:
		break;
	case KONTRON_LIKE_KEM:
	case KEAPI_FULL:
		return KEAPI_RET_FUNCTION_NOT_IMPLEMENTED;
	default:
		return KEAPI_RET_ERROR;
	}

	/* Check if wachdog file exists */
	if ((ret = checkRWAccess(wdtInfo->path)) != KEAPI_RET_SUCCESS) {
		if (ret == KEAPI_RET_RETRIEVAL_ERROR)
			return KEAPI_RET_FUNCTION_NOT_SUPPORTED;
		else
			return ret;
	}

	/* Open watchdog file /dev/watchdog */
	if ((fd = open(wdtInfo->path, O_WRONLY)) < 0)
		return KEAPI_RET_SETTING_ERROR;

	/* Stop watchdog */
	errno = 0;
	ret = ioctl(fd, WDIOC_KEEPALIVE, NULL);
	if (ret < 0)
		return failIoctlCheckErrno(&fd);

	close(fd);

	return KEAPI_RET_SUCCESS;
}

/*******************************************************************************/

KEAPI_RETVAL KEApiL_WatchdogGetExpired(int32_t *pWTE)
{
	int32_t fd, ret;
	int status;

	if (wdtInfo == NULL)
		if ((ret = GetWatchgdogConfig()) != KEAPI_RET_SUCCESS)
			return ret;

	/* Check if wachdog file exists */
	if ((ret = checkRWAccess(wdtInfo->path)) != KEAPI_RET_SUCCESS) {
		if (ret == KEAPI_RET_RETRIEVAL_ERROR)
			return KEAPI_RET_FUNCTION_NOT_SUPPORTED;
		else
			return ret;
	}

	switch (wdtInfo->wdtStyle) {
	case KONTRON_LIKE_KOM:

		/* Open watchdog file /dev/watchdog */
		if ((fd = open(wdtInfo->path, O_WRONLY)) < 0)
			return KEAPI_RET_SETTING_ERROR;

		errno = 0;
		ret = ioctl(fd, WDIOC_GETSTATUS, &status);
		if (ret < 0)
			return failIoctlCheckErrno(&fd);
		else if (ret == 0)
			*pWTE = status;

		close(fd);

		break;

	case LINUX_LIKE:
	case KONTRON_LIKE_KEM:
	case KEAPI_FULL:
		return KEAPI_RET_FUNCTION_NOT_IMPLEMENTED;
	default:
		return KEAPI_RET_ERROR;
	}

	if (ret)
		return KEAPI_RET_ERROR;

	return KEAPI_RET_SUCCESS;
}

/*******************************************************************************/

KEAPI_RETVAL KEApiL_WatchdogClearExpired()
{
	return KEAPI_RET_FUNCTION_NOT_IMPLEMENTED;
}

static struct sigaction sa;
int32_t keapiWdTimeoutOccured = 0;
/*******************************************************************************/

KEAPI_RETVAL KEApiL_WatchdogWaitUntilExpired()
{
	int32_t fd, ret;

	if (wdtInfo == NULL)
		if ((ret = GetWatchgdogConfig()) != KEAPI_RET_SUCCESS)
			return ret;

	/* Check if wachdog file exists */
	if ((ret = checkRWAccess(wdtInfo->path)) != KEAPI_RET_SUCCESS) {
		if (ret == KEAPI_RET_RETRIEVAL_ERROR)
			return KEAPI_RET_FUNCTION_NOT_SUPPORTED;
		else
			return ret;
	}

	switch (wdtInfo->wdtStyle) {
	case KONTRON_LIKE_KOM:

		/* Open watchdog file /dev/watchdog */
		if ((fd = open(wdtInfo->path, O_WRONLY)) < 0)
			return KEAPI_RET_SETTING_ERROR;

		keapiWdTimeoutOccured = 0;
		/* Install irq handler */
		sa.sa_handler = wd_signal_handler;
		sigemptyset(&sa.sa_mask);
		sa.sa_flags = 0;
		sigaction(SIGUSR2, &sa, NULL);

		/* and wait for a IRQ */
		while (!keapiWdTimeoutOccured) {
			usleep(100000);
		}

		break;

	case LINUX_LIKE:
	case KONTRON_LIKE_KEM:
	case KEAPI_FULL:
		return KEAPI_RET_FUNCTION_NOT_IMPLEMENTED;
	default:
		return KEAPI_RET_ERROR;
	}

	return KEAPI_RET_SUCCESS;
}

/* Signal handler for NMI or IRQ from watchdog */
void wd_signal_handler(int sig)
{
	if (sig == SIGUSR2) {
		printf("****************************************************\n"
		       "         WATCHDOG TIMEOUT OCCURED\n"
		       "           in NMI or IRQ mode\n"
		       "****************************************************\n");
	}

	keapiWdTimeoutOccured = 1;
}

/******************************************************************************/
static int getWdTimeIndex(int wdt_fd, int time)
{
	int index = 0;
	int found = 0;
	int timevals[MAX_WDT_TIME_VALS]; /* Pointer to a list of time values */

	/* Get info from watchdog. Demonstrate ioctl() functions. */
	if (ioctl(wdt_fd, WDIOC_TIMEVALUES, (unsigned long)timevals)) {
		return (-1);
	}

	for (index = 1; index <= timevals[0]; index++) {
		if (time == timevals[index] * 125) {
			found = 1;
			break;
		}
	}

	if (found != 1) {
		printf("#######################################################\n");
		printf("###########          W A R N I N G         ############\n");
		printf("#  ERROR: Check exact WdogTime WHICH CAN BE SET       #\n");
		printf("#######################################################\n");

		printf("\nSupported Timeout values:\n");
		printf("Index  Timeout [s]\n");
		for (index = 1; index <= timevals[0]; index++) {
			printf("%2d : %7d ms\n", index, timevals[index] * 125);
		}

		return -1;
	}

	index--;

	return (index);
}

/******************************************************************************/

int32_t readWdtSetup(int fd, PWDT_STAGE *setup)
{
	int32_t stage, timeout, mode, match_found, rc, stage_found = 0, i = 0;
	char buf[KEAPI_MAX_STR] = { 0 };

	do {
		rc = read(fd, &buf[i], 1);
		if (buf[i] == '\n' || buf[i] == '\0' || buf[i] == '\r') {
			match_found = sscanf(buf, "%d %d %d", &stage, &timeout, &mode);
			if (match_found == 3) {
				*setup = realloc(*setup, sizeof(WDT_STAGE) * (stage_found + 1));
				(*setup)[stage_found].stageNr = stage;
				(*setup)[stage_found].timeout = timeout;
				(*setup)[stage_found].mode = mode;

				stage_found++;
			}

			memset(buf, 0, KEAPI_MAX_STR);
			i = 0;
		} else
			i++;
	} while (rc > 0 && i < KEAPI_MAX_STR);

	return stage_found;
}

void writeWdtSetup(int fd, PWDT_STAGE setup, int32_t setup_count)
{
	int32_t i, rc;
	char buf[KEAPI_MAX_STR];

	rc = ftruncate(fd, 0);
	if (rc == -1)
		return;

	for (i = 0; i < setup_count; i++) {
		snprintf(buf, KEAPI_MAX_STR, "%d %d %d\n", setup[i].stageNr, setup[i].timeout, setup[i].mode);
		rc = write(fd, buf, strlen(buf));
		if (rc == -1)
			return;
	}
}

void resetWdtSetup(int fd)
{
	int32_t rc;

	rc = ftruncate(fd, 0);

	(void)rc;
}

int32_t updateWdtSetup(int32_t stage, int32_t timeout, int32_t mode, PWDT_STAGE *setup, int32_t stage_count)
{
	int32_t i;

	for (i = 0; i < stage_count; i++)
		if ((*setup)[i].stageNr == stage) {
			(*setup)[i].timeout = timeout;
			(*setup)[i].mode = mode;
			return stage_count;
		}

	*setup = realloc(*setup, sizeof(WDT_STAGE) * (stage_count + 1));

	(*setup)[stage_count].stageNr = stage;
	(*setup)[stage_count].timeout = timeout;
	(*setup)[stage_count].mode = mode;
	stage_count++;

	return stage_count;
}
