/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron AG */

/* Temperature monitoring functions. */

#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <regex.h>
#include <string.h>
#include <pcre.h>
#include <stdlib.h>

#include "keapi_inc.h"

/*******************************************************************************/
KEAPI_RETVAL KEApiL_GetTempSensorCount(int32_t *pTempSensorCount)
{
	struct dirent *ent, *ent2;
	char path[PATH_MAX];
	char *sensorName, *sens_name_brk;
	DIR *dir, *dir2;
	int8_t found;
	int32_t ret;

	/* Check function parameters */
	if (pTempSensorCount == NULL)
		return KEAPI_RET_PARAM_NULL;

	/* Initialization */
	*pTempSensorCount = 0;

	if ((ret = checkRAccess("/sys/class/hwmon")) != KEAPI_RET_SUCCESS)
		return ret;

	if ((dir = opendir("/sys/class/hwmon")) == NULL)
		return KEAPI_RET_ERROR;

	/* Directories hwmon0, 1, 2... */
	while ((ent = readdir(dir)) != NULL) {
		if (ent->d_name[0] == '.')
			continue;

		snprintf(path, PATH_MAX, "/sys/class/hwmon/%s/name", ent->d_name);
		/* if "name" attr is not exist, try to look for hwmon
		 * attributes on the level down (in device directory) */
		if (access(path, F_OK) == 0)
			snprintf(path, PATH_MAX, "/sys/class/hwmon/%s", ent->d_name);
		else
			snprintf(path, PATH_MAX, "/sys/class/hwmon/%s/device", ent->d_name);

		if ((ret = checkRAccess(path)) != KEAPI_RET_SUCCESS) {
			if (ret == KEAPI_RET_RETRIEVAL_ERROR)
				continue;
			else
				return ret;
		}

		if ((dir2 = opendir(path)) == NULL) {
			closedir(dir);
			return KEAPI_RET_ERROR;
		}

		/* Look for files temp1_input, temp2_input... */
		while ((ent2 = readdir(dir2)) != NULL) {
			if (FindRegex(ent2->d_name, "temp[[:digit:]]{1,2}_input", &found, REG_EXTENDED | REG_NEWLINE) !=
			    KEAPI_RET_SUCCESS) {
				closedir(dir2);
				closedir(dir);
				return KEAPI_RET_ERROR;
			}
			if (found == TRUE) {
				sensorName = strdup(ent2->d_name);
				if ((sens_name_brk = strpbrk(sensorName, "_")) != NULL)
					*sens_name_brk = '\0';

				if (!IsSensorIgnored(ent->d_name, sensorName))
					(*pTempSensorCount)++;

				free(sensorName);
			}
		}
		closedir(dir2);
	}
	closedir(dir);
	return KEAPI_RET_SUCCESS;
}

/*******************************************************************************/

