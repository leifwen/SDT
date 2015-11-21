/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: Comm_File.h
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
#ifndef Comm_FileH
#define Comm_FileH
//------------------------------------------------------------------------------------------//
#include "Comm_Tree.h"
#include "Comm_FIFO.h"
#include <fstream>
//------------------------------------------------------------------------------------------//
int32	CFS_CheckFile		(const std::string &fName);
uint64	CFS_CheckFileSize	(const std::string &fName);
uint64	CFS_ReadFile		(std::string *retContent,const std::string &fName,uint64 fsLength = -1,uint64 fsOffset = 0);
uint64	CFS_ReadFile		(FIFO_UINT8 *retFifo,const std::string &fName,uint64 fsLength = -1,uint64 fsOffset = 0);
void	CFS_WriteFile		(const std::string &fName,const std::string &strContent);
void	CFS_AddToFile		(const std::string &fName,const std::string &strContent);
void	CFS_WriteFile		(const std::string &fName,const std::string &strContent,uint64 fsOffset);
//------------------------------------------------------------------------------------------//





//------------------------------------------------------------------------------------------//
class FILE_NODE : public RTREE_NODE{
	public:
		enum{RFLAG_C = 0, RFLAG_S = RTREE_NODE::RFLAG_S + RTREE_NODE::RFLAG_C};
		enum{FPS = 1024 * 1024 * 16};
		enum FT{FT_NONE = 0,FT_DIR = 1,FT_FILE = 2};
		enum RECURSION{R_NO = 0,R_YES};
	public:
				 FILE_NODE(void) : RTREE_NODE(){;};
		virtual ~FILE_NODE(void){;};
	public:
		uint32			cgRemoteNodeID;
		std::string		cgStrPath;
		std::string		cgStrName;
		FT				cgType;
		uint64			cgSize;
		uint64			cgLastModifyTime;
		uint64			cgAttrib;
	public:
		std::string		cgStrNodeHash;
		uint64			cgFilePackageSize;
		std::string		cgStrFilePackageHash;
	public:
		FILE_NODE &operator=(const FILE_NODE &tNode);
	public:
		static void			Clear				(FILE_NODE *tNode);
		static FT			FillBasicInfoToNode	(FILE_NODE *tNode,const std::string &tFullName);
		static int32		FillFileHashToNode	(FILE_NODE *tNode);
		static int32		FillDirHashToNode	(FILE_NODE *tNode);
		static std::string	CreateNodeToStr		(FILE_NODE *tNode){return(CreateNodeToStrV0_1(tNode));};
		static std::string	CreateNodeToStrV0_1	(FILE_NODE *tNode);
		static void			SetStrToNode		(FILE_NODE *tNode,const std::string &strInput){SetStrToNodeV0_1(tNode,strInput);};
		static void			SetStrToNodeV0_1	(FILE_NODE *tNode,const std::string &strInput);
};
//------------------------------------------------------------------------------------------//
#endif