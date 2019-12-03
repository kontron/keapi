/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron Europe GmbH */

/* KEAPI CPU info and performance functions. */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <regex.h>
#include <jansson.h>

#include "keapi_inc.h"
#include "cpu_arm.h"

#define MAX_AVAIL_FREQ		20
#define MAX_AVAIL_CPU		32

#define MAX_STATE_QUANTITY	16
#define MAX_PARAM_QUANTITY	MAX_STATE_QUANTITY

/* MSR offsets */
#define MSR_FSB_FREQ		0x0cd
#define MSR_PLATFORM_INFO	0x0ce
/* MSR register size */
#define MSR_REG_SIZE		8
#define CPUID_REG_SIZE		16

#define SLM_BCLK_FREQS		5
#define AIM_BCLK_FREQS		9
double slm_freqs[SLM_BCLK_FREQS] = { 83.3, 100.0, 133.3, 116.7, 80.0 };
double aim_freqs[AIM_BCLK_FREQS] = { 83.3, 100, 133.3, 116.7, 80, 93.3, 90, 88.9, 87.5 };

#define INTEL_FAMILY_6			0x06 /* Most of Intel CPUs */
#define INTEL_FAMILY_F			0x0f /* Old Pentium and Xeon, pre Core iX */

#define INTEL_FAM6_ATOM_SILVERMONT1	0x37
#define INTEL_FAM6_ATOM_SILVERMONT2	0x4D
#define INTEL_FAM6_ATOM_AIRMONT		0x4C
#define INTEL_FAM6_ATOM_GOLDMONT	0x5C

/*******************************************************************************/
KEAPI_RETVAL KEApiL_GetCpuInfo(PKEAPI_CPU_INFO pCpuInfo)
{
	uint32_t retval, i, cpu_count, ret;
	uint16_t table_len, struct_count;
	char *table, *str, str_type, str_len, index;
	char *res, path[KEAPI_MAX_STR];
	int ver = -1, rev = -1, impl = -1, part = -1;
	const char *arm_name, *arm_part;
	char cpu_name[KEAPI_MAX_STR];
	const struct id_part *parts = NULL;

	retval = KEAPI_RET_SUCCESS;

	/* Check function parameters */
	if (pCpuInfo == NULL)
		return KEAPI_RET_PARAM_NULL;

	/* Initialization */
	pCpuInfo->cpuCoreCount = 0;
	pCpuInfo->cpuCount = 0;
	pCpuInfo->cpuName[0] = '\0';
	pCpuInfo->cpuThreadCount = 0;

	/* Try to get the most information from sysfs in case we don't have DMI */
	/* get CPU name on ARM */
	if (MachineIsArm()) {
		char *substr;

		if ((ret = checkRAccess(CPUINFO_PATH)) != KEAPI_RET_SUCCESS)
			return ret;

		if ((ret = ReadFile(CPUINFO_PATH, &res)) != KEAPI_RET_SUCCESS)
			return ret;

		/* Get CPU name */
		if (MachineIsArm64()) {
			/* Get CPU arch version */
			if (GetSubStrRegex(res, "CPU architecture[ \t]*: (.*)", &substr, REG_EXTENDED | REG_NEWLINE) ==
			    KEAPI_RET_SUCCESS) {
				ver = strtol(substr, NULL, 0);
				free(substr);
			}
			/* Get CPU implementer (e.g.: ARM) */
			if (GetSubStrRegex(res, "CPU implementer[ \t]*: (.*)", &substr, REG_EXTENDED | REG_NEWLINE) ==
			    KEAPI_RET_SUCCESS) {
				impl = strtol(substr, NULL, 0);
				free(substr);
			}
			/* Get CPU part */
			if (GetSubStrRegex(res, "CPU part[ \t]*: (.*)", &substr, REG_EXTENDED | REG_NEWLINE) ==
			    KEAPI_RET_SUCCESS) {
				part = strtol(substr, NULL, 0);
				free(substr);
			}
			/* Get CPU revision */
			if (GetSubStrRegex(res, "CPU revision[ \t]*: (.*)", &substr, REG_EXTENDED | REG_NEWLINE) ==
			    KEAPI_RET_SUCCESS) {
				rev = strtol(substr, NULL, 0);
				free(substr);
			}
			/* Lookup CPU implementer and part names */
			for (i = 0; hw_implementer[i].id != -1; i++) {
				if (hw_implementer[i].id == impl)
					break;
			}
			parts = hw_implementer[i].parts;
			arm_name = hw_implementer[i].name;

			for (i = 0; parts[i].id != -1; i++) {
				if (parts[i].id == part)
					break;
			}
			arm_part = parts[i].name;

			snprintf(cpu_name, KEAPI_MAX_STR - 1, "ARMv%d rev %d (aarch64) %s %s CPU", ver, rev, arm_name,
				 arm_part);
			strncat(pCpuInfo->cpuName, cpu_name, KEAPI_MAX_STR - 1);
		} else {
			if (GetSubStrRegex(res, "model name\t: (.*)", &substr, REG_EXTENDED | REG_NEWLINE) ==
			    KEAPI_RET_SUCCESS) {
				strncat(pCpuInfo->cpuName, substr, KEAPI_MAX_STR - 1);
				free(substr);
			}
		}

		free(res);
	}

	/* Count CPUs/cores (linux doesn't have a difference between them) */
	cpu_count = 0;
	snprintf(path, KEAPI_MAX_STR, "/sys/devices/system/cpu/cpu%d", cpu_count);

	while (access(path, F_OK) == 0) {
		cpu_count++;
		snprintf(path, KEAPI_MAX_STR, "/sys/devices/system/cpu/cpu%d", cpu_count);
	};

	if (cpu_count == 0)
		return KEAPI_RET_RETRIEVAL_ERROR;

	/* x86 will use DMI below, for others hardcode 1 CPU for now, and return
	 * number of counted cores. Also hardcode 1 thread per core.
	 * TODO: use /sys/devices/system/cpu/cpu%d/topology to calculate
	 * CPUs/cores/threads properly */
	pCpuInfo->cpuCount = 1;
	pCpuInfo->cpuCoreCount = cpu_count;
	pCpuInfo->cpuThreadCount = cpu_count;

	/* We tried hard to gather all possible info, but there's always something missing,
	 * so it's still a partial success */
	if (MachineIsX86() == FALSE)
		return KEAPI_RET_SUCCESS;

	/* GET DMI TABLE */
	if ((ret = getDmiTable(&table, &table_len, &struct_count)) != KEAPI_RET_SUCCESS)
		return ret;

	pCpuInfo->cpuCount = 0;

	/* Look for struct type */
	str = table;
	for (i = 0; i < struct_count; i++) {
		str_type = str[0x0];
		str_len = str[0x01];

		/* Processing structure */
		if (str_type == 4) /* Processor information */
		{
			pCpuInfo->cpuCount++; /* One structure for each processor */

			if (str_len > 0x23)
				pCpuInfo->cpuCoreCount = str[0x23];
			if (str_len > 0x25)
				pCpuInfo->cpuThreadCount = str[0x25];

			if (str_len < 0x23)
				retval = KEAPI_RET_PARTIAL_SUCCESS;

			/* Processor name is at position 0x10 */
			if (str[0x10] != 0) {
				index = str[0x10];
				res = str + str_len;
				while (index > 1 && *res) {
					res += strlen(res);
					res++;
					index--;
				}
				strncat(pCpuInfo->cpuName, res, KEAPI_MAX_STR - 1);
			} else
				retval = KEAPI_RET_PARTIAL_SUCCESS;
		}

		str += str_len;
		/* looking for \0\0 */
		while (str[0] != 0 || str[1] != 0)
			str++;
		str++;
		str++;
	}

	free(table);
	return retval;
}

