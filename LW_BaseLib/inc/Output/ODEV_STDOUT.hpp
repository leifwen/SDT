//
//  ODEV_STDOUT.hpp
//  SDT
//
//  Created by Leif Wen on 31/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#ifndef ODEV_STDOUT_hpp
#define ODEV_STDOUT_hpp
//------------------------------------------------------------------------------------------//
#include "ODEV_STDOUT.h"
#ifdef ODEV_STDOUT_h
#include "DS_STRING.h"
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_Unix
//------------------------------------------------------------------------------------------//
inline ODEV_STDOUT& ODEV_STDOUT::operator << (const _Left& data){
	if (data.value != 0)
		*this << "\033[" << Str_ToStr(data.value) << "\x44";
	return(*this);
};
//------------------------------------------------------------------------------------------//
inline ODEV_STDOUT& ODEV_STDOUT::operator << (const _Right& data){
	if (data.value != 0)
		*this << "\033[" << Str_ToStr(data.value) << "\x43";
	return(*this);
};
//------------------------------------------------------------------------------------------//
inline ODEV_STDOUT& ODEV_STDOUT::operator << (const _Down& data){
	if (data.value != 0)
		*this << "\033[" << Str_ToStr(data.value) << "\x42";
	return(*this);
};
//------------------------------------------------------------------------------------------//
inline ODEV_STDOUT& ODEV_STDOUT::operator << (const _Up& data){
	if (data.value != 0)
		*this << "\033[" << Str_ToStr(data.value) << "\x41";
	return(*this);
};
//------------------------------------------------------------------------------------------//
inline ODEV_STDOUT& ODEV_STDOUT::operator << (const _CNL& data){
	*this << "\033[" << Str_ToStr(data.value) << "\x45";
	return(*this);
};
//------------------------------------------------------------------------------------------//
inline ODEV_STDOUT& ODEV_STDOUT::operator << (const _ICH& data){
	if (data.value != 0)
		*this << "\033[" << Str_ToStr(data.value) << "\x40";
	return(*this);
};
//------------------------------------------------------------------------------------------//
inline ODEV_STDOUT& ODEV_STDOUT::operator << (const _DCH& data){
	if (data.value != 0)
		*this << "\033[" << Str_ToStr(data.value) << "\x50";
	return(*this);
};
//------------------------------------------------------------------------------------------//
inline ODEV_STDOUT& ODEV_STDOUT::operator << (const _EL& data){
	*this << "\033[" << Str_ToStr(data.value) << "\x4b";
	return(*this);
};
//------------------------------------------------------------------------------------------//
inline void ODEV_STDOUT::SetCurLeft(int32 num){
	if (num > 0)
		*this << Begin << SetLeft(num) << Endl;
};
//------------------------------------------------------------------------------------------//
inline void ODEV_STDOUT::SetCurRight(int32 num){
	if (num > 0)
		*this << Begin << SetRight(num) << Endl;
};
//------------------------------------------------------------------------------------------//
inline void ODEV_STDOUT::InsterChar(COLORENUM col,const uint8* data,uint32 length){
	if (length > 0)
		*this << Begin << col << SetICH(length)  << IUD(data,(uint64)length) << Endl;
};
//------------------------------------------------------------------------------------------//
inline void ODEV_STDOUT::RewriteChar(int32 moveLeft,COLORENUM col,const STDSTR& strIn){
	*this << Begin << SetLeft(moveLeft) << SetEL(0) << col;
	if (strIn.length() > 0)
		*this << strIn;
	*this << Endl;
};
//------------------------------------------------------------------------------------------//
inline void ODEV_STDOUT::BKChar(void){
	*this << Begin << SetLeft(1) << SetDCH(1) << Endl;
};
//------------------------------------------------------------------------------------------//
inline void ODEV_STDOUT::DelChar(void){
	*this << Begin << SetDCH(1) << Endl;
};
//------------------------------------------------------------------------------------------//
inline void ODEV_STDOUT::DoCleanLastLine(void){
	*this << Begin << SetEL(2) << SetCNL(1) << SetUp(1) << COL_clDefault << Endl;
};
//------------------------------------------------------------------------------------------//
#endif
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_VC
//------------------------------------------------------------------------------------------//
inline void ODEV_STDOUT::DoCleanLastLine(void){
	Write(nullptr,COL_FE,IUD(" "));
};
//------------------------------------------------------------------------------------------//
inline void	ODEV_STDOUT::ToHome(void){
	Write(nullptr,COL_FF,IUD(" "));
};
//------------------------------------------------------------------------------------------//
#endif
//------------------------------------------------------------------------------------------//
#endif /* ODEV_STDOUT_h */
#endif /* ODEV_STDOUT_hpp */
