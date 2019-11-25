/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron AG */

/* GPIO control functions. */

#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <jansson.h>

#include "keapi_inc.h"
#include "globals.h"

#include <config.h>
#ifdef HAVE_LINUX_GPIO_H
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/gpio.h>
#define _USE_KEAPI_FULL
#endif

#define BIT(nr) (1 << (nr))
/******************************************************************************/

/* function helper, which read gpio configuration
   from disk and fill gpio info cache */
static uint32_t GetGpioConfig(void)
{
	json_t *root, *jGpPortArr, *jGpPort, *jGpioArr, *jGpio, *data;
	json_error_t error;
	uint32_t i;
	int32_t j, gpioCount, ret;
	char bufstr[KEAPI_MAX_STR];

	if (pthread_mutex_trylock(&gpio_mutex))
		return KEAPI_RET_BUSY_COLLISION;

	if ((ret = checkRAccess(GPIOCONF_PATH)) != KEAPI_RET_SUCCESS)
		return ret;

	ret = KEAPI_RET_ERROR;
	root = json_load_file(GPIOCONF_PATH, JSON_DECODE_ANY, &error);
	if (!root)
		goto exit;

	data = json_object_get(root, "gpioStyle");
	if (!json_is_string(data))
		goto exit;

	strcpy(bufstr, json_string_value(data));

	if (strcmp(bufstr, "linux-like") == 0)
		gpioStyle = LINUX_LIKE;
	else if (strcmp(bufstr, "kontron-like-kem") == 0) {
		gpioStyle = KONTRON_LIKE_KEM;
	} else if (strcmp(bufstr, "keapi-full") == 0) {
#ifdef _USE_KEAPI_FULL
		gpioStyle = KEAPI_FULL;
#else
		ret = KEAPI_RET_FUNCTION_NOT_SUPPORTED;
		goto exit;
#endif
	} else
		goto exit;

	jGpPortArr = json_object_get(root, "gpioPort");
	if (!json_is_array(jGpPortArr))
		goto exit;

	gpPortArr = calloc(json_array_size(jGpPortArr), sizeof(struct GpioPortInfo));

	for (i = 0; i < json_array_size(jGpPortArr); i++) {
		jGpPort = json_array_get(jGpPortArr, i);
		if (!json_is_object(jGpPort))
			goto exit;

		jGpioArr = json_object_get(jGpPort, "gpio");
		if (!json_is_array(jGpioArr))
			goto exit;

		gpioCount = json_array_size(jGpioArr);
		if (gpioCount <= 0 || gpioCount > GPIO_PER_PORT)
			goto exit;
		gpPortArr[i].gpioCount = gpioCount;

		data = json_object_get(jGpPort, "label");
		if (json_is_string(data))
			strncpy(gpPortArr[i].label, json_string_value(data), KEAPI_MAX_STR);
		else if (gpioStyle == KEAPI_FULL)
			goto exit;

		for (j = 0; j < gpioCount; j++) {
			int32_t gpIdx = 0;
			char substr[KEAPI_MAX_STR];

			jGpio = json_array_get(jGpioArr, j);
			if (!json_is_object(jGpio))
				goto exit;

			data = json_object_get(jGpio, "gpioIdx");
			if (!json_is_integer(data))
				goto exit;

			gpIdx = json_integer_value(data);
			if (gpIdx < 0)
				goto exit;
			gpPortArr[i].gpioOffs[j] = gpIdx;

			/* If "directionCaps" is not mentioned in configuration file,
			   make an assumption that both directions are possible for the pin
			 */
			data = json_object_get(jGpio, "directionCaps");
			if (json_is_string(data))
				strcpy(substr, json_string_value(data));
			else
				substr[0] = 'A';

			switch (substr[0]) {
			case 'I':
				gpPortArr[i].isIn |= (1 << j);
				break;
			case 'O':
				gpPortArr[i].isOut |= (1 << j);
				break;
			case 'A':
			default:
				gpPortArr[i].isIn |= (1 << j);
				gpPortArr[i].isOut |= (1 << j);
				break;
			}

			if (gpioStyle == LINUX_LIKE) {
				data = json_object_get(jGpio, "gpioNum");
				if (!json_is_integer(data))
					goto exit;
				gpPortArr[i].gpioOffs[0] = json_integer_value(data);
				break;
			}

			if (gpioStyle == KONTRON_LIKE_KEM) {
				data = json_object_get(jGpio, "gpioNum");
				if (!json_is_integer(data))
					goto exit;
				gpPortArr[i].gpioOffs[j] = json_integer_value(data);
			}
		}
		gpPortArrCount++;
	}

	ret = KEAPI_RET_SUCCESS;

	data = json_object_get(root, "UNEXPORT");
	if (!json_is_string(data))
		goto exit;

	if (strcasecmp(json_string_value(data), "DISABLED") == 0)
		unexportTrig = FALSE;

exit:
	json_decref(root);

	if (pthread_mutex_unlock(&gpio_mutex) || ret != KEAPI_RET_SUCCESS) {
		gpPortArrCount = 0;
		unexportTrig = TRUE;
		if (gpPortArr) {
			free(gpPortArr);
			gpPortArr = NULL;
		}
		return ret;
	}

	return KEAPI_RET_SUCCESS;
}

