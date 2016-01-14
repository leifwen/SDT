/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: APISocketServer.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2012.12.22
 * @2013.7.12 :	improve TCP/UDP performance
 change to Liunx style
 add init method
 use int32 to replace bool
 use int32 to replace uint32
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "Commu_SocketServer.h"
#include "Comm_Convert.h"
#include "ODEV_Include.h"
#include "SMC.h"
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
void APISocketServer_Socket::PrintRecDataTitle(ODEV_LIST_POOL *output, uint32 byteNum){
	std::string	strName,strSelfName;
	
	if (output == nullptr)
		return;
	
	strSelfName = GetBufName() + ":" + Str_IntToString(GetBufPar());
	
	if (GetcgCoupleNode(this) != nullptr)
		strName = ((COMMU_DBUF*)GetcgCoupleNode(this))->GetBufName() + ":" + Str_IntToString(((COMMU_DBUF*)GetcgCoupleNode(this))->GetBufPar());
	if (CheckEnableEcho() != 0){
		output->WriteEchoTitle(strSelfName, byteNum, FwBytes(),RICH_CF_clMaroon);
		if (GetcgCNType(this) == CN_1)
			output->WriteTxTitle(strSelfName, strName, byteNum, RxBytes(), RICH_CF_clRed);
	}
	else if (GetcgCNType(this) == CN_None){
		output->WriteRxTitle(strSelfName, "", byteNum, RxBytes(), RICH_CF_clRed);
	}
	else if (GetcgCNType(this) == CN_1){
		output->WriteTxTitle(strSelfName, strName, byteNum, RxBytes(), RICH_CF_clRed);
	}
	else if (GetcgCNType(this) == CN_0){
		output->WriteRxTitle(strName, strSelfName, byteNum, RxBytes(), RICH_CF_clMaroon);
	}
}
//------------------------------------------------------------------------------------------//
int32 APISocketServer_Socket::Ex2ThreadFun(void){
	int32		byteNum;
	std::string	strPrintdata;
	uint64		oDevFlagU64;
	
	strPrintdata = "";
	SetblcgRxBufferUsed();
	while(ex2Thread.IsTerminated() == 0){
		oDevFlagU64 = 0;
		if (cgRxBuffer.Used() > 0){
			Spin_InUse_set();
			strPrintdata = "";
			if (GetcgODevList() != nullptr)
				oDevFlagU64 = GetcgODevList()->cgODevFlagU64;
			
			if (B_ChkFLAG64(oDevFlagU64,ODEV_FLAG_EnHEXViewMode) != 0){
				byteNum = ReadInHEX(&strPrintdata,G_SPACE_ON);
			}
			else{
				byteNum = ReadInASCII(&strPrintdata,(B_ChkFLAG64(oDevFlagU64,ODEV_FLAG_EnEscape)>0)?G_ESCAPE_ON:G_ESCAPE_OFF);
			}
			
			if (GetcgOutput() != nullptr){
				GetcgOutput()->Spin_InUse_set();
				if (CheckblSelected() == 0){
					PrintRecDataTitle(GetcgOutput(),byteNum);
				}
				else{
					PrintRecData(GetcgOutput(),strPrintdata,byteNum,2);
				}
				GetcgOutput()->Spin_InUse_clr();
			}
			if (GetcgODevPool() != nullptr)
				PrintRecData_lock(GetcgODevPool(),strPrintdata,byteNum,1);
			if ((GetcgCoupleNode(this) != nullptr) && (((APISocket*)GetcgCoupleNode(this))->GetcgODevPool() != nullptr))
				PrintRecData_lock(((APISocket*)GetcgCoupleNode(this))->GetcgODevPool(),strPrintdata,byteNum,1);
			Spin_InUse_clr();
		}
		SYS_SleepMS(10);
	}
	return 1;
}
//------------------------------------------------------------------------------------------//
void APISocketServer_Socket::CreateLogFile(void){
	SMC_EncryptI(0)
	std::string strName;
	
	strName = ODEV_CreateNewLOGFileName(GetBufName(),GetBufPar());
	if (GetCSType() == CSType_TCP)
		strName += "_TCP";
	if (GetCSType() == CSType_UDP)
		strName += "_UDP";
	SMC_EncryptS(0)
	CreateODevFile(strName);
	SMC_EncryptE(0)
}
//------------------------------------------------------------------------------------------//














