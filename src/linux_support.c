/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron Europe GmbH */

/* KEAPI support functions for Linux platform. */

#include <fcntl.h>
#include <regex.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <pcre.h>
#include <dirent.h>
#include <sys/mman.h>
#include <sys/utsname.h>
#include <jansson.h>

#include "keapi_inc.h"
#include "globals.h"
#include "storage_picmg.h"
#include "eapi/EeeP.h"

void keapiErrLog(const char *fmt, ...)
{
	time_t t;
	struct tm *today;
	char tstr[200];

	t = time(NULL);
	today = localtime(&t);
	strftime(tstr, sizeof(tstr), "%D-%R", today);
	fprintf(stderr, "LOG [%s]: \n", tstr);

	return;
}

/* Functions to lock and unlock access to temporary created files.
 * Goal of these function is synchronize access of several processes
 * to the files. They should be used in every places where are going to do
 * something with temporary files.*/
static void mkLock(char *path)
{
	int fd;

	fd = open(path, O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);

	if (fd > 0)
		close(fd);
	else
		keapiErrLog("FATAL ERROR: can't create lock: %s", strerror(errno));
}

int openLock(char *path, pthread_mutex_t *mutex)
{
	int fd;

	pthread_mutex_lock(mutex);
	mkLock(path);

	fd = open(path, O_RDWR);

	if (fd < 0 || lockf(fd, F_LOCK, 0))
		keapiErrLog("FATAL ERROR: failed to lock: %s", strerror(errno));

	return fd;
}

void closeUnlock(int fd, pthread_mutex_t *mutex)
{
	if (lockf(fd, F_ULOCK, 0)) {
		keapiErrLog("FATAL ERROR: failed to unlock: %s", strerror(errno));
	}

	close(fd);
	pthread_mutex_unlock(mutex);
}

/*******************************************************************************/
int8_t MachineIsArm(void)
{
	struct utsname name;

	uname(&name);

	if (!strncmp(name.machine, "arm", 3))
		return TRUE;
	if (!strncmp(name.machine, "aarch64", 7))
		return TRUE;

	return FALSE;
}

/*******************************************************************************/
int8_t MachineIsArm64(void)
{
	struct utsname name;

	uname(&name);

	if (!strncmp(name.machine, "aarch64", 7))
		return TRUE;

	return FALSE;
}

/*******************************************************************************/
int8_t MachineIsX86(void)
{
	struct utsname name;

	uname(&name);

	if (!strncmp(name.machine, "i686", 4))
		return TRUE;
	if (!strncmp(name.machine, "x86_64", 5))
		return TRUE;

	return FALSE;
}
/*******************************************************************************/

int32_t checkAccess(char *path, int type)
{
	errno = 0;

	if (access(path, type) != 0) {
		/* we don't have enough permissions */
		if (errno == EACCES || errno == EPERM)
			return KEAPI_RET_PERMISSION_DENIED;
		/* entry is not exist */
		if (errno == ENOENT || errno == ENOTDIR)
			return KEAPI_RET_RETRIEVAL_ERROR;
		/* some other non analyzed error */
		return KEAPI_RET_ERROR;
	}

	return KEAPI_RET_SUCCESS;
}

int32_t checkRAccess(char *path)
{
	return checkAccess(path, R_OK);
}

int32_t checkRWAccess(char *path)
{
	return checkAccess(path, R_OK | W_OK);
}

int32_t failIoctlCheckErrno(int *file)
{
	if (errno == EACCES || errno == EPERM)
		return KEAPI_RET_PERMISSION_DENIED;

	close((*file));
	return KEAPI_RET_ERROR;
}
/*******************************************************************************/
/* Function for read remap information about backlight resources
 * from configuration files which could be located
 * in /opt/eapi/{backlight.conf}.
 * Earlier it was used also for remapping i2c and smbus resources,
 * but now this function is deprecated for these purposes. */
int findRemapping(char *where, char list[][PATH_MAX], int32_t *count)
{
	char regex[KEAPI_MAX_STR], *data;
	int32_t rc, len;
	int32_t ovector[KEAPI_MAX_STR], start_offset;
	pcre *re;
	const char *error;
	int erroffset;

	(*count) = 0;

	if ((rc = ReadFile(where, &data)) != 0)
		return rc;

	snprintf(regex, KEAPI_MAX_STR, "^\\s*(\\S+)\\s*$");

	if ((re = pcre_compile(regex, PCRE_MULTILINE | PCRE_NEWLINE_ANYCRLF | PCRE_DOTALL, &error, &erroffset, NULL)) ==
	    NULL) {
		free(data);
		return KEAPI_RET_ERROR;
	}

	start_offset = 0;

	while ((*count) < KEAPI_MAX_STR) {
		if ((rc = pcre_exec(re, NULL, data, strlen(data), start_offset, 0, ovector, KEAPI_MAX_STR)) <= 0) {
			break;
		}

		len = ovector[3] - ovector[2];
		memcpy(list[*count], data + ovector[2], len);
		list[*count][len] = '\0';
		(*count)++;

		start_offset = ovector[1];
		if (start_offset > strlen(data))
			break;
	}

	pcre_free(re);
	free(data);
	return KEAPI_RET_SUCCESS;
}

/*****************************************************************************/
/* function reads corresponding configuration file from the file system		 */
int32_t GetInfoFromSerialBusConfFile(char sbType)
{
	char *sbPath, *bus_tmplt, substr[KEAPI_MAX_STR], *data;
	int conf_bus_count, real_bus_count, unit_count, i, ret, mf_ret = 0;
	int *units;

	/* tmp bus vars */
	struct SerialBusInfo *sbArr = NULL;
	int sbArrCount;

	/* root, json bus array, json bus array element, json data */
	json_t *root, *jbusarr, *jbusarrel, *jdata;
	json_error_t jerror;

	/* init temp counters */
	sbArrCount = conf_bus_count = real_bus_count = unit_count = 0;

	DIR *dir;
	struct dirent *ent;

	/* mutex lock, data initialization etc.*/
	switch (sbType) {
	case I2C_CONFTYPE:
		ret = pthread_mutex_trylock(&i2c_mutex);
		if (ret)
			return KEAPI_RET_BUSY_COLLISION;

		sbPath = I2C_CONF_PATH;
		bus_tmplt = "i2c";
		i2cArrCount = 0;
		if (i2cArr) {
			free(i2cArr);
			i2cArr = NULL;
		}
		break;
	case SMBUS_CONFTYPE:
		ret = pthread_mutex_trylock(&smbus_mutex);
		if (ret)
			return KEAPI_RET_BUSY_COLLISION;

		sbPath = SMBUS_CONF_PATH;
		bus_tmplt = "smbus";
		smbusArrCount = 0;
		if (smbusArr) {
			free(smbusArr);
			smbusArr = NULL;
		}
		break;
	default:
		return KEAPI_RET_ERROR;
	}

	/* trying reading configuration file*/
	if ((ret = checkRAccess(sbPath)) != KEAPI_RET_SUCCESS) {
		if (ret == KEAPI_RET_RETRIEVAL_ERROR)
			goto fs_scan;
		else
			return ret;
	}

	root = json_load_file(sbPath, JSON_DECODE_ANY, &jerror);
	ret = KEAPI_RET_ERROR;
	if (root) {
		jdata = json_object_get(root, "type");
		if (!json_is_string(jdata)) {
			json_decref(root);
			goto error;
		}
		substr[0] = '\0';
		strncat(substr, json_string_value(jdata), KEAPI_MAX_STR - 1);

		if (strcmp(substr, bus_tmplt) != 0) {
			json_decref(root);
			goto error;
		}

		jbusarr = json_object_get(root, "bus");
		if (!json_is_array(jbusarr)) {
			json_decref(root);
			goto error;
		}
		conf_bus_count = json_array_size(jbusarr);

		if (conf_bus_count == 0) {
			json_decref(root);
			ret = KEAPI_RET_SUCCESS;
			goto exit;
		}

		sbArr = realloc(sbArr, sizeof(struct SerialBusInfo) * (conf_bus_count));

		for (i = 0; i < conf_bus_count; i++) {
			jbusarrel = json_array_get(jbusarr, i);
			if (!json_is_object(jbusarrel)) {
				json_decref(root);
				goto error;
			}

			jdata = json_object_get(jbusarrel, "name");
			if (!json_is_string(jdata)) {
				json_decref(root);
				goto error;
			}
			sbArr[i].name[0] = '\0';
			strncat(sbArr[i].name, json_string_value(jdata), KEAPI_MAX_STR - 1);

			jdata = json_object_get(jbusarrel, "unit");
			if (!json_is_integer(jdata)) {
				json_decref(root);
				goto error;
			}
			sbArr[i].unit = json_integer_value(jdata);
			sbArr[i].realNr = -1;
		}

		json_decref(root);

		if ((ret = checkRAccess("/sys/bus/i2c/devices/")) != KEAPI_RET_SUCCESS)
			return ret;

		if ((dir = opendir("/sys/bus/i2c/devices/")) == NULL)
			return KEAPI_RET_ERROR;

		units = (int *)calloc(conf_bus_count, sizeof(int));
		while ((ent = readdir(dir)) != NULL) {
			char *retstr;

			if ((ent->d_type != DT_LNK) && (ent->d_type != DT_DIR))
				continue;
			if ((strncmp(ent->d_name, "i2c-", 4) != 0) || (strlen(ent->d_name) > 14))
				continue;

			snprintf(substr, KEAPI_MAX_STR, "/sys/bus/i2c/devices/%.14s/name", ent->d_name);

			if ((ret = checkRAccess(substr)) != KEAPI_RET_SUCCESS) {
				free(units);
				goto exit;
			}

			if ((ret = ReadFile(substr, &data)) != KEAPI_RET_SUCCESS)
				continue;

			/* Get real bus number */
			real_bus_count = strtol(ent->d_name + 4, &retstr, 10);
			if (*retstr != '\0') {
				free(data);
				continue;
			}

			do {
				int real_bus_count_next = 0;
				for (i = 0; i < conf_bus_count; i++) {
					if (strncmp(sbArr[i].name, data, strlen(sbArr[i].name)) != 0)
						continue;

					/* First check if everything is sorted
					 * before updating the elements */
					if (sbArr[i].realNr > real_bus_count) {
						real_bus_count_next = sbArr[i].realNr;
						sbArr[i].realNr = real_bus_count;
					} else {
						if (units[i] <= sbArr[i].unit) {
							sbArr[i].realNr = real_bus_count;
							if (units[i] == sbArr[i].unit)
								sbArrCount++;
							units[i]++;
						}
					}
				}
				real_bus_count = real_bus_count_next;
			} while (real_bus_count);

			free(data);
		}
		free(units);

		if (sbArrCount != conf_bus_count) {
			sbArrCount = 0;
			ret = KEAPI_RET_RETRIEVAL_ERROR;
			goto exit;
		}

		ret = KEAPI_RET_SUCCESS;
		goto exit;
	}

	/* if we get an ERROR during reading configuration file (not parsing!)
	 * we assume that configuration file is not exist or empty,
	 * so fall back to old looking for serial busses */
	/* Count i2c-x files in /sys/bus/i2c/devices/ */
fs_scan:
	real_bus_count = sbArrCount = 0;
	snprintf(substr, KEAPI_MAX_STR, "/sys/bus/i2c/devices/i2c-%d/name", real_bus_count);
	while (access(substr, F_OK) == 0) {
		if ((ret = checkRAccess(substr)) != KEAPI_RET_SUCCESS)
			goto exit;

		if ((ret = ReadFile(substr, &data)) == KEAPI_RET_SUCCESS) {
			if (strncmp("SMBus", data, strlen("SMBus")) == 0) {
				/* SMBus found */
				if (sbType == SMBUS_CONFTYPE) { /* if looking for smbus */
					sbArr = realloc(sbArr, sizeof(struct SerialBusInfo) * (sbArrCount + 1));
					sbArr[sbArrCount].realNr = real_bus_count;
					sbArr[sbArrCount].unit = 0;
					sbArr[sbArrCount].name[0] = '\0';
					strncat(sbArr[sbArrCount].name, data, KEAPI_MAX_STR - 1);
					sbArrCount++;
				}
			} else if (sbType == I2C_CONFTYPE) { /* if bus is not smbus and looking for i2c */
				/* I2C found */
				sbArr = realloc(sbArr, sizeof(struct SerialBusInfo) * (sbArrCount + 1));
				sbArr[sbArrCount].realNr = real_bus_count;
				sbArr[sbArrCount].unit = 0;
				sbArr[sbArrCount].name[0] = '\0';
				strncat(sbArr[sbArrCount].name, data, KEAPI_MAX_STR - 1);
				sbArrCount++;
			}
			free(data);
			real_bus_count++;
			snprintf(substr, KEAPI_MAX_STR, "/sys/bus/i2c/devices/i2c-%d/name", real_bus_count);
		} else
			goto exit;
	}
	ret = KEAPI_RET_SUCCESS;

	/* setup global variables */
exit:
	switch (sbType) {
	case I2C_CONFTYPE:
		i2cArr = sbArr;
		i2cArrCount = sbArrCount;
		break;
	case SMBUS_CONFTYPE:
		smbusArr = sbArr;
		smbusArrCount = sbArrCount;
		break;
	}

	/*free mutex and exit*/
error:
	switch (sbType) {
	case I2C_CONFTYPE:
		mf_ret = pthread_mutex_unlock(&i2c_mutex);
		break;
	case SMBUS_CONFTYPE:
		mf_ret = pthread_mutex_unlock(&smbus_mutex);
		break;
	}

	if (ret != KEAPI_RET_SUCCESS)
		return ret;

	if (mf_ret)
		return KEAPI_RET_ERROR;

	return KEAPI_RET_SUCCESS;
}

/*******************************************************************************/
int32_t GetStorageConfig(void)
{
	char buf_str[KEAPI_MAX_STR];
	char *data;
	int i = 0, k, ret;
	int eep_count, blCnt;
	struct EepUsr *eepUsr;

	/* root, json storage array, json estorage element, json data */
	json_t *jstarr, *jstel, *jdata, *root = NULL;
	json_error_t jerror;

	ret = pthread_mutex_trylock(&eep_mutex);
	if (ret)
		return KEAPI_RET_BUSY_COLLISION;

	/* trying reading configuration file*/
	/* storage EEPROM configuration files are located in /etc/keapi/ */
	if ((ret = checkRAccess(STORAGE_EEPROM_PATH)) != KEAPI_RET_SUCCESS)
		goto conf_error;

	ret = KEAPI_RET_READ_ERROR;
	root = json_load_file(STORAGE_EEPROM_PATH, JSON_DECODE_ANY, &jerror);
	if (!root)
		goto conf_error;

	siArrCount = eep_count = 0;

	jstarr = json_object_get(root, "storage");
	if (!json_is_array(jstarr))
		goto conf_error;

	/* get quantity of described storages */
	eep_count = json_array_size(jstarr);

	/* iterate them */
	for (i = 0; i < eep_count; i++) {
		jstel = json_array_get(jstarr, i);
		if (!json_is_object(jstel))
			goto conf_error;

		siArr = realloc(siArr, sizeof(struct StorageInfo) * (i + 1));
		siArr[i].path[0] = '\0';
		siArr[i].eepUsr = NULL;
		siArr[i].picmg_flag = FALSE;
		siArr[i].usr_flag = FALSE;

		jdata = json_object_get(jstel, "path");
		if (!json_is_string(jdata))
			goto conf_error;
		strncat(siArr[i].path, json_string_value(jdata), KEAPI_MAX_STR - 1);

		buf_str[0] = '\0';
		jdata = json_object_get(jstel, "type");
		if (json_is_string(jdata))
			strncat(buf_str, json_string_value(jdata), KEAPI_MAX_STR - 1);

		/* Storage EEPROM info files are located in /etc/keapi/
		 * They contain 1. Path to EEPROM file, 2. start offset, 3. size
		 * OR They contain 1. Path to EEPROM file, 2. picmg label, - that means
		 * we should to parse eeprom data and look for User Data Area Structure
		 */

		/* If picmg label is found, trying to parse picmg table
		 * and pull out usr data offsets*/
		if (!strcmp(buf_str, "picmg") || !strcmp(buf_str, "PICMG")) {
			siArr[i].picmg_flag = TRUE;

			ret = ReadFile(siArr[i].path, &data);
			if (ret != KEAPI_RET_SUCCESS)
				goto read_error;

			ret = KEAPI_RET_READ_ERROR;

			if (!EepromIsPicmg(data)) {
				free(data);
				goto read_error;
			}

			struct BlockHead *blockHeads;
			int32_t blockCount;

			blockCount = GetDynamicBlockCount(data);

			blockHeads = calloc(blockCount, sizeof(struct BlockHead));
			blCnt = GetDynamicBlockData(data, blockHeads, blockCount);
			if (blCnt != blockCount) {
				free(blockHeads);
				free(data);
				goto read_error;
			}

			for (k = 0; k < blockCount; k++)
				if (blockHeads[k].id == EEEP_BLOCK_ID_VENDOR_SPECIFIC) {
					eepUsr =
						TryToGetUserDataArea(&data[blockHeads[k].offset], blockHeads[k].offset);
					if (eepUsr) {
						siArr[i].eepUsr = eepUsr;
						siArr[i].usr_flag = TRUE;
						break;
					}
				}

			free(blockHeads);
			free(data);

			/* If picmg label was not found, trying to get start offset and size
			 * from configuration file */
		} else {
			eepUsr = calloc(1, sizeof(struct EepUsr));
			if (!eepUsr)
				goto conf_error;

			siArr[i].picmg_flag = FALSE;
			ret = KEAPI_RET_READ_ERROR;

			/* trying to get storage start offset from json configuration file*/
			jdata = json_object_get(jstel, "start");
			if (!json_is_string(jdata)) {
				free(eepUsr);
				goto conf_error;
			}

			eepUsr->start = strtoul(json_string_value(jdata), NULL, 16);

			if (eepUsr->start < 0) {
				free(eepUsr);
				goto conf_error;
			}

			/* trying to get storage size from json configuration file*/
			jdata = json_object_get(jstel, "size");
			if (!json_is_string(jdata)) {
				free(eepUsr);
				goto conf_error;
			}

			eepUsr->size = strtoul(json_string_value(jdata), NULL, 16);
			if (eepUsr->size <= 0) {
				free(eepUsr);
				goto conf_error;
			}

			siArr[i].eepUsr = eepUsr;
			siArr[i].usr_flag = TRUE;
		}

	} /* for (i = 0; i < eep_count; i++) { */

	siArrCount = eep_count;

	json_decref(root);
	pthread_mutex_unlock(&eep_mutex);

	return KEAPI_RET_SUCCESS;

read_error:

	/* If we have more than one eeprom are described in configuration file
	 * and it is parsed well without errors, and at least 1 eeprom is
	 * accessable think that it is ok and return PARTIAL_SUCCESS,
	 * as some eeproms may be optional, for example on carrier board.
	 */
	if (i > 0) {
		json_decref(root);
		if (siArr[i].eepUsr)
			free(siArr[i].eepUsr);

		siArr = realloc(siArr, sizeof(struct StorageInfo) * i);
		siArrCount = i;

		pthread_mutex_unlock(&eep_mutex);
		return KEAPI_RET_PARTIAL_SUCCESS;
	}

conf_error:
	json_decref(root);

	if (siArr) {
		for (k = 0; k <= i; k++)
			if (siArr[k].eepUsr)
				free(siArr[k].eepUsr);

		free(siArr);
		siArr = NULL;
	}
	siArrCount = 0;

	pthread_mutex_unlock(&eep_mutex);
	return ret;
}