static KEAPI_RETVAL getCpuCorePath(int32_t coreNr, char *path, int *core_nr_virt, ssize_t size)
{
	FILE *fd;
	int32_t cpu_count = 0, core_nr = -1;
	char path_tmp[KEAPI_MAX_STR];
	int core_id_curr, core_id_prev = -1;

	snprintf(path_tmp, KEAPI_MAX_STR, "/sys/devices/system/cpu/cpu%d/topology/core_id", cpu_count);

	while (access(path_tmp, F_OK) == 0) {
		if ((fd = fopen(path_tmp, "r")) == NULL)
			return KEAPI_RET_RETRIEVAL_ERROR;

		if (fscanf(fd, "%d", &core_id_curr) == EOF) {
			fclose(fd);
			return KEAPI_RET_RETRIEVAL_ERROR;
		}
		fclose(fd);
		/* Determine the number of the physical core in order (core_nr).
		 * We are based on the assumption that numbers in the core_id are written
		 * in ascending order.
		 * For more details see: https://www.kernel.org/doc/Documentation/x86/topology.txt
		 */
		if (core_id_prev < core_id_curr) {
			core_nr++;
			core_id_prev = core_id_curr;
		}

		if (core_nr == coreNr) {
			snprintf(path, size, "/sys/devices/system/cpu/cpu%d", cpu_count);
			*core_nr_virt = cpu_count;
			return KEAPI_RET_SUCCESS;
		}

		cpu_count++;
		snprintf(path_tmp, KEAPI_MAX_STR, "/sys/devices/system/cpu/cpu%d/topology/core_id", cpu_count);
	}

	return KEAPI_RET_PARAM_ERROR;
}

