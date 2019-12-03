/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron Europe GmbH */

/* Jida functions wrapper. */

#include "keapi_inc.h"

#if PLATFORM(VxWorks)
#include "versionK.h"
KONTRON_VERSION(JidaWrapper_c, 01d) /* version identifier */
#include <strings.h>
#endif /* VxWorks */

#if PLATFORM(Windows)
#include <windows.h>
#include <tchar.h>
#define strcasecmp _tcsicmp
#else
#include <string.h>
#include "jida/JWinDefs.h"
#endif

#include <stdio.h>
#include <time.h>
#include "jida/Jida.h"

static int32_t jidaDllInitCnt = 0;
static BOOL jidaBoardIsOpen = FALSE;

BOOL WINAPI JidaDllInitialize(void)
{
	if (jidaDllInitCnt == INT32_MAX || KEApiLibInitialize() != KEAPI_RET_SUCCESS)
		return FALSE;

	jidaDllInitCnt++;

	return TRUE;
}

BOOL WINAPI JidaDllUninitialize(void)
{
	if (jidaDllInitCnt <= 0 || KEApiLibUnInitialize() != KEAPI_RET_SUCCESS)
		return FALSE;

	jidaDllInitCnt--;

	return TRUE;
}

BOOL WINAPI JidaDllInstall(BOOL install)
{
	return TRUE;
}

DWORD WINAPI JidaDllGetVersion(void)
{
	return JidaDllVersion;
}

BOOL WINAPI JidaDllIsAvailable(void)
{
	if (!jidaDllInitCnt)
		return FALSE;

	return TRUE;
}

DWORD WINAPI JidaBoardCountA(LPCSTR pszClass, DWORD dwFlags)
{
	return 1; // KEAPI always works with the only 1 board
}

BOOL WINAPI JidaBoardOpenA(LPCSTR pszClass, DWORD dwNum, DWORD dwFlags, PHJIDA phJida)
{
	if (dwNum != 0)
		return FALSE;

	jidaBoardIsOpen = TRUE;

	return TRUE;
}

BOOL WINAPI JidaBoardOpenByNameA(LPCSTR pszName, PHJIDA phJida)
{
	CHAR buf[JIDA_BOARD_MAX_SIZE_ID_STRING];
	DWORD buf_len = JIDA_BOARD_MAX_SIZE_ID_STRING;
	HJIDA hJida = 0;

	if (!JidaBoardGetNameA(hJida, buf, buf_len))
		return FALSE;

	if (strcasecmp(pszName, buf) != 0)
		return FALSE;

	jidaBoardIsOpen = TRUE;

	return TRUE;
}

BOOL WINAPI JidaBoardClose(HJIDA hJida)
{
	if (!jidaBoardIsOpen)
		return FALSE;

	jidaBoardIsOpen = FALSE;

	return TRUE;
}

BOOL WINAPI JidaBoardGetNameA(HJIDA hJida, LPTSTR pszName, DWORD dwSize)
{
	KEAPI_BOARD_INFO bInfo;

	if (KEApiGetBoardInfo(&bInfo) != KEAPI_RET_SUCCESS)
		return FALSE;

	strncpy(pszName, bInfo.boardName, (size_t)dwSize);

	return TRUE;
}

