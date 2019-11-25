/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron AG */

/* PICMG functions helpers declaration. */

/*
 * For more information reference to:
 * - Embedded EEPROM Specification (rev 1.0)
 * - System Management BIOS (SMBIOS) Reference Specification (v 2.6.1)
 * - KEM EFI Extensions Specification (rev 1.06)
 */

#ifndef EEP_PICMG_H
#define EEP_PICMG_H

#ifdef __VXWORKS__
#include "versionK.h"
KONTRON_VERSION(storage_picmg_h, 01b) /* version identifier */
#endif /* __VXWORKS__ */

#include "keapi_inc.h"

int8_t EepromIsPicmg(char *eep_data);
int8_t EepromIsOnModule(char *eep_data);
int8_t EepromIsOnCarrier(char *eep_data);
int8_t EepromIsOnExpansion(char *eep_data);
/* Returns size of eeprom in bytes */
int32_t EepromGetSize(char *eep_data);
/* Returns Page Write Length in bytes */
int32_t EepromGetPageWriteSize(char *eep_data);
int8_t EepromUsesExtendedIndex(char *eep_data);
/* Absolute Offset to First Dynamic Block in bytes */
int16_t GetOffsetToFirstDBlock(char *eep_data);
int16_t GetVendorDevicePndId(char *eep_data);
int16_t DecodeVendorDevicePndId(int16_t pnd_num, char *pnd_str);
int16_t GetVendorDeviceId(char *eep_data);
int16_t GetVendorDeviceFlavor(char *eep_data);
int16_t GetVendorDeviceRevision(char *eep_data);
/* On module and on carrier eeproms offset for "type" (of module and carrier
 * board) is the same. In Expansion devivece header there is not type field */
int16_t GetModuleOrCarrierType(char *eep_data);
/* Returns Com Specification Revision as string in format "x.x"
 * (eg. 1.1 or 2.0) */
int16_t GetDecodedComSpecVersion(char *eep_data, char *vers_buf);
int32_t GetDynamicBlockCount(char *eep_data);

struct BlockHead {
	int32_t offset;
	uint8_t id;
	int32_t len;
};

/* We assume that user have already got BlockCount
 * and allocated enought memory for "struct dBlks *blocks"
 * which is transmitted to GetDynamicBlockData() */
int32_t GetDynamicBlockData(char *eep_data, struct BlockHead *blocks, int32_t count);

struct dmi_header {
	uint8_t type;
	uint8_t length;
	uint16_t handle;
	char *data;
};
const char *dmi_string(struct dmi_header *dm, char s);

/* SMBIOS struct of zero type (SMBIOS Type 0) (see SMBios spec.)*/
struct BIOSInfo {
	char vendor[KEAPI_MAX_STR];
	char version[KEAPI_MAX_STR];
	char release_date[KEAPI_MAX_STR];
};

/* SMBIOS struct of first type (SMBIOS Type 1) (see SMBios spec.)*/
struct SysInfo {
	char manufacturer[KEAPI_MAX_STR];
	char product[KEAPI_MAX_STR];
	char version[KEAPI_MAX_STR];
	char serial[KEAPI_MAX_STR];
};

/* SMBIOS struct of second type (SMBIOS Type 2) (see SMBios spec.)*/
struct HWInfo {
	char manufacturer[KEAPI_MAX_STR];
	char product[KEAPI_MAX_STR];
	char version[KEAPI_MAX_STR];
	char serial[KEAPI_MAX_STR];
	char assetTag[KEAPI_MAX_STR];
	unsigned char featureFlag;
	char location[KEAPI_MAX_STR];
	unsigned char boardType;
};

/* KEM Extended Board Information (DMI Type 160) (see KEM spec.)*/
struct KemExtInfo {
	char kemId[KEAPI_MAX_STR];
	char projectName[KEAPI_MAX_STR];
	char manufacturerDate[KEAPI_MAX_STR];
	char repairDate[KEAPI_MAX_STR];
	char patchLevel[KEAPI_MAX_STR];
};

/* KEM Running-time data (DMI Type 161) (see KEM spec.)*/
struct KemRTDat {
	char kemId[KEAPI_MAX_STR];
	uint64_t bootCounter;
	uint64_t runningTimeMeter;
};

void *TryToParsePicmgSmbiosHeader(char *data, unsigned char *type);
void *TryToParseSmbiosHeader(char *data, unsigned char *type);

/* User Area structure (see KEM spec.) */
struct EepUsr {
	int32_t start;
	int32_t size;
};

struct EepUsr *TryToGetUserDataArea(char *eep_data, int32_t offset);

// DMI_T - DMI TYPE
// DMI_STR - Number of string in a table
// DMI_BF - Bit field in a table
// DMI_BT - Byte in a table
// DMI_4B - Four bytes
#define DMI_T_BIOS 0 // BIOS Information (DMI Type 0)
#define DMI_STR_BIOS_VENDOR		0x04
#define DMI_STR_BIOS_VERSION		0x05
#define DMI_STR_BIOS_RELEASE_DATE	0x08

#define DMI_T_SYS 1 // System Information (DMI Type 1)
#define DMI_STR_SYS_MANUFACTURER	0x04
#define DMI_STR_SYS_PRDCT_NM		0x05
#define DMI_STR_SYS_VERSION		0x06
#define DMI_STR_SYS_SERIALNUM		0x07

#define DMI_T_HW 2 // Base Board (or Module) Information (DMI Type 2)
#define DMI_STR_HW_MANUFATURER		0x04
#define DMI_STR_HW_PRDCT		0x05
#define DMI_STR_HW_VERSION		0x06
#define DMI_STR_HW_SERIALNUM		0x07
#define DMI_STR_HW_ASSETTAG		0x08
#define DMI_BF_HW_FEATURE_FLAG		0x09
#define DMI_STR_HW_LOCATION		0x0A
#define DMI_BT_HW_BOARD_TYPE		0x0D

#define DMI_T_KEM_EXT_BRD		160 // KEM Extended Board Information (DMI Type 160)
#define DMI_4B_KEM_EXT_KEM_ID		0x04 // KEM ID
#define DMI_STR_KEM_EXT_PRJ_NM		0x09 // project name
#define DMI_STR_KEM_EXT_MNFCTRNG_DATE	0x0A // manufacturing date
#define DMI_STR_KEM_EXT_REPAIR_DATE	0x0B // repair date
#define DMI_STR_KEM_EXT_PTCH_LVL	0x0C // patch level

#define DMI_T_KEM_RT_DAT		161 // KEM Running-time data (DMI Type 161)
#define DMI_4B_KEM_RTDAT_KEM_ID		0x04 // KEM ID
#define DMI_8B_KEM_RTDAT_BOOTCNT	0x0C // Boot counter
#define DMI_8B_KEM_RTDAT_RTM		0x14 // Running time meter
#define DMI_T_KEM_BKL			162 // KEM Backlight Information (DMI Type 162)
#define DMI_T_KEM_PRFRMC		163 // KEM Performance Information (DMI Type 163)
#define DMI_T_KEM_MAC			164 // KEM MAC Address Information (DMI Type 164)

#endif /* EEP_PICMG_H */
