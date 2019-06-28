//
//  MSG_Center.cpp
//  SDT
//
//  Created by Leif Wen on 14/03/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "MSG_ID.h"
#include "MSG_Center.h"
#ifdef MSG_Center_h
//------------------------------------------------------------------------------------------//
//#define LOGPRINT_ENABLE
//#define LOGTHREAD_ENABLE
#include "SYS_Log.h"
#include "Commu_SSL.h"
//------------------------------------------------------------------------------------------//
MSG_BASE::MSG_BASE(void) : TNFP(){
	cgMSGID = MESG_NONE;
};
//------------------------------------------------------------------------------------------//
CMDID MSG_BASE::MessageProcessing(CMD_ENV* env,const uint32& mID,const STDSTR& msg,void* _team){
	CMDID	retCode;
	retCode = MESG_NONE;
	TREE_DownChain_Traversal_LINE_nolock(MSG_BASE,this,
		retCode = _opNode->MessageProcessing(env,mID,msg,_team);
		if (retCode != MESG_NONE)
			break;
	);
	return(retCode);
};
//------------------------------------------------------------------------------------------//
void MSG_BASE::Init(uint32 rxSize,uint32 txSize){
	TREE_DownChain_Traversal_LINE_nolock(MSG_BASE,this,_opNode->Init(rxSize,txSize););
};
//------------------------------------------------------------------------------------------//
void MSG_BASE::Reset(void* _team){
	TREE_DownChain_Traversal_LINE_nolock(MSG_BASE,this,_opNode->Reset(_team););
};
//------------------------------------------------------------------------------------------//
void MSG_BASE::Close(void* _team){
	TREE_DownChain_Traversal_LINE_nolock(MSG_BASE,this,_opNode->Close(_team););
};
//------------------------------------------------------------------------------------------//
CMDID MSG_BASE::GetMSGID(void){
	return(cgMSGID);
};
//------------------------------------------------------------------------------------------//









