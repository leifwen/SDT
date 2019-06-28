//
//  DS_Bitmap.cpp
//  SDT
//
//  Created by Leif Wen on 2019/4/22.
//  Copyright © 2019 Leif Wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "DS_Bitmap.h"
#ifdef DS_Bitmap_h
//------------------------------------------------------------------------------------------//
#define DIV_ROUND_UP(n,d) (((n) + (d) - 1) / (d))
#define BITS_PER_LONG		64
#define BITOP_WORD(nr)		((nr) / BITS_PER_LONG)
#define BITMAP_FIRST_WORD_MASK(start) (~((uint64)0) << ((start) & (BITS_PER_LONG - 1)))
#define BITMAP_LAST_WORD_MASK(nbits) (~((uint64)0) >> ((0-(nbits)) & (BITS_PER_LONG - 1)))
#define BITS_PER_BYTE		8
#define BITS_TO_LONGS(nr)	DIV_ROUND_UP(nr, BITS_PER_BYTE * sizeof(long))
#define BIT_WORD(nr)		((nr) / BITS_PER_LONG)

#ifdef CommonDefH_VC2013
	#define round_down(x, y) ((x) & ~(uint64)((y)-1))
#else
	#define __round_mask(x, y) ((__typeof__(x))((y)-1))
	#define round_up(x, y) ((((x)-1) | __round_mask(x, y))+1)
	#define round_down(x, y) ((x) & ~__round_mask(x, y))
#endif
//------------------------------------------------------------------------------------------//
/**
 * __ffs - find first bit in word.
 * @word: The word to search
 *
 * Undefined if no bit exists, so code should check against 0 first.
 */
static uint64 __ffs(uint64 word){
	uint32 num = 0;
	
#if BITS_PER_LONG == 64
	if ((word & 0xffffffff) == 0) {
		num += 32;
		word >>= 32;
	}
#endif
	if ((word & 0xffff) == 0) {
		num += 16;
		word >>= 16;
	}
	if ((word & 0xff) == 0) {
		num += 8;
		word >>= 8;
	}
	if ((word & 0xf) == 0) {
		num += 4;
		word >>= 4;
	}
	if ((word & 0x3) == 0) {
		num += 2;
		word >>= 2;
	}
	if ((word & 0x1) == 0)
		num += 1;
	return(num);
};
//------------------------------------------------------------------------------------------//
/**
 * __fls - find last (most-significant) set bit in a long word
 * @word: the word to search
 *
 * Undefined if no set bit exists, so code should check against 0 first.
 */
static uint64 __fls(uint64 word){
	uint32 num = BITS_PER_LONG - 1;
	
#if BITS_PER_LONG == 64
	if (!(word & (~(uint64)0 << 32))) {
		num -= 32;
		word <<= 32;
	}
#endif
	if (!(word & (~(uint64)0 << (BITS_PER_LONG - 16)))) {
		num -= 16;
		word <<= 16;
	}
	if (!(word & (~(uint64)0 << (BITS_PER_LONG - 8)))) {
		num -= 8;
		word <<= 8;
	}
	if (!(word & (~(uint64)0 << (BITS_PER_LONG - 4)))) {
		num -= 4;
		word <<= 4;
	}
	if (!(word & (~(uint64)0 << (BITS_PER_LONG - 2)))) {
		num -= 2;
		word <<= 2;
	}
	if (!(word & (~(uint64)0 << (BITS_PER_LONG - 1))))
		num -= 1;
	return(num);
};
//------------------------------------------------------------------------------------------//
#define ffz(x)  __ffs(~(x))
//------------------------------------------------------------------------------------------//
static uint64 _find_next_bit(const uint64 *addr,uint64 nbits, uint64 start, uint64 invert){
	uint64 tmp;
	
	if (start >= nbits)
		return nbits;
	
	tmp = addr[start / BITS_PER_LONG] ^ invert;
	
	/* Handle 1st word. */
	tmp &= BITMAP_FIRST_WORD_MASK(start);
	start = round_down(start, BITS_PER_LONG);
	
	while (!tmp) {
		start += BITS_PER_LONG;
		if (start >= nbits)
			return nbits;
		
		tmp = addr[start / BITS_PER_LONG] ^ invert;
	}
	start += __ffs(tmp);
	return((start < nbits) ? start : nbits);
};
//------------------------------------------------------------------------------------------//
/*
 * Find the first set bit in a memory region.
 */
uint64 find_first_bit(const uint64 *addr, uint64 size){
	uint64 idx,tmp;
	
	for (idx = 0; idx * BITS_PER_LONG < size; idx++) {
		if (addr[idx]){
			tmp = idx * BITS_PER_LONG + __ffs(addr[idx]);
			return(tmp < size ? tmp : size);
		}
	}
	
	return(size);
};
//------------------------------------------------------------------------------------------//
/*
 * Find the first cleared bit in a memory region.
 */
