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
#ifndef Commu_NTPServerH
#define Commu_NTPServerH
//------------------------------------------------------------------------------------------//
#include "Comm_Buffer.h"
#include "Commu_Socket.h"
#include "Commu_SocketServer.h"
//------------------------------------------------------------------------------------------//
class NTPServer : public APISocketServer{
	public:
		enum{ RFLAG_C = 0, RFLAG_S = APISocketServer::RFLAG_S + APISocketServer::RFLAG_C };
	public:
				 NTPServer(void) : APISocketServer(nullptr,64){cgPort = 123;};
		virtual ~NTPServer(void){;};
	private:
		virtual int32	UDPThreadListionFun	(void);
	public:
		int32	Run		(int32 port){return(APISocketServer::OpenD(port,COMMU_DBUF::CSType_UDP,0));};
};
//------------------------------------------------------------------------------------------//
#endif