/*******************************************************************************/
int8_t IsSensorIgnored(char *hwmonName, char *sensorName)
{
	/* Get information whether the sensor should be ignored or not from the
	 * sensors.conf file */

	int8_t retval = FALSE;
	char path[KEAPI_MAX_STR], chip[KEAPI_MAX_STR], *info;
	char regex[KEAPI_MAX_STR], *substr, *sensors_conf;
	int32_t rc, len;
	int32_t ovector[30];
	pcre *re, *re2;
	const char *error;
	int erroffset;

	snprintf(path, KEAPI_MAX_STR, "/sys/class/hwmon/%s/device/name", hwmonName);
	/* if "device" subdir do not exist, we will try to look for hwmon
	 * attributes on the level up (top dir a hwmon device) */
	if (access(path, F_OK) != 0)
		snprintf(path, KEAPI_MAX_STR, "/sys/class/hwmon/%s/name", hwmonName);

	if (ReadFile(path, &info) == KEAPI_RET_SUCCESS) {
		strncpy(chip, info, KEAPI_MAX_STR);
		free(info);

		/* Get rid of \n at the end */
		if (chip[strnlen(chip, KEAPI_MAX_STR) - 1] == '\n')
			chip[strnlen(chip, KEAPI_MAX_STR) - 1] = '\0';

		/* Read names from sensors.conf file */
		if (ReadFile(GetSensorsConfName(), &info) == KEAPI_RET_SUCCESS) {
			/* Create regex */
			snprintf(regex, KEAPI_MAX_STR, "(%s\\-\\*.*?)(?:^chip|\\Z)", chip);
			if ((re = pcre_compile(regex, PCRE_MULTILINE | PCRE_NEWLINE_ANYCRLF | PCRE_DOTALL, &error,
					       &erroffset, NULL)) == NULL) {
				free(info);
				return retval;
			}

			if ((rc = pcre_exec(re, NULL, info, strlen(info), 0, 0, ovector, 30)) == 2) {
				/* Find appropriate part of sensors file */
				len = ovector[3] - ovector[2];
				substr = (char *)malloc(len + 1);
				memcpy(substr, info + ovector[2], len);
				substr[len] = '\0';
				sensors_conf = strdup(substr);
				free(substr);

				/* Find sensor name in it */
				snprintf(regex, KEAPI_MAX_STR, "ignore %s", sensorName);
				if ((re2 = pcre_compile(regex, PCRE_MULTILINE | PCRE_NEWLINE_ANYCRLF | PCRE_DOTALL,
							&error, &erroffset, NULL)) == NULL) {
					pcre_free(re);
					free(info);
					return retval;
				}

				if ((rc = pcre_exec(re2, NULL, sensors_conf, strlen(sensors_conf), 0, 0, ovector,
						    30)) == 1) {
					/*ovector[1] - next character after the substring that is was found */
					if ((sensors_conf[ovector[1]] == ' ') || (sensors_conf[ovector[1]] == '\0') ||
					    (sensors_conf[ovector[1]] == '\n') || (sensors_conf[ovector[1]] == '\r'))
						retval = TRUE;
				}

				pcre_free(re2);

				free(sensors_conf);
			}

			pcre_free(re);
			free(info);
		}
	}

	return retval;
}

