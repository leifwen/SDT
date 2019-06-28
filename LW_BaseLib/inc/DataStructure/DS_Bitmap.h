//
//  DS_Bitmap.h
//  SDT
//
//  Created by Leif Wen on 2019/4/22.
//  Copyright © 2019 Leif Wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#ifndef DS_Bitmap_h
#define DS_Bitmap_h
#ifdef DS_Bitmap_h
//------------------------------------------------------------------------------------------//
#include "BasicDefine.h"
//------------------------------------------------------------------------------------------//
class DS_BITMAP{
	public:
				 DS_BITMAP(void);
	 			 DS_BITMAP(uint64 nBits);
		virtual ~DS_BITMAP(void);
	private:
		uint64*		cgBitmap;
		uint64		cgNBits;
	public:
				uint64		InitSize	(uint64 nBits);
		const	uint64&		MaxSize		(void)const;
	public:
		uint64		FindNextBit			(uint64 start);
		uint64		FindNextZeroBit		(uint64 start);
		uint64		FindFirstBit		(void);
		uint64		FindFirstZeroBit	(void);
		uint64		FindLastBit			(void);
	public:
		void		SetAllBit			(void);
		void		ClrAllBit			(void);
		bool32		SetOneBit			(uint64 offset);
		bool32		ClrOneBit			(uint64 offset);
		bool32		ChkOneBit			(uint64 offset);
		void		ShiftRight			(uint64 shift);
		void		ShiftLeft			(uint64 shift);
};
//------------------------------------------------------------------------------------------//
#endif /* DS_Bitmap_h */
#endif /* DS_Bitmap_h */