#if 0
/* function helper, returns 1 if gpio with num is used and 0 otherwise */
int8_t gpioIsUsed(struct GpioPortInfo *port, int32_t num) {
	if ((port->isIn && BIT(num)) || (port->isOut && BIT(num)))
		return TRUE;
	else
		return FALSE;
}

/* function helper, current direction of gpio pin with num */
uint8_t gpioDir(uint32_t port_directions, int32_t gpio_num) {
	if (port_directions & BIT(gpio_num))
		return KEAPI_GPIO_DIRECTION_IN;
	else
		return KEAPI_GPIO_DIRECTION_OUT;
}
#endif

/******************************************************************************/
KEAPI_RETVAL KEApiL_GetGpioPortCount(int32_t *pCount)
{
	int ret;
	/* Check function parameters */
	if (pCount == NULL)
		return KEAPI_RET_PARAM_NULL;

	*pCount = 0;
	if (gpPortArr == NULL || gpPortArrCount == 0) {
		if ((ret = GetGpioConfig()) != KEAPI_RET_SUCCESS) {
			/* KEAPI_RET_RETRIEVAL_ERROR - is returned in case if there in no file
			 * It's ok and means thar there is no gpio on the platfrorm */
			if (ret == KEAPI_RET_RETRIEVAL_ERROR)
				return KEAPI_RET_SUCCESS;

			return ret;
		}
	}

#ifndef _USE_KEAPI_FULL
	if ((gpioStyle != LINUX_LIKE) && (gpioStyle != KONTRON_LIKE_KEM))
		return KEAPI_RET_FUNCTION_NOT_SUPPORTED;
#endif

	*pCount = gpPortArrCount;

	return KEAPI_RET_SUCCESS;
}

/******************************************************************************/
KEAPI_RETVAL KEApiL_GetGpioPortDirectionCaps(int32_t portNr, uint32_t *pIns, uint32_t *pOuts)
{
	int32_t devCnt, ret;

	/* Check function parameters */
	if ((pIns == NULL) || (pOuts == NULL))
		return KEAPI_RET_PARAM_NULL;

	if ((ret = KEApiL_GetGpioPortCount(&devCnt)) != KEAPI_RET_SUCCESS)
		return ret;

	if (portNr < 0 || portNr >= gpPortArrCount)
		return KEAPI_RET_PARAM_ERROR;

	*pIns = gpPortArr[portNr].isIn;
	*pOuts = gpPortArr[portNr].isOut;

	return KEAPI_RET_SUCCESS;
}

#ifdef _USE_KEAPI_FULL
/* New linux interface support functions */
#define GPIO_DEV_MAX 256
/* Find /dev/gpiochipX which has 'label' label. Return fd in *retfd or error. */
static KEAPI_RETVAL gpio_get_chip_dev(char *label, int *retfd)
{
	int fd, ret, i;
	struct gpiochip_info gi;
	char path[KEAPI_MAX_STR];

	for (i = 0; i < GPIO_DEV_MAX; i++) {
		snprintf(path, KEAPI_MAX_STR, "/dev/gpiochip%d", i);
		ret = checkRWAccess(path);
		if (ret == KEAPI_RET_RETRIEVAL_ERROR)
			continue;
		else if (ret != KEAPI_RET_SUCCESS)
			return ret;

		fd = open(path, O_RDWR);
		if (fd < 0)
			continue;

		ret = ioctl(fd, GPIO_GET_CHIPINFO_IOCTL, &gi);
		if (ret < 0) {
			close(fd);
			return KEAPI_RET_RETRIEVAL_ERROR;
		}

		if (strcmp(gi.label, label) == 0) {
			*retfd = fd;
			return KEAPI_RET_SUCCESS;
		}
	}

	return KEAPI_RET_RETRIEVAL_ERROR;
}

