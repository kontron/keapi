
option expr32
option casemap:none

; Begin of file include\masm\EApiConv.h
; 
;<KHeader>
;+=========================================================================
;I  Project Name: EApiDK Embedded Application Development Kit
;+=========================================================================
;I  $HeadURL: https://sourceforge.net/p/eapidk/code/HEAD/tree/trunk/include/masm/EApiConv.inc $
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
;I Description: Auto Created for EApiConv.h
;I
;+-------------------------------------------------------------------------
;I
;I  File Name            : EApiConv.inc
;I  File Location        : include\masm
;I
;+=========================================================================
;</KHeader>
;  
; Begin of file include\masm\h2inc.h
; 
;  *<KHeader>
;  *+=========================================================================
;  *I               EApiDK Embedded Application Development Kit
;  *+=========================================================================
;  *I  $HeadURL: https://sourceforge.net/p/eapidk/code/HEAD/tree/trunk/include/masm/EApiConv.inc $
;  *+=========================================================================
;  *I   Copyright: Copyright (c) 2002-2009, Kontron Embedded Modules GmbH
;  *I      Author: John Kearney,                  John.Kearney@kontron.com
;  *I
;  *I     License: All rights reserved. This program and the accompanying 
;  *I              materials are licensed and made available under the 
;  *I              terms and conditions of the BSD License which 
;  *I              accompanies this distribution. The full text of the 
;  *I              license may be found at 
;  *I              http://opensource.org/licenses/bsd-license.php
;  *I              
;  *I              THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "
;  *I              AS IS" BASIS, WITHOUT WARRANTIES OR REPRESENTATIONS OF 
;  *I              ANY KIND, EITHER EXPRESS OR IMPLIED.
;  *I
;  *I Description: Auto Created for EApiConv.h
;  *I
;  *+------------------------------------------------------------------------=
;  *I
;  *I  File Name            : EApiConv.h
;  *I  File Location        : includemasm
;  *I
;  *+=========================================================================
;  *</KHeader>
;  
; 
;  * Conversion File to allow easier Conversion using h2inc
;  * h2inc /C /WIN32 /Zu /Ht /FaincludemasmEApiConv.inc includemasmEApiConv.h
;  * Please note this settings are only for valid for WIN32
;  
; 
;  *  Necessary to do change 
;  *  uint64_t		TYPEDEF		DWORD
;  *  int64_t		  TYPEDEF		SDWORD
;  *  to
;  *  uint64_t		TYPEDEF		QWORD
;  *  int64_t		  TYPEDEF		QWORD
;  *
;  *  h2inc doesn't recognise unsigned long long
;  *
;  * Masm doesn't support SQWORD
;  
uint64_t		TYPEDEF		DWORD

; typedef   signed long   int64_t;
uint32_t		TYPEDEF		DWORD

int32_t		TYPEDEF		SDWORD

uint16_t		TYPEDEF		WORD

int16_t		TYPEDEF		SWORD

uint8_t		TYPEDEF		BYTE

int8_t		TYPEDEF		SBYTE

size_t		TYPEDEF		int32_t

wchar_t		TYPEDEF		uint16_t

EEEP_INT32_C		TEXTEQU		<>
EEEP_INT16_C		TEXTEQU		<>
EEEP_INT8_C		TEXTEQU		<>
EEEP_UINT32_C		TEXTEQU		<>
EEEP_UINT16_C		TEXTEQU		<>
EEEP_UINT8_C		TEXTEQU		<>
EAPI_INT32_C		TEXTEQU		<>
EAPI_INT16_C		TEXTEQU		<>
EAPI_INT8_C		TEXTEQU		<>
EAPI_UINT32_C		TEXTEQU		<>
EAPI_UINT16_C		TEXTEQU		<>
EAPI_UINT8_C		TEXTEQU		<>
__IN		TEXTEQU		<>
__OUT		TEXTEQU		<>
__INOPT		TEXTEQU		<>
__INOUT		TEXTEQU		<>
__OUTOPT		TEXTEQU		<>
__INOUTOPT		TEXTEQU		<>
UINT8_MAX		EQU		0000000ffh
INT8_MAX		EQU		00000007fh
UINT16_MAX		EQU		00000ffffh
INT16_MAX		EQU		000007fffh
UINT32_MAX		EQU		0ffffffffh
INT32_MAX		EQU		07fffffffh
UINT64_MAX		EQU		0ffffffffh
INT64_MAX		EQU		0ffffffffh
; End of file include\masm\h2inc.h
; Begin of file include\masm\..\EApi.h
; 
;  *<KHeader>
;  *+=========================================================================
;  *I               EApiDK Embedded Application Development Kit
;  *+=========================================================================
;  *I  $HeadURL: https://sourceforge.net/p/eapidk/code/HEAD/tree/trunk/include/masm/EApiConv.inc $
;  *+=========================================================================
;  *I   Copyright: Copyright (c) 2009, PICMG
;  *I      Author: John Kearney,                  John.Kearney@kontron.com
;  *I
;  *I     License: All rights reserved. This program and the accompanying 
;  *I              materials are licensed and made available under the 
;  *I              terms and conditions of the BSD License which 
;  *I              accompanies this distribution. The full text of the 
;  *I              license may be found at 
;  *I              http://opensource.org/licenses/bsd-license.php
;  *I              
;  *I              THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "
;  *I              AS IS" BASIS, WITHOUT WARRANTIES OR REPRESENTATIONS OF 
;  *I              ANY KIND, EITHER EXPRESS OR IMPLIED.
;  *I
;  *I Description: Auto Created for EApi.h
;  *I
;  *+------------------------------------------------------------------------=
;  *I
;  *I  File Name            : EApi.h
;  *I  File Location        : include
;  *I
;  *+=========================================================================
;  *</KHeader>
;  
_EAPI_H_		TEXTEQU		<>
; *********************************************************************
EAPI_CALLTYPE		TEXTEQU		<>
; ***********************************************************************
;  * All version numbers 
;  * +=========+===============================+
;  * | Bits    | Descriptions                  |
;  * +=========+===============================+
;  * | 24 - 31 | Version Number                |
;  * +---------+-------------------------------+
;  * | 16 - 23 | Revision Number               |
;  * +---------+-------------------------------+
;  * | 0  - 15 | Build Number                  |
;  * +=========+===============================+
;  
EAPI_VER_MASK_VER		EQU		0ff000000h
EAPI_VER_MASK_REV		EQU		000ff0000h
EAPI_VER_MASK_BUILD		EQU		00000ffffh
;  Embedded API Standard Revision 
EAPI_VER		EQU		0t
EAPI_REVISION		EQU		5t
EAPI_VERSION		EQU		000050000h
; ***********************************************************************
;  
;  Embedded Application System Interface 
; 
;  *
;  *
;  *  S T A T U S   C O D E S
;  *
;  *
;  
;  Description
;  *   The EAPI library is not yet or unsuccessfully initialized. 
;  *   EApiLibInitialize needs to be called prior to the first access of any 
;  *   other EAPI function.
;  * Actions
;  *   Call EApiLibInitialize..
;  
EAPI_STATUS_NOT_INITIALIZED		EQU		0ffffffffh
;  Description
;  *   Library is initialized.
;  * Actions
;  *   none.
;  
EAPI_STATUS_INITIALIZED		EQU		0fffffffeh
;  Description
;  *   Memory Allocation Error.
;  * Actions
;  *   Free memory and try again..
;  
EAPI_STATUS_ALLOC_ERROR		EQU		0fffffffdh
;  Description 
;  *   Time out in driver. This is Normally caused by hardware/software 
;  *   semaphore timeout. 
;  * Actions
;  *   Retry.
;  
EAPI_STATUS_DRIVER_TIMEOUT		EQU		0fffffffch
;  Description 
;  *  One or more of the EAPI function call parameters are out of the 
;  *  defined range.
;  *
;  *  Possible Reasons include be
;  *  NULL Pointer
;  *  Invalid Offset
;  *  Invalid Length
;  *  Undefined Value
;  *  
;  *   Storage Write
;  *    Incorrectly Aligned Offset
;  *    Invalid Write Length
;  * Actions
;  *   Verify Function Parameters.
;  
EAPI_STATUS_INVALID_PARAMETER		EQU		0fffffeffh
;  Description
;  *   The Block Alignment is incorrect.
;  * Actions
;  *   Use pInputs and pOutputs to correctly select input and outputs. 
;  
EAPI_STATUS_INVALID_BLOCK_ALIGNMENT		EQU		0fffffefeh
;  Description
;  *   This means that the Block length is too long.
;  * Actions
;  *   Use Alignment Capabilities information to correctly align write access.
;  
EAPI_STATUS_INVALID_BLOCK_LENGTH		EQU		0fffffefdh
;  Description
;  *   The current Direction Argument attempts to set GPIOs to a unsupported 
;  *   directions. I.E. Setting GPI to Output.
;  * Actions
;  *   Use pInputs and pOutputs to correctly select input and outputs. 
;  
EAPI_STATUS_INVALID_DIRECTION		EQU		0fffffefch
;  Description
;  *   The Bitmask Selects bits/GPIOs which are not supported for the current ID.
;  * Actions
;  *   Use pInputs and pOutputs to probe supported bits..
;  
EAPI_STATUS_INVALID_BITMASK		EQU		0fffffefbh
;  Description
;  *   Watchdog timer already started.
;  * Actions
;  *   Call EApiWDogStop, before retrying.
;  
EAPI_STATUS_RUNNING		EQU		0fffffefah
;  Description
;  *   This function or ID is not supported at the actual hardware environment.
;  * Actions
;  *   none.
;  
EAPI_STATUS_UNSUPPORTED		EQU		0fffffcffh
;  Description
;  *   I2C Device Error
;  *   No Acknowledge For Device Address, 7Bit Address Only
;  *   10Bit Address may cause Write error if 2 10Bit addressed devices 
;  *   present on the bus.
;  * Actions
;  *   none.
;  
EAPI_STATUS_NOT_FOUND		EQU		0fffffbffh
;  Description
;  *   I2C Time-out
;  *   Device Clock stretching time-out, Clock pulled low by device 
;  *   for too long
;  * Actions
;  *   none.
;  
EAPI_STATUS_TIMEOUT		EQU		0fffffbfeh
;  Description
;  *   EApi  I2C functions specific. The addressed I2C bus is busy or there 
;  *   is a bus collision.
;  *   The I2C bus is in use. Either CLK or DAT are low.
;  *   Arbitration loss or bus Collision, data remains low when writing a 1
;  * Actions
;  *   Retry.
;  
EAPI_STATUS_BUSY_COLLISION		EQU		0fffffbfdh
;  Description
;  *   I2C Read Error
;  *    Not Possible to detect.
;  *   Storage Read Error
;  *    ....
;  * Actions
;  *   Retry.
;  
EAPI_STATUS_READ_ERROR		EQU		0fffffaffh
;  Description
;  *   I2C Write Error
;  *     No Acknowledge received after writing any Byte after the First Address 
;  *     Byte.
;  *     Can be caused by 
;  *     unsupported Device Command/Index
;  *     Ext Command/Index used on Standard Command/Index Device
;  *     10Bit Address Device Not Present
;  *   Storage Write Error
;  *    ...
;  * Actions
;  *   Retry.
;  
EAPI_STATUS_WRITE_ERROR		EQU		0fffffafeh
;  Description
;  *   The amount of available data exceeds the buffer size.
;  *   Storage buffer overflow was prevented. Read count was larger then 
;  *   the defined buffer length.
;  *   Read Count > Buffer Length
;  * Actions
;  *   Either increase the buffer size or reduce the block length.
;  
EAPI_STATUS_MORE_DATA		EQU		0fffff9ffh
;  Description
;  *   Generic error message. No further error details are available.
;  * Actions
;  *   none.
;  
EAPI_STATUS_ERROR		EQU		0fffff0ffh
;  Description
;  *   The operation was successful.
;  * Actions
;  *   none.
;  
EAPI_STATUS_SUCCESS		EQU		000000000h
;  Library 
;  
;  * EApiLibInitialize
;  * 
;  * Condition                              | Return Values 
;  * ---------------------------------------+------------------------------
;  * Library Already initialized            | EAPI_STATUS_INITIALIZED
;  * Common Error                           | Common Error Code
;  * Else                                   | EAPI_STATUS_SUCCESS
;  
@proto_0		TYPEDEF		PROTO STDCALL 
EApiLibInitialize		PROTO		@proto_0

