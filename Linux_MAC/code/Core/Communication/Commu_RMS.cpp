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
//#define LOGPRINT_ENABLE
#include "Commu_RMS.h"
#include "Commu_SSLSocket.h"
#include "Comm_Convert.h"
#include "SYS_Time.h"
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#ifdef USE_OPENSSL
//------------------------------------------------------------------------------------------//
void MServer_Socket::ThreadsStart(void){
	txThread.ThreadRun();
	if (GetCSType() != CSType_UDPS)
		rxThread.ThreadRun();
}
//------------------------------------------------------------------------------------------//
void MServer_Socket::ForwardToCouple(const uint8 *databuf,int32 num){
	if (num > 0){
		ELogPrint(this,GetcgCoupleNode(this),"MServer_Socket::ForwardToCouple()::Rec data num:%d",num);
		((RemoteSSLSocket*)GetcgCoupleNode(this))->SSLWrite(databuf, num);
	}
}
//------------------------------------------------------------------------------------------//
void MServer_Socket::OnCloseDev(void){
	RemoteSSLSocket	*coupleSocket;
	ELogPrint(this, "MServer_Socket::OnCloseDev()");
	coupleSocket = (RemoteSSLSocket*)GetcgCoupleNode(this);
	if (coupleSocket != nullptr){
		coupleSocket->UnlinkCoupleNode();
		ELogPrint(this, coupleSocket,"MServer_Socket::OnCloseDev()::Will be close");
		coupleSocket->CloseD(0);
	}
	ELogPrint(this, "MServer_Socket::Call APISocket::OnCloseDev()");
	APISocket::OnCloseDev();
}
//------------------------------------------------------------------------------------------//
int32 MAPPINGServer::OnOpenTCPSocket(APISocket *newSocket){
	return(((RemoteSSLSocket*)GetFather(this))->SendRequestSetupDataChannel(newSocket,cgMapIP,cgMapPort,cgCSType));
};
//------------------------------------------------------------------------------------------//







