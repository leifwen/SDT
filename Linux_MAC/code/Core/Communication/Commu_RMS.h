/*
 * Copyright (c) 2012-2022
 * All rights reserved.
 *
 * File Name: Commu_RMS.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2016.01.06
*/
#ifndef Commu_RMSH
#define Commu_RMSH
//------------------------------------------------------------------------------------------//
#include "Commu_SocketServer.h"
#include "Commu_SSLSocket.h"
//------------------------------------------------------------------------------------------//
#ifdef USE_OPENSSL
//------------------------------------------------------------------------------------------//
enum{
	MESG_REQ_SetupMServer = MESG_NEXT_SSL,
	MESG_ANS_SetupMServer,
	MESG_REQ_CloseMServer,
	MESG_ANS_CloseMServer,
	MESG_REQ_ListMServer,
	MESG_ANS_ListMServer,
	MESG_REQ_SetupDataChannel,
	MESG_ANS_SetupDataChannel,
	MESG_REQ_Link,
	MESG_ANS_Link,
	MESG_NEXT_RSSL,
};
//------------------------------------------------------------------------------------------//
class MServer_Socket : public APISocket{
	public:
		enum{RFLAG_C = 0, RFLAG_S = APISocket::RFLAG_S + APISocket::RFLAG_C};
				 MServer_Socket(const ODEV_LIST *tODEV_LIST,int32 tSize) : APISocket(nullptr,tSize){selfName = "MServer->Socket";};
		virtual ~MServer_Socket(void){;};
	private:
		void	Init(void);
		virtual	void	PrintUserDisconnectReport	(const std::string &strDevName){;};
		virtual	void	PrintConnectInfo			(int32 blSendWelcome){;};
		virtual	void	PrintDisconnectInfo			(void){;};
		virtual	void	ForwardToCouple				(const uint8 *databuf,int32 num);
		virtual	void	OnCloseDev					(void);
		virtual void	ThreadsStart				(void);
};
//------------------------------------------------------------------------------------------//
class MAPPINGServer : public APISocketServer{
	public:
		enum{RFLAG_C = 0, RFLAG_S = APISocketServer::RFLAG_S + APISocketServer::RFLAG_C};
				 MAPPINGServer(const ODEV_LIST *tODEV_LIST,int32 tSize) : APISocketServer(tODEV_LIST,tSize){cgMapIP = "";cgMapPort = 0;selfName = "MServer";};
		virtual ~MAPPINGServer(void){;};
	private:
		virtual APISocket	*CreateNewSocket_TCP(const ODEV_LIST *tODEV_LIST,uint32 tSize){
			MServer_Socket *tPDB = new MServer_Socket(tODEV_LIST,tSize);
			if (tPDB != nullptr)
				tPDB->selfName += Str_IntToString(GetnodeID(tPDB).load());
			return(tPDB);
		};
		virtual int32	OnOpenTCPSocket(APISocket *newSocket);
	private:
		std::string	cgMapIP;
		int32		cgMapPort;
	public:
		inline	const	std::string &GetcgMapIP	(void){return(cgMapIP);};
		inline			int32		GetcgMapPort(void){return(cgMapPort);};
	public:
		inline	int32	Run(int32 localPort,const std::string &mapIP,int32 mapPort,COMMU_DBUF::CSType tCSType = COMMU_DBUF::CSType_TCP){
			cgMapIP = mapIP;
			cgMapPort = mapPort;
			return(APISocketServer::OpenD(localPort,tCSType,0));
		};
};
//------------------------------------------------------------------------------------------//
class RemoteSSLSocket : public SSLSocket{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SSLSocket::RFLAG_S + SSLSocket::RFLAG_C};
				 RemoteSSLSocket(const ODEV_LIST *tODEV_LIST,int32 tSize) : SSLSocket(tODEV_LIST,tSize){Init();SetSelfName("RemoteSSLSocket");};
		virtual ~RemoteSSLSocket(void){;};
	public:
		RemoteSSLSocket		*cgLSSLSocket;
		MServer_Socket		*cgRSocket;
	private:
		uint32				cgRSSLFlag;
	private:
				void	Init						(void){cgRSocket = nullptr;cgLSSLSocket = nullptr;};
		virtual	void	PrintUserDisconnectReport	(const std::string &strDevName){;};
		virtual	void	PrintConnectInfo			(int32 blSendWelcome){;};
		virtual	void	PrintDisconnectInfo			(void){;};
		virtual	void	DisconnectAll				(void){;};
		virtual	void	ForwardToCouple				(const uint8 *databuf,int32 num);
	protected:
		virtual	int32	MessageProcessing			(FNode_MESG *RecMesg,int32 blReady);
		virtual	void	OnCloseDev					(void);
	private:
		int32	OpenMappingServer			(const std::string &strMesg);
		int32	CloseMappingServer			(const std::string &strMesg);
		int32	ListMappingServer			(std::string *strMesg);
		int32	OpenCoupleSocket			(std::string strMesg);
		int32	SendRequestLink				(const std::string &strMesg);
	public:
		int32	SendRequestSetupMServer		(int32 remotePort,const std::string &mapIP,int32 mapPort,COMMU_DBUF::CSType tCSType = COMMU_DBUF::CSType_TCP);
		int32	SendRequestOffMServer		(int32 remotePort);
		int32	SendRequestListMServer		(std::string *retStr);
		int32	SendRequestSetupDataChannel	(APISocket *mSocket,const std::string &mapIP,int32 mapPort,COMMU_DBUF::CSType tCSType = COMMU_DBUF::CSType_TCP);
};
//------------------------------------------------------------------------------------------//
class ControlSocket : public RemoteSSLSocket{
	public:
		enum{RFLAG_C = 0, RFLAG_S = RemoteSSLSocket::RFLAG_S + RemoteSSLSocket::RFLAG_C};
				 ControlSocket(const ODEV_LIST *tODEV_LIST,int32 tSize) : RemoteSSLSocket(tODEV_LIST,tSize){PreCreate();SetSelfName("ControlSocket");};
		virtual ~ControlSocket(void);
	private:
				void	PreCreate			(void);
		virtual	int32	Ex2ThreadFun		(void);
		virtual	void	DisconnectAll		(void);
		virtual void	ThreadsStart		(void){RemoteSSLSocket::ThreadsStart();ex2Thread.ThreadRun();};
	protected:
		virtual	void	OnCloseDev			(void);
};
//------------------------------------------------------------------------------------------//
class RemoteSSLServer : public APISocketServer{
	public:
		enum{RFLAG_C = 0, RFLAG_S = APISocketServer::RFLAG_S + APISocketServer::RFLAG_C};
				 RemoteSSLServer(const ODEV_LIST *tODEV_LIST,int32 tSize) : APISocketServer(tODEV_LIST,tSize){
					 cgMServer = nullptr;
					 selfName = "RemoteSSLServer";
				 };
		virtual ~RemoteSSLServer(void){;};
	private:
		SYS_Lock		cgGetNewLTCPServerInUsed;
		MAPPINGServer	*cgMServer;
		inline void		NLTCPS_set(G_LOCK_VAILD blVaild = G_LOCK_ON){cgGetNewLTCPServerInUsed.Lock(blVaild);};
		inline void		NLTCPS_clr(G_LOCK_VAILD blVaild = G_LOCK_ON){cgGetNewLTCPServerInUsed.Unlock(blVaild);};
	private:
		virtual APISocket	*CreateNewSocket_TCP(const ODEV_LIST *tODEV_LIST,uint32 tSize){
			ControlSocket *tPDB = new ControlSocket(tODEV_LIST,tSize);
			if (tPDB != nullptr)
				tPDB->SetSelfName("RSS->Socket" + Str_IntToString(GetnodeID(tPDB).load()));
			return(tPDB);
		};
	public:
		MAPPINGServer	*GetNewMServer	(void);
		void			CreateNewMServer(int32 blenable);
		int32			Run				(int32 port,COMMU_DBUF::CSType tCSType){return(APISocketServer::OpenD(port,tCSType,0));};
};
//------------------------------------------------------------------------------------------//
#endif
//------------------------------------------------------------------------------------------//
#endif