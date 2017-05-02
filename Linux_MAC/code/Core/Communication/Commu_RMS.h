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
//------------------------------------------------------------------------------------------//
#include "Commu_Socket.h"
#ifdef Commu_SSLSocketH
#ifndef Commu_RMSH
#define Commu_RMSH
#ifdef Commu_RMSH
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
class SOCKET_POOL : public COMMU_DBUF_FRAME_LOG{
	public:
			 SOCKET_POOL(void) : COMMU_DBUF_FRAME_LOG(){SetSelfName("");};
	virtual ~SOCKET_POOL(void){ChildCloseAll();DestroyAll();};
	public:
		void	Init		(uint32 tSize,const ODEV_SYSTEM *logSys = nullptr){
			COMMU_DBUF_FRAME_LOG::Init(0,logSys);
			cgBufMaxSize = tSize;
			CreateTrash(this);
		};
};
//------------------------------------------------------------------------------------------//
class POOL_SSLSOCKET : public SOCKET_POOL{
	public:
				 POOL_SSLSOCKET(void) : SOCKET_POOL(){SetSelfName("POOLS");};
		virtual ~POOL_SSLSOCKET(void){;};
	public:
		virtual	TNFP*	CreateNode(void);
};
//------------------------------------------------------------------------------------------//
class POOL_BSOCKET : public SOCKET_POOL{
	public:
				 POOL_BSOCKET(void) : SOCKET_POOL(){SetSelfName("POOLB");};
		virtual ~POOL_BSOCKET(void){;};
	public:
		virtual	TNFP*	CreateNode(void);
};
//------------------------------------------------------------------------------------------//
class BSOCKET_C;
class RSSLSocket;
//------------------------------------------------------------------------------------------//
class CPPOOL : public TREE_NODE{
	public:
				 CPPOOL(void) : TREE_NODE(){SetSelfName("CPPOOL");};
		virtual ~CPPOOL(void){ChildCloseAll();};
	public:
		inline	virtual void	SetSelfName	(const STDSTR &strName)	{selfName = strName;cgPoolS.SetSelfName(strName);cgPoolB.SetSelfName(strName);};
	public:
		POOL_SSLSOCKET	cgPoolS;
		POOL_BSOCKET	cgPoolB;
	public:
		void	Init			(uint32 tSize,const ODEV_SYSTEM *logSys = nullptr){cgPoolS.Init(tSize,logSys);cgPoolB.Init(tSize,logSys);};
		int32	GetCP			(RSSLSocket **rsslsocket,BSOCKET_C **bsocket);
		void	ChildClose		(RSSLSocket *rsslsocket,BSOCKET_C *bsocket);
		void	ChildCloseAll	(void){cgPoolS.ChildCloseAll();cgPoolB.ChildCloseAll();};
};
//------------------------------------------------------------------------------------------//
class BSOCKET_C : public BSOCKET{
	public:
				 BSOCKET_C(uint32 tSize,const ODEV_SYSTEM *logSys = nullptr) : BSOCKET(tSize,logSys){SetSelfName("BSOCKET");};
		virtual ~BSOCKET_C(void){;};
	protected:
		virtual	void	DoClose(void);
};
//------------------------------------------------------------------------------------------//
class MAPPINGServer : public BSOCKETSERVER{
	public:
				 MAPPINGServer(uint32 tSize,const ODEV_SYSTEM *logSys = nullptr) : BSOCKETSERVER(tSize,logSys){cgMapIP = "";cgMapPort = 0;SetSelfName("MServer");};
		virtual ~MAPPINGServer(void){;};
	private:
		inline	virtual	TNFP*	CreateNode		(void){return(SetSubNodeSelfName(new BSOCKET_C(cgBufMaxSize,GetLogSystem())));};
				virtual int32	OnOpenTCPSocket	(ASOCKET *newSocket);
	private:
		STDSTR	cgMapIP;
		int32	cgMapPort;
	public:
		inline	const	STDSTR&	GetMapIP	(void){return(cgMapIP);};
		inline			int32	GetMapPort(void){return(cgMapPort);};
	public:
		int32	Open(int32 localPort,const STDSTR &mapIP,int32 mapPort,CSType tCSType = CSType_TCPS);
};
//------------------------------------------------------------------------------------------//
class RSSLSocket : public BSOCKET_C{
	public:
		enum	{RFLAG_C = 8, RFLAG_S = BSOCKET_C::RFLAG_S + BSOCKET_C::RFLAG_C};
	private:
		enum{
			blSetupMServerY	= RFLAG_CREATE(0),
			blSetupMServerN	= RFLAG_CREATE(1),
			blOffMServerY	= RFLAG_CREATE(2),
			blOffMServerN	= RFLAG_CREATE(3),
			blListMServerY	= RFLAG_CREATE(4),
			blListMServerN	= RFLAG_CREATE(5),
			blLinkY			= RFLAG_CREATE(6),
			blLinkN			= RFLAG_CREATE(7),
		};
	public:
				 RSSLSocket(uint32 tSize,const ODEV_SYSTEM *logSys = nullptr) : BSOCKET_C(tSize,logSys){
					 cgCPPool.Init(tSize,logSys);
					 SetSelfName("RSSLSocket");
					 cgCPPool.SetSelfName(selfName);
					 SetblUseSSL();
					 CreateTrash(this);
				 };
		virtual ~RSSLSocket(void){cgCPPool.ChildCloseAll();ChildCloseAll();DestroyAll();};
	public:
	CPPOOL	cgCPPool;
	protected:
		virtual	STDSTR	GetMesgText			(uint32 mID);
		virtual	void	DoClose				(void);
		virtual	int32	MessageProcessing	(const uint32 &mID,const STDSTR &strMesg);
		virtual	TNFP*	CreateNode			(void){return(SetSubNodeSelfName(new MAPPINGServer(cgBufMaxSize,GetLogSystem())));};
	private:
		int32	OpenMappingServer			(MAPPINGServer *retMserver,const STDSTR &strMesg);
		int32	CloseMappingServer			(const STDSTR &strMesg);
		int32	ListMappingServer			(STDSTR *strMesg);
		int32	OpenCoupleSocket			(RSSLSocket **rsslsocket,BSOCKET_C **bsocket,STDSTR strMesg);
		int32	SendRequestLink				(const STDSTR &strMesg);
	public:
		int32	SendRequestSetupMServer		(int32 remotePort,const STDSTR &mapIP,int32 mapPort,CSType tCSType = CSType_TCPS);
		int32	SendRequestOffMServer		(int32 remotePort);
		int32	SendRequestListMServer		(STDSTR *retStr);
		int32	SendRequestSetupDataChannel	(ASOCKET *mSocket,const STDSTR &mapIP,int32 mapPort,CSType tCSType = CSType_TCPS);
};
//------------------------------------------------------------------------------------------//
class RSSLServer : public BSOCKETSERVER{
	public:
				 RSSLServer(uint32 tSize,const ODEV_SYSTEM *logSys = nullptr) : BSOCKETSERVER(tSize,logSys){SetSelfName("RSSLServer");};
		virtual ~RSSLServer(void){;};
	private:
		inline	virtual	TNFP*	CreateNode(void){return(SetSubNodeSelfName(new RSSLSocket(cgBufMaxSize,GetLogSystem())));};
	public:
		int32	Open	(int32 port,CSType tCSType){return(BSOCKETSERVER::OpenD("",port,tCSType,0));};
};
//------------------------------------------------------------------------------------------//
#endif
#endif
#endif
