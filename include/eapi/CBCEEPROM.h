/*
 *<KHeader>
 *+=========================================================================
 *I  Project Name: EApiDK Embedded Application Development Kit
 *+=========================================================================
 *I  $HeadURL: https://sourceforge.net/p/eapidk/code/HEAD/tree/trunk/include/CBCEEPROM.h $
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
 *I              THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN 
 *I              "AS IS" BASIS, WITHOUT WARRANTIES OR REPRESENTATIONS OF 
 *I              ANY KIND, EITHER EXPRESS OR IMPLIED.
 *I
 *I Description: Auto Created for CBCEEPROM.h
 *I
 *+-------------------------------------------------------------------------
 *I
 *I  File Name            : CBCEEPROM.h
 *I  File Location        : include
 *I
 *+=========================================================================
 *</KHeader>
 */
/* Structures for COM0 CBCEEP */
#ifndef _CBCEEPROM_H_    
#define _CBCEEPROM_H_

typedef union COM0R10_LaneDescData_u{
  uint32_t D[1]
  uint16_t W[2]
  uint8_t  B[4]
  struct LaneDescData_s{
    uint8_t  LinkSLN        ; /* 0x00 Descriptor bits 7..0 
                               *      describe the starting 
                               *      lane number of the link. 
                               */
    uint8_t  LinkWidth      ; /* 0x01 Descriptor bits 15..8 
                               *      describe the width of the PCI
                               *      Express link.
                               */
    uint16_t reserved0      ; /* 0x02 Reserved */
  }S;
}COM0R10_LaneDescData_t;

typedef union COM0R10_BB_u{
  uint32_t D[0x100/sizeof(uint32_t)];
  uint16_t W[0x100/sizeof(uint16_t)];
  uint8_t  B[0x100/sizeof(uint8_t)];
  struct COM0R10_BB_s{
    COM0R10_LaneDescData_t LDD[32]       ; /* 0x00 */       
    uint8_t        Reserved1[0xD8 - 0x80]; /* 0x80 Reserved */
    uint8_t        MiscIo1               ; /* 0xD8 Miscellaneous I/O Descriptor Byte 1 */
    uint8_t        LanDesc               ; /* 0xD9 LAN Descriptor Byte */
    uint8_t        DispDesc              ; /* 0xDA Display Descriptor Byte */
    uint8_t        UsbDesc               ; /* 0xDB USB Descriptor Byte */
    uint8_t        ExpCardDesc0          ; /* 0xDC Express Card 0 Descriptor Byte */
    uint8_t        ExpCardDesc1          ; /* 0xDD Express Card 1 Descriptor Byte */
    uint8_t        SasDevDesc            ; /* 0xDE SAS Device Descriptor Byte */
    uint8_t        FruRevision           ; /* 0xDF FRU Revision: fill with 00 for R1.0 */
    uint8_t        Identifier[0x10]      ; /* 0xE0 ASCII ID string: "COMExpressConfig" */
    uint8_t        Reserved2[0xFE - 0xF0]; /* 0xF0 Reserved */
    uint8_t        Checksum              ; /* 0xFE Unsigned 16 bit checksum over the 
                                            * unsigned bytes at addresses 
                                            * 0x00 through 0xFD
                                            */
  }S;
}COM0R10_BB_t;
/*
 * BitMask Macros for 
 * SATA / SAS Device Descriptor Byte
 */
#define COM0R10_SAS_CONNECTOR_PRESENT COM0_UINT8_C(1<<0)
#define COM0R10_SAS_DEVICE            COM0_UINT8_C(1<<1)
/*
 * BitMask Macros for 
 * Express Card Descriptor Byte
 */
#define COM0R10_EXPC_PRESENT          COM0_UINT8_C(1<<7)
#define COM0R10_EXPC_PMAP_MASK        COM0_UINT8_C(0x07)
#define COM0R10_EXPC_PMAP_OFFSET      COM0_UINT8_C(0x04)
#define COM0R10_EXPC_ELANE_MASK       COM0_UINT8_C(0x07)
#define COM0R10_EXPC_ELANE_OFFSET     COM0_UINT8_C(0x00)
/*
 * BitMask Macros for 
 * USB Descriptor Byte
 */
#define COM0R10_USB_PCNT_MASK         COM0_UINT8_C(0x0F)
#define COM0R10_USB_PCNT_OFFSET       COM0_UINT8_C(0x00)
/*
 * BitMask Macros for 
 * Display Descriptor Byte
 */
#define COM0R10_TV_SVID_PRESENT       COM0_UINT8_C(1<<6)
#define COM0R10_TV_OUT_COMPOS_PRESENT COM0_UINT8_C(1<<5)
#define COM0R10_VGA_PRESENT           COM0_UINT8_C(1<<4)
#define COM0R10_LVDSCB_PRESENT        COM0_UINT8_C(1<<3)
#define COM0R10_LVDSCA_PRESENT        COM0_UINT8_C(1<<2)
#define COM0R10_SDVOCC_PRESENT        COM0_UINT8_C(1<<1)
#define COM0R10_SDVOCB_PRESENT        COM0_UINT8_C(1<<0)
/*
 * BitMask Macros for 
 * LAN Descriptor Byte
 */
#define COM0R10_GBE0_PRESENT        COM0_UINT8_C(1<<0)
#define COM0R10_GBE1_PRESENT        COM0_UINT8_C(1<<1)
#define COM0R10_GBE2_PRESENT        COM0_UINT8_C(1<<2)
#define COM0R10_GB1E0_PRESENT       COM0_UINT8_C(1<<3)
/*
 * BitMask Macros for 
 * Miscellaneous I/O Descriptor Byte 1
 */
#define COM0R10_WAKE0_PRESENT        COM0_UINT8_C(1<<0)
#define COM0R10_WAKE1_PRESENT        COM0_UINT8_C(1<<1)
#define COM0R10_SUS_PRESENT          COM0_UINT8_C(1<<2)
#define COM0R10_BATLOW_PRESENT       COM0_UINT8_C(1<<3)
#define COM0R10_THRMP_PRESENT        COM0_UINT8_C(1<<4)
#define COM0R10_EBROM_PRESENT        COM0_UINT8_C(1<<5)
#define COM0R10_WDT_PRESENT          COM0_UINT8_C(1<<6)
#define COM0R10_AC97_PRESENT         COM0_UINT8_C(1<<7)

#endif /* _CBCEEPROM_H_ */

