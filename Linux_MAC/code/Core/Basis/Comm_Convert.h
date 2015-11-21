/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: Comm_Convert.h
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
#ifndef Comm_ConvertH
#define Comm_ConvertH
//------------------------------------------------------------------------------------------//
#include "BasicDef.h"
std::string		Str_FloatToString	(double fZ,int32 blNozero = 1,int32 slen = 5,int32 alen = 3);
std::string		Str_IntToString		(int32 fZ);
std::string		Str_UIntToString	(uint32 fZ);
std::string		Str_UInt64ToString	(uint64 fZ);
std::string		Str_Int64ToString	(int64 fZ);

uint64			Str_HexToDec		(const std::string &str);
std::string		Str_DecToHex		(uint64 fZ);
std::string		Str_DecToHex		(const std::string &str);

const uint8 *Str_Uint16ToChar(uint8 *retBuf,uint16 data,G_Endian_VAILD endianType = G_LITTLE_ENDIAN);
const uint8 *Str_Uint32ToChar(uint8 *retBuf,uint32 data,G_Endian_VAILD endianType = G_LITTLE_ENDIAN);
const uint8 *Str_Uint64ToChar(uint8 *retBuf,uint64 data,G_Endian_VAILD endianType = G_LITTLE_ENDIAN);

uint16 Str_CharToUint16(const uint8 *retBuf,G_Endian_VAILD endianType = G_LITTLE_ENDIAN);
uint32 Str_CharToUint32(const uint8 *retBuf,G_Endian_VAILD endianType = G_LITTLE_ENDIAN);
uint64 Str_CharToUint64(const uint8 *retBuf,G_Endian_VAILD endianType = G_LITTLE_ENDIAN);

const	std::string&	Str_LTrimSelf		(std::string &str);
		std::string		Str_LTrim			(const std::string &str);
const	std::string&	Str_LTrim			(std::string *returnStr,const std::string &str);

const	std::string&	Str_RTrimSelf		(std::string &str);
		std::string		Str_RTrim			(const std::string &str);
const	std::string&	Str_RTrim			(std::string *returnStr,const std::string &str);

const	std::string&	Str_TrimSelf		(std::string &str);
		std::string		Str_Trim			(const std::string &str);
const	std::string&	Str_Trim			(std::string *returnStr,const std::string &str);

const	std::string&	Str_UpperCaseSelf	(std::string &str);
		std::string		Str_UpperCase		(const std::string &str);
const	std::string&	Str_UpperCase		(std::string *returnStr,const std::string &str);//*returnStr cannot be the same as &str

const	std::string&	Str_LowerCaseSelf	(std::string &str);
		std::string		Str_LowerCase		(const std::string &str);
const	std::string&	Str_LowerCase		(std::string *returnStr,const std::string &str);//*returnStr  cannot be the same as &str

std::string		Str_Replace			(const std::string &str,const std::string &strS,const std::string &strD);
std::string		Str_ReadSubItem		(std::string *strInput,const std::string &subChar,int32 blReturnSubChar = 0);
std::string		Str_ReadSubItemR	(std::string *strInput,const std::string &subChar,int32 blReturnSubChar = 0);

const	std::string&	Str_CharToStr		(std::string *retStr,const uint8 *buf,uint64 num,int32 blToHex,G_ESCAPE_VAILD blEscape,G_SPACE_VAILD blSpace);
const	std::string&	Str_HEXTo			(std::string *retStr,const std::string &strInput,int32 blToASCII,G_SPACE_VAILD blSpace);

				std::string		Str_UnTransferChar	(const std::string &strInput);
inline			std::string		Str_CharToASCIIStr	(const uint8 *buf,uint64 num,G_ESCAPE_VAILD blEscape){
	std::string	retStr;
	return(Str_CharToStr(&retStr,buf,num,0,blEscape,G_SPACE_OFF));
};
inline const 	std::string&	Str_CharToASCIIStr	(std::string *retStr,const uint8 *buf,uint64 num,G_ESCAPE_VAILD blEscape){
	return(Str_CharToStr(retStr,buf,num,0,blEscape,G_SPACE_OFF));
};
inline			std::string		Str_CharToHEXStr	(const uint8 *buf,uint64 num,G_SPACE_VAILD blSpace){
	std::string	retStr;
	return(Str_CharToStr(&retStr,buf,num,1,G_ESCAPE_OFF,blSpace));
}
inline const	std::string&	Str_CharToHEXStr	(std::string *retStr,const uint8 *buf,uint64 num,G_SPACE_VAILD blSpace){
	return(Str_CharToStr(retStr,buf,num,1,G_ESCAPE_OFF,blSpace));
}
inline			std::string		Str_ASCIIToHEXs		(const std::string &strInput,G_ESCAPE_VAILD blEscape){
	std::string	retStr;
	return(Str_CharToStr(&retStr,(uint8*)strInput.c_str(),strInput.length(),1,blEscape,G_SPACE_ON));
};
inline const	std::string&	Str_ASCIIToHEXs		(std::string *retStr,const std::string &strInput,G_ESCAPE_VAILD blEscape){
	return(Str_CharToStr(retStr,(uint8*)strInput.c_str(),strInput.length(),1,blEscape,G_SPACE_ON));
};
inline			std::string		Str_ASCIIToHEX		(const std::string &strInput,G_ESCAPE_VAILD blEscape){
	std::string	retStr;
	return(Str_CharToStr(&retStr,(uint8*)strInput.c_str(),strInput.length(),1,blEscape,G_SPACE_OFF));
};
inline const	std::string&	Str_ASCIIToHEX		(std::string *retStr,const std::string &strInput,G_ESCAPE_VAILD blEscape){
	return(Str_CharToStr(retStr,(uint8*)strInput.c_str(),strInput.length(),1,blEscape,G_SPACE_OFF));
};

inline			std::string		Str_HEXToASCII		(const std::string &strInput){
	std::string	retStr;
	return(Str_HEXTo(&retStr,strInput,1,G_SPACE_OFF));
};
inline const	std::string&	Str_HEXToASCII		(std::string *retStr,const std::string &strInput){
	return(Str_HEXTo(retStr,strInput,1,G_SPACE_OFF));
};
inline			std::string		Str_HEXToHEX		(const std::string &strInput){
	std::string	retStr;
	return(Str_HEXTo(&retStr,strInput,0,G_SPACE_OFF));
};
inline const	std::string&	Str_HEXToHEX		(std::string *retStr,const std::string &strInput){
	return(Str_HEXTo(retStr,strInput,0,G_SPACE_OFF));
};
inline			std::string		Str_HEXToHEXs		(const std::string &strInput){
	std::string	retStr;
	return(Str_HEXTo(&retStr,strInput,0,G_SPACE_ON));
};
inline const	std::string&	Str_HEXToHEXs		(std::string *retStr,const std::string &strInput){
	return(Str_HEXTo(retStr,strInput,0,G_SPACE_ON));
};

std::string::size_type  Str_DCompare(const std::string &strContent,const std::string &strD1,std::string::size_type strContentPostion);
#ifdef CommonDefH_VC
std::wstring	Str_ANSIToUnicode	(const std::string &strInput);
std::string		Str_UnicodeToANSI	(const std::wstring &strInput);
std::wstring	Str_UTF8ToUnicode	(const std::string &strInput);
std::string		Str_UnicodeToUTF8	(const std::wstring &strInput);
#endif
//------------------------------------------------------------------------------------------//
int ExecuteConsoleCommand(const std::string &strCommand);
//------------------------------------------------------------------------------------------//
#endif