/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron AG */

/* PICMG EAPI functions wrapper. */

#include "keapi_inc.h"

#if PLATFORM(VxWorks)
#include "versionK.h"
KONTRON_VERSION(PicmgWrapper_c, 01c) /* version identifier */
#endif /* VxWorks */

#if PLATFORM(Linux)
#include "eapi/linux/EApiOs.h"
#elif PLATFORM(VxWorks)
#include "eapi/vxWorks/EApiOs.h"
#elif PLATFORM(Windows)
#include "eapi/winnt/EApiOs.h"
#elif PLATFORM(WindowsCE)
#include "eapi/wince/EApiOs.h"
#else
#error "Unknown Platform"
#endif

#include "eapi/EApi.h"
#include "eapi/EApiCommon.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if PLATFORM(Linux)
#include <fcntl.h>
#endif

#if PLATFORM(Windows)
#define STRICMP(a, b) stricmp(a, b)
#elif PLATFORM(WindowsCE)
#define STRICMP(a, b) _stricmp(a, b)
#else
#define STRICMP(a, b) strcasecmp(a, b)
#endif /* PLATFORM(Windows) */

int32_t keyStringIsFound(char *keyString, char *var1String, char *var2String, char *var3String, char *var4String,
			 char *var5String)
{
	if (keyString == NULL)
		return 0;
	if ((var1String != NULL) && STRICMP(keyString, var1String) == 0)
		return 1;
	if ((var2String != NULL) && STRICMP(keyString, var2String) == 0)
		return 1;
	if ((var3String != NULL) && STRICMP(keyString, var3String) == 0)
		return 1;
	if ((var4String != NULL) && STRICMP(keyString, var4String) == 0)
		return 1;
	if ((var5String != NULL) && STRICMP(keyString, var5String) == 0)
		return 1;
	return 0;
}

/*******************************************************************************/
EApiStatus_t convertErrorCodes(uint32_t keapiCode)
{
	switch (keapiCode) {
	case KEAPI_RET_SUCCESS:
		return EAPI_STATUS_SUCCESS;
	case KEAPI_RET_ERROR:
		return EAPI_STATUS_ERROR;
	case KEAPI_RET_PARAM_ERROR:
		return EAPI_STATUS_INVALID_PARAMETER;
	case KEAPI_RET_PARAM_NULL:
		return EAPI_STATUS_INVALID_PARAMETER;
	case KEAPI_RET_BUFFER_OVERFLOW:
		return EAPI_STATUS_ERROR;
	case KEAPI_RET_SETTING_ERROR:
		return EAPI_STATUS_WRITE_ERROR;
	case KEAPI_RET_WRITE_ERROR:
		return EAPI_STATUS_WRITE_ERROR;
	case KEAPI_RET_READ_ERROR:
		return EAPI_STATUS_READ_ERROR;
	case KEAPI_RET_MALLOC_ERROR:
		return EAPI_STATUS_ALLOC_ERROR;
	case KEAPI_RET_LIBRARY_ERROR:
		return EAPI_STATUS_ERROR;
	case KEAPI_RET_WMI_ERROR:
		return EAPI_STATUS_READ_ERROR;
	case KEAPI_RET_NOT_INITIALIZED:
		return EAPI_STATUS_NOT_INITIALIZED;
	case KEAPI_RET_PARTIAL_SUCCESS:
		return EAPI_STATUS_SUCCESS;
	case KEAPI_RET_FUNCTION_NOT_SUPPORTED:
		return EAPI_STATUS_UNSUPPORTED;
	case KEAPI_RET_RETRIEVAL_ERROR:
		return EAPI_STATUS_READ_ERROR;
	case KEAPI_RET_FUNCTION_NOT_IMPLEMENTED:
		return EAPI_STATUS_UNSUPPORTED;
	}
	return EAPI_STATUS_ERROR;
}

/*******************************************************************************/
EApiStatus_t EAPI_CALLTYPE EApiLibInitialize(void)
{
	if (KEApiLibInitialize() == KEAPI_RET_SUCCESS)
		return EAPI_STATUS_SUCCESS;
	else
		return EAPI_STATUS_ERROR;
}

/*******************************************************************************/
EApiStatus_t EAPI_CALLTYPE EApiLibUnInitialize(void)
{
	if (KEApiLibUnInitialize() == KEAPI_RET_SUCCESS)
		return EAPI_STATUS_SUCCESS;
	else
		return EAPI_STATUS_ERROR;
}

