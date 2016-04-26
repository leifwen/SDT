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
#ifndef Comm_FIFOH
#define Comm_FIFOH
//------------------------------------------------------------------------------------------//
#include "BasicDef.h"
//------------------------------------------------------------------------------------------//
template <typename TYPE_QUEUE_FIFOBUFFER>
class	QUEUE_FIFO{
	public:
				 QUEUE_FIFO(uint32 bufferSize);
				 QUEUE_FIFO(void);
		virtual ~QUEUE_FIFO(void);
	private:
		uint32		Copy(TYPE_QUEUE_FIFOBUFFER *&in_addr,const TYPE_QUEUE_FIFOBUFFER *&out_addr,uint32 &in_length,uint32 &out_length)const;
	protected:
		TYPE_QUEUE_FIFOBUFFER	*dataAreaPointer;
		uint32		maxSize;
		uint32		offsetIn;
		uint32		offsetOut;
	public:
		uint32		Init		(uint32 bufferSize);
		uint32		Resize		(uint32 bufferSize);
	
		inline	void	Empty	(void){offsetOut = offsetIn;};
	
		inline			int32	IsEmpty		(void)const{return(offsetIn == offsetOut);};				//return 1 if empty.
		inline			int32	IsFull		(void)const{return(maxSize == (offsetIn - offsetOut));};	//return 1 if full.
		inline			uint32	Unused		(void)const{return(maxSize - offsetIn + offsetOut);};
		inline			uint32	Used		(void)const{return(offsetIn - offsetOut);};
		inline	const	uint32&	MaxSize		(void)const{return(maxSize);};
	public:
				uint32	CalcOutLength		(uint32 &fLength,uint32 &offset)const;
				uint32	CalcInLength		(uint32 &fLength,uint32 &offset)const;
				uint32	CalcOutCapacity		(uint32 &fLength,uint32 &offset)const;
				uint32	CalcInCapacity		(uint32 &fLength,uint32 &offset)const;
		inline	TYPE_QUEUE_FIFOBUFFER*	GetOutPointer	(uint32 offset = 0)	const{return(dataAreaPointer + (offset & (maxSize - 1)));};
	public:
				uint32	Put		(const	TYPE_QUEUE_FIFOBUFFER *data ,uint32 num);
				uint32	Get		(		TYPE_QUEUE_FIFOBUFFER *bufferOut,uint32 num);
				uint32	Update	(const	TYPE_QUEUE_FIFOBUFFER *data ,uint32 num,uint32 offset = 0);
				uint32	Read	(		TYPE_QUEUE_FIFOBUFFER *bufferOut,uint32 num,uint32 offset = 0)const;

				uint32	Put		(const	QUEUE_FIFO<TYPE_QUEUE_FIFOBUFFER> &fifoIn ,uint32 num,uint32 fifoInOffset = 0);
		inline	uint32	Get		(		QUEUE_FIFO<TYPE_QUEUE_FIFOBUFFER> *fifoOut,uint32 num){return(Out(fifoOut->Put(*this,num)));};
		inline	uint32	Read	(QUEUE_FIFO<TYPE_QUEUE_FIFOBUFFER> *fifoOut,uint32 num,uint32 offset = 0)const{return(fifoOut->Put(*this,num,offset));};
	
