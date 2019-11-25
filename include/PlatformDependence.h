/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron AG */

/* KEAPI platform dependence definitions. */

#ifndef _PLATFORM_DEPENDENCE
#define _PLATFORM_DEPENDENCE

#include "keapi_inc.h"

#if PLATFORM(VxWorks)
#include "versionK.h"
KONTRON_VERSION(PlatformDependence_h, 01b) /* version identifier */
#include <pthread.h>
#endif /* PLATFORM(VxWorks) */

#if PLATFORM(Windows) || PLATFORM(WindowsCE)
#include <windows.h>
#endif

#if PLATFORM(Linux)
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#endif

/* Linux-specific declarations */
#if PLATFORM(Linux)
/* #define WSAEWOULDBLOCK EINPROGRESS */
#define WOULD_BLOCK_ERROR(err) (((err == EINPROGRESS) || (err == EAGAIN)) ? 1 : 0)

typedef void *THREAD_RESULT;
#define __THREAD__ /* nothing! */
typedef THREAD_RESULT(__THREAD__ *ThreadFunc)(void *);
#define threadfunc THREAD_RESULT __THREAD__
#define currentThreadID pthread_self()
#define currentProcessID() ((uint32_t)getpid())

enum RetValWaitForSingleObject {
	wait_OBJECT_0 = 0,
	wait_TIMEOUT = ETIMEDOUT,
	wait_ERROR = 0xFFFFFFFF
};

typedef struct _event_t {
	pthread_mutex_t mutex;
	pthread_cond_t condition;
} event_t;

typedef struct _critical_section_t {
	pthread_mutex_t mutex;
} critical_section_t;

typedef struct _thread_t {
	pthread_t thread;
	event_t stop; /* used for stopping of the thread */
	event_t stopped; /* signas that thread is retminating */
} thread_t;

extern int memcpy_s(void *dest, int32_t dest_size, const void *src, int32_t src_size);

#endif /* PLATFORM(Linux) */

/*----------------------------------------------------------------------------*/

/* Windows-specific declarations */
#if PLATFORM(Windows) || PLATFORM(WindowsCE)

#define WOULD_BLOCK_ERROR(err) ((err == WSAEWOULDBLOCK) ? 1 : 0)

typedef uint32_t THREAD_RESULT;
#define __THREAD__ WINAPI
typedef THREAD_RESULT(__THREAD__ *ThreadFunc)(void *);
#define threadfunc THREAD_RESULT __THREAD__
#define currentThreadID GetCurrentThreadId()
#define currentProcessID() ((uint32_t)GetCurrentProcessId())

enum RetValWaitForSingleObject {
	wait_OBJECT_0 = WAIT_OBJECT_0,
	wait_TIMEOUT = WAIT_TIMEOUT,
	wait_ERROR = WAIT_FAILED
};

typedef struct _event_t {
	HANDLE handle;
} event_t;

typedef struct _critical_section_t {
	CRITICAL_SECTION cs;
} critical_section_t;

typedef struct _thread_t {
	HANDLE handle;
	event_t stop;
} thread_t;

#if PLATFORM(WindowsCE)
#define KEAPI_REG_PATH L"SOFTWARE\\Kontron\\KEAPI"
#else
#define KEAPI_REG_PATH "SOFTWARE\\Kontron\\KEAPI"
#endif

#if PLATFORM(WindowsCE)
uint32_t CeInitialize(VOID);
uint32_t CeUninitialize(VOID);
#endif

#endif /* PLATFORM(Windows) */

/*----------------------------------------------------------------------------*/

/* VxWorks-specific declarations */
#if PLATFORM(VxWorks)

/* 6.8 is lack of stdint.h */
#ifndef UINT8_MAX
/* TODO */
#endif

#define WOULD_BLOCK_ERROR(err) (((err == EINPROGRESS) || (err == EAGAIN)) ? 1 : 0)

typedef void *THREAD_RESULT;
#define __THREAD__ /* nothing! */
typedef THREAD_RESULT(__THREAD__ *ThreadFunc)(void *);
#define threadfunc THREAD_RESULT __THREAD__
#define currentThreadID pthread_self()
#define currentProcessID() ((uint32_t)getpid())

enum RetValWaitForSingleObject {
	wait_OBJECT_0 = 0,
	wait_TIMEOUT = ETIMEDOUT,
	wait_ERROR = 0xFFFFFFFF
};

typedef struct _event_t {
	pthread_mutex_t mutex;
	pthread_cond_t condition;
} event_tt;

typedef struct _critical_section_t {
	pthread_mutex_t mutex;
} critical_section_t;

typedef struct _thread_t {
	pthread_t thread;
	event_tt stop; /* used for stopping of the thread */
	event_tt stopped; /* signas that thread is retminating */
} thread_t;

extern int memcpy_s(void *dest, int32_t dest_size, const void *src, int32_t src_size);

#endif /* PLATFORM(VxWorks) */

/*----------------------------------------------------------------------------*/
#if !PLATFORM(VxWorks)
extern int32_t createThread(ThreadFunc threadFunc, thread_t *threadData);
extern void setEvent(event_t *e);
extern int32_t createEvent(event_t *e);
extern int32_t closeEvent(event_t *e);
extern int32_t waitForSingleObject(event_t *e, uint32_t milliseconds);
extern void deleteCriticalSection(critical_section_t *cs);
extern void destroyThread(thread_t *threadData, uint32_t timeout_ms);
extern uint32_t getTickCount();
extern void exitThread(uint32_t exitcode);
#endif /* !PLATFORM(VxWorks) */

extern void initializeCriticalSection(critical_section_t *cs);
extern void enterCriticalSection(critical_section_t *cs);
extern void leaveCriticalSection(critical_section_t *cs);

/* public interface for all platforms */
extern uint32_t getCurrentProcessId();
extern uint32_t getLastError();

#if PLATFORM(Windows)
#ifndef snprintf
#define snprintf(buffer, buffer_size, stringbuffer, ...) (_snprintf(buffer, buffer_size, stringbuffer, __VA_ARGS__))
#endif
#elif PLATFORM(WindowsCE)
typedef unsigned __int64 ULONGLONG;
#ifndef snprintf
#define snprintf _snprintf
#endif
#ifndef vsnprintf
#define vsnprintf _vsnprintf
#ifndef time
#define time(a) (_time64(a))
#endif
#endif

/*
 * Meet a lack of WinCE STDLIB.H header
 */

/*
 * Sizes for buffers used by the _makepath() and _splitpath() functions.
 * note that the sizes include space for 0-terminator
 */

#define _MAX_DRIVE  3   /* max. length of drive component */
#define _MAX_DIR    256 /* max. length of path component */
#define _MAX_FNAME  256 /* max. length of file name component */
#define _MAX_EXT    256 /* max. length of extension component */
#endif /* windowsCE */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _PLATFORM_DEPENDENCE */
