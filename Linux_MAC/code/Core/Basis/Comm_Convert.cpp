
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
std::string Str_FloatToString(double fZ,int32 blNozero,int32 slen,int32 alen){
	char tmpstr[256];
	std::string	result;
	std::string::size_type		pos1,pos;
	
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
		if (pos1 != std::string::npos){
			if (pos < pos1)
				pos = pos1 + 1;
		}
		result = result.substr(0,pos);
	}
	
	return(result);
}
//------------------------------------------------------------------------------------------//
std::string Str_IntToString(int32 fZ){
	char tmpstr[256];
	std::string	result;
	
	memset(tmpstr,'\0',sizeof(tmpstr));
#ifdef CommonDefH_Unix
	snprintf(tmpstr,sizeof(tmpstr),"%d",fZ);
#endif
#ifdef CommonDefH_VC
	sprintf_s(tmpstr, sizeof(tmpstr), "%d", fZ);
#endif
	result = tmpstr;
	return(result);
}
//------------------------------------------------------------------------------------------//
std::string Str_UIntToString(uint32 fZ){
	char tmpstr[256];
	std::string	result;
	
	memset(tmpstr,'\0',sizeof(tmpstr));
#ifdef CommonDefH_Unix
	snprintf(tmpstr,sizeof(tmpstr),"%u",fZ);
#endif
#ifdef CommonDefH_VC
	sprintf_s(tmpstr,sizeof(tmpstr),"%u",fZ);
#endif
	result = tmpstr;
	return(result);
}
//------------------------------------------------------------------------------------------//
std::string Str_UInt64ToString(uint64 fZ){
	char tmpstr[256];
	std::string	result;
	
	memset(tmpstr,'\0',sizeof(tmpstr));
#ifdef CommonDefH_Unix
	snprintf(tmpstr,sizeof(tmpstr),"%llu",fZ);
#endif
#ifdef CommonDefH_VC
	sprintf_s(tmpstr,sizeof(tmpstr),"%llu",fZ);
#endif
	result = tmpstr;
	return(result);
}
//------------------------------------------------------------------------------------------//
std::string Str_Int64ToString(int64 fZ){
	char tmpstr[256];
	std::string	result;
	
	memset(tmpstr,'\0',sizeof(tmpstr));
#ifdef CommonDefH_Unix
	snprintf(tmpstr,sizeof(tmpstr),"%lld",fZ);
#endif
#ifdef CommonDefH_VC
	sprintf_s(tmpstr,sizeof(tmpstr),"%lld",fZ);
#endif
	result = tmpstr;
	return(result);
}
//------------------------------------------------------------------------------------------//
uint64 Str_HexToDec(const std::string &str){
	uint8	charP;
	uint32	i,length;
	uint64	result;
	std::string	asciiStr;
	
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
std::string Str_DecToHex(uint64 fZ){
	char tmpstr[256];
	std::string	result;

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
std::string Str_DecToHex(const std::string &str){
	char		tmpstr[256];
	std::string	result;
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
uint64 Str_CharToUint64(const uint8 *retBuf,G_Endian_VAILD endianType){
	uint64 ret;
	ret = 0;
	if (endianType == G_LITTLE_ENDIAN){
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
uint32 Str_CharToUint32(const uint8 *retBuf,G_Endian_VAILD endianType){
	uint32 ret;
	ret = 0;
	if (endianType == G_LITTLE_ENDIAN){
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
uint16 Str_CharToUint16(const uint8 *retBuf,G_Endian_VAILD endianType){
	uint16 ret;
	ret = 0;
	if (endianType == G_LITTLE_ENDIAN){
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
const uint8 *Str_Uint64ToChar(uint8 *retBuf,uint64 data,G_Endian_VAILD endianType){
	if (endianType == G_LITTLE_ENDIAN){
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
const uint8 *Str_Uint32ToChar(uint8 *retBuf,uint32 data,G_Endian_VAILD endianType){
	if (endianType == G_LITTLE_ENDIAN){
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
const uint8 *Str_Uint16ToChar(uint8 *retBuf,uint16 data,G_Endian_VAILD endianType){
	if (endianType == G_LITTLE_ENDIAN){
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
const std::string &Str_LTrimSelf(std::string &str){
	std::string::size_type		ret;
	
	ret = str.find_first_not_of(' ');
	if (ret == std::string::npos){
		str = "";
	}
	else if (ret > 0){
		str = str.erase(0,ret);
	}
	return(str);
}
//------------------------------------------------------------------------------------------//
std::string Str_LTrim(const std::string &str){
	std::string::size_type		ret;
	std::string		returnStr;
	
	ret = str.find_first_not_of(' ');
	if (ret == std::string::npos){
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
const std::string &Str_LTrim(std::string *returnStr,const std::string &str){
	std::string::size_type		ret;
	
	ret = str.find_first_not_of(' ');
	if (ret == std::string::npos){
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
const std::string &Str_RTrimSelf(std::string &str){
	std::string::size_type		ret;
	
	ret = str.find_last_not_of(' ');
	if (ret == std::string::npos){
		str = "";
	}
	else if (ret < (str.length() - 1)){
		str.erase(ret + 1);
	}
	return(str);
}
//------------------------------------------------------------------------------------------//
std::string Str_RTrim(const std::string &str){
	std::string::size_type		ret;
	std::string returnStr;
	
	ret = str.find_last_not_of(' ');
	if (ret == std::string::npos){
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
const std::string &Str_RTrim(std::string *returnStr,const std::string &str){
	std::string::size_type		ret;
	
	ret = str.find_last_not_of(' ');
	if (ret == std::string::npos){
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
const std::string &Str_TrimSelf(std::string &str){
	std::string::size_type		s;
	
	s = str.find_last_not_of(' ');
	if (s == std::string::npos){
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
std::string Str_Trim(const std::string &str){
	std::string::size_type		s,e;
	std::string returnStr;
	s = str.find_first_not_of(' ');
	if (s == std::string::npos){
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
const std::string &Str_Trim(std::string *returnStr,const std::string &str){
	std::string::size_type		s,e;
	
	s = str.find_first_not_of(' ');
	if (s == std::string::npos){
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
const std::string &Str_UpperCaseSelf(std::string &str){
 	std::transform(str.begin(), str.end(), str.begin(), toupper);
	return(str);
}
//------------------------------------------------------------------------------------------//
std::string	Str_UpperCase(const std::string &str){
	std::string::size_type		i,length;
	std::string	strResult;
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
const std::string& Str_UpperCase(std::string *returnStr,const std::string &str){
	std::string::size_type		i,length;
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
const std::string &Str_LowerCaseSelf(std::string &str){
	std::transform(str.begin(), str.end(), str.begin(), tolower);
	return(str);
}
//------------------------------------------------------------------------------------------//
std::string	Str_LowerCase(const std::string &str){
	std::string::size_type		i,length;
	std::string	strResult;
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
const std::string& Str_LowerCase(std::string *returnStr,const std::string &str){
	std::string::size_type		i,length;
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
std::string Str_Replace(const std::string &str,const std::string &strS,const std::string &strD){
	std::string		stringResult;
	std::string::size_type		ret,head,length;
	
	stringResult = "";
	head = 0;
	length = strS.length();
	
	while(head < str.length()){
		ret = str.find(strS,head);
		if (ret == std::string::npos){
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
std::string Str_SplitSubItem(std::string *strInput,uint8 subChar){
	std::string				strResult;
	std::string::size_type	pos,length;
	uint8*					charP;
	
	length = strInput->length();
	pos = 0;
	strResult = "";
	charP = (uint8*)strInput->c_str();
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
				*strInput = strInput->substr(pos);
				return (strResult);
			}
			strResult += *charP;
		}
		++ charP;
	}
	*strInput = "";
	return(strResult);
}
//------------------------------------------------------------------------------------------//
std::string Str_ReadSubItem(std::string *strInput,const std::string &subChar,int32 blReturnSubChar){
	std::string	strResult;
	std::string::size_type		pos;
	
	pos = strInput->find(subChar,0);
	
	if (pos != std::string::npos){
		if (blReturnSubChar == 0){
			strResult = strInput->substr(0,pos);
			*strInput = strInput->substr(pos + subChar.length());
		}
		else{
			strResult = strInput->substr(0,pos + subChar.length());
			*strInput = strInput->substr(pos + subChar.length());
		}
		return(strResult);
	}
	strResult = *strInput;
	*strInput = "";
	return(strResult);
}
//------------------------------------------------------------------------------------------//
std::string Str_ReadSubItemR(std::string *strInput,const std::string &subChar,int32 blReturnSubChar){
	std::string	strResult;
	std::string::size_type		pos;
	
	pos = strInput->rfind(subChar);
	
	if (pos != std::string::npos){
		if (blReturnSubChar == 0){
			strResult = strInput->substr(pos + subChar.length());
			*strInput = strInput->substr(0,strInput->length() - strResult.length() - subChar.length());
		}
		else{
			strResult = strInput->substr(pos);
			*strInput = strInput->substr(0,strInput->length() - strResult.length());
		}
		return(strResult);
	}
	return("");
}
//------------------------------------------------------------------------------------------//
std::string	Str_UnTransferChar(const std::string &strInput){
	std::string::size_type i,length;
	std::string	retStr;
	uint8	charT;
	
	length = strInput.length();
	i = 0;
	retStr = "";
	while(i < length){
		charT = strInput[i];
		switch (charT) {
			case '\\':
				retStr += "\\\\";
				break;
			case '\a':
				retStr += "\\a";
				break;
			case '\b':
				retStr += "\\b";
				break;
			case '\f':
				retStr += "\\f";
				break;
			case '\n':
				retStr += "\\n";
				break;
			case '\r':
				retStr += "\\r";
				break;
			case '\t':
				retStr += "\\t";
				break;
			case '\v':
				retStr += "\\v";
				break;
			case '\0':
				retStr += "\\0";
				break;
			default:
				retStr += charT;
				break;
		}
		++ i;
	}
	return(retStr);
}
//------------------------------------------------------------------------------------------//
const std::string &Str_CharToStr(std::string *retStr,const uint8 *buf,uint64 num,int32 blToHex
								 ,G_ESCAPE_VAILD blEscape,G_SPACE_VAILD blSpace,G_Append_VAILD blAppend){
	//eacape \0xhh,\a,\b,\f,\n,\r,\t,\v,\\,\',\",\0,\/
	std::string				strData;
	std::string::size_type	i,length,j;
	uint8					charData,charData1;
	
	length = (std::string::size_type)num;
	i = 0;
	if (blAppend == G_Append_OFF)
		*retStr = "";
	charData1 = 0;
	while(i < length){
		charData = buf[i];
		if (blEscape != G_ESCAPE_OFF){
			j = 0;
			if ((i + 4) < length){
				j = FIFO_UINT8::Escape0xhhToChar(&charData1,Str_CharToStr(&strData,&buf[i],5,0,G_ESCAPE_OFF,G_SPACE_OFF,G_Append_OFF));
			}
			else if ((i + 4) == length){
				j = FIFO_UINT8::Escape0xhhToChar(&charData1,Str_CharToStr(&strData,&buf[i],4,0,G_ESCAPE_OFF,G_SPACE_OFF,G_Append_OFF));
			}
			if (j > 0){
				i += j;
				charData = charData1;
			}
			else if ((i + 1) < length){
				j = FIFO_UINT8::EscapeToChar(&charData1,Str_CharToStr(&strData,&buf[i],2,0,G_ESCAPE_OFF,G_SPACE_OFF,G_Append_OFF));
				if (j > 0){
					i += j;
					charData = charData1;
				}
			}
			else if (charData == '\\'){
				++ i;
				continue;
			}
		}
		if (blToHex == 0){
			*retStr += charData;
		}
		else{
			*retStr += FIFO_UINT8::CharToHex(charData);
			if (blSpace != G_SPACE_OFF)
				*retStr += ' ';
		}
		++ i;
	};
	return(*retStr);
}
//------------------------------------------------------------------------------------------//
const std::string &Str_HEXTo(std::string *retStr,const std::string &strInput,int32 blToASCII,G_SPACE_VAILD blSpace,G_Append_VAILD blAppend){
	std::string::size_type	i,length,j;
	uint8	charP,charData,charResult;
	
	length = strInput.length();
	i = 0;
	j = 0;
	charResult = 0;
	charData = 0;
	if (blAppend == G_Append_OFF)
		*retStr = "";
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
		if ((j > 0) && ((charP == ' ') || (j == 2) || (i == length))){
			if (blToASCII != 0){
				*retStr += charResult;
			}
			else{
				*retStr += FIFO_UINT8::CharToHex(charResult);
				if (blSpace == G_SPACE_ON)
					*retStr += ' ';
			}
			j = 0;
		}
	}
	return(*retStr);
}
//------------------------------------------------------------------------------------------//
std::string::size_type Str_DCompare(const std::string &strContent,const std::string &strD1,std::string::size_type strContentPostion){
	std::string::size_type	ret;
	
	do{
		ret = strContent.find(strD1,strContentPostion);
		
		if (ret == std::string::npos)
			return ret;
		if ((ret % 2) == 0)
			return ret;
		++ strContentPostion;
	}while((++ strContentPostion) < strContent.length());
	return 0;
}
#ifdef CommonDefH_VC
//------------------------------------------------------------------------------------------//
std::wstring Str_ANSIToUnicode(const std::string &strInput){
	int32			unicodeLen;
	wchar_t			*pUnicode;
	std::wstring	ret;
	
	unicodeLen = MultiByteToWideChar(CP_ACP,0,strInput.c_str(),-1,nullptr,0);
	
	pUnicode = new  wchar_t[unicodeLen + 1];
	memset(pUnicode,0,(unicodeLen + 1)*sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP,0,strInput.c_str(),-1,(LPWSTR)pUnicode,unicodeLen);
	
	ret = (wchar_t*)pUnicode;
	delete  []pUnicode;
	return  ret;
}
//------------------------------------------------------------------------------------------//
std::string Str_UnicodeToANSI(const std::wstring &strInput){
	char*			pElementText;
	int32			iTextLen;
	std::string		strText;
	
	iTextLen = WideCharToMultiByte(CP_ACP,0,strInput.c_str(),-1,nullptr,0,nullptr,nullptr);
	pElementText = new char[iTextLen + 1];
	memset((void*)pElementText,0,sizeof(char) * (iTextLen + 1));
	WideCharToMultiByte( CP_ACP,0,strInput.c_str(),-1,pElementText,iTextLen,nullptr,nullptr);
	
	strText = pElementText;
	delete []pElementText;
	return(strText);
}
//------------------------------------------------------------------------------------------//
std::wstring Str_UTF8ToUnicode(const std::string &strInput){
	int32			unicodeLen;
	wchar_t			*pUnicode;
	std::wstring	ret;
	
	unicodeLen = MultiByteToWideChar(CP_UTF8,0,strInput.c_str(),-1,nullptr,0);
	
	pUnicode = new  wchar_t[unicodeLen+1];
	memset(pUnicode,0,(unicodeLen + 1)*sizeof(wchar_t));
	MultiByteToWideChar( CP_UTF8,0,strInput.c_str(),-1,(LPWSTR)pUnicode,unicodeLen);
	
	ret = (wchar_t*)pUnicode;
	delete  []pUnicode;
	return(ret);
}
//------------------------------------------------------------------------------------------//
std::string Str_UnicodeToUTF8(const std::wstring &strInput){
	char*			pElementText;
	int32			iTextLen;
	std::string		strText;
	iTextLen = WideCharToMultiByte( CP_UTF8,0,strInput.c_str(),-1,nullptr,0,nullptr,nullptr);
	pElementText = new char[iTextLen + 1];
	memset((void*)pElementText,0,sizeof(char) * (iTextLen + 1));
	WideCharToMultiByte(CP_UTF8,0,strInput.c_str(),-1,pElementText,iTextLen,nullptr,nullptr);
	
	strText = pElementText;
	delete []pElementText;
	return(strText);
}
//------------------------------------------------------------------------------------------//
#endif
//------------------------------------------------------------------------------------------//
int ExecuteConsoleCommand(const std::string &strCommand){
#ifdef CommonDefH_Unix
	char	**argv;
	std::string	cmd,strArg;
	int32	num,ret;
	
	num = 0;
	cmd = strCommand;
	do{
		Str_TrimSelf(cmd);
		strArg = Str_ReadSubItem(&cmd, " ");
		if (strArg.length() > 0)
			++ num;
	}while(cmd.length() > 0);
	
	if (num > 0){
		argv = new char*[num + 1];
		argv[num] = (char*)nullptr;
		
		num = 0;
		cmd = strCommand;
		do{
			Str_TrimSelf(cmd);
			strArg = Str_ReadSubItem(&cmd, " ");
			if (strArg.length() > 0){
				argv[num] = new char[strArg.length() + 1];
				memcpy(argv[num],strArg.c_str(),strArg.length());
				argv[num][strArg.length()] = 0;
				++ num;
			}
		}while(cmd.length() > 0);
		ret = execvp(argv[0],argv);
		delete []argv;
		return(ret);
	}
#endif
	return 0;
}
//------------------------------------------------------------------------------------------//

