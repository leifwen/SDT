//
//  Commu_RST.h
//  SDT
//
//  Created by Leif Wen on 24/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "MSG_RST.h"
#include "Commu_Terminal.h"
#ifdef MSG_RMS_h
//------------------------------------------------------------------------------------------//
#ifndef Commu_RST_h
#define Commu_RST_h
#ifdef Commu_RST_h
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_Linux
class MSG_RST1 : public MSG_RST{
	public:
 			 MSG_RST1(uint32 size,void* p) : MSG_RST(size,p){;};
	virtual ~MSG_RST1(void){;};
};
typedef COMMU_NOSMC2<COMMU_SOCKET2<COMMU_MSG<COMMU_SSL,MSG_RST1>>>	RSTBSocket;
#else
typedef COMMU_NOSMC2<COMMU_SOCKET2<COMMU_MSG<COMMU_SSL,MSG_RST>>>	RSTBSocket;
#endif
typedef COMMU_NOSMC2<COMMU_SOCKETSERVER2<RSTBSocket>>				RSTBServer;
//------------------------------------------------------------------------------------------//
#endif /* Commu_RST_h */
#endif /* Commu_RST_h */
#endif /* MSG_RMS_h */
