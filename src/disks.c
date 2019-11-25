/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron AG */

/* KEAPI disk drives info implementation. */

#include <stdio.h>
#include <dirent.h>
#include <regex.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <pcre.h>

#include <sys/statvfs.h>
#include <sys/ioctl.h>
#include <linux/fs.h>
#include <linux/hdreg.h>
#include <atasmart.h>

#include "keapi_inc.h"
#include "ids_database.h"

#define REGEX_BLOCK_DISK_TEMPLATE "[hs]d.|mmcblk[0-9]+$|nvme[0-9]+n[0-9]+$"
#define REGEX_BLOCK_PART_TEMPLATE "[hs]d.|mmcblk[0-9]+$|nvme[0-9]+n[0-9]+$|mtdblock[0-9]+$|nvme[0-9]+n[0-9]+p[0-9]+$"
#define REGEX_WWID "\\S*\\s+([\\w]*)\\s+([ \\w\\-\\+]+)\\s([\\w\\-\\+]+)\\s+$"

/*******************************************************************************/
KEAPI_RETVAL KEApiL_GetDiskDriveCount(int32_t *pDiskDriveCount)
{
	DIR *dir;
	struct dirent *ent;
	int8_t found;
	int32_t ret;

	*pDiskDriveCount = 0;
	/* Check function parameters */
	if (pDiskDriveCount == NULL)
		return KEAPI_RET_PARAM_NULL;

	if ((ret = checkRAccess("/sys/block")) != KEAPI_RET_SUCCESS) {
		if (ret == KEAPI_RET_RETRIEVAL_ERROR)
			return KEAPI_RET_SUCCESS; /* no directory => no disks
						   * (may be block devs disabled in the kernel) */
		else
			return ret;
	}

	if ((dir = opendir("/sys/block")) == NULL)
		return KEAPI_RET_ERROR;

	/* Count directories sd* and hd* or mmcblk* */
	while ((ent = readdir(dir)) != NULL) {
		if (FindRegex(ent->d_name, REGEX_BLOCK_DISK_TEMPLATE, &found, REG_EXTENDED | REG_NEWLINE) !=
		    KEAPI_RET_SUCCESS) {
			closedir(dir);
			return KEAPI_RET_ERROR;
		}
		if (found) {
			(*pDiskDriveCount)++;
		}
	}

	closedir(dir);
	return KEAPI_RET_SUCCESS;
}

/*******************************************************************************/
/* KEApiL_GetDiskDriveList
 * These helper functions implements different ways to obtain information
 * (model, serial number, etc) about disk drives.
*/
KEAPI_RETVAL GetDiskInfo_wwid(PKEAPI_DISK_DRIVE pDiskDrive)
{
	char path[PATH_MAX], *info;
	pcre *re;
	const char *error;
	int erroffset;
	int32_t rc;
	int32_t ovector[30];
	char *tmp_str;
	int len;

	snprintf(path, PATH_MAX, "/sys/block/%s/device/wwid", pDiskDrive->name);

	if (checkRAccess(path) == KEAPI_RET_SUCCESS && ReadFile(path, &info) == KEAPI_RET_SUCCESS) {
		/* TODO: Is the WWID a fixed offset format?
		if (strlen(info) == 73) {
			strncpy(pDiskDrive->model, info + 12, 40);
			strncpy(pDiskDrive->diskSerialNumber, info + 52, 20);
		} else
		*/
		if ((re = pcre_compile(REGEX_WWID, PCRE_NEWLINE_ANYCRLF | PCRE_DOTALL, &error, &erroffset, NULL)) !=
		    NULL) {
			rc = pcre_exec(re, NULL, info, strlen(info), 0, 0, ovector, 30);
			if (rc > 0) {
				len = 0;
				tmp_str = (char *)malloc(KEAPI_MAX_STR * sizeof(char));
				memcpy(tmp_str, info + ovector[2], ovector[3] - ovector[2]);
				len += ovector[3] - ovector[2];
				if (len > 0) {
					memcpy(tmp_str + len, " ", 1);
					len += 1;
				}
				memcpy(tmp_str + len, info + ovector[4], ovector[5] - ovector[4]);
				len += ovector[5] - ovector[4];
				tmp_str[len] = '\0';
				strncpy(pDiskDrive->model, tmp_str, len + 1);
				free(tmp_str);
				memcpy(pDiskDrive->diskSerialNumber, info + ovector[6], ovector[7] - ovector[6]);
				pDiskDrive->diskSerialNumber[ovector[7] - ovector[6]] = '\0';
			}
			pcre_free(re);
			return KEAPI_RET_SUCCESS;
		} else {
			return KEAPI_RET_RETRIEVAL_ERROR;
		}
	} else {
		return KEAPI_RET_READ_ERROR;
	}
}

