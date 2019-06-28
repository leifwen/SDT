//
//  DS_STRING.cpp
//  SDT
//
//  Created by Leif Wen on 19/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "DS_STRING.hpp"
#include <iomanip>
#include <algorithm>
//------------------------------------------------------------------------------------------//
const STDSTR& Str_LTrimSelf(STDSTR& str){
	STRSIZE	ret;

	ret = str.find_first_not_of(' ');
	if (ret == STDSTR::npos){
		str = "";
	}
	else if (ret > 0){
		str = str.erase(0,ret);
	}
	return(str);
};
//------------------------------------------------------------------------------------------//
const STDSTR& Str_LTrim(STDSTR* _out,const STDSTR& str){
	STRSIZE	ret;
	
	ret = str.find_first_not_of(' ');
	if (ret == STDSTR::npos){
		*_out = "";
	}
	else if (ret > 0){
		*_out = str.substr(ret);
	}
	else{
		*_out = str;
	}
	return(*_out);
};
//------------------------------------------------------------------------------------------//
STDSTR Str_LTrim(const STDSTR& str){
	STDSTR		retStr;
	return(Str_LTrim(&retStr,str));
};
//------------------------------------------------------------------------------------------//
const STDSTR& Str_RTrimSelf(STDSTR& str){
	STRSIZE	ret;
	
	ret = str.find_last_not_of(' ');
	if (ret == STDSTR::npos){
		str = "";
	}
	else if (ret < (str.length() - 1)){
		str.erase(ret + 1);
	}
	return(str);
};
//------------------------------------------------------------------------------------------//
const STDSTR& Str_RTrim(STDSTR* _out,const STDSTR& str){
	STRSIZE	ret;
	
	ret = str.find_last_not_of(' ');
	if (ret == STDSTR::npos){
		*_out = "";
	}
	else if(ret < (str.length() - 1)){
		*_out = str.substr(0,ret + 1);
	}
	else{
		*_out = str;
	}
	return(*_out);
};
//------------------------------------------------------------------------------------------//
STDSTR Str_RTrim(const STDSTR& str){
	STDSTR		retStr;
	return(Str_RTrim(&retStr,str));
};
//------------------------------------------------------------------------------------------//
const STDSTR& Str_TrimSelf(STDSTR& str){
	STRSIZE	s;
	
	s = str.find_last_not_of(' ');
	if (s == STDSTR::npos){
		str = "";
	}
	else{
		if (s < (str.length() - 1))
			str.erase(s + 1);
		s = str.find_first_not_of(' ');
		if (s > 0)
			str.erase(0,s);
	}
	return(str);
};
//------------------------------------------------------------------------------------------//
const STDSTR& Str_Trim(STDSTR* _out,const STDSTR& str){
	STRSIZE	s,e;
	
	s = str.find_first_not_of(' ');
	if (s == STDSTR::npos){
		*_out = "";
	}
	else{
		e = str.find_last_not_of(' ');
		if ((e - s + 1) == str.length()){
			*_out = str;
		}
		else{
			*_out = str.substr(s,e - s + 1);
		}
	}
	return(*_out);
};
//------------------------------------------------------------------------------------------//
STDSTR Str_Trim(const STDSTR& str){
	STDSTR		retStr;
	return(Str_Trim(&retStr,str));
};
//------------------------------------------------------------------------------------------//
const STDSTR& Str_UpperCaseSelf(STDSTR& str){
	std::transform(str.begin(), str.end(), str.begin(), toupper);
	return(str);
};
//------------------------------------------------------------------------------------------//
const STDSTR& Str_UpperCase(STDSTR* _out,const STDSTR& str){
	STRSIZE	i,length;
	uint8	charP;
	
	length = str.length();
	i = 0;
	*_out = "";
	while(i < length){
		charP = str[i];
		if ((charP >= 'a') && (charP <= 'z'))
			charP += ('A' - 'a');
		*_out += charP;
		++ i;
	}
	return(*_out);
};
//------------------------------------------------------------------------------------------//
STDSTR Str_UpperCase(const STDSTR& str){
	STDSTR	retStr;
	return(Str_UpperCase(&retStr,str));
};
//------------------------------------------------------------------------------------------//
const STDSTR& Str_LowerCaseSelf(STDSTR& str){
	std::transform(str.begin(), str.end(), str.begin(), tolower);
	return(str);
};
//------------------------------------------------------------------------------------------//
const STDSTR& Str_LowerCase(STDSTR* _out,const STDSTR& str){
	STRSIZE	i,length;
	uint8	charP;
	
	length = str.length();
	i = 0;
	*_out = "";
	while(i < length){
		charP = str[i];
		if ((charP >= 'A') && (charP <= 'Z'))
			charP += ('a' - 'A');
		*_out += charP;
		++ i;
	}
	return(*_out);
};
//------------------------------------------------------------------------------------------//
STDSTR Str_LowerCase(const STDSTR& str){
	STDSTR	retStr;
	return(Str_LowerCase(&retStr,str));
};
//------------------------------------------------------------------------------------------//
STDSTR Str_Replace(const STDSTR& str,const STDSTR& strS,const STDSTR& strD){
	STDSTR	retStr;
	STRSIZE	ret,head,length;
	
	retStr = "";
	head = 0;
	length = strS.length();
	
	while(head < str.length()){
		ret = str.find(strS,head);
		if (ret == STDSTR::npos){
			retStr += str.substr(head);
			break;
		}
		else{
			retStr += str.substr(head,ret - head);
			retStr += strD;
			head = (ret + length);
		}
	}
	return(retStr);
};
//------------------------------------------------------------------------------------------//
STDSTR Str_SplitSubItem(STDSTR* strIn,uint8 subChar){
	STDSTR	retStr;
	STRSIZE	pos,length;
	uint8*	charP;
	
	length = strIn->length();
	pos = 0;
	retStr = "";
	charP = (uint8*)strIn->c_str();
	while(pos < length){
		++ pos;
		if ((*charP == '\\') && (pos < length)){
			++ pos;
			++ charP;
			if (*charP == subChar){
				retStr += subChar;
			}
			else{
				retStr += '\\';
				retStr += *charP;
			}
		}
		else{
			if (*charP == subChar){
				strIn->erase(0,pos);
				return (retStr);
			}
			retStr += *charP;
		}
		++ charP;
	}
	*strIn = "";
	return(retStr);
};
//------------------------------------------------------------------------------------------//
STDSTR Str_ReadSubItem(STDSTR* strIn,const STDSTR& subChar,bool32 blReturnSubChar){
	STDSTR	retStr = "";
	STRSIZE	pos;
	
	pos = strIn->find(subChar,0);

	if (pos != STDSTR::npos){
		if (blReturnSubChar == G_FALSE){
			retStr = strIn->substr(0,pos);
			strIn->erase(0,pos + subChar.length());
		}
		else{
			retStr = strIn->substr(0,pos + subChar.length());
			strIn->erase(0,pos + subChar.length());
		}
		return(retStr);
	}
	retStr = *strIn;
	*strIn = "";
	return(retStr);
};
//------------------------------------------------------------------------------------------//
STDSTR Str_ReadSubItemR(STDSTR* strIn,const STDSTR& subChar,bool32 blReturnSubChar){
	STDSTR	retStr;
	STRSIZE	pos;
	
	pos = strIn->rfind(subChar);
	
	if (pos != STDSTR::npos){
		if (blReturnSubChar == G_FALSE){
			retStr = strIn->substr(pos + subChar.length());
			strIn->erase(pos);
		}
		else{
			retStr = strIn->substr(pos);
			strIn->erase(pos + 1);
		}
		return(retStr);
	}
	return("");
};
//------------------------------------------------------------------------------------------//
const STDSTR& Str_AddSpaceInFront(STDSTR* strIn,uint32 length){
	if (strIn->length() < length)
		strIn->insert(0,length - strIn->length(),' ');
	return(*strIn);
};
//------------------------------------------------------------------------------------------//
STDSTR Str_FloatToStr(double fZ,int32 blNozero,int32 slen,int32 alen){
	char 	tmpstr[256];
	STDSTR	result;
	STRSIZE	pos1,pos;
	
	memset(tmpstr,'\0',sizeof(tmpstr));
#ifdef CommonDefH_Unix
	snprintf(tmpstr,sizeof(tmpstr),"%*.*lf",slen,alen,fZ);
#endif
#ifdef CommonDefH_VC
	sprintf_s(tmpstr, sizeof(tmpstr), "%*.*lf", slen, alen, fZ);
#endif
	result = tmpstr;
	if (blNozero != 0){
		pos1 = result.find_last_not_of('0');
		pos = result.find_last_of('.');
		if (pos1 != STDSTR::npos){
			if (pos < pos1)
				pos = pos1 + 1;
		}
		if (pos != STDSTR::npos)
			result.erase(pos);
	}
	
	return(result);
};
//------------------------------------------------------------------------------------------//
uint64 Str_HexToDec(const STDSTR& str){
	uint8	charP;
	uint32	i,length;
	uint64	result;
	STDSTR	asciiStr;
	
	//test:B6 53 CC 1C C4 F4 AB E2
	
	length = (uint32)str.length();
	result =0;
	i = 0;
	while(length > 0){
		charP = str[-- length];
		
		if (charP >= '0' && charP <= '9'){
			charP -= '0';
		}
		else if (charP >= 'A' && charP <= 'F'){
			charP -= ('A' - 0x0a);
		}
		else if (charP >= 'a' && charP <= 'f'){
			charP -= ('a' - 0x0a);
		}
		else{
			continue;
		}
		
		result |= ((uint64)charP << i);
		
		i += 4;
	}
	return(result);
};
//------------------------------------------------------------------------------------------//
STDSTR Str_DecToHex(uint64 fZ){
	std::stringstream tStream;
	STDSTR	retStr;
	tStream << std::setiosflags(std::ios::uppercase) << std::hex << std::setw(4) << std::setfill('0') << fZ;
	tStream >> retStr;
	return(retStr);
};
//------------------------------------------------------------------------------------------//
STDSTR Str_DecToHex(const STDSTR& str){
	std::stringstream tStream;
	STDSTR	retStr;
	uint64	num;
	
	tStream << str;
	tStream >> num;
	tStream.clear();
	
	tStream << std::setiosflags(std::ios::uppercase) << std::hex << std::setw(4) << std::setfill('0') << num;
	tStream >> retStr;
	return(retStr);
};
//------------------------------------------------------------------------------------------//
uint64 Str_CharToUint64(const uint8* data,G_ENDIAN endianType){
	uint64 ret;
	ret = 0;
	if (endianType == G_ENDIAN_LITTLE){
		data += 7;
		ret |= (*data--);ret <<= 8;
		ret |= (*data--);ret <<= 8;
		ret |= (*data--);ret <<= 8;
		ret |= (*data--);ret <<= 8;
		ret |= (*data--);ret <<= 8;
		ret |= (*data--);ret <<= 8;
		ret |= (*data--);ret <<= 8;
		ret |= (*data);
	}
	else{
		ret |= (*data++);ret <<= 8;
		ret |= (*data++);ret <<= 8;
		ret |= (*data++);ret <<= 8;
		ret |= (*data++);ret <<= 8;
		ret |= (*data++);ret <<= 8;
		ret |= (*data++);ret <<= 8;
		ret |= (*data++);ret <<= 8;
		ret |= (*data);
	}
	return(ret);
};
//------------------------------------------------------------------------------------------//
uint32 Str_CharToUint32(const uint8* data,G_ENDIAN endianType){
	uint32 ret;
	ret = 0;
	if (endianType == G_ENDIAN_LITTLE){
		data += 3;
		ret |= (*data--);ret <<= 8;
		ret |= (*data--);ret <<= 8;
		ret |= (*data--);ret <<= 8;
		ret |= (*data);
	}
	else{
		ret |= (*data++);ret <<= 8;
		ret |= (*data++);ret <<= 8;
		ret |= (*data++);ret <<= 8;
		ret |= (*data);
	}
	return(ret);
};
//------------------------------------------------------------------------------------------//
uint16 Str_CharToUint16(const uint8* data,G_ENDIAN endianType){
	uint16 ret;
	ret = 0;
	if (endianType == G_ENDIAN_LITTLE){
		data += 1;
		ret |= (*data--);ret <<= 8;
		ret |= (*data);
	}
	else{
		ret |= (*data++);ret <<= 8;
		ret |= (*data);
	}
	return(ret);
};
//------------------------------------------------------------------------------------------//
const uint8* Str_Uint64ToChar(uint8* _out,uint64 data,G_ENDIAN endianType){
	if (endianType == G_ENDIAN_LITTLE){
		*_out++ = (uint8)(data);data >>= 8;
		*_out++ = (uint8)(data);data >>= 8;
		*_out++ = (uint8)(data);data >>= 8;
		*_out++ = (uint8)(data);data >>= 8;
		*_out++ = (uint8)(data);data >>= 8;
		*_out++ = (uint8)(data);data >>= 8;
		*_out++ = (uint8)(data);data >>= 8;
		*_out = (uint8)(data);
	}
	else{
		_out += 7;
		*_out-- = (uint8)(data);data >>= 8;
		*_out-- = (uint8)(data);data >>= 8;
		*_out-- = (uint8)(data);data >>= 8;
		*_out-- = (uint8)(data);data >>= 8;
		*_out-- = (uint8)(data);data >>= 8;
		*_out-- = (uint8)(data);data >>= 8;
		*_out-- = (uint8)(data);data >>= 8;
		*_out = (uint8)(data);
	}
	return(_out);
};
//------------------------------------------------------------------------------------------//
const uint8* Str_Uint32ToChar(uint8* _out,uint32 data,G_ENDIAN endianType){
	if (endianType == G_ENDIAN_LITTLE){
		*_out++ = (uint8)(data);data >>= 8;
		*_out++ = (uint8)(data);data >>= 8;
		*_out++ = (uint8)(data);data >>= 8;
		*_out = (uint8)(data);
	}
	else{
		_out += 3;
		*_out-- = (uint8)(data);data >>= 8;
		*_out-- = (uint8)(data);data >>= 8;
		*_out-- = (uint8)(data);data >>= 8;
		*_out = (uint8)(data);
	}
	return(_out);
};
//------------------------------------------------------------------------------------------//
const uint8* Str_Uint16ToChar(uint8* _out,uint16 data,G_ENDIAN endianType){
	if (endianType == G_ENDIAN_LITTLE){
		*_out++ = (uint8)(data);
		*_out = (uint8)(data >> 8);
	}
	else{
		*_out++ = (uint8)(data >> 8);
		*_out = (uint8)(data);
	}
	return(_out);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
bool32 HToD(uint8& retChar,const uint8*& data){
	if (*data >= '0' && *data <= '9'){
		retChar = *data - '0';
		return G_TRUE;
	}
	else if (*data >= 'A' && *data <= 'F'){
		retChar = *data - 'A' + 0x0a;
		return G_TRUE;
	}
	else if (*data >= 'a' && *data <= 'f'){
		retChar = *data - 'a' + 0x0a;
		return G_TRUE;
	}
	return G_FALSE;
};
//------------------------------------------------------------------------------------------//
template <typename TYPE_INT>
uint32 Escape_xhh(uint8& retChar,const uint8*& data,TYPE_INT& num){
	uint8	ret;
	if (HToD(retChar,data) > 0){
		++ data;
		if (--num > 0){
			ret = retChar;
			if (HToD(retChar,data) > 0){
				retChar |= (ret << 4);
				++data;
				--num;
				return 2;
			}
		}
		return 1;
	}
	return 0;
};
//------------------------------------------------------------------------------------------//
uint32 Str_Escape_xhh(uint8& retChar,const uint8*& data,uint32& num){return(Escape_xhh(retChar,data,num));};
uint32 Str_Escape_xhh(uint8& retChar,const uint8*& data,uint64& num){return(Escape_xhh(retChar,data,num));};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
inline bool32 OToD(uint8& retChar,const uint8*& data){
	if (*data >= '0' && *data <= '7'){
		retChar = *data - '0';
		return G_TRUE;
	}
	return G_FALSE;
};
//------------------------------------------------------------------------------------------//
template <typename TYPE_INT>
inline uint32 Escape_ooo(uint8& retChar,const uint8*& data,TYPE_INT& num){
	uint8	ret;
	if (OToD(retChar,data) > 0){
		++ data;
		if (--num > 0){
			ret = retChar;
			if (OToD(retChar,data) > 0){
				retChar |= (ret << 3);
				++ data;
				if (--num > 0){
					ret = retChar;
					if (OToD(retChar,data) > 0){
						retChar |= (ret << 3);
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
};
//------------------------------------------------------------------------------------------//
template <typename TYPE_INT>
uint32 EscapeToChar(uint8& retChar,const uint8*& data,TYPE_INT& num){
	switch (*data) {
		case 'a':	retChar = '\a';++ data;-- num;return 1;
		case 'b':	retChar = '\b';++ data;-- num;return 1;
		case 'f':	retChar = '\f';++ data;-- num;return 1;
		case 'n':	retChar = '\n';++ data;-- num;return 1;
		case 'r':	retChar = '\r';++ data;-- num;return 1;
		case 't':	retChar = '\t';++ data;-- num;return 1;
		case 'v':	retChar = '\v';++ data;-- num;return 1;
		case '\\':	retChar = '\\';++ data;-- num;return 1;
		case '\'':	retChar = '\'';++ data;-- num;return 1;
		case '\"':	retChar = '\"';++ data;-- num;return 1;
		case '\?':	retChar = '\?';++ data;-- num;return 1;
		case '\0':	retChar = '\0';++ data;-- num;return 1;
		case 'x':
			retChar = 'x';
			++ data;
			if (--num > 0)
				return(Escape_xhh(retChar,data,num) + 1);
			return 1;
		default:
			return(Escape_ooo(retChar,data,num));
	}
	return 0;
};
//------------------------------------------------------------------------------------------//
uint32	Str_EscapeToChar	(uint8& retChar,const uint8*& data,uint32& num){return(EscapeToChar(retChar,data,num));};
uint32	Str_EscapeToChar	(uint8& retChar,const uint8*& data,uint64& num){return(EscapeToChar(retChar,data,num));};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
void Str_CharToHex(STDSTR* _out,uint8 data){
	uint8	charData;
	
	charData = (data >> 4) & (0x0f);
	if (charData < 0x0a){
		charData += '0';
	}
	else{
		charData += ('A' - 0x0a);
	}
	
	*_out += charData;
	
	charData = data & (0x0f);
	if (charData < 0x0a){
		charData += '0';
	}
	else{
		charData += ('A' - 0x0a);
	}
	
	*_out += charData;
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
const STDSTR& Str_EscapeToASCII(STDSTR* _out,const uint8 data){
	switch (data) {
		case '\\':
			*_out += "\\\\";
			break;
		case '\a':
			*_out += "\\a";
			break;
		case '\b':
			*_out += "\\b";
			break;
		case '\f':
			*_out += "\\f";
			break;
		case '\n':
			*_out += "\\n";
			break;
		case '\r':
			*_out += "\\r";
			break;
		case '\t':
			*_out += "\\t";
			break;
		case '\v':
			*_out += "\\v";
			break;
		case '\0':
			*_out += "\\0";
			break;
		default:
			*_out += data;
			break;
	}
	return(*_out);
};
//------------------------------------------------------------------------------------------//
const STDSTR& Str_EscapeToASCII(STDSTR* _out,const uint8* data,uint64 num){
	while(num-- > 0){
		Str_EscapeToASCII(_out,*data);
		++ data;
	};
	return(*_out);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
const STDSTR& Str_CharToStrEscape(STDSTR* _out,const uint8* data,uint64 num){
	uint64	count;
	uint8	chardata;
	const uint8	*p;
	
	p = data;
	count = 0;
	
	while(num > 0){
		if (*p == '\\'){
			_out->append((char*)data,count);
			count = 0;
			++ p;
			if (--num > 0){
				if (Str_EscapeToChar(chardata,p,num) > 0)
					*_out += chardata;
			}
			data = p;
		}
		else{
			++ p;
			-- num;
			++ count;
		}
	}
	_out->append((char*)data,count);
	
	return(*_out);
};
//------------------------------------------------------------------------------------------//
const STDSTR& Str_CharToStr(STDSTR* _out,const uint8* data,uint64 num,G_HA blHA,G_SPACE blSpace,G_ESCAPE blEscape){
	uint8	chardata;

	if (blHA == G_HEX){
		if (blEscape == G_ESCAPE_OFF){
			while(num-- > 0){
				Str_CharToHex(_out,*data);
				++ data;
				if (blSpace == G_SPACE_ON)
					*_out += ' ';
			}
		}
		else{
			while(num > 0){
				if (*data == '\\'){
					++ data;
					if (--num > 0){
						if (Str_EscapeToChar(chardata,data,num) > 0){
							Str_CharToHex(_out,chardata);
							if (blSpace == G_SPACE_ON)
								*_out += ' ';
						}
					}
				}
				else{
					Str_CharToHex(_out,*data);
					if (blSpace == G_SPACE_ON)
						*_out += ' ';
					++ data;
					-- num;
				}
			}
		}
	}
	else if (blEscape == G_ESCAPE_ON){
		Str_CharToStrEscape(_out,data,num);
	}
	else{
		_out->append((char*)data,num);
	}
	return(*_out);
};
//------------------------------------------------------------------------------------------//
const STDSTR &Str_HEXToStr(STDSTR* _out,const uint8* data,uint64 num,G_HA blHA,G_SPACE blSpace){
	uint8	chardata;

	while(num > 0){
		if (Str_Escape_xhh(chardata,data,num) > 0){
			if (blHA == G_HEX){
				Str_CharToHex(_out,chardata);
				if (blSpace == G_SPACE_ON)
					*_out += ' ';
			}
			else{
				*_out += chardata;
			}
		}
		else{
			++ data;
			-- num;
		}
	}
	return(*_out);
};
//------------------------------------------------------------------------------------------//











//------------------------------------------------------------------------------------------//
namespace STRFIND {	
	static	void	FormatString		(STDSTR*  retStr,const STDSTR& strIn);
	static	STDSTR	SplitLetter			(STRSIZE* offset,const STDSTR& strSearch);
	static	bool32	SplitSymbol			(const STDSTR&	strSearch
										 ,STRSIZE*		offset
										 ,STRSIZE*		symbolLength
										 ,bool32*		blOnlyQuestionMark);
	static	bool32	SearchSubString		(STDSTR* 		result
										 ,const STDSTR& strContent
										 ,STRSIZE		strContentPostion
										 ,const STDSTR& strSearch
										 ,STRSIZE		strSearchPostion
										 ,STRSIZE*		returnContentPostion);
};
//------------------------------------------------------------------------------------------//
bool32 Str_Find(STDSTR* _out,const STDSTR& strContent,const STDSTR& searchString){
	//strContent is ASCII
	//strSearch is ASCII,escape \0xhh,\0Xhh,\a,\b,\f,\n,\r,\t,\v,\\,\',\",\0,\/,\*,\??
	STRSIZE	strContentPostion,returnContentPostion;
	STDSTR	strD1,searchStr;
	STDSTR	result;
	
	STRFIND::FormatString(&searchStr,searchString);
	strContentPostion = 0;
	
	do{
		result = "";
		if (STRFIND::SearchSubString(&result,strContent,strContentPostion,searchStr,0,&returnContentPostion))
			break;
		strContentPostion += 1;
	}while(strContentPostion < strContent.length());
	if (_out != nullptr)
		*_out = result;
	return(result.length() > 0);
};
//------------------------------------------------------------------------------------------//
static void STRFIND::FormatString(STDSTR* retStr,const STDSTR& strIn){
	//formate to HEX without space
	//escape *,?
	STDSTR	strData,strResult;
	STRSIZE	i,length;
	uint8	charData;
	
	length = strIn.length();
	i = 0;
	*retStr = "";
	strData = "";
	while(i < length){
		charData = strIn[i];
		++ i;
		if ((charData == '*') || (charData == '?')){
			*retStr += Str_ASCIIToHEX(strData,G_ESCAPE_ON);
			*retStr += charData;
			strData = "";
		}
		else{
			if ((charData == '\\') && (i < length)){	//   (\*)(\?)
				charData = strIn[i];
				++ i;
				if ((charData != '*') && (charData != '?'))
					strData += '\\';
			}
			strData += charData;
		}
	}
	*retStr += Str_ASCIIToHEX(strData,G_ESCAPE_ON);
};
//------------------------------------------------------------------------------------------//
static STDSTR STRFIND::SplitLetter(STRSIZE* offset,const STDSTR& searchStr){
	STDSTR	subStr;
	uint8	charData;
	STRSIZE	i,length;
	
	length = searchStr.length();
	subStr = "";
	i = *offset;
	do{
		charData = searchStr[i];
		if ((charData == '*') || (charData == '?'))
			break;
		subStr += charData;
	}while(++i < length);
	
	if (subStr.length() > 0)
		*offset = i;
	return(Str_HEXToASCII(subStr));
};
//------------------------------------------------------------------------------------------//
static bool32 STRFIND::SplitSymbol(const STDSTR& strSearch,STRSIZE* offset,STRSIZE* symbolLength,bool32* blOnlyQuestionMark){
	//3 type symbol 1-->*,2-->*?,3--?
	STDSTR	strD1;
	uint8	charData;
	STRSIZE	length;
	bool32	blSymbol;
	int32	symbolType;
	
	length = strSearch.length();
	
	strD1 = "";
	blSymbol = G_FALSE;
	*symbolLength = 0;
	*blOnlyQuestionMark = G_TRUE;//if ?
	symbolType = 0;
	do{
		charData = strSearch[*offset];
		if (charData == '*'){
			if (symbolType == 0)
				symbolType = 1;
			if (symbolType == 1){
				blSymbol = G_TRUE;
				*blOnlyQuestionMark = G_FALSE;
				continue;
			}
		}else if (charData == '?'){
			if (symbolType == 0)
				symbolType = 3;
			blSymbol = G_TRUE;
			*symbolLength += 1;
			continue;
		}
		break;
	}while(++(*offset) < length);
	return(blSymbol);
};
//------------------------------------------------------------------------------------------//
static bool32 STRFIND::SearchSubString(STDSTR*			result
									   ,const STDSTR&	strContent
									   ,STRSIZE			strContentPostion
									   ,const STDSTR&	strSearch
									   ,STRSIZE			searchStrOffset
									   ,STRSIZE*		returnContentPostion){
	
	STDSTR		strD1,strReslut,strD2,strSubRet;
	STRSIZE		strContentNextPostion,reContentPostion;
	STRSIZE		symbolLength,subSearchLength;
	bool32		blOnlyQuestionMark;
	
	strReslut = "";
	strSubRet = *result;
	strD1 = SplitLetter(&searchStrOffset,strSearch);
	
	strContentPostion = strContent.find(strD1,strContentPostion);
	if (strContentPostion == STDSTR::npos)
		return G_FALSE;
	
	strContentNextPostion = (strContentPostion + strD1.length());
	
	if (SplitSymbol(strSearch,&searchStrOffset,&symbolLength,&blOnlyQuestionMark)){
		if (searchStrOffset >= strSearch.length()){
			if (blOnlyQuestionMark){
				if ((strContent.length() - strContentNextPostion) != symbolLength)
					return G_FALSE;
			}
			if ((strContent.length() - strContentNextPostion) < symbolLength)
				return G_FALSE;
			
			strReslut = strContent.substr(strContentNextPostion);
			if ((symbolLength > 0) && (blOnlyQuestionMark == G_FALSE)){//*??
				strD2 = strReslut.substr(strReslut.length() - symbolLength);
				strReslut.erase(strReslut.length() - symbolLength);
				if (strReslut.length() == 0){
					strReslut = Str_ASCIIToHEX(strD2, G_ESCAPE_OFF);
				}else if (strD2.length() > 0){
					strReslut = Str_ASCIIToHEX(strReslut,G_ESCAPE_OFF) + "," + Str_ASCIIToHEX(strD2,G_ESCAPE_OFF);
				}
			}
			else{
				strReslut = Str_ASCIIToHEX(strReslut,G_ESCAPE_OFF);
			}
			if (result->length() == 0){
				*result = strReslut;
			}
			else if (strReslut.length() > 0){
				*result = strReslut + "," + *result;
			}
			*returnContentPostion = strContentPostion;
			return G_TRUE;
		}
		subSearchLength = 0;
		do{
			*result = strSubRet;
			if (SearchSubString(result,strContent,strContentNextPostion + symbolLength + subSearchLength,strSearch,searchStrOffset,&reContentPostion) != 0){
				if (blOnlyQuestionMark){
					if ((reContentPostion - strContentNextPostion) != symbolLength)
						return G_FALSE;
				}
				strReslut = strContent.substr(strContentNextPostion,reContentPostion - strContentNextPostion);
				if ((symbolLength > 0) && (blOnlyQuestionMark == G_FALSE)){
					strD2 = strReslut.substr(strReslut.length() - symbolLength);
					strReslut.erase(strReslut.length() - symbolLength);
					if (strReslut.length() == 0){
						strReslut =  Str_ASCIIToHEX(strD2,G_ESCAPE_OFF);
					}else if (strD2.length() > 0){
						strReslut = Str_ASCIIToHEX(strReslut,G_ESCAPE_OFF) + "," + Str_ASCIIToHEX(strD2,G_ESCAPE_OFF);
					}
				}
				else{
					strReslut = Str_ASCIIToHEX(strReslut,G_ESCAPE_OFF);
				}
				if (result->length() == 0){
					*result = strReslut;
				}
				else if (strReslut.length() > 0){
					*result = strReslut + "," + *result;
				}
				*returnContentPostion = strContentPostion;
				return G_TRUE;
			}
			subSearchLength += 1;
		}while(strContentNextPostion + symbolLength + subSearchLength < strContent.length());
		return G_FALSE;
	}
	*returnContentPostion = strContentPostion;
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//









//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_VC
//------------------------------------------------------------------------------------------//
std::wstring Str_ANSIToUnicode(const STDSTR &strIn){
	int32			unicodeLen;
	wchar_t			*pUnicode;
	std::wstring	ret;
	
	unicodeLen = MultiByteToWideChar(CP_ACP,0,strIn.c_str(),-1,nullptr,0);
	
	pUnicode = new  wchar_t[unicodeLen + 1];
	memset(pUnicode,0,(unicodeLen + 1)*sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP,0,strIn.c_str(),-1,(LPWSTR)pUnicode,unicodeLen);
	
	ret = (wchar_t*)pUnicode;
	delete  []pUnicode;
	return  ret;
};
//------------------------------------------------------------------------------------------//
STDSTR Str_UnicodeToANSI(const std::wstring &strIn){
	char*		pElementText;
	int32		iTextLen;
	STDSTR		strText;
	
	iTextLen = WideCharToMultiByte(CP_ACP,0,strIn.c_str(),-1,nullptr,0,nullptr,nullptr);
	pElementText = new char[iTextLen + 1];
	memset((void*)pElementText,0,sizeof(char) * (iTextLen + 1));
	WideCharToMultiByte( CP_ACP,0,strIn.c_str(),-1,pElementText,iTextLen,nullptr,nullptr);
	
	strText = pElementText;
	delete []pElementText;
	return(strText);
};
//------------------------------------------------------------------------------------------//
std::wstring Str_UTF8ToUnicode(const STDSTR &strIn){
	int32			unicodeLen;
	wchar_t			*pUnicode;
	std::wstring	ret;
	
	unicodeLen = MultiByteToWideChar(CP_UTF8,0,strIn.c_str(),-1,nullptr,0);
	
	pUnicode = new  wchar_t[unicodeLen+1];
	memset(pUnicode,0,(unicodeLen + 1)*sizeof(wchar_t));
	MultiByteToWideChar( CP_UTF8,0,strIn.c_str(),-1,(LPWSTR)pUnicode,unicodeLen);
	
	ret = (wchar_t*)pUnicode;
	delete  []pUnicode;
	return(ret);
};
//------------------------------------------------------------------------------------------//
STDSTR Str_UnicodeToUTF8(const std::wstring &strIn){
	char*		pElementText;
	int32		iTextLen;
	STDSTR		strText;
	iTextLen = WideCharToMultiByte( CP_UTF8,0,strIn.c_str(),-1,nullptr,0,nullptr,nullptr);
	pElementText = new char[iTextLen + 1];
	memset((void*)pElementText,0,sizeof(char) * (iTextLen + 1));
	WideCharToMultiByte(CP_UTF8,0,strIn.c_str(),-1,pElementText,iTextLen,nullptr,nullptr);
	
	strText = pElementText;
	delete []pElementText;
	return(strText);
};
//------------------------------------------------------------------------------------------//
#endif
//------------------------------------------------------------------------------------------//
