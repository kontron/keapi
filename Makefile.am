AUTOMAKE_OPTIONS = subdir-objects

ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} -I m4

EXTRA_DIST = CHANGES LICENSE README.md

AM_CPPFLAGS = -I$(srcdir)/include -I$(srcdir)/src

lib_LTLIBRARIES = libkeapi.la libEApi.la libjida.la

K_HEADER=$(srcdir)/include/keapi.h
L_HEADER=$(srcdir)/include/linux_support.h

SPEC_MAJOR_H = $(shell grep define.KEAPI_SPEC_VERSION_MAJOR ${K_HEADER} | cut -dx -f2 | cut -b1-2)
SPEC_MINOR_H = $(shell grep define.KEAPI_SPEC_VERSION_MINOR ${K_HEADER} | cut -dx -f2 | cut -b1-2)
LNX_IMPL_MAJOR_H = $(shell grep define.KEAPI_LINUX_VERSION_MAJOR ${L_HEADER} | cut -dx -f2 | cut -b1-2)
LNX_IMPL_MINOR_H = $(shell grep define.KEAPI_LINUX_VERSION_MINOR ${L_HEADER} | cut -dx -f2 | cut -b1-2)

SPEC_MAJOR_D = $(shell echo "ibase=16; ${SPEC_MAJOR_H}" | bc)
SPEC_MINOR_D = $(shell echo "ibase=16; ${SPEC_MINOR_H}" | bc)
LNX_IMPL_MAJOR_D = $(shell echo "ibase=16; ${LNX_IMPL_MAJOR_H}" | bc)
LNX_IMPL_MINOR_D = $(shell echo "ibase=16; ${LNX_IMPL_MINOR_H}" | bc)

KEAPI_VERSION_INFO = -version-info ${SPEC_MAJOR_D}:${LNX_IMPL_MAJOR_D}

COMMON_SRCS = \
	src/keapi_errors.c \
	src/libinit.c \
	src/PlatformDependence.c \
	src/keapiNative.c

libkeapi_la_LIBADD = -lpthread -lpcre -latasmart -ljansson
libkeapi_la_CFLAGS = -Wall -Wextra -Wno-stringop-truncation -Wno-format-truncation -pedantic
libkeapi_la_LDFLAGS = $(KEAPI_VERSION_INFO)
libkeapi_la_SOURCES = \
	$(COMMON_SRCS) \
	src/addon.c \
	src/globals.c \
	src/battery.c \
	src/board.c \
	src/cpu_perf.c \
	src/disks.c \
	src/display.c \
	src/fan.c \
	src/gpio_pins.c \
	src/i2c.c \
	src/intruder.c \
	src/keapi.c \
	src/mem.c \
	src/network.c \
	src/pci.c \
	src/sm_bus.c \
	src/spi.c \
	src/storage.c \
	src/storage_picmg.c \
	src/linux_support.c \
	src/temperature.c \
	src/voltage.c \
	src/wd.c

INTERPRETER:=$(shell readelf -l $(shell which ld)|grep "\[.*:.*\]"| sed -nr "s/\s*\[.*:\s+(.*)\]/\1/p")

libEApi_la_LIBADD = libkeapi.la
libEApi_la_CFLAGS = -D EAPI_INTERPRETER=\"$(INTERPRETER)\"
libEApi_la_LDFLAGS = -fvisibility=hidden -fPIC -Wl,-e,EApiMain -version-info 1:0:0
libEApi_la_SOURCES = \
	src/PicmgWrapper.c \
	src/interp.c \
	src/Message.c

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = keapi.pc

libkeapi_includedir = $(includedir)/keapi
libkeapi_include_HEADERS = include/keapi.h

libkeapi2_includedir = $(includedir)/eapi
libkeapi2_include_HEADERS = \
	include/eapi/EApi.h \
	include/eapi/EApiCommon.h \
	include/eapi/EApiQ7.h \
	include/eapi/COM0EEP.h \
	include/eapi/EApiETX.h \
	include/eapi/CBCEEPROM.h \
	include/eapi/EApiEPIC.h \
	include/eapi/EeeP.h \
	include/eapi/EApiTCA.h \
	include/eapi/EApiMOPS.h \
	include/eapi/EApiPISA.h \
	include/eapi/PNPID.h \
	include/eapi/EApiCOM0.h

libkeapi3_includedir = $(includedir)/eapi/masm
libkeapi3_include_HEADERS = \
	include/eapi/masm/EeePConv.h \
	include/eapi/masm/EApiConv.inc \
	include/eapi/masm/EApi.inc \
	include/eapi/masm/EApiMacro.inc \
	include/eapi/masm/h2inc.h \
	include/eapi/masm/EApiConv.h \
	include/eapi/masm/EeePConv.inc \
	include/eapi/masm/EeeP.inc

libkeapi4_includedir = $(includedir)/eapi/linux
libkeapi4_include_HEADERS = include/eapi/linux/EApiOs.h

libjida_la_LIBADD = libkeapi.la
libjida_la_CFLAGS = -Wall
libjida_la_LDFLAGS = -version-info 1:6:0
libjida_la_SOURCES = src/JidaWrapper.c
libkeapi5_includedir = $(includedir)/jida
libkeapi5_include_HEADERS = \
	include/jida/Jida.h \
	include/jida/JWinDefs.h

keapidocdir = $(docdir)/
keapidoc_DATA = \
	LICENSE \
	docs/CONFIGURATION_FILES_FORMAT \
	docs/KEAPI_3.pdf \
	docs/Kontron_EAPI_for_Linux_AppNote.pdf