KEAPI_RETVAL GetDiskInfo_vendor_model(PKEAPI_DISK_DRIVE pDiskDrive)
{
	char path[PATH_MAX], *info;
	int32_t ret_vendor, ret_model;

	/* vendor */
	snprintf(path, PATH_MAX, "/sys/block/%s/device/vendor", pDiskDrive->name);

	ret_vendor = checkRAccess(path);
	if (ret_vendor == KEAPI_RET_SUCCESS) {
		if ((ret_vendor = ReadFile(path, &info)) != KEAPI_RET_SUCCESS) {
			return ret_vendor;
		}

		strncat(pDiskDrive->model, info, KEAPI_MAX_STR - 1);
		free(info);
	}

	/* Remove \n at the end */
	if (strlen(pDiskDrive->model) > 0 && pDiskDrive->model[strlen(pDiskDrive->model) - 1] == '\n')
		pDiskDrive->model[strlen(pDiskDrive->model) - 1] = '\0';

	/* Remove spaces at the end */
	while (strlen(pDiskDrive->model) > 0 && pDiskDrive->model[strlen(pDiskDrive->model) - 1] == ' ')
		pDiskDrive->model[strlen(pDiskDrive->model) - 1] = '\0';

	/* Append model */
	snprintf(path, PATH_MAX, "/sys/block/%s/device/model", pDiskDrive->name);

	ret_model = checkRAccess(path);
	if (ret_model == KEAPI_RET_SUCCESS) {
		if ((ret_model = ReadFile(path, &info)) != KEAPI_RET_SUCCESS) {
			return ret_model;
		}

		/* Add " " */
		const char *hyphen = " ";
		int left_len, info_len;
		if (strlen(pDiskDrive->model))
			strncat(pDiskDrive->model, hyphen, KEAPI_MAX_STR - 1);
		info_len = strlen(info);
		left_len = KEAPI_MAX_STR - strlen(pDiskDrive->model) - 1;
		strncat(pDiskDrive->model, info, info_len > left_len ? left_len : info_len);
		free(info);
	}

	if (ret_vendor == KEAPI_RET_RETRIEVAL_ERROR || ret_model == KEAPI_RET_RETRIEVAL_ERROR) {
		if (ret_vendor == KEAPI_RET_RETRIEVAL_ERROR && ret_model == KEAPI_RET_RETRIEVAL_ERROR) {
			strncpy(pDiskDrive->model, "unknown", KEAPI_MAX_STR - 1);
		}
		return KEAPI_RET_PARTIAL_SUCCESS;
	}
	return KEAPI_RET_SUCCESS;
}

