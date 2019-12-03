/*
 *<KHeader>
 *+=========================================================================
 *I  Project Name: EApiDK Embedded Application Development Kit
 *+=========================================================================
 *I  $HeadURL: https://sourceforge.net/p/eapidk/code/HEAD/tree/trunk/lib/linux/Message.c $
 *+=========================================================================
 *I  SPDX-License-Identifier: BSD-3-Clause
 *I  Copyright (C) 2019 Kontron Europe GmbH
 *I  Author: John Kearney, John.Kearney@kontron.com
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
#define MANUF_NAME "Kontron Europe GmbH"
#define PROGRAM_DESC "Embedded Application Programming Interface"
char Header[]="\n"
"+----------------------------------------------------------------------------+\n"
"I              PICMG Wrapper over Kontron KEAPI                              I\n"
"I              Copyright (C) Kontron Europe GmbH 2019                        I\n"
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

