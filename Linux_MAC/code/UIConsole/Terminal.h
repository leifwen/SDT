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
#include "Commu_RMS.h"
#include "BIC.h"
//------------------------------------------------------------------------------------------//
class TerminalSocket : public APISocket{
	public:
		enum{RFLAG_C = 0, RFLAG_S = APISocket::RFLAG_S + APISocket::RFLAG_C};
				 TerminalSocket(const ODEV_LIST *tODEV_LIST,int32 tSize): APISocket(tODEV_LIST,tSize){
					CreateSelfODev(COLSTRING::COLType_TXT);
					selfName = "TerminalSocket";
				 };
		virtual ~TerminalSocket(void){;};
	private:
		BICPAR			cgBICPAR;
		virtual	int32	ExThreadFun		(void);
		virtual void	ThreadsStart	(void);
		virtual	void	OnCloseDev		(void);
};
//------------------------------------------------------------------------------------------//
class TerminalServer : public APISocketServer{
	public:
		enum{RFLAG_C = 0, RFLAG_S = APISocketServer::RFLAG_S + APISocketServer::RFLAG_C};
				 TerminalServer(int32 tSize) : APISocketServer(nullptr,tSize){selfName = "TS";};
		virtual ~TerminalServer(void){;};
	private:
		virtual APISocket	*CreateNewSocket_TCP(const ODEV_LIST *tODEV_LIST,uint32 tSize){
			TerminalSocket *tPDB;
			tPDB = new TerminalSocket(tODEV_LIST,tSize);
			if (tPDB != nullptr)
				tPDB->selfName = "TS->Socket" + Str_IntToString(GetnodeID(tPDB).load());
			return(tPDB);
		};
	public:
		int32	Run		(int32 port){return(APISocketServer::OpenD(port,COMMU_DBUF::CSType_TCP,0));};
};
//------------------------------------------------------------------------------------------//
#ifdef USE_OPENSSL
//------------------------------------------------------------------------------------------//
class RSTSocket : public ControlSocket{
	public:
		enum{RFLAG_C = 0, RFLAG_S = ControlSocket::RFLAG_S + ControlSocket::RFLAG_C};
				 RSTSocket(const ODEV_LIST *tODEV_LIST,int32 tSize): ControlSocket(tODEV_LIST,tSize){Init();SetSelfName("RSTCilent");};
		virtual ~RSTSocket(void){commandThread.RemoveSelf();};
	private:
		SYS_ThreadEx<RSTSocket>	commandThread;
		BICPAR			cgBICPAR;
		uint32			cgTSFlag;
	private:
				void	Init			(void);
		virtual	int32	CommandThreadFun(void);
		virtual	void	OnCloseDev		(void);
		virtual	int32	MessageProcessing(FNode_MESG *RecMesg,int32 blReady);
	public:
				int32	SendRequestSetupTerminal(void);
				int32	SendRequestCloseTerminal(void);
				int32	CheckRemoteTerminalStatus(void);
};
//------------------------------------------------------------------------------------------//
class RSTServer : public RemoteSSLServer{
	public:
		enum{RFLAG_C = 0, RFLAG_S = RemoteSSLServer::RFLAG_S + RemoteSSLServer::RFLAG_C};
				 RSTServer(int32 tSize) : RemoteSSLServer(nullptr,tSize){selfName = "RST";};
		virtual ~RSTServer(void){;};
	private:
		virtual APISocket	*CreateNewSocket_TCP(const ODEV_LIST *tODEV_LIST,uint32 tSize){
			RSTSocket *tPDB = new RSTSocket(tODEV_LIST,tSize);
			if (tPDB != nullptr)
				tPDB->SetSelfName("RSTS->Socket" + Str_IntToString(GetnodeID(tPDB).load()));
			return(tPDB);
		};
	public:
		int32	Run		(int32 port){return(RemoteSSLServer::Run(port,COMMU_DBUF::CSType_TCP));};
};
//------------------------------------------------------------------------------------------//
#endif
//------------------------------------------------------------------------------------------//
#endif