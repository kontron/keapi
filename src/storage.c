/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron Europe GmbH */

/* KEAPI EEPROM user storage function implementation. */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "keapi_inc.h"
#include "storage_picmg.h"
#include "globals.h"
#include "eapi/EeeP.h"

/*******************************************************************************/
KEAPI_RETVAL KEApiL_GetStorageCount(int32_t *pStorageCount)
{
	int32_t i, ret;

	/* Check function parameters */
	if (pStorageCount == NULL)
		return KEAPI_RET_PARAM_NULL;

	/* Initialization */
	*pStorageCount = 0;

	if (siArr == NULL || siArrCount == 0) {
		ret = GetStorageConfig();
		if (ret != KEAPI_RET_SUCCESS && ret != KEAPI_RET_PARTIAL_SUCCESS) {
			if (ret == KEAPI_RET_PERMISSION_DENIED)
				return ret;

			return KEAPI_RET_SUCCESS;
		}
	}

	for (i = 0; i < siArrCount; i++) {
		if (siArr[i].usr_flag)
			(*pStorageCount)++;
	}

	return KEAPI_RET_SUCCESS;
}

/*******************************************************************************/
KEAPI_RETVAL KEApiL_GetStorageSize(int32_t storageNr, int32_t *pStorageSize)
{
	int i, j;
	int32_t devCnt, ret;

	/* Check function parameters */
	if ((ret = KEApiL_GetStorageCount(&devCnt)) != KEAPI_RET_SUCCESS)
		return ret;

	if (storageNr < 0 || storageNr >= devCnt)
		return KEAPI_RET_PARAM_ERROR;

	if (pStorageSize == NULL)
		return KEAPI_RET_PARAM_NULL;

	/* Initialization */
	*pStorageSize = 0;

	j = 0;
	for (i = 0; i < siArrCount; i++)
		if (siArr[i].usr_flag) {
			if (j == storageNr) {
				(*pStorageSize) = siArr[i].eepUsr->size;
				return KEAPI_RET_SUCCESS;
			}
			j++;
		}

	return KEAPI_RET_ERROR;
}

/*******************************************************************************/
KEAPI_RETVAL KEApiL_StorageRead(int32_t storageNr, int32_t offset, uint8_t *pData, int32_t dataLength)
{
	char loc[PATH_MAX];
	int32_t fd, devCnt, ret;
	int i, j;

	/* Check function parameters */
	if ((ret = KEApiL_GetStorageCount(&devCnt)) != KEAPI_RET_SUCCESS)
		return ret;

	if (storageNr < 0 || storageNr >= devCnt)
		return KEAPI_RET_PARAM_ERROR;

	if (pData == NULL)
		return KEAPI_RET_PARAM_NULL;

	if (dataLength < 1 || offset < 0)
		return KEAPI_RET_PARAM_ERROR;

	j = 0;
	loc[0] = '\0';
	for (i = 0; i < siArrCount; i++)
		if (siArr[i].usr_flag) {
			if (j == storageNr) {
				snprintf(loc, PATH_MAX - 1, "%s", siArr[i].path);
				break;
			}
			j++;
		}
	if (loc[0] == '\0')
		return KEAPI_RET_PARAM_ERROR;

	/* Check if offset + dataLength <= StorageSize */
	if (offset + dataLength > siArr[i].eepUsr->size)
		return KEAPI_RET_PARAM_ERROR;

	/* Initialization */
	memset(pData, 0, dataLength);

	if ((ret = checkRAccess(loc)) != KEAPI_RET_SUCCESS)
		return ret;

	/* Read from storage */
	if ((fd = open(loc, O_RDONLY)) < 0)
		return KEAPI_RET_READ_ERROR;

	if (lseek(fd, siArr[i].eepUsr->start + offset, SEEK_SET) < 0) {
		close(fd);
		return KEAPI_RET_READ_ERROR;
	}

	{
		int offset = 0;
		int nBytes = 0;
		for (;;) {
			nBytes = read(fd, (void *)&pData[offset], dataLength);
			if (nBytes == dataLength)
				break;
			else {
				if (nBytes == -1) {
					close(fd);
					return KEAPI_RET_READ_ERROR;
				}
				dataLength -= nBytes;
				offset += nBytes;
			}
		}
	}

	close(fd);
	return KEAPI_RET_SUCCESS;
}

/*******************************************************************************/
KEAPI_RETVAL KEApiL_StorageWrite(int32_t storageNr, int32_t offset, uint8_t *pData, int32_t dataLength)
{
	char loc[PATH_MAX];
	int32_t fd, devCnt, ret;
	int i, j;

	/* Check function parameters */
	if ((ret = KEApiL_GetStorageCount(&devCnt)) != KEAPI_RET_SUCCESS)
		return ret;

	if (storageNr < 0 || storageNr >= devCnt)
		return KEAPI_RET_PARAM_ERROR;

	if (pData == NULL)
		return KEAPI_RET_PARAM_NULL;

	if (dataLength < 1 || offset < 0)
		return KEAPI_RET_PARAM_ERROR;

	j = 0;
	loc[0] = '\0';
	for (i = 0; i < siArrCount; i++)
		if (siArr[i].usr_flag) {
			if (j == storageNr) {
				snprintf(loc, PATH_MAX - 1, "%s", siArr[i].path);
				break;
			}
			j++;
		}
	if (loc[0] == '\0')
		return KEAPI_RET_PARAM_ERROR;

	/* Check if offset + dataLength <= StorageSize */
	if (offset + dataLength > siArr[i].eepUsr->size)
		return KEAPI_RET_PARAM_ERROR;

	if ((ret = checkRWAccess(loc)) != KEAPI_RET_SUCCESS)
		return ret;

	/* Write to storage */
	if ((fd = open(loc, O_WRONLY)) < 0)
		return KEAPI_RET_WRITE_ERROR;

	if (lseek(fd, siArr[i].eepUsr->start + offset, SEEK_SET) < 0) {
		close(fd);
		return KEAPI_RET_WRITE_ERROR;
	}

	{
		int offset = 0;
		int nBytes = 0;

		for (;;) {
			nBytes = write(fd, (void *)&pData[offset], dataLength);
			if (nBytes == dataLength)
				break;
			else {
				if (nBytes == -1) {
					close(fd);
					return KEAPI_RET_WRITE_ERROR;
				}
				dataLength -= nBytes;
				offset += nBytes;
			}
		}
	}

	close(fd);
	return KEAPI_RET_SUCCESS;
}