/*******************************************************************************/
EApiStatus_t EAPI_CALLTYPE EApiBoardGetStringA(__IN EApiId_t Id, /* Name Id */
					       __OUT char *pBuffer, /* Destination pBuffer */
					       __INOUT uint32_t *pBuffLen /* pBuffer Length */
)
{
	KEAPI_BOARD_INFO board_info;
	uint32_t res;
	char *resStr = NULL;
	size_t len;

	/* Check function parameters */
	if (pBuffLen == NULL || (*pBuffLen && pBuffer == NULL))
		return EAPI_STATUS_INVALID_PARAMETER;

	if ((res = KEApiGetBoardInfo(&board_info)) != KEAPI_RET_SUCCESS && res != KEAPI_RET_PARTIAL_SUCCESS)
		return EAPI_STATUS_READ_ERROR;

	switch (Id) {
	case EAPI_ID_BOARD_MANUFACTURER_STR: {
		resStr = board_info.boardManufacturer;
		break;
	}

	case EAPI_ID_BOARD_NAME_STR: {
		resStr = board_info.boardName;
		break;
	}

	case EAPI_ID_BOARD_SERIAL_STR: {
		resStr = board_info.boardSerialNumber;
		break;
	}

	case EAPI_ID_BOARD_BIOS_REVISION_STR: {
		resStr = board_info.firmwareVersion;
		break;
	}

	case EAPI_ID_BOARD_HW_REVISION_STR: {
		resStr = board_info.hardwareVersion;
		break;
	}

	case EAPI_ID_BOARD_PLATFORM_TYPE_STR:
	default:
		return EAPI_STATUS_UNSUPPORTED;
	}

	len = strlen(resStr);
	if (len + 1 <= *pBuffLen) {
		memcpy(pBuffer, resStr, len);
		*pBuffLen = (uint32_t)(len + 1); //64-bit type conversion
		pBuffer[len] = '\0';
		return EAPI_STATUS_SUCCESS;
	} else {
		*pBuffLen = (uint32_t)(len + 1); //64-bit type conversion
		return EAPI_STATUS_MORE_DATA;
	}
}

#define to1x10Kelvins(millicelsius) ((millicelsius + 100 * EAPI_KELVINS_OFFSET) / 100)