/*******************************************************************************/
void *mem_chunk(size_t base, size_t len, const char *devmem)
{
	void *p;
	int fd;
	size_t mmoffset;
	void *mmp;

	if ((fd = open(devmem, O_RDONLY)) == -1)
		return NULL;

	if ((p = malloc(len)) == NULL)
		return NULL;

	mmoffset = base % getpagesize();
	mmp = mmap(0, mmoffset + len, PROT_READ, MAP_SHARED, fd, base - mmoffset);
	if (mmp == MAP_FAILED) {
		fprintf(stderr, "%s: ", devmem);
		perror("mmap");
		free(p);
		return NULL;
	}

	memcpy(p, (char *)mmp + mmoffset, len);

	if (munmap(mmp, mmoffset + len) == -1) {
		fprintf(stderr, "%s: ", devmem);
		perror("munmap");
	}

	close(fd);

	return p;
}

/*******************************************************************************/

/* Looking for EFI interface */
static int smbios_address_from_efi(size_t *address)
{
	FILE *efi_systab;
	const char *filename;
	char linebuf[64];
	int ret;

	*address = 0;

	/*
	 * Linux up to 2.6.6: /proc/efi/systab
	 * Linux 2.6.7 and up: /sys/firmware/efi/systab
	 */
	if ((ret = checkRAccess("/sys/firmware/efi/systab")) != KEAPI_RET_SUCCESS &&
	    (ret = checkRAccess("/proc/efi/systab")) != KEAPI_RET_SUCCESS)
		return ret;

	if ((efi_systab = fopen(filename = "/sys/firmware/efi/systab", "r")) == NULL &&
	    (efi_systab = fopen(filename = "/proc/efi/systab", "r")) == NULL) {
		/* No EFI interface, fallback to memory scan */
		return KEAPI_RET_RETRIEVAL_ERROR;
	}

	ret = KEAPI_RET_READ_ERROR; /* if we cannot find smbios info */
	while ((fgets(linebuf, sizeof(linebuf) - 1, efi_systab)) != NULL) {
		char *addrp = strchr(linebuf, '=');
		*(addrp++) = '\0';

		if (strcmp(linebuf, "SMBIOS") == 0) {
			*address = strtoul(addrp, NULL, 0);
			ret = KEAPI_RET_SUCCESS;
			break;
		}
	}

	if (fclose(efi_systab) != 0) {
		perror(filename);
	}

	return ret;
}

