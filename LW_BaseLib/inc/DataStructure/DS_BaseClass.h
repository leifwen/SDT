//
//  DS_BaseClass.h
//  SDT
//
//  Created by Leif Wen on 18/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#ifndef DS_BaseClass_h
#define DS_BaseClass_h
//------------------------------------------------------------------------------------------//
#include "BasicDefine.h"
#include <atomic>
//------------------------------------------------------------------------------------------//
#define RFLAG_CREATE(offset)			(((uint64)0x01) << (RFLAG_S + (offset)))
//------------------------------------------------------------------------------------------//
class BASE_FLAG{
	protected:
		enum	{RFLAG_C = 0,RFLAG_S = 0};
	public:
				 BASE_FLAG	(void)	{blFlag.store(0);};
		virtual	~BASE_FLAG	(void)	{;};
	private:
		std::atomic_ullong		blFlag;
	protected:
		inline void		ClearAllFlag	(void)							{blFlag.store(0);};
		inline void		SetSFlag		(const uint64& rStatus)			{blFlag.fetch_or(rStatus);};
		inline void		ClrSFlag		(const uint64& rStatus)			{blFlag.fetch_and(~rStatus);};
		inline bool32	CheckSFlag		(const uint64& rStatus) const	{return((blFlag.load() & rStatus) != 0);};
};
//------------------------------------------------------------------------------------------//
#endif /* DS_BaseClass_h */
