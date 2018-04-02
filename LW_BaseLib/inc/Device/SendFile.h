//
//  SendFile.h
//  SDT
//
//  Created by Leif Wen on 22/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "Device.h"
#ifdef Device_h
//------------------------------------------------------------------------------------------//
#ifndef SendFile_h
#define SendFile_h
#ifdef SendFile_h
//------------------------------------------------------------------------------------------//
class TFileSend : public DEVICE_EXE_FRAME{
		enum{PACKAGE_MAX_SIZE = 1024 * 2};
	public:
				 TFileSend(void);
		virtual ~TFileSend(void){;};
	private:
		STDSTR					cgFileName;
		SYS_Thread<TFileSend>	sendThread;
	private:
		bool32	SendThreadFun	(void* p);
	public:
		bool32	Execute			(const DEVICE* dev,const STDSTR& fileName);
};
//------------------------------------------------------------------------------------------//
#endif /* SendFile_h */
#endif /* SendFile_h */
#endif /* Device_h */
