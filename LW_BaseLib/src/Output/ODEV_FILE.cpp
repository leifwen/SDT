//
//  ODEV_FILE.cpp
//  SDT
//
//  Created by Leif Wen on 31/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
#include "ODEV_FILE.h"
//------------------------------------------------------------------------------------------//
#ifdef ODEV_FILE_h
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_Unix
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#endif
#include "DS_STRING.h"
#include "SYS_File.h"
//------------------------------------------------------------------------------------------//
#ifndef ODEV_LOG_FDIR
#define	ODEV_LOG_FDIR	"_Record"
#endif
//------------------------------------------------------------------------------------------//
#define		RICH_HEAD				"{\\rtf1\\ansi\\ansicpg1252\\deff0\\deflang1033\\deflangfe2052{\\fonttbl{\\f0\\fnil\\fcharset134 Courier New;}}\r\n"
#define		RICH_TEXT_HEAD			"\\viewkind4\\uc1\\pard\\lang2052\\f0\\fs20 "
#define		RICH_END				"}\r\n"
#define		RICH_PARL				"\\par\r\n"
#define		RICH_COLOR				"{\\colortbl;\
\\red0\\green0\\blue0;\
\\red127\\green0\\blue0;\
\\red0\\green160\\blue0;\
\\red160\\green160\\blue0;\
\\red0\\green0\\blue160;\
\\red160\\green0\\blue160;\
\\red0\\green160\\blue160;\
\\red192\\green192\\blue192;\
\\red128\\green128\\blue128;\
\\red255\\green0\\blue0;\
\\red0\\green255\\blue0;\
\\red255\\green255\\blue0;\
\\red0\\green0\\blue255;\
\\red255\\green0\\blue255;\
\\red0\\green255\\blue255;\
\\red255\\green255\\blue255;\
\\red127\\green0\\blue127;\
}\r\n"
//------------------------------------------------------------------------------------------//
#define		COL_CF_clDefault	COL_CF_clBlack

#define		COL_CF_clBlack		"\\cf1 "
#define		COL_CF_clMaroon		"\\cf2 "
#define		COL_CF_clDGreen		"\\cf3 "
#define		COL_CF_clDYellow	"\\cf4 "
#define		COL_CF_clDBlue		"\\cf5 "
#define		COL_CF_clDMagenta	"\\cf6 "
#define		COL_CF_clDCyan		"\\cf7 "
#define		COL_CF_clDGray		"\\cf8 "

#define		COL_CF_clGray		"\\cf9 "
#define		COL_CF_clRed		"\\cf10 "
#define		COL_CF_clGreen		"\\cf11 "
#define		COL_CF_clYellow		"\\cf12 "
#define		COL_CF_clBlue		"\\cf13 "
#define		COL_CF_clMagenta	"\\cf14 "
#define		COL_CF_clCyan		"\\cf15 "
#define		COL_CF_clWhite		"\\cf16 "