BOOL WINAPI JidaBoardGetInfoA(HJIDA hJida, PJIDABOARDINFO pBoardInfo)
{
	KEAPI_BOARD_INFO bInfo;
	KEAPI_VERSION_DATA kvd;
	int32_t i = 0, ret, revBuf[4];
	struct tm *ts;

	if (pBoardInfo == NULL)
		return FALSE;

	if (KEApiGetBoardInfo(&bInfo) != KEAPI_RET_SUCCESS)
		return FALSE;

	pBoardInfo->dwFlags = 0;
	pBoardInfo->szBoard[0] = '\0';
	pBoardInfo->szBoardSub[0] = '\0';
	pBoardInfo->szPrimaryClass[0] = '\0';
	pBoardInfo->szClasses[0] = '\0';
	pBoardInfo->szManufacturer[0] = '\0';
	pBoardInfo->szSerialNumber[0] = '\0';

	/* DMI type 2 str at 0x05 offset
	 * or DMI 160 (if it was found) str at 0x09 */
	strncat(pBoardInfo->szBoard, bInfo.boardName, JIDA_BOARD_MAX_SIZE_ID_STRING - 1);

	strncat(pBoardInfo->szBoardSub, pBoardInfo->szBoard, JIDA_BOARD_MAX_SIZE_ID_STRING - 1);

	/* https://kdp.kontron.com/issues/8521 */
	strncat(pBoardInfo->szPrimaryClass, JIDA_BOARD_CLASS_CPU, JIDA_BOARD_MAX_SIZE_ID_STRING - 1);

	strncat(pBoardInfo->szClasses, JIDA_BOARD_CLASS_CPU "," JIDA_BOARD_CLASS_VGA "," JIDA_BOARD_CLASS_IO,
		JIDA_BOARD_MAX_SIZE_CLASSES_STRING - 1);

	/* usual manufacturer is KEM so we should put "JUMP".*/
	strncat(pBoardInfo->szManufacturer, "JUMP", JIDA_BOARD_MAX_SIZE_ID_STRING - 1);

	ts = gmtime((time_t *)&bInfo.manufacturingDate);

	pBoardInfo->stManufacturingDate.wDay = (WORD)ts->tm_mday;
	pBoardInfo->stManufacturingDate.wDayOfWeek = (WORD)ts->tm_wday;
	pBoardInfo->stManufacturingDate.wHour = (WORD)ts->tm_hour;
	pBoardInfo->stManufacturingDate.wMilliseconds = (WORD)0;
	pBoardInfo->stManufacturingDate.wMinute = (WORD)ts->tm_min;
	pBoardInfo->stManufacturingDate.wMonth = (WORD)ts->tm_mon + 1;
	pBoardInfo->stManufacturingDate.wSecond = (WORD)ts->tm_sec;
	pBoardInfo->stManufacturingDate.wYear = (WORD)ts->tm_year + 1900;

	memset(&pBoardInfo->stLastRepairDate, 0, sizeof(SYSTEMTIME));
	strncpy(pBoardInfo->szSerialNumber, bInfo.boardSerialNumber, JIDA_BOARD_MAX_SIZE_ID_STRING);

	/* wFeatureNumber is the first digit of the KEM BIOS revision
	 * (e.g 1 for MODBR123).
	 * looking for first digit */
	while (bInfo.firmwareVersion[i] < 0x30 && bInfo.firmwareVersion[i] > 0x39)
		i++;
	pBoardInfo->wFeatureNumber = (WORD)bInfo.firmwareVersion[i];

	/* wFirmwareRevision is the last two digits of the BIOS Revision
	 * (e.g. 0x00020003 for MODBR123).
	 * looking for first non-digit */
	while (bInfo.firmwareVersion[i] >= 0x30 && bInfo.firmwareVersion[i] <= 0x39)
		i++;
	/* and take 2 last digit */
	pBoardInfo->wFirmwareRevision =
		(WORD)(bInfo.firmwareVersion[i - 2] << 0x8) | (WORD)(bInfo.firmwareVersion[i - 1]);

	/* convert to BCD. use 2 LOW bytes.
	 * Example: "Rev. 1.24"
	 *   0    1    2    4
	 *  0000 0001 0010 0100 */
	i = 0;
	pBoardInfo->wHardwareRevision = 0;
	ret = sscanf(bInfo.hardwareVersion, "%d.%d.%d.%d", &revBuf[0], &revBuf[1], &revBuf[2], &revBuf[3]);
	for (i = 0; i < ret; i++) {
		if (revBuf[i] >= 0 && revBuf[i] <= 0xFF)
			pBoardInfo->wHardwareRevision = (WORD)((pBoardInfo->wHardwareRevision << 4) | revBuf[i]);
		else
			break;
	}

	if (KEApiGetLibVersion(&kvd) != KEAPI_RET_SUCCESS)
		return FALSE;
	pBoardInfo->wJidaRevision = (WORD)(kvd.version >> 0x0f);

	return TRUE;
}

BOOL WINAPI JidaBoardGetOption(HJIDA hJida, DWORD dwOption, LPDWORD pdwSetting)
{
	/* Not supported */

	return FALSE;
}

BOOL WINAPI JidaBoardSetOption(HJIDA hJida, DWORD dwOption, DWORD dwSetting)
{
	/* Not supported */

	return FALSE;
}

BOOL WINAPI JidaBoardGetBootErrorLog(HJIDA hJida, DWORD dwType, PDWORD pdwLogType, LPBYTE pBytes, PDWORD pdwLen)
{
	/* Not supported */

	return FALSE;
}

BOOL WINAPI JidaBoardGetBootCounter(HJIDA hJida, LPDWORD pdwCount)
{
	int32_t count;

	if (KEApiGetBootCounter(&count) != KEAPI_RET_SUCCESS)
		return FALSE;

	*pdwCount = (DWORD)count;

	return TRUE;
}

BOOL WINAPI JidaBoardGetRunningTimeMeter(HJIDA hJida, LPDWORD pdwCount)
{
	/* Not supported */

	return FALSE;
}

