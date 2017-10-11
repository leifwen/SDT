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

//------------------------------------------------------------------------------------------//
#include "Comm_Tree.h"
#include "SYS_Thread.h"
#include "Device.h"
//------------------------------------------------------------------------------------------//
#ifdef DeviceH
#ifndef SendFilesH
#define SendFilesH
#ifdef SendFilesH
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
class TFileSend : public DEVICE_EXE_FRAME{
		enum{PACKAGE_MAX_SIZE = 1024 * 2};
	public:
				 TFileSend(void);
		virtual ~TFileSend(void){cgfileName = "";};
	private:
		STDSTR					cgfileName;
		SYS_Thread<TFileSend>	sendThread;
	private:
		int32	SendThreadFun	(void *p);
	public:
		int32	Execute			(const DEVICE *tDevice,const STDSTR &tfileName);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif
#endif
#endif
