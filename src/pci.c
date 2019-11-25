/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron AG */

/* Functions providing list of PCI devices. */

#include <stdio.h>
#include <string.h>
#include <pcre.h>
#include <dirent.h>

#include "keapi_inc.h"
#include "pcihdr.h"

/*******************************************************************************/
KEAPI_RETVAL KEApiL_GetPciDeviceCount(int32_t *pPciDeviceCount)
{
	DIR *dir;
	int32_t ret;
	struct dirent *ent;

	/* Check function parameters */
	if (pPciDeviceCount == NULL)
		return KEAPI_RET_PARAM_NULL;

	if ((ret = checkRAccess(PCI_PATH)) != KEAPI_RET_SUCCESS)
		return ret;

	if ((dir = opendir(PCI_PATH)) == NULL)
		return KEAPI_RET_ERROR;

	/* Count directories (PCI devices) starting with 0 */
	*pPciDeviceCount = 0;
	while ((ent = readdir(dir)) != NULL) {
		if (ent->d_name[0] == '0')
			(*pPciDeviceCount)++;
	}
	closedir(dir);
	return KEAPI_RET_SUCCESS;
}

/*******************************************************************************/
KEAPI_RETVAL KEApiL_GetPciDeviceList(PKEAPI_PCI_DEVICE pPciDevices, int32_t pciDeviceCount)
{
	DIR *dir;
	struct dirent *ent;
	char path[KEAPI_MAX_STR], regex[KEAPI_MAX_STR], *info, *substr;
	int32_t count, i, rc, len, real_dev_count = 0;
	uint32_t j;
	int32_t ovector[30], ret;
	int8_t found;
	pcre *re;
	const char *error;
	int erroffset;

	/* Check function parameters */
	if (pPciDevices == NULL)
		return KEAPI_RET_PARAM_NULL;

	if ((ret = KEApiL_GetPciDeviceCount(&real_dev_count)) != KEAPI_RET_SUCCESS)
		return ret;

	if (pciDeviceCount < 0 || pciDeviceCount > real_dev_count)
		return KEAPI_RET_PARAM_ERROR;

	if (pciDeviceCount == 0) {
		if (real_dev_count == 0)
			return KEAPI_RET_SUCCESS;
		else
			return KEAPI_RET_PARTIAL_SUCCESS;
	}

	/* Create regex - Domain, bus, slot, function nr. */
	snprintf(regex, KEAPI_MAX_STR, "(....):(..):(..)\\.(.)");
	if ((re = pcre_compile(regex, PCRE_MULTILINE | PCRE_NEWLINE_ANYCRLF | PCRE_DOTALL, &error, &erroffset, NULL)) ==
	    NULL)
		return KEAPI_RET_ERROR;

	if ((ret = checkRAccess(PCI_PATH)) != KEAPI_RET_SUCCESS) {
		pcre_free(re);
		return ret;
	}

	if ((dir = opendir(PCI_PATH)) == NULL) {
		pcre_free(re);
		return KEAPI_RET_ERROR;
	}

	/* directories (PCI devices) starting with 0 */
	count = 0;
	while ((ent = readdir(dir)) != NULL && count < pciDeviceCount) {
		if (ent->d_name[0] != '0')
			continue;

		/* Check if enough memory is provided */
		if (count >= pciDeviceCount) {
			pcre_free(re);
			closedir(dir);
			return KEAPI_RET_PARAM_ERROR;
		}

		if ((rc = pcre_exec(re, NULL, ent->d_name, strlen(ent->d_name), 0, 0, ovector, 30)) != 5)
			continue; // Not found, goto next dir

		// Domain
		len = ovector[3] - ovector[2];
		substr = (char *)malloc(len + 1);
		memcpy(substr, ent->d_name + ovector[2], len);
		substr[len] = '\0';
		pPciDevices[count].domain = strtol(substr, NULL, 16);
		free(substr);

		// Bus
		len = ovector[5] - ovector[4];
		substr = (char *)malloc(len + 1);
		memcpy(substr, ent->d_name + ovector[4], len);
		substr[len] = '\0';
		pPciDevices[count].bus = strtol(substr, NULL, 16);
		free(substr);

		// Slot
		len = ovector[7] - ovector[6];
		substr = (char *)malloc(len + 1);
		memcpy(substr, ent->d_name + ovector[6], len);
		substr[len] = '\0';
		pPciDevices[count].slot = strtol(substr, NULL, 16);
		free(substr);

		// Function
		len = ovector[9] - ovector[8];
		substr = (char *)malloc(len + 1);
		memcpy(substr, ent->d_name + ovector[8], len);
		substr[len] = '\0';
		pPciDevices[count].funct = strtol(substr, NULL, 16);
		free(substr);

		/* Vendor ID */
		if (snprintf(path, KEAPI_MAX_STR, "%s/%s/vendor", PCI_PATH, ent->d_name) < 0)
			continue;

		if ((ret = checkRAccess(path)) != KEAPI_RET_SUCCESS) {
			pcre_free(re);
			closedir(dir);
			return ret;
		}

		if ((ret = ReadFile(path, &info)) != KEAPI_RET_SUCCESS) {
			pcre_free(re);
			closedir(dir);
			return ret;
		}

		FindRegex(info, "0x....", &found, 0);
		if (found != TRUE) {
			pcre_free(re);
			closedir(dir);
			free(info);
			return KEAPI_RET_ERROR;
		}

		pPciDevices[count].vendorId = strtol(info, NULL, 16);

		free(info);

		/* Device ID */
		if (snprintf(path, KEAPI_MAX_STR, "%s/%s/device", PCI_PATH, ent->d_name) < 0)
			continue;

		if ((ret = checkRAccess(path)) != KEAPI_RET_SUCCESS) {
			pcre_free(re);
			closedir(dir);
			return ret;
		}

		if ((ret = ReadFile(path, &info)) != KEAPI_RET_SUCCESS) {
			pcre_free(re);
			closedir(dir);
			return ret;
		}

		FindRegex(info, "0x....", &found, 0);
		if (found != TRUE) {
			pcre_free(re);
			closedir(dir);
			free(info);
			return KEAPI_RET_ERROR;
		}

		pPciDevices[count].deviceId = strtol(info, NULL, 16);

		free(info);

		/* Class ID */
		if (snprintf(path, KEAPI_MAX_STR, "%s/%s/class", PCI_PATH, ent->d_name) < 0)
			continue;

		if ((ret = checkRAccess(path)) != KEAPI_RET_SUCCESS) {
			pcre_free(re);
			closedir(dir);
			return ret;
		}

		if ((ret = ReadFile(path, &info)) != KEAPI_RET_SUCCESS) {
			pcre_free(re);
			closedir(dir);
			return ret;
		}

		FindRegex(info, "0x......", &found, 0);
		if (found != TRUE) {
			pcre_free(re);
			closedir(dir);
			free(info);
			return KEAPI_RET_ERROR;
		}
		/* Only Class - 0x + first two chars */
		info[4] = '\0';

		pPciDevices[count].classId = strtol(info, NULL, 16);

		free(info);

		/* Initialize name */
		pPciDevices[count].deviceName[0] = '\0';
		pPciDevices[count].vendorName[0] = '\0';
		pPciDevices[count].className[0] = '\0';

		count++;
	}

	pcre_free(re);
	closedir(dir);

	for (i = 0; i < count; i++) {
		/* TODO: possibly it will be good idea to realize here binary search
		 * algorithm, for the future )) */

		for (j = 0; j < PCI_VENTABLE_LEN; j++)
			if (pPciDevices[i].vendorId == PciVenTable[j].VenId) {
				strncat(pPciDevices[i].vendorName, PciVenTable[j].VenFull, KEAPI_MAX_STR - 1);
				break;
			}

		for (j = 0; j < PCI_DEVTABLE_LEN; j++)
			if (pPciDevices[i].vendorId == PciDevTable[j].VenId &&
			    pPciDevices[i].deviceId == PciDevTable[j].DevId) {
				strncat(pPciDevices[i].deviceName, PciDevTable[j].ChipDesc, KEAPI_MAX_STR - 1);
				break;
			}

		for (j = 0; j < PCI_CLASSCODETABLE_LEN; j++)
			if (pPciDevices[i].classId == PciClassCodeTable[j].BaseClass) {
				strncat(pPciDevices[i].className, PciClassCodeTable[j].BaseDesc, KEAPI_MAX_STR - 1);
				break;
			}
	}

	if (pciDeviceCount < real_dev_count)
		return KEAPI_RET_PARTIAL_SUCCESS;

	return KEAPI_RET_SUCCESS;
}
