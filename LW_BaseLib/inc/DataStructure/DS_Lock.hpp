//
//  DS_Lock.hpp
//  SDT
//
//  Created by Leif Wen on 2019/4/28.
//  Copyright © 2019 Leif Wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#ifndef DS_Lock_hpp
#define DS_Lock_hpp
//------------------------------------------------------------------------------------------//
#include "DS_Lock.h"
#ifdef DS_Lock_h
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_Linux
#include <assert.h>
#endif
//------------------------------------------------------------------------------------------//
inline const std::thread::id& GetNullThreadID(void){
	static std::thread::id	nullThreadID;
	return(nullThreadID);
};
//------------------------------------------------------------------------------------------//
inline DS_RWLock::DS_RWLock(bool32 blWriteFirst) : cgblWriteFirst(blWriteFirst){
	cgLockCount.store(0);
	cgWriteWaitCount.store(0);
	cgWThreadID = GetNullThreadID();
};
//------------------------------------------------------------------------------------------//
inline void DS_RWLock::W_set(G_LOCK blVaild){
	if (blVaild == G_LOCK_OFF)
		return;
	if (cgWThreadID != std::this_thread::get_id()){
		if (cgblWriteFirst){
			cgWriteWaitCount.fetch_add(1);
			for(int count = 0;!atomic_compare_exchange_weak(&cgLockCount,&count,-1);count = 0);
			cgWriteWaitCount.fetch_sub(1);
		}
		else{
			for(int count = 0;!atomic_compare_exchange_weak(&cgLockCount,&count,-1);count = 0);
		}
		cgWThreadID = std::this_thread::get_id();
	}
	else{
		cgLockCount.fetch_sub(1);
	}
};
//------------------------------------------------------------------------------------------//
inline bool32 DS_RWLock::W_try(G_LOCK blVaild){
	bool32	ret = G_TRUE;
	if (blVaild == G_LOCK_OFF)
		return G_TRUE;
	if (cgWThreadID != std::this_thread::get_id()){
		int count = 0;
		if (cgblWriteFirst){
			cgWriteWaitCount.fetch_add(1);
			ret = atomic_compare_exchange_weak(&cgLockCount,&count,-1);
			cgWriteWaitCount.fetch_sub(1);
		}
		else{
			ret = atomic_compare_exchange_weak(&cgLockCount,&count,-1);
		}
		if (ret)
			cgWThreadID = std::this_thread::get_id();
	}
	else{
		cgLockCount.fetch_sub(1);
	}
	return(ret);
};
//------------------------------------------------------------------------------------------//
inline void DS_RWLock::W_clr(G_LOCK blVaild){
	if (blVaild == G_LOCK_OFF)
		return;
	if (cgWThreadID != std::this_thread::get_id())
		throw std::runtime_error("wLock/wUnlock mismatch");
	assert(cgLockCount < 0);
	if (cgLockCount == -1){
		cgWThreadID = GetNullThreadID();
		cgLockCount.store(0);
	}
	else{
		cgLockCount.fetch_add(1);
	}
};
//------------------------------------------------------------------------------------------//
inline void DS_RWLock::R_set(G_LOCK blVaild){
	if (blVaild == G_LOCK_OFF)
		return;
	if (cgWThreadID != std::this_thread::get_id()){
		int	count = cgLockCount.load();
		do{
			if (count < 0)
				count = cgWriteWaitCount;
		}while(!atomic_compare_exchange_weak(&cgLockCount,&count, count + 1));
	}
};
//------------------------------------------------------------------------------------------//
inline bool32 DS_RWLock::R_try(G_LOCK blVaild){
	bool32	ret = G_TRUE;
	if (blVaild == G_LOCK_OFF)
		return G_TRUE;
	if (cgWThreadID != std::this_thread::get_id()){
		int	count = cgLockCount.load();
		if (count < 0)
			count = cgWriteWaitCount;
		ret = atomic_compare_exchange_weak(&cgLockCount,&count, count + 1);
	}
	return(ret);
};
//------------------------------------------------------------------------------------------//
inline void DS_RWLock::R_clr(G_LOCK blVaild){
	if (blVaild == G_LOCK_OFF)
		return;
	if (cgWThreadID != std::this_thread::get_id())
		cgLockCount.fetch_sub(1);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
inline DS_SpinLock::DS_SpinLock(void){
	cgSpin.clear();
	cgLockCount.store(0);
	cgThreadID = GetNullThreadID();
};
//------------------------------------------------------------------------------------------//
inline void DS_SpinLock::Set(G_LOCK blVaild){
	if (blVaild == G_LOCK_OFF)
		return;
	if (cgThreadID != std::this_thread::get_id()){
		while(cgSpin.test_and_set(std::memory_order_acquire));
		cgThreadID = std::this_thread::get_id();
	}
	else{
		cgLockCount.fetch_add(1);
	}
};
//------------------------------------------------------------------------------------------//
inline bool32 DS_SpinLock::Try(G_LOCK blVaild){
	bool32	ret = G_TRUE;
	if (blVaild == G_LOCK_OFF)
		return G_TRUE;
	if (cgThreadID != std::this_thread::get_id()){
		ret = (cgSpin.test_and_set(std::memory_order_acquire) == 0);
		if (ret)
			cgThreadID = std::this_thread::get_id();
	}
	else{
		cgLockCount.fetch_add(1);
	}
	return(ret);
};
//------------------------------------------------------------------------------------------//
inline void DS_SpinLock::Clr(G_LOCK blVaild){
	if (blVaild == G_LOCK_OFF)
		return;
	if (cgThreadID != std::this_thread::get_id())
		throw std::runtime_error("wLock/wUnlock mismatch");
	assert(cgLockCount >= 0);
	if (cgLockCount > 0){
		cgLockCount.fetch_sub(1);
	}
	else{
		cgThreadID = GetNullThreadID();
		cgLockCount.store(0);
		cgSpin.clear(std::memory_order_release);
	}
};
//------------------------------------------------------------------------------------------//
#endif
#endif /* DS_Lock_hpp */
