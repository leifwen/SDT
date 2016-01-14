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
#include "Comm_Convert.h"
#include "Comm_File.h"
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_Unix
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#endif
//------------------------------------------------------------------------------------------//
int32 ODEV_NODE_FILE::Print(G_LOCK_VAILD blLock){
	std::string	strName,strT;
	
	Spin_InUse_set(G_LOCK_ON);
#ifdef CommonDefH_VC
	strName = ODEV_CreateLOGDIR() + "\\" + cgfileName;
#endif
#ifdef CommonDefH_Unix
	strName = ODEV_CreateLOGDIR() + "/" + cgfileName;
#endif
	Spin_InUse_clr(G_LOCK_ON);
	if ((GetUnreadLength(G_LOCK_ON) > 1024 * 8) || (SYS_Delay_CheckTS(&cgTimeS) != 0)){
		SYS_Delay_SetTS(&cgTimeS, 1000);
		if (cgCOLType == COLType_COL){
			ODEV_AddToRTFFile(strName, ReadStr(&strT,G_LOCK_OFF));
		}
		else if (cgCOLType == COLType_TXT){
			ODEV_AddToTXTFile(strName, ReadStr(&strT,G_LOCK_OFF));
		}
	}
	return 1;
}
//------------------------------------------------------------------------------------------//
std::string	ODEV_CreateFileTime(void){
	SYS_DateTime 		dtDate;
	dtDate.Now();
	return(dtDate.FormatDateTime("YYYY.MM.DD@hh.mm.ss(zzz)"));
}
//------------------------------------------------------------------------------------------//
std::string ODEV_CreateLOGDIR(void){
#ifdef CommonDefH_Unix
	{
		std::string		strfileDir;
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
std::string ODEV_GetLOGDIR(void){
#ifdef CommonDefH_Unix
	{
		std::string		strfileDir;
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
std::string ODEV_CreateNewLOGFileName(void){
	std::string		strName,strTemp,fileDir,strRet;
	int32 i;
	
	fileDir = ODEV_CreateLOGDIR();
	i = 100;
	while(-- i > 0){
		strRet = ODEV_CreateFileTime();
#ifdef CommonDefH_Unix
		strName = fileDir + "/" + strRet;
		strTemp = strName + ".txt";
		if (access(strName.c_str(),0) == -1){
			strTemp = strName + ".rtf";
			if (access(strName.c_str(),0) == -1)
				break;
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
			if (!PathFileExists(cName))
				break;
		}
#endif
	}
	return(strRet);
}
//------------------------------------------------------------------------------------------//
std::string ODEV_CreateNewLOGFileName(const std::string &tIP,int32 tPort){
	std::string		strName,strTemp,fileDir,strRet,strNameEx;
	int32 i;
	
	fileDir = ODEV_CreateLOGDIR();
	strNameEx = "_" + tIP + "@" + Str_IntToString(tPort);
	i = 100;
	while(-- i > 0){
		strRet = ODEV_CreateFileTime() + strNameEx;
#ifdef CommonDefH_Unix
		strName = fileDir + "/" + strRet;
		strTemp = strName + ".txt";
		if (access(strName.c_str(),0) == -1){
			strTemp = strName + ".rtf";
			if (access(strName.c_str(),0) == -1)
				break;
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
			if (!PathFileExists(cName))
				break;
		}
#endif
	}
	return(strRet);
}
//------------------------------------------------------------------------------------------//


//------------------------------------------------------------------------------------------//
void ODEV_AddToTXTFile(const std::string &fName,const std::string &content){
	std::string	strName, stringOut, str0;
	if (content.length() == 0)
		return;
	strName = fName + ".txt";
	
	stringOut = Str_Replace(content, "\r\n", "\r");
	stringOut = Str_Replace(stringOut, "\n", "\r");
	stringOut = Str_Replace(stringOut, "\r", "\r\n");
	str0 = '\0';
	stringOut = Str_Replace(stringOut, str0, "\\0");
	
	if (CFS_CheckFile(strName) == 0){
		CFS_WriteFile(strName, stringOut);
	}
	else{
		CFS_AddToFile(strName, stringOut);
	}
}
//------------------------------------------------------------------------------------------//
void ODEV_CreateEmptyRTFFile(const std::string &fName){
	std::string		strResult;
	
	strResult = RICH_HEAD;
	strResult += RICH_COLOR;
	strResult += RICH_TEXT_HEAD;
	strResult += RICH_END;
	CFS_WriteFile(fName + ".rtf",strResult);
}
//------------------------------------------------------------------------------------------//
void ODEV_FormatString(std::string *returnStr,const std::string &strInput){
	std::string::size_type		i,length;
	uint8	charData;
	
	length = strInput.length();
	
	i = 0;
	*returnStr = "";
	
	while(i < length){
		charData = strInput[i];
		if (charData == '\\'){
			*returnStr += "\\\\";
		}
		else if (charData == '}'){	//\}
			*returnStr += "  \\}";
		}
		else if (charData == '{'){	//\}
			*returnStr += "  \\{";
		}
		else{
			*returnStr += charData;
		}
		++ i;
	}
}
//------------------------------------------------------------------------------------------//
void ODEV_AddToRTFFile(const std::string &fName,const std::string &content){
	std::fstream	fileStream;
	std::string		fileName,stringTemp,stringOut,stringTemp1,strColor,str0;
	
	std::string	strName;
	if (content.length() == 0)
		return;
	strName = fName + ".rtf";
	
	if (CFS_CheckFile(strName) == 0)
		ODEV_CreateEmptyRTFFile(fName);
	
	fileStream.open(strName.c_str(),std::ios::in|std::ios::out|std::ios::binary);
	fileStream.seekp(-3,std::ios::end);
	
	stringTemp = content;
	while(stringTemp.length() > 0){
		stringOut = Str_ReadSubItem(&stringTemp,",");
		if (stringOut.length() == 0)
			continue;
		if (stringOut == RICH_CF_clBlack){
			if (strColor != stringOut){
				strColor  = stringOut;
				fileStream << RICH_CF_clBlack;
			}
			continue;
		}
		if (stringOut == RICH_CF_clMaroon){
			if (strColor != stringOut){
				strColor  = stringOut;
				fileStream << RICH_CF_clMaroon;
			}
			continue;
		}
		if (stringOut == RICH_CF_clDGreen){
			if (strColor != stringOut){
				strColor  = stringOut;
				fileStream << RICH_CF_clDGreen;
			}
			continue;
		}
		if (stringOut == RICH_CF_clDBrown){
			if (strColor != stringOut){
				strColor  = stringOut;
				fileStream << RICH_CF_clDBrown;
			}
			continue;
		}
		if (stringOut == RICH_CF_clDBlue){
			if (strColor != stringOut){
				strColor  = stringOut;
				fileStream << RICH_CF_clDBlue;
			}
			continue;
		}
		if (stringOut == RICH_CF_clDMagenta){
			if (strColor != stringOut){
				strColor  = stringOut;
				fileStream << RICH_CF_clDMagenta;
			}
			continue;
		}
		if (stringOut == RICH_CF_clDCyan){
			if (strColor != stringOut){
				strColor  = stringOut;
				fileStream << RICH_CF_clDCyan;
			}
			continue;
		}
		if (stringOut == RICH_CF_clDGray){
			if (strColor != stringOut){
				strColor  = stringOut;
				fileStream << RICH_CF_clDGray;
			}
			continue;
		}
		if (stringOut == RICH_CF_clGray){
			if (strColor != stringOut){
				strColor  = stringOut;
				fileStream << RICH_CF_clGray;
			}
			continue;
		}
		if (stringOut == RICH_CF_clRed){
			if (strColor != stringOut){
				strColor  = stringOut;
				fileStream << RICH_CF_clRed;
			}
			continue;
		}
		if (stringOut == RICH_CF_clGreen){
			if (strColor != stringOut){
				strColor  = stringOut;
				fileStream << RICH_CF_clGreen;
			}
			continue;
		}
		if (stringOut == RICH_CF_clYellow){
			if (strColor != stringOut){
				strColor  = stringOut;
				fileStream << RICH_CF_clYellow;
			}
			continue;
		}
		if (stringOut == RICH_CF_clBlue){
			if (strColor != stringOut){
				strColor  = stringOut;
				fileStream << RICH_CF_clBlue;
			}
			continue;
		}
		if (stringOut == RICH_CF_clMagenta){
			if (strColor != stringOut){
				strColor  = stringOut;
				fileStream << RICH_CF_clMagenta;
			}
			continue;
		}
		if (stringOut == RICH_CF_clCyan){
			if (strColor != stringOut){
				strColor  = stringOut;
				fileStream << RICH_CF_clCyan;
			}
			continue;
		}
		if (stringOut == RICH_CF_clPurple){
			if (strColor != stringOut){
				strColor  = stringOut;
				fileStream << RICH_CF_clPurple;
			}
			continue;
		}
		if (stringOut == RICH_CF_clWhite){
			if (strColor != stringOut){
				strColor  = stringOut;
				fileStream << RICH_CF_clWhite;
			}
			continue;
		}
		
		if (stringOut == RICH_LIN_clDefault){
			if (strColor != stringOut){
				strColor  = stringOut;
				fileStream << RICH_CF_clBlack;
			}
			continue;
		}
		if (stringOut == RICH_LIN_clBlack){
			if (strColor != stringOut){
				strColor  = stringOut;
				fileStream << RICH_CF_clBlack;
			}
			continue;
		}
		if (stringOut == RICH_LIN_clRed){
			if (strColor != stringOut){
				strColor  = stringOut;
				fileStream << RICH_CF_clMaroon;
			}
			continue;
		}
		if (stringOut == RICH_LIN_clGreen){
			if (strColor != stringOut){
				strColor  = stringOut;
				fileStream << RICH_CF_clDGreen;
			}
			continue;
		}
		if (stringOut == RICH_LIN_clBrown){
			if (strColor != stringOut){
				strColor  = stringOut;
				fileStream << RICH_CF_clDBrown;
			}
			continue;
		}
		if (stringOut == RICH_LIN_clBlue){
			if (strColor != stringOut){
				strColor  = stringOut;
				fileStream << RICH_CF_clDBlue;
			}
			continue;
		}
		if (stringOut == RICH_LIN_clMagenta){
			if (strColor != stringOut){
				strColor  = stringOut;
				fileStream << RICH_CF_clDMagenta;
			}
			continue;
		}
		if (stringOut == RICH_LIN_clCyan){
			if (strColor != stringOut){
				strColor  = stringOut;
				fileStream << RICH_CF_clDCyan;
			}
			continue;
		}
		if (stringOut == RICH_LIN_clGray){
			if (strColor != stringOut){
				strColor  = stringOut;
				fileStream << RICH_CF_clDGray;
			}
			continue;
		}
		if (stringOut == RICH_LIN_clDarkGray){
			if (strColor != stringOut){
				strColor  = stringOut;
				fileStream << RICH_CF_clGray;
			}
			continue;
		}
		if (stringOut == RICH_LIN_clLightRed){
			if (strColor != stringOut){
				strColor  = stringOut;
				fileStream << RICH_CF_clRed;
			}
			continue;
		}
		if (stringOut == RICH_LIN_clLightGreen){
			if (strColor != stringOut){
				strColor  = stringOut;
				fileStream << RICH_CF_clGreen;
			}
			continue;
		}
		if (stringOut == RICH_LIN_clYellow){
			if (strColor != stringOut){
				strColor  = stringOut;
				fileStream << RICH_CF_clYellow;
			}
			continue;
		}
		if (stringOut == RICH_LIN_clLightBlue){
			if (strColor != stringOut){
				strColor  = stringOut;
				fileStream << RICH_CF_clBlue;
			}
			continue;
		}
		if (stringOut == RICH_LIN_clLightMagenta){
			if (strColor != stringOut){
				strColor  = stringOut;
				fileStream << RICH_CF_clMagenta;
			}
			continue;
		}
		if (stringOut == RICH_LIN_clLightCyan){
			if (strColor != stringOut){
				strColor  = stringOut;
				fileStream << RICH_CF_clCyan;
			}
			continue;
		}
		if (stringOut == RICH_LIN_clWhite){
			if (strColor != stringOut){
				strColor  = stringOut;
				fileStream << RICH_CF_clBlack;
			}
			continue;
		}
		
		stringTemp1 = Str_HEXToASCII(stringOut);
		ODEV_FormatString(&stringOut,stringTemp1);
		stringOut = Str_Replace(stringOut,"\r\n","\r");
		stringOut = Str_Replace(stringOut,"\n","\r");
		stringOut = Str_Replace(stringOut,"\r",RICH_PARL);
		str0 = '\0';
		stringOut = Str_Replace(stringOut,str0,"\\0");
		
		fileStream << stringOut;
	}
	fileStream << RICH_END;
	fileStream.flush();
	fileStream.close();
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//












