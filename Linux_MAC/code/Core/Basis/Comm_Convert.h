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
#include <sstream>
//------------------------------------------------------------------------------------------//
template <typename outType,typename inType> outType& Str_Convert(outType *result,const inType &value){
	std::stringstream tStream;
	tStream << value;
	tStream >> *result;
	return(*result);
}
template <typename inType> STDSTR	Str_ToString(const inType &value){
	STDSTR	result;
	return(Str_Convert(&result,value));
}
//------------------------------------------------------------------------------------------//
STDSTR	Str_FloatToString	(double fZ,int32 blNozero = 1,int32 slen = 5,int32 alen = 3);

uint64	Str_HexToDec		(const STDSTR &str);
STDSTR	Str_DecToHex		(uint64 fZ);
STDSTR	Str_DecToHex		(const STDSTR &str);

const uint8 *Str_Uint16ToChar(uint8 *retBuf,uint16 data,G_ENDIAN endianType = G_ENDIAN_LITTLE);
const uint8 *Str_Uint32ToChar(uint8 *retBuf,uint32 data,G_ENDIAN endianType = G_ENDIAN_LITTLE);
const uint8 *Str_Uint64ToChar(uint8 *retBuf,uint64 data,G_ENDIAN endianType = G_ENDIAN_LITTLE);

uint16 Str_CharToUint16(const uint8 *retBuf,G_ENDIAN endianType = G_ENDIAN_LITTLE);
uint32 Str_CharToUint32(const uint8 *retBuf,G_ENDIAN endianType = G_ENDIAN_LITTLE);
uint64 Str_CharToUint64(const uint8 *retBuf,G_ENDIAN endianType = G_ENDIAN_LITTLE);

const	STDSTR&	Str_LTrimSelf		(STDSTR &str);
		STDSTR		Str_LTrim		(const STDSTR &str);
const	STDSTR&	Str_LTrim			(STDSTR *returnStr,const STDSTR &str);

const	STDSTR&	Str_RTrimSelf		(STDSTR &str);
		STDSTR		Str_RTrim		(const STDSTR &str);
const	STDSTR&	Str_RTrim			(STDSTR *returnStr,const STDSTR &str);

const	STDSTR&	Str_TrimSelf		(STDSTR &str);
		STDSTR		Str_Trim		(const STDSTR &str);
const	STDSTR&	Str_Trim			(STDSTR *returnStr,const STDSTR &str);

const	STDSTR&	Str_UpperCaseSelf	(STDSTR &str);
		STDSTR		Str_UpperCase	(const STDSTR &str);
const	STDSTR&	Str_UpperCase		(STDSTR *returnStr,const STDSTR &str);//*returnStr cannot be the same as &str

const	STDSTR&	Str_LowerCaseSelf	(STDSTR &str);
		STDSTR		Str_LowerCase	(const STDSTR &str);
const	STDSTR&	Str_LowerCase		(STDSTR *returnStr,const STDSTR &str);//*returnStr  cannot be the same as &str

STDSTR	Str_Replace			(const STDSTR &str,const STDSTR &strS,const STDSTR &strD);
STDSTR	Str_SplitSubItem	(STDSTR *strIn,uint8 subChar);
STDSTR	Str_ReadSubItem		(STDSTR *strIn,const STDSTR &subChar,int32 blReturnSubChar = 0);
STDSTR	Str_ReadSubItemR	(STDSTR *strIn,const STDSTR &subChar,int32 blReturnSubChar = 0);

		const	STDSTR& AddSpaceInFront		(STDSTR *strIn,uint32 length);
		const	STDSTR& Str_UnTransferChar	(STDSTR *retStr,const uint8 *buf,uint64 num);
inline	const	STDSTR& Str_UnTransferChar	(STDSTR *retStr,const STDSTR &strIn){return(Str_UnTransferChar(retStr,(uint8*)strIn.c_str(),strIn.length()));};


const	STDSTR& Str_CharToStrInEscape	(STDSTR *retStr,const uint8 *buf,uint64 num);
const	STDSTR&	Str_CharToStr			(STDSTR *retStr,const uint8 *buf,uint64 num,G_HA blHA,G_ESCAPE blEscape,G_SPACE blSpace,G_APPEND blAppend);
const	STDSTR&	Str_HEXTo				(STDSTR *retStr,const uint8 *buf,uint64 num,G_HA blHA,G_SPACE blSpace,G_APPEND blAppend);

