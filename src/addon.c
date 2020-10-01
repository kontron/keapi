/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron Europe GmbH */

/* KEAPI ADDONe function implementation. */

#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <jansson.h>

#include "keapi_inc.h"
#include "storage_picmg.h"
#include "globals.h"
#include "eapi/EeeP.h"

#define  TMP_KEAPI_OUT "/tmp/keapi_cmd"


// TODO
// _--> linux_support.h
#define LEDCONF_PATH "/etc/keapi/led.conf"
// rename: gpio_mutex --> led_mutex;  define in globals.h init: libinit.c 

#define MAX_LEDS 10
#define MAX_LED_NAME 50
//--> globals.h"
/* Data structures and variables for led */
struct LedPortInfo {
	uint32_t ledCount;
	char     ledName[MAX_LEDS][MAX_LED_NAME];
	uint32_t ledLight[MAX_LEDS]; /*still unsupported Blinking flashing */
	uint32_t ledColour[MAX_LEDS];
    /* still unused */
	uint32_t ledMode[MAX_LEDS];
	uint32_t ledCtrl[MAX_LEDS];
};
struct LedPortInfo *ledPortArr I(= NULL);
//EXTERN struct LedPortInfo *ledPortArr I(= NULL);
int32_t ledPortArrCount = 0;
// EXTERN int32_t ledPortArrCount I(= 0);

enum FeatureStyle ledStyle; /* LED style. support onlyof 1 case:
				     * LINUX_LIKE - LED: only on/off via sysFs
				     */

				     

/* TODO				     
* copy from gpio_pins 
* make a globel API in linux_support */
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


/* function helper, which read led configuration
   from disk and fill led info cache */
static uint32_t GetLedConfig(void)
{
	json_t *root, *jGpPortArr, *jGpPort, *jGpioArr, *jGpio, *data;
	json_error_t error;
	uint32_t i;
	int32_t j, ledCount, ret;
	char bufstr[KEAPI_MAX_STR];

	if (pthread_mutex_trylock(&gpio_mutex))
		return KEAPI_RET_BUSY_COLLISION;

	if ((ret = checkRAccess(LEDCONF_PATH)) != KEAPI_RET_SUCCESS)
		return ret;

	ret = KEAPI_RET_ERROR;
	root = json_load_file(LEDCONF_PATH, JSON_DECODE_ANY, &error);

	if (!root)
		goto exit;
    
	data = json_object_get(root, "ledStyle");
	if (!json_is_string(data))
		goto exit;
	strcpy(bufstr, json_string_value(data));

	if (strcmp(bufstr, "LEDSysFs") == 0)
		ledStyle = LINUX_LIKE;
	else {
		ret = KEAPI_RET_FUNCTION_NOT_SUPPORTED;
		goto exit;
    }

	jGpPortArr = json_object_get(root, "ledPort");
	if (!json_is_array(jGpPortArr))
		goto exit;

	ledPortArr = calloc(json_array_size(jGpPortArr), sizeof(struct LedPortInfo));

	for (i = 0; i < json_array_size(jGpPortArr); i++) {
		jGpPort = json_array_get(jGpPortArr, i);
		if (!json_is_object(jGpPort))
			goto exit;

		jGpioArr = json_object_get(jGpPort, "led");
		if (!json_is_array(jGpioArr))
			goto exit;

		ledCount = json_array_size(jGpioArr);
		if (ledCount <= 0 || ledCount > MAX_LEDS)
			goto exit;
		ledPortArr[i].ledCount = ledCount;

		data = json_object_get(jGpPort, "label");

		for (j = 0; j < ledCount; j++) {
			int32_t ledIdx = 0, k;
			char substr[MAX_LED_NAME];

			jGpio = json_array_get(jGpioArr, j);
			if (!json_is_object(jGpio))
				goto exit;

			data = json_object_get(jGpio, "ledIdx");
			if (!json_is_integer(data)){
                printf ("error json_object_get: ledIdx\n");
				goto exit;
            }
            json_array_size(jGpPortArr);

			ledIdx = json_integer_value(data);
			if (ledIdx != j) {
                printf ("error check ledIdx %d != %d \n", ledIdx, j);               
				goto exit;
            }

			/* ledName == name in SYS-FS			 */
			data = json_object_get(jGpio, "ledName");
			if (json_is_string(data))
				strcpy(substr, json_string_value(data));
			else {
                printf ("error json_object_get: ledName\n");
				goto exit;
            }
			strcpy(ledPortArr[i].ledName[j] ,substr);
            
            /* ledLight O: LED_LIGHT_PERMANENT 
             * for future use 1:LED_LIGHT_BLINKING 2:LED_LIGHT_FLASHING */
			data = json_object_get(jGpio, "ledMode");
			if (!json_is_integer(data))          {
                printf ("error json_object_get: ledMode\n");
                goto exit;
            }

			k = json_integer_value(data);
            if (k < 0) {
                printf ("error ledMode (%d) < 0\n", k);                
				goto exit;
            }
			ledPortArr[i].ledLight[j] = k;            

            /* ledColour 1: RED 4:GREEN
             * for future use 2:YELLOW 8:BLUE 0x10:WHITE 0x300:AMPER  
             * 0x10000000:RGB */
			data = json_object_get(jGpio, "ledColour");
			if (!json_is_integer(data))
				goto exit;

			k = json_integer_value(data);
            if (k < 0)
				goto exit;
			ledPortArr[i].ledColour[j] = k;      

		}
		ledPortArrCount++;
	}

	ret = KEAPI_RET_SUCCESS;

exit:
	json_decref(root);
	if (pthread_mutex_unlock(&gpio_mutex) || ret != KEAPI_RET_SUCCESS) {
		ledPortArrCount = 0;
		unexportTrig = TRUE;
		if (gpPortArr) {
			free(gpPortArr);
			gpPortArr = NULL;
		}
		return ret;
	}
	return KEAPI_RET_SUCCESS;
}