#define		COL_CF_clPurple		"\\cf17 "
//------------------------------------------------------------------------------------------//
namespace  {
	STDSTR ColorConvert(uint32 tCol){
		switch (tCol){
			case	COL_clBlack:	return(COL_CF_clBlack);
			case	COL_clMaroon:	return(COL_CF_clMaroon);
			case	COL_clDGreen:	return(COL_CF_clDGreen);
			case	COL_clDYellow:	return(COL_CF_clDYellow);
			case	COL_clDBlue:	return(COL_CF_clDBlue);
			case	COL_clDMagenta:	return(COL_CF_clDMagenta);
			case	COL_clDCyan:	return(COL_CF_clDCyan);
			case	COL_clDGray:	return(COL_CF_clDGray);
				
			case	COL_clGray:		return(COL_CF_clGray);
			case	COL_clRed:		return(COL_CF_clRed);
			case	COL_clGreen:	return(COL_CF_clGreen);
			case	COL_clYellow:	return(COL_CF_clYellow);
			case	COL_clBlue:		return(COL_CF_clBlue);
			case	COL_clMagenta:	return(COL_CF_clMagenta);
			case	COL_clCyan:		return(COL_CF_clCyan);
			case	COL_clWhite:	return(COL_CF_clWhite);
			case	COL_clPurple:	return(COL_CF_clPurple);
			case	COL_NONE:;
			case	COL_clDefault:	return(COL_CF_clDefault);
		}
		return("");
	}
};
//------------------------------------------------------------------------------------------//
ODEV_FILE::ODEV_FILE(OUTPUT_CACHE* cache,uint32 group) : OUTPUT_NODE(COLType_RAW,cache,group){
	
	Init("",ODEV_blRTF);
	SetSelfName("ODEV_FILE");
};
//------------------------------------------------------------------------------------------//
void ODEV_FILE::Init(const STDSTR& filename,uint64 colType){
	InUse_set();
	cgfileName = filename;
	cgContentRAW = "";
	cgContentTXT = "";
	cgContentRTF = "";
	SYS_Delay_SetTS(&cgTimeS, FREQUENCY);
	ClrSFlag(ODEV_blRAW | ODEV_blTXT | ODEV_blRTF);
	SetSFlag(colType);
	InUse_clr();
};
//------------------------------------------------------------------------------------------//
bool32 ODEV_FILE::CheckPrint(uint32 ctrl)const{
	if (GetGroup() == CRD::CRD_G3)
		return (OUTPUT_NODE::CheckPrint(ctrl));
	if (GetGroup() == CRD::CRD_G2){
		if (CRD::CheckGroup(CRD::CRD_G3,ctrl) == 0)
			return G_FALSE;
	}
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
void ODEV_FILE::Print(uint32 ctrl,COLORENUM col,const uint8* data, uint32 num){
	if (CheckSFlag(ODEV_blRAW))
		PrintRAW(ctrl,col,data,num);
	if (CheckSFlag(ODEV_blTXT))
		PrintTXT(ctrl,col,data,num);
	if (CheckSFlag(ODEV_blRTF))
		PrintRTF(ctrl,col,data,num);
	if (SYS_Delay_CheckTS(&cgTimeS) != 0)
		SYS_Delay_SetTS(&cgTimeS, FREQUENCY);
}
//------------------------------------------------------------------------------------------//
void ODEV_FILE::PrintRTF(uint32 ctrl,COLORENUM col,const uint8* data, uint32 num){
	STDSTR	strName;
	uint32  length;
	uint8   *p;
	
	InUse_set();
	
	if (num > 0){
		ctrl &= COLRECORD::CRD_NL;
		
		if ((cgLastCOL != col) && (col != COL_NONE))
			cgContentRTF += ColorConvert(col);
		
		if ((ctrl != 0) && (cgLastCR == 0))
			cgContentRTF += RICH_PARL;
		
		p = (uint8*)data;
		if ((*p == '\r') || (*p == '\n')){
			if ((ctrl == 0) && (cgLastCR == 0))
				cgContentRTF += RICH_PARL;
			++ data;
			-- num;
			++ p;
		}
		
		length  = 0;
		while(++length <= num){
			if (*p == '\\'){
				Str_CharToStr(&cgContentRTF, data, length - 1, G_ASCII, G_SPACE_OFF, G_ESCAPE_OFF);
				cgContentRTF += "\\\\";
				data += length;
				num -= length;;
				length = 0;
			}
			else if (*p == '}'){
				Str_CharToStr(&cgContentRTF, data, length - 1, G_ASCII, G_SPACE_OFF, G_ESCAPE_OFF);
				cgContentRTF += "  \\}";
				data += length;
				num -= length;;
				length = 0;
			}
			else if (*p == '{'){
				Str_CharToStr(&cgContentRTF, data, length - 1, G_ASCII, G_SPACE_OFF, G_ESCAPE_OFF);
				cgContentRTF += "  \\{";
				data += length;
				num -= length;;
				length = 0;
			}
			else if (*p == '\r'){
				Str_CharToStr(&cgContentRTF, data, length - 1, G_ASCII, G_SPACE_OFF, G_ESCAPE_OFF);
				cgContentRTF += RICH_PARL;
				data += length;
				num -= length;;
				length = 0;
			}
			else if ((*p == '\n') && (*(p - 1) == '\r')){
				Str_CharToStr(&cgContentRTF, data, length - 1, G_ASCII, G_SPACE_OFF, G_ESCAPE_OFF);
				data += length;
				num -= length;
				length = 0;
			}
			else if (*p == '\n'){
				Str_CharToStr(&cgContentRTF, data, length - 1, G_ASCII, G_SPACE_OFF, G_ESCAPE_OFF);
				cgContentRTF += RICH_PARL;
				data += length;
				num -= length;
				length = 0;
			}
			else if (*p <= 0x1f){
				Str_CharToStr(&cgContentRTF, data, length - 1, G_ASCII, G_SPACE_OFF, G_ESCAPE_OFF);
				data += length;
				num -= length;
				length = 0;
			}
			++ p;
		}
		Str_CharToStr(&cgContentRTF, data, length - 1, G_ASCII, G_SPACE_OFF, G_ESCAPE_OFF);
	}
	if ((cgContentRTF.length() > UNWRITESIZE) || (SYS_Delay_CheckTS(&cgTimeS) != 0)){
#ifdef CommonDefH_VC
		strName = CreateLOGDIR() + "\\" + cgfileName + ".rtf";
#endif
#ifdef CommonDefH_Unix
		strName = CreateLOGDIR() + "/" + cgfileName + ".rtf";
#endif
		AddToRTFFile(strName,cgContentRTF);
		cgContentRTF = "";
	}
	InUse_clr();
}
//------------------------------------------------------------------------------------------//
void ODEV_FILE::PrintTXT(uint32 ctrl,COLORENUM col,const uint8* data, uint32 num){
	STDSTR	strName;
	uint32  length;
	uint8   *p;
	
	InUse_set();
	if (num > 0){
		ctrl &= COLRECORD::CRD_NL;
		
		if ((ctrl != 0) && (cgLastCR == 0))
			cgContentTXT += "\r\n";
		
		p = (uint8*)data;
		if ((*p == '\r') || (*p == '\n')){
			if ((ctrl == 0) && (cgLastCR == 0))
				cgContentTXT += "\r\n";
			++ data;
			-- num;
			++ p;
		}
		
		length  = 0;
		while(++length <= num){
			if (*p == '\r'){
				Str_CharToStr(&cgContentTXT, data, length - 1, G_ASCII, G_SPACE_OFF, G_ESCAPE_OFF);
				cgContentTXT += "\r\n";
				data += length;
				num -= length;;
				length = 0;
			}
			else if ((*p == '\n') && (*(p - 1) == '\r')){
				Str_CharToStr(&cgContentTXT, data, length - 1, G_ASCII, G_SPACE_OFF, G_ESCAPE_OFF);
				data += length;
				num -= length;
				length = 0;
			}
			else if (*p == '\n'){
				Str_CharToStr(&cgContentTXT, data, length - 1, G_ASCII, G_SPACE_OFF, G_ESCAPE_OFF);
				cgContentTXT += "\r\n";
				data += length;
				num -= length;
				length = 0;
			}
			else if (*p <= 0x1f){
				Str_CharToStr(&cgContentTXT, data, length - 1, G_ASCII, G_SPACE_OFF, G_ESCAPE_OFF);
				data += length;
				num -= length;
				length = 0;
			}
			++ p;
		}
		Str_CharToStr(&cgContentTXT, data, length - 1, G_ASCII, G_SPACE_OFF, G_ESCAPE_OFF);
	}
	if ((cgContentTXT.length() > UNWRITESIZE) || (SYS_Delay_CheckTS(&cgTimeS) != 0)){
#ifdef CommonDefH_VC
		strName = CreateLOGDIR() + "\\" + cgfileName + ".txt";
#endif
#ifdef CommonDefH_Unix
		strName = CreateLOGDIR() + "/" + cgfileName + ".txt";
#endif
		AddToTXTFile(strName,cgContentTXT);
		cgContentTXT = "";
	}
	InUse_clr();
}
//------------------------------------------------------------------------------------------//
void ODEV_FILE::PrintRAW(uint32 ctrl,COLORENUM col,const uint8* data, uint32 num){
	STDSTR	strName;
	InUse_set();
	
	if (num > 0)
		Str_CharToStr(&cgContentRAW, data, num, G_ASCII, G_SPACE_OFF, G_ESCAPE_OFF);
	
	if ((cgContentRAW.length() > UNWRITESIZE) || (SYS_Delay_CheckTS(&cgTimeS) != 0)){
#ifdef CommonDefH_VC
		strName = CreateLOGDIR() + "\\" + cgfileName + ".bin";
#endif
#ifdef CommonDefH_Unix
		strName = CreateLOGDIR() + "/" + cgfileName + ".bin";
#endif
		AddToTXTFile(strName,cgContentRAW);
		cgContentRAW = "";
	}
	InUse_clr();
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
STDSTR	ODEV_FILE::CreateFileTime(void){
	DTIME 	dtDate;
	dtDate.Now();
	return(dtDate.Format("YYYY.MM.DD@hh.mm.ss(zzz)"));
}
//------------------------------------------------------------------------------------------//
STDSTR ODEV_FILE::CreateLOGDIR(void){
#ifdef CommonDefH_Unix
	{
		STDSTR		strfileDir;
		char work_path[256];
		char *path;
		
		path = getcwd(work_path,sizeof(work_path));
		strfileDir = Str_CharToASCII((uint8*)path,(uint32)strlen(path),G_ESCAPE_OFF);
		
		strfileDir += "/";
		strfileDir += ODEV_LOG_FDIR;
		if (access(strfileDir.c_str(),0) == -1)
			mkdir(strfileDir.c_str(),0777);
		return(strfileDir);
	}
#endif
#ifdef CommonDefH_VC
	{
		TCHAR			path[MAX_PATH];
		CString			fileDir;
		std::wstring	strfileDir;
		
		GetCurrentDirectory(MAX_PATH,path);
		fileDir = path;
		fileDir += "\\";
		fileDir += _T(ODEV_LOG_FDIR);
		
		if (!PathIsDirectory(fileDir))
			CreateDirectory(fileDir,nullptr);
		strfileDir = fileDir;
		return(Str_UnicodeToANSI(strfileDir));
	}
#endif
}
//------------------------------------------------------------------------------------------//
STDSTR ODEV_FILE::GetLOGDIR(void){
#ifdef CommonDefH_Unix
	{
		STDSTR		strfileDir;
		char work_path[256];
		char *path;
		
		path = getcwd(work_path,sizeof(work_path));
		strfileDir = Str_CharToASCII((uint8*)path,(uint32)strlen(path),G_ESCAPE_OFF);
		
		strfileDir += "/";
		strfileDir += ODEV_LOG_FDIR;
		return(strfileDir);
	}
#endif
#ifdef CommonDefH_VC
	{
		TCHAR			path[MAX_PATH];
		CString			fileDir;
		std::wstring	strfileDir;
		
		GetCurrentDirectory(MAX_PATH,path);
		fileDir = path;
		fileDir += "\\";
		fileDir += _T(ODEV_LOG_FDIR);
		
		strfileDir = fileDir;
		return(Str_UnicodeToANSI(strfileDir));
	}
#endif
}
//------------------------------------------------------------------------------------------//
STDSTR ODEV_FILE::CreateNewLOGFileName(void){
	STDSTR		strName,strTemp,fileDir,strRet;
	int32 i;
	
	fileDir = CreateLOGDIR();
	i = 100;
	while(-- i > 0){
		strRet = CreateFileTime();
#ifdef CommonDefH_Unix
		strName = fileDir + "/" + strRet;
		strTemp = strName + ".txt";
		if (access(strName.c_str(),0) == -1){
			strTemp = strName + ".rtf";
			if (access(strName.c_str(),0) == -1){
				strTemp = strName + ".bin";
				if (access(strName.c_str(),0) == -1)
					break;
			}
		}
#endif
#ifdef CommonDefH_VC
		CString			cName;
		strName = fileDir + "\\" + strRet;
		strTemp = strName + ".txt";
		cName = Str_ANSIToUnicode(strTemp).c_str();
		if (!PathFileExists(cName)){
			strTemp = strName + ".rtf";
			cName = Str_ANSIToUnicode(strTemp).c_str();
			if (!PathFileExists(cName)){
				strTemp = strName + ".bin";
				cName = Str_ANSIToUnicode(strTemp).c_str();
				if (!PathFileExists(cName))
					break;
			}
		}
#endif
	}
	return(strRet);
}
//------------------------------------------------------------------------------------------//
STDSTR ODEV_FILE::CreateNewLOGFileName(const STDSTR& tIP,int32 tPort){
	STDSTR		strName,strTemp,fileDir,strRet,strNameEx;
	int32 i;
	
	fileDir = CreateLOGDIR();
	strNameEx = "_" + tIP + "@" + Str_ToStr(tPort);
	i = 100;
	while(-- i > 0){
		strRet = CreateFileTime() + strNameEx;
#ifdef CommonDefH_Unix
		strName = fileDir + "/" + strRet;
		strTemp = strName + ".txt";
		if (access(strName.c_str(),0) == -1){
			strTemp = strName + ".rtf";
			if (access(strName.c_str(),0) == -1){
				strTemp = strName + ".bin";
				if (access(strName.c_str(),0) == -1)
					break;
			}
		}
#endif
#ifdef CommonDefH_VC
		CString			cName;
		strName = fileDir + "\\" + strRet;
		strTemp = strName + ".txt";
		cName = Str_ANSIToUnicode(strTemp).c_str();
		if (!PathFileExists(cName)){
			strTemp = strName + ".rtf";
			cName = Str_ANSIToUnicode(strTemp).c_str();
			if (!PathFileExists(cName)){
				strTemp = strName + ".bin";
				cName = Str_ANSIToUnicode(strTemp).c_str();
				if (!PathFileExists(cName))
					break;
			}
		}
#endif
	}
	return(strRet);
}
//------------------------------------------------------------------------------------------//
void ODEV_FILE::CreateEmptyRTFFile(const STDSTR& fName){
	STDSTR		strResult;
	
	strResult = RICH_HEAD;
	strResult += RICH_COLOR;
	strResult += RICH_TEXT_HEAD;
	strResult += RICH_END;
	CFS_WriteFile(fName,strResult);
}
//------------------------------------------------------------------------------------------//
void ODEV_FILE::AddToRTFFile(const STDSTR& fName,const STDSTR& content){
	std::fstream	fileStream;
	STDSTR			fileName;
	
	if (content.length() == 0)
		return;
	
	if (CFS_CheckFile(fName) == 0)
		CreateEmptyRTFFile(fName);
	
	fileStream.open(fName.c_str(),std::ios::in|std::ios::out|std::ios::binary);
	fileStream.seekp(-3,std::ios::end);
	
	fileStream << content << RICH_END;
	fileStream.flush();
	fileStream.close();
}
//------------------------------------------------------------------------------------------//
void ODEV_FILE::AddToTXTFile(const STDSTR& fName,const STDSTR& content){
	if (content.length() == 0)
		return;
	
	if (CFS_CheckFile(fName) == 0){
		CFS_WriteFile(fName, content);
	}
	else{
		CFS_AddToFile(fName, content);
	}
}
//------------------------------------------------------------------------------------------//
#endif /* ODEV_FILE_h */
