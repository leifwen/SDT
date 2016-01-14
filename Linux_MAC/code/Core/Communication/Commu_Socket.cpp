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
//------------------------------------------------------------------------------------------//
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
//------------------------------------------------------------------------------------------//
int32 APISocket::Socket_OpenDev(const std::string &tCDBufName,int32 tCDBufPar,CSType tCSType){
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
		Handle = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	}
	else{
		Handle = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	}
	if (Handle == INVALID_SOCKET){
		Close_Do();
		return 0;
	}
	
	SetSocketBufferSize();
	
	if (connect(Handle,(struct sockaddr *)&tcpaddr, sizeof(tcpaddr)) == SOCKET_ERROR){
		Close_Do();
		return 0;
	}
	return 1;
}
//------------------------------------------------------------------------------------------//
void APISocket::Socket_CloseDev(void){
	if (Handle != INVALID_SOCKET){
		shutdown(Handle,SD_BOTH);
#ifdef CommonDefH_Unix
		close(Handle);
#endif
#ifdef CommonDefH_VC
		
		closesocket(Handle);
#endif
	}
	Handle = INVALID_SOCKET;
}
//------------------------------------------------------------------------------------------//
int32 APISocket::Server_OpenDev(const std::string &tCDBufName,int32 tCDBufPar,CSType tCSType){
	SetCSType(tCSType);
	if (tCSType == CSType_TCPS)
		return 1;
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
void APISocket::Server_CloseDev(void){
	if (GetCSType() == CSType_TCPS)
		Socket_CloseDev();
	Handle = INVALID_SOCKET;
}
//------------------------------------------------------------------------------------------//
int32 APISocket::OpenDev(const std::string &tCDBufName,int32 tCDBufPar,CSType tCSType){
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
void APISocket::CloseDev(void){
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
void APISocket::SetSocketBufferSize(void){
	int	rcvbuf;
#ifdef CommonDefH_Unix
	socklen_t	rcvbufsize;
	rcvbufsize = sizeof(socklen_t);
#endif
#ifdef CommonDefH_VC
	int		rcvbufsize;
	rcvbufsize = sizeof(int);
#endif
	if(getsockopt(Handle,SOL_SOCKET,SO_RCVBUF,(char*)&rcvbuf,&rcvbufsize) != SOCKET_ERROR){
		if(rcvbuf < (int32)cgRxBuffer.BufferMaxSize())
			rcvbuf = (int32)cgRxBuffer.BufferMaxSize();
		setsockopt(Handle,SOL_SOCKET,SO_RCVBUF,(char*)&rcvbuf,rcvbufsize);
	}
	if(getsockopt(Handle,SOL_SOCKET,SO_SNDBUF,(char*)&rcvbuf,&rcvbufsize) != SOCKET_ERROR){
		if(rcvbuf < (int32)cgTxBuffer.BufferMaxSize())
			rcvbuf = (int32)cgTxBuffer.BufferMaxSize();//PACKAGE_MAX_SIZE;
		setsockopt(Handle,SOL_SOCKET,SO_SNDBUF,(char*)&rcvbuf,rcvbufsize);
	}
}
//------------------------------------------------------------------------------------------//
int32 APISocket::ReadFromDevice(uint32 *retNum,uint8 *buffer,uint32 length){
	int64		retCode;
	*retNum = 0;
	
	retCode = recv(Handle,(char*)buffer,length,0);
	if ((GetCSType() == CSType_TCP) || (GetCSType() == CSType_TCPS)){
		if (((retCode == SOCKET_ERROR) && (errno != EINTR) && (errno != EWOULDBLOCK) && (errno != EAGAIN))
			|| ((retCode == 0) && (errno != EINTR))){
			SYS_SleepMS(50);
			if (CheckblAClose() == 0){
				SetblSDC();
				if (GetCSType() == CSType_TCP)
					PrintUserDisconnectReport(" TCP " + GetBufName() + ":" + Str_IntToString(GetBufPar()) + " connection disconnected.\r\n");
			}
			ClrblConnected();
			CloseDev();
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
int32 APISocket::SendToDevice(uint32 *retNum,const uint8 *buffer,uint32 length){
	int64		retCode;
	uint32		i;
	if (GetCSType() == CSType_UDPS)
		return(UDPS_SendToDevice(retNum,buffer,length));
	
	i = 0;
	while((i < length) && (CheckblAClose() == 0)){
		retCode = send(Handle,(char*)&buffer[i],length - i,0);
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
int32 APISocket::UDPS_SendToDevice(uint32 *retNum,const uint8 *buffer,uint32 length){
	int64		retCode;
	uint32		i;
	
	i = 0;
	while((i < length) && (CheckblAClose() == 0)){
		retCode = sendto(Handle,(char*)&buffer[i],length - i,0,(struct sockaddr *)&cgUDPS_RemoteAddr,sizeof(cgUDPS_RemoteAddr));
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
void APISocket::ThreadsStart(void){
	if (GetCSType() == CSType_UDPS)
		return UDPS_ThreadsStart();
	Socket_ThreadsStart();
}
//------------------------------------------------------------------------------------------//
void APISocket::Socket_ThreadsStart(void){
	SMC_EncryptI(0)
	SMC_EncryptS(0)
	txThread.ThreadRun();
	rxThread.ThreadRun();
	ex2Thread.ThreadRun();
	SMC_EncryptE(0)
}
//------------------------------------------------------------------------------------------//
void APISocket::UDPS_ThreadsStart(void){
	SMC_EncryptI(0)
	SMC_EncryptS(0)
	txThread.ThreadRun();
	ex2Thread.ThreadRun();
	SMC_EncryptE(0)
}
//------------------------------------------------------------------------------------------//
void APISocket::PrintConnectInfo(int32 blSendWelcome){
	std::string		strPrintData;
	SYS_DateTime	cDataTime;
	
	if (blSendWelcome != 0){
		strPrintData = DEV_LINE_START;
		strPrintData += "\r\nHello Visitor,\r\n  Welcome to visit TCP server.Your record log file should be:\r\n";
		if (GetcgODevPool() != nullptr){
			strPrintData += ODEV_GetLOGDIR();
#ifdef CommonDefH_Unix
			strPrintData += "/";
#endif
#ifdef CommonDefH_VC
			strPrintData += "\\";
#endif
			strPrintData += GetcgODevPool()->cODevFileTXT->GetFileName();
		}
		
		strPrintData += "\r\n\r\nNow the server time is ";
		cDataTime.Now();
		strPrintData += cDataTime.FormatDateTime("YYYY/MM/DD,hh:mm:ss.zzz");
		strPrintData += ".\r\n\r\nWish you have a nice day.\r\n";
		strPrintData += "Best regards.\r\n";
		strPrintData += "TCP SERVER, ";
		strPrintData += SWVERSION_VER;
		strPrintData += "\r\n";
		strPrintData += SWVERSION_WR;
		strPrintData += "\r\n";
		strPrintData += DEV_LINE_START;
		strPrintData += "\r\n";
		WriteInASCII(strPrintData,G_ESCAPE_OFF);
	}
	
	strPrintData = SYS_MakeTimeNow();
	strPrintData += " Client ";
	strPrintData += GetBufName();
	strPrintData += ":";
	strPrintData += Str_IntToString(GetBufPar());
	strPrintData += " connected.\r\n";
	Spin_InUse_set();
	if (GetcgODevPool() != nullptr){
		GetcgODevPool()->Spin_InUse_set();
		GetcgODevPool()->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
		GetcgODevPool()->WriteStrN(strPrintData,RICH_CF_clPurple,COLSTRING::COL_EP_YES);
		GetcgODevPool()->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
		GetcgODevPool()->Spin_InUse_clr();
	}
	if (GetcgOutput() != nullptr){
		GetcgOutput()->Spin_InUse_set();
		GetcgOutput()->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
		GetcgOutput()->WriteStrN(strPrintData,RICH_CF_clPurple,COLSTRING::COL_EP_YES);
		GetcgOutput()->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
		GetcgOutput()->Spin_InUse_clr();
	}
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void APISocket::PrintDisconnectInfo(void){
	std::string		strPrintData;
	
	strPrintData = SYS_MakeTimeNow();
	strPrintData += " Client ";
	strPrintData += GetBufName();
	strPrintData += ":";
	strPrintData += Str_IntToString(GetBufPar());
	strPrintData += " disconnected.\r\n";
	strPrintData += ("Received : " + Str_UInt64ToString(RxBytes()) + " bytes\r\n");
	strPrintData += ("Echo     : " + Str_UInt64ToString(FwBytes()) + " bytes\r\n");
	strPrintData += ("Sent     : " + Str_UInt64ToString(TxBytes()) + " bytes\r\n");
	
	Spin_InUse_set();
	if (GetcgOutput() != nullptr){
		GetcgOutput()->Spin_InUse_set();
		GetcgOutput()->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
		GetcgOutput()->WriteStrN(strPrintData,RICH_CF_clPurple,COLSTRING::COL_EP_YES);
		GetcgOutput()->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
		GetcgOutput()->Spin_InUse_clr();
	}
	if (GetcgODevPool() != nullptr){
		GetcgODevPool()->Spin_InUse_set();
		GetcgODevPool()->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
		GetcgODevPool()->WriteStrN(strPrintData,RICH_CF_clPurple,COLSTRING::COL_EP_YES);
		GetcgODevPool()->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
		GetcgODevPool()->Spin_InUse_clr();
	}
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
