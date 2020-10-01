/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron Europe GmbH */

/* KEAPI interface specification. */

#ifndef KEAPI_H
#define KEAPI_H

#include <stdint.h>
#ifdef __VXWORKS__
#include "versionK.h"
KONTRON_VERSION(keapi_h, 01c) /* version identifier */
#endif /* __VXWORKS__ */

#ifdef __cplusplus
extern "C" {
#endif

#define KEAPI_SPEC_VERSION_MAJOR	(0x03)
#define KEAPI_SPEC_VERSION_MINOR	(0x00)

#define KEAPI_MAX_STR 128 /*!< Maximal count of characters for string fields */

#ifdef _WIN32
#	if defined(KEAPI_COMMON_BUILD)
#		define KEAPI_CALLTYPE
#	else
#		if defined(KONTRON_API_EXPORTS)
#			define KEAPI_CALLTYPE __declspec (dllexport)
#		elif defined(__cplusplus)
#			define KEAPI_CALLTYPE extern "C" __declspec (dllimport)
#		else
#			define KEAPI_CALLTYPE __declspec (dllimport)
#		endif
#	endif
#else
#define KEAPI_CALLTYPE
#endif

typedef uint32_t KEAPI_RETVAL;

/* General return values */
#define KEAPI_RET_SUCCESS			0x00000000   /*!< no error */
#define KEAPI_RET_ERROR				0xFFFFFFFF   /*!< Unknown or internal error. */
#define KEAPI_RET_PARAM_ERROR			0x00000001   /*!< Wrong parameter value */
#define KEAPI_RET_PARAM_NULL			0x00000002   /*!< Parameter is NULL where it is not allowed   */
#define KEAPI_RET_BUFFER_OVERFLOW		0x00000008   /*!< Buffer overflow (probably configuration error) */
#define KEAPI_RET_SETTING_ERROR			0x00000009   /*!< Error while setting value or feature (enable, disable) */
#define KEAPI_RET_WRITE_ERROR			0x0000000A   /*!< Cannot write */
#define KEAPI_RET_READ_ERROR			0x0000000B   /*!< Cannot read */
#define KEAPI_RET_MALLOC_ERROR			0x0000000C   /*!< Memory allocation failed */
#define KEAPI_RET_LIBRARY_ERROR			0x0000000D   /*!< Exported function could not be loaded from library*/
#define KEAPI_RET_WMI_ERROR			0x0000000E   /*!< This signalizes some problem during reading from WMI */
#define KEAPI_RET_NOT_INITIALIZED		0x0000000F   /*!< KEAPI library is not initialized */
#define KEAPI_RET_PARTIAL_SUCCESS		0x00000010   /*!< Part of requested information couldn't be retrieved.
								  Returned information isn't complete (buffer is not enough). */
#define KEAPI_RET_FUNCTION_NOT_SUPPORTED	0x00000011   /*!< Function is not supported on current platform/HW */
#define KEAPI_RET_RETRIEVAL_ERROR		0x00000012   /*!< Error while retrieving information */
#define KEAPI_RET_FUNCTION_NOT_IMPLEMENTED	0x00000013   /*!< Function is not yet implemented */
#define KEAPI_RET_BUSY_COLLISION		0x00000014   /*!< Bus/Device Busy or Arbitration Error/Collision Error */
#define KEAPI_RET_HW_TIMEOUT			0x00000015   /*!< Timeout occurred while accessing to device */
#define KEAPI_RET_BUS_ERROR			0x00000016   /*!< No acknowledge on address or bus error during operation */
#define KEAPI_RET_CANCELLED			0x00000017   /*!< Operation is cancelled */

#define KEAPI_RET_PERMISSION_DENIED		0x00010009   /*!< Insufficient user permissions (cannot access the device) */

/* KRS Errors */
#define KEAPI_RET_RMT_CON_ERROR			0x00010007   /*!< RPC - Remote connection failed  */
#define KEAPI_RET_UNAUTHORIZED			0x0001000a   /*!< Bad password */

/* Intruder states - used by functions KEApiGetIntruderStatus and KEApiSetIntruderStatus */
#define KEAPI_INTRUDER_STATUS_CASE_CLOSED	(0)   /*!< Case was not opened */
#define KEAPI_INTRUDER_STATUS_CASE_OPENED	(1)   /*!< Case was opened */

/* Battery power states */
#define KEAPI_POWER_STATE_CHARGING	(0)
#define KEAPI_POWER_STATE_CHARGED	(1)
#define KEAPI_POWER_STATE_DISCHARGING	(2)

#define KEAPI_DISPLAY_BRIGHTNESS_MAX (255)

/* Performance states */
#define KEAPI_PM_P0 (0) /* always capable */
#define KEAPI_PM_P1 (1 << 0)
#define KEAPI_PM_P2 (1 << 1)
#define KEAPI_PM_P3 (1 << 2)
#define KEAPI_PM_P4 (1 << 3)
#define KEAPI_PM_P5 (1 << 4)
#define KEAPI_PM_P6 (1 << 5)
#define KEAPI_PM_P7 (1 << 6)
#define KEAPI_PM_P8 (1 << 7)
#define KEAPI_PM_P9 (1 << 8)
#define KEAPI_PM_P10 (1 << 9)
#define KEAPI_PM_P11 (1 << 10)
#define KEAPI_PM_P12 (1 << 11)
#define KEAPI_PM_P13 (1 << 12)
#define KEAPI_PM_P14 (1 << 13)
#define KEAPI_PM_P15 (1 << 14)
#define KEAPI_PM_P16 (1 << 15)

#define KEAPI_SENSOR_STATUS_ACTIVE (1 << 0) /* Sensor is operating  */
#define KEAPI_SENSOR_STATUS_ALARM (1 << 1)  /* Sensor reports alarm condition */
#define KEAPI_SENSOR_STATUS_BROKEN (1 << 2) /* Sensor circuit is broken */
#define KEAPI_SENSOR_STATUS_SHORTCIRCUIT (1 << 3) /* Sensor has a short circuit */

#define KEAPI_SENSOR_INFO_UNKNOWN ((int32_t)0x7FFFFFFF)

enum KeapiTempSensorType {
	KEAPI_TEMP_CPU = 1,
	KEAPI_TEMP_BOX,
	KEAPI_TEMP_ENV,
	KEAPI_TEMP_BOARD,
	KEAPI_TEMP_BACKPLANE,
	KEAPI_TEMP_CHIPSET,
	KEAPI_TEMP_VIDEO,
	KEAPI_TEMP_OTHER
};

enum KeapiVoltageSensorType {
	KEAPI_VOLTAGE_VCORE = 1,
	KEAPI_VOLTAGE_1V8,
	KEAPI_VOLTAGE_2V5,
	KEAPI_VOLTAGE_3V3,
	KEAPI_VOLTAGE_VBAT,
	KEAPI_VOLTAGE_5V,
	KEAPI_VOLTAGE_5VSB,
	KEAPI_VOLTAGE_12V,
	KEAPI_VOLTAGE_AC,
	KEAPI_VOLTAGE_OTHER
};

enum KeapiFanSensorType {
	KEAPI_FAN_CPU = 1,
	KEAPI_FAN_BOX,
	KEAPI_FAN_ENV,
	KEAPI_FAN_CHIPSET,
	KEAPI_FAN_VIDEO,
	KEAPI_FAN_OTHER
};

/* Fan control modes */
#define KEAPI_FAN_METHOD_MAX_SPEED (0)
#define KEAPI_FAN_METHOD_MANUAL (1)
#define KEAPI_FAN_METHOD_THERMAL_CRUISE (2)
#define KEAPI_FAN_METHOD_SPECIAL (3)

/* CPU frequencies */
#define KEAPI_CPU_FREQUENCY_MIN (0)
#define KEAPI_CPU_FREQUENCY_MAX (1)
#define KEAPI_CPU_FREQUENCY_CURRENT (2)
#define KEAPI_CPU_FREQUENCY_TURBO (3)

/* GPIO direction */
#define KEAPI_GPIO_DIRECTION_IN (1)
#define KEAPI_GPIO_DIRECTION_OUT (0)

/* SMBus quick command */
#define KEAPI_SMBUS_WRITE_OP (0)
#define KEAPI_SMBUS_READ_OP (1)

#define KEAPI_WD_MODE_RESET (0)
#define KEAPI_WD_MODE_INTERRUPT (1)
#define KEAPI_WD_MODE_TIMER_ONLY (2)
#define KEAPI_WD_MODE_DISABLE (-1)

#define KEAPI_WD_EXPIRED (1)

/*! \brief Structure for library version */

#ifdef _WIN32
#pragma pack(push, 8)
#endif

/* KEAPI Library version is 4-byte code:
 * major, minor, patch, build in Big Endian format
 * major and minor are defined in KEAPI specification
 * patch and build are defined by the implementation
 */
typedef struct Keapi_Version_Data {
	char versionText[KEAPI_MAX_STR]; /*!< human readable version string: "Nmj.Nmn.Npc.Nbd" */
	uint32_t version; /*!< major, minor, patch, build in Big Endian format */
} KEAPI_VERSION_DATA, *PKEAPI_VERSION_DATA;

typedef struct Keapi_Board_Info {
	char	 boardManufacturer[KEAPI_MAX_STR];	/*!< Board manufacturer */
	char	 boardName[KEAPI_MAX_STR];	/*!< Board name */
	char	 boardSerialNumber[KEAPI_MAX_STR];	/*!< Board serial number */
	char	 hardwareVersion[KEAPI_MAX_STR]; /*!< hardware revision in text form */
	int64_t	 manufacturingDate;	/*!< Board Manufacturing date as POSIX timestamp (time_t) */
	int64_t	 lastRepairDate;	/*!< Date that the system was last repaired or
									 refurbished. Valid only if later than the manufacturing date.
									 POSIX timestamp (time_t) */
	char	 carrierInfo[KEAPI_MAX_STR]; /*!< Carrier name and version */
	char	 firmwareVersion[KEAPI_MAX_STR]; /*!< Bootloader/BIOS version */
	int64_t	 firmwareDate;	/*!< Bootloader/BIOS date as POSIX timestamp (time_t) */
} KEAPI_BOARD_INFO, *PKEAPI_BOARD_INFO;

/*////////////////////////////////////////////////////////////////////////////// */
/*! \brief Structure for CPU basic info */
typedef struct Keapi_Cpu_Info {
	char cpuName[KEAPI_MAX_STR]; /*!< CPU name*/
	int32_t cpuCount; /*!< Number of CPUs*/
	int32_t cpuCoreCount; /*!< Number of cores of each CPU*/
	int32_t cpuThreadCount; /*!< Number of CPU threads*/
} KEAPI_CPU_INFO, *PKEAPI_CPU_INFO;

/*////////////////////////////////////////////////////////////////////////////// */
/*! \brief Structure for physical memory info*/
typedef struct Keapi_Memory_Info {
	int32_t memTotal; /*!< Total physical memory size in MB*/
	int32_t memFree; /*!< Free memory in MB*/
	int32_t memSpeed; /*!< Memory speed in MHz*/
	char memType[KEAPI_MAX_STR]; /*!< Type of memory (DDR, DDR2, etc.  */
} KEAPI_MEMORY_INFO, *PKEAPI_MEMORY_INFO;

/*////////////////////////////////////////////////////////////////////////////// */
/*! \brief Structure for battery info*/
typedef struct Keapi_Battery_Info {
	char deviceName[KEAPI_MAX_STR]; /*!< Device name */
	char type[KEAPI_MAX_STR]; /*!< LION, NiCd, NiMH */
	char serialNumber[KEAPI_MAX_STR]; /*!< Serial number */
	int32_t designedVoltage; /*!< Designed voltage in mV */
	int32_t designedCapacity; /*!< Designed capacity of fully charged battery in mAh */
	int32_t fullyChargedCapacity; /*!< Real capacity of fully charged battery in mAh */
	int32_t cycleCount; /*!< Number of charge/discharge cycles experienced during lifetime*/
} KEAPI_BATTERY_INFO, *PKEAPI_BATTERY_INFO;

/*////////////////////////////////////////////////////////////////////////////// */
/*! \brief Structure for actual battery state */
typedef struct Keapi_BatteryState {
	int32_t powerState; /*!< Current power state - charging = 0, charged = 1, discharging = 2*/
	int32_t fullBatteryRemainingTime; /*!< Remaining time in seconds when battery is full and AC power unplugged*/
	int32_t remainingTime; /*!< Remaining time in seconds*/
	int32_t remainingCapacity; /*!< Remaining capacity in mAh*/
	int32_t currentVoltage; /*!< Current voltage in mV*/
	int32_t rate; /*!< Current charging/discharging rate in mA*/
	int32_t chargeState; /*!< Battery charge state in percentage */
} KEAPI_BATTERY_STATE, *PKEAPI_BATTERY_STATE;

/*////////////////////////////////////////////////////////////////////////////// */
/*! \brief Structure for PCI device info*/
typedef struct Keapi_Pci_Device {
	int32_t domain; /*!< Domain number*/
	int32_t bus; /*!< Bus number*/
	int32_t slot; /*!< Slot number*/
	int32_t funct; /*!< Function number*/
	int32_t deviceId; /*!< Device ID*/
	int32_t vendorId; /*!< Vendor ID*/
	int32_t classId; /*!< Class ID*/
	char deviceName[KEAPI_MAX_STR]; /*!< Name of the device*/
	char vendorName[KEAPI_MAX_STR]; /*!< Name of the vendor*/
	char className[KEAPI_MAX_STR]; /*!< Name of the class*/
} KEAPI_PCI_DEVICE, *PKEAPI_PCI_DEVICE;

/* ////////////////////////////////////////////////////////////////////////////// */
/*! \brief Structure for network device info*/
typedef struct Keapi_Network_Device {
	char ip[KEAPI_MAX_STR]; /*!< IP address*/
	char mac[KEAPI_MAX_STR]; /*!< MAC address in format XX-XX-XX-XX-XX-XX*/
	int32_t speed; /*!< Connection speed in MB/s */
	char deviceName[KEAPI_MAX_STR]; /*!< Name of the network device*/
} KEAPI_NETWORK_DEVICE, *PKEAPI_NETWORK_DEVICE;

/* ////////////////////////////////////////////////////////////////////////////// */
/*! \brief Structure for disk drive info*/
typedef struct Keapi_Disk_Drive {
	char name[KEAPI_MAX_STR]; /*!< HDD name*/
	char model[KEAPI_MAX_STR]; /*!< Model*/
	char diskSerialNumber[KEAPI_MAX_STR]; /*!< Serial number*/
	int32_t size; /*!< Size in MB*/
} KEAPI_DISK_DRIVE, *PKEAPI_DISK_DRIVE;

/* ////////////////////////////////////////////////////////////////////////////// */
/*! \brief mount point info*/
typedef struct Keapi_Mount_Point
{
	char	name[KEAPI_MAX_STR];	/*!< mount point name */
	char	fsType[KEAPI_MAX_STR];	/*!< Filesystem type */
	int32_t	size;	/*!< Size in MB */
	int32_t	freeSpace;	/*!< Free space in MB */
} KEAPI_MOUNT_POINT, *PKEAPI_MOUNT_POINT;

/* ////////////////////////////////////////////////////////////////////////////// */
/*! \brief Structure representing disk SMART Attribute */
typedef struct Keapi_Smart_Attr {
	uint8_t attrID; /*!< attribute ID */
	uint16_t statusFlags;
	uint8_t attrValue; /*!<  normalized value */
	uint8_t worstValue; /*!<  worst value */
	uint8_t rawValue[6]; /*!<  raw value */
} KEAPI_SMART_ATTR, *PKEAPI_SMART_ATTR;

/* ////////////////////////////////////////////////////////////////////////////// */
/*! \brief Current status and sensor value */
typedef struct Keapi_Sensor_Value {
	int32_t value; /*!< Value obtained from sensor (negative values allowed) */
	int32_t status; /*!< see KEAPI_SENSOR_STATUS_... values */
} KEAPI_SENSOR_VALUE, *PKEAPI_SENSOR_VALUE;

/* ////////////////////////////////////////////////////////////////////////////// */
/*! \brief Structure for sensor info, fields which do not provide valid information are set to KEAPI_SENSOR_INFO_UNKNOWN */
typedef struct Keapi_Sensor_Info {
	char name[KEAPI_MAX_STR]; /*!< Sensor's description */
	int32_t type; /*!< temp/fan/voltage type, see Keapi.....SensorType enumerations */
	int32_t min; /*!< minimum sensor value that can be measured */
	int32_t max; /*!< maximum sensor value that can be measured */
	int32_t alarmHi; /*!< upper alarm threshold, i.e. the value up to which the value must rise to generate an alarm */
	int32_t hystHi; /*!< upper alarm hysteresis,
			     i.e. how much the value must decrease to reset an alarm,
			     must be reported as an absolute value, NOT a delta */
	int32_t alarmLo; /*!< lower alarm threshold,
			      i.e. the value down to which the value must fall to generate an alarm */
	int32_t hystLo; /*!< lower alarm hysteresis,
			     i.e. how much the value must rise to reset an alarm,
			     must be reported as an absolute value, NOT a delta */
} KEAPI_SENSOR_INFO, *PKEAPI_SENSOR_INFO;

#ifdef _WIN32
#pragma pack(pop)
#endif

#ifdef KEAPI_INTERFACE_REMOTE
#	define KEAPI_PRM_START uint32_t connHandle,
#   define KEAPI_PRM_VOID uint32_t connHandle
#else
#	define KEAPI_PRM_START
#   define KEAPI_PRM_VOID
#endif

/*/////////////////////////////////////////////////////////////////////////////
 // LIBRARY INITIALIZATION
 ///////////////////////////////////////////////////////////////////////////// */

#ifndef KEAPI_INTERFACE_REMOTE
/*!
 \brief          Initialization of Kontron EAPI.
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOTROOT - the caller has no root privileges
 * - KEAPI_RET_ERROR - other error
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiLibInitialize(void);

/*!
 \brief          Uninitialization of Kontron EAPI on a selected board.
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiLibUnInitialize(void);

#endif /* KEAPI_INTERFACE_REMOTE */

/*!
 \brief          Provides information version of library
 \param[out]     pVersion Returned KEAPI_VERSION_DATA structure
 \return
 * - KEAPI_RET_SUCCESS on success
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiGetLibVersion(KEAPI_PRM_START
	PKEAPI_VERSION_DATA pVersion
);

/*!
 \brief          Provides text description of a KEAPI error code
 \param[in]      error code returned by any keapi function
 \param[out]     pBuffer preallocated buffer for returned error description
 \param[in]      bufferLength
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_PARAM_ERROR - unknown code
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiGetErrorString(
	KEAPI_RETVAL error,
	char *pBuffer,
	int32_t bufferLength
);

/*/////////////////////////////////////////////////////////////////////////////
 // General information
 ///////////////////////////////////////////////////////////////////////////// */

/*!
 \brief          Provides information about Kontron motherboard
 \param[out]     pBoardInfo Returned board info structure KEAPI_BOARD_INFO
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiGetBoardInfo(KEAPI_PRM_START
	PKEAPI_BOARD_INFO pBoardInfo
);


/*!
 \brief          Provides information about number of boot cycles within the board's lifetime
 \param[out]     pBootCount Number of boot cycles
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiGetBootCounter(KEAPI_PRM_START
	int32_t * pBootCount
);


/*!
 \brief          Provides time left since last boot
 \param[out]     pSystemUpTime Pointer to a variable that receives system running time in seconds
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiSystemUpTime(KEAPI_PRM_START
	int32_t *pSystemUpTime
);


/*!
 \brief          Provides actual information whether computer case was opened or not
 \param[out]     pIntruderStatus  Pointer to a variable that receives actual intruder status
 * - KEAPI_INTRUDER_STATUS_CASE_CLOSED, 0
 * - KEAPI_INTRUDER_STATUS_CASE_OPENED , 1
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiGetIntruderStatus(KEAPI_PRM_START
	int32_t *pIntruderStatus
);


/*!
 \brief          Resets the case intruder status
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiResetIntruderStatus(KEAPI_PRM_VOID);


/*!
 \brief          Get result of Power-on built-in test (PBIT). Valid for platforms with PBIT support.
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiGetPBITResult(KEAPI_PRM_START
	uint32_t *pResult, uint32_t *pCumulativeResult
);

/*!
 \brief          Resets latest result of Power-on built-in test (PBIT). Valid for platforms with PBIT support.
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiClearPBITResult(KEAPI_PRM_VOID);

/*/////////////////////////////////////////////////////////////////////////////
 // CPU information
 ///////////////////////////////////////////////////////////////////////////// */

/*!
 \brief          Provides the CPU frequency of a given type
 * - CPU minimal frequency: KEAPI_CPU_FREQUENCY_MIN, 0
 * - CPU maximal frequency: KEAPI_CPU_FREQUENCY_MAX, 1
 * - CPU current frequency: KEAPI_CPU_FREQUENCY_CURRENT, 2
 \param[in]      freqType Type of frequency (minimal (KEAPI_CPU_FREQUENCY_MIN, 0), maximal (KEAPI_CPU_FREQUENCY_MAX, 1), current (KEAPI_CPU_FREQUENCY_CURRENT, 2))
 \param[out]     frequency Frequency of the CPU, in kHz
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiGetCpuFreq(KEAPI_PRM_START
	int32_t coreNr,
	int8_t freqType,
	int32_t *pFrequency
);


/*
 \brief          Provides information about processors

 \param[out]     pCpuInfo Returned KEAPI_CPU_INFO structure
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiGetCpuInfo(KEAPI_PRM_START
        PKEAPI_CPU_INFO pCpuInfo
);


/*!
 \brief          Provides information about the current CPU performance in percentage
 \param[out]     pPerformancePercentage Pointer to current CPU performance in percentage
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiGetCpuPerformance(KEAPI_PRM_START
	int32_t coreNr,
	int8_t *pPerformancePercentage
);


/*/////////////////////////////////////////////////////////////////////////////
 // Memory information
 ///////////////////////////////////////////////////////////////////////////// */

/*!
 \brief          Provides information about physical memory
 \param[out]     pMemoryInfo Returned KEAPI_MEMORY_INFO structure
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiGetMemoryInfo(KEAPI_PRM_START
	PKEAPI_MEMORY_INFO pMemoryInfo
);

/*/////////////////////////////////////////////////////////////////////////////
 // HDD information
 ///////////////////////////////////////////////////////////////////////////// */

/*!
 \brief          Provides number of installed disk drives
 \param[out]     pDiskDriveCount Number of installed disk drives
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiGetDiskDriveCount(KEAPI_PRM_START
	int32_t *pDiskDriveCount
);


/*!
 \brief          Provides list of installed disk drives and their properties

 \param[in]      diskDriveCount Number of disks
 \param[out]     pDiskDrives Returned array of KEAPI_DISK_DRIVE structures.
 The array must be allocated as DiskDriveCount * sizeof(KEAPI_DISK_DRIVE),
 where DiskDriveCount is obtaind by calling KEApiGetDiskDriveCount.
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiGetDiskDriveList(KEAPI_PRM_START
	PKEAPI_DISK_DRIVE pDiskDrives,
	int32_t diskDriveCount
);


/*!
 \brief          Provides number of mount points in the system.

 \param[out]     pCount Number of mount points
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiGetMountPointCount(KEAPI_PRM_START
	int32_t *pCount
);


/*!
 \brief          Provides list of disk partitions
 \param[in]      diskPartitionCount Number of partitions (size of elements in the pDiskPartitionData array)
 \param[out]     pDiskPartitionData Array of DISK_PARTITION structures
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiGetMountPointList (KEAPI_PRM_START
	PKEAPI_MOUNT_POINT pMountPointList,
	int32_t mountPointCount
);

/*!
 \brief          Provides number of SMART attributes of the disk drive.
 \param[in]      diskNr Disk number from 0 to diskDriveCount - 1
 \param[out]     pAttrCount Where to put number of SMART attributes of the disk drive
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiGetDiskDriveSMARTAttrCount(KEAPI_PRM_START
	int32_t diskNr,
	int32_t * pAttrCount
);


/*!
 \brief          Provides list of disk SMART attributes.
 \param[in]      diskNr Disk number from 0 to diskDriveCount - 1
 \param[out]     pAttrs Where to put number of SMART attributes of the disk drive
 \param[in]      attrCount Number of attributes (size of the pAttrs array)
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiGetDiskDriveSMARTAttrs(KEAPI_PRM_START
	int32_t diskNr,
	PKEAPI_SMART_ATTR pAttrs,
	int32_t attrCount
);

/*/////////////////////////////////////////////////////////////////////////////
 // Battery information
 ///////////////////////////////////////////////////////////////////////////// */

/*!
 \brief          Provides number of available battery slots. SMBUS specifies 4 slots.

 \param[out]     pBatteryCount Number of available battery slots.
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiGetBatteryCount(KEAPI_PRM_START
	int32_t *pBatteryCount
);


/*!
 \brief          Provides information about battery in slot

 \param[in]      slotNr Requested battery's slot number. SMBUS specifies 4 slots. Numbers start with zero.
 \param[out]     pBatteryInfo Returned KEAPI_BATTERY_INFO structure
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiGetBatteryInfo(KEAPI_PRM_START
	int32_t slotNr,
	PKEAPI_BATTERY_INFO pBatteryInfo
);


/*!
 \brief          Provides information about selected battery
 \param[in]      slotNr    Requested battery's slot number. Numbers start with zero
 \param[out]     pBatteryState  Pointer to a KEAPI_BATTERY_STATE structure
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiGetBatteryState(KEAPI_PRM_START
	int32_t slotNr,
	PKEAPI_BATTERY_STATE pBatteryState
);

/*/////////////////////////////////////////////////////////////////////////////
 // Performance management
 ///////////////////////////////////////////////////////////////////////////// */

/*!
 \brief          Provides power states mask the system is capable to run.
 \param[out]     pStatesMask Pointer to states mask returned, available masks:
 KEAPI_PM_P0 - max power and frequency
 KEAPI_PM_P1 - less than P0, voltage/frequency scaled
 KEAPI_PM_P2
 KEAPI_PM_P3
 ...
 KEAPI_PM_P16 - less than P15, voltage/frequency scaled
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiPerformanceStateCaps(KEAPI_PRM_START
	uint32_t *pStatesMask
);

/*!
 \brief          Get description of specified power state.
 \param[in]      state the state to be described:
 KEAPI_PM_P0 - max power and frequency
 KEAPI_PM_P1 - less than P0, voltage/frequency scaled
 KEAPI_PM_P2
 KEAPI_PM_P3
 ...
 KEAPI_PM_P16 - less than P15, voltage/frequency scaled
 \param[out]     pDescription buffer with KEAPI_MAX_STR capacity to put state description
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiGetPerformanceStateDescription(KEAPI_PRM_START
	uint32_t state,
	char *pDescription
);

/*!
 \brief          Get information about device power state.
 \param[out]      pState Pointer to the state value returned:
 KEAPI_PM_P0 - max power and frequency
 KEAPI_PM_P1 - less than P0, voltage/frequency scaled
 KEAPI_PM_P2
 KEAPI_PM_P3
 ...
 KEAPI_PM_P16 - less than P15, voltage/frequency scaled
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiGetPerformanceState(KEAPI_PRM_START
	uint32_t *pState
);

/*!
 \brief          Prepares system to enter to defined performance state.
 \param[in]      state the state requested:
 KEAPI_PM_P0 - max power and frequency
 KEAPI_PM_P1 - less than P0, voltage/frequency scaled
 KEAPI_PM_P2
 KEAPI_PM_P3
 ...
 KEAPI_PM_P16 - less than P15, voltage/frequency scaled
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiSetPerformanceState(KEAPI_PRM_START
	uint32_t state
);

/*/////////////////////////////////////////////////////////////////////////////
 // Temperature sensors information
 ///////////////////////////////////////////////////////////////////////////// */

/*!
 \brief          Provides number of temperature sensors
 \param[out]     pTempSensorCount Pointer to number of installed temperature sensors
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiGetTempSensorCount(KEAPI_PRM_START
	int32_t *pTempSensorCount
);


/*!
 \brief          Derives information about current value and status of the temperature sensor with a given ID
 \param[in]      sensorNr Number (index) of a temperature sensor. Numbers start with 0
 \param[out]     pSensorValue Pointer to the value structure to store sensor status and
		 value (in 1/1000 Celsius, negative values allowed)
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiGetTempSensorValue(KEAPI_PRM_START
	int32_t sensorNr,
	PKEAPI_SENSOR_VALUE pSensorValue
);


/*!
 \brief          Provides information about temperature sensors (current value and status), each stored in the KEAPI_SENSOR_VALUE structure
 \param[out]     pValues Buffer to store value list
 \param[in]      sensorCount Number of temperature sensors (size of buffer / sizeof(KEAPI_SENSOR_VALUE))
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiGetTempSensorValueList(KEAPI_PRM_START
	PKEAPI_SENSOR_VALUE pSensorValues,
	int32_t sensorCount
);

/*!
 \brief          Derives detailed information sensor settings
 \param[in]      sensorNr Number (index) of a temperature sensor. Numbers start with 0
 \param[out]     pSensorInfo Buffer to store the info
 	 	 	 	 values are in 1/1000 Celsius (negative values allowed),
 	 	 	 	 fields which do not provide valid information are set to KEAPI_SENSOR_INFO_UNKNOWN
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiGetTempSensorInfo(KEAPI_PRM_START
	int32_t sensorNr,
	PKEAPI_SENSOR_INFO pSensorInfo
);


/*/////////////////////////////////////////////////////////////////////////////
 // Voltage sensors information
 ///////////////////////////////////////////////////////////////////////////// */

/*!
 \brief          Provides number of Voltage sensors
 \param[out]     pVoltageSensorCount Pointer to number of installed Voltage sensors
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiGetVoltageSensorCount(KEAPI_PRM_START
	int32_t *pVoltageSensorCount
);


/*!
 \brief          Derives information about current value and status of the Voltage sensor with a given ID
 \param[in]      sensorNr Number (index) of a Voltage sensor. Numbers start with 0
 \param[out]     pSensorValue Pointer to the value structure to store sensor status and
 	 	 	 	 value (in 1/1000 Volts)
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiGetVoltageSensorValue(KEAPI_PRM_START
	int32_t sensorNr,
	PKEAPI_SENSOR_VALUE pSensorValue
);


/*!
 \brief          Provides information about Voltage sensors (current value and status), each stored in the KEAPI_SENSOR_VALUE structure
 \param[out]     pValues Buffer to store value list
 \param[in]      sensorCount Number of Voltage sensors (size of buffer / sizeof(KEAPI_SENSOR_VALUE))
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiGetVoltageSensorValueList(KEAPI_PRM_START
	PKEAPI_SENSOR_VALUE pSensorValues,
	int32_t sensorCount
);

/*!
 \brief          Derives detailed information sensor settings
 \param[in]      sensorNr Number (index) of a temperature sensor. Numbers start with 0
 \param[out]     pSensorInfo Buffer to store the info
 	 	 	 	 values are in 1/1000 Volts,
 	 	 	 	 fields which do not provide valid information are set to KEAPI_SENSOR_INFO_UNKNOWN
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiGetVoltageSensorInfo(KEAPI_PRM_START
	int32_t sensorNr,
	PKEAPI_SENSOR_INFO pSensorInfo
);


/*/////////////////////////////////////////////////////////////////////////////
 // Fan control
 ///////////////////////////////////////////////////////////////////////////// */

/*!
 \brief          Provides number of Fan sensors
 \param[out]     pFanSensorCount Pointer to number of installed Fan sensors
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiGetFanSensorCount(KEAPI_PRM_START
	int32_t *pFanSensorCount
);


/*!
 \brief          Derives information about current value and status of the Fan sensor with a given ID
 \param[in]      sensorNr Number (index) of a Fan sensor. Numbers start with 0
 \param[out]     pSensorValue Pointer to the value structure to store sensor status and
 	 	 	 	 value (in RPMs, revolutions-per-minute)
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiGetFanSensorValue(KEAPI_PRM_START
	int32_t sensorNr,
	PKEAPI_SENSOR_VALUE pSensorValue
);


/*!
 \brief          Provides information about Fan sensors (current value and status), each stored in the KEAPI_SENSOR_VALUE structure
 \param[out]     pValues Buffer to store value list
 \param[in]      sensorCount Number of Fan sensors (size of buffer / sizeof(KEAPI_SENSOR_VALUE))
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiGetFanSensorValueList(KEAPI_PRM_START
	PKEAPI_SENSOR_VALUE pSensorValues,
	int32_t sensorCount
);

/*!
 \brief          Derives detailed information sensor settings
 \param[in]      sensorNr Number (index) of a temperature sensor. Numbers start with 0
 \param[out]     pSensorInfo Buffer to store the info
 	 	 	 	 values are in RPMs (revolutions-per-minute),
 	 	 	 	 fields which do not provide valid information are set to KEAPI_SENSOR_INFO_UNKNOWN
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiGetFanSensorInfo(KEAPI_PRM_START
	int32_t sensorNr,
	PKEAPI_SENSOR_INFO pSensorInfo
);


/*/////////////////////////////////////////////////////////////////////////////
 // Display
 ///////////////////////////////////////////////////////////////////////////// */
/*!
 \brief          Provides number of installed displays
 \param[out]     pDisplayCount Number of installed displays
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiGetDisplayCount(KEAPI_PRM_START
	int32_t *pDisplayCount
);


/*!
 \brief          Provides information about current backlight intensity of the selected display.
 \param[in]      displayNr Requested display's number. Numbers start with zero
 \param[out]     pValue Pointer to variable that receives actual brightness intensity.
 	 	 	 	 The value ranges from 0 to KEAPI_DISPLAY_BRIGHTNESS_MAX (255).
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiGetBacklightValue(KEAPI_PRM_START
	int32_t displayNr,
	int32_t *pValue
);

/*!
 \brief          Enables backlight and sets backlight intensity of the selected display. Value 0 sets backlight to OFF.
 \param[in]      displayNr Requested display's number. Numbers start with zero
 \param[in]      value Backlight intensity.
 	 	 	 	 The value ranges from 0 to KEAPI_DISPLAY_BRIGHTNESS_MAX (255).
 	 	 	 	 Value 0 sets backlight to OFF.
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiSetBacklightValue(KEAPI_PRM_START
	int32_t displayNr,
	int32_t value
);

/*/////////////////////////////////////////////////////////////////////////////
 // Network and PCI devices info
 ///////////////////////////////////////////////////////////////////////////// */

/*!
 \brief          Provides a number of installed network devices

 \param[out]     pNetworkDeviceCount Number of installed network devices
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiGetNetworkDeviceCount(KEAPI_PRM_START
	int32_t *pNetworkDeviceCount
);


/*!
 \brief          Provides a list of installed network devices information
 \param[in]      networkDeviceCount Number of network devices
 \param[out]     pNetworkDevices Pointer to array of KEAPI_NETWORK_DEVICE structures.
 The array must be allocated as NetworkDeviceCount * sizeof(KEAPI_NETWORK_DEVICE),
 where NetworkDeviceCount is obtaind by calling KEApiGetNetworkDeviceCount.
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiGetNetworkDeviceList(KEAPI_PRM_START
	PKEAPI_NETWORK_DEVICE pNetworkDevices,
	int32_t networkDeviceCount
);


/*!
 \brief          Provides a number of installed PCI devices

 \param[out]     pPciDeviceCount Number of installed PCI devices
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiGetPciDeviceCount(KEAPI_PRM_START
	int32_t *pPciDeviceCount
);


/*!
 \brief          Provides a list of PCI devices

 \param[in]      pciDeviceCount Number of installed PCI devices
 \param[out]     pPciDevices Returned array of KEAPI_PCI_DEVICE structures
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiGetPciDeviceList(KEAPI_PRM_START
	PKEAPI_PCI_DEVICE pPciDevices,
	int32_t pciDeviceCount
);

/*/////////////////////////////////////////////////////////////////////////////
 // Storage area
 ///////////////////////////////////////////////////////////////////////////// */

/*!
 \brief          Provides information about EEPROM storage areas count
 \param[out]     pStorageCount Number of available storage areas
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiGetStorageCount(KEAPI_PRM_START
	int32_t *pStorageCount
);


/*!
 \brief          Provides information about EEPROM storage area sizes
 \param[in]      storageNr Number of the storage area. Number of storage areas can be obtained from the KEApiGetStorageCount function
 \param[out]     pStorageSize  Pointer to array of sizes of available storage areas. Length of the array is StorageCount * sizeof(int32_t)
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiGetStorageSize(KEAPI_PRM_START
	int32_t storageNr,
	int32_t *pStorageSize
);


/*!
 \brief          Reads block of bytes from selected EEPROM
 \param[in]      storageNr    EEPROM storage number. Number of storage areas can be obtained from KEApiGetStorageCount function
 \param[in]      offset       Start address offset
 \param[out]     pData      Pointer to buffer that receives data
 \param[in]      dataLength    Number of bytes to read
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiStorageRead(KEAPI_PRM_START
	int32_t storageNr,
	int32_t offset,
	uint8_t *pData,
	int32_t dataLength
);


/*!
 \brief          Writes block of bytes to selected EEPROM
 \param[in]      storageNr    EEPROM storage number. Number of storage areas can be obtained FROM KEApiGetStorageCount function
 \param[in]      offset       Start address offset
 \param[in]      pData      Pointer to buffer that contains data to write to EEPROM
 \param[in]      dataLength    Number of bytes to write
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiStorageWrite(KEAPI_PRM_START
	int32_t storageNr,
	int32_t offset,
	uint8_t *pData,
	int32_t dataLength
);

/*/////////////////////////////////////////////////////////////////////////////
 // I2C
 ///////////////////////////////////////////////////////////////////////////// */


/*!
 \brief          Getting number of active I2C buses.

 \param[out]     pI2cBusCount Pointer to the variable where the I2C bus count is saved
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiGetI2cBusCount(KEAPI_PRM_START
	int32_t * pI2cBusCount
);


/*!
 \brief          Universal function for write-read operations to the I2C bus.
 This function performs write operation passing device address and writes data,
 then performs I2C START, transfer device address and reads data from
 the slave I2C device connected to the I2C bus. Write operation will not be
 performed if no write data is provided.
 The I2C operations sequence shall be:
 [Start<Addr Byte><W>Ack<Write Data Byte[1]>Ack
 ... < Write Data Byte[writeLength]>Ack]
 Start<Addr Byte><R>Ack<Read Data Byte[1]>Ack
 ... <Read Data Byte[readLength]>Nak Stop

 \param[in]      i2cNr Number of I2C Bus. From 0 to (I2cBusCount - 1) returned by KEApiGetI2cBusCount.
 \param[in]      i2cAddress Address of I2C slave device.
 \param[in]      pWriteData Data to write, can be NULL if writeLength == 0
 \param[in]      writeLength Length of data to write.
 \param[out]     pReadData Buffer for data to read.
 \param[inout]     pReadLength Also an 'out' parameter. When the function finishes, this parameter contains a real value of the read data length.
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiI2cXfer(KEAPI_PRM_START
	int32_t i2cNr,
	uint8_t i2cAddress,
	uint8_t *pWriteData,
	int32_t writeLength,
	uint8_t *pReadData,
	int32_t *pReadLength
);


/*!
 \brief          Probes I2C Device: Start<Addr Byte><W>Ack[<Memory Address Byte>Ack]Stop

 \param[in]      i2cNr Number of I2C Bus. From 0 to (I2cBusCount - 1) returned by KEApiGetI2cBusCount
 \param[in]      i2cAddress Address of I2C slave device
 \param[in]      memoryAddress Address of register/memory (10-bit I2C addressing or register address inside the I2C device).
 \param[in]      memoryAddressUsed If TRUE - memoryAddress data has to be written to I2C.
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_ERROR: no such device
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiI2cProbe(KEAPI_PRM_START
	int32_t i2cNr,
	uint8_t i2cAddress,
	uint8_t memoryAddress,
	uint8_t memoryAddressUsed
);

/*/////////////////////////////////////////////////////////////////////////////
 // SPI
 ///////////////////////////////////////////////////////////////////////////// */


/*!
 \brief          Getting number of active SPI Buses.

 \param[out]     pSpiBusCount Pointer to variable to save SpiBus count
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiGetSpiBusCount(KEAPI_PRM_START
	int32_t *pSpiBusCount
);


/*!
 \brief          Exchange SPI data. This function transfers data between
 the master SPI bus controller and a slave SPI device.
 Each transfer consists of a command word, receive data
 and transmit data. The command format is device specific.
 It typically directs I/O to a specific register set within
 the device and may identify a data transfer direction
 (input/output). If the transmit buffer is null, zeros are
 sent out to the slave. If the receive buffer is null,
 no data is read. The transfer starts by asserting the chip
 select code as defined in the deviceId. The command word is
 then transmitted to the slave, command word size is specified
 in bytes, 0 value in commandSize defines no use of command
 word. Data provided in the transmit buffer is sent out on
 the SPI MOSI line and receive data provided by the slave
 by MISO line is captured in the receive buffer.
 The function returns when the number of payload transfers
 have completed.

 \param[in]      spiNr I2C Bus id. From 0 to (spiBusCount - 1) returned by
 KEApiGetSpiBusCount.
 \param[in]      deviceId device number (chip select)
 \param[in]      command command word
 \param[in]      commandSize command word size in bytes
 (lowest bits of 32-bit word are used)
 \param[in]      numTransfers total number of transfers excluding command
 \param[in]      pWriteData data to write (of numTransfers size), NULL means read-only operation
 \param[out]     pReadData read data buffer (of numTransfers size), NULL means write-only operation
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiSpiXfer(KEAPI_PRM_START
	int32_t spiNr,
	uint16_t deviceId,
	uint32_t command,
	uint8_t commandSize,
	int32_t numTransfers,
	uint8_t *pWriteData,
	uint8_t *pReadData
);

/*/////////////////////////////////////////////////////////////////////////////
 // SMBus
 ///////////////////////////////////////////////////////////////////////////// */

KEAPI_CALLTYPE KEAPI_RETVAL KEApiGetSmbusCount(KEAPI_PRM_START
	int32_t *pSmbusCount
);


KEAPI_CALLTYPE KEAPI_RETVAL KEApiSmbusQuickCommand(KEAPI_PRM_START
	int32_t smbusNr,
	uint8_t smbusAddress,
	uint8_t operation
);


KEAPI_CALLTYPE KEAPI_RETVAL KEApiSmbusSendByte(KEAPI_PRM_START
	int32_t smbusNr,
	uint8_t smbusAddress,
	uint8_t byte
);


KEAPI_CALLTYPE KEAPI_RETVAL KEApiSmbusReceiveByte(KEAPI_PRM_START
	int32_t smbusNr,
	uint8_t smbusAddress,
	uint8_t *pByte
);


KEAPI_CALLTYPE KEAPI_RETVAL KEApiSmbusWriteByte(KEAPI_PRM_START
	int32_t smbusNr,
	uint8_t smbusAddress,
	uint8_t command,
	uint8_t byte
);


KEAPI_CALLTYPE KEAPI_RETVAL KEApiSmbusReadByte(KEAPI_PRM_START
	int32_t smbusNr,
	uint8_t smbusAddress,
	uint8_t command,
	uint8_t *pByte
);


KEAPI_CALLTYPE KEAPI_RETVAL KEApiSmbusWriteWord(KEAPI_PRM_START
	int32_t smbusNr,
	uint8_t smbusAddress,
	uint8_t command,
	uint16_t word
);


KEAPI_CALLTYPE KEAPI_RETVAL KEApiSmbusReadWord(KEAPI_PRM_START
	int32_t smbusNr,
	uint8_t smbusAddress,
	uint8_t command,
	uint16_t *pWord
);


KEAPI_CALLTYPE KEAPI_RETVAL KEApiSmbusWriteBlock(KEAPI_PRM_START
	int32_t smbusNr,
	uint8_t smbusAddress,
	uint8_t command,
	uint8_t *pData,
	int8_t dataLength
);


KEAPI_CALLTYPE KEAPI_RETVAL KEApiSmbusReadBlock(KEAPI_PRM_START
	int32_t smbusNr,
	uint8_t smbusAddress,
	uint8_t command,
	uint8_t *pData,
	int8_t *pDataLength
);

/*/////////////////////////////////////////////////////////////////////////////
 // GPIO
 ///////////////////////////////////////////////////////////////////////////// */

KEAPI_CALLTYPE KEAPI_RETVAL KEApiGetGpioPortCount(KEAPI_PRM_START
	int32_t *pCount
);

KEAPI_CALLTYPE KEAPI_RETVAL KEApiGetGpioPortDirectionCaps(KEAPI_PRM_START
	int32_t portNr,
	uint32_t *pIns,
	uint32_t *pOuts
);

KEAPI_CALLTYPE KEAPI_RETVAL KEApiGetGpioPortDirections(KEAPI_PRM_START
	int32_t portNr,
	uint32_t *pDirections
);

KEAPI_CALLTYPE KEAPI_RETVAL KEApiSetGpioPortDirections(KEAPI_PRM_START
	int32_t portNr,
	uint32_t directions
);

KEAPI_CALLTYPE KEAPI_RETVAL KEApiGetGpioPortLevels(KEAPI_PRM_START
	int32_t portNr,
	uint32_t *pLevels
);

KEAPI_CALLTYPE KEAPI_RETVAL KEApiSetGpioPortLevels(KEAPI_PRM_START
	int32_t portNr,
	uint32_t levels
);

/*/////////////////////////////////////////////////////////////////////////////
 // Watchdog
 ///////////////////////////////////////////////////////////////////////////// */

KEAPI_CALLTYPE KEAPI_RETVAL KEApiWatchdogGetCaps(KEAPI_PRM_START
	int32_t *pMaxTimeout,
	int32_t *pMinTimeout,
	int32_t *pStagesNr
);

KEAPI_CALLTYPE KEAPI_RETVAL KEApiWatchdogSetup(KEAPI_PRM_START
	int32_t stage,
	int32_t timeout,
	int32_t mode
);

KEAPI_CALLTYPE KEAPI_RETVAL KEApiWatchdogEnable(KEAPI_PRM_VOID);

KEAPI_CALLTYPE KEAPI_RETVAL KEApiWatchdogTrigger(KEAPI_PRM_VOID);

KEAPI_CALLTYPE KEAPI_RETVAL KEApiWatchdogDisable(KEAPI_PRM_VOID);

KEAPI_CALLTYPE KEAPI_RETVAL KEApiWatchdogGetExpired (KEAPI_PRM_START
		int32_t *pWTE);

KEAPI_CALLTYPE KEAPI_RETVAL KEApiWatchdogClearExpired (KEAPI_PRM_VOID);

KEAPI_CALLTYPE KEAPI_RETVAL KEApiWatchdogWaitUntilExpired (KEAPI_PRM_VOID);

/*/////////////////////////////////////////////////////////////////////////////
 // ADDON LED
 ///////////////////////////////////////////////////////////////////////////// */
/*!
 \brief          reading number of LEDs

 \param[out]     pLedCount number of LEDs from this device
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiLedGetCount(KEAPI_PRM_START
    int32_t *pLedCount);

/*!
 \brief          reding LED status

 \param[in]      ledNb   index of LED (0..MAX_NB_LED-1)
 \param[out]     pStatus 0:off 1:on
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiLedGetStatus(KEAPI_PRM_START
    int32_t ledNb, int32_t *pStatus);

/*!
 \brief          setting LED

 \param[in]      ledNb  index of LED (0..MAX_NB_LED-1)
 \param[in]      status 0:off 1:on
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiLedSetStatus(KEAPI_PRM_START
    int32_t ledNb, int32_t status);

typedef struct Keapi_LED_Config {
	uint32_t Colour; /* 1:RED 2:YELLOW 4:GREEN 8:BLUE 0x10:WHITE 0x300:AMPER 0x10000000:RGB  */
	uint32_t Light; /* 0:LIGHT_PERMANENT       unsupported 1:BLINKING 2:FLASHING  */
	uint32_t Mode; /* 0:user mode 1:system mode */
	uint32_t TOn; /* only for blinking/flashing */
	uint32_t TOff; /* only for blinking/flashing  */
} KEAPI_LED_CONFIG, *PKEAPI_LED_CONFIG;

/*!
 \brief          setting status of LED

 \param[in]      ledNb  index of LED (0..MAX_NB_LED-1)
 \param[in]      Config KEAPI_LED_CONFIG structure representing configuration of LED
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiLedSetConfig(KEAPI_PRM_START
    int32_t ledNb, KEAPI_LED_CONFIG Config);

/*!
 \brief          getting status of LED

 \param[in]      ledNb  index of LED (0..MAX_NB_LED-1)
 \param[out]     pConfig return PKEAPI_LED_CONFIG structure; representing configuration of LED
 \return
 * - KEAPI_RET_SUCCESS on success
 * - KEAPI_RET_NOT_INITIALIZED when library is not initialized
 */
KEAPI_CALLTYPE KEAPI_RETVAL KEApiLedGetConfig(KEAPI_PRM_START
    int32_t ledNb, PKEAPI_LED_CONFIG pConfig);

#ifdef __cplusplus
} // #ifdef __cplusplus
#endif

#endif  /* KEAPI_H */
