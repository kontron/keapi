/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron Europe GmbH */

/* Voltage monitoring functions. */

#include <stdio.h>
#include <dirent.h>
#include <regex.h>
#include <unistd.h>
#include <string.h>
#include <pcre.h>

#include "keapi_inc.h"

/*******************************************************************************/
KEAPI_RETVAL KEApiL_GetVoltageSensorCount(int32_t *pVoltageSensorCount)
{
	struct dirent *ent, *ent2;
	char path[PATH_MAX];
	DIR *dir, *dir2;
	int8_t found;
	int32_t ret;

	/* Check function parameters */
	if (pVoltageSensorCount == NULL)
		return KEAPI_RET_PARAM_NULL;

	/* Initialization */
	*pVoltageSensorCount = 0;

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

		if ((dir2 = opendir(path)) == NULL) { // Doesn't have /device subdir
			closedir(dir);
			return KEAPI_RET_ERROR;
		}

		/* Look for files in0_input, in1_input, in2_input... */
		while ((ent2 = readdir(dir2)) != NULL) {
			if (FindRegex(ent2->d_name, "in[0-9]+_input", &found, REG_EXTENDED | REG_NEWLINE) !=
			    KEAPI_RET_SUCCESS) {
				closedir(dir2);
				closedir(dir);
				return KEAPI_RET_ERROR;
			}
			if (found == TRUE) {
				char sensorName[KEAPI_MAX_STR];
				strncpy(sensorName, ent2->d_name, 4);
				if (sensorName[3] == '_')
					sensorName[3] = '\0';
				else
					sensorName[4] = '\0';

				if (!IsSensorIgnored(ent->d_name, sensorName))
					(*pVoltageSensorCount)++;
			}
		}
		closedir(dir2);
	}
	closedir(dir);

	return KEAPI_RET_SUCCESS;
}

/*******************************************************************************/

