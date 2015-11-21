/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: SendFiles.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2012.12.12
 *
*/
#ifndef SendFilesH
#define SendFilesH
//------------------------------------------------------------------------------------------//
#include "Comm_Tree.h"
#include "SYS_Thread.h"
//------------------------------------------------------------------------------------------//
class DEVICE;
//------------------------------------------------------------------------------------------//
class TFileSend : public RTREE_NODE{
	public:
		enum{RFLAG_C = 1, RFLAG_S = RTREE_NODE::RFLAG_S + RTREE_NODE::RFLAG_C};
		enum{PACKAGE_MAX_SIZE = 1024 * 2};
	public:
				 TFileSend(void);
		virtual ~TFileSend(void);
	private:
		std::string 	cfileName;
		DEVICE			*cDevice;
		SYS_ThreadEx<TFileSend>		exThread;
		void	SetInRun		(void){SetSFlag(RFLAG_CREATE(0));};
		void	ClrInRun		(void){ClrSFlag(RFLAG_CREATE(0));};
		int32	CheckblInRun	(void){return(CheckSFlag(RFLAG_CREATE(0)));};
		int32	SendFile		(void);
	public:
		void	StopSend(void);
		int32	Execute(const DEVICE *tDevice,const std::string &tfileName);
};
//------------------------------------------------------------------------------------------//

//------------------------------------------------------------------------------------------//
#endif