/*******************************************************************************/
KEAPI_RETVAL KEApiL_GetCpuFreq(int32_t coreNr, int8_t freqType, int32_t *pFrequency)
{
	/* Before CPU performance setting it is necessary to enable CPU
	 * Speed Step and C-STATE in BIOS */

	FILE *fd;
	int msr_fd, cpuid_fd, bclk_freq_id;
	int no_turbo;
	char path[KEAPI_MAX_STR];
	char msr_path[KEAPI_MAX_STR], cpuid_path[KEAPI_MAX_STR];
	int32_t ret;
	int core_nr_virt; /* Virtual core number in sysfs */
	int msr_stat = 0;
	int treat_turbo_as_max = 0;
	uint64_t msr_val;
	uint64_t msr_fsb_freq;
	ssize_t ret_msr;
	double fsb_freq = 133.33; /* default FSB for old Intel CPUs (pre Sandy Bridge) */
	unsigned int family = 0, model = 0;
	struct cpuid_regs {
		uint32_t eax;
		uint32_t ebx;
		uint32_t ecx;
		uint32_t edx;
	} cpuid_val;

	/* Check function parameters */
	if (pFrequency == NULL)
		return KEAPI_RET_PARAM_NULL;

	ret = getCpuCorePath(coreNr, path, &core_nr_virt, KEAPI_MAX_STR);
	if (ret != KEAPI_RET_SUCCESS)
		return ret;

	/* Initialization */
	*pFrequency = 0;

	if (MachineIsX86() == TRUE && (freqType == KEAPI_CPU_FREQUENCY_MIN || freqType == KEAPI_CPU_FREQUENCY_MAX ||
				       freqType == KEAPI_CPU_FREQUENCY_TURBO)) {
		/* Try to take CPU model and family from CPUID */
		snprintf(cpuid_path, KEAPI_MAX_STR, "/dev/cpu/%d/cpuid", core_nr_virt);
		cpuid_fd = open(cpuid_path, O_RDONLY);
		if (cpuid_fd != -1) {
			/* Read family and model from CPUID */
			ret_msr = pread(cpuid_fd, &cpuid_val, CPUID_REG_SIZE, 1);
			if (ret_msr == CPUID_REG_SIZE) {
				msr_stat = 1;
				family = (cpuid_val.eax >> 8) & 0xf;
				model = (cpuid_val.eax >> 4) & 0xf;
				if (family == INTEL_FAMILY_6 || family == INTEL_FAMILY_F)
					model += ((cpuid_val.eax >> 16) & 0xf) << 4;
			}
			close(cpuid_fd);
		}
		/* Try to read MSR registers MSR_PLATFORM_INFO */
		snprintf(msr_path, KEAPI_MAX_STR, "/dev/cpu/%d/msr", core_nr_virt);
		msr_fd = open(msr_path, O_RDONLY);
		if (msr_fd != -1) {
			ret_msr = pread(msr_fd, &msr_val, MSR_REG_SIZE, MSR_PLATFORM_INFO);
			if (ret_msr == MSR_REG_SIZE)
				msr_stat = 1;
			else
				msr_stat = 0;
		}
		if (model == INTEL_FAM6_ATOM_GOLDMONT)
			treat_turbo_as_max = 1;
		/* Calc FSB */
		if (msr_stat) {
			if (model == INTEL_FAM6_ATOM_SILVERMONT1 || model == INTEL_FAM6_ATOM_SILVERMONT2 ||
			    model == INTEL_FAM6_ATOM_AIRMONT) {
				/* Value from MSR_FSB_FREQ needs for FSB on BayTrail and Braswell */
				ret_msr = pread(msr_fd, &msr_fsb_freq, MSR_REG_SIZE, MSR_FSB_FREQ);
				if (ret_msr == MSR_REG_SIZE) {
					bclk_freq_id = msr_fsb_freq & 0xf;
					if (model == INTEL_FAM6_ATOM_SILVERMONT1 ||
					    model == INTEL_FAM6_ATOM_SILVERMONT2) {
						if (bclk_freq_id >= SLM_BCLK_FREQS)
							msr_stat = 0;
						else
							fsb_freq = slm_freqs[bclk_freq_id];
					} else if (model == INTEL_FAM6_ATOM_AIRMONT) {
						if (bclk_freq_id >= AIM_BCLK_FREQS)
							msr_stat = 0;
						else
							fsb_freq = aim_freqs[bclk_freq_id];
					}
				} else
					msr_stat = 0;
			} else {
				/* Common value for modern Intel CPUs (beginning with Sandy Bridge) */
				fsb_freq = 100.0;
			}
		}
		close(msr_fd);
	}

	switch (freqType) {
	case KEAPI_CPU_FREQUENCY_MIN:
		if (msr_stat) {
			/* MSR_ratio * FSB freq * 1000 */
			*pFrequency = ((msr_val >> 40) & 0xFF) * fsb_freq * 1000;
			return KEAPI_RET_SUCCESS;
		} else
			strcpy(path, CPU_MIN_FREQ_PATH);
		break;
	case KEAPI_CPU_FREQUENCY_MAX:
		if (msr_stat && treat_turbo_as_max != 1) {
			*pFrequency = ((msr_val >> 8) & 0xFF) * fsb_freq * 1000;
			return KEAPI_RET_SUCCESS;
		} else
			strcpy(path, CPU_MAX_FREQ_PATH);
		break;
	case KEAPI_CPU_FREQUENCY_CURRENT:
		/* check for cpuinfo_cur_freq is present, see for details:
		 * http://elixir.free-electrons.com/linux/v4.14.2/source/Documentation/admin-guide/pm/cpufreq.rst
		 */
		snprintf(path, KEAPI_MAX_STR, CPU_CORE_SCALING_PATH, core_nr_virt, "cpuinfo_cur_freq");
		ret = checkRAccess(path);
		if (ret == KEAPI_RET_RETRIEVAL_ERROR)
			snprintf(path, KEAPI_MAX_STR, CPU_CORE_SCALING_PATH, core_nr_virt, "scaling_cur_freq");
		break;
	case KEAPI_CPU_FREQUENCY_TURBO:
		if (MachineIsX86() == TRUE) {
			if (treat_turbo_as_max)
				return KEAPI_RET_FUNCTION_NOT_SUPPORTED;

			strcpy(path, CPU_INTEL_PSTATE_TURBO);
			ret = checkRAccess(path);
			if (ret == KEAPI_RET_SUCCESS) {
				if ((fd = fopen(path, "r")) == NULL)
					return KEAPI_RET_ERROR;

				if (fscanf(fd, "%d", &no_turbo) == EOF) {
					fclose(fd);
					return KEAPI_RET_ERROR;
				}

				if (no_turbo)
					return KEAPI_RET_FUNCTION_NOT_SUPPORTED;

				fclose(fd);
				/* If P-State not supported no_turbo will not be present,
				 * skip this error and try to use cpufreq
				 */
			} else if (ret != KEAPI_RET_RETRIEVAL_ERROR)
				return ret;
			else {
				strcpy(path, CPU_CORE_TURBO_PATH);
				ret = checkRAccess(path);
				if (ret == KEAPI_RET_SUCCESS) {
					if ((fd = fopen(path, "r")) == NULL)
						return KEAPI_RET_ERROR;

					if (fscanf(fd, "%d", &no_turbo) == EOF) {
						fclose(fd);
						return KEAPI_RET_ERROR;
					}

					if (!no_turbo)
						return KEAPI_RET_FUNCTION_NOT_SUPPORTED;
				} else if (ret == KEAPI_RET_RETRIEVAL_ERROR)
					return KEAPI_RET_FUNCTION_NOT_SUPPORTED;
				else {
					return ret;
				}
			}

			strcpy(path, CPU_MAX_FREQ_PATH);
		} else
			return KEAPI_RET_FUNCTION_NOT_SUPPORTED;
		break;
	default:
		return KEAPI_RET_PARAM_ERROR;
	}

	if ((ret = checkRAccess(path)) != KEAPI_RET_SUCCESS)
		return ret;

	if ((fd = fopen(path, "r")) == NULL)
		return KEAPI_RET_ERROR;

	if (fscanf(fd, "%d", pFrequency) == EOF) {
		fclose(fd);
		return KEAPI_RET_ERROR;
	}
	fclose(fd);

	return KEAPI_RET_SUCCESS;
}