;  Should be called before 
;                              * calling any other API 
;                              * function is called
;                              
;  
;  * EApiLibUnInitialize
;  * 
;  * Condition                              | Return Values 
;  * ---------------------------------------+------------------------------
;  * Library Uninitialized                  | EAPI_STATUS_NOT_INITIALIZED
;  * Common Error                           | Common Error Code
;  * Else                                   | EAPI_STATUS_SUCCESS
;  
@proto_1		TYPEDEF		PROTO STDCALL 
EApiLibUnInitialize		PROTO		@proto_1

;  Should be called before 
;                              * program exit 
;                              
;  
;  *
;  * Plug and Play Identifiers
;  *
;  *
;  *
;  
;  
;  * The following MACROS are for Creating OEM IDs
;  * OEM ID macros should be named
;  * EAPI_[PNPID]_ID_[TYPE]_[NAME] 
;  * E.G.
;  * EAPI_PMG_ID_BOARD_CUSTOMERID
;  
;  PICMG Should
;                                                             * register this. 
;                                                             
EAPI_PNPID_PICMG		EQU		00000a741h
; 
;  *
;  *      B O A R D   I N F O M A T I O N   S T R I N G S 
;  *
;  
;  IDS 
;  Board Manufacturer
;                                                              * Name String 
;                                                              
EAPI_ID_BOARD_MANUFACTURER_STR		EQU		000000000h
;  Board Name String 
EAPI_ID_BOARD_NAME_STR		EQU		000000001h
;  Board Name String 
EAPI_ID_BOARD_REVISION_STR		EQU		000000002h
;  Board Serial 
; 							     * Number String 
; 							     
EAPI_ID_BOARD_SERIAL_STR		EQU		000000003h
;  Board Bios Revision
; 							     * String 
; 							     
EAPI_ID_BOARD_BIOS_REVISION_STR		EQU		000000004h
;  Board Hardware 
; 							     * Revision String 
; 							     
EAPI_ID_BOARD_HW_REVISION_STR		EQU		000000005h
;  Platform ID 
; 							     * (ETX, COM Express, 
; 							     * etc...) 
; 							     
EAPI_ID_BOARD_PLATFORM_TYPE_STR		EQU		000000006h
;  
;  * EApiBoardGetStringA
;  * 
;  * Condition                              | Return Values 
;  * ---------------------------------------+------------------------------
;  * Library Uninitialized                  | EAPI_STATUS_NOT_INITIALIZED
;  * pBufLen==NULL                          | EAPI_STATUS_INVALID_PARAMETER
;  * pBufLen!=NULL&&*pBufLen&&pBuffer==NULL | EAPI_STATUS_INVALID_PARAMETER
;  * unknown Id                             | EAPI_STATUS_UNSUPPORTED
;  * strlen(Id)+1>*pBufLen                  | EAPI_STATUS_MORE_DATA
;  * Common Error                           | Common Error Code
;  * Else                                   | EAPI_STATUS_SUCCESS
;  
;  Name Id 
;  Destination pBuffer 
;  pBuffer Length 
@proto_2		TYPEDEF		PROTO STDCALL :uint32_t, :PTR SBYTE, :PTR uint32_t
EApiBoardGetStringA		PROTO		@proto_2

; 
;  *
;  *      B O A R D   I N F O M A T I O N   V A L U E S
;  *
;  
;  IDS 
;  EAPI Specification 
;                                                                 * Revision I.E. The 
;                                                                 * EAPI Spec Version 
;                                                                 * Bits 31-24, Revision 
;                                                                 * 23-16, 15-0 always 0
;                                                                 * Used to implement 
;                                                                 * this interface
;                                                                 
EAPI_ID_GET_EAPI_SPEC_VERSION		EQU		000000000h
;  Units = Boots 
EAPI_ID_BOARD_BOOT_COUNTER_VAL		EQU		000000001h
;  Units = Minutes 
EAPI_ID_BOARD_RUNNING_TIME_METER_VAL		EQU		000000002h
;  Encoded PNP ID 
;                                                                 * Format 
;                                                                 * (Compressed ASCII)
;                                                                 
EAPI_ID_BOARD_PNPID_VAL		EQU		000000003h
;  Platform Revision 
;                                                                 * I.E. The PICMG Spec 
;                                                                 * Version Bits 31-24,
;                                                                 * Revision 23-16, 
;                                                                 * 15-0 always 0
;                                                                 
EAPI_ID_BOARD_PLATFORM_REV_VAL		EQU		000000004h
;  Vendor Specific 
;                                                                   * (Optional) 
;                                                                   
EAPI_ID_BOARD_DRIVER_VERSION_VAL		EQU		000010000h
;  Vendor Specific
;                                                                   * (Optional) 
;                                                                   
EAPI_ID_BOARD_LIB_VERSION_VAL		EQU		000010001h
;  0.1 Kelvins 
EAPI_ID_HWMON_CPU_TEMP		EQU		000020000h
;  0.1 Kelvins 
EAPI_ID_HWMON_CHIPSET_TEMP		EQU		000020001h
;  0.1 Kelvins 
EAPI_ID_HWMON_SYSTEM_TEMP		EQU		000020002h
EAPI_KELVINS_OFFSET		EQU		2731t
;  millivolts 
EAPI_ID_HWMON_VOLTAGE_VCORE		EQU		000021004h
;  millivolts 
EAPI_ID_HWMON_VOLTAGE_2V5		EQU		000021008h
;  millivolts 
EAPI_ID_HWMON_VOLTAGE_3V3		EQU		00002100ch
;  millivolts 
EAPI_ID_HWMON_VOLTAGE_VBAT		EQU		000021010h
;  millivolts 
EAPI_ID_HWMON_VOLTAGE_5V		EQU		000021014h
;  millivolts 
EAPI_ID_HWMON_VOLTAGE_5VSB		EQU		000021018h
;  millivolts 
EAPI_ID_HWMON_VOLTAGE_12V		EQU		00002101ch
;  RPM 
EAPI_ID_HWMON_FAN_CPU		EQU		000022000h
;  RPM 
EAPI_ID_HWMON_FAN_SYSTEM		EQU		000022001h
;  
;  * EApiBoardGetValue
;  * 
;  * Condition                              | Return Values 
;  * ---------------------------------------+------------------------------
;  * Library Uninitialized                  | EAPI_STATUS_NOT_INITIALIZED
;  * pValue==NULL                           | EAPI_STATUS_INVALID_PARAMETER
;  * unknown Id                             | EAPI_STATUS_UNSUPPORTED
;  * Common Error                           | Common Error Code
;  * Else                                   | EAPI_STATUS_SUCCESS
;  
;  Value Id 
;  Return Value 
@proto_3		TYPEDEF		PROTO STDCALL :uint32_t, :PTR uint32_t
EApiBoardGetValue		PROTO		@proto_3

; 
;  *
;  *      B A C K L I G H T
;  *
;  
;  IDS 
EAPI_ID_BACKLIGHT_1		EQU		000000000h
EAPI_ID_BACKLIGHT_2		EQU		000000001h
EAPI_ID_BACKLIGHT_3		EQU		000000002h
;  Backlight Values 
EAPI_BACKLIGHT_SET_ON		EQU		000000000h
EAPI_BACKLIGHT_SET_OFF		EQU		0ffffffffh
EAPI_BACKLIGHT_SET_DIMEST		EQU		000000000h
EAPI_BACKLIGHT_SET_BRIGHTEST		EQU		0000000ffh
;  
;  * EApiVgaGetBacklightEnable
;  * 
;  * Condition                              | Return Values 
;  * ---------------------------------------+------------------------------
;  * Library Uninitialized                  | EAPI_STATUS_NOT_INITIALIZED
;  * pEnable==NULL                          | EAPI_STATUS_INVALID_PARAMETER
;  * unknown Id                             | EAPI_STATUS_UNSUPPORTED
;  * Common Error                           | Common Error Code
;  * Else                                   | EAPI_STATUS_SUCCESS
;  
;  Backlight Id 
;  Backlight Enable 
@proto_4		TYPEDEF		PROTO STDCALL :uint32_t, :PTR uint32_t
EApiVgaGetBacklightEnable		PROTO		@proto_4

;  
;  * EApiVgaSetBacklightEnable
;  * 
;  * Condition                              | Return Values 
;  * ---------------------------------------+------------------------------
;  * Library Uninitialized                  | EAPI_STATUS_NOT_INITIALIZED
;  * Enable!=EAPI_BACKLIGHT_SET_ON&&
;  * Enable!=EAPI_BACKLIGHT_SET_OFF         | EAPI_STATUS_INVALID_PARAMETER
;  * unknown Id                             | EAPI_STATUS_UNSUPPORTED
;  * Common Error                           | Common Error Code
;  * Else                                   | EAPI_STATUS_SUCCESS
;  
;  Backlight Id 
;  Backlight Enable 
@proto_5		TYPEDEF		PROTO STDCALL :uint32_t, :uint32_t
EApiVgaSetBacklightEnable		PROTO		@proto_5

;  
;  * EApiVgaGetBacklightBrightness
;  * 
;  * Condition                              | Return Values 
;  * ---------------------------------------+------------------------------
;  * Library Uninitialized                  | EAPI_STATUS_NOT_INITIALIZED
;  * pBright==NULL                          | EAPI_STATUS_INVALID_PARAMETER
;  * unknown Id                             | EAPI_STATUS_UNSUPPORTED
;  * Common Error                           | Common Error Code
;  * Else                                   | EAPI_STATUS_SUCCESS
;  
;  Backlight Id 
;  Backlight Brightness 
@proto_6		TYPEDEF		PROTO STDCALL :uint32_t, :PTR uint32_t
EApiVgaGetBacklightBrightness		PROTO		@proto_6

;  
;  * EApiVgaSetBacklightBrightness
;  * 
;  * Condition                              | Return Values 
;  * ---------------------------------------+------------------------------
;  * Library Uninitialized                  | EAPI_STATUS_NOT_INITIALIZED
;  * Bright>EAPI_BACKLIGHT_SET_BRIGHTEST    | EAPI_STATUS_INVALID_PARAMETER
;  * unknown Id                             | EAPI_STATUS_UNSUPPORTED
;  * Common Error                           | Common Error Code
;  * Else                                   | EAPI_STATUS_SUCCESS
;  
;  Backlight Id 
;  Backlight Brightness 
@proto_7		TYPEDEF		PROTO STDCALL :uint32_t, :uint32_t
EApiVgaSetBacklightBrightness		PROTO		@proto_7

; 
;  *
;  *      S T O R A G E
;  *
;  
;  IDs 
EAPI_ID_STORAGE_STD		EQU		000000000h
;  Dummy Example 
EAPI_PMG_ID_STORAGE_SAMPLE		EQU		0fa741000h
;  
;  * EApiStorageCap
;  * 
;  * Condition                                    | Return Values 
;  * ---------------------------------------------+------------------------------
;  * Library Uninitialized                        | EAPI_STATUS_NOT_INITIALIZED
;  * ((pStorageSize==NULL)&&(pBlockLength==NULL)) | EAPI_STATUS_INVALID_PARAMETER
;  * Unsupported Id                               | EAPI_STATUS_UNSUPPORTED
;  * Common Error                                 | Common Error Code
;  * Else                                         | EAPI_STATUS_SUCCESS
;  
;  Storage Area Id 
;  Total 
;  Write Block Length
;                                      * & Alignment 
;                                      
@proto_8		TYPEDEF		PROTO STDCALL :uint32_t, :PTR uint32_t, :PTR uint32_t
EApiStorageCap		PROTO		@proto_8