/*******************************************************************************/
EApiStatus_t EAPI_CALLTYPE EApiBoardGetValue(__IN EApiId_t Id, /* Value Id */
					     __OUT uint32_t *pValue /* Return Value */
)
{
	KEAPI_SENSOR_VALUE sensorValue;
	KEAPI_SENSOR_INFO sensorInfo;
	KEAPI_VERSION_DATA keapiVersion;
	int32_t sens_count, i;

	/* Check function parameters */
	if (pValue == NULL)
		return EAPI_STATUS_INVALID_PARAMETER;

	switch (Id) {
	case EAPI_ID_GET_EAPI_SPEC_VERSION: {
		*pValue = EAPI_VERSION;
		return EAPI_STATUS_SUCCESS;
	}

	case EAPI_ID_BOARD_BOOT_COUNTER_VAL: {
		int32_t bootCount;
		if (KEApiGetBootCounter(&bootCount) == KEAPI_RET_SUCCESS) {
			*pValue = bootCount;
			return EAPI_STATUS_SUCCESS;
		} else
			return EAPI_STATUS_READ_ERROR;
	}

	case EAPI_ID_BOARD_RUNNING_TIME_METER_VAL: {
		int32_t systemUpTime;
		if (KEApiSystemUpTime(&systemUpTime) == KEAPI_RET_SUCCESS) {
			*pValue = systemUpTime;
			return EAPI_STATUS_SUCCESS;
		} else
			return EAPI_STATUS_READ_ERROR;
	}

	case EAPI_ID_BOARD_PNPID_VAL: {
		return EAPI_STATUS_UNSUPPORTED;
	}

	case EAPI_ID_BOARD_PLATFORM_REV_VAL: {
		return EAPI_STATUS_UNSUPPORTED;
	}

	case EAPI_ID_BOARD_DRIVER_VERSION_VAL: {
		return EAPI_STATUS_UNSUPPORTED;
	}

	case EAPI_ID_BOARD_LIB_VERSION_VAL: {
		if (KEApiGetLibVersion(&keapiVersion) == KEAPI_RET_SUCCESS) {
			*pValue = keapiVersion.version;
			return EAPI_STATUS_SUCCESS;
		} else
			return EAPI_STATUS_ERROR;
	}

	case EAPI_ID_HWMON_CPU_TEMP: {
		if (KEApiGetTempSensorCount(&sens_count) != KEAPI_RET_SUCCESS)
			return EAPI_STATUS_READ_ERROR;

		for (i = 0; i < sens_count; i++) {
			if (KEApiGetTempSensorInfo(i, &sensorInfo) != KEAPI_RET_SUCCESS)
				return EAPI_STATUS_READ_ERROR;

			if (sensorInfo.type != KEAPI_TEMP_CPU)
				continue;

			if (KEApiGetTempSensorValue(i, &sensorValue) != KEAPI_RET_SUCCESS)
				return EAPI_STATUS_READ_ERROR;

			*pValue = sensorValue.value;
			return EAPI_STATUS_SUCCESS;
		}

		return EAPI_STATUS_READ_ERROR;
	}

	case EAPI_ID_HWMON_CHIPSET_TEMP: {
		if (KEApiGetTempSensorCount(&sens_count) != KEAPI_RET_SUCCESS)
			return EAPI_STATUS_READ_ERROR;

		for (i = 0; i < sens_count; i++) {
			if (KEApiGetTempSensorInfo(i, &sensorInfo) != KEAPI_RET_SUCCESS)
				return EAPI_STATUS_READ_ERROR;

			if (sensorInfo.type != KEAPI_TEMP_CHIPSET)
				continue;

			if (KEApiGetTempSensorValue(i, &sensorValue) != KEAPI_RET_SUCCESS)
				return EAPI_STATUS_READ_ERROR;

			*pValue = sensorValue.value;
			return EAPI_STATUS_SUCCESS;
		}

		return EAPI_STATUS_READ_ERROR;
	}

	case EAPI_ID_HWMON_SYSTEM_TEMP: {
		if (KEApiGetTempSensorCount(&sens_count) != KEAPI_RET_SUCCESS)
			return EAPI_STATUS_READ_ERROR;

		for (i = 0; i < sens_count; i++) {
			if (KEApiGetTempSensorInfo(i, &sensorInfo) != KEAPI_RET_SUCCESS)
				return EAPI_STATUS_READ_ERROR;

			if (sensorInfo.type != KEAPI_TEMP_BOARD)
				continue;

			if (KEApiGetTempSensorValue(i, &sensorValue) != KEAPI_RET_SUCCESS)
				return EAPI_STATUS_READ_ERROR;

			*pValue = sensorValue.value;
			return EAPI_STATUS_SUCCESS;
		}

		return EAPI_STATUS_READ_ERROR;
	}

	case EAPI_ID_HWMON_VOLTAGE_VCORE: {
		if (KEApiGetVoltageSensorCount(&sens_count) != KEAPI_RET_SUCCESS)
			return EAPI_STATUS_READ_ERROR;

		for (i = 0; i < sens_count; i++) {
			if (KEApiGetVoltageSensorInfo(i, &sensorInfo) != KEAPI_RET_SUCCESS)
				return EAPI_STATUS_READ_ERROR;

			if (sensorInfo.type != KEAPI_VOLTAGE_VCORE)
				continue;

			if (KEApiGetVoltageSensorValue(i, &sensorValue) != KEAPI_RET_SUCCESS)
				return EAPI_STATUS_READ_ERROR;

			*pValue = sensorValue.value;
			return EAPI_STATUS_SUCCESS;
		}

		return EAPI_STATUS_READ_ERROR;
	}
	case EAPI_ID_HWMON_VOLTAGE_2V5: {
		if (KEApiGetVoltageSensorCount(&sens_count) != KEAPI_RET_SUCCESS)
			return EAPI_STATUS_READ_ERROR;

		for (i = 0; i < sens_count; i++) {
			if (KEApiGetVoltageSensorInfo(i, &sensorInfo) != KEAPI_RET_SUCCESS)
				return EAPI_STATUS_READ_ERROR;

			if (sensorInfo.type != KEAPI_VOLTAGE_2V5)
				continue;

			if (KEApiGetVoltageSensorValue(i, &sensorValue) != KEAPI_RET_SUCCESS)
				return EAPI_STATUS_READ_ERROR;

			*pValue = sensorValue.value;
			return EAPI_STATUS_SUCCESS;
		}

		return EAPI_STATUS_READ_ERROR;
	}

	case EAPI_ID_HWMON_VOLTAGE_3V3: {
		if (KEApiGetVoltageSensorCount(&sens_count) != KEAPI_RET_SUCCESS)
			return EAPI_STATUS_READ_ERROR;

		for (i = 0; i < sens_count; i++) {
			if (KEApiGetVoltageSensorInfo(i, &sensorInfo) != KEAPI_RET_SUCCESS)
				return EAPI_STATUS_READ_ERROR;

			if (sensorInfo.type != KEAPI_VOLTAGE_3V3)
				continue;

			if (KEApiGetVoltageSensorValue(i, &sensorValue) != KEAPI_RET_SUCCESS)
				return EAPI_STATUS_READ_ERROR;

			*pValue = sensorValue.value;
			return EAPI_STATUS_SUCCESS;
		}

		return EAPI_STATUS_READ_ERROR;
	}

	case EAPI_ID_HWMON_VOLTAGE_VBAT: {
		if (KEApiGetVoltageSensorCount(&sens_count) != KEAPI_RET_SUCCESS)
			return EAPI_STATUS_READ_ERROR;

		for (i = 0; i < sens_count; i++) {
			if (KEApiGetVoltageSensorInfo(i, &sensorInfo) != KEAPI_RET_SUCCESS)
				return EAPI_STATUS_READ_ERROR;

			if (sensorInfo.type != KEAPI_VOLTAGE_VBAT)
				continue;

			if (KEApiGetVoltageSensorValue(i, &sensorValue) != KEAPI_RET_SUCCESS)
				return EAPI_STATUS_READ_ERROR;

			*pValue = sensorValue.value;
			return EAPI_STATUS_SUCCESS;
		}

		return EAPI_STATUS_READ_ERROR;
	}

	case EAPI_ID_HWMON_VOLTAGE_5V: {
		if (KEApiGetVoltageSensorCount(&sens_count) != KEAPI_RET_SUCCESS)
			return EAPI_STATUS_READ_ERROR;

		for (i = 0; i < sens_count; i++) {
			if (KEApiGetVoltageSensorInfo(i, &sensorInfo) != KEAPI_RET_SUCCESS)
				return EAPI_STATUS_READ_ERROR;

			if (sensorInfo.type != KEAPI_VOLTAGE_5V)
				continue;

			if (KEApiGetVoltageSensorValue(i, &sensorValue) != KEAPI_RET_SUCCESS)
				return EAPI_STATUS_READ_ERROR;

			*pValue = sensorValue.value;
			return EAPI_STATUS_SUCCESS;
		}

		return EAPI_STATUS_READ_ERROR;
	}

	case EAPI_ID_HWMON_VOLTAGE_5VSB: {
		if (KEApiGetVoltageSensorCount(&sens_count) != KEAPI_RET_SUCCESS)
			return EAPI_STATUS_READ_ERROR;

		for (i = 0; i < sens_count; i++) {
			if (KEApiGetVoltageSensorInfo(i, &sensorInfo) != KEAPI_RET_SUCCESS)
				return EAPI_STATUS_READ_ERROR;

			if (sensorInfo.type != KEAPI_VOLTAGE_5VSB)
				continue;

			if (KEApiGetVoltageSensorValue(i, &sensorValue) != KEAPI_RET_SUCCESS)
				return EAPI_STATUS_READ_ERROR;

			*pValue = sensorValue.value;
			return EAPI_STATUS_SUCCESS;
		}

		return EAPI_STATUS_READ_ERROR;
	}

	case EAPI_ID_HWMON_VOLTAGE_12V: {
		if (KEApiGetVoltageSensorCount(&sens_count) != KEAPI_RET_SUCCESS)
			return EAPI_STATUS_READ_ERROR;

		for (i = 0; i < sens_count; i++) {
			if (KEApiGetVoltageSensorInfo(i, &sensorInfo) != KEAPI_RET_SUCCESS)
				return EAPI_STATUS_READ_ERROR;

			if (sensorInfo.type != KEAPI_VOLTAGE_12V)
				continue;

			if (KEApiGetVoltageSensorValue(i, &sensorValue) != KEAPI_RET_SUCCESS)
				return EAPI_STATUS_READ_ERROR;

			*pValue = sensorValue.value;
			return EAPI_STATUS_SUCCESS;
		}

		return EAPI_STATUS_READ_ERROR;
	}

	case EAPI_ID_HWMON_FAN_CPU: {
		if (KEApiGetFanSensorCount(&sens_count) != KEAPI_RET_SUCCESS)
			return EAPI_STATUS_READ_ERROR;

		for (i = 0; i < sens_count; i++) {
			if (KEApiGetFanSensorInfo(i, &sensorInfo) != KEAPI_RET_SUCCESS)
				return EAPI_STATUS_READ_ERROR;

			if (sensorInfo.type != KEAPI_FAN_CPU)
				continue;

			if (KEApiGetFanSensorValue(i, &sensorValue) != KEAPI_RET_SUCCESS)
				return EAPI_STATUS_READ_ERROR;

			*pValue = sensorValue.value;
			return EAPI_STATUS_SUCCESS;
		}

		return EAPI_STATUS_READ_ERROR;
	}

	case EAPI_ID_HWMON_FAN_SYSTEM: {
		if (KEApiGetFanSensorCount(&sens_count) != KEAPI_RET_SUCCESS)
			return EAPI_STATUS_READ_ERROR;

		for (i = 0; i < sens_count; i++) {
			if (KEApiGetFanSensorInfo(i, &sensorInfo) != KEAPI_RET_SUCCESS)
				return EAPI_STATUS_READ_ERROR;

			if (sensorInfo.type != KEAPI_FAN_BOX)
				continue;

			if (KEApiGetFanSensorValue(i, &sensorValue) != KEAPI_RET_SUCCESS)
				return EAPI_STATUS_READ_ERROR;

			*pValue = sensorValue.value;
			return EAPI_STATUS_SUCCESS;
		}

		return EAPI_STATUS_READ_ERROR;
	}

	default:
		return EAPI_STATUS_UNSUPPORTED;
	}
}

