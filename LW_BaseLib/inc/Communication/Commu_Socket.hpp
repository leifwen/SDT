//
//  Commu_Socket.hpp
//  SDT
//
//  Created by Leif Wen on 05/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#ifndef Commu_Socket_hpp
#define Commu_Socket_hpp
//------------------------------------------------------------------------------------------//
#include "Commu_Socket.h"
#ifdef Commu_Socket_h
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_VC
static inline bool32 CheckWSAStartup(void){
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
}
//------------------------------------------------------------------------------------------//
template <typename T_COMMU> COMMU_SOCKET<T_COMMU>::COMMU_SOCKET(uint32 size,const ODEV_SYSTEM* logSys) : T_COMMU(size,logSys){
	osHandle = INVALID_SOCKET;
	TNFP::SetSelfName("SOCKET");
	this->SetSelfName(this->selfName);
	this->SetFatherName("");
};
//------------------------------------------------------------------------------------------//
template <typename T_COMMU> COMMU_SOCKET<T_COMMU>::~COMMU_SOCKET(void){
	this->Close();
};
//------------------------------------------------------------------------------------------//
template <typename T_COMMU> bool32 COMMU_SOCKET<T_COMMU>::Socket_OpenDev(const OPEN_PAR& par){
	sockaddr_in		tcpaddr;
	hostent			*lpHostEnt;
	
	this->SetTypePar(par.type);
	
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
	this->rxThread.Enable();
	return G_TRUE;
}
//------------------------------------------------------------------------------------------//
template <typename T_COMMU> void COMMU_SOCKET<T_COMMU>::Socket_CloseDev(void){
	CloseSocket(osHandle);
	osHandle = INVALID_SOCKET;
}
//------------------------------------------------------------------------------------------//
template <typename T_COMMU> bool32 COMMU_SOCKET<T_COMMU>::Server_OpenDev(const OPEN_PAR& par){
	this->SetTypePar(par.type);
#ifdef ODEV_FILE_h
	this->CreateODev_G3File(ODEV_FILE::CreateNewLOGFileName(par.name, par.port));
#endif
	if (par.type == OPEN_TCPS){
		SetSocketBufferSize();
		this->rxThread.Enable();
		return G_TRUE;
	}
	this->rxThread.Disable();
#ifdef CommonDefH_Unix
	cgUDPS_RemoteAddr.sin_addr.s_addr = inet_addr(par.name.c_str());
#endif
#ifdef CommonDefH_VC
	cgUDPS_RemoteAddr.sin_addr.S_un.S_addr = inet_addr(par.name.c_str());
#endif
	cgUDPS_RemoteAddr.sin_family = AF_INET;
	cgUDPS_RemoteAddr.sin_port = htons(par.port);
	return G_TRUE;
}
//------------------------------------------------------------------------------------------//
template <typename T_COMMU> void COMMU_SOCKET<T_COMMU>::Server_CloseDev(void){
	if (this->GetOpenPar().type == OPEN_TCPS)
		Socket_CloseDev();
	osHandle = INVALID_SOCKET;
}
//------------------------------------------------------------------------------------------//
template <typename T_COMMU> bool32 COMMU_SOCKET<T_COMMU>::OpenDev(const OPEN_PAR& par){
	switch (par.type) {
		case OPEN_TCP:;
		case OPEN_UDP:;
			return(Socket_OpenDev(par));
		case OPEN_TCPS:;
		case OPEN_UDPS:;
			return(Server_OpenDev(par));
		default:;
	};
	return G_FALSE;
}
//------------------------------------------------------------------------------------------//
template <typename T_COMMU> bool32 COMMU_SOCKET<T_COMMU>::OpenD(const SOCKET& sID,const sockaddr_in& addr,uint32 type,uint32 cfg){
	int32	ret;
	STDSTR	strIP;
	int32	port;
	ret = 0;
	if (this->InDoing_try()){
		osHandle = sID;
		strIP = inet_ntoa(addr.sin_addr);
		port = ntohs(addr.sin_port);
		ret = this->Open(SetOpenPar(type,strIP,port,cfg),G_LOCK_OFF);
		this->InDoing_clr();
	}
	return(ret);
};
//------------------------------------------------------------------------------------------//
template <typename T_COMMU> void COMMU_SOCKET<T_COMMU>::CloseDev(void){
	switch (this->GetOpenPar().type) {
		case OPEN_TCPS:;
		case OPEN_UDPS:;
			Server_CloseDev();
			break;
		case OPEN_TCP:;
		case OPEN_UDP:;
		default:;
			Socket_CloseDev();
	};
	T_COMMU::CloseDev();
}
//------------------------------------------------------------------------------------------//
template <typename T_COMMU> void COMMU_SOCKET<T_COMMU>::SetSocketBufferSize(void){
	/*
#ifdef CommonDefH_Unix
	typedef socklen_t SizeT;
#endif
#ifdef CommonDefH_VC
	typedef int SizeT;
#endif
	int		rcvbuf;
	SizeT	rcvbufsize = sizeof(SizeT);
	
	if(getsockopt(osHandle,SOL_SOCKET,SO_RCVBUF,(char*)&rcvbuf,&rcvbufsize) != SOCKET_ERROR){
		if(rcvbuf < (int32)this->cgRxSBUF.MaxSize())
			rcvbuf = (int32)this->cgRxSBUF.MaxSize();
		setsockopt(osHandle,SOL_SOCKET,SO_RCVBUF,(char*)&rcvbuf,rcvbufsize);
	}
	if(getsockopt(osHandle,SOL_SOCKET,SO_SNDBUF,(char*)&rcvbuf,&rcvbufsize) != SOCKET_ERROR){
		if(rcvbuf < (int32)this->cgTxSBUF.MaxSize())
			rcvbuf = (int32)this->cgTxSBUF.MaxSize();//PACKAGE_MAX_SIZE;
		setsockopt(osHandle,SOL_SOCKET,SO_SNDBUF,(char*)&rcvbuf,rcvbufsize);
	}
	 */
}
//------------------------------------------------------------------------------------------//
template <typename T_COMMU> bool32 COMMU_SOCKET<T_COMMU>::ReadFromDevice(uint32* retNum,uint8* buffer,uint32 length){
	int64	retCode;
	*retNum = 0;
	
	do{
		retCode = recv(osHandle,(char*)buffer,length,0);
		if ((this->GetOpenPar().type == OPEN_TCP) || (this->GetOpenPar().type == OPEN_TCPS)){
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
}
//------------------------------------------------------------------------------------------//
template <typename T_COMMU> bool32 COMMU_SOCKET<T_COMMU>::SendToDevice(uint32* retNum,const uint8* buffer,uint32 length){
	int64		retCode = 0;
	uint32		alreadySend;
	
	if (this->GetOpenPar().type == OPEN_UDPS)
		return(UDPS_SendToDevice(retNum,buffer,length));
	
	alreadySend = 0;
	while((alreadySend < length) && this->IsConnected()){
		retCode = send(osHandle,(char*)&buffer[alreadySend],length - alreadySend,0);
		if (retCode < 0)
			break;
		alreadySend += (uint32)retCode;
	}
	*retNum = alreadySend;
	return(!((alreadySend < length) || retCode));
}
//------------------------------------------------------------------------------------------//
template <typename T_COMMU> bool32 COMMU_SOCKET<T_COMMU>::UDPS_SendToDevice(uint32* retNum,const uint8* buffer,uint32 length){
	int64		retCode = 0;
	uint32		alreadySend;
	
	alreadySend = 0;
	while((alreadySend < length) && this->IsConnected()){
		retCode = sendto(osHandle,(char*)&buffer[alreadySend],length - alreadySend,0,(struct sockaddr *)&cgUDPS_RemoteAddr,sizeof(cgUDPS_RemoteAddr));
		if (retCode < 0)
			break;
		alreadySend += (uint32)retCode;
	}
	*retNum = alreadySend;
	return(!((alreadySend < length) || retCode));
}
//------------------------------------------------------------------------------------------//
template <typename T_COMMU> void COMMU_SOCKET<T_COMMU>::UDPS_ReadFromDevice(const uint8* buffer,uint32 num){
	uint32 sendnum;
	
	do{
		sendnum = this->cgArrayIn->Put(buffer,num);
		this->cgRxBytes += sendnum;
		this->AfterReadFromDevice();
		num -= sendnum;
		buffer += sendnum;
	}while(num > 0);
}
//------------------------------------------------------------------------------------------//
template <typename T_COMMU> void COMMU_SOCKET<T_COMMU>::DoPrintOnOpenSuccess(void){
	switch (this->GetOpenPar().type) {
		case OPEN_TCP:
			this->PrintOpenSuccessReport("TCP");
			break;
		case OPEN_UDP:
			this->PrintOpenSuccessReport("UDP");
			break;
		case OPEN_TCPS:;
		case OPEN_UDPS:;
			this->PrintMessageDot("Client",this->GetDevName(),"connected");
			break;
		default:
			break;
	}
}
//------------------------------------------------------------------------------------------//
template <typename T_COMMU> void COMMU_SOCKET<T_COMMU>::DoPrintOnClose(void){
	if (this->CheckSFlag(COMMU_SOCKET<T_COMMU>::CF_blCloseDueToOS)){
		switch (this->GetOpenPar().type) {
			case OPEN_TCP:
				this->PrintConnectionReport(this->cgAttrTitle,"TCP",this->GetDevName(),"connection disconnected");
				break;
			case OPEN_UDP:
				this->PrintConnectionReport(this->cgAttrTitle,"UDP",this->GetDevName(),"connection disconnected");
				break;
			case OPEN_TCPS:
				this->PrintConnectionReport(this->cgAttrTitle,"Client TCP",this->GetDevName(),"disconnected");
				break;
			case OPEN_UDPS:
				this->PrintConnectionReport(this->cgAttrTitle,"Client UDP",this->GetDevName(),"disconnected");
			default:
				break;
		}
	}
	else{
		switch (this->GetOpenPar().type) {
			case OPEN_TCP:
				this->PrintConnectionReport("User closed",this->cgAttrTitle,"TCP connection");
				break;
			case OPEN_UDP:
				this->PrintConnectionReport("User closed",this->cgAttrTitle,"UDP connection");
				break;
			case OPEN_TCPS:
				this->PrintConnectionReport(this->cgAttrTitle,"Client TCP",this->GetDevName(),"disconnected");
				break;
			case OPEN_UDPS:
				this->PrintConnectionReport(this->cgAttrTitle,"Client UDP",this->GetDevName(),"disconnected");
			default:
				break;
		}
	}
}
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
template <typename T_COMMU>
COMMU_SOCKETSERVER<T_COMMU>::COMMU_SOCKETSERVER(uint32 size,const ODEV_SYSTEM* logSys) : COMMU_FRAME_LOGSYS(size,logSys){
	SetSFlag(CF_blNoInitSize);
	
	listionSocket = INVALID_SOCKET;
	listionThread.ThreadInit(this,&COMMU_SOCKETSERVER::ListionThreadFun,"listion");
	
	cgThreadList  < listionThread;
	
	TNFP::SetSelfName("AServer");
	SetSelfName(selfName);
	SetFatherName("");
};
//------------------------------------------------------------------------------------------//
template <typename T_COMMU>COMMU_SOCKETSERVER<T_COMMU>::~COMMU_SOCKETSERVER(void){
	Close();
	listionThread.ThreadStop();
	listionThread.RemoveSelf();
};
//------------------------------------------------------------------------------------------//
template <typename T_COMMU> void COMMU_SOCKETSERVER<T_COMMU>::SetSelfName(const STDSTR& strName){
	COMMU_FRAME_LOGSYS::SetSelfName(strName);
	listionThread.SetFatherName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
template <typename T_COMMU> void COMMU_SOCKETSERVER<T_COMMU>::SetFatherName(const STDSTR& strName){
	COMMU_FRAME_LOGSYS::SetFatherName(strName);
	listionThread.SetFatherName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
template <typename T_COMMU> void COMMU_SOCKETSERVER<T_COMMU>::ChildSetSel(DBUF* sdb){
	if ((sdb != nullptr) && (sdb != this)){
		InUse_set();
		if ((GetSelDB() != this) && (GetSelDB() != sdb))
			static_cast<COMMU_FRAME_LOGSYS*>(GetSelDB())->UseSelfFwSBufList();
		static_cast<COMMU_FRAME_LOGSYS*>(sdb)->UseExternalFwSBufList(GetFwSBufList());
		InUse_clr();
		COMMU_FRAME_LOGSYS::ChildSetSel(sdb);
	}
};
//------------------------------------------------------------------------------------------//
template <typename T_COMMU> void COMMU_SOCKETSERVER<T_COMMU>::ChildClrSel(DBUF* sdb){
	if ((sdb != nullptr) && (sdb != this))
		static_cast<COMMU_FRAME_LOGSYS*>(sdb)->UseSelfFwSBufList();
	COMMU_FRAME_LOGSYS::ChildClrSel(sdb);
};
//------------------------------------------------------------------------------------------//
template <typename T_COMMU> void COMMU_SOCKETSERVER<T_COMMU>::EnableEcho(void){
	COMMU_FRAME_LOGSYS::EnableEcho();
	TREE_LChildRChain_Traversal_LINE(T_COMMU, this, _opNode->EnableEcho(););
};
//------------------------------------------------------------------------------------------//
template <typename T_COMMU> void COMMU_SOCKETSERVER<T_COMMU>::DisableEcho(void){
	COMMU_FRAME_LOGSYS::DisableEcho();
	TREE_LChildRChain_Traversal_LINE(T_COMMU, this, _opNode->DisableEcho(););
};
//------------------------------------------------------------------------------------------//
template <typename T_COMMU> bool32 COMMU_SOCKETSERVER<T_COMMU>::OpenDev(const OPEN_PAR& par){
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
	return G_TRUE;
}
//------------------------------------------------------------------------------------------//
template <typename T_COMMU> void COMMU_SOCKETSERVER<T_COMMU>::CloseDev(void){
	CloseSocket(listionSocket);
	listionSocket = INVALID_SOCKET;
}
//------------------------------------------------------------------------------------------//
template <typename T_COMMU> void COMMU_SOCKETSERVER<T_COMMU>::DoClose(void){
//	ELog(this << "ASOCKETSERVER::DoClose()");
	COMMU_FRAME_LOGSYS::DoClose();
	ChildClrSel(GetSelDB());
	CleanChild(this, this);
	CleanTrash(this);
	SetblUpdate();
}
//------------------------------------------------------------------------------------------//
template <typename T_COMMU> bool32 COMMU_SOCKETSERVER<T_COMMU>::ListionThreadFun(void* commu){
	if (GetOpenPar().type == OPEN_TCPS){
		return(ListionTCP(commu));
	}
	return(ListionUDP(commu));
}
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_Unix
	typedef socklen_t LEN_Scoket;
#endif
#ifdef CommonDefH_VC
	typedef int LEN_Scoket;
#endif
//------------------------------------------------------------------------------------------//
template <typename T_COMMU> bool32 COMMU_SOCKETSERVER<T_COMMU>::ListionTCP(void* commu){
	sockaddr_in		ListionAddr;
	LEN_Scoket		addrlen;
	SOCKET			sID;
	T_COMMU			*newSocket;
	
//	ETLogThreadStart(listionThread);
	addrlen = sizeof(ListionAddr);
	while(listionThread.IsTerminated() == G_FALSE){
		newSocket = static_cast<T_COMMU*>(GetNewNode());
		sID = accept(listionSocket,(struct sockaddr *)&ListionAddr,&addrlen);
		if (sID != INVALID_SOCKET){
			if (newSocket == nullptr){
				CloseSocket(sID);
				PrintWarningMessageDot("No enough resource to create new socket");
			}
			else{
				AddChild(newSocket);
				if (OnOpenTCPSocket(newSocket) > 0){
					if (newSocket->OpenD(sID,ListionAddr,OPEN_TCPS,cgOpenPar.cfg) > 0){
						SetblUpdate();
						newSocket = nullptr;
					}
				}
				else{
					CloseSocket(sID);
				}
			}
		}
		CloseChild(newSocket);
	}
//	ETLogThreadStop(listionThread);
	return G_TRUE;
}
//------------------------------------------------------------------------------------------//
template <typename T_COMMU> bool32 COMMU_SOCKETSERVER<T_COMMU>::ListionUDP(void* commu){
	sockaddr_in		ListionAddr;
	LEN_Scoket		addrlen;
	
	T_COMMU		*newSocket;
	T_COMMU		*udpSocket;
	uint8		*databuf;
	STDSTR		strIP;
	int32		bytesNum,port;
	
//	ETLogThreadStart(listionThread);
	cgRxSBUF.InitSize(PACKAGE_MAX_SIZE);
	databuf = cgRxSBUF.GetArray().GetPointer(0);
	
	addrlen = sizeof(ListionAddr);
	while(listionThread.IsTerminated() == G_FALSE){
		newSocket = nullptr;
		bytesNum = (int32)recvfrom(listionSocket,(char*)databuf,cgMaxSize,0,(struct sockaddr *)&ListionAddr,&addrlen);
		if (bytesNum > 0){
			strIP = inet_ntoa(ListionAddr.sin_addr);
			port = ntohs(ListionAddr.sin_port);
			
			udpSocket = static_cast<T_COMMU*>(Find(strIP,port));
			if (udpSocket != nullptr){
				udpSocket->UDPS_ReadFromDevice(databuf,bytesNum);
			}
			else {
				newSocket = static_cast<T_COMMU*>(GetNewNode());
				
				if (newSocket == nullptr){
					PrintWarningMessageDot("No enough resource to create new socket");
				}
				else {
					AddChild(newSocket);
					if ((OnOpenUDPSocket(newSocket) > 0) && (newSocket->OpenD(listionSocket,ListionAddr,OPEN_UDPS,cgOpenPar.cfg) > 0)){
						SetblUpdate();
						newSocket->UDPS_ReadFromDevice(databuf,bytesNum);
						newSocket = nullptr;
					}
				}
				CloseChild(newSocket);
			}
		}
	}
//	ETLogThreadStop(listionThread);
	return G_TRUE;
}
//------------------------------------------------------------------------------------------//
template <typename T_COMMU> void COMMU_SOCKETSERVER<T_COMMU>::DoPrintOnOpenSuccess(void){
	if (GetOpenPar().type == OPEN_TCPS){
		PrintMessageDot("Successfully setup TCP Server, listion Port is", Str_ToStr(GetOpenPar().port));
	}
	else{
		PrintMessageDot("Successfully setup UDP Server, listion Port is", Str_ToStr(GetOpenPar().port));
	}
}
//------------------------------------------------------------------------------------------//
template <typename T_COMMU> void COMMU_SOCKETSERVER<T_COMMU>::DoPrintOnOpenFail(void){
	if (GetOpenPar().type == OPEN_TCPS){
		PrintWarningMessageDot("Failed setup TCP Server,", "please try again");
	}
	else{
		PrintWarningMessageDot("Failed setup UDP Server,", "please try again");
	}
}
//------------------------------------------------------------------------------------------//
template <typename T_COMMU> void COMMU_SOCKETSERVER<T_COMMU>::DoPrintOnClose(void){
	
	PrintMessageDot("User stopped the server");
};
//------------------------------------------------------------------------------------------//
template <typename T_COMMU> bool32 COMMU_SOCKETSERVER<T_COMMU>::OnDoDisconnect	(COMMU_FRAME* delSocket)	{return G_TRUE;};
template <typename T_COMMU> bool32 COMMU_SOCKETSERVER<T_COMMU>::OnOpenTCPSocket	(COMMU_FRAME* newSocket)	{return G_TRUE;};
template <typename T_COMMU> bool32 COMMU_SOCKETSERVER<T_COMMU>::OnOpenUDPSocket	(COMMU_FRAME* newSocket)	{return(OnOpenTCPSocket(newSocket));};
//------------------------------------------------------------------------------------------//
template <typename T_COMMU> TNFP* COMMU_SOCKETSERVER<T_COMMU>::CreateNode(void){
	return(SetSubNodeFatherName(new T_COMMU(cgMaxSize,GetLogSystem())));
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif /* Commu_Socket_h */
#endif /* Commu_Socket_hpp */
