/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron AG */

/* KEAPI network info implementation. */

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <linux/ethtool.h>
#include <linux/sockios.h>

#include "keapi_inc.h"

/*******************************************************************************/
KEAPI_RETVAL KEApiL_GetNetworkDeviceCount(int32_t *pNetworkDeviceCount)
{
	int32_t sck, numdev = 0;
	struct ifreq ifr;
	char buf[KEAPI_MAX_STR];

	/* Check function parameters */
	if (pNetworkDeviceCount == NULL)
		return KEAPI_RET_PARAM_NULL;

	errno = 0;
	if ((sck = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)) < 0) {
		if (errno == EACCES || errno == EPERM)
			return KEAPI_RET_PERMISSION_DENIED;

		return KEAPI_RET_ERROR;
	}

	ifr.ifr_ifindex = 1;
	numdev = 0;
	errno = 0;
	while (ioctl(sck, SIOCGIFNAME, &ifr) >= 0) {
		strncpy(buf, ifr.ifr_name, IF_NAMESIZE);
		buf[IF_NAMESIZE] = '\0';

		if ((strncmp("lo", buf, KEAPI_MAX_STR) != 0) && (strncmp("sit", buf, strlen("sit")) != 0) &&
		    (strncmp("can", buf, strlen("can")) != 0))
			numdev++;

		ifr.ifr_ifindex++;
	}

	(*pNetworkDeviceCount) = numdev;

	close(sck);
	return KEAPI_RET_SUCCESS;
}

/*******************************************************************************/
KEAPI_RETVAL KEApiL_GetNetworkDeviceList(PKEAPI_NETWORK_DEVICE pNetworkDevices, int32_t networkDeviceCount)
{
	int32_t sck, out_dev_num, real_dev_cnt, dev_cnt, ret;
	struct ifreq ifr;
	struct ethtool_cmd cmd;
	char buf[1024], mac[20];

	/*
	 * Root permisions are needed to get network speed.
	 * If isn't running as root then speed = 0
	 * and return KEAPI_RET_PARTIAL_SUCCESS
	 */

	/* Check function parameters */
	if (pNetworkDevices == NULL)
		return KEAPI_RET_PARAM_NULL;

	if ((ret = KEApiL_GetNetworkDeviceCount(&real_dev_cnt)) != KEAPI_RET_SUCCESS)
		return ret;

	if (networkDeviceCount < 0 || networkDeviceCount > real_dev_cnt)
		return KEAPI_RET_PARAM_ERROR;

	if (networkDeviceCount == 0) {
		if (real_dev_cnt == 0)
			return KEAPI_RET_SUCCESS;
		else
			return KEAPI_RET_PARTIAL_SUCCESS;
	}

	memset(pNetworkDevices, 0, sizeof(KEAPI_NETWORK_DEVICE) * networkDeviceCount);

	errno = 0;
	if ((sck = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)) < 0) {
		if (errno == EACCES || errno == EPERM)
			return KEAPI_RET_PERMISSION_DENIED;

		return KEAPI_RET_ERROR;
	}

	ifr.ifr_ifindex = dev_cnt = 1;
	out_dev_num = 0;
	errno = 0;
	while (ioctl(sck, SIOCGIFNAME, &ifr) >= 0 && out_dev_num < networkDeviceCount) {
		strncpy(buf, ifr.ifr_name, IF_NAMESIZE);
		buf[IF_NAMESIZE] = '\0';

		if ((strncmp("lo", buf, KEAPI_MAX_STR) == 0) || (strncmp("sit", buf, strlen("sit")) == 0) ||
		    (strncmp("can", buf, strlen("can")) == 0)) {
			dev_cnt++;
			ifr.ifr_ifindex = dev_cnt;
			continue;
		}
		strncat(pNetworkDevices[out_dev_num].deviceName, ifr.ifr_name, KEAPI_MAX_STR - 1);

		/* MAC address */
		errno = 0;
		if (ioctl(sck, SIOCGIFHWADDR, &ifr) < 0) {
			if (errno == EACCES || errno == EPERM) {
				close(sck);
				return KEAPI_RET_PERMISSION_DENIED;
			}

			close(sck);
			return KEAPI_RET_ERROR;
		}

		sprintf(mac, "%02x:%02x:%02x:%02x:%02x:%02x", (int32_t)((uint8_t *)ifr.ifr_hwaddr.sa_data)[0],
			(int32_t)((uint8_t *)ifr.ifr_hwaddr.sa_data)[1],
			(int32_t)((uint8_t *)ifr.ifr_hwaddr.sa_data)[2],
			(int32_t)((uint8_t *)ifr.ifr_hwaddr.sa_data)[3],
			(int32_t)((uint8_t *)ifr.ifr_hwaddr.sa_data)[4],
			(int32_t)((uint8_t *)ifr.ifr_hwaddr.sa_data)[5]);
		strncat(pNetworkDevices[out_dev_num].mac, mac, KEAPI_MAX_STR - 1);

		/* Get ipv4*/
		memset(&ifr, 0, sizeof(struct ifreq));
		ifr.ifr_addr.sa_family = AF_INET;
		strncat(ifr.ifr_name, buf, IFNAMSIZ - 1);

		errno = 0;
		if (ioctl(sck, SIOCGIFADDR, &ifr) < 0) {
			if (errno == EACCES || errno == EPERM) {
				close(sck);
				return KEAPI_RET_PERMISSION_DENIED;
			}
		}
		strncat(pNetworkDevices[out_dev_num].ip, inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr),
			KEAPI_MAX_STR - 1);

		/* Get speed */
		errno = 0;
		ifr.ifr_data = (void *)&cmd;
		cmd.cmd = ETHTOOL_GSET; /* "Get settings" */
		if (ioctl(sck, SIOCETHTOOL, &ifr) < 0) {
			if (errno == EACCES || errno == EPERM) {
				close(sck);
				return KEAPI_RET_PERMISSION_DENIED;
			} else {
				pNetworkDevices[out_dev_num].speed = -1;
			}
		} else {
			pNetworkDevices[out_dev_num].speed = ethtool_cmd_speed(&cmd);
		}

		out_dev_num++;
		dev_cnt++;
		ifr.ifr_ifindex = dev_cnt;
	}

	if (out_dev_num == 0 && (errno == EACCES || errno == EPERM)) {
		close(sck);
		return KEAPI_RET_PERMISSION_DENIED;
	}

	close(sck);

	if (out_dev_num < real_dev_cnt) {
		if (networkDeviceCount < real_dev_cnt && out_dev_num == networkDeviceCount)
			return KEAPI_RET_PARTIAL_SUCCESS;
		else
			return KEAPI_RET_ERROR;
	}

	return KEAPI_RET_SUCCESS;
}
