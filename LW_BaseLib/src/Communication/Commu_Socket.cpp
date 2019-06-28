//
//  Commu_Com.cpp
//  SDT
//
//  Created by Leif Wen on 05/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "Commu_Socket.h"
#ifdef Commu_Socket_h
//------------------------------------------------------------------------------------------//
//#define LOGPRINT_ENABLE
//#define LOGTHREAD_ENABLE
#include "SYS_Log.h"
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_VC
bool32 CheckWSAStartup(void){
	WSAData		wsaData;
	if (WSAStartup(WINSOCK_VERSION,&wsaData) == 0)
		return G_TRUE;
	return G_FALSE;
}
#endif
//------------------------------------------------------------------------------------------//
static inline void CloseSocket(SOCKET sID){
	if (sID != INVALID_SOCKET){
		shutdown(sID,SD_BOTH);
#ifdef CommonDefH_Unix
		close(sID);
#endif
#ifdef CommonDefH_VC
		closesocket(sID);
#endif
	}
};
//------------------------------------------------------------------------------------------//
CORE_SOCKET::CORE_SOCKET(void) : COMMU_CORE(){
	osHandle = INVALID_SOCKET;
	SetSelfName("CORE_SOCKET");
	SetUpName("");
};
//------------------------------------------------------------------------------------------//
CORE_SOCKET::~CORE_SOCKET(void){
	CloseDev();
};
//------------------------------------------------------------------------------------------//
bool32 CORE_SOCKET::OpenDev_socket(const OPEN_PAR& par){
	sockaddr_in		tcpaddr;
	hostent			*lpHostEnt;
	
	SetTypePar(par.type);
	
	lpHostEnt = gethostbyname(par.name.c_str());
	if (lpHostEnt == nullptr)
		return G_FALSE;
	
	tcpaddr.sin_addr = *((struct in_addr *)lpHostEnt->h_addr);
	tcpaddr.sin_family = AF_INET;
	tcpaddr.sin_port = htons(par.port);
	
	if (par.type == OPEN_TCP){
		osHandle = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	}
	else{
		osHandle = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	}
	if (osHandle == INVALID_SOCKET)
		return G_FALSE;
	
	SetSocketBufferSize();
	
	if (connect(osHandle,(struct sockaddr *)&tcpaddr, sizeof(tcpaddr)) < 0)
		return G_FALSE;
	return(COMMU_CORE::OpenDev(par));
};
//------------------------------------------------------------------------------------------//
void CORE_SOCKET::CloseDev_socket(void){
	CloseSocket(osHandle);
	osHandle = INVALID_SOCKET;
};
//------------------------------------------------------------------------------------------//
bool32 CORE_SOCKET::OpenDev_server(const OPEN_PAR& par){
	SetTypePar(par.type);
#ifdef ODEV_System_h
	COMMU_LOGSYS::CreateODev_G3File(unitTeam->logSys,ODEV_FILE::CreateNewLOGFileName(par.name, par.port));
#endif
	SetSocketBufferSize();
	if (par.type == OPEN_TCPS)
		return(COMMU_CORE::OpenDev(par));
#ifdef CommonDefH_Unix
	cgUDPS_RemoteAddr.sin_addr.s_addr = inet_addr(par.name.c_str());
#endif
#ifdef CommonDefH_VC
	cgUDPS_RemoteAddr.sin_addr.S_un.S_addr = inet_addr(par.name.c_str());
#endif
	cgUDPS_RemoteAddr.sin_family = AF_INET;
	cgUDPS_RemoteAddr.sin_port = htons(par.port);
	return(COMMU_CORE::OpenDev(par));
};
//------------------------------------------------------------------------------------------//
void CORE_SOCKET::CloseDev_server(void){
	ODEV_FILE	*node;
	if (GetOpenPar().type == OPEN_TCPS)
		CloseDev_socket();
	
	node = COMMU_LOGSYS::GetG3DefFile(unitTeam->logSys);
	if (node != nullptr)
		node->RemoveSelf();
	osHandle = INVALID_SOCKET;
};
//------------------------------------------------------------------------------------------//
bool32 CORE_SOCKET::OpenDev(const OPEN_PAR& par){
	switch (par.type) {
		case OPEN_TCP:;
		case OPEN_UDP:;
			return(OpenDev_socket(par));
		case OPEN_TCPS:;
		case OPEN_UDPS:;
			return(OpenDev_server(par));
		default:;
	};
	return G_FALSE;
};
//------------------------------------------------------------------------------------------//
bool32 CORE_SOCKET::Open(const SOCKET& sID,const sockaddr_in& addr,uint32 type,uint32 cfg){
	STDSTR	strIP = inet_ntoa(addr.sin_addr);
	int32	port = ntohs(addr.sin_port);
	
	osHandle = sID;
	return(unitTeam->commu->Open(SetOpenPar(type,strIP,port,cfg)));
};
//------------------------------------------------------------------------------------------//
void CORE_SOCKET::CloseDev(void){
	switch (GetOpenPar().type) {
		case OPEN_TCPS:;
		case OPEN_UDPS:;
			CloseDev_server();
			break;
		case OPEN_TCP:;
		case OPEN_UDP:;
		default:;
			CloseDev_socket();
	};
	COMMU_CORE::CloseDev();
};
//------------------------------------------------------------------------------------------//
void CORE_SOCKET::SetSocketBufferSize(void){
#if 0//nouse 2019/05.12
#ifdef CommonDefH_Unix
	typedef socklen_t SizeT;
#endif
#ifdef CommonDefH_VC
	typedef int SizeT;
#endif
	int		rcvbuf;
	SizeT	rcvbufsize = sizeof(SizeT);
	if (unitTeam->bridge != nullptr){
		if(getsockopt(osHandle,SOL_SOCKET,SO_RCVBUF,(char*)&rcvbuf,&rcvbufsize) != SOCKET_ERROR){
			if(rcvbuf < (int32)COMMU_BRIDGE::GetArrayRx(unitTeam->bridge)->MaxSize())
				rcvbuf = (int32)COMMU_BRIDGE::GetArrayRx(unitTeam->bridge-)->MaxSize();
			setsockopt(osHandle,SOL_SOCKET,SO_RCVBUF,(char*)&rcvbuf,rcvbufsize);
		}
		if(getsockopt(osHandle,SOL_SOCKET,SO_SNDBUF,(char*)&rcvbuf,&rcvbufsize) != SOCKET_ERROR){
			if(rcvbuf < (int32)COMMU_BRIDGE::GetArrayTx(unitTeam->bridge-)->MaxSize())
				rcvbuf = (int32)COMMU_BRIDGE::GetArrayTx(unitTeam->bridge-)->MaxSize();//PACKAGE_MAX_SIZE;
			setsockopt(osHandle,SOL_SOCKET,SO_SNDBUF,(char*)&rcvbuf,rcvbufsize);
		}
	}
#endif
};
//------------------------------------------------------------------------------------------//
bool32 CORE_SOCKET::ReadFromDevice(uint32* retNum,uint8* buffer,uint32 length){
	int64	retCode;
	*retNum = 0;
	
	do{
		if (GetOpenPar().type != OPEN_UDPS){
			retCode = recv(osHandle,(char*)buffer,length,0);
		}
		else{
			retCode = cgArrayUDPSrx.Get(buffer, length);
			if (retCode == 0)
				break;
		}
		if ((GetOpenPar().type == OPEN_TCP) || (GetOpenPar().type == OPEN_TCPS)){
			if ((retCode < 0) && ((errno == EINTR) || (errno == EWOULDBLOCK) || (errno = EAGAIN)))
				break;
			if ((retCode == 0) && (errno == EINTR)){
				break;
			}
		}
		if ((retCode == 0) && (length == 0))
			break;
		if (retCode > 0){
			*retNum = (uint32)retCode;
			return G_TRUE;
		}
		return -1;
	}while(0);
	return G_FALSE;
};
//------------------------------------------------------------------------------------------//
bool32 CORE_SOCKET::SendToDevice(uint32* retNum,const uint8* buffer,uint32 length){
	int64		retCode = 0;
	uint32		alreadySend = 0;
	uint32		sendNum = 0;
	
	while((alreadySend < length) && IsConnected()){
		sendNum = length - alreadySend;
		if (sendNum > 1024)
			sendNum = 1024;
		if (GetOpenPar().type != OPEN_UDPS){
			retCode = send(osHandle,(char*)&buffer[alreadySend],sendNum,0);
		}
		else{
			retCode = sendto(osHandle,(char*)&buffer[alreadySend],sendNum,0,(struct sockaddr *)&cgUDPS_RemoteAddr,sizeof(cgUDPS_RemoteAddr));
		}
		
		if (retCode < 0)
			break;
		alreadySend += (uint32)retCode;
	}
	*retNum = alreadySend;
	return(!((alreadySend < length) || retCode));
};
//------------------------------------------------------------------------------------------//
void CORE_SOCKET::UDPS_recv(const uint8* buffer,uint32 num){
	cgArrayUDPSrx.InitSize(cgArrayUDPSrx.Used() + num);
	cgArrayUDPSrx.Put(buffer, num);
};
//------------------------------------------------------------------------------------------//
void CORE_SOCKET::PrintOpenSuccess(const STDSTR& strTitle){
	switch (GetOpenPar().type) {
		case OPEN_TCP:
			COMMU_CORE::PrintOpenSuccess("TCPS");
			break;
		case OPEN_UDP:
			COMMU_CORE::PrintOpenSuccess("UDPS");
			break;
		case OPEN_TCPS:;
		case OPEN_UDPS:;
			COMMU_LOGSYS::PrintMessageDot(unitTeam->logSys,"Client",this->GetDevName(),"connected");
			break;
		default:
			break;
	}
};
//------------------------------------------------------------------------------------------//
void CORE_SOCKET::PrintClose(const uint64& rxBytes,const uint64& txBytes,const uint64& fwBytes){
	if (unitTeam->commu->IsCloseDueToOS()){
		switch (GetOpenPar().type) {
			case OPEN_TCP:
				COMMU_LOGSYS::PrintConnectionReport(unitTeam->logSys,rxBytes,txBytes,fwBytes,"TCPS",GetDevName(),"connection disconnected");
				break;
			case OPEN_UDP:
				COMMU_LOGSYS::PrintConnectionReport(unitTeam->logSys,rxBytes,txBytes,fwBytes,"UDPS",GetDevName(),"connection disconnected");
				break;
			case OPEN_TCPS:
				COMMU_LOGSYS::PrintConnectionReport(unitTeam->logSys,rxBytes,txBytes,fwBytes,"Client TCP",GetDevName(),"disconnected");
				break;
			case OPEN_UDPS:
				COMMU_LOGSYS::PrintConnectionReport(unitTeam->logSys,rxBytes,txBytes,fwBytes,"Client UDP",GetDevName(),"disconnected");
			default:
				break;
		}
	}
	else{
		switch (GetOpenPar().type) {
			case OPEN_TCP:
				COMMU_LOGSYS::PrintConnectionReport(unitTeam->logSys,rxBytes,txBytes,fwBytes,"User closed TCP connection");
				break;
			case OPEN_UDP:
				COMMU_LOGSYS::PrintConnectionReport(unitTeam->logSys,rxBytes,txBytes,fwBytes,"User closed UDP connection");
				break;
			case OPEN_TCPS:
				COMMU_LOGSYS::PrintConnectionReport(unitTeam->logSys,rxBytes,txBytes,fwBytes,"Client TCP",GetDevName(),"disconnected");
				break;
			case OPEN_UDPS:
				COMMU_LOGSYS::PrintConnectionReport(unitTeam->logSys,rxBytes,txBytes,fwBytes,"Client UDP",GetDevName(),"disconnected");
			default:
				break;
		}
	}
};
//------------------------------------------------------------------------------------------//









