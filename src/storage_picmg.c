/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron AG */

/* PICMG functions helpers. */

#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "eapi/EeeP.h"
#include "eapi/COM0EEP.h"
#include "storage_picmg.h"

#ifdef __VXWORKS__
#include "versionK.h"
KONTRON_VERSION(storage_picmg_c, 01b) /* version identifier */
#endif /* __VXWORKS__ */

int8_t EepromIsPicmg(char *eep_data)
{
	if (!memcmp(&eep_data[0x01], "3P", 0x02))
		return TRUE;
	else
		return FALSE;
}

int8_t EepromIsOnModule(char *eep_data)
{
	if (!memcmp(&eep_data[0x06], "coM0", 0x04) || !memcmp(&eep_data[0x06], "smA0", 0x04))
		return TRUE;
	else
		return FALSE;
}

int8_t EepromIsOnCarrier(char *eep_data)
{
	if (!memcmp(&eep_data[0x06], "Com0", 0x04) || !memcmp(&eep_data[0x06], "Sma0", 0x04))
		return TRUE;
	else
		return FALSE;
}

int8_t EepromIsOnExpansion(char *eep_data)
{
	if (!memcmp(&eep_data[0x06], "EXP1", 0x04))
		return TRUE;
	else
		return FALSE;
}

/* Returns size of eeprom in bytes */
int32_t EepromGetSize(char *eep_data)
{
	return 0x100 << (eep_data[0x05] & EEEP_DEVICE_SIZE_MASK);
}

/* Returns Page Write Length in bytes */
int32_t EepromGetPageWriteSize(char *eep_data)
{
	char type = (eep_data[0x05] & EEEP_DEVICE_WRITE_LEN_MASK) >> EEEP_DEVICE_WRITE_LEN_OFFSET;

	switch (type) {
	case 0:
		return 1;
	case 1:
		return 8;
	case 2:
		return 16;
	case 3:
		return 32;
	case 4:
		return 64;
	case 5:
		return 128;
	case 6:
		return 256;
	case 7:
		return 512;
	default:
		return -1;
	}
}

int8_t EepromUsesExtendedIndex(char *eep_data)
{
	if (eep_data[0x05] & EEEP_DEVICE_EXT_INDEX)
		return TRUE;
	else
		return FALSE;
}

/* Absolute Offset to First Dynamic Block in bytes */
int16_t GetOffsetToFirstDBlock(char *eep_data)
{
	return eep_data[0x04] * 2;
}

int16_t GetVendorDevicePndId(char *eep_data)
{
	if (EepromIsOnModule(eep_data))
		return (eep_data[0x0A + 0x00] << 8) | eep_data[0x0A + 0x01];

	if (EepromIsOnCarrier(eep_data))
		return (eep_data[0x0C + 0x00] << 8) | eep_data[0x0C + 0x01];

	if (EepromIsOnExpansion(eep_data))
		return (eep_data[0x06 + 0x00] << 8) | eep_data[0x06 + 0x01];

	return -1;
}

int16_t DecodeVendorDevicePndId(int16_t pnd_num, char *pnd_str)
{
	if (!(pnd_num < 0)) {
		pnd_str[0] = (char)((pnd_num & 0x7C) >> 2) + 0x41;
		pnd_str[1] = (char)(((pnd_num & 0x03) << 3) & ((pnd_num & 0xE000) >> 13)) + 0x41;
		pnd_str[2] = (char)((pnd_num & 0x1F00) >> 8) + 0x41;
		pnd_str[3] = '\0';

		return 0;
	}

	return -1;
}

int16_t GetVendorDeviceId(char *eep_data)
{
	if (EepromIsOnModule(eep_data))
		return (eep_data[0x0A + 0x02] << 8) | eep_data[0x0A + 0x03];

	if (EepromIsOnCarrier(eep_data))
		return (eep_data[0x0C + 0x02] << 8) | eep_data[0x0C + 0x03];

	if (EepromIsOnExpansion(eep_data))
		return (eep_data[0x06 + 0x02] << 8) | eep_data[0x06 + 0x03];

	return -1;
}

int16_t GetVendorDeviceFlavor(char *eep_data)
{
	if (EepromIsOnModule(eep_data))
		return eep_data[0x0A + 0x04];

	if (EepromIsOnCarrier(eep_data))
		return eep_data[0x0C + 0x04];

	if (EepromIsOnExpansion(eep_data))
		return eep_data[0x06 + 0x04];

	return -1;
}

