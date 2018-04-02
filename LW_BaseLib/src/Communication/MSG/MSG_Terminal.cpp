//
//  MSG_Terminal.cpp
//  SDT
//
//  Created by Leif Wen on 16/03/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
#include "MSG_ID.h"
#include "MSG_Terminal.h"
#include "Commu_DBuf.h"
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
MSG_Terminal::MSG_Terminal(uint32 size,void* p) : MSG_NODE(size,p){
	cgMSGID = MESG_ID_TERMINAL;
	BICThread.ThreadInit(this, &MSG_Terminal::BICThreadFun, "BICThread");
	BICThread.ThreadInit(&MSG_Terminal::BICThreadOnStop);
	
	selfName = "MSG_TERMINAL";
	SetFatherName("");
}
//------------------------------------------------------------------------------------------//
void MSG_Terminal::Reset(void* commu){
	COMMU_THREAD*	_commu = static_cast<COMMU_THREAD*>(commu);
	SYS_AThread*	thread = _commu->GetThread("BICThread");
	if (thread != nullptr)
		thread->Disable();
	ClrSFlag(TER_blSetupTerminalY | TER_blSetupTerminalN | TER_blCloseTerminalY);
};
//------------------------------------------------------------------------------------------//
void MSG_Terminal::Close(void* commu){
	BICThread.ThreadStop(commu);
};
//------------------------------------------------------------------------------------------//
bool32 MSG_Terminal::BICThreadFun(void* commu){
	COMMU_THREAD*	_commu = static_cast<COMMU_THREAD*>(commu);
	SYS_AThread*	thread;
	bool32			blRet;
	ETLogThreadStart(BICThread);
	ClrSFlag(TER_blSetupTerminalY | TER_blSetupTerminalN | TER_blCloseTerminalY);
	thread = _commu->GetThread("BICThread");
	if (thread != nullptr){
		SetSFlag(TER_blSetupTerminalY);
		MSGSend(MESG_ANS_SetupTerminal,IUD("Y"),blRet,"BICThreadFun()::");
		
		thread->Execute(commu);
	
		MSGSend(MESG_ANS_CloseTerminal,IUD(""),blRet,"BICThreadFun()::");
	}
	else{
		MSGSend(MESG_ANS_SetupTerminal,IUD("N"),blRet,"BICThreadFun()::");
	}
	ClrSFlag(TER_blSetupTerminalY | TER_blSetupTerminalN | TER_blCloseTerminalY);
	ETLogThreadStop(BICThread);
	return G_TRUE;
}
//------------------------------------------------------------------------------------------//
bool32 MSG_Terminal::BICThreadOnStop(void* commu){
	COMMU_THREAD*	_commu = static_cast<COMMU_THREAD*>(commu);
	SYS_AThread*	thread;
	thread = _commu->GetThread("BICThread");
	if (thread != nullptr)
		thread->ThreadStop(commu);
	return G_TRUE;
}
//------------------------------------------------------------------------------------------//
bool32 MSG_Terminal::Send_REQ_SetupTerminal(CMD_ENV* env){
	bool32	blRet;
	ClrSFlag(TER_blCloseTerminalY);
	MSGSend2(env,MESG_REQ_SetupTerminal,IUD(""),TER_blSetupTerminalY,TER_blSetupTerminalN,MSG_CENTER::HandshakeTime,blRet,"");
	
	if ((blRet == 1) || (blRet == -3))
		return G_TRUE;
	return G_FALSE;
}
//------------------------------------------------------------------------------------------//
bool32 MSG_Terminal::Send_REQ_CloseTerminal(CMD_ENV* env){
	bool32	blRet;
	
	MSGSend2(env,MESG_REQ_CloseTerminal,IUD(""),TER_blCloseTerminalY,TER_blCloseTerminalY,MSG_CENTER::HandshakeTime,blRet,"");

	if ((blRet == 1) || (blRet == -3))
		return G_TRUE;
	return G_FALSE;
}
//------------------------------------------------------------------------------------------//
bool32 MSG_Terminal::CheckTerminalClosed(void){
	
	return(CheckSFlag(TER_blCloseTerminalY));
};
//------------------------------------------------------------------------------------------//
CMDID MSG_Terminal::MessageProcessing(CMD_ENV* env,const uint32& mID,const STDSTR& msg,void* commu){
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
			BICThread.ThreadStop(commu);
			BICThread.ThreadRun(commu);
			break;
		case MESG_ANS_SetupTerminal:
			SetSFlag((msg == "Y") ? TER_blSetupTerminalY : TER_blSetupTerminalN);
			break;
		case MESG_REQ_CloseTerminal:
			BICThread.ThreadStop(commu);
			break;
		case MESG_ANS_CloseTerminal:
			SetSFlag(TER_blCloseTerminalY);
			break;
		default:
			retCode = MESG_NONE;
			break;
	}
	return(retCode);
}
//------------------------------------------------------------------------------------------//
#endif /* MSG_Terminal_h */
