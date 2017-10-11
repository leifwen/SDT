/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: License_SSLSocket.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2015.10.26
*/

//------------------------------------------------------------------------------------------//
#include "Commu_Socket.h"
#include "License_Checker.h"
#if defined Commu_SSLSocketH && defined License_CheckerH
#ifndef License_SSLSocketH
#define License_SSLSocketH
//------------------------------------------------------------------------------------------//
#ifdef License_SSLSocketH
//------------------------------------------------------------------------------------------//
class LicenseSSLSocket : public BSOCKET{
	public:
		enum{RFLAG_C = 2, RFLAG_S = BSOCKET::RFLAG_S + BSOCKET::RFLAG_C};
	private:
		enum {blApprove	= RFLAG_CREATE(0),blReject	= RFLAG_CREATE(1),};
	public:
				 LicenseSSLSocket(uint32 tSize): BSOCKET(tSize,nullptr){SetblUseSSL();SetSelfName("LicenseSSLSocket");};
		virtual ~LicenseSSLSocket(void){;};
	protected:
		virtual	STDSTR	GetMesgText			(uint32 mID);
	private:
		virtual	int32	MessageProcessing	(const uint32 &mID,const STDSTR &strMesg);
		Linense_Signature	gLS;
	public:
		int32	CreateRegSignature(STDSTR *strReg,STDSTR *retStatus);
		int32	SendRequestLicense(const STDSTR &regSignature,uint32 waitTimeS);
};
//------------------------------------------------------------------------------------------//
class LicenseSSLServer : public BSOCKETSERVER{
	public:
		enum{RFLAG_C = 2, RFLAG_S = BSOCKETSERVER::RFLAG_S + BSOCKETSERVER::RFLAG_C};
	private:
		enum {blAnswer	= RFLAG_CREATE(0),blRequest	= RFLAG_CREATE(1),};
	public:
				 LicenseSSLServer(int32 tSize) : BSOCKETSERVER(tSize,nullptr){ClrSFlag(blAnswer | blRequest);SetSelfName("LicenseSSLServer");};
		virtual ~LicenseSSLServer(void){;};
	private:
		inline	virtual	TREE_NODE*	CreateNode(void){return(new LicenseSSLSocket(cgBufMaxSize));};
	private:
		SYS_Lock		cgRequestLock;
		inline	void	Spin_Request_Lock	(G_LOCK blVaild = G_LOCK_ON){cgRequestLock.Lock(blVaild);};
		inline	void	Spin_Request_Unlock	(G_LOCK blVaild = G_LOCK_ON){cgRequestLock.Unlock(blVaild);};
		inline	int32	Spin_Request_Try	(G_LOCK blVaild = G_LOCK_ON){return(cgRequestLock.TryLock(blVaild));};
		LicenseSSLSocket	*cgRequestSocket;
		uint64				*cgApproveSecond;
	public:
		inline	int32	CheckblRequest		(void)const	{return(CheckSFlag(blRequest));};
				int32	CheckApprove		(LicenseSSLSocket *tSocket,uint64 *retSeconds);
				void	ApproveRegistration	(const uint64 &approveSecond);
				void	RejectRegistration	(void);
		const STDSTR	&RequestSocketInfo	(STDSTR *strPrint);
	public:
				int32	Open				(int32 port){return(BSOCKETSERVER::OpenD("",port,CSType_TCPS,0));};
};
//------------------------------------------------------------------------------------------//
#endif
#endif
#endif
