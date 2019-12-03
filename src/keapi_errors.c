/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron Europe GmbH */

/* KEAPI error messages specified. */

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef KEAPI_REMOTE
#include "keapi_r.h"
#else
#include "keapi.h"
#endif
#include "string.h"

#ifdef __VXWORKS__
#include "versionK.h"
KONTRON_VERSION(keapi_errors_c, 01b) /* version identifier */
#endif /* __VXWORKS__ */

uint32_t KEApiGetErrorString(KEAPI_RETVAL error, char *pBuffer, int32_t bufferLength)
{
	uint32_t size;
	if (pBuffer == NULL)
		return KEAPI_RET_PARAM_NULL;

	if (bufferLength < 1)
		return KEAPI_RET_BUFFER_OVERFLOW;

	size = bufferLength - 1;
	memset(pBuffer, 0, bufferLength);
	switch (error) {
	case KEAPI_RET_SUCCESS:
		strncpy(pBuffer, "The function was successful", size);
		break;

	case KEAPI_RET_ERROR:
		strncpy(pBuffer, "General error. Use only when there is no appropriate specific error return value",
			size);
		break;

	case KEAPI_RET_PARAM_ERROR:
		strncpy(pBuffer, "Wrong parameter value", size);
		break;

	case KEAPI_RET_PARAM_NULL:
		strncpy(pBuffer, "Parameter is NULL where it is not allowed", size);
		break;
	/*
	case KEAPI_RET_WD_TIMEOUT_NOT_SUPPORTED:
		strncpy(pBuffer, 	"WD time-out not supported", size);
		break;
	*/
	case KEAPI_RET_BUFFER_OVERFLOW:
		strncpy(pBuffer, "Buffer overflow (probably configuration error)", size);
		break;

	case KEAPI_RET_SETTING_ERROR:
		strncpy(pBuffer, "Error while setting value or feature (enable, disable)", size);
		break;

	case KEAPI_RET_WRITE_ERROR:
		strncpy(pBuffer, "Cannot write", size);
		break;

	case KEAPI_RET_READ_ERROR:
		strncpy(pBuffer, "Cannot read", size);
		break;

	case KEAPI_RET_MALLOC_ERROR:
		strncpy(pBuffer, "Memory allocation failed", size);
		break;

	case KEAPI_RET_LIBRARY_ERROR:
		strncpy(pBuffer, "Exported function could not be loaded from library", size);
		break;

	case KEAPI_RET_WMI_ERROR:
		strncpy(pBuffer, "This signalizes some problem during reading from WMI", size);
		break;

	case KEAPI_RET_NOT_INITIALIZED:
		strncpy(pBuffer, "KEAPI library is not initialized", size);
		break;

	case KEAPI_RET_PARTIAL_SUCCESS:
		strncpy(pBuffer,
			"Part of requested information couldn't be retrieved. Returned information isn't complete (buffer is not enough).",
			size);
		break;

	case KEAPI_RET_FUNCTION_NOT_SUPPORTED:
		strncpy(pBuffer, "Function is not supported on current platform/HW", size);
		break;

	case KEAPI_RET_RETRIEVAL_ERROR:
		strncpy(pBuffer, "Error while retrieving information", size);
		break;

	case KEAPI_RET_FUNCTION_NOT_IMPLEMENTED:
		strncpy(pBuffer, "Function is not yet implemented", size);
		break;

	case KEAPI_RET_BUSY_COLLISION:
		strncpy(pBuffer, "Bus/Device Busy or Arbitration Error/Collision Error", size);
		break;

	case KEAPI_RET_HW_TIMEOUT:
		strncpy(pBuffer, "Timeout occurred while accessing to device", size);
		break;

	case KEAPI_RET_BUS_ERROR:
		strncpy(pBuffer, "No acknowledge on address or bus error during operation", size);
		break;

	case KEAPI_RET_CANCELLED:
		strncpy(pBuffer, "Operation is cancelled", size);
		break;

	case KEAPI_RET_PERMISSION_DENIED:
		strncpy(pBuffer, "Permission denied", size);
		break;

#ifdef KEAPI_INTERFACE_REMOTE
	case KEAPI_RET_RPC_ERR_MAX_CONN_COUNT_EXCEEDED:
		strncpy(pBuffer, "RPC - Maximal count of allowed connections on the client side was reached", size);
		break;

	case KEAPI_RET_RPC_ERR_SOCKET_ERROR:
		strncpy(pBuffer, "RPC - This signalizes some error with socket manipulation", size);
		break;

	case KEAPI_RET_RPC_ERR_CONNECTION_NOT_FOUND:
		strncpy(pBuffer, "RPC - Connection for given board handle does not exist", size);
		break;

	case KEAPI_RET_RPC_ERR_NOT_IMPLEMENTED:
		strncpy(pBuffer, "RPC - Function is not implemented for remote call", size);
		break;

	case KEAPI_RET_RPC_ERR_SOCKET_TIMEOUT:
		strncpy(pBuffer, "RPC - This error is returned if time limit for select function expired", size);
		break;

	case KEAPI_RET_RPC_ERR_PACKET_ERROR:
		strncpy(pBuffer, "RPC - This error is returned if format of keapi packet is wrong", size);
		break;

	case KEAPI_RET_RMT_CON_ERROR:
		strncpy(pBuffer, "RPC - Remote call failed", size);
		break;

	case KEAPI_RET_INVALID_HOST:
		strncpy(pBuffer, "RPC - Failed to resolve host", size);
		break;

	case KEAPI_RET_UNAUTHORIZED:
		strncpy(pBuffer, "RPC - Authentication failed", size);
		break;
#endif /* KEAPI_INTERFACE_REMOTE */

	default:
		strncpy(pBuffer, "Unspecified error code", size);
		break;
	}
	return KEAPI_RET_SUCCESS;
}