#define CPU_INTEL_PSTATE_MAX 100
#define CPU_INTEL_PSTATE_MIN 101

/*******************************************************************************/
KEAPI_RETVAL setCpu(int8_t coreNum, int8_t freqType, int32_t frequency)
{
	FILE *fr;
	char path[KEAPI_MAX_STR];
	int32_t ret;

	switch (freqType) {
	case KEAPI_CPU_FREQUENCY_MIN:
		snprintf(path, KEAPI_MAX_STR, CPU_CORE_SCALING_PATH, coreNum, "scaling_min_freq");
		break;
	case KEAPI_CPU_FREQUENCY_MAX:
		snprintf(path, KEAPI_MAX_STR, CPU_CORE_SCALING_PATH, coreNum, "scaling_max_freq");
		break;
	case KEAPI_CPU_FREQUENCY_CURRENT:
		snprintf(path, KEAPI_MAX_STR, CPU_CORE_SCALING_PATH, coreNum, "scaling_setspeed");
		break;

	/* the following 2 cases were introduces for Intel PSTATE driver and these defs
	 * are missed in keapi.h */
	case CPU_INTEL_PSTATE_MAX:
		snprintf(path, KEAPI_MAX_STR, CPU_INTEL_PSTATE_PATH, "max");
		break;
	case CPU_INTEL_PSTATE_MIN:
		snprintf(path, KEAPI_MAX_STR, CPU_INTEL_PSTATE_PATH, "min");
		break;
		/* *** */

	default:
		return KEAPI_RET_PARAM_ERROR;
	}

	if ((ret = checkRWAccess(path)) != KEAPI_RET_SUCCESS)
		return ret;

	if ((fr = fopen(path, "w")) == NULL)
		return KEAPI_RET_ERROR;

	if (fprintf(fr, "%d", frequency) == EOF) {
		fclose(fr);
		return KEAPI_RET_SETTING_ERROR;
	}
	fclose(fr);

	return KEAPI_RET_SUCCESS;
}

/*******************************************************************************/
#if 0
KEAPI_RETVAL KEApiL_GetAvailFreq(uint32_t availFrequency[]) {

	int32_t i = 0, j, k, hold, ret;
	char scaled_avail_str[KEAPI_MAX_STR];
	FILE *fr;

	if ((ret = checkRWAccess(SCALING_AVAILFREQ_PATH)) != KEAPI_RET_SUCCESS)
		return ret;

	if ((fr = fopen(SCALING_AVAILFREQ_PATH, "r")) == NULL)
		return KEAPI_RET_ERROR;

	while (fscanf(fr, "%s", scaled_avail_str) != EOF) {
		availFrequency[i++] = atoi(scaled_avail_str);
	}

	availFrequency[i] = 0;

	/* On ARM (or some x86 drivers) frequencies comes in ascending order */
	if (availFrequency[0] < availFrequency[i - 1]) {
		for (j = 0; j < i; j++)
			for (k = j + 1; k < i; k++)
				if (availFrequency[j] < availFrequency[k]) {
					hold = availFrequency[j];
					availFrequency[j] = availFrequency[k];
					availFrequency[k] = hold;
				}
	}

	fclose(fr);
	return KEAPI_RET_SUCCESS;
}
#endif
/*******************************************************************************/

enum ptypes { cpuGovernor = 1, cpuFrequency, cpuIPstMax, cpuIPstMin };

struct sstate {
	char id;
	char description[KEAPI_MAX_STR];
	int paramCount;
	char paramType[MAX_PARAM_QUANTITY];
	char paramValue[MAX_PARAM_QUANTITY][KEAPI_MAX_STR];
};