/*******************************************************************************/
EApiStatus_t EAPI_CALLTYPE EApiVgaGetBacklightEnable(__IN EApiId_t Id, /* Backlight Id */
						     __OUT uint32_t *pEnable /* Backlight Enable */
)
{
	uint32_t res;
	int32_t dev_cnt, displayNr = (int32_t)Id;
	int32_t value = -1;

	if (pEnable == NULL)
		return EAPI_STATUS_INVALID_PARAMETER;

	if ((res = KEApiGetDisplayCount(&dev_cnt)) != KEAPI_RET_SUCCESS)
		return convertErrorCodes(res);

	if (displayNr < 0 || displayNr >= dev_cnt)
		return EAPI_STATUS_UNSUPPORTED;

	if ((res = KEApiGetBacklightValue(displayNr, &value)) != KEAPI_RET_SUCCESS)
		return convertErrorCodes(res);

	if (value > 0)
		*pEnable = EAPI_BACKLIGHT_SET_ON;
	else
		*pEnable = EAPI_BACKLIGHT_SET_OFF;

	return EAPI_STATUS_SUCCESS;
}

/*******************************************************************************/
EApiStatus_t EAPI_CALLTYPE EApiVgaSetBacklightEnable(__IN EApiId_t Id, /* Backlight Id */
						     __IN uint32_t Enable /* Backlight Enable */
)
{
	uint32_t res;
	int32_t dev_cnt, displayNr = (int32_t)Id;
	int32_t value = -1;

	if ((res = KEApiGetDisplayCount(&dev_cnt)) != KEAPI_RET_SUCCESS)
		return convertErrorCodes(res);

	if (displayNr < 0 || displayNr >= dev_cnt)
		return EAPI_STATUS_UNSUPPORTED;

	if (Enable == EAPI_BACKLIGHT_SET_ON)
		value = KEAPI_DISPLAY_BRIGHTNESS_MAX;
	else if (Enable == EAPI_BACKLIGHT_SET_OFF)
		value = 0;
	else
		return EAPI_STATUS_INVALID_PARAMETER;

	if ((res = KEApiSetBacklightValue(displayNr, value)) != KEAPI_RET_SUCCESS)
		return convertErrorCodes(res);

	return EAPI_STATUS_SUCCESS;
}

