/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: Commu_Socket.cpp
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
#include "Commu_Socket.h"
#include "Comm_Convert.h"
#include "ODEV_Include.h"
#include "SMC.h"

#define LOGPRINT_ENABLE
#define LOGPRINT_ENABLE2
#include "Comm_Log.h"
//------------------------------------------------------------------------------------------//
#ifdef Commu_SocketH
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_VC
int32 CheckWSAStartup(void){
	WSAData		wsaData;
	if (WSAStartup(WINSOCK_VERSION,&wsaData) == 0)
		return 1;
	return 0;
}
#endif
//------------------------------------------------------------------------------------------//
void CloseSocket(SOCKET sID){
	if (sID != INVALID_SOCKET){
		shutdown(sID,SD_BOTH);
#ifdef CommonDefH_Unix
		close(sID);
#endif
#ifdef CommonDefH_VC
		closesocket(sID);
#endif
	}
}
//------------------------------------------------------------------------------------------//
void ASOCKET::Init(void){
	osHandle = INVALID_SOCKET;
}
//------------------------------------------------------------------------------------------//
int32 ASOCKET::Socket_OpenDev(const STDSTR &tCDBufName,int32 tCDBufPar,CSType tCSType){
	sockaddr_in		tcpaddr;
	hostent			*lpHostEnt;

	SetCSType(tCSType);
	
	lpHostEnt = gethostbyname(tCDBufName.c_str());
	if (lpHostEnt == nullptr)
		return 0;
	
	tcpaddr.sin_addr = *((struct in_addr *)lpHostEnt->h_addr);
	tcpaddr.sin_family = AF_INET;
	tcpaddr.sin_port = htons(tCDBufPar);
	
	if (tCSType == CSType_TCP){
		osHandle = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	}
	else{
		osHandle = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	}
	if (osHandle == INVALID_SOCKET)
		return 0;
	
	SetSocketBufferSize();
	
	if (connect(osHandle,(struct sockaddr *)&tcpaddr, sizeof(tcpaddr)) == SOCKET_ERROR)
		return 0;
	rxThread.Enable();
	return 1;
}
//------------------------------------------------------------------------------------------//
void ASOCKET::Socket_CloseDev(void){
	CloseSocket(osHandle);
	osHandle = INVALID_SOCKET;
}
//------------------------------------------------------------------------------------------//
int32 ASOCKET::Server_OpenDev(const STDSTR &tCDBufName,int32 tCDBufPar,CSType tCSType){
	SetCSType(tCSType);
	CreateODev_G3File(ODEV_FILE::CreateNewLOGFileName(tCDBufName, tCDBufPar));
	if (tCSType == CSType_TCPS){
		SetSocketBufferSize();
		rxThread.Enable();
		return 1;
	}
	rxThread.Disable();
#ifdef CommonDefH_Unix
	cgUDPS_RemoteAddr.sin_addr.s_addr = inet_addr(tCDBufName.c_str());
#endif
#ifdef CommonDefH_VC
	cgUDPS_RemoteAddr.sin_addr.S_un.S_addr = inet_addr(tCDBufName.c_str());
#endif
	cgUDPS_RemoteAddr.sin_family = AF_INET;
	cgUDPS_RemoteAddr.sin_port = htons(tCDBufPar);
	return 1;
}
//------------------------------------------------------------------------------------------//
void ASOCKET::Server_CloseDev(void){
	if (GetCSType() == CSType_TCPS)
		Socket_CloseDev();
	osHandle = INVALID_SOCKET;
}
//------------------------------------------------------------------------------------------//
int32 ASOCKET::OpenDev(const STDSTR &tCDBufName,int32 tCDBufPar,CSType tCSType,int32 blEnEcho){
	switch (tCSType) {
		case CSType_TCP:;
		case CSType_UDP:;
			return(Socket_OpenDev(tCDBufName,tCDBufPar,tCSType));
		case CSType_TCPS:;
		case CSType_UDPS:;
			return(Server_OpenDev(tCDBufName,tCDBufPar,tCSType));
		default:;
	};
	return 0;
}
//------------------------------------------------------------------------------------------//
int32 ASOCKET::Open(const SOCKET &sID,const sockaddr_in &addr,CSType tCSType,int32 blEnEcho){
	int32	ret;
	STDSTR	strIP;
	int32	port;
	ret = 0;
	if (InDoing_try() != 0){
		osHandle = sID;
		strIP = inet_ntoa(addr.sin_addr);
		port = ntohs(addr.sin_port);
		ret = OpenD(strIP,port,tCSType,blEnEcho,G_LOCK_OFF);
		InDoing_clr();
	}
	return(ret);
};
//------------------------------------------------------------------------------------------//
void ASOCKET::CloseDev(void){
	switch (GetCSType()) {
		case CSType_TCPS:;
		case CSType_UDPS:;
			Server_CloseDev();
			break;
		case CSType_TCP:;
		case CSType_UDP:;
		default:;
			Socket_CloseDev();
	};
	COMMU_DBUF::CloseDev();
}
//------------------------------------------------------------------------------------------//
void ASOCKET::SetSocketBufferSize(void){
	int	rcvbuf;
#ifdef CommonDefH_Unix
	socklen_t	rcvbufsize;
	rcvbufsize = sizeof(socklen_t);
#endif
#ifdef CommonDefH_VC
	int		rcvbufsize;
	rcvbufsize = sizeof(int);
#endif
	if(getsockopt(osHandle,SOL_SOCKET,SO_RCVBUF,(char*)&rcvbuf,&rcvbufsize) != SOCKET_ERROR){
		if(rcvbuf < (int32)cgRxSBUF.BufferMaxSize())
			rcvbuf = (int32)cgRxSBUF.BufferMaxSize();
		setsockopt(osHandle,SOL_SOCKET,SO_RCVBUF,(char*)&rcvbuf,rcvbufsize);
	}
	if(getsockopt(osHandle,SOL_SOCKET,SO_SNDBUF,(char*)&rcvbuf,&rcvbufsize) != SOCKET_ERROR){
		if(rcvbuf < (int32)cgTxSBUF.BufferMaxSize())
			rcvbuf = (int32)cgTxSBUF.BufferMaxSize();//PACKAGE_MAX_SIZE;
		setsockopt(osHandle,SOL_SOCKET,SO_SNDBUF,(char*)&rcvbuf,rcvbufsize);
	}
}
//------------------------------------------------------------------------------------------//
int32 ASOCKET::ReadFromDevice(uint32 *retNum,uint8 *buffer,uint32 length){
	int64		retCode;
	*retNum = 0;
	
	retCode = recv(osHandle,(char*)buffer,length,0);
	if ((GetCSType() == CSType_TCP) || (GetCSType() == CSType_TCPS)){
		if (((retCode == SOCKET_ERROR) && (errno != EINTR) && (errno != EWOULDBLOCK) && (errno != EAGAIN))
			|| ((retCode == 0) && (errno != EINTR))){
			return -1;
		}
	}
	if (retCode > 0){
		*retNum = (uint32)retCode;
		return 1;
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
int32 ASOCKET::SendToDevice(uint32 *retNum,const uint8 *buffer,uint32 length){
	int64		retCode;
	uint32		i;
	if (GetCSType() == CSType_UDPS)
		return(UDPS_SendToDevice(retNum,buffer,length));
	
	i = 0;
	while((i < length) && (IsConnected() != 0)){
		retCode = send(osHandle,(char*)&buffer[i],length - i,0);
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
int32 ASOCKET::UDPS_SendToDevice(uint32 *retNum,const uint8 *buffer,uint32 length){
	int64		retCode;
	uint32		i;
	
	i = 0;
	while((i < length) && (IsConnected() != 0)){
		retCode = sendto(osHandle,(char*)&buffer[i],length - i,0,(struct sockaddr *)&cgUDPS_RemoteAddr,sizeof(cgUDPS_RemoteAddr));
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
void ASOCKET::UDPS_ReadFromDevice(const uint8 *buffer,uint32 num){
	uint32 sendnum;
	
	do{
		sendnum = ReceiveFifo()->Put(buffer,num);
		cgRxBytes += sendnum;
		AfterReadFromDevice();
		num -= sendnum;
		buffer += sendnum;
	}while(num > 0);
}
//------------------------------------------------------------------------------------------//
void ASOCKET::DoPrintOnOpenSuccess(void){
	switch (GetCSType()) {
		case CSType_TCP:
			PrintOpenSuccessReport("TCP");
			break;
		case CSType_UDP:
			PrintOpenSuccessReport("UDP");
			break;
		case CSType_TCPS:;
		case CSType_UDPS:;
			PrintNormalMessage("Client",GetDevName(),"connected");
			break;
		default:
			break;
	}
}
//------------------------------------------------------------------------------------------//
void ASOCKET::DoPrintOnClose(void){
	if (ChkblCloseDueToOS() != 0){
		switch (GetCSType()) {
			case CSType_TCP:
				PrintConnectionReport(cgAttrTitle,"TCP",GetDevName(),"connection disconnected");
				break;
			case CSType_UDP:
				PrintConnectionReport(cgAttrTitle,"UDP",GetDevName(),"connection disconnected");
				break;
			case CSType_TCPS:
				PrintConnectionReport(cgAttrTitle,"Client TCP",GetDevName(),"disconnected");
				break;
			case CSType_UDPS:
				PrintConnectionReport(cgAttrTitle,"Client UDP",GetDevName(),"disconnected");
			default:
				break;
		}
	}
	else{
		switch (GetCSType()) {
			case CSType_TCP:
				PrintConnectionReport("User closed",cgAttrTitle,"TCP connection");
				break;
			case CSType_UDP:
				PrintConnectionReport("User closed",cgAttrTitle,"UDP connection");
				break;
			case CSType_TCPS:
				PrintConnectionReport(cgAttrTitle,"Client TCP",GetDevName(),"disconnected");
				break;
			case CSType_UDPS:
				PrintConnectionReport(cgAttrTitle,"Client UDP",GetDevName(),"disconnected");
			default:
				break;
		}
	}
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//









//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
void ASOCKETSERVER::Init(uint32 tSize){
	listionSocket = INVALID_SOCKET;
	listionThread.ThreadInit(this,&ASOCKETSERVER::ListionThreadFun,"listion");
	
	cgThreadList  < listionThread;
	
	cgBufMaxSize = tSize;
	CreateTrash(this);
}
//------------------------------------------------------------------------------------------//
ASOCKETSERVER::~ASOCKETSERVER(void){
	CloseD();
	listionThread.ThreadStop();
	listionThread.RemoveSelf();
};
//------------------------------------------------------------------------------------------//
void ASOCKETSERVER::ChildSetSel(DBUFFER *sdb){
	if ((sdb != nullptr) && (sdb != this)){
		Spin_InUse_set();
		if ((GetSelDB() != this) && (GetSelDB() != sdb))
			static_cast<COMMU_DBUF_FRAME_FW*>(GetSelDB())->UseSelfFwSBufList();
		static_cast<COMMU_DBUF_FRAME_FW*>(sdb)->UseExternalFwSBufList(GetFwSBufList());
		Spin_InUse_clr();
		COMMU_DBUF_FRAME_FW::ChildSetSel(sdb);
	}
};
//------------------------------------------------------------------------------------------//
void ASOCKETSERVER::ChildClrSel(DBUFFER *sdb){
	if ((sdb != nullptr) && (sdb != this))
		static_cast<COMMU_DBUF_FRAME_FW*>(sdb)->UseSelfFwSBufList();
	COMMU_DBUF_FRAME_FW::ChildClrSel(sdb);
};
//------------------------------------------------------------------------------------------//
void ASOCKETSERVER::EnableEcho(void){
	COMMU_DBUF_FRAME_FW::EnableEcho();
	TREE_LChildRChain_Traversal_LINE(ASOCKET, this, operateNode_t->EnableEcho(););
};
//------------------------------------------------------------------------------------------//
void ASOCKETSERVER::DisableEcho(void){
	COMMU_DBUF_FRAME_FW::DisableEcho();
	TREE_LChildRChain_Traversal_LINE(ASOCKET, this, operateNode_t->DisableEcho(););
};
//------------------------------------------------------------------------------------------//
int32 ASOCKETSERVER::OpenDev(const STDSTR &tCDBufName,int32 tCDBufPar,CSType tCSType,int32 blEnEcho){
	sockaddr_in		serviceAddr;
	listionSocket = INVALID_SOCKET;
	
	serviceAddr.sin_family = AF_INET;
	serviceAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serviceAddr.sin_port = htons(tCDBufPar);
	
	if (tCSType == CSType_TCPS){
		listionSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	}
	else if (tCSType == CSType_UDPS){
		listionSocket = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	}
	if (listionSocket == INVALID_SOCKET)
		return 0;
#ifdef CommonDefH_Unix
	if(bind(listionSocket,(struct sockaddr *)&serviceAddr,sizeof(serviceAddr)) == SOCKET_ERROR)
		return 0;
#endif
#ifdef CommonDefH_VC
	if(bind(listionSocket,(LPSOCKADDR)&serviceAddr,sizeof(serviceAddr)) == SOCKET_ERROR)
		return 0;
#endif
	if ((tCSType == CSType_TCPS) && (listen(listionSocket,10) == SOCKET_ERROR))
		return 0;
	return 1;
}
//------------------------------------------------------------------------------------------//
void ASOCKETSERVER::CloseDev(void){
	CloseSocket(listionSocket);
	listionSocket = INVALID_SOCKET;
}
//------------------------------------------------------------------------------------------//
void ASOCKETSERVER::DoClose(void){
	ELog(this << "ASOCKETSERVER::DoClose()");
	COMMU_DBUF_FRAME_FW::DoClose();
	ChildClrSel(GetSelDB());
	DestroyAll();
	SetblUpdate();
}
//------------------------------------------------------------------------------------------//
int32 ASOCKETSERVER::ListionThreadFun(void *p){
	if (GetCSType() == CSType_TCPS){
		return(ListionTCP(p));
	}
	return(ListionUDP(p));
}
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_Unix
typedef socklen_t LEN_Scoket;
#endif
#ifdef CommonDefH_VC
typedef int LEN_Scoket;
#endif
//------------------------------------------------------------------------------------------//
int32 ASOCKETSERVER::ListionTCP(void *p){
	sockaddr_in		ListionAddr;
	LEN_Scoket		addrlen;
	SOCKET			sID;
	ASOCKET			*newSocket;
	
	ELog(this << "ListionTCP()::Running");
	addrlen = sizeof(ListionAddr);
	while(listionThread.IsTerminated() == 0){
		newSocket = static_cast<ASOCKET*>(GetNewNode());
		sID = accept(listionSocket,(struct sockaddr *)&ListionAddr,&addrlen);
		if (sID != INVALID_SOCKET){
			if (newSocket == nullptr){
				CloseSocket(sID);
				PrintWarningMessage("No enough resource to create new socket");
			}
			else{
				AddSon(newSocket);
				if (OnOpenTCPSocket(newSocket) > 0){
					if (newSocket->Open(sID,ListionAddr,CSType_TCPS,CheckEcho()) > 0){
						SetblUpdate();
						newSocket = nullptr;
					}
				}
				else{
					CloseSocket(sID);
				}
			}
		}
		ChildClose(newSocket);
	}
	ELog(this << "ListionTCP()::Stop");
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 ASOCKETSERVER::ListionUDP(void *p){
	sockaddr_in		ListionAddr;
	LEN_Scoket		addrlen;
	
	ASOCKET		*newSocket;
	ASOCKET		*udpSocket;
	uint8		*databuf;
	STDSTR		strIP;
	int32		bytesNum,port;
	
	ELog(this << "ListionUDP()::Running");
	cgRxSBUF.Init(PACKAGE_MAX_SIZE);
	databuf = cgRxSBUF.cgFIFO.GetPointer(0);
	
	addrlen = sizeof(ListionAddr);
	while(listionThread.IsTerminated() == 0){
		newSocket = static_cast<ASOCKET*>(GetNewNode());
		bytesNum = (int32)recvfrom(listionSocket,(char*)databuf,cgBufMaxSize,0,(struct sockaddr *)&ListionAddr,&addrlen);
		if ((bytesNum != SOCKET_ERROR) && (bytesNum > 0)){
			strIP = inet_ntoa(ListionAddr.sin_addr);
			port = ntohs(ListionAddr.sin_port);
			
			udpSocket = static_cast<ASOCKET*>(Find(strIP,port));
			if (udpSocket != nullptr){
				udpSocket->UDPS_ReadFromDevice(databuf,bytesNum);
			}
			else if (newSocket == nullptr){
				PrintWarningMessage("No enough resource to create new socket");
			}
			else {
				AddSon(newSocket);
				if ((OnOpenUDPSocket(newSocket) > 0) && (newSocket->Open(listionSocket,ListionAddr,CSType_UDPS,CheckEcho()) > 0)){
					SetblUpdate();
					newSocket->UDPS_ReadFromDevice(databuf,bytesNum);
					newSocket = nullptr;
				}
			}
		}
		ChildClose(newSocket);
	}
	ELog(this << "ListionUDP()::Stop");
	return 1;
}
//------------------------------------------------------------------------------------------//
void ASOCKETSERVER::DoPrintOnOpenSuccess(void){
	if (GetCSType() == CSType_TCPS){
		PrintNormalMessage("Successfully setup TCP Server, listion Port is", Str_ToString(GetBufPar()));
	}
	else{
		PrintNormalMessage("Successfully setup UDP Server, listion Port is", Str_ToString(GetBufPar()));
	}
}
//------------------------------------------------------------------------------------------//
void ASOCKETSERVER::DoPrintOnOpenFail(void){
	if (GetCSType() == CSType_TCPS){
		PrintWarningMessage("Failed setup TCP Server,", "please try again");
	}
	else{
		PrintWarningMessage("Failed setup UDP Server,", "please try again");
	}
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif

