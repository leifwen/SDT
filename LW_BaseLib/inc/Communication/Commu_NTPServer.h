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
class NTPServer : public ASOCKETSERVER{
	public:
				 NTPServer(void);
		virtual ~NTPServer(void){;};
	private:
		virtual bool32	ListionUDP	(void* commu);
	public:
				bool32	Run			(int32 port = 123);
};
//------------------------------------------------------------------------------------------//
#endif /* Commu_NTPServer_h */
#endif /* Commu_NTPServer_h */
#endif /* Commu_Socket_h */
