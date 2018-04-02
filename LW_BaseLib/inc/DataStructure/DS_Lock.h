//
//  DS_Lock.h
//  SDT
//
//  Created by Leif Wen on 18/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#ifndef DS_Lock_h
#define DS_Lock_h
//------------------------------------------------------------------------------------------//
#include "BasicDefine.h"
#include <atomic>
#ifdef  CommonDefH_Unix
#include <pthread.h>
#endif
#ifdef CommonDefH_VC
#include <windows.h>
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
class DS_Lock{
	public:
				DS_Lock(void){
					pthread_mutexattr_t			attr;
					pthread_mutexattr_init		(&attr);
					pthread_mutexattr_settype	(&attr,PTHREAD_MUTEX_RECURSIVE_NP);
					pthread_mutex_init			(&m_Mutex, &attr);
				};
		virtual ~DS_Lock(void){pthread_mutex_destroy(&m_Mutex);};
	private:
		pthread_mutex_t		m_Mutex;
	public:
		inline	void	Lock	(G_LOCK blVaild = G_LOCK_ON){
			if (blVaild == G_LOCK_ON)
				pthread_mutex_lock(&m_Mutex);
		};
		inline	bool32	TryLock	(G_LOCK blVaild = G_LOCK_ON){
			if (blVaild == G_LOCK_ON)
				return(pthread_mutex_trylock(&m_Mutex) == 0);
			return G_TRUE;
		};
		inline	void	Unlock	(G_LOCK blVaild = G_LOCK_ON){
			if (blVaild == G_LOCK_ON)
				pthread_mutex_unlock(&m_Mutex);
		};
};
#endif
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_VC
class DS_Lock{
	public:
				 DS_Lock(void){InitializeCriticalSection(&m_Mutex);};
		virtual ~DS_Lock(void){DeleteCriticalSection(&m_Mutex);};
	private:
		CRITICAL_SECTION		m_Mutex;
	public:
		inline	void	Lock	(G_LOCK blVaild = G_LOCK_ON){
			if (blVaild == G_LOCK_ON)
				EnterCriticalSection(&m_Mutex);
		};
		inline	bool32	TryLock	(G_LOCK blVaild = G_LOCK_ON){
			if (blVaild == G_LOCK_ON)
				return(TryEnterCriticalSection(&m_Mutex));
			return G_TRUE;
		};
		inline	void	Unlock	(G_LOCK blVaild = G_LOCK_ON){
			if (blVaild == G_LOCK_ON)
				LeaveCriticalSection(&m_Mutex);
		};
};
#endif
//------------------------------------------------------------------------------------------//
#else
class DS_Lock{
	public:
				 DS_Lock(void){m_Mutex.clear();};
		virtual ~DS_Lock(void){m_Mutex.clear();};
	private:
		std::atomic_flag	m_Mutex;
	public:
		inline	void	Lock	(G_LOCK blVaild = G_LOCK_ON){
			if (blVaild == G_LOCK_ON)
				while(m_Mutex.test_and_set(std::memory_order_acquire));
		};
		inline	bool32	TryLock	(G_LOCK blVaild = G_LOCK_ON){
			if (blVaild == G_LOCK_ON)
				return(m_Mutex.test_and_set(std::memory_order_acquire) == 0);
			return G_TRUE;
		};
		inline	void	Unlock	(G_LOCK blVaild = G_LOCK_ON){
			if (blVaild == G_LOCK_ON)
				m_Mutex.clear(std::memory_order_release);
		};
};
#endif
//------------------------------------------------------------------------------------------//
#endif /* SYS_Lock_h */