KEAPI_RETVAL GetDiskInfo_cid_register(PKEAPI_DISK_DRIVE pDiskDrive)
{
	/* (See: mmc-dev-attrs.txt in linux docs) */

	char path[PATH_MAX], *info;
	int32_t ret_vendor, ret_model;

	if (strncmp("mmcblk", pDiskDrive->name, strlen("mmcblk")) != 0)
		return KEAPI_RET_RETRIEVAL_ERROR;

	/* manfid	Manufacturer ID (from CID Register) */
	snprintf(path, PATH_MAX, "/sys/block/%s/device/manfid", pDiskDrive->name);

	ret_vendor = checkRAccess(path);
	if (ret_vendor == KEAPI_RET_SUCCESS) {
		if ((ret_vendor = ReadFile(path, &info)) != KEAPI_RET_SUCCESS) {
			return ret_vendor;
		}
		/* convert string with hex value to int */
		int manfid = (int)strtol(info, NULL, 16);
		free(info);

		uint8_t ids_cnt = sizeof(database) / sizeof(struct ids_database);
		uint8_t i;
		for (i = 0; i < ids_cnt; i++) {
			if (manfid == database[i].id) {
				strncpy(pDiskDrive->model, database[i].manufacturer, KEAPI_MAX_STR - 1);
				break;
			}
		}
		if (i == ids_cnt) {
			pDiskDrive->model[0] = '\0';
			ret_vendor = KEAPI_RET_RETRIEVAL_ERROR;
		}

	} else if (ret_vendor != KEAPI_RET_RETRIEVAL_ERROR) {
		return ret_vendor;
	}

	// name		Product Name (from CID Register)
	snprintf(path, PATH_MAX, "/sys/block/%s/device/name", pDiskDrive->name);

	ret_model = checkRAccess(path);
	if (ret_model == KEAPI_RET_SUCCESS) {
		if ((ret_model = ReadFile(path, &info)) != KEAPI_RET_SUCCESS) {
			return ret_model;
		}
		/* Add " " */
		const char *hyphen = " ";
		int left_len, info_len;
		if (strlen(pDiskDrive->model))
			strncat(pDiskDrive->model, hyphen, KEAPI_MAX_STR - 1);
		info_len = strlen(info);
		left_len = KEAPI_MAX_STR - strlen(pDiskDrive->model) - 1;
		strncat(pDiskDrive->model, info, info_len > left_len ? left_len : info_len);
		free(info);

	} else if (ret_model != KEAPI_RET_RETRIEVAL_ERROR) {
		return ret_model;
	}

	if (ret_vendor == KEAPI_RET_RETRIEVAL_ERROR || ret_model == KEAPI_RET_RETRIEVAL_ERROR) {
		if (ret_vendor == KEAPI_RET_RETRIEVAL_ERROR && ret_model == KEAPI_RET_RETRIEVAL_ERROR) {
			strncpy(pDiskDrive->model, "unknown", KEAPI_MAX_STR - 1);
		}
		return KEAPI_RET_PARTIAL_SUCCESS;
	}
	return KEAPI_RET_SUCCESS;
}

KEAPI_RETVAL GetDiskInfo_serial(PKEAPI_DISK_DRIVE pDiskDrive)
{
	char path[PATH_MAX], *info;
	int32_t fd, ret;

	if ((strncmp("mmcblk", pDiskDrive->name, strlen("mmcblk")) == 0) ||
	    (strncmp("nvme", pDiskDrive->name, strlen("nvme")) == 0)) {
		/* for S/N look for "serial" attribute in sysfs*/
		snprintf(path, PATH_MAX, "/sys/block/%s/device/serial", pDiskDrive->name);

		if ((ret = checkRAccess(path)) == KEAPI_RET_SUCCESS) {
			if ((ret = ReadFile(path, &info)) == KEAPI_RET_SUCCESS) {
				/* Remove \n at the end */
				if (strlen(info) > 0 && info[strlen(info) - 1] == '\n')
					info[strlen(info) - 1] = '\0';

				/* Remove spaces at the end */
				while (strlen(info) > 0 && info[strlen(info - 1)] == ' ')
					info[strlen(info) - 1] = '\0';

				strncat(pDiskDrive->diskSerialNumber, info, KEAPI_MAX_STR - 1);
				free(info);
			}
		}
		/* for non mmc device */
	} else {
		struct hd_driveid id;

		snprintf(path, PATH_MAX, "/dev/%s", pDiskDrive->name);

		ret = checkRAccess(path);
		if (ret == KEAPI_RET_SUCCESS) {
			if ((fd = open(path, O_RDONLY | O_NONBLOCK)) <= 0) {
				return ret;
			}

			errno = 0;
			ret = ioctl(fd, HDIO_GET_IDENTITY, &id);
			if (ret >= 0) {
				strncpy(pDiskDrive->diskSerialNumber, (char *)id.serial_no, 20);
				pDiskDrive->diskSerialNumber[20] = '\0';
			} else {
				/* if ioctl is unsupported - it is OK */
				if (errno != EINVAL) {
					close(fd);
					return failIoctlCheckErrno(&fd);
				}
			}
			close(fd);
		}
	}
	return ret;
}