inline const	STDSTR&	Str_CharToStr		(STDSTR *retStr,const STDSTR &strIn,G_HA blHA,G_ESCAPE blEscape,G_SPACE blSpace,G_APPEND blAppend){
	return(Str_CharToStr(retStr,(uint8*)strIn.c_str(),strIn.length(),blHA,blEscape,blSpace,blAppend));
}
inline const	STDSTR&	Str_HEXTo			(STDSTR *retStr,const STDSTR &strIn,G_HA blHA,G_SPACE blSpace,G_APPEND blAppend){
	return(Str_HEXTo(retStr,(uint8*)strIn.c_str(),strIn.length(),blHA,blSpace,blAppend));
}

inline			STDSTR	Str_CharToASCIIStr	(const uint8 *buf,uint64 num,G_ESCAPE blEscape){
	STDSTR	retStr;
	return(Str_CharToStr(&retStr,buf,num,G_ASCII,blEscape,G_SPACE_OFF,G_APPEND_OFF));
};
inline const 	STDSTR&	Str_CharToASCIIStr	(STDSTR *retStr,const uint8 *buf,uint64 num,G_ESCAPE blEscape){
	return(Str_CharToStr(retStr,buf,num,G_ASCII,blEscape,G_SPACE_OFF,G_APPEND_OFF));
};
inline			STDSTR	Str_CharToHEXStr	(const uint8 *buf,uint64 num,G_SPACE blSpace){
	STDSTR	retStr;
	return(Str_CharToStr(&retStr,buf,num,G_HEX,G_ESCAPE_OFF,blSpace,G_APPEND_OFF));
}
inline const	STDSTR&	Str_CharToHEXStr	(STDSTR *retStr,const uint8 *buf,uint64 num,G_SPACE blSpace){
	return(Str_CharToStr(retStr,buf,num,G_HEX,G_ESCAPE_OFF,blSpace,G_APPEND_OFF));
}
inline			STDSTR	Str_ASCIIToHEXs		(const STDSTR &strIn,G_ESCAPE blEscape){
	STDSTR	retStr;
	return(Str_CharToStr(&retStr,strIn,G_HEX,blEscape,G_SPACE_ON,G_APPEND_OFF));
};
inline const	STDSTR&	Str_ASCIIToHEXs		(STDSTR *retStr,const STDSTR &strIn,G_ESCAPE blEscape){
	return(Str_CharToStr(retStr,strIn,G_HEX,blEscape,G_SPACE_ON,G_APPEND_OFF));
};
inline			STDSTR	Str_ASCIIToHEX		(const STDSTR &strIn,G_ESCAPE blEscape){
	STDSTR	retStr;
	return(Str_CharToStr(&retStr,strIn,G_HEX,blEscape,G_SPACE_OFF,G_APPEND_OFF));
};
inline const	STDSTR&	Str_ASCIIToHEX		(STDSTR *retStr,const STDSTR &strIn,G_ESCAPE blEscape){
	return(Str_CharToStr(retStr,strIn,G_HEX,blEscape,G_SPACE_OFF,G_APPEND_OFF));
};

inline			STDSTR	Str_HEXToASCII		(const STDSTR &strIn)				{STDSTR retStr;return(Str_HEXTo(&retStr,strIn,G_ASCII,G_SPACE_OFF,G_APPEND_OFF));};
inline const	STDSTR&	Str_HEXToASCII		(STDSTR *retStr,const STDSTR &strIn){return(Str_HEXTo(retStr,strIn,G_ASCII,G_SPACE_OFF,G_APPEND_OFF));};
inline			STDSTR	Str_HEXToHEX		(const STDSTR &strIn)				{STDSTR retStr;return(Str_HEXTo(&retStr,strIn,G_HEX,G_SPACE_OFF,G_APPEND_OFF));};
inline const	STDSTR&	Str_HEXToHEX		(STDSTR *retStr,const STDSTR &strIn){return(Str_HEXTo(retStr,strIn,G_HEX,G_SPACE_OFF,G_APPEND_OFF));};
inline			STDSTR	Str_HEXToHEXs		(const STDSTR &strIn)				{STDSTR retStr;return(Str_HEXTo(&retStr,strIn,G_HEX,G_SPACE_ON,G_APPEND_OFF));};
inline const	STDSTR&	Str_HEXToHEXs		(STDSTR *retStr,const STDSTR &strIn){return(Str_HEXTo(retStr,strIn,G_HEX,G_SPACE_ON,G_APPEND_OFF));};

#ifdef CommonDefH_VC
std::wstring	Str_ANSIToUnicode	(const STDSTR &strIn);
STDSTR			Str_UnicodeToANSI	(const std::wstring &strIn);
std::wstring	Str_UTF8ToUnicode	(const STDSTR &strIn);
STDSTR			Str_UnicodeToUTF8	(const std::wstring &strIn);
#endif
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif
