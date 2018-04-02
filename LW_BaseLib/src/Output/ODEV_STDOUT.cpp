//
//  ODEV_STDOUT.cpp
//  SDT
//
//  Created by Leif Wen on 31/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
#include "ODEV_STDOUT.h"
//------------------------------------------------------------------------------------------//
#ifdef ODEV_STDOUT_h
//------------------------------------------------------------------------------------------//
#include "SYS_Time.h"
#ifdef CommonDefH_VC
#include "RichView.h"
#endif
//------------------------------------------------------------------------------------------//
ODEV_STDOUT::ODEV_STDOUT(COLType colType,OUTPUT_CACHE* cache,uint32 group) : OUTPUT_NODE(colType,cache,group){
	SetSelfName("ODEV_STDOUT");
#ifdef CommonDefH_VC
	cgRichEdit = nullptr;
#endif
};
//------------------------------------------------------------------------------------------//
ODEV_STDOUT::~ODEV_STDOUT(void){
#ifdef CommonDefH_VC
	cgRichEdit = nullptr;
#endif
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_Unix
//------------------------------------------------------------------------------------------//
namespace{
	STDSTR ColorConvert(COLORENUM col){
		switch (col){
			case	COL_LIN_clDefault:		return(COL_STR_clDefault);
			case	COL_LIN_clBlack:		return(COL_STR_clBlack);
			case	COL_LIN_clRed:			return(COL_STR_clRed);
			case	COL_LIN_clGreen:		return(COL_STR_clGreen);
			case	COL_LIN_clYellow:		return(COL_STR_clYellow);
			case	COL_LIN_clBlue:			return(COL_STR_clBlue);
			case	COL_LIN_clMagenta:		return(COL_STR_clMagenta);
			case	COL_LIN_clCyan:			return(COL_STR_clCyan);
			case	COL_LIN_clGray:			return(COL_STR_clWhite);
				
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
void ODEV_STDOUT::Print(uint32 ctrl,COLORENUM col,const uint8* data,uint32 num){
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
COLORREF ColorConvert(COLORENUM col){
	switch (col){
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
void ODEV_STDOUT::Init(const void* tRichEdit,COLType colType){
	cgRichEdit = (CMyRichView*)tRichEdit;
	cgColType = colType;
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
void ODEV_STDOUT::RewriteCharFromEnd(int32 offset, COLORENUM col, const STDSTR& strIn){
	uint8 data;
	if (cgRichEdit != nullptr){
		cgRichEdit->RewriteCharFromEnd(offset, ColorConvert(col), strIn);
		data = strIn[strIn.length() - 1];
		UpdataLastStatus(col,&data);
	}
}
//------------------------------------------------------------------------------------------//
void ODEV_STDOUT::InsterCharFromEnd(int32 offset, COLORENUM col, const STDSTR& strIn){
	if (cgRichEdit != nullptr)
		cgRichEdit->InsterCharFromEnd(offset, ColorConvert(col),strIn);
}
//------------------------------------------------------------------------------------------//
void ODEV_STDOUT::AppendChar(COLORENUM col, const STDSTR& strIn){
	if (cgRichEdit != nullptr)
		cgRichEdit->AppendChar(ColorConvert(col),strIn);
}
//------------------------------------------------------------------------------------------//
void ODEV_STDOUT::Clean(G_LOCK blLock){
	if (cgRichEdit != nullptr)
		cgRichEdit->Clean(blLock);
}
//------------------------------------------------------------------------------------------//
void ODEV_STDOUT::Print(uint32 ctrl,COLORENUM col,const uint8* data, uint32 num){
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
			Str_CharToStr(&strOut, data, length - 1, G_ASCII, G_SPACE_OFF, G_ESCAPE_OFF);
			strOut += "\n";
			data += length;
			num -= length;;
			length = 0;
		}
		else if ((*p == '\0') || (*p == '\b') || ((*p == '\n') && (*(p - 1) == '\r'))){
			Str_CharToStr(&strOut, data, length - 1, G_ASCII, G_SPACE_OFF, G_ESCAPE_OFF);
			data += length;
			num -= length;
			length = 0;
		}
		++ p;
	}
	Str_CharToStr(&strOut, data, length - 1, G_ASCII, G_SPACE_OFF, G_ESCAPE_OFF);
	
	AppendChar(col,strOut);
}
//------------------------------------------------------------------------------------------//
#endif
//------------------------------------------------------------------------------------------//
#endif /* ODEV_STDOUT_h */
