/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron AG */

/* KEAPI battery information functions. */

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <regex.h>

#include "keapi_inc.h"

/*******************************************************************************/
KEAPI_RETVAL KEApiL_GetBatteryCount(int32_t *pBatteryCount)
{
	DIR *dir;
	struct dirent *ent;
	char path[PATH_MAX], *info;
	int8_t found;
	int32_t ret;

	/* Check function parameters */
	if (pBatteryCount == NULL)
		return KEAPI_RET_PARAM_NULL;

	*pBatteryCount = 0;

	if ((ret = checkRAccess("/sys/class/power_supply")) != KEAPI_RET_SUCCESS) {
		if (ret == KEAPI_RET_RETRIEVAL_ERROR)
			return KEAPI_RET_SUCCESS; // no files = no batteries
		else
			return ret;
	}

	if ((dir = opendir("/sys/class/power_supply")) != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			if (ent->d_name[0] == '.')
				continue;

			snprintf(path, PATH_MAX, "/sys/class/power_supply/%s/type", ent->d_name);

			if ((ret = checkRAccess(path)) != KEAPI_RET_SUCCESS) {
				if (ret == KEAPI_RET_RETRIEVAL_ERROR) // if no file, its OK
					continue;
				else
					return ret;
			}

			if ((ret = ReadFile(path, &info)) != KEAPI_RET_SUCCESS)
				return ret;

			if (FindRegex(info, "Battery", &found, REG_EXTENDED | REG_NEWLINE) != KEAPI_RET_SUCCESS) {
				free(info);
				continue;
			}

			if (found == TRUE)
				(*pBatteryCount)++;

			free(info);
		}
		closedir(dir);
		return KEAPI_RET_SUCCESS;
	}

	/* Let's try to get battery info using good old ACPI battery interface */
	if ((ret = checkRAccess(BATTERY_PATH)) != KEAPI_RET_SUCCESS) {
		if (ret == KEAPI_RET_RETRIEVAL_ERROR)
			return KEAPI_RET_SUCCESS; // no files = no batteries
		else
			return ret;
	}

	if ((dir = opendir(BATTERY_PATH)) != NULL) {
		/* Count directories (batteries)
		 * only if directory contains info file and "present: yes" */
		while ((ent = readdir(dir)) != NULL) {
			if (ent->d_name[0] == '.')
				continue;

			if (snprintf(path, KEAPI_MAX_STR, "%s/%s/info", BATTERY_PATH, ent->d_name) < 0)
				continue;

			if ((ret = checkRAccess(path)) != KEAPI_RET_SUCCESS) {
				if (ret == KEAPI_RET_RETRIEVAL_ERROR) // if no file, its OK
					continue;
				else
					return ret;
			}

			if ((ret = ReadFile(path, &info)) != KEAPI_RET_SUCCESS)
				return ret;

			if (FindRegex(info, "present:\\s+yes", &found, REG_EXTENDED | REG_NEWLINE) !=
			    KEAPI_RET_SUCCESS) {
				free(info);
				continue;
			}

			if (found == TRUE)
				(*pBatteryCount)++;

			free(info);
		}

		closedir(dir);
	} /* opendir(BATTERY_PATH)) != NULL */

	return KEAPI_RET_SUCCESS;
}