KEAPI_RETVAL KEApiL_GetDiskDriveList(PKEAPI_DISK_DRIVE pDiskDrives, int32_t diskDriveCount)
{
	DIR *dir;
	struct dirent *ent;
	int8_t found;
	int32_t index, fd, ret, real_disk_count = 0;
	char path[PATH_MAX], *info;

	/* Check function parameters */
	if (pDiskDrives == NULL)
		return KEAPI_RET_PARAM_NULL;

	if ((ret = KEApiL_GetDiskDriveCount(&real_disk_count)) != KEAPI_RET_SUCCESS)
		return ret;

	if (diskDriveCount < 0 || diskDriveCount > real_disk_count)
		return KEAPI_RET_PARAM_ERROR;

	if (diskDriveCount == 0) {
		if (real_disk_count == 0)
			return KEAPI_RET_SUCCESS;
		else
			return KEAPI_RET_PARTIAL_SUCCESS;
	}

	if ((ret = checkRAccess("/sys/block")) != KEAPI_RET_SUCCESS)
		return ret;

	if ((dir = opendir("/sys/block")) == NULL)
		return KEAPI_RET_ERROR;

	memset(pDiskDrives, 0, sizeof(KEAPI_DISK_DRIVE) * diskDriveCount);

	/* Directories sd* and hd* or mmcblk* */
	index = 0;
	while ((ent = readdir(dir)) != NULL && index < diskDriveCount) {
		if (FindRegex(ent->d_name, REGEX_BLOCK_DISK_TEMPLATE, &found, REG_EXTENDED | REG_NEWLINE) !=
		    KEAPI_RET_SUCCESS) {
			closedir(dir);
			return KEAPI_RET_ERROR;
		}

		if (found) {
			/* DISK NAME */
			strncat(pDiskDrives[index].name, ent->d_name, KEAPI_MAX_STR - 1);
			// strncat(pDiskDrives[index].model, "unknown", KEAPI_MAX_STR-1);

			/* MODEL and SERIAL
			 *
			 * try to use wwid in sysfs;
			 * if fails try to use vendor and model in sysfs;
			 * if fails try to parse vpd_pg83 (FIXME: not implemented yet!)
			 * if fails try to use cid and manfid in sysfs (for mmc/sd only);
			 * */
			if (GetDiskInfo_wwid(&pDiskDrives[index]) != KEAPI_RET_SUCCESS) {
				ret = GetDiskInfo_vendor_model(&pDiskDrives[index]);
				if (ret != KEAPI_RET_SUCCESS) {
					GetDiskInfo_cid_register(&pDiskDrives[index]);
				}
				GetDiskInfo_serial(&pDiskDrives[index]);
			}

			// FIXME: should return "n/a" string if unable to fetch valid num ?
			//			/* if did not find any valid information */
			//			if(pDiskDrives[index].diskSerialNumber[0] == '\0')
			//			    strcat(pDiskDrives[index].diskSerialNumber, "n/a");

			/* fetching SIZE */
			/* look for "size" attribute in sysfs*/
			snprintf(path, PATH_MAX, "/sys/block/%s/size", ent->d_name);

			if ((ret = checkRAccess(path)) == KEAPI_RET_SUCCESS) {
				if ((ret = ReadFile(path, &info)) != KEAPI_RET_SUCCESS) {
					closedir(dir);
					return ret;
				}

				pDiskDrives[index].size = (long)(((atol(info) / 1024) * 512) / 1024);
				free(info);

				/* if size attr was not found, try old method */
			} else if (ret == KEAPI_RET_RETRIEVAL_ERROR) {
				unsigned long numblocks;

				snprintf(path, PATH_MAX, "/dev/%s", ent->d_name);

				if ((ret = checkRAccess(path)) == KEAPI_RET_SUCCESS) {
					if ((fd = open(path, O_RDONLY | O_NONBLOCK)) <= 0) {
						closedir(dir);
						return ret;
					}

					errno = 0;
					if ((ret = ioctl(fd, BLKGETSIZE, &numblocks)) >= 0) {
						pDiskDrives[index].size = (long)((numblocks / 1024) * 512) / 1024;
					} else if (errno != EINVAL) {
						close(fd);
						closedir(dir);
						return failIoctlCheckErrno(&fd);
					}

					close(fd);

				} else if (ret != KEAPI_RET_RETRIEVAL_ERROR) {
					closedir(dir);
					return ret;
				}

			} else {
				closedir(dir);
				return ret;
			}

			/* Remove \n at the end */
			if (strlen(pDiskDrives[index].model) > 0 &&
			    pDiskDrives[index].model[strlen(pDiskDrives[index].model) - 1] == '\n')
				pDiskDrives[index].model[strlen(pDiskDrives[index].model) - 1] = '\0';
			if (strlen(pDiskDrives[index].diskSerialNumber) > 0 &&
			    pDiskDrives[index].diskSerialNumber[strlen(pDiskDrives[index].diskSerialNumber) - 1] ==
				    '\n')
				pDiskDrives[index].diskSerialNumber[strlen(pDiskDrives[index].diskSerialNumber) - 1] =
					'\0';

			/* Remove spaces at the end */
			while (strlen(pDiskDrives[index].model) > 0 &&
			       pDiskDrives[index].model[strlen(pDiskDrives[index].model) - 1] == ' ')
				pDiskDrives[index].model[strlen(pDiskDrives[index].model) - 1] = '\0';
			while (strlen(pDiskDrives[index].diskSerialNumber) > 0 &&
			       pDiskDrives[index].diskSerialNumber[strlen(pDiskDrives[index].diskSerialNumber) - 1] ==
				       ' ')
				pDiskDrives[index].diskSerialNumber[strlen(pDiskDrives[index].diskSerialNumber) - 1] =
					'\0';

			index++;
		}
	}
	closedir(dir);

	if (index < real_disk_count)
		return KEAPI_RET_PARTIAL_SUCCESS;

	return KEAPI_RET_SUCCESS;
}

