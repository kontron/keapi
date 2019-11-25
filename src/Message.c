/*
 *<KHeader>
 *+=========================================================================
 *I  Project Name: EApiDK Embedded Application Development Kit
 *+=========================================================================
 *I  $HeadURL: https://sourceforge.net/p/eapidk/code/HEAD/tree/trunk/lib/linux/Message.c $
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
 *I Description: Auto Created for Message.c
 *I
 *+-------------------------------------------------------------------------
 *I
 *I  File Name            : Message.c
 *I  File Location        : lib\linux
 *I
 *+=========================================================================
 *</KHeader>
 */
#include <stdio.h>
#include "eapi/linux/EApiOs.h"
#include "eapi/EApi.h"
#include "eapi/EApiCommon.h"
#include "eapi/LibVer.h"

#define MPNPID ""
#define MANUF_NAME "Kontron AG"
#define PROGRAM_DESC "Embedded Application Programming Interface"
char Header[]="\n"
"+----------------------------------------------------------------------------+\n"
"I              PICMG Wrapper over Kontron KEAPI                              I\n"
"I              Copyright (C) Kontron AG 2012                                 I\n"
"+----------------------------------------------------------------------------+\n"
"SVersion=%u.%u\n"
"LVersion=%u.%u.%u\n"
"Manufacturer="MANUF_NAME"\n"
"MPNPID="MPNPID"\n"
"OFilename=libEApi"MPNPID".so.%u.%u.0\n"
"Description="PROGRAM_DESC"\n";

void EApiMain (void)
{ 
  printf(
      Header, 
      EAPI_VER, EAPI_REVISION,
      LIB_VERSION, LIB_REVISION, LIB_BUILD,
      EAPI_VER, EAPI_REVISION
      );
  exit(0);
}

