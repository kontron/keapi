/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron Europe GmbH */

/* KEAPI SMBus functions implementation. */

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "keapi_inc.h"
#include "globals.h"
#include "sm_bus.h"

/*******************************************************************************/
KEAPI_RETVAL KEApiL_GetSmbusCount(int32_t *pSmbusCount)
{
	int32_t ret;

	/* Check function parameters */
	if (pSmbusCount == NULL)
		return KEAPI_RET_PARAM_NULL;

	/* Initialization */
	*pSmbusCount = 0;

	if (smbusArr == NULL || smbusArrCount == 0) {
		if ((ret = GetInfoFromSerialBusConfFile(SMBUS_CONFTYPE)) != KEAPI_RET_SUCCESS) {
			if (ret == KEAPI_RET_PERMISSION_DENIED)
				return ret;

			return KEAPI_RET_SUCCESS;
		}
	}

	*pSmbusCount = smbusArrCount;
	return KEAPI_RET_SUCCESS;
}

/*******************************************************************************/
int32_t giveSmbusFilename(int32_t smbusNr, char *filename)
{
	if (smbusArr == NULL || smbusArrCount == 0) {
		if (GetInfoFromSerialBusConfFile(SMBUS_CONFTYPE) != KEAPI_RET_SUCCESS)
			return KEAPI_RET_ERROR;
	}

	if (smbusNr < 0 || smbusNr >= smbusArrCount)
		return KEAPI_RET_PARAM_ERROR;

	snprintf(filename, KEAPI_MAX_STR, "/dev/i2c-%d", smbusArr[smbusNr].realNr);
	return KEAPI_RET_SUCCESS;
}

/*******************************************************************************/
KEAPI_RETVAL KEApiL_SmbusQuickCommand(int32_t smbusNr, uint8_t smbusAddress, uint8_t operation)
{
	char filename[20];
	int file;
	int32_t devCnt, ret;
	unsigned long funcs;
	__s32 res;

	if ((ret = KEApiL_GetSmbusCount(&devCnt)) != KEAPI_RET_SUCCESS)
		return ret;

	if (smbusNr < 0 || smbusNr >= devCnt)
		return KEAPI_RET_PARAM_ERROR;

	/* Check function parameters */
	if (operation != 0 && operation != 1)
		return KEAPI_RET_PARAM_ERROR;

	ret = giveSmbusFilename(smbusNr, filename);
	if (ret != KEAPI_RET_SUCCESS)
		return ret;

	if (smbusAddress < 0x1 || smbusAddress > 0x7f)
		return KEAPI_RET_PARAM_ERROR;

	if ((ret = checkRAccess(filename)) != KEAPI_RET_SUCCESS)
		return ret;

	if ((file = open(filename, O_RDONLY)) < 0)
		return KEAPI_RET_ERROR;

	errno = 0;
	/* Check if i2c adapter supports this function */
	if (ioctl(file, I2C_FUNCS, &funcs) < 0)
		return failIoctlCheckErrno(&file);

	if (!(funcs & I2C_FUNC_SMBUS_QUICK)) {
		close(file);
		return KEAPI_RET_FUNCTION_NOT_SUPPORTED;
	}

	/* Set slave device address */
	if (ioctl(file, I2C_SLAVE, smbusAddress) < 0)
		return failIoctlCheckErrno(&file);

	res = i2c_smbus_write_quick(file, operation);

	if (res < 0)
		return failIoctlCheckErrno(&file);

	close(file);
	return KEAPI_RET_SUCCESS;
}

/*******************************************************************************/
KEAPI_RETVAL KEApiL_SmbusSendByte(int32_t smbusNr, uint8_t smbusAddress, uint8_t byte)
{
	char filename[20];
	int file;
	unsigned long funcs;
	__s32 res;
	int32_t devCnt, ret;

	if ((ret = KEApiL_GetSmbusCount(&devCnt)) != KEAPI_RET_SUCCESS)
		return ret;

	if (smbusNr < 0 || smbusNr >= devCnt)
		return KEAPI_RET_PARAM_ERROR;

	ret = giveSmbusFilename(smbusNr, filename);
	if (ret != KEAPI_RET_SUCCESS)
		return ret;

	if (smbusAddress < 0x1 || smbusAddress > 0x7f)
		return KEAPI_RET_PARAM_ERROR;

	if ((ret = checkRAccess(filename)) != KEAPI_RET_SUCCESS)
		return ret;

	if ((file = open(filename, O_RDONLY)) < 0)
		return KEAPI_RET_ERROR;

	errno = 0;
	/* Check if i2c adapter supports this function */
	if (ioctl(file, I2C_FUNCS, &funcs) < 0)
		return failIoctlCheckErrno(&file);

	if (!(funcs & I2C_FUNC_SMBUS_WRITE_BYTE)) {
		close(file);
		return KEAPI_RET_FUNCTION_NOT_SUPPORTED;
	}

	/* Set slave device address */
	if (ioctl(file, I2C_SLAVE, smbusAddress) < 0)
		return failIoctlCheckErrno(&file);

	res = i2c_smbus_write_byte(file, byte);

	if (res < 0)
		return failIoctlCheckErrno(&file);

	close(file);
	return KEAPI_RET_SUCCESS;
}