#define DF_FIRST_PATH	"/bin/df"
#define DF_SECOND_PATH	"/usr/bin/df"

// df output columns
#define FS_M		0 // Filesystem
#define FSTYPE_M	1 // Filesystem type
#define BSIZE_M		2 // 1K-blocks
#define USEDS_M		3 // Used
#define AVLBL_M		4 // Available
#define USEDP_M		5 // Use%
#define MP_M		6 // Mounted on

/*******************************************************************************/
KEAPI_RETVAL GetDfOutput(char **pdata)
{
	int32_t ret;
	char path[PATH_MAX] = { 0 };

	// check if df binary exist in /bin/ or /usr/bin and if we have access
	ret = checkRAccess(DF_FIRST_PATH);
	if (ret == KEAPI_RET_SUCCESS) {
		snprintf(path, PATH_MAX, "%s%s", DF_FIRST_PATH, " -T");
	} else if (ret == KEAPI_RET_RETRIEVAL_ERROR) {
		ret = checkRAccess(DF_SECOND_PATH);
		if (ret == KEAPI_RET_SUCCESS) {
			snprintf(path, PATH_MAX, "%s%s", DF_SECOND_PATH, " -T");
		} else {
			return ret;
		}
	} else
		return ret;

	// get df output
	if ((ret = GetExternalCommandOutput(path, pdata)) != KEAPI_RET_SUCCESS)
		return ret;

	if (strlen(*pdata) <= 1) {
		free(pdata);
		return KEAPI_RET_RETRIEVAL_ERROR;
	}

	return KEAPI_RET_SUCCESS;
}

