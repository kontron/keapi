/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron AG */

/* KEAPI platform dependence code. */

#include <assert.h>

#ifdef __VXWORKS__
#include "versionK.h"
KONTRON_VERSION(PlatformDependence_c, 01b) /* version identifier */
#endif /* __VXWORKS__ */

#ifdef _WIN32_WCE
#define INCL_WINSOCK_API_PROTOTYPES 1
#include <winsock2.h>
#endif /* _WIN32_WCE */

#include "PlatformDependence.h"

#if PLATFORM(Linux)

#include <errno.h>
#include <sys/timeb.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>

uint32_t getCurrentProcessId()
{
	return getpid();
}

/*-------------------------------------------------------------------------*/
void setEvent(event_t *e)
{
	pthread_mutex_lock(&e->mutex);

	pthread_cond_signal(&e->condition);

	pthread_mutex_unlock(&e->mutex);
}

/*-------------------------------------------------------------------------*/
int32_t createEvent(event_t *e)
{
	int32_t ret;

	ret = pthread_cond_init(&e->condition, 0);

	if (ret != 0) {
		assert(0);
		return -1;
	}

	ret = pthread_mutex_init(&e->mutex, 0);

	if (ret != 0) {
		assert(0);
		return -1;
	}

	return 0;
}

/*-------------------------------------------------------------------------*/
int32_t closeEvent(event_t *e)
{
	int32_t ret;

	pthread_mutex_lock(&e->mutex);
	pthread_cond_broadcast(&e->condition);

	ret = pthread_cond_destroy(&e->condition);
	pthread_mutex_unlock(&e->mutex);
	if (ret != 0) {
		assert(0);
		return -1;
	}

	pthread_mutex_lock(&e->mutex);
	pthread_mutex_unlock(&e->mutex);

	ret = pthread_mutex_destroy(&e->mutex);
	if (ret != 0) {
		assert(0);
		return -1;
	}

	return 0;
}

/*-------------------------------------------------------------------------*/
int32_t waitForSingleObject(event_t *e, uint32_t milliseconds)
{
	int32_t rc;
	uint32_t sec, millisec, ret;
	struct timespec tm;
	struct timeb tp;

	rc = pthread_mutex_lock(&(e->mutex));
	if (rc)
		return wait_ERROR;

	sec = milliseconds / 1000;
	millisec = milliseconds % 1000;
	ftime(&tp);
	tp.time += sec;
	tp.millitm += millisec;

	if (tp.millitm > 999) {
		tp.millitm -= 1000;
		tp.time++;
	}

	tm.tv_sec = tp.time;
	tm.tv_nsec = tp.millitm * 1000000;

	rc = pthread_cond_timedwait(&(e->condition), &(e->mutex), &tm);

	switch (rc) {
	case ETIMEDOUT:
		ret = wait_TIMEOUT;
		break;

	case 0:
		ret = wait_OBJECT_0;
		break;

	default:
		ret = wait_ERROR;
		break;
	}

	rc = pthread_mutex_unlock(&(e->mutex));
	if (rc)
		return wait_ERROR;

	return ret;
}

/*-------------------------------------------------------------------------*/
int32_t createThread(ThreadFunc threadFunc, thread_t *threadData)
{
	int32_t ret;

	ret = createEvent(&threadData->stop);
	if (ret != 0) {
		assert(0);
		return -1;
	}

	ret = createEvent(&threadData->stopped);
	if (ret != 0) {
		assert(0);
		return -1;
	}

	ret = pthread_create(&threadData->thread, // thread id (result)
			     NULL, // default attributes
			     threadFunc, // start routine
			     threadData // arguments
	);

	if (ret != 0) {
		assert(0);
		return -1;
	}

	return 0;
}

/*-------------------------------------------------------------------------*/
void destroyThread(thread_t *threadData, uint32_t timeout_ms)
{
	int32_t ret;

	setEvent(&threadData->stop);
	sleep(0);

	ret = waitForSingleObject(&threadData->stopped, timeout_ms);
	if (ret == wait_TIMEOUT)
		pthread_cancel(threadData->thread);
	else
		pthread_join(threadData->thread, NULL);

	sleep(0);
	closeEvent(&threadData->stop);
	closeEvent(&threadData->stopped);
}