/*******************************************************************************/
KEAPI_RETVAL KEApiL_SmbusReceiveByte(int32_t smbusNr, uint8_t smbusAddress, uint8_t *pByte)
{
	char filename[20];
	int file;
	unsigned long funcs;
	__s32 res;
	int32_t devCnt, ret;

	if ((ret = KEApiL_GetSmbusCount(&devCnt)) != KEAPI_RET_SUCCESS)
		return ret;

	if (smbusNr < 0 || smbusNr >= devCnt)
		return KEAPI_RET_PARAM_ERROR;

	/* Check function parameters */
	if (pByte == NULL)
		return KEAPI_RET_PARAM_NULL;

	ret = giveSmbusFilename(smbusNr, filename);
	if (ret != KEAPI_RET_SUCCESS)
		return ret;

	if (smbusAddress < 0x1 || smbusAddress > 0x7f)
		return KEAPI_RET_PARAM_ERROR;

	/* Initialization */
	*pByte = 0;

	if ((ret = checkRAccess(filename)) != KEAPI_RET_SUCCESS)
		return ret;

	if ((file = open(filename, O_RDONLY)) < 0)
		return KEAPI_RET_ERROR;

	errno = 0;
	/* Check if i2c adapter supports this function */
	if (ioctl(file, I2C_FUNCS, &funcs) < 0)
		return failIoctlCheckErrno(&file);

	if (!(funcs & I2C_FUNC_SMBUS_READ_BYTE)) {
		close(file);
		return KEAPI_RET_FUNCTION_NOT_SUPPORTED;
	}

	/* Set slave device address */
	if (ioctl(file, I2C_SLAVE, smbusAddress) < 0)
		return failIoctlCheckErrno(&file);

	res = i2c_smbus_read_byte(file);

	if (res < 0)
		return failIoctlCheckErrno(&file);

	close(file);
	*pByte = res;
	return KEAPI_RET_SUCCESS;
}

/*******************************************************************************/
KEAPI_RETVAL KEApiL_SmbusWriteByte(int32_t smbusNr, uint8_t smbusAddress, uint8_t command, uint8_t byte)
{
	char filename[20];
	int file;
	unsigned long funcs;
	__s32 res;
	int32_t devCnt, ret;

	if ((ret = KEApiL_GetSmbusCount(&devCnt)) != KEAPI_RET_SUCCESS)
		return ret;

	if (smbusNr < 0 || smbusNr >= devCnt)
		return KEAPI_RET_PARAM_ERROR;

	ret = giveSmbusFilename(smbusNr, filename);
	if (ret != KEAPI_RET_SUCCESS)
		return ret;

	if (smbusAddress < 0x1 || smbusAddress > 0x7f)
		return KEAPI_RET_PARAM_ERROR;

	if ((ret = checkRAccess(filename)) != KEAPI_RET_SUCCESS)
		return ret;

	if ((file = open(filename, O_RDONLY)) < 0)
		return KEAPI_RET_ERROR;

	errno = 0;
	/* Check if i2c adapter supports this function */
	if (ioctl(file, I2C_FUNCS, &funcs) < 0)
		return failIoctlCheckErrno(&file);

	if (!(funcs & I2C_FUNC_SMBUS_WRITE_BYTE_DATA)) {
		close(file);
		return KEAPI_RET_FUNCTION_NOT_SUPPORTED;
	}

	/* Set slave device address */
	if (ioctl(file, I2C_SLAVE, smbusAddress) < 0)
		return failIoctlCheckErrno(&file);

	res = i2c_smbus_write_byte_data(file, command, byte);

	if (res < 0)
		return failIoctlCheckErrno(&file);

	close(file);
	return KEAPI_RET_SUCCESS;
}