// Contrast -------------------------------------------------------------------
/* The functions are not supported. Functionality is deprecated. */
BOOL WINAPI JidaVgaGetContrast(HJIDA hJida, LPDWORD pdwSetting)
{
	return FALSE;
}

BOOL WINAPI JidaVgaSetContrast(HJIDA hJida, DWORD dwSetting)
{
	return FALSE;
}

BOOL WINAPI JidaVgaGetContrastEnable(HJIDA hJida, LPDWORD pdwSetting)
{
	return FALSE;
}

BOOL WINAPI JidaVgaSetContrastEnable(HJIDA hJida, DWORD dwSetting)
{
	return FALSE;
}

// Backlight ------------------------------------------------------------------
BOOL WINAPI JidaVgaGetBacklight(HJIDA hJida, LPDWORD pdwSetting)
{
	int32_t displayNr = 0;
	int32_t value;

	if (KEApiGetBacklightValue(displayNr, &value) != KEAPI_RET_SUCCESS)
		return FALSE;

	if (value < 0 || value > JIDA_VGA_BACKLIGHT_MAX)
		return FALSE;

	*pdwSetting = (DWORD)value;

	return TRUE;
}

BOOL WINAPI JidaVgaSetBacklight(HJIDA hJida, DWORD dwSetting)
{
	int32_t displayNr = 0;

	if (KEApiSetBacklightValue(displayNr, (int8_t)dwSetting) != KEAPI_RET_SUCCESS)
		return FALSE;

	return TRUE;
}

BOOL WINAPI JidaVgaGetBacklightEnable(HJIDA hJida, LPDWORD pdwSetting)
{
	int32_t displayNr = 0;
	int32_t value = -1;

	if (KEApiGetBacklightValue(displayNr, &value) != KEAPI_RET_SUCCESS)
		return FALSE;

	if (value > 0)
		*pdwSetting = TRUE;
	else
		*pdwSetting = FALSE;

	return TRUE;
}

BOOL WINAPI JidaVgaSetBacklightEnable(HJIDA hJida, DWORD dwSetting)
{
	int32_t displayNr = 0;
	int32_t value;

	if (dwSetting)
		value = KEAPI_DISPLAY_BRIGHTNESS_MAX;
	else
		value = 0;

	if (KEApiSetBacklightValue(displayNr, value) != KEAPI_RET_SUCCESS)
		return FALSE;

	return TRUE;
}

BOOL WINAPI JidaVgaEndDarkBoot(DWORD dwReserved)
{
	return FALSE; //deprecated
}

// Storage --------------------------------------------------------------------
DWORD WINAPI JidaStorageAreaCount(HJIDA hJida, DWORD dwType)
{
	int32_t storageCount;

	if (dwType != JIDA_STORAGE_AREA_EEPROM) // other types are not supported in v1.9a
		return 0;

	if (KEApiGetStorageCount(&storageCount) != KEAPI_RET_SUCCESS)
		return 0;

	return (DWORD)storageCount;
}

DWORD WINAPI JidaStorageAreaType(HJIDA hJida, DWORD dwType)
{
	int32_t storageCount;

	if (KEApiGetStorageCount(&storageCount) != KEAPI_RET_SUCCESS)
		return 0;

	if (dwType >= (DWORD)storageCount)
		return 0;

	return JIDA_STORAGE_AREA_EEPROM;
}

DWORD WINAPI JidaStorageAreaSize(HJIDA hJida, DWORD dwType)
{
	int32_t storageSize;
	int32_t targetNum;

	if (!(dwType & JIDA_STORAGE_AREA_EEPROM)) // other types are not supported in v1.9a
		return 0;

	targetNum = dwType & (!JIDA_STORAGE_AREA_EEPROM);

	if (KEApiGetStorageSize(targetNum, &storageSize) != KEAPI_RET_SUCCESS)
		return 0;

	return (DWORD)storageSize;
}

DWORD WINAPI JidaStorageAreaBlockSize(HJIDA hJida, DWORD dwType)
{
	return 0; // Is not implemented even in JIDA
}

BOOL WINAPI JidaStorageAreaRead(HJIDA hJida, DWORD dwType, DWORD dwOffset, LPBYTE pBytes, DWORD dwLen)
{
	int32_t targetNum;

	if (!(dwType & JIDA_STORAGE_AREA_EEPROM)) // other types are not supported in v1.9a
		return FALSE;

	targetNum = dwType & (!JIDA_STORAGE_AREA_EEPROM);

	if (KEApiStorageRead(targetNum, (int32_t)dwOffset, (uint8_t *)pBytes, (int32_t)dwLen) != KEAPI_RET_SUCCESS)
		return FALSE;

	return TRUE;
}

