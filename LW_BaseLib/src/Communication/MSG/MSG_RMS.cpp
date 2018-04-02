//
//  MSG_RMS.cpp
//  SDT
//
//  Created by Leif Wen on 15/03/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
#include "MSG_ID.h"
#include "MSG_RMS.h"
#ifdef MSG_RMS_h
//------------------------------------------------------------------------------------------//
//#define LOGPRINT_ENABLE
//#define LOG0_ENABLE
//#define LOGA_ENABLE
//#define LOGMSG_ENABLE

#ifdef LOG0_ENABLE
#define ELog0(x) ELog(x);
#else
#define ELog0(x);
#endif

#ifdef LOGA_ENABLE
#define ELogA(x) ELog(x);
#else
#define ELogA(x);
#endif

#ifdef LOGMSG_ENABLE
#define ELogMSG(x) ELog(x);
#else
#define ELogMSG(x);
#endif

#include "SYS_Log.h"
//------------------------------------------------------------------------------------------//
MAPBSERVER::MAPBSERVER(uint32 size,const ODEV_SYSTEM* logSys) : BSOCKETSERVER(size,logSys){
	cgMapPar.type = 0;
	cgMapPar.name = "";
	cgMapPar.port = 0;
	cgMapPar.cfg = 0;
	TNFP::SetSelfName("MServer");
	SetSelfName(selfName);
	SetFatherName("");
	ELogA(this);
};
//------------------------------------------------------------------------------------------//
const OPEN_PAR& MAPBSERVER::GetMapPar(void){
	
	return(cgMapPar);
};
//------------------------------------------------------------------------------------------//
bool32 MAPBSERVER::Open(CMD_ENV* env,MSG_RMS* msg,uint32 localPort,const OPEN_PAR& mapPar){
	OPEN_PAR	opar;
	bool32 		ret;
	
	opar.type = (mapPar.type == OPEN_UDP || mapPar.type == OPEN_UDPS) ? OPEN_UDPS : OPEN_TCPS;
	opar.name = "";
	opar.port = localPort;
	opar.cfg = 0;
	
	ret = G_FALSE;
	if (InDoing_try()){
		SetOpenPar(&cgMapPar, mapPar);
		cgENV = env;
		cgMsgRMS = msg;
		cgMapPar.type = (mapPar.type == OPEN_UDP || mapPar.type == OPEN_UDPS) ? OPEN_UDP : OPEN_TCP;
		ret = BSOCKETSERVER::Open(opar,G_LOCK_OFF);
		InDoing_clr();
	}
	return(ret);
};
//------------------------------------------------------------------------------------------//
bool32 MAPBSERVER::OnOpenTCPSocket(COMMU_FRAME* newSocket){
	ELog0(this << "MAPPINGServer::OnOpenTCPSocket():: " << newSocket);
	if (newSocket != nullptr)
		newSocket->EnableCloseCouple();
	return(cgMsgRMS->Send_REQ_SetupDataChannel(cgENV,newSocket,cgMapPar));
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
MSG_RMS::MSG_RMS(uint32 size,void* logSys) : MSG_NODE(size,(void*)logSys)
,cgMSPool(size,(ODEV_SYSTEM*)logSys)
,cgCPPool(size,(ODEV_SYSTEM*)logSys)
{
	cgMSGID = MESG_ID_RMS;
	selfName = "MSG_RMS";
	cgMSPool.SetSelfName("MSPool");
	cgCPPool.SetSelfName("CPPool");
	SetFatherName("");
}
//------------------------------------------------------------------------------------------//
void MSG_RMS::Close(void* commu){
	cgMSPool.Close();
	cgCPPool.Close();
};
//------------------------------------------------------------------------------------------//
void MSG_RMS::SetSelfName(const STDSTR& strName){
	selfName = strName;
	cgMSPool.SetFatherName(GetFullName(this));
	cgCPPool.SetFatherName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
void MSG_RMS::SetFatherName(const STDSTR& strName){
	fatherName = strName;
	cgMSPool.SetFatherName(GetFullName(this));
	cgCPPool.SetFatherName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
void MSG_RMS::Reset(void* commu){
	ClrSFlag(RMS_blSetupMServerY | RMS_blSetupMServerN
			 | RMS_blCloseMServerY |RMS_blCloseMServerN
			 | RMS_blListMServerY | RMS_blListMServerN
			 | RMS_blLinkY | RMS_blLinkN);
};
//------------------------------------------------------------------------------------------//
bool32 MSG_RMS::OpenMServer(CMD_ENV* env,MAPBSERVER** mServer,const STDSTR& strMesg){
	OPEN_PAR	opar;
	uint32 		remotePort;
	
	opar.name = strMesg;
	remotePort = (uint32)Str_HexToDec(Str_ReadSubItem(&opar.name, ","));
	opar.type = (uint32)Str_HexToDec(Str_ReadSubItem(&opar.name, ","));
	opar.port = (uint32)Str_HexToDec(Str_ReadSubItem(&opar.name, ","));
	opar.cfg = 0;
	
	*mServer = cgMSPool.GetNewChild();
	if ((*mServer != nullptr) && (*mServer)->Open(env,this,remotePort,opar))
		return G_TRUE;
	cgMSPool.CloseChild(*mServer);
	return G_FALSE;
}
//------------------------------------------------------------------------------------------//
bool32 MSG_RMS::CloseMServer(const STDSTR& strMesg){
	uint32	remotePort;
	bool32	ret;
	ret = G_FALSE;
	
	remotePort = (int32)Str_HexToDec(strMesg);
	if (remotePort == 0){
		ELog0(this << "CloseMappingServer()::Close all MServer");
		cgMSPool.CloseAllChild();
		ret = G_TRUE;
	}
	else{
		TREE_LChildRChain_Traversal_LINE(MAPBSERVER,this,
			if (_opNode->GetOpenPar().port == remotePort){
				ELog0(this << "CloseMappingServer()::Close MServer(" << remotePort << ")");
				cgMSPool.CloseChild(_opNode);
				ret = G_TRUE;
				break;
			}
		);
	}
	return(ret);
}
//------------------------------------------------------------------------------------------//
bool32 MSG_RMS::ListMappingServer(STDSTR* strMesg){
	ELog0(this << "ListMappingServer()::Prepare MServer list string");
	
	TREE_LChildRChain_Traversal_LINE(MAPBSERVER,(&cgMSPool),
		*strMesg += " MServer(";
		*strMesg += Str_ToStr(_opNode->GetOpenPar().port);
		*strMesg += ") mapping to ";
		*strMesg += _opNode->GetMapPar().name;
		*strMesg += ":";
		*strMesg += Str_ToStr(_opNode->GetMapPar().port);
		if (_opNode->GetOpenPar().type == OPEN_TCPS){
			*strMesg += ",TCP";
		}
		else{
			*strMesg += ",UDP";
		}
		*strMesg += "\n";
	);
	return G_TRUE;
}
//------------------------------------------------------------------------------------------//
bool32 MSG_RMS::OpenCoupleSocket(CMD_ENV* env,RMSBSOCKET** rmssocket,BSOCKET** bsocket,STDSTR strMesg,const OPEN_PAR& par){
	OPEN_PAR	opar;
	
	opar.type = (uint32)Str_HexToDec(Str_ReadSubItem(&strMesg, ","));
	opar.port = (uint32)Str_HexToDec(Str_ReadSubItem(&strMesg, ","));
	opar.name = Str_ReadSubItem(&strMesg, ",");
	opar.cfg = 0;
	
	do{
		if (cgCPPool.GetCP(rmssocket, bsocket) == G_FALSE){
			ELog0(this << "OpenCoupleSocket()::GetCP() fail");
			break;
		}
		ELog0(this << "OpenCoupleSocket()::Created " << *rmssocket << *bsocket);
		
		if ((*rmssocket)->Open(par) == G_FALSE){
			ELog0(this << "OpenCoupleSocket()::Open RSSLSocket fail");
			break;
		}
		ELog0(this << "OpenCoupleSocket()::Open RSSLSocket success");
		if ((*rmssocket)->cgMsg.Send_REG_Link(env,strMesg) == G_FALSE){
			ELog(this << "OpenCoupleSocket()::" << *rmssocket << "Link to MappingServer fail");
			break;
		}
		ELog0(this << "OpenCoupleSocket()::" << *rmssocket << "Link to MappingServer success");
		if ((*bsocket != nullptr) && ((*bsocket)->Open(opar) == G_FALSE)){
			ELog(this << "OpenCoupleSocket()::Open BSocket fail");
			break;
		}
		ELog0(this << "OpenCoupleSocket()::Open BSocket success");
		return G_TRUE;
	}while(0);
	cgCPPool.CloseChild(*rmssocket,*bsocket);
	return G_FALSE;
}
//------------------------------------------------------------------------------------------//
bool32 MSG_RMS::Send_REQ_SetupMServer(CMD_ENV* env,uint32 remotePort,const OPEN_PAR& mapPar){
	bool32	blRet;
	STDSTR	strMesg;
	
	strMesg = Str_DecToHex((uint64)remotePort);
	strMesg += ',';
	strMesg += Str_DecToHex((uint64)mapPar.type);
	strMesg += ',';
	strMesg += Str_DecToHex((uint64)mapPar.port);
	strMesg += ',';
	strMesg += mapPar.name;
	
	MSGSend2(env,MESG_REQ_SetupMServer,strMesg,RMS_blSetupMServerY,RMS_blSetupMServerN,MSG_CENTER::HandshakeTime,blRet,"");
	return(blRet);
}
//------------------------------------------------------------------------------------------//
bool32 MSG_RMS::Send_REQ_CloseMServer(CMD_ENV* env,uint32 remotePort){
	bool32	blRet;
	STDSTR	strMesg;
	strMesg = Str_DecToHex((uint64)remotePort);//0-->close all
	
	MSGSend2(env,MESG_REQ_CloseMServer,strMesg,RMS_blCloseMServerY,RMS_blCloseMServerN,MSG_CENTER::HandshakeTime,blRet,"");
	return(blRet);
}
//------------------------------------------------------------------------------------------//
bool32 MSG_RMS::Send_REG_ListMServer(CMD_ENV* env,STDSTR* retStr){
	bool32	blRet;
	STDSTR	strMesg;
	strMesg = Str_DecToHex((uint64)retStr);
	
	MSGSend2(env,MESG_REQ_ListMServer,strMesg,RMS_blListMServerY,RMS_blListMServerN,MSG_CENTER::HandshakeTime,blRet,"");
	return(blRet);
}
//------------------------------------------------------------------------------------------//
bool32 MSG_RMS::Send_REQ_SetupDataChannel(CMD_ENV* env,COMMU_FRAME* mSocket,const OPEN_PAR& mapPar){
	bool32	blRet;
	STDSTR	strMesg;
	
	strMesg = Str_DecToHex((uint64)mapPar.type);
	strMesg += ',';
	strMesg += Str_DecToHex((uint64)mapPar.port);
	strMesg += ',';
	strMesg += mapPar.name;
	strMesg += ',';
	strMesg += Str_DecToHex((uint64)mSocket);
	
	MSGSend2(env,MESG_REQ_SetupDataChannel,strMesg,RMS_blLinkY,RMS_blLinkN,MSG_CENTER::HandshakeTime,blRet,"");
	return(blRet);
}
//------------------------------------------------------------------------------------------//
bool32 MSG_RMS::Send_REG_Link(CMD_ENV* env,const STDSTR& strMesg){
	bool32	blRet;
	
	MSGSend2(env,MESG_REQ_Link,strMesg,RMS_blLinkY,RMS_blLinkN,MSG_CENTER::HandshakeTime,blRet,"");
	return(blRet);
}
//------------------------------------------------------------------------------------------//
CMDID MSG_RMS::MessageProcessing(CMD_ENV* env,const uint32& mID,const STDSTR& msg,void* commu){
	COMMU_THREAD*	_commu = static_cast<COMMU_THREAD*>(commu);
	STDSTR		strMesg,*strP;
	bool32		blRet,blRet2;
	MAPBSERVER	*mServer = nullptr;
	RMSBSOCKET	*rmssocket = nullptr;
	BSOCKET		*bsocket = nullptr;
	CMDID		retCode = mID;
	
#ifdef LOGMSG_ENABLE
	switch (mID){
		case MESG_REQ_SetupMServer:;
		case MESG_REQ_CloseMServer:;
		case MESG_REQ_ListMServer:;
		case MESG_ANS_ListMServer:;
		case MESG_REQ_SetupDataChannel:;
		case MESG_REQ_Link:;
			ELogMPRecMsg(mID);
			break;
		case MESG_ANS_SetupMServer:;
		case MESG_ANS_CloseMServer:;
		case MESG_ANS_SetupDataChannel:;
		case MESG_ANS_Link:;
			ELogMPRecMsg2(mID,msg);
		default:
			break;
	}
#endif
	blRet = G_TRUE;
	blRet2 = G_TRUE;
	switch (mID){
		case MESG_REQ_SetupMServer:
			blRet = OpenMServer(env,&mServer,msg);
			MPSend3(MESG_ANS_SetupMServer,((blRet > 0) ? "Y" : "N"),blRet2);
			if (blRet2 == G_FALSE)
				cgMSPool.CloseChild(mServer);
			break;
		case MESG_ANS_SetupMServer:
			SetSFlag((msg == "Y") ? RMS_blSetupMServerY : RMS_blSetupMServerN);
			break;
		case MESG_REQ_CloseMServer:
			blRet = CloseMServer(msg);
			MPSend3(MESG_ANS_CloseMServer,((blRet > 0) ? "Y" : "N"),blRet2);
			break;
		case MESG_ANS_CloseMServer:
			SetSFlag((msg == "Y") ? RMS_blCloseMServerY : RMS_blCloseMServerN);
			break;
		case MESG_REQ_ListMServer:
			strMesg = msg;
			strMesg += ',';
			ListMappingServer(&strMesg);
			MPSend(MESG_ANS_ListMServer,strMesg,blRet2);
			if (blRet2 == G_FALSE)
				cgMSPool.CloseChild(mServer);
			break;
		case MESG_ANS_ListMServer:
			strMesg = msg;
			strP = (STDSTR*)Str_HexToDec(Str_ReadSubItem(&strMesg, ","));
			*strP = strMesg;
			strP = nullptr;
			SetSFlag(RMS_blListMServerY);
			break;
		case MESG_REQ_SetupDataChannel:
			blRet = OpenCoupleSocket(env,&rmssocket,&bsocket,msg,_commu->GetOpenPar());
			MPSend3(MESG_ANS_SetupDataChannel,((blRet > 0) ? "Y" : "N"),blRet2);
			if (blRet2 == G_FALSE)
				cgCPPool.CloseChild(rmssocket,bsocket);
			if (blRet > 0){
				ELogMP("Setup Data Channel successful");
				ELogMP("Create pre-couple");
				cgCPPool.GetCP(&rmssocket, &bsocket);
				cgCPPool.CloseChild(rmssocket,bsocket);
				ELogMP("pre-couple" << rmssocket << bsocket);
				break;
			}
			ELogMP("Setup Data Channel fail");
			break;
		case MESG_ANS_SetupDataChannel:
			SetSFlag((msg == "Y") ? RMS_blLinkY : RMS_blLinkN);
			ELogMP("Setup Data Channel " << (CheckSFlag(RMS_blLinkY) ? "successful" : "fail"));
			break;
		case MESG_REQ_Link:
			bsocket = (BSOCKET*)Str_HexToDec(msg);
			blRet = G_FALSE;
			try{
				_commu->LinkCoupleNode(bsocket);
				ELogMP(this << "MessageProcessing()::Link to " << bsocket);
				blRet = G_TRUE;
			}
			catch(...){
				blRet = G_FALSE;
			}
			MPSend3(MESG_ANS_Link,((blRet > 0) ? "Y" : "N"),blRet2);
			break;
		case MESG_ANS_Link:
			SetSFlag((msg == "Y") ? RMS_blLinkY : RMS_blLinkN);
			break;
		default:
			retCode = MESG_NONE;
			break;
	}
	if (blRet2 == G_FALSE){
		ELogMP("Call SelfClose()");
		cgMsgCenter->SetblREQHS();
		_commu->CloseSelf(1);
	}
	return(retCode);
}
//------------------------------------------------------------------------------------------//
#endif /* MSG_RMS_h */
