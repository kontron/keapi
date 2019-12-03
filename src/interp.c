/*
 *<KHeader>
 *+=========================================================================
 *I  Project Name: EApiDK Embedded Application Development Kit
 *+=========================================================================
 *I  $HeadURL: https://sourceforge.net/p/eapidk/code/HEAD/tree/trunk/lib/linux/interp.c $
 *+=========================================================================
 *I  SPDX-License-Identifier: BSD-3-Clause
 *I  Copyright (C) 2019 Kontron Europe GmbH
 *I  Author: John Kearney, John.Kearney@kontron.com
 *I
 *I Description: Auto Created for interp.c
 *I
 *+-------------------------------------------------------------------------
 *I
 *I  File Name            : interp.c
 *I  File Location        : lib\linux
 *I
 *+=========================================================================
 *</KHeader>
 */
#ifndef EAPI_INTERPRETER
#error ERROR: EAPI_INTERPRETER is not defined
#endif
#define _MAKE_STRING(x) #x
#define MAKE_STRING(x) _MAKE_STRING(x)
const char __invoke_dynamic_linker__[] __attribute__ ((section (".interp")))
  = EAPI_INTERPRETER;
 
