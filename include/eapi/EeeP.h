/*
 *<KHeader>
 *+=========================================================================
 *I  Project Name: EApiDK Embedded Application Development Kit
 *+=========================================================================
 *I  $HeadURL: https://sourceforge.net/p/eapidk/code/HEAD/tree/trunk/include/EeeP.h $
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
 *I              THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN 
 *I              "AS IS" BASIS, WITHOUT WARRANTIES OR REPRESENTATIONS OF 
 *I              ANY KIND, EITHER EXPRESS OR IMPLIED.
 *I
 *I Description: Embedded EEPROM
 *I
 *+-------------------------------------------------------------------------
 *I
 *I  File Name            : EeeP.h
 *I  File Location        : include
 *I
 *+=========================================================================
 *</KHeader>
 */

/* Structures for Embedded EEPROM */
#ifndef __EEEP_H__    
#define __EEEP_H__

#ifndef EEEP_UINT32_C
#  define EEEP_UINT8_C(x)  ((uint8_t)(x))
#  define EEEP_UINT16_C(x) ((uint16_t)(x))
#  define EEEP_UINT32_C(x) ((uint32_t)(x))
#  define EEEP_LO_UINT8(x)  EEEP_UINT8_C( (x)&UINT8_MAX )
#  define EEEP_LO_UINT16(x) EEEP_UINT16_C((x)&UINT16_MAX)
#  define EEEP_LO_UINT32(x) EEEP_UINT32_C((x)&UINT32_MAX)
#endif

#pragma pack(push, 1)  /* push current alignment to stack */
#pragma pack(1)     /* set alignment to 1 byte boundary */


/* 
 * Detecting EeeP EEPROM
 *
 * High Level Check
 * if(!memcmp(
 *        &EeePEEP[0x01]            , 
 *        "3P"                      , 
 *        0x02
 *      )
 *   )
 * {
 *    // Found EeeP EEPROM
 * }
 *
 * Sample I2C Transfers
 *
 *  Device Address : 0xAE(0x57)
 *  Index Type     : Standard
 *    Start<0x57><W>Ack<0x00>Ack
 *    Start<0x57><R>Ack<0x00>Ack<'3'>Ack<'P'>Ack<0x10>Nak Stop
 *
 *  Device Address : 0xAE(0x57)
 *  Index Type     : Extended
 *    Start<0x57><W>Ack<0x00>Ack<0x00>Ack
 *    Start<0x57><R>Ack<0x00>Ack<'3'>Ack<'P'>Ack<0x10>Nak Stop
 * 
 */



#define EEEP_VER_MASK_VER     EEEP_UINT8_C(0xF0)
#define EEEP_VER_MASK_REV     EEEP_UINT8_C(0x0F)
#define EEEP_VER_GET_VER(x)   EEEP_UINT8_C (((x)>>4)&0x0F )
#define EEEP_VER_GET_REV(x)   EEEP_UINT8_C (((x)>>0)&0x0F )
#define EEEP_VER_CREATE(Version,Revision) (\
                      EEEP_UINT8_C(\
                        (((Version )&0x0F )<<4)|\
                        (((Revision)&0x0F )<<0) \
                      )\
                    )

/* EeeP Standard Revision */
#define EEEP_VER      0
#define EEEP_REVISION 5
#define EEEP_VERSION EEEP_VER_CREATE(EEEP_VER, EEEP_REVISION)




/*
 * EEPROM Common Header
 *
 */