char getTypeId(char *name)
{
	if (strcmp(name, "cpugov") == 0)
		return ((char)cpuGovernor);
	else if (strcmp(name, "cpufreq") == 0)
		return ((char)cpuFrequency);
	else if (strcmp(name, "cpuipstmax") == 0)
		return ((char)cpuIPstMax);
	else if (strcmp(name, "cpuipstmin") == 0)
		return ((char)cpuIPstMin);

	return 0;
}

#define numberToMask(x) (1 << (x - 1))

/*******************************************************************************/
KEAPI_RETVAL KEApiL_GetCpuPerformance(int32_t coreNr, int8_t *pPerformancePercentage)
{
	int32_t maxFrequency, currFrequency, ret, cpu_count = 0;
	char path[KEAPI_MAX_STR];
	int16_t PerformancePercentage;

	if (pPerformancePercentage == NULL)
		return KEAPI_RET_PARAM_NULL;

	snprintf(path, KEAPI_MAX_STR, "/sys/devices/system/cpu/cpu%d", cpu_count);

	while (access(path, F_OK) == 0) {
		cpu_count++;
		snprintf(path, KEAPI_MAX_STR, "/sys/devices/system/cpu/cpu%d", cpu_count);
	}

	if (cpu_count == 0)
		return KEAPI_RET_RETRIEVAL_ERROR;

	if (coreNr < 0 || coreNr >= cpu_count)
		return KEAPI_RET_PARAM_ERROR;

	if ((ret = KEApiL_GetCpuFreq(0, KEAPI_CPU_FREQUENCY_MAX, &maxFrequency)) != KEAPI_RET_SUCCESS)
		return ret;

	if ((ret = KEApiL_GetCpuFreq(coreNr, KEAPI_CPU_FREQUENCY_CURRENT, &currFrequency)) != KEAPI_RET_SUCCESS)
		return ret;

	PerformancePercentage = (int16_t)(((float)currFrequency / (float)maxFrequency) * 100);

	if (PerformancePercentage > 127)
		*pPerformancePercentage = 127;
	else
		*pPerformancePercentage = (int8_t)PerformancePercentage;

	return KEAPI_RET_SUCCESS;
}

/*******************************************************************************/

int SetupSystemState(struct sstate *State)
{
	int32_t i, j, ret;
	FILE *fd;
	char path[KEAPI_MAX_STR];

	for (i = 0; i < State->paramCount; i++) {
		switch (State->paramType[i]) {
		case cpuGovernor: {
			/* Check every core frequency management policy */
			for (j = 0; j < MAX_AVAIL_CPU; j++) {
				snprintf(path, KEAPI_MAX_STR, CPU_CORE_SCALING_PATH, j, "scaling_governor");

				if ((ret = checkRWAccess(path)) != KEAPI_RET_SUCCESS) {
					if (ret == KEAPI_RET_RETRIEVAL_ERROR) {
						if (j == 0) /* Performance cannot be set to the first core -> error */
							return ret;
						else /* Performance has been set to at least one core -> ok */
							break;
					} else
						return ret;
				}

				if ((fd = fopen(path, "w")) == NULL)
					return KEAPI_RET_ERROR;

				if (fprintf(fd, "%s", State->paramValue[i]) == EOF) {
					fclose(fd);
					return KEAPI_RET_ERROR;
				}

				fclose(fd);
			}
			break;
		} /*case cpuGovernor*/
		case cpuFrequency: {
			for (j = 0; j < MAX_AVAIL_CPU; j++) {
				int SMINF, SMAXF; /* data in scaling_min_freq and scaling_max_freq */

				/* Getting minimum and maximum boundaries of available to set frequency */
				snprintf(path, KEAPI_MAX_STR, CPU_CORE_SCALING_PATH, j, "scaling_min_freq");

				if ((ret = checkRAccess(path)) != KEAPI_RET_SUCCESS) {
					if (ret == KEAPI_RET_RETRIEVAL_ERROR) {
						if (j == 0) /* Performance cannot be set to the first core -> error */
							return ret;
						else /* Performance has been set to at least one core -> ok */
							break;
					} else
						return ret;
				}

				if ((fd = fopen(path, "r")) == NULL)
					return KEAPI_RET_ERROR;

				if (fscanf(fd, "%d", &SMINF) == EOF) {
					fclose(fd);
					return KEAPI_RET_ERROR;
				}
				fclose(fd);

				snprintf(path, KEAPI_MAX_STR, CPU_CORE_SCALING_PATH, j, "scaling_max_freq");

				if ((ret = checkRAccess(path)) != KEAPI_RET_SUCCESS) {
					if (ret == KEAPI_RET_RETRIEVAL_ERROR) {
						if (j == 0) /* Performance cannot be set to the first core -> error */
							return ret;
						else /* Performance has been set to at least one core -> ok */
							break;
					} else
						return ret;
				}

				if ((fd = fopen(path, "r")) == NULL)
					return KEAPI_RET_RETRIEVAL_ERROR;

				if (fscanf(fd, "%d", &SMAXF) == EOF) {
					fclose(fd);
					return KEAPI_RET_ERROR;
				}
				fclose(fd);

				/* Verifying boundaries and trying to move these if it need */
				if (atoi(State->paramValue[i]) < SMINF) {
					ret = KEApiL_GetCpuFreq(0, KEAPI_CPU_FREQUENCY_MIN, &SMINF);
					if (ret != KEAPI_RET_SUCCESS)
						return ret;

					ret = setCpu(j, KEAPI_CPU_FREQUENCY_MIN, SMINF);
					if (ret != KEAPI_RET_SUCCESS)
						return ret;
				}

				if (atoi(State->paramValue[i]) > SMAXF) {
					ret = KEApiL_GetCpuFreq(0, KEAPI_CPU_FREQUENCY_MAX, &SMAXF);
					if (ret != KEAPI_RET_SUCCESS)
						return ret;

					ret = setCpu(j, KEAPI_CPU_FREQUENCY_MAX, SMAXF);
					if (ret != KEAPI_RET_SUCCESS)
						return ret;
				}

				/* Ok, now all is ok, so we can set requested frequency */
				ret = setCpu(j, KEAPI_CPU_FREQUENCY_CURRENT, atoi(State->paramValue[i]));
				if (ret != KEAPI_RET_SUCCESS)
					return ret;
			}
			break;
		} /* case cpuFrequency: */
		case cpuIPstMax: {
			ret = setCpu(0, CPU_INTEL_PSTATE_MAX, atoi(State->paramValue[i]));
			if (ret != KEAPI_RET_SUCCESS)
				return ret;

			break;
		} /* cpuIPstMax: */
		case cpuIPstMin: {
			ret = setCpu(0, CPU_INTEL_PSTATE_MIN, atoi(State->paramValue[i]));
			if (ret != KEAPI_RET_SUCCESS)
				return ret;

			break;
		} /* cpuIPstMin: */
		default:
			return KEAPI_RET_ERROR;
		} /*swith*/
	} /*for()*/

	return KEAPI_RET_SUCCESS;
}

