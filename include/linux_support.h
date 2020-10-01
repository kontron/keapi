/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron Europe GmbH */

/* KEAPI predefined constants and helpers declaration for Linux. */

#include "pthread.h"

#ifndef LINUXSUPPORT_H
#define LINUXSUPPORT_H

#include <linux/limits.h>

#define KEAPI_LINUX_VERSION_MAJOR (0x06)
#define KEAPI_LINUX_VERSION_MINOR (0x00)

/* Linux predefined constants */
#define SOCKET_PATH "/var/run/keapi_socket"
#define CPUINFO_PATH "/proc/cpuinfo"
#define UPTIME_PATH "/proc/uptime"
#define WATCHDOG_PATH "/dev/watchdog"
#define BATTERY_PATH "/proc/acpi/battery"
#define DISPLAY_PATH "/sys/class/backlight"
#define PCI_PATH "/sys/bus/pci/devices"
#define PCI_IDS_PATH "/usr/share/hwdata/pci.ids"
#define MOUNT_TAB_PATH "/etc/mtab"

#define CPU_MIN_FREQ_PATH "/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_min_freq"
#define CPU_MAX_FREQ_PATH "/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq"
#define SCALING_AVAILFREQ_PATH "/sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies"
#define CPU_CORE_SCALING_PATH "/sys/devices/system/cpu/cpu%d/cpufreq/%s"
#define CPU_INTEL_PSTATE_PATH "/sys/devices/system/cpu/intel_pstate/%s_perf_pct"
#define CPU_INTEL_PSTATE_TURBO "/sys/devices/system/cpu/intel_pstate/no_turbo"
#define CPU_CORE_TURBO_PATH "/sys/devices/system/cpu/cpufreq/boost"

#define MEMORY_INFO_PATH "/proc/meminfo"
#define MEMORY_CONFIG_PATH "/etc/keapi/mem.conf"
#define BCKL_CONF_PATH "/etc/keapi/backlight.conf"
#define I2C_CONF_PATH "/etc/keapi/i2c.conf"
#define SMBUS_CONF_PATH "/etc/keapi/smbus.conf"
#define SPI_CONF_PATH "/etc/keapi/spi.conf"

#define WDTCONF_PATH "/etc/keapi/watchdog.conf"
#define GPIOCONF_PATH "/etc/keapi/gpio.conf"
#define SYS_PERF_CONF_PATH "/etc/keapi/systemstates.conf"
#define BOOT_COUNT_PATH "/etc/keapi/bootcount0"
#define INTRUDER_PATH "/etc/keapi/intruder"
#define STORAGE_EEPROM_PATH "/etc/keapi/storage.conf"

#define SENSORSCONF_PATH "/etc/keapi/sensors.conf"
#define SENSORSCONFDIR_PATH "/etc/sensors.d/kontron-keapi.conf"
#define SENSORSCONFLEGACY_PATH "/etc/sensors3.conf"

#define PCRE_OVECSIZE 60

#define I2C_CONFTYPE 0
#define SMBUS_CONFTYPE 1

#define GPIO_PER_PORT 32 //maximal gpio per port

enum FeatureStyle {
	LINUX_LIKE = 1,
	KONTRON_LIKE_KEM,
	KONTRON_LIKE_KOM,
	KEAPI_FULL,
	EN01_152
};

/* Linux helper functions */
int openLock(char *path, pthread_mutex_t *mutex);
void closeUnlock(int fd, pthread_mutex_t *mutex);
int8_t MachineIsArm(void);
int8_t MachineIsArm64(void);
int8_t MachineIsX86(void);
int32_t checkRAccess(char *path);
int32_t checkRWAccess(char *path);
int32_t failIoctlCheckErrno(int *file);
int32_t findRemapping(char *where, char list[][PATH_MAX], int32_t *count);
int32_t GetInfoFromSerialBusConfFile(char sbType);
int32_t GetStorageConfig(void);
int8_t IsSensorIgnored(char *hwmonName, char *sensorName);
uint32_t getDmiTable(char **table, uint16_t *table_len, uint16_t *struct_count);
int32_t ReadFile(char *filename, char **outbuff);
int32_t GetExternalCommandOutput(char *command, char **data);
int32_t FindRegex(char *str, char *regex, int8_t *found, int32_t flags);
int32_t GetSubStrRegex(char *str, char *regex, char **substr, int32_t flags);
char *GetSensorsConfName(void);

#endif /* LINUXSUPPORT_H */
