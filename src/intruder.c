/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron Europe GmbH */

/* KEAPI intrusion alarm functions implementation. */

#include <stdio.h>
#include <unistd.h>

#include "keapi_inc.h"

/*******************************************************************************/

KEAPI_RETVAL KEApiL_GetIntruderStatus(int32_t *pIntruderStatus)
{
	FILE *fr;
	int32_t temp, ret;

	/* Check function parameters */
	if (pIntruderStatus == NULL)
		return KEAPI_RET_PARAM_NULL;

	/* Check if intruder file exists */
	if ((ret = checkRAccess(INTRUDER_PATH)) != KEAPI_RET_SUCCESS) {
		if (ret == KEAPI_RET_RETRIEVAL_ERROR)
			return KEAPI_RET_FUNCTION_NOT_SUPPORTED;
		else
			return ret;
	}

	/* Read intruder status -> 0 or 1 */
	if ((fr = fopen(INTRUDER_PATH, "r")) == NULL)
		return KEAPI_RET_ERROR;

	if (fscanf(fr, "%d", &temp) == EOF)
		return KEAPI_RET_ERROR;

	fclose(fr);

	if (temp != KEAPI_INTRUDER_STATUS_CASE_CLOSED && temp != KEAPI_INTRUDER_STATUS_CASE_OPENED)
		return KEAPI_RET_ERROR;

	*pIntruderStatus = temp;

	return KEAPI_RET_SUCCESS;
}

/*******************************************************************************/

KEAPI_RETVAL KEApiL_ResetIntruderStatus()
{
	FILE *fw;
	int32_t ret;

	/* Check if intruder file exists */
	if ((ret = checkRWAccess(INTRUDER_PATH)) != KEAPI_RET_SUCCESS) {
		if (ret == KEAPI_RET_RETRIEVAL_ERROR)
			return KEAPI_RET_FUNCTION_NOT_SUPPORTED;
		else
			return ret;
	}

	/* Write intruder status -> 0 */
	if ((fw = fopen(INTRUDER_PATH, "w")) == NULL)
		return KEAPI_RET_ERROR;

	if (fprintf(fw, "%d", KEAPI_INTRUDER_STATUS_CASE_CLOSED) == EOF)
		return KEAPI_RET_ERROR;

	fclose(fw);

	return KEAPI_RET_SUCCESS;
}

/*******************************************************************************/

KEAPI_RETVAL KEApiL_ClearPBITResult()
{
	return KEAPI_RET_FUNCTION_NOT_IMPLEMENTED;
}

/*******************************************************************************/

KEAPI_RETVAL KEApiL_GetPBITResult(uint32_t *pResult, uint32_t *pCumulativeResult)
{
	UNUSED(pResult);
	UNUSED(pCumulativeResult);

	return KEAPI_RET_FUNCTION_NOT_IMPLEMENTED;
}