//------------------------------------------------------------------------------------------//
#ifdef Commu_SSL_h
static inline COMMU_TEAM* _GetTeam(void* _team){
	return(static_cast<COMMU_TEAM*>(_team));
};
static inline MEM_SSL* _GetMEM(void* _team){
	return(static_cast<MEM_SSL*>(static_cast<COMMU_TEAM*>(_team)->mem));
};
#endif
//------------------------------------------------------------------------------------------//
MSG_CENTER::MSG_CENTER(void) : MSG_BASE(){
	cgMSGID = MESG_NONE;
	
	cgSSLDev = nullptr;
	cgENV = nullptr;
	cgTeam = nullptr;
	t1CtrlRxThread.ThreadInit(this,&MSG_CENTER::T1CtrlRxThreadFun	,"T1CRx");

	TNFP::SetSelfName		("SSL");
	cgT1CtrlCH.SetSelfName	("T1C");
	SetUpName("");
};
//------------------------------------------------------------------------------------------//
MSG_CENTER::~MSG_CENTER(void){
	t1CtrlRxThread.RemoveSelf();
};
//------------------------------------------------------------------------------------------//
void MSG_CENTER::Init(void* _team,CMD_ENV* env){
#ifdef Commu_SSL_h
	_GetTeam(_team)->commu->ThreadAccept(&t1CtrlRxThread);
	cgTeam = _team;
	cgENV = env;
#endif
};
//------------------------------------------------------------------------------------------//
void MSG_CENTER::Init(const CSSL_DEV* sslDev,uint32 maxPackage,uint32 maxPages){
	cgSSLDev = (CSSL_DEV*)sslDev;
	cgT1CtrlCH.Init(0, &sslDev->cgT0, maxPackage, maxPages);
};
//------------------------------------------------------------------------------------------//
void MSG_CENTER::Init(uint32 rxSize,uint32 txSize){
	MSG_BASE::Init(rxSize,txSize);
};
//------------------------------------------------------------------------------------------//
CSSL_DEV* MSG_CENTER::GetSSLDev(void){
	return(cgSSLDev);
};
//------------------------------------------------------------------------------------------//
void MSG_CENTER::SetDelayPar(uint32 maxDlyMS,const uint32 maxTimes){
	cgT1CtrlCH.SetDelayPar(maxDlyMS,maxTimes);
};
//------------------------------------------------------------------------------------------//
void MSG_CENTER::SetSelfName(const STDSTR& strName){
	selfName = strName;
	cgT1CtrlCH.SetUpName(GetFullName(this));
	t1CtrlRxThread.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
void MSG_CENTER::SetUpName(const STDSTR& strName){
	fatherName = strName;
	cgT1CtrlCH.SetUpName(GetFullName(this));
	t1CtrlRxThread.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
void MSG_CENTER::Reset(void* _team){
	cgT1CtrlCH.Reset();

	ClrSFlag(HS_blHandshakeY | HS_blREQClose);
#ifdef Commu_SSL_h
	_GetMEM(cgTeam)->ResetT0T1(_team);
#endif
	MSG_BASE::Reset(_team);
};
//------------------------------------------------------------------------------------------//
void MSG_CENTER::Close(void* _team){
	cgT1CtrlCH.Close();
	MSG_BASE::Close(_team);
};
//------------------------------------------------------------------------------------------//
void MSG_CENTER::Empty(void){
	cgT1CtrlCH.Empty();
#ifdef Commu_SSL_h
	_GetMEM(cgTeam)->EmptyArray();
#endif
};
//------------------------------------------------------------------------------------------//
void MSG_CENTER::SetConfig(uint32 cfg,const STDSTR& sKey){
#ifdef Commu_SSL_h
	_GetMEM(cgTeam)->SetConfig(cfg,sKey);
#endif
};
//------------------------------------------------------------------------------------------//
void MSG_CENTER::SetKey(const STDSTR& sKey){
#ifdef Commu_SSL_h
	_GetMEM(cgTeam)->SetKey(sKey);
#endif
};
//------------------------------------------------------------------------------------------//
void MSG_CENTER::Register(MSG_BASE* msg){
	AppendDownNode(msg);
	((MSG_NODE*)msg)->Join(this);
};
//------------------------------------------------------------------------------------------//
void MSG_CENTER::SetblREQClose(void){
	SetSFlag(HS_blREQClose);
};
//------------------------------------------------------------------------------------------//
void MSG_CENTER::SetblREQHS(void){
	SetSFlag(HS_blHandshakeY);
};
//------------------------------------------------------------------------------------------//
bool32 MSG_CENTER::ChkblREQClose(void){
	return(CheckSFlag(HS_blREQClose));
};
//------------------------------------------------------------------------------------------//
bool32 MSG_CENTER::ChkblREQHS(void){
	return(CheckSFlag(HS_blHandshakeY));
};
//------------------------------------------------------------------------------------------//
static bool32 _IsClosed(void* _team){
#ifdef Commu_SSL_h
	return(CommuIsClosed(_team));
#else
	return G_TRUE;
#endif
};
//------------------------------------------------------------------------------------------//
bool32 MSG_CENTER::T1CtrlRxThreadFun(void* _team){
	ETLogThreadStart(t1CtrlRxThread);
	
	STDSTR	strMesg;
	uint32	mID = MESG_INI_Handshake;
#ifdef LOGTHREAD_ENABLE
	IOS		ios;
#endif
	
	while(t1CtrlRxThread.IsTerminated() == G_FALSE){
#ifndef LOGTHREAD_ENABLE
		cgT1CtrlCH.Read(ACF(_IsClosed,_team),nullptr,&mID,_EMPTY(&strMesg));
#else
		if (cgT1CtrlCH.Read(ACF(_IsClosed,_team),IOS_clr(&ios),&mID,_EMPTY(&strMesg)) > 0){
			ELog(ELogThread(t1CtrlRxThread,"Received: " << "MLen=" << strMesg.length() << "/" << ios.total_out) << "," << _GetMesgText(mID));
		}
#endif
		MessageProcessing(cgENV,mID,strMesg,_team);
		SYS_SleepMS(2);
	};
	ETLogThreadStop(t1CtrlRxThread);
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
bool32 MSG_CENTER::Send(IOS* _ios,uint32 mID,const UVIn& _in){
	return(cgT1CtrlCH.Send(ACF(_IsClosed,cgTeam),_ios,mID,_in) >= IOS_OK);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
MSG_NODE::MSG_NODE(void) : MSG_BASE(){
	cgMSGID = MESG_NONE;
	cgMsgCenter = nullptr;
	
	TNFP::SetSelfName("MSG_NODE");
	SetUpName("");
};
//------------------------------------------------------------------------------------------//
void MSG_NODE::SetSelfName(const STDSTR& strName){
	selfName = strName;
	TREE_DownChain_Traversal_LINE_nolock(MSG_NODE,this,_opNode->SetUpName(GetFullName(this)););
};
//------------------------------------------------------------------------------------------//
void MSG_NODE::SetUpName(const STDSTR& strName){
	fatherName = strName;
	TREE_DownChain_Traversal_LINE_nolock(MSG_NODE,this,_opNode->SetUpName(GetFullName(this)););
};
//------------------------------------------------------------------------------------------//
void MSG_NODE::Join(MSG_CENTER* msgCenter){
	if (cgMsgCenter == nullptr){
		cgMsgCenter = msgCenter;
		cgMsgCenter->Register(this);
		TREE_DownChain_Traversal_LINE_nolock(MSG_NODE,this,_opNode->cgMsgCenter = msgCenter;);
	}
};
//------------------------------------------------------------------------------------------//
void MSG_NODE::SetConfig(uint32 cfg,const STDSTR& sKey){
	cgMsgCenter->SetConfig(cfg, sKey);
};
//------------------------------------------------------------------------------------------//
void MSG_NODE::SetKey(const STDSTR& sKey){
	cgMsgCenter->SetKey(sKey);
};
//------------------------------------------------------------------------------------------//
bool32 MSG_NODE::Send(uint32 mID,const UVIn& _in){
	
	return(cgMsgCenter->Send(nullptr,mID,_in));
};
//------------------------------------------------------------------------------------------//
bool32 MSG_NODE::Send(CMD_ENV* env,uint32 mID,const UVIn& _in,const uint64& flagY,const uint64& flagN,const uint32& waitTimeS){
	SYS_TIME_S		timedly;
	bool32			err;
	
	if (CMD_NODE::IsExit(env)){
		err = -1;
	}
	else{
		SYS_Delay_SetTS(&timedly, waitTimeS << 10);
		ClrSFlag(flagY | flagN);
		
		err = Send(mID,_in);
		if (err == G_FALSE){
			err = -3;
		}
		else{
			err = Check(env,flagY,flagN,timedly);
		}
	}
	return(err);
};
//------------------------------------------------------------------------------------------//
bool32 MSG_NODE::Check(CMD_ENV* env,const uint64& flagY,const uint64& flagN,SYS_TIME_S& timedly){
	
	while((CheckSFlag(flagY | flagN) == G_FALSE) && (CMD_NODE::IsExit(env) == G_FALSE) && (SYS_Delay_IsTimeout(&timedly) == G_FALSE))
		SYS_SleepMS(2);
	if (CheckSFlag(flagY) > 0){
		return G_TRUE;
	}
	else if (CheckSFlag(flagN) > 0){
		return G_FALSE;
	}
	else if (CMD_NODE::IsExit(env)){
		return -1;
	}
	return -2;
};
//------------------------------------------------------------------------------------------//
#endif /* MSG_Center_h */
