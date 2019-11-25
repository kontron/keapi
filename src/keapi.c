/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron AG */

/* KEAPI general implementation. */

#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <fcntl.h>

#include "keapi_inc.h"
#include "storage_picmg.h"
#include "globals.h"
#include "eapi/EeeP.h"

#define KREADSZ (1024)
#define BTCNT_MARKER "bootcount="
#define PROC_CMDLINE "/proc/cmdline"

/******************************************************************************/
KEAPI_RETVAL KEApiL_GetLibVersion(PKEAPI_VERSION_DATA pLibVersion)
{
	if (NULL == pLibVersion) {
		return KEAPI_RET_PARAM_NULL;
	}

	snprintf(pLibVersion->versionText, KEAPI_MAX_STR, "KEAPI v. %d.%d.%d.%d", KEAPI_SPEC_VERSION_MAJOR,
		 KEAPI_SPEC_VERSION_MINOR, KEAPI_LINUX_VERSION_MAJOR, KEAPI_LINUX_VERSION_MINOR);

	pLibVersion->version = (KEAPI_SPEC_VERSION_MAJOR << 24) | (KEAPI_SPEC_VERSION_MINOR << 16) |
			       (KEAPI_LINUX_VERSION_MAJOR << 8) | KEAPI_LINUX_VERSION_MINOR;

	return KEAPI_RET_SUCCESS;
}

/*******************************************************************************/
KEAPI_RETVAL KEApiL_SystemUpTime(int32_t *pSystemUpTime)
{
	FILE *fr;
	float temp;
	int32_t ret;

	/* Check function parameters */
	if (pSystemUpTime == NULL)
		return KEAPI_RET_PARAM_NULL;

	/* Initialization */
	*pSystemUpTime = 0;

	/* Read uptime from /proc/uptime */
	if ((ret = checkRAccess(UPTIME_PATH)) != KEAPI_RET_SUCCESS)
		return ret;

	if ((fr = fopen(UPTIME_PATH, "r")) == NULL)
		return KEAPI_RET_ERROR;

	if (fscanf(fr, "%f", &temp) == EOF) {
		fclose(fr);
		return KEAPI_RET_ERROR;
	}

	fclose(fr);
	*pSystemUpTime = temp;

	return KEAPI_RET_SUCCESS;
}

static const char *kreadfile(const char *const name)
{
	int fd;
	char *buf;
	size_t buflen;
	ssize_t readsz;

	if ((fd = open(name, O_RDONLY)) < 0)
		return NULL;

	buf = NULL;
	buflen = 0;
	do {
		buf = realloc(buf, buflen + KREADSZ);
		if (buf == NULL) {
			goto fail;
		}
		if ((readsz = read(fd, buf + buflen, KREADSZ)) < 0) {
			goto fail;
		}
		buflen += readsz;
	} while (readsz);

	buf[buflen] = '\0';

	close(fd);
	return buf;

fail:
	close(fd);
	free(buf);
	return NULL;
}

/* returns -1 if not success */
static int cmdlineBootcount()
{
	const char *cmdline;
	int cnt = -1;

	cmdline = kreadfile(PROC_CMDLINE);
	if (cmdline != NULL) {
		const char *bootcnt;

		bootcnt = strstr(cmdline, BTCNT_MARKER);
		if (bootcnt) {
			bootcnt += strlen(BTCNT_MARKER);
			if (sscanf(bootcnt, "%d", &cnt) != 1) {
				cnt = -1;
			}
		}

		free((void *)cmdline);
	}

	return cnt;
}

/*******************************************************************************/
KEAPI_RETVAL KEApiL_GetBootCounter(int32_t *pBootCount)
{
	void *parsedSmbiosHeader = NULL;
	struct BlockHead *blockHeads;
	int fd, i, j;
	int32_t temp, ret, blockCount;
	char tempstr[255], *data;
	unsigned char parsedSmbiosType = 0;

	/* Check function parameters */
	if (pBootCount == NULL)
		return KEAPI_RET_PARAM_NULL;

	/* first try kernel cmdline */
	temp = cmdlineBootcount();
	if (temp >= 0) {
		*pBootCount = temp;
		return KEAPI_RET_SUCCESS;
	}

	/* Check if bootcount file exists */
	if ((ret = checkRAccess(BOOT_COUNT_PATH)) == KEAPI_RET_SUCCESS) {
		/* Read number of boot cycles from /opt/eapi/bootcount0 */
		if ((fd = open(BOOT_COUNT_PATH, O_RDONLY)) < 0)
			return KEAPI_RET_ERROR;

		if (read(fd, &tempstr, sizeof(tempstr)) <= 0) {
			close(fd);
			return KEAPI_RET_ERROR;
		}

		close(fd);
		*pBootCount = atoi(tempstr);

		return KEAPI_RET_SUCCESS;
	} else if (ret != KEAPI_RET_RETRIEVAL_ERROR) /* file exist but can't read */
		return ret;

	/* if file not exist check if there is picmg eeprom */
	/* if storage configuration file has not been read, read it*/
	if (siArr == NULL || siArrCount == 0) {
		ret = GetStorageConfig();
		if (ret != KEAPI_RET_SUCCESS && ret != KEAPI_RET_PARTIAL_SUCCESS)
			return ret;
	}

	/* looking for picmg label across all storages info */
	for (i = 0; i < siArrCount; i++) {
		/* if we found picmg storage */
		if (!siArr[i].picmg_flag)
			continue;

		if ((ret = checkRAccess(siArr[i].path)) != KEAPI_RET_SUCCESS) {
			if (ret == KEAPI_RET_RETRIEVAL_ERROR) //no file
				continue;
			else
				return ret; // file present, but smth failed
		}

		/* we read it */
		if ((ret = ReadFile(siArr[i].path, &data)) != KEAPI_RET_SUCCESS)
			return ret;

		/* and pull out dynamic blocks */
		blockCount = GetDynamicBlockCount(data);
		blockHeads = calloc(blockCount, sizeof(struct BlockHead));

		ret = GetDynamicBlockData(data, blockHeads, blockCount);
		if (ret != blockCount) {
			free(blockHeads);
			free(data);
			continue;
		}

		/* then we go through all dynamic blocks and looking for SMBIOS headers */
		for (j = 0; j < blockCount; j++) {
			if (blockHeads[j].id != EEEP_BLOCK_ID_SMBIOS)
				continue;

			parsedSmbiosHeader =
				TryToParsePicmgSmbiosHeader(&data[blockHeads[j].offset], &parsedSmbiosType);

			/* if we found it, we look for table 161 of SMBIOS and take info */
			if (parsedSmbiosHeader && parsedSmbiosType == DMI_T_KEM_RT_DAT) {
				struct KemRTDat *kemRTDat = parsedSmbiosHeader;

				*pBootCount = kemRTDat->bootCounter;
				free(parsedSmbiosHeader);
				free(blockHeads);
				free(data);
				return KEAPI_RET_SUCCESS;
			}

			free(parsedSmbiosHeader);
		}

		free(blockHeads);
		free(data);
	}

	return KEAPI_RET_FUNCTION_NOT_SUPPORTED;
}
