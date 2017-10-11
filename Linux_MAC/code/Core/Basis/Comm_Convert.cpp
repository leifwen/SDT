
/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: Comm_Convert.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2012.02.09
 * @2013.12.11 : update Thread to make more safe.
 * @2013.06.30 : change to Liunx style
 add init method
 use int32 to replace bool
 use int32 to replace uint32
 @2014.05.24 : form PublicSYS.h
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "Comm_Convert.h"
#include "Comm_FIFO.h"
#include <algorithm>
//------------------------------------------------------------------------------------------//
STDSTR Str_FloatToString(double fZ,int32 blNozero,int32 slen,int32 alen){
	char tmpstr[256];
	STDSTR	result;
	STDSTR::size_type		pos1,pos;
	
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
		result = result.substr(0,pos);
	}
	
	return(result);
}
//------------------------------------------------------------------------------------------//
uint64 Str_HexToDec(const STDSTR &str){
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
}
//------------------------------------------------------------------------------------------//
STDSTR Str_DecToHex(uint64 fZ){
	char tmpstr[256];
	STDSTR	result;

	memset(tmpstr,'\0',sizeof(tmpstr));
#ifdef CommonDefH_Unix
	snprintf(tmpstr,sizeof(tmpstr),"%llX",fZ);
#endif
#ifdef CommonDefH_VC
	sprintf_s(tmpstr,sizeof(tmpstr),"%llX",fZ);
#endif
	result = tmpstr;
	return(result);
}
//------------------------------------------------------------------------------------------//
STDSTR Str_DecToHex(const STDSTR &str){
	char		tmpstr[256];
	STDSTR	result;
	uint64		ret64;
	
	ret64 = strtoull(str.c_str(),nullptr,10);
	
	memset(tmpstr,'\0',sizeof(tmpstr));
#ifdef CommonDefH_Unix
	snprintf(tmpstr,sizeof(tmpstr),"%llX",ret64);
#endif
#ifdef CommonDefH_VC
	sprintf_s(tmpstr,sizeof(tmpstr),"%llX",ret64);
#endif
	result = tmpstr;
	return(result);
}
//------------------------------------------------------------------------------------------//
uint64 Str_CharToUint64(const uint8 *retBuf,G_ENDIAN endianType){
	uint64 ret;
	ret = 0;
	if (endianType == G_ENDIAN_LITTLE){
		retBuf += 7;
		ret |= (*retBuf--);ret <<= 8;
		ret |= (*retBuf--);ret <<= 8;
		ret |= (*retBuf--);ret <<= 8;
		ret |= (*retBuf--);ret <<= 8;
		ret |= (*retBuf--);ret <<= 8;
		ret |= (*retBuf--);ret <<= 8;
		ret |= (*retBuf--);ret <<= 8;
		ret |= (*retBuf);
	}
	else{
		ret |= (*retBuf++);ret <<= 8;
		ret |= (*retBuf++);ret <<= 8;
		ret |= (*retBuf++);ret <<= 8;
		ret |= (*retBuf++);ret <<= 8;
		ret |= (*retBuf++);ret <<= 8;
		ret |= (*retBuf++);ret <<= 8;
		ret |= (*retBuf++);ret <<= 8;
		ret |= (*retBuf);
	}
	return(ret);
}
//------------------------------------------------------------------------------------------//
uint32 Str_CharToUint32(const uint8 *retBuf,G_ENDIAN endianType){
	uint32 ret;
	ret = 0;
	if (endianType == G_ENDIAN_LITTLE){
		retBuf += 3;
		ret |= (*retBuf--);ret <<= 8;
		ret |= (*retBuf--);ret <<= 8;
		ret |= (*retBuf--);ret <<= 8;
		ret |= (*retBuf);
	}
	else{
		ret |= (*retBuf++);ret <<= 8;
		ret |= (*retBuf++);ret <<= 8;
		ret |= (*retBuf++);ret <<= 8;
		ret |= (*retBuf);
	}
	return(ret);
}
//------------------------------------------------------------------------------------------//
uint16 Str_CharToUint16(const uint8 *retBuf,G_ENDIAN endianType){
	uint16 ret;
	ret = 0;
	if (endianType == G_ENDIAN_LITTLE){
		retBuf += 1;
		ret |= (*retBuf--);ret <<= 8;
		ret |= (*retBuf);
	}
	else{
		ret |= (*retBuf++);ret <<= 8;
		ret |= (*retBuf);
	}
	return(ret);
}
//------------------------------------------------------------------------------------------//
const uint8 *Str_Uint64ToChar(uint8 *retBuf,uint64 data,G_ENDIAN endianType){
	if (endianType == G_ENDIAN_LITTLE){
		*retBuf++ = (uint8)(data);data >>= 8;
		*retBuf++ = (uint8)(data);data >>= 8;
		*retBuf++ = (uint8)(data);data >>= 8;
		*retBuf++ = (uint8)(data);data >>= 8;
		*retBuf++ = (uint8)(data);data >>= 8;
		*retBuf++ = (uint8)(data);data >>= 8;
		*retBuf++ = (uint8)(data);data >>= 8;
		*retBuf = (uint8)(data);
	}
	else{
		retBuf += 7;
		*retBuf-- = (uint8)(data);data >>= 8;
		*retBuf-- = (uint8)(data);data >>= 8;
		*retBuf-- = (uint8)(data);data >>= 8;
		*retBuf-- = (uint8)(data);data >>= 8;
		*retBuf-- = (uint8)(data);data >>= 8;
		*retBuf-- = (uint8)(data);data >>= 8;
		*retBuf-- = (uint8)(data);data >>= 8;
		*retBuf = (uint8)(data);
	}
	return(retBuf);
}
//------------------------------------------------------------------------------------------//
const uint8 *Str_Uint32ToChar(uint8 *retBuf,uint32 data,G_ENDIAN endianType){
	if (endianType == G_ENDIAN_LITTLE){
		*retBuf++ = (uint8)(data);data >>= 8;
		*retBuf++ = (uint8)(data);data >>= 8;
		*retBuf++ = (uint8)(data);data >>= 8;
		*retBuf = (uint8)(data);
	}
	else{
		retBuf += 3;
		*retBuf-- = (uint8)(data);data >>= 8;
		*retBuf-- = (uint8)(data);data >>= 8;
		*retBuf-- = (uint8)(data);data >>= 8;
		*retBuf = (uint8)(data);
	}
	return(retBuf);
}
//------------------------------------------------------------------------------------------//
const uint8 *Str_Uint16ToChar(uint8 *retBuf,uint16 data,G_ENDIAN endianType){
	if (endianType == G_ENDIAN_LITTLE){
		*retBuf++ = (uint8)(data);
		*retBuf = (uint8)(data >> 8);
	}
	else{
		*retBuf++ = (uint8)(data >> 8);
		*retBuf = (uint8)(data);
	}
	return(retBuf);
}
//------------------------------------------------------------------------------------------//
const STDSTR &Str_LTrimSelf(STDSTR &str){
	STDSTR::size_type		ret;
	
	ret = str.find_first_not_of(' ');
	if (ret == STDSTR::npos){
		str = "";
	}
	else if (ret > 0){
		str = str.erase(0,ret);
	}
	return(str);
}
//------------------------------------------------------------------------------------------//
STDSTR Str_LTrim(const STDSTR &str){
	STDSTR::size_type		ret;
	STDSTR		returnStr;
	
	ret = str.find_first_not_of(' ');
	if (ret == STDSTR::npos){
		returnStr = "";
	}
	else if (ret > 0){
		returnStr = str.substr(ret);
	}
	else{
		returnStr = str;
	}
	return(returnStr);
}
//------------------------------------------------------------------------------------------//
const STDSTR &Str_LTrim(STDSTR *returnStr,const STDSTR &str){
	STDSTR::size_type		ret;
	
	ret = str.find_first_not_of(' ');
	if (ret == STDSTR::npos){
		*returnStr = "";
	}
	else if (ret > 0){
		*returnStr = str.substr(ret);
	}
	else{
		*returnStr = str;
	}
	return(*returnStr);
}
//------------------------------------------------------------------------------------------//
const STDSTR &Str_RTrimSelf(STDSTR &str){
	STDSTR::size_type		ret;
	
	ret = str.find_last_not_of(' ');
	if (ret == STDSTR::npos){
		str = "";
	}
	else if (ret < (str.length() - 1)){
		str.erase(ret + 1);
	}
	return(str);
}
//------------------------------------------------------------------------------------------//
STDSTR Str_RTrim(const STDSTR &str){
	STDSTR::size_type		ret;
	STDSTR returnStr;
	
	ret = str.find_last_not_of(' ');
	if (ret == STDSTR::npos){
		returnStr = "";
	}
	else if(ret < (str.length() - 1)){
		returnStr = str.substr(0,ret + 1);
	}
	else{
		returnStr = str;
	}
	return(returnStr);
}
//------------------------------------------------------------------------------------------//
const STDSTR &Str_RTrim(STDSTR *returnStr,const STDSTR &str){
	STDSTR::size_type		ret;
	
	ret = str.find_last_not_of(' ');
	if (ret == STDSTR::npos){
		*returnStr = "";
	}
	else if(ret < (str.length() - 1)){
		*returnStr = str.substr(0,ret + 1);
	}
	else{
		*returnStr = str;
	}
	return(*returnStr);
}
//------------------------------------------------------------------------------------------//
const STDSTR &Str_TrimSelf(STDSTR &str){
	STDSTR::size_type		s;
	
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
}
//------------------------------------------------------------------------------------------//
STDSTR Str_Trim(const STDSTR &str){
	STDSTR::size_type		s,e;
	STDSTR returnStr;
	s = str.find_first_not_of(' ');
	if (s == STDSTR::npos){
		returnStr = "";
	}
	else{
		e = str.find_last_not_of(' ');
		if ((e - s + 1) == str.length()){
			returnStr = str;
		}
		else{
			returnStr = str.substr(s,e - s + 1);
		}
	}
	return(returnStr);
}
//------------------------------------------------------------------------------------------//
const STDSTR &Str_Trim(STDSTR *returnStr,const STDSTR &str){
	STDSTR::size_type		s,e;
	
	s = str.find_first_not_of(' ');
	if (s == STDSTR::npos){
		*returnStr = "";
	}
	else{
		e = str.find_last_not_of(' ');
		if ((e - s + 1) == str.length()){
			*returnStr = str;
		}
		else{
			*returnStr = str.substr(s,e - s + 1);
		}
	}
	return(*returnStr);
}
//------------------------------------------------------------------------------------------//
const STDSTR &Str_UpperCaseSelf(STDSTR &str){
 	std::transform(str.begin(), str.end(), str.begin(), toupper);
	return(str);
}
//------------------------------------------------------------------------------------------//
STDSTR	Str_UpperCase(const STDSTR &str){
	STDSTR::size_type		i,length;
	STDSTR	strResult;
	uint8		charP;
	
	length = str.length();
	i = 0;
	
	strResult = "";
	while(i < length){
		charP = str[i];
		if ((charP >= 'a') && (charP <= 'z'))
			charP += ('A' - 'a');
		strResult += charP;
		++ i;
	}
	return(strResult);
}
//------------------------------------------------------------------------------------------//
const STDSTR& Str_UpperCase(STDSTR *returnStr,const STDSTR &str){
	STDSTR::size_type		i,length;
	uint8		charP;
	
	length = str.length();
	i = 0;
	*returnStr = "";
	while(i < length){
		charP = str[i];
		if ((charP >= 'a') && (charP <= 'z'))
			charP += ('A' - 'a');
		*returnStr += charP;
		++ i;
	}
	return(*returnStr);
}
//------------------------------------------------------------------------------------------//
const STDSTR &Str_LowerCaseSelf(STDSTR &str){
	std::transform(str.begin(), str.end(), str.begin(), tolower);
	return(str);
}
//------------------------------------------------------------------------------------------//
STDSTR	Str_LowerCase(const STDSTR &str){
	STDSTR::size_type		i,length;
	STDSTR	strResult;
	uint8		charP;
	
	length = str.length();
	i = 0;
	strResult = "";
	while(i < length){
		charP = str[i];
		if ((charP >= 'A') && (charP <= 'Z'))
			charP += ('a' - 'A');
		strResult += charP;
		++ i;
	}
	return(strResult);
}
//------------------------------------------------------------------------------------------//
const STDSTR& Str_LowerCase(STDSTR *returnStr,const STDSTR &str){
	STDSTR::size_type		i,length;
	uint8		charP;
	
	length = str.length();
	i = 0;
	*returnStr = "";
	while(i < length){
		charP = str[i];
		if ((charP >= 'A') && (charP <= 'Z'))
			charP += ('a' - 'A');
		*returnStr += charP;
		++ i;
	}
	return(*returnStr);
}
//------------------------------------------------------------------------------------------//
STDSTR Str_Replace(const STDSTR &str,const STDSTR &strS,const STDSTR &strD){
	STDSTR		stringResult;
	STDSTR::size_type		ret,head,length;
	
	stringResult = "";
	head = 0;
	length = strS.length();
	
	while(head < str.length()){
		ret = str.find(strS,head);
		if (ret == STDSTR::npos){
			stringResult += str.substr(head);
			break;
		}
		else{
			stringResult += str.substr(head,ret - head);
			stringResult += strD;
			head = (ret + length);
		}
	}
	return(stringResult);
}
//------------------------------------------------------------------------------------------//
STDSTR Str_SplitSubItem(STDSTR *strIn,uint8 subChar){
	STDSTR				strResult;
	STDSTR::size_type	pos,length;
	uint8*					charP;
	
	length = strIn->length();
	pos = 0;
	strResult = "";
	charP = (uint8*)strIn->c_str();
	while(pos < length){
		++ pos;
		if ((*charP == '\\') && (pos < length)){
			++ pos;
			++ charP;
			if (*charP == subChar){
				strResult += subChar;
			}
			else{
				strResult += '\\';
				strResult += *charP;
			}
		}
		else{
			if (*charP == subChar){
				*strIn = strIn->substr(pos);
				return (strResult);
			}
			strResult += *charP;
		}
		++ charP;
	}
	*strIn = "";
	return(strResult);
}
//------------------------------------------------------------------------------------------//
STDSTR Str_ReadSubItem(STDSTR *strIn,const STDSTR &subChar,int32 blReturnSubChar){
	STDSTR	strResult;
	STDSTR::size_type		pos;
	
	pos = strIn->find(subChar,0);
	
	if (pos != STDSTR::npos){
		if (blReturnSubChar == 0){
			strResult = strIn->substr(0,pos);
			*strIn = strIn->substr(pos + subChar.length());
		}
		else{
			strResult = strIn->substr(0,pos + subChar.length());
			*strIn = strIn->substr(pos + subChar.length());
		}
		return(strResult);
	}
	strResult = *strIn;
	*strIn = "";
	return(strResult);
}
//------------------------------------------------------------------------------------------//
STDSTR Str_ReadSubItemR(STDSTR *strIn,const STDSTR &subChar,int32 blReturnSubChar){
	STDSTR	strResult;
	STDSTR::size_type		pos;
	
	pos = strIn->rfind(subChar);
	
	if (pos != STDSTR::npos){
		if (blReturnSubChar == 0){
			strResult = strIn->substr(pos + subChar.length());
			*strIn = strIn->substr(0,strIn->length() - strResult.length() - subChar.length());
		}
		else{
			strResult = strIn->substr(pos);
			*strIn = strIn->substr(0,strIn->length() - strResult.length());
		}
		return(strResult);
	}
	return("");
}
//------------------------------------------------------------------------------------------//
const STDSTR& AddSpaceInFront(STDSTR *strIn,uint32 length){
	if (strIn->length() < length)
		strIn->insert(0,length - strIn->length(),' ');
	return(*strIn);
}
//------------------------------------------------------------------------------------------//
const STDSTR &Str_UnTransferChar(STDSTR *retStr,const uint8 *buf,uint64 num){
	while(num-- > 0){
		FIFO8::UnEscapeToStr(retStr,*buf);
		++ buf;
	};
	return(*retStr);
}
//------------------------------------------------------------------------------------------//
const STDSTR &Str_CharToStrInEscape(STDSTR *retStr,const uint8 *buf,uint64 num){
	uint64	count;
	uint8	chardata;
	const uint8	*p;

	p = buf;
	count = 0;
	
	while(num > 0){
		if (*p == '\\'){
			retStr->append((char*)buf,count);
			count = 0;
			++p;
			if (--num > 0){
				if (FIFO8::EscapeToChar(&chardata,p,num) > 0)
					*retStr += chardata;
			}
			buf = p;
		}
		else{
			++p;
			--num;
			++count;
		}
	}
	retStr->append((char*)buf,count);

	return(*retStr);
}
//------------------------------------------------------------------------------------------//
const STDSTR &Str_CharToStr(STDSTR *retStr,const uint8 *buf,uint64 num,G_HA blHA,G_ESCAPE blEscape,G_SPACE blSpace,G_APPEND blAppend){
	uint8	chardata;
	if (blAppend == G_APPEND_OFF)
		*retStr = "";
	
	if (blHA == G_HEX){
		if (blEscape == G_ESCAPE_OFF){
			while(num-- > 0){
				FIFO8::CharToHex(retStr,*buf);
				++buf;
				if (blSpace == G_SPACE_ON)
					*retStr += ' ';
			}
		}
		else{
			while(num > 0){
				if (*buf == '\\'){
					++buf;
					if (--num > 0){
						if (FIFO8::EscapeToChar(&chardata,buf,num) > 0){
							FIFO8::CharToHex(retStr,chardata);
							if (blSpace == G_SPACE_ON)
								*retStr += ' ';
						}
					}
				}
				else{
					FIFO8::CharToHex(retStr,*buf);
					if (blSpace == G_SPACE_ON)
						*retStr += ' ';
					++buf;
					--num;
				}
			}
		}
	}
	else if (blEscape == G_ESCAPE_ON){
		Str_CharToStrInEscape(retStr,buf,num);
	}
	else{
		retStr->append((char*)buf,num);
	}
	return(*retStr);
}
//------------------------------------------------------------------------------------------//
const STDSTR &Str_HEXTo(STDSTR *retStr,const uint8 *buf,uint64 num,G_HA blHA,G_SPACE blSpace,G_APPEND blAppend){
	uint8	*dataT,chardata;
	if (blAppend == G_APPEND_OFF)
		*retStr = "";
	
	dataT = &chardata;
	

	while(num > 0){
		if (FIFO8::Escape_xhh(dataT,buf,num) > 0){
			if (blHA == G_HEX){
				FIFO8::CharToHex(retStr,chardata);
				if (blSpace == G_SPACE_ON)
					*retStr += ' ';
			}
			else{
				*retStr += chardata;
			}
		}
		else{
			++buf;
			--num;
		}
	}
	return(*retStr);
}
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
}
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
}
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
}
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
}
//------------------------------------------------------------------------------------------//
#endif
//------------------------------------------------------------------------------------------//

