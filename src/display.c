/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron Europe GmbH */

/* Display's backlight and contrast control functions. */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

#include "globals.h"
#include "keapi_inc.h"

void FreeDisplayPathArr()
{
	int i;

	if (dispPathArr || dispPathArrCount) {
		for (i = 0; i < dispPathArrCount; i++)
			free(dispPathArr[i]);
		free(dispPathArr);
		dispPathArr = NULL;
		dispPathArrCount = 0;
	}
}

KEAPI_RETVAL GetDisplayPathArr()
{
	DIR *dir;
	struct dirent *ent;
	char conf_paths[KEAPI_MAX_STR][PATH_MAX];
	int32_t conf_display_count, i, ret;

	FreeDisplayPathArr();

	if ((ret = checkRAccess(DISPLAY_PATH)) != KEAPI_RET_SUCCESS)
		return ret;

	if ((dir = opendir(DISPLAY_PATH)) == NULL)
		return KEAPI_RET_ERROR;

	/* If we found configuration file with remapping information
	 * trying to read it */
	if ((ret = checkRAccess(BCKL_CONF_PATH)) == KEAPI_RET_SUCCESS) {
		if ((ret = findRemapping(BCKL_CONF_PATH, conf_paths, &conf_display_count)) == KEAPI_RET_SUCCESS) {
			for (i = 0; i < conf_display_count; i++) {
				if (checkRAccess(conf_paths[i]) == 0) {
					dispPathArr = realloc(dispPathArr, sizeof(char *) * (dispPathArrCount + 1));
					dispPathArr[dispPathArrCount] = calloc(KEAPI_MAX_STR, sizeof(char));
					strncat(dispPathArr[dispPathArrCount], conf_paths[i], KEAPI_MAX_STR - 1);
					dispPathArrCount++;
				}
			}

			closedir(dir);
			return KEAPI_RET_SUCCESS;
		} else
			goto error;

	} else if (ret != KEAPI_RET_RETRIEVAL_ERROR) /* there is file but something wrong */
		goto error;

	/* fallback to legacy search */
	while ((ent = readdir(dir)) != NULL) {
		if (ent->d_name[0] == '.')
			continue;

		snprintf(conf_paths[0], PATH_MAX, "%s/%s/device", DISPLAY_PATH, ent->d_name);

		if ((ret = checkRAccess(conf_paths[0])) != KEAPI_RET_SUCCESS) {
			/* if ret=KEAPI_RET_RETRIEVAL_ERROR it means siply that "device" subdir
			 * doesn't exist => OK */
			if (ret != KEAPI_RET_RETRIEVAL_ERROR)
				goto error;
		}

		dispPathArr = realloc(dispPathArr, sizeof(char *) * (dispPathArrCount + 1));
		dispPathArr[dispPathArrCount] = calloc(KEAPI_MAX_STR, sizeof(char));
		snprintf(dispPathArr[dispPathArrCount], PATH_MAX - 1, "%s/%s", DISPLAY_PATH, ent->d_name);
		dispPathArrCount++;
	}

	closedir(dir);
	return KEAPI_RET_SUCCESS;

error:
	closedir(dir);
	FreeDisplayPathArr();
	return ret;
}

/*******************************************************************************/
KEAPI_RETVAL KEApiL_GetDisplayCount(int32_t *pDisplayCount)
{
	int ret;

	/* Check function parameters */
	if (pDisplayCount == NULL)
		return KEAPI_RET_PARAM_NULL;

	/* Initialization */
	*pDisplayCount = 0;

	if (dispPathArr == NULL || dispPathArrCount == 0) {
		ret = GetDisplayPathArr();
		if (ret != KEAPI_RET_SUCCESS)
			return ret;
	}

	*pDisplayCount = dispPathArrCount;
	return KEAPI_RET_SUCCESS;
}

