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
std::string FIFO_UINT8::CharToHex(uint8 data){
	std::string		stringData;
	uint8			charData;
	
	charData = (data >> 4) & (0x0f);
	if (charData < 0x0a){
		charData += '0';
	}
	else{
		charData += ('A' - 0x0a);
	}
	
	stringData = charData;
	
	charData = data & (0x0f);
	if (charData < 0x0a){
		charData += '0';
	}
	else{
		charData += ('A' - 0x0a);
	}
	
	stringData += charData;
	return(stringData);
}
//------------------------------------------------------------------------------------------//
uint32 FIFO_UINT8::Escape0xhhToChar(uint8 *retChar,const std::string &strInput){
	std::string::size_type		j,length;
	std::string	stringData;
	uint8		charData,charResult;
	
	*retChar = 0;
	length = strInput.length();
	if (length < 4)
		return 0;
	
	stringData = strInput.substr(0,3);
	if ((stringData == "\\0x") || (stringData == "\\0X")){
		charData = strInput[3];
		j = 0;
		if (charData >= '0' && charData <= '9'){
			charData = charData - '0';
			++ j;
		}
		else if (charData >= 'A' && charData <= 'F'){
			charData = charData - 'A' + 0x0a;
			++ j;
		}
		else if (charData >= 'a' && charData <= 'f'){
			charData = charData - 'a' + 0x0a;
			++ j;
		}
		if (j == 0)
			return 0;
		charResult = charData;
		if (length == 4){
			*retChar = charResult;
			return 3;
		}
		charData = strInput[4];
		if (charData >= '0' && charData <= '9'){
			charData = charData - '0';
			++ j;
		}
		else if (charData >= 'A' && charData <= 'F'){
			charData = charData - 'A' + 0x0a;
			++ j;
		}
		else if (charData >= 'a' && charData <= 'f'){
			charData = charData - 'a' + 0x0a;
			++ j;
		}
		if (j == 1){
			*retChar = charResult;
			return 3;
		}
		charResult = ((charResult << 4) & 0xf0) | charData;
		*retChar = charResult;
		return 4;
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
uint32 FIFO_UINT8::EscapeToChar(uint8 *retChar,const std::string &strInput){
	//	\a,\b,\f,\n,\r,\t,\v,\\,\',\",\0
	
	if (strInput == "\\\\"){
		*retChar = '\\';
		return 1;
	}
	if (strInput == "\\a"){
		*retChar = '\a';
		return 1;
	}
	if (strInput == "\\b"){
		*retChar = '\b';
		return 1;
	}
	if (strInput == "\\f"){
		*retChar = '\f';
		return 1;
	}
	if (strInput == "\\n"){
		*retChar = '\n';
		return 1;
	}
	if (strInput == "\\r"){
		*retChar = '\r';
		return 1;
	}
	if (strInput == "\\t"){
		*retChar = '\t';
		return 1;
	}
	if (strInput == "\\v"){
		*retChar = '\v';
		return 1;
	}
	if (strInput == "\\\'"){
		*retChar = '\'';
		return 1;
	}
	if (strInput == "\\\""){
		*retChar = '\"';
		return 1;
	}
	if (strInput == "\\0"){
		*retChar = '\0';
		return 1;
	}
	if (strInput[0] == '\\'){
		*retChar = strInput[1];
		return 1;
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
void FIFO_UINT8::EscapeToStr(std::string *retStr,uint8 dataChar){
	switch (dataChar) {
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
			*retStr += dataChar;
			break;
	}
}
//------------------------------------------------------------------------------------------//
uint32 FIFO_UINT8::GetInLine(std::string *retStr,uint8 endChar){
	uint32	length,slength,offset,numRet;
	uint8	*addr;
	
	if (dataAreaPointer == nullptr)
		return 0;
	
	numRet = 0;
	offset = 0;
	slength = CalcOutCapacity(length, offset);

	if (length > 0){
		do{
			addr = dataAreaPointer + offset;
			while(length-- > 0){
				*retStr += *addr;
				++ numRet;
				if (*addr == endChar)
					goto GetInLine_end;
				++ addr;
			}
			if (slength == 0)
				break;
			length = slength;
			offset = 0;
			slength = 0;
		}while(1);
	GetInLine_end:
		offsetOut += numRet;
	}
	return(numRet);
}
//------------------------------------------------------------------------------------------//
uint32 FIFO_UINT8::ReadInLine(std::string *retStr,uint8 endChar,uint32 offset)const{
	uint32	length,slength,numRet;
	uint8	*addr;
	
	if (dataAreaPointer == nullptr)
		return 0;
	
	numRet = 0;
	slength = CalcOutCapacity(length, offset);
	
	if (length > 0){
		do{
			addr = dataAreaPointer + offset;
			while(length-- > 0){
				*retStr += *addr;
				++ numRet;
				if (*addr == endChar)
					goto GetInLine_end;
				++ addr;
			}
			if (slength == 0)
				break;
			length = slength;
			offset = 0;
			slength = 0;
		}while(1);
	}
GetInLine_end:
	return(numRet);
}
//------------------------------------------------------------------------------------------//
uint32 FIFO_UINT8::GetInASCII(std::string *retStr,uint32 num){
	uint32	offset,slength,numRet;
	uint8	*addr;
	
	if (dataAreaPointer == nullptr)
		return 0;
	
	offset = 0;
	slength = CalcOutLength(num, offset);
	numRet = num + slength;
	if (num > 0){
		do{
			addr = dataAreaPointer + offset;
			while(num-- > 0){
				*retStr += *addr;
				++ addr;
			}
			if (slength == 0)
				break;
			num = slength;
			offset = 0;
			slength = 0;
		}while(1);
		offsetOut += numRet;
	}
	return(numRet);
}
//------------------------------------------------------------------------------------------//
uint32 FIFO_UINT8::ReadInASCII(std::string *retStr,uint32 num,uint32 offset)const{
	uint32	slength,numRet;
	uint8	*addr;
	
	if (dataAreaPointer == nullptr)
		return 0;
	
	slength = CalcOutLength(num, offset);
	numRet = num + slength;
	if (num > 0){
		do{
			addr = dataAreaPointer + offset;
			while(num-- > 0){
				*retStr += *addr;
				++ addr;
			}
			if (slength == 0)
				break;
			num = slength;
			offset = 0;
			slength = 0;
		}while(1);
	}
	return(numRet);
}
//------------------------------------------------------------------------------------------//
uint32 FIFO_UINT8::GetInASCII_E(std::string *retStr,uint32 num){
	uint32	offset,slength,numRet;
	uint8	*addr;
	
	if (dataAreaPointer == nullptr)
		return 0;
	
	offset = 0;
	slength = CalcOutLength(num, offset);
	numRet = num + slength;
	if (num > 0){
		do{
			addr = dataAreaPointer + offset;
			while(num-- > 0){
				EscapeToStr(retStr,*addr);
				++ addr;
			}
			if (slength == 0)
				break;
			num = slength;
			offset = 0;
			slength = 0;
		}while(1);
		offsetOut += numRet;
	}
	return(numRet);
}
//------------------------------------------------------------------------------------------//
uint32 FIFO_UINT8::ReadInASCII_E(std::string *retStr,uint32 num,uint32 offset)const{
	uint32	slength,numRet;
	uint8	*addr;
	
	if (dataAreaPointer == nullptr)
		return 0;
	
	offset = 0;
	slength = CalcOutLength(num, offset);
	numRet = num + slength;
	if (num > 0){
		do{
			addr = dataAreaPointer + offset;
			while(num-- > 0){
				EscapeToStr(retStr,*addr);
				++ addr;
			}
			if (slength == 0)
				break;
			num = slength;
			offset = 0;
			slength = 0;
		}while(1);
	}
	return(numRet);
}
//------------------------------------------------------------------------------------------//
uint32 FIFO_UINT8::GetInHEX(std::string *retStr,uint32 num){
	uint32	offset,slength,numRet;
	uint8	*addr;
	
	if (dataAreaPointer == nullptr)
		return 0;
	
	offset = 0;
	slength = CalcOutLength(num, offset);
	numRet = num + slength;
	if (num > 0){
		do{
			addr = dataAreaPointer + offset;
			while(num-- > 0){
				*retStr += CharToHex(*addr);
				++ addr;
			}
			if (slength == 0)
				break;
			num = slength;
			offset = 0;
			slength = 0;
		}while(1);
		offsetOut += numRet;
	}
	return(numRet);
}
//------------------------------------------------------------------------------------------//
uint32 FIFO_UINT8::ReadInHEX(std::string *retStr,uint32 num,uint32 offset)const{
	uint32	slength,numRet;
	uint8	*addr;
	
	if (dataAreaPointer == nullptr)
		return 0;
	
	offset = 0;
	slength = CalcOutLength(num, offset);
	numRet = num + slength;
	if (num > 0){
		do{
			addr = dataAreaPointer + offset;
			while(num-- > 0){
				*retStr += CharToHex(*addr);
				++ addr;
			}
			if (slength == 0)
				break;
			num = slength;
			offset = 0;
			slength = 0;
		}while(1);
	}
	return(numRet);
}
//------------------------------------------------------------------------------------------//
uint32 FIFO_UINT8::GetInHEX_S(std::string *retStr,uint32 num){
	uint32	offset,slength,numRet;
	uint8	*addr;
	
	if (dataAreaPointer == nullptr)
		return 0;
	
	offset = 0;
	slength = CalcOutLength(num, offset);
	numRet = num + slength;
	if (num > 0){
		do{
			addr = dataAreaPointer + offset;
			while(num-- > 0){
				*retStr += CharToHex(*addr);
				*retStr += ' ';
				++ addr;
			}
			if (slength == 0)
				break;
			num = slength;
			offset = 0;
			slength = 0;
		}while(1);
		offsetOut += numRet;
	}
	return(numRet);
}
//------------------------------------------------------------------------------------------//
uint32 FIFO_UINT8::ReadInHEX_S(std::string *retStr,uint32 num,uint32 offset)const{
	uint32	slength,numRet;
	uint8	*addr;
	
	if (dataAreaPointer == nullptr)
		return 0;
	
	offset = 0;
	slength = CalcOutLength(num, offset);
	numRet = num + slength;
	if (num > 0){
		do{
			addr = dataAreaPointer + offset;
			while(num-- > 0){
				*retStr += CharToHex(*addr);
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
	return(numRet);
}
//------------------------------------------------------------------------------------------//
uint32 FIFO_UINT8::PutInHEX(const std::string &strInput){
	std::string::size_type	i,length,j;
	uint8	charP,charData,charResult;
	uint32	unuse,unuseToEnd,count;
	uint8	*addr;
	
	if (dataAreaPointer == nullptr)
		return 0;
	
	unuse = maxSize - offsetIn + offsetOut;
	unuseToEnd = maxSize - (offsetIn & (maxSize - 1));
	if (unuseToEnd > unuse)
		unuseToEnd = unuse;
	addr = dataAreaPointer + (offsetIn & (maxSize - 1));
	
	length = strInput.length();
	i = 0;
	j = 0;
	count = 0;
	charResult = 0;
	charData = 0;
	while(i < length){
		charP = strInput[i ++];
		
		if (charP >= '0' && charP <= '9'){
			charData = charP - '0';
			++ j;
		}
		else if (charP >= 'A' && charP <= 'F'){
			charData = charP - 'A' + 0x0a;
			++ j;
		}
		else if (charP >= 'a' && charP <= 'f'){
			charData = charP - 'a' + 0x0a;
			++ j;
		}
		if (j == 1){
			charResult = charData;
		}
		else if (j == 2){
			charResult = ((charResult << 4) & 0xf0) | charData;
		}
		if (((charP == ' ') && (j > 0)) || (j == 2) || (i == length)){
			if (unuseToEnd > 0){
				*addr = charResult;
				++ addr;
				++ count;
				if (--unuseToEnd == 0){
					unuse -= count;
					addr = dataAreaPointer;
				}
			}
			else if (unuse > 0){
				*addr = charResult;
				++ addr;
				++ count;
				-- unuse;
			}
			else{
				break;
			}
			j = 0;
		}
	}
	offsetIn += count;
	return(count);
}
//------------------------------------------------------------------------------------------//
uint32 FIFO_UINT8::PutInEscape(const std::string &strInput){
	//eacape \0xhh,\a,\b,\f,\n,\r,\t,\v,\\,\',\",\0,\/
	std::string				strData;
	std::string::size_type	i,length,j;
	uint8					charData,charData1;
	uint32	unuse,unuseToEnd,count;
	uint8	*addr;
	
	if (dataAreaPointer == nullptr)
		return 0;
	
	unuse = maxSize - offsetIn + offsetOut;
	unuseToEnd = maxSize - (offsetIn & (maxSize - 1));
	if (unuseToEnd > unuse)
		unuseToEnd = unuse;
	addr = dataAreaPointer + (offsetIn & (maxSize - 1));
	
	length = strInput.length();
	i = 0;
	count = 0;
	charData1 = 0;
	while(i < length){
		charData = strInput[i];
		j = 0;
		if ((i + 4) < length){
			strData = strInput.substr(i,5);
			j = Escape0xhhToChar(&charData1,strData);
		}
		else if ((i + 4) == length){
			strData = strInput.substr(i,4);
			j = Escape0xhhToChar(&charData1,strData);
		}
		if (j > 0){
			i += j;
			charData = charData1;
		}
		else if ((i + 1) < length){
			strData = strInput.substr(i,2);
			j = EscapeToChar(&charData1,strData);
			if (j > 0){
				i += j;
				charData = charData1;
			}
		}
		else if (charData == '\\'){
			++ i;
			continue;
		}
		if (unuseToEnd > 0){
			*addr = charData;
			++ addr;
			++ count;
			if (--unuseToEnd == 0){
				unuse -= count;
				addr = dataAreaPointer;
			}
		}
		else if (unuse > 0){
			*addr = charData;
			++ addr;
			++ count;
			-- unuse;
		}
		else{
			break;
		}
		++ i;
	};
	offsetIn += count;
	return(count);
}
//------------------------------------------------------------------------------------------//
uint32 FIFO_UINT8::PutInASCII(const std::string &strInput,G_ESCAPE_VAILD blEscape){
	if (blEscape == G_ESCAPE_OFF)
		return(Put((uint8*)strInput.c_str(),(uint32)strInput.length()));
	return(PutInEscape(strInput));
}
//------------------------------------------------------------------------------------------//
void FIFO_UINT8::FillZero(void){
	uint8 *p;
	uint32 i;
	p = dataAreaPointer;
	i = 0;
	while(i++ < maxSize)
		*p++ = 0;
	offsetIn = 0;
	offsetOut = 0;
}
//------------------------------------------------------------------------------------------/

