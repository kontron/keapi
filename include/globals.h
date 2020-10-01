/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron Europe GmbH */

/* Global variables. Cached data should be described here. */

#include "keapi_inc.h"
#include "pthread.h"

#ifdef DEFINE_GLOBAL_VARIABLES
#define I(x) x
#define EXTERN
#else
#define I(x)
#define EXTERN extern
#endif

/* Data structures and variables for EEPROMs */
struct StorageInfo {
	char path[KEAPI_MAX_STR];
	int8_t usr_flag;
	int8_t picmg_flag;
	struct EepUsr *eepUsr;
};

EXTERN struct StorageInfo *siArr I(= NULL);
EXTERN int siArrCount I(= 0);
EXTERN pthread_mutex_t eep_mutex;

/* Data structures and variables for Serial buses (i2c, smbus) */
struct SerialBusInfo {
	char name[KEAPI_MAX_STR];
	int32_t unit;
	int32_t realNr;
};

EXTERN struct SerialBusInfo *i2cArr I(= NULL);
EXTERN int i2cArrCount I(= 0);

EXTERN struct SerialBusInfo *smbusArr I(= NULL);
EXTERN int smbusArrCount I(= 0);

EXTERN pthread_mutex_t i2c_mutex;
EXTERN pthread_mutex_t smbus_mutex;

/* Data structures and variables for gpio */
struct GpioPortInfo {
	uint32_t isIn;
	uint32_t isOut;
	uint32_t gpioCount;
	/* gpioOffs
	 * if gpioStye=KEAPI_FULL - stores gpios offsets in the particular gpio device,
	 * if gpioStye=LINUX_LIKE - stores in gpioOffs[0] the gpio pin number in SysFs,
	 * if gpioStye=KONTRON_LIKE_KEM - stores gpios pin numbers in SysFs.
	 * if gpioStye=EN01_152 - use internal commends to to handle this special kontron IO-Module.
	 */
	uint32_t gpioOffs[GPIO_PER_PORT];
	char label[KEAPI_MAX_STR];
	char devname[KEAPI_MAX_STR];
};
EXTERN struct GpioPortInfo *gpPortArr I(= NULL);
EXTERN int32_t gpPortArrCount I(= 0);
EXTERN pthread_mutex_t gpio_mutex;

/* backward compatibility */
EXTERN int8_t unexportTrig I(= TRUE); /* lets us to know should we do unexport gpio or not */
EXTERN enum FeatureStyle gpioStyle I(= LINUX_LIKE); /* means 1 gpio per one port */

/* Mutex for atomic access to watchdog temporary file. */
EXTERN pthread_mutex_t wdt_mutex;

/* Display/Backlight paths name array and counter */
EXTERN char **dispPathArr I(= NULL);
EXTERN int dispPathArrCount I(= 0);