BOOL WINAPI JidaStorageAreaWrite(HJIDA hJida, DWORD dwType, DWORD dwOffset, LPBYTE pBytes, DWORD dwLen)
{
	int32_t targetNum;

	if (!(dwType & JIDA_STORAGE_AREA_EEPROM)) // other types are not supported in v1.9a
		return FALSE;

	targetNum = dwType & (!JIDA_STORAGE_AREA_EEPROM);

	if (KEApiStorageWrite(targetNum, (int32_t)dwOffset, (uint8_t *)pBytes, (int32_t)dwLen) != KEAPI_RET_SUCCESS)
		return FALSE;

	return TRUE;
}

BOOL WINAPI JidaStorageAreaErase(HJIDA hJida, DWORD dwType, DWORD dwOffset, DWORD dwLen)
{
	return FALSE; // Is not implemented even in v1.9a
}

BOOL WINAPI JidaStorageAreaEraseStatus(HJIDA hJida, DWORD dwType, DWORD dwOffset, DWORD dwLen, LPDWORD lpStatus)
{
	return FALSE; // Is not implemented even in v1.9a
}

// I2C/SMBUS busses -----------------------------------------------------------
DWORD WINAPI JidaI2CCount(HJIDA hJida)
{
	int32_t i2cCount;

	if (KEApiGetI2cBusCount(&i2cCount) != KEAPI_RET_SUCCESS)
		return 0;

	return (DWORD)i2cCount;
}

DWORD WINAPI JidaI2CType(HJIDA hJida, DWORD dwType)
{
	int32_t i2cCount;

	if (KEApiGetI2cBusCount(&i2cCount) != KEAPI_RET_SUCCESS)
		return 0;

	if (i2cCount > 0 && dwType >= 0 && dwType < (DWORD)i2cCount)
		return JIDA_I2C_TYPE_PRIMARY;

	return JIDA_I2C_TYPE_UNKNOWN;
}

BOOL WINAPI JidaI2CIsAvailable(HJIDA hJida, DWORD dwType)
{
	int32_t i2cCount;

	if (KEApiGetI2cBusCount(&i2cCount) != KEAPI_RET_SUCCESS)
		return FALSE;

	if (i2cCount > 0 && dwType >= 0 && dwType < (DWORD)i2cCount)
		return TRUE;

	return FALSE;
}

BOOL WINAPI JidaI2CWriteReadCombined(HJIDA hJida, DWORD dwType, BYTE bAddr, LPBYTE pBytesWrite, DWORD dwLenWrite,
				     LPBYTE pBytesRead, DWORD dwLenRead)
{
	uint8_t devAddr = bAddr >> 1; // normalize for KEAPI i2c device address
	int32_t bytesRead = (int32_t)dwLenRead;

	if (KEApiI2cXfer((int32_t)dwType, devAddr, (uint8_t *)pBytesWrite, (int32_t)dwLenWrite, (uint8_t *)pBytesRead,
			 &bytesRead) != KEAPI_RET_SUCCESS)
		return FALSE;

	return TRUE;
}

BOOL WINAPI JidaI2CRead(HJIDA hJida, DWORD dwType, BYTE bAddr, LPBYTE pBytes, DWORD dwLen)
{
	uint8_t devAddr = bAddr >> 1; // normalize for KEAPI i2c device address

	if (KEApiI2cXfer((int32_t)dwType, devAddr, NULL, 0, (uint8_t *)pBytes, (int32_t *)dwLen) != KEAPI_RET_SUCCESS)
		return FALSE;

	return TRUE;
}

BOOL WINAPI JidaI2CWrite(HJIDA hJida, DWORD dwType, BYTE bAddr, LPBYTE pBytes, DWORD dwLen)
{
	int32_t readLen = 0;
	uint8_t devAddr = bAddr >> 1; // normalize for KEAPI i2c device address

	if (KEApiI2cXfer((int32_t)dwType, devAddr, (uint8_t *)pBytes, (int32_t)dwLen, NULL, &readLen) !=
	    KEAPI_RET_SUCCESS)
		return FALSE;

	return TRUE;
}

BOOL WINAPI JidaI2CReadRegister(HJIDA hJida, DWORD dwType, BYTE bAddr, WORD wReg, LPBYTE pDataByte)
{
	int32_t readLen = 1;
	uint8_t devAddr = bAddr >> 1; // normalize for KEAPI i2c device address

	if (KEApiI2cXfer((int32_t)dwType, devAddr, (uint8_t *)&wReg, sizeof(WORD), (uint8_t *)pDataByte, &readLen) !=
	    KEAPI_RET_SUCCESS)
		return FALSE;

	if (readLen != 1)
		return FALSE;

	return TRUE;
}

