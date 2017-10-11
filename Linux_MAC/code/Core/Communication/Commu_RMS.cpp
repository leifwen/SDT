/*
 * Copyright (c) 2012-2022
 * All rights reserved.
 *
 * File Name: Commu_RMS.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2016.01.06
 
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "Commu_RMS.h"
#include "Comm_Convert.h"
#include "SYS_Time.h"
#define LOGPRINT_ENABLE
#define LOGPRINT_ENABLE2
#include "Comm_Log.h"
//------------------------------------------------------------------------------------------//
#ifdef Commu_RMSH
//------------------------------------------------------------------------------------------//
TREE_NODE_FRAME_POOL *POOL_SSLSOCKET::CreateNode(void){
	return(SetSubNodeSelfName(new RSSLSocket(cgBufMaxSize,GetLogSystem())));
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
TREE_NODE_FRAME_POOL *POOL_BSOCKET::CreateNode(void){
	return(SetSubNodeSelfName(new BSOCKET_C(cgBufMaxSize,GetLogSystem())));
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 CPPOOL::GetCP(RSSLSocket **rsslsocket,BSOCKET_C **bsocket){
	*rsslsocket = (RSSLSocket*)cgPoolS.GetNewSon();
	*bsocket = (BSOCKET_C*)cgPoolB.GetNewSon();
	if (*rsslsocket == nullptr){
		cgPoolB.ChildClose(*bsocket);
		bsocket = nullptr;
	}
	else if (bsocket == nullptr){
		cgPoolS.ChildClose(*rsslsocket);
		rsslsocket = nullptr;
	}
	else{
		(*rsslsocket)->LinkCoupleNode(*bsocket);
		return 1;
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
void CPPOOL::ChildClose(RSSLSocket *rsslsocket,BSOCKET_C *bsocket){
	cgPoolS.ChildClose(rsslsocket);
	cgPoolB.ChildClose(bsocket);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
void BSOCKET_C::DoClose(void){
	BSOCKET	*coupleSocket;
	ELog(this << "BSOCKET_C::DoClose()");
	coupleSocket = static_cast<BSOCKET*>(UnlinkCoupleNode());
	if (coupleSocket != nullptr){
		ELog(this << "BSOCKET_C::DoClose()::Call " << coupleSocket << "::CloseD()");
		coupleSocket->CloseD();
	}
	BSOCKET::DoClose();
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 MAPPINGServer::Open(int32 localPort,const STDSTR &mapIP,int32 mapPort,CSType tCSType){
	int32 ret;
	ret = 0;
	if (InDoing_try() != 0){
		cgMapIP = mapIP;
		cgMapPort = mapPort;
		ret = BSOCKETSERVER::OpenD("",localPort,tCSType,0,G_LOCK_OFF);
		InDoing_clr();
	}
	return(ret);
};
//------------------------------------------------------------------------------------------//
int32 MAPPINGServer::OnOpenTCPSocket(ASOCKET *newSocket){
	ELog(this << "MAPPINGServer::OnOpenTCPSocket():: " << newSocket);
	return(((RSSLSocket*)GetFDB())->SendRequestSetupDataChannel(newSocket,cgMapIP,cgMapPort,(GetCSType() == CSType_TCPS)?CSType_TCP:CSType_UDP));
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//











//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
STDSTR RSSLSocket::GetMesgText(uint32 mID){
	STDSTR retStr = "";
#ifdef LOGPRINT
	retStr = COMMU_DBUF_SSL::GetMesgText(mID);
	if (retStr.length() > 0)
		return(retStr);
	
	retStr = "(" + Str_ToString(mID)+ ")";
	switch(mID){
		case MESG_REQ_SetupMServer		:retStr += "MESG_REQ_SetupMServer ";break;
		case MESG_ANS_SetupMServer		:retStr += "MESG_ANS_SetupMServer ";break;
		case MESG_REQ_CloseMServer		:retStr += "MESG_REQ_CloseMServer ";break;
		case MESG_ANS_CloseMServer		:retStr += "MESG_ANS_CloseMServer ";break;
		case MESG_REQ_ListMServer		:retStr += "MESG_REQ_ListMServer ";break;
		case MESG_ANS_ListMServer		:retStr += "MESG_ANS_ListMServer ";break;
		case MESG_REQ_SetupDataChannel	:retStr += "MESG_REQ_SetupDataChannel ";break;
		case MESG_ANS_SetupDataChannel	:retStr += "MESG_ANS_SetupDataChannel ";break;
		case MESG_REQ_Link				:retStr += "MESG_REQ_Link ";break;
		case MESG_ANS_Link				:retStr += "MESG_ANS_Link ";break;
		default							:retStr = "";break;
	}
#endif
	return(retStr);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
void RSSLSocket::DoClose(void){
	ELog(this << "RSSLSocket::DoClose()");
	BSOCKET_C::DoClose();
	cgCPPool.ChildCloseAll();
	DestroyAll();
	cgCPPool.cgPoolS.DestroyAll();
	cgCPPool.cgPoolB.DestroyAll();
	ClrSFlag(blSetupMServerY | blSetupMServerN | blOffMServerY | blOffMServerN | blListMServerY | blListMServerN | blLinkY | blLinkN);
}
//------------------------------------------------------------------------------------------//
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
int32 RSSLSocket::MessageProcessing(const uint32 &mID,const STDSTR &strMesg){
	STDSTR			strMesgT,strContent,*strP;
	int32			blDo,blRet,blRet2;
	MAPPINGServer	*mServer = nullptr;
	RSSLSocket		*rsslsocket = nullptr;
	BSOCKET_C		*bsocket = nullptr;
	
	if (BSOCKET_C::MessageProcessing(mID,strMesg) > 0)
		return 1;

	if (mID == MESG_NONE)
		return 0;
	
	blDo = 1;
	switch (mID){
		case MESG_REQ_SetupMServer:
			CHRecInMP(mID);
			blRet = OpenMappingServer(mServer,strMesg);
			CHSendMInMP(MESG_ANS_SetupMServer,((blRet > 0) ? "Y" : "N"),blRet2);
			if (blRet2 == 0){
				ChildClose(mServer);
				E2LogInMP("Call SelfClose()");
				SetblNoSendCloseMesg();
				SelfClose();
			}
			break;
		case MESG_ANS_SetupMServer:
			CHRecMInMP(mID,strMesg);
			SetSFlag((strMesg == "Y")?blSetupMServerY:blSetupMServerN);
			break;
		case MESG_REQ_CloseMServer:
			CHRecInMP(mID);
			blRet = CloseMappingServer(strMesg);
			CHSendMInMP(MESG_ANS_CloseMServer,((blRet > 0) ? "Y" : "N"),blRet2);
			break;
		case MESG_ANS_CloseMServer:
			CHRecMInMP(mID,strMesg);
			SetSFlag((strMesg == "Y")?blOffMServerY:blOffMServerN);
			break;
		case MESG_REQ_ListMServer:
			CHRecInMP(mID);
			ListMappingServer(&strContent);
			strMesgT = strMesg;
			strMesgT += ',';
			strMesgT += strContent;
			CHSendInMP(MESG_ANS_ListMServer,strMesgT,blRet);
			break;
		case MESG_ANS_ListMServer:
			CHRecInMP(mID);
			strMesgT = strMesg;
			strP = (STDSTR*)Str_HexToDec(Str_ReadSubItem(&strMesgT, ","));
			*strP = strMesgT;
			SetSFlag(blListMServerY);
			break;
		case MESG_REQ_SetupDataChannel:
			CHRecInMP(mID);
			blRet = OpenCoupleSocket(&rsslsocket,&bsocket,strMesg);
			CHSendMInMP(MESG_ANS_SetupDataChannel,((blRet > 0) ? "Y" : "N"),blRet2);
			if (blRet2 == 0){
				E2LogInMP("Setup Data Channel fail");
				cgCPPool.ChildClose(rsslsocket,bsocket);
				E2LogInMP("Call SelfClose()");
				SetblNoSendCloseMesg();
				SelfClose();
			}
			if (blRet > 0){
				E2LogInMP("Setup Data Channel successful");
				E2LogInMP("Create pre-couple");
				cgCPPool.GetCP(&rsslsocket, &bsocket);
				cgCPPool.ChildClose(rsslsocket,bsocket);
				E2LogInMP("pre-couple" << rsslsocket << bsocket);
				break;
			}
			E2LogInMP("Setup Data Channel fail");
			break;
		case MESG_ANS_SetupDataChannel:
			CHRecMInMP(mID,strMesg);
			SetSFlag((strMesg == "Y")?blLinkY:blLinkN);
			E2LogInMP("Setup Data Channel " << ((CheckSFlag(blLinkY) > 0) ? "successful" : "fail"));
			break;
		case MESG_REQ_Link:
			CHRecInMP(mID);
			bsocket = (BSOCKET_C*)Str_HexToDec(strMesg);
			blRet = 0;
			try{
				LinkCoupleNode(bsocket);
				ELog(this << "MessageProcessing()::Link to " << bsocket);
				blRet = 1;
			}
			catch(...){blRet = 0;}
			CHSendMInMP(MESG_ANS_Link,((blRet > 0) ? "Y" : "N"),blRet2);
			break;
		case MESG_ANS_Link:
			CHRecMInMP(mID,strMesg);
			SetSFlag((strMesg == "Y")?blLinkY:blLinkN);
			break;
		default:
			blDo = 0;
			break;
	}
	return(blDo);
}
//------------------------------------------------------------------------------------------//
int32 RSSLSocket::SendRequestSetupMServer(int32 remotePort,const STDSTR &mapIP,int32 mapPort,CSType tCSType){
	STDSTR	strMesg;
	
	strMesg = Str_DecToHex((uint64)remotePort);
	strMesg += ',';
	strMesg += Str_DecToHex((uint64)tCSType);
	strMesg += ',';
	strMesg += Str_DecToHex((uint64)mapPort);
	strMesg += ',';
	strMesg += mapIP;
	return((SendCHMesg(MESG_REQ_SetupMServer,strMesg,blSetupMServerY,blSetupMServerN,CSSL_FR_T2::HandshakeTime) > 0)?1:0);
}
//------------------------------------------------------------------------------------------//
int32 RSSLSocket::SendRequestOffMServer(int32 remotePort){
	STDSTR		strMesg;
	strMesg = Str_DecToHex((uint64)remotePort);//0-->close all
	return((SendCHMesg(MESG_REQ_CloseMServer,strMesg,blOffMServerY,blOffMServerN,CSSL_FR_T2::HandshakeTime) > 0)?1:0);
}
//------------------------------------------------------------------------------------------//
int32 RSSLSocket::SendRequestListMServer(STDSTR *retStr){
	STDSTR		strMesg;
	strMesg = Str_DecToHex((uint64)retStr);
	return((SendCHMesg(MESG_REQ_ListMServer,strMesg,blListMServerY,blListMServerN,CSSL_FR_T2::HandshakeTime) > 0)?1:0);
}
//------------------------------------------------------------------------------------------//
int32 RSSLSocket::SendRequestSetupDataChannel(ASOCKET *mSocket,const STDSTR &mapIP,int32 mapPort,CSType tCSType){
	STDSTR		strMesg;
	
	strMesg = Str_DecToHex((uint64)tCSType);
	strMesg += ',';
	strMesg += Str_DecToHex((uint64)mapPort);
	strMesg += ',';
	strMesg += mapIP;
	strMesg += ',';
	strMesg += Str_DecToHex((uint64)mSocket);
	return((SendCHMesg(MESG_REQ_SetupDataChannel,strMesg,blLinkY,blLinkN,CSSL_FR_T2::HandshakeTime) > 0)?1:0);
}
//------------------------------------------------------------------------------------------//
int32 RSSLSocket::SendRequestLink(const STDSTR &strMesg){
	return((SendCHMesg(MESG_REQ_Link,strMesg,blLinkY,blLinkN,CSSL_FR_T2::HandshakeTime) > 0)?1:0);
}
//------------------------------------------------------------------------------------------//
int32 RSSLSocket::OpenMappingServer(MAPPINGServer *retMserver,const STDSTR &strMesg){
	STDSTR		strMapIP;
	int32 		remotePort,mapPort,mapCSType;
	
	strMapIP = strMesg;
	remotePort = (int32)Str_HexToDec(Str_ReadSubItem(&strMapIP, ","));
	mapCSType = (int32)Str_HexToDec(Str_ReadSubItem(&strMapIP, ","));
	mapPort = (int32)Str_HexToDec(Str_ReadSubItem(&strMapIP, ","));
	
	retMserver = (MAPPINGServer*)GetNewSon();
	if ((retMserver != nullptr) && (retMserver->Open(remotePort,strMapIP,mapPort,(CSType)mapCSType) != 0))
		return 1;
	ChildClose(retMserver);
	retMserver = nullptr;
	return 0;
}
//------------------------------------------------------------------------------------------//
int32 RSSLSocket::CloseMappingServer(const STDSTR &strMesg){
	int32	remotePort;
	int32	ret;
	ret = 0;
	
	remotePort = (int32)Str_HexToDec(strMesg);
	if (remotePort == 0){
		ELog(this << "CloseMappingServer()::Close all MServer");
		ChildCloseAll();
		ret = 1;
	}
	else{
		TREE_LChildRChain_Traversal_LINE(MAPPINGServer,this,
			if (operateNode_t->GetBufPar() == remotePort){
				ELog(this << "CloseMappingServer()::Close MServer(" << remotePort << ")");
				ChildClose(operateNode_t);
				ret = 1;
				break;
			}
		);
	}
	return(ret);
}
//------------------------------------------------------------------------------------------//
int32 RSSLSocket::ListMappingServer(STDSTR *strMesg){
	ELog(this << "ListMappingServer()::Prepare MServer list string");
	*strMesg = "";
	TREE_LChildRChain_Traversal_LINE(MAPPINGServer,this,
		*strMesg += "MServer(";
		*strMesg += Str_ToString(operateNode_t->GetBufPar());
		*strMesg += ") mapping to ";
		*strMesg += operateNode_t->GetMapIP();
		*strMesg += ":";
		*strMesg += Str_ToString(operateNode_t->GetMapPort());
		if (operateNode_t->GetCSType() == CSType_TCPS){
			*strMesg += ",TCP";
		}
		else{
			*strMesg += ",UDP";
		}
		*strMesg += "\n";
	);
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 RSSLSocket::OpenCoupleSocket(RSSLSocket **rsslsocket,BSOCKET_C **bsocket,STDSTR strMesg){
	STDSTR		strMapIP;
	int32 		mapPort,mapCSType;
	
	mapCSType = (int32)Str_HexToDec(Str_ReadSubItem(&strMesg, ","));
	mapPort = (int32)Str_HexToDec(Str_ReadSubItem(&strMesg, ","));
	strMapIP = Str_ReadSubItem(&strMesg, ",");
	
	do{
		if (cgCPPool.GetCP(rsslsocket, bsocket) == 0){
			ELog(this << "OpenCoupleSocket()::GetCP() fail");
			break;
		}
		ELog(this << "OpenCoupleSocket()::Created " << *rsslsocket << *bsocket);
		
		if ((*rsslsocket)->OpenD(GetBufName(), GetBufPar(), GetCSType(), 0) == 0){
			ELog(this << "OpenCoupleSocket()::Open RSSLSocket fail");
			break;
		}
		ELog(this << "OpenCoupleSocket()::Open RSSLSocket success");
		if ((*rsslsocket)->SendRequestLink(strMesg) == 0){
			ELog(this << "OpenCoupleSocket()::" << *rsslsocket << "Link to MappingServer fail");
			break;
		}
		ELog(this << "OpenCoupleSocket()::" << *rsslsocket << "Link to MappingServer success");
		if ((*bsocket)->OpenD(strMapIP, mapPort, (CSType)mapCSType, 0) == 0){
			ELog(this << "OpenCoupleSocket()::Open BSocket fail");
			break;
		}
		ELog(this << "OpenCoupleSocket()::Open BSocket success");
		return 1;
	}while(0);
	cgCPPool.ChildClose(*rsslsocket,*bsocket);
	return 0;
}
//------------------------------------------------------------------------------------------//
#endif