/*******************************************************************************/
uint32_t getDmiTable(char **table, uint16_t *table_len, uint16_t *struct_count)
{
	size_t fp;
	int32_t ret;
	uint8_t *buf = NULL;
	uint8_t *eps = NULL;

	/* check if it is efi*/
	ret = smbios_address_from_efi(&fp);

	if (ret != KEAPI_RET_SUCCESS && ret != KEAPI_RET_RETRIEVAL_ERROR) {
		return ret;

	} else if (ret == KEAPI_RET_SUCCESS) { /* efi was found */

		if ((ret = checkRAccess("/dev/mem")) != KEAPI_RET_SUCCESS)
			return ret;

		if ((buf = mem_chunk(fp, 0x1000, "/dev/mem")) == NULL)
			return KEAPI_RET_ERROR;

		eps = buf;
	} else { /* if it is efi without SMBIOS info
		  * fallback to legacy x86 SMBIOS searching */
		/* Grab a piece of memory */

		if ((ret = checkRAccess("/dev/mem")) != KEAPI_RET_SUCCESS)
			return ret;

		if ((buf = mem_chunk(0xF0000, 0x10000, "/dev/mem")) == NULL)
			return KEAPI_RET_ERROR;

		/* Try to find SMBios Entry Point Structure */
		for (fp = 0; fp <= 0xFFF0; fp += 16) {
			if (memcmp(buf + fp, "_SM_", 4) == 0 && fp <= 0xFFE0 &&
			    memcmp(buf + fp + 0x10, "_DMI_", 5) == 0) {
				eps = buf + fp;
				break;
			}
		}

		if (eps == NULL) /* ret = EFI_NO_SMBIOS or no EPS found */ {
			free(buf);
			return KEAPI_RET_ERROR;
		}
	}

	*struct_count = *(uint16_t *)(eps + 0x1C);
	*table_len = *(uint32_t *)(eps + 0x16);

	/* Get the table with structures */
	if ((*table = (char *)mem_chunk((size_t)(*(uint32_t *)(eps + 0x18)), *(uint16_t *)(eps + 0x16), "/dev/mem")) ==
	    NULL) {
		free(buf);
		return KEAPI_RET_ERROR;
	}

	if (buf)
		free(buf);

	return KEAPI_RET_SUCCESS;
}

/*******************************************************************************/
int32_t ReadFile(char *filename, char **outbuff)
{
	int32_t fd, amount, bufflen;
	char buff[1024], *newbuff;

	if ((fd = open(filename, O_RDONLY)) < 0)
		return KEAPI_RET_ERROR;

	*outbuff = NULL;
	bufflen = 0;
	while ((amount = read(fd, buff, sizeof(buff))) > 0) {
		if ((newbuff = realloc(*outbuff, bufflen + amount + 1)) == NULL) /* +1 is for '\0' */ {
			close(fd);
			return (KEAPI_RET_ERROR);
		}
		*outbuff = newbuff;
		memcpy(*outbuff + bufflen, buff, amount);
		bufflen += amount;
	}
	close(fd);

	if (amount < 0) /* if filename is directory, it can be opened but read() returns -1 */
		return KEAPI_RET_ERROR;

	if (bufflen == 0) /* we also need to handle properly 0-size file */
		if ((*outbuff = malloc(1)) == NULL)
			return (KEAPI_RET_ERROR);

	(*outbuff)[bufflen] = '\0';
	return KEAPI_RET_SUCCESS;
}