BOOL WINAPI JidaI2CWriteRegister(HJIDA hJida, DWORD dwType, BYTE bAddr, WORD wReg, BYTE bData)
{
	int32_t readLen = 0;
	uint8_t devAddr = bAddr >> 1; // normalize for KEAPI i2c device address
	static const int32_t wrDataLen = 3; //sizeof(WORD) + sizeof(BYTE)
	uint8_t wrData[3]; //wrDataLen

	wrData[0] = (uint8_t)(0xff & (wReg >> 8));
	wrData[1] = (uint8_t)(0xff & wReg);
	wrData[2] = 0xff & bData;

	if (KEApiI2cXfer((int32_t)dwType, devAddr, wrData, wrDataLen, NULL, &readLen) != KEAPI_RET_SUCCESS)
		return FALSE;

	return TRUE;
}

// GPIO -----------------------------------------------------------------------
DWORD WINAPI JidaIOCount(HJIDA hJida)
{
	int32_t count = 0;

	if (KEApiGetGpioPortCount(&count) != KEAPI_RET_SUCCESS)
		return 0;

	return count;
}

BOOL WINAPI JidaIOIsAvailable(HJIDA hJida, DWORD dwType)
{
	int32_t count = 0;

	if (KEApiGetGpioPortCount(&count) != KEAPI_RET_SUCCESS)
		return FALSE;

	if (dwType < 0 || dwType >= (DWORD)count) /* 64-bit type convertation */
		return FALSE;

	return TRUE;
}

BOOL WINAPI JidaIORead(HJIDA hJida, DWORD dwType, LPDWORD pdwData)
{
	uint32_t levels = 0;

	if (KEApiGetGpioPortLevels((int32_t)dwType, &levels) != KEAPI_RET_SUCCESS)
		return FALSE;

	*pdwData = (DWORD)levels;

	return TRUE;
}

BOOL WINAPI JidaIOWrite(HJIDA hJida, DWORD dwType, DWORD dwData)
{
	uint32_t levels = (uint32_t)dwData;
	int32_t port = (int32_t)dwType;

	if (KEApiSetGpioPortLevels(port, levels) != KEAPI_RET_SUCCESS)
		return FALSE;

	return TRUE;
}

BOOL WINAPI JidaIOXorAndXor(HJIDA hJida, DWORD dwType, DWORD dwXorMask1, DWORD dwAndMask, DWORD dwXorMask2)
{
	uint32_t currentPortValue, newPortValue;
	int32_t port = (int32_t)dwType;

	if (KEApiGetGpioPortLevels(port, &currentPortValue) != KEAPI_RET_SUCCESS)
		return FALSE;

	newPortValue = (((currentPortValue ^ (uint32_t)dwXorMask1) & (uint32_t)dwAndMask) ^ (uint32_t)dwXorMask2);

	if (KEApiSetGpioPortLevels(port, newPortValue) != KEAPI_RET_SUCCESS)
		return FALSE;

	return FALSE;
}

BOOL WINAPI JidaIOGetDirection(HJIDA hJida, DWORD dwType, LPDWORD pdwData)
{
	uint32_t dirs = 0;

	if (KEApiGetGpioPortDirections((int32_t)dwType, &dirs) != KEAPI_RET_SUCCESS)
		return FALSE;

	*pdwData = (DWORD)dirs;

	return TRUE;
}

BOOL WINAPI JidaIOSetDirection(HJIDA hJida, DWORD dwType, DWORD dwData)
{
	uint32_t dirs = (uint32_t)dwData;
	int32_t port = (int32_t)dwType;

	if (KEApiSetGpioPortDirections(port, dirs) != KEAPI_RET_SUCCESS)
		return FALSE;

	return TRUE;
}

BOOL WINAPI JidaIOGetDirectionCaps(HJIDA hJida, DWORD dwType, LPDWORD pdwInputs, LPDWORD pdwOutputs)
{
	uint32_t isIns, isOuts = 0;

	if (KEApiGetGpioPortDirectionCaps((int32_t)dwType, &isIns, &isOuts) != KEAPI_RET_SUCCESS)
		return FALSE;

	*pdwInputs = (DWORD)isIns;
	*pdwOutputs = (DWORD)isOuts;

	return TRUE;
}

/* This function is not implemented yet and will always return FALSE. */
BOOL WINAPI JidaIOGetNameA(HJIDA hJida, DWORD dwType, LPSTR pszName, DWORD dwSize)
{
	return FALSE;
}

// Watchdog -------------------------------------------------------------------
DWORD WINAPI JidaWDogCount(HJIDA hJida)
{
	int32_t maxT, minT, stagesCnt;

	if (KEApiWatchdogGetCaps(&maxT, &minT, &stagesCnt) != KEAPI_RET_SUCCESS)
		return 0;

	return 1;
}

