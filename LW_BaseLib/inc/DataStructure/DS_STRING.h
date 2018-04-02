//
//  DS_STRING.h
//  SDT
//
//  Created by Leif Wen on 19/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#ifndef DS_STRING_h
#define DS_STRING_h
//------------------------------------------------------------------------------------------//
#include "BasicDefine.h"
//------------------------------------------------------------------------------------------//
const	STDSTR&	Str_LTrimSelf		(		STDSTR& str);
const	STDSTR&	Str_LTrim			(		STDSTR* _out,const STDSTR& _in);
		STDSTR	Str_LTrim			(const	STDSTR& str);

const	STDSTR&	Str_RTrimSelf		(		STDSTR& str);
const	STDSTR&	Str_RTrim			(		STDSTR* _out,const STDSTR& _in);
		STDSTR	Str_RTrim			(const	STDSTR& str);

const	STDSTR&	Str_TrimSelf		(		STDSTR& str);
const	STDSTR&	Str_Trim			(		STDSTR* _out,const STDSTR& _in);
		STDSTR	Str_Trim			(const	STDSTR& str);

const	STDSTR&	Str_UpperCaseSelf	(		STDSTR& str);
const	STDSTR&	Str_UpperCase		(		STDSTR* _out,const STDSTR& _in);//*_out cannot be the same as &str
		STDSTR	Str_UpperCase		(const	STDSTR& _in);

const	STDSTR&	Str_LowerCaseSelf	(		STDSTR& str);
const	STDSTR&	Str_LowerCase		(		STDSTR* _out,const STDSTR& _in);//*_out  cannot be the same as &str
		STDSTR	Str_LowerCase		(const	STDSTR& str);
//------------------------------------------------------------------------------------------//
		bool32	Str_Find			(STDSTR* _out,const STDSTR& strContent,const STDSTR& searchString);
		STDSTR	Str_Replace			(const STDSTR& _in,const STDSTR& strS,const STDSTR& strD);
		STDSTR	Str_SplitSubItem	(STDSTR* strIn,uint8 subChar);
		STDSTR	Str_ReadSubItem		(STDSTR* strIn,const STDSTR& subChar,bool32 blReturnSubChar = G_FALSE);
		STDSTR	Str_ReadSubItemR	(STDSTR* strIn,const STDSTR& subChar,bool32 blReturnSubChar = G_FALSE);
const	STDSTR& Str_AddSpaceInFront	(STDSTR* strIn,uint32 length);
//------------------------------------------------------------------------------------------//
template <typename outType,typename inType> outType&	Str_Convert	(outType *result,const inType &value);
template <typename inType> 					STDSTR		Str_ToStr	(const inType &value);
//------------------------------------------------------------------------------------------//
		STDSTR	Str_FloatToStr		(double fZ,int32 blNozero = 1,int32 slen = 5,int32 alen = 3);

		uint64	Str_HexToDec		(const STDSTR& str);
		STDSTR	Str_DecToHex		(uint64 fZ);
		STDSTR	Str_DecToHex		(const STDSTR& str);

		uint16	Str_CharToUint16	(const uint8* data,G_ENDIAN endianType = G_ENDIAN_LITTLE);
		uint32	Str_CharToUint32	(const uint8* data,G_ENDIAN endianType = G_ENDIAN_LITTLE);
		uint64	Str_CharToUint64	(const uint8* data,G_ENDIAN endianType = G_ENDIAN_LITTLE);

const	uint8*	Str_Uint16ToChar	(uint8* _out,uint16 data,G_ENDIAN endianType = G_ENDIAN_LITTLE);
const	uint8*	Str_Uint32ToChar	(uint8* _out,uint32 data,G_ENDIAN endianType = G_ENDIAN_LITTLE);
const	uint8*	Str_Uint64ToChar	(uint8* _out,uint64 data,G_ENDIAN endianType = G_ENDIAN_LITTLE);
//------------------------------------------------------------------------------------------//
		uint32	Str_Escape_xhh		(uint8& retChar,const uint8*& data,uint32& num);
		uint32	Str_Escape_xhh		(uint8& retChar,const uint8*& data,uint64& num);
		uint32	Str_EscapeToChar	(uint8& retChar,const uint8*& data,uint32& num);
		uint32	Str_EscapeToChar	(uint8& retChar,const uint8*& data,uint64& num);
		void	Str_CharToHex		(STDSTR* _out,uint8 data);
