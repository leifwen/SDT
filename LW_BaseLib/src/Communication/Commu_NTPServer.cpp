//
//  Commu_NTPServer.cpp
//  SDT
//
//  Created by Leif Wen on 05/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "Commu_NTPServer.h"
#ifdef Commu_NTPServer_h
//------------------------------------------------------------------------------------------//
CORE_NTPServer::CORE_NTPServer(void) : CORE_SOCKETSERVER(){
	TNFP::SetSelfName("NTPServer");
	SetUpName("");
};
//------------------------------------------------------------------------------------------//
bool32 CORE_NTPServer::ListionUDP(void* _team){
	sockaddr_in		ListionAddr;
#ifdef CommonDefH_Unix
	socklen_t	addrlen;
#endif
#ifdef CommonDefH_VC
	int			addrlen;
#endif
	uint8	buffer[48];
	int32	bytesNum;
	STDSTR	strTemp;
	DTIME	dT2, dT3, dTR;
	
	addrlen = sizeof(ListionAddr);
	dTR.Now();
	
	while(listionThread.IsTerminated() == G_FALSE){
		bytesNum = (int32)recvfrom(listionSocket, (char*)buffer, 48, 0, (struct sockaddr *)&ListionAddr, &addrlen);
		if ((bytesNum != SOCKET_ERROR) && (bytesNum == 48)){
			dT2.Now();
			/*
			 strH = "CB 01 00 00";
			 strR1 = "00 00 00 00";
			 strR2 = "00 00 00 00";
			 strR3 = "00 00 00 00";
			 strTR = "00 00 00 00 00 00 00 00";
			 strT1 = "00 00 00 00 00 00 00 00";
			 strT2 = "00 00 00 00 00 00 00 00";
			 strT3 = "00 00 00 00 00 00 00 00";
			 */
			buffer[0] = 0;
			while (++buffer[0] < 48){
				buffer[buffer[0]] = 0;
			};
			buffer[0] = 0xCB;
			buffer[1] = 0x01;
			SYS_NTPToChar(SYS_TimeToNTP(dTR.GetSec()), buffer + 16);
			SYS_NTPToChar(SYS_TimeToNTP(dT2.GetSec()), buffer + 32);
			dT3.Now();
			SYS_NTPToChar(SYS_TimeToNTP(dT2.GetSec()), buffer + 40);
			sendto(listionSocket, (char*)buffer, 48, 0, (struct sockaddr *)&ListionAddr, sizeof(ListionAddr));
		}
	}
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
bool32 NTPServer::Run(int32 port){
	return(Open(SetOpenPar(OPEN_UDPS,"",port,NTPServer::CFG_blDisableEcho)));
};
//------------------------------------------------------------------------------------------//
#endif /* Commu_NTPServer_h */
