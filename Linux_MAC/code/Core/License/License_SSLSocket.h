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
#ifndef License_SSLSocketH
#define License_SSLSocketH
//------------------------------------------------------------------------------------------//
#include "Commu_SSLSocket.h"
#include "Commu_SocketServer.h"
//------------------------------------------------------------------------------------------//
#ifdef USE_OPENSSL
//------------------------------------------------------------------------------------------//
class LicenseSSLSocket : public SSLSocket{
	public:
		enum{RFLAG_C = 2, RFLAG_S = SSLSocket::RFLAG_S + SSLSocket::RFLAG_C};
				 LicenseSSLSocket(const ODEV_LIST *tODEV_LIST,int32 tSize): SSLSocket(nullptr,tSize){SetSelfName("LicenseSSLSocket");};
		virtual ~LicenseSSLSocket(void){;};
	private:
		virtual	void	PrintUserDisconnectReport(const std::string &strDevName){;};
		virtual	void	PrintConnectInfo	(int32 blSendWelcome){;};
		virtual	void	PrintDisconnectInfo	(void){;};
		virtual	int32	MessageProcessing	(FNode_MESG *RecMesg,int32 blReady);
	private:
		inline	void	SetblANSLicense		(void)		{SetSFlag(RFLAG_CREATE(0));};
		inline	void	ClrblANSLicense		(void)		{ClrSFlag(RFLAG_CREATE(0));};
		inline	int32	CheckblANSLicense	(void)const	{return(CheckSFlag(RFLAG_CREATE(0)));};
		inline	void	SetblANSLicenseOK	(void)		{SetSFlag(RFLAG_CREATE(1));};
		inline	void	ClrblANSLicenseOK	(void)		{ClrSFlag(RFLAG_CREATE(1));};
		inline	int32	CheckblANSLicenseOK	(void)const	{return(CheckSFlag(RFLAG_CREATE(1)));};
	public:
		int32	CreateRegSignature(std::string *strReg,std::string *retStatus);
		int32	SendRequestLicense(const std::string &regSignature,uint32 waitTimeS);
};
//------------------------------------------------------------------------------------------//
class LicenseSSLServer : public APISocketServer{
	public:
		enum{RFLAG_C = 2, RFLAG_S = APISocketServer::RFLAG_S + APISocketServer::RFLAG_C};
				 LicenseSSLServer(int32 tSize) : APISocketServer(nullptr,tSize){ClrblApprove();ClrblRequset();selfName = "LicenseSSLServer";};
		virtual ~LicenseSSLServer(void){;};
	private:
		virtual APISocket	*CreateNewSocket_TCP(const ODEV_LIST *tODEV_LIST,uint32 tSize){
			LicenseSSLSocket *tPDB = new LicenseSSLSocket(tODEV_LIST,tSize);
			return(tPDB);
		};
	private:
		SYS_Lock		cgRequestLock;
		inline	void	Spin_Request_Lock	(G_LOCK_VAILD blVaild = G_LOCK_ON){cgRequestLock.Lock(blVaild);};
		inline	void	Spin_Request_Unlock	(G_LOCK_VAILD blVaild = G_LOCK_ON){cgRequestLock.Unlock(blVaild);};
		inline	int32	Spin_Request_Try	(G_LOCK_VAILD blVaild = G_LOCK_ON){return(cgRequestLock.TryLock(blVaild));};
		LicenseSSLSocket	*cgRequestSocket;
		uint64				*cgApproveSecond;
	private:
		inline	void	SetblApprove		(void)		{SetSFlag(RFLAG_CREATE(0));};
		inline	void	ClrblApprove		(void)		{ClrSFlag(RFLAG_CREATE(0));};
		inline	int32	CheckblApprove		(void)const	{return(CheckSFlag(RFLAG_CREATE(0)));};
	
		inline	void	SetblRequest		(void)		{SetSFlag(RFLAG_CREATE(1));};
		inline	void	ClrblRequset		(void)		{ClrSFlag(RFLAG_CREATE(1));};
	public:
		inline	int32	CheckblRequest		(void)const	{return(CheckSFlag(RFLAG_CREATE(1)));};
				int32	CheckApprove		(LicenseSSLSocket *tSocket,uint64 *retSeconds);
				void	ApproveRegistration	(const uint64 &approveSecond);
				void	RejectRegistration	(void);
		const std::string	&RequestSocketInfo(std::string *strPrint);
	
		int32	Run		(int32 port){return(APISocketServer::OpenD(port,COMMU_DBUF::CSType_TCP,0));};
};
//------------------------------------------------------------------------------------------//
#endif
//------------------------------------------------------------------------------------------//
#endif