uint64 find_first_zero_bit(const uint64 *addr, uint64 size){
	uint64 idx,tmp;
	
	for (idx = 0; idx * BITS_PER_LONG < size; idx++) {
		if (addr[idx] != ~((uint64)0)){
			tmp = idx * BITS_PER_LONG + ffz(addr[idx]);
			return(tmp < size ? tmp : size);
		}
	}
	
	return(size);
};
//------------------------------------------------------------------------------------------//
uint64 find_last_bit(const uint64 *addr, uint64 size){
	if (size) {
		uint64 val = BITMAP_LAST_WORD_MASK(size);
		uint64 idx = (size-1) / BITS_PER_LONG;
		
		do {
			val &= addr[idx];
			if (val)
				return(idx * BITS_PER_LONG + __fls(val));
			
			val = ~0ul;
		} while (idx--);
	}
	return(size);
};
//------------------------------------------------------------------------------------------//
/**
 * __bitmap_shift_right - logical right shift of the bits in a bitmap
 *   @dst : destination bitmap
 *   @src : source bitmap
 *   @shift : shift by this many bits
 *   @nbits : bitmap size, in bits
 *
 * Shifting right (dividing) means moving bits in the MS -> LS bit
 * direction.  Zeros are fed into the vacated MS positions and the
 * LS bits shifted off the bottom are lost.
 */
void __bitmap_shift_right(uint64 *dst, const uint64 *src,uint64 shift, uint64 nbits){
	uint64 k, lim = BITS_TO_LONGS(nbits);
	uint64 off = shift/BITS_PER_LONG, rem = shift % BITS_PER_LONG;
	uint64 mask = BITMAP_LAST_WORD_MASK(nbits);
	for (k = 0; off + k < lim; ++k) {
		uint64 upper, lower;
		
		/*
		 * If shift is not word aligned, take lower rem bits of
		 * word above and make them the top rem bits of result.
		 */
		if (!rem || off + k + 1 >= lim)
			upper = 0;
		else {
			upper = src[off + k + 1];
			if (off + k + 1 == lim - 1)
				upper &= mask;
			upper <<= (BITS_PER_LONG - rem);
		}
		lower = src[off + k];
		if (off + k == lim - 1)
			lower &= mask;
		lower >>= rem;
		dst[k] = lower | upper;
	}
	if (off)
		memset(&dst[lim - off], 0, off*sizeof(uint64));
};
//------------------------------------------------------------------------------------------//
/**
 * __bitmap_shift_left - logical left shift of the bits in a bitmap
 *   @dst : destination bitmap
 *   @src : source bitmap
 *   @shift : shift by this many bits
 *   @nbits : bitmap size, in bits
 *
 * Shifting left (multiplying) means moving bits in the LS -> MS
 * direction.  Zeros are fed into the vacated LS bit positions
 * and those MS bits shifted off the top are lost.
 */

void __bitmap_shift_left(uint64 *dst, const uint64 *src,uint64 shift, uint64 nbits){
	int64 k;
	uint64 lim = BITS_TO_LONGS(nbits);
	uint64 off = shift/BITS_PER_LONG, rem = shift % BITS_PER_LONG;
	for (k = lim - off - 1; k >= 0; --k) {
		uint64 upper, lower;
		
		/*
		 * If shift is not word aligned, take upper rem bits of
		 * word below and make them the bottom rem bits of result.
		 */
		if (rem && k > 0)
			lower = src[k - 1] >> (BITS_PER_LONG - rem);
		else
			lower = 0;
		upper = src[k] << rem;
		dst[k + off] = lower | upper;
	}
	if (off)
		memset(dst, 0, off*sizeof(uint64));
};
//------------------------------------------------------------------------------------------//
void __bitmap_set(uint64 *map, uint64 start, int64 len){
	uint64 *p = map + BIT_WORD(start);
	const uint64 size = start + len;
	int64 bits_to_set = BITS_PER_LONG - (start % BITS_PER_LONG);
	uint64 mask_to_set = BITMAP_FIRST_WORD_MASK(start);
	
	while (len - bits_to_set >= 0) {
		*p |= mask_to_set;
		len -= bits_to_set;
		bits_to_set = BITS_PER_LONG;
		mask_to_set = ~0UL;
		p++;
	}
	if (len) {
		mask_to_set &= BITMAP_LAST_WORD_MASK(size);
		*p |= mask_to_set;
	}
};
//------------------------------------------------------------------------------------------//
void __bitmap_clear(uint64 *map, uint64 start, int64 len){
	uint64 *p = map + BIT_WORD(start);
	const uint64 size = start + len;
	int64 bits_to_clear = BITS_PER_LONG - (start % BITS_PER_LONG);
	uint64 mask_to_clear = BITMAP_FIRST_WORD_MASK(start);
	
	while (len - bits_to_clear >= 0) {
		*p &= ~mask_to_clear;
		len -= bits_to_clear;
		bits_to_clear = BITS_PER_LONG;
		mask_to_clear = ~0UL;
		p++;
	}
	if (len) {
		mask_to_clear &= BITMAP_LAST_WORD_MASK(size);
		*p &= ~mask_to_clear;
	}
};
//------------------------------------------------------------------------------------------//







