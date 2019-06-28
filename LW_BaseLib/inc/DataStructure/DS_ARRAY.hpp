//
//  DS_ARRAY.hpp
//  SDT
//
//  Created by Leif Wen on 18/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#ifndef DS_ARRAY_hpp
#define DS_ARRAY_hpp
//------------------------------------------------------------------------------------------//
#include "DS_ARRAY.h"
#ifdef DS_ARRAY_h
//------------------------------------------------------------------------------------------//
template <typename DS_T> DS_ARRAY_FRAME<DS_T>::DS_ARRAY_FRAME(void){
	cgDataBuffer = nullptr;
	maxSize = 0;
	
	offsetIn = 0;
	offsetOut = 0;

	offsetPreIn = 0;
	countPreIn = 0;
	
	status = DSAF_blNormal;
};
//------------------------------------------------------------------------------------------//
template <typename DS_T> DS_ARRAY_FRAME<DS_T>::~DS_ARRAY_FRAME(void){
	if (cgDataBuffer != nullptr){
		try{
			delete []cgDataBuffer;
		}
		catch(...){}
	}
	cgDataBuffer = nullptr;
};
//------------------------------------------------------------------------------------------//
template <typename DS_T> uint32 DS_ARRAY_FRAME<DS_T>::InitSize(uint32 size){
	int32	i;
	uint32	tSize;
	DS_T	*tempBufP;
	
	tempBufP = nullptr;
	if (size > maxSize){
		tSize = (uint32)0x01 << 5;//the smallest is 32byte
		
		i = 5;
		while (i++ < 32){
			if (size <= tSize){
				size = tSize;
				break;
			}
			tSize = tSize << 1;
		}
		if (size > 1024 * 1024 * 1024)	//the biggest is 1G
			size = 1024 * 1024 * 1024;	//2^10 * 2^10 * 2^10
		try{
			tempBufP = new DS_T[size];
		}
		catch(...){
			tempBufP = nullptr;
		}
		if (tempBufP != nullptr){
			if (cgDataBuffer != nullptr){
				if (countPreIn > 0){
					tSize = Get(tempBufP,offsetPreIn - offsetOut);//copy to new buffer.
				}
				else{
					tSize = Get(tempBufP,offsetIn - offsetOut);//copy to new buffer.
				}
				offsetPreIn = tSize + offsetPreIn - offsetIn;
				offsetIn = tSize;
				offsetOut = 0;
				try{
					delete []cgDataBuffer;
				}
				catch(...){}
			}
			cgDataBuffer = tempBufP;
			maxSize = size;
		}
	}
	return(maxSize);
};
//------------------------------------------------------------------------------------------//
template <typename DS_T> inline void 	DS_ARRAY_FRAME<DS_T>::Empty(void){
	
	offsetOut = offsetIn;
	status = DSAF_blNormal;
};
//------------------------------------------------------------------------------------------//
template <typename DS_T> inline void 	DS_ARRAY_FRAME<DS_T>::Reset(void){
	offsetOut = 0;
	offsetIn = 0;
	offsetPreIn = 0;
	countPreIn = 0;
	
	status = DSAF_blNormal;
};
//------------------------------------------------------------------------------------------//
template <typename DS_T> inline uint32 	DS_ARRAY_FRAME<DS_T>::Unused(void)const{
	
	return((countPreIn == 0)?(maxSize - offsetIn + offsetOut):(maxSize - offsetPreIn + offsetOut));
};
//------------------------------------------------------------------------------------------//
template <typename DS_T> inline uint32 	DS_ARRAY_FRAME<DS_T>::Used(void)const{
	
	return((countPreIn == 0)?(offsetIn - offsetOut):(offsetPreIn - offsetOut));
};
//------------------------------------------------------------------------------------------//
template <typename DS_T> inline	const uint32&	DS_ARRAY_FRAME<DS_T>::MaxSize(void)const{

	return(maxSize);
};
//------------------------------------------------------------------------------------------//
template <typename DS_T> inline bool32 	DS_ARRAY_FRAME<DS_T>::IsEmpty(void)const{
	
	return((countPreIn == 0)?(offsetIn == offsetOut):(offsetPreIn == offsetOut));
};
//------------------------------------------------------------------------------------------//
template <typename DS_T> inline bool32 	DS_ARRAY_FRAME<DS_T>::IsFull(void)const{
	
	return((countPreIn == 0)?(maxSize == (offsetIn - offsetOut)):(maxSize == (offsetPreIn - offsetOut)));
};
//------------------------------------------------------------------------------------------//
template <typename DS_T> inline bool32 	DS_ARRAY_FRAME<DS_T>::IsNoMEM(void)const{
	
	return(status == DSAF_blNoMem);
};
//------------------------------------------------------------------------------------------//
template <typename DS_T> uint32 DS_ARRAY_FRAME<DS_T>::Prepare_Set(void){
	if (countPreIn == 0)
		offsetPreIn = offsetIn;
	++ countPreIn;
	return(countPreIn);
};
//------------------------------------------------------------------------------------------//
template <typename DS_T> uint32 DS_ARRAY_FRAME<DS_T>::Prepare_Clr(bool32 blGiveUp){
	if (--countPreIn == 0){
		if (blGiveUp != G_FALSE)
			offsetPreIn = offsetIn;
		offsetIn = offsetPreIn;
	}
	return(countPreIn);
};
//------------------------------------------------------------------------------------------//
template <typename DS_T> inline	uint32 DS_ARRAY_FRAME<DS_T>::GetOffsetPreIn	(void)const			{return(offsetPreIn);};
template <typename DS_T> inline	uint32 DS_ARRAY_FRAME<DS_T>::GetPreInNum	(void)const			{return(offsetPreIn - offsetIn);};
template <typename DS_T> inline	DS_T*  DS_ARRAY_FRAME<DS_T>::GetPointer		(uint32 offset)const{return(cgDataBuffer + (offset & (maxSize - 1)));};
//------------------------------------------------------------------------------------------//
template <typename DS_T> uint32 DS_ARRAY_FRAME<DS_T>::CalcOutLength(uint32& fLength,uint32& offset)const{
	uint32	usedSize,maxOut;
	usedSize = offsetIn - offsetOut;
	if (offset >= usedSize){
		offset = 0;
		fLength = 0;
		return 0;
	}
	usedSize -= offset;
	maxOut = (usedSize < fLength) ? usedSize : fLength;
	offset = ((offsetOut + offset) & (maxSize - 1));
	fLength = maxSize - offset;
	if (fLength > maxOut)
		fLength = maxOut;
	return(maxOut - fLength);//sLength
};
//------------------------------------------------------------------------------------------//
template <typename DS_T> uint32 DS_ARRAY_FRAME<DS_T>::CalcInLength(uint32& fLength,uint32& offset)const{
	uint32	unUsedSize,maxIn;
	uint32	offsetInT;
	if (countPreIn == 0){
		offsetInT = offsetIn;
	}
	else{
		offsetInT = offsetPreIn;
	}
	unUsedSize = maxSize - offsetInT + offsetOut;
	
	if (offset >= unUsedSize){
		offset = 0;
		fLength = 0;
		return 0;
	}
	unUsedSize -= offset;
	maxIn = (unUsedSize < fLength) ? unUsedSize : fLength;
	offset = ((offsetInT + offset)& (maxSize - 1));
	fLength = maxSize - offset;
	if (fLength > maxIn)
		fLength = maxIn;
	return(maxIn - fLength);//sLength
};
//------------------------------------------------------------------------------------------//
template <typename DS_T> uint32 DS_ARRAY_FRAME<DS_T>::CalcOutCapacity(uint32& fLength,uint32& offset)const{
	uint32	usedSize;
	usedSize = offsetIn - offsetOut;
	if (offset >= usedSize){
		offset = 0;
		fLength = 0;
		return 0;
	}
	usedSize -= offset;
	
	offset = ((offsetOut + offset) & (maxSize - 1));
	fLength = maxSize - offset;
	if (fLength > usedSize)
		fLength = usedSize;
	return(usedSize - fLength);//sLength
};
//------------------------------------------------------------------------------------------//
template <typename DS_T> uint32 DS_ARRAY_FRAME<DS_T>::CalcInCapacity(uint32& fLength,uint32& offset)const{
	uint32	unUsedSize;
	uint32	offsetInT;
	if (countPreIn == 0){
		offsetInT = offsetIn;
	}
	else{
		offsetInT = offsetPreIn;
	}
	unUsedSize = maxSize - offsetInT + offsetOut;
	
	if (offset >= unUsedSize){
		offset = 0;
		fLength = 0;
		return 0;
	}
	unUsedSize -= offset;
	
	offset = ((offsetInT + offset) & (maxSize - 1));
	fLength = maxSize - offset;
	if (fLength > unUsedSize)
		fLength = unUsedSize;
	return(unUsedSize - fLength);//sLength
};
//------------------------------------------------------------------------------------------//
template <typename DS_T> uint32 DS_ARRAY_FRAME<DS_T>::CalcInCapacityAbsolute(uint32& fLength,uint32& offset)const{
	uint32	unUsedSize;
	
	unUsedSize = maxSize - offsetIn + offsetOut;
	
	if (offset >= unUsedSize){
		offset = 0;
		fLength = 0;
		return 0;
	}
	unUsedSize -= offset;
	
	offset = ((offsetIn + offset) & (maxSize - 1));
	fLength = maxSize - offset;
	if (fLength > unUsedSize)
		fLength = unUsedSize;
	return(unUsedSize - fLength);//sLength
};
//------------------------------------------------------------------------------------------//
template <typename DS_T> uint32 DS_ARRAY_FRAME<DS_T>::CalcOutLengthInToPre(uint32& fLength,uint32& offset)const{
	uint32	usedSize,maxOut;
	usedSize = offsetPreIn - offsetIn;
	if (offset >= usedSize){
		offset = 0;
		fLength = 0;
		return 0;
	}
	usedSize -= offset;
	maxOut = (usedSize < fLength) ? usedSize : fLength;
	offset = ((offsetIn + offset) & (maxSize - 1));
	fLength = maxSize - offset;
	if (fLength > maxOut)
		fLength = maxOut;
	return(maxOut - fLength);//sLength
};
//------------------------------------------------------------------------------------------//
template <typename DS_T> uint32 DS_ARRAY_FRAME<DS_T>::CalcOutCapacityInToPre(uint32& fLength,uint32& offset)const{
	uint32	usedSize;
	
	usedSize = offsetPreIn - offsetIn;
	if (offset >= usedSize){
		offset = 0;
		fLength = 0;
		return 0;
	}
	usedSize -= offset;
	
	offset = ((offsetIn + offset) & (maxSize - 1));
	fLength = maxSize - offset;
	if (fLength > usedSize)
		fLength = usedSize;
	return(usedSize - fLength);//sLength
};
//------------------------------------------------------------------------------------------//
template <typename DS_T> uint32 DS_ARRAY_FRAME<DS_T>::Copy(DS_T*& to_addr,uint32& to_length,const DS_T*& in_addr,uint32& in_length)const{
	uint32	i,copyNum;
	i = (to_length <= in_length) ? to_length : in_length;
	copyNum = i;
	while(i-- > 0){
		*to_addr = *in_addr;
		++ to_addr;
		++ in_addr;
	}
	to_length -= copyNum;
	in_length -= copyNum;
	return(copyNum);
};
//------------------------------------------------------------------------------------------//
template <typename DS_T> uint32 DS_ARRAY_FRAME<DS_T>::Put(const DS_T* data,uint32 num){
	uint32	length,offset;
	uint32	slength,copyNum;
	DS_T	*addr;
	
	status = (Unused() >= num) ? DSAF_blNormal : DSAF_blNoMem;
	if ((cgDataBuffer == nullptr) || (num == 0))
		return 0;
	
	copyNum = 0;
	offset = 0;
	slength = CalcInCapacity(length,offset);
	
	if (length > 0){
		addr = cgDataBuffer + offset;
		copyNum += Copy(addr,length,data,num);
		if ((num > 0) && (slength > 0)){
			addr = cgDataBuffer;
			copyNum += Copy(addr,slength,data,num);
		}
		if (countPreIn == 0){
			offsetIn += copyNum;
		}
		else{
			offsetPreIn += copyNum;
		}
	}
	return(copyNum);
};
//------------------------------------------------------------------------------------------//
template <typename DS_T> inline uint32 DS_ARRAY_FRAME<DS_T>::Put(const DS_T& data){return(Put(&data,1));};
//------------------------------------------------------------------------------------------//
template <typename DS_T> uint32 DS_ARRAY_FRAME<DS_T>::Read(DS_T* dataOut,uint32 num,uint32 offset)const{
	uint32		length;
	uint32		slength,copyNum;
	const DS_T	*addr;
	
	if ((cgDataBuffer == nullptr) || (num == 0))
		return 0;
	
	copyNum = 0;
	slength = CalcOutCapacity(length,offset);
	
	if (length > 0){
		addr = cgDataBuffer + offset;
		copyNum = Copy(dataOut,num,addr,length);
		if ((num > 0) && (slength > 0)){
			addr = cgDataBuffer;
			copyNum = Copy(dataOut,num,addr,slength);
		}
	}
	return(copyNum);
};
//------------------------------------------------------------------------------------------//
template <typename DS_T> uint32 DS_ARRAY_FRAME<DS_T>::Get(DS_T *dataOut,uint32 num){
	uint32	copyNum;
	copyNum = Read(dataOut,num,0);
	offsetOut += copyNum;
	return(copyNum);
};
//------------------------------------------------------------------------------------------//
template <typename DS_T> uint32 DS_ARRAY_FRAME<DS_T>::Put(const DS_ARRAY_FRAME<DS_T>& _in,uint32 num,uint32 offset){
	uint32	copyNum;
	uint32	in_1			,in_Offset				,in_2;
	uint32	&out_1 = num	,&out_Offset = offset	,out_2;
			DS_T					*in_addr;
	const	DS_T					*out_addr;
	const	DS_ARRAY_FRAME<DS_T> 	&_out = _in;
	
	status = (Unused() >= (_in.Used() < num ? _in.Used() : num)) ? DSAF_blNormal : DSAF_blNoMem;
	
	if (cgDataBuffer == nullptr)
		return 0;
	copyNum = 0;
	
	in_Offset = 0;
	in_2 = CalcInCapacity(in_1,in_Offset);
	out_2 = _out.CalcOutLength(out_1,out_Offset);
	
	if ((in_1 > 0) && (out_1 > 0)){
		in_addr = cgDataBuffer + in_Offset;
		out_addr = _out.GetPointer(out_Offset);
		copyNum = Copy(in_addr,in_1,out_addr,out_1);
		if ((in_1 > 0) && (out_2 > 0)){
			out_addr = _out.GetPointer(0);
			copyNum += Copy(in_addr,in_1,out_addr,out_2);
			if ((in_2 > 0) && (out_2 > 0)){
				in_addr = cgDataBuffer;
				copyNum += Copy(in_addr,in_2,out_addr,out_2);
			}
		}
		else if ((in_2 > 0) && (out_1 > 0)){
			in_addr = cgDataBuffer;
			copyNum += Copy(in_addr,in_2,out_addr,out_1);
			if ((in_2 > 0) && (out_2 > 0)){
				out_addr = _out.GetPointer(0);
				copyNum += Copy(in_addr,in_2,out_addr,out_2);
			}
		}
		if (countPreIn == 0){
			offsetIn += copyNum;
		}
		else{
			offsetPreIn += copyNum;
		}
	}
	return(copyNum);
};
//------------------------------------------------------------------------------------------//
template <typename DS_T> inline	uint32 DS_ARRAY_FRAME<DS_T>::Get (DS_ARRAY_FRAME<DS_T>* _out,uint32 num){
	
	return(Out(_out->Put(*this,num)));
};
//------------------------------------------------------------------------------------------//
template <typename DS_T> inline	uint32 DS_ARRAY_FRAME<DS_T>::Read(DS_ARRAY_FRAME<DS_T>* _out,uint32 num,uint32 offset)const{
	
	return(_out->Put(*this,num,offset));
};
//------------------------------------------------------------------------------------------//
template <typename DS_T> uint32 DS_ARRAY_FRAME<DS_T>::UpdateByOffsetOut(const DS_T* data,uint32 num,uint32 offset){
	uint32	length;
	uint32	slength,copyNum;
	DS_T	*addr;
	
	if ((cgDataBuffer == nullptr) || (num == 0))
		return 0;
	
	copyNum = 0;
	slength = CalcOutCapacity(length,offset);
	
	if (length > 0){
		addr = cgDataBuffer + offset;
		copyNum = Copy(addr,length,data,num);
		if ((num > 0) && (slength > 0)){
			addr = cgDataBuffer;
			copyNum = Copy(addr,slength,data,num);
		}
	}
	return(copyNum);
};
//------------------------------------------------------------------------------------------//
template <typename DS_T>
uint32 DS_ARRAY_FRAME<DS_T>::UpdateByOffsetIn(const DS_T* data,uint32 num,uint32 offset){
	uint32	length;
	uint32	slength,copyNum;
	DS_T	*addr;
	
	if ((cgDataBuffer == nullptr) || (num == 0))
		return 0;
	
	copyNum = 0;
	slength = CalcInCapacityAbsolute(length,offset);
	
	if (length > 0){
		addr = cgDataBuffer + offset;
		copyNum = Copy(addr,length,data,num);
		if ((num > 0) && (slength > 0)){
			addr = cgDataBuffer;
			copyNum = Copy(addr,slength,data,num);
		}
	}
	return(copyNum);
};
//------------------------------------------------------------------------------------------//
template <typename DS_T> uint32 DS_ARRAY_FRAME<DS_T>::ReadByOffsetIn(DS_T* dataOut,uint32 num,uint32 offset){
	uint32		length;
	uint32		slength,copyNum;
	const DS_T	*addr;
	
	if ((cgDataBuffer == nullptr) || (num == 0))
		return 0;
	
	copyNum = 0;
	slength = CalcInCapacityAbsolute(length,offset);
	
	if (length > 0){
		addr = cgDataBuffer + offset;
		copyNum = Copy(dataOut,num,addr,length);
		if ((num > 0) && (slength > 0)){
			addr = cgDataBuffer;
			copyNum = Copy(dataOut,num,addr,slength);
		}
	}
	return(copyNum);
};
//------------------------------------------------------------------------------------------//
template <typename DS_T> inline uint32 DS_ARRAY_FRAME<DS_T>::Out(uint32 num){
	uint32	i;
	
	i = offsetIn - offsetOut;
	if (num > i)
		num = i;
	
	offsetOut += num;
	return(num);
};
//------------------------------------------------------------------------------------------//
template <typename DS_T> inline uint32 DS_ARRAY_FRAME<DS_T>::In(uint32 num){
	uint32	i;
	
	i = maxSize - offsetIn + offsetOut;
	if (num > i)
		num = i;
	
	if (countPreIn == 0){
		offsetIn += num;
	}
	else{
		offsetPreIn += num;
	}
	return(num);
};
//------------------------------------------------------------------------------------------//
template <typename DS_T> inline uint32 DS_ARRAY_FRAME<DS_T>::Out_RollBack(uint32 num){
	uint32	i;
	
	i = maxSize - offsetIn + offsetOut;
	if (num > i)
		num = i;
	
	offsetOut -= num;
	return(num);
};
//------------------------------------------------------------------------------------------//
template <typename DS_T> inline uint32 DS_ARRAY_FRAME<DS_T>::In_RollBack(uint32 num){
	uint32	i;
	
	i = offsetIn - offsetOut;
	if (num > i)
		num = i;
	
	offsetIn -= num;
	return(num);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
inline	uint32	ARRAY::Put		(const	uint8& data)									{return(DS_ARRAY_FRAME<uint8>::Put(data));};
inline	uint32	ARRAY::Put		(const	uint8* data ,uint32 num)						{return(DS_ARRAY_FRAME<uint8>::Put(data,num));};
inline	uint32	ARRAY::Put		(const	char*  data)									{return(Put((uint8*)data,(uint32)strlen(data)));};
inline	uint32	ARRAY::Get		(		uint8* dataOut,uint32 num)						{return(DS_ARRAY_FRAME<uint8>::Get(dataOut,num));};
inline	uint32	ARRAY::Read		(		uint8* dataOut,uint32 num,uint32 offset)const	{return(DS_ARRAY_FRAME<uint8>::Read(dataOut,num,offset));};

inline	uint32	ARRAY::Put		(const	ARRAY& _in,uint32 num,uint32 offset)			{return(DS_ARRAY_FRAME<uint8>::Put(_in,num,offset));};
inline	uint32	ARRAY::Get		(		ARRAY* _out,uint32 num)							{return(DS_ARRAY_FRAME<uint8>::Get(_out,num));};
inline	uint32	ARRAY::Read		(		ARRAY* _out,uint32 num,uint32 offset)const		{return(DS_ARRAY_FRAME<uint8>::Read(_out,num,offset));};

inline	uint32	ARRAY::PutInHEX	(const STDSTR &strIn)									{return(PutInHEX((uint8*)strIn.c_str(),(uint32)strIn.length()));};
//------------------------------------------------------------------------------------------//
#endif /* DS_ARRAY_h */
#endif /* DS_ARRAY_hpp */
