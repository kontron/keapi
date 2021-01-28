
/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron Europe GmbH */

/* GPIO control functions. */

#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <jansson.h>
#include <errno.h>
#include <termios.h>

#include "keapi_inc.h"
#include "globals.h"

#include <config.h>
#ifdef HAVE_LINUX_GPIO_H
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/gpio.h>
#define _USE_KEAPI_FULL
#endif

/* Settings for the EN01-152 Kontron-USB-GPIO module */
#define SER_DEVICE "/dev/ttyACM0"
#define SER_BAUD   57600
#define GPIO_DIRECTION_FILE "/tmp/gpioDir"
#define EN01_152_NB_IO 8  /* Number IO of EN01-152 */

// Message start [1-3]:"<0F" 
//    [4]messageType [5-6]commandCode [7-8] Length 
//  End ">" <CR> >LF>
#define CmdMessage   "<0F%01d%02d%02d>"

#define CmdFwVers    "<0F10000>\r\n"
#define CmdBoardInfo "<0F10100>\r\n"
#define CmdReadInput "<0F11000>\r\n"
#define CmdGetOutput "<0F12000>\r\n"

// CmdSetOutput "<0F12004mmoo>"
#define CmdSetOutput "<0F12004%02lx%02lx>\r\n"

int setupSerial(int speed)
{
    int fd;
    struct termios tty;
     
    fd = open(SER_DEVICE, O_RDWR | O_NOCTTY);	/* O_RDWR   - Read/Write access to serial port       */
                                                /* O_NOCTTY - No terminal will control the process   */
                                                /* Open in blocking mode,read will wait              */
   	if(fd == -1)	{
        printf(" open Error! %s\n", SER_DEVICE);
        return -1;
    }

    if (tcgetattr (fd, &tty) != 0)
    {
           printf ("error %d from tcgetattr", errno);
           return -1;
    }

    cfsetospeed (&tty, speed);
    cfsetispeed (&tty, speed);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
    // disable IGNBRK for mismatched speed tests; otherwise receive break
     // as \000 chars
    tty.c_iflag &= ~IGNBRK;         // disable break processing
    tty.c_lflag = 0;                // no signaling chars, no echo,
                                    // no canonical processing
    tty.c_oflag = 0;                // no remapping, no delays
    tty.c_cc[VMIN]  = 0;            // read doesn't block
    tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

    tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
                                    // enable reading
    tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    if (tcsetattr (fd, TCSANOW, &tty) != 0)
    {
        printf ("error %d from tcsetattr\n", errno);
        return -1;
    }
     return fd;
}  

int gpiotoint (char *buffer)
{
    char output[8];
    int i;   
    
    output[0] = 0x30;   /* '0' */ output[1] = 'X';
    output[2] = buffer[0]; output[3] = buffer[1]; output[4] = '\0'; 
    i = (int)(strtol)(output, NULL, 0);    
    return i;
}

void debugBuffer (int myDebug, char *string, char *buffer, int bufferLength)
{
  if (myDebug == 0)
      return; 
  // cut last 2 char if == \n or \r
  if (bufferLength >2)
    if ( (buffer[bufferLength -2] == '\n') || (buffer[bufferLength -2] == '\r'))
        buffer[bufferLength -2] ='\0';
  printf("%s NbBytes:%d buffer:%s\n", string, bufferLength, buffer); 
} 



int gpioGetInputs (uint32_t *inputVal)
{
    char buffer[32];   /* Buffer to store the data received */
    int bytesRead, fd;

    if ((fd = setupSerial(SER_BAUD)) == -1)
        return -1;
    
    if (write (fd, CmdReadInput, strlen(CmdReadInput)) == -1)
        return -1;
        
    bytesRead = read (fd, &buffer, 32); /* Read the response  */
#ifdef PRINTK_DEBUG
    debugBuffer (1, "gpioGetInputs",  buffer, bytesRead);
#endif			
    *inputVal = gpiotoint (&buffer[8]);
    close (fd);
    return 0;        
}

int gpioGetOutputs (uint32_t *outValue)
{
    char buffer[32];   /* Buffer to store the data received */
    int bytesRead, fd;

    if ((fd = setupSerial(SER_BAUD)) == -1)
        return -1;
    
    if (write (fd, CmdGetOutput, strlen(CmdGetOutput)) == -1)
        return -1;
        
    bytesRead = read (fd, &buffer, 32); /* Read the data                   */
#ifdef PRINTK_DEBUG
    debugBuffer (1, "gpioGetOutputs",  buffer, bytesRead);
#endif    
    *outValue = (uint32_t)gpiotoint (&buffer[8]);
   
    close (fd);
    return 0;           
}

