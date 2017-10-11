/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: Comm_FIFO.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2012.12.22
 * @2013.6.26 :	change to Liunx style
 add init method
 use int32 to replace bool
 use int32 to replace uint32
 * @2014.6.01 : change to no lock verion
 */
//------------------------------------------------------------------------------------------//
#include "BasicDef.h"
//------------------------------------------------------------------------------------------//
#ifndef Comm_FIFO_DataH
#define Comm_FIFO_DataH
namespace {
	template <typename Type_P>struct _Data		{Type_P p;uint32 num;uint32 offset;};
	template <typename Type_P>struct _DataSome	{Type_P p;uint32 num;uint32 offset;};
	
	template <typename Type_P>static inline _Data	 <Type_P>	Data	(Type_P p,uint32 num = 0,uint32 offset = 0){return{p,num,offset};};
	template <typename Type_P>static inline _DataSome<Type_P>	DataSome(Type_P p,uint32 num = 0,uint32 offset = 0){return{p,num,offset};};
};
#endif
//------------------------------------------------------------------------------------------//
#ifndef Comm_FIFOH
#define Comm_FIFOH
#ifdef Comm_FIFOH
//------------------------------------------------------------------------------------------//
#include <sstream>
//------------------------------------------------------------------------------------------//
template <typename TYPE_QUEUE_FIFOBUFFER>
class	QUEUE_FIFO{
	public:
				 QUEUE_FIFO(uint32 bufferSize);
				 QUEUE_FIFO(void);
				 QUEUE_FIFO(const QUEUE_FIFO& C){;};
		virtual ~QUEUE_FIFO(void);
	private:
		virtual	uint32	Copy(TYPE_QUEUE_FIFOBUFFER *&to_addr,uint32 &to_length,const TYPE_QUEUE_FIFOBUFFER *&in_addr,uint32 &in_length)const;
	protected:
		TYPE_QUEUE_FIFOBUFFER	*dataAreaPointer;
		uint32		maxSize;
		uint32		offsetIn;
		uint32		offsetOut;
		uint32		offsetInPre;
		uint32		inPreCount;
	public:
		uint32		Init		(uint32 bufferSize);
		uint32		Resize		(uint32 bufferSize);
	
		inline	void	Empty	(void){offsetOut = offsetIn;};
	
		inline			int32	IsEmpty		(void)const{return((inPreCount == 0)?(offsetIn == offsetOut):(offsetInPre == offsetOut));};							//return 1 if empty.
		inline			int32	IsFull		(void)const{return((inPreCount == 0)?(maxSize == (offsetIn - offsetOut)):(maxSize == (offsetInPre - offsetOut)));};	//return 1 if full.
		inline			uint32	Unused		(void)const{return((inPreCount == 0)?(maxSize - offsetIn + offsetOut):(maxSize - offsetInPre + offsetOut));};
		inline			uint32	Used		(void)const{return((inPreCount == 0)?(offsetIn - offsetOut):(offsetInPre - offsetOut));};
		inline	const	uint32&	MaxSize		(void)const{return(maxSize);};
	public:
		uint32	CalcInAbsoluteCapacity		(uint32 &fLength,uint32 &offset)const;
	public:
				uint32	CalcOutLength		(uint32 &fLength,uint32 &offset)const;
				uint32	CalcInLength		(uint32 &fLength,uint32 &offset)const;
				uint32	CalcOutCapacity		(uint32 &fLength,uint32 &offset)const;
				uint32	CalcInCapacity		(uint32 &fLength,uint32 &offset)const;
	
		inline	TYPE_QUEUE_FIFOBUFFER*	GetPointer	(uint32 offset = 0)	const{return(dataAreaPointer + (offset & (maxSize - 1)));};
	public:
				uint32	Put		(const	TYPE_QUEUE_FIFOBUFFER *data ,uint32 num);
		inline	uint32	Put		(const	TYPE_QUEUE_FIFOBUFFER &data)				{return(Put(&data,1));};
				uint32	Get		(		TYPE_QUEUE_FIFOBUFFER *dataOut,uint32 num);
				uint32	Read	(		TYPE_QUEUE_FIFOBUFFER *dataOut,uint32 num,uint32 offset = 0)const;
	
