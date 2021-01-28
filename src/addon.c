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

/* current LED_CFG 
 * used for the colour of bicolour leds */
#define LED_CFG_FILE_FMT "/tmp/ledCfg%d"

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
	uint32_t ledColour[MAX_LEDS];
	char     led2Name[MAX_LEDS][MAX_LED_NAME];
	uint32_t led2Colour[MAX_LEDS];
	uint32_t ledLight[MAX_LEDS]; /*still unsupported Blinking flashing */
    /* still unused */
	uint32_t ledMode[MAX_LEDS];
	uint32_t ledCtrl[MAX_LEDS];
};
struct LedPortInfo *ledPortArr I(= NULL);
//EXTERN struct LedPortInfo *ledPortArr I(= NULL);
int32_t ledPortArrCount = 0;
// EXTERN int32_t ledPortArrCount I(= 0);

#define KEAPI_LED_COLOR_RED		0x00000001
// #define KEAPI_LED_COLOR_YELLOW	0x00000002
#define KEAPI_LED_COLOR_GREEN	0x00000004
// #define KEAPI_LED_COLOR_BLUE		0x00000008
// #define KEAPI_LED_COLOR_WHITE	0x00000010
// #define KEAPI_LED_COLOR_ORANGE	0x00000300
#define KEAPI_LED_COLOR_AMBER	0x00000500
//#define KEAPI_LED_COLOR_RGB   	0x10000000


enum FeatureStyle ledStyle; /* LED style. support only 1 case:
				     * LINUX_LIKE - LED: only on/off via sysFs
				     */

				     

uint32_t readCurColour (uint32_t ledNb)				     
{
    char ledFile[20];
    char *data;
    uint32_t colour;
    
    sprintf (ledFile, LED_CFG_FILE_FMT, ledNb);
        
    ReadFile(ledFile, &data); /* default no colour set */
    sscanf (data, "0x%X", &colour);

    return ((uint32_t)colour);
}       

void writeCurColour (uint32_t ledNb, uint32_t colour)				     
{
    char ledFile[20];
    char data [20];
    
    sprintf (ledFile, LED_CFG_FILE_FMT, ledNb);
        
    sprintf (data, "0x%X\n", colour);
    WriteFile(ledFile, data); 
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
            
            /* optional parameter led2Name + led2Colour for a bicolour LED */
			data = json_object_get(jGpio, "led2Name");
			if (json_is_string(data))
				strcpy(substr, json_string_value(data));
			else {
                strcpy(substr, "");
            }
			strcpy(ledPortArr[i].led2Name[j] ,substr);
            
            data = json_object_get(jGpio, "led2Colour");
			if (!json_is_integer(data))
            	k=0; /* flag no colour */
            else
            {
                k = json_integer_value(data);
                if (k < 0)
				 goto exit;
            }
			ledPortArr[i].led2Colour[j] = k;
            
            
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


		}
		ledPortArrCount++;
	}
	
	for (j=0; j<ledCount; j++)
    { 
        char ledFile[20];
        sprintf (ledFile, LED_CFG_FILE_FMT, j);
        
        if ((ret = checkRWAccess(ledFile)) == KEAPI_RET_RETRIEVAL_ERROR) {
            /* create file */
            FILE *fp;
            uint32_t colour;
            fp = fopen(ledFile, "w+");
            fclose (fp);

            colour = ledPortArr[0].ledColour[j] | ledPortArr[0].led2Colour[j];
            if (colour == (KEAPI_LED_COLOR_RED | KEAPI_LED_COLOR_GREEN))
                colour = KEAPI_LED_COLOR_AMBER;
            writeCurColour (j, colour);
        }
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
	int32_t  ret;

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
	int32_t i=0, ret; 
    uint32_t colour;
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
        colour = readCurColour (ledNb);
        switch (colour)
        {
            case KEAPI_LED_COLOR_RED:
            case KEAPI_LED_COLOR_GREEN:
                if (ledPortArr[0].ledColour[ledNb] == colour)
                    i=0;
                else 
                    i=1;
                break;
            case KEAPI_LED_COLOR_AMBER:
                i=0;
                break;
            default: 
                return KEAPI_RET_PARAM_NULL;
        }


        if (i==0)
            sprintf (l_cmd,"/sys/class/leds/%s/brightness", ledPortArr[0].ledName[ledNb]);
        else
            sprintf (l_cmd,"/sys/class/leds/%s/brightness", ledPortArr[0].led2Name[ledNb]);

        if ((ret = ReadFile(l_cmd, &data)) != KEAPI_RET_SUCCESS) {
			return ret;
		}

		*pStatus = atoi(data);
        if (*pStatus != 0) *pStatus=1;
        return KEAPI_RET_SUCCESS;
    }
    
	return KEAPI_RET_FUNCTION_NOT_SUPPORTED;
}