KEAPI_RETVAL KEApiL_GetMountPointCount(int32_t *pMountPointCount)
{
	int32_t ret, column = 0, matchCnt = 0;
	char *tok, *svptr, *data = NULL;

	if (pMountPointCount == NULL)
		return KEAPI_RET_PARAM_NULL;

	*pMountPointCount = 0;

	if ((ret = GetDfOutput(&data)) != KEAPI_RET_SUCCESS)
		return ret;

	//begin to parse df output

	// skip head of output (1st string)
	tok = strtok_r(data, "\n", &svptr);

	// and take first token
	if (tok)
		tok = strtok_r(NULL, "\n ", &svptr);

	// count useful rows
	// by counting colums, as sometimes 1 item holds several rows
	while (tok != NULL) {
		// increase column counter
		column++;

		// if just caught last one column, reset column counter
		// and increase common match counter
		if (column == MP_M + 1) {
			column = 0;
			matchCnt++;
		}

		tok = strtok_r(NULL, "\n ", &svptr);
	}

	free(data);

	if (matchCnt > 0)
		(*pMountPointCount) = matchCnt;

	return KEAPI_RET_SUCCESS;
}

/*******************************************************************************/
KEAPI_RETVAL KEApiL_GetMountPointList(PKEAPI_MOUNT_POINT pMountPointData, int32_t mountPointCount)
{
	int32_t realMountPointCount, ret;
	char *data;
	char *tok, *svptr;
	int matchCnt = 0, column = 0;

	/* Check function parameters */
	if (pMountPointData == NULL)
		return KEAPI_RET_PARAM_NULL;

	if ((ret = KEApiL_GetMountPointCount(&realMountPointCount)) != KEAPI_RET_SUCCESS)
		return ret;

	if (mountPointCount < 0 || mountPointCount > realMountPointCount)
		return KEAPI_RET_PARAM_ERROR;

	if (mountPointCount == 0) {
		if (realMountPointCount == 0)
			return KEAPI_RET_SUCCESS;
		else
			return KEAPI_RET_PARTIAL_SUCCESS;
	}

	/* Initialization */
	memset(pMountPointData, 0, sizeof(KEAPI_MOUNT_POINT) * mountPointCount);

	if ((ret = GetDfOutput(&data)) != KEAPI_RET_SUCCESS)
		return ret;

	//begin parsing of df output

	// skip head of output (1st string)
	tok = strtok_r(data, "\n", &svptr);

	// and take first token
	if (tok)
		tok = strtok_r(NULL, "\n ", &svptr);

	// parse columns that we need
	while (tok != NULL) {
		// save token in depends on column number
		switch (column) {
		case FSTYPE_M:
			strncat(pMountPointData[matchCnt].fsType, tok, KEAPI_MAX_STR - 1);
			break;
		case BSIZE_M:
			pMountPointData[matchCnt].size = atol(tok) / 1024;
			break;
		case AVLBL_M:
			pMountPointData[matchCnt].freeSpace = atol(tok) / 1024;
			break;
		case MP_M:
			strncat(pMountPointData[matchCnt].name, tok, KEAPI_MAX_STR - 1);
			break;
		case FS_M:
		case USEDS_M:
		case USEDP_M:
			break;
		}

		// increase column counter
		column++;

		// if just caught last one column, reset column counter
		// and increase common match counter
		if (column == MP_M + 1) {
			column = 0;
			matchCnt++;
		}

		tok = strtok_r(NULL, "\n ", &svptr);
	}

	free(data);
	if (mountPointCount < realMountPointCount)
		return KEAPI_RET_PARTIAL_SUCCESS;

	return KEAPI_RET_SUCCESS;
}

/*******************************************************************************/

