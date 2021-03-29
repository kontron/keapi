/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron Europe GmbH */

/* Functions providing list of PCI devices. */

#include <stdio.h>
#include <string.h>
#include <pcre.h>
#include <dirent.h>
#include <regex.h>
#include <jansson.h>

#include "keapi_inc.h"
#include "pcihdr.h"
#include "globals.h"

#define DEV_ARR_CHUNK_SIZE 256
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
/* function helper  for reading pci vendor/device ids and
   device names from config file and filling a cache */
static void GetPciConfig(void)
{
	json_t *root, *jPciArr, *jPciDev, *data;
	json_error_t error;
	uint32_t i;
	void *tmp;

	if (checkRAccess(PCI_CONF_PATH) != KEAPI_RET_SUCCESS)
		return;

	root = json_load_file(PCI_CONF_PATH, JSON_DECODE_ANY, &error);
	if (!root)
		goto exit;

	jPciArr = json_object_get(root, "pciDevices");
	if (!json_is_array(jPciArr))
		goto exit;

	for (i = 0; i < json_array_size(jPciArr); i++) {
		jPciDev = json_array_get(jPciArr, i);
		if (!json_is_object(jPciDev)) {
			continue;
		}

		if (pciDevArrCount % DEV_ARR_CHUNK_SIZE == 0) {
			tmp = realloc(pciDevtArr, sizeof(struct PciInfo) * (pciDevArrCount + DEV_ARR_CHUNK_SIZE));
			if (!tmp)
				return;
			pciDevtArr = tmp;
		}

		data = json_object_get(jPciDev, "VendorId");
		if (json_is_string(data))
			pciDevtArr[pciDevArrCount].vendorId = strtol(json_string_value(data), NULL, 16);
		else
			continue;

		data = json_object_get(jPciDev, "DeviceId");
		if (json_is_string(data))
			pciDevtArr[pciDevArrCount].deviceId = strtol(json_string_value(data), NULL, 16);
		else
			continue;

		data = json_object_get(jPciDev, "DeviceName");
		if (json_is_string(data)) {
			strncpy(pciDevtArr[pciDevArrCount].deviceName, json_string_value(data), KEAPI_MAX_STR);
		} else {
			pciDevtArr[pciDevArrCount].deviceName[0] = '\0';
		}

		data = json_object_get(jPciDev, "VendorName");
		if (json_is_string(data)) {
			strncpy(pciDevtArr[pciDevArrCount].vendorName, json_string_value(data), KEAPI_MAX_STR);
		} else {
			pciDevtArr[pciDevArrCount].vendorName[0] = '\0';
		}

		pciDevArrCount++;
	}
exit:
	json_decref(root);
}

/*******************************************************************************/
/* function helper  for reading system pci.ids file and
 * filling a cache
*/
static void GetPciIds(void)
{
	char *line_buf = NULL;
	size_t line_buf_size = 0;
	uint16_t vendorId = 0, deviceId = 0;
	char *substr, *pci_ids_filename;
	char vendorField[5] = "", vendorName[KEAPI_MAX_STR] = "";
	void *tmp;

	if (checkRAccess("/usr/share/hwdata/pci.ids") == KEAPI_RET_SUCCESS)
		pci_ids_filename = "/usr/share/hwdata/pci.ids";
	else if ((checkRAccess("/usr/share/misc/pci.ids")) == KEAPI_RET_SUCCESS)
		pci_ids_filename = "/usr/share/misc/pci.ids";
	else if (checkRAccess("/usr/share/pci.ids") == KEAPI_RET_SUCCESS)
		pci_ids_filename = "/usr/share/pci.ids";
	else if (checkRAccess("/var/lib/pciutils/pci.ids") == KEAPI_RET_SUCCESS)
		pci_ids_filename = "/var/lib/pciutils/pci.ids";
	else
		pci_ids_filename = NULL;

	if (pci_ids_filename) {
		FILE *fp = fopen(pci_ids_filename, "r");

		if (!fp)
			return;

		while (getline(&line_buf, &line_buf_size, fp) >= 0) {
			if (line_buf[0] != '\t' && line_buf[0] != '#') { /* probably Vendor field */
				strncpy(vendorField, line_buf, 4);
				vendorId = strtol(vendorField, NULL, 16);
				if (GetSubStrRegex(line_buf, "^[0-9a-f]*\\s*(.*)", &substr,
						   REG_EXTENDED | REG_NEWLINE | REG_ICASE) == KEAPI_RET_SUCCESS) {
					strncpy(vendorName, substr, KEAPI_MAX_STR);
					free(substr);
				}
				continue;

			} else if (GetSubStrRegex(line_buf, "^\t([0-9a-f]{4})", &substr,
						  REG_EXTENDED | REG_NEWLINE | REG_ICASE) == KEAPI_RET_SUCCESS) {
				deviceId = strtol(substr, NULL, 16);
				free(substr);

				if (GetSubStrRegex(line_buf, "^\t[0-9a-f]*\\s*(.*)", &substr,
						   REG_EXTENDED | REG_NEWLINE | REG_ICASE) == KEAPI_RET_SUCCESS) {
					if (pciDevArrCount % DEV_ARR_CHUNK_SIZE == 0) {
						tmp = realloc(pciDevtArr, sizeof(struct PciInfo) * (pciDevArrCount + DEV_ARR_CHUNK_SIZE));
						if (!tmp) {
							free(substr);
							return;
						}
						pciDevtArr = tmp;
					}
					pciDevtArr[pciDevArrCount].vendorId = vendorId;
					pciDevtArr[pciDevArrCount].deviceId = deviceId;
					strncpy(pciDevtArr[pciDevArrCount].deviceName, substr, KEAPI_MAX_STR);
					strncpy(pciDevtArr[pciDevArrCount].vendorName, vendorName, KEAPI_MAX_STR);
					free(substr);
					pciDevArrCount++;
				}
			}
		}
		free(line_buf);
		line_buf = NULL;
		fclose(fp);
	}
}

/*******************************************************************************/
/* function helper  for reading driver name with modinfo
 * and filling a cache
*/
static void GetModinfo(PKEAPI_PCI_DEVICE pPciDevices, int32_t count)
{
	char module_name[KEAPI_MAX_STR], modinfo_cmd[KEAPI_MAX_STR], *pdata;
	int32_t i;
	char *modinfo, *substr;
	DIR *dir;
	struct dirent *ent;
	char path[KEAPI_MAX_STR];
	void *tmp;

	if (checkRAccess("/usr/sbin/modinfo") == KEAPI_RET_SUCCESS)
		modinfo = "/usr/sbin/modinfo";
	else if (checkRAccess("/sbin/modinfo") == KEAPI_RET_SUCCESS)
		modinfo = "/sbin/modinfo";
	else if (checkRAccess("/usr/bin/modinfo") == KEAPI_RET_SUCCESS)
		modinfo = "/usr/bin/modinfo";
	else
		modinfo = NULL;

	for (modinfo && (i = 0); i < count; i++) {
		/* look pci driver name */
		if (snprintf(path, KEAPI_MAX_STR, "%s/%04x:%02x:%02x.%01x/driver/module/drivers", PCI_PATH,
			     pPciDevices[i].domain, pPciDevices[i].bus, pPciDevices[i].slot, pPciDevices[i].funct) < 0)
			continue;

		if (checkRAccess(path) != KEAPI_RET_SUCCESS)
			continue;

		if ((dir = opendir(path)) == NULL)
			continue;

		while ((ent = readdir(dir)) != NULL) {
			if (strncmp(ent->d_name, "pci:", 4) == 0) {
				strcpy(module_name, ent->d_name + 4);
			}
		}
		closedir(dir);

		if (snprintf(modinfo_cmd, KEAPI_MAX_STR, "%s %s 2>/dev/null\n", modinfo, module_name) < 0) {
			continue;
		}

		/* ask modinfo for pci driver name */
		if (GetExternalCommandOutput(modinfo_cmd, &pdata) != KEAPI_RET_SUCCESS) {
			continue;
		}

		if (GetSubStrRegex(pdata, "description:\\s+(.*)", &substr, REG_EXTENDED | REG_NEWLINE | REG_ICASE) ==
		    KEAPI_RET_SUCCESS) {
			if (pciDevArrCount % DEV_ARR_CHUNK_SIZE == 0) {
				tmp = realloc(pciDevtArr, sizeof(struct PciInfo) * (pciDevArrCount + DEV_ARR_CHUNK_SIZE));
				if (!tmp) {
					free(substr);
					return;
				}
				pciDevtArr = tmp;
			}
			pciDevtArr[pciDevArrCount].vendorId = pPciDevices[i].vendorId;
			pciDevtArr[pciDevArrCount].deviceId = pPciDevices[i].deviceId;
			strncpy(pciDevtArr[pciDevArrCount].deviceName, substr, KEAPI_MAX_STR);
			pciDevtArr[pciDevArrCount].vendorName[0] = '\0';
			pciDevArrCount++;
			free(substr);
		}
		free(pdata);
	}
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

	GetPciConfig();

	/* /etc/keapi/pci.conf overrides any device or vendor name */
	for (i = 0; i < count; i++) {
		for (j = 0; j < pciDevArrCount; j++) {
			if (pPciDevices[i].vendorId == pciDevtArr[j].vendorId &&
			    pPciDevices[i].deviceId == pciDevtArr[j].deviceId) {
				strcpy(pPciDevices[i].deviceName, pciDevtArr[j].deviceName);
				if (strlen(pPciDevices[i].vendorName) > 0)
					strcpy(pPciDevices[i].vendorName, pciDevtArr[j].vendorName);
				break;
			}
		}
	}

	GetPciIds();
	GetModinfo(pPciDevices, count);

	/* pci.ids and modinfo are valid only for empty pci device name */
	for (i = 0; i < count; i++) {
		if (strlen(pPciDevices[i].deviceName) == 0) {
			for (j = 0; j < pciDevArrCount; j++) {
				if (pPciDevices[i].vendorId == pciDevtArr[j].vendorId &&
				    pPciDevices[i].deviceId == pciDevtArr[j].deviceId) {
					strcpy(pPciDevices[i].deviceName, pciDevtArr[j].deviceName);
					break;
				}
			}
		}
		if (strlen(pPciDevices[i].vendorName) == 0) {
			for (j = 0; j < pciDevArrCount; j++) {
				if (pPciDevices[i].vendorId == pciDevtArr[j].vendorId &&
				    pPciDevices[i].deviceId == pciDevtArr[j].deviceId) {
					strcpy(pPciDevices[i].vendorName, pciDevtArr[j].vendorName);
					break;
				}
			}
		}
	}
	if (pciDevtArr)
		free(pciDevtArr);

	if (pciDeviceCount < real_dev_count)
		return KEAPI_RET_PARTIAL_SUCCESS;

	return KEAPI_RET_SUCCESS;
}
