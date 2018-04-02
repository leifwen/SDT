//
//  DS_BaseClass.h
//  SDT
//
//  Created by Leif Wen on 18/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#ifndef DS_BaseClass_h
#define DS_BaseClass_h
//------------------------------------------------------------------------------------------//
#include "BasicDefine.h"
#include "DS_Lock.h"
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#define RFLAG_CREATE(offset)			(((uint64)0x01) << (RFLAG_S + (offset)))
//------------------------------------------------------------------------------------------//
class BASE_FLAG{
	protected:
		enum	{RFLAG_C = 0,RFLAG_S = 0};
	public:
				 BASE_FLAG	(void)	{blFlag = 0;};
		virtual	~BASE_FLAG	(void)	{;};
	private:
		std::atomic_ullong		blFlag;
	private:
		DS_Lock	cgInUse;
		DS_Lock	cgInDoing;
	public:
		inline void		InUse_set		(G_LOCK blVaild = G_LOCK_ON)	{cgInUse.Lock(blVaild);};
		inline void		InUse_clr		(G_LOCK blVaild = G_LOCK_ON)	{cgInUse.Unlock(blVaild);};
		inline bool32	InUse_try		(G_LOCK blVaild = G_LOCK_ON)	{return(cgInUse.TryLock(blVaild));};
		
		inline void		InDoing_set		(G_LOCK blVaild = G_LOCK_ON)	{cgInDoing.Lock(blVaild);};
		inline void		InDoing_clr		(G_LOCK blVaild = G_LOCK_ON)	{cgInDoing.Unlock(blVaild);};
		inline bool32	InDoing_try		(G_LOCK blVaild = G_LOCK_ON)	{return(cgInDoing.TryLock(blVaild));};
	protected:
		inline void		ClearAllFlag	(void)							{blFlag = 0;};
		inline void		SetSFlag		(const uint64& rStatus)			{blFlag |= rStatus;};
		inline void		ClrSFlag		(const uint64& rStatus)			{blFlag &= (~rStatus);};
		inline bool32	CheckSFlag		(const uint64& rStatus) const	{return((blFlag & rStatus) != 0);};
};
//------------------------------------------------------------------------------------------//
#endif /* DS_BaseClass_h */
