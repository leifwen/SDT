//
//  SYS_File.cpp
//  SDT
//
//  Created by Leif Wen on 31/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "SYS_File.h"
#include "DS_string.h"
//------------------------------------------------------------------------------------------//
#ifdef SYS_File_h
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_Unix
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#endif
//------------------------------------------------------------------------------------------//
bool32 CFS_CheckFile(const STDSTR& fName){
#ifdef CommonDefH_Unix
	if (access(fName.c_str(),0) == -1)
		return G_FALSE;
#endif
#ifdef CommonDefH_VC
	if (!PathFileExists(Str_ANSIToUnicode(fName).c_str()))
		return G_FALSE;
#endif
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
uint64 CFS_CheckFileSize(const STDSTR& fName){
#ifdef CommonDefH_Unix
	struct stat		nodeInfo;
	
	if(lstat(fName.c_str(), &nodeInfo) < 0)
		return 0;
	
	if(S_ISREG(nodeInfo.st_mode))
		return(nodeInfo.st_size);
	return 0;
#endif
#ifdef CommonDefH_VC
	std::fstream	fileStream;
	uint64			retLength;
	if (CFS_CheckFile(fName) < 1)
		return 0;
	fileStream.open(fName.c_str(),std::ios::in|std::ios::binary);
	fileStream.seekg(0,std::ios::end);
	retLength = fileStream.tellg();
	fileStream.close();
	return(retLength);
#endif
};
//------------------------------------------------------------------------------------------//
uint64 CFS_ReadFile(STDSTR* retStr,const STDSTR &fName){
	std::fstream	fileStream;
	uint64			returnCount,num;
	
	returnCount = 0;
	if (CFS_CheckFile(fName)){
		uint8	buffer[1024 * 8];

		fileStream.open(fName.c_str(),std::ios::in|std::ios::binary);

		while(!fileStream.eof()){
			fileStream.read((char*)buffer,sizeof(buffer));
			num = fileStream.gcount();
			returnCount += num;
			retStr->append((char*)buffer,num);
		};
		fileStream.close();
	}
	return(returnCount);
};
//------------------------------------------------------------------------------------------//
void CFS_AddToFile(const STDSTR& fName,const STDSTR& strContent){
	std::fstream 	fileStream;
	
	fileStream.open(fName.c_str(),std::ios::out|std::ios::app|std::ios::binary);
	fileStream << strContent;
	fileStream.flush();
	fileStream.close();
};
//------------------------------------------------------------------------------------------//
void CFS_WriteFile(const STDSTR& fName,const STDSTR& strContent){
	std::fstream 	fileStream;

	fileStream.open(fName.c_str(),std::ios::out|std::ios::trunc|std::ios::binary);
	fileStream << strContent;
	fileStream.flush();
	fileStream.close();
};
//------------------------------------------------------------------------------------------//
bool32 CFS_CheckDIR(const STDSTR& dir){
#ifdef CommonDefH_Unix
	{
		bool32	ret = G_FALSE;
		struct stat s_buf;
		
		if (stat(dir.c_str(),&s_buf) == 0){
			if(S_ISDIR(s_buf.st_mode))
				ret = G_TRUE;
		}
		return(ret);
	}
#endif
#ifdef CommonDefH_VC
	{
		CString			fileDir;
		std::wstring	strfileDir;
		
		strfileDir = Str_ANSIToUnicode(dir);
		fileDir = strfileDir.c_str();
		
		return(PathIsDirectory(fileDir));
	}
#endif
};
//------------------------------------------------------------------------------------------//
STDSTR CFS_GetSelfDIR(void){
#ifdef CommonDefH_Unix
	{
		STDSTR	strfileDir;
		char 	work_path[256];
		char 	*path;
		
		path = getcwd(work_path,sizeof(work_path));
		strfileDir = Str_CharToASCII((uint8*)path,(uint32)strlen(path),G_ESCAPE_OFF);
		
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

		strfileDir = fileDir;
		return(Str_UnicodeToANSI(strfileDir));
	}
#endif
};
//------------------------------------------------------------------------------------------//
bool32 CFS_CreateDIRLoop(STDSTR dir_root, STDSTR dir_sub){
#ifdef CommonDefH_Unix
	STDSTR	dir_t = "/";
#endif
#ifdef CommonDefH_VC
	STDSTR	dir_t = "\\";
#endif
	bool32	ret;
	
	dir_root = CFS_FormatFileName(dir_root);
	dir_sub = CFS_FormatFileName(dir_sub);

	do{
		dir_root = CFS_FormatFileName(dir_root + dir_t + Str_ReadSubItem(&dir_sub, dir_t));

		if (CFS_CheckFile(dir_root)){
			ret = G_FALSE;
			if (CFS_CheckDIR(dir_root) == G_FALSE)
				break;
		}
		else{
			ret = CFS_CreateDIR(dir_root);
			if (ret == G_FALSE)
				break;
		}
		ret = G_TRUE;
	} while (dir_sub.length() > 0);
	return(ret);
};
//------------------------------------------------------------------------------------------//
bool32 CFS_CreateDIR(const STDSTR& dir){
#ifdef CommonDefH_Unix
	{
		if (access(dir.c_str(),0) == -1)
			return(mkdir(dir.c_str(),0777) == 0);
		return(G_TRUE);
	}
#endif
#ifdef CommonDefH_VC
	{
		CString			fileDir;
		std::wstring	strfileDir;
		
		strfileDir = Str_ANSIToUnicode(dir);
		fileDir = strfileDir.c_str();
		
		if (!PathIsDirectory(fileDir))
			return(CreateDirectory(fileDir,nullptr));
		return(G_TRUE);
	}
#endif
};
//------------------------------------------------------------------------------------------//
STDSTR CFS_GetDIR(STDSTR fn){
#ifdef CommonDefH_Unix
	{
		Str_ReadSubItemR(&fn, "/");
		return(fn);
	}
#endif
#ifdef CommonDefH_VC
	{
		Str_ReadSubItemR(&fn, "\\");
		return(fn);
	}
#endif
};
//------------------------------------------------------------------------------------------//
STDSTR CFS_GetFileName(STDSTR fn){
#ifdef CommonDefH_Unix
	{
		return(Str_ReadSubItemR(&fn, "/"));
	}
#endif
#ifdef CommonDefH_VC
	{
		return(Str_ReadSubItemR(&fn, "\\"));
	}
#endif
};
//------------------------------------------------------------------------------------------//
STDSTR CFS_FormatFileName(STDSTR fn){
#ifdef CommonDefH_Unix
	{
		fn = Str_Replace(fn, "\\", "/");
		fn = Str_Replace(fn, "//", "/");
		return(fn);
	}
#endif
#ifdef CommonDefH_VC
	{
		fn = Str_Replace(fn, "/", "\\");
		fn = Str_Replace(fn, "\\\\", "\\");
		return(fn);
	}
#endif
};
//------------------------------------------------------------------------------------------//
#endif /* SYS_File_h */