;  
;  * EApiStorageAreaRead
;  * 
;  * Condition                              | Return Values 
;  * ---------------------------------------+------------------------------
;  * Library Uninitialized                  | EAPI_STATUS_NOT_INITIALIZED
;  * pBuffer==NULL                          | EAPI_STATUS_INVALID_PARAMETER
;  * ByteCnt==0                             | EAPI_STATUS_INVALID_PARAMETER
;  * BufLen==0                              | EAPI_STATUS_INVALID_PARAMETER
;  * unknown Id                             | EAPI_STATUS_UNSUPPORTED
;  * Offset+ByteCnt>pStorageSize            | EAPI_STATUS_INVALID_BLOCK_LENGTH
;  * Read Error                             | EAPI_STATUS_READ_ERROR
;  * ByteCnt>BufLen                         | EAPI_STATUS_MORE_DATA
;  * Common Error                           | Common Error Code
;  * Else                                   | EAPI_STATUS_SUCCESS
;  
;  Storage Area Id 
;  Byte Offset 
;  Pointer to Data pBuffer 
;  Data pBuffer Size in 
;                                * bytes 
;                                
;  Number of bytes to read 
@proto_9		TYPEDEF		PROTO STDCALL :uint32_t, :uint32_t, :PTR , :uint32_t, :uint32_t
EApiStorageAreaRead		PROTO		@proto_9

;  
;  * EApiStorageAreaWrite
;  * 
;  * Condition                              | Return Values 
;  * ---------------------------------------+------------------------------
;  * Library Uninitialized                  | EAPI_STATUS_NOT_INITIALIZED
;  * pBuffer==NULL                          | EAPI_STATUS_INVALID_PARAMETER
;  * ByteCnt==0                             | EAPI_STATUS_INVALID_PARAMETER
;  * unknown Id                             | EAPI_STATUS_UNSUPPORTED
;  * Offset%pBlockLength                    | EAPI_STATUS_INVALID_BLOCK_ALIGNMENT
;  * ByteCnt%pBlockLength                   | EAPI_STATUS_INVALID_BLOCK_ALIGNMENT
;  * Offset+ByteCnt>pStorageSize            | EAPI_STATUS_INVALID_BLOCK_LENGTH
;  * Write Error                            | EAPI_STATUS_WRITE_ERROR
;  * Common Error                           | Common Error Code
;  * Else                                   | EAPI_STATUS_SUCCESS
;  
;  Storage Area Id 
;  Byte Offset 
;  Pointer to Data pBuffer 
;  Number of bytes to write 
@proto_10		TYPEDEF		PROTO STDCALL :uint32_t, :uint32_t, :PTR , :uint32_t
EApiStorageAreaWrite		PROTO		@proto_10

; 
;  *
;  *      I 2 C
;  *
;  
;  IDs 
;  Baseboard I2C Interface
;                                                      * required 
;                                                      
EAPI_ID_I2C_EXTERNAL		EQU		000000000h
;  LVDS/EDP 1 Interface 
;                                                      * (optional)
;                                                      
EAPI_ID_I2C_LVDS_1		EQU		000000001h
;  LVDS/EDP 2 Interface 
;                                                      * (optional)
;                                                      
EAPI_ID_I2C_LVDS_2		EQU		000000002h
; 
;  * I2C Address Format 
;  *
;  *  L = Set to 0   
;  *  H = Set to 1   
;  *  X = Don't Care(Direction Bit)
;  *  0-F Address Bit 
;  *
;  *  Bits 31-16 are Reserved and should be set to 0
;  *
;  *  Bit Offset      | F E D C B A 9 8 7 6 5 4 3 2 1 0
;  *  ----------------+--------------------------------
;  *  7  Bit Address  | L L L L L L L L 6 5 4 3 2 1 0 X
;  *  10 Bit Address  | H H H H L 9 8 X 7 6 5 4 3 2 1 0
;  *
;  *  Examples where Don't Care bits set to 0
;  *             Encoded Encoded
;  *  Address     7Bit    10Bit
;  *    0x01      0x02   0xF001
;  *    0x58      0xA0   0xF058
;  *    0x59      0xA2   0xF059
;  *    0x77      0xEE   0xF077
;  *    0x3FF            0xF6FF
;  *
;  
;  
;  * EApiI2CGetBusCap
;  * 
;  * Condition                              | Return Values 
;  * ---------------------------------------+------------------------------
;  * Library Uninitialized                  | EAPI_STATUS_NOT_INITIALIZED
;  * pMaxBlkLen==NULL                       | EAPI_STATUS_INVALID_PARAMETER
;  * unknown Id                             | EAPI_STATUS_UNSUPPORTED
;  * Common Error                           | Common Error Code
;  * Else                                   | EAPI_STATUS_SUCCESS
;  
;  I2C Bus Id 
;  Max Block Length 
;                                   * Supported on this
;                                   * interface 
;                                   
@proto_11		TYPEDEF		PROTO STDCALL :uint32_t, :PTR uint32_t
EApiI2CGetBusCap		PROTO		@proto_11

;  
;  * EApiI2CWriteRead
;  * 
;  * Condition                                | Return Values 
;  * -----------------------------------------+------------------------------
;  * Library Uninitialized                    | EAPI_STATUS_NOT_INITIALIZED
;  * (WriteBCnt>1)&&(pWBuffer==NULL)          | EAPI_STATUS_INVALID_PARAMETER
;  * (ReadBCnt>1)&&(pRBuffer==NULL)           | EAPI_STATUS_INVALID_PARAMETER
;  * (ReadBCnt>1)&&(RBufLen==0)               | EAPI_STATUS_INVALID_PARAMETER
;  * ((WriteBCnt==0)&&(ReadBCnt==0))          | EAPI_STATUS_INVALID_PARAMETER
;  * unknown Id                               | EAPI_STATUS_UNSUPPORTED
;  * WriteBCnt>(pMaxBlkLen+1)                 | EAPI_STATUS_INVALID_BLOCK_LENGTH
;  * ReadBCnt>(pMaxBlkLen+1)                  | EAPI_STATUS_INVALID_BLOCK_LENGTH
;  * Bus Busy  SDA/SDC low                    | EAPI_STATUS_BUSY_COLLISION
;  * Arbitration Error/Collision Error        | EAPI_STATUS_BUSY_COLLISION
;  *   On Write 1 write cycle                 |
;  *   SDA Remains low                        |
;  * Timeout due to clock stretching          | EAPI_STATUS_TIMEOUT
;  * start<Addr Byte><W>Nak                   | EAPI_STATUS_NOT_FOUND
;  * start<Addr Byte><R>Nak                   | EAPI_STATUS_NOT_FOUND
;  * start<Addr Byte><W>Ack<Data Byte 1>Nak   | EAPI_STATUS_WRITE_ERROR
;  * ReadBCnt>(RBufLen+1)                     | EAPI_STATUS_MORE_DATA
;  * Common Error                             | Common Error Code
;  * Else                                     | EAPI_STATUS_SUCCESS
;  
;  I2C Bus Id 
;  Encoded 7Bit I2C
;                                    * Device Address 
;                                    
;  Write Data pBuffer 
;  Number of Bytes to 
;                                    * write plus 1
;                                    
;  Read Data pBuffer 
;  Data pBuffer Length 
;  Number of Bytes to 
;                                    * Read plus 1
;                                    
@proto_12		TYPEDEF		PROTO STDCALL :uint32_t, :uint8_t, :PTR , :uint32_t, :PTR , :uint32_t, :uint32_t
EApiI2CWriteReadRaw		PROTO		@proto_12

; 
;  * I2C Transfer Types
;  * Bits 31 & 30 Selects Command Type
;  *
;  * Transfer Type 1: 
;  * Address Format : 7Bit
;  * Command Type   : None
;  * Data Direction : Write
;  * Start<Addr Byte><W>Ack<Data Byte 1>Ack Stop
;  *
;  * Transfer Type 2: 
;  * Address Format : 7Bit
;  * Command Type   : None
;  * Data Direction : Read 
;  * Start<Addr Byte><R>Ack<Data Byte 1>Nak Stop
;  *
;  * Transfer Type 3: 
;  * Address Format : 7Bit
;  * Command Type   : Standard
;  * Data Direction : Write
;  * Start<Addr Byte><W>Ack<CMD Byte>Ack<Data Byte 1>Ack Stop
;  *
;  * Transfer Type 4: 
;  * Address Format : 7Bit
;  * Command Type   : Standard
;  * Data Direction : Read
;  * Start<Addr Byte><W>Ack<CMD Byte>Ack 
;  * Start<Addr Byte><R>Ack<Data Byte 1>Nak Stop
;  *
;  * Transfer Type 5: 
;  * Address Format : 7Bit
;  * Command Type   : Extended
;  * Data Direction : Write
;  * Start<Addr Byte><W>Ack<CMD Byte MSB>Ack<CMD Byte LSB>Ack<Data Byte 1>Ack Stop
;  *
;  * Transfer Type 6: 
;  * Address Format : 7Bit
;  * Command Type   : Extended
;  * Data Direction : Read
;  * Start<Addr Byte><W>Ack<CMD Byte MSB>Ack<CMD Byte LSB>Ack 
;  * Start<Addr Byte><R>Ack<Data Byte 1>Nak Stop
;  *
;  * Transfer Type 7: 
;  * Address Format : 10Bit
;  * Command Type   : None
;  * Data Direction : Write
;  * Start<Addr Byte MSB><W>Ack<Addr Byte LSB>Ack<Data Byte 1>Ack Stop
;  *
;  * Transfer Type 8: 
;  * Address Format : 10Bit
;  * Command Type   : None
;  * Data Direction : Read
;  * Start<Addr Byte MSB><W>Ack<Addr Byte LSB>Ack 
;  * Start<Addr Byte MSB><R>Ack<Data Byte 1>Nak Stop
;  *
;  * Transfer Type 9: 
;  * Address Format : 10Bit
;  * Command Type   : Standard
;  * Data Direction : Write
;  * Start<Addr Byte MSB><W>Ack<Addr Byte LSB>Ack<CMD Byte>Ack<Data Byte 1>Ack Stop
;  *
;  * Transfer Type 10: 
;  * Address Format : 10Bit
;  * Command Type   : Standard
;  * Data Direction : Read
;  * Start<Addr Byte MSB><W>Ack<Addr Byte LSB>Ack<CMD Byte>Ack 
;  * Start<Addr Byte MSB><R>Ack<Data Byte 1>Nak Stop
;  *
;  * Transfer Type 11: 
;  * Address Format : 10Bit
;  * Command Type   : Extended
;  * Data Direction : Write
;  * Start<Addr Byte MSB><W>Ack<Addr Byte LSB>Ack<CMD Byte MSB>Ack<CMD Byte LSB>Ack<Data Byte 1>Ack Stop
;  *
;  * Transfer Type 12: 
;  * Address Format : 10Bit
;  * Command Type   : Extended
;  * Data Direction : Read
;  * Start<Addr Byte MSB><W>Ack<Addr Byte LSB>Ack<CMD Byte MSB>Ack<CMD Byte LSB>Ack
;  * Start<Addr Byte MSB><R>Ack<Data Byte 1>Nak Stop
;  *
;  
EAPI_I2C_STD_CMD		EQU		000000000h
EAPI_I2C_EXT_CMD		EQU		080000000h
EAPI_I2C_NO_CMD		EQU		040000000h
EAPI_I2C_CMD_TYPE_MASK		EQU		0c0000000h
;  
;  * EApiI2CReadTransfer
;  * Addr Byte 1 Below Designates Addr MSB in a 10bit address transfer and 
;  * the complete address in an 7bit address transfer.
;  * 
;  * Condition                                | Return Values 
;  * -----------------------------------------+------------------------------
;  * Library Uninitialized                    | EAPI_STATUS_NOT_INITIALIZED
;  * pBuffer==NULL                            | EAPI_STATUS_INVALID_PARAMETER
;  * BufLen==0                                | EAPI_STATUS_INVALID_PARAMETER
;  * ByteCnt==0                               | EAPI_STATUS_INVALID_PARAMETER
;  * unknown Id                               | EAPI_STATUS_UNSUPPORTED
;  * ByteCnt>pMaxBlkLen                       | EAPI_STATUS_INVALID_BLOCK_LENGTH
;  * Bus Busy  SDA/SDC low                    | EAPI_STATUS_BUSY_COLLISION
;  * Arbitration Error/Collision Error        | EAPI_STATUS_BUSY_COLLISION
;  *   On Write 1 write cycle                 |
;  *   SDA Remains low                        |
;  * Time-out due to clock stretching         | EAPI_STATUS_TIMEOUT
;  * start<Addr Byte 1><W>Nak                 | EAPI_STATUS_NOT_FOUND
;  * start<Addr Byte 1><R>Nak                 | EAPI_STATUS_NOT_FOUND
;  * start<Addr Byte 1><W>Ack<Addr Byte 2>Nak | EAPI_STATUS_WRITE_ERROR
;  * start<Addr Byte 1><W>Ack<CMD Byte 1>Nak  | EAPI_STATUS_WRITE_ERROR
;  * start<Addr Byte 1><W>Ack<Data Byte 1>Nak | EAPI_STATUS_WRITE_ERROR
;  * ByteCnt>BufLen                           | EAPI_STATUS_MORE_DATA
;  * Common Error                             | Common Error Code
;  * Else                                     | EAPI_STATUS_SUCCESS
;  
;  I2C Bus Id 
;  Encoded 7/10Bit I2C
;                                * Device Address
;                                
;  I2C Command/Offset 
;  Transfer Data pBuffer 
;  Data pBuffer Length 
;  Byte Count to read 
@proto_13		TYPEDEF		PROTO STDCALL :uint32_t, :uint32_t, :uint32_t, :PTR , :uint32_t, :uint32_t
EApiI2CReadTransfer		PROTO		@proto_13

