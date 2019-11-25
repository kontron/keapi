;<KHeader>
;+=========================================================================
;I  Project Name: EApiDK Embedded Application Development Kit
;+=========================================================================
;I  $HeadURL: https://sourceforge.net/p/eapidk/code/HEAD/tree/trunk/include/masm/EApiMacro.inc $
;+=========================================================================
;I   Copyright: Copyright (c) 2002-2009, Kontron Embedded Modules GmbH
;I      Author: John Kearney,                  John.Kearney@kontron.com
;I
;I     License: All rights reserved. This program and the accompanying 
;I              materials are licensed and made available under the 
;I              terms and conditions of the BSD License which 
;I              accompanies this distribution. The full text of the 
;I              license may be found at 
;I              http://opensource.org/licenses/bsd-license.php
;I              
;I              THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "
;I              AS IS" BASIS, WITHOUT WARRANTIES OR REPRESENTATIONS OF 
;I              ANY KIND, EITHER EXPRESS OR IMPLIED.
;I
;I Description: Auto Created for EApiMacro.inc
;I
;+-------------------------------------------------------------------------
;I
;I  File Name            : EApiMacro.inc
;I  File Location        : include\masm
;I
;+=========================================================================
;</KHeader>

;
; PNPID
;
@BYTE_SWAP_W MACRO VAl:REQ
  EXITM <((((VAl AND 0FFh) SHL 8)OR((Val AND 0xFF00h) SHR 8)))>
ENDM

@EAPI_CREATE_PNPID MACRO L1:REQ, L2:REQ, L3:REQ
  EXITM <(@BYTE_SWAP_W(((((L1)-'A'+1) and 01Fh)SHL 10)OR((((L2)-'A'+1) and 01Fh)SHL 5)OR((((L3)-'A'+1) and 01Fh)SHL  0)))>
ENDM

@EAPI_CREATE_CUST_ID MACRO L1:REQ, L2:REQ, L3:REQ, ID:REQ
  EXITM <((0Fh SHL 24)OR(@EAPI_CREATE_PNPID(L1, L2, L3)SHL 12)OR((ID) AND 0FFFh))>
ENDM

;
; All version numbers are bits 31..24: major, 23..16: minor, 15..0 build#
;
@EAPI_VER_GET_VER MACRO Version:REQ
  EXITM <(((Version)SHR 24)AND 0FFh)>
ENDM
@EAPI_VER_GET_REV MACRO Version:REQ
  EXITM <(((Version)SHR 16)AND 0FFh)>
ENDM
@EAPI_VER_GET_BUILD MACRO Version:REQ
  EXITM <(((Version)SHR  0)AND 0FFFFh)>
ENDM
@EAPI_VER_CREATE MACRO Version:REQ, Revision:REQ, Build:REQ
  EXITM <(((Version)AND 0FFh)SHL 24)OR ((Revision)AND 0FFh)SHL 16) OR ((Build)AND 0FFFFh)SHL 0))>
ENDM

;
; I2C
;
@EAPI_I2C_ENC_STD_CMD MACRO Command:REQ
  EXITM <(((Command)and 0FFh))>
ENDM

@EAPI_I2C_ENC_EXT_CMD MACRO Command:REQ
  EXITM <(((Command)and 0FFFFh)OR (1 SHL 31))>
ENDM

comment ~
@EAPI_I2C_ENC_7BIT_ADDR MACRO Addr:REQ
  EXITM <(((Addr)and 07Fh)SHL 1)>
ENDM

@EAPI_I2C_DEC_7BIT_ADDR MACRO Addr:REQ
  EXITM <(((Addr)SHR 1)and 07Fh)>
ENDM

@EAPI_I2C_ENC_10BIT_ADDR MACRO Addr:REQ
  EXITM <(((Addr)AND 0FFh)OR ((((Addr)AND 0300h))SHL 1)OR 0xF000)>
ENDM

@EAPI_I2C_DEC_10BIT_ADDR MACRO Addr:REQ
  EXITM <(((Addr)AND 0FFh)OR ((((Addr) SHR 1)AND 0300h)))>
ENDM

; 
; Can only be used like this
; .if(@EAPI_I2C_IS_10BIT_ADDR(Addr))
; .endif
@EAPI_I2C_IS_10BIT_ADDR MACRO Addr:REQ
  EXITM <(((Addr)AND 0F800h)==0F000h)>
ENDM
@EAPI_I2C_IS_7BIT_ADDR MACRO Addr:REQ
  EXITM <(!@EAPI_I2C_IS_10BIT_ADDR(Addr))>
ENDM

@EAPI_I2C_IS_EXT_CMD MACRO Command:REQ
  EXITM <((Command)AND(1 SHL 31))>
ENDM
@EAPI_I2C_IS_STD_CMD MACRO Command:REQ
  EXITM <(!@EAPI_I2C_IS_EXT_CMD(Command))>
ENDM
~

;
; GPIO
;

@EAPI_GPIO_GPIO_ID MACRO GPIO:REQ
  EXITM <(GPIO)>
ENDM

;EAPI_GPIO_GPIO_BITMASK EQU 1

@EAPI_GPIO_BITMASK_ID MACRO GPIO:REQ
  EXITM <(0x10000|((GPIO)SHR 5))>
ENDM

@EAPI_GPIO_BITMASK_MASK MACRO GPIO:REQ
  EXITM <(1 SHL ((GPIO) AND 01Fh))>
ENDM