/*******************************************************************************/
KEAPI_RETVAL KEApiL_SmbusReadByte(int32_t smbusNr, uint8_t smbusAddress, uint8_t command, uint8_t *pByte)
{
	char filename[20];
	int file;
	unsigned long funcs;
	__s32 res;
	int32_t devCnt, ret;

	if ((ret = KEApiL_GetSmbusCount(&devCnt)) != KEAPI_RET_SUCCESS)
		return ret;

	if (smbusNr < 0 || smbusNr >= devCnt)
		return KEAPI_RET_PARAM_ERROR;

	/* Check function parameters */
	if (pByte == NULL)
		return KEAPI_RET_PARAM_NULL;

	ret = giveSmbusFilename(smbusNr, filename);
	if (ret != KEAPI_RET_SUCCESS)
		return ret;

	if (smbusAddress < 0x1 || smbusAddress > 0x7f)
		return KEAPI_RET_PARAM_ERROR;

	/* Initialization */
	*pByte = 0;

	if ((ret = checkRAccess(filename)) != KEAPI_RET_SUCCESS)
		return ret;

	if ((file = open(filename, O_RDONLY)) < 0)
		return KEAPI_RET_ERROR;

	errno = 0;
	/* Check if i2c adapter supports this function */
	if (ioctl(file, I2C_FUNCS, &funcs) < 0)
		return failIoctlCheckErrno(&file);

	if (!(funcs & I2C_FUNC_SMBUS_READ_BYTE_DATA)) {
		close(file);
		return KEAPI_RET_FUNCTION_NOT_SUPPORTED;
	}

	/* Set slave device address */
	if (ioctl(file, I2C_SLAVE, smbusAddress) < 0)
		return failIoctlCheckErrno(&file);

	res = i2c_smbus_read_byte_data(file, command);

	if (res < 0)
		return failIoctlCheckErrno(&file);

	close(file);
	*pByte = res;
	return KEAPI_RET_SUCCESS;
}

/*******************************************************************************/
KEAPI_RETVAL KEApiL_SmbusWriteWord(int32_t smbusNr, uint8_t smbusAddress, uint8_t command, uint16_t word)
{
	char filename[20];
	int file;
	unsigned long funcs;
	__s32 res;
	int32_t devCnt, ret;

	if ((ret = KEApiL_GetSmbusCount(&devCnt)) != KEAPI_RET_SUCCESS)
		return ret;

	if (smbusNr < 0 || smbusNr >= devCnt)
		return KEAPI_RET_PARAM_ERROR;

	ret = giveSmbusFilename(smbusNr, filename);
	if (ret != KEAPI_RET_SUCCESS)
		return ret;

	if (smbusAddress < 0x1 || smbusAddress > 0x7f)
		return KEAPI_RET_PARAM_ERROR;

	if ((ret = checkRAccess(filename)) != KEAPI_RET_SUCCESS)
		return ret;

	if ((file = open(filename, O_RDONLY)) < 0)
		return KEAPI_RET_ERROR;

	errno = 0;
	/* Check if i2c adapter supports this function */
	if (ioctl(file, I2C_FUNCS, &funcs) < 0)
		return failIoctlCheckErrno(&file);

	if (!(funcs & I2C_FUNC_SMBUS_WRITE_WORD_DATA)) {
		close(file);
		return KEAPI_RET_FUNCTION_NOT_SUPPORTED;
	}

	/* Set slave device address */
	if (ioctl(file, I2C_SLAVE, smbusAddress) < 0)
		return failIoctlCheckErrno(&file);

	res = i2c_smbus_write_word_data(file, command, word);

	if (res < 0)
		return failIoctlCheckErrno(&file);

	close(file);
	return KEAPI_RET_SUCCESS;
}

/*******************************************************************************/
KEAPI_RETVAL KEApiL_SmbusReadWord(int32_t smbusNr, uint8_t smbusAddress, uint8_t command, uint16_t *pWord)
{
	char filename[20];
	int file;
	unsigned long funcs;
	__s32 res;
	int32_t devCnt, ret;

	if ((ret = KEApiL_GetSmbusCount(&devCnt)) != KEAPI_RET_SUCCESS)
		return ret;

	if (smbusNr < 0 || smbusNr >= devCnt)
		return KEAPI_RET_PARAM_ERROR;

	/* Check function parameters */
	if (pWord == NULL)
		return KEAPI_RET_PARAM_NULL;

	ret = giveSmbusFilename(smbusNr, filename);
	if (ret != KEAPI_RET_SUCCESS)
		return ret;

	if (smbusAddress < 0x1 || smbusAddress > 0x7f)
		return KEAPI_RET_PARAM_ERROR;

	/* Initialization */
	*pWord = 0;

	if ((ret = checkRAccess(filename)) != KEAPI_RET_SUCCESS)
		return ret;

	if ((file = open(filename, O_RDONLY)) < 0)
		return KEAPI_RET_ERROR;

	errno = 0;
	/* Check if i2c adapter supports this function */
	if (ioctl(file, I2C_FUNCS, &funcs) < 0)
		return failIoctlCheckErrno(&file);

	if (!(funcs & I2C_FUNC_SMBUS_READ_WORD_DATA)) {
		close(file);
		return KEAPI_RET_FUNCTION_NOT_SUPPORTED;
	}

	/* Set slave device address */
	if (ioctl(file, I2C_SLAVE, smbusAddress) < 0)
		return failIoctlCheckErrno(&file);

	res = i2c_smbus_read_word_data(file, command);

	if (res < 0)
		return failIoctlCheckErrno(&file);

	close(file);
	*pWord = res;
	return KEAPI_RET_SUCCESS;
}

