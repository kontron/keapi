/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron AG */

/* KEAPI Linux global variables. */

#define DEFINE_GLOBAL_VARIABLES
#include <stdio.h>
#include "globals.h"

void __attribute__((destructor)) KEApiL_finalize(void)
{
	int32_t i;

	if (siArr) {
		for (i = 0; i < siArrCount; i++)
			if (siArr[i].eepUsr)
				free(siArr[i].eepUsr);

		free(siArr);
	}

	if (i2cArr)
		free(i2cArr);

	if (smbusArr)
		free(smbusArr);

	if (gpPortArr)
		free(gpPortArr);

	if (dispPathArr) {
		for (i = 0; i < dispPathArrCount; i++)
			free(dispPathArr[i]);

		free(dispPathArr);
	}
}