/*******************************************************************************/
EApiStatus_t EAPI_CALLTYPE EApiVgaGetBacklightBrightness(__IN EApiId_t Id, /* Backlight Id */
							 __OUT uint32_t *pBright /* Backlight Brightness */
)
{
	uint32_t res;
	int32_t backlight;

	/* Check function parameters */
	if (pBright == NULL)
		return EAPI_STATUS_INVALID_PARAMETER;

	if ((res = KEApiGetBacklightValue(Id, &backlight)) == KEAPI_RET_SUCCESS) {
		*pBright = (backlight * 255) / KEAPI_DISPLAY_BRIGHTNESS_MAX;
		return EAPI_STATUS_SUCCESS;
	} else
		return convertErrorCodes(res);
}

/*******************************************************************************/
EApiStatus_t EAPI_CALLTYPE EApiVgaSetBacklightBrightness(__IN EApiId_t Id, /* Backlight Id */
							 __IN uint32_t Bright /* Backlight Brightness */
)
{
	uint32_t res;

	/* Check function parameters */
	if (Bright > 255)
		return EAPI_STATUS_INVALID_PARAMETER;

	if ((res = KEApiSetBacklightValue(Id, (Bright * KEAPI_DISPLAY_BRIGHTNESS_MAX) / 255)) == KEAPI_RET_SUCCESS) {
		return EAPI_STATUS_SUCCESS;
	} else
		return convertErrorCodes(res);
}

/*******************************************************************************/
EApiStatus_t EAPI_CALLTYPE EApiStorageCap(__IN EApiId_t Id, /* Storage Area Id */
					  __OUT uint32_t *pStorageSize, /* Total */
					  __OUT uint32_t *pBlockLength /* Write Block Length & Alignment */
)
{
	uint32_t res;
	int32_t storageSize;

	/* Check function parameters */
	if (pStorageSize == NULL || pBlockLength == NULL)
		return EAPI_STATUS_INVALID_PARAMETER;

	if ((res = KEApiGetStorageSize(Id, &storageSize)) == KEAPI_RET_SUCCESS) {
		*pStorageSize = storageSize;
		*pBlockLength = storageSize;
		return EAPI_STATUS_SUCCESS;
	}
	return convertErrorCodes(res);
}

/*******************************************************************************/
EApiStatus_t EAPI_CALLTYPE EApiStorageAreaRead(__IN EApiId_t Id, /* Storage Area Id */
					       __IN uint32_t offset, /* Byte Offset */
					       __OUT void *pBuffer, /* Pointer to Data pBuffer */
					       __IN uint32_t buffLen, /* Data pBuffer Size in bytes */
					       __IN uint32_t byteCnt /* Number of bytes to read */
)
{
	uint32_t res;

	/* Check function parameters */
	if (pBuffer == NULL)
		return EAPI_STATUS_INVALID_PARAMETER;

	res = KEApiStorageRead(Id, offset, (uint8_t *)pBuffer, byteCnt);
	return convertErrorCodes(res);
}

/*******************************************************************************/
EApiStatus_t EAPI_CALLTYPE EApiStorageAreaWrite(__IN EApiId_t Id, /* Storage Area Id */
						__IN uint32_t offset, /* Byte Offset */
						__IN void *pBuffer, /* Pointer to Data pBuffer */
						__IN uint32_t byteCnt /* Number of bytes to write */
)
{
	uint32_t res;

	/* Check function parameters */
	if (pBuffer == NULL)
		return EAPI_STATUS_INVALID_PARAMETER;

	res = KEApiStorageWrite(Id, offset, (uint8_t *)pBuffer, byteCnt);
	return convertErrorCodes(res);
}

/*******************************************************************************/
EApiStatus_t EAPI_CALLTYPE EApiI2CGetBusCap(__IN EApiId_t Id, /* I2C Bus Id */
					    __OUT uint32_t *pMaxBlkLen /* Max Block Length supported on this interface */
)
{
	if (pMaxBlkLen == NULL)
		return EAPI_STATUS_INVALID_PARAMETER;

	*pMaxBlkLen = 0x20; /* FIXME put maximum for LVDS utinl KEAPIv2 */
	return EAPI_STATUS_SUCCESS;
}

/*******************************************************************************/
EApiStatus_t EAPI_CALLTYPE EApiI2CWriteReadRaw(__IN EApiId_t Id, /* I2C Bus Id */
					       __IN uint8_t Addr, /* Encoded 7Bit I2C Device Address */
					       __INOPT void *pWBuffer, /* Write Data pBuffer */
					       __IN uint32_t WriteBCnt, /* Number of Bytes to write plus 1 */
					       __OUTOPT void *pRBuffer, /* Read Data pBuffer */
					       __IN uint32_t RBufLen, /* Data pBuffer Length */
					       __IN uint32_t ReadBCnt /* Number of Bytes to Read plus 1 */
)
{
	uint32_t res, rdLen, wrLen;

	/* Check function parameters */
	if (pWBuffer == NULL && pRBuffer == NULL)
		return EAPI_STATUS_INVALID_PARAMETER;

	if ((pRBuffer != NULL && ReadBCnt < 2) || (pRBuffer == NULL && ReadBCnt > 1))
		return EAPI_STATUS_INVALID_PARAMETER;

	if ((pWBuffer != NULL && WriteBCnt < 2) || (pWBuffer == NULL && WriteBCnt > 1))
		return EAPI_STATUS_INVALID_PARAMETER;

	if ((ReadBCnt < 2) && (WriteBCnt < 2))
		return EAPI_STATUS_INVALID_PARAMETER;

	if (ReadBCnt > (RBufLen + 1))
		return EAPI_STATUS_MORE_DATA;

	rdLen = (pRBuffer == NULL) ? 0 : ReadBCnt - 1;
	wrLen = (pWBuffer == NULL) ? 0 : WriteBCnt - 1;

	res = KEApiI2cXfer(Id, EAPI_I2C_DEC_7BIT_ADDR(Addr), pWBuffer, wrLen, pRBuffer, (int32_t *)&rdLen);

	return convertErrorCodes(res);
}

