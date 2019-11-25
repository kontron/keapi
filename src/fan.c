/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron AG */

/* Fan monitoring and control functions. */

#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <regex.h>
#include <string.h>
#include <pcre.h>

#include "keapi_inc.h"

/******************************************************************************/
KEAPI_RETVAL KEApiL_GetFanSensorCount(int32_t *pFanCount)
{
	DIR *dir, *dir2;
	struct dirent *ent, *ent2;
	int8_t found;
	int32_t ret;
	char path[PATH_MAX];

	/* Check function parameters */
	if (pFanCount == NULL)
		return KEAPI_RET_PARAM_NULL;

	/* Initialization */
	*pFanCount = 0;

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
			else {
				closedir(dir);
				return ret;
			}
		}

		if ((dir2 = opendir(path)) == NULL) { /* Doesn't have /device subdir */
			closedir(dir);
			return KEAPI_RET_ERROR;
		}

		/* Look for files fan1_input, fan2_input... */
		while ((ent2 = readdir(dir2)) != NULL) {
			if (FindRegex(ent2->d_name, "fan[0-9]+_input", &found, REG_EXTENDED | REG_NEWLINE) !=
			    KEAPI_RET_SUCCESS) {
				closedir(dir2);
				closedir(dir);
				return KEAPI_RET_ERROR;
			}
			if (found == TRUE) {
				char sensorName[KEAPI_MAX_STR];
				strncpy(sensorName, ent2->d_name, 4);
				sensorName[4] = '\0';

				if (!IsSensorIgnored(ent->d_name, sensorName))
					(*pFanCount)++;
			}
		}
		closedir(dir2);
	}
	closedir(dir);
	return KEAPI_RET_SUCCESS;
}

