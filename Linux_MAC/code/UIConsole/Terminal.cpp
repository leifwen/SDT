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
//#define LOGPRINT_ENABLE
#include "Terminal.h"
#include "AppLayer.h"
//------------------------------------------------------------------------------------------//
void TerminalSocket::OnCloseDev(void){
	cgBICPAR.blExit = -1;
	GetcgSelfODdev()->RemoveSelf();
	APISocket::OnCloseDev();
}
//------------------------------------------------------------------------------------------//
int32 TerminalSocket::ExThreadFun(void){
	cgBICPAR.sdtApp = &GSDTApp;
	cgBICPAR.oDevNode = GetcgSelfODdev();
	cgBICPAR.charSBUF = &cgRxBuffer;
	cgBICPAR.validComList = &cgBICPAR.sdtApp->m_IPComList;
	cgBICPAR.retCommand = "";
	cgBICPAR.blExit = 0;
	cgBICPAR.blInOnlineMode = 0;
	cgBICPAR.blInPressKeyMode = 0;
	cgBICPAR.blUseSecondLH = 0;
	cgBICPAR.blDisplayAuto = 1;
	cgBICPAR.gDID = 0;
	
	cgBICPAR.sdtApp->m_oDevOutputListPool.AddNode(GetcgSelfODdev());
	SetblcgRxBufferUsed();
	while(exThread.IsTerminated() == 0){
		SYS_SleepMS(50);
		if (cgBICPAR.sdtApp->m_BIC_Console.Do(&cgBICPAR) == -1)
			break;
		if (cgBICPAR.blExit == -1)
			break;
		GetcgSelfODdev()->Print();
	}
	GetcgSelfODdev()->RemoveSelf();
	ClrblConnected();
	return 0;
}
//------------------------------------------------------------------------------------------//
void TerminalSocket::ThreadsStart(void){
	txThread.ThreadRun();
	rxThread.ThreadRun();
	exThread.ThreadRun();
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#ifdef USE_OPENSSL
//------------------------------------------------------------------------------------------//
#define SetTSSLFLAG(u64)		B_SetFLAG32(cgTSFlag,(u64))
#define ClrTSSLFLAG(u64)		B_ClrFLAG32(cgTSFlag,(u64))
#define ChkTSSLFLAG(u64)		B_ChkFLAG32(cgTSFlag,(u64))
enum{
	blThreadRun		= BD_FLAG64(0),
	blThreadRunY	= BD_FLAG64(1),
	blThreadRunN	= BD_FLAG64(2),
	blThreadStop	= BD_FLAG64(2),
};
//------------------------------------------------------------------------------------------//
enum{
	MESG_REQ_SetupTerminal = MESG_NEXT_RSSL,
	MESG_ANS_SetupTerminal,
	MESG_REQ_CloseTerminal,
	MESG_ANS_CloseTerminal,
	MESG_NEXT_TSSL,
};
//------------------------------------------------------------------------------------------//
void RSTSocket::Init(void){
	CreateSelfODev(COLSTRING::COLType_TXT);
	commandThread.ThreadInit(this, &RSTSocket::CommandThreadFun);
	cgThreadList.AddNode(&commandThread);
	ClrTSSLFLAG(blThreadRun | blThreadRunY | blThreadRunN | blThreadStop);
}
//------------------------------------------------------------------------------------------//
void RSTSocket::OnCloseDev(void){
	cgBICPAR.blExit = -1;
	GetcgSelfODdev()->RemoveSelf();
	ControlSocket::OnCloseDev();
}
//------------------------------------------------------------------------------------------//
int32 RSTSocket::CommandThreadFun(void){
	int32 	blRet;
	if (CheckHandshake() > 0){
		cgBICPAR.sdtApp = &GSDTApp;
		cgBICPAR.oDevNode = GetcgSelfODdev();
		cgBICPAR.charSBUF = &cgDataCHSBUF;
		cgBICPAR.validComList = &cgBICPAR.sdtApp->m_IPComList;
		cgBICPAR.retCommand = "";
		cgBICPAR.blExit = 0;
		cgBICPAR.blInOnlineMode = 0;
		cgBICPAR.blInPressKeyMode = 0;
		cgBICPAR.blUseSecondLH = 0;
		cgBICPAR.blDisplayAuto = 1;
		cgBICPAR.gDID = 0;
		
		cgBICPAR.sdtApp->m_oDevOutputListPool.AddNode(GetcgSelfODdev());
		SetTSSLFLAG(blThreadRunY);
		SetTSSLFLAG(blThreadRun);
		SYS_SleepMS(2);
		while(commandThread.IsTerminated() == 0){
			SYS_SleepMS(50);
			if (cgBICPAR.sdtApp->m_BIC_Console.Do(&cgBICPAR) == -1)
				break;
			if (cgBICPAR.blExit == -1)
				break;
			GetcgSelfODdev()->Print();
		}
		GetcgSelfODdev()->RemoveSelf();
	}
	ELogPrint(this, "CommandThreadFun()::Send MESG_ANS_CloseTerminal");
	blRet = CtrlCHWrite("",MESG_ANS_SetupTerminal);
	ELogPrint(this, "CommandThreadFun()::Send MESG_ANS_CloseTerminal %s",(blRet > 0) ? "successful" : "fail");
	ClrTSSLFLAG(blThreadRun | blThreadRunY | blThreadRunN);
	return 0;
}
//------------------------------------------------------------------------------------------//
int32 RSTSocket::MessageProcessing(FNode_MESG *RecMesg,int32 blReady){
	std::string		strMesg,strContent;
	uint32 			mID;
	int32			blDo,blRet;
	
	if (ControlSocket::MessageProcessing(RecMesg,blReady) > 0)
		return 1;
	
	if (blReady == 0)
		return 0;
	
	blDo = 1;
	RecMesg->ReadContent(&strMesg,&mID);
	switch (mID){
		case MESG_REQ_SetupTerminal:
			ELogPrint(this, "MessageProcessing()::Rec  MESG_REQ_SetupTerminal");
			commandThread.ThreadRun();
			SYS_SleepMS(10);
			ELogPrint(this, "MessageProcessing()::Send MESG_ANS_SetupTerminal with MESG:%s",(ChkTSSLFLAG(blThreadRunY) > 0) ? "Y" : "N");
			blRet = CtrlCHWrite((ChkTSSLFLAG(blThreadRunY) > 0) ? "Y" : "N",MESG_ANS_SetupTerminal);
			ELogPrint(this, "MessageProcessing()::Send MESG_ANS_SetupTerminal %s",(blRet > 0) ? "successful" : "fail");
			break;
		case MESG_ANS_SetupTerminal:
			ELogPrint(this, "MessageProcessing()::Rec  MESG_ANS_SetupTerminal:%s",strMesg.c_str());
			if (strMesg == "Y")
				SetTSSLFLAG(blThreadRunY);
			SetTSSLFLAG(blThreadRun);
			break;
		case MESG_REQ_CloseTerminal:
			ELogPrint(this, "MessageProcessing()::Rec MESG_REQ_CloseTerminal");
			cgBICPAR.blExit = -1;
			commandThread.ThreadStop();
			break;
		case MESG_ANS_CloseTerminal:
			ELogPrint(this, "MessageProcessing()::Rec  MESG_ANS_CloseTerminal");
			ELogPrint(this, "MessageProcessing()::Send MESG_REQ_CloseTerminal");
			blRet = CtrlCHWrite("",MESG_REQ_CloseTerminal);
			ELogPrint(this, "MessageProcessing()::Send MESG_REQ_CloseTerminal %s",(blRet > 0) ? "successful" : "fail");
			SetTSSLFLAG(blThreadRunN);
			SetTSSLFLAG(blThreadRun);
			break;
		default:
			blDo = 0;
			break;
	}
	return(blDo);
}
//------------------------------------------------------------------------------------------//
int32 RSTSocket::SendRequestSetupTerminal(void){
	SYS_TIME_S		Timedly;
	std::string		strMesg;
	
	ClrTSSLFLAG(blThreadRun | blThreadRunY | blThreadRunN);
	ELogPrint(this, "Send MESG_REQ_SetupTerminal");
	if (CtrlCHWrite(strMesg,MESG_REQ_SetupTerminal) > 0){
		SYS_Delay_SetTS(&Timedly, 1000);
		while((ChkTSSLFLAG(blThreadRun) == 0) && (IsConnected() > 0) && (SYS_Delay_CheckTS(&Timedly) == 0))
			SYS_SleepMS(100);
	}
	ClrTSSLFLAG(blThreadRun);
	return(ChkTSSLFLAG(blThreadRunY));
}
//------------------------------------------------------------------------------------------//
int32 RSTSocket::SendRequestCloseTerminal(void){
	SYS_TIME_S		Timedly;
	std::string		strMesg;
	
	ClrTSSLFLAG(blThreadRun | blThreadRunY | blThreadRunN);
	ELogPrint(this, "Send MESG_REQ_SetupTerminal");
	if (CtrlCHWrite(strMesg,MESG_REQ_CloseTerminal) > 0){
		SYS_Delay_SetTS(&Timedly, 1000);
		while((ChkTSSLFLAG(blThreadRun) == 0) && (IsConnected() > 0) && (SYS_Delay_CheckTS(&Timedly) == 0))
			SYS_SleepMS(100);
	}
	ClrTSSLFLAG(blThreadRun);
	return(ChkTSSLFLAG(blThreadRunN));
}
//------------------------------------------------------------------------------------------//
int32 RSTSocket::CheckRemoteTerminalStatus(void){
	return(ChkTSSLFLAG(blThreadRun | blThreadRunN));
}
//------------------------------------------------------------------------------------------//
#endif