/* function for auto calculating performance states for IntelPState driver */
static uint32_t CalculateIPStates(struct sstate *pState, int *pCount)
{
	int32_t minFreq = 0, maxFreq = 0, stateCnt = 0;
	int32_t minPercent = 0, maxPercent = 0, delta = 0;
	int i;
	uint32_t ret;
	char *descTempl = "CPUfreq policy set to 'powersave' and up to %d of max performance";

	ret = KEApiL_GetCpuFreq(0, KEAPI_CPU_FREQUENCY_MIN, &minFreq);
	if (ret != KEAPI_RET_SUCCESS)
		goto error;

	ret = KEApiL_GetCpuFreq(0, KEAPI_CPU_FREQUENCY_TURBO, &maxFreq);
	if (ret != KEAPI_RET_SUCCESS) {
		ret = KEApiL_GetCpuFreq(0, KEAPI_CPU_FREQUENCY_MAX, &maxFreq);
		if (ret != KEAPI_RET_SUCCESS)
			goto error;
	}

	stateCnt = (*pCount);
	minPercent = (minFreq / (maxFreq / 100)) + 0.5;
	delta = (((100 - minPercent) * 1.0) / (stateCnt - 1)) + 0.5;

	for (i = 0; i < stateCnt; i++) {
		maxPercent = minPercent + (delta * (stateCnt - (i + 1)));
		if (maxPercent > 100)
			maxPercent = 100;

		pState[i].id = i + 1;
		pState[i].paramCount = 3;

		pState[i].paramType[0] = cpuGovernor;
		pState[i].paramValue[0][0] = '\0';
		strncat(pState[i].paramValue[0], "powersave", KEAPI_MAX_STR - 1);

		pState[i].paramType[1] = cpuIPstMin;
		pState[i].paramValue[1][0] = '\0';
		snprintf(pState[i].paramValue[1], KEAPI_MAX_STR - 1, "%d", minPercent);

		pState[i].paramType[2] = cpuIPstMax;
		pState[i].paramValue[2][0] = '\0';
		snprintf(pState[i].paramValue[2], KEAPI_MAX_STR - 1, "%d", maxPercent);

		pState[i].description[0] = '\0';
		snprintf(pState[i].description, KEAPI_MAX_STR - 1, descTempl, maxPercent);
	}

	return KEAPI_RET_SUCCESS;

error:
	(*pCount) = 0;
	return ret;
}

