/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron AG */

/* KEAPI board info implementation. */

#include <string.h>
#include <regex.h>
#include <pcre.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <ctype.h>

#include "keapi_inc.h"
#include "linux_support.h"
#include "storage_picmg.h"
#include "globals.h"
#include "eapi/EeeP.h"

#define UBOOT_VERSION_PATH "/proc/device-tree/chosen/u-boot,version"

static int monthLetToDig(const char *orig_month)
{
	// Apr 3
	// Aug 7
	// Dec 11
	// Feb 1
	// Jan 0
	// Jul 6
	// Jun 5
	// Mar 2
	// May 4
	// Nov 10
	// Oct 9
	// Sep 8

	switch (tolower(orig_month[0])) {
	case 'j':
		if (tolower(orig_month[1]) == 'u') {
			if (tolower(orig_month[2]) == 'l')
				return 6;
			return 5;
		}
		return 0;
	case 'a':
		if (tolower(orig_month[1]) == 'p')
			return 3;
		return 7;
	case 'm':
		if (tolower(orig_month[2]) == 'r')
			return 2;
		return 4;
	case 'd':
		return 11;
	case 'f':
		return 1;
	case 'n':
		return 10;
	case 'o':
		return 9;
	default:
		return 8;
	}
}

static void fillDataFromHeader(unsigned char tableType, void *parsedSmbiosHeader, PKEAPI_BOARD_INFO pBoardInfo)
{
	struct tm ts;

	if (!parsedSmbiosHeader)
		return;

	switch (tableType) {
	case DMI_T_BIOS: {
		struct BIOSInfo *bInfo = parsedSmbiosHeader;

		strncat(pBoardInfo->firmwareVersion, bInfo->version, KEAPI_MAX_STR - 1);

		memset(&ts, 0, sizeof(struct tm));
		sscanf(bInfo->release_date, "%d/%d/%d", &ts.tm_mon, &ts.tm_mday, &ts.tm_year);
		if (ts.tm_year) {
			ts.tm_mon = ts.tm_mon - 1;
			ts.tm_year = ts.tm_year - 1900;
			pBoardInfo->firmwareDate = timegm(&ts);
		}

		break;
	}
	case DMI_T_SYS: {
		struct SysInfo *sysInfo = parsedSmbiosHeader;

		strncat(pBoardInfo->carrierInfo, sysInfo->product, KEAPI_MAX_STR - 1);

		break;
	}
	case DMI_T_HW: {
		struct HWInfo *hwInfo = parsedSmbiosHeader;

		strncat(pBoardInfo->boardManufacturer, hwInfo->manufacturer, KEAPI_MAX_STR - 1);

		if (strlen(hwInfo->product) > 0) {
			pBoardInfo->boardName[0] = '\0';
			strncat(pBoardInfo->boardName, hwInfo->product, KEAPI_MAX_STR - 1);
		}

		pBoardInfo->hardwareVersion[0] = '\0';
		strncat(pBoardInfo->hardwareVersion, hwInfo->version, KEAPI_MAX_STR - 1);

		strncat(pBoardInfo->boardSerialNumber, hwInfo->serial, KEAPI_MAX_STR - 1);

		break;
	}
	case DMI_T_KEM_EXT_BRD: {
		struct KemExtInfo *keInfo = parsedSmbiosHeader;

		/* Copy found board name only if it is still empty
		 * as data from table Type 2 is more preferable */
		if (pBoardInfo->boardName[0] == '\0')
			strncat(pBoardInfo->boardName, keInfo->projectName, KEAPI_MAX_STR - 1);

		memset(&ts, 0, sizeof(struct tm));
		sscanf(keInfo->manufacturerDate, "%d/%d/%d", &ts.tm_mon, &ts.tm_mday, &ts.tm_year);
		if (ts.tm_year) {
			ts.tm_mon = ts.tm_mon - 1;
			ts.tm_year = ts.tm_year - 1900;
			pBoardInfo->manufacturingDate = timegm(&ts);
		}

		memset(&ts, 0, sizeof(struct tm));
		sscanf(keInfo->repairDate, "%d/%d/%d", &ts.tm_mon, &ts.tm_mday, &ts.tm_year);
		if (ts.tm_year) {
			ts.tm_mon = ts.tm_mon - 1;
			ts.tm_year = ts.tm_year - 1900;
			pBoardInfo->lastRepairDate = timegm(&ts);
		}

		break;
	}
	default:
		break;
	}

	return;
}

