/*
 *<KHeader>
 *+=========================================================================
 *I  Project Name: EApiDK Embedded Application Development Kit
 *+=========================================================================
 *I  $HeadURL: https://sourceforge.net/p/eapidk/code/HEAD/tree/trunk/lib/linux/interp.c $
 *+=========================================================================
 *I   Copyright: Copyright (c) 2002-2009, Kontron Embedded Modules GmbH
 *I      Author: John Kearney,                  John.Kearney@kontron.com
 *I
 *I     License: All rights reserved. This program and the accompanying 
 *I              materials are licensed and made available under the 
 *I              terms and conditions of the BSD License which 
 *I              accompanies this distribution. The full text of the 
 *I              license may be found at 
 *I              http://opensource.org/licenses/bsd-license.php
 *I              
 *I              THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "
 *I              AS IS" BASIS, WITHOUT WARRANTIES OR REPRESENTATIONS OF 
 *I              ANY KIND, EITHER EXPRESS OR IMPLIED.
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
 
