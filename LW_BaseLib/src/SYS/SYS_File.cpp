//
//  SYS_File.cpp
//  SDT
//
//  Created by Leif Wen on 31/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
#include "SYS_File.h"
//------------------------------------------------------------------------------------------//
#ifdef SYS_File_h
//------------------------------------------------------------------------------------------//
#include "DS_string.h"
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
}
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
}
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
}
//------------------------------------------------------------------------------------------//
void CFS_AddToFile(const STDSTR& fName,const STDSTR& strContent){
	std::fstream 	fileStream;
	
	fileStream.open(fName.c_str(),std::ios::out|std::ios::app|std::ios::binary);
	fileStream << strContent;
	fileStream.flush();
	fileStream.close();
}
//------------------------------------------------------------------------------------------//
void CFS_WriteFile(const STDSTR& fName,const STDSTR& strContent){
	std::fstream 	fileStream;

	fileStream.open(fName.c_str(),std::ios::out|std::ios::trunc|std::ios::binary);
	fileStream << strContent;
	fileStream.flush();
	fileStream.close();
}
//------------------------------------------------------------------------------------------//
#endif /* SYS_File_h */