				uint32	UpdateOffsetByOut	(const	TYPE_QUEUE_FIFOBUFFER *data ,uint32 num,uint32 offset = 0);
				uint32	UpdateOffsetByIn	(const	TYPE_QUEUE_FIFOBUFFER *data ,uint32 num,uint32 offset = 0);
				uint32	ReadOffsetByIn		(		TYPE_QUEUE_FIFOBUFFER *data ,uint32 num,uint32 offset = 0);
				void	Prepare_Set			(void);
				void	Prepare_Clr			(void);
		inline	uint32	GetPreInNum			(void)const{return(offsetInPre - offsetIn);};

				uint32	Put		(const	QUEUE_FIFO<TYPE_QUEUE_FIFOBUFFER> &fifoIn ,uint32 num,uint32 fifoInOffset = 0);
		inline	uint32	Get		(		QUEUE_FIFO<TYPE_QUEUE_FIFOBUFFER> *fifoOut,uint32 num)					{return(Out(fifoOut->Put(*this,num)));};
		inline	uint32	Read	(QUEUE_FIFO<TYPE_QUEUE_FIFOBUFFER> *fifoOut,uint32 num,uint32 offset = 0)const	{return(fifoOut->Put(*this,num,offset));};
	
				uint32	Out		(uint32 num);
				uint32	In		(uint32 num);
				uint32	Out_RollBack	(uint32 num);
				uint32	In_RollBack		(uint32 num);
	public:
		inline QUEUE_FIFO<TYPE_QUEUE_FIFOBUFFER>& operator << (const _Data <const TYPE_QUEUE_FIFOBUFFER*> &data){Put(data.p,data.num);return(*this);};
		inline QUEUE_FIFO<TYPE_QUEUE_FIFOBUFFER>& operator >> (const _Data <const TYPE_QUEUE_FIFOBUFFER*> &data){Get(data.p,data.num);return(*this);};
		inline uint32 operator << (const _DataSome <const  TYPE_QUEUE_FIFOBUFFER*> &data){return(Put(data.p,data.num));};
		inline uint32 operator >> (const _DataSome <const  TYPE_QUEUE_FIFOBUFFER*> &data){return(Get(data.p,data.num));};
	