/******************************************************************************/
KEAPI_RETVAL KEApiL_GetFanSensorValue(int32_t sensorNr, PKEAPI_SENSOR_VALUE pSensorValue)
{
	DIR *dir, *dir2;
	struct dirent *ent, *ent2;
	char path[PATH_MAX], base_path[PATH_MAX], *info;
	int8_t found;
	int32_t numdev, index, ret;

	/* Check function parameters */
	if (pSensorValue == NULL)
		return KEAPI_RET_PARAM_NULL;

	if ((ret = KEApiL_GetFanSensorCount(&numdev)) != KEAPI_RET_SUCCESS)
		return ret;

	if (sensorNr < 0 || sensorNr >= numdev)
		return KEAPI_RET_PARAM_ERROR;

	memset(pSensorValue, 0, sizeof(KEAPI_SENSOR_VALUE));

	if ((ret = checkRAccess("/sys/class/hwmon")) != KEAPI_RET_SUCCESS)
		return ret;

	if ((dir = opendir("/sys/class/hwmon")) == NULL)
		return KEAPI_RET_ERROR;

	/* Directories hwmon0, 1, 2... */
	index = 0;
	while ((ent = readdir(dir)) != NULL) {
		if (ent->d_name[0] == '.')
			continue;

		snprintf(path, PATH_MAX, "/sys/class/hwmon/%s/name", ent->d_name);
		/* if "name" attr is not exist, try to look for hwmon
		 * attributes on the level down (in device directory) */
		if (access(path, F_OK) == 0)
			snprintf(base_path, PATH_MAX, "/sys/class/hwmon/%s", ent->d_name);
		else
			snprintf(base_path, PATH_MAX, "/sys/class/hwmon/%s/device", ent->d_name);

		if ((ret = checkRAccess(base_path)) != KEAPI_RET_SUCCESS) {
			if (ret == KEAPI_RET_RETRIEVAL_ERROR) /* Doesn't have /device subdir */
				continue;
			else {
				closedir(dir);
				return ret;
			}
		}

		if ((dir2 = opendir(base_path)) == NULL) {
			closedir(dir);
			return KEAPI_RET_ERROR;
		}

		/* Look for files fan1_input, fan2_input... */
		while ((ent2 = readdir(dir2)) != NULL) {
			if (FindRegex(ent2->d_name, "fan[0-9]+_input", &found, REG_EXTENDED | REG_NEWLINE) !=
			    KEAPI_RET_SUCCESS) {
				closedir(dir2);
				closedir(dir);
				return KEAPI_RET_ERROR;
			}

			if (found == TRUE) {
				char sensorName[KEAPI_MAX_STR];
				int32_t sensorNum = atoi(&ent2->d_name[3]);

				strncpy(sensorName, ent2->d_name, 4);
				sensorName[4] = '\0';

				if (IsSensorIgnored(ent->d_name, sensorName))
					continue;

				if (index == sensorNr) {
					snprintf(path, PATH_MAX, "%s/fan%d_input", base_path, sensorNum);

					if ((ret = checkRAccess(path)) != KEAPI_RET_SUCCESS) {
						closedir(dir2);
						closedir(dir);
						return ret; /* File not exist */
					}

					if ((ret = ReadFile(path, &info)) != KEAPI_RET_SUCCESS) {
						closedir(dir2);
						closedir(dir);
						return ret;
					}
					pSensorValue->value = atoi(info);
					free(info);
					// TODO: make here ALARM detection
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

	/* Fan not found */
	return KEAPI_RET_ERROR;
}

/******************************************************************************/
KEAPI_RETVAL KEApiL_GetFanSensorValueList(PKEAPI_SENSOR_VALUE pSensorValues, int32_t sensorCount)
{
	int32_t i, numdev, ret;
	PKEAPI_SENSOR_VALUE pSensorCurrent = pSensorValues;

	/* Check function parameters */
	if (pSensorValues == NULL)
		return KEAPI_RET_PARAM_NULL;

	if ((ret = KEApiL_GetFanSensorCount(&numdev)) != KEAPI_RET_SUCCESS)
		return ret;

	if (sensorCount < 0 || sensorCount > numdev)
		return KEAPI_RET_PARAM_ERROR;

	for (i = 0; i < sensorCount && i < numdev; i++) {
		if ((ret = KEApiL_GetFanSensorValue(i, pSensorCurrent)) != KEAPI_RET_SUCCESS)
			return ret;

		pSensorCurrent++;
	}

	if (sensorCount < numdev)
		return KEAPI_RET_PARTIAL_SUCCESS;

	return KEAPI_RET_SUCCESS;
}

/******************************************************************************/
KEAPI_RETVAL KEApiL_GetFanSensorInfo(int32_t sensorNr, PKEAPI_SENSOR_INFO pSensorInfo)
{
	DIR *dir, *dir2;
	struct dirent *ent, *ent2;
	char base_path[PATH_MAX], path[PATH_MAX], *info;
	char regex[KEAPI_MAX_STR], chip[KEAPI_MAX_STR], *substr, *sensors_conf;
	int32_t ret, len, index, numdev, ovector[30];
	int8_t found;
	pcre *re, *re2;
	const char *error;
	int erroffset;

	/* Check function parameters */
	if (pSensorInfo == NULL)
		return KEAPI_RET_PARAM_NULL;

	if ((ret = KEApiL_GetFanSensorCount(&numdev)) != KEAPI_RET_SUCCESS)
		return ret;

	if (sensorNr < 0 || sensorNr >= numdev)
		return KEAPI_RET_PARAM_ERROR;

	if ((ret = checkRAccess("/sys/class/hwmon")) != KEAPI_RET_SUCCESS)
		return ret;

	if ((dir = opendir("/sys/class/hwmon")) == NULL)
		return KEAPI_RET_ERROR;

	/* Directories hwmon0, 1, 2... */
	index = 0;
	while ((ent = readdir(dir)) != NULL) {
		if (ent->d_name[0] == '.')
			continue;

		snprintf(path, PATH_MAX, "/sys/class/hwmon/%s/name", ent->d_name);
		/* if "name" attr is not exist, try to look for hwmon
		 * attributes on the level down (in device directory) */
		if (access(path, F_OK) == 0)
			snprintf(base_path, PATH_MAX, "/sys/class/hwmon/%s", ent->d_name);
		else
			snprintf(base_path, PATH_MAX, "/sys/class/hwmon/%s/device", ent->d_name);

		if ((ret = checkRAccess(base_path)) != KEAPI_RET_SUCCESS) {
			if (ret == KEAPI_RET_RETRIEVAL_ERROR)
				continue;
			else {
				closedir(dir);
				return ret;
			}
		}

		if ((dir2 = opendir(base_path)) == NULL) { /* Doesn't have /device subdir */
			closedir(dir);
			return ret;
		}

		/* Look for files fan1_input, fan2_input... */
		while ((ent2 = readdir(dir2)) != NULL) {
			if (FindRegex(ent2->d_name, "fan[0-9]+_input", &found, REG_EXTENDED | REG_NEWLINE) !=
			    KEAPI_RET_SUCCESS) {
				closedir(dir);
				closedir(dir2);
				return KEAPI_RET_ERROR;
			}

			if (found == TRUE) {
				char sensorName[KEAPI_MAX_STR];
				int32_t sensorNum = atoi(&ent2->d_name[3]);

				strncpy(sensorName, ent2->d_name, 4);
				sensorName[4] = '\0';

				if (IsSensorIgnored(ent->d_name, sensorName))
					continue;

				if (index == sensorNr) {
					/* initialization */
					memset(pSensorInfo, 0, sizeof(KEAPI_SENSOR_INFO));

					/* vvvvvvv      VALUES      vvvvvvvv */
					snprintf(path, KEAPI_MAX_STR, "%s/fan%d_max", base_path, sensorNum);

					if ((ret = checkRAccess(path)) == KEAPI_RET_SUCCESS) {
						if ((ret = ReadFile(path, &info)) == KEAPI_RET_SUCCESS) {
							pSensorInfo->max = atoi(info);
							free(info);
						} else {
							closedir(dir2);
							closedir(dir);
							return ret;
						}
					} else if (ret == KEAPI_RET_RETRIEVAL_ERROR)
						pSensorInfo->max = KEAPI_SENSOR_INFO_UNKNOWN;
					else {
						closedir(dir2);
						closedir(dir);
						return ret;
					}

					snprintf(path, PATH_MAX, "%s/fan%d_min", base_path, sensorNum);

					if ((ret = checkRAccess(path)) == KEAPI_RET_SUCCESS) {
						if ((ret = ReadFile(path, &info)) == KEAPI_RET_SUCCESS) {
							pSensorInfo->min = atoi(info);
							free(info);
						} else {
							closedir(dir2);
							closedir(dir);
							return ret;
						}
					} else if (ret == KEAPI_RET_RETRIEVAL_ERROR)
						pSensorInfo->min = KEAPI_SENSOR_INFO_UNKNOWN;
					else {
						closedir(dir2);
						closedir(dir);
						return ret;
					}

					/* TODO */
					pSensorInfo->alarmHi = KEAPI_SENSOR_INFO_UNKNOWN;
					pSensorInfo->alarmLo = KEAPI_SENSOR_INFO_UNKNOWN;
					pSensorInfo->hystHi = KEAPI_SENSOR_INFO_UNKNOWN;
					pSensorInfo->hystLo = KEAPI_SENSOR_INFO_UNKNOWN;
					/* ^^^^^^^^      VALUES      ^^^^^^^^ */

					/* vvvvvvv      NAME      vvvvvvvv */
					/* Get real sensor name from sensors conf file */
					snprintf(path, PATH_MAX, "%s/name", base_path);

					if ((ret = checkRAccess(path)) == KEAPI_RET_SUCCESS) {
						if ((ret = ReadFile(path, &info)) != KEAPI_RET_SUCCESS)
							goto read_error;

						strncpy(chip, info, KEAPI_MAX_STR);
						free(info);

						/* Get rid of \n at the end */
						if (chip[strnlen(chip, KEAPI_MAX_STR) - 1] == '\n')
							chip[strnlen(chip, KEAPI_MAX_STR) - 1] = '\0';

						/* Read names from sensors conf file */
						if ((ret = checkRAccess(GetSensorsConfName())) == KEAPI_RET_SUCCESS) {
							if ((ret = ReadFile(GetSensorsConfName(), &info)) !=
							    KEAPI_RET_SUCCESS)
								goto read_error;

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

							if ((ret = pcre_exec(re, NULL, info, strlen(info), 0, 0,
									     ovector, 30)) == 2) {
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
								if ((ret = pcre_exec(re2, NULL, sensors_conf,
										     strlen(sensors_conf), 0, 0,
										     ovector, 30)) == 2) {
									len = ovector[3] - ovector[2];
									substr = (char *)malloc(len + 1);
									memcpy(substr, sensors_conf + ovector[2], len);
									substr[len] = '\0';
									strncat(pSensorInfo->name, substr,
										KEAPI_MAX_STR - 1);
									free(substr);
								}
								pcre_free(re2);

								free(sensors_conf);
							}

							pcre_free(re);
							free(info);
						} /* checkRAccess("/etc/sensors3.conf") == OK */
						else if (ret != KEAPI_RET_RETRIEVAL_ERROR)
							goto read_error;

					} /* checkRAccess("/sys/class/hwmon/%s/device/name")) == OK */
					else if (ret != KEAPI_RET_RETRIEVAL_ERROR)
						goto read_error;

					/* ^^^^^^^^      NAME      ^^^^^^^^ */

					/* vvvvvvv      TYPE      vvvvvvvv */
					if (strcasecmp(pSensorInfo->name, "box-fan") == 0)
						pSensorInfo->type = KEAPI_FAN_BOX;
					else if (strcasecmp(pSensorInfo->name, "chip-fan") == 0)
						pSensorInfo->type = KEAPI_FAN_CHIPSET;
					else if (strcasecmp(pSensorInfo->name, "cpu-fan") == 0)
						pSensorInfo->type = KEAPI_FAN_CPU;
					else if (strcasecmp(pSensorInfo->name, "env-fan") == 0)
						pSensorInfo->type = KEAPI_FAN_ENV;
					else if (strcasecmp(pSensorInfo->name, "gpu-fan") == 0)
						pSensorInfo->type = KEAPI_FAN_VIDEO;
					else
						pSensorInfo->type = KEAPI_FAN_OTHER;
					/* ^^^^^^^^      TYPE      ^^^^^^^^ */

					closedir(dir2);
					closedir(dir);
					return KEAPI_RET_SUCCESS;
				} /* if (index == sensorNr) { */
				index++;
			}
		}
		closedir(dir2);
	}
	closedir(dir);

	/* Fan not found */
	return KEAPI_RET_ERROR;

read_error:
	closedir(dir2);
	closedir(dir);
	return ret;
}
