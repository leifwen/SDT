/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: Terminal.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.01.25
 
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "Terminal.h"
#include "AppLayer.h"
#define LOGPRINT_ENABLE
#define LOGPRINT_ENABLE2
#include "Comm_Log.h"
//------------------------------------------------------------------------------------------//
#ifdef TerminalH
//------------------------------------------------------------------------------------------//
void TerminalSocket::Init(SDTAPP *sdtApp){
	BICThread.ThreadInit(this, &TerminalSocket::BICThreadFun,"BIC",sdtApp);
	AddThread(&BICThread);
	ClrblUseSSL();
}
//------------------------------------------------------------------------------------------//
void TerminalSocket::DoClose(void){
	ELog(this << "TerminalSocket::DoClose()");
	cgBICenv.blExit = -1;
	RSSLSocket::DoClose();
}
//------------------------------------------------------------------------------------------//
int32 TerminalSocket::DoBICThreadFun(void *p){
	SetGetDataByRead();
	SDTAPP	*sdt = static_cast<SDTAPP*>(p);
	CreateODev_G2Self();
	GetG2DefSelf()->RegisterToCache(&sdt->m_LogCache, COLRECORD::CRD_G2);
	
	SetDefConsoleAttr(&cgBICenv,sdt,&cgRxSBUF,GetG2DefSelf());
	
	while(BICThread.IsTerminated() == 0){
		if (sdt->ExecBIC(&cgBICenv) == -1)
			break;
	}
	GetG2DefSelf()->RemoveSelf();
	SetGetDataByShare();
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 TerminalSocket::BICThreadFun(void *p){
	DoBICThreadFun(p);
	SelfClose();
	return 1;
}
//------------------------------------------------------------------------------------------//
#endif
//------------------------------------------------------------------------------------------//
#ifdef Terminal_RSTH
//------------------------------------------------------------------------------------------//
enum{
	MESG_REQ_SetupTerminal = MESG_NEXT_RSSL,
	MESG_ANS_SetupTerminal,
	MESG_REQ_CloseTerminal,
	MESG_ANS_CloseTerminal,
#ifdef Terminal_LicenseH
	MESG_REQ_CreateLicense,
	MESG_ANS_CreateLicense,
	MESG_REQ_WriteLicense,
	MESG_ANS_WriteLicense,
#endif
	MESG_NEXT_TSSL,
};
//------------------------------------------------------------------------------------------//
STDSTR RSTSocket::GetMesgText(uint32 mID){
	STDSTR retStr = "";
#ifdef LOGPRINT
	retStr = TerminalSocket::GetMesgText(mID);
	if (retStr.length() > 0)
		return(retStr);
	
	retStr = "(" + Str_ToString(mID)+ ")";
	switch(mID){
		case MESG_REQ_SetupTerminal		:retStr += "MESG_REQ_SetupTerminal ";break;
		case MESG_ANS_SetupTerminal		:retStr += "MESG_ANS_SetupTerminal ";break;
		case MESG_REQ_CloseTerminal		:retStr += "MESG_REQ_CloseTerminal ";break;
		case MESG_ANS_CloseTerminal		:retStr += "MESG_ANS_CloseTerminal ";break;
#ifdef Terminal_LicenseH
		case MESG_REQ_CreateLicense		:retStr += "MESG_REQ_CreateLicense ";break;
		case MESG_ANS_CreateLicense		:retStr += "MESG_ANS_CreateLicense ";break;
		case MESG_REQ_WriteLicense		:retStr += "MESG_REQ_WriteLicense ";break;
		case MESG_ANS_WriteLicense		:retStr += "MESG_ANS_WriteLicense ";break;
#endif
		default							:retStr = "";break;
	}
#endif
	return(retStr);
}
//------------------------------------------------------------------------------------------//
void RSTSocket::Init(void){
	BICThread.RemoveSelf();
	ClrSFlag(blSetupTerminalY | blSetupTerminalN | blCloseTerminalY);
#ifdef Terminal_LicenseH
	approveTime = 0;
#endif
}
//---------------------------------MessageProcessing---------------------------------------------------------//
#define CHSendM(_mID,_strMesg,_blRet,_Title) \
	E2Log(this << _Title << "Send " << GetMesgText(_mID) << "with MESG:" << _strMesg);\
	_blRet = CtrlCHWrite(_mID,_strMesg);\
	E2Log(this << _Title << "Send " << GetMesgText(_mID) << ((_blRet > 0) ? "successful" : "fail"));
#define CHRecM(_mID,_strMesg,_Title) E2Log(this << _Title << "Rec  " << GetMesgText(_mID) << ": " << _strMesg);

#define CHSendMInMP(_mID,_strMesg,_blRet)	CHSendM(_mID,_strMesg,_blRet,"MessageProcessing()::")
#define CHSendInMP(_mID,_strMesg,_blRet)	CHSend(_mID,_strMesg,_blRet,"MessageProcessing()::")
#define CHRecInMP(_mID)						CHRec(_mID,"MessageProcessing()::");
#define CHRecMInMP(_mID,_strMesg)			CHRecM(_mID,_strMesg,"MessageProcessing()::");
#define E2LogInMP(_strMesg)					E2Log(this << "MessageProcessing()::" << _strMesg);
//------------------------------------------------------------------------------------------//
void RSTSocket::ThreadsStop(void){
	ELog(this << "RSTSocket::DoClose()");
	cgBICenv.blExit = -1;
	BICThread.ThreadStop();
	TerminalSocket::ThreadsStop();
	ClrSFlag(blSetupTerminalY | blSetupTerminalN | blCloseTerminalY | blApproveY | blApproveN);
}
//------------------------------------------------------------------------------------------//
int32 RSTSocket::BICThreadFun(void *p){
	int32	blRet;
	SetSFlag(blSetupTerminalY);
	DoBICThreadFun(p);
	
	CHSend(MESG_ANS_CloseTerminal,"",blRet,"BICThreadFun()::")
	ClrSFlag(blSetupTerminalY | blSetupTerminalN | blCloseTerminalY);
	return 0;
}
//---------------------------------MessageProcessing---------------------------------------------------------//
int32 RSTSocket::MessageProcessing(const uint32 &mID,const STDSTR &strMesg){
	STDSTR		strMesgT,strContent;
	int32		blDo,blRet;
	
	if (RSSLSocket::MessageProcessing(mID,strMesg) > 0)
		return 1;
	
	if (mID == MESG_NONE)
		return 0;
	
	blDo = 1;
	switch (mID){
		case MESG_REQ_SetupTerminal:
			CHRecInMP(mID);
			cgBICenv.blExit = -1;
			BICThread.ThreadStop();
			BICThread.ThreadRun();
			SYS_SleepMS(20);
			CHSendMInMP(MESG_ANS_SetupTerminal,((CheckSFlag(blSetupTerminalY) != 0) ? "Y" : "N"),blRet);
			break;
		case MESG_ANS_SetupTerminal:
			CHRecMInMP(mID,strMesg);
			SetSFlag((strMesg == "Y")?blSetupTerminalY:blSetupTerminalN);
			break;
		case MESG_REQ_CloseTerminal:
			CHRecInMP(mID);
			cgBICenv.blExit = -1;
			BICThread.ThreadStop();
			break;
		case MESG_ANS_CloseTerminal:
			CHRecInMP(mID);
			SetSFlag(blCloseTerminalY);
			break;
#ifdef Terminal_LicenseH
		case MESG_REQ_CreateLicense:
			CHRecInMP(mID);
			if (regS.Encode(&strMesgT,nullptr) > 0){
				CHSendInMP(MESG_ANS_CreateLicense,strMesgT,blRet);
			}
			else{
				E2LogInMP("Create Reg_Signature fail");
			}
			break;
		case MESG_ANS_CreateLicense:
			CHRecInMP(mID);
			if (gLS.Encode(&strContent, strMesg, approveTime * 60 * 60) > 0){
				CHSendInMP(MESG_REQ_WriteLicense,strContent,blRet);
			}
			else{
				E2LogInMP("Create Linense_Signature fail");
				SetSFlag(blApproveN);
			}
			approveTime = 0;
			break;
		case MESG_REQ_WriteLicense:
			CHRecInMP(mID);
			E2LogInMP("Write to License.key");
			CFS_WriteFile("License.key", strMesg);
			CHSendInMP(MESG_ANS_WriteLicense,"",blRet);
			break;
		case MESG_ANS_WriteLicense:
			CHRecInMP(mID);
			SetSFlag(blApproveY);
			break;
#endif
		default:
			blDo = 0;
			break;
	}
	return(blDo);
}
//------------------------------------------------------------------------------------------//
int32 RSTSocket::SendRequestSetupTerminal(void){
	int32	blRet;
	ClrSFlag(blCloseTerminalY);
	blRet = SendCHMesg(MESG_REQ_SetupTerminal,"",blSetupTerminalY,blSetupTerminalN,CSSL_FR_T2::HandshakeTime);
	if ((blRet == 1) || (blRet == -3))
		return 1;
	return 0;
}
//------------------------------------------------------------------------------------------//
int32 RSTSocket::SendRequestCloseTerminal(void){
	int32	blRet;
	blRet = SendCHMesg(MESG_REQ_CloseTerminal,"",blCloseTerminalY,blCloseTerminalY,CSSL_FR_T2::HandshakeTime);
	if ((blRet == 1) || (blRet == -3))
		return 1;
	return 0;
}
//------------------------------------------------------------------------------------------//
#ifdef Terminal_LicenseH
int32 RSTSocket::SendApproveSDT(uint32 approveH){
	int32	blRet;
	ClrSFlag(blApproveY);
	approveTime = approveH;
	blRet = SendCHMesg(MESG_REQ_CreateLicense,"",blApproveY,blApproveN,CSSL_FR_T2::HandshakeTime);
	if ((blRet == 1) || (blRet == -3))
		return 1;
	return 0;
}
#endif
//------------------------------------------------------------------------------------------//
#endif

