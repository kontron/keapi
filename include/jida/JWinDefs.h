/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron AG */

/* JUMPtec JIDA non-Windows environment support declarations */

//***************************************************************************

#ifndef __JWINDEFS_H__
#define __JWINDEFS_H__

#ifndef WIN32

//***************************************************************************
// Basic Types
//

#define MAX_PATH 255
#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#ifndef NULL
#define NULL ((void*) 0)
#endif

#ifndef max
#define max(a,b) ((a)<(b)?(b):(a))
#define min(a,b) ((a)<(b)?(a):(b))
#endif

#define CONST const

#define VOID void
typedef char CHAR;
typedef short SHORT;
typedef long LONG;
#ifndef __VXWORKS__
typedef unsigned char UCHAR;
typedef unsigned short USHORT;
typedef unsigned long ULONG;
#endif

typedef wchar_t WCHAR;
#ifdef _UNICODE
typedef wchar_t TCHAR;
#define TEXT(x)  L ## x
#else
typedef char TCHAR;
#define TEXT(x)  x
#endif
typedef CONST WCHAR *LPCWSTR;
typedef WCHAR *LPWSTR;

typedef CHAR *PCHAR;
typedef TCHAR *PTCHAR;
typedef UCHAR *PUCHAR;
typedef ULONG *PULONG;
typedef CHAR *LPCH, *PCH;

typedef CONST CHAR *LPCCH, *PCCH;
typedef CHAR *NPSTR;
typedef CHAR *LPSTR, *PSTR;
typedef CHAR *LPTSTR;
typedef CONST CHAR *LPCSTR, *PCSTR;

#define WINBASEAPI
#define WINAPI
#define near
#define far
#define FAR
#define PASCAL
#define NEAR
#define __cdecl
#define _DWORD_DEFINED
typedef int (FAR WINAPI *FARPROC)();
typedef unsigned long       DWORD;
#ifndef __VXWORKS__
typedef int                 BOOL;
#endif
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
#ifndef __VXWORKS__
typedef unsigned int        UINT;
#endif
typedef float               FLOAT;
typedef FLOAT               *PFLOAT;
typedef BOOL near           *PBOOL;
typedef BOOL far            *LPBOOL;
typedef BYTE near           *PBYTE;
typedef BYTE far            *LPBYTE;
typedef int near            *PINT;
typedef int far             *LPINT;
typedef WORD near           *PWORD;
typedef WORD far            *LPWORD;
typedef long far            *LPLONG;
typedef DWORD near          *PDWORD;
typedef DWORD far           *LPDWORD;
typedef void far            *LPVOID;
typedef CONST void far      *LPCVOID;
typedef void                *PVOID;
typedef double              DWORDLONG;
typedef long long	    LONGLONG;

//***************************************************************************
// Additional Types
//

#ifndef __VXWORKS__
typedef void *HANDLE;
#endif
typedef void *HMODULE;
typedef void *HINSTANCE;

typedef struct _SYSTEMTIME {
	WORD wYear;
	WORD wMonth;
	WORD wDayOfWeek;
	WORD wDay;
	WORD wHour;
	WORD wMinute;
	WORD wSecond;
	WORD wMilliseconds;
} SYSTEMTIME, *LPSYSTEMTIME;

//***************************************************************************

#endif /* WIN32 */

#endif /* __JWINDEFS_H__ */

//***************************************************************************