static KEAPI_RETVAL gpio_get_directions(int32_t portNr, uint32_t *pDirections)
{
	int ret, chipfd;
	uint32_t dir_bit, i;
	struct gpioline_info gli;

	ret = gpio_get_chip_dev(gpPortArr[portNr].label, &chipfd);
	if (ret != KEAPI_RET_SUCCESS)
		return ret;

	for (i = 0; i < gpPortArr[portNr].gpioCount; i++) {
		gli.line_offset = gpPortArr[portNr].gpioOffs[i];
		ret = ioctl(chipfd, GPIO_GET_LINEINFO_IOCTL, &gli);
		if (ret < 0) {
			ret = KEAPI_RET_RETRIEVAL_ERROR;
			goto exit_close;
		}

		dir_bit = gli.flags & GPIOLINE_FLAG_IS_OUT ? 0 : 1;
		*pDirections |= (dir_bit << i);
	}

	ret = KEAPI_RET_SUCCESS;

exit_close:
	close(chipfd);
	return ret;
}

static KEAPI_RETVAL gpio_set_directions(int32_t portNr, uint32_t directions)
{
	int ret, chipfd, j;
	uint32_t dir_bit, set_in, set_out, old_directions = 0, i;
	struct gpioline_info gli;
	struct gpiohandle_request ghr;

	memset(&ghr, 0, sizeof(ghr));
	ghr.fd = -1;

	ret = gpio_get_chip_dev(gpPortArr[portNr].label, &chipfd);
	if (ret != KEAPI_RET_SUCCESS)
		return ret;

	for (i = 0; i < gpPortArr[portNr].gpioCount; i++) {
		gli.line_offset = gpPortArr[portNr].gpioOffs[i];

		ret = ioctl(chipfd, GPIO_GET_LINEINFO_IOCTL, &gli);
		if (ret < 0) {
			ret = KEAPI_RET_RETRIEVAL_ERROR;
			goto exit_close;
		}

		dir_bit = gli.flags & GPIOLINE_FLAG_IS_OUT ? 0 : 1;
		old_directions |= (dir_bit << i);
	}

	set_in = directions & ~old_directions;
	if (set_in) {
		for (i = 0, j = 0; i < gpPortArr[portNr].gpioCount; i++) {
			if (set_in & (1 << i))
				ghr.lineoffsets[j++] = gpPortArr[portNr].gpioOffs[i];
		}

		ghr.lines = j;
		ghr.flags = GPIOHANDLE_REQUEST_INPUT;

		ret = ioctl(chipfd, GPIO_GET_LINEHANDLE_IOCTL, &ghr);
		if (ret < 0) {
			ret = KEAPI_RET_RETRIEVAL_ERROR;
			goto exit_close;
		}

		close(ghr.fd);
		ghr.fd = -1;
	}

	set_out = ~directions & old_directions;
	if (set_out) {
		for (i = 0, j = 0; i < gpPortArr[portNr].gpioCount; i++) {
			if (set_out & (1 << i))
				ghr.lineoffsets[j++] = gpPortArr[portNr].gpioOffs[i];
		}
		ghr.lines = j;
		ghr.flags = GPIOHANDLE_REQUEST_OUTPUT;
		ret = ioctl(chipfd, GPIO_GET_LINEHANDLE_IOCTL, &ghr);
		if (ret < 0) {
			ret = KEAPI_RET_RETRIEVAL_ERROR;
			goto exit_close;
		}

		close(ghr.fd);
		ghr.fd = -1;
	}

	ret = KEAPI_RET_SUCCESS;

exit_close:

	if (ghr.fd >= 0)
		close(ghr.fd);

	close(chipfd);
	return ret;
}