				uint32	Out		(uint32 num);
				uint32	In		(uint32 num);
				uint32	Out_RollBack	(uint32 num);
				uint32	In_RollBack		(uint32 num);	
};
//------------------------------------------------------------------------------------------//
template <typename TYPE_QUEUE_FIFOBUFFER>
QUEUE_FIFO<TYPE_QUEUE_FIFOBUFFER>::QUEUE_FIFO(void){
	offsetIn = 0;
	offsetOut = 0;
	maxSize = 0;
	dataAreaPointer = nullptr;
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
				tSize = Get(tempBufP,offsetIn - offsetOut);//copy to new buffer.
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
uint32 QUEUE_FIFO<TYPE_QUEUE_FIFOBUFFER>::Resize(uint32 bufferSize){
	int32		i;
	uint32		tSize;
	TYPE_QUEUE_FIFOBUFFER	*tempBufP;
	
	tempBufP = nullptr;
	
	tSize = (uint32)0x01 << 5;//the smallest is 32byte
	
	i = 5;
	while (i++ < 32){
		if (bufferSize <= tSize){
			bufferSize = tSize;
			break;
		}
		tSize = tSize << 1;
	}
	if (bufferSize > 1024 * 1024 * 1024)	//the biggest is 4M
		bufferSize = 1024 * 1024 * 1024;
	try{
		tempBufP = new TYPE_QUEUE_FIFOBUFFER[bufferSize];
	}
	catch(...){
		tempBufP = nullptr;
	}
	if (tempBufP != nullptr){
		if (dataAreaPointer != nullptr){
			offsetIn = 0;
			offsetOut = 0;
			try{
				delete []dataAreaPointer;
			}
			catch(...){}
		}
		dataAreaPointer = tempBufP;
		maxSize = bufferSize;
	}
	return(maxSize);
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
	
	unUsedSize = maxSize - offsetIn + offsetOut;
	
	if (offset >= unUsedSize){
		offset = 0;
		fLength = 0;
		return 0;
	}
	unUsedSize -= offset;
	maxIn = (unUsedSize < fLength) ? unUsedSize : fLength;
	offset = ((offsetIn + offset)& (maxSize - 1));
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
uint32 QUEUE_FIFO<TYPE_QUEUE_FIFOBUFFER>::Copy(TYPE_QUEUE_FIFOBUFFER *&in_addr,const TYPE_QUEUE_FIFOBUFFER *&out_addr,uint32 &in_length,uint32 &out_length)const{
	uint32	i,copyNum;
	i = (in_length <= out_length) ? in_length : out_length;
	copyNum = i;
	while(i-- > 0){
		*in_addr = *out_addr;
		++ in_addr;
		++ out_addr;
	}
	in_length -= copyNum;
	out_length -= copyNum;
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
		copyNum += Copy(addr,data,length,num);
		if ((num > 0) && (slength > 0)){
			addr = dataAreaPointer;
			copyNum += Copy(addr,data,slength,num);
		}
		offsetIn += copyNum;
	}
	return(copyNum);
}
//------------------------------------------------------------------------------------------//
template <typename TYPE_QUEUE_FIFOBUFFER>
uint32 QUEUE_FIFO<TYPE_QUEUE_FIFOBUFFER>::Get(TYPE_QUEUE_FIFOBUFFER *bufferOut,uint32 num){
	uint32	length,offset;
	uint32	slength,copyNum;
	const TYPE_QUEUE_FIFOBUFFER	*addr;
	
	if ((dataAreaPointer == nullptr) || (num == 0))
		return 0;
	
	copyNum = 0;
	offset = 0;
	slength = CalcOutCapacity(length,offset);

	if (length > 0){
		addr = dataAreaPointer + offset;
		copyNum = Copy(bufferOut,addr,num,length);
		if ((num > 0) && (slength > 0)){
			addr = dataAreaPointer;
			copyNum = Copy(bufferOut,addr,num,slength);
		}
		offsetOut += copyNum;
	}
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
		out_addr = out_fifo.GetOutPointer(out_Offset);
		copyNum = Copy(in_addr,out_addr,in_1,out_1);
		if ((in_1 > 0) && (out_2 > 0)){
			out_addr = out_fifo.GetOutPointer(0);
			copyNum += Copy(in_addr,out_addr,in_1,out_2);
			if ((in_2 > 0) && (out_2 > 0)){
				in_addr = dataAreaPointer;
				copyNum += Copy(in_addr,out_addr,in_2,out_2);
			}
		}
		else if ((in_2 > 0) && (out_1 > 0)){
			in_addr = dataAreaPointer;
			copyNum += Copy(in_addr,out_addr,in_2,out_1);
			if ((in_2 > 0) && (out_2 > 0)){
				out_addr = out_fifo.GetOutPointer(0);
				copyNum += Copy(in_addr,out_addr,in_2,out_2);
			}
		}
		offsetIn += copyNum;
	}
	return(copyNum);
}
//------------------------------------------------------------------------------------------//
template <typename TYPE_QUEUE_FIFOBUFFER>
uint32 QUEUE_FIFO<TYPE_QUEUE_FIFOBUFFER>::Update(const TYPE_QUEUE_FIFOBUFFER *bufferIn,uint32 num,uint32 offset){
	uint32	length;
	uint32	slength,copyNum;
	TYPE_QUEUE_FIFOBUFFER	*addr;
	
	if ((dataAreaPointer == nullptr) || (num == 0))
		return 0;
	
	copyNum = 0;
	slength = CalcOutCapacity(length,offset);
	
	if (length > 0){
		addr = dataAreaPointer + offset;
		copyNum = Copy(addr,bufferIn,length,num);
		if ((num > 0) && (slength > 0)){
			addr = dataAreaPointer;
			copyNum = Copy(addr,bufferIn,slength,num);
		}
	}
	return(copyNum);
}
//------------------------------------------------------------------------------------------//
template <typename TYPE_QUEUE_FIFOBUFFER>
uint32 QUEUE_FIFO<TYPE_QUEUE_FIFOBUFFER>::Read(TYPE_QUEUE_FIFOBUFFER *bufferOut,uint32 num,uint32 offset)const{
	uint32	length;
	uint32	slength,copyNum;
	const TYPE_QUEUE_FIFOBUFFER	*addr;
	
	if ((dataAreaPointer == nullptr) || (num == 0))
		return 0;
	
	copyNum = 0;
	slength = CalcOutCapacity(length,offset);
	
	if (length > 0){
		addr = dataAreaPointer + offset;
		copyNum = Copy(bufferOut,addr,num,length);
		if ((num > 0) && (slength > 0)){
			addr = dataAreaPointer;
			copyNum = Copy(bufferOut,addr,num,slength);
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
	
	offsetIn += num;
	return(num);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------/
class FIFO_UINT8 : public QUEUE_FIFO<uint8>{
	public:
				 FIFO_UINT8(int32 bufferSize) : QUEUE_FIFO<uint8>(bufferSize){;};
				 FIFO_UINT8(void) : QUEUE_FIFO<uint8>(){;};
		virtual ~FIFO_UINT8(void){;};
	public:
		static	std::string		CharToHex			(uint8 data);
		static	uint32			Escape0xhhToChar	(uint8 *retChar,const std::string &strInput);
		static	uint32			EscapeToChar		(uint8 *retChar,const std::string &strInput);
		static	void			EscapeToStr			(std::string *retStr,uint8 dataChar);
	public:
		void	FillZero	(void);
		uint32	PutInEscape	(const std::string &strInput);
		uint32	PutInASCII	(const std::string &strInput,G_ESCAPE_VAILD blEscape);
		uint32	PutInHEX	(const std::string &strInput);								//put std::string to FIFO,input in HEX mode.

		uint32	GetInLine	(std::string *retStr,uint8 endChar = '\n');//output as std::string,original data.
		uint32	ReadInLine	(std::string *retStr,uint8 endChar = '\n',uint32 offset = 0)const;//output as std::string,original data.
		uint32	GetInASCII	(std::string *retStr,uint32 num);
		uint32	ReadInASCII	(std::string *retStr,uint32 num,uint32 offset = 0)const;
		uint32	GetInASCII_E(std::string *retStr,uint32 num);
		uint32	ReadInASCII_E(std::string *retStr,uint32 num,uint32 offset = 0)const;
		uint32	GetInHEX	(std::string *retStr,uint32 num);//output as std::string,HEX mode without space.
		uint32	ReadInHEX	(std::string *retStr,uint32 num,uint32 offset = 0)const;//output as std::string,HEX mode with space.
		uint32	GetInHEX_S	(std::string *retStr,uint32 num);//output as std::string,HEX mode without space.
		uint32	ReadInHEX_S	(std::string *retStr,uint32 num,uint32 offset = 0)const;//output as std::string,HEX mode with space.
};
//------------------------------------------------------------------------------------------/
#endif
