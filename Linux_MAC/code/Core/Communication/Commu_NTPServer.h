/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: Commu_NTPServer.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.12.20
*/
//------------------------------------------------------------------------------------------//
#include "Commu_Socket.h"
//------------------------------------------------------------------------------------------//
#ifdef Commu_SocketH
#ifndef Commu_NTPServerH
#define Commu_NTPServerH
#ifdef Commu_NTPServerH
//------------------------------------------------------------------------------------------//
class NTPServer : public ASOCKETSERVER{
	public:
				 NTPServer(void) : ASOCKETSERVER(1024,nullptr){SetSelfName("NTPServer");};
		virtual ~NTPServer(void){;};
	private:
		virtual int32	ListionUDP(void *p);
	public:
		int32	Run		(int32 port = 123){return(OpenD("",port,CSType_UDPS,0));};
};
//------------------------------------------------------------------------------------------//
#endif
#endif
#endif