void switchLed (char *ledName, int32_t status)
{
    char l_cmd[40];
    int ret;
    
    sprintf (l_cmd,"/sys/class/leds/%s/brightness", ledName);
    if (status == 0)
        ret = WriteFile(l_cmd, "0");
    else
        ret = WriteFile(l_cmd, "1");
}

/*******************************************************************************/
KEAPI_RETVAL KEApiLedSetStatus(int32_t ledNb, int32_t status)
{
	int32_t ret;
    uint32_t curColour;

	/* Initialization */
	if ((ret = GetLedConfig()) != KEAPI_RET_SUCCESS) {
			return ret;
	}
	
	if (ledStyle == LINUX_LIKE)   {       
        curColour = readCurColour ((uint32_t) ledNb);				     
        
        if ((curColour == KEAPI_LED_COLOR_AMBER) || (curColour == ledPortArr[0].ledColour[ledNb]))   {
            switchLed (ledPortArr[0].ledName[ledNb], status);
        }
        
        if ((curColour == KEAPI_LED_COLOR_AMBER) || (curColour == ledPortArr[0].led2Colour[ledNb]))  {
            switchLed (ledPortArr[0].led2Name[ledNb], status);
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
        pConfig->Colour = readCurColour (ledNb);        
        pConfig->Light = ledPortArr[0].ledLight[ledNb];
        pConfig->Mode = ledPortArr[0].ledMode[ledNb];
        pConfig->TOn = 0; /* dummy setting of still unused parameter*/
        pConfig->TOff = 0;      
        return KEAPI_RET_SUCCESS;
    }
    return KEAPI_RET_LIBRARY_ERROR;
}

/*******************************************************************************/
KEAPI_RETVAL KEApiLedGetCaps (int32_t ledNb, PKEAPI_LED_CONFIG pConfig)
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
        pConfig->Colour = ledPortArr[0].ledColour[ledNb] | ledPortArr[0].led2Colour[ledNb];
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

	/* Initialization */
	if ((ret = GetLedConfig()) != KEAPI_RET_SUCCESS) {
			return ret;
	}
	
	if (ledStyle == LINUX_LIKE){
        /* configurations: Light + mode unsupported*/
        /* setting ignored */

        /* it is possible to reconf the colour of a bicolour led */
        switch (pConfig.Colour)
        {
            case KEAPI_LED_COLOR_RED :
                /* check: colour in cfg ? */
//                if ((ledPortArr[0].ledColour[ledNb] | ledPortArr[0].led2Colour[ledNb]) != pConfig.Colour )
                if (((ledPortArr[0].ledColour[ledNb] | ledPortArr[0].led2Colour[ledNb]) & pConfig.Colour) == 0)
                    return KEAPI_RET_PARAM_ERROR;
                /* check if bicolor led; switch off green */
                if (ledPortArr[0].ledColour[ledNb] == KEAPI_LED_COLOR_GREEN)
                    switchLed (ledPortArr[0].ledName[ledNb], 0);
                if (ledPortArr[0].led2Colour[ledNb] == KEAPI_LED_COLOR_GREEN)
                    switchLed (ledPortArr[0].led2Name[ledNb], 0);
                break;

            case KEAPI_LED_COLOR_GREEN :
                /* check: colour in cfg ? */
//                 if ((ledPortArr[0].ledColour[ledNb] | ledPortArr[0].led2Colour[ledNb]) != pConfig.Colour )
                if (((ledPortArr[0].ledColour[ledNb] | ledPortArr[0].led2Colour[ledNb]) & pConfig.Colour) == 0)
                    return KEAPI_RET_PARAM_ERROR;
                /* check if bicolor led; switch off red */
                if (ledPortArr[0].ledColour[ledNb] == KEAPI_LED_COLOR_RED)
                    switchLed (ledPortArr[0].ledName[ledNb], 0);
                if (ledPortArr[0].led2Colour[ledNb] == KEAPI_LED_COLOR_RED)
                    switchLed (ledPortArr[0].led2Name[ledNb], 0);
                break;

            case KEAPI_LED_COLOR_AMBER :
                /* check: cfg a bicolour led */
                if ((ledPortArr[0].ledColour[ledNb] | ledPortArr[0].led2Colour[ledNb]) != (KEAPI_LED_COLOR_RED | KEAPI_LED_COLOR_GREEN) )
                    return KEAPI_RET_PARAM_ERROR;
                break;
                
            default :
                return KEAPI_RET_PARAM_ERROR;
        }
        
        /* save current colour-setting */
        writeCurColour (ledNb, pConfig.Colour);

        return KEAPI_RET_SUCCESS;
    }
    return KEAPI_RET_LIBRARY_ERROR;
}