/*****************************************************************************/
static uint32_t GetStatesDescription(struct sstate *pState, int *pCount)
{
	char substr[KEAPI_MAX_STR];
	int32_t i, j, ret, stateCount;
	struct sstate bufState;

	/* json root, json state array, json state array element, json parameters array, json parameters element, json data */
	json_t *root, *jstarr, *jstarrel, *jparamarr, *jparamarrel, *jdata;
	json_error_t error;

	(*pCount) = 0;

	if (pState == NULL || pCount == NULL)
		return KEAPI_RET_PARAM_NULL;

	if ((ret = checkRAccess(SYS_PERF_CONF_PATH)) != KEAPI_RET_SUCCESS)
		return ret;

	root = json_load_file(SYS_PERF_CONF_PATH, JSON_DECODE_ANY, &error);
	if (!root)
		return KEAPI_RET_READ_ERROR;

	jstarr = json_object_get(root, "state");
	if (!json_is_array(jstarr)) {
		/* if on particular Intel platform several CPUs can be installed,
		 * it is hard to predefine all states, but in case
		 * of using IntelPState driver we can calculate them */
		/* check that "auto" mode is set */
		if (json_is_string(jstarr)) {
			substr[0] = '\0';
			strncat(substr, json_string_value(jstarr), KEAPI_MAX_STR - 1);
			if (strcmp(substr, "auto") == 0) {
				/* getting quantity of required states */
				jdata = json_object_get(root, "howmany");
				if (json_is_integer(jdata)) {
					stateCount = json_integer_value(jdata);
					if (stateCount <= MAX_STATE_QUANTITY) {
						(*pCount) = stateCount;
						json_decref(root);
						/* call calculate function */
						return CalculateIPStates(pState, pCount);
					}
				}
			}
		}

		json_decref(root);
		return KEAPI_RET_ERROR;
	}
	stateCount = json_array_size(jstarr);

	if (stateCount == 0) {
		json_decref(root);
		return KEAPI_RET_SUCCESS;
	}

	if (stateCount > MAX_STATE_QUANTITY) {
		json_decref(root);
		return KEAPI_RET_ERROR;
	}

	/* iterate all states */
	for (i = 0; i < stateCount; i++) {
		jstarrel = json_array_get(jstarr, i);
		if (!json_is_object(jstarrel)) {
			json_decref(root);
			return KEAPI_RET_ERROR;
		}

		/* get state number */
		jdata = json_object_get(jstarrel, "stateNum");
		if (!json_is_integer(jdata)) {
			json_decref(root);
			return KEAPI_RET_ERROR;
		}
		bufState.id = json_integer_value(jdata);

		/* verify id number*/
		if (bufState.id < 1 || bufState.id > MAX_STATE_QUANTITY) {
			json_decref(root);
			return KEAPI_RET_ERROR;
		}

		for (j = 0; j < i; j++)
			if (pState[j].id == bufState.id) {
				json_decref(root);
				return KEAPI_RET_ERROR;
			}

		/* get description of power state */
		jdata = json_object_get(jstarrel, "description");
		if (!json_is_string(jdata)) {
			json_decref(root);
			return KEAPI_RET_ERROR;
		}
		bufState.description[0] = '\0';
		strncat(bufState.description, json_string_value(jdata), KEAPI_MAX_STR - 1);

		/* get conditions array */
		jparamarr = json_object_get(jstarrel, "condition");
		if (!json_is_string(jdata)) {
			json_decref(root);
			return KEAPI_RET_ERROR;
		}
		bufState.paramCount = json_array_size(jparamarr);

		/* parse pairs of key and value */
		for (j = 0; j < bufState.paramCount; j++) {
			jparamarrel = json_array_get(jparamarr, j);
			if (!json_is_object(jparamarrel)) {
				json_decref(root);
				return KEAPI_RET_ERROR;
			}

			jdata = json_object_get(jparamarrel, "key");
			if (!json_is_string(jdata)) {
				json_decref(root);
				return KEAPI_RET_ERROR;
			}
			strncpy(substr, json_string_value(jdata), KEAPI_MAX_STR);
			bufState.paramType[j] = getTypeId(substr);

			jdata = json_object_get(jparamarrel, "value");
			if (!json_is_string(jdata)) {
				json_decref(root);
				return KEAPI_RET_ERROR;
			}
			bufState.paramValue[j][0] = '\0';
			strncat(bufState.paramValue[j], json_string_value(jdata), KEAPI_MAX_STR - 1);
		}

		memcpy(&pState[i], &bufState, sizeof(struct sstate));
	}

	(*pCount) = i;

	return KEAPI_RET_SUCCESS;
}

/*******************************************************************************/
KEAPI_RETVAL KEApiL_PerformanceStateCaps(uint32_t *pStatesMask)
{
	int32_t count, ret;
	struct sstate States[MAX_STATE_QUANTITY];

	if (pStatesMask == NULL)
		return KEAPI_RET_PARAM_NULL;

	(*pStatesMask) = 0;

	if ((ret = GetStatesDescription(States, &count)) != KEAPI_RET_SUCCESS)
		return ret;

	for (; count > 0; count--)
		(*pStatesMask) |= numberToMask(States[count - 1].id);

	return KEAPI_RET_SUCCESS;
}

/*******************************************************************************/
KEAPI_RETVAL KEApiL_GetPerformanceStateDescription(uint32_t state, char *pDescription)
{
	int32_t ret;
	int count;
	struct sstate States[MAX_STATE_QUANTITY];

	if (pDescription == NULL)
		return KEAPI_RET_PARAM_NULL;

	if (state == 0) {
		strcpy(pDescription, "Maximal performance.");
		return KEAPI_RET_SUCCESS;
	}

	if ((ret = GetStatesDescription(States, &count)) != KEAPI_RET_SUCCESS)
		return ret;

	for (; count > 0; count--)
		if (state == numberToMask(States[count - 1].id)) {
			strcpy(pDescription, States[count - 1].description);
			return KEAPI_RET_SUCCESS;
		}

	return KEAPI_RET_PARAM_ERROR;
}