/*******************************************************************************/
KEAPI_RETVAL KEApiLedGetCount(int32_t *pLedCount)
{
	int32_t i, ret;
    char l_cmd[100];

	/* Check function parameters */
	if (pLedCount == NULL)
		return KEAPI_RET_PARAM_NULL;       

	/* Initialization */
	*pLedCount = 0;
	if ((ret = GetLedConfig()) != KEAPI_RET_SUCCESS) {
			return ret;
	}
	
	if (ledStyle == LINUX_LIKE){
        *pLedCount = ledPortArr[0].ledCount;
        return KEAPI_RET_SUCCESS;
    }
        
    return KEAPI_RET_FUNCTION_NOT_SUPPORTED;
}



/*******************************************************************************/
KEAPI_RETVAL KEApiLedGetStatus(int32_t ledNb, int32_t *pStatus)
{
	int32_t i, ret;
    char l_cmd[100], *data;

	/* Check function parameters */
	if (pStatus == NULL)
		return KEAPI_RET_PARAM_NULL;

	/* Initialization */
	*pStatus = 0;
	if ((ret = GetLedConfig()) != KEAPI_RET_SUCCESS) {
			return ret;
	}
	
	if (ledStyle == LINUX_LIKE){

        sprintf (l_cmd,"/sys/class/leds/%s/brightness", ledPortArr[0].ledName[ledNb]);

        if ((ret = ReadFile(l_cmd, &data)) != KEAPI_RET_SUCCESS) {
			return ret;
		}
		
		*pStatus = atoi(data);
        if (*pStatus != 0) *pStatus=1;
        return KEAPI_RET_SUCCESS;
    }
    
	return KEAPI_RET_FUNCTION_NOT_SUPPORTED;
}

/*******************************************************************************/
KEAPI_RETVAL KEApiLedSetStatus(int32_t ledNb, int32_t status)
{
	int32_t i, ret;
    char l_cmd[100];
    

	/* Initialization */
	if ((ret = GetLedConfig()) != KEAPI_RET_SUCCESS) {
			return ret;
	}
	
	if (ledStyle == LINUX_LIKE){
        sprintf (l_cmd,"/sys/class/leds/%s/brightness", ledPortArr[0].ledName[ledNb]);

        if (status == 0)
            ret = WriteFile(l_cmd, "0");
        else
            ret = WriteFile(l_cmd, "1");
        if (ret != KEAPI_RET_SUCCESS) {
			return ret;
		}
        return KEAPI_RET_SUCCESS;
    }
    
	return KEAPI_RET_FUNCTION_NOT_SUPPORTED;
}




/*******************************************************************************/
KEAPI_RETVAL KEApiLedGetConfig (int32_t ledNb, PKEAPI_LED_CONFIG pConfig)
{
	int32_t  ret;

	/* Check function parameters */
	if (pConfig == NULL)
		return KEAPI_RET_PARAM_NULL;

	/* Initialization */
	if ((ret = GetLedConfig()) != KEAPI_RET_SUCCESS) {
			return ret;
	}
	
	if (ledStyle == LINUX_LIKE){
        /* use the parameters from the configuration file */
        pConfig->Colour = ledPortArr[0].ledColour[ledNb];
        pConfig->Light = ledPortArr[0].ledLight[ledNb];
        pConfig->Mode = ledPortArr[0].ledMode[ledNb];
        pConfig->TOn = 0; /* dummy setting of still unused parameter*/
        pConfig->TOff = 0;      
        return KEAPI_RET_SUCCESS;
    }
    return KEAPI_RET_LIBRARY_ERROR;
}


/*******************************************************************************/
KEAPI_RETVAL KEApiLedSetConfig (int32_t ledNb, KEAPI_LED_CONFIG pConfig)
{
	int32_t ret;
    char *data;
    

	/* Initialization */
	if ((ret = GetLedConfig()) != KEAPI_RET_SUCCESS) {
			return ret;
	}
	
	if (ledStyle == LINUX_LIKE){
        /* master configuration in the config-file; */
        /* not possible to reconf the settings */
        /* Check the parameter */
        if ( (pConfig.Colour != ledPortArr[0].ledColour[ledNb] ) ||
            (pConfig.Light != ledPortArr[0].ledLight[ledNb] ) ||
            (pConfig.Mode != ledPortArr[0].ledMode[ledNb] ) )
            return KEAPI_RET_PARAM_ERROR;
        return KEAPI_RET_SUCCESS;
    }
    return KEAPI_RET_LIBRARY_ERROR;
}