static KEAPI_RETVAL gpio_get_linehandle(int chipfd, struct GpioPortInfo *pPort, int *retfd)
{
	int ret;
	uint32_t i;
	struct gpiohandle_request ghr;

	memset(&ghr, 0, sizeof(ghr));

	for (i = 0; i < pPort->gpioCount; i++) {
		ghr.lineoffsets[i] = pPort->gpioOffs[i];
	}
	ghr.lines = i;

	ret = ioctl(chipfd, GPIO_GET_LINEHANDLE_IOCTL, &ghr);
	if (ret < 0)
		return KEAPI_RET_RETRIEVAL_ERROR;

	*retfd = ghr.fd;

	return KEAPI_RET_SUCCESS;
}

static KEAPI_RETVAL gpio_get_levels(int32_t portNr, uint32_t *pLevels)
{
	int ret, chipfd, lfd;
	uint32_t i;
	struct gpiohandle_data ghd;

	ret = gpio_get_chip_dev(gpPortArr[portNr].label, &chipfd);
	if (ret != KEAPI_RET_SUCCESS)
		return ret;

	ret = gpio_get_linehandle(chipfd, &gpPortArr[portNr], &lfd);
	if (ret < 0) {
		ret = KEAPI_RET_RETRIEVAL_ERROR;
		goto exit_close;
	}

	ret = ioctl(lfd, GPIOHANDLE_GET_LINE_VALUES_IOCTL, &ghd);
	if (ret < 0) {
		ret = KEAPI_RET_RETRIEVAL_ERROR;
		goto exit_close2;
	}

	for (i = 0; i < gpPortArr[portNr].gpioCount; i++) {
		if (ghd.values[i])
			*pLevels |= (1 << i);
	}

	ret = KEAPI_RET_SUCCESS;

exit_close2:
	close(lfd);

exit_close:
	close(chipfd);
	return ret;
}

static KEAPI_RETVAL gpio_set_levels(int32_t portNr, uint32_t Levels)
{
	int ret, chipfd, lfd;
	uint32_t i;
	struct gpiohandle_data ghd;

	ret = gpio_get_chip_dev(gpPortArr[portNr].label, &chipfd);
	if (ret != KEAPI_RET_SUCCESS)
		return ret;

	ret = gpio_get_linehandle(chipfd, &gpPortArr[portNr], &lfd);
	if (ret < 0) {
		ret = KEAPI_RET_RETRIEVAL_ERROR;
		goto exit_close;
	}
	memset(&ghd, 0, sizeof(ghd));
	for (i = 0; i < gpPortArr[portNr].gpioCount; i++) {
		if (Levels & (1 << i))
			ghd.values[i] = 1;
	}
	ret = ioctl(lfd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &ghd);
	if (ret < 0)
		ret = KEAPI_RET_RETRIEVAL_ERROR;
	else
		ret = KEAPI_RET_SUCCESS;

	close(lfd);

exit_close:
	close(chipfd);
	return ret;
}

#endif

/* old linux-style interface support functions */

