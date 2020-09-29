//
//  SendFile.h
//  SDT
//
//  Created by Leif Wen on 22/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "Bus.h"
#ifdef Bus_h
//------------------------------------------------------------------------------------------//
#ifndef SendFile_h
#define SendFile_h
#ifdef SendFile_h
//------------------------------------------------------------------------------------------//
class TFileSend : public CDEVBUS_APP{
	public:
		enum{PACKAGE_MAX_SIZE = 1024 * 2,FS_BPS_DEF = 1024 * 100};
				 TFileSend(void);
		virtual ~TFileSend(void){;};
	private:
		STDSTR					cgFileName;
		uint32					cgPackSize;
		uint32					cgRateBps;
		SYS_Thread<TFileSend>	sendThread;
		OUTPUT_NODE*			extSDTOUT;
	private:
		bool32	SendThreadFunc	(void* p);
	public:
		bool32	Execute			(const CDEVBUS* cDevBus,const STDSTR& fileName,OUTPUT_NODE* sdtout,uint32 rateBps,uint32 packSize);
};
//------------------------------------------------------------------------------------------//
#endif /* SendFile_h */
#endif /* SendFile_h */
#endif /* Bus_h */