/*******************************************************************************/
KEAPI_RETVAL KEApiL_GetBatteryInfo(int32_t slotNr, PKEAPI_BATTERY_INFO pBatteryInfo)
{
	DIR *dir;
	struct dirent *ent;
	char path[PATH_MAX], *info, *substr;
	int8_t found, powsup_used = FALSE;
	int32_t count, realDevNum, ret, retval = KEAPI_RET_ERROR;

	/* Check function parameters */
	if (pBatteryInfo == NULL)
		return KEAPI_RET_PARAM_NULL;

	if ((ret = KEApiL_GetBatteryCount(&realDevNum)) != KEAPI_RET_SUCCESS)
		return ret;

	if (slotNr < 0 || slotNr >= realDevNum)
		return KEAPI_RET_PARAM_ERROR;

	/* Initialization */
	pBatteryInfo->deviceName[0] = '\0';
	pBatteryInfo->type[0] = '\0';
	pBatteryInfo->serialNumber[0] = '\0';
	pBatteryInfo->designedVoltage = 0;
	pBatteryInfo->designedCapacity = 0;
	pBatteryInfo->fullyChargedCapacity = 0;
	pBatteryInfo->cycleCount = 0;

	if ((ret = checkRAccess("/sys/class/power_supply/")) != KEAPI_RET_SUCCESS) {
		if (ret == KEAPI_RET_RETRIEVAL_ERROR)
			goto acpi_search;
	}

	if ((dir = opendir("/sys/class/power_supply/")) != NULL) {
		count = 0;

		while ((ent = readdir(dir)) != NULL) {
			if (ent->d_name[0] == '.')
				continue;

			snprintf(path, PATH_MAX, "/sys/class/power_supply/%s/type", ent->d_name);

			if ((ret = checkRAccess(path)) != KEAPI_RET_SUCCESS) {
				if (ret == KEAPI_RET_RETRIEVAL_ERROR)
					continue;
				else
					return ret;
			}

			powsup_used = 1;

			if ((ret = ReadFile(path, &info)) != KEAPI_RET_SUCCESS)
				return ret;

			if (FindRegex(info, "Battery", &found, REG_EXTENDED | REG_NEWLINE) != KEAPI_RET_SUCCESS) {
				free(info);
				continue;
			}
			free(info);

			if (found)
				count++;

			/* if it is not needed bat, continue */
			if (count - 1 != slotNr)
				continue;

			retval = KEAPI_RET_SUCCESS;

			/* Device name */
			strncat(pBatteryInfo->deviceName, ent->d_name, KEAPI_MAX_STR - 1);

			/* Type */
			snprintf(path, PATH_MAX, "/sys/class/power_supply/%s/technology", ent->d_name);

			if ((ret = checkRAccess(path)) == KEAPI_RET_SUCCESS) {
				if ((ret = ReadFile(path, &info)) == KEAPI_RET_SUCCESS) {
					/* Remove \n at the end */
					if (info[strlen(info) - 1] == '\n')
						info[strlen(info) - 1] = '\0';
					strncat(pBatteryInfo->type, info, KEAPI_MAX_STR - 1);
					free(info);

				} else
					return ret;

			} else if (ret == KEAPI_RET_RETRIEVAL_ERROR) {
				retval = KEAPI_RET_PARTIAL_SUCCESS;

			} else {
				closedir(dir);
				return ret;
			}

			/* Serial number */
			snprintf(path, PATH_MAX, "/sys/class/power_supply/%s/serial_number", ent->d_name);

			if ((ret = checkRAccess(path)) == KEAPI_RET_SUCCESS) {
				if ((ret = ReadFile(path, &info)) == KEAPI_RET_SUCCESS) {
					/* Remove \n at the end */
					if (info[strlen(info) - 1] == '\n')
						info[strlen(info) - 1] = '\0';
					strncat(pBatteryInfo->serialNumber, info, KEAPI_MAX_STR - 1);
					free(info);

				} else
					return ret;

			} else if (ret == KEAPI_RET_RETRIEVAL_ERROR) {
				retval = KEAPI_RET_PARTIAL_SUCCESS;

			} else {
				closedir(dir);
				return ret;
			}

			/* Designed voltage */
			snprintf(path, PATH_MAX, "/sys/class/power_supply/%s/voltage_max_design", ent->d_name);

			if ((ret = checkRAccess(path)) == KEAPI_RET_SUCCESS) {
				if ((ret = ReadFile(path, &info)) == KEAPI_RET_SUCCESS) {
					pBatteryInfo->designedVoltage = atoi(info);
					free(info);

				} else
					return ret;

			} else if (ret == KEAPI_RET_RETRIEVAL_ERROR) {
				retval = KEAPI_RET_PARTIAL_SUCCESS;

			} else {
				closedir(dir);
				return ret;
			}

			/* Designed capacity */
			snprintf(path, PATH_MAX, "/sys/class/power_supply/%s/charge_full_design", ent->d_name);

			if ((ret = checkRAccess(path)) == KEAPI_RET_SUCCESS) {
				if ((ret = ReadFile(path, &info)) == KEAPI_RET_SUCCESS) {
					pBatteryInfo->designedCapacity = atoi(info);
					free(info);

				} else
					return ret;

			} else if (ret == KEAPI_RET_RETRIEVAL_ERROR) {
				retval = KEAPI_RET_PARTIAL_SUCCESS;

			} else {
				closedir(dir);
				return ret;
			}

			/* Fully charged capacity */
			snprintf(path, PATH_MAX, "/sys/class/power_supply/%s/charge_full", ent->d_name);

			if ((ret = checkRAccess(path)) == KEAPI_RET_SUCCESS) {
				if ((ret = ReadFile(path, &info)) == KEAPI_RET_SUCCESS) {
					pBatteryInfo->fullyChargedCapacity = atoi(info);
					free(info);

				} else
					return ret;

			} else if (ret == KEAPI_RET_RETRIEVAL_ERROR) {
				retval = KEAPI_RET_PARTIAL_SUCCESS;

			} else {
				closedir(dir);
				return ret;
			}

			/* Cycle count */
			snprintf(path, PATH_MAX, "/sys/class/power_supply/%s/cycle_count", ent->d_name);

			if ((ret = checkRAccess(path)) == KEAPI_RET_SUCCESS) {
				if ((ret = ReadFile(path, &info)) == KEAPI_RET_SUCCESS) {
					pBatteryInfo->cycleCount = atoi(info);
					free(info);

				} else
					return ret;

			} else if (ret == KEAPI_RET_RETRIEVAL_ERROR) {
				retval = KEAPI_RET_PARTIAL_SUCCESS;

			} else {
				closedir(dir);
				return ret;
			}

			closedir(dir);
			return retval;
		}
	}

	/* if /sys/class/power_supply isn't empty, return here
	 * else fall back to ACPI method */
	if (powsup_used)
		return retval;

acpi_search:

	if ((ret = checkRAccess(BATTERY_PATH)) != KEAPI_RET_SUCCESS)
		return ret;

	if ((dir = opendir(BATTERY_PATH)) == NULL)
		return KEAPI_RET_RETRIEVAL_ERROR;

	/* Count directories (batteries)
	 * only if directory contains info file and "present: yes" */
	count = 0;
	while ((ent = readdir(dir)) != NULL) {
		if (ent->d_name[0] == '.')
			continue;

		snprintf(path, PATH_MAX, "%s/%s/info", BATTERY_PATH, ent->d_name);

		if ((ret = checkRAccess(path)) != KEAPI_RET_SUCCESS) {
			if (ret == KEAPI_RET_RETRIEVAL_ERROR)
				continue;
			else {
				closedir(dir);
				return ret;
			}
		}

		if ((ret = ReadFile(path, &info)) != KEAPI_RET_SUCCESS)
			return ret;

		if (FindRegex(info, "present:\\s+yes", &found, REG_EXTENDED | REG_NEWLINE) != KEAPI_RET_SUCCESS) {
			free(info);
			continue;
		}

		if (found == TRUE)
			count++;

		/* Check Battery number (starts from zero) */
		if (count - 1 != slotNr) {
			free(info);
			continue;
		}

		/* Device name */
		if (GetSubStrRegex(info, "model number:\\s+(.*)", &substr, REG_EXTENDED | REG_NEWLINE) !=
		    KEAPI_RET_SUCCESS)
			retval = KEAPI_RET_PARTIAL_SUCCESS;
		else {
			strncat(pBatteryInfo->deviceName, substr, KEAPI_MAX_STR - 1);
			free(substr);
		}

		/* Type */
		if (GetSubStrRegex(info, "battery type:\\s+(.*)", &substr, REG_EXTENDED | REG_NEWLINE) !=
		    KEAPI_RET_SUCCESS)
			retval = KEAPI_RET_PARTIAL_SUCCESS;
		else {
			strncat(pBatteryInfo->type, substr, KEAPI_MAX_STR - 1);
			free(substr);
		}

		/* Serial number */
		if (GetSubStrRegex(info, "serial number:\\s+(.*)", &substr, REG_EXTENDED | REG_NEWLINE) !=
		    KEAPI_RET_SUCCESS)
			ret = KEAPI_RET_PARTIAL_SUCCESS;
		else {
			strncat(pBatteryInfo->serialNumber, substr, KEAPI_MAX_STR - 1);
			free(substr);
		}

		/* Designed voltage */
		if (GetSubStrRegex(info, "design voltage:\\s+(.*) mV", &substr, REG_EXTENDED | REG_NEWLINE) !=
		    KEAPI_RET_SUCCESS)
			ret = KEAPI_RET_PARTIAL_SUCCESS;
		else {
			pBatteryInfo->designedVoltage = atoi(substr);
			free(substr);
		}

		/* Designed capacity */
		if (GetSubStrRegex(info, "design capacity:\\s+(.*) mAh", &substr, REG_EXTENDED | REG_NEWLINE) !=
		    KEAPI_RET_SUCCESS)
			ret = KEAPI_RET_PARTIAL_SUCCESS;
		else {
			pBatteryInfo->designedCapacity = atoi(substr);
			free(substr);
		}

		/* Fully charged capacity */
		if (GetSubStrRegex(info, "last full capacity:\\s+(.*) mAh", &substr, REG_EXTENDED | REG_NEWLINE) !=
		    KEAPI_RET_SUCCESS)
			ret = KEAPI_RET_PARTIAL_SUCCESS;
		else {
			pBatteryInfo->fullyChargedCapacity = atoi(substr);
			free(substr);
		}

		/* Cycle count */
		retval = KEAPI_RET_PARTIAL_SUCCESS;
		pBatteryInfo->cycleCount = 0;

		free(info);
		closedir(dir);
		return retval;
	}
	closedir(dir);
	return retval;
}

