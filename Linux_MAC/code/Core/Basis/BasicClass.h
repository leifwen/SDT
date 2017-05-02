/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: BasicClass.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.05.24
 * @2015.9.10 :	change uint64 to atomic_ullong
 				Use inline fun to redefine B_FLAG64,B_SetFLAG64,B_ClrFLAG64,B_ChkFLAG64
 *
*/

#ifndef BasicClassH
#define BasicClassH
//------------------------------------------------------------------------------------------//
#include "BasicDef.h"
#include "SYS_Lock.h"
//------------------------------------------------------------------------------------------//
#define BD_FLAG64(offset)				(((uint64)0x01) << (offset))
#define BD_FLAG32(offset)				(((uint32)0x01) << (offset))
//#define B_SetFLAG64(variable,u64)		(variable |= (u64))
//#define B_ClrFLAG64(variable,u64)		(variable &= (~u64))
//#define B_ChkFLAG64(variable,u64)		((variable & (u64)) != 0)
inline uint64	B_FLAG64	(uint64 offset)						{return((uint64)0x01 << offset);};
inline uint64&	B_SetFLAG64	(uint64 &variable,uint64 u64)		{return(variable |= u64);};
inline uint64&	B_ClrFLAG64	(uint64 &variable,uint64 u64)		{return(variable &= (~u64));};
inline int32	B_ChkFLAG64	(uint64 variable,uint64 u64)		{return((variable & u64) != 0);};
inline uint64	B_FLAG32	(uint32 offset)						{return((uint64)0x01 << offset);};
inline uint32&	B_SetFLAG32	(uint32 &variable,uint32 u32)		{return(variable |= u32);};
inline uint32&	B_ClrFLAG32	(uint32 &variable,uint32 u32)		{return(variable &= (~u32));};
inline int32	B_ChkFLAG32	(uint32 variable,uint32 u32)		{return((variable & u32) != 0);};
//------------------------------------------------------------------------------------------//
#define RFLAG_CREATE(offset)			(((uint64)0x01) << (RFLAG_S + (offset)))
//------------------------------------------------------------------------------------------//
class BASIC_CFLAG{
	public:
		enum{RFLAG_C = 0,RFLAG_S = 0};
	public:
				 BASIC_CFLAG	(void){blFlag = 0;};
		virtual	~BASIC_CFLAG	(void){;};
	private:
		std::atomic_ullong		blFlag;
	private:
		SYS_Lock	cgInUse;
		SYS_Lock	cgInDoing;
	public:
		inline void		Spin_InUse_set(G_LOCK blVaild = G_LOCK_ON){cgInUse.Lock(blVaild);};
		inline void		Spin_InUse_clr(G_LOCK blVaild = G_LOCK_ON){cgInUse.Unlock(blVaild);};
		inline int32	Spin_InUse_try(G_LOCK blVaild = G_LOCK_ON){return(cgInUse.TryLock(blVaild));};
	
		inline	void	InDoing_set		(G_LOCK blVaild = G_LOCK_ON){cgInDoing.Lock(blVaild);};
		inline	void	InDoing_clr		(G_LOCK blVaild = G_LOCK_ON){cgInDoing.Unlock(blVaild);};
		inline	int32	InDoing_try		(G_LOCK blVaild = G_LOCK_ON){return(cgInDoing.TryLock(blVaild));};
	protected:
		inline void		ClearAllFlag	(void)							{blFlag = 0;};
		inline void		SetSFlag		(const uint64 &rStatus)			{blFlag |= rStatus;};
		inline void		ClrSFlag		(const uint64 &rStatus)			{blFlag &= (~rStatus);};
		inline int32	CheckSFlag		(const uint64 &rStatus) const	{return((blFlag & rStatus) != 0);};
};
//------------------------------------------------------------------------------------------//
#endif