int gpioSetOutput (long mask, long value) 
{
    char buffer[32];   /* Buffer to store the data received */ 
    int bytesRead, fd;
    
    if ((fd = setupSerial(SER_BAUD)) == -1)
        return -1;
    
    sprintf (buffer, CmdSetOutput, mask, value);   
    if (write (fd, buffer, strlen(buffer)) == -1)
        return -1;
    
    bytesRead = read (fd, &buffer, 32); /* Read response  */
		
    close (fd);
    return 0;               
}



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
    }else if (strcmp(bufstr, "EN01_152") == 0) {
		gpioStyle = EN01_152;
        gpPortArrCount = 1; 
	} else if (strcmp(bufstr, "keapi-full") == 0) {
#ifdef _USE_KEAPI_FULL
		gpioStyle = KEAPI_FULL;
#else
		ret = KEAPI_RET_FUNCTION_NOT_SUPPORTED;
		goto exit;
#endif
	} else
		goto exit;
     
    if (gpioStyle == EN01_152) {
        if ((ret = checkRWAccess(GPIO_DIRECTION_FILE)) == KEAPI_RET_RETRIEVAL_ERROR) {
            /* create file */
            FILE *fp;
            char data[20];
            uint32_t outputs;
            fp = fopen(GPIO_DIRECTION_FILE, "w+");
            fclose (fp);
            
            /* default setting;  an output is: if the output-value == 1 */
            /* (an output with the value ==0 my be an input too) */
            if (gpioGetOutputs (&outputs) == 0) {
                /* !! in direction Bit ==0 output; ==1 input */
                sprintf (data, "0x%x\n", ~outputs & 0xFF);
                ret = WriteFile(GPIO_DIRECTION_FILE, data);
            }              
        }
        return KEAPI_RET_SUCCESS;
    }

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

	data = json_object_get(root, "UNEXPgpioStyleORT");
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
	
    if (gpioStyle == EN01_152) {
        *pCount = 1;
        return KEAPI_RET_SUCCESS;
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
    
    if (gpioStyle == EN01_152)    {
//        gpioGetOutputs (pOuts);
//	    *pIns = ~(*pOuts) & 0xff;
        *pOuts = 0xFF;
        *pIns = 0xFF;
    }
    else {
        *pIns = gpPortArr[portNr].isIn;
        *pOuts = gpPortArr[portNr].isOut;
    }

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

static KEAPI_RETVAL gpio_get_directions(int32_t portNr, uint32_t *pDirections)
{
	int ret, chipfd, lfd;
	uint32_t dir_bit, i;
	struct gpioline_info gli;

	ret = gpio_get_chip_dev(gpPortArr[portNr].label, &chipfd);
	if (ret != KEAPI_RET_SUCCESS)
		return ret;

	ret = gpio_get_linehandle(chipfd, &gpPortArr[portNr], &lfd);
	if (ret < 0) {
		ret = KEAPI_RET_RETRIEVAL_ERROR;
		goto exit_close;
	}
	close(lfd);

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
	int ret, chipfd, j, lfd;
	uint32_t dir_bit, set_in, set_out, old_directions = 0, i;
	struct gpioline_info gli;
	struct gpiohandle_request ghr;

	memset(&ghr, 0, sizeof(ghr));
	ghr.fd = -1;

	ret = gpio_get_chip_dev(gpPortArr[portNr].label, &chipfd);
	if (ret != KEAPI_RET_SUCCESS)
		return ret;

	ret = gpio_get_linehandle(chipfd, &gpPortArr[portNr], &lfd);
	if (ret < 0) {
		ret = KEAPI_RET_RETRIEVAL_ERROR;
		goto exit_close;
	}
	close(lfd);

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
	uint32_t dir_bit, i, j;
	int ret, chipfd;
	struct gpioline_info gli;
	struct gpiohandle_request ghr;

	ret = gpio_get_chip_dev(gpPortArr[portNr].label, &chipfd);
	if (ret != KEAPI_RET_SUCCESS)
		return ret;

	memset(&ghr, 0, sizeof(ghr));

	for (i = 0, j = 0; i < gpPortArr[portNr].gpioCount; i++) {
		gli.line_offset = gpPortArr[portNr].gpioOffs[i];

		ret = ioctl(chipfd, GPIO_GET_LINEINFO_IOCTL, &gli);
		if (ret < 0) {
			ret = KEAPI_RET_RETRIEVAL_ERROR;
			goto exit_close;
		}

		dir_bit = gli.flags & GPIOLINE_FLAG_IS_OUT ? 0 : 1;

		if (!dir_bit) {
			ghr.lineoffsets[j] = gpPortArr[portNr].gpioOffs[i];
			ghr.default_values[j++] = (Levels >> i) & 0x1;
		}
	}
	ghr.lines = j;
	ghr.flags = GPIOHANDLE_REQUEST_OUTPUT;

	ret = ioctl(chipfd, GPIO_GET_LINEHANDLE_IOCTL, &ghr);
	if (ret < 0) {
		ret = KEAPI_RET_RETRIEVAL_ERROR;
		goto exit_close;
	} else
		ret = KEAPI_RET_SUCCESS;

exit_close:
	if (ghr.fd >= 0)
		close(ghr.fd);
	close(chipfd);
	return ret;
}

#endif

#if 0
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
#endif 

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
    
    if (gpioStyle == EN01_152)
    {
//        gpioGetOutputs (pDirections);
//        *pDirections = ~(*pDirections) & 0xFF;
        char *data;
        if ((ret = ReadFile(GPIO_DIRECTION_FILE, &data)) == 0) {
           *pDirections =strtol (data, NULL, 0);
        }
		return ret;
    }
    
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
    
    if (gpioStyle == EN01_152)    {
        char data[20];
        uint32_t outputs, i;
        sprintf (data, "0x%x\n", directions);
        ret = WriteFile(GPIO_DIRECTION_FILE, data);
        /* if a bit is set to an input, this output value has to be set to "0" */
        if (gpioGetOutputs (&outputs) == 0) {
            for (i=0; i<EN01_152_NB_IO; i++) {
                if (directions & BIT(i)) { 
                    /* set to input */
                    if (outputs && BIT(i)) {
                        /* output set to 1 == configured to an output */
                        gpioSetOutput (BIT(i), 0); /*set output = 0 == input too */
                    }
                }
            }
        }                     
        return ret;
    }

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
    if (gpioStyle == EN01_152)
    {
        uint32_t inputs, outputs;
        
        /* read input + output levels */
        if ((gpioGetOutputs (&outputs) == 0) &&
            (gpioGetInputs (&inputs) == 0) )
        {
            *pLevels = outputs | inputs;
            return KEAPI_RET_SUCCESS;
        }
        else
            return KEAPI_RET_ERROR;
            
    }

	if (gpioStyle == LINUX_LIKE || gpioStyle == KONTRON_LIKE_KEM)
		return linux_gpio_get_levels(portNr, pLevels);
	else
		return KEAPI_RET_FUNCTION_NOT_SUPPORTED;
}