typedef struct EeePCmn_s{
    uint8_t     DontCareByte;/* 0x00 Don't Care Byte 
                              *      The purpose of this
                              *      Byte is to reduce the Damage
                              *      Extended Index read access is
                              *      used on a Standard Index Device
                              *      
                              */
    uint8_t     EepId[2]   ; /* 0x01 3P   */
#       define  EEEP_EEPROM_MARKER    "3P"
    uint8_t     SpecRev    ; /* 0x03 EeeP Specification Revision
                              *      +=======+==================+
                              *      | Bits  | Descriptions     |
                              *      +=======+==================+
                              *      | 0 - 3 | Revision         |
                              *      +-------+------------------+
                              *      | 4 - 7 | Version          |
                              *      +=======+==================+
                              */

    uint8_t     BlkOffset  ; /* 0x04 Absolute Offset to
                              *      First Dynamic Block
                              *      in words(2 bytes)
                              */
    uint8_t     DeviceDesc ; /* 0x05 Device Descriptor
                              * +========+========================+
                              * | Bits   | Description            |
                              * +========+========================+
                              * | 0 - 3  | Size 2^(8+n) Bytes     |
                              * |        | (256 << n) Bytes       |
                              * |        |                        |
                              * |        | Addressable Bytes      |
                              * |        |   Standard Index       |
                              * |        |     8bit - 11Bit       |
                              * |        |   Extended Index       |
                              * |        |     16bit - 19Bit      |
                              * |        | 2048 KBit - 4096 MBit  |
                              * |        | 256 Bytes - 512 KBytes |
                              * |        |                        |
                              * +--------+------------------------+
                              * | 4      | Std/Ext Index          |
                              * +--------+------------------------+
                              * | 5 - 7  | Page Write Length/     |
                              * |        | Alignment              |
                              * |        | 0 = 1 Byte             |
                              * |        | 1 = 8 Byte             |
                              * |        | 2 = 16 Byte            |
                              * |        | 3 = 32 Byte            |
                              * |        | 4 = 64 Byte            |
                              * |        | 5 = 128 Byte           |
                              * |        | 6 = 256 Byte           |
                              * |        | 7 = 512 Byte           |
                              * +========+========================+
                              */
#define EEEP_DEVICE_SIZE_MASK           0x0F
#define EEEP_DEVICE_TYPE_OFFSET         4
#define EEEP_DEVICE_EXT_INDEX           (1 << EEEP_DEVICE_TYPE_OFFSET)
#define EEEP_DEVICE_WRITE_LEN_OFFSET     5
#define EEEP_DEVICE_WRITE_LEN_MASK      (0x7 << EEEP_DEVICE_WRITE_LEN_OFFSET)
}EeePCmn_t;


/*
 * EeeP    
 * Universal Device Identifier
 *
 * Standardized Compact
 * Mechanism to Uniquely 
 * Identify Device Revisions
 *
 */
typedef struct EeePUDId_s{
    uint8_t     VendId[2]  ; /* 0x00 Vendor Unique PNPID         */
    uint8_t     DeviceId[2]; /* 0x02 Vendor Specific Device ID   */
    uint8_t     DeviceFlav ; /* 0x04 Device Specific Flavor ID   */
    uint8_t     RevId      ; /* 0x05 Device Specific Revision ID */
}EeePUDId_t; 

/*
 * EeeP
 * Expansion EEPROM Header
 *
 */
typedef struct Exp_EEP_s{
    EeePCmn_t   EeePHdr  ; /* 0x00 EeeP Common Header */
    uint8_t     GenId[4] ; /* 0x06 "EXP1"   */
    #define EEEP_EXP_HEADER_ID "EXP1"
    EeePUDId_t  DevId    ; /* 0x06 Unique Device Id   */
}Exp_EEP_t;




/*
 * Dynamic Block Common Header
 *
 */
typedef struct DBlockIdHdr_s{
    uint8_t   DBlockId       ; /* 0x00 Block Id       */
    uint8_t   DBlockLength[2]; /* 0x01 Block Length/
                                *      Offset to next Block
                                *      in words (2 Bytes)
                                */
}DBlockIdHdr_t;

/*
 *
 *
 *
 * Block Types
 *
 *
 *
 */

/*
 * Block Ids
 *
 */
