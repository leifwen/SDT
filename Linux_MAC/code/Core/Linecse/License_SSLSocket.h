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
		enum{RFLAG_C = 1, RFLAG_S = SSLSocket::RFLAG_S + SSLSocket::RFLAG_C};
				 LicenseSSLSocket(const ODEV_LIST *tODEV_LIST,int32 tSize): SSLSocket(tODEV_LIST,tSize){selfName = "LicenseSSLSocket ";};
		virtual ~LicenseSSLSocket(void){;};
	private:
		virtual	void	PrintUserDisconnectReport(const std::string &strDevName){;};
		virtual	void	PrintConnectInfo	(int32 blSendWelcome){;};
		virtual	void	PrintDisconnectInfo	(void){;};
		virtual	int32	PrintThreadFun		(void);
	private:
		inline	void	SetblANSLicense		(void)		{SetSFlag(RFLAG_CREATE(0));};
		inline	void	ClrblANSLicense		(void)		{ClrSFlag(RFLAG_CREATE(0));};
		inline	int32	CheckblANSLicense	(void)const	{return(CheckSFlag(RFLAG_CREATE(0)));};
				int32	CheckANSLicense		(uint32 waitTimeS);
	public:
		int32	GetLicense(uint32 waitTimeS,std::string *retStatus = nullptr);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
class LicenseSSLSocketS : public LicenseSSLSocket{
	public:
		enum{RFLAG_C = 0, RFLAG_S = LicenseSSLSocket::RFLAG_S + LicenseSSLSocket::RFLAG_C};
				 LicenseSSLSocketS(const ODEV_LIST *tODEV_LIST,int32 tSize): LicenseSSLSocket(tODEV_LIST,tSize){selfName = "LicenseSSLSocketS";};
		virtual ~LicenseSSLSocketS(void){;};
	private:
	
		virtual	void	Handshake	(CCY_FNLC_MESG *fnMesg){;};
		virtual int32	OpenDev		(const std::string &tCDBufName,int32 tCDBufPar,CSType tCSType){return 1;};
};
//------------------------------------------------------------------------------------------//
class LicenseSSLServer : public APISocketServer{
	public:
		enum{RFLAG_C = 0, RFLAG_S = APISocketServer::RFLAG_S + APISocketServer::RFLAG_C};
				 LicenseSSLServer(const ODEV_LIST *tODEV_LIST,int32 tSize) : APISocketServer(tODEV_LIST,tSize){
					 selfName = "LicenseSSLServer";
					 cgLicenseSBUF.Init(tSize);
					 cgfnMesg.Init(&cgLicenseSBUF.cgBufFIFO);
				 };
		virtual ~LicenseSSLServer(void){;};
	private:
		PUB_SBUF		cgLicenseSBUF;
		CCY_FNLC_MESG 	cgfnMesg;
		virtual APISocket	*CreateNewSocket_TCP(const ODEV_LIST *tODEV_LIST,uint32 tSize){
			LicenseSSLSocketS *tPDB;
			tPDB = new LicenseSSLSocketS(tODEV_LIST,tSize);
				AddNode(tPDB);
			return(tPDB);
		};
	public:
		void	SendRegistrationMesg(const std::string &strMesg,uint32 mID){cgfnMesg.SetContent(strMesg, mID);};
		void	ApproveRegistration	(const uint64 &approveSecond);
		void	RejectRegistration	(void);
		int32	GetRegistration		(uint32 *mID);
	
		int32	Run		(int32 port){return(APISocketServer::OpenD(port,COMMU_DBUF::CSType_TCP,0));};
};
//------------------------------------------------------------------------------------------//
#endif
//------------------------------------------------------------------------------------------//
#endif