KEAPI_RETVAL KEApiL_GetTempSensorValue(int32_t sensorNr, PKEAPI_SENSOR_VALUE pSensorValue)
{
	char path_dev_dir[PATH_MAX], path[PATH_MAX], *info;
	char *sensorName, *sens_name_brk;
	struct dirent *ent, *ent2;
	int32_t index, numdev, ret;
	DIR *dir, *dir2;
	int8_t found;

	/* Check function parameters */
	if (pSensorValue == NULL)
		return KEAPI_RET_PARAM_NULL;

	if ((ret = KEApiL_GetTempSensorCount(&numdev)) != KEAPI_RET_SUCCESS)
		return ret;

	if (sensorNr < 0 || sensorNr >= numdev)
		return KEAPI_RET_PARAM_ERROR;

	memset(pSensorValue, 0, sizeof(KEAPI_SENSOR_VALUE));

	if ((ret = checkRAccess("/sys/class/hwmon")) != KEAPI_RET_SUCCESS)
		return ret;

	if ((dir = opendir("/sys/class/hwmon")) == NULL)
		return KEAPI_RET_ERROR;

	index = 0;
	/* Directories hwmon0, 1, 2... */
	while ((ent = readdir(dir)) != NULL) {
		if (ent->d_name[0] == '.')
			continue;

		snprintf(path, PATH_MAX, "/sys/class/hwmon/%s/name", ent->d_name);
		/* If "name" attr is not exist, try to look for hwmon
		 * attributes on the level down (in device directory) */
		if (access(path, F_OK) == 0)
			snprintf(path_dev_dir, PATH_MAX, "/sys/class/hwmon/%s", ent->d_name);
		else
			snprintf(path_dev_dir, PATH_MAX, "/sys/class/hwmon/%s/device", ent->d_name);

		if ((ret = checkRAccess(path_dev_dir)) != KEAPI_RET_SUCCESS) {
			if (ret == KEAPI_RET_RETRIEVAL_ERROR)
				continue;
			else
				return ret;
		}

		if ((dir2 = opendir(path_dev_dir)) == NULL)
			return KEAPI_RET_ERROR;

		/* Look for files temp1_input, temp2_input... */
		while ((ent2 = readdir(dir2)) != NULL) {
			if (FindRegex(ent2->d_name, "temp[[:digit:]]{1,2}_input", &found, REG_EXTENDED | REG_NEWLINE) !=
			    KEAPI_RET_SUCCESS) {
				closedir(dir2);
				closedir(dir);
				return KEAPI_RET_ERROR;
			}

			if (found == TRUE) {
				sensorName = strdup(ent2->d_name);
				if ((sens_name_brk = strpbrk(sensorName, "_")) != NULL)
					*sens_name_brk = '\0';

				if (IsSensorIgnored(ent->d_name, sensorName)) {
					free(sensorName);
					continue;
				} else {
					free(sensorName);
				}

				if (index == sensorNr) {
					snprintf(path, PATH_MAX, "%s/%s", path_dev_dir, ent2->d_name);

					if ((ret = checkRAccess(path)) != KEAPI_RET_SUCCESS) {
						closedir(dir2);
						closedir(dir);
						return ret;
					}

					if ((ret = ReadFile(path, &info)) != KEAPI_RET_SUCCESS) {
						closedir(dir2);
						closedir(dir);
						return ret;
					}

					pSensorValue->value = atoi(info);
					free(info);

					/*TODO: make detection alarm_status here*/
					pSensorValue->status |= KEAPI_SENSOR_STATUS_ACTIVE;

					closedir(dir2);
					closedir(dir);
					return KEAPI_RET_SUCCESS;
				}
				index++;
			}
		}
		closedir(dir2);
	}
	closedir(dir);

	return KEAPI_RET_PARAM_ERROR;
}

/*******************************************************************************/

KEAPI_RETVAL KEApiL_GetTempSensorValueList(PKEAPI_SENSOR_VALUE pSensorValues, int32_t sensorCount)
{
	int32_t i, numdev, ret;
	PKEAPI_SENSOR_VALUE pSensorCurrent = pSensorValues;

	/* Check function parameters */
	if (pSensorValues == NULL)
		return KEAPI_RET_PARAM_NULL;

	if ((ret = KEApiL_GetTempSensorCount(&numdev)) != KEAPI_RET_SUCCESS)
		return ret;

	if (sensorCount < 0 || sensorCount > numdev)
		return KEAPI_RET_PARAM_ERROR;

	for (i = 0; i < sensorCount && i < numdev; i++) {
		if ((ret = KEApiL_GetTempSensorValue(i, pSensorCurrent)) != KEAPI_RET_SUCCESS)
			return ret;

		pSensorCurrent++;
	}

	if (sensorCount < numdev)
		return KEAPI_RET_PARTIAL_SUCCESS;

	return KEAPI_RET_SUCCESS;
}

/*******************************************************************************/

