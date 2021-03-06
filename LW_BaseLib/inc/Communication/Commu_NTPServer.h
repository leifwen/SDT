//
//  Commu_NTPServer.h
//  SDT
//
//  Created by Leif Wen on 05/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "Commu_Socket.h"
#ifdef Commu_Socket_h
//------------------------------------------------------------------------------------------//
#ifndef Commu_NTPServer_h
#define Commu_NTPServer_h
#ifdef Commu_NTPServer_h
//------------------------------------------------------------------------------------------//
class CORE_NTPServer : public CORE_SOCKETSERVER{
	public:
				 CORE_NTPServer(void);
		virtual ~CORE_NTPServer(void){;};
	private:
		virtual bool32	ListionUDP	(void* _team);
};
//------------------------------------------------------------------------------------------//
typedef	COMMU<0,COMMU_FRAME,CORE_NTPServer>	NTPServer_BASE;
class NTPServer : public NTPServer_BASE{
	public:
				 NTPServer(void) : NTPServer_BASE(1024,1024,nullptr){;};
		virtual ~NTPServer(void){;};
	public:
		bool32	Run			(int32 port = 123);
};
//------------------------------------------------------------------------------------------//
#endif /* Commu_NTPServer_h */
#endif /* Commu_NTPServer_h */
#endif /* Commu_Socket_h */