/*******************************************************************************/
KEAPI_RETVAL KEApiL_GetPerformanceState(uint32_t *pState)
{
	FILE *fd;
	char bufState = 0;
	int8_t found = FALSE;
	int32_t i, j, k, ret, count;
	char path[KEAPI_MAX_STR], scan_val[KEAPI_MAX_STR];
	struct sstate States[MAX_STATE_QUANTITY];

	if (pState == NULL)
		return KEAPI_RET_PARAM_NULL;

	*pState = 0;

	if ((ret = GetStatesDescription(States, &count)) != KEAPI_RET_SUCCESS)
		return ret;

	for (i = 0; i < count; i++) {
		for (j = 0; j < States[i].paramCount; j++) {
			switch (States[i].paramType[j]) {
			case cpuGovernor:
				/* Check every core frequency management policy */
				for (k = 0; k < MAX_AVAIL_CPU; k++) {
					snprintf(path, KEAPI_MAX_STR, CPU_CORE_SCALING_PATH, k, "scaling_governor");

					if ((ret = checkRAccess(path)) != KEAPI_RET_SUCCESS) {
						if (ret == KEAPI_RET_RETRIEVAL_ERROR) {
							if (k == 0)
							/* Performance cannot be set to the first core -> error */
								return ret;
							else /* Performance has been set to at least one core -> ok */
								break;
						} else
							return ret;
					}

					if ((fd = fopen(path, "r")) == NULL)
						return KEAPI_RET_ERROR;

					if (fscanf(fd, "%s", scan_val) == EOF) {
						fclose(fd);
						return KEAPI_RET_ERROR;
					}
					fclose(fd);

					if (strncmp(scan_val, States[i].paramValue[j],
						    strlen(States[i].paramValue[j])) == 0) {
						bufState = States[i].id;
						found = TRUE;
					} else {
						found = FALSE;
						break;
					}
				}
				break;

			case cpuFrequency:
				/* Check every core frequency management policy */
				for (k = 0; k < MAX_AVAIL_CPU; k++) {
					snprintf(path, KEAPI_MAX_STR, CPU_CORE_SCALING_PATH, k, "cpuinfo_cur_freq");

					if ((ret = checkRAccess(path)) != KEAPI_RET_SUCCESS) {
						if (ret == KEAPI_RET_RETRIEVAL_ERROR) {
							if (k == 0)
							/* Performance cannot be set to the first core -> error */
								return ret;
							else /* Performance has been set to at least one core -> ok */
								break;
						} else
							return ret;
					}

					if ((fd = fopen(path, "r")) == NULL)
						return KEAPI_RET_ERROR;

					if (fscanf(fd, "%s", scan_val) == EOF) {
						fclose(fd);
						return KEAPI_RET_ERROR;
					}
					fclose(fd);

					if (strncmp(scan_val, States[i].paramValue[j],
						    strlen(States[i].paramValue[j])) == 0) {
						bufState = States[i].id;
						found = TRUE;
					} else {
						found = FALSE;
						break;
					}
				}
				break;

			case cpuIPstMax:
				snprintf(path, KEAPI_MAX_STR, CPU_INTEL_PSTATE_PATH, "max");

				if ((ret = checkRAccess(path)) != KEAPI_RET_SUCCESS)
					return ret;

				if ((fd = fopen(path, "r")) == NULL)
					return KEAPI_RET_ERROR;

				if (fscanf(fd, "%s", scan_val) == EOF) {
					fclose(fd);
					return KEAPI_RET_ERROR;
				}
				fclose(fd);

				if (strncmp(scan_val, States[i].paramValue[j], strlen(States[i].paramValue[j])) == 0) {
					bufState = States[i].id;
					found = TRUE;
				} else
					found = FALSE;

				break;

			case cpuIPstMin:
				snprintf(path, KEAPI_MAX_STR, CPU_INTEL_PSTATE_PATH, "min");

				if ((ret = checkRAccess(path)) != KEAPI_RET_SUCCESS)
					return ret;

				if ((fd = fopen(path, "r")) == NULL)
					return KEAPI_RET_ERROR;

				if (fscanf(fd, "%s", scan_val) == EOF) {
					fclose(fd);
					return KEAPI_RET_ERROR;
				}
				fclose(fd);

				if (strncmp(scan_val, States[i].paramValue[j], strlen(States[i].paramValue[j])) == 0) {
					bufState = States[i].id;
					found = TRUE;
				} else
					found = FALSE;

				break;
			} /*switch paramType*/

			if (found == FALSE)
				break;

		} /*loop paramTypes in one State*/

		if (found == TRUE) {
			*pState = numberToMask(bufState);
			return KEAPI_RET_SUCCESS;
		}
	} /*loop by States*/

	*pState = 0;
	return KEAPI_RET_SUCCESS;
}

KEAPI_RETVAL KEApiL_SetPerformanceState(uint32_t state)
{
	struct sstate States[MAX_STATE_QUANTITY];
	int count, ret;
	char path[KEAPI_MAX_STR];

	if (state == 0) {
		struct sstate newState = { .id = 1,
					   .description = "performance",
					   .paramCount = 3,
					   .paramType[0] = cpuGovernor,
					   .paramValue[0] = "performance",
					   .paramType[1] = cpuIPstMax,
					   .paramValue[1] = "100",
					   .paramType[2] = cpuIPstMin,
					   .paramValue[2] = "100" };

		snprintf(path, KEAPI_MAX_STR, CPU_INTEL_PSTATE_PATH, "max");
		if ((ret = checkRAccess(path)) != KEAPI_RET_SUCCESS)
			newState.paramCount = 1;

		return SetupSystemState(&newState);
	}

	ret = GetStatesDescription(States, &count);
	if (ret != KEAPI_RET_SUCCESS)
		return ret;

	for (; count > 0; count--) {
		if (state == numberToMask(States[count - 1].id))
			return SetupSystemState(&States[count - 1]);
	}

	return KEAPI_RET_PARAM_ERROR;
}