/*******************************************************************************/
EApiStatus_t EAPI_CALLTYPE EApiI2CReadTransfer(__IN EApiId_t Id, /* I2C Bus Id */
					       __IN uint32_t Addr, /* Encoded 7/10Bit I2C Device Address */
					       __IN uint32_t Cmd, /* I2C Command/Offset */
					       __OUT void *pBuffer, /* Transfer Data pBuffer */
					       __IN uint32_t BufLen, /* Data pBuffer Length */
					       __IN uint32_t ByteCnt /* Byte Count to read */
)
{
	uint32_t res, rdLen = ByteCnt;
	uint32_t wrLen = 0;
	uint8_t wrBuf[2];

	if ((pBuffer != NULL && ByteCnt < 1) || (pBuffer == NULL && ByteCnt > 0))
		return EAPI_STATUS_INVALID_PARAMETER;

	if (BufLen < ByteCnt)
		return EAPI_STATUS_MORE_DATA;

	if (!EAPI_I2C_IS_NO_CMD(Cmd)) {
		if (EAPI_I2C_IS_EXT_CMD(Cmd))
			wrBuf[wrLen++] = (uint8_t)((Cmd >> 0x08) & 0xFF);

		wrBuf[wrLen++] = (uint8_t)(Cmd & 0xFF);
	}

	res = KEApiI2cXfer(Id, EAPI_I2C_DEC_7BIT_ADDR(Addr), wrBuf, wrLen, pBuffer, (int32_t *)&rdLen);

	return convertErrorCodes(res);
}

/*******************************************************************************/
EApiStatus_t EAPI_CALLTYPE EApiI2CWriteTransfer(__IN EApiId_t Id, /* I2C Bus Id */
						__IN uint32_t Addr, /* Encoded 7/10Bit I2C Device Address */
						__IN uint32_t Cmd, /* I2C Command/Offset */
						__IN void *pBuffer, /* Transfer Data pBuffer */
						__IN uint32_t ByteCnt /* Byte Count to write */
)
{
	uint32_t res, i, wrLen, rdLen = 0;
	uint8_t *wrBuf, cmdCnt = 0;

	if (pBuffer == NULL || ByteCnt == 0)
		return EAPI_STATUS_INVALID_PARAMETER;

	if ((pBuffer != NULL && ByteCnt < 1) || (pBuffer == NULL && ByteCnt > 0))
		return EAPI_STATUS_INVALID_PARAMETER;

	if (EAPI_I2C_IS_STD_CMD(Cmd))
		wrLen = ByteCnt + 1;
	else if (EAPI_I2C_IS_EXT_CMD(Cmd))
		wrLen = ByteCnt + 2;
	else
		wrLen = ByteCnt;

	wrBuf = malloc(wrLen);

	if (!EAPI_I2C_IS_NO_CMD(Cmd)) {
		if (EAPI_I2C_IS_EXT_CMD(Cmd))
			wrBuf[cmdCnt++] = (uint8_t)((Cmd >> 0x08) & 0xFF);

		wrBuf[cmdCnt++] = (uint8_t)(Cmd & 0xFF);
	}

	for (i = 0; i < ByteCnt; i++)
		wrBuf[cmdCnt + i] = ((uint8_t *)pBuffer)[i];

	res = KEApiI2cXfer(Id, EAPI_I2C_DEC_7BIT_ADDR(Addr), wrBuf, wrLen, NULL, (int32_t *)&rdLen);
	free(wrBuf);

	return convertErrorCodes(res);
}

/*******************************************************************************/
EApiStatus_t EAPI_CALLTYPE EApiI2CProbeDevice(__IN EApiId_t Id, /* I2C Bus Id */
					      __IN uint32_t Addr /* Encoded 7/10Bit I2C Device Address */
)
{
	EApiStatus_t StatusCode;
	uint32_t res;

	res = KEApiI2cProbe((uint8_t)Id, EAPI_I2C_DEC_7BIT_ADDR((uint8_t)Addr), 0, 0);
	StatusCode = convertErrorCodes(res);

	if (StatusCode == EAPI_STATUS_WRITE_ERROR)
		StatusCode = EAPI_STATUS_NOT_FOUND;

	return StatusCode;
}

/*******************************************************************************/
EApiStatus_t EAPI_CALLTYPE EApiWDogGetCap(__OUTOPT uint32_t *pMaxDelay, /* Maximum Supported Delay in milliseconds */
					  __OUTOPT uint32_t *pMaxEventTimeout, /* Maximum Supported
										* Event Timeout in milliseconds
										* 0 == Unsupported */
					  __OUTOPT uint32_t *pMaxResetTimeout /* Maximum Supported
									       * Reset Timeout in milliseconds */
)
{
	uint32_t res;
	int32_t maxTimeout, minTimeout, stageCnt;

	if (pMaxDelay == NULL || pMaxEventTimeout == NULL || pMaxResetTimeout == NULL)
		return EAPI_STATUS_INVALID_PARAMETER;

	if ((res = KEApiWatchdogGetCaps(&maxTimeout, &minTimeout, &stageCnt)) != KEAPI_RET_SUCCESS)
		return convertErrorCodes(res);

	*pMaxDelay = 0;
	*pMaxEventTimeout = *pMaxResetTimeout = maxTimeout;

	return EAPI_STATUS_SUCCESS;
}