/*******************************************************************************/
KEAPI_RETVAL KEApiL_GetBatteryState(int32_t slotNr, PKEAPI_BATTERY_STATE pBatteryState)
{
	DIR *dir;
	struct dirent *ent;
	char path[PATH_MAX], *info, *substr;
	int8_t found, powsup_used = 0;
	int32_t count, realDevNum, ret, retval = KEAPI_RET_ERROR;

	/* Check function parameters */
	if (pBatteryState == NULL)
		return KEAPI_RET_PARAM_NULL;

	if ((ret = KEApiL_GetBatteryCount(&realDevNum)) != KEAPI_RET_SUCCESS)
		return ret;

	if (slotNr < 0 || slotNr >= realDevNum)
		return KEAPI_RET_PARAM_ERROR;

	/* Initialization */
	pBatteryState->powerState = -1;
	pBatteryState->fullBatteryRemainingTime = -1;
	pBatteryState->remainingTime = -1;
	pBatteryState->remainingCapacity = -1;
	pBatteryState->currentVoltage = -1;
	pBatteryState->rate = -1;
	pBatteryState->chargeState = -1;

	if ((ret = checkRAccess("/sys/class/power_supply/")) != KEAPI_RET_SUCCESS) {
		if (ret == KEAPI_RET_RETRIEVAL_ERROR)
			goto acpi_search;
	}

	if ((dir = opendir("/sys/class/power_supply/")) != NULL) {
		count = 0;

		while ((ent = readdir(dir)) != NULL) {
			if (ent->d_name[0] == '.')
				continue;

			snprintf(path, PATH_MAX, "/sys/class/power_supply/%s/type", ent->d_name);

			if ((ret = checkRAccess(path)) != KEAPI_RET_SUCCESS) {
				if (ret == KEAPI_RET_RETRIEVAL_ERROR)
					continue;
				else
					return ret;
			}

			powsup_used = 1;

			if ((ret = ReadFile(path, &info)) != KEAPI_RET_SUCCESS)
				return ret;

			if (FindRegex(info, "Battery", &found, REG_EXTENDED | REG_NEWLINE) != KEAPI_RET_SUCCESS) {
				free(info);
				continue;
			}
			free(info);

			if (found)
				count++;

			/* Battery numbers start from zero */
			if (count - 1 != slotNr)
				continue;

			retval = KEAPI_RET_SUCCESS;

			/* Get status */
			snprintf(path, PATH_MAX, "/sys/class/power_supply/%s/status", ent->d_name);

			if ((ret = checkRAccess(path)) == KEAPI_RET_SUCCESS) {
				if ((ret = ReadFile(path, &info)) == KEAPI_RET_SUCCESS) {
					if (strcmp(info, "Charging\n") == 0)
						pBatteryState->powerState = KEAPI_POWER_STATE_CHARGING;
					else if (strcmp(info, "Full\n") == 0) {
						pBatteryState->powerState = KEAPI_POWER_STATE_CHARGED;
					} else if (strcmp(info, "Discharging\n") == 0)
						pBatteryState->powerState = KEAPI_POWER_STATE_DISCHARGING;
					else
						retval = KEAPI_RET_PARTIAL_SUCCESS;

					free(info);

				} else
					return ret;

			} else if (ret == KEAPI_RET_RETRIEVAL_ERROR) {
				retval = KEAPI_RET_PARTIAL_SUCCESS;

			} else {
				closedir(dir);
				return ret;
			}

			/* GET TIME TO EMPTY NOW*/
			snprintf(path, PATH_MAX, "/sys/class/power_supply/%s/time_to_empty_now", ent->d_name);

			if ((ret = checkRAccess(path)) == KEAPI_RET_SUCCESS) {
				if ((ret = ReadFile(path, &info)) == KEAPI_RET_SUCCESS) {
					pBatteryState->remainingTime = atoi(info);
					free(info);

				} else
					return ret;

			} else if (ret == KEAPI_RET_RETRIEVAL_ERROR) {
				retval = KEAPI_RET_PARTIAL_SUCCESS;

			} else {
				closedir(dir);
				return ret;
			}

			/* GET CHARGE NOW*/
			snprintf(path, PATH_MAX, "/sys/class/power_supply/%s/charge_now", ent->d_name);

			if ((ret = checkRAccess(path)) == KEAPI_RET_SUCCESS) {
				if ((ret = ReadFile(path, &info)) == KEAPI_RET_SUCCESS) {
					pBatteryState->remainingCapacity = atoi(info);
					free(info);

				} else
					return ret;

			} else if (ret == KEAPI_RET_RETRIEVAL_ERROR) {
				retval = KEAPI_RET_PARTIAL_SUCCESS;

			} else {
				closedir(dir);
				return ret;
			}

			/* GET VOLTAGE NOW*/
			snprintf(path, PATH_MAX, "/sys/class/power_supply/%s/voltage_now", ent->d_name);

			if ((ret = checkRAccess(path)) == KEAPI_RET_SUCCESS) {
				if ((ret = ReadFile(path, &info)) == KEAPI_RET_SUCCESS) {
					pBatteryState->currentVoltage = atoi(info);
					free(info);

				} else
					return ret;

			} else if (ret == KEAPI_RET_RETRIEVAL_ERROR) {
				retval = KEAPI_RET_PARTIAL_SUCCESS;

			} else {
				closedir(dir);
				return ret;
			}

			/* GET CURRENT NOW */
			snprintf(path, PATH_MAX, "/sys/class/power_supply/%s/current_now", ent->d_name);

			if ((ret = checkRAccess(path)) == KEAPI_RET_SUCCESS) {
				if ((ret = ReadFile(path, &info)) == KEAPI_RET_SUCCESS) {
					pBatteryState->rate = abs(atoi(info)); /* FIXME: is abs really needed? */
					free(info);

				} else
					return ret;

			} else if (ret == KEAPI_RET_RETRIEVAL_ERROR) {
				retval = KEAPI_RET_PARTIAL_SUCCESS;

			} else {
				closedir(dir);
				return ret;
			}

			/* GET CAPACITY*/

			snprintf(path, PATH_MAX, "/sys/class/power_supply/%s/capacity", ent->d_name);

			if ((ret = checkRAccess(path)) == KEAPI_RET_SUCCESS) {
				if ((ret = ReadFile(path, &info)) == KEAPI_RET_SUCCESS) {
					pBatteryState->chargeState = atoi(info);
					free(info);

				} else
					return ret;

			} else if (ret == KEAPI_RET_RETRIEVAL_ERROR) {
				retval = KEAPI_RET_PARTIAL_SUCCESS;

			} else {
				closedir(dir);
				return ret;
			}

			/* CALCULATE fullBatteryRemainingTime*/
			if (pBatteryState->remainingTime >= 0 && pBatteryState->chargeState >= 0) {
				pBatteryState->fullBatteryRemainingTime =
					(pBatteryState->remainingTime * 100) / pBatteryState->chargeState;
			} else
				retval = KEAPI_RET_PARTIAL_SUCCESS;

			closedir(dir);
			return retval;
		}

		closedir(dir);
	}

	/* if /sys/class/power_supply isn't empty, return here
	 * else fall back to ACPI method */
	if (powsup_used)
		return retval;

acpi_search:

	if ((ret = checkRAccess(BATTERY_PATH)) != KEAPI_RET_SUCCESS)
		return ret;

	if ((dir = opendir(BATTERY_PATH)) == NULL)
		return KEAPI_RET_RETRIEVAL_ERROR;

	/* Count directories (batteries)
	 * only if directory contains info file and "present: yes" */
	count = 0;
	while ((ent = readdir(dir)) != NULL) {
		if (ent->d_name[0] == '.')
			continue;

		snprintf(path, PATH_MAX, "%s/%s/state", BATTERY_PATH, ent->d_name);

		if ((ret = checkRAccess(path)) != KEAPI_RET_SUCCESS) {
			if (ret == KEAPI_RET_RETRIEVAL_ERROR)
				continue;
			else {
				closedir(dir);
				return ret;
			}
		}

		if ((ret = ReadFile(path, &info)) != KEAPI_RET_SUCCESS)
			return ret;

		if (FindRegex(info, "present:\\s+yes", &found, REG_EXTENDED | REG_NEWLINE) != KEAPI_RET_SUCCESS) {
			free(info);
			continue;
		}

		if (found == TRUE)
			count++;

		/* Check Battery number (starts from zero) */
		if (count - 1 != slotNr) {
			free(info);
			continue;
		}

		retval = KEAPI_RET_SUCCESS;

		/* Power state */
		if (GetSubStrRegex(info, "charging state:\\s+(.*)", &substr, REG_EXTENDED | REG_NEWLINE) !=
		    KEAPI_RET_SUCCESS)
			retval = KEAPI_RET_PARTIAL_SUCCESS;
		else {
			if (strcmp(substr, "charging") == 0)
				pBatteryState->powerState = KEAPI_POWER_STATE_CHARGING;
			else if (strcmp(substr, "charged") == 0)
				pBatteryState->powerState = KEAPI_POWER_STATE_CHARGED;
			else if (strcmp(substr, "discharging") == 0)
				pBatteryState->powerState = KEAPI_POWER_STATE_DISCHARGING;
			else
				retval = KEAPI_RET_PARTIAL_SUCCESS;

			free(substr);
		}

		/* Charging/discharging rate */
		if (GetSubStrRegex(info, "present rate:\\s+(.*) mA", &substr, REG_EXTENDED | REG_NEWLINE) !=
		    KEAPI_RET_SUCCESS)
			retval = KEAPI_RET_PARTIAL_SUCCESS;
		else {
			pBatteryState->rate = atoi(substr);
			free(substr);
		}

		/* Current voltage */
		if (GetSubStrRegex(info, "present voltage:\\s+(.*) mV", &substr, REG_EXTENDED | REG_NEWLINE) !=
		    KEAPI_RET_SUCCESS)
			retval = KEAPI_RET_PARTIAL_SUCCESS;
		else {
			pBatteryState->currentVoltage = atoi(substr);
			free(substr);
		}

		/* Remaining capacity */
		if (GetSubStrRegex(info, "remaining capacity:\\s+(.*) mAh", &substr, REG_EXTENDED | REG_NEWLINE) !=
		    KEAPI_RET_SUCCESS)
			retval = KEAPI_RET_PARTIAL_SUCCESS;
		else {
			pBatteryState->remainingCapacity = atoi(substr);
			free(substr);
		}

		/* Charge state in % (it is possible that rate == 0 && chargeState != 100 */
		if (pBatteryState->powerState == KEAPI_POWER_STATE_CHARGED) {
			pBatteryState->chargeState = 100;
		}
		free(info);

		/* Remaining time */
		if (pBatteryState->rate == 0) {
			pBatteryState->remainingTime = 0;
			pBatteryState->fullBatteryRemainingTime = 0;
			continue;
		}

		pBatteryState->remainingTime = 3600 * pBatteryState->remainingCapacity / pBatteryState->rate;

		/* Battery charge state and Full battery remaining time - last full capacity needed from info file */
		if (snprintf(path, PATH_MAX, "%s/%s/info", BATTERY_PATH, ent->d_name) < 0) {
			retval = KEAPI_RET_PARTIAL_SUCCESS;
			continue;
		}

		if (ReadFile(path, &info) != KEAPI_RET_SUCCESS) {
			retval = KEAPI_RET_PARTIAL_SUCCESS;
			continue;
		}

		if (GetSubStrRegex(info, "last full capacity:\\s+(.*) mAh", &substr, REG_EXTENDED | REG_NEWLINE) !=
		    KEAPI_RET_SUCCESS)
			retval = KEAPI_RET_PARTIAL_SUCCESS;
		else {
			pBatteryState->fullBatteryRemainingTime = 3600 * atoi(substr) / pBatteryState->rate;
			pBatteryState->chargeState = 100 * pBatteryState->remainingCapacity / atoi(substr);
			free(substr);
		}

		free(info);
		closedir(dir);
		return retval;
	}

	closedir(dir);
	return retval;
}
