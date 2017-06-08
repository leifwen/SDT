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

//------------------------------------------------------------------------------------------//
#include "Commu_RMS.h"
#include "BIC.h"
#include "License_Checker.h"
//------------------------------------------------------------------------------------------//
#if defined Commu_SSLSocketH && defined License_CheckerH
#ifndef Terminal_LicenseH
#define Terminal_LicenseH
#endif
#endif
#ifdef Commu_RMSH
#ifndef TerminalH
#define TerminalH
#ifdef TerminalH
//------------------------------------------------------------------------------------------//
class TerminalSocket : public RSSLSocket{
	public:
				 TerminalSocket(uint32 tSize,SDTAPP *sdtApp): RSSLSocket(tSize,nullptr){Init(sdtApp);SetSelfName("TerminalSocket");};
		virtual ~TerminalSocket(void){BICThread.RemoveSelf();};
	protected:
		BIC_ENV			cgBICenv;
		SBUFFER			cmdSBUF;
		SYS_Thread<TerminalSocket>	BICThread;
	private:
				void	Init		(SDTAPP *sdtApp);
		virtual	void	DoClose		(void);
	protected:
				int32	DoBICThreadFun		(void *p);
		virtual	int32	BICThreadFun		(void *p);
};
//------------------------------------------------------------------------------------------//
class TerminalServer : public BSOCKETSERVER{
	public:
				 TerminalServer(uint32 tSize,SDTAPP *sdtApp) : BSOCKETSERVER(tSize,nullptr){SetSelfName("TS");cgSDTApp = sdtApp;};
		virtual ~TerminalServer(void){;};
	protected:
		SDTAPP	*cgSDTApp;
	protected:
		virtual	TNFP*	CreateNode		(void){return(SetSubNodeSelfName(new TerminalSocket(cgBufMaxSize,cgSDTApp)));};
	public:
				int32	Open			(int32 port){return(BSOCKETSERVER::OpenD("",port,CSType_TCPS,0));};
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#ifndef Terminal_RSTH
#define Terminal_RSTH
#ifdef Terminal_RSTH
//------------------------------------------------------------------------------------------//
class RSTSocket : public TerminalSocket{
	public:
		enum{RFLAG_C = 5, RFLAG_S = TerminalSocket::RFLAG_S + TerminalSocket::RFLAG_C};
	private:
		enum{blSetupTerminalY = RFLAG_CREATE(0),blSetupTerminalN = RFLAG_CREATE(1),blCloseTerminalY = RFLAG_CREATE(2)
			,blApproveY = RFLAG_CREATE(3),blApproveN = RFLAG_CREATE(4),};
	public:
				 RSTSocket(uint32 tSize,SDTAPP *sdtApp): TerminalSocket(tSize,sdtApp){Init();SetblUseSSL();SetSelfName("RSTCilent");};
		virtual ~RSTSocket(void){;};
	private:
#ifdef Terminal_LicenseH
		Linense_Signature	gLS;
		Reg_Signature		regS;
		uint32				approveTime;
#endif
	private:
				void	Init				(void);
				int32	BICThreadFun		(void *p);
		virtual	void	ThreadsStop			(void);
		virtual	STDSTR	GetMesgText			(uint32 mID);
		virtual	int32	MessageProcessing	(const uint32 &mID,const STDSTR &strMesg);
	public:
				int32	SendRequestSetupTerminal	(void);
				int32	SendRequestCloseTerminal	(void);
				int32	CheckTerminalClosed			(void){return(CheckSFlag(blCloseTerminalY));};
#ifdef Terminal_LicenseH
				int32	SendApproveSDT				(uint32 approveH);
#endif
};
//------------------------------------------------------------------------------------------//
class RSTServer : public TerminalServer{
	public:
				 RSTServer(uint32 tSize,SDTAPP *sdtApp) : TerminalServer(tSize,sdtApp){selfName = "RST";};
		virtual ~RSTServer(void){;};
	private:
		virtual	TNFP*	CreateNode		(void){return(SetSubNodeSelfName(new RSTSocket(cgBufMaxSize,cgSDTApp)));};
};
//------------------------------------------------------------------------------------------//
#endif
#endif
#endif
#endif
#endif
