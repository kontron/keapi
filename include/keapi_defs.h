/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron AG */

/* Various support macros. */

#ifndef _KEAPI_DEFS_H
#define _KEAPI_DEFS_H

#ifdef __VXWORKS__
#include "versionK.h"
KONTRON_VERSION(keapi_defs_h, 01b) /* version identifier */
#endif /* __VXWORKS__ */

#define KEAPI_RETURN_ON_ERROR(ret)                                                                                     \
	if (KEAPI_RET_SUCCESS != ret)                                                                                  \
	return ret

#define FREE(x, l)                                                                                                     \
	if (x != NULL) {                                                                                               \
		free(x);                                                                                               \
		x = NULL;                                                                                              \
		l = 0;                                                                                                 \
	}

#define KEAPI_RET(r, re, e)                                                                                            \
	(((re = e) == KEAPI_RET_SUCCESS) ?                                                                             \
		 ((r == KEAPI_RET_SUCCESS) ? KEAPI_RET_SUCCESS : KEAPI_RET_PARTIAL_SUCCESS) :                          \
		 ((r == KEAPI_RET_SUCCESS) ? KEAPI_RET_PARTIAL_SUCCESS : re))

#define CHECK_NULL(e)                                                                                                  \
	if ((e) == NULL)                                                                                               \
	goto error
#define MALLOC_ERROR(e)                                                                                                \
	if ((e) == NULL)                                                                                               \
	return KEAPI_RET_MALLOC_ERROR

#define KEAPI_SUCCESS(x) (((x == KEAPI_RET_SUCCESS) || (x == KEAPI_RET_PARTIAL_SUCCESS)) ? 1 : 0)
#define KEAPI_FAILED(x) (((x != KEAPI_RET_SUCCESS) && (x != KEAPI_RET_PARTIAL_SUCCESS)) ? 1 : 0)

#endif /* _KEAPI_DEFS_H */