/*******************************************************************************/
EApiStatus_t EAPI_CALLTYPE EApiWDogStart(__IN uint32_t Delay, /* Delay in milliseconds */
					 __IN uint32_t EventTimeout, /* Event Timeout in milliseconds */
					 __IN uint32_t ResetTimeout /* Reset Timeout in milliseconds */
)
{
	uint32_t res;
	int32_t stageCnt = 0;

	/* Check function parameters */
	if (Delay > 0)
		return EAPI_STATUS_INVALID_PARAMETER;

	if (EventTimeout > 0) {
		if ((res = KEApiWatchdogSetup(stageCnt, EventTimeout, KEAPI_WD_MODE_INTERRUPT)) != KEAPI_RET_SUCCESS)
			return convertErrorCodes(res);

		stageCnt++;
	}

	if (ResetTimeout > 0) {
		if ((res = KEApiWatchdogSetup(stageCnt, ResetTimeout, KEAPI_WD_MODE_RESET)) != KEAPI_RET_SUCCESS)
			return convertErrorCodes(res);
	}

	return EAPI_STATUS_SUCCESS;
}

/*******************************************************************************/
EApiStatus_t EAPI_CALLTYPE EApiWDogTrigger(void)
{
	uint32_t res;
	res = KEApiWatchdogTrigger();
	return convertErrorCodes(res);
}

/*******************************************************************************/
EApiStatus_t EAPI_CALLTYPE EApiWDogStop(void)
{
	uint32_t res;
	res = KEApiWatchdogDisable();
	return convertErrorCodes(res);
}

/*******************************************************************************/
#define EAPI_ID_GPIO_BITMASK00 (8)

inline EApiStatus_t convertErrorCodesGpio(EApiStatus_t ret)
{
	/* here error may be only if port is invalid, so return according spec. */
	if (ret == KEAPI_RET_PARAM_ERROR)
		return EAPI_STATUS_UNSUPPORTED;
	else
		return convertErrorCodes(ret);
}

/*******************************************************************************/
EApiStatus_t EAPI_CALLTYPE EApiGPIOGetDirectionCaps(__IN EApiId_t Id, /* GPIO Id */
						    __OUTOPT uint32_t *pInputs, /* Supported GPIO Input Bit Mask */
						    __OUTOPT uint32_t *pOutputs /* Supported GPIO Output Bit Mask */
)
{
	uint32_t res;
	uint32_t isInput, isOutput;
	int32_t port;

	/* Check function parameters */
	if (pInputs == NULL || pOutputs == NULL)
		return EAPI_STATUS_INVALID_PARAMETER;

	/* Initialization */
	*pInputs = 0;
	*pOutputs = 0;

	if (Id < EAPI_ID_GPIO_BITMASK00) { /* Single bit (gpio) addressing (only for port 0)*/

		port = 0;

		if ((res = KEApiGetGpioPortDirectionCaps(port, &isInput, &isOutput)) != KEAPI_RET_SUCCESS)
			return convertErrorCodesGpio(res);

		*pInputs = ((isInput >> Id) & 0x0001) ? 1 : 0;
		*pOutputs = ((isInput >> Id) & 0x0001) ? 1 : 0;

	} else if (Id & EAPI_ID_GPIO_BANK00) { /* Bitmask and port (bank) addressing
						* 0x10000 bit indicate that we have a deal with gpio bank */
		port = Id & 0x0000FFFF; // take first 16 bit

		if ((res = KEApiGetGpioPortDirectionCaps(port, &isInput, &isOutput)) != KEAPI_RET_SUCCESS)
			return convertErrorCodesGpio(res);

		*pInputs |= isInput;
		*pOutputs |= isOutput;
	} else
		return EAPI_STATUS_UNSUPPORTED;

	return EAPI_STATUS_SUCCESS;
}

/*******************************************************************************/
EApiStatus_t EAPI_CALLTYPE EApiGPIOGetDirection(__IN EApiId_t Id, /* GPIO Id */
						__IN uint32_t Bitmask, /* Bit mask of Affected Bits */
						__OUT uint32_t *pDirection /* Current Direction */
)
{
	uint32_t res;
	uint32_t pindir;
	int32_t port;

	if (Bitmask == 0)
		return EAPI_STATUS_INVALID_PARAMETER;

	/* Check function parameters */
	if (pDirection == NULL)
		return EAPI_STATUS_INVALID_PARAMETER;

	/* Initialization */
	*pDirection = 0;

	if (Id < EAPI_ID_GPIO_BITMASK00) { /* Single bit (gpio) addressing (only for port 0)*/

		port = 0;

		if (Bitmask != 1)
			return EAPI_STATUS_INVALID_BITMASK;

		if ((res = KEApiGetGpioPortDirections(port, &pindir)) != KEAPI_RET_SUCCESS)
			return convertErrorCodesGpio(res);

		*pDirection = (pindir >> Id) & 0x0001;

	} else if (Id & EAPI_ID_GPIO_BANK00) { /* Bitmask and port (bank) addressing
						* 0x10000 bit indicate that we have a deal with gpio bank */

		port = Id & 0x0000FFFF; // take first 16 bit

		if ((res = KEApiGetGpioPortDirections(port, &pindir)) != KEAPI_RET_SUCCESS)
			return convertErrorCodesGpio(res);

		*pDirection |= (pindir & Bitmask);
	} else
		return EAPI_STATUS_UNSUPPORTED;

	return EAPI_STATUS_SUCCESS;
}