BOOL WINAPI JidaWDogIsAvailable(HJIDA hJida, DWORD dwType)
{
	int32_t maxT, minT, stageCnt;

	if (dwType != 0)
		return FALSE;

	if (KEApiWatchdogGetCaps(&maxT, &minT, &stageCnt) != KEAPI_RET_SUCCESS)
		return FALSE;

	return TRUE;
}

BOOL WINAPI JidaWDogTrigger(HJIDA hJida, DWORD dwType)
{
	if (dwType != 0)
		return FALSE;

	if (KEApiWatchdogTrigger() != KEAPI_RET_SUCCESS)
		return FALSE;

	return TRUE;
}

BOOL WINAPI JidaWDogGetConfigStruct(HJIDA hJida, DWORD dwType, PJWDCONFIG pConfig)
{
	int32_t maxTimeout, minTimeout, stagesCnt;

	if (dwType != 0 || pConfig == NULL)
		return FALSE;

	if (KEApiWatchdogGetCaps(&maxTimeout, &minTimeout, &stagesCnt) != KEAPI_RET_SUCCESS)
		return FALSE;

	pConfig->dwDelay = 0;
	pConfig->dwMode = JWDModeRebootPC;
	pConfig->dwTimeout = 0; /*TODO: hm. This is not Maximal Timeout....
	 * How we can get this value? We do not store this value? */

	return TRUE;
}

BOOL WINAPI JidaWDogSetConfigStruct(HJIDA hJida, DWORD dwType, PJWDCONFIG pConfig)
{
	if (dwType != 0 || pConfig == NULL || pConfig->dwDelay > 0 || pConfig->dwMode != JWDModeRebootPC)
		return FALSE;

	if (KEApiWatchdogSetup(0, (int32_t)pConfig->dwTimeout, KEAPI_WD_MODE_RESET) != KEAPI_RET_SUCCESS)
		return FALSE;

	if (KEApiWatchdogEnable() != KEAPI_RET_SUCCESS)
		return FALSE;

	return TRUE;
}

BOOL WINAPI JidaWDogSetConfig(HJIDA hJida, DWORD dwType, DWORD timeout, DWORD delay, DWORD mode)
{
	if (dwType != 0 || delay > 0 || mode != JWDModeRebootPC)
		return FALSE;

	if (KEApiWatchdogSetup(0, (int32_t)timeout, KEAPI_WD_MODE_RESET) != KEAPI_RET_SUCCESS)
		return FALSE;

	if (KEApiWatchdogEnable() != KEAPI_RET_SUCCESS)
		return FALSE;

	return TRUE;
}

BOOL WINAPI JidaWDogDisable(HJIDA hJida, DWORD dwType)
{
	if (dwType != 0)
		return FALSE;

	if (KEApiWatchdogDisable() != KEAPI_RET_SUCCESS)
		return FALSE;

	return TRUE;
}

// Performance ----------------------------------------------------------------
BOOL WINAPI JidaPerformanceGetCurrent(HJIDA hJida, DWORD dwType, LPDWORD pdwSetting)
{
	int32_t coreNr = 0;
	int8_t performance = 0;

	if (dwType != 0)
		return FALSE;

	if (KEApiGetCpuPerformance(coreNr, &performance) != KEAPI_RET_SUCCESS)
		return FALSE;

	*pdwSetting = (DWORD)performance;

	return TRUE;
}

BOOL WINAPI JidaPerformanceSetCurrent(HJIDA hJida, DWORD dwType, DWORD dwSetting)
{
	return FALSE; /* SET PERFORMANCE functionality was removed in KEAPI specification v3.
	 * The reason is inability to provide support of this functionality on
	 * different platforms and architectures. */
}

BOOL WINAPI JidaPerformanceGetPolicyCaps(HJIDA hJida, DWORD dwType, LPDWORD pdwSetting)
{
	return FALSE; // Unsupported by KEAPIv3
}

BOOL WINAPI JidaPerformanceGetPolicy(HJIDA hJida, DWORD dwType, LPDWORD pdwSetting)
{
	return FALSE; // Unsupported by KEAPIv3
}

BOOL WINAPI JidaPerformanceSetPolicy(HJIDA hJida, DWORD dwType, DWORD dwSetting)
{
	return FALSE; // Unsupported by KEAPIv3
}

// Temperature ----------------------------------------------------------------
DWORD WINAPI JidaTemperatureCount(HJIDA hJida)
{
	int32_t count = 0;

	if (KEApiGetTempSensorCount(&count) != KEAPI_RET_SUCCESS)
		return 0;

	return (DWORD)count;
}

