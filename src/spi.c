/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron AG */

/* KEAPI SPI implementation. */

#include <regex.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include "keapi_inc.h"
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>

KEAPI_RETVAL KEApiL_GetSpiBusCount(int32_t *pSpiBusCount)
{
	DIR *dir;
	struct dirent *ent;
	int8_t found;
	char conf_paths[KEAPI_MAX_STR][PATH_MAX];
	int32_t conf_spi_count, ret;

	/* Check function parameters */
	if (pSpiBusCount == NULL)
		return KEAPI_RET_PARAM_NULL;

	*pSpiBusCount = 0;

	/* If we found configuration file with remapping information
	 * trying to read it */
	if ((ret = checkRAccess(SPI_CONF_PATH)) == KEAPI_RET_SUCCESS) {
		if ((ret = findRemapping(SPI_CONF_PATH, conf_paths, &conf_spi_count)) == KEAPI_RET_SUCCESS) {
			*pSpiBusCount = conf_spi_count;
			return KEAPI_RET_SUCCESS;
		} else
			return ret;

	} else if (ret != KEAPI_RET_RETRIEVAL_ERROR)
		return ret;

	/* Count spi busses in "/sys/class/spi_master/" */
	if ((ret = checkRAccess("/sys/class/spi_master/")) != KEAPI_RET_SUCCESS) {
		if (ret == KEAPI_RET_RETRIEVAL_ERROR)
			return KEAPI_RET_SUCCESS;
		else
			return ret;
	}

	if ((dir = opendir("/sys/class/spi_master/")) == NULL)
		return KEAPI_RET_ERROR;

	while ((ent = readdir(dir)) != NULL) {
		if (FindRegex(ent->d_name, "spi[0-9]", &found, REG_EXTENDED | REG_NEWLINE) != KEAPI_RET_SUCCESS) {
			closedir(dir);
			return KEAPI_RET_ERROR;
		}

		if (found)
			(*pSpiBusCount)++;
	}

	closedir(dir);
	return KEAPI_RET_SUCCESS;
}

KEAPI_RETVAL KEApiL_SpiXfer(int32_t spiNr, uint16_t deviceId, uint32_t command, uint8_t commandSize,
			    int32_t numTransfers, uint8_t *pWriteData, uint8_t *pReadData)
{
	char path[PATH_MAX];
	struct spi_ioc_transfer xfer[2];
	int fd, status, tCount = 0;
	char conf_paths[KEAPI_MAX_STR][PATH_MAX];
	int32_t conf_spi_count, ret;
	uint8_t cmd8;
	uint16_t cmd16;

	if ((commandSize > 4) || (commandSize == 0 && numTransfers == 0) ||
	    (numTransfers != 0 && pWriteData == NULL && pReadData == NULL))
		return KEAPI_RET_PARAM_ERROR;

	/* If we found configuration file with remapping information
	 * trying to read it and use these paths
	 * if there is not, use default methods */
	if ((ret = checkRAccess(SPI_CONF_PATH)) == KEAPI_RET_SUCCESS) {
		if ((ret = findRemapping(SPI_CONF_PATH, conf_paths, &conf_spi_count)) == KEAPI_RET_SUCCESS) {
			/* Check that number that we got is correct */
			if (spiNr >= conf_spi_count)
				return KEAPI_RET_PARAM_ERROR;

			snprintf(path, PATH_MAX, "%s.%d", conf_paths[spiNr], deviceId);
		} else
			return ret;

	} else if (ret == KEAPI_RET_RETRIEVAL_ERROR)
		snprintf(path, PATH_MAX, "/dev/spidev%d.%d", spiNr, deviceId);
	else
		return ret;

	if ((ret = checkRAccess(path)) != KEAPI_RET_SUCCESS)
		return ret;

	fd = open(path, O_RDWR);
	if (fd < 0)
		return KEAPI_RET_ERROR;

	memset(xfer, 0, sizeof xfer);

	switch (commandSize) {
	case 0:
		break;
	case 1:
		cmd8 = (uint8_t)command;
		xfer[tCount].tx_buf = (unsigned long)&cmd8;
		xfer[tCount].bits_per_word = 8;
		xfer[tCount].len = 1;
		tCount++;
		break;
	case 2:
		cmd16 = (uint16_t)command;
		xfer[tCount].tx_buf = (unsigned long)&cmd16;
		xfer[tCount].bits_per_word = 16;
		xfer[tCount].len = 1;
		tCount++;
		break;
	case 3:
	case 4:
		xfer[tCount].tx_buf = (unsigned long)&command;
		xfer[tCount].bits_per_word = 32;
		xfer[tCount].len = 1;
		tCount++;
	}

	if (numTransfers > 0) {
		xfer[tCount].tx_buf = (unsigned long)pWriteData;
		xfer[tCount].rx_buf = (unsigned long)pReadData;
		xfer[tCount].bits_per_word = 8;
		xfer[tCount].len = numTransfers;
		tCount++;
	}

	errno = 0;
	status = ioctl(fd, SPI_IOC_MESSAGE(tCount), xfer);

	if (status < 0)
		return failIoctlCheckErrno(&fd);

	close(fd);
	return KEAPI_RET_SUCCESS;
}