/*******************************************************************************/
KEAPI_RETVAL KEApiL_GetBoardInfo(PKEAPI_BOARD_INFO pBoardInfo)
{
	uint16_t table_len, struct_count;
	char *data, *str;
	unsigned char str_len;
	int i, j, ret;

	/* Check function parameters */
	if (pBoardInfo == NULL)
		return KEAPI_RET_PARAM_NULL;

	/* Initialization */
	pBoardInfo->boardManufacturer[0] = '\0';
	pBoardInfo->boardName[0] = '\0';
	pBoardInfo->boardSerialNumber[0] = '\0';
	pBoardInfo->carrierInfo[0] = '\0';
	pBoardInfo->firmwareVersion[0] = '\0';
	pBoardInfo->firmwareDate = 0;
	pBoardInfo->lastRepairDate = 0;
	pBoardInfo->manufacturingDate = 0;

	if (MachineIsArm()) {
		int8_t found = FALSE; /* picmg is found trigger */

		/* looking for picmg eeprom and get some info from it */

		/* if storage configuration file have not been read, read it*/
		if (siArr == NULL || siArrCount == 0) {
			/* ROOT PRIVILEGES NEEDED */
			ret = GetStorageConfig();
			if (ret != KEAPI_RET_SUCCESS && ret != KEAPI_RET_PARTIAL_SUCCESS)
				return ret;
		}

		/* looking for picmg label across all storages info */
		i = 0;
		while (i < siArrCount) {
			/* if we found picmg storage */
			if (siArr[i].picmg_flag) {
				struct BlockHead *blockHeads;
				int32_t blockCount;

				if ((ret = checkRAccess(siArr[i].path)) != KEAPI_RET_SUCCESS) {
					if (ret == KEAPI_RET_RETRIEVAL_ERROR) //no file
						break;
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
					break;
				}

				/* then we go through all dynamic blocks and looking for SMBIOS headers */
				for (j = 0; j < blockCount; j++) {
					if (blockHeads[j].id == EEEP_BLOCK_ID_SMBIOS) {
						void *parsedSmbiosHeader = NULL;
						unsigned char parsedSmbiosType = 0;

						parsedSmbiosHeader = TryToParsePicmgSmbiosHeader(
							&data[blockHeads[j].offset], &parsedSmbiosType);

						/* if we found it, we look for table 2 of SMBIOS and take info */
						if (parsedSmbiosHeader) {
							if (parsedSmbiosType == DMI_T_HW)
								found = TRUE;

							fillDataFromHeader(parsedSmbiosType, parsedSmbiosHeader,
									   pBoardInfo);
							free(parsedSmbiosHeader);
						}
					}
				} /* for (j = 0; j < blockCount; j++) */
				free(blockHeads);
				free(data);
			} /* if (siArr[i].picmg_flag) */
			i++;
		} /* while ( i < siArrCount) */

		if (pBoardInfo->firmwareVersion[0] == '\0' && checkRAccess(UBOOT_VERSION_PATH) == KEAPI_RET_SUCCESS) {
			struct tm ts;
			char month[KEAPI_MAX_STR];

			if ((ret = ReadFile(UBOOT_VERSION_PATH, &data)) != KEAPI_RET_SUCCESS)
				return ret;

			/*
			 * Format of Firmware version in this file should be:
			 * U-Boot 2017.03-00078-g5a55701420 (Jan 11 2018 - 15:00:16 +0100)
			 */
			memset(&ts, 0, sizeof(struct tm));

			str = strchr(data, '(');
			if (str != NULL) {
				*(str - 1) = '\0';
				strncat(pBoardInfo->firmwareVersion, data, KEAPI_MAX_STR - 1);
			}

			ret = sscanf(str, "(%s %d %d -", month, &ts.tm_mday, &ts.tm_year);
			if (ret == 3) {
				ts.tm_mon = monthLetToDig(month);
				ts.tm_year = ts.tm_year - 1900;
				pBoardInfo->firmwareDate = timegm(&ts);
			}
		}

		if (pBoardInfo->firmwareVersion[0] == '\0' && checkRAccess(UBOOT_VERSION_PATH) != KEAPI_RET_SUCCESS) {
			FILE *fd;
			int32_t rc;
			char *mtd_info;
			char path[PATH_MAX];

			if (ReadFile("/proc/mtd", &mtd_info) == KEAPI_RET_SUCCESS) {
				if (GetSubStrRegex(mtd_info, "mtd([0-9]+):.*\"bootloader\"", &str,
						   REG_EXTENDED | REG_NEWLINE) == KEAPI_RET_SUCCESS) {
					strcpy(path, "/dev/mtd");
					strcat(path, str);
					free(str);
				}
				free(mtd_info);
			}

			if ((fd = fopen(path, "rb")) != NULL) {
				uint16_t i, bufsize = 1024;
				char info[bufsize], regex[KEAPI_MAX_STR];
				pcre *re;
				const char *error;
				int erroffset;
				int32_t ovector[15];

				/*
				 * Format of Firmware version:
				 * U-Boot 2017.03-00078-g5a55701420 (Jan 11 2018 - 15:00:16 +0100)
				*/
				snprintf(regex, KEAPI_MAX_STR,
					 "(U-Boot\\s+\\d.*)\\s+\\((\\w+)\\s+(\\d+)\\s+(\\d+).*[\\+\\-]\\d{4}\\)");

				if ((re = pcre_compile(regex, PCRE_NEWLINE_ANYCRLF | PCRE_DOTALL, &error, &erroffset,
						       NULL)) != NULL) {
					int rd_num = 0;
					while ((rd_num = fread(info, 1, bufsize - 1, fd)) > 0) {
						for (i = 0; i < bufsize; i++) {
							if (info[i] < 32 || info[i] > 126)
								info[i] = ' ';
						}
						info[bufsize - 1] = '\0';

						rc = pcre_exec(re, NULL, info, bufsize, 0, 0, ovector, 15);
						if (rc > 0) {
							memcpy(pBoardInfo->firmwareVersion, info + ovector[2],
							       ovector[3] - ovector[2]);
							pBoardInfo->firmwareVersion[ovector[3] - ovector[2]] = '\0';

							struct tm ts;
							memset(&ts, 0, sizeof(struct tm));

							char day[3], month[3], year[5];
							memcpy(month, info + ovector[4], ovector[5] - ovector[4]);
							memcpy(day, info + ovector[6], ovector[7] - ovector[6]);
							memcpy(year, info + ovector[8], ovector[9] - ovector[8]);
							month[2] = day[2] = year[4] = '\0';
							ts.tm_mon = monthLetToDig(month);
							ts.tm_mday = atoi(day);
							ts.tm_year = atoi(year) - 1900;
							pBoardInfo->firmwareDate = timegm(&ts);
							break;
						}
						if (rd_num == bufsize - 1)
							fseek(fd, -KEAPI_MAX_STR, SEEK_CUR);
					}
					fclose(fd);
				}
			}
		}

		if (found)
			return KEAPI_RET_SUCCESS;

		/* if we didn't find picmg eeprom
		 * try to get most info from sysfs/procfs before falling back to DMI
		 * contents of /proc/cpuinfo below is arm-specific */

		if ((ret = checkRAccess(CPUINFO_PATH)) != KEAPI_RET_SUCCESS)
			return ret;

		if (ReadFile(CPUINFO_PATH, &data) != KEAPI_RET_SUCCESS)
			return KEAPI_RET_ERROR;

		if (GetSubStrRegex(data, "Hardware\t: (.*)", &str, REG_EXTENDED | REG_NEWLINE) == KEAPI_RET_SUCCESS) {
			strncat(pBoardInfo->boardName, str, KEAPI_MAX_STR - 1);
			free(str);
		}

		if (GetSubStrRegex(data, "Revision\t: (.*)", &str, REG_EXTENDED | REG_NEWLINE) == KEAPI_RET_SUCCESS) {
			strncat(pBoardInfo->carrierInfo, str, KEAPI_MAX_STR - 1);
			free(str);
		}

		if (GetSubStrRegex(data, "Serial\t\t: (.*)", &str, REG_EXTENDED | REG_NEWLINE) == KEAPI_RET_SUCCESS) {
			strncat(pBoardInfo->boardSerialNumber, str, KEAPI_MAX_STR - 1);
			free(str);
		}

		free(data);
	}

	/* on non-X86 we'll always have something missing (BIOS at least) */
	if (MachineIsX86() == FALSE)
		return KEAPI_RET_SUCCESS;

	/* GET DMI TABLE */
	if ((ret = getDmiTable(&data, &table_len, &struct_count)) != KEAPI_RET_SUCCESS)
		return ret;

	/* Walking through table and looking for tables 0(BIOS), 2(MODULE) and 160(KEM) */
	str = data;
	for (i = 0; i < struct_count; i++) {
		unsigned char tableType;
		void *parsedSmbiosHeader = NULL;

		str_len = str[0x01];

		parsedSmbiosHeader = TryToParseSmbiosHeader(str, &tableType);

		if (parsedSmbiosHeader) {
			fillDataFromHeader(tableType, parsedSmbiosHeader, pBoardInfo);
			free(parsedSmbiosHeader);
		} /*if (parsedSmbiosHeader) {*/

		str += str_len;
		/* looking for \0\0 */
		while (str[0] != 0 || str[1] != 0)
			str++;
		str++;
		str++;
	}

	free(data);
	return KEAPI_RET_SUCCESS;
}