BOOL WINAPI JidaTemperatureGetCurrent(HJIDA hJida, DWORD dwType, LPDWORD pdwSetting, LPDWORD pdwStatus)
{
	KEAPI_SENSOR_VALUE sensorValue;

	if (KEApiGetTempSensorValue((int32_t)dwType, &sensorValue) != KEAPI_RET_SUCCESS)
		return FALSE;

	*pdwSetting = (DWORD)sensorValue.value;
	*pdwStatus = (DWORD)sensorValue.status;

	return TRUE;
}

BOOL WINAPI JidaTemperatureGetInfo(HJIDA hJida, DWORD dwType, PJIDATEMPERATUREINFO pInfo)
{
	int32_t sens_count = 0;
	KEAPI_SENSOR_INFO sensorInfo;
	KEAPI_SENSOR_VALUE sensorValue;

	if (pInfo == NULL)
		return FALSE;

	if (KEApiGetTempSensorCount(&sens_count) != KEAPI_RET_SUCCESS || (DWORD)sens_count <= dwType)
		return FALSE;

	if (KEApiGetTempSensorInfo((int32_t)dwType, &sensorInfo) != KEAPI_RET_SUCCESS)
		return FALSE;

	switch (sensorInfo.type) {
	case KEAPI_TEMP_BACKPLANE:
		pInfo->dwType = JIDA_TEMP_BACKPLANE;
		break;
	case KEAPI_TEMP_BOARD:
		pInfo->dwType = JIDA_TEMP_BOARD;
		break;
	case KEAPI_TEMP_BOX:
		pInfo->dwType = JIDA_TEMP_BOX;
		break;
	case KEAPI_TEMP_CHIPSET:
		pInfo->dwType = JIDA_TEMP_CHIPSETS;
		break;
	case KEAPI_TEMP_CPU:
		pInfo->dwType = JIDA_TEMP_CPU;
		break;
	case KEAPI_TEMP_ENV:
		pInfo->dwType = JIDA_TEMP_ENV;
		break;
	case KEAPI_TEMP_VIDEO:
		pInfo->dwType = JIDA_TEMP_VIDEO;
		break;
	default:
		pInfo->dwType = JIDA_TEMP_OTHER;
		break;
	}

	/* field "flags" - is not supported */
	pInfo->dwAlarmHi = sensorInfo.alarmHi;
	pInfo->dwAlarmLo = sensorInfo.alarmLo;
	pInfo->dwFlags = 0;
	pInfo->dwHystHi = sensorInfo.hystHi;
	pInfo->dwHystLo = sensorInfo.hystLo;
	pInfo->dwMax = sensorInfo.max;
	pInfo->dwMin = sensorInfo.min;

	if (KEApiGetTempSensorValue((int32_t)dwType, &sensorValue) != KEAPI_RET_SUCCESS)
		return FALSE;

	pInfo->dwRes = (DWORD)sensorValue.value;
	pInfo->dwAlarm = (DWORD)sensorValue.status;

	return TRUE;
}

// Fan -------------------------------------------------------------------------
DWORD WINAPI JidaFanCount(HJIDA hJida)
{
	int32_t count = 0;

	if (KEApiGetFanSensorCount(&count) != KEAPI_RET_SUCCESS)
		return 0;

	return (DWORD)count;
}

BOOL WINAPI JidaFanGetCurrent(HJIDA hJida, DWORD dwType, LPDWORD pdwSetting, LPDWORD pdwStatus)
{
	KEAPI_SENSOR_VALUE sensorValue;

	if (KEApiGetFanSensorValue((int32_t)dwType, &sensorValue) != KEAPI_RET_SUCCESS)
		return FALSE;

	*pdwSetting = (DWORD)sensorValue.value;
	*pdwStatus = (DWORD)sensorValue.status;

	return TRUE;
}