/*******************************************************************************/
KEAPI_RETVAL KEApiL_GetBacklightValue(int32_t displayNr, int32_t *pBacklight)
{
	char pathFull[PATH_MAX];
	FILE *fr;
	int32_t max_brightness, actual_brightness, realDisplayCount;
	uint32_t ret;

	/* Check function parameters */
	if (pBacklight == NULL)
		return KEAPI_RET_PARAM_NULL;

	ret = KEApiL_GetDisplayCount(&realDisplayCount);
	if (ret != KEAPI_RET_SUCCESS)
		return ret;

	if (displayNr < 0 || displayNr >= realDisplayCount)
		return KEAPI_RET_PARAM_ERROR;

	/* Initialization */
	*pBacklight = 0;

	/* Get maximal brightness */
	snprintf(pathFull, PATH_MAX, "%s/max_brightness", dispPathArr[displayNr]);

	if ((ret = checkRAccess(pathFull)) != KEAPI_RET_SUCCESS)
		return ret;

	if ((fr = fopen(pathFull, "r")) == NULL)
		return KEAPI_RET_ERROR;

	if (fscanf(fr, "%d", &max_brightness) == EOF) {
		fclose(fr);
		return KEAPI_RET_RETRIEVAL_ERROR;
	}

	fclose(fr);

	/* Get actual brightness */
	snprintf(pathFull, PATH_MAX, "%s/actual_brightness", dispPathArr[displayNr]);

	if ((ret = checkRAccess(pathFull)) != KEAPI_RET_SUCCESS)
		return ret;

	if ((fr = fopen(pathFull, "r")) == NULL)
		return KEAPI_RET_ERROR;

	if (fscanf(fr, "%d", &actual_brightness) == EOF) {
		fclose(fr);
		return KEAPI_RET_RETRIEVAL_ERROR;
		;
	}

	fclose(fr);

	/* Re-scale brightness range supported by driver to 0..255 */
	*pBacklight = 255 * actual_brightness / max_brightness;

	return KEAPI_RET_SUCCESS;
}

/*******************************************************************************/
KEAPI_RETVAL KEApiL_SetBacklightValue(int32_t displayNr, int32_t backlight)
{
	FILE *fd;
	char pathFull[PATH_MAX];
	int32_t max_brightness, target_brightness, realDisplayCount, ret;

	ret = KEApiL_GetDisplayCount(&realDisplayCount);
	if (ret != KEAPI_RET_SUCCESS)
		return ret;

	/* Check function parameters */
	if (displayNr < 0 || displayNr >= realDisplayCount || backlight < 0 || backlight > KEAPI_DISPLAY_BRIGHTNESS_MAX)
		return KEAPI_RET_PARAM_ERROR;

	/* Get maximal brightness */
	snprintf(pathFull, PATH_MAX, "%s/max_brightness", dispPathArr[displayNr]);

	if ((ret = checkRAccess(pathFull)) != KEAPI_RET_SUCCESS)
		return ret;

	if ((fd = fopen(pathFull, "r")) == NULL)
		return KEAPI_RET_ERROR;

	if (fscanf(fd, "%d", &max_brightness) == EOF) {
		fclose(fd);
		return KEAPI_RET_RETRIEVAL_ERROR;
		;
	}

	fclose(fd);

	/* Set brightness */
	snprintf(pathFull, PATH_MAX, "%s/brightness", dispPathArr[displayNr]);

	if ((ret = checkRWAccess(pathFull)) != KEAPI_RET_SUCCESS)
		return ret;

	if ((fd = fopen(pathFull, "w")) == NULL)
		return KEAPI_RET_ERROR;

	/* Re-scale brightness 0..255 to range supported by driver */
	target_brightness = backlight * max_brightness / 255;

	if (fprintf(fd, "%d", target_brightness) == EOF) {
		fclose(fd);
		return KEAPI_RET_SETTING_ERROR;
	}

	fclose(fd);

	return KEAPI_RET_SUCCESS;
}