/******************************************************************************/
KEAPI_RETVAL KEApiL_SetGpioPortLevels(int32_t portNr, uint32_t levels)
{
	int32_t devCnt, ret, directions;

	/* Check function parameters */
	if ((ret = KEApiL_GetGpioPortCount(&devCnt)) != KEAPI_RET_SUCCESS)
		return ret;

	if (portNr < 0 || portNr >= gpPortArrCount)
		return KEAPI_RET_PARAM_ERROR;

#ifdef _USE_KEAPI_FULL
	if (gpioStyle == KEAPI_FULL)   {
		return gpio_set_levels(portNr, levels);
    }
#endif
    if (gpioStyle == EN01_152) {
        char *data;
        if ((ret = ReadFile(GPIO_DIRECTION_FILE, &data)) == 0) {
           directions = strtol (data, NULL, 0);
           /* attention direction-bit == 0 == Output */
           if ((levels & ~directions) != levels)
            return KEAPI_RET_ERROR;
        }
        return (gpioSetOutput (0xFF, levels));
    }

	if (gpioStyle == LINUX_LIKE || gpioStyle == KONTRON_LIKE_KEM)
		return linux_gpio_set_levels(portNr, levels);
	else
		return KEAPI_RET_FUNCTION_NOT_SUPPORTED;
}
