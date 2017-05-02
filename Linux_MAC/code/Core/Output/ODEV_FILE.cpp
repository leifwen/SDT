/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: ODEV_FILE.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2013.7.5
 *			: 2014.1.13 remove support BCB2012
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "ODEV_FILE.h"
//------------------------------------------------------------------------------------------//
#ifdef ODEV_FILEH
//------------------------------------------------------------------------------------------//
#include "SYS_Time.h"
#include "Comm_Convert.h"
#include "Comm_File.h"
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_Unix
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#endif
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
void ODEV_FILE::Init(const STDSTR &filename,uint64 tColType){
	Spin_InUse_set();
	cgfileName = filename;
	cgContentRAW = "";
	cgContentTXT = "";
	cgContentRTF = "";
	SYS_Delay_SetTS(&cgTimeS, FREQUENCY);
	ClrSFlag(of_blRAW | of_blTXT | of_blRTF);
	SetSFlag(tColType);
	Spin_InUse_clr();
};
//------------------------------------------------------------------------------------------//
uint32 ODEV_FILE::CheckPrint(uint32 addr)const{
	if (COLRECORD::GetGroup(cgAddress) == COLRECORD::CRD_G3)
		return (OUTPUT_NODE::CheckPrint(addr));
	if (COLRECORD::GetGroup(cgAddress) == COLRECORD::CRD_G2){
		if (COLRECORD::CheckGroup(COLRECORD::CRD_G3,addr) == 0)
			return 0;
	}
	return 1;
};
//------------------------------------------------------------------------------------------//
void ODEV_FILE::Print(uint32 col, uint32 ctrl,const uint8 *data, uint32 num){
	if (CheckSFlag(of_blRAW) != 0)
		PrintRAW(col,ctrl,data,num);
	if (CheckSFlag(of_blTXT) != 0)
		PrintTXT(col,ctrl,data,num);
	if (CheckSFlag(of_blRTF) != 0)
		PrintRTF(col,ctrl,data,num);
	if (SYS_Delay_CheckTS(&cgTimeS) != 0)
		SYS_Delay_SetTS(&cgTimeS, FREQUENCY);
}
//------------------------------------------------------------------------------------------//
void ODEV_FILE::PrintRTF(uint32 col, uint32 ctrl,const uint8 *data, uint32 num){
	STDSTR	strName;
	uint32  length;
	uint8   *p;
	
	Spin_InUse_set();
	
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
				Str_CharToStr(&cgContentRTF, data, length - 1, G_ASCII, G_ESCAPE_OFF, G_SPACE_OFF, G_APPEND_ON);
				cgContentRTF += "\\\\";
				data += length;
				num -= length;;
				length = 0;
			}
			else if (*p == '}'){
				Str_CharToStr(&cgContentRTF, data, length - 1, G_ASCII, G_ESCAPE_OFF, G_SPACE_OFF, G_APPEND_ON);
				cgContentRTF += "  \\}";
				data += length;
				num -= length;;
				length = 0;
			}
			else if (*p == '{'){
				Str_CharToStr(&cgContentRTF, data, length - 1, G_ASCII, G_ESCAPE_OFF, G_SPACE_OFF, G_APPEND_ON);
				cgContentRTF += "  \\{";
				data += length;
				num -= length;;
				length = 0;
			}
			else if (*p == '\r'){
				Str_CharToStr(&cgContentRTF, data, length - 1, G_ASCII, G_ESCAPE_OFF, G_SPACE_OFF, G_APPEND_ON);
				cgContentRTF += RICH_PARL;
				data += length;
				num -= length;;
				length = 0;
			}
			else if ((*p == '\n') && (*(p - 1) == '\r')){
				Str_CharToStr(&cgContentRTF, data, length - 1, G_ASCII, G_ESCAPE_OFF, G_SPACE_OFF, G_APPEND_ON);
				data += length;
				num -= length;
				length = 0;
			}
			else if (*p == '\n'){
				Str_CharToStr(&cgContentRTF, data, length - 1, G_ASCII, G_ESCAPE_OFF, G_SPACE_OFF, G_APPEND_ON);
				cgContentRTF += RICH_PARL;
				data += length;
				num -= length;
				length = 0;
			}
			else if (*p <= 0x1f){
				Str_CharToStr(&cgContentRTF, data, length - 1, G_ASCII, G_ESCAPE_OFF, G_SPACE_OFF, G_APPEND_ON);
				data += length;
				num -= length;
				length = 0;
			}
			++ p;
		}
		Str_CharToStr(&cgContentRTF, data, length - 1, G_ASCII, G_ESCAPE_OFF, G_SPACE_OFF, G_APPEND_ON);
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
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void ODEV_FILE::PrintTXT(uint32 col, uint32 ctrl,const uint8 *data, uint32 num){
	STDSTR	strName;
	uint32  length;
	uint8   *p;
	
	Spin_InUse_set();
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
				Str_CharToStr(&cgContentTXT, data, length - 1, G_ASCII, G_ESCAPE_OFF, G_SPACE_OFF, G_APPEND_ON);
				cgContentTXT += "\r\n";
				data += length;
				num -= length;;
				length = 0;
			}
			else if ((*p == '\n') && (*(p - 1) == '\r')){
				Str_CharToStr(&cgContentTXT, data, length - 1, G_ASCII, G_ESCAPE_OFF, G_SPACE_OFF, G_APPEND_ON);
				data += length;
				num -= length;
				length = 0;
			}
			else if (*p == '\n'){
				Str_CharToStr(&cgContentTXT, data, length - 1, G_ASCII, G_ESCAPE_OFF, G_SPACE_OFF, G_APPEND_ON);
				cgContentTXT += "\r\n";
				data += length;
				num -= length;
				length = 0;
			}
			else if (*p <= 0x1f){
				Str_CharToStr(&cgContentTXT, data, length - 1, G_ASCII, G_ESCAPE_OFF, G_SPACE_OFF, G_APPEND_ON);
				data += length;
				num -= length;
				length = 0;
			}
			++ p;
		}
		Str_CharToStr(&cgContentTXT, data, length - 1, G_ASCII, G_ESCAPE_OFF, G_SPACE_OFF, G_APPEND_ON);
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
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void ODEV_FILE::PrintRAW(uint32 col, uint32 ctrl,const uint8 *data, uint32 num){
	STDSTR	strName;
	Spin_InUse_set();
	
	if (num > 0)
		Str_CharToStr(&cgContentRAW, data, num, G_ASCII, G_ESCAPE_OFF, G_SPACE_OFF, G_APPEND_ON);
	
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
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
STDSTR	ODEV_FILE::CreateFileTime(void){
	TIME 	dtDate;
	dtDate.Now();
	return(dtDate.FormatDateTime("YYYY.MM.DD@hh.mm.ss(zzz)"));
}
//------------------------------------------------------------------------------------------//
STDSTR ODEV_FILE::CreateLOGDIR(void){
#ifdef CommonDefH_Unix
	{
		STDSTR		strfileDir;
		char work_path[256];
		char *path;
		
		path = getcwd(work_path,sizeof(work_path));
		strfileDir = Str_CharToASCIIStr((uint8*)path,(uint32)strlen(path),G_ESCAPE_OFF);
		
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
		strfileDir = Str_CharToASCIIStr((uint8*)path,(uint32)strlen(path),G_ESCAPE_OFF);
		
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
STDSTR ODEV_FILE::CreateNewLOGFileName(const STDSTR &tIP,int32 tPort){
	STDSTR		strName,strTemp,fileDir,strRet,strNameEx;
	int32 i;
	
	fileDir = CreateLOGDIR();
	strNameEx = "_" + tIP + "@" + Str_ToString(tPort);
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
void ODEV_FILE::CreateEmptyRTFFile(const STDSTR &fName){
	STDSTR		strResult;
	
	strResult = RICH_HEAD;
	strResult += RICH_COLOR;
	strResult += RICH_TEXT_HEAD;
	strResult += RICH_END;
	CFS_WriteFile(fName,strResult);
}
//------------------------------------------------------------------------------------------//
void ODEV_FILE::AddToRTFFile(const STDSTR &fName,const STDSTR &content){
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
void ODEV_FILE::AddToTXTFile(const STDSTR &fName,const STDSTR &content){
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
//------------------------------------------------------------------------------------------//
#endif
