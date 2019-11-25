/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2010-2011 ST-Ericsson SA */

struct ids_database {
	char *type;
	int id;
	char *manufacturer;
};

struct ids_database database[] = {
	{
		.type = "sd",
		.id = 0x01,
		.manufacturer = "Panasonic",
	},
	{
		.type = "sd",
		.id = 0x02,
		.manufacturer = "Toshiba/Kingston/Viking",
	},
	{
		.type = "sd",
		.id = 0x03,
		.manufacturer = "SanDisk",
	},
	{
		.type = "sd",
		.id = 0x08,
		.manufacturer = "Silicon Power",
	},
	{
		.type = "sd",
		.id = 0x18,
		.manufacturer = "Infineon",
	},
	{
		.type = "sd",
		.id = 0x1b,
		.manufacturer = "Transcend",
	},
	{
		.type = "sd",
		.id = 0x1c,
		.manufacturer = "Transcend",
	},
	{
		.type = "sd",
		.id = 0x1d,
		.manufacturer = "Corsair",
	},
	{
		.type = "sd",
		.id = 0x1e,
		.manufacturer = "Transcend",
	},
	{
		.type = "sd",
		.id = 0x1f,
		.manufacturer = "Kingston",
	},
	{
		.type = "sd",
		.id = 0x28,
		.manufacturer = "Lexar",
	},
	{
		.type = "sd",
		.id = 0x30,
		.manufacturer = "SanDisk",
	},
	{
		.type = "sd",
		.id = 0x33,
		.manufacturer = "STMicroelectronics",
	},
	{
		.type = "sd",
		.id = 0x41,
		.manufacturer = "Kingston",
	},
	{
		.type = "sd",
		.id = 0x6f,
		.manufacturer = "STMicroelectronics",
	},
	{
		.type = "sd",
		.id = 0x89,
		.manufacturer = "Unknown",
	},
	{
		.type = "mmc",
		.id = 0x00,
		.manufacturer = "SanDisk",
	},
	{
		.type = "mmc",
		.id = 0x02,
		.manufacturer = "Kingston/SanDisk",
	},
	{
		.type = "mmc",
		.id = 0x03,
		.manufacturer = "Toshiba",
	},
	{
		.type = "mmc",
		.id = 0x05,
		.manufacturer = "Unknown",
	},
	{
		.type = "mmc",
		.id = 0x06,
		.manufacturer = "Unknown",
	},
	{
		.type = "mmc",
		.id = 0x11,
		.manufacturer = "Toshiba",
	},
	{
		.type = "mmc",
		.id = 0x15,
		.manufacturer = "Samsung/SanDisk/LG",
	},
	{
		.type = "mmc",
		.id = 0x37,
		.manufacturer = "KingMax",
	},
	{
		.type = "mmc",
		.id = 0x44,
		.manufacturer = "SanDisk",
	},
	{
		.type = "mmc",
		.id = 0x2c,
		.manufacturer = "Kingston",
	},
	{
		.type = "mmc",
		.id = 0x70,
		.manufacturer = "Kingston",
	},
};