;  
;  * EApiI2CWriteTransfer
;  * Addr Byte 1 Below Designates Addr MSB in a 10bit address transfer and 
;  * the complete address in an 7bit address transfer.
;  * 
;  * Condition                                | Return Values 
;  * ---------------------------------------+------------------------------
;  * Library Uninitialized                    | EAPI_STATUS_NOT_INITIALIZED
;  * pBuffer==NULL                            | EAPI_STATUS_INVALID_PARAMETER
;  * ByteCnt==0                               | EAPI_STATUS_INVALID_PARAMETER
;  * unknown Id                               | EAPI_STATUS_UNSUPPORTED
;  * ByteCnt+(overhead)>pMaxBlkLen            | EAPI_STATUS_INVALID_BLOCK_LENGTH
;  * Bus Busy  SDA/SDC low                    | EAPI_STATUS_BUSY_COLLISION
;  * Arbitration Error/Collision Error        | EAPI_STATUS_BUSY_COLLISION
;  *   On Write 1 write cycle                 |
;  *   SDA Remains low                        |
;  * Time-out due to clock stretching         | EAPI_STATUS_TIMEOUT
;  * start<Addr Byte 1><W>Nak                 | EAPI_STATUS_NOT_FOUND
;  * start<Addr Byte 1><R>Nak                 | EAPI_STATUS_NOT_FOUND
;  * start<Addr Byte 1><W>Ack<Addr Byte 2>Nak | EAPI_STATUS_WRITE_ERROR
;  * start<Addr Byte 1><W>Ack<CMD Byte 1>Nak  | EAPI_STATUS_WRITE_ERROR
;  * start<Addr Byte 1><W>Ack<Data Byte 1>Nak | EAPI_STATUS_WRITE_ERROR
;  * Common Error                             | Common Error Code
;  * Else                                     | EAPI_STATUS_SUCCESS
;  
;  I2C Bus Id 
;  Encoded 7/10Bit I2C 
;                                * Device Address 
;                                
;  I2C Command/Offset 
;  Transfer Data pBuffer 
;  Byte Count to write 
@proto_14		TYPEDEF		PROTO STDCALL :uint32_t, :uint32_t, :uint32_t, :PTR , :uint32_t
EApiI2CWriteTransfer		PROTO		@proto_14

; 
;  * I2C Probe Types
;  *
;  * Probe Type 1: 
;  * Address Format : 7Bit
;  * Start<Addr Byte><W>Ack Stop
;  *
;  * Probe Type 2: 
;  * Address Format : 10Bit
;  * Start<Addr Byte MSB><W>Ack<Addr Byte LSB>Ack Stop
;  *
;  
;  
;  * EApiI2CProbeDevice
;  * 
;  * Condition                                    | Return Values 
;  * ---------------------------------------------+------------------------------
;  * Library Uninitialized                        | EAPI_STATUS_NOT_INITIALIZED
;  * Bus Busy  SDA/SDC low                        | EAPI_STATUS_BUSY_COLLISION
;  * Arbitration Error/Collision Error            | EAPI_STATUS_BUSY_COLLISION
;  *   On Write 1 write cycle                     |
;  *   SDA Remains low                            |
;  * Time-out due to clock stretching             | EAPI_STATUS_TIMEOUT
;  *                                              |
;  * 7Bit Address                                 |
;  * start<Addr Byte><W>Nak                       | EAPI_STATUS_NOT_FOUND
;  *                                              |
;  * 10Bit Address                                |
;  * start<Addr Byte MSB><W>Nak                   | EAPI_STATUS_NOT_FOUND
;  * start<Addr Byte MSB><W>Ack<Addr Byte LSB>Nak | EAPI_STATUS_NOT_FOUND
;  *                                              |
;  * Common Error                                 | Common Error Code
;  * Else                                         | EAPI_STATUS_SUCCESS
;  
;  I2C Bus Id 
;  Encoded 7/10Bit 
;                             * I2C Device Address 
;                             
@proto_15		TYPEDEF		PROTO STDCALL :uint32_t, :uint32_t
EApiI2CProbeDevice		PROTO		@proto_15

; 
;  *
;  *      W A T C H D O G
;  *
;  
; 
;  *
;  * After EApiWDogStart
;  *
;  * |<- Delay ->|<- Event Timeout ->|<- Reset Timeout ->|
;  * A-----------B-------------------C-------------------D
;  *
;  *
;  * After EApiWDogTrigger
;  *
;  * |<- Event Timeout ->|<- Reset Timeout ->|
;  * E-------------------F-------------------G
;  *
;  *
;  * Stage A
;  *  Watchdog is started.
;  *
;  * Stage B
;  *  Initial Delay Period is exhausted.
;  *
;  * Stage C/F
;  *  Event is triggered, NMI, IRQ, or PIN is Triggered.
;  *  To Allow for possible Software Recovery.
;  *
;  * Stage D/G
;  *  System is reset.
;  *
;  * Stage E
;  *  Watchdog is Triggered.
;  *
;  * EApiWDogTrigger/EApiWDogStop Must be called before Stage C/F
;  * to prevent event from being generated.
;  *
;  * EApiWDogTrigger/EApiWDogStop Must be called before Stage D/G
;  * to prevent The system from being reset.
;  *
;  
;  
;  * EApiWDogGetCap
;  * 
;  * Condition                              | Return Values 
;  * ---------------------------------------+------------------------------
;  * Library Uninitialized                  | EAPI_STATUS_NOT_INITIALIZED
;  * Unsupported                            | EAPI_STATUS_UNSUPPORTED
;  * pMaxDelay==NULL&&                      |
;  * pMaxResetTimeout==NULL&&               |
;  * pMaxEventTimeout==NULL                 | EAPI_STATUS_INVALID_PARAMETER
;  * Common Error                           | Common Error Code
;  * Else                                   | EAPI_STATUS_SUCCESS
;  
;  Maximum Supported 
;                                          * Delay in milliseconds
;                                          
;  Maximum Supported 
;                                          * Event Timeout in 
;                                          * milliseconds
;                                          * 0 == Unsupported
;                                          
;  Maximum Supported 
;                                          * Reset Timeout in 
;                                          * milliseconds
;                                          
@proto_16		TYPEDEF		PROTO STDCALL :PTR uint32_t, :PTR uint32_t, :PTR uint32_t
EApiWDogGetCap		PROTO		@proto_16

;  
;  * EApiWDogStart
;  * 
;  * Condition                        | Return Values 
;  * ---------------------------------+------------------------------
;  * Library Uninitialized            | EAPI_STATUS_NOT_INITIALIZED
;  * Unsupported                      | EAPI_STATUS_UNSUPPORTED
;  * Delay>pMaxDelay                  | EAPI_STATUS_INVALID_PARAMETER
;  * EventTimeout>pMaxEventTimeout    | EAPI_STATUS_INVALID_PARAMETER
;  * ResetTimeout>pMaxResetTimeout    | EAPI_STATUS_INVALID_PARAMETER
;  * Already Running                  | EAPI_STATUS_RUNNING
;  * Common Error                     | Common Error Code
;  * Else                             | EAPI_STATUS_SUCCESS
;  
;  Delay in milliseconds 
;  Event Timeout in 
;                                   * milliseconds 
;                                   
;  Reset Timeout in 
;                                   * milliseconds 
;                                   
@proto_17		TYPEDEF		PROTO STDCALL :uint32_t, :uint32_t, :uint32_t
EApiWDogStart		PROTO		@proto_17

;  
;  * EApiWDogTrigger
;  * 
;  * Condition                        | Return Values 
;  * ---------------------------------+------------------------------
;  * Library Uninitialized            | EAPI_STATUS_NOT_INITIALIZED
;  * Unsupported                      | EAPI_STATUS_UNSUPPORTED
;  * Watchdog Not Started             | EAPI_STATUS_ERROR
;  * Common Error                     | Common Error Code
;  * Else                             | EAPI_STATUS_SUCCESS
;  
@proto_18		TYPEDEF		PROTO STDCALL 
EApiWDogTrigger		PROTO		@proto_18

;  
;  * EApiWDogStop
;  * 
;  * Condition                        | Return Values 
;  * ---------------------------------+------------------------------
;  * Library Uninitialized            | EAPI_STATUS_NOT_INITIALIZED
;  * Unsupported                      | EAPI_STATUS_UNSUPPORTED
;  * Common Error                     | Common Error Code
;  * Else                             | EAPI_STATUS_SUCCESS
;  
@proto_19		TYPEDEF		PROTO STDCALL 
EApiWDogStop		PROTO		@proto_19