int16_t GetVendorDeviceRevision(char *eep_data)
{
	if (EepromIsOnModule(eep_data))
		return eep_data[0x0A + 0x05];

	if (EepromIsOnCarrier(eep_data))
		return eep_data[0x0C + 0x05];

	if (EepromIsOnExpansion(eep_data))
		return eep_data[0x06 + 0x05];

	return -1;
}

/* On module and on carrier eeproms offset for "type" (of module and carrier
 * board) is the same. In Expansion devivece header there is not type field */
int16_t GetModuleOrCarrierType(char *eep_data)
{
	if (EepromIsOnModule(eep_data) || EepromIsOnCarrier(eep_data))
		return eep_data[0x10];

	return -1;
}

/* Returns Com Specification Revision as string in format "x.x"
 * (eg. 1.1 or 2.0) */
int16_t GetDecodedComSpecVersion(char *eep_data, char *vers_buf)
{
	if (EepromIsOnModule(eep_data) || EepromIsOnCarrier(eep_data)) {
		vers_buf[0] = ((eep_data[0x11] & 0xF0) >> 4) + 0x30;
		vers_buf[1] = '.';
		vers_buf[2] = (eep_data[0x11] & 0x0F) + 0x30;
		vers_buf[3] = '\0';

		return 0;
	}

	return -1;
}

int32_t GetDynamicBlockCount(char *eep_data)
{
	int16_t blen; /* Dynamic Block Lenght */
	int32_t bpos; /* Start offset of current block */
	int32_t count = 0; /* Blocks counter */

	bpos = GetOffsetToFirstDBlock(eep_data);
	while (1) {
		blen = ((eep_data[bpos + 1] << 8) | eep_data[bpos + 2]) * 2;
		if (blen == 0) {
			if (eep_data[bpos] == 0xFE) /* KEM specific CRC block */
				bpos = bpos + 0x06; /* Length of CRC block */
			else /* or eep_data[bpos] == 0x00 and it is end of list */
				break; /* or it is unknown situation */
		} else
			bpos = bpos + blen;

		count++;
	}

	return count;
}

/* We assume that user have already got BlockCount
 * and allocated enought memory for "struct dBlks *blocks"
 * which is transmitted to GetDynamicBlockData() */
int32_t GetDynamicBlockData(char *eep_data, struct BlockHead *blocks, int32_t count)
{
	int32_t bpos = GetOffsetToFirstDBlock(eep_data);
	int32_t i;

	for (i = 0; i < count; i++) {
		blocks[i].offset = bpos;
		blocks[i].id = eep_data[bpos];
		blocks[i].len = ((eep_data[bpos + 1] << 8) | eep_data[bpos + 2]) * 2;
		if (blocks[i].len == 0) {
			if (blocks[i].id == 0xFE) /* CRC block, length fields of crc block is 0 */
				blocks[i].len = 0x06; /* but in fact length is equal 6 */
			else /* or blocks[i].id == 0 and it is end of list */
				break; /* or it is unknown situation */
		}
		bpos = bpos + blocks[i].len;
	}

	return i;
}

/************************************************************************/

/* the block below is stolen from dmidecode with modifications */
const char *dmi_string(struct dmi_header *dm, char s)
{
	char *bp = (char *)dm->data;

	if (s == 0)
		return "Not Specified (0)";

	bp += dm->length;
	while (s > 1 && *bp) {
		bp += strlen(bp);
		bp++;
		s--;
	}

	if (!*bp)
		return "Not Specified (1)";

	return bp;
}

/************************************************************************/
void *TryToParsePicmgSmbiosHeader(char *data, unsigned char *type)
{
	data += 3; /* moving dat pointer data on 3 bytes where
		    * 1 byte is picmg dynamic block id and 2 bytes are its length */

	return TryToParseSmbiosHeader(data, type);
}