KEAPI_RETVAL KEApiL_GetTempSensorInfo(int32_t sensorNr, PKEAPI_SENSOR_INFO pSensorInfo)
{
	char path_dev_dir[PATH_MAX], path[PATH_MAX], chip[KEAPI_MAX_STR];
	char *sensorName, *sens_name_brk;
	char *info;
	struct dirent *ent, *ent2;
	int32_t index, numdev, ret;
	DIR *dir, *dir2;
	int8_t found;

	char regex[KEAPI_MAX_STR], *substr, *sensors_conf;
	int32_t rc, len, ovector[30];
	pcre *re, *re2;
	const char *error;
	int erroffset;

	/* Check function parameters */
	if (pSensorInfo == NULL)
		return KEAPI_RET_PARAM_NULL;

	if ((ret = KEApiL_GetTempSensorCount(&numdev)) != KEAPI_RET_SUCCESS)
		return ret;

	if (sensorNr < 0 || sensorNr >= numdev)
		return KEAPI_RET_PARAM_ERROR;

	if ((ret = checkRAccess("/sys/class/hwmon")) != KEAPI_RET_SUCCESS)
		return ret;

	if ((dir = opendir("/sys/class/hwmon")) == NULL)
		return KEAPI_RET_ERROR;

	index = 0;
	/* Directories hwmon0, 1, 2... */
	while ((ent = readdir(dir)) != NULL) {
		if (ent->d_name[0] == '.')
			continue;

		snprintf(path, PATH_MAX, "/sys/class/hwmon/%s/name", ent->d_name);
		/* if "name" attr is not exist, try to look for hwmon
		 * attributes on the level down (in device directory) */
		if (access(path, F_OK) == 0)
			snprintf(path, PATH_MAX, "/sys/class/hwmon/%s", ent->d_name);
		else
			snprintf(path, PATH_MAX, "/sys/class/hwmon/%s/device", ent->d_name);

		/* Save the path for further using */
		strncpy(path_dev_dir, path, KEAPI_MAX_STR);

		if ((ret = checkRAccess(path)) != KEAPI_RET_SUCCESS) {
			if (ret == KEAPI_RET_RETRIEVAL_ERROR)
				continue;
			else
				return ret;
		}

		if ((dir2 = opendir(path)) == NULL)
			return KEAPI_RET_ERROR;

		/* Look for files temp1_input, temp2_input... */
		while ((ent2 = readdir(dir2)) != NULL) {
			if (FindRegex(ent2->d_name, "temp[[:digit:]]{1,2}_input", &found, REG_EXTENDED | REG_NEWLINE) !=
			    KEAPI_RET_SUCCESS) {
				closedir(dir2);
				closedir(dir);
				return KEAPI_RET_ERROR;
			}

			if (found == TRUE) {
				sensorName = strdup(ent2->d_name);
				if ((sens_name_brk = strpbrk(sensorName, "_")) != NULL)
					*sens_name_brk = '\0';

				if (IsSensorIgnored(ent->d_name, sensorName)) {
					free(sensorName);
					continue;
				}

				if (index == sensorNr) {
					memset(pSensorInfo, 0, sizeof(KEAPI_SENSOR_INFO));

					/* NAME */
					snprintf(pSensorInfo->name, KEAPI_MAX_STR, "%s", sensorName);

					/* Try to read sensor label */
					//snprintf(path, KEAPI_MAX_STR, "%s/%s_label", path_dev_dir, pSensorInfo->name);
					//if (ReadFile(path, &info) == KEAPI_RET_SUCCESS) {
					//	int len = strlen(info);
					//	// Remove \n at the end
					//	if (len <= KEAPI_MAX_STR && info[len - 1] == '\n')
					//		info[len - 1] = '\0';
					//	else
					//		info[KEAPI_MAX_STR - 1] = '\0';
					//	snprintf(pSensorInfo->name, KEAPI_MAX_STR, "%s", info);
					//	free(info);
					//}

					/* vvvvvvv      VALUES      vvvvvvvv */
					snprintf(path, PATH_MAX, "%s/%s%s", path_dev_dir, pSensorInfo->name, "_min");
					if ((ret = checkRAccess(path)) == KEAPI_RET_SUCCESS) {
						if ((ret = ReadFile(path, &info)) == KEAPI_RET_SUCCESS) {
							pSensorInfo->min = atoi(info);
							free(info);
						} else
							goto error;

					} else if (ret == KEAPI_RET_RETRIEVAL_ERROR)
						pSensorInfo->min = KEAPI_SENSOR_INFO_UNKNOWN;
					else
						goto error;

					snprintf(path, PATH_MAX, "%s/%s%s", path_dev_dir, pSensorInfo->name, "_max");
					if ((ret = checkRAccess(path)) == KEAPI_RET_SUCCESS) {
						if ((ret = ReadFile(path, &info)) == KEAPI_RET_SUCCESS) {
							pSensorInfo->max = atoi(info);
							free(info);
						} else
							goto error;

					} else if (ret == KEAPI_RET_RETRIEVAL_ERROR)
						pSensorInfo->max = KEAPI_SENSOR_INFO_UNKNOWN;
					else
						goto error;

					snprintf(path, PATH_MAX, "%s/%s%s", path_dev_dir, pSensorInfo->name,
						 "_min_hyst");
					if ((ret = checkRAccess(path)) == KEAPI_RET_SUCCESS) {
						if ((ret = ReadFile(path, &info)) == KEAPI_RET_SUCCESS) {
							pSensorInfo->hystLo = atoi(info);
							free(info);
						} else
							goto error;

					} else if (ret == KEAPI_RET_RETRIEVAL_ERROR)
						pSensorInfo->hystLo = KEAPI_SENSOR_INFO_UNKNOWN;
					else
						goto error;

					snprintf(path, PATH_MAX, "%s/%s%s", path_dev_dir, pSensorInfo->name,
						 "_max_hyst");
					if ((ret = checkRAccess(path)) == KEAPI_RET_SUCCESS) {
						if ((ret = ReadFile(path, &info)) == KEAPI_RET_SUCCESS) {
							pSensorInfo->hystHi = atoi(info);
							free(info);
						} else
							goto error;

					} else if (ret == KEAPI_RET_RETRIEVAL_ERROR)
						pSensorInfo->hystHi = KEAPI_SENSOR_INFO_UNKNOWN;
					else
						goto error;

					snprintf(path, PATH_MAX, "%s/%s%s", path_dev_dir, pSensorInfo->name,
						 "_min_alarm");
					if ((ret = checkRAccess(path)) == KEAPI_RET_SUCCESS) {
						if ((ret = ReadFile(path, &info)) == KEAPI_RET_SUCCESS) {
							pSensorInfo->alarmLo = atoi(info);
							free(info);
						} else
							goto error;

					} else if (ret == KEAPI_RET_RETRIEVAL_ERROR)
						pSensorInfo->alarmLo = KEAPI_SENSOR_INFO_UNKNOWN;
					else
						goto error;

					snprintf(path, PATH_MAX, "%s/%s%s", path_dev_dir, pSensorInfo->name,
						 "_max_alarm");
					if ((ret = checkRAccess(path)) == KEAPI_RET_SUCCESS) {
						if ((ret = ReadFile(path, &info)) == KEAPI_RET_SUCCESS) {
							pSensorInfo->alarmHi = atoi(info);
							free(info);
						} else
							goto error;

					} else if (ret == KEAPI_RET_RETRIEVAL_ERROR)
						pSensorInfo->alarmHi = KEAPI_SENSOR_INFO_UNKNOWN;
					else
						goto error;

					/* ^^^^^^^^      VALUES      ^^^^^^^^ */

					/* vvvvvvv      NAME      vvvvvvvv */
					/* Get real sensor chip name from sensors conf file */
					snprintf(path, PATH_MAX, "%s/name", path_dev_dir);
					if ((ret = checkRAccess(path)) != KEAPI_RET_SUCCESS)
						goto type_detect;

					if ((ret = ReadFile(path, &info)) == KEAPI_RET_SUCCESS) {
						strncpy(chip, info, KEAPI_MAX_STR);
						free(info);

						/* Get rid of \n at the end */
						if (chip[strnlen(chip, KEAPI_MAX_STR) - 1] == '\n')
							chip[strnlen(chip, KEAPI_MAX_STR) - 1] = '\0';

						/* Read names from sensors conf file */
						if ((ret = checkRAccess(GetSensorsConfName())) != KEAPI_RET_SUCCESS) {
							if (ret == KEAPI_RET_RETRIEVAL_ERROR)
								goto type_detect;
							else
								goto error;
						}

						if ((ret = ReadFile(GetSensorsConfName(), &info)) ==
						    KEAPI_RET_SUCCESS) {
							/* Create regex */
							snprintf(regex, KEAPI_MAX_STR, "(%s\\-\\*.*?)(?:^chip|\\Z)",
								 chip);
							if ((re = pcre_compile(regex,
									       PCRE_MULTILINE | PCRE_NEWLINE_ANYCRLF |
										       PCRE_DOTALL,
									       &error, &erroffset, NULL)) == NULL) {
								free(info);
								continue;
							}

							if ((rc = pcre_exec(re, NULL, info, strlen(info), 0, 0, ovector,
									    30)) == 2) {
								/* Find appropriate part of sensors file */
								len = ovector[3] - ovector[2];
								substr = (char *)malloc(len + 1);
								memcpy(substr, info + ovector[2], len);
								substr[len] = '\0';
								sensors_conf = strdup(substr);
								free(substr);

								/* Find sensor name in it */
								snprintf(regex, KEAPI_MAX_STR, "label %s \\\"(.*?)\\\"",
									 sensorName);
								if ((re2 = pcre_compile(
									     regex,
									     PCRE_MULTILINE | PCRE_NEWLINE_ANYCRLF |
										     PCRE_DOTALL,
									     &error, &erroffset, NULL)) == NULL) {
									pcre_free(re);
									free(info);
									continue;
								}

								if ((rc = pcre_exec(re2, NULL, sensors_conf,
										    strlen(sensors_conf), 0, 0, ovector,
										    30)) == 2) {
									len = ovector[3] - ovector[2];
									substr = (char *)malloc(len + 1);
									memcpy(substr, sensors_conf + ovector[2], len);
									pSensorInfo->name[0] = substr[len] = '\0';
									strncat(pSensorInfo->name, substr,
										KEAPI_MAX_STR - 1);
									free(substr);
								}

								free(sensors_conf);
								pcre_free(re2);
							}

							pcre_free(re);
							free(info);
						} else //if ((ret = ReadFile("/etc/sensors3.conf", &info)) == KEAPI_RET_SUCCESS) {
							goto error;
					} //if ((ret = ReadFile(path, &info)) == KEAPI_RET_SUCCESS)
					else
						goto error;
					/* ^^^^^^^^      NAME      ^^^^^^^^ */

				type_detect:
					/* vvvvvvv      TYPE      vvvvvvvv */
					if (strcasecmp(pSensorInfo->name, "backplane-temp") == 0)
						pSensorInfo->type = KEAPI_TEMP_BACKPLANE;
					else if (strcasecmp(pSensorInfo->name, "board-temp") == 0)
						pSensorInfo->type = KEAPI_TEMP_BOARD;
					else if (strcasecmp(pSensorInfo->name, "box-temp") == 0)
						pSensorInfo->type = KEAPI_TEMP_BOX;
					else if (strcasecmp(pSensorInfo->name, "chip-temp") == 0)
						pSensorInfo->type = KEAPI_TEMP_CHIPSET;
					else if (strcasecmp(pSensorInfo->name, "cpu-temp") == 0)
						pSensorInfo->type = KEAPI_TEMP_CPU;
					else if (strcasecmp(pSensorInfo->name, "env-temp") == 0)
						pSensorInfo->type = KEAPI_TEMP_ENV;
					else if (strcasecmp(pSensorInfo->name, "gpu-temp") == 0)
						pSensorInfo->type = KEAPI_TEMP_VIDEO;
					else
						pSensorInfo->type = KEAPI_TEMP_OTHER;
					/* ^^^^^^^^      TYPE      ^^^^^^^^ */

					closedir(dir2);
					closedir(dir);
					free(sensorName);
					return KEAPI_RET_SUCCESS;
				}
				index++;
				free(sensorName);
			}
		}
		closedir(dir2);
	}
	closedir(dir);

	return KEAPI_RET_PARAM_ERROR;

error:
	closedir(dir2);
	closedir(dir);
	return ret;
}
