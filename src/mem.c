/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron Europe GmbH */

/* Memory information functions. */

#include <stdio.h>
#include <string.h>
#include <regex.h>
#include <jansson.h>

#include "keapi_inc.h"
#include "linux_support.h"

#define DTB_MEM_NODE "/proc/device-tree/memory"

/*******************************************************************************/
KEAPI_RETVAL KEApiL_GetMemoryInfo(PKEAPI_MEMORY_INFO pMemoryInfo)
{
	int32_t i, ret;
	char *table, *str, str_type, str_len;
	uint16_t table_len, struct_count;
	char *info, *substr;

	/* json root, json jdata */
	json_t *root, *jdata;
	json_error_t error;

	/* Check function parameters */
	if (pMemoryInfo == NULL)
		return KEAPI_RET_PARAM_NULL;

	/* Initialization */
	pMemoryInfo->memTotal = 0;
	pMemoryInfo->memFree = 0;
	pMemoryInfo->memSpeed = 0;
	pMemoryInfo->memType[0] = '\0';

	/* Read total and free size from the file /proc/meminfo */
	if ((ret = checkRAccess(MEMORY_INFO_PATH)) != KEAPI_RET_SUCCESS)
		return ret;

	if ((ret = ReadFile(MEMORY_INFO_PATH, &info)) != KEAPI_RET_SUCCESS)
		return ret;

	/* Get memory size */
	if (GetSubStrRegex(info, "MemTotal:\\s+(.*) kB", &substr, REG_EXTENDED | REG_NEWLINE | REG_ICASE) !=
	    KEAPI_RET_SUCCESS) {
		free(info);
		return KEAPI_RET_ERROR;
	} else {
		pMemoryInfo->memTotal = atoi(substr) / 1024;
		free(substr);
	}

	/* Get free memory */
	if (GetSubStrRegex(info, "MemFree:\\s+(.*) kB", &substr, REG_EXTENDED | REG_NEWLINE | REG_ICASE) !=
	    KEAPI_RET_SUCCESS) {
		free(info);
		return KEAPI_RET_ERROR;
	} else {
		pMemoryInfo->memFree = atoi(substr) / 1024;
		free(substr);
	}

	free(info);

	if (MachineIsArm()) {
		char path[KEAPI_MAX_STR];

		/* Firstly trying to take memory info from config file */
		if ((ret = checkRAccess(MEMORY_CONFIG_PATH)) == KEAPI_RET_SUCCESS) {
			root = json_load_file(MEMORY_CONFIG_PATH, JSON_DECODE_ANY, &error);
			if (!root)
				return KEAPI_RET_READ_ERROR;

			jdata = json_object_get(root, "memoryFrequency");
			if (json_is_integer(jdata))
				pMemoryInfo->memSpeed = json_integer_value(jdata);

			jdata = json_object_get(root, "memoryType");
			if (json_is_string(jdata))
				strncpy(pMemoryInfo->memType, json_string_value(jdata), KEAPI_MAX_STR - 1);

			json_decref(root);
			return KEAPI_RET_SUCCESS;
		}

		/* Otherwise takes memory info from DT */
		snprintf(path, KEAPI_MAX_STR, "%s/%s", DTB_MEM_NODE, "memory-type");
		if ((ret = checkRAccess(path)) == KEAPI_RET_SUCCESS) {
			if ((ret = ReadFile(path, &info)) != KEAPI_RET_SUCCESS)
				return ret;

			strncat(pMemoryInfo->memType, info, KEAPI_MAX_STR - 1);
			free(info);
		} else if (ret != KEAPI_RET_RETRIEVAL_ERROR) // file present, but smth goes wrong
			return ret;

		snprintf(path, KEAPI_MAX_STR, "%s/%s", DTB_MEM_NODE, "memory-frequency");
		if ((ret = checkRAccess(path)) == KEAPI_RET_SUCCESS) {
			uint64_t mem_freq = 0;
			if ((ret = ReadFile(path, &info)) != KEAPI_RET_SUCCESS)
				return ret;

			for (i = 0; i < 4; i++) // frequency should be represented by 8 bytes in dts
				/* FIXME: but really represented by 4 bytes for 1600 MHz freq, e.g.
				 * There are "ReadFile" routine must be replaced to open/read routine in which
				 * number of read bytes can be taken */
				mem_freq = (mem_freq << 8) | info[i];

			pMemoryInfo->memSpeed = (int32_t)(mem_freq / 1000000); // adjust to MHz
			free(info);
		} else if (ret != KEAPI_RET_RETRIEVAL_ERROR) // file present, but smth goes wrong
			return ret;
	}

	/* TODO: add support / fetching information about memory from picmg eeproms.
	 * For reference see board.c
	 */

	/* On non-x86 arches DMI isn't supported */
	if (MachineIsX86() == FALSE)
		return KEAPI_RET_SUCCESS;

	/* Get speed and type from DMI */

	/* Get Module structures from DMI */
	if ((ret = getDmiTable(&table, &table_len, &struct_count)) != KEAPI_RET_SUCCESS)
		return ret;

	/* Look for struct type */
	str = table;
	for (i = 0; i < struct_count; i++) {
		str_type = str[0x0];
		str_len = str[0x01];

		/* Processing structure */
		if (str_type == 17) /* Memory device - get info from first module */ {
			/* If no module is installed - Size == 0 */
			if (str_len > 0x0C && *(uint16_t *)(str + 0x0C) != 0) {
				/* Memory type is at position 0x12 */
				if (str_len > 0x12) {
					switch (str[0x12]) {
					case 0x0F:
						strncat(pMemoryInfo->memType, "SDRAM", KEAPI_MAX_STR - 1);
						break;
					case 0x12:
						strncat(pMemoryInfo->memType, "DDR", KEAPI_MAX_STR - 1);
						break;
					case 0x13:
						strncat(pMemoryInfo->memType, "DDR2", KEAPI_MAX_STR - 1);
						break;
					case 0x18:
						strncat(pMemoryInfo->memType, "DDR3", KEAPI_MAX_STR - 1);
						break;
					case 0x1a:
						strncat(pMemoryInfo->memType, "DDR4", KEAPI_MAX_STR - 1);
						break;
					case 0x1b:
						strncat(pMemoryInfo->memType, "LPDDR", KEAPI_MAX_STR - 1);
						break;
					case 0x1c:
						strncat(pMemoryInfo->memType, "LPDDR2", KEAPI_MAX_STR - 1);
						break;
					case 0x1d:
						strncat(pMemoryInfo->memType, "LPDDR3", KEAPI_MAX_STR - 1);
						break;
					case 0x1e:
						strncat(pMemoryInfo->memType, "LPDDR4", KEAPI_MAX_STR - 1);
						break;
					default:
						strncat(pMemoryInfo->memType, "n/a", KEAPI_MAX_STR - 1);
					}

					if (str_len >= 0x17) {
						pMemoryInfo->memSpeed = *(uint16_t *)(str + 0x15);
					}
					break;
				}
			}
		}
		str += str_len;
		/* looking for \0\0 */
		while (str[0] != 0 || str[1] != 0)
			str++;
		str++;
		str++;
	}

	free(table);
	return KEAPI_RET_SUCCESS;
}