/*******************************************************************************/
KEAPI_RETVAL KEApiL_SmbusWriteBlock(int32_t smbusNr, uint8_t smbusAddress, uint8_t command, uint8_t *pData,
				    int8_t dataLength)
{
	char filename[20];
	int file;
	unsigned long funcs;
	__s32 res;
	int32_t devCnt, ret;

	if ((ret = KEApiL_GetSmbusCount(&devCnt)) != KEAPI_RET_SUCCESS)
		return ret;

	if (smbusNr < 0 || smbusNr >= devCnt)
		return KEAPI_RET_PARAM_ERROR;

	/* Check function parameters */
	if (pData == NULL)
		return KEAPI_RET_PARAM_NULL;

	if (dataLength == 0)
		return KEAPI_RET_PARAM_ERROR;

	ret = giveSmbusFilename(smbusNr, filename);
	if (ret != KEAPI_RET_SUCCESS)
		return ret;

	if (smbusAddress < 0x1 || smbusAddress > 0x7f)
		return KEAPI_RET_PARAM_ERROR;

	if ((ret = checkRAccess(filename)) != KEAPI_RET_SUCCESS)
		return ret;

	if ((file = open(filename, O_RDONLY)) < 0)
		return KEAPI_RET_ERROR;

	errno = 0;
	/* Check if i2c adapter supports this function */
	if (ioctl(file, I2C_FUNCS, &funcs) < 0)
		return failIoctlCheckErrno(&file);

	if (!(funcs & I2C_FUNC_SMBUS_WRITE_BLOCK_DATA)) {
		close(file);
		return KEAPI_RET_FUNCTION_NOT_SUPPORTED;
	}

	/* Set slave device address */
	if (ioctl(file, I2C_SLAVE, smbusAddress) < 0)
		return failIoctlCheckErrno(&file);

	res = i2c_smbus_write_block_data(file, command, dataLength, pData);

	if (res < 0)
		return failIoctlCheckErrno(&file);

	close(file);
	return KEAPI_RET_SUCCESS;
}

/*******************************************************************************/
KEAPI_RETVAL KEApiL_SmbusReadBlock(int32_t smbusNr, uint8_t smbusAddress, uint8_t command, uint8_t *pData,
				   int8_t *pDataLength)
{
	char filename[20];
	int file;
	unsigned long funcs;
	__s32 res;
	int32_t devCnt, ret;

	if ((ret = KEApiL_GetSmbusCount(&devCnt)) != KEAPI_RET_SUCCESS)
		return ret;

	if (smbusNr < 0 || smbusNr >= devCnt)
		return KEAPI_RET_PARAM_ERROR;

	/* Check function parameters */
	if (pData == NULL || pDataLength == NULL)
		return KEAPI_RET_PARAM_NULL;

	ret = giveSmbusFilename(smbusNr, filename);
	if (ret != KEAPI_RET_SUCCESS)
		return ret;

	if (smbusAddress < 0x1 || smbusAddress > 0x7f)
		return KEAPI_RET_PARAM_ERROR;

	/* Initialization */
	memset(pData, 0, *pDataLength);

	if ((ret = checkRAccess(filename)) != KEAPI_RET_SUCCESS)
		return ret;

	if ((file = open(filename, O_RDONLY)) < 0)
		return KEAPI_RET_ERROR;

	errno = 0;
	/* Check if i2c adapter supports this function */
	if (ioctl(file, I2C_FUNCS, &funcs) < 0)
		return failIoctlCheckErrno(&file);

	if (!(funcs & I2C_FUNC_SMBUS_READ_BLOCK_DATA)) {
		close(file);
		return KEAPI_RET_FUNCTION_NOT_SUPPORTED;
	}

	/* Set slave device address */
	if (ioctl(file, I2C_SLAVE, smbusAddress) < 0)
		return failIoctlCheckErrno(&file);

	res = i2c_smbus_read_block_data(file, command, pData);

	if (res < 0)
		return failIoctlCheckErrno(&file);

	close(file);
	*pDataLength = res;
	return KEAPI_RET_SUCCESS;
}