KEAPI_RETVAL KEApiL_GetVoltageSensorValue(int32_t sensorNr, PKEAPI_SENSOR_VALUE pSensorValue)
{
	DIR *dir, *dir2;
	struct dirent *ent, *ent2;
	int8_t found;
	char path[PATH_MAX], base_path[PATH_MAX], *info;
	int32_t index, numdev, ret;

	char regex[KEAPI_MAX_STR], sensor_name[KEAPI_MAX_STR], old_sensor_name[KEAPI_MAX_STR], chip[KEAPI_MAX_STR],
		*substr, *sensors_conf;
	int32_t rc, len;
	int32_t ovector[30];
	float val_flt;
	pcre *re, *re2;
	const char *error;
	int erroffset;

	/* Check function parameters */
	if (pSensorValue == NULL)
		return KEAPI_RET_PARAM_NULL;

	if ((ret = KEApiL_GetVoltageSensorCount(&numdev)) != KEAPI_RET_SUCCESS)
		return ret;

	if (sensorNr < 0 || sensorNr >= numdev)
		return KEAPI_RET_PARAM_ERROR;

	/* Initialization */
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

		if ((dir2 = opendir(base_path)) == NULL) { // Doesn't have /device subdir
			closedir(dir);
			return KEAPI_RET_ERROR;
		}

		/* Look for files in0_input, in1_input, in2_input... */
		while ((ent2 = readdir(dir2)) != NULL) {
			if (FindRegex(ent2->d_name, "in[0-9]+_input", &found, REG_EXTENDED | REG_NEWLINE) !=
			    KEAPI_RET_SUCCESS) {
				closedir(dir2);
				closedir(dir);
				return KEAPI_RET_ERROR;
			}
			if (found == TRUE) {
				char sensorName[KEAPI_MAX_STR];
				strncpy(sensorName, ent2->d_name, 4);
				if (sensorName[3] == '_')
					sensorName[3] = '\0';
				else
					sensorName[4] = '\0';

				if (IsSensorIgnored(ent->d_name, sensorName))
					continue;

				if (index == sensorNr) {
					/* VALUE */
					snprintf(path, PATH_MAX, "%s/%s", base_path, ent2->d_name);
					if ((ret = checkRAccess(path)) != KEAPI_RET_SUCCESS)
						goto read_error;

					if ((ret = ReadFile(path, &info)) != KEAPI_RET_SUCCESS)
						goto read_error;

					pSensorValue->value = atoi(info);
					free(info);

					/* TODO: make here detectiong of ALARM state */
					pSensorValue->status |= KEAPI_SENSOR_STATUS_ACTIVE;

					/* NAME */
					snprintf(sensor_name, KEAPI_MAX_STR, "%.3s", ent2->d_name);

					/* Compute real sensor value from sensors conf file */
					strncpy(old_sensor_name, sensor_name, KEAPI_MAX_STR);
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

							if ((rc = pcre_exec(re, NULL, info, strlen(info), 0, 0, ovector,
									    30)) == 2) {
								/* Find appropriate part of sensors file */
								len = ovector[3] - ovector[2];
								substr = (char *)malloc(len + 1);
								memcpy(substr, info + ovector[2], len);
								substr[len] = '\0';
								sensors_conf = strdup(substr);
								free(substr);

								/* Correct sensor's value */
								/* Multiply measured value by value in sensors.conf */
								/* IT HAS TO BE A SINGLE NUMBER LIKE @*0.123456 + 0.654321, NOT *((@ + 3)/10) */

								snprintf(
									regex, KEAPI_MAX_STR,
									"^(?:\\s|[^#]*?)?compute\\s*%s\\s*\\@\\s*?\\*?\\s*?((?:\\d+\\.)?\\d+)?\\s*?(\\-|\\+)\\s*?((?:\\d+\\.)?\\d+)?,",
									sensorName);

								if ((re2 = pcre_compile(
									     regex,
									     PCRE_MULTILINE | PCRE_NEWLINE_ANYCRLF |
										     PCRE_DOTALL,
									     &error, &erroffset, NULL)) == NULL) {
									free(sensors_conf);
									pcre_free(re);
									free(info);
									continue;
								}

								if ((rc = pcre_exec(re2, NULL, sensors_conf,
										    strlen(sensors_conf), 0, 0, ovector,
										    30)) >= 2) {
									int32_t i;
									float correction[3] = { 1.0, 0.0, 0.0 };

									for (i = 2; i <= rc; i++) {
										len = ovector[2 * i - 1] -
										      ovector[2 * i - 2];
										substr = (char *)malloc(len + 1);
										memcpy(substr,
										       sensors_conf +
											       ovector[2 * i - 2],
										       len);
										substr[len] = '\0';
										val_flt = strtof(substr, NULL);
										if ((val_flt != 0.0))
											correction[i - 2] = val_flt;
										else {
											if (strncmp(substr, "+", 1) ==
											    0)
												correction[1] = 1.0;
											else if (strncmp(substr, "-",
													 1) == 0)
												correction[1] = -1.0;
										}
										free(substr);
									}

									pSensorValue->value =
										pSensorValue->value * correction[0] +
										correction[1] * correction[2] * 1000.0;
								}
								pcre_free(re2);

								free(sensors_conf);
							}

							pcre_free(re);
							free(info);
						} // if ((ret = checkRAccess("/etc/sensors3.conf")) == KEAPI_RET_SUCCESS) {
						else if (ret != KEAPI_RET_RETRIEVAL_ERROR)
							goto read_error;
					}

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

read_error:
	closedir(dir2);
	closedir(dir);
	return ret;
}

/*******************************************************************************/

KEAPI_RETVAL KEApiL_GetVoltageSensorValueList(PKEAPI_SENSOR_VALUE pSensorValues, int32_t sensorCount)
{
	int32_t numdev, i, ret;
	PKEAPI_SENSOR_VALUE pSensorCurrent = pSensorValues;

	/* Check function parameters */
	if (pSensorValues == NULL)
		return KEAPI_RET_PARAM_NULL;

	if ((ret = KEApiL_GetVoltageSensorCount(&numdev)) != KEAPI_RET_SUCCESS)
		return ret;

	if (sensorCount < 0 || sensorCount > numdev)
		return KEAPI_RET_PARAM_ERROR;

	for (i = 0; i < sensorCount && i < numdev; i++) {
		if ((ret = KEApiL_GetVoltageSensorValue(i, pSensorCurrent)) != KEAPI_RET_SUCCESS)
			return ret;

		pSensorCurrent++;
	}

	if (sensorCount < numdev)
		return KEAPI_RET_PARTIAL_SUCCESS;

	return KEAPI_RET_SUCCESS;
}

/*******************************************************************************/

KEAPI_RETVAL KEApiL_GetVoltageSensorInfo(int32_t sensorNr, PKEAPI_SENSOR_INFO pSensorInfo)
{
	DIR *dir, *dir2;
	struct dirent *ent, *ent2;
	int8_t found;
	char path[PATH_MAX], base_path[PATH_MAX], *info;
	int32_t index, numdev, ret;

	char regex[KEAPI_MAX_STR];
	char chip[KEAPI_MAX_STR], *substr, *sensors_conf;
	int32_t rc, len;
	int32_t ovector[30];
	float val_flt;
	pcre *re, *re2;
	const char *error;
	int erroffset;

	/* Check function parameters */
	if (pSensorInfo == NULL)
		return KEAPI_RET_PARAM_NULL;

	if ((ret = KEApiL_GetVoltageSensorCount(&numdev)) != KEAPI_RET_SUCCESS)
		return ret;

	if (sensorNr < 0 || sensorNr >= numdev)
		return KEAPI_RET_PARAM_ERROR;

	/* Initialization */
	memset(pSensorInfo, 0, sizeof(KEAPI_SENSOR_INFO));

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

		if ((dir2 = opendir(base_path)) == NULL) { // Doesn't have /device subdir
			closedir(dir);
			return KEAPI_RET_ERROR;
		}

		/* Look for files in0_input, in1_input, in2_input... */
		while ((ent2 = readdir(dir2)) != NULL) {
			if (FindRegex(ent2->d_name, "in[0-9]+_input", &found, REG_EXTENDED | REG_NEWLINE) !=
			    KEAPI_RET_SUCCESS) {
				closedir(dir2);
				closedir(dir);
				return KEAPI_RET_ERROR;
			}

			if (found == TRUE) {
				char sensorName[KEAPI_MAX_STR];

				strncpy(sensorName, ent2->d_name, 4);
				if (sensorName[3] == '_')
					sensorName[3] = '\0';
				else
					sensorName[4] = '\0';

				if (IsSensorIgnored(ent->d_name, sensorName))
					continue;

				if (index == sensorNr) {
					/*************************************************/
					/* Initialization */
					memset(pSensorInfo, 0, sizeof(KEAPI_SENSOR_INFO));

					/* NAME */
					if (sensorName[3] == '\0')
						snprintf(pSensorInfo->name, KEAPI_MAX_STR, "%.3s", ent2->d_name);
					else
						snprintf(pSensorInfo->name, KEAPI_MAX_STR, "%.4s", ent2->d_name);

					/* VALUES */
					snprintf(path, KEAPI_MAX_STR, "%s/%s_max", base_path, sensorName);
					if ((ret = checkRAccess(path)) == KEAPI_RET_SUCCESS) {
						if ((ret = ReadFile(path, &info)) == KEAPI_RET_SUCCESS) {
							pSensorInfo->max = atoi(info);
							free(info);
						} else
							goto read_error;
					} else if (ret == KEAPI_RET_RETRIEVAL_ERROR)
						pSensorInfo->max = KEAPI_SENSOR_INFO_UNKNOWN;
					else
						goto read_error;

					snprintf(path, KEAPI_MAX_STR, "%s/%s_min", base_path, sensorName);
					if ((ret = checkRAccess(path)) == KEAPI_RET_SUCCESS) {
						if ((ret = ReadFile(path, &info)) == KEAPI_RET_SUCCESS) {
							pSensorInfo->min = atoi(info);
							free(info);
						} else
							goto read_error;
					} else if (ret == KEAPI_RET_RETRIEVAL_ERROR)
						pSensorInfo->min = KEAPI_SENSOR_INFO_UNKNOWN;
					else
						goto read_error;

					snprintf(path, KEAPI_MAX_STR, "%s/%s_min_alarm", base_path, sensorName);
					if ((ret = checkRAccess(path)) == KEAPI_RET_SUCCESS) {
						if ((ret = ReadFile(path, &info)) == KEAPI_RET_SUCCESS) {
							pSensorInfo->alarmLo = atoi(info);
							free(info);
						} else
							goto read_error;
					} else if (ret == KEAPI_RET_RETRIEVAL_ERROR)
						pSensorInfo->alarmLo = KEAPI_SENSOR_INFO_UNKNOWN;
					else
						goto read_error;

					snprintf(path, KEAPI_MAX_STR, "%s/%s_max_alarm", base_path, sensorName);
					if ((ret = checkRAccess(path)) == KEAPI_RET_SUCCESS) {
						if ((ret = ReadFile(path, &info)) == KEAPI_RET_SUCCESS) {
							pSensorInfo->alarmHi = atoi(info);
							free(info);
						} else
							goto read_error;
					} else if (ret == KEAPI_RET_RETRIEVAL_ERROR)
						pSensorInfo->alarmHi = KEAPI_SENSOR_INFO_UNKNOWN;
					else
						goto read_error;

					/*TODO: */
					pSensorInfo->hystHi = KEAPI_SENSOR_INFO_UNKNOWN;
					pSensorInfo->hystLo = KEAPI_SENSOR_INFO_UNKNOWN;

					/* Get real sensor name from sensors conf file */
					snprintf(path, KEAPI_MAX_STR, "%s/name", base_path);
					if ((ret = checkRAccess(path)) != KEAPI_RET_SUCCESS)
						goto read_error;

					if ((ret = ReadFile(path, &info)) != KEAPI_RET_SUCCESS)
						goto read_error;

					strncpy(chip, info, KEAPI_MAX_STR);
					free(info);

					/* Get rid of \n at the end */
					if (chip[strnlen(chip, KEAPI_MAX_STR) - 1] == '\n')
						chip[strnlen(chip, KEAPI_MAX_STR) - 1] = '\0';

					/* Read names from sensors conf file */
					if ((ret = checkRAccess(path)) != KEAPI_RET_SUCCESS) {
						if (ret == KEAPI_RET_RETRIEVAL_ERROR) {
							closedir(dir2);
							closedir(dir);
							return KEAPI_RET_SUCCESS;
						} else
							goto read_error;
					}

					if ((ret = checkRAccess(GetSensorsConfName())) != KEAPI_RET_SUCCESS) {
						if (ret == KEAPI_RET_RETRIEVAL_ERROR)
							goto type_detect;
						else
							goto read_error;
					}

					if ((ret = ReadFile(GetSensorsConfName(), &info)) != KEAPI_RET_SUCCESS)
						goto read_error;

					/* Create regex */
					snprintf(regex, KEAPI_MAX_STR, "(%s\\-\\*.*?)(?:^chip|\\Z)", chip);
					if ((re = pcre_compile(regex,
							       PCRE_MULTILINE | PCRE_NEWLINE_ANYCRLF | PCRE_DOTALL,
							       &error, &erroffset, NULL)) == NULL) {
						free(info);
						continue;
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
						if (pSensorInfo->name[3] == '\0')
							snprintf(regex, KEAPI_MAX_STR, "label %.3s \\\"(.*?)\\\"",
								 ent2->d_name);
						else
							snprintf(regex, KEAPI_MAX_STR, "label %.4s \\\"(.*?)\\\"",
								 ent2->d_name);
						if ((re2 = pcre_compile(
							     regex, PCRE_MULTILINE | PCRE_NEWLINE_ANYCRLF | PCRE_DOTALL,
							     &error, &erroffset, NULL)) == NULL) {
							pcre_free(re);
							free(info);
							continue;
						}

						if ((rc = pcre_exec(re2, NULL, sensors_conf, strlen(sensors_conf), 0, 0,
								    ovector, 30)) == 2) {
							len = ovector[3] - ovector[2];
							substr = (char *)malloc(len + 1);
							memcpy(substr, sensors_conf + ovector[2], len);
							substr[len] = '\0';
							strncpy(pSensorInfo->name, substr, KEAPI_MAX_STR);
							free(substr);
						}
						pcre_free(re2);

						/* Correct sensor's value */
						/* Multiply measured value by value in sensors.conf */
						/* IT HAS TO BE A SINGLE NUMBER LIKE @*0.123456 + 0.654321, NOT *((@ + 3)/10) */

						/*FIXME: DIRTY. MAKE it in FINE WAY!
						 * sensors.conf cannot contain values like in0_max with compute statement. Use
						 * compute statement for sensor itself. */

						int32_t *pointerToCorrectingVal, k;

						for (k = 0; k < 4; k++) {
							if (k == 0)
								pointerToCorrectingVal = &pSensorInfo->max;
							else if (k == 1)
								pointerToCorrectingVal = &pSensorInfo->min;
							else if (k == 2)
								pointerToCorrectingVal = &pSensorInfo->alarmHi;
							else if (k == 4)
								pointerToCorrectingVal = &pSensorInfo->alarmLo;

							if (*pointerToCorrectingVal == KEAPI_SENSOR_INFO_UNKNOWN) {
								/* Do not correct KEAPI_SENSOR_INFO_UNKNOWN value */
								continue;
							}

							/*FIXME: Move regex out of loop */
							snprintf(
								regex, KEAPI_MAX_STR,
								"^(?:\\s|[^#]*?)?compute\\s*%s\\s*\\@\\s*?\\*?\\s*?((?:\\d+\\.)?\\d+)?\\s*?(\\-|\\+)\\s*?((?:\\d+\\.)?\\d+)?,",
								sensorName);

							if ((re2 = pcre_compile(regex,
										PCRE_MULTILINE | PCRE_NEWLINE_ANYCRLF |
											PCRE_DOTALL,
										&error, &erroffset, NULL)) == NULL) {
								free(sensors_conf);
								pcre_free(re);
								free(info);
								continue;
							}

							if ((rc = pcre_exec(re2, NULL, sensors_conf,
									    strlen(sensors_conf), 0, 0, ovector, 30)) >=
							    2) {
								int i;

								/*FIXME: correction can be float, e.g. 10.1. Currently we will lose fraction value.*/
								int correction[3] = { 1, 0, 0 };

								for (i = 2; i <= rc; i++) {
									len = ovector[2 * i - 1] - ovector[2 * i - 2];
									substr = (char *)malloc(len + 1);

									memcpy(substr,
									       sensors_conf + ovector[2 * i - 2], len);

									substr[len] = '\0';
									val_flt = strtof(substr, NULL);

									if ((val_flt != 0.0))
										correction[i - 2] = val_flt;
									else {
										if (strncmp(substr, "+", 1) == 0)
											correction[1] = 1;
										else if (strncmp(substr, "-", 1) == 0)
											correction[1] = -1;
									}
									free(substr);
								}

								(*pointerToCorrectingVal) =
									(*pointerToCorrectingVal) * correction[0] +
									correction[1] * correction[2] * 1000;
							}
							pcre_free(re2);
						} /*for (k = 0; k < 4; k++) */

						free(sensors_conf);
					} /*if pcre found */

					pcre_free(re);
					free(info);

				type_detect:
					/* setup type of sensor */
					if (strcasecmp(pSensorInfo->name, "12v") == 0)
						pSensorInfo->type = KEAPI_VOLTAGE_12V;
					else if (strcasecmp(pSensorInfo->name, "1v8") == 0)
						pSensorInfo->type = KEAPI_VOLTAGE_1V8;
					else if (strcasecmp(pSensorInfo->name, "2v5") == 0)
						pSensorInfo->type = KEAPI_VOLTAGE_2V5;
					else if (strcasecmp(pSensorInfo->name, "3v3") == 0)
						pSensorInfo->type = KEAPI_VOLTAGE_3V3;
					else if (strcasecmp(pSensorInfo->name, "5v") == 0 &&
						 strlen(pSensorInfo->name) == strlen("5v"))
						pSensorInfo->type = KEAPI_VOLTAGE_5V;
					else if (strcasecmp(pSensorInfo->name, "5vsb") == 0)
						pSensorInfo->type = KEAPI_VOLTAGE_5VSB;
					else if (strcasecmp(pSensorInfo->name, "vac") == 0)
						pSensorInfo->type = KEAPI_VOLTAGE_AC;
					else if (strcasecmp(pSensorInfo->name, "vbat") == 0)
						pSensorInfo->type = KEAPI_VOLTAGE_VBAT;
					else if (strcasecmp(pSensorInfo->name, "vcore") == 0)
						pSensorInfo->type = KEAPI_VOLTAGE_VCORE;
					else
						pSensorInfo->type = KEAPI_VOLTAGE_OTHER;

					closedir(dir2);
					closedir(dir);

					return KEAPI_RET_SUCCESS;
				} /* if index == sensorNr*/
				index++;
			} /* if FOUND == TRUE*/
		} /* open dir /sys/class/hwmon/hwmonX*/
		closedir(dir2);
	} /*open dir /sys/class/hwmon */

	closedir(dir);

	return KEAPI_RET_ERROR;

read_error:
	closedir(dir2);
	closedir(dir);
	return ret;
}