//------------------------------------------------------------------------------------------//
DS_BITMAP::DS_BITMAP(void){
	cgBitmap = nullptr;
	cgNBits = 0;
};
//------------------------------------------------------------------------------------------//
DS_BITMAP::DS_BITMAP(uint64 nbits){
	cgBitmap = nullptr;
	cgNBits = 0;
	InitSize(nbits);
};
//------------------------------------------------------------------------------------------//
DS_BITMAP::~DS_BITMAP(void){
	if (cgBitmap != nullptr){
		try{
			delete []cgBitmap;
		}
		catch(...){}
	}
	cgBitmap = nullptr;
};
//------------------------------------------------------------------------------------------//
uint64 DS_BITMAP::InitSize(uint64 nBits){
	uint64	i;
	uint64	newSize,size;
	uint64	*tempBufP;
	
	newSize = (nBits - 1) / BITS_PER_LONG + 1;
	size = (cgNBits == 0) ? 0 : (cgNBits - 1) / BITS_PER_LONG + 1;
	tempBufP = nullptr;
	if (newSize > size){
		try{
			tempBufP = new uint64[newSize];
		}
		catch(...){
			tempBufP = nullptr;
		}
		if (tempBufP != nullptr){
			if (cgBitmap != nullptr){
				i = 0;
				do{
					tempBufP[i] = cgBitmap[i];
				}while(++i < size);
				while(i < newSize){
					tempBufP[i] = cgBitmap[i];
					++ i;
				};
				try{
					delete []cgBitmap;
				}
				catch(...){}
			}
			else{
				ClrAllBit();
			}
			cgBitmap = tempBufP;
			cgNBits = nBits;
		}
	}
	return(cgNBits);
};
//------------------------------------------------------------------------------------------//
const uint64& DS_BITMAP::MaxSize(void)const{
	return(cgNBits);
};
//------------------------------------------------------------------------------------------//
uint64 DS_BITMAP::FindNextBit(uint64 start){
	return(_find_next_bit(cgBitmap, cgNBits, start, 0));
};
//------------------------------------------------------------------------------------------//
uint64 DS_BITMAP::FindNextZeroBit(uint64 start){
	return(_find_next_bit(cgBitmap, cgNBits, start, ~((uint64)0)));
};
//------------------------------------------------------------------------------------------//
uint64 DS_BITMAP::FindFirstBit(void){
	return(find_first_bit(cgBitmap, cgNBits));
};
//------------------------------------------------------------------------------------------//
uint64 DS_BITMAP::FindFirstZeroBit(void){
	return(find_first_zero_bit(cgBitmap, cgNBits));
};
//------------------------------------------------------------------------------------------//
uint64 DS_BITMAP::FindLastBit(void){
	return(find_last_bit(cgBitmap, cgNBits));
};
//------------------------------------------------------------------------------------------//
bool32 DS_BITMAP::SetOneBit(uint64 offset){
	if (offset < cgNBits){
		B_SetFLAG64(cgBitmap[offset / BITS_PER_LONG],BD_FLAG64(offset % BITS_PER_LONG));
		return G_TRUE;
	}
	return G_FALSE;
};
//------------------------------------------------------------------------------------------//
bool32 DS_BITMAP::ClrOneBit(uint64 offset){
	if (offset < cgNBits){
		B_ClrFLAG64(cgBitmap[offset / BITS_PER_LONG],BD_FLAG64(offset % BITS_PER_LONG));
		return G_TRUE;
	}
	return G_FALSE;
};
//------------------------------------------------------------------------------------------//
bool32 DS_BITMAP::ChkOneBit(uint64 offset){
	if (offset < cgNBits)
		return B_ChkFLAG64(cgBitmap[offset / BITS_PER_LONG],BD_FLAG64(offset % BITS_PER_LONG));
	return G_FALSE;
};
//------------------------------------------------------------------------------------------//
void DS_BITMAP::ShiftRight(uint64 shift){
	__bitmap_shift_right(cgBitmap, cgBitmap, shift, cgNBits);
};
//------------------------------------------------------------------------------------------//
void DS_BITMAP::ShiftLeft(uint64 shift){
	__bitmap_shift_left(cgBitmap, cgBitmap, shift, cgNBits);
};
//------------------------------------------------------------------------------------------//
void DS_BITMAP::SetAllBit(void){
	__bitmap_set(cgBitmap,0,cgNBits);
};
//------------------------------------------------------------------------------------------//
void DS_BITMAP::ClrAllBit(void){
	__bitmap_clear(cgBitmap,0,cgNBits);
};
//------------------------------------------------------------------------------------------//
#endif