void readSmartAttrCallback(SkDisk *d, const SkSmartAttributeParsedData *a, void *userdata)
{
	UNUSED(d);

	PKEAPI_SMART_ATTR *pAttrs = userdata;

	(*pAttrs)->attrID = a->id;
	(*pAttrs)->statusFlags = a->flags;
	(*pAttrs)->attrValue = a->current_value;
	(*pAttrs)->worstValue = a->worst_value;
	memcpy((*pAttrs)->rawValue, a->raw, 6);

	(*pAttrs)++;
}

void countSmartAttrCallback(SkDisk *d, const SkSmartAttributeParsedData *a, void *userdata)
{
	UNUSED(d);
	UNUSED(a);

	uint32_t *i = userdata;
	(*i)++;
}

KEAPI_RETVAL KEApiL_GetDiskDriveSMARTAttrCount(int32_t diskNr, int32_t *pAttrCount)
{
	DIR *dir;
	struct dirent *ent;
	int8_t found;

	int32_t real_disk_count, err, cnt = 0;
	uint32_t ret = KEAPI_RET_ERROR;
	SkBool bvar = 0;
	struct SkDisk *smart_dev;
	char dev_name[PATH_MAX];
	dev_name[0] = '\0';

	/* Check function parameters */
	if (pAttrCount == NULL)
		return KEAPI_RET_PARAM_NULL;

	if ((ret = KEApiL_GetDiskDriveCount(&real_disk_count)) != KEAPI_RET_SUCCESS)
		return KEAPI_RET_ERROR;

	if (diskNr < 0 || diskNr >= real_disk_count)
		return KEAPI_RET_PARAM_ERROR;

	if ((ret = getuid()) != 0)
		return KEAPI_RET_PERMISSION_DENIED;

	if ((ret = checkRAccess("/sys/block")) != KEAPI_RET_SUCCESS)
		return ret;

	if ((dir = opendir("/sys/block")) == NULL)
		return KEAPI_RET_ERROR;

	/* Count directories sd* and hd* or mmcblk* */
	while ((ent = readdir(dir)) != NULL) {
		if (FindRegex(ent->d_name, REGEX_BLOCK_DISK_TEMPLATE, &found, REG_EXTENDED | REG_NEWLINE) !=
		    KEAPI_RET_SUCCESS) {
			closedir(dir);
			return KEAPI_RET_ERROR;
		}
		if (found) {
			if (diskNr == cnt) {
				snprintf(dev_name, PATH_MAX, "/dev/%s", ent->d_name);
				break;
			}
			cnt++;
			continue;
		}
	}
	closedir(dir);

	/*Check if we found dev*/
	if (!strlen(dev_name))
		return KEAPI_RET_ERROR;

	/*try to open to read smart info*/
	if ((ret = checkRAccess(dev_name)) != KEAPI_RET_SUCCESS)
		return ret;

	err = sk_disk_open(dev_name, &smart_dev);
	if (err < 0)
		return KEAPI_RET_ERROR;

	/* initialize memory */
	*pAttrCount = 0;

	/*check if is smart info available*/
	err = sk_disk_smart_is_available(smart_dev, &bvar);
	if ((err < 0) || (!bvar)) {
		ret = KEAPI_RET_SUCCESS;
		goto exit0;
	}

	/*if disk is sleeping, we won't wake it*/
	err = sk_disk_check_sleep_mode(smart_dev, &bvar);
	if ((err < 0) || (!bvar))
		goto exit0;

	/* read smart info */
	err = sk_disk_smart_read_data(smart_dev);
	if (err < 0)
		goto exit0;

	/* count attr quantity */
	err = sk_disk_smart_parse_attributes(smart_dev, &countSmartAttrCallback, pAttrCount);
	if (err < 0)
		goto exit0;

	ret = KEAPI_RET_SUCCESS;

exit0:
	sk_disk_free(smart_dev);

	return ret;
}