#define EEEP_BLOCK_ID_UNUSED            EEEP_UINT8_C(0x00)
/*
 * 0xC0 -0xCF See Platform Specific Headers
 *
 */
#define EEEP_BLOCK_ID_VENDOR_SPECIFIC   EEEP_UINT8_C(0xF0)
#define EEEP_BLOCK_ID_EXP_EEPROM        EEEP_UINT8_C(0xF1)
/*
 * 0xE0 -0xEF See Platform Specific Headers
 *
 */
#define EEEP_BLOCK_ID_SMBIOS            EEEP_UINT8_C(0xD0)
#define EEEP_BLOCK_ID_LFP               EEEP_UINT8_C(0xD1)
#define EEEP_BLOCK_ID_CRC_CHK           EEEP_UINT8_C(0xF2)
#define EEEP_BLOCK_ID_IGNORE            EEEP_UINT8_C(0xFF)

#define EEEP_OFFSET_VALUE_EOL           EEEP_UINT16_C(0x0000)
#define EEEP_OFFSET_VALUE_EOL_ALT       EEEP_UINT16_C(0xFFFF)

/*
 * CRC Checksum Block
 *
 */
typedef struct EeePCRC16ChkBlock_s{
    DBlockIdHdr_t CDBHdr        ; /* 0x00 Dynamic Block Header  */
    uint8_t       CrC16[2]      ; /* 0x03 CRC16 Checksum */
    uint8_t       Reserved0     ; /* 0x05 Alignment Byte */
}EeePCRC16ChkBlock_t;


typedef union EeePuint16u_u{
  uint8_t  b[2];
  uint16_t n;     /* Native Architecture Access */
}EeePuint16u_t;
typedef union EeePuint32u_u{
  uint8_t  b[4];
  uint32_t n;     /* Native Architecture Access */
}EeePuint32u_t;
typedef union EeePuint64u_u{
  uint8_t  b[8];
  uint64_t n;     /* Native Architecture Access */
}EeePuint64u_t;
typedef struct EeePEeePGUID_s{
  EeePuint32u_t  a;
  EeePuint16u_t  b[2];
  uint8_t        c[8];
}EeePGUID_t;

typedef EeePuint16u_t SmbiosHandle_t;
/*
 *      DMI/SMBIOS Common Header Format
 *
 *  see http://www.dmtf.org/standards/published_documents/DSP0134_2.6.1.pdf
 */