typedef COMMU<0,COMMU_FRAME,CORE_SOCKET> S_SOCKET;
//------------------------------------------------------------------------------------------//
CORE_SOCKETSERVER::CORE_SOCKETSERVER(void) : COMMU_CORE(){
	listionSocket = INVALID_SOCKET;
	listionThread.ThreadInit(this,&CORE_SOCKETSERVER::ListionThreadFun,"listion");
	
	SetSelfName("CORE_AServer");
	SetUpName("");
};
//------------------------------------------------------------------------------------------//
CORE_SOCKETSERVER::~CORE_SOCKETSERVER(void){
	CloseDev();
	listionThread.ThreadStop();
	listionThread.RemoveSelf();
};
//------------------------------------------------------------------------------------------//
void CORE_SOCKETSERVER::Init(const COMMU_TEAM* _team){
	COMMU_UNIT::Init(_team);
	unitTeam->commu->ThreadAccept(&listionThread);
};
//------------------------------------------------------------------------------------------//
void CORE_SOCKETSERVER::SetSelfName(const STDSTR& strName){
	COMMU_CORE::SetSelfName(strName);
	listionThread.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
void CORE_SOCKETSERVER::SetUpName(const STDSTR& strName){
	COMMU_CORE::SetUpName(strName);
	listionThread.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
bool32 CORE_SOCKETSERVER::OpenDev(const OPEN_PAR& par){
	sockaddr_in		serviceAddr;
	listionSocket = INVALID_SOCKET;
	
	serviceAddr.sin_family = AF_INET;
	serviceAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serviceAddr.sin_port = htons(par.port);
	
	if (par.type == OPEN_TCPS){
		listionSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	}
	else if (par.type == OPEN_UDPS){
		listionSocket = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	}
	if (listionSocket == INVALID_SOCKET)
		return G_FALSE;
#ifdef CommonDefH_Unix
	if(bind(listionSocket,(struct sockaddr *)&serviceAddr,sizeof(serviceAddr)) < 0)
		return G_FALSE;
#endif
#ifdef CommonDefH_VC
	if(bind(listionSocket,(LPSOCKADDR)&serviceAddr,sizeof(serviceAddr)) < 0)
		return G_FALSE;
#endif
	if ((par.type == OPEN_TCPS) && (listen(listionSocket,10) < 0))
		return G_FALSE;
	return(COMMU_CORE::OpenDev(par));
};
//------------------------------------------------------------------------------------------//
void CORE_SOCKETSERVER::CloseDev(void){
	CloseSocket(listionSocket);
	listionSocket = INVALID_SOCKET;
	COMMU_CORE::CloseDev();
};
//------------------------------------------------------------------------------------------//
bool32 CORE_SOCKETSERVER::ListionThreadFun(void* _team){
	if (GetOpenPar().type == OPEN_TCPS){
		return(ListionTCP(_team));
	}
	return(ListionUDP(_team));
};
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_Unix
typedef socklen_t LEN_Scoket;
#endif
#ifdef CommonDefH_VC
typedef int LEN_Scoket;
#endif
//------------------------------------------------------------------------------------------//
bool32 CORE_SOCKETSERVER::ListionTCP(void* _team){
	COMMU_TEAM*		team = static_cast<COMMU_TEAM*>(_team);
	COMMU_FRAME*	commu = team->commu;
	sockaddr_in		ListionAddr;
	LEN_Scoket		addrlen;
	SOCKET			sID;
	S_SOCKET		*newSocket;
	CORE_SOCKET		*core;
	
	ETLogThreadStart(listionThread);
	addrlen = sizeof(ListionAddr);
	while(listionThread.IsTerminated() == G_FALSE){
		newSocket = static_cast<S_SOCKET*>(commu->GetNewNode());
		sID = accept(listionSocket,(struct sockaddr *)&ListionAddr,&addrlen);
		if (sID != INVALID_SOCKET){
			if (newSocket == nullptr){
				CloseSocket(sID);
				COMMU_LOGSYS:: PrintWarningMessageDot(team->logSys,"No enough resource to create new socket");
			}
			else{
				commu->AppendChild(newSocket);
				if (OnOpenTCPSocket(newSocket) > 0){
					core = static_cast<CORE_SOCKET*>(newSocket->Core());
					if (core->Open(sID,ListionAddr,OPEN_TCPS,cgOpenPar.cfg) > 0){
						SetblUpdate(commu);
						newSocket = nullptr;
					}
				}
				else{
					CloseSocket(sID);
				}
			}
		}
		commu->CloseChild(newSocket);
	}
	ETLogThreadStop(listionThread);
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
bool32 CORE_SOCKETSERVER::ListionUDP(void* _team){
	COMMU_TEAM*		team = static_cast<COMMU_TEAM*>(_team);
	COMMU_FRAME*	commu = team->commu;
	sockaddr_in		ListionAddr;
	LEN_Scoket		addrlen;
	
	S_SOCKET		*newSocket;
	S_SOCKET		*udpSocket;
	CORE_SOCKET		*core;
	STDSTR			strIP;
	int32			bytesNum,port;
	uint8			databuf[CORE_SOCKET::PACKAGE_MAX_SIZE];
	
	ETLogThreadStart(listionThread);
	
	addrlen = sizeof(ListionAddr);
	while(listionThread.IsTerminated() == G_FALSE){
		newSocket = nullptr;
		bytesNum = (int32)recvfrom(listionSocket,(char*)databuf,CORE_SOCKET::PACKAGE_MAX_SIZE,0,(struct sockaddr *)&ListionAddr,&addrlen);
		if (bytesNum > 0){
			strIP = inet_ntoa(ListionAddr.sin_addr);
			port = ntohs(ListionAddr.sin_port);
			
			udpSocket = static_cast<S_SOCKET*>(commu->Find(strIP,port));
			if (udpSocket != nullptr){
				core = static_cast<CORE_SOCKET*>(udpSocket->Core());
				core->UDPS_recv(databuf,bytesNum);
			}
			else {
				newSocket = static_cast<S_SOCKET*>(commu->GetNewNode());
				
				if (newSocket == nullptr){
					COMMU_LOGSYS::PrintWarningMessageDot(team->logSys,"No enough resource to create new socket");
				}
				else {
					commu->AppendChild(newSocket);
					if (OnOpenUDPSocket(newSocket) > 0){
						core = static_cast<CORE_SOCKET*>(newSocket->Core());
						if (core->Open(listionSocket,ListionAddr,OPEN_UDPS,cgOpenPar.cfg) > 0){
							SetblUpdate(commu);
							core->UDPS_recv(databuf,bytesNum);
							newSocket = nullptr;
						}
					}
				}
				commu->CloseChild(newSocket);
			}
		}
	}
	ETLogThreadStop(listionThread);
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
void CORE_SOCKETSERVER::PrintOpenSuccess(const STDSTR& strTitle){
	if (GetOpenPar().type == OPEN_TCPS){
		COMMU_LOGSYS::PrintMessageDot(unitTeam->logSys,"Successfully setup TCP Server, listion Port is", Str_ToStr(GetOpenPar().port));
	}
	else{
		COMMU_LOGSYS::PrintMessageDot(unitTeam->logSys,"Successfully setup UDP Server, listion Port is", Str_ToStr(GetOpenPar().port));
	}
};
//------------------------------------------------------------------------------------------//
void CORE_SOCKETSERVER::PrintOpenFail(const STDSTR& strTitle){
	if (GetOpenPar().type == OPEN_TCPS){
		COMMU_LOGSYS::PrintMessageDot(unitTeam->logSys,"Failed setup TCP Server,", "please try again");
	}
	else{
		COMMU_LOGSYS::PrintMessageDot(unitTeam->logSys,"Failed setup UDP Server,", "please try again");
	}
};
//------------------------------------------------------------------------------------------//
void CORE_SOCKETSERVER::PrintClose(const uint64& rxBytes,const uint64& txBytes,const uint64& fwBytes){
	COMMU_LOGSYS::PrintMessageDot(unitTeam->logSys,"User stopped the server");
};
//------------------------------------------------------------------------------------------//
bool32 CORE_SOCKETSERVER::OnOpenTCPSocket	(COMMU_FRAME* newSocket)	{return G_TRUE;};
bool32 CORE_SOCKETSERVER::OnOpenUDPSocket	(COMMU_FRAME* newSocket)	{return(OnOpenTCPSocket(newSocket));};
//------------------------------------------------------------------------------------------//
#endif /* Commu_Com_h */