KEAPI_RETVAL KEApiL_GetDiskDriveSMARTAttrs(int32_t diskNr, PKEAPI_SMART_ATTR pAttrs, int32_t attrCount)
{
	DIR *dir;
	struct dirent *ent;
	int8_t found;

	int32_t real_disk_count, real_attr_count, err, cnt = 0;
	uint32_t ret = KEAPI_RET_ERROR;
	SkBool bvar = 0;
	struct SkDisk *smart_dev;
	char dev_name[PATH_MAX];
	dev_name[0] = '\0';

	KEAPI_SMART_ATTR attrs_buffer[255];
	PKEAPI_SMART_ATTR pab = attrs_buffer;

	if (pAttrs == NULL)
		return KEAPI_RET_PARAM_NULL;

	if ((ret = KEApiL_GetDiskDriveCount(&real_disk_count)) != KEAPI_RET_SUCCESS)
		return ret;

	if (diskNr < 0 || diskNr >= real_disk_count)
		return KEAPI_RET_PARAM_ERROR;

	if ((ret = getuid()) != 0)
		return KEAPI_RET_PERMISSION_DENIED;

	if ((ret = KEApiL_GetDiskDriveSMARTAttrCount(diskNr, &real_attr_count)) != KEAPI_RET_SUCCESS)
		return ret;

	if (attrCount < 0 || attrCount > real_attr_count)
		return KEAPI_RET_PARAM_ERROR;

	if (attrCount == 0) {
		if (real_attr_count == 0)
			return KEAPI_RET_SUCCESS;
		else
			return KEAPI_RET_PARAM_ERROR;
	}

	if ((ret = checkRAccess("/sys/block")) != KEAPI_RET_SUCCESS)
		return ret;

	if ((dir = opendir("/sys/block")) == NULL)
		return KEAPI_RET_ERROR;

	/* Count directories sd* and hd* or mmcblk* */
	while ((ent = readdir(dir)) != NULL) {
		if (FindRegex(ent->d_name, REGEX_BLOCK_DISK_TEMPLATE, &found, REG_EXTENDED | REG_NEWLINE) !=
		    KEAPI_RET_SUCCESS) {
			closedir(dir);
			return KEAPI_RET_ERROR;
		}
		if (found) {
			if (diskNr == cnt) {
				snprintf(dev_name, PATH_MAX, "/dev/%s", ent->d_name);
				break;
			}
			cnt++;
			continue;
		}
	}
	closedir(dir);

	if (!strlen(dev_name))
		return KEAPI_RET_ERROR;

	if ((ret = checkRAccess(dev_name)) != KEAPI_RET_SUCCESS)
		return ret;

	err = sk_disk_open(dev_name, &smart_dev);
	if (err < 0)
		return KEAPI_RET_ERROR;
	err = sk_disk_smart_is_available(smart_dev, &bvar);
	if ((err < 0) || (!bvar))
		goto exit0;

	err = sk_disk_check_sleep_mode(smart_dev, &bvar);
	if ((err < 0) || (!bvar))
		goto exit0;

	err = sk_disk_smart_read_data(smart_dev);
	if (err < 0)
		goto exit0;

	cnt = 0;
	err = sk_disk_smart_parse_attributes(smart_dev, &countSmartAttrCallback, &cnt);
	if (err < 0)
		goto exit0;

	err = sk_disk_smart_parse_attributes(smart_dev, &readSmartAttrCallback, &pab);
	if (err < 0)
		goto exit0;

	/*if allocated memory is less than the attributes we get*/
	if (attrCount < cnt) {
		memcpy(pAttrs, attrs_buffer, sizeof(KEAPI_SMART_ATTR) * attrCount);
		ret = KEAPI_RET_PARTIAL_SUCCESS;
		goto exit0;
	}

	/*if allocated memory is more or equal*/
	memcpy(pAttrs, attrs_buffer, sizeof(KEAPI_SMART_ATTR) * cnt);

	/*if we got attributes less than expected*/
	if (cnt < attrCount) {
		ret = KEAPI_RET_ERROR;
		goto exit0;
	}

	/*default: if all is ok*/
	ret = KEAPI_RET_SUCCESS;

exit0:
	sk_disk_free(smart_dev);

	return ret;
}