enum SMBIOS_BlockId_e{
  SMBIOS_TypeBIOS_INFORMATION                               =   0,
  SMBIOS_TypeSYSTEM_INFORMATION                             =   1,
  SMBIOS_TypeBASE_BOARD_MODULE_INFORMATION                  =   2,
  SMBIOS_TypeSYSTEM_ENCLOSURE_OR_CHASSIS                    =   3,
  SMBIOS_TypePROCESSOR_INFORMATION                          =   4,
  SMBIOS_TypeMEMORY_CONTROLLER_INFORMATION                  =   5,
  SMBIOS_TypeMEMORY_MODULE_INFORMATION                      =   6,
  SMBIOS_TypeCACHE_INFORMATION                              =   7,
  SMBIOS_TypePORT_CONNECTOR_INFORMATION                     =   8,
  SMBIOS_TypeSYSTEM_SLOTS                                   =   9,
  SMBIOS_TypeON_BOARD_DEVICES_INFORMATION                   =  10,
  SMBIOS_TypeOEM_STRINGS                                    =  11,
  SMBIOS_TypeSYSTEM_CONFIGURATION_OPTIONS                   =  12,
  SMBIOS_TypeBIOS_LANGUAGE_INFORMATION                      =  13,
  SMBIOS_TypeGROUP_ASSOCIATIONS                             =  14,
  SMBIOS_TypeSYSTEM_EVENT_LOG                               =  15,
  SMBIOS_TypePHYSICAL_MEMORY_ARRAY                          =  16,
  SMBIOS_TypeMEMORY_DEVICE                                  =  17,
  SMBIOS_Type32_BIT_MEMORY_ERROR_INFORMATION                =  18,
  SMBIOS_TypeMEMORY_ARRAY_MAPPED_ADDRESS                    =  19,
  SMBIOS_TypeMEMORY_DEVICE_MAPPED_ADDRESS                   =  20,
  SMBIOS_TypeBUILT_IN_POINTING_DEVICE                       =  21,
  SMBIOS_TypePORTABLE_BATTERY                               =  22,
  SMBIOS_TypeSYSTEM_RESET                                   =  23,
  SMBIOS_TypeHARDWARE_SECURITY                              =  24,
  SMBIOS_TypeSYSTEM_POWER_CONTROLS                          =  25,
  SMBIOS_TypeVOLTAGE_PROBE                                  =  26,
  SMBIOS_TypeCOOLING_DEVICE                                 =  27,
  SMBIOS_TypeTEMPERATURE_PROBE                              =  28,
  SMBIOS_TypeELECTRICAL_CURRENT_PROBE                       =  29,
  SMBIOS_TypeOUT_OF_BAND_REMOTE_ACCESS                      =  30,
  SMBIOS_TypeBOOT_INTEGRITY_SERVICES_ENTRY_POINT            =  31,
  SMBIOS_TypeSYSTEM_BOOT_INFORMATION                        =  32,
  SMBIOS_Type64_BIT_MEMORY_ERROR_INFORMATION                =  33,
  SMBIOS_TypeMANAGEMENT_DEVICE                              =  34,
  SMBIOS_TypeMANAGEMENT_DEVICE_COMPONENT                    =  35,
  SMBIOS_TypeMANAGEMENT_DEVICE_THRESHOLD_DATA               =  36,
  SMBIOS_TypeMEMORY_CHANNEL                                 =  37,
  SMBIOS_TypeIPMI_DEVICE_INFORMATION                        =  38,
  SMBIOS_TypeSYSTEM_POWER_SUPPLY                            =  39,
  SMBIOS_TypeINACTIVE                                       = 126,
  SMBIOS_TypeEND_OF_TABLE                                   = 127
};
typedef uint8_t SMBIOS_BlockId_t;

typedef struct EeePSmbiosHdr_s{
    DBlockIdHdr_t    CDBHdr     ; /* 0x00 Dynamic Block Header  */
    SMBIOS_BlockId_t Type       ; /* 0x03 Structure Type        */
    uint8_t          Length     ; /* 0x04 Specifies the length of
                                   * the formatted area of the 
                                   * structure, starting at the 
                                   * Type field. The length of the 
                                   * structure's string-set is not
                                   * included. 
                                   */
    SmbiosHandle_t   Handle     ; /* 0x05 Specifies the structure's 
                                   * handle, a unique 16-bit number 
                                   * in the range 0 to 0FEFFh 
                                   */
}EeePSmbiosHdr_t;
/*
 *      System Information
 *
 *  see http://www.dmtf.org/standards/published_documents/DSP0134_2.6.1.pdf
 *  Type 1 Block
 */
typedef struct EeePSystemInfo_s{
    EeePSmbiosHdr_t SDBHdr; /* 0x00 Smbios Dynamic Block Header */
    uint8_t Manufacturer   ; /* 0x06 Number of ASCIIZ String */
    uint8_t ProductName    ; /* 0x07 Number of ASCIIZ String */
    uint8_t Version        ; /* 0x08 Number of ASCIIZ String */
    uint8_t SerialNumber   ; /* 0x09 Number of ASCIIZ String */
    EeePGUID_t UUID        ; /* 0x0A Universal Unique ID number. */
    uint8_t Reserved0      ; /* 0x1A Reserved Set to Zero    */
    uint8_t SKU_Number     ; /* 0x1B Number of ASCIIZ String */
    uint8_t Family         ; /* 0x1C Number of ASCIIZ String */
}EeePSystemInfo_t;

