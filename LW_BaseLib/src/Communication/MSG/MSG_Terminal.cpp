//
//  MSG_Terminal.cpp
//  SDT
//
//  Created by Leif Wen on 16/03/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "MSG_ID.h"
#include "MSG_Terminal.h"
#include "Commu_Base.h"
#ifdef MSG_Terminal_h
//------------------------------------------------------------------------------------------//
//#define LOGPRINT_ENABLE
//#define LOGTHREAD_ENABLE
//#define LOGMSG_ENABLE

#ifdef LOGMSG_ENABLE
#define ELogMSG(x) ELog(x);
#else
#define ELogMSG(x);
#endif

#include "SYS_Log.h"
//------------------------------------------------------------------------------------------//
MSG_Terminal::MSG_Terminal(void) : MSG_NODE(){
	cgMSGID = MESG_ID_TERMINAL;
	BICThread.ThreadInit(this, &MSG_Terminal::BICThreadFun, "BICThread");
	BICThread.ThreadInit(&MSG_Terminal::BICThreadOnStop);
	
	selfName = "MSG_TERMINAL";
	BICThread.SetSelfName("BICThread");
	SetUpName("");
};
//------------------------------------------------------------------------------------------//
void MSG_Terminal::Reset(void* _team){
#ifdef Commu_Base_h
	COMMU_TEAM*		team = static_cast<COMMU_TEAM*>(_team);
	COMMU_FRAME*	commu = team->commu;
	SYS_AThread*	thread = nullptr;

	thread = commu->ThreadGet("BICThread");
	TNF::Disable(thread);
	ClrSFlag(TER_blSetupTerminalY | TER_blSetupTerminalN | TER_blCloseTerminalY);
#endif
};
//------------------------------------------------------------------------------------------//
void MSG_Terminal::SetSelfName(const STDSTR& strName){
	selfName = strName;
	BICThread.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
void MSG_Terminal::SetUpName(const STDSTR& strName){
	fatherName = strName;
	BICThread.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
void MSG_Terminal::Close(void* _team){
	BICThread.ThreadStop(_team);
};
//------------------------------------------------------------------------------------------//
bool32 MSG_Terminal::BICThreadFun(void* _team){
#ifdef Commu_Base_h
	COMMU_TEAM*		team = static_cast<COMMU_TEAM*>(_team);
	COMMU_FRAME*	commu = team->commu;
	SYS_AThread*	thread = nullptr;
	bool32			blRet;
	ETLogThreadStart(BICThread);
	ClrSFlag(TER_blSetupTerminalY | TER_blSetupTerminalN | TER_blCloseTerminalY);

	thread = commu->ThreadGet("BICThread");

	if (thread != nullptr){
		SetSFlag(TER_blSetupTerminalY);
		MSGSend(MESG_ANS_SetupTerminal,IUD("Y"),blRet,"BICThreadFun","Send");
		
		thread->Execute(_team);
	
		MSGSend(MESG_ANS_CloseTerminal,IUD(""),blRet,"BICThreadFun","Send");
	}
	else{
		MSGSend(MESG_ANS_SetupTerminal,IUD("N"),blRet,"BICThreadFun","Send");
	}
	ClrSFlag(TER_blSetupTerminalY | TER_blSetupTerminalN | TER_blCloseTerminalY);
	ETLogThreadStop(BICThread);
#endif
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
bool32 MSG_Terminal::BICThreadOnStop(void* _team){
#ifdef Commu_Base_h
	COMMU_TEAM*		team = static_cast<COMMU_TEAM*>(_team);
	COMMU_FRAME*	commu = team->commu;
	SYS_AThread*	thread;
	thread = commu->ThreadGet("BICThread");
	if (thread != nullptr)
		thread->ThreadStop(_team);
#endif
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
bool32 MSG_Terminal::Send_REQ_SetupTerminal(CMD_ENV* env){
	bool32	blRet;
	ClrSFlag(TER_blCloseTerminalY);
	MSGSend2(env,MESG_REQ_SetupTerminal,IUD("")
			 ,TER_blSetupTerminalY
			 ,TER_blSetupTerminalN
			 ,MSG_CENTER::HandshakeTime,blRet
			 ,"MSG_Terminal"
			 ,"Send_REQ_SetupTerminal");
	
	if ((blRet == 1) || (blRet == -3))
		return G_TRUE;
	return G_FALSE;
};
//------------------------------------------------------------------------------------------//
bool32 MSG_Terminal::Send_REQ_CloseTerminal(CMD_ENV* env){
	bool32	blRet;
	
	MSGSend2(env,MESG_REQ_CloseTerminal,IUD("")
			 ,TER_blCloseTerminalY
			 ,TER_blCloseTerminalY
			 ,MSG_CENTER::HandshakeTime,blRet
			 ,"MSG_Terminal"
			 ,"Send_REQ_CloseTerminal");

	if ((blRet == 1) || (blRet == -3))
		return G_TRUE;
	return G_FALSE;
};
//------------------------------------------------------------------------------------------//
bool32 MSG_Terminal::CheckTerminalClosed(void){
	
	return(CheckSFlag(TER_blCloseTerminalY));
};
//------------------------------------------------------------------------------------------//
CMDID MSG_Terminal::MessageProcessing(CMD_ENV* env,const uint32& mID,const STDSTR& msg,void* _team){
	CMDID	retCode = mID;
	
#ifdef LOGMSG_ENABLE
	switch (mID){
		case MESG_REQ_SetupTerminal:;
		case MESG_REQ_CloseTerminal:;
		case MESG_ANS_CloseTerminal:;
			ELogMPRecMsg(mID);
			break;
		case MESG_ANS_SetupTerminal:;
			ELogMPRecMsg2(mID,msg);
		default:
			break;
	}
#endif
	
	switch (mID){
		case MESG_REQ_SetupTerminal:
			BICThread.ThreadStop(_team);
			BICThread.ThreadRun(_team);
			break;
		case MESG_ANS_SetupTerminal:
			SetSFlag((msg == "Y") ? TER_blSetupTerminalY : TER_blSetupTerminalN);
			break;
		case MESG_REQ_CloseTerminal:
			BICThread.ThreadStop(_team);
			break;
		case MESG_ANS_CloseTerminal:
			SetSFlag(TER_blCloseTerminalY);
			break;
		default:
			retCode = MESG_NONE;
			break;
	}
	return(retCode);
};
//------------------------------------------------------------------------------------------//
#endif /* MSG_Terminal_h */