		inline QUEUE_FIFO<TYPE_QUEUE_FIFOBUFFER>& operator << (const _Data <const QUEUE_FIFO<TYPE_QUEUE_FIFOBUFFER>*> &data){Put(*data.p,data.num,data.offset);return(*this);};
		inline QUEUE_FIFO<TYPE_QUEUE_FIFOBUFFER>& operator >> (const _Data <const QUEUE_FIFO<TYPE_QUEUE_FIFOBUFFER>*> &data){Get(data.p,data.num);return(*this);};
		inline uint32 operator << (const _DataSome <const QUEUE_FIFO<TYPE_QUEUE_FIFOBUFFER>*> &data){return(Put(*data.p,data.num,data.offset));};
		inline uint32 operator >> (const _DataSome <const QUEUE_FIFO<TYPE_QUEUE_FIFOBUFFER>*> &data){return(Get(data.p,data.num));};
};
//------------------------------------------------------------------------------------------//
template <typename TYPE_QUEUE_FIFOBUFFER>
QUEUE_FIFO<TYPE_QUEUE_FIFOBUFFER>::QUEUE_FIFO(void){
	offsetInPre = 0;
	offsetIn = 0;
	offsetOut = 0;
	maxSize = 0;
	dataAreaPointer = nullptr;
	inPreCount = 0;
}
//------------------------------------------------------------------------------------------//
template <typename TYPE_QUEUE_FIFOBUFFER>
QUEUE_FIFO<TYPE_QUEUE_FIFOBUFFER>::QUEUE_FIFO(uint32 bufferSize){
	QUEUE_FIFO();
	Init(bufferSize);
}
//------------------------------------------------------------------------------------------//
template <typename TYPE_QUEUE_FIFOBUFFER>
QUEUE_FIFO<TYPE_QUEUE_FIFOBUFFER>::~QUEUE_FIFO(void){
	if (dataAreaPointer != nullptr){
		try{
			delete []dataAreaPointer;
		}
		catch(...){}
	}
	dataAreaPointer = nullptr;
}
//------------------------------------------------------------------------------------------//
template <typename TYPE_QUEUE_FIFOBUFFER>
uint32 QUEUE_FIFO<TYPE_QUEUE_FIFOBUFFER>::Init(uint32 bufferSize){
	int32		i;
	uint32		tSize;
	TYPE_QUEUE_FIFOBUFFER	*tempBufP;
	
	tempBufP = nullptr;
	if (bufferSize > maxSize){
		tSize = (uint32)0x01 << 5;//the smallest is 32byte
		
		i = 5;
		while (i++ < 32){
			if (bufferSize <= tSize){
				bufferSize = tSize;
				break;
			}
			tSize = tSize << 1;
		}
		if (bufferSize > 1024 * 1024 * 1024)	//the biggest is 1G
			bufferSize = 1024 * 1024 * 1024;	//2^10 * 2^10 * 2^10
		try{
			tempBufP = new TYPE_QUEUE_FIFOBUFFER[bufferSize];
		}
		catch(...){
			tempBufP = nullptr;
		}
		if (tempBufP != nullptr){
			if (dataAreaPointer != nullptr){
				if (inPreCount > 0){
					tSize = Get(tempBufP,offsetInPre - offsetOut);//copy to new buffer.
				}
				else{
					tSize = Get(tempBufP,offsetIn - offsetOut);//copy to new buffer.
				}
				offsetInPre = tSize + offsetInPre - offsetIn;
				offsetIn = tSize;
				offsetOut = 0;
				try{
					delete []dataAreaPointer;
				}
				catch(...){}
			}
			dataAreaPointer = tempBufP;
			maxSize = bufferSize;
		}
	}
	return(maxSize);
}
//------------------------------------------------------------------------------------------//
template <typename TYPE_QUEUE_FIFOBUFFER>
void QUEUE_FIFO<TYPE_QUEUE_FIFOBUFFER>::Prepare_Set(void){
	if (inPreCount < 0)
		inPreCount = 0;
	if (inPreCount == 0)
		offsetInPre = offsetIn;
	inPreCount ++;
}
//------------------------------------------------------------------------------------------//
template <typename TYPE_QUEUE_FIFOBUFFER>
void QUEUE_FIFO<TYPE_QUEUE_FIFOBUFFER>::Prepare_Clr(void){
	inPreCount --;
	if (inPreCount < 0)
		inPreCount = 0;
	if (inPreCount == 0)
		offsetIn = offsetInPre;
}
//------------------------------------------------------------------------------------------//
template <typename TYPE_QUEUE_FIFOBUFFER>
uint32 QUEUE_FIFO<TYPE_QUEUE_FIFOBUFFER>::CalcOutLength(uint32 &fLength,uint32 &offset)const{
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
template <typename TYPE_QUEUE_FIFOBUFFER>
uint32 QUEUE_FIFO<TYPE_QUEUE_FIFOBUFFER>::CalcInLength(uint32 &fLength,uint32 &offset)const{
	uint32	unUsedSize,maxIn;
	uint32	offsetInT;
	if (inPreCount == 0){
		offsetInT = offsetIn;
	}
	else{
		offsetInT = offsetInPre;
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
template <typename TYPE_QUEUE_FIFOBUFFER>
uint32 QUEUE_FIFO<TYPE_QUEUE_FIFOBUFFER>::CalcOutCapacity(uint32 &fLength,uint32 &offset)const{
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
template <typename TYPE_QUEUE_FIFOBUFFER>
uint32 QUEUE_FIFO<TYPE_QUEUE_FIFOBUFFER>::CalcInCapacity(uint32 &fLength,uint32 &offset)const{
	uint32	unUsedSize;
	uint32	offsetInT;
	if (inPreCount == 0){
		offsetInT = offsetIn;
	}
	else{
		offsetInT = offsetInPre;
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
template <typename TYPE_QUEUE_FIFOBUFFER>
uint32 QUEUE_FIFO<TYPE_QUEUE_FIFOBUFFER>::CalcInAbsoluteCapacity(uint32 &fLength,uint32 &offset)const{
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
template <typename TYPE_QUEUE_FIFOBUFFER>
uint32 QUEUE_FIFO<TYPE_QUEUE_FIFOBUFFER>::Copy(TYPE_QUEUE_FIFOBUFFER *&to_addr,uint32 &to_length,const TYPE_QUEUE_FIFOBUFFER *&in_addr,uint32 &in_length)const{
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
}
//------------------------------------------------------------------------------------------//
template <typename TYPE_QUEUE_FIFOBUFFER>
uint32 QUEUE_FIFO<TYPE_QUEUE_FIFOBUFFER>::Put(const TYPE_QUEUE_FIFOBUFFER *data,uint32 num){
	uint32	length,offset;
	uint32	slength,copyNum;
	TYPE_QUEUE_FIFOBUFFER	*addr;
	
	if ((dataAreaPointer == nullptr) || (num == 0))
		return 0;

	copyNum = 0;
	offset = 0;
	slength = CalcInCapacity(length,offset);

	if (length > 0){
		addr = dataAreaPointer + offset;
		copyNum += Copy(addr,length,data,num);
		if ((num > 0) && (slength > 0)){
			addr = dataAreaPointer;
			copyNum += Copy(addr,slength,data,num);
		}
		if (inPreCount == 0){
			offsetIn += copyNum;
		}
		else{
			offsetInPre += copyNum;
		}
	}
	return(copyNum);
}
//------------------------------------------------------------------------------------------//
template <typename TYPE_QUEUE_FIFOBUFFER>
uint32 QUEUE_FIFO<TYPE_QUEUE_FIFOBUFFER>::Read(TYPE_QUEUE_FIFOBUFFER *dataOut,uint32 num,uint32 offset)const{
	uint32	length;
	uint32	slength,copyNum;
	const TYPE_QUEUE_FIFOBUFFER	*addr;
	
	if ((dataAreaPointer == nullptr) || (num == 0))
		return 0;
	
	copyNum = 0;
	slength = CalcOutCapacity(length,offset);
	
	if (length > 0){
		addr = dataAreaPointer + offset;
		copyNum = Copy(dataOut,num,addr,length);
		if ((num > 0) && (slength > 0)){
			addr = dataAreaPointer;
			copyNum = Copy(dataOut,num,addr,slength);
		}
	}
	return(copyNum);
}
//------------------------------------------------------------------------------------------//
template <typename TYPE_QUEUE_FIFOBUFFER>
uint32 QUEUE_FIFO<TYPE_QUEUE_FIFOBUFFER>::Get(TYPE_QUEUE_FIFOBUFFER *dataOut,uint32 num){
	uint32	copyNum;
	copyNum = Read(dataOut,num,0);
	offsetOut += copyNum;
	return(copyNum);
}
//------------------------------------------------------------------------------------------//
template <typename TYPE_QUEUE_FIFOBUFFER>
uint32 QUEUE_FIFO<TYPE_QUEUE_FIFOBUFFER>::Put(const QUEUE_FIFO<TYPE_QUEUE_FIFOBUFFER> &fifoIn,uint32 num,uint32 fifoInOffset){
	uint32	copyNum;
	uint32	in_1,in_Offset,in_2;
	uint32	&out_1 = num,&out_Offset = fifoInOffset,out_2;
	TYPE_QUEUE_FIFOBUFFER	*in_addr;
	const TYPE_QUEUE_FIFOBUFFER	*out_addr;
	const QUEUE_FIFO<TYPE_QUEUE_FIFOBUFFER> &out_fifo = fifoIn;
	
	if (dataAreaPointer == nullptr)
		return 0;
	copyNum = 0;

	in_Offset = 0;
	in_2 = CalcInCapacity(in_1,in_Offset);
	out_2 = out_fifo.CalcOutLength(out_1,out_Offset);

	if ((in_1 > 0) && (out_1 > 0)){
		in_addr = dataAreaPointer + in_Offset;
		out_addr = out_fifo.GetPointer(out_Offset);
		copyNum = Copy(in_addr,in_1,out_addr,out_1);
		if ((in_1 > 0) && (out_2 > 0)){
			out_addr = out_fifo.GetPointer(0);
			copyNum += Copy(in_addr,in_1,out_addr,out_2);
			if ((in_2 > 0) && (out_2 > 0)){
				in_addr = dataAreaPointer;
				copyNum += Copy(in_addr,in_2,out_addr,out_2);
			}
		}
		else if ((in_2 > 0) && (out_1 > 0)){
			in_addr = dataAreaPointer;
			copyNum += Copy(in_addr,in_2,out_addr,out_1);
			if ((in_2 > 0) && (out_2 > 0)){
				out_addr = out_fifo.GetPointer(0);
				copyNum += Copy(in_addr,in_2,out_addr,out_2);
			}
		}
		if (inPreCount == 0){
			offsetIn += copyNum;
		}
		else{
			offsetInPre += copyNum;
		}
	}
	return(copyNum);
}
//------------------------------------------------------------------------------------------//
template <typename TYPE_QUEUE_FIFOBUFFER>
uint32 QUEUE_FIFO<TYPE_QUEUE_FIFOBUFFER>::UpdateOffsetByOut(const TYPE_QUEUE_FIFOBUFFER *bufferIn,uint32 num,uint32 offset){
	uint32	length;
	uint32	slength,copyNum;
	TYPE_QUEUE_FIFOBUFFER	*addr;
	
	if ((dataAreaPointer == nullptr) || (num == 0))
		return 0;
	
	copyNum = 0;
	slength = CalcOutCapacity(length,offset);
	
	if (length > 0){
		addr = dataAreaPointer + offset;
		copyNum = Copy(addr,length,bufferIn,num);
		if ((num > 0) && (slength > 0)){
			addr = dataAreaPointer;
			copyNum = Copy(addr,slength,bufferIn,num);
		}
	}
	return(copyNum);
}
//------------------------------------------------------------------------------------------//
template <typename TYPE_QUEUE_FIFOBUFFER>
uint32 QUEUE_FIFO<TYPE_QUEUE_FIFOBUFFER>::UpdateOffsetByIn(const TYPE_QUEUE_FIFOBUFFER *bufferIn,uint32 num,uint32 offset){
	uint32	length;
	uint32	slength,copyNum;
	TYPE_QUEUE_FIFOBUFFER	*addr;
	
	if ((dataAreaPointer == nullptr) || (num == 0))
		return 0;
	
	copyNum = 0;
	slength = CalcInAbsoluteCapacity(length,offset);
	
	if (length > 0){
		addr = dataAreaPointer + offset;
		copyNum = Copy(addr,length,bufferIn,num);
		if ((num > 0) && (slength > 0)){
			addr = dataAreaPointer;
			copyNum = Copy(addr,slength,bufferIn,num);
		}
	}
	return(copyNum);
}
//------------------------------------------------------------------------------------------//
template <typename TYPE_QUEUE_FIFOBUFFER>
uint32 QUEUE_FIFO<TYPE_QUEUE_FIFOBUFFER>::ReadOffsetByIn(TYPE_QUEUE_FIFOBUFFER *dataOut,uint32 num,uint32 offset){
	uint32	length;
	uint32	slength,copyNum;
	const TYPE_QUEUE_FIFOBUFFER	*addr;
	
	if ((dataAreaPointer == nullptr) || (num == 0))
		return 0;
	
	copyNum = 0;
	slength = CalcInAbsoluteCapacity(length,offset);
	
	if (length > 0){
		addr = dataAreaPointer + offset;
		copyNum = Copy(dataOut,num,addr,length);
		if ((num > 0) && (slength > 0)){
			addr = dataAreaPointer;
			copyNum = Copy(dataOut,num,addr,slength);
		}
	}
	return(copyNum);
}
//------------------------------------------------------------------------------------------//
template <typename TYPE_QUEUE_FIFOBUFFER>
inline uint32 QUEUE_FIFO<TYPE_QUEUE_FIFOBUFFER>::Out_RollBack(uint32 num){
	uint32	i;
	
	i = maxSize - offsetIn + offsetOut;
	if (num > i)
		num = i;
	
	offsetOut -= num;
	return(num);
}
//------------------------------------------------------------------------------------------//
template <typename TYPE_QUEUE_FIFOBUFFER>
 inline uint32 QUEUE_FIFO<TYPE_QUEUE_FIFOBUFFER>::In_RollBack(uint32 num){
	uint32	i;
	
	i = offsetIn - offsetOut;
	if (num > i)
	num = i;
	
	offsetIn -= num;
	return(num);
 }
//------------------------------------------------------------------------------------------//
template <typename TYPE_QUEUE_FIFOBUFFER>
inline uint32 QUEUE_FIFO<TYPE_QUEUE_FIFOBUFFER>::Out(uint32 num){
	uint32	i;
	
	i = offsetIn - offsetOut;
	if (num > i)
		num = i;
	
	offsetOut += num;
	return(num);
}
//------------------------------------------------------------------------------------------//
template <typename TYPE_QUEUE_FIFOBUFFER>
inline uint32 QUEUE_FIFO<TYPE_QUEUE_FIFOBUFFER>::In(uint32 num){
	uint32	i;
	
	i = maxSize - offsetIn + offsetOut;
	if (num > i)
		num = i;
	
	if (inPreCount == 0){
		offsetIn += num;
	}
	else{
		offsetInPre += num;
	}
	return(num);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------/
class FIFO8 : public QUEUE_FIFO<uint8>{
	public:
				 FIFO8(int32 bufferSize) : QUEUE_FIFO<uint8>(bufferSize){;};
				 FIFO8(void) : QUEUE_FIFO<uint8>(){;};
		virtual ~FIFO8(void){;};
	public:
		static	uint32	Escape_xhh		(uint8 *&retChar,const uint8 *&data,uint32 &num);
		static	uint32	Escape_xhh		(uint8 *&retChar,const uint8 *&data,uint64 &num);
		static	uint32	EscapeToChar	(uint8 *retChar,const uint8 *&data,uint32 &num);
		static	uint32	EscapeToChar	(uint8 *retChar,const uint8 *&data,uint64 &num);
		static	void	CharToHex		(STDSTR *retStr,uint8 data);
		static	void	UnEscapeToStr	(STDSTR *retStr,uint8 data);
	protected:
		inline	uint32	Copy(uint8 *&to_addr,uint32 &to_length,const uint8 *&in_addr,uint32 &in_length)const{
			uint32	copyNum;
			copyNum = (to_length <= in_length) ? to_length : in_length;
			memcpy(to_addr,in_addr,copyNum);
			to_addr += copyNum;
			in_addr += copyNum;
			to_length -= copyNum;
			in_length -= copyNum;
			return(copyNum);
		};
	public:
		inline	uint32	Put		(const	uint8 &data)			{return(QUEUE_FIFO<uint8>::Put(data));};
		inline	uint32	Put		(const	uint8 *data ,uint32 num){return(QUEUE_FIFO<uint8>::Put(data,num));};
		inline	uint32	Put		(const	FIFO8 &fifoIn ,uint32 num,uint32 fifoInOffset = 0)	{return(QUEUE_FIFO<uint8>::Put(fifoIn,num,fifoInOffset));};
	
		inline	uint32	Get		(uint8 *dataOut,uint32 num)		{return(QUEUE_FIFO<uint8>::Get(dataOut,num));};
		inline	uint32	Get		(FIFO8 *fifoOut,uint32 num)		{return(QUEUE_FIFO<uint8>::Get(fifoOut,num));};
	
		inline	uint32	Read	(uint8 *dataOut,uint32 num,uint32 offset = 0)const{return(QUEUE_FIFO<uint8>::Read(dataOut,num,offset));};
		inline	uint32	Read	(FIFO8 *fifoOut,uint32 num,uint32 offset = 0)const{return(QUEUE_FIFO<uint8>::Read(fifoOut,num,offset));};
	
		inline	uint32	Put		(const	char *data){return(Put((uint8*)data,(uint32)strlen(data)));};
	public:
		void	FillZero	(void);
	
		uint32	PutInEscape	(const uint8 *data,uint32 num);
		uint32	ReadInEscape(uint8 *dataOut,uint32 num,uint32 offset)const;
	
		uint32	PutInHEX	(const uint8 *data,uint32 num);//put STDSTR to FIFO,input in HEX mode.
	
		uint32	Put			(const STDSTR &strIn,G_ESCAPE blEscape);
		uint32	PutInHEX	(const STDSTR &strIn){return(PutInHEX((uint8*)strIn.c_str(),(uint32)strIn.length()));};

		const STDSTR&	Get			(STDSTR *retStr,uint32 num);
		const STDSTR&	Read		(STDSTR *retStr,uint32 num,uint32 offset = 0)const;
		const STDSTR&	GetUnEscape	(STDSTR *retStr,uint32 num);
		const STDSTR&	ReadUnEscape(STDSTR *retStr,uint32 num,uint32 offset = 0)const;
	
		const STDSTR&	GetInHEX	(STDSTR *retStr,uint32 num);//output as STDSTR,HEX mode without space.
		const STDSTR&	ReadInHEX	(STDSTR *retStr,uint32 num,uint32 offset = 0)const;//output as STDSTR,HEX mode with space.
		const STDSTR&	GetInHEX_S	(STDSTR *retStr,uint32 num);//output as STDSTR,HEX mode without space.
		const STDSTR&	ReadInHEX_S	(STDSTR *retStr,uint32 num,uint32 offset = 0)const;//output as STDSTR,HEX mode with space.
	
		const STDSTR&	GetInLine	(STDSTR *retStr,uint8 endChar = '\n');//output as STDSTR,original data.
		const STDSTR&	ReadInLine	(STDSTR *retStr,uint8 endChar = '\n',uint32 offset = 0)const;//output as STDSTR,original data.
	
		uint32	Put		(std::stringstream &streamIn);
		uint32	Get		(std::stringstream *streamOut);
	public:
		inline FIFO8&	operator << (const _Data	<const	uint8*> &data){Put(data.p,data.num);return(*this);};
		inline FIFO8&	operator >> (const _Data	<		uint8*> &data){Get(data.p,data.num);return(*this);};
		inline uint32	operator << (const _DataSome<const	uint8*> &data){return(Put(data.p,data.num));};
		inline uint32	operator >> (const _DataSome<		uint8*> &data){return(Get(data.p,data.num));};
	
	 	inline FIFO8&	operator << (const _Data	<const	char*> &data){Put((uint8*)data.p,data.num);return(*this);};
		inline FIFO8&	operator >> (const _Data	<		char*> &data){Get((uint8*)data.p,data.num);return(*this);};
		inline uint32	operator << (const _DataSome<const	char*> &data){return(Put((uint8*)data.p,data.num));};
		inline uint32	operator >> (const _DataSome<		char*> &data){return(Get((uint8*)data.p,data.num));};
	
		inline FIFO8&	operator << (const _Data	<const	FIFO8*> &data){Put(*data.p,data.num,data.offset);	return(*this);};
		inline FIFO8&	operator >> (const _Data	<		FIFO8*> &data){Get(data.p,data.num);				return(*this);};
		inline uint32	operator << (const _DataSome<const	FIFO8*> &data){return(Put(*data.p,data.num,data.offset));};
		inline uint32	operator >> (const _DataSome<		FIFO8*> &data){return(Get(data.p,data.num));};
	
		inline FIFO8&	operator << (std::stringstream &streamIn)	{Put(streamIn);				return(*this);};
		inline FIFO8&	operator >> (std::stringstream *streamOut)	{Get(streamOut);			return(*this);};
		inline FIFO8&	operator << (const STDSTR &strIn)			{Put(strIn,G_ESCAPE_OFF);	return(*this);};
		inline FIFO8&	operator >> (STDSTR *strOutput)				{Get(strOutput,-1);			return(*this);};
	
		inline FIFO8&	operator << (char *data)					{Put(data);					return(*this);};
};
//------------------------------------------------------------------------------------------/
#endif
#endif