/*-------------------------------------------------------------------------*/
uint32_t getTickCount()
{
	struct timeval tv;

	gettimeofday(&tv, NULL);

	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

/*-------------------------------------------------------------------------*/
void exitThread(uint32_t exitcode)
{
	pthread_exit((void *)(&exitcode));
}

/*-------------------------------------------------------------------------*/
int32_t memcpy_s(void *dest, int32_t dest_size, const void *src, int32_t src_size)
{
	if (dest_size < src_size)
		return -1;

	if ((dest == NULL) || (src == NULL))
		return -1;

	memcpy(dest, src, src_size);

	return 0;
}

/*-------------------------------------------------------------------------*/
uint32_t getLastError()
{
	return errno;
}

void Sleep(uint32_t time)
{
	uint32_t microsec = time * 1000;

	usleep(microsec);
}

#endif /* PLATFORM(Linux) */

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/

#if !PLATFORM(Windows) && !PLATFORM(WindowsCE)

/*-------------------------------------------------------------------------*/
void initializeCriticalSection(critical_section_t *cs)
{
	int32_t ret;

	ret = pthread_mutex_init(&cs->mutex, /* pthread_mutex_t *mutex */
				 0 /* const pthread_mutexattr_t *attr */
	);

	assert(ret == 0);
}

/*-------------------------------------------------------------------------*/
void deleteCriticalSection(critical_section_t *cs)
{
	int32_t ret;

	ret = pthread_mutex_destroy(&cs->mutex /*pthread_mutex_t *mutex */
	);

	assert(ret == 0);
}

/*-------------------------------------------------------------------------*/
void enterCriticalSection(critical_section_t *cs)
{
	pthread_mutex_lock(&cs->mutex);
}

/*-------------------------------------------------------------------------*/
void leaveCriticalSection(critical_section_t *cs)
{
	pthread_mutex_unlock(&cs->mutex);
}

#endif /* !PLATFORM(Windows) && !PLATFORM(WindowsCE) */

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/

#if PLATFORM(Windows) || PLATFORM(WindowsCE)

int32_t createThread(ThreadFunc threadFunc, thread_t *threadData)
{
	threadData->stop.handle = CreateEvent(NULL, //LPSECURITY_ATTRIBUTES lpEventAttributes - NULL
					      0, //BOOL bManualReset - auto reset
					      0, //BOOL bInitialState - non signalized
					      NULL //LPCWSTR lpName - NULL
	);

	threadData->handle = CreateThread(NULL, // LPSECURITY_ATTRIBUTES
					  0, // stack size (0=default)
					  threadFunc, // LPTHREAD_START_ROUTINE
					  threadData, // arguments
					  0, // creation flags (0 = runs immediately)
					  NULL // thread id
	);

	assert(threadData->handle);
	assert(threadData->stop.handle);
	SetThreadPriority(threadData->handle, THREAD_PRIORITY_ABOVE_NORMAL);

	return 0;
}

void destroyThread(thread_t *threadData, uint32_t timeout_ms)
{
	setEvent(&threadData->stop);

	if (WaitForSingleObject(threadData->handle, timeout_ms) != WAIT_OBJECT_0)
		TerminateThread(threadData->handle, 0);

	CloseHandle(threadData->stop.handle);
	CloseHandle(threadData->handle);
}

void exitThread(uint32_t exitcode)
{
	ExitThread(exitcode);
}

void setEvent(event_t *e)
{
	assert(e);
	SetEvent(e->handle);
}

int32_t createEvent(event_t *e)
{
	assert(e);

	e->handle = CreateEvent(NULL, //LPSECURITY_ATTRIBUTES lpEventAttributes - NULL
				0, //BOOL bManualReset - auto reset
				0, //BOOL bInitialState - non signalized
				NULL //LPCWSTR lpName - NULL
	);

	assert(e->handle);
	return 0;
}

int32_t closeEvent(event_t *e)
{
	assert(e);
	CloseHandle(e->handle);
	e->handle = NULL;

	return 0;
}

int32_t waitForSingleObject(event_t *e, uint32_t milliseconds)
{
	uint32_t ret;

	assert(e);
	ret = WaitForSingleObject(e->handle, milliseconds);

	switch (ret) {
	case WAIT_OBJECT_0:
		return wait_OBJECT_0;

	case WAIT_TIMEOUT:
		return wait_TIMEOUT;

	default:
		break;
	}

	return wait_ERROR;
}

void initializeCriticalSection(critical_section_t *cs)
{
	assert(cs);
	InitializeCriticalSection(&cs->cs);
}

void deleteCriticalSection(critical_section_t *cs)
{
	assert(cs);
	DeleteCriticalSection(&cs->cs);
}

void enterCriticalSection(critical_section_t *cs)
{
	assert(cs);
	EnterCriticalSection(&cs->cs);
}

void leaveCriticalSection(critical_section_t *cs)
{
	assert(cs);
	LeaveCriticalSection(&cs->cs);
}

uint32_t getTickCount()
{
	return GetTickCount();
}

uint32_t getLastError()
{
	return GetLastError();
}

uint32_t getCurrentProcessId()
{
	return GetCurrentProcessId();
}

#endif /* PLATFORM(Windows) || PLATFORM(WindowsCE) */

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/

#if PLATFORM(VxWorks)

#include <errno.h>
#include <time.h>
#include <sys/ioctl.h>
#include <sockLib.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <private/rtpLibP.h>
#include <ioLib.h>

/*-------------------------------------------------------------------------*/
uint32_t getTickCount()
{
	struct timespec tp;

	clock_gettime(CLOCK_REALTIME, &tp);

	return (tp.tv_sec * 1000 + (tp.tv_nsec / 1000000));
}

/*-------------------------------------------------------------------------*/
int32_t memcpy_s(void *dest, int32_t dest_size, const void *src, int32_t src_size)
{
	if (dest_size < src_size)
		return -1;

	if ((dest == NULL) || (src == NULL))
		return -1;

	memcpy(dest, src, src_size);

	return 0;
}

/*-------------------------------------------------------------------------*/
uint32_t getLastError()
{
	return errno;
}

#endif /* PLATFORM (VxWorks) */