/*
 *     Base Board (or Module) Information
 *
 *  see http://www.dmtf.org/standards/published_documents/DSP0134_2.6.1.pdf
 *  Type 2 Block
 */
enum SMBIOS_BoardTypes_e{
  SMBIOS_BoardType_Unknown=0,
  SMBIOS_BoardType_Other,
  SMBIOS_BoardType_ServerBlade,
  SMBIOS_BoardType_ConnectivitySwitch,
  SMBIOS_BoardType_SystemManagementModule,
  SMBIOS_BoardType_ProcessorModule,
  SMBIOS_BoardType_IO_Module,
  SMBIOS_BoardType_Memory_Module,
  SMBIOS_BoardType_DaughterBoard,
  SMBIOS_BoardType_Motherboard,
  SMBIOS_BoardType_ProcessorMemory_Module,
  SMBIOS_BoardType_Interconnect_Board
};
typedef uint8_t SMBIOS_BoardTypes_t;
typedef struct EeePModuleInfo_s{
    EeePSmbiosHdr_t SDBHdr; /* 0x00 Smbios Dynamic Block Header */
    uint8_t Manufacturer   ; /* 0x06 Number of ASCIIZ String */
    uint8_t Product        ; /* 0x07 Number of ASCIIZ String */
    uint8_t Version        ; /* 0x08 Number of ASCIIZ String */
    uint8_t SerialNumber   ; /* 0x09 Number of ASCIIZ String */
    uint8_t AssetTag       ; /* 0x0A Number of ASCIIZ String */
    uint8_t FeatureFlag    ; /* 0x0B A collection of 
                              *      flags that identify 
                              *      features of this 
                              *      baseboard.
                              *      +=======+=========================+
                              *      | Bits  | Descriptions            |
                              *      +=======+=========================+
                              *      | 0     | Is Motherboard          |
                              *      +-------+-------------------------+
                              *      | 1     | Requires Daughter Board |
                              *      +-------+-------------------------+
                              *      | 2     | Removable               |
                              *      +-------+-------------------------+
                              *      | 3     | Replaceable             |
                              *      +-------+-------------------------+
                              *      | 4     | Hot Swap Capable        |
                              *      +-------+-------------------------+
                              *      | 5 - 7 | Reserved                |
                              *      +=======+=========================+
                              */
#       define SMBIOS_IS_Motherboard  EEEP_UINT8_C(1 << 0)
#       define SMBIOS_REQ_DAUGHTER    EEEP_UINT8_C(1 << 1)
#       define SMBIOS_REMOVABLE       EEEP_UINT8_C(1 << 2)
#       define SMBIOS_REPLACEABLE     EEEP_UINT8_C(1 << 3)
#       define SMBIOS_HOT_SWAP_CAP    EEEP_UINT8_C(1 << 4)
    uint8_t Location       ; /* 0x0C Number of ASCIIZ String */
    SmbiosHandle_t LocHdl  ; /* 0x0D Chassis Handle */
    SMBIOS_BoardTypes_t BoardType ; /* 0x0F SMBIOS_BoardTypes_t */
    uint8_t ContainedHndls ; /* 0x0C Number Of Contained 
                              *      Object Handles That 
                              *      Follow
                              */
    SmbiosHandle_t Handles[1] ; /* 0x10 Handles */
}EeePModuleInfo_t;



/*
 *      Chassis Information
 *
 *  see http://www.dmtf.org/standards/published_documents/DSP0134_2.6.1.pdf
 *  Type 3 Block
 */
