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
int32	CFS_CheckFile		(const STDSTR &fName);
uint64	CFS_CheckFileSize	(const STDSTR &fName);
uint64	CFS_ReadFile		(STDSTR *retStr,const STDSTR &fName,uint64 fsLength = -1,uint64 fsOffset = 0,G_APPEND blAppend = G_APPEND_OFF);
#ifdef Comm_FIFOH
uint64	CFS_ReadFile		(FIFO8 *retFifo,const STDSTR &fName,uint64 fsLength = -1,uint64 fsOffset = 0);
#endif
void	CFS_WriteFile		(const STDSTR &fName,const STDSTR &strContent);
void	CFS_AddToFile		(const STDSTR &fName,const STDSTR &strContent);
void	CFS_WriteFile		(const STDSTR &fName,const STDSTR &strContent,uint64 fsOffset);
//------------------------------------------------------------------------------------------//




#ifdef EN
//------------------------------------------------------------------------------------------//
class FILE_NODE : public TREE_NODE{
		enum{FPS = 1024 * 1024 * 16};
		enum FT{FT_NONE = 0,FT_DIR = 1,FT_FILE = 2};
		enum RECURSION{R_NO = 0,R_YES};
	public:
				 FILE_NODE(void) : TREE_NODE(){;};
		virtual ~FILE_NODE(void){;};
	public:
		uint32			cgRemoteNodeID;
		STDSTR		cgStrPath;
		STDSTR		cgStrName;
		FT				cgType;
		uint64			cgSize;
		uint64			cgLastModifyTime;
		uint64			cgAttrib;
	public:
		STDSTR		cgStrNodeHash;
		uint64			cgFilePackageSize;
		STDSTR		cgStrFilePackageHash;
	public:
		FILE_NODE &operator=(const FILE_NODE &tNode);
	public:
		static void			Clear				(FILE_NODE *tNode);
		static FT			FillBasicInfoToNode	(FILE_NODE *tNode,const STDSTR &tFullName);
		static int32		FillFileHashToNode	(FILE_NODE *tNode);
		static int32		FillDirHashToNode	(FILE_NODE *tNode);
		static STDSTR	CreateNodeToStr		(FILE_NODE *tNode){return(CreateNodeToStrV0_1(tNode));};
		static STDSTR	CreateNodeToStrV0_1	(FILE_NODE *tNode);
		static void			SetStrToNode		(FILE_NODE *tNode,const STDSTR &strIn){SetStrToNodeV0_1(tNode,strIn);};
		static void			SetStrToNodeV0_1	(FILE_NODE *tNode,const STDSTR &strIn);
};
#endif
//------------------------------------------------------------------------------------------//
#endif