; 
;  *
;  *      G P I O
;  *
;  
; 
;  *
;  * +-----------------------------------------------+
;  * |              Physical GPIO                    |
;  * +-----+-----+-----+-----+-----+-----+-----+-----+
;  * |  0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |
;  * +-----+-----+-----+-----+-----+-----+-----+-----+
;  *    |     |     |     |     |     |     |     |
;  *    |     |     |     |     |     |     |     +-----------------------------------------------+
;  *    |     |     |     |     |     |     |     |                                               |
;  *    |     |     |     |     |     |     +-----|-----------------------------------------+     |
;  *    |     |     |     |     |     |     |     |                                         |     |
;  *    |     |     |     |     |     +-----|-----|-----------------------------------+     |     |
;  *    |     |     |     |     |     |     |     |                                   |     |     |
;  *    |     |     |     |     +-----|-----|-----|-----------------------------+     |     |     |
;  *    |     |     |     |     |     |     |     |                             |     |     |     |
;  *    |     |     |     +-----|-----|-----|-----|-----------------------+     |     |     |     |
;  *    |     |     |     |     |     |     |     |                       |     |     |     |     |
;  *    |     |     +-----|-----|-----|-----|-----|-----------------+     |     |     |     |     |
;  *    |     |     |     |     |     |     |     |                 |     |     |     |     |     |
;  *    |     +-----|-----|-----|-----|-----|-----|-----------+     |     |     |     |     |     |
;  *    |     |     |     |     |     |     |     |           |     |     |     |     |     |     |
;  *    +-----|-----|-----|-----|-----|-----|-----|-----+     |     |     |     |     |     |     |
;  *    |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |
;  *    |     |     |     |     |     |     |     |  +-----+-----+-----+-----+-----+-----+-----+-----+
;  *    |     |     |     |     |     |     |     |  | Bit | Bit | Bit | Bit | Bit | Bit | Bit | Bit |
;  *    |     |     |     |     |     |     |     |  |  0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |
;  *    |     |     |     |     |     |     |     |  +-----------------------------------------------+
;  *    |     |     |     |     |     |     |     |  |            EAPI_ID_GPIO_BITMASK00             |
;  *    |     |     |     |     |     |     |     |  +-----------------------------------------------+
;  *    |     |     |     |     |     |     |     |
;  *    |     |     |     |     |     |     |     |     0     0     0     0     0     0     0
;  *    |     |     |     |     |     |     |  +-----+-----+-----+-----+-----+-----+-----+-----+
;  *    |     |     |     |     |     |     |  | Bit | Bit | Bit | Bit | Bit | Bit | Bit | Bit |
;  *    |     |     |     |     |     |     |  |  0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |
;  *    |     |     |     |     |     |     |  +-----------------------------------------------+
;  *    |     |     |     |     |     |     |  |            EAPI_ID_GPIO_GPIO07                |
;  *    |     |     |     |     |     |     |  +-----------------------------------------------+
;  *    |     |     |     |     |     |     |
;  *    |     |     |     |     |     |     |     0     0     0     0     0     0     0
;  *    |     |     |     |     |     |  +-----+-----+-----+-----+-----+-----+-----+-----+
;  *    |     |     |     |     |     |  | Bit | Bit | Bit | Bit | Bit | Bit | Bit | Bit |
;  *    |     |     |     |     |     |  |  0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |
;  *    |     |     |     |     |     |  +-----------------------------------------------+
;  *    |     |     |     |     |     |  |            EAPI_ID_GPIO_GPIO06                |
;  *    |     |     |     |     |     |  +-----------------------------------------------+
;  *    |     |     |     |     |     |
;  *    |     |     |     |     |     |     0     0     0     0     0     0     0
;  *    |     |     |     |     |  +-----+-----+-----+-----+-----+-----+-----+-----+
;  *    |     |     |     |     |  | Bit | Bit | Bit | Bit | Bit | Bit | Bit | Bit |
;  *    |     |     |     |     |  |  0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |
;  *    |     |     |     |     |  +-----------------------------------------------+
;  *    |     |     |     |     |  |            EAPI_ID_GPIO_GPIO05                |
;  *    |     |     |     |     |  +-----------------------------------------------+
;  *    |     |     |     |     |
;  *    |     |     |     |     |     0     0     0     0     0     0     0
;  *    |     |     |     |  +-----+-----+-----+-----+-----+-----+-----+-----+
;  *    |     |     |     |  | Bit | Bit | Bit | Bit | Bit | Bit | Bit | Bit |
;  *    |     |     |     |  |  0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |
;  *    |     |     |     |  +-----------------------------------------------+
;  *    |     |     |     |  |            EAPI_ID_GPIO_GPIO04                |
;  *    |     |     |     |  +-----------------------------------------------+
;  *    |     |     |     |
;  *    |     |     |     |     0     0     0     0     0     0     0
;  *    |     |     |  +-----+-----+-----+-----+-----+-----+-----+-----+
;  *    |     |     |  | Bit | Bit | Bit | Bit | Bit | Bit | Bit | Bit |
;  *    |     |     |  |  0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |
;  *    |     |     |  +-----------------------------------------------+
;  *    |     |     |  |            EAPI_ID_GPIO_GPIO03                |
;  *    |     |     |  +-----------------------------------------------+
;  *    |     |     |
;  *    |     |     |     0     0     0     0     0     0     0
;  *    |     |  +-----+-----+-----+-----+-----+-----+-----+-----+
;  *    |     |  | Bit | Bit | Bit | Bit | Bit | Bit | Bit | Bit |
;  *    |     |  |  0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |
;  *    |     |  +-----------------------------------------------+
;  *    |     |  |            EAPI_ID_GPIO_GPIO02                |
;  *    |     |  +-----------------------------------------------+
;  *    |     |
;  *    |     |     0     0     0     0     0     0     0
;  *    |  +-----+-----+-----+-----+-----+-----+-----+-----+
;  *    |  | Bit | Bit | Bit | Bit | Bit | Bit | Bit | Bit |
;  *    |  |  0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |
;  *    |  +-----------------------------------------------+
;  *    |  |            EAPI_ID_GPIO_GPIO01                |
;  *    |  +-----------------------------------------------+
;  *    |
;  *    |     0     0     0     0     0     0     0
;  * +-----+-----+-----+-----+-----+-----+-----+-----+
;  * | Bit | Bit | Bit | Bit | Bit | Bit | Bit | Bit |
;  * |  0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |
;  * +-----------------------------------------------+
;  * |            EAPI_ID_GPIO_GPIO00                |
;  * +-----------------------------------------------+
;  
;  IDs 
; 
;  * Individual ID Per GPIO Mapping
;  
EAPI_GPIO_GPIO_BITMASK		EQU		000000001h
;  (Optional) 
EAPI_ID_GPIO_GPIO00		EQU		000000000h
;  (Optional) 
EAPI_ID_GPIO_GPIO01		EQU		000000001h
;  (Optional) 
EAPI_ID_GPIO_GPIO02		EQU		000000002h
;  (Optional) 
EAPI_ID_GPIO_GPIO03		EQU		000000003h
; 
;  * Multiple GPIOs Per ID Mapping
;  
;  GPIOs  0 - 31 
;                                                             * (optional)
;                                                             
EAPI_ID_GPIO_BITMASK00		EQU		000010000h
;  GPIOs 32 - 63 
;                                                             * (optional)
;                                                             
EAPI_ID_GPIO_BITMASK01		EQU		000010001h
;  GPIOs 64 - 95
;                                                             * (optional)
;                                                             
EAPI_ID_GPIO_BITMASK02		EQU		000010002h
;  Bit mask Bit States 
EAPI_GPIO_BITMASK_SELECT		EQU		1t
EAPI_GPIO_BITMASK_NOSELECT		EQU		0t
;  Levels 
EAPI_GPIO_LOW		EQU		0t
EAPI_GPIO_HIGH		EQU		1t
;  Directions 
EAPI_GPIO_INPUT		EQU		1t
EAPI_GPIO_OUTPUT		EQU		0t
;  
;  * EApiGPIOGetDirectionCaps
;  * 
;  * Condition                          | Return Values 
;  * -----------------------------------+------------------------------
;  * Library Uninitialized              | EAPI_STATUS_NOT_INITIALIZED
;  * ((pOutputs==NULL)&&(pInputs==NULL))| EAPI_STATUS_INVALID_PARAMETER
;  * Unsupported Id                     | EAPI_STATUS_UNSUPPORTED
;  * Common Error                       | Common Error Code
;  * Else                               | EAPI_STATUS_SUCCESS
;  
;  GPIO Id 
;  Supported GPIO Input
;                                    * Bit Mask 
;                                    
;  Supported GPIO Output
;                                    * Bit Mask 
;                                    
@proto_20		TYPEDEF		PROTO STDCALL :uint32_t, :PTR uint32_t, :PTR uint32_t
EApiGPIOGetDirectionCaps		PROTO		@proto_20

;  
;  * EApiGPIOGetDirection
;  * 
;  * Condition                          | Return Values 
;  * -----------------------------------+------------------------------
;  * Library Uninitialized              | EAPI_STATUS_NOT_INITIALIZED
;  * Bitmask==0                         | EAPI_STATUS_INVALID_PARAMETER
;  * Unsupported Id                     | EAPI_STATUS_UNSUPPORTED
;  * (Bitmask&~(pInputs|pOutputs))      | EAPI_STATUS_INVALID_BITMASK
;  * Common Error                       | Common Error Code
;  * Else                               | EAPI_STATUS_SUCCESS
;  
;  GPIO Id 
;  Bit mask of Affected
;                                   * Bits 
;                                   
;  Current Direction 
@proto_21		TYPEDEF		PROTO STDCALL :uint32_t, :uint32_t, :PTR uint32_t
EApiGPIOGetDirection		PROTO		@proto_21

;  
;  * EApiGPIOSetDirection
;  * 
;  * Condition                          | Return Values 
;  * -----------------------------------+------------------------------
;  * Library Uninitialized              | EAPI_STATUS_NOT_INITIALIZED
;  * Bitmask==0                         | EAPI_STATUS_INVALID_PARAMETER
;  * Unsupported Id                     | EAPI_STATUS_UNSUPPORTED
;  * (Bitmask&~(pInputs|pOutputs))      | EAPI_STATUS_INVALID_BITMASK
;  * (Bitmask&pDirection)&~pInputs      | EAPI_STATUS_INVALID_DIRECTION
;  * (Bitmask&~pDirection)&~pOutputs    | EAPI_STATUS_INVALID_DIRECTION
;  * Common Error                       | Common Error Code
;  * Else                               | EAPI_STATUS_SUCCESS
;  
;  GPIO Id 
;  Bit mask of Affected 
;                                   * Bits 
;                                   
;  Direction 
@proto_22		TYPEDEF		PROTO STDCALL :uint32_t, :uint32_t, :uint32_t
EApiGPIOSetDirection		PROTO		@proto_22

;  
;  * EApiGPIOGetLevel
;  * 
;  * Condition                          | Return Values 
;  * -----------------------------------+------------------------------
;  * Library Uninitialized              | EAPI_STATUS_NOT_INITIALIZED
;  * Bitmask==0                         | EAPI_STATUS_INVALID_PARAMETER
;  * Unsupported Id                     | EAPI_STATUS_UNSUPPORTED
;  * (Bitmask&~(pInputs|pOutputs))      | EAPI_STATUS_INVALID_BITMASK
;  * Common Error                       | Common Error Code
;  * Else                               | EAPI_STATUS_SUCCESS
;  
;  GPIO Id 
;  Bit mask of Affected
;                                   * Bits 
;                                   
;  Current Level 
@proto_23		TYPEDEF		PROTO STDCALL :uint32_t, :uint32_t, :PTR uint32_t
EApiGPIOGetLevel		PROTO		@proto_23

;  
;  * EApiGPIOSetLevel
;  * 
;  * Condition                          | Return Values 
;  * -----------------------------------+------------------------------
;  * Library Uninitialized              | EAPI_STATUS_NOT_INITIALIZED
;  * Bitmask==0                         | EAPI_STATUS_INVALID_PARAMETER
;  * Unsupported Id                     | EAPI_STATUS_UNSUPPORTED
;  * Common Error                       | Common Error Code
;  * Else                               | EAPI_STATUS_SUCCESS
;  
;  GPIO Id 
;  Bit mask of Affected 
;                                   * Bits 
;                                   
;  Level 
@proto_24		TYPEDEF		PROTO STDCALL :uint32_t, :uint32_t, :uint32_t
EApiGPIOSetLevel		PROTO		@proto_24

;  COMExpress Platform Specific ID Mappings   
; Begin of file include\masm\..\EApiCOM0.h
; 
;  *<KHeader>
;  *+=========================================================================
;  *I               EApiDK Embedded Application Development Kit
;  *+=========================================================================
;  *I  $HeadURL: https://sourceforge.net/p/eapidk/code/HEAD/tree/trunk/include/masm/EApiConv.inc $
;  *+=========================================================================
;  *I   Copyright: Copyright (c) 2009, PICMG
;  *I      Author: John Kearney,                  John.Kearney@kontron.com
;  *I
;  *I     License: All rights reserved. This program and the accompanying 
;  *I              materials are licensed and made available under the 
;  *I              terms and conditions of the BSD License which 
;  *I              accompanies this distribution. The full text of the 
;  *I              license may be found at 
;  *I              http://opensource.org/licenses/bsd-license.php
;  *I              
;  *I              THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "
;  *I              AS IS" BASIS, WITHOUT WARRANTIES OR REPRESENTATIONS OF 
;  *I              ANY KIND, EITHER EXPRESS OR IMPLIED.
;  *I
;  *I Description: Auto Created for EApiCOM0.h
;  *I
;  *+------------------------------------------------------------------------=
;  *I
;  *I  File Name            : EApiCOM0.h
;  *I  File Location        : include
;  *I
;  *+=========================================================================
;  *</KHeader>
;  
_EAPICOM0_H_		TEXTEQU		<>
; 
;  *
;  *      B O A R D   I N F O M A T I O N   S T R I N G S 
;  *
;  
;  IDS 
EAPI_COM0_ID_BOARD_MANUFACTURER_STR		EQU		000000000h
EAPI_COM0_ID_BOARD_NAME_STR		EQU		000000001h
EAPI_COM0_ID_BOARD_SERIAL_STR		EQU		000000003h
EAPI_COM0_ID_BOARD_BIOS_REVISION_STR		EQU		000000004h
EAPI_COM0_ID_BOARD_PLATFORM_TYPE_STR		EQU		000000006h
EAPI_COM0_PLATFORM_STR		TEXTEQU		<"COMExpress">
; 
;  *
;  *      B O A R D   I N F O M A T I O N   V A L U E S
;  *
;  
;  IDS 
EAPI_COM0_ID_BOARD_BOOT_COUNTER_VAL		EQU		000000001h
EAPI_COM0_ID_BOARD_RUNNING_TIME_METER_VAL		EQU		000000002h
EAPI_COM0_ID_BOARD_PNPID_VAL		EQU		000000003h
EAPI_COM0_ID_BOARD_PLATFORM_REV_VAL		EQU		000000004h
EAPI_COM0_ID_BOARD_DRIVER_VERSION_VAL		EQU		000010000h
EAPI_COM0_ID_BOARD_LIB_VERSION_VAL		EQU		000010001h
EAPI_COM0_REV_1_0		EQU		001000000h
EAPI_COM0_REV_2_0		EQU		002000000h
EAPI_COM0_ID_HWMON_CPU_TEMP		EQU		000020000h
EAPI_COM0_ID_HWMON_CHIPSET_TEMP		EQU		000020001h
EAPI_COM0_ID_HWMON_SYSTEM_TEMP		EQU		000020002h
EAPI_COM0_ID_HWMON_VOLTAGE_VCORE		EQU		000021004h
EAPI_COM0_ID_HWMON_VOLTAGE_2V5		EQU		000021008h
EAPI_COM0_ID_HWMON_VOLTAGE_3V3		EQU		00002100ch
EAPI_COM0_ID_HWMON_VOLTAGE_VBAT		EQU		000021010h
EAPI_COM0_ID_HWMON_VOLTAGE_5V		EQU		000021014h
EAPI_COM0_ID_HWMON_VOLTAGE_5VSB		EQU		000021018h
EAPI_COM0_ID_HWMON_VOLTAGE_12V		EQU		00002101ch
EAPI_COM0_ID_HWMON_FAN_CPU		EQU		000022000h
EAPI_COM0_ID_HWMON_FAN_SYSTEM		EQU		000022001h
; 
;  *
;  *      B A C K L I G H T
;  *
;  
; 
;  * COM Express Backlight Fill Order
;  *
;  * Internal PWM
;  * EAPI_COM0_ID_I2C_LVDS_1 I2C Device
;  * SDVOB PWM
;  * SDVOB I2C Device
;  * SDVOC PWM
;  * SDVOC I2C Device
;  * DDI1 I2C Device
;  * DDI2 I2C Device
;  * DDI3 I2C Device
;  *
;  
;  IDS 
EAPI_COM0_ID_BACKLIGHT_1		EQU		000000000h
EAPI_COM0_ID_BACKLIGHT_2		EQU		000000001h
; 
;  *
;  *      S T O R A G E
;  *
;  
;  IDs 
EAPI_COM0_ID_STORAGE_STD		EQU		000000000h
; 
;  *
;  *      I 2 C
;  *
;  
;  IDs 
EAPI_COM0_ID_I2C_EXTERNAL		EQU		000000000h
EAPI_COM0_ID_I2C_LVDS_1		EQU		000000001h
EAPI_COM0_ID_I2C_LVDS_2		EQU		000000002h
; 
;  *
;  *      G P I O
;  *
;  
;  IDs 
EAPI_COM0_ID_GPIO_BITMASK		EQU		000010000h
EAPI_COM0_ID_GPIO_GPI0		EQU		000000000h
EAPI_COM0_ID_GPIO_GPI1		EQU		000000001h
EAPI_COM0_ID_GPIO_GPI2		EQU		000000002h
EAPI_COM0_ID_GPIO_GPI3		EQU		000000003h
EAPI_COM0_ID_GPIO_GPO0		EQU		000000004h
EAPI_COM0_ID_GPIO_GPO1		EQU		000000005h
EAPI_COM0_ID_GPIO_GPO2		EQU		000000006h
EAPI_COM0_ID_GPIO_GPO3		EQU		000000007h
;  _EAPICOM0_H_ 
; End of file include\masm\..\EApiCOM0.h
;  TCA Platform Specific ID Mappings          
; Begin of file include\masm\..\EApiTCA.h
; 
;  *<KHeader>
;  *+=========================================================================
;  *I               EApiDK Embedded Application Development Kit
;  *+=========================================================================
;  *I  $HeadURL: https://sourceforge.net/p/eapidk/code/HEAD/tree/trunk/include/masm/EApiConv.inc $
;  *+=========================================================================
;  *I   Copyright: Copyright (c) 2002-2009, Kontron Embedded Modules GmbH
;  *I      Author: John Kearney,                  John.Kearney@kontron.com
;  *I
;  *I     License: All rights reserved. This program and the accompanying 
;  *I              materials are licensed and made available under the 
;  *I              terms and conditions of the BSD License which 
;  *I              accompanies this distribution. The full text of the 
;  *I              license may be found at 
;  *I              http://opensource.org/licenses/bsd-license.php
;  *I              
;  *I              THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "
;  *I              AS IS" BASIS, WITHOUT WARRANTIES OR REPRESENTATIONS OF 
;  *I              ANY KIND, EITHER EXPRESS OR IMPLIED.
;  *I
;  *I Description: Auto Created for EApiTCA.h
;  *I
;  *+------------------------------------------------------------------------=
;  *I
;  *I  File Name            : EApiTCA.h
;  *I  File Location        : include
;  *I
;  *+=========================================================================
;  *</KHeader>
;  
_EAPICOM_H_		TEXTEQU		<>
; 
;  *
;  *      B O A R D   I N F O M A T I O N   S T R I N G S 
;  *
;  
;  IDS 
EAPI_TCA_ID_BOARD_MANUFACTURER_STR		EQU		000000000h
EAPI_TCA_ID_BOARD_NAME_STR		EQU		000000001h
EAPI_TCA_ID_BOARD_SERIAL_STR		EQU		000000003h
EAPI_TCA_ID_BOARD_BIOS_REVISION_STR		EQU		000000004h
EAPI_TCA_ID_BOARD_PLATFORM_TYPE_STR		EQU		000000006h
EAPI_TCA_PLATFORM_STR		TEXTEQU		<"TCA">
EAPI_ATCA_PLATFORM_STR		TEXTEQU		<"AdvancedTCA">
EAPI_MTCA_PLATFORM_STR		TEXTEQU		<"MicroTCA">
; 
;  *
;  *      B O A R D   I N F O M A T I O N   V A L U E S
;  *
;  
;  IDS 
EAPI_TCA_ID_BOARD_BOOT_COUNTER_VAL		EQU		000000001h
EAPI_TCA_ID_BOARD_RUNNING_TIME_METER_VAL		EQU		000000002h
EAPI_TCA_ID_BOARD_PNPID_VAL		EQU		000000003h
EAPI_TCA_ID_BOARD_PLATFORM_REV_VAL		EQU		000000004h
EAPI_TCA_ID_BOARD_DRIVER_VERSION_VAL		EQU		000010000h
EAPI_TCA_ID_BOARD_LIB_VERSION_VAL		EQU		000010001h
EAPI_TCA_REV_1_0		EQU		001000000h
EAPI_TCA_REV_2_0		EQU		002000000h
EAPI_TCA_ID_HWMON_CPU_TEMP		EQU		000020000h
EAPI_TCA_ID_HWMON_CHIPSET_TEMP		EQU		000020001h
EAPI_TCA_ID_HWMON_SYSTEM_TEMP		EQU		000020002h
EAPI_TCA_ID_HWMON_VOLTAGE_VCORE		EQU		000021004h
EAPI_TCA_ID_HWMON_VOLTAGE_2V5		EQU		000021008h
EAPI_TCA_ID_HWMON_VOLTAGE_3V3		EQU		00002100ch
EAPI_TCA_ID_HWMON_VOLTAGE_VBAT		EQU		000021010h
EAPI_TCA_ID_HWMON_VOLTAGE_5V		EQU		000021014h
EAPI_TCA_ID_HWMON_VOLTAGE_5VSB		EQU		000021018h
EAPI_TCA_ID_HWMON_VOLTAGE_12V		EQU		00002101ch
EAPI_TCA_ID_HWMON_FAN_CPU		EQU		000022000h
EAPI_TCA_ID_HWMON_FAN_SYSTEM		EQU		000022001h
; 
;  *
;  *      B A C K L I G H T
;  *
;  
;  IDS 
EAPI_TCA_ID_BACKLIGHT_1		EQU		000000000h
EAPI_TCA_ID_BACKLIGHT_2		EQU		000000001h
; 
;  *
;  *      S T O R A G E
;  *
;  
;  IDs 
EAPI_TCA_ID_STORAGE_STD		EQU		000000000h
; 
;  *
;  *      I 2 C
;  *
;  
;  IDs 
EAPI_TCA_ID_I2C_EXTERNAL		EQU		000000000h
EAPI_TCA_ID_I2C_LVDS_1		EQU		000000001h
EAPI_TCA_ID_I2C_LVDS_2		EQU		000000002h
; 
;  *
;  *      G P I O
;  *
;  
;  IDs 
; 
;  * No GPIOs Supported
;  *
;  
;  _EAPICOM_H_ 
; End of file include\masm\..\EApiTCA.h
;  ETX Platform Specific ID Mappings          
; Begin of file include\masm\..\EApiETX.h
; 
;  *<KHeader>
;  *+=========================================================================
;  *I               EApiDK Embedded Application Development Kit
;  *+=========================================================================
;  *I  $HeadURL: https://sourceforge.net/p/eapidk/code/HEAD/tree/trunk/include/masm/EApiConv.inc $
;  *+=========================================================================
;  *I   Copyright: Copyright (c) 2002-2009, Kontron Embedded Modules GmbH
;  *I      Author: John Kearney,                  John.Kearney@kontron.com
;  *I
;  *I     License: All rights reserved. This program and the accompanying 
;  *I              materials are licensed and made available under the 
;  *I              terms and conditions of the BSD License which 
;  *I              accompanies this distribution. The full text of the 
;  *I              license may be found at 
;  *I              http://opensource.org/licenses/bsd-license.php
;  *I              
;  *I              THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "
;  *I              AS IS" BASIS, WITHOUT WARRANTIES OR REPRESENTATIONS OF 
;  *I              ANY KIND, EITHER EXPRESS OR IMPLIED.
;  *I
;  *I Description: Auto Created for EApiETX.h
;  *I
;  *+------------------------------------------------------------------------=
;  *I
;  *I  File Name            : EApiETX.h
;  *I  File Location        : include
;  *I
;  *+=========================================================================
;  *</KHeader>
;  
_EAPIETX_H_		TEXTEQU		<>
; 
;  *
;  *      B O A R D   I N F O M A T I O N   S T R I N G S 
;  *
;  
;  IDS 
EAPI_ETX_ID_BOARD_MANUFACTURER_STR		EQU		000000000h
EAPI_ETX_ID_BOARD_NAME_STR		EQU		000000001h
EAPI_ETX_ID_BOARD_SERIAL_STR		EQU		000000003h
EAPI_ETX_ID_BOARD_BIOS_REVISION_STR		EQU		000000004h
EAPI_ETX_ID_BOARD_PLATFORM_TYPE_STR		EQU		000000006h
EAPI_ETX_PLATFORM_STR		TEXTEQU		<"ETX">
; 
;  *
;  *      B O A R D   I N F O M A T I O N   V A L U E S
;  *
;  
;  IDS 
EAPI_ETX_ID_BOARD_BOOT_COUNTER_VAL		EQU		000000001h
EAPI_ETX_ID_BOARD_RUNNING_TIME_METER_VAL		EQU		000000002h
EAPI_ETX_ID_BOARD_PNPID_VAL		EQU		000000003h
EAPI_ETX_ID_BOARD_PLATFORM_REV_VAL		EQU		000000004h
EAPI_ETX_ID_BOARD_DRIVER_VERSION_VAL		EQU		000010000h
EAPI_ETX_ID_BOARD_LIB_VERSION_VAL		EQU		000010001h
EAPI_ETX_REV_1_0		EQU		001000000h
EAPI_ETX_REV_2_0		EQU		002000000h
EAPI_ETX_ID_HWMON_CPU_TEMP		EQU		000020000h
EAPI_ETX_ID_HWMON_CHIPSET_TEMP		EQU		000020001h
EAPI_ETX_ID_HWMON_SYSTEM_TEMP		EQU		000020002h
EAPI_ETX_ID_HWMON_VOLTAGE_VCORE		EQU		000021004h
EAPI_ETX_ID_HWMON_VOLTAGE_2V5		EQU		000021008h
EAPI_ETX_ID_HWMON_VOLTAGE_3V3		EQU		00002100ch
EAPI_ETX_ID_HWMON_VOLTAGE_VBAT		EQU		000021010h
EAPI_ETX_ID_HWMON_VOLTAGE_5V		EQU		000021014h
EAPI_ETX_ID_HWMON_VOLTAGE_5VSB		EQU		000021018h
EAPI_ETX_ID_HWMON_VOLTAGE_12V		EQU		00002101ch
EAPI_ETX_ID_HWMON_FAN_CPU		EQU		000022000h
EAPI_ETX_ID_HWMON_FAN_SYSTEM		EQU		000022001h
; 
;  *
;  *      B A C K L I G H T
;  *
;  
;  IDS 
EAPI_ETX_ID_BACKLIGHT_1		EQU		000000000h
EAPI_ETX_ID_BACKLIGHT_2		EQU		000000001h
; 
;  *
;  *      S T O R A G E
;  *
;  
;  IDs 
EAPI_ETX_ID_STORAGE_STD		EQU		000000000h
; 
;  *
;  *      I 2 C
;  *
;  
;  IDs 
EAPI_ETX_ID_I2C_EXTERNAL		EQU		000000000h
EAPI_ETX_ID_I2C_LVDS_1		EQU		000000001h
EAPI_ETX_ID_I2C_LVDS_2		EQU		000000002h
; 
;  *
;  *      G P I O
;  *
;  
;  IDs 
; 
;  * No GPIOs Supported
;  *
;  
;  _EAPIETX_H_ 
; End of file include\masm\..\EApiETX.h
;  Q Seven Platform Specific ID Mappings      
; Begin of file include\masm\..\EApiQ7.h
; 
;  *<KHeader>
;  *+=========================================================================
;  *I               EApiDK Embedded Application Development Kit
;  *+=========================================================================
;  *I  $HeadURL: https://sourceforge.net/p/eapidk/code/HEAD/tree/trunk/include/masm/EApiConv.inc $
;  *+=========================================================================
;  *I   Copyright: Copyright (c) 2002-2009, Kontron Embedded Modules GmbH
;  *I      Author: John Kearney,                  John.Kearney@kontron.com
;  *I
;  *I     License: All rights reserved. This program and the accompanying 
;  *I              materials are licensed and made available under the 
;  *I              terms and conditions of the BSD License which 
;  *I              accompanies this distribution. The full text of the 
;  *I              license may be found at 
;  *I              http://opensource.org/licenses/bsd-license.php
;  *I              
;  *I              THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "
;  *I              AS IS" BASIS, WITHOUT WARRANTIES OR REPRESENTATIONS OF 
;  *I              ANY KIND, EITHER EXPRESS OR IMPLIED.
;  *I
;  *I Description: Auto Created for EApiQ7.h
;  *I
;  *+------------------------------------------------------------------------=
;  *I
;  *I  File Name            : EApiQ7.h
;  *I  File Location        : include
;  *I
;  *+=========================================================================
;  *</KHeader>
;  
_EAPIQ7_H_		TEXTEQU		<>
; 
;  *
;  *      B O A R D   I N F O M A T I O N   S T R I N G S 
;  *
;  
;  IDS 
EAPI_Q7_ID_BOARD_MANUFACTURER_STR		EQU		000000000h
EAPI_Q7_ID_BOARD_NAME_STR		EQU		000000001h
EAPI_Q7_ID_BOARD_SERIAL_STR		EQU		000000003h
EAPI_Q7_ID_BOARD_BIOS_REVISION_STR		EQU		000000004h
EAPI_Q7_ID_BOARD_PLATFORM_TYPE_STR		EQU		000000006h
EAPI_Q7_PLATFORM_STR		TEXTEQU		<"QSeven">
; 
;  *
;  *      B O A R D   I N F O M A T I O N   V A L U E S
;  *
;  
;  IDS 
EAPI_Q7_ID_BOARD_BOOT_COUNTER_VAL		EQU		000000001h
EAPI_Q7_ID_BOARD_RUNNING_TIME_METER_VAL		EQU		000000002h
EAPI_Q7_ID_BOARD_PNPID_VAL		EQU		000000003h
EAPI_Q7_ID_BOARD_PLATFORM_REV_VAL		EQU		000000004h
EAPI_Q7_ID_BOARD_DRIVER_VERSION_VAL		EQU		000010000h
EAPI_Q7_ID_BOARD_LIB_VERSION_VAL		EQU		000010001h
EAPI_Q7_REV_1_0		EQU		001000000h
EAPI_Q7_ID_HWMON_CPU_TEMP		EQU		000020000h
EAPI_Q7_ID_HWMON_CHIPSET_TEMP		EQU		000020001h
EAPI_Q7_ID_HWMON_SYSTEM_TEMP		EQU		000020002h
EAPI_Q7_ID_HWMON_VOLTAGE_VCORE		EQU		000021004h
EAPI_Q7_ID_HWMON_VOLTAGE_2V5		EQU		000021008h
EAPI_Q7_ID_HWMON_VOLTAGE_3V3		EQU		00002100ch
EAPI_Q7_ID_HWMON_VOLTAGE_VBAT		EQU		000021010h
EAPI_Q7_ID_HWMON_VOLTAGE_5V		EQU		000021014h
EAPI_Q7_ID_HWMON_VOLTAGE_5VSB		EQU		000021018h
EAPI_Q7_ID_HWMON_VOLTAGE_12V		EQU		00002101ch
EAPI_Q7_ID_HWMON_FAN_CPU		EQU		000022000h
EAPI_Q7_ID_HWMON_FAN_SYSTEM		EQU		000022001h
; 
;  *
;  *      B A C K L I G H T
;  *
;  
;  IDS 
EAPI_Q7_ID_BACKLIGHT_1		EQU		000000000h
EAPI_Q7_ID_BACKLIGHT_2		EQU		000000001h
; 
;  *
;  *      S T O R A G E
;  *
;  
;  IDs 
EAPI_Q7_ID_STORAGE_STD		EQU		000000000h
; 
;  *
;  *      I 2 C
;  *
;  
;  IDs 
EAPI_Q7_ID_I2C_EXTERNAL		EQU		000000000h
EAPI_Q7_ID_I2C_LVDS_1		EQU		000000001h
EAPI_Q7_ID_I2C_LVDS_2		EQU		000000002h
; 
;  *
;  *      G P I O
;  *
;  
;  IDs 
; 
;  * No GPIOs Supported
;  *
;  
;  _EAPIQ7_H_ 
; End of file include\masm\..\EApiQ7.h
;  MOPS Platform Specific ID Mappings         
; Begin of file include\masm\..\EApiMOPS.h
; 
;  *<KHeader>
;  *+=========================================================================
;  *I               EApiDK Embedded Application Development Kit
;  *+=========================================================================
;  *I  $HeadURL: https://sourceforge.net/p/eapidk/code/HEAD/tree/trunk/include/masm/EApiConv.inc $
;  *+=========================================================================
;  *I   Copyright: Copyright (c) 2002-2009, Kontron Embedded Modules GmbH
;  *I      Author: John Kearney,                  John.Kearney@kontron.com
;  *I
;  *I     License: All rights reserved. This program and the accompanying 
;  *I              materials are licensed and made available under the 
;  *I              terms and conditions of the BSD License which 
;  *I              accompanies this distribution. The full text of the 
;  *I              license may be found at 
;  *I              http://opensource.org/licenses/bsd-license.php
;  *I              
;  *I              THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "
;  *I              AS IS" BASIS, WITHOUT WARRANTIES OR REPRESENTATIONS OF 
;  *I              ANY KIND, EITHER EXPRESS OR IMPLIED.
;  *I
;  *I Description: Auto Created for EApiMOPS.h
;  *I
;  *+------------------------------------------------------------------------=
;  *I
;  *I  File Name            : EApiMOPS.h
;  *I  File Location        : include
;  *I
;  *+=========================================================================
;  *</KHeader>
;  
_EAPIMOPS_H_		TEXTEQU		<>
; 
;  *
;  *      B O A R D   I N F O M A T I O N   S T R I N G S 
;  *
;  
;  IDS 
EAPI_MOPS_ID_BOARD_MANUFACTURER_STR		EQU		000000000h
EAPI_MOPS_ID_BOARD_NAME_STR		EQU		000000001h
EAPI_MOPS_ID_BOARD_SERIAL_STR		EQU		000000003h
EAPI_MOPS_ID_BOARD_BIOS_REVISION_STR		EQU		000000004h
EAPI_MOPS_ID_BOARD_PLATFORM_TYPE_STR		EQU		000000006h
EAPI_MOPS_PLATFORM_STR		TEXTEQU		<"MOPS">
; 
;  *
;  *      B O A R D   I N F O M A T I O N   V A L U E S
;  *
;  
;  IDS 
EAPI_MOPS_ID_BOARD_BOOT_COUNTER_VAL		EQU		000000001h
EAPI_MOPS_ID_BOARD_RUNNING_TIME_METER_VAL		EQU		000000002h
EAPI_MOPS_ID_BOARD_PNPID_VAL		EQU		000000003h
EAPI_MOPS_ID_BOARD_PLATFORM_REV_VAL		EQU		000000004h
EAPI_MOPS_ID_BOARD_DRIVER_VERSION_VAL		EQU		000010000h
EAPI_MOPS_ID_BOARD_LIB_VERSION_VAL		EQU		000010001h
EAPI_MOPS_REV_1_0		EQU		001000000h
EAPI_MOPS_ID_HWMON_CPU_TEMP		EQU		000020000h
EAPI_MOPS_ID_HWMON_CHIPSET_TEMP		EQU		000020001h
EAPI_MOPS_ID_HWMON_SYSTEM_TEMP		EQU		000020002h
EAPI_MOPS_ID_HWMON_VOLTAGE_VCORE		EQU		000021004h
EAPI_MOPS_ID_HWMON_VOLTAGE_2V5		EQU		000021008h
EAPI_MOPS_ID_HWMON_VOLTAGE_3V3		EQU		00002100ch
EAPI_MOPS_ID_HWMON_VOLTAGE_VBAT		EQU		000021010h
EAPI_MOPS_ID_HWMON_VOLTAGE_5V		EQU		000021014h
EAPI_MOPS_ID_HWMON_VOLTAGE_5VSB		EQU		000021018h
EAPI_MOPS_ID_HWMON_VOLTAGE_12V		EQU		00002101ch
EAPI_MOPS_ID_HWMON_FAN_CPU		EQU		000022000h
EAPI_MOPS_ID_HWMON_FAN_SYSTEM		EQU		000022001h
; 
;  *
;  *      B A C K L I G H T
;  *
;  
;  IDS 
EAPI_MOPS_ID_BACKLIGHT_1		EQU		000000000h
EAPI_MOPS_ID_BACKLIGHT_2		EQU		000000001h
; 
;  *
;  *      S T O R A G E
;  *
;  
;  IDs 
EAPI_MOPS_ID_STORAGE_STD		EQU		000000000h
; 
;  *
;  *      I 2 C
;  *
;  
;  IDs 
EAPI_MOPS_ID_I2C_EXTERNAL		EQU		000000000h
EAPI_MOPS_ID_I2C_LVDS_1		EQU		000000001h
EAPI_MOPS_ID_I2C_LVDS_2		EQU		000000002h
; 
;  *
;  *      G P I O
;  *
;  
;  IDs 
; 
;  * No GPIOs Supported
;  *
;  
;  _EAPIMOPS_H_ 
; End of file include\masm\..\EApiMOPS.h
;  PISA Platform Specific ID Mappings         
; Begin of file include\masm\..\EApiPISA.h
; 
;  *<KHeader>
;  *+=========================================================================
;  *I               EApiDK Embedded Application Development Kit
;  *+=========================================================================
;  *I  $HeadURL: https://sourceforge.net/p/eapidk/code/HEAD/tree/trunk/include/masm/EApiConv.inc $
;  *+=========================================================================
;  *I   Copyright: Copyright (c) 2002-2009, Kontron Embedded Modules GmbH
;  *I      Author: John Kearney,                  John.Kearney@kontron.com
;  *I
;  *I     License: All rights reserved. This program and the accompanying 
;  *I              materials are licensed and made available under the 
;  *I              terms and conditions of the BSD License which 
;  *I              accompanies this distribution. The full text of the 
;  *I              license may be found at 
;  *I              http://opensource.org/licenses/bsd-license.php
;  *I              
;  *I              THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "
;  *I              AS IS" BASIS, WITHOUT WARRANTIES OR REPRESENTATIONS OF 
;  *I              ANY KIND, EITHER EXPRESS OR IMPLIED.
;  *I
;  *I Description: Auto Created for EApiPISA.h
;  *I
;  *+------------------------------------------------------------------------=
;  *I
;  *I  File Name            : EApiPISA.h
;  *I  File Location        : include
;  *I
;  *+=========================================================================
;  *</KHeader>
;  
_EAPIPISA_H_		TEXTEQU		<>
; 
;  *
;  *      B O A R D   I N F O M A T I O N   S T R I N G S 
;  *
;  
;  IDS 
EAPI_PISA_ID_BOARD_MANUFACTURER_STR		EQU		000000000h
EAPI_PISA_ID_BOARD_NAME_STR		EQU		000000001h
EAPI_PISA_ID_BOARD_SERIAL_STR		EQU		000000003h
EAPI_PISA_ID_BOARD_BIOS_REVISION_STR		EQU		000000004h
EAPI_PISA_ID_BOARD_PLATFORM_TYPE_STR		EQU		000000006h
EAPI_PISA_PLATFORM_STR		TEXTEQU		<"PISA">
; 
;  *
;  *      B O A R D   I N F O M A T I O N   V A L U E S
;  *
;  
;  IDS 
EAPI_PISA_ID_BOARD_BOOT_COUNTER_VAL		EQU		000000001h
EAPI_PISA_ID_BOARD_RUNNING_TIME_METER_VAL		EQU		000000002h
EAPI_PISA_ID_BOARD_PNPID_VAL		EQU		000000003h
EAPI_PISA_ID_BOARD_PLATFORM_REV_VAL		EQU		000000004h
EAPI_PISA_ID_BOARD_DRIVER_VERSION_VAL		EQU		000010000h
EAPI_PISA_ID_BOARD_LIB_VERSION_VAL		EQU		000010001h
EAPI_PISA_REV_1_0		EQU		001000000h
EAPI_PISA_ID_HWMON_CPU_TEMP		EQU		000020000h
EAPI_PISA_ID_HWMON_CHIPSET_TEMP		EQU		000020001h
EAPI_PISA_ID_HWMON_SYSTEM_TEMP		EQU		000020002h
EAPI_PISA_ID_HWMON_VOLTAGE_VCORE		EQU		000021004h
EAPI_PISA_ID_HWMON_VOLTAGE_2V5		EQU		000021008h
EAPI_PISA_ID_HWMON_VOLTAGE_3V3		EQU		00002100ch
EAPI_PISA_ID_HWMON_VOLTAGE_VBAT		EQU		000021010h
EAPI_PISA_ID_HWMON_VOLTAGE_5V		EQU		000021014h
EAPI_PISA_ID_HWMON_VOLTAGE_5VSB		EQU		000021018h
EAPI_PISA_ID_HWMON_VOLTAGE_12V		EQU		00002101ch
EAPI_PISA_ID_HWMON_FAN_CPU		EQU		000022000h
EAPI_PISA_ID_HWMON_FAN_SYSTEM		EQU		000022001h
; 
;  *
;  *      B A C K L I G H T
;  *
;  
;  IDS 
EAPI_PISA_ID_BACKLIGHT_1		EQU		000000000h
EAPI_PISA_ID_BACKLIGHT_2		EQU		000000001h
; 
;  *
;  *      S T O R A G E
;  *
;  
;  IDs 
EAPI_PISA_ID_STORAGE_STD		EQU		000000000h
; 
;  *
;  *      I 2 C
;  *
;  
;  IDs 
EAPI_PISA_ID_I2C_EXTERNAL		EQU		000000000h
EAPI_PISA_ID_I2C_LVDS_1		EQU		000000001h
EAPI_PISA_ID_I2C_LVDS_2		EQU		000000002h
; 
;  *
;  *      G P I O
;  *
;  
;  IDs 
; 
;  * No GPIOs Supported
;  *
;  
;  _EAPIPISA_H_ 
; End of file include\masm\..\EApiPISA.h
;  EPIC Platform Specific ID Mappings         
; Begin of file include\masm\..\EApiEPIC.h
; 
;  *<KHeader>
;  *+=========================================================================
;  *I               EApiDK Embedded Application Development Kit
;  *+=========================================================================
;  *I  $HeadURL: https://sourceforge.net/p/eapidk/code/HEAD/tree/trunk/include/masm/EApiConv.inc $
;  *+=========================================================================
;  *I   Copyright: Copyright (c) 2002-2009, Kontron Embedded Modules GmbH
;  *I      Author: John Kearney,                  John.Kearney@kontron.com
;  *I
;  *I     License: All rights reserved. This program and the accompanying 
;  *I              materials are licensed and made available under the 
;  *I              terms and conditions of the BSD License which 
;  *I              accompanies this distribution. The full text of the 
;  *I              license may be found at 
;  *I              http://opensource.org/licenses/bsd-license.php
;  *I              
;  *I              THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "
;  *I              AS IS" BASIS, WITHOUT WARRANTIES OR REPRESENTATIONS OF 
;  *I              ANY KIND, EITHER EXPRESS OR IMPLIED.
;  *I
;  *I Description: Auto Created for EApiEPIC.h
;  *I
;  *+------------------------------------------------------------------------=
;  *I
;  *I  File Name            : EApiEPIC.h
;  *I  File Location        : include
;  *I
;  *+=========================================================================
;  *</KHeader>
;  
_EAPIEPIC_H_		TEXTEQU		<>
; 
;  *
;  *      B O A R D   I N F O M A T I O N   S T R I N G S 
;  *
;  
;  IDS 
EAPI_EPIC_ID_BOARD_MANUFACTURER_STR		EQU		000000000h
EAPI_EPIC_ID_BOARD_NAME_STR		EQU		000000001h
EAPI_EPIC_ID_BOARD_SERIAL_STR		EQU		000000003h
EAPI_EPIC_ID_BOARD_BIOS_REVISION_STR		EQU		000000004h
EAPI_EPIC_ID_BOARD_PLATFORM_TYPE_STR		EQU		000000006h
EAPI_EPIC_PLATFORM_STR		TEXTEQU		<"EPIC">
; 
;  *
;  *      B O A R D   I N F O M A T I O N   V A L U E S
;  *
;  
;  IDS 
EAPI_EPIC_ID_BOARD_BOOT_COUNTER_VAL		EQU		000000001h
EAPI_EPIC_ID_BOARD_RUNNING_TIME_METER_VAL		EQU		000000002h
EAPI_EPIC_ID_BOARD_PNPID_VAL		EQU		000000003h
EAPI_EPIC_ID_BOARD_PLATFORM_REV_VAL		EQU		000000004h
EAPI_EPIC_ID_BOARD_DRIVER_VERSION_VAL		EQU		000010000h
EAPI_EPIC_ID_BOARD_LIB_VERSION_VAL		EQU		000010001h
EAPI_EPIC_REV_1_0		EQU		001000000h
EAPI_EPIC_ID_HWMON_CPU_TEMP		EQU		000020000h
EAPI_EPIC_ID_HWMON_CHIPSET_TEMP		EQU		000020001h
EAPI_EPIC_ID_HWMON_SYSTEM_TEMP		EQU		000020002h
EAPI_EPIC_ID_HWMON_VOLTAGE_VCORE		EQU		000021004h
EAPI_EPIC_ID_HWMON_VOLTAGE_2V5		EQU		000021008h
EAPI_EPIC_ID_HWMON_VOLTAGE_3V3		EQU		00002100ch
EAPI_EPIC_ID_HWMON_VOLTAGE_VBAT		EQU		000021010h
EAPI_EPIC_ID_HWMON_VOLTAGE_5V		EQU		000021014h
EAPI_EPIC_ID_HWMON_VOLTAGE_5VSB		EQU		000021018h
EAPI_EPIC_ID_HWMON_VOLTAGE_12V		EQU		00002101ch
EAPI_EPIC_ID_HWMON_FAN_CPU		EQU		000022000h
EAPI_EPIC_ID_HWMON_FAN_SYSTEM		EQU		000022001h
; 
;  *
;  *      B A C K L I G H T
;  *
;  
; 
;  * COM Express Backlight Fill Order
;  *
;  * Internal PWM
;  * EAPI_COM0_ID_I2C_LVDS_1 I2C Device
;  * SDVOB PWM
;  * SDVOB I2C Device
;  * SDVOC PWM
;  * SDVOC I2C Device
;  * DDI1 I2C Device
;  * DDI2 I2C Device
;  * DDI3 I2C Device
;  *
;  
;  IDS 
EAPI_EPIC_ID_BACKLIGHT_1		EQU		000000000h
EAPI_EPIC_ID_BACKLIGHT_2		EQU		000000001h
; 
;  *
;  *      S T O R A G E
;  *
;  
;  IDs 
EAPI_EPIC_ID_STORAGE_STD		EQU		000000000h
; 
;  *
;  *      I 2 C
;  *
;  
;  IDs 
EAPI_EPIC_ID_I2C_EXTERNAL		EQU		000000000h
EAPI_EPIC_ID_I2C_LVDS_1		EQU		000000001h
EAPI_EPIC_ID_I2C_LVDS_2		EQU		000000002h
; 
;  *
;  *      G P I O
;  *
;  
;  IDs 
EAPI_EPIC_ID_GPIO_BITMASK		EQU		000010000h
EAPI_EPIC_ID_GPIO_GPIO0		EQU		000000000h
EAPI_EPIC_ID_GPIO_GPIO1		EQU		000000001h
EAPI_EPIC_ID_GPIO_GPIO2		EQU		000000002h
EAPI_EPIC_ID_GPIO_GPIO3		EQU		000000003h
EAPI_EPIC_ID_GPIO_GPIO4		EQU		000000004h
EAPI_EPIC_ID_GPIO_GPIO5		EQU		000000005h
EAPI_EPIC_ID_GPIO_GPIO6		EQU		000000006h
EAPI_EPIC_ID_GPIO_GPIO7		EQU		000000007h
;  _EAPIEPIC_H_ 
; End of file include\masm\..\EApiEPIC.h
; **************************************************************************
;  _EAPI_H_ 
; End of file include\masm\..\EApi.h
; End of file include\masm\EApiConv.h