/*******************************************************************************/
EApiStatus_t EAPI_CALLTYPE EApiGPIOSetDirection(__IN EApiId_t Id, /* GPIO Id */
						__IN uint32_t Bitmask, /* Bit mask of Affected Bits */
						__IN uint32_t Direction /* Direction */
)
{
	uint32_t res;
	uint32_t new_dir, orig_dir;
	int32_t port = 0;

	if (Bitmask == 0)
		return EAPI_STATUS_INVALID_PARAMETER;

	if (Id < EAPI_ID_GPIO_BITMASK00) { /* Single bit (gpio) addressing (only for port 0)*/

		port = 0;

		if (Bitmask != 1)
			return EAPI_STATUS_INVALID_BITMASK;

		if ((res = KEApiGetGpioPortDirections(port, &orig_dir)) != KEAPI_RET_SUCCESS)
			return convertErrorCodes(res);

		if (Direction & 0x0001)
			new_dir = orig_dir | (0x0001 << Id);
		else
			new_dir = orig_dir & (~(0x0001 << Id));

	} else if (Id & EAPI_ID_GPIO_BANK00) { /* Bitmask and port (bank) addressing */

		port = Id & 0x0000FFFF; // take first 16 bit

		if ((res = KEApiGetGpioPortDirections(port, &orig_dir)) != KEAPI_RET_SUCCESS)
			return convertErrorCodesGpio(res);

		new_dir = (Direction & Bitmask) | (orig_dir & (~Bitmask));
	} else
		return EAPI_STATUS_UNSUPPORTED;

	if ((res = KEApiSetGpioPortDirections(port, new_dir)) != KEAPI_RET_SUCCESS)
		return convertErrorCodesGpio(res);

	return EAPI_STATUS_SUCCESS;
}

/*******************************************************************************/
EApiStatus_t EAPI_CALLTYPE EApiGPIOGetLevel(__IN EApiId_t Id, /* GPIO Id */
					    __IN uint32_t Bitmask, /* Bit mask of Affected Bits */
					    __OUT uint32_t *pLevel /* Current Level */
)
{
	uint32_t res;
	uint32_t levels;
	int32_t port;

	if (Bitmask == 0)
		return EAPI_STATUS_INVALID_PARAMETER;

	/* Check function parameters */
	if (pLevel == NULL)
		return EAPI_STATUS_INVALID_PARAMETER;

	/* Initialization */
	*pLevel = 0;

	if (Id < EAPI_ID_GPIO_BITMASK00) {
		port = 0;

		if ((res = KEApiGetGpioPortLevels(port, &levels)) != KEAPI_RET_SUCCESS)
			return convertErrorCodesGpio(res);

		if (Bitmask != 1)
			return EAPI_STATUS_INVALID_BITMASK;

		*pLevel = (levels >> Id) & 0x0001;

	} else if (Id & EAPI_ID_GPIO_BANK00) {
		port = Id & 0x0000FFFF;

		if ((res = KEApiGetGpioPortLevels(port, &levels)) != KEAPI_RET_SUCCESS)
			return convertErrorCodesGpio(res);

		*pLevel |= (levels & Bitmask);

	} else
		return EAPI_STATUS_UNSUPPORTED;

	return EAPI_STATUS_SUCCESS;
}

/*******************************************************************************/
EApiStatus_t EAPI_CALLTYPE EApiGPIOSetLevel(__IN EApiId_t Id, /* GPIO Id */
					    __IN uint32_t Bitmask, /* Bit mask of Affected Bits */
					    __IN uint32_t Level /* Level */
)
{
	uint32_t res;
	uint32_t orig_lvls, new_lvls;
	int32_t port;

	if (Bitmask == 0)
		return EAPI_STATUS_INVALID_PARAMETER;

	if (Id < EAPI_ID_GPIO_BITMASK00) {
		port = 0;

		if (Bitmask != 1)
			return EAPI_STATUS_INVALID_BITMASK;

		if ((res = KEApiGetGpioPortLevels(port, &orig_lvls)) != KEAPI_RET_SUCCESS)
			return convertErrorCodesGpio(res);

		if (Level & 0x0001)
			new_lvls = orig_lvls | (0x0001 << Id);
		else
			new_lvls = orig_lvls & (~(0x0001 << Id));

	} else if (Id & EAPI_ID_GPIO_BANK00) {
		port = Id & 0x0000FFFF;

		if ((res = KEApiGetGpioPortLevels(port, &orig_lvls)) != KEAPI_RET_SUCCESS)
			return convertErrorCodesGpio(res);

		new_lvls = (Level & Bitmask) | (orig_lvls & (~Bitmask));

	} else
		return EAPI_STATUS_UNSUPPORTED;

	if ((res = KEApiSetGpioPortLevels(port, new_lvls)) != KEAPI_RET_SUCCESS)
		return convertErrorCodesGpio(res);

	return EAPI_STATUS_SUCCESS;
}
