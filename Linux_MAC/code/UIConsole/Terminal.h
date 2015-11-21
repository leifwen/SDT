/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: Terminal.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.01.25
*/
#ifndef TerminalH
#define TerminalH
//------------------------------------------------------------------------------------------//
#include "Commu_SocketServer.h"
#include "BIC.h"
//------------------------------------------------------------------------------------------//
class TerminalSocket : public APISocket{
	public:
		enum{RFLAG_C = 0, RFLAG_S = APISocket::RFLAG_S + APISocket::RFLAG_C};
				 TerminalSocket(const ODEV_LIST *tODEV_LIST,int32 tSize): APISocket(tODEV_LIST,tSize){CreateSelfODev(COLSTRING::COLType_TXT);};
		virtual ~TerminalSocket(void){;};
	private:
		BICPAR			cgBICPAR;
		virtual	int32	ExThreadFun		(void);
		virtual void	ThreadsStart	(void);
	private:
		virtual int32	OpenDev			(const std::string &tCDBufName,int32 tCDBufPar,CSType tCSType){return 1;};
		virtual	void	CloseDev		(void);
};
//------------------------------------------------------------------------------------------//
class TerminalServer : public APISocketServer{
	public:
		enum{RFLAG_C = 0, RFLAG_S = APISocketServer::RFLAG_S + APISocketServer::RFLAG_C};
				 TerminalServer() : APISocketServer(nullptr,1024 * 32){;};
		virtual ~TerminalServer(void){;};
	private:
		virtual APISocket	*CreateNewSocket_TCP(const ODEV_LIST *tODEV_LIST,uint32 tSize){	TerminalSocket *tPDB;
			tPDB = new TerminalSocket(tODEV_LIST,tSize);
			AddNode(tPDB);
			return(tPDB);
		};
	public:
		int32	Run		(int32 port){return(APISocketServer::OpenD(port,COMMU_DBUF::CSType_TCP,0));};
};
//------------------------------------------------------------------------------------------//
#endif