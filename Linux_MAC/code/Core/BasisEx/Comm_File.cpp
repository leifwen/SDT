/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: PublicSYS.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2012.02.09
 * @2013.12.11 : update Thread to make more safe.
 * @2013.06.30 : change to Liunx style
 				add init method
 				use int32 to replace bool
 				use int32 to replace uint32
   @2014.05.24 : form PublicSYS.h
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "Comm_File.h"
#include "Comm_Convert.h"
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_Unix
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#endif
//------------------------------------------------------------------------------------------//
int32 CFS_CheckFile(const std::string &fName){
#ifdef CommonDefH_Unix
	if (access(fName.c_str(),0) == -1)
		return 0;
#endif
#ifdef CommonDefH_VC
	if (!PathFileExists(Str_ANSIToUnicode(fName).c_str()))
		return 0;
#endif
	return 1;
}
//------------------------------------------------------------------------------------------//
uint64 CFS_CheckFileSize(const std::string &fName){
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
uint64 CFS_ReadFile(std::string *retContent,const std::string &fName,uint64 fsLength,uint64 fsOffset){
	//read all data in ASCII format to result,
	uint8			buffer[1024 * 8];
	std::fstream	fileStream;
	uint64			returnCount,readNum,num;
	
	if (CFS_CheckFile(fName) < 1)
		return 0;
	
	*retContent = "";
	returnCount = 0;
	fileStream.open(fName.c_str(),std::ios::in|std::ios::binary);
	if (fsOffset > 0)
		fileStream.seekp(fsOffset,std::ios::beg);
	do{
		readNum = (fsLength > sizeof(buffer)) ? sizeof(buffer) : fsLength;
		fileStream.read((char*)buffer,(std::size_t)readNum);
		num = fileStream.gcount();
		returnCount += num;
		fsLength -= num;
		*retContent += Str_CharToASCIIStr(buffer,num,G_ESCAPE_OFF);
	}while((!fileStream.eof()) && (fsLength > 0));
	fileStream.close();
	
	return(returnCount);
}
//------------------------------------------------------------------------------------------//
uint64 CFS_ReadFile(FIFO_UINT8 *retFifo,const std::string &fName,uint64 fsLength,uint64 fsOffset){
	//read all data in ASCII format to result,
	uint8			buffer[1024 * 8];
	std::fstream	fileStream;
	uint64			returnCount,readNum,num;
	
	if (CFS_CheckFile(fName) < 1)
		return 0;
	
	retFifo->Empty();
	returnCount = 0;
	fileStream.open(fName.c_str(),std::ios::in|std::ios::binary);
	if (fsOffset > 0)
		fileStream.seekp(fsOffset,std::ios::beg);
	do{
		readNum = (fsLength > sizeof(buffer)) ? sizeof(buffer) : fsLength;
		fileStream.read((char*)buffer,(std::size_t)readNum);
		num = fileStream.gcount();
		returnCount += num;
		fsLength -= num;
		retFifo->Put(buffer, (uint32)num);
	}while((!fileStream.eof()) && (fsLength > 0));
	fileStream.close();
	
	return(returnCount);
}
//------------------------------------------------------------------------------------------//
void CFS_AddToFile(const std::string &fName,const std::string &strContent){
	std::fstream 	fileStream;

	fileStream.open(fName.c_str(),std::ios::out|std::ios::app|std::ios::binary);
	fileStream << strContent;
	fileStream.flush();
	fileStream.close();
}
//------------------------------------------------------------------------------------------//
void CFS_WriteFile(const std::string &fName,const std::string &strContent){
	std::fstream 	fileStream;
	
	fileStream.open(fName.c_str(),std::ios::out|std::ios::trunc|std::ios::binary);
	fileStream << strContent;
	fileStream.flush();
	fileStream.close();
}
//------------------------------------------------------------------------------------------//
void CFS_WriteFile(const std::string &fName,const std::string &strContent,uint64 fsOffset){
	std::fstream 	fileStream;
	
	fileStream.open(fName.c_str(),std::ios::in|std::ios::out|std::ios::binary);
	fileStream.seekp(fsOffset,std::ios::beg);
	fileStream << strContent;
	fileStream.flush();
	fileStream.close();
}
//------------------------------------------------------------------------------------------//























































































//------------------------------------------------------------------------------------------//
FILE_NODE &FILE_NODE::operator =(const FILE_NODE &tNode){
	cgRemoteNodeID = tNode.cgRemoteNodeID;
	cgStrPath = tNode.cgStrPath;
	cgStrName = tNode.cgStrName;
	cgType = tNode.cgType;
	cgSize = tNode.cgSize;
	cgLastModifyTime = tNode.cgLastModifyTime;
	cgAttrib = tNode.cgAttrib;
	
	
	
	cgStrNodeHash = tNode.cgStrNodeHash;
	cgFilePackageSize = tNode.cgFilePackageSize;
	cgStrFilePackageHash = tNode.cgStrFilePackageHash;
	return(*this);
};
//------------------------------------------------------------------------------------------//
void FILE_NODE::Clear(FILE_NODE *tNode){
	tNode->cgRemoteNodeID = 0;
	tNode->cgStrPath = "";
	tNode->cgStrName = "";
	tNode->cgType = FT_NONE;
	tNode->cgSize = 0;
	tNode->cgLastModifyTime = 0;
	tNode->cgAttrib = 0;
	
	
	
	tNode->cgStrNodeHash = "";
	tNode->cgFilePackageSize = 0;
	tNode->cgStrFilePackageHash = "";
}
//------------------------------------------------------------------------------------------//
FILE_NODE::FT FILE_NODE::FillBasicInfoToNode(FILE_NODE *tNode,const std::string &tFullName){
	if (tNode == nullptr)
		return(FT_NONE);
#ifdef CommonDefH_Unix
	struct stat 	nodeInfo;
	if (lstat(tFullName.c_str(), &nodeInfo) < 0)
		return(FT_NONE);
	
	Clear(tNode);
	tNode->cgStrPath = tFullName;
	tNode->cgStrName = Str_ReadSubItemR(&tNode->cgStrPath, "/");
#ifdef CommonDefH_MAC
	tNode->cgLastModifyTime = nodeInfo.st_mtimespec.tv_sec;
#endif
#ifdef CommonDefH_Linux
	tNode->cgLastModifyTime = ;
#endif
	tNode->cgAttrib = 0;
	
	if(S_ISDIR(nodeInfo.st_mode)) {
		tNode->cgType = FT_DIR;
		tNode->cgSize =0;
	}
	else if(S_ISREG(nodeInfo.st_mode)){
		tNode->cgType = FT_FILE;
		tNode->cgSize = nodeInfo.st_size;
	}
	return(tNode->cgType);
#endif
	return(FT_NONE);
}
//------------------------------------------------------------------------------------------//
int32 FILE_NODE::FillFileHashToNode(FILE_NODE *tNode){
	std::string		fileName;
	std::fstream	fileStream;
	uint32			num,count;
	uint8			buf[1024 * 8];
	uint8			m_rawHash[16];
	
	if (tNode == nullptr)
		return 0;
#ifdef CommonDefH_Unix
	fileName = tNode->cgStrPath + "/" + tNode->cgStrName;
#endif
#ifdef CommonDefH_VC
	fileName = tNode->cgStrPath + "\\" + tNode->cgStrName;
#endif

	if (CFS_CheckFile(fileName) == 0)
		return 0;
	
	tNode->cgStrNodeHash = "";
	tNode->cgStrFilePackageHash = "";
	tNode->cgFilePackageSize = FPS;
	/*
	count = 0;
	MD5Init(&md5ctx);
	fileStream.open(fileName.c_str(),std::ios::in|std::ios::binary);
	do{
		fileStream.read((char*)buf, sizeof(buf));
		num = (uint32)fileStream.gcount();
		count += num;
		MD5Update(&md5ctx,buf,num);
		if ((count == FPS) || fileStream.eof()){
			MD5Final(m_rawHash, &md5ctx);
			tNode->cgStrFilePackageHash += Str_CharToHEXStr(m_rawHash, sizeof(m_rawHash),G_SPACE_OFF);
			tNode->cgStrFilePackageHash += "\n";
			count = 0;
			MD5Init(&md5ctx);
		}
	}while(!fileStream.eof());
	fileStream.close();
	MD5Update(&md5ctx,(uint8*)tNode->cgStrFilePackageHash.c_str(),(uint32)tNode->cgStrFilePackageHash.length());
	MD5Final(m_rawHash, &md5ctx);
	tNode->cgStrNodeHash = Str_CharToHEXStr(m_rawHash, sizeof(m_rawHash),G_SPACE_OFF);
	*/
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 FILE_NODE::FillDirHashToNode(FILE_NODE *tNode){
#ifdef CommonDefH_Unix
	struct dirent	*structNode;
	DIR				*dir;
#endif
	std::string		strFullName,strResult;
	FILE_NODE		subFileNode;
	//MD5_CTX			md5ctx;
	uint8			m_rawHash[16];
	uint32 			ret;

	if (tNode == nullptr)
		return 0;
	
	tNode->cgStrFilePackageHash = "";
#ifdef CommonDefH_Unix
	strFullName = tNode->cgStrPath + "/" + tNode->cgStrName;
		dir = opendir(strFullName.c_str());
	
	ret = 0;
	if (dir != nullptr){
		while((structNode = readdir(dir)) != nullptr){
			if((strcmp(structNode->d_name, ".") == 0) || (strcmp(structNode->d_name, "..") == 0))
				continue;
			
			strFullName += "/";
			strFullName += structNode->d_name;
			
			FillBasicInfoToNode(&subFileNode,strFullName);
			tNode->cgStrFilePackageHash += CreateNodeToStr(&subFileNode);
		}
		closedir(dir);
		ret = 1;
	}
#endif
#ifdef CommonDefH_VC
	strFullName = tNode->cgStrPath + "\\" + tNode->cgStrName;
#endif
/*	if (ret != 0){
		MD5Init(&md5ctx);
		MD5Update(&md5ctx,(uint8*)tNode->cgStrFilePackageHash.c_str(),(uint32)tNode->cgStrFilePackageHash.length());
		MD5Final(m_rawHash, &md5ctx);
		tNode->cgStrNodeHash = Str_CharToHEXStr(m_rawHash, sizeof(m_rawHash),G_SPACE_OFF);
	}*/
	return(1);
}
//------------------------------------------------------------------------------------------//
std::string FILE_NODE::CreateNodeToStrV0_1(FILE_NODE *tNode){
	std::string		strResult;

	if (tNode == nullptr)
		return("");
	
	strResult = (Str_DecToHex(GetdRNodeID(tNode)) + "\r");
	strResult = (tNode->cgStrPath + "\r");
	strResult = (tNode->cgStrName + "\r");
	strResult += (Str_DecToHex(tNode->cgType) + "\r");
	strResult += (Str_DecToHex(tNode->cgSize) + "\r");
	strResult += (Str_DecToHex(tNode->cgLastModifyTime) + "\r");
	strResult += (Str_DecToHex(tNode->cgAttrib) + "\r");
	strResult += (tNode->cgStrNodeHash + "\r");
	strResult += (Str_DecToHex(tNode->cgFilePackageSize) + "\r");
	strResult += (tNode->cgStrFilePackageHash + "\r");
	return (strResult);
}
//------------------------------------------------------------------------------------------//
void FILE_NODE::SetStrToNodeV0_1(FILE_NODE *tNode,const std::string &strInput){
	std::string		strResult;
	
	if (tNode == nullptr)
		return;
	
	Clear(tNode);
	tNode->cgStrFilePackageHash = strInput;
	tNode->cgRemoteNodeID = (uint32)Str_HexToDec(Str_ReadSubItem(&tNode->cgStrFilePackageHash,"\r"));
	tNode->cgStrPath = Str_ReadSubItem(&tNode->cgStrFilePackageHash,"\r");
	tNode->cgStrName = Str_ReadSubItem(&tNode->cgStrFilePackageHash,"\r");
	tNode->cgType = (Str_HexToDec(Str_ReadSubItem(&tNode->cgStrFilePackageHash,"\r")) == 1) ? FT_DIR : FT_FILE;
	tNode->cgSize = Str_HexToDec(Str_ReadSubItem(&tNode->cgStrFilePackageHash,"\r"));
	tNode->cgLastModifyTime = Str_HexToDec(Str_ReadSubItem(&tNode->cgStrFilePackageHash,"\r"));
	tNode->cgAttrib = Str_HexToDec(Str_ReadSubItem(&tNode->cgStrFilePackageHash,"\r"));

	tNode->cgStrNodeHash = Str_ReadSubItem(&tNode->cgStrFilePackageHash,"\r");
	tNode->cgFilePackageSize = Str_HexToDec(Str_ReadSubItem(&tNode->cgStrFilePackageHash,"\r"));
	Str_ReadSubItemR(&tNode->cgStrFilePackageHash,"\r");
}
//------------------------------------------------------------------------------------------//









//------------------------------------------------------------------------------------------//