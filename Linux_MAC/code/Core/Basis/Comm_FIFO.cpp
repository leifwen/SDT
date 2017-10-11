/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: Comm_FIFO.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2012.12.22
 * @2013.6.26 :	change to Liunx style
 add init method
 use int32 to replace bool
 use int32 to replace uint32
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "Comm_FIFO.h"
//------------------------------------------------------------------------------------------//
#ifdef Comm_FIFOH
//------------------------------------------------------------------------------------------//
inline uint32 HToD(uint8 *&retChar,const uint8 *&data){
	if (*data >= '0' && *data <= '9'){
		*retChar = *data - '0';
		return 1;
	}
	else if (*data >= 'A' && *data <= 'F'){
		*retChar = *data - 'A' + 0x0a;
		return 1;
	}
	else if (*data >= 'a' && *data <= 'f'){
		*retChar = *data - 'a' + 0x0a;
		return 1;
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
template <typename TYPE_INT>
inline uint32 Escape_xhh(uint8 *&retChar,const uint8 *&data,TYPE_INT &num){
	uint8	ret;
	if (HToD(retChar,data) > 0){
		++ data;
		if (--num > 0){
			ret = *retChar;
			if (HToD(retChar,data) > 0){
				*retChar |= (ret << 4);
				++data;
				--num;
				return 2;
			}
		}
		return 1;
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
uint32 FIFO8::Escape_xhh(uint8 *&retChar,const uint8 *&data,uint32 &num){
	return(::Escape_xhh(retChar,data,num));
}
//------------------------------------------------------------------------------------------//
uint32 FIFO8::Escape_xhh(uint8 *&retChar,const uint8 *&data,uint64 &num){
	return(::Escape_xhh(retChar,data,num));
}
//------------------------------------------------------------------------------------------//
inline uint32 OToD(uint8 *&retChar,const uint8 *&data){
	if (*data >= '0' && *data <= '7'){
		*retChar = *data - '0';
		return 1;
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
template <typename TYPE_INT>
inline uint32 Escape_ooo(uint8 *&retChar,const uint8 *&data,TYPE_INT &num){
	uint8	ret;
	if (OToD(retChar,data) > 0){
		++ data;
		if (--num > 0){
			ret = *retChar;
			if (OToD(retChar,data) > 0){
				*retChar |= (ret << 3);
				++ data;
				if (--num > 0){
					ret = *retChar;
					if (OToD(retChar,data) > 0){
						*retChar |= (ret << 3);
						++ data;
						-- num;
						return 3;
					}
				}
				return 2;
			}
		}
		return 1;
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
template <typename TYPE_INT>
uint32 EscapeToChar(uint8 *retChar,const uint8 *&data,TYPE_INT &num){
	switch (*data) {
		case 'a':	*retChar = '\a';++ data;-- num;return 1;
		case 'b':	*retChar = '\b';++ data;-- num;return 1;
		case 'f':	*retChar = '\f';++ data;-- num;return 1;
		case 'n':	*retChar = '\n';++ data;-- num;return 1;
		case 'r':	*retChar = '\r';++ data;-- num;return 1;
		case 't':	*retChar = '\t';++ data;-- num;return 1;
		case 'v':	*retChar = '\v';++ data;-- num;return 1;
		case '\\':	*retChar = '\\';++ data;-- num;return 1;
		case '\'':	*retChar = '\'';++ data;-- num;return 1;
		case '\"':	*retChar = '\"';++ data;-- num;return 1;
		case '\?':	*retChar = '\?';++ data;-- num;return 1;
		case '\0':	*retChar = '\0';++ data;-- num;return 1;
		case 'x':
			*retChar = 'x';
			++ data;
			if (--num > 0)
				return(Escape_xhh(retChar,data,num) + 1);
			return 1;
		default:
			return(::Escape_ooo(retChar,data,num));
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
uint32 FIFO8::EscapeToChar(uint8 *retChar,const uint8 *&data,uint32 &num){
	return(::EscapeToChar(retChar,data,num));
}
//------------------------------------------------------------------------------------------//
uint32 FIFO8::EscapeToChar(uint8 *retChar,const uint8 *&data,uint64 &num){
	return(::EscapeToChar(retChar,data,num));
}
//------------------------------------------------------------------------------------------//
uint32 FIFO8::PutInHEX(const uint8 *data,uint32 num){
	uint32	length,offset,slength,copyNum;
	uint8	*addr,*dataT,chardata;
	
	if ((dataAreaPointer == nullptr) || (num == 0))
		return 0;
	
	copyNum = 0;
	offset = 0;
	slength = CalcInCapacity(length,offset);
	dataT = &chardata;
	
	if (length > 0){
		addr = dataAreaPointer + offset;
		while(num > 0){
			if (Escape_xhh(dataT,data,num) > 0){
				*addr = chardata;
				++addr;
				++copyNum;
				--length;
			}
			else{
				++data;
				--num;
			}
			if ((length == 0) && (slength > 0)){
				addr = dataAreaPointer;
				length = slength;
				slength = 0;
			}
			if (length == 0)
				break;
		}
	};
	if (inPreCount == 0){
		offsetIn += copyNum;
	}
	else{
		offsetInPre += copyNum;
	}
	return(copyNum);
}
//------------------------------------------------------------------------------------------//
uint32 FIFO8::PutInEscape(const uint8 *data,uint32 num){
	uint32	length,offset,slength,copyNum,count;
	uint8	*addr,chardata;
	const uint8	*p;
	
	if ((dataAreaPointer == nullptr) || (num == 0))
		return 0;
	
	copyNum = 0;
	offset = 0;
	slength = CalcInCapacity(length,offset);
	
	if (length > 0){
		addr = dataAreaPointer + offset;
		p = data;
		count  = 0;
		while(num > 0){
			if (*p == '\\'){
				copyNum += Copy(addr,length,data,count);
				if ((length == 0) && (slength > 0)){
					addr = dataAreaPointer;
					length = slength;
					slength = 0;
					copyNum += Copy(addr,length,data,count);
				}
				if ((count > 0) || (length == 0))
					break;
				++p;
				if (--num > 0){
					if (EscapeToChar(&chardata,p,num) > 0){
						*addr = chardata;
						++addr;
						++copyNum;
						--length;
					}
				}
				data = p;
			}
			else{
				++p;
				--num;
				++count;
			}
		}
		copyNum += Copy(addr,length,data,count);
		if ((count > 0) && (slength > 0)){
			addr = dataAreaPointer;
			copyNum += Copy(addr,slength,data,count);
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
uint32 FIFO8::ReadInEscape(uint8 *dataOut,uint32 num,uint32 offset)const{
	uint32	length,slength,copyNum;
	const uint8	*addr,*p,*p2;
	uint32	count,i,j;
	uint8	chardata,data[3];
	
	if ((dataAreaPointer == nullptr) || (num == 0))
		return 0;
	
	copyNum = 0;
	slength = CalcOutCapacity(length,offset);
	
	if (length > 0){
		addr = dataAreaPointer + offset;
		p = addr;
		count  = 0;
		while(num > count){
			if (*p == '\\'){
				copyNum += Copy(dataOut,num,addr,count);
				if (num == 0)
					return(copyNum);
				++p;
				--length;
				if ((length < 3) && (slength > 0)){
					switch(length){
						case 0:;
							data[0] = *dataAreaPointer;
							if (slength > 1)
								data[1] = *(dataAreaPointer + 1);
							if (slength > 2)
								data[2] = *(dataAreaPointer + 2);
							break;
						case 1:;
							data[0] = *p;
							data[1] = *dataAreaPointer;
							if (slength > 1)
								data[2] = *(dataAreaPointer + 1);
							break;
						case 2:;
							data[0] = *p;
							data[1] = *(p + 1);
							data[2] = *dataAreaPointer;
						default:;
					}
					j = length + slength;
					p2 = data;
					i = EscapeToChar(&chardata,p2,j);
					if (i > 0){
						*dataOut = chardata;
						++dataOut;
						++copyNum;
						--num;
						if (i > ((length > 0)?(length - 1):0)){
							p = dataAreaPointer + i - length;
							length = j;
							slength = 0;
						}
						else{
							length -= i;
							p += i;
						}
					}
				}
				else if (length > 0){
					if (EscapeToChar(&chardata,p,length) > 0){
						*dataOut = chardata;
						++dataOut;
						++copyNum;
						--num;
					}
				}
				addr = p;
			}
			else{
				++p;
				++count;
				--length;
			}
			if ((length == 0) && (slength > 0)){
				copyNum += Copy(dataOut,num,addr,count);
				p = dataAreaPointer;
				length = slength;
				slength = 0;
				addr = p;
			}
			if (length == 0)
				break;
		}
		copyNum += Copy(dataOut,num,addr,count);
	}
	return(copyNum);
}
//------------------------------------------------------------------------------------------//
uint32 FIFO8::Put(const STDSTR &strIn,G_ESCAPE blEscape){
	if (blEscape == G_ESCAPE_OFF)
		return(Put((uint8*)strIn.c_str(),(uint32)strIn.length()));
	return(PutInEscape((uint8*)strIn.c_str(),(uint32)strIn.length()));
}
//------------------------------------------------------------------------------------------//
const STDSTR& FIFO8::Read(STDSTR *retStr,uint32 num,uint32 offset)const{
	uint32	slength;
	uint8	*addr;
	
	if ((dataAreaPointer == nullptr) || (num == 0))
		return(*retStr);
	
	slength = CalcOutLength(num,offset);
	
	if (num > 0){
		addr = dataAreaPointer + offset;
		retStr->append((char*)addr,num);
		if (slength > 0){
			addr = dataAreaPointer;
			retStr->append((char*)addr,slength);
		}
	}
	return(*retStr);
}
//------------------------------------------------------------------------------------------//
const STDSTR& FIFO8::Get(STDSTR *retStr,uint32 num){
	uint32	offset;
	
	if ((dataAreaPointer == nullptr) || (num == 0))
		return(*retStr);
	
	offset = 0;
	num += CalcOutLength(num,offset);
	
	Read(retStr,num,0);
	offsetOut += num;
	return(*retStr);
}
//------------------------------------------------------------------------------------------//
void FIFO8::UnEscapeToStr(STDSTR *retStr,uint8 data){
	switch (data) {
		case '\\':
			*retStr += "\\\\";
			break;
		case '\a':
			*retStr += "\\a";
			break;
		case '\b':
			*retStr += "\\b";
			break;
		case '\f':
			*retStr += "\\f";
			break;
		case '\n':
			*retStr += "\\n";
			break;
		case '\r':
			*retStr += "\\r";
			break;
		case '\t':
			*retStr += "\\t";
			break;
		case '\v':
			*retStr += "\\v";
			break;
		case '\0':
			*retStr += "\\0";
			break;
		default:
			*retStr += data;
			break;
	}
}
//------------------------------------------------------------------------------------------//
const STDSTR& FIFO8::ReadUnEscape(STDSTR *retStr,uint32 num,uint32 offset)const{
	uint32	slength;
	uint8	*addr;
	
	if ((dataAreaPointer == nullptr) || (num == 0))
		return(*retStr);
	
	offset = 0;
	slength = CalcOutLength(num, offset);
	if (num > 0){
		do{
			addr = dataAreaPointer + offset;
			while(num-- > 0){
				UnEscapeToStr(retStr,*addr);
				++ addr;
			}
			if (slength == 0)
				break;
			num = slength;
			offset = 0;
			slength = 0;
		}while(1);
	}
	return(*retStr);
}
//------------------------------------------------------------------------------------------//
const STDSTR& FIFO8::GetUnEscape(STDSTR *retStr,uint32 num){
	uint32	offset;
	
	if ((dataAreaPointer == nullptr) || (num == 0))
		return(*retStr);
	
	offset = 0;
	num += CalcOutLength(num,offset);
	
	ReadUnEscape(retStr,num,0);
	offsetOut += num;
	return(*retStr);
}
//------------------------------------------------------------------------------------------//
void FIFO8::CharToHex(STDSTR *retStr,uint8 data){
	uint8	charData;
	
	charData = (data >> 4) & (0x0f);
	if (charData < 0x0a){
		charData += '0';
	}
	else{
		charData += ('A' - 0x0a);
	}
	
	*retStr += charData;
	
	charData = data & (0x0f);
	if (charData < 0x0a){
		charData += '0';
	}
	else{
		charData += ('A' - 0x0a);
	}
	
	*retStr += charData;
}
//------------------------------------------------------------------------------------------//
const STDSTR& FIFO8::ReadInHEX(STDSTR *retStr,uint32 num,uint32 offset)const{
	uint32	slength;
	uint8	*addr;
	
	if (dataAreaPointer == nullptr)
		return(*retStr);
	
	offset = 0;
	slength = CalcOutLength(num, offset);
	if (num > 0){
		do{
			addr = dataAreaPointer + offset;
			while(num-- > 0){
				CharToHex(retStr,*addr);
				++ addr;
			}
			if (slength == 0)
				break;
			num = slength;
			offset = 0;
			slength = 0;
		}while(1);
	}
	return(*retStr);
}
//------------------------------------------------------------------------------------------//
const STDSTR& FIFO8::GetInHEX(STDSTR *retStr,uint32 num){
	uint32	offset;
	
	if ((dataAreaPointer == nullptr) || (num == 0))
		return(*retStr);
	
	offset = 0;
	num += CalcOutLength(num,offset);
	
	ReadInHEX(retStr,num,0);
	offsetOut += num;
	return(*retStr);
}
//------------------------------------------------------------------------------------------//
const STDSTR& FIFO8::ReadInHEX_S(STDSTR *retStr,uint32 num,uint32 offset)const{
	uint32	slength;
	uint8	*addr;
	
	if (dataAreaPointer == nullptr)
		return(*retStr);
	
	offset = 0;
	slength = CalcOutLength(num, offset);
	if (num > 0){
		do{
			addr = dataAreaPointer + offset;
			while(num-- > 0){
				CharToHex(retStr,*addr);
				*retStr += ' ';
				++ addr;
			}
			if (slength == 0)
				break;
			num = slength;
			offset = 0;
			slength = 0;
		}while(1);
	}
	return(*retStr);
}
//------------------------------------------------------------------------------------------//
const STDSTR& FIFO8::GetInHEX_S(STDSTR *retStr,uint32 num){
	uint32	offset;
	
	if ((dataAreaPointer == nullptr) || (num == 0))
		return(*retStr);
	
	offset = 0;
	num += CalcOutLength(num,offset);
	
	ReadInHEX_S(retStr,num,0);
	offsetOut += num;
	return(*retStr);
}
//------------------------------------------------------------------------------------------//
const STDSTR& FIFO8::ReadInLine(STDSTR *retStr,uint8 endChar,uint32 offset)const{
	uint32	length,slength,count;
	uint8	*addr,*p;
	
	if (dataAreaPointer == nullptr)
		return(*retStr);
	
	slength = CalcOutCapacity(length, offset);
	addr = dataAreaPointer + offset;
	
	while (length > 0){
		count  = 0;
		p = addr;
		while(++count <= length){
			if (*p == endChar){
				retStr->append((char*)addr, count - 1);
				return(*retStr);
			}
			++ p;
		}
		retStr->append((char*)addr, count - 1);
		
		length = slength;
		addr = dataAreaPointer;
	}
	return(*retStr);
}
//------------------------------------------------------------------------------------------//
const STDSTR& FIFO8::GetInLine(STDSTR *retStr,uint8 endChar){
	uint32	length,slength,offset,count,copyNum;
	uint8	*addr,*p;
	
	if (dataAreaPointer == nullptr)
		return(*retStr);
	
	offset = 0;
	slength = CalcOutCapacity(length, offset);
	addr = dataAreaPointer + offset;
	copyNum = 0;
	while (length > 0){
		count  = 0;
		p = addr;
		while(++count <= length){
			if (*p == endChar){
				retStr->append((char*)addr, count - 1);
				copyNum += count;
				offsetOut += copyNum;
				return(*retStr);
			}
			++ p;
		}
		retStr->append((char*)addr, count - 1);
		copyNum += (count - 1);
		length = slength;
		addr = dataAreaPointer;
	}
	offsetOut += copyNum;
	return(*retStr);
}
//------------------------------------------------------------------------------------------//
uint32 FIFO8::Put(std::stringstream &streamIn){
	uint32	length,offset;
	uint32	slength,copyNum;
	uint8	*addr;
	
	if (dataAreaPointer == nullptr)
		return 0;
	
	copyNum = 0;
	offset = 0;
	slength = CalcInCapacity(length,offset);
	
	if (length > 0){
		addr = dataAreaPointer + offset;
		copyNum += (uint32)streamIn.readsome((char*)addr, length);
		if ((!streamIn.eof()) && (slength > 0)){
			addr = dataAreaPointer;
			copyNum += (uint32)streamIn.readsome((char*)addr, slength);
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
uint32 FIFO8::Get(std::stringstream *streamOut){
	uint32	length,offset;
	uint32	slength,copyNum;
	uint8	*addr;
	
	if (dataAreaPointer == nullptr)
		return 0;
	
	copyNum = 0;
	offset = 0;
	slength = CalcOutCapacity(length,offset);
	
	if (length > 0){
		addr = dataAreaPointer + offset;
		streamOut->write((char*)addr, length);
		copyNum += length;
		if (slength > 0){
			addr = dataAreaPointer;
			streamOut->write((char*)addr, length);
			copyNum += length;
		}
		offsetOut += copyNum;
	}
	return(copyNum);
}
//------------------------------------------------------------------------------------------//
void FIFO8::FillZero(void){
	uint8 *p;
	uint32 i;
	p = dataAreaPointer;
	i = 0;
	while(i++ < maxSize)
		*p++ = 0;
	offsetIn = 0;
	offsetOut = 0;
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------/
#endif