static int WriteFile(char *path, char *data)
{
	FILE *fp;
	int ret;

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

static KEAPI_RETVAL linux_gpio_export(int32_t Nr)
{
	FILE *fw;
	int ret;

	if ((ret = checkRWAccess("/sys/class/gpio/export")) != KEAPI_RET_SUCCESS)
		return ret;

	if ((fw = fopen("/sys/class/gpio/export", "w")) == NULL)
		return KEAPI_RET_ERROR;

	if (fprintf(fw, "%d", Nr) < 0)
		ret = KEAPI_RET_WRITE_ERROR;

	fclose(fw);
	return ret;
}

static KEAPI_RETVAL linux_gpio_unexport(int32_t Nr)
{
	FILE *fw;
	int ret;

	if ((ret = checkRWAccess("/sys/class/gpio/unexport")) != KEAPI_RET_SUCCESS)
		return ret;

	if ((fw = fopen("/sys/class/gpio/unexport", "w")) == NULL)
		return KEAPI_RET_ERROR;

	if (fprintf(fw, "%d", Nr) < 0)
		ret = KEAPI_RET_WRITE_ERROR;

	fclose(fw);
	return ret;
}

static KEAPI_RETVAL linux_gpio_get_directions(int32_t portNr, uint32_t *pDirections)
{
	char path[KEAPI_MAX_STR], *data;
	int32_t ret;
	uint32_t i;
	int8_t unexport;

	unexport = unexportTrig;

	if ((ret = checkRAccess("/sys/class/gpio")) != KEAPI_RET_SUCCESS) {
		if (ret == KEAPI_RET_RETRIEVAL_ERROR)
			return KEAPI_RET_FUNCTION_NOT_SUPPORTED;
		else
			return ret;
	}

	for (i = 0; i < gpPortArr[portNr].gpioCount; i++) {
		snprintf(path, KEAPI_MAX_STR, "/sys/class/gpio/gpio%d/direction", gpPortArr[portNr].gpioOffs[i]);
		if ((ret = checkRAccess(path)) == KEAPI_RET_RETRIEVAL_ERROR) {
			if ((ret = linux_gpio_export(gpPortArr[portNr].gpioOffs[i])) != KEAPI_RET_SUCCESS)
				return ret;

			unexport = TRUE;
			ret = checkRAccess(path);
		}

		if (ret != KEAPI_RET_SUCCESS)
			goto ret_unexport;

		/* Get direction */
		if ((ret = ReadFile(path, &data)) != KEAPI_RET_SUCCESS)
			goto ret_unexport;

		/* KEAPI_GPIO_DIRECTION_OUT is equal to 0. So check only for KEAPI_GPIO_DIRECTION_IN */
		if (strncmp(data, "in", 2) == 0)
			*pDirections |= KEAPI_GPIO_DIRECTION_IN << i;

#if 0
	/* Update note: decided to do not do it anymore and let return actual
	 * values, even if it contradicts with capabilities.
	 * Implicit change is worse. */

	/* Development note:
	 * if config file is used, some directions may be banned,
	 * even though chip and driver supports it
	 * (use case: GPIO chip can change direction of a pin,
	 * but it's connected via some one-way buffer)
	 * Handle this situation.
	 */
	if (*pDirections == KEAPI_GPIO_DIRECTION_IN &&
		!gpPortArr[portNr].isIn) {
		*pDirections = KEAPI_GPIO_DIRECTION_OUT;
		ret = WriteFile(path, "out");
	}
	if (*pDirections == KEAPI_GPIO_DIRECTION_OUT &&
		!gpPortArr[portNr].isOut) {
		*pDirections = KEAPI_GPIO_DIRECTION_IN;
		ret = WriteFile(path, "in");
	}
#endif

		free(data);

		/* Unexport pin */
	ret_unexport:
		if (unexport) {
			if (ret == KEAPI_RET_SUCCESS) {
				ret = linux_gpio_unexport(gpPortArr[portNr].gpioOffs[i]);
				if (ret != KEAPI_RET_SUCCESS)
					break;
			} else {
				linux_gpio_unexport(gpPortArr[portNr].gpioOffs[i]);
				break;
			}
		}
	}

	return ret;
}

static KEAPI_RETVAL linux_gpio_set_directions(int32_t portNr, uint32_t direction)
{
	char path[KEAPI_MAX_STR];
	int32_t ret;
	int8_t unexport;
	uint32_t newDirection, i;

	unexport = unexportTrig;

	if ((ret = checkRAccess("/sys/class/gpio")) != KEAPI_RET_SUCCESS) {
		if (ret == KEAPI_RET_RETRIEVAL_ERROR)
			return KEAPI_RET_FUNCTION_NOT_SUPPORTED;
		else
			return ret;
	}

	for (i = 0; i < gpPortArr[portNr].gpioCount; i++) {
		snprintf(path, KEAPI_MAX_STR, "/sys/class/gpio/gpio%d/direction", gpPortArr[portNr].gpioOffs[i]);
		if ((ret = checkRWAccess(path)) == KEAPI_RET_RETRIEVAL_ERROR) {
			if ((ret = linux_gpio_export(gpPortArr[portNr].gpioOffs[i])) != KEAPI_RET_SUCCESS)
				return ret;

			unexport = TRUE;
			ret = checkRWAccess(path);
		}

		if (ret != KEAPI_RET_SUCCESS)
			goto ret_unexport;

		/* Set direction */
		newDirection = (direction >> i) & 0x01;
		if (newDirection == KEAPI_GPIO_DIRECTION_IN)
			ret = WriteFile(path, "in");
		else
			ret = WriteFile(path, "out");

		/* Unexport pin */
	ret_unexport:
		if (unexport) {
			if (ret == KEAPI_RET_SUCCESS) {
				ret = linux_gpio_unexport(gpPortArr[portNr].gpioOffs[i]);
				if (ret != KEAPI_RET_SUCCESS)
					break;
			} else {
				linux_gpio_unexport(gpPortArr[portNr].gpioOffs[i]);
				break;
			}
		}
	}

	return ret;
}

static KEAPI_RETVAL linux_gpio_get_levels(int32_t portNr, uint32_t *pLevels)
{
	char path[KEAPI_MAX_STR], *data;
	int32_t ret;
	uint32_t i;
	int8_t unexport;

	unexport = unexportTrig;

	if ((ret = checkRAccess("/sys/class/gpio")) != KEAPI_RET_SUCCESS) {
		if (ret == KEAPI_RET_RETRIEVAL_ERROR)
			return KEAPI_RET_FUNCTION_NOT_SUPPORTED;
		else
			return ret;
	}

	for (i = 0; i < gpPortArr[portNr].gpioCount; i++) {
		snprintf(path, KEAPI_MAX_STR, "/sys/class/gpio/gpio%d/value", gpPortArr[portNr].gpioOffs[i]);
		if ((ret = checkRAccess(path)) == KEAPI_RET_RETRIEVAL_ERROR) {
			if ((ret = linux_gpio_export(gpPortArr[portNr].gpioOffs[i])) != KEAPI_RET_SUCCESS)
				return ret;

			unexport = TRUE;
			ret = checkRAccess(path);
		}

		if (ret != KEAPI_RET_SUCCESS)
			goto ret_unexport;

		snprintf(path, KEAPI_MAX_STR, "/sys/class/gpio/gpio%d/value", gpPortArr[portNr].gpioOffs[i]);

		if ((ret = ReadFile(path, &data)) != KEAPI_RET_SUCCESS)
			goto ret_unexport;

		*pLevels |= atoi(data) << i;

		free(data);

	/* Unexport pin */
	ret_unexport:
		if (unexport) {
			if (ret == KEAPI_RET_SUCCESS) {
				ret = linux_gpio_unexport(gpPortArr[portNr].gpioOffs[i]);
				if (ret != KEAPI_RET_SUCCESS)
					break;
			} else {
				linux_gpio_unexport(gpPortArr[portNr].gpioOffs[i]);
				break;
			}
		}
	}

	return ret;
}

static KEAPI_RETVAL linux_gpio_set_levels(int32_t portNr, uint32_t levels)
{
	char path[KEAPI_MAX_STR], *data;
	int32_t ret;
	uint32_t i;
	int8_t unexport;

	unexport = unexportTrig;

	if ((ret = checkRAccess("/sys/class/gpio")) != KEAPI_RET_SUCCESS) {
		if (ret == KEAPI_RET_RETRIEVAL_ERROR)
			return KEAPI_RET_FUNCTION_NOT_SUPPORTED;
		else
			return ret;
	}

	for (i = 0; i < gpPortArr[portNr].gpioCount; i++) {
		snprintf(path, KEAPI_MAX_STR, "/sys/class/gpio/gpio%d/direction", gpPortArr[portNr].gpioOffs[i]);
		if ((ret = checkRWAccess(path)) == KEAPI_RET_RETRIEVAL_ERROR) {
			if ((ret = linux_gpio_export(gpPortArr[portNr].gpioOffs[i])) != KEAPI_RET_SUCCESS)
				return ret;

			unexport = TRUE;
			ret = checkRWAccess(path);
		}

		if (ret != KEAPI_RET_SUCCESS)
			goto ret_unexport;

		/* Get direction */
		if ((ret = ReadFile(path, &data)) != KEAPI_RET_SUCCESS)
			goto ret_unexport;

		/* if the pin is in DIRECTION_IN  state, just skip it.
		 * Same behaviour in gpio-cdev/keapi-full */
		if (strncmp(data, "in", 2) == 0) {
			free(data);
			goto ret_unexport;
		}
		free(data);

		snprintf(path, KEAPI_MAX_STR, "/sys/class/gpio/gpio%d/value", gpPortArr[portNr].gpioOffs[i]);
		/* Set levels */
		if ((levels >> i) & 0x01)
			ret = WriteFile(path, "1");
		else
			ret = WriteFile(path, "0");

		/* Unexport pin */
	ret_unexport:
		if (unexport) {
			if (ret == KEAPI_RET_SUCCESS) {
				ret = linux_gpio_unexport(gpPortArr[portNr].gpioOffs[i]);
				if (ret != KEAPI_RET_SUCCESS)
					break;
			} else {
				linux_gpio_unexport(gpPortArr[portNr].gpioOffs[i]);
				break;
			}
		}
	}

	return ret;
}

/******************************************************************************/
KEAPI_RETVAL KEApiL_GetGpioPortDirections(int32_t portNr, uint32_t *pDirections)
{
	int32_t devCnt;
	int ret;

	/* Check function parameters */
	if (pDirections == NULL)
		return KEAPI_RET_PARAM_NULL;

	if ((ret = KEApiL_GetGpioPortCount(&devCnt)) != KEAPI_RET_SUCCESS)
		return ret;

	if (portNr < 0 || portNr >= gpPortArrCount)
		return KEAPI_RET_PARAM_ERROR;

	*pDirections = 0;

#ifdef _USE_KEAPI_FULL
	if (gpioStyle == KEAPI_FULL)
		return gpio_get_directions(portNr, pDirections);
#endif

	if (gpioStyle == LINUX_LIKE || gpioStyle == KONTRON_LIKE_KEM)
		return linux_gpio_get_directions(portNr, pDirections);
	else
		return KEAPI_RET_FUNCTION_NOT_SUPPORTED;
}

/******************************************************************************/
KEAPI_RETVAL KEApiL_SetGpioPortDirections(int32_t portNr, uint32_t directions)
{
	uint32_t dir_out;
	int32_t devCnt, ret;

	/* Check function parameters */
	if ((ret = KEApiL_GetGpioPortCount(&devCnt)) != KEAPI_RET_SUCCESS)
		return ret;

	if (portNr < 0 || portNr >= gpPortArrCount)
		return KEAPI_RET_PARAM_ERROR;

	/* sanity check: don't allow setting direction which is not
	 * supported by the pin */
	dir_out = ((1 << gpPortArr[portNr].gpioCount) - 1) & ~directions;
	if ((directions & gpPortArr[portNr].isIn) != directions || (dir_out & gpPortArr[portNr].isOut) != dir_out)
		return KEAPI_RET_SETTING_ERROR;

#ifdef _USE_KEAPI_FULL
	if (gpioStyle == KEAPI_FULL)
		return gpio_set_directions(portNr, directions);
#endif

	if (gpioStyle == LINUX_LIKE || gpioStyle == KONTRON_LIKE_KEM)
		return linux_gpio_set_directions(portNr, directions);
	else
		return KEAPI_RET_FUNCTION_NOT_SUPPORTED;
}

/******************************************************************************/
KEAPI_RETVAL KEApiL_GetGpioPortLevels(int32_t portNr, uint32_t *pLevels)
{
	int32_t devCnt, ret;

	/* Check function parameters */
	if (pLevels == NULL)
		return KEAPI_RET_PARAM_NULL;

	if ((ret = KEApiL_GetGpioPortCount(&devCnt)) != KEAPI_RET_SUCCESS)
		return ret;

	if (portNr < 0 || portNr >= gpPortArrCount)
		return KEAPI_RET_PARAM_ERROR;

	*pLevels = 0;

#ifdef _USE_KEAPI_FULL
	if (gpioStyle == KEAPI_FULL)
		return gpio_get_levels(portNr, pLevels);
#endif

	if (gpioStyle == LINUX_LIKE || gpioStyle == KONTRON_LIKE_KEM)
		return linux_gpio_get_levels(portNr, pLevels);
	else
		return KEAPI_RET_FUNCTION_NOT_SUPPORTED;
}

/******************************************************************************/
KEAPI_RETVAL KEApiL_SetGpioPortLevels(int32_t portNr, uint32_t levels)
{
	int32_t devCnt, ret;

	/* Check function parameters */
	if ((ret = KEApiL_GetGpioPortCount(&devCnt)) != KEAPI_RET_SUCCESS)
		return ret;

	if (portNr < 0 || portNr >= gpPortArrCount)
		return KEAPI_RET_PARAM_ERROR;

#ifdef _USE_KEAPI_FULL
	if (gpioStyle == KEAPI_FULL)
		return gpio_set_levels(portNr, levels);
#endif

	if (gpioStyle == LINUX_LIKE || gpioStyle == KONTRON_LIKE_KEM)
		return linux_gpio_set_levels(portNr, levels);
	else
		return KEAPI_RET_FUNCTION_NOT_SUPPORTED;
}