typedef struct CCElement_s{
  uint8_t ElementType     ; /* 0x00 Specifies the type of 
                             *      element associated with 
                             *      this record.
                             */
  uint8_t MinNum          ; /* 0x01 Specifies the minimum  
                             *      number of the element  
                             *      type that can be installed 
                             *      in the chassis for the  
                             *      chassis to properly operate.
                             */
  uint8_t MaxNum          ; /* 0x02 Specifies the maximum   
                             *      number of the element   
                             *      type that can be installed   
                             *      in the chassis.
                             */
}CCElement_t;
enum ChassisTypes_e{
    SMBIOS_ChassisType_Other                         =0x01, 
    SMBIOS_ChassisType_Unknown                       =0x02, 
    SMBIOS_ChassisType_Desktop                       =0x03, 
    SMBIOS_ChassisType_Low_Profile_Desktop           =0x04, 
    SMBIOS_ChassisType_Pizza_Box                     =0x05, 
    SMBIOS_ChassisType_Mini_Tower                    =0x06, 
    SMBIOS_ChassisType_Tower                         =0x07, 
    SMBIOS_ChassisType_Portable                      =0x08, 
    SMBIOS_ChassisType_Laptop                        =0x09, 
    SMBIOS_ChassisType_Notebook                      =0x0A, 
    SMBIOS_ChassisType_Hand_Held                     =0x0B, 
    SMBIOS_ChassisType_Docking_Station               =0x0C, 
    SMBIOS_ChassisType_All_In_One                    =0x0D, 
    SMBIOS_ChassisType_Sub_Notebook                  =0x0E, 
    SMBIOS_ChassisType_Space_saving_Lunch_Box        =0x0F, 
    SMBIOS_ChassisType_Main_Server_Chassis           =0x11, 
    SMBIOS_ChassisType_Expansion_Chassis             =0x12, 
    SMBIOS_ChassisType_SubChassis                    =0x13, 
    SMBIOS_ChassisType_Bus_Expansion_Chassis         =0x14, 
    SMBIOS_ChassisType_Peripheral_Chassis            =0x15, 
    SMBIOS_ChassisType_RAID_Chassis                  =0x16, 
    SMBIOS_ChassisType_Rack_Mount_Chassis            =0x17, 
    SMBIOS_ChassisType_Sealed_case_PC                =0x18, 
    SMBIOS_ChassisType_Multi_system_chassis          =0x19 
};
typedef uint8_t ChassisTypes_t;

typedef struct EeePChassisInfo_s{
    EeePSmbiosHdr_t SDBHdr; /* 0x00 Smbios Dynamic Block Header */
    uint8_t Manufacturer   ; /* 0x06 Number of ASCIIZ String */
    ChassisTypes_t Type    ; /* 0x07 ENUM */
    uint8_t Version        ; /* 0x08 Number of ASCIIZ String */
    uint8_t SerialNumber   ; /* 0x09 Number of ASCIIZ String */
    uint8_t AssetTagNumber ; /* 0x0A Number of ASCIIZ String */
    uint8_t Reserved0[4]   ; /* 0x0B Reserved */
    EeePuint32u_t OEM      ; /* 0x0F Contains OEM- or BIOS 
                              * vendor-specific information 
                              */
    uint8_t Height         ; /* 0x13 The height of the  
                              * enclosure, in 'U's.  
                              */
    uint8_t NumPowerCords  ; /* 0x14 Identifies the number   
                              * of power cords associated with  
                              * the enclosure or chassis. 
                              */
    uint8_t CElementCnt    ; /* 0x15 Contained Element Count */   
    uint8_t CElementSize   ; /* 0x16 Contained Element Size  */   
    CCElement_t CElement[1]; /* 0x17 Contained Element(s) */   
}EeePChassisInfo_t;


/*
 * Local Flat Panel(LFP) Data Block
 *
 */
typedef struct EeePLFPDataBlock_s{
    DBlockIdHdr_t CDBHdr     ; /* 0x00 Dynamic Block Header  */
    uint8_t	  Interface      ; /* 0x03 Display Interface */
#	define 	  EEEP_DISP_INT_LVDS   EEEP_UINT8_C(0x02)
#	define 	  EEEP_DISP_INT_SDVOB  EEEP_UINT8_C(0x03)
#	define 	  EEEP_DISP_INT_SDVOC  EEEP_UINT8_C(0x04)
#	define 	  EEEP_DISP_INT_DDI1	 EEEP_UINT8_C(0x05)
#	define 	  EEEP_DISP_INT_DDI2	 EEEP_UINT8_C(0x06)
#	define 	  EEEP_DISP_INT_DDI3	 EEEP_UINT8_C(0x07)
    uint8_t	  RawData[1]; /* 0x04 Display Raw Data
			                     *      DisplayID
			                     *      EDID
			                     *      UDS
			                     *      EPI
    			                 */
}EeePLFPDataBlock_t;

/*
 * Vendor Specific Dynamic Block Header 
 *
 */
typedef struct EeePVendBlockHdr_s{
    DBlockIdHdr_t CDBHdr     ; /* 0x00 Dynamic Block Header  */
    uint8_t       VendId[2]  ; /* 0x03 Compressed ASCII PNPID  */
    /* After This Point is only 
     * Suggested
     */
    uint8_t       VendBlockId; /* 0x05 Vendor Specific Block Id  */
    /*uint8_t       VendData[1]; * 0x06 Vendor Data */
}EeePVendBlockHdr_t;


/*
 * Descriptor for Additional EEPROMS
 *
 * I.E. For Chassis/System/Base Board EEPROMs
 */
typedef struct EeePExtI2CDeviceDesc_s{
    DBlockIdHdr_t CDBHdr       ; /* 0x00 Dynamic Block Header  */
    uint8_t       DeviceAddr[2]; /* 0x03 Encoded 7/10 Bit Device Address */
    uint8_t       DeviceBus    ; /* 0x05 Device Bus       */
#	define 	  EEEP_I2CBuSID_I2C	   EEEP_UINT8_C(0x00)
#	define 	  EEEP_I2CBuSID_SMB	   EEEP_UINT8_C(0x01)
#	define 	  EEEP_I2CBuSID_LVDS   EEEP_UINT8_C(0x02)
#	define 	  EEEP_I2CBuSID_DDI1	 EEEP_UINT8_C(0x03)
#	define 	  EEEP_I2CBuSID_DDI2	 EEEP_UINT8_C(0x04)
#	define 	  EEEP_I2CBuSID_DDI3	 EEEP_UINT8_C(0x05)
#	define 	  EEEP_I2CBuSID_SDVOB  EEEP_UINT8_C(0x06)
#	define 	  EEEP_I2CBuSID_SDVOC	 EEEP_UINT8_C(0x07)
#	define 	  EEEP_I2CBuSID_CRT  	 EEEP_UINT8_C(0x08)
    uint8_t       DeviceDesc   ; /* 0x06 Device Descriptor
                                * +========+========================+
                                * | Bits   | Description            |
                                * +========+========================+
                                * | 0 - 3  | Size 2^(8+n) Bytes     |
                                * |        | (256 << n) Bytes       |
                                * |        |                        |
                                * |        | Addressable Bytes      |
                                * |        |   Standard Index       |
                                * |        |     8bit - 11Bit       |
                                * |        |   Extended Index       |
                                * |        |     16bit - 19Bit      |
                                * |        | 2048 KBit - 4096 MBit  |
                                * |        | 256 Bytes - 512 KBytes |
                                * |        |                        |
                                * +--------+------------------------+
                                * | 4      | Std/Ext Index          |
                                * +--------+------------------------+
                                * | 5 - 7  | Reserved               |
                                * +========+========================+
                                */
#   define EEEP_EXT_INDX EEEP_UINT8_C(1<<4)
}EeePExtI2CDeviceDesc_t;


/*
 * Platform Specific Headers
 *
 */
#include "COM0EEP.h"


#pragma pack(pop)   /* restore original alignment from stack */
#endif /* __EEEP_H__ */

