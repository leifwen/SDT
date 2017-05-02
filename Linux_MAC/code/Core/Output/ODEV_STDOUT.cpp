/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: ODEV_STDOUT.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2013.7.5
 *			: 2014.1.13 remove support BCB2012
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "ODEV_STDOUT.h"
//------------------------------------------------------------------------------------------//
#ifdef ODEV_SDOUTH
//------------------------------------------------------------------------------------------//
#include "SYS_Time.h"
#include "Comm_Convert.h"
//#define LOGPRINT_ENABLE
//#define LOGPRINT_ENABLE2
#include "Comm_Log.h"

#ifdef CommonDefH_VC
#include "RichView.h"
#endif
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
ODEV_STDOUT::~ODEV_STDOUT(void){
#ifdef CommonDefH_VC
	cgRichEdit = nullptr;
	if (cgCFrm != nullptr)
		*cgCFrm = nullptr;
	cgCFrm = nullptr;
#endif
};
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_Unix
//------------------------------------------------------------------------------------------//
namespace{
	STDSTR ColorConvert(uint32 tCol){
		switch (tCol){
			case	COL_LIN_clDefault:	return(COL_STR_clDefault);
			case	COL_LIN_clBlack:	return(COL_STR_clBlack);
			case	COL_LIN_clRed:		return(COL_STR_clRed);
			case	COL_LIN_clGreen:	return(COL_STR_clGreen);
			case	COL_LIN_clYellow:	return(COL_STR_clYellow);
			case	COL_LIN_clBlue:		return(COL_STR_clBlue);
			case	COL_LIN_clMagenta:	return(COL_STR_clMagenta);
			case	COL_LIN_clCyan:		return(COL_STR_clCyan);
			case	COL_LIN_clGray:		return(COL_STR_clWhite);
				
			case	COL_LIN_clBrightBlack:	return(COL_STR_clBrightBlack);
			case	COL_LIN_clBrightRed:	return(COL_STR_clBrightRed);
			case	COL_LIN_clBrightGreen:	return(COL_STR_clBrightGreen);
			case	COL_LIN_clBrightYellow:	return(COL_STR_clBrightYellow);
			case	COL_LIN_clBrightBlue:	return(COL_STR_clBrightBlue);
			case	COL_LIN_clBrightMagenta:return(COL_STR_clBrightMagenta);
			case	COL_LIN_clBrightCyan:	return(COL_STR_clBrightCyan);
			case	COL_LIN_clBrightGray:	return(COL_STR_clBrightWhite);
			case	COL_LIN_clPurple:		return(COL_STR_clPurple);
			case	COL_LIN_NONE:;
			default:;
		}
		return("");
	}
}
//------------------------------------------------------------------------------------------//
ODEV_STDOUT& ODEV_STDOUT::operator << (const _Left &data){
	if (data.value > 0)
		*cgRecordCache << "\033[" << Str_ToString(data.value) << "\x44";
	return(*this);
}
//------------------------------------------------------------------------------------------//
ODEV_STDOUT& ODEV_STDOUT::operator << (const _Right &data){
	if (data.value > 0)
		*cgRecordCache << "\033[" << Str_ToString(data.value) << "\x43";
	return(*this);
}
//------------------------------------------------------------------------------------------//
ODEV_STDOUT& ODEV_STDOUT::operator << (const _ICH &data){
	if (data.value > 0)
		*cgRecordCache << "\033[" << Str_ToString(data.value) << "\x40";
	return(*this);
}
//------------------------------------------------------------------------------------------//
ODEV_STDOUT& ODEV_STDOUT::operator << (const _DCH &data){
	if (data.value > 0)
		*cgRecordCache << "\033[" << Str_ToString(data.value) << "\x50";
	return(*this);
}
//------------------------------------------------------------------------------------------//
ODEV_STDOUT& ODEV_STDOUT::operator << (const _EL &data){
	*cgRecordCache << "\033[0\x4b";
	return(*this);
}
//------------------------------------------------------------------------------------------//
void ODEV_STDOUT::SetCurLeft(int32 num){
	*this << Start;
	*this << SetLeft(num);
	*this << Endl;
}
//------------------------------------------------------------------------------------------//
void ODEV_STDOUT::SetCurRight(int32 num){
	*this << Start;
	*this << SetRight(num);
	*this << Endl;
}
//------------------------------------------------------------------------------------------//
void ODEV_STDOUT::InsterChar(uint32 col,const uint8* data,uint32 length){
	if (length > 0){
		*this << Start << col;
		*this << SetICH(length) << Data(data,length);
		*this << Endl;
	}
}
//------------------------------------------------------------------------------------------//
void ODEV_STDOUT::BKChar(void){
	*this << Start;
	*this << SetLeft(1) << SetDCH(1);
	*this << Endl;
}
//------------------------------------------------------------------------------------------//
void ODEV_STDOUT::DelChar(void){
	*this << Start;
	*this << SetDCH(1);
	*this << Endl;
}
//------------------------------------------------------------------------------------------//
void ODEV_STDOUT::RewriteChar(int32 moveLeft,uint32 col,const STDSTR &strIn){
	*this << Start << col;
	*this << SetLeft(moveLeft) << SetEL(0);
	if (strIn.length() > 0)
		*this << strIn;
	*this << Endl;
}
//------------------------------------------------------------------------------------------//
void FlushCout(void){
	std::cout.flush();
	SYS_DelayMS(1);
	while (std::cout.bad()){
		SYS_DelayMS(1);
		std::cout.clear();
		std::cout.flush();
	}
}
//------------------------------------------------------------------------------------------//
void ODEV_STDOUT::Print(uint32 col, uint32 ctrl,const uint8 *data,uint32 num){
	uint32  length;
	uint8   *p;
	
	if (num  == 0)
		return;
	
	ctrl &= COLRECORD::CRD_NL;
	
	if ((ctrl != 0) && (cgLastCR == 0))
		std::cout << std::endl;
	
	if ((cgColType == COLType_COL) && (cgLastCOL != col) && (col != COL_NONE))
		std::cout << ColorConvert(col);
	
	p = (uint8*)data;
	if ((*p == '\r') || (*p == '\n')){
		if ((ctrl == 0) && (cgLastCR == 0))
			std::cout << std::endl;
		++ data;
		-- num;
		++ p;
	}
	length  = 0;
	while(++length <= num){
		if (*p == '\r'){
			std::cout.write((char*)data, length - 1) << std::endl;
			FlushCout();
			data += length;
			num -= length;;
			length = 0;
		}
		else if ((*p == '\b') || ((*p == '\n') && (*(p - 1) == '\r'))){
			std::cout.write((char*)data, length - 1);
			FlushCout();
			data += length;
			num -= length;
			length = 0;
		}else if (length > 896){
			std::cout.write((char*)data, length);
			FlushCout();
			data += length;
			num -= length;
			length = 0;
		}
		++ p;
	}
	std::cout.write((char*)data, length - 1);
	FlushCout();
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_VC
//------------------------------------------------------------------------------------------//
COLORREF ColorConvert(uint32 tCol){
	switch (tCol){
		case	COL_clBlack:	return(COL_RGB_clBlack);
		case	COL_clMaroon:	return(COL_RGB_clMaroon);
		case	COL_clDGreen:	return(COL_RGB_clDGreen);
		case	COL_clDYellow:	return(COL_RGB_clDYellow);
		case	COL_clDBlue:	return(COL_RGB_clDBlue);
		case	COL_clDMagenta:	return(COL_RGB_clDMagenta);
		case	COL_clDCyan:	return(COL_RGB_clDCyan);
		case	COL_clDGray:	return(COL_RGB_clDGray);
			
		case	COL_clGray:		return(COL_RGB_clGray);
		case	COL_clRed:		return(COL_RGB_clRed);
		case	COL_clGreen:	return(COL_RGB_clGreen);
		case	COL_clYellow:	return(COL_RGB_clDYellow);
		case	COL_clBlue:		return(COL_RGB_clBlue);
		case	COL_clMagenta:	return(COL_RGB_clMagenta);
		case	COL_clCyan:		return(COL_RGB_clCyan);
		case	COL_clWhite:	return(COL_RGB_clWhite);
		case	COL_clPurple:	return(COL_RGB_clPurple);;
		case	COL_NONE:;
		case	COL_clDefault:	return(COL_RGB_clDefault);
		default:;
	}
	return(COL_RGB_clDefault);
}
//------------------------------------------------------------------------------------------//
void ODEV_STDOUT::Init(const void *tRichEdit,const void **tCFrm,COLType tColType){
	cgRichEdit = (CMyRichView*)tRichEdit;
	cgCFrm = (void**)tCFrm;
	cgColType = tColType;
};
//------------------------------------------------------------------------------------------//
void ODEV_STDOUT::SetCurFromEnd(int32 offset){
	if (cgRichEdit != nullptr)
		cgRichEdit->SetCurFromEnd(offset);
}
//------------------------------------------------------------------------------------------//
void ODEV_STDOUT::DelCharFromEnd(int32 offset,int32 length){
	if (cgRichEdit != nullptr)
		cgRichEdit->DelCharFromEnd(offset, length);
}
//------------------------------------------------------------------------------------------//
void ODEV_STDOUT::RewriteCharFromEnd(int32 offset,uint32 col,const STDSTR &strIn){
	if (cgRichEdit != nullptr)
		cgRichEdit->RewriteCharFromEnd(offset, ColorConvert(col), strIn);
}
//------------------------------------------------------------------------------------------//
void ODEV_STDOUT::InsterCharFromEnd(int32 offset,uint32 col,const STDSTR &strIn){
	if (cgRichEdit != nullptr)
		cgRichEdit->InsterCharFromEnd(offset, ColorConvert(col),strIn);
}
//------------------------------------------------------------------------------------------//
void ODEV_STDOUT::AppendChar(uint32 col,const STDSTR &strIn){
	if (cgRichEdit != nullptr)
		cgRichEdit->AppendChar(ColorConvert(col),strIn);
}
//------------------------------------------------------------------------------------------//
void ODEV_STDOUT::Clean(G_LOCK blLock){
	if (cgRichEdit != nullptr)
		cgRichEdit->Clean(blLock);
}
//------------------------------------------------------------------------------------------//
void ODEV_STDOUT::Print(uint32 col, uint32 ctrl,const uint8 *data, uint32 num){
	STDSTR	strOut;
	uint32  length;
	uint8   *p;

	if (num  == 0)
		return;

	if (col == COL_FF){
		if (cgRichEdit != nullptr)
			cgRichEdit->ToHome();
		return;
	}

	ctrl &= COLRECORD::CRD_NL;
	
	strOut = "";
	if ((ctrl != 0) && (cgLastCR == 0))
		strOut = "\n";
	
	p = (uint8*)data;
	if ((*p == '\r') || (*p == '\n')){
		if ((ctrl == 0) && (cgLastCR == 0))
			strOut = "\n";
		++ data;
		-- num;
		++ p;
	}
	length  = 0;
	while(++length <= num){
		if (*p == '\r'){
			Str_CharToStr(&strOut, data, length - 1, G_ASCII, G_ESCAPE_OFF, G_SPACE_OFF, G_APPEND_ON);
			strOut += "\n";
			data += length;
			num -= length;;
			length = 0;
		}
		else if ((*p == '\0') || (*p == '\b') || ((*p == '\n') && (*(p - 1) == '\r'))){
			Str_CharToStr(&strOut, data, length - 1, G_ASCII, G_ESCAPE_OFF, G_SPACE_OFF, G_APPEND_ON);
			data += length;
			num -= length;
			length = 0;
		}
		++ p;
	}
	Str_CharToStr(&strOut, data, length - 1, G_ASCII, G_ESCAPE_OFF, G_SPACE_OFF, G_APPEND_ON);
	
	AppendChar(col,strOut);
}
//------------------------------------------------------------------------------------------//
#endif
#endif
