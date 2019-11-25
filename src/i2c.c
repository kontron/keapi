/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron AG */

/* KEAPI I2C function implementation. */

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <errno.h>

#include "keapi_inc.h"
#include "globals.h"

/*******************************************************************************/
KEAPI_RETVAL KEApiL_GetI2cBusCount(int32_t *pI2cBusCount)
{
	/* Check function parameters */
	if (pI2cBusCount == NULL)
		return KEAPI_RET_PARAM_NULL;

	/* Initialization */
	*pI2cBusCount = 0;

	if (i2cArr == NULL || i2cArrCount == 0) {
		if (GetInfoFromSerialBusConfFile(I2C_CONFTYPE) != KEAPI_RET_SUCCESS)
			return KEAPI_RET_SUCCESS;
	}

	*pI2cBusCount = i2cArrCount;
	return KEAPI_RET_SUCCESS;
}

/*******************************************************************************/
int32_t giveI2cFilename(int32_t i2cNr, char *filename)
{
	int32_t ret;

	if (i2cArr == NULL || i2cArrCount == 0) {
		ret = GetInfoFromSerialBusConfFile(I2C_CONFTYPE);

		if (ret != KEAPI_RET_SUCCESS)
			return ret;
	}

	if (i2cNr < 0 || i2cNr >= i2cArrCount)
		return KEAPI_RET_PARAM_ERROR;

	snprintf(filename, KEAPI_MAX_STR, "/dev/i2c-%d", i2cArr[i2cNr].realNr);
	return KEAPI_RET_SUCCESS;
}

/*******************************************************************************/
KEAPI_RETVAL KEApiL_I2cXfer(int32_t i2cNr, uint8_t i2cAddress, uint8_t *pWriteData, int32_t writeLength,
			    uint8_t *pReadData, int32_t *pReadLength)
{
	struct i2c_msg msgs[2];
	struct i2c_rdwr_ioctl_data msgset;
	char filename[20];
	int file, msg_count = 0;
	unsigned long funcs = 0;
	int32_t devCnt, ret;

	/* Check function parameters */
	if ((ret = KEApiL_GetI2cBusCount(&devCnt)) != KEAPI_RET_SUCCESS)
		return ret;

	if (i2cNr < 0 || i2cNr >= devCnt)
		return KEAPI_RET_PARAM_ERROR;

	if (!pReadLength || ((*pReadLength > 0) && !pReadData) || ((writeLength > 0) && !pWriteData))
		return KEAPI_RET_PARAM_NULL;

	if ((*pReadLength < 0) || (writeLength < 0))
		return KEAPI_RET_PARAM_ERROR;

	if (i2cAddress < 0x1 || i2cAddress > 0x7f)
		return KEAPI_RET_PARAM_ERROR;

	/* i2cNr is validated into giveI2cFilename */
	ret = giveI2cFilename(i2cNr, filename);
	if (ret != KEAPI_RET_SUCCESS)
		return ret;

	if ((ret = checkRWAccess(filename)) != KEAPI_RET_SUCCESS)
		return ret;

	if ((file = open(filename, O_RDWR)) < 0)
		return KEAPI_RET_ERROR;

	errno = 0;
	if (ioctl(file, I2C_SLAVE, i2cAddress) < 0) {
		if (errno == EBUSY) {
			close(file);
			return KEAPI_RET_BUSY_COLLISION;
		}

		return failIoctlCheckErrno(&file);
	}

	if (ioctl(file, I2C_FUNCS, &funcs) < 0)
		return failIoctlCheckErrno(&file);

	if (!(funcs & I2C_FUNC_I2C)) {
		close(file);
		return KEAPI_RET_PARAM_ERROR;
	}

	memset(msgs, 0, sizeof msgs);

	/* Write Message */
	if (writeLength > 0) {
		msgs[msg_count].addr = i2cAddress;
		msgs[msg_count].flags = 0;
		msgs[msg_count].buf = (uint8_t *)pWriteData;
		msgs[msg_count].len = writeLength;
		msg_count++;
	}

	/* Read Message */
	if (*pReadLength > 0) {
		msgs[msg_count].addr = i2cAddress;
		msgs[msg_count].flags = I2C_M_RD;
		msgs[msg_count].buf = pReadData;
		msgs[msg_count].len = *pReadLength;
		msg_count++;
	}

	/* For Probe functionality */
	if (writeLength == 0 && *pReadLength == 0) {
		msgs[0].addr = i2cAddress;
		msgs[0].flags = 0;
		msgs[0].buf = NULL;
		msgs[0].len = 0;
		msg_count = 1;
	}

	msgset.msgs = msgs;
	msgset.nmsgs = msg_count;

	if (ioctl(file, I2C_RDWR, &msgset) < 0)
		return failIoctlCheckErrno(&file);

	close(file);

	return KEAPI_RET_SUCCESS;
}

/*******************************************************************************/
KEAPI_RETVAL KEApiL_I2cProbe(int32_t i2cNr, uint8_t i2cAddress, uint8_t memoryAddress, uint8_t memoryAddressUsed)
{
	char filename[20];
	int file;
	unsigned long funcs = 0;
	struct i2c_smbus_ioctl_data args;
	int32_t devCnt, ret;

	/* Check function parameters */
	if ((ret = KEApiL_GetI2cBusCount(&devCnt)) != KEAPI_RET_SUCCESS)
		return ret;

	if (i2cNr < 0 || i2cNr >= devCnt)
		return KEAPI_RET_PARAM_ERROR;

	/* Plain I2c byte write - S Addr Wr [A] Data [A] P */
	if (memoryAddressUsed == TRUE) {
		int32_t readLength = 0;

		return KEApiL_I2cXfer(i2cNr, i2cAddress, &memoryAddress, 1, NULL, &readLength);
	}

	/* else if (memoryAddressUsed == FALSE)
	 * I2C_FUNC_SMBUS_QUICK - S Addr Wr [A] P */

	/* Check function parameters */
	if (i2cAddress < 0x1 || i2cAddress > 0x7f)
		return KEAPI_RET_PARAM_ERROR;

	/* i2cNr is validated into giveI2cFilename */
	ret = giveI2cFilename(i2cNr, filename);
	if (ret != KEAPI_RET_SUCCESS)
		return ret;

	if ((ret = checkRWAccess(filename)) != KEAPI_RET_SUCCESS)
		return ret;

	if ((file = open(filename, O_RDWR)) < 0)
		return KEAPI_RET_ERROR;

	errno = 0;
	if (ioctl(file, I2C_FUNCS, &funcs) < 0)
		return failIoctlCheckErrno(&file);

	if (!(funcs & I2C_FUNC_SMBUS_QUICK)) {
		close(file);
		return KEAPI_RET_ERROR;
	}

	if (ioctl(file, I2C_SLAVE, i2cAddress) < 0) {
		if (errno == EBUSY) {
			close(file);
			return KEAPI_RET_BUSY_COLLISION;
		}

		return failIoctlCheckErrno(&file);
	}

	args.read_write = I2C_SMBUS_WRITE;
	args.command = 0;
	args.size = I2C_SMBUS_QUICK;
	args.data = NULL;

	if ((ret = ioctl(file, I2C_SMBUS, &args)) < 0)
		return failIoctlCheckErrno(&file);

	close(file);
	return KEAPI_RET_SUCCESS;
}