//------------------------------------------------------------------------------------------//
APISocketServer::APISocketServer(const ODEV_LIST *tODEV_LIST,uint32 tSize) : CDBUF_POOL(){
	listionSocket = INVALID_SOCKET;
	
	disconnectThread.ThreadInit(this,&APISocketServer::DisconnectThreadFun);
	tcplistionThread.ThreadInit(this,&APISocketServer::TCPThreadListionFun);
	udplistionThread.ThreadInit(this,&APISocketServer::UDPThreadListionFun);
	
	cgThreadList.AddNode(&disconnectThread);
	cgThreadList.AddNode(&tcplistionThread);
	cgThreadList.AddNode(&udplistionThread);
	
	cgCSType = COMMU_DBUF::CSType_TCP;
	
	cgBufMaxSize = tSize;
	cgPort = 0;
	SetODEV_LIST(tODEV_LIST);
	selfName = "APISocketServer";
	cgNewSocket = nullptr;
}
//------------------------------------------------------------------------------------------//
void APISocketServer::SetODEV_LIST(const ODEV_LIST *tODEV_LIST){
	Spin_InUse_set();
	cgODevList = (ODEV_LIST*)tODEV_LIST;
	cgOutput = nullptr;
	if (cgODevList != nullptr)
		cgOutput = cgODevList->cgOutput;
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
APISocket *APISocketServer::CreateNewSocket_TCP(const ODEV_LIST *tODEV_LIST,uint32 tSize){
	APISocketServer_Socket *tPDB = new APISocketServer_Socket(tODEV_LIST,tSize);
	return(tPDB);
}
//------------------------------------------------------------------------------------------//
int32 APISocketServer::OpenD(int32 port,COMMU_DBUF::CSType tStype,int32 blEnEcho){
	int32 ret;
	ret = 0;
	if (InDoing_try() != 0){
		ret = OpenDev(port,tStype,blEnEcho);
		if (ret != 0)
			Enable();
		InDoing_clr();
	}
	return(ret);
}
//------------------------------------------------------------------------------------------//
int32 APISocketServer::OpenDev(int32 port,COMMU_DBUF::CSType tStype,int32 blEnEcho){
	sockaddr_in		serviceAddr;
	
	cgCSType = (tStype == COMMU_DBUF::CSType_None) ? COMMU_DBUF::CSType_TCP : tStype;

	Close_Do();
	
	serviceAddr.sin_family = AF_INET;
	serviceAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serviceAddr.sin_port = htons(port);
	
	if (cgCSType == COMMU_DBUF::CSType_TCP){
		listionSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	}
	else{
		listionSocket = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	}
	if (listionSocket == INVALID_SOCKET){
		Close_Do();
		return 0;
	}
#ifdef CommonDefH_Unix
	if(bind(listionSocket,(struct sockaddr *)&serviceAddr,sizeof(serviceAddr)) == SOCKET_ERROR){
		Close_Do();
		return 0;
	}
#endif
#ifdef CommonDefH_VC

	if(bind(listionSocket,(LPSOCKADDR)&serviceAddr,sizeof(serviceAddr)) == SOCKET_ERROR){
		Close_Do();
		return 0;
	}
#endif
	if (cgCSType == COMMU_DBUF::CSType_TCP){
		if (listen(listionSocket,10) == SOCKET_ERROR){
			Close_Do();
			return 0;
		}
		cgNewSocket = CreateNewSocket_TCP(cgODevList,cgBufMaxSize);
		tcplistionThread.ThreadRun();
	}
	else if (cgCSType == COMMU_DBUF::CSType_UDP){
		cgNewSocket = CreateNewSocket_UDP(cgODevList,cgBufMaxSize);
		udplistionThread.ThreadRun();
	}
	disconnectThread.ThreadRun();
	
	SetblConnected();
	cgPort = port;
	LDisableEcho();
	if (blEnEcho != 0)
		LEnableEcho();
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 APISocketServer::CloseD(void){
	int32 ret;
	InDoing_set();
	ret = Close_Do();
	Disable();
	InDoing_clr();
	return(ret);
}
//------------------------------------------------------------------------------------------//
int32 APISocketServer::Close_Do(void){
	cgPort = 0;
	MoveNodeToTrash(cgNewSocket,this);
	cgNewSocket = nullptr;
	DisconnectAll();
	DestroyAll();

	if (listionSocket != INVALID_SOCKET){
		shutdown(listionSocket,SD_BOTH);
#ifdef CommonDefH_Unix
		close(listionSocket);
#endif
#ifdef CommonDefH_VC
		closesocket(listionSocket);
#endif
		listionSocket = INVALID_SOCKET;
	}
	disconnectThread.ThreadStop();
	tcplistionThread.ThreadStop();
	udplistionThread.ThreadStop();
	cgThreadList.LThreadStop();
	ClrblConnected();
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 APISocketServer::TCPThreadListionFun(void){
	sockaddr_in		ListionAddr;
#ifdef CommonDefH_Unix
	socklen_t	addrlen;
#endif
#ifdef CommonDefH_VC
	int			addrlen;
#endif
	SOCKET			newSocket;
	std::string		strTemp,strIP;
	int32			port;
	
	addrlen = sizeof(ListionAddr);
	while(tcplistionThread.IsTerminated() == 0){
		newSocket = accept(listionSocket,(struct sockaddr *)&ListionAddr,&addrlen);
		if (newSocket != INVALID_SOCKET){
			strIP = inet_ntoa(ListionAddr.sin_addr);
			port = ntohs(ListionAddr.sin_port);
			Spin_InUse_set();
			if (cgNewSocket == nullptr)
				cgNewSocket = (APISocket*)CreateNewSocket_TCP(cgODevList,cgBufMaxSize);
			if (cgNewSocket == nullptr){
				shutdown(newSocket,SD_BOTH);
				#ifdef CommonDefH_Unix
					close(newSocket);
				#endif
				#ifdef CommonDefH_VC
					closesocket(newSocket);
				#endif
				if (cgOutput != nullptr){
					cgOutput->Spin_InUse_set();
					cgOutput->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
					cgOutput->WriteStrN("No enough resource to create new connection.\r\n",RICH_CF_clRed,COLSTRING::COL_EP_YES);
					cgOutput->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
					cgOutput->Spin_InUse_clr();
				}
			}
			else{
				cgNewSocket->Handle = newSocket;
				cgNewSocket->SetSocketBufferSize();
				if (OnOpenTCPSocket(cgNewSocket) > 0){
					if (cgNewSocket->OpenD(strIP,port,COMMU_DBUF::CSType_TCPS,CheckblEcho()) > 0){
						cgNewSocket->SetblHold();
						cgNewSocket->CreateLogFile();
						cgNewSocket->PrintConnectInfo(0);
						cgNewSocket->ClrblHold();
						AddNode(cgNewSocket);
						SetblUpdate();
						cgNewSocket = (APISocket*)CreateNewSocket_TCP(cgODevList,cgBufMaxSize);
					}
					else{
						cgNewSocket->CloseD(1);
					}
				}
				else{
					cgNewSocket->CloseD(1);
				}
			}
			Spin_InUse_clr();
		}
	}
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 APISocketServer::UDPThreadListionFun(void){
	sockaddr_in		ListionAddr;
#ifdef CommonDefH_Unix
	socklen_t	addrlen;
#endif
#ifdef CommonDefH_VC
	int			addrlen;
#endif
	APISocket		*udpSocket;
	uint8			*tempBuffer;
	std::string		strIP;
	int32			bytesNum;
	int32			port;
	std::string		strTemp;
	
	tempBuffer = new uint8[cgBufMaxSize];
	addrlen = sizeof(ListionAddr);
	
	while(udplistionThread.IsTerminated() == 0){
		bytesNum = (int32)recvfrom(listionSocket,(char*)tempBuffer,cgBufMaxSize,0,(struct sockaddr *)&ListionAddr,&addrlen);
		if ((bytesNum != SOCKET_ERROR) && (bytesNum > 0)){
			strIP = inet_ntoa(ListionAddr.sin_addr);
			port = ntohs(ListionAddr.sin_port);
			Spin_InUse_set();
			udpSocket = (APISocket*)Find(strIP,port);
			if (udpSocket == nullptr){
				if (cgNewSocket == nullptr)
					cgNewSocket = (APISocket*)CreateNewSocket_UDP(cgODevList,cgBufMaxSize);
				if (cgNewSocket == nullptr){
					if (cgOutput != nullptr){
						cgOutput->Spin_InUse_set();
						cgOutput->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
						cgOutput->WriteStrN("No enough resource to create new connection.\r\n",RICH_CF_clRed,COLSTRING::COL_EP_YES);
						cgOutput->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
						cgOutput->Spin_InUse_clr();
					}
				}
				else{
					udpSocket = cgNewSocket;
					cgNewSocket->Handle = listionSocket;
					if (OnOpenUDPSocket(cgNewSocket) > 0){
						if (cgNewSocket->OpenD(strIP,port,COMMU_DBUF::CSType_UDPS,CheckblEcho()) > 0){
							cgNewSocket->SetblHold();
							cgNewSocket->CreateLogFile();
							cgNewSocket->ClrblHold();
							AddNode(cgNewSocket);
							SetblUpdate();
							cgNewSocket = (APISocket*)CreateNewSocket_UDP(cgODevList,cgBufMaxSize);
						}
						else{
							cgNewSocket->CloseD(1);
						}
					}
					else{
						cgNewSocket->CloseD(1);
					}
				}
			}
			if (udpSocket != nullptr)
				udpSocket->DoAfterReadFromDevice(tempBuffer,bytesNum);
			Spin_InUse_clr();
		}
	}
	try{
		delete []tempBuffer;
	}
	catch(...){}
	tempBuffer = nullptr;
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 APISocketServer::DisconnectThreadFun(void){
	APISocket	*delNode,*fromNode;
	int32		blNeedUpdate;
	
	while(disconnectThread.IsTerminated() == 0){
		SYS_SleepMS(20);
		Spin_InUse_set();
		
		fromNode = (APISocket*)GetcgLChild(this);
		blNeedUpdate = 0;
		do{
			RTREE_RChain_Find(APISocket,fromNode,delNode,(operateNode_t->IsConnected() == 0));
			fromNode = nullptr;
			if (delNode != nullptr){
				OnDoDisconnect(delNode);
				fromNode = (APISocket*)GetcgRChild(delNode);
				delNode->PrintDisconnectInfo();
				delNode->CloseD(0);
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
void APISocketServer::DisconnectAll(void){
	Spin_InUse_set();
	RTREE_RChain_Traversal_LINE_nolock(APISocket,GetcgLChild(this),operateNode_t->CloseD(0));
	SetblUpdate();
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void APISocketServer::PrintOpenSuccessReport(void){
	std::string strPrintData;
	
	strPrintData = SYS_MakeTimeNow();
	strPrintData += " Successfully setup ";
	if (cgCSType == COMMU_DBUF::CSType_TCP){
		strPrintData += "TCP";
	}
	else{
		strPrintData += "UDP";
	}
	strPrintData += " Server. Listion Port is ";
	strPrintData += Str_IntToString(cgPort);
	strPrintData += ".\r\n";
	Spin_InUse_set();
	if (cgOutput != nullptr){
		cgOutput->Spin_InUse_set();
		cgOutput->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
		cgOutput->WriteStrN(strPrintData,RICH_CF_clPurple,COLSTRING::COL_EP_YES);
		cgOutput->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
		cgOutput->Spin_InUse_clr();
	}
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void APISocketServer::PrintOpenFailReport(void){
	std::string strPrintData;
	
	strPrintData = SYS_MakeTimeNow();
	if (cgCSType == COMMU_DBUF::CSType_TCP){
		strPrintData += " Failed setup TCP Server, please try again.";
	}
	else{
		strPrintData += " Failed setup UDP Server, please try again.";
	}
	
	Spin_InUse_set();
	if (cgOutput != nullptr){
		cgOutput->Spin_InUse_set();
		cgOutput->WriteDividingLine(RICH_CF_clBlack,COLSTRING::COL_EP_YES);
		cgOutput->WriteStrN(strPrintData,RICH_CF_clRed,COLSTRING::COL_EP_YES);
		cgOutput->WriteDividingLine(RICH_CF_clBlack,COLSTRING::COL_EP_YES);
		cgOutput->Spin_InUse_clr();
	}
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void APISocketServer::PrintUserDisconnectReport(const std::string &strType){
	std::string		strPrintData;
	Spin_InUse_set();
	if (cgOutput != nullptr){
		strPrintData = SYS_MakeTimeNow();
		strPrintData += strType;
		cgOutput->Spin_InUse_set();
		cgOutput->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
		cgOutput->WriteStrN(strPrintData,RICH_CF_clPurple,COLSTRING::COL_EP_YES);
		cgOutput->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
		cgOutput->Spin_InUse_clr();
	}
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
