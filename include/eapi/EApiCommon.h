/*
 *<KHeader>
 *+=========================================================================
 *I  Project Name: EApiDK Embedded Application Development Kit
 *+=========================================================================
 *I  $HeadURL: https://sourceforge.net/p/eapidk/code/HEAD/tree/trunk/include/EApiCommon.h $
 *+=========================================================================
 *I   Copyright: Copyright (c) 2009-2010, PICMG
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
 *I Description: Auto Created for EApiCommon.h
 *I
 *+-------------------------------------------------------------------------
 *I
 *I  File Name            : EApiCommon.h
 *I  File Location        : include
 *I
 *+=========================================================================
 *</KHeader>
 */
/* EApi Common Header */
#ifndef _EAPICOMMON_H_    
#define _EAPICOMMON_H_
/*
 * ALL Of these MACROS can be overridden by MACROS
 * in the OS Specific Header File EApiOs.h
 */
#ifndef ELEMENT_SIZE
#  define ELEMENT_SIZE(x) (sizeof((x)[0]))
#endif
#ifndef ARRAY_SIZE
#  define ARRAY_SIZE(x) (sizeof(x)/ELEMENT_SIZE(x))
#endif
#ifndef STRLEN
#  define STRLEN(x) (ARRAY_SIZE(x)- ARRAY_SIZE(""))
#endif

#define EAPI_CREATE_PTR(Base, ByteOffset, PtrType) \
  ((PtrType)(((uint8_t*)(Base))+(ByteOffset)))

#define EAPI_GET_PTR_OFFSET(Ptr1, Ptr2) \
  ((unsigned long)(((const uint8_t* const)(Ptr1))-((const uint8_t*const)(Ptr2))))

#ifndef STRICT_VALIDATION
#  define  STRICT_VALIDATION 2
#endif

#endif /* _EAPICOMMON_H_ */