void *TryToParseSmbiosHeader(char *data, unsigned char *type)
{
	struct dmi_header hd;
	hd.type = data[0];
	hd.length = data[1];
	hd.handle = (data[2] << 8) | data[3];
	hd.data = data;

	switch (hd.type) {
	case DMI_T_BIOS: {
		struct BIOSInfo *biosInfo = calloc(1, sizeof(struct BIOSInfo));
		strncat(biosInfo->vendor, dmi_string(&hd, data[DMI_STR_BIOS_VENDOR]), KEAPI_MAX_STR - 1);
		strncat(biosInfo->version, dmi_string(&hd, data[DMI_STR_BIOS_VERSION]), KEAPI_MAX_STR - 1);
		strncat(biosInfo->release_date, dmi_string(&hd, data[DMI_STR_BIOS_RELEASE_DATE]), KEAPI_MAX_STR - 1);
		*type = hd.type;
		return biosInfo;
	}
	case DMI_T_HW: {
		struct HWInfo *hwInfo = calloc(1, sizeof(struct HWInfo));
		snprintf(hwInfo->manufacturer, KEAPI_MAX_STR, "%s", dmi_string(&hd, data[DMI_STR_HW_MANUFATURER]));
		snprintf(hwInfo->product, KEAPI_MAX_STR, "%s", dmi_string(&hd, data[DMI_STR_HW_PRDCT]));
		snprintf(hwInfo->version, KEAPI_MAX_STR, "%s", dmi_string(&hd, data[DMI_STR_HW_VERSION]));
		snprintf(hwInfo->serial, KEAPI_MAX_STR, "%s", dmi_string(&hd, data[DMI_STR_HW_SERIALNUM]));
		snprintf(hwInfo->assetTag, KEAPI_MAX_STR, "%s", dmi_string(&hd, data[DMI_STR_HW_ASSETTAG]));
		hwInfo->featureFlag = data[DMI_BF_HW_FEATURE_FLAG];
		snprintf(hwInfo->location, KEAPI_MAX_STR, "%s", dmi_string(&hd, data[DMI_STR_HW_LOCATION]));
		hwInfo->boardType = data[DMI_BT_HW_BOARD_TYPE];
		*type = hd.type;
		return hwInfo;
	}
	case DMI_T_KEM_EXT_BRD: {
		struct KemExtInfo *kemExtInfo = calloc(1, sizeof(struct KemExtInfo));
		memcpy(kemExtInfo->kemId, &data[DMI_4B_KEM_EXT_KEM_ID], 0x04);
		snprintf(kemExtInfo->projectName, KEAPI_MAX_STR, "%s", dmi_string(&hd, data[DMI_STR_KEM_EXT_PRJ_NM]));
		snprintf(kemExtInfo->manufacturerDate, KEAPI_MAX_STR, "%s",
			 dmi_string(&hd, data[DMI_STR_KEM_EXT_MNFCTRNG_DATE]));
		snprintf(kemExtInfo->repairDate, KEAPI_MAX_STR, "%s",
			 dmi_string(&hd, data[DMI_STR_KEM_EXT_REPAIR_DATE]));
		snprintf(kemExtInfo->patchLevel, KEAPI_MAX_STR, "%s", dmi_string(&hd, data[DMI_STR_KEM_EXT_PTCH_LVL]));
		*type = hd.type;
		return kemExtInfo;
	}
	case DMI_T_KEM_RT_DAT: {
		int i;
		struct KemRTDat *kemRTDat = calloc(1, sizeof(struct KemRTDat));
		memcpy(kemRTDat->kemId, &data[DMI_4B_KEM_RTDAT_KEM_ID], 0x04);
		for (i = 0; i < 0x08; i++) {
			kemRTDat->bootCounter = (kemRTDat->bootCounter << 8) | data[DMI_8B_KEM_RTDAT_BOOTCNT + i];
			kemRTDat->runningTimeMeter = (kemRTDat->runningTimeMeter << 8) | data[DMI_8B_KEM_RTDAT_RTM + i];
		}
		*type = hd.type;
		return kemRTDat;
	}
	case DMI_T_SYS: {
		struct SysInfo *sysInfo = calloc(1, sizeof(struct SysInfo));
		strncat(sysInfo->manufacturer, dmi_string(&hd, data[DMI_STR_SYS_MANUFACTURER]), KEAPI_MAX_STR - 1);
		strncat(sysInfo->product, dmi_string(&hd, data[DMI_STR_SYS_PRDCT_NM]), KEAPI_MAX_STR - 1);
		strncat(sysInfo->version, dmi_string(&hd, data[DMI_STR_SYS_VERSION]), KEAPI_MAX_STR - 1);
		strncat(sysInfo->serial, dmi_string(&hd, data[DMI_STR_SYS_SERIALNUM]), KEAPI_MAX_STR - 1);
		*type = hd.type;
		return sysInfo;
	}
	case DMI_T_KEM_BKL: /* to be filled */
	default:
		return NULL;
	}
}

struct EepUsr *TryToGetUserDataArea(char *eep_data, int32_t offset)
{
	if (eep_data[0x05] == 1 && eep_data[0x06 + 0x00] == '$' && eep_data[0x06 + 0x01] == 'U' &&
	    eep_data[0x06 + 0x02] == 'S' && eep_data[0x06 + 0x03] == 'R') {
		struct EepUsr *eepUsr = calloc(1, sizeof(struct EepUsr));
		eepUsr->start = offset + 0x0E;
		eepUsr->size = 2 * ((eep_data[0x01] << 8) | eep_data[0x02]) - 0x0E;
		return eepUsr;
	}
	return NULL;
}
