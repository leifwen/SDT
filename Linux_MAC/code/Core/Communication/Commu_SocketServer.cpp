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
void APISocketS_TCP::PrintRecDataTitle(ODEV_LIST_POOL *output, uint32 byteNum){
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
int32 APISocketS_TCP::PrintThreadFun(void){
	int32		byteNum;
	std::string	strPrintdata;
	uint64		oDevFlagU64;
	
	strPrintdata = "";
	while(printThread.IsTerminated() == 0){
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
void APISocketS_TCP::ThreadsStart(void){
	SMC_EncryptI(0)
	SMC_EncryptS(0)
	txThread.ThreadRun();
	rxThread.ThreadRun();
	printThread.ThreadRun();
	SMC_EncryptE(0)
}
//------------------------------------------------------------------------------------------//
void APISocketS_TCP::CreateLogFile(void){
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
int32 APISocketS_UDP::OpenDev(const std::string &tCDBufName,int32 tCDBufPar,CSType tCSType){
#ifdef CommonDefH_Unix
	cgRemoteAddr.sin_addr.s_addr = inet_addr(GetBufName().c_str());
#endif
#ifdef CommonDefH_VC
	cgRemoteAddr.sin_addr.S_un.S_addr = inet_addr(GetBufName().c_str());
#endif
	cgRemoteAddr.sin_family = AF_INET;
	cgRemoteAddr.sin_port = htons(GetBufPar());
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 APISocketS_UDP::SendToDevice(const uint8 *buffer,uint32 length,uint32 *retNum){
	int64		retCode;
	uint32		i;

	i = 0;
	while((i < length) && (CheckblAClose() == 0)){
		retCode = sendto(Handle,(char*)&buffer[i],length - i,0,(struct sockaddr *)&cgRemoteAddr,sizeof(cgRemoteAddr));
		if (retCode == SOCKET_ERROR){
			*retNum = i;
			return -1;
		}
		i += (uint32)retCode;
	}
	*retNum = i;
	return 1;
}
//------------------------------------------------------------------------------------------//
void APISocketS_UDP::ThreadsStart(void){
	SMC_EncryptI(0)
	SMC_EncryptS(0)
	txThread.ThreadRun();
	printThread.ThreadRun();
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
	cgODevList = (ODEV_LIST*)tODEV_LIST;
	cgOutput = nullptr;
	if (cgODevList != nullptr)
		cgOutput = cgODevList->cgOutput;
}
//------------------------------------------------------------------------------------------//
APISocketServer::~APISocketServer(void){
	CloseD();
}
//------------------------------------------------------------------------------------------//
APISocket *APISocketServer::CreateNewSocket_TCP(const ODEV_LIST *tODEV_LIST,uint32 tSize){
	APISocketS_TCP *tPDB;
	tPDB = new APISocketS_TCP(tODEV_LIST,tSize);
	AddNode(tPDB);
	return(tPDB);
}
//------------------------------------------------------------------------------------------//
APISocket *APISocketServer::CreateNewSocket_UDP(const ODEV_LIST *tODEV_LIST,uint32 tSize){
	APISocketS_UDP *tPDB;
	tPDB = new APISocketS_UDP(tODEV_LIST,tSize);
	AddNode(tPDB);
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
	cgCSType = tStype;
	if (cgCSType == COMMU_DBUF::CSType_None)
		cgCSType = COMMU_DBUF::CSType_TCP;
	
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
		tcplistionThread.ThreadRun();
	}
	else if (cgCSType == COMMU_DBUF::CSType_UDP){
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
	APISocket		*newTCPClient;
	std::string		strTemp,strIP;
	int32			port;
	
	addrlen = sizeof(ListionAddr);
	while(tcplistionThread.IsTerminated() == 0){
		newSocket = accept(listionSocket,(struct sockaddr *)&ListionAddr,&addrlen);
		strIP = inet_ntoa(ListionAddr.sin_addr);
		port = ntohs(ListionAddr.sin_port);
		if (newSocket != INVALID_SOCKET){
			Spin_InUse_set();
			newTCPClient = (APISocket*)CreateNewSocket_TCP(cgODevList,cgBufMaxSize);
			if (newTCPClient == nullptr){
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
				newTCPClient->Handle = newSocket;
				newTCPClient->OpenD(strIP,port,COMMU_DBUF::CSType_TCP,CheckblEcho());
				newTCPClient->CreateLogFile();
				newTCPClient->PrintConnectInfo(0);
				SetblUpdate();
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
	APISocket		*newClient;
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
			newClient = (APISocket*)Find(strIP,port);
			if (newClient == nullptr){
				newClient = (APISocket*)CreateNewSocket_UDP(cgODevList,cgBufMaxSize);
				if (newClient == nullptr){
					if (cgOutput != nullptr){
						cgOutput->Spin_InUse_set();
						cgOutput->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
						cgOutput->WriteStrN("No enough resource to create new connection.\r\n",RICH_CF_clRed,COLSTRING::COL_EP_YES);
						cgOutput->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
						cgOutput->Spin_InUse_clr();
					}
				}
				else{
					newClient->Handle = listionSocket;
					newClient->OpenD(strIP,port,COMMU_DBUF::CSType_UDP,CheckblEcho());
					newClient->CreateLogFile();
					SetblUpdate();
				}
			}
			if (newClient != nullptr)
				newClient->RxForward(tempBuffer,bytesNum);
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
	APISocket	*delSocket,*fromNode;
	int32		blNeedUpdate;
	
	while(disconnectThread.IsTerminated() == 0){
		SYS_SleepMS(20);
		Spin_InUse_set();
		Spin_Link_Lock();
		
		fromNode = (APISocket*)GetcgLChild(this);
		blNeedUpdate = 0;
		do{
			RTREE_RChain_Find(APISocket,fromNode,delSocket,(operateNode_t->IsConnected() == 0));
			fromNode = nullptr;
			if (delSocket != nullptr){
				fromNode = (APISocket*)GetcgRChild(delSocket);
				delSocket->PrintDisconnectInfo();
				delSocket->CloseD(0);
				RemoveNodesInRChain(delSocket,nullptr,G_LOCK_OFF);
				InsertLChild(GetcgTrash(this), delSocket);
				blNeedUpdate = 1;
			}
		}while(fromNode != nullptr);
		if (blNeedUpdate != 0){
			CleanTrash(this);
			SetblUpdate();
		}
		Spin_Link_Unlock();
		Spin_InUse_clr();
	}
	return 1;
}
//------------------------------------------------------------------------------------------//
void APISocketServer::DisconnectAll(void){
	Spin_InUse_set();
	RTREE_LChildRChain_Traversal_LINE(APISocket,this,operateNode_t->CloseD(0));
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