//------------------------------------------------------------------------------------------//
void RemoteSSLSocket::OnCloseDev(void){
	APISocket	*coupleSocket;
	ELogPrint(this, "RemoteSSLSocket::OnCloseDev()");
	coupleSocket = (APISocket*)GetcgCoupleNode(this);
	if (coupleSocket != nullptr){
		UnlinkCoupleNode();
		ELogPrint(this, coupleSocket,"RemoteSSLSocket::OnCloseDev()::Will be close");
		coupleSocket->CloseD(0);
	}
	ELogPrint(this, "RemoteSSLSocket::Call DisconnectAll()");
	DisconnectAll();
	ELogPrint(this, "RemoteSSLSocket::Call SSLSocket::OnCloseDev()");
	SSLSocket::OnCloseDev();
}
//------------------------------------------------------------------------------------------//
void RemoteSSLSocket::ForwardToCouple(const uint8 *databuf,int32 num){
	if (num > 0){
		ELogPrint(this,GetcgCoupleNode(this),"RemoteSSLSocket::ForwardToCouple()::Rec data num:%d",num);
		((MServer_Socket*)GetcgCoupleNode(this))->Write(databuf, num);
	}
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#define SetRSSLFLAG(u64)		B_SetFLAG32(cgRSSLFlag,(u64))
#define ClrRSSLFLAG(u64)		B_ClrFLAG32(cgRSSLFlag,(u64))
#define ChkRSSLFLAG(u64)		B_ChkFLAG32(cgRSSLFlag,(u64))
enum{
	blSetupMServer	= BD_FLAG64(0),
	blSetupMServerY	= BD_FLAG64(1),
	blOffMServer	= BD_FLAG64(2),
	blOffMServerY	= BD_FLAG64(3),
	blListMServer	= BD_FLAG64(4),
	blListMServerY	= BD_FLAG64(5),
	blLink			= BD_FLAG64(6),
	blLinkY			= BD_FLAG64(7),
};
//------------------------------------------------------------------------------------------//
int32 RemoteSSLSocket::MessageProcessing(FNode_MESG *RecMesg,int32 blReady){
	std::string			strMesg,strContent,*strP;
	uint32 				mID;
	MServer_Socket		*mSocket;
	int32				blDo,blRet,blRet2;
	
	if (SSLSocket::MessageProcessing(RecMesg,blReady) > 0)
		return 1;

	if (blReady == 0)
		return 0;
	
	blDo = 1;
	RecMesg->ReadContent(&strMesg,&mID);
	switch (mID){
		case MESG_REQ_SetupMServer:
			ELogPrint(this, "MessageProcessing()::Rec  MESG_REQ_SetupMServer");
			blRet = OpenMappingServer(strMesg);
			ELogPrint(this, "MessageProcessing()::Send MESG_ANS_SetupMServer with MESG:%s",(blRet > 0) ? "Y" : "N");
			blRet2 = CtrlCHWrite((blRet > 0) ? "Y" : "N",MESG_ANS_SetupMServer);
			ELogPrint(this, "MessageProcessing()::Send MESG_ANS_SetupMServer %s",(blRet2 > 0) ? "successful" : "fail");
			if ((blRet > 0) && (blRet2 > 0)){
				((RemoteSSLServer*)GetFather(this))->CreateNewMServer(blRet);
			}
			else{
				((RemoteSSLServer*)GetFather(this))->RemoveSelf();
				((RemoteSSLServer*)GetFather(this))->CloseD();
			}
			break;
		case MESG_ANS_SetupMServer:
			ELogPrint(this, "MessageProcessing()::Rec  MESG_ANS_SetupMServer:%s",strMesg.c_str());
			if (strMesg == "Y")
				SetRSSLFLAG(blSetupMServerY);
			SetRSSLFLAG(blSetupMServer);
			break;
		case MESG_REQ_CloseMServer:
			ELogPrint(this, "MessageProcessing()::Rec  MESG_REQ_CloseMServer");
			blRet = CloseMappingServer(strMesg);
			ELogPrint(this, "MessageProcessing()::Send MESG_ANS_OffMServer with MESG:%s",(blRet > 0) ? "Y" : "N");
			blRet2 = CtrlCHWrite((blRet > 0) ? "Y" : "N",MESG_ANS_CloseMServer);
			ELogPrint(this, "MessageProcessing()::Send MESG_ANS_OffMServer %s",(blRet2 > 0) ? "successful" : "fail");
			break;
		case MESG_ANS_CloseMServer:
			ELogPrint(this, "MessageProcessing()::Rec MESG_ANS_CloseMServer:%s",strMesg.c_str());
			if (strMesg == "Y")
				SetRSSLFLAG(blOffMServerY);
			SetRSSLFLAG(blOffMServer);
			break;
		case MESG_REQ_ListMServer:
			ELogPrint(this, "MessageProcessing()::Rec MESG_REQ_ListMServer");
			ListMappingServer(&strContent);
			strMesg += ",";
			strMesg += strContent;
			ELogPrint(this, "MessageProcessing()::Send MESG_ANS_ListMServer");
			blRet2 = CtrlCHWrite(strMesg,MESG_ANS_ListMServer);
			ELogPrint(this, "MessageProcessing()::Send MESG_ANS_ListMServer %s",(blRet2 > 0) ? "successful" : "fail");
			break;
		case MESG_ANS_ListMServer:
			ELogPrint(this, "MessageProcessing()::Rec MESG_ANS_ListMServer");
			strP = (std::string*)Str_HexToDec(Str_ReadSubItem(&strMesg, ","));
			*strP = strMesg;
			SetRSSLFLAG(blListMServerY);
			SetRSSLFLAG(blListMServer);
			break;
		case MESG_REQ_SetupDataChannel:
			ELogPrint(this, "MessageProcessing()::Rec MESG_REQ_SetupDataChannel");
			blRet = OpenCoupleSocket(strMesg);
			ELogPrint(this, "MessageProcessing()::Send MESG_ANS_SetupDataChannel with MESG:%s",(blRet > 0) ? "Y" : "N");
			blRet2 = CtrlCHWrite((blRet > 0) ? "Y" : "N",MESG_ANS_SetupDataChannel);
			ELogPrint(this, "MessageProcessing()::Send MESG_ANS_SetupDataChannel ",(blRet2 > 0) ? "successful" : "fail");
			if ((blRet > 0) && (blRet2 > 0)){
				cgRSocket = new MServer_Socket(GetcgODevList(),BufferRxMaxSize());
				cgLSSLSocket = new RemoteSSLSocket(GetcgODevList(),BufferRxMaxSize());
				ELogPrint(this, "MessageProcessing()::Create new pre-SSLSocket");
				ELogPrint(this, "MessageProcessing()::Create new pre-RSocket");
				ELogPrint(this, "MessageProcessing()::Setup Data Channel successful");
			}
			else{
				ELogPrint(this, "MessageProcessing()::Setup Data Channel fail");
				cgLSSLSocket->UnlinkCoupleNode();
				cgLSSLSocket->RemoveSelf();
				cgRSocket->RemoveSelf();
				cgRSocket->CloseD();
				cgLSSLSocket->CloseD();
			}
			break;
		case MESG_ANS_SetupDataChannel:
			ELogPrint(this, "MessageProcessing()::Rec MESG_ANS_SetupDataChannel");
			if (strMesg == "Y")
				SetRSSLFLAG(blLinkY);
			SetRSSLFLAG(blLink);
			ELogPrint(this, "MessageProcessing()::Setup Data Channel ",(ChkRSSLFLAG(blLinkY) > 0) ? "successful" : "fail");
			break;
		case MESG_REQ_Link:
			ELogPrint(this, "MessageProcessing()::Rec MESG_REQ_Link");
			mSocket = (MServer_Socket*)Str_HexToDec(strMesg);
			blRet = 0;
			try{
				LinkCoupleNode(mSocket);
				ELogPrint(this, mSocket, "MessageProcessing()::Is linked");
				blRet = 1;
			}
			catch(...){blRet = 0;}
			ELogPrint(this, "MessageProcessing()::Send MESG_ANS_Link with MESG:%s",(blRet > 0) ? "Y" : "N");
			blRet2 = CtrlCHWrite((blRet > 0) ? "Y" : "N",MESG_ANS_Link);
			ELogPrint(this, "MessageProcessing()::Send MESG_ANS_Link %s",(blRet2 > 0) ? "successful" : "fail");
			break;
		case MESG_ANS_Link:
			ELogPrint(this, "MessageProcessing()::Rec MESG_ANS_Link:%s",strMesg.c_str());
			if (strMesg == "Y")
				SetRSSLFLAG(blLinkY);
			SetRSSLFLAG(blLink);
			break;
		default:
			blDo = 0;
			break;
	}
	return(blDo);
}
//------------------------------------------------------------------------------------------//
int32 RemoteSSLSocket::OpenMappingServer(const std::string &strMesg){
	MAPPINGServer	*tMServer;
	std::string		strMapIP;
	int32 			rPort,mapPort,mapCSType;
	int32	ret;
	ret = 0;
	
	strMapIP = strMesg;
	rPort = (int32)Str_HexToDec(Str_ReadSubItem(&strMapIP, ","));
	mapCSType = (int32)Str_HexToDec(Str_ReadSubItem(&strMapIP, ","));
	mapPort = (int32)Str_HexToDec(Str_ReadSubItem(&strMapIP, ","));
	
	tMServer = ((RemoteSSLServer*)GetFather(this))->GetNewMServer();
	if (tMServer != nullptr){
		if (tMServer->Run(rPort,strMapIP,mapPort,(COMMU_DBUF::CSType)mapCSType) > 0){
			ret = 1;
			AddNode(tMServer);
		}
		else{
			tMServer->CloseD();
		}
	}
	return(ret);
}
//------------------------------------------------------------------------------------------//
int32 RemoteSSLSocket::CloseMappingServer(const std::string &strMesg){
	int32 			rPort;
	int32			ret;
	ret = 0;
	
	rPort = (int32)Str_HexToDec(strMesg);
	if (rPort == 0){
		ELogPrint(this, "CloseMappingServer()::Close all MServer");
		DisconnectAll();
		ret = 1;
	}
	else{
		Spin_InUse_set();
		RTREE_RChain_Traversal_LINE_nolock(MAPPINGServer,GetcgLChild(this),
			if (operateNode_t->GetcgPort() == rPort){
				ELogPrint(this, "CloseMappingServer()::Close MServer(%d)",rPort);
				operateNode_t->CloseD();
				ret = 1;
				break;
			}
		);
		Spin_InUse_clr();
	}
	return(ret);
}
//------------------------------------------------------------------------------------------//
int32 RemoteSSLSocket::ListMappingServer(std::string *strMesg){
	ELogPrint(this, "ListMappingServer()::Prepare MServer list string");
	*strMesg = "";
	Spin_InUse_set();
	RTREE_RChain_Traversal_LINE_nolock(MAPPINGServer,GetcgLChild(this),
		*strMesg += "MServer(";
		*strMesg += Str_IntToString(operateNode_t->GetcgPort());
		*strMesg += ") mapping to ";
		*strMesg += operateNode_t->GetcgMapIP();
		*strMesg += ":";
		*strMesg += Str_IntToString(operateNode_t->GetcgMapPort());
		if (operateNode_t->GetCSType() == CSType_TCP){
			*strMesg += ",TCP";
		}
		else{
			*strMesg += ",UDP";
		}
		*strMesg += "\r\n";
	);
	Spin_InUse_clr();
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 RemoteSSLSocket::OpenCoupleSocket(std::string strMesg){
	int32 			ret;
	std::string		strMapIP;
	int32 			mapPort,mapCSType;
	
	ret = 0;
	mapCSType = (int32)Str_HexToDec(Str_ReadSubItem(&strMesg, ","));
	mapPort = (int32)Str_HexToDec(Str_ReadSubItem(&strMesg, ","));
	strMapIP = Str_ReadSubItem(&strMesg, ",");
	
	if (cgRSocket == nullptr)
		cgRSocket = new MServer_Socket(GetcgODevList(),BufferRxMaxSize());
	if (cgLSSLSocket == nullptr)
		cgLSSLSocket = new RemoteSSLSocket(GetcgODevList(),BufferRxMaxSize());
	
	if ((cgRSocket != nullptr) && (cgLSSLSocket != nullptr)){
		cgRSocket->selfName = selfName + "->RSocket" + Str_IntToString(GetnodeID(cgRSocket).load());
		cgLSSLSocket->SetSelfName(selfName + "->SSLSocket" + Str_IntToString(GetnodeID(cgLSSLSocket).load()));
		ELogPrint(this, "OpenCoupleSocket()::Created %s",cgRSocket->selfName.c_str());
		ELogPrint(this, "OpenCoupleSocket()::Created %s",cgLSSLSocket->selfName.c_str());
		if ((cgLSSLSocket->OpenD(GetBufName(), GetBufPar(), GetCSType(), 0) > 0)
			&& (cgLSSLSocket->CheckHandshake() > 0)){
			ELogPrint(this, "OpenCoupleSocket()::Open SSLSocket OK");
			if (cgLSSLSocket->SendRequestLink(strMesg) > 0){
				cgLSSLSocket->LinkCoupleNode(cgRSocket);
				ELogPrint(cgLSSLSocket, cgRSocket, "OpenCoupleSocket()::Linked");
				if (cgRSocket->OpenD(strMapIP, mapPort, (COMMU_DBUF::CSType)mapCSType, 0) > 0){
					ELogPrint(this, "OpenCoupleSocket()::Open RSocket OK");
					AddNode(cgRSocket);
					AddNode(cgLSSLSocket);
					ret = 1;
				}
				else{
					ELogPrint(this, "OpenCoupleSocket()::Open RSocket Fail");
					cgLSSLSocket->LinkCoupleNode(cgRSocket);
					cgLSSLSocket->CloseD();
				}
			}
			else{
				cgLSSLSocket->CloseD();
			}
		}
		else{
			ELogPrint(this, "OpenCoupleSocket()::Open SSLSocket Fail");
		}
	}
	return(ret);
}
//------------------------------------------------------------------------------------------//
int32 RemoteSSLSocket::SendRequestSetupMServer(int32 remotePort,const std::string &mapIP,int32 mapPort,COMMU_DBUF::CSType tCSType){
	SYS_TIME_S		Timedly;
	std::string		strMesg;

	ClrRSSLFLAG(blSetupMServer | blSetupMServerY);
	strMesg = Str_DecToHex((uint64)remotePort);
	strMesg += ',';
	strMesg += Str_DecToHex((uint64)tCSType);
	strMesg += ',';
	strMesg += Str_DecToHex((uint64)mapPort);
	strMesg += ',';
	strMesg += mapIP;
	ELogPrint(this, "Send MESG_REQ_SetupRemoteServer");
	if (CtrlCHWrite(strMesg,MESG_REQ_SetupMServer) > 0){
		SYS_Delay_SetTS(&Timedly, HandshakeTime);
		while((ChkRSSLFLAG(blSetupMServer) == 0) && (IsConnected() > 0) && (SYS_Delay_CheckTS(&Timedly) == 0))
			SYS_SleepMS(100);
	}
	return(ChkRSSLFLAG(blSetupMServerY));
}
//------------------------------------------------------------------------------------------//
int32 RemoteSSLSocket::SendRequestOffMServer(int32 remotePort){
	SYS_TIME_S		Timedly;
	std::string		strMesg;
	
	ClrRSSLFLAG(blOffMServer | blOffMServerY);
	strMesg = Str_DecToHex((uint64)remotePort);//0-->close all
	ELogPrint(this, "Send MESG_REQ_CloseMServer");
	if (CtrlCHWrite(strMesg,MESG_REQ_CloseMServer) > 0){
		SYS_Delay_SetTS(&Timedly, HandshakeTime);
		while((ChkRSSLFLAG(blOffMServer) == 0) && (IsConnected() > 0) && (SYS_Delay_CheckTS(&Timedly) == 0))
			SYS_SleepMS(100);
	}
	return(ChkRSSLFLAG(blOffMServerY));
}
//------------------------------------------------------------------------------------------//
int32 RemoteSSLSocket::SendRequestListMServer(std::string *retStr){
	SYS_TIME_S		Timedly;
	std::string		strMesg;
	
	ClrRSSLFLAG(blListMServer | blListMServerY);
	strMesg = Str_DecToHex((uint64)retStr);
	ELogPrint(this, "Send MESG_REQ_ListMServer");
	if (CtrlCHWrite(strMesg,MESG_REQ_ListMServer) > 0){
		SYS_Delay_SetTS(&Timedly, HandshakeTime);
		while((ChkRSSLFLAG(blListMServer) == 0) && (IsConnected() > 0) && (SYS_Delay_CheckTS(&Timedly) == 0))
			SYS_SleepMS(100);
	}
	return(ChkRSSLFLAG(blListMServerY));
}
//------------------------------------------------------------------------------------------//
int32 RemoteSSLSocket::SendRequestSetupDataChannel(APISocket *mSocket,const std::string &mapIP,int32 mapPort,COMMU_DBUF::CSType tCSType){
	SYS_TIME_S		Timedly;
	std::string		strMesg;
	
	ClrRSSLFLAG(blLink | blLinkY);
	strMesg = Str_DecToHex((uint64)tCSType);
	strMesg += ',';
	strMesg += Str_DecToHex((uint64)mapPort);
	strMesg += ',';
	strMesg += mapIP;
	strMesg += ',';
	strMesg += Str_DecToHex((uint64)mSocket);
	ELogPrint(this, "Send MESG_REQ_SetupDataChannel");
	if (CtrlCHWrite(strMesg,MESG_REQ_SetupDataChannel) > 0){
		SYS_Delay_SetTS(&Timedly, (HandshakeTime << 1));
		while((ChkRSSLFLAG(blLink) == 0) && (IsConnected() > 0) && (SYS_Delay_CheckTS(&Timedly) == 0))
			SYS_SleepMS(100);
	}
	return(ChkRSSLFLAG(blLinkY));
}
//------------------------------------------------------------------------------------------//
int32 RemoteSSLSocket::SendRequestLink(const std::string &strMesg){
	SYS_TIME_S			Timedly;
	
	ClrRSSLFLAG(blLink | blLinkY);
	ELogPrint(this, "Send MESG_REQ_Link");
	if (CtrlCHWrite(strMesg,MESG_REQ_Link) > 0){
		SYS_Delay_SetTS(&Timedly, HandshakeTime);
		while((ChkRSSLFLAG(blLink) == 0) && (IsConnected() > 0) && (SYS_Delay_CheckTS(&Timedly) == 0))
			SYS_SleepMS(100);
	}
	return(ChkRSSLFLAG(blLinkY));
}
//------------------------------------------------------------------------------------------//


//------------------------------------------------------------------------------------------//
ControlSocket::~ControlSocket(void){
	MoveNodeToTrash(cgLSSLSocket,this);
	MoveNodeToTrash(cgRSocket,this);
	cgLSSLSocket = nullptr;
	cgRSocket = nullptr;
	DisconnectAll();
}
//------------------------------------------------------------------------------------------//
void ControlSocket::PreCreate(void){
	if ((GetCSType() == CSType_TCP) || (GetCSType() == CSType_UDP)){
		cgRSocket = new MServer_Socket(GetcgODevList(),BufferRxMaxSize());
		cgLSSLSocket = new RemoteSSLSocket(GetcgODevList(),BufferRxMaxSize());
	}
}
//------------------------------------------------------------------------------------------//
void ControlSocket::OnCloseDev(void){
	ELogPrint(this, "ControlSocket::OnCloseDev()");
	MoveNodeToTrash(cgLSSLSocket, this);
	MoveNodeToTrash(cgRSocket, this);
	cgLSSLSocket = nullptr;
	cgRSocket = nullptr;
	ELogPrint(this, "ControlSocket::Call RemoteSSLSocket::OnCloseDev()");
	RemoteSSLSocket::OnCloseDev();
}
//------------------------------------------------------------------------------------------//
int32 ControlSocket::Ex2ThreadFun(void){
	RTREE_NODE	*delNode,*fromNode;
	int32		blNeedUpdate;
	
	SYS_SleepMS(6);
	while(ex2Thread.IsTerminated() == 0){
		SYS_SleepMS(20);
		Spin_InUse_set();
		
		fromNode = GetcgLChild(this);
		blNeedUpdate = 0;
		delNode = nullptr;
		do{
			RTREE_RChain_Traversal_LINE(RTREE_NODE,fromNode,
				if ((GetCSType() == CSType_TCP) || (GetCSType() == CSType_UDP)){
					if (((APISocket*)operateNode_t)->IsConnected() == 0){
						delNode = operateNode_t;
						break;
					}
				}
				else{
					if (((MAPPINGServer*)operateNode_t)->CheckblConnected() == 0){
						delNode = operateNode_t;
						break;
					}
				}
			);
			fromNode = nullptr;
			if (delNode != nullptr){
				fromNode = GetcgRChild(delNode);
				ELogPrint(this, delNode, "Ex2ThreadFun()::Call CloseD()");
				if ((GetCSType() == CSType_TCP) || (GetCSType() == CSType_UDP)){
					((APISocket*)delNode)->CloseD(0);
				}
				else{
					((MAPPINGServer*)delNode)->CloseD();
				}
				RemoveNodesInRChain(delNode,nullptr,G_LOCK_OFF);
				InsertLChild(GetcgTrash(this), delNode);
				blNeedUpdate = 1;
			}
		}while(fromNode != nullptr);
		if (blNeedUpdate != 0){
			CleanTrash(this);
			SetblUpdate();
		}
		Spin_InUse_clr();
	}
	return 1;
}
//------------------------------------------------------------------------------------------//
void ControlSocket::DisconnectAll(void){
	Spin_InUse_set();
	if ((GetCSType() == CSType_TCP) || (GetCSType() == CSType_UDP)){
		RTREE_RChain_Traversal_LINE_nolock(APISocket,GetcgLChild(this),operateNode_t->CloseD(0));
	}
	else{
		RTREE_RChain_Traversal_LINE_nolock(MAPPINGServer,GetcgLChild(this),operateNode_t->CloseD());
	}
	SetblUpdate();
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
MAPPINGServer *RemoteSSLServer::GetNewMServer(void){
	NLTCPS_set();
	if (cgMServer == nullptr)
		cgMServer = new MAPPINGServer(GetcgODevList(),cgBufMaxSize);
	return(cgMServer);
}
//------------------------------------------------------------------------------------------//
void RemoteSSLServer::CreateNewMServer(int32 blenable){
	if (blenable != 0)
		cgMServer = new MAPPINGServer(GetcgODevList(),cgBufMaxSize);
	NLTCPS_clr();
}
//------------------------------------------------------------------------------------------//
#endif




