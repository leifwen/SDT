//
//  DS_Lock.h
//  SDT
//
//  Created by Leif Wen on 18/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#ifndef DS_Lock_h
#define DS_Lock_h
#ifdef DS_Lock_h
//------------------------------------------------------------------------------------------//
#include "BasicDefine.h"
#include <atomic>
#include <thread>
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
static	inline const	std::thread::id&	GetNullThreadID(void);
//------------------------------------------------------------------------------------------//
class DS_SpinLock{
	public:
				 DS_SpinLock(void);
	 			 DS_SpinLock(const DS_SpinLock&) = delete;
		virtual ~DS_SpinLock(void){;};
		DS_SpinLock& operator=(const DS_SpinLock&) = delete;
	private:
		std::thread::id		cgThreadID;
		std::atomic_int		cgLockCount;
		std::atomic_flag	cgSpin;
	public:
		inline	void	Set	(G_LOCK blVaild = G_LOCK_ON);
		inline	bool32	Try	(G_LOCK blVaild = G_LOCK_ON);
		inline	void	Clr	(G_LOCK blVaild = G_LOCK_ON);
};
//------------------------------------------------------------------------------------------//
class DS_RWLock{
	public:
		inline	 DS_RWLock(bool32 blWriteFirst = G_FALSE);
				 DS_RWLock(const DS_RWLock&) = delete;
		virtual ~DS_RWLock(void){;};	
		DS_RWLock& operator=(const DS_RWLock&) = delete;
	private:
						const	bool32				cgblWriteFirst;
								std::thread::id		cgWThreadID;
								std::atomic_int		cgLockCount;
								std::atomic_int		cgWriteWaitCount;
	public:
				inline	void	W_set		(G_LOCK blVaild = G_LOCK_ON);
				inline	bool32	W_try		(G_LOCK blVaild = G_LOCK_ON);
				inline	void	W_clr		(G_LOCK blVaild = G_LOCK_ON);
	
				inline	void	R_set		(G_LOCK blVaild = G_LOCK_ON);
				inline	bool32	R_try		(G_LOCK blVaild = G_LOCK_ON);
				inline	void	R_clr		(G_LOCK blVaild = G_LOCK_ON);
};
//------------------------------------------------------------------------------------------//
#include "DS_Lock.hpp"
#endif /* DS_Lock_h */
#endif /* DS_Lock_h */