//------------------------------------------------------------------------------------------//
				const	STDSTR& Str_UnEscapeToStr	(STDSTR* _out,const uint8 data);
				const	STDSTR& Str_UnEscapeToStr	(STDSTR* _out,const uint8* data,uint64 num);
static inline	const	STDSTR& Str_UnEscapeToStr	(STDSTR* _out,const STDSTR& _in);
//------------------------------------------------------------------------------------------//
				const	STDSTR& Str_CharToStrEscape	(STDSTR* _out,const uint8 *data,uint64 num);
				const	STDSTR&	Str_CharToStr		(STDSTR* _out,const uint8 *data,uint64 num,G_HA blHA,G_SPACE blSpace,G_ESCAPE blEscape);
				const	STDSTR&	Str_HEXToStr		(STDSTR* _out,const uint8 *data,uint64 num,G_HA blHA,G_SPACE blSpace);

static inline	const	STDSTR&	Str_CharToStr		(STDSTR* _out,const STDSTR& _in,G_HA blHA,G_SPACE blSpace,G_ESCAPE blEscape);
static inline	const	STDSTR&	Str_HEXToStr		(STDSTR* _out,const STDSTR& _in,G_HA blHA,G_SPACE blSpace);

static inline			STDSTR	Str_CharToASCII		(				const uint8* data,uint64 num,G_ESCAPE blEscape);
static inline	const 	STDSTR&	Str_CharToASCII		(STDSTR* _out,	const uint8* data,uint64 num,G_ESCAPE blEscape);
static inline			STDSTR	Str_CharToHEX		(				const uint8* data,uint64 num,G_SPACE blSpace);
static inline	const	STDSTR&	Str_CharToHEX		(STDSTR* _out,	const uint8* data,uint64 num,G_SPACE blSpace);
static inline			STDSTR	Str_ASCIIToHEXs		(				const STDSTR& _in,G_ESCAPE blEscape);
static inline	const	STDSTR&	Str_ASCIIToHEXs		(STDSTR* _out,	const STDSTR& _in,G_ESCAPE blEscape);
static inline			STDSTR	Str_ASCIIToHEX		(				const STDSTR& _in,G_ESCAPE blEscape);
static inline	const	STDSTR&	Str_ASCIIToHEX		(STDSTR* _out,	const STDSTR& _in,G_ESCAPE blEscape);

static inline			STDSTR	Str_HEXToASCII		(				const STDSTR& _in);
static inline	const	STDSTR&	Str_HEXToASCII		(STDSTR* _out,	const STDSTR& _in);
static inline			STDSTR	Str_HEXToHEX		(				const STDSTR& _in);
static inline	const	STDSTR&	Str_HEXToHEX		(STDSTR* _out,	const STDSTR& _in);
static inline			STDSTR	Str_HEXToHEXs		(				const STDSTR& _in);
static inline	const	STDSTR&	Str_HEXToHEXs		(STDSTR* _out,	const STDSTR& _in);
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_VC
std::wstring	Str_ANSIToUnicode	(const STDSTR&			strIn);
STDSTR			Str_UnicodeToANSI	(const std::wstring&	strIn);
std::wstring	Str_UTF8ToUnicode	(const STDSTR&			strIn);
STDSTR			Str_UnicodeToUTF8	(const std::wstring&	strIn);
#endif
//------------------------------------------------------------------------------------------//
#include "DS_STRING.hpp"
#endif /* DS_STRING_h */