/*******************************************************************************/
int32_t WriteFile(char *path, char *data)
{
	FILE *fp;
	int32_t ret;

	if ((ret = checkRWAccess(path)) != KEAPI_RET_SUCCESS)
		return ret;

	fp = fopen(path, "w");
	if (!fp)
		return KEAPI_RET_ERROR;

	ret = fwrite(data, 1, strlen(data), fp);
	if (ret > 0) {
		ret = fflush(fp);
		if (ret == 0)
			ret = KEAPI_RET_SUCCESS;
		else
			ret = KEAPI_RET_ERROR;
	} else {
		ret = KEAPI_RET_ERROR;
	}

	fclose(fp);
	return ret;
}				     


#define BUF_SIZE 1024

int32_t GetExternalCommandOutput(char *command, char **data)
{
	int32_t bufsize, amount;
	char buf[BUF_SIZE], *mainbuf;
	FILE *fp;

	if (command == NULL || data == NULL)
		return KEAPI_RET_PARAM_NULL;

	// try to execute command command
	fp = popen(command, "r");
	if (!fp)
		return KEAPI_RET_RETRIEVAL_ERROR;

	// read command output
	bufsize = 0;
	mainbuf = NULL;
	while ((amount = fread(buf, sizeof(char), BUF_SIZE, fp)) > 0) {
		if ((mainbuf = realloc(mainbuf, bufsize + amount + 1)) == NULL) {
			pclose(fp);
			return KEAPI_RET_MALLOC_ERROR;
		}
		memcpy(mainbuf + bufsize, buf, amount);
		bufsize += amount;
	}
	mainbuf[bufsize] = '\0';
	bufsize++;
	pclose(fp);

	*data = mainbuf;

	return KEAPI_RET_SUCCESS;
}
/*******************************************************************************/
int32_t FindRegex(char *str, char *regex, int8_t *found, int32_t flags)
{
	regex_t rx;
	regmatch_t *matches;

	if (regcomp(&rx, regex, flags))
		return KEAPI_RET_ERROR;

	matches = (regmatch_t *)malloc((rx.re_nsub + 1) * sizeof(regmatch_t));

	if (regexec(&rx, str, rx.re_nsub + 1, matches, 0) == 0) //REG_NOTBOL | REG_NOTEOL
		*found = TRUE;
	else
		*found = FALSE;

	regfree(&rx);
	free(matches);

	return KEAPI_RET_SUCCESS;
}

/*******************************************************************************/
int32_t GetSubStrRegex(char *str, char *regex, char **substr, int32_t flags)
{
	regex_t rx;
	regmatch_t *matches;
	int32_t len;

	if (regcomp(&rx, regex, flags))
		return KEAPI_RET_ERROR;

	matches = (regmatch_t *)malloc((rx.re_nsub + 1) * sizeof(regmatch_t));

	if (regexec(&rx, str, rx.re_nsub + 1, matches, 0) != 0) /* REG_NOTBOL | REG_NOTEOL */
		return KEAPI_RET_RETRIEVAL_ERROR;

	/* Take the second result */
	if (matches[0].rm_so != -1 && matches[1].rm_so != -1) {
		len = matches[1].rm_eo - matches[1].rm_so;
		*substr = (char *)malloc(len + 1);
		strncpy(*substr, str + matches[1].rm_so, len);
		(*substr)[len] = '\0';
	} else
		return KEAPI_RET_RETRIEVAL_ERROR;

	regfree(&rx);
	free(matches);

	return KEAPI_RET_SUCCESS;
}

/*******************************************************************************/
char *GetSensorsConfName(void)
{
	if (checkRAccess(SENSORSCONFDIR_PATH) == KEAPI_RET_SUCCESS)
		return SENSORSCONFDIR_PATH;
	else if (checkRAccess(SENSORSCONF_PATH) == KEAPI_RET_SUCCESS)
		return SENSORSCONF_PATH;

	return SENSORSCONFLEGACY_PATH;
}
