//
//  DS_ARRAY/cpp
//  SDT
//
//  Created by Leif Wen on 18/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "DS_ARRAY.h"
#include "DS_STRING.h"
#ifdef DS_ARRAY_h
//------------------------------------------------------------------------------------------//
ARRAY::ARRAY(void) : DS_ARRAY_FRAME<uint8>(){alreadyReadNum = 0;};
//------------------------------------------------------------------------------------------//
uint32 ARRAY::Copy(uint8 *&to_addr,uint32 &to_length,const uint8 *&in_addr,uint32 &in_length)const{
	uint32	copyNum;
	copyNum = (to_length <= in_length) ? to_length : in_length;
	memcpy(to_addr,in_addr,copyNum);
	to_addr += copyNum;
	in_addr += copyNum;
	to_length -= copyNum;
	in_length -= copyNum;
	return(copyNum);
};
//------------------------------------------------------------------------------------------//
void ARRAY::FillZero(void){
	uint8 *p;
	uint32 i;
	p = cgDataBuffer;
	i = 0;
	while(i++ < maxSize)
		*p++ = 0;
	offsetIn = 0;
	offsetOut = 0;
};
//------------------------------------------------------------------------------------------//
uint32 ARRAY::PutInEscape(const uint8* data,uint32 num){
	uint32	length,offset,slength,copyNum,count;
	uint8	*addr,chardata;
	const uint8	*p;
	
	status = DSAF_blNormal;
	if ((cgDataBuffer == nullptr) || (num == 0))
		return 0;
	
	copyNum = 0;
	offset = 0;
	slength = CalcInCapacity(length,offset);
	
	if (length > 0){
		addr = cgDataBuffer + offset;
		p = data;
		count  = 0;
		while(num > 0){
			--num;
			if (*p == '\\'){
				copyNum += Copy(addr,length,data,count);
				if ((length == 0) && (slength > 0)){
					addr = cgDataBuffer;
					length = slength;
					slength = 0;
					copyNum += Copy(addr,length,data,count);
				}
				if ((count > 0) || (length == 0))
					break;
				++p;
				if (num > 0){
					if (Str_EscapeToChar(chardata,p,num) > 0){
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
				++count;
			}
		}
		copyNum += Copy(addr,length,data,count);
		if ((count > 0) && (slength > 0)){
			addr = cgDataBuffer;
			copyNum += Copy(addr,slength,data,count);
		}
		
		if (countPreIn == 0){
			offsetIn += copyNum;
		}
		else{
			offsetPreIn += copyNum;
		}
	}
	if (num > 0)
		status = DSAF_blNoMem;
	return(copyNum);
};
//------------------------------------------------------------------------------------------//
uint32 ARRAY::ReadInEscape(uint8* dataOut,uint32 num,uint32 offset)const{
	uint32	length,slength,copyNum;
	const uint8	*addr,*p,*p2;
	uint32	count,i,j;
	uint8	chardata,data[3];
	
	if ((cgDataBuffer == nullptr) || (num == 0))
		return 0;
	
	copyNum = 0;
	slength = CalcOutCapacity(length,offset);
	
	if (length > 0){
		addr = cgDataBuffer + offset;
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
							data[0] = *cgDataBuffer;
							if (slength > 1)
								data[1] = *(cgDataBuffer + 1);
							if (slength > 2)
								data[2] = *(cgDataBuffer + 2);
							break;
						case 1:;
							data[0] = *p;
							data[1] = *cgDataBuffer;
							if (slength > 1)
								data[2] = *(cgDataBuffer + 1);
							break;
						case 2:;
							data[0] = *p;
							data[1] = *(p + 1);
							data[2] = *cgDataBuffer;
						default:;
					}
					j = length + slength;
					p2 = data;
					i = Str_EscapeToChar(chardata,p2,j);
					if (i > 0){
						*dataOut = chardata;
						++dataOut;
						++copyNum;
						--num;
						if (i > ((length > 0)?(length - 1):0)){
							p = cgDataBuffer + i - length;
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
					if (Str_EscapeToChar(chardata,p,length) > 0){
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
				p = cgDataBuffer;
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
};
//------------------------------------------------------------------------------------------//
uint32 ARRAY::PutInHEX(const uint8* data,uint32 num){
	uint32	length,offset,slength,copyNum;
	uint8	*addr,chardata;
	
	status = DSAF_blNormal;
	if ((cgDataBuffer == nullptr) || (num == 0))
		return 0;
	
	copyNum = 0;
	offset = 0;
	slength = CalcInCapacity(length,offset);
	
	if (length > 0){
		addr = cgDataBuffer + offset;
		while(num > 0){
			if (Str_Escape_xhh(chardata,data,num) > 0){
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
				addr = cgDataBuffer;
				length = slength;
				slength = 0;
			}
			if (length == 0)
				break;
		}
	};
	if (countPreIn == 0){
		offsetIn += copyNum;
	}
	else{
		offsetPreIn += copyNum;
	}
	if (num > 0)
		status = DSAF_blNoMem;
	return(copyNum);
};
//------------------------------------------------------------------------------------------//
uint32 ARRAY::Put(const STDSTR &strIn,G_ESCAPE blEscape){
	if (blEscape == G_ESCAPE_OFF)
		return(Put((uint8*)strIn.c_str(),(uint32)strIn.length()));
	return(PutInEscape((uint8*)strIn.c_str(),(uint32)strIn.length()));
};
//------------------------------------------------------------------------------------------//
const STDSTR& ARRAY::Read(STDSTR* _out,uint32 num,uint32 offset)const{
	uint32	slength;
	uint8	*addr;
	
	if ((cgDataBuffer == nullptr) || (num == 0))
		return(*_out);
	
	slength = CalcOutLength(num,offset);
	
	if (num > 0){
		addr = cgDataBuffer + offset;
		_out->append((char*)addr,num);
		if (slength > 0){
			addr = cgDataBuffer;
			_out->append((char*)addr,slength);
		}
	}
	return(*_out);
};
//------------------------------------------------------------------------------------------//
const STDSTR& ARRAY::Get(STDSTR* _out,uint32 num){
	uint32	offset;
	
	if ((cgDataBuffer == nullptr) || (num == 0))
		return(*_out);
	
	offset = 0;
	num += CalcOutLength(num,offset);
	
	Read(_out,num,0);
	offsetOut += num;
	return(*_out);
};
//------------------------------------------------------------------------------------------//
const STDSTR& ARRAY::ReadInASCII(STDSTR* _out,uint32 num,uint32 offset)const{
	uint32	slength;
	uint8	*addr;
	
	if ((cgDataBuffer == nullptr) || (num == 0))
		return(*_out);
	
	offset = 0;
	slength = CalcOutLength(num, offset);
	if (num > 0){
		do{
			addr = cgDataBuffer + offset;
			while(num-- > 0){
				Str_EscapeToASCII(_out,*addr);
				++ addr;
			}
			if (slength == 0)
				break;
			num = slength;
			offset = 0;
			slength = 0;
		}while(1);
	}
	return(*_out);
};
//------------------------------------------------------------------------------------------//
const STDSTR& ARRAY::GetInASCII(STDSTR* _out,uint32 num){
	uint32	offset;
	
	if ((cgDataBuffer == nullptr) || (num == 0))
		return(*_out);
	
	offset = 0;
	num += CalcOutLength(num,offset);
	
	ReadInASCII(_out,num,0);
	offsetOut += num;
	return(*_out);
};
//------------------------------------------------------------------------------------------//
const STDSTR& ARRAY::ReadInHEX(STDSTR* _out,uint32 num,uint32 offset)const{
	uint32	slength;
	uint8	*addr;
	
	if (cgDataBuffer == nullptr)
		return(*_out);
	
	offset = 0;
	slength = CalcOutLength(num, offset);
	if (num > 0){
		do{
			addr = cgDataBuffer + offset;
			while(num-- > 0){
				Str_CharToHex(_out,*addr);
				++ addr;
			}
			if (slength == 0)
				break;
			num = slength;
			offset = 0;
			slength = 0;
		}while(1);
	}
	return(*_out);
};
//------------------------------------------------------------------------------------------//
const STDSTR& ARRAY::GetInHEX(STDSTR* _out,uint32 num){
	uint32	offset;
	
	if ((cgDataBuffer == nullptr) || (num == 0))
		return(*_out);
	
	offset = 0;
	num += CalcOutLength(num,offset);
	
	ReadInHEX(_out,num,0);
	offsetOut += num;
	return(*_out);
};
//------------------------------------------------------------------------------------------//
const STDSTR& ARRAY::ReadInHEX_S(STDSTR* _out,uint32 num,uint32 offset)const{
	uint32	slength;
	uint8	*addr;
	
	if (cgDataBuffer == nullptr)
		return(*_out);
	
	offset = 0;
	slength = CalcOutLength(num, offset);
	if (num > 0){
		do{
			addr = cgDataBuffer + offset;
			while(num-- > 0){
				Str_CharToHex(_out,*addr);
				*_out += ' ';
				++ addr;
			}
			if (slength == 0)
				break;
			num = slength;
			offset = 0;
			slength = 0;
		}while(1);
	}
	return(*_out);
};
//------------------------------------------------------------------------------------------//
const STDSTR& ARRAY::GetInHEX_S(STDSTR* _out,uint32 num){
	uint32	offset;
	
	if ((cgDataBuffer == nullptr) || (num == 0))
		return(*_out);
	
	offset = 0;
	num += CalcOutLength(num,offset);
	
	ReadInHEX_S(_out,num,0);
	offsetOut += num;
	return(*_out);
};
//------------------------------------------------------------------------------------------//
const STDSTR& ARRAY::ReadInLine(STDSTR* _out,uint8 endChar,uint32 offset)const{
	uint32	length,slength,count;
	uint8	*addr,*p;
	
	if (cgDataBuffer == nullptr)
		return(*_out);
	
	slength = CalcOutCapacity(length, offset);
	addr = cgDataBuffer + offset;
	
	while (length > 0){
		count  = 0;
		p = addr;
		while(++count <= length){
			if (*p == endChar){
				_out->append((char*)addr, count - 1);
				return(*_out);
			}
			++ p;
		}
		_out->append((char*)addr, count - 1);
		
		length = slength;
		addr = cgDataBuffer;
	}
	return(*_out);
};
//------------------------------------------------------------------------------------------//
const STDSTR& ARRAY::GetInLine(STDSTR* _out,uint8 endChar){
	uint32	length,slength,offset,count,copyNum;
	uint8	*addr,*p;
	
	if (cgDataBuffer == nullptr)
		return(*_out);
	
	offset = 0;
	slength = CalcOutCapacity(length, offset);
	addr = cgDataBuffer + offset;
	copyNum = 0;
	while (length > 0){
		count  = 0;
		p = addr;
		while(++count <= length){
			if (*p == endChar){
				_out->append((char*)addr, count - 1);
				copyNum += count;
				offsetOut += copyNum;
				return(*_out);
			}
			++ p;
		}
		_out->append((char*)addr, count - 1);
		copyNum += (count - 1);
		length = slength;
		addr = cgDataBuffer;
	}
	offsetOut += copyNum;
	return(*_out);
};
//------------------------------------------------------------------------------------------//
#endif
