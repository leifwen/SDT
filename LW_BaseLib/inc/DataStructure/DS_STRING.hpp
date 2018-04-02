//
//  DS_STRING.hpp
//  SDT
//
//  Created by Leif Wen on 19/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#ifndef DS_STRING_hpp
#define DS_STRING_hpp
//------------------------------------------------------------------------------------------//
#include "DS_STRING.h"
#include <sstream>
#ifdef DS_STRING_h
//------------------------------------------------------------------------------------------//
template <typename outType,typename inType> outType& Str_Convert(outType *result,const inType &value){
	std::stringstream tStream;
	tStream << value;
	tStream >> *result;
	return(*result);
}
template <typename inType> STDSTR	Str_ToStr(const inType &value){
	STDSTR	result;
	return(Str_Convert(&result,value));
}
//------------------------------------------------------------------------------------------//
static inline const	STDSTR& Str_UnEscapeToStr	(STDSTR* _out,	const STDSTR& _in){
	
	return(Str_UnEscapeToStr(_out,(uint8*)_in.c_str(),_in.length()));
};
static inline const	STDSTR& Str_CharToStr		(STDSTR* _out,	const STDSTR& _in,G_HA blHA,G_SPACE blSpace,G_ESCAPE blEscape){
	
	return(Str_CharToStr(_out,(uint8*)_in.c_str(),_in.length(),blHA,blSpace,blEscape));
}
static inline const	STDSTR& Str_HEXToStr		(STDSTR* _out,	const STDSTR& _in,G_HA blHA,G_SPACE blSpace){
	
	return(Str_HEXToStr(_out,(uint8*)_in.c_str(),_in.length(),blHA,blSpace));
}
static inline		STDSTR	Str_CharToASCII		(				const uint8* data,uint64 num,G_ESCAPE blEscape){
	
	STDSTR	retStr;return(Str_CharToStr(&retStr,data,num,G_ASCII,G_SPACE_OFF,blEscape));
};
static inline const STDSTR&	Str_CharToASCII		(STDSTR* _out,	const uint8* data,uint64 num,G_ESCAPE blEscape){
	
	return(Str_CharToStr(_out,data,num,G_ASCII,G_SPACE_OFF,blEscape));
};
static inline		STDSTR	Str_CharToHEX		(				const uint8* data,uint64 num,G_SPACE blSpace){
	
	STDSTR	retStr;return(Str_CharToStr(&retStr,data,num,G_HEX,blSpace,G_ESCAPE_OFF));
}
static inline const	STDSTR&	Str_CharToHEX		(STDSTR* _out,	const uint8* data,uint64 num,G_SPACE blSpace){
	
	return(Str_CharToStr(_out,data,num,G_HEX,blSpace,G_ESCAPE_OFF));
}
static inline		STDSTR	Str_ASCIIToHEXs		(				const STDSTR& _in,G_ESCAPE blEscape){
	
	STDSTR	retStr;return(Str_CharToStr(&retStr,_in,G_HEX,G_SPACE_ON,blEscape));
};
static inline const	STDSTR&	Str_ASCIIToHEXs		(STDSTR* _out,	const STDSTR& _in,G_ESCAPE blEscape){
	
	return(Str_CharToStr(_out,_in,G_HEX,G_SPACE_ON,blEscape));
};
static inline		STDSTR	Str_ASCIIToHEX		(				const STDSTR& _in,G_ESCAPE blEscape){
	
	STDSTR	retStr;return(Str_CharToStr(&retStr,_in,G_HEX,G_SPACE_OFF,blEscape));
};
static inline const	STDSTR&	Str_ASCIIToHEX		(STDSTR* _out,	const STDSTR& _in,G_ESCAPE blEscape){
	
	return(Str_CharToStr(_out,_in,G_HEX,G_SPACE_OFF,blEscape));
};
static inline		STDSTR	Str_HEXToASCII		(				const STDSTR& _in){
	
	STDSTR retStr;return(Str_HEXToStr(&retStr,_in,G_ASCII,G_SPACE_OFF));
};
static inline const	STDSTR&	Str_HEXToASCII		(STDSTR* _out,	const STDSTR& _in){
	
	return(Str_HEXToStr(_out,_in,G_ASCII,G_SPACE_OFF));
};
static inline		STDSTR	Str_HEXToHEX		(				const STDSTR& _in){
	
	STDSTR retStr;return(Str_HEXToStr(&retStr,_in,G_HEX,G_SPACE_OFF));
};
static inline const	STDSTR&	Str_HEXToHEX		(STDSTR* _out,	const STDSTR& _in){
	
	return(Str_HEXToStr(_out,_in,G_HEX,G_SPACE_OFF));
};
static inline		STDSTR	Str_HEXToHEXs		(				const STDSTR& _in){

	STDSTR retStr;return(Str_HEXToStr(&retStr,_in,G_HEX,G_SPACE_ON));
};
static inline const	STDSTR&	Str_HEXToHEXs		(STDSTR* _out,	const STDSTR& _in){

	return(Str_HEXToStr(_out,_in,G_HEX,G_SPACE_ON));
};
//------------------------------------------------------------------------------------------//
#endif /* DS_STRING_h */
#endif /* DS_STRING_hpp */
