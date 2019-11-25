/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron AG */

/* JIDA interface declarations */

//***************************************************************************

#ifndef WINAPI
#define WINAPI
#endif

#ifndef _JIDA_H_
#define _JIDA_H_

#ifdef __cplusplus
extern "C" {
#endif

//***********************************************************************

#define JidaDllVersionMajor 1
#define JidaDllVersionMinor 6
#define JidaDllVersion ((DWORD)JidaDllVersionMajor*65536+JidaDllVersionMinor)

//***********************************************************************

#define JIDA_BOARD_MAX_LEN_ID_STRING 7
#define JIDA_BOARD_MAX_SIZE_ID_STRING 16
#define JIDA_BOARD_MAX_LEN_SERIAL_STRING 10
#define JIDA_BOARD_MAX_SIZE_SERIAL_STRING 16
#define JIDA_BOARD_MAX_LEN_CLASSES_STRING 127
#define JIDA_BOARD_MAX_SIZE_CLASSES_STRING 128

#define JIDA_BOARDINFO_FLAGS_DEFAULT 0

typedef struct {
  DWORD dwSize;
  DWORD dwFlags;
  CHAR szPrimaryClass[JIDA_BOARD_MAX_SIZE_ID_STRING];
  CHAR szBoard[JIDA_BOARD_MAX_SIZE_ID_STRING];
  CHAR szBoardSub[JIDA_BOARD_MAX_SIZE_ID_STRING];
  CHAR szManufacturer[JIDA_BOARD_MAX_SIZE_ID_STRING];
  SYSTEMTIME stManufacturingDate;
  SYSTEMTIME stLastRepairDate;
  CHAR szSerialNumber[JIDA_BOARD_MAX_SIZE_SERIAL_STRING];
  WORD wHardwareRevision;
  WORD wFirmwareRevision;
  WORD wJidaRevision;
  WORD wFeatureNumber;
  CHAR szClasses[JIDA_BOARD_MAX_SIZE_CLASSES_STRING];
  } JIDABOARDINFOA, *PJIDABOARDINFOA;

typedef struct {
  DWORD dwSize;
  DWORD dwFlags;
  WCHAR szPrimaryClass[JIDA_BOARD_MAX_SIZE_ID_STRING];
  WCHAR szBoard[JIDA_BOARD_MAX_SIZE_ID_STRING];
  WCHAR szBoardSub[JIDA_BOARD_MAX_SIZE_ID_STRING];
  WCHAR szManufacturer[JIDA_BOARD_MAX_SIZE_ID_STRING];
  SYSTEMTIME stManufacturingDate;
  SYSTEMTIME stLastRepairDate;
  WCHAR szSerialNumber[JIDA_BOARD_MAX_SIZE_SERIAL_STRING];
  WORD wHardwareRevision;
  WORD wFirmwareRevision;
  WORD wJidaRevision;
  WORD wFeatureNumber;
  WCHAR szClasses[JIDA_BOARD_MAX_SIZE_CLASSES_STRING];
  } JIDABOARDINFOW, *PJIDABOARDINFOW;

#ifdef UNICODE
#define JIDABOARDINFO JIDABOARDINFOW
#define PJIDABOARDINFO PJIDABOARDINFOW
#else
#define JIDABOARDINFO JIDABOARDINFOA
#define PJIDABOARDINFO PJIDABOARDINFOA
#endif


//***********************************************************************

//
// Board handle
//

typedef DWORD HJIDA, *PHJIDA;

//
// Board Classes
//

#define JIDA_BOARD_CLASS_CPU_A "CPU"
#define JIDA_BOARD_CLASS_VGA_A "VGA"
#define JIDA_BOARD_CLASS_IO_A "IO"

#define JIDA_BOARD_CLASS_CPU_W L"CPU"
#define JIDA_BOARD_CLASS_VGA_W L"VGA"
#define JIDA_BOARD_CLASS_IO_W L"IO"

#define JIDA_BOARD_CLASS_CPU TEXT("CPU")
#define JIDA_BOARD_CLASS_VGA TEXT("VGA")
#define JIDA_BOARD_CLASS_IO TEXT("IO")

//
// Board Open/Count Flags
//

#define JIDA_BOARD_OPEN_FLAGS_DEFAULT 0
#define JIDA_BOARD_OPEN_FLAGS_PRIMARYONLY 1

//
// max values for LCD settings
//

#define JIDA_VGA_CONTRAST_MAX 255
#define JIDA_VGA_BACKLIGHT_MAX 255

//
// Type identifiers for storage areas
//

#define JIDA_STORAGE_AREA_UNKNOWN   0
#define JIDA_STORAGE_AREA_EEPROM    0x00010000
#define JIDA_STORAGE_AREA_FLASH     0x00020000
#define JIDA_STORAGE_AREA_CMOS      0x00030000
#define JIDA_STORAGE_AREA_RAM       0x00040000

#define JIDA_STORAGE_AREA_EEPROM_IP 0x01010000

//
// I2C bus types returned by JidaI2CType()
//

#define JIDA_I2C_TYPE_UNKNOWN 0	          // I2C bus for unknown or special purposes
#define JIDA_I2C_TYPE_PRIMARY 0x00010000  // primary I2C bus
#define JIDA_I2C_TYPE_SMB     0x00020000  // system management bus
#define JIDA_I2C_TYPE_JILI    0x00030000  // I2C bus of the JILI interface
#define JIDA_I2C_TYPE_VIDEO   0x00040000  // I2C bus for video
#define JIDA_I2C_TYPE_CRT     0x00050000  // I2C bus for CRT
#define JIDA_I2C_TYPE_EXT_I2C 0x00060000  // I2C bus
#define JIDA_I2C_TYPE_JILI2   0x00070000  // I2C bus of the 2nd JILI interface (for contrast)

//
// I2C Addresses as used by current JUMPtec boards. May change any time.
//

#ifndef JIDA_I2C_ADDR_EEPROM
#define JIDA_I2C_ADDR_EEPROM          0xA0
#endif

#ifndef JIDA_I2C_ADDR_CONTRAST_DAC
#define JIDA_I2C_ADDR_CONTRAST_DAC    0x5C
#endif

#ifndef JIDA_I2C_ADDR_BACKLIGHT_DAC
#define JIDA_I2C_ADDR_BACKLIGHT_DAC   0x58
#endif

#ifndef JIDA_I2C_ADDR_WATCHDOG_PIC
#define JIDA_I2C_ADDR_WATCHDOG_PIC    0xB0
#endif

#ifndef JIDA_I2C_ADDR_MATRIX_PIC
#define JIDA_I2C_ADDR_MATRIX_PIC      0x5A
#endif

//***********************************************************************

//
// Watchdog structure
//

#define JIDA_WDOG_MODE_REBOOT_PC 0
#define JIDA_WDOG_MODE_RESTART_WINDOWS 1

enum {JWDModeRebootPC=0, JWDModeRestartWindows };

typedef struct {
  DWORD dwSize;
  DWORD dwTimeout;
  DWORD dwDelay;
  DWORD dwMode;
  } JWDCONFIG, *PJWDCONFIG;

//***********************************************************************

//
// Temperature, fan, and voltage structures
//

// temperature in units of 1/1000th degrees celcius

typedef struct {
  DWORD dwSize;
  DWORD	dwType;
  DWORD	dwFlags;
  DWORD	dwAlarm;
  DWORD	dwRes;
  DWORD	dwMin;
  DWORD	dwMax;
  DWORD	dwAlarmHi;
  DWORD	dwHystHi;
  DWORD	dwAlarmLo;
  DWORD	dwHystLo;
  } JIDATEMPERATUREINFO, *PJIDATEMPERATUREINFO;

// fan speed values in RPM (revolutions per minute)

typedef struct {
  DWORD dwSize;
  DWORD	dwType;
  DWORD	dwFlags;
  DWORD	dwAlarm;
  DWORD	dwSpeedNom;
  DWORD	dwMin;
  DWORD	dwMax;
  DWORD	dwAlarmHi;
  DWORD	dwHystHi;
  DWORD	dwAlarmLo;
  DWORD	dwHystLo;
  DWORD	dwOutMin;
  DWORD	dwOutMax;
  } JIDAFANINFO, *PJIDAFANINFO;

// voltage in units of 1/1000th volt

typedef struct {
  DWORD dwSize;
  DWORD	dwType;
  DWORD	dwNom;
  DWORD	dwFlags;
  DWORD	dwAlarm;
  DWORD	dwRes;
  DWORD	dwMin;
  DWORD	dwMax;
  DWORD	dwAlarmHi;
  DWORD	dwHystHi;
  DWORD	dwAlarmLo;
  DWORD	dwHystLo;
  } JIDAVOLTAGEINFO, *PJIDAVOLTAGEINFO;

// types

#define JIDA_TEMP_CPU 0
#define JIDA_TEMP_BOX 1
#define JIDA_TEMP_ENV 2
#define JIDA_TEMP_BOARD 3
#define JIDA_TEMP_BACKPLANE 4
#define JIDA_TEMP_CHIPSETS 5
#define JIDA_TEMP_VIDEO 6
#define JIDA_TEMP_OTHER 7

#define JIDA_FAN_CPU 0
#define JIDA_FAN_BOX 1
#define JIDA_FAN_ENV 2
#define JIDA_FAN_CHIPSET 3
#define JIDA_FAN_VIDEO 4
#define JIDA_FAN_OTHER 5

#define JIDA_VOLTAGE_CPU 0
#define JIDA_VOLTAGE_DC 1
#define JIDA_VOLTAGE_DC_STANDBY 2
#define JIDA_VOLTAGE_BAT_CMOS 3
#define JIDA_VOLTAGE_BAT_POWER 4
#define JIDA_VOLTAGE_AC 5
#define JIDA_VOLTAGE_OTHER 6

// temperature, fan, voltage status flags

#define JIDA_SENSOR_ACTIVE 0x01
#define JIDA_SENSOR_ALARM 0x02
#define JIDA_SENSOR_BROKEN 0x04
#define JIDA_SENSOR_SHORTCIRCUIT 0x08

//***********************************************************************

//
// Performance
//

#define JIDA_CPU_PERF_THROTTLING	1
#define JIDA_CPU_PERF_FREQUENCY	2

//***********************************************************************

#ifndef NOWIN32API

// API version 1.0

// DLL

DWORD WINAPI JidaDllGetVersion(void);
BOOL WINAPI JidaDllInitialize(void);
BOOL WINAPI JidaDllUninitialize(void);
BOOL WINAPI JidaDllIsAvailable(void);
BOOL WINAPI JidaDllInstall(BOOL install);

// Generic board

BOOL WINAPI JidaBoardClose(HJIDA hJida);

DWORD WINAPI JidaBoardCountA(LPCSTR pszClass, DWORD dwFlags);
BOOL WINAPI JidaBoardOpenA(LPCSTR pszClass, DWORD dwNum, DWORD dwFlags, PHJIDA phJida);
BOOL WINAPI JidaBoardOpenByNameA(LPCSTR pszName, PHJIDA phJida);
BOOL WINAPI JidaBoardGetNameA(HJIDA hJida, LPSTR pszName, DWORD dwSize);
BOOL WINAPI JidaBoardGetInfoA(HJIDA hJida, PJIDABOARDINFOA pBoardInfo);

DWORD WINAPI JidaBoardCountW(LPCWSTR pszClass, DWORD dwFlags);
BOOL WINAPI JidaBoardOpenW(LPCWSTR pszClass, DWORD dwNum, DWORD dwFlags, PHJIDA phJida);
BOOL WINAPI JidaBoardOpenByNameW(LPCWSTR pszName, PHJIDA phJida);
BOOL WINAPI JidaBoardGetNameW(HJIDA hJida, LPWSTR pszName, DWORD dwSize);
BOOL WINAPI JidaBoardGetInfoW(HJIDA hJida, PJIDABOARDINFOW pBoardInfo);

#ifdef UNICODE
#define JidaBoardCount JidaBoardCountW
#define JidaBoardOpen JidaBoardOpenW
#define JidaBoardOpenByName JidaBoardOpenByNameW
#define JidaBoardGetName JidaBoardGetNameW
#define JidaBoardGetInfo JidaBoardGetInfoW
#define JidaIOGetName JidaIOGetNameW
#else
#define JidaBoardCount JidaBoardCountA
#define JidaBoardOpen JidaBoardOpenA
#define JidaBoardOpenByName JidaBoardOpenByNameA
#define JidaBoardGetName JidaBoardGetNameA
#define JidaBoardGetInfo JidaBoardGetInfoA
#define JidaIOGetName JidaIOGetNameA
#endif

BOOL WINAPI JidaBoardGetBootCounter(HJIDA hJida, LPDWORD pdwCount);
BOOL WINAPI JidaBoardGetRunningTimeMeter(HJIDA hJida, LPDWORD pdwCount);

BOOL WINAPI JidaBoardGetOption(HJIDA hJida, DWORD dwOption, LPDWORD pdwSetting);
BOOL WINAPI JidaBoardSetOption(HJIDA hJida, DWORD dwOption, DWORD dwSetting);

BOOL WINAPI JidaBoardGetBootErrorLog(HJIDA hJida, DWORD dwType, PDWORD pdwLogType, LPBYTE pBytes, PDWORD pdwLen); // V1.6

// VGA (LCD)

BOOL WINAPI JidaVgaGetContrast(HJIDA hJida, LPDWORD pdwSetting);
BOOL WINAPI JidaVgaSetContrast(HJIDA hJida, DWORD dwSetting);
BOOL WINAPI JidaVgaGetContrastEnable(HJIDA hJida, LPDWORD pdwSetting); // V1.3
BOOL WINAPI JidaVgaSetContrastEnable(HJIDA hJida, DWORD dwSetting); // V1.3
BOOL WINAPI JidaVgaGetBacklight(HJIDA hJida, LPDWORD pdwSetting);
BOOL WINAPI JidaVgaSetBacklight(HJIDA hJida, DWORD dwSetting);
BOOL WINAPI JidaVgaGetBacklightEnable(HJIDA hJida, LPDWORD pdwSetting); // V1.2
BOOL WINAPI JidaVgaSetBacklightEnable(HJIDA hJida, DWORD dwSetting); // V1.2

BOOL WINAPI JidaVgaEndDarkBoot(DWORD dwReserved);

// EEPROM, FLASH, CMOS

DWORD WINAPI JidaStorageAreaCount(HJIDA hJida, DWORD dwType);
DWORD WINAPI JidaStorageAreaType(HJIDA hJida, DWORD dwType);
DWORD WINAPI JidaStorageAreaSize(HJIDA hJida, DWORD dwType);
DWORD WINAPI JidaStorageAreaBlockSize(HJIDA hJida, DWORD dwType);
BOOL WINAPI JidaStorageAreaRead(HJIDA hJida, DWORD dwType, DWORD dwOffset, LPBYTE pBytes, DWORD dwLen);
BOOL WINAPI JidaStorageAreaWrite(HJIDA hJida, DWORD dwType, DWORD dwOffset, LPBYTE pBytes, DWORD dwLen);
BOOL WINAPI JidaStorageAreaErase(HJIDA hJida, DWORD dwType, DWORD dwOffset, DWORD dwLen);
BOOL WINAPI JidaStorageAreaEraseStatus(HJIDA hJida, DWORD dwType, DWORD dwOffset, DWORD dwLen, LPDWORD lpStatus);

// I2C Bus

DWORD WINAPI JidaI2CCount(HJIDA hJida);
DWORD WINAPI JidaI2CType(HJIDA hJida, DWORD dwType); // V1.3
BOOL WINAPI JidaI2CIsAvailable(HJIDA hJida, DWORD dwType);
BOOL WINAPI JidaI2CRead(HJIDA hJida, DWORD dwType, BYTE bAddr, LPBYTE pBytes, DWORD dwLen);
BOOL WINAPI JidaI2CWrite(HJIDA hJida, DWORD dwType, BYTE bAddr, LPBYTE pBytes, DWORD dwLen);

BOOL WINAPI JidaI2CReadRegister(HJIDA hJida, DWORD dwType, BYTE bAddr, WORD wReg, LPBYTE pDataByte);
BOOL WINAPI JidaI2CWriteRegister(HJIDA hJida, DWORD dwType, BYTE bAddr, WORD wReg, BYTE bData);

BOOL WINAPI JidaI2CWriteReadCombined(HJIDA hJida, DWORD dwType, BYTE bAddr, LPBYTE pBytesWrite,
                                     DWORD dwLenWrite, LPBYTE pBytesRead, DWORD dwLenRead); //V1.4

// General purpose IO

DWORD WINAPI JidaIOCount(HJIDA hJida);
BOOL WINAPI JidaIOIsAvailable(HJIDA hJida, DWORD dwType);
BOOL WINAPI JidaIORead(HJIDA hJida, DWORD dwType, LPDWORD pdwData);
BOOL WINAPI JidaIOWrite(HJIDA hJida, DWORD dwType, DWORD dwData);
BOOL WINAPI JidaIOXorAndXor(HJIDA hJida, DWORD dwType, DWORD dwXorMask1, DWORD dwAndMask, DWORD dwXorMask2);
BOOL WINAPI JidaIOGetDirection(HJIDA hJida, DWORD dwType, LPDWORD pdwData); // V1.5
BOOL WINAPI JidaIOSetDirection(HJIDA hJida, DWORD dwType, DWORD dwData); // V1.5
BOOL WINAPI JidaIOGetDirectionCaps(HJIDA hJida, DWORD dwType, LPDWORD pdwInputs, LPDWORD pdwOutputs); // V1.5
BOOL WINAPI JidaIOGetNameA(HJIDA hJida, DWORD dwType, LPSTR pszName, DWORD dwSize); // V1.5
BOOL WINAPI JidaIOGetNameW(HJIDA hJida, DWORD dwType, LPWSTR pszName, DWORD dwSize); // V1.5


// API version 1.1

// Watchdog

DWORD WINAPI JidaWDogCount(HJIDA hJida);
BOOL WINAPI JidaWDogIsAvailable(HJIDA hJida, DWORD dwType);
BOOL WINAPI JidaWDogTrigger(HJIDA hJida, DWORD dwType);
DWORD WINAPI JidaWDogGetTriggerCount(HJIDA hJida, DWORD dwType);
BOOL WINAPI JidaWDogSetTriggerCount(HJIDA hJida, DWORD dwType, DWORD cnt);
BOOL WINAPI JidaWDogGetConfigStruct(HJIDA hJida, DWORD dwType, PJWDCONFIG pConfig);
BOOL WINAPI JidaWDogSetConfigStruct(HJIDA hJida, DWORD dwType, PJWDCONFIG pConfig);
BOOL WINAPI JidaWDogSetConfig(HJIDA hJida, DWORD dwType, DWORD timeout, DWORD delay, DWORD mode);
BOOL WINAPI JidaWDogDisable(HJIDA hJida, DWORD dwType);

// API version 1.3

// CPU Performance

BOOL WINAPI JidaPerformanceGetCurrent(HJIDA hJida, DWORD dwType, LPDWORD pdwSetting);
BOOL WINAPI JidaPerformanceSetCurrent(HJIDA hJida, DWORD dwType, DWORD dwSetting);
BOOL WINAPI JidaPerformanceGetPolicyCaps(HJIDA hJida, DWORD dwType, LPDWORD pdwSetting); // V1.6
BOOL WINAPI JidaPerformanceGetPolicy(HJIDA hJida, DWORD dwType, LPDWORD pdwSetting); // V1.6
BOOL WINAPI JidaPerformanceSetPolicy(HJIDA hJida, DWORD dwType, DWORD dwSetting); // V1.6

// API version 1.6

// Temperature

DWORD WINAPI JidaTemperatureCount(HJIDA hJida);
BOOL WINAPI JidaTemperatureGetInfo(HJIDA hJida, DWORD dwType, PJIDATEMPERATUREINFO pInfo);
BOOL WINAPI JidaTemperatureGetCurrent(HJIDA hJida, DWORD dwType, LPDWORD pdwSetting, LPDWORD pdwStatus);
BOOL WINAPI JidaTemperatureSetLimits(HJIDA hJida, DWORD dwType, PJIDATEMPERATUREINFO pInfo);

// Fan

DWORD WINAPI JidaFanCount(HJIDA hJida);
BOOL WINAPI JidaFanGetInfo(HJIDA hJida, DWORD dwType, PJIDAFANINFO pInfo);
BOOL WINAPI JidaFanGetCurrent(HJIDA hJida, DWORD dwType, LPDWORD pdwSetting, LPDWORD pdwStatus);
BOOL WINAPI JidaFanSetLimits(HJIDA hJida, DWORD dwType, PJIDAFANINFO pInfo);

// Voltage

DWORD WINAPI JidaVoltageCount(HJIDA hJida);
BOOL WINAPI JidaVoltageGetInfo(HJIDA hJida, DWORD dwType, PJIDAVOLTAGEINFO pInfo);
BOOL WINAPI JidaVoltageGetCurrent(HJIDA hJida, DWORD dwType, LPDWORD pdwSetting, LPDWORD pdwStatus);
BOOL WINAPI JidaVoltageSetLimits(HJIDA hJida, DWORD dwType, PJIDAVOLTAGEINFO pInfo);

#endif

//***************************************************************************

#ifdef __cplusplus
}
#endif

#endif /* _JIDA_H_ */
