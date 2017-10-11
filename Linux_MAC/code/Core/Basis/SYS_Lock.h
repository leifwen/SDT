/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: SYS_Lock.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2012.02.09
 * @2013.12.11 : update Thread to make more safe.
 * @2013.06.30 : change to Liunx style
 				add init method
 				use int32 to replace bool
 				use int32 to replace uint32
   @2014.05.24 : form PublicSYS.h
				change PUB_Clock to SYS_ThreadLock
				change PUB_Thread to SYS_Thread
				change PUB_WaitMS to SYS_WaitMS
*/
#ifndef SYS_LockH
#define SYS_LockH
//------------------------------------------------------------------------------------------//
#include "BasicDef.h"
#include <atomic>
#ifdef  CommonDefH_Unix
#include <pthread.h>
#endif
//------------------------------------------------------------------------------------------//
enum	G_LOCK	{G_LOCK_OFF = 0		,G_LOCK_ON};
//------------------------------------------------------------------------------------------//
#ifdef USE_MUTEXLOCK
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_Unix
#ifdef CommonDefH_MAC
	#define		PTHREAD_MUTEX_RECURSIVE_NP		PTHREAD_MUTEX_RECURSIVE
#endif
class SYS_Lock{
	public:
		 SYS_Lock(void){
				pthread_mutexattr_t			attr;
				pthread_mutexattr_init		(&attr);
				pthread_mutexattr_settype	(&attr,PTHREAD_MUTEX_RECURSIVE_NP);
				pthread_mutex_init			(&m_Mutex, &attr);
		};
		virtual ~SYS_Lock(void){pthread_mutex_destroy(&m_Mutex);};
	private:
			pthread_mutex_t		m_Mutex;
	public:
		inline	void	Lock	(G_LOCK blVaild = G_LOCK_ON){
			if (blVaild == G_LOCK_ON)
				pthread_mutex_lock(&m_Mutex);
		};
		inline	int32	TryLock	(G_LOCK blVaild = G_LOCK_ON){
			if (blVaild == G_LOCK_ON)
				return(pthread_mutex_trylock(&m_Mutex) == 0);
			return 1;
		};
		inline	void	Unlock	(G_LOCK blVaild = G_LOCK_ON){
			if (blVaild == G_LOCK_ON)
				pthread_mutex_unlock(&m_Mutex);
		};
};
#endif
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_VC
class SYS_Lock{
	public:
			 SYS_Lock(void){InitializeCriticalSection(&m_Mutex);};
	virtual ~SYS_Lock(void){DeleteCriticalSection(&m_Mutex);};
	private:
		CRITICAL_SECTION		m_Mutex;
	public:
		inline	void	Lock	(G_LOCK blVaild = G_LOCK_ON){
			if (blVaild == G_LOCK_ON)
				EnterCriticalSection(&m_Mutex);
		};
		inline	int32	TryLock	(G_LOCK blVaild = G_LOCK_ON){
			if (blVaild == G_LOCK_ON)
				return(TryEnterCriticalSection(&m_Mutex));
			return 1;
		};
		inline	void	Unlock	(G_LOCK blVaild = G_LOCK_ON){
			if (blVaild == G_LOCK_ON)
				LeaveCriticalSection(&m_Mutex);
		};
};
#endif
//------------------------------------------------------------------------------------------//
#else
class SYS_Lock{
	public:
				 SYS_Lock(void){m_Mutex.clear();};
		virtual ~SYS_Lock(void){m_Mutex.clear();};
	private:
		std::atomic_flag	m_Mutex;
	public:
		inline	void	Lock	(G_LOCK blVaild = G_LOCK_ON){
			if (blVaild == G_LOCK_ON)
				while(m_Mutex.test_and_set(std::memory_order_acquire));
		};
		inline	int32	TryLock	(G_LOCK blVaild = G_LOCK_ON){
			if (blVaild == G_LOCK_ON)
				return(m_Mutex.test_and_set(std::memory_order_acquire) == 0);
			return 1;
		};
		inline	void	Unlock	(G_LOCK blVaild = G_LOCK_ON){
			if (blVaild == G_LOCK_ON)
				m_Mutex.clear(std::memory_order_release);
		};
};
#endif
//------------------------------------------------------------------------------------------//
#endif