BOOL WINAPI JidaFanGetInfo(HJIDA hJida, DWORD dwType, PJIDAFANINFO pInfo)
{
	int32_t sens_count = 0;
	KEAPI_SENSOR_INFO sensorInfo;
	KEAPI_SENSOR_VALUE sensorValue;

	if (pInfo == NULL)
		return FALSE;

	if (KEApiGetFanSensorCount(&sens_count) != KEAPI_RET_SUCCESS || (DWORD)sens_count <= dwType)
		return FALSE;

	if (KEApiGetFanSensorInfo((int32_t)dwType, &sensorInfo) != KEAPI_RET_SUCCESS)
		return FALSE;

	switch (sensorInfo.type) {
	case KEAPI_FAN_BOX:
		pInfo->dwType = JIDA_FAN_BOX;
		break;
	case KEAPI_FAN_CHIPSET:
		pInfo->dwType = JIDA_FAN_CHIPSET;
		break;
	case KEAPI_FAN_CPU:
		pInfo->dwType = JIDA_FAN_CPU;
		break;
	case KEAPI_FAN_ENV:
		pInfo->dwType = JIDA_FAN_ENV;
		break;
	case KEAPI_FAN_VIDEO:
		pInfo->dwType = JIDA_FAN_VIDEO;
		break;
	default:
		pInfo->dwType = JIDA_FAN_OTHER;
		break;
	}

	/* field "flags" - is not supported */
	pInfo->dwAlarmHi = sensorInfo.alarmHi;
	pInfo->dwAlarmLo = sensorInfo.alarmLo;
	pInfo->dwFlags = 0;
	pInfo->dwHystHi = sensorInfo.hystHi;
	pInfo->dwHystLo = sensorInfo.hystLo;
	pInfo->dwMax = sensorInfo.max;
	pInfo->dwMin = sensorInfo.min;

	if (KEApiGetTempSensorValue((int32_t)dwType, &sensorValue) != KEAPI_RET_SUCCESS)
		return FALSE;

	pInfo->dwSpeedNom = (DWORD)sensorValue.value;
	pInfo->dwAlarm = (DWORD)sensorValue.status;

	return TRUE;
}

// Voltage --------------------------------------------------------------------
DWORD WINAPI JidaVoltageCount(HJIDA hJida)
{
	int32_t count = 0;

	if (KEApiGetVoltageSensorCount(&count) != KEAPI_RET_SUCCESS)
		return 0;

	return (DWORD)count;
}

BOOL WINAPI JidaVoltageGetCurrent(HJIDA hJida, DWORD dwType, LPDWORD pdwSetting, LPDWORD pdwStatus)
{
	KEAPI_SENSOR_VALUE sensorValue;

	if (KEApiGetVoltageSensorValue((int32_t)dwType, &sensorValue) != KEAPI_RET_SUCCESS)
		return FALSE;

	*pdwSetting = (DWORD)sensorValue.value;
	*pdwStatus = (DWORD)sensorValue.status;

	return TRUE;
}

BOOL WINAPI JidaVoltageGetInfo(HJIDA hJida, DWORD dwType, PJIDAVOLTAGEINFO pInfo)
{
	int32_t sens_count = 0;
	KEAPI_SENSOR_INFO sensorInfo;
	KEAPI_SENSOR_VALUE sensorValue;

	if (pInfo == NULL)
		return FALSE;

	if (KEApiGetVoltageSensorCount(&sens_count) != KEAPI_RET_SUCCESS || (DWORD)sens_count <= dwType)
		return FALSE;

	if (KEApiGetVoltageSensorInfo((int32_t)dwType, &sensorInfo) != KEAPI_RET_SUCCESS)
		return FALSE;

	switch (sensorInfo.type) {
	case KEAPI_VOLTAGE_VCORE:
		pInfo->dwType = JIDA_VOLTAGE_CPU;
		break;
	case KEAPI_VOLTAGE_VBAT:
		pInfo->dwType = JIDA_VOLTAGE_BAT_CMOS;
		break;
	case KEAPI_VOLTAGE_5VSB:
		pInfo->dwType = JIDA_VOLTAGE_DC_STANDBY;
		break;
	case KEAPI_VOLTAGE_5V:
		pInfo->dwType = JIDA_VOLTAGE_DC;
		break;
	case KEAPI_VOLTAGE_AC:
		pInfo->dwType = JIDA_VOLTAGE_AC;
		break;
	case KEAPI_VOLTAGE_12V:
	case KEAPI_VOLTAGE_1V8:
	case KEAPI_VOLTAGE_2V5:
	case KEAPI_VOLTAGE_3V3:
	default:
		pInfo->dwType = JIDA_VOLTAGE_OTHER;
		break;
	}

	/* field "flags" - is not supported */
	pInfo->dwAlarmHi = sensorInfo.alarmHi;
	pInfo->dwAlarmLo = sensorInfo.alarmLo;
	pInfo->dwFlags = 0;
	pInfo->dwHystHi = sensorInfo.hystHi;
	pInfo->dwHystLo = sensorInfo.hystLo;
	pInfo->dwMax = sensorInfo.max;
	pInfo->dwMin = sensorInfo.min;

	if (KEApiGetVoltageSensorValue((int32_t)dwType, &sensorValue) != KEAPI_RET_SUCCESS)
		return FALSE;

	pInfo->dwNom = (DWORD)sensorValue.value;
	pInfo->dwAlarm = (DWORD)sensorValue.status;

	return TRUE;
}