#define RESETCONF_PATH "/etc/keapi/reset.conf"
#define MAX_ERR0RS 10

struct errorListStruct {
    int nbErrors;
    int cpldError[MAX_ERR0RS];
    int keapiError[MAX_ERR0RS];
} errorList;


static uint32_t GetResetConfig(void)
{
	json_t *root, *data, *jErrArr, *jErr;
	json_error_t error;
	int32_t i, ret, nbErrors, cpldValue, keapiValue;

	if (pthread_mutex_trylock(&gpio_mutex))
		return KEAPI_RET_BUSY_COLLISION;

	if ((ret = checkRAccess(RESETCONF_PATH)) != KEAPI_RET_SUCCESS)
		return ret;

	ret = KEAPI_RET_ERROR;
	root = json_load_file(RESETCONF_PATH, JSON_DECODE_ANY, &error);
	if (!root)
		goto exit;
    
	jErrArr = json_object_get(root, "error");
    if (!json_is_array(jErrArr))
			goto exit;
    
    nbErrors = json_array_size(jErrArr);
    if ( (nbErrors <= 0)|| (nbErrors > MAX_ERR0RS) )  {
        printf ("ERROR nbErrors:%d out of limits (should be < %d)\n", nbErrors, MAX_ERR0RS);
    }
    errorList.nbErrors = nbErrors;
	
	for (i = 0; i < nbErrors; i++) {
			jErr = json_array_get(jErrArr, i);
			if (!json_is_object(jErr))
				goto exit;
            
			data = json_object_get(jErr, "CPLD");
            if (!json_is_string(data))
                goto exit;
			cpldValue = strtol (json_string_value(data), NULL, 16);
            errorList.cpldError[i] = cpldValue;

			data = json_object_get(jErr, "KEAPI");
            if (!json_is_string(data))
                goto exit;
			keapiValue = strtol (json_string_value(data), NULL, 16);
            errorList.keapiError[i] = keapiValue;
    }
    ret = KEAPI_RET_SUCCESS;

exit:
	json_decref(root);
	if (pthread_mutex_unlock(&gpio_mutex) || ret != KEAPI_RET_SUCCESS) {
		return ret;
	}
	return KEAPI_RET_SUCCESS;
}



/*******************************************************************************/
#define SYS_FS_RSTAT  "/sys/devices/platform/komdrv.0/RSTAT"

KEAPI_RETVAL KEApiGetResetSource(int32_t *pResetSource)
{
	int32_t ret, cpldError, i;
    char *data;   
 
    if ((ret = GetResetConfig()) != KEAPI_RET_SUCCESS) {
			return ret;
	}
    
	if ((ret = ReadFile(SYS_FS_RSTAT, &data)) != KEAPI_RET_SUCCESS) {
		return ret;
	}

    cpldError = strtol(data, NULL, 0);
    
    *pResetSource = 0;
    for (i=0; i<errorList.nbErrors; i++)  {
        if (cpldError & errorList.cpldError[i])
            *pResetSource |= errorList.keapiError[i];
    }
    
    return KEAPI_RET_SUCCESS;    
}

/*******************************************************************************/
KEAPI_RETVAL KEApiClearResetSource()
{
    WriteFile(SYS_FS_RSTAT, "0xff");
    return KEAPI_RET_SUCCESS;
} 
