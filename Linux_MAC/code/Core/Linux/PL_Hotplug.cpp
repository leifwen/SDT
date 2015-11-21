/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: PL_Hotplug.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2012.02.09
 * @2013.7.03 :	dedicated for Linux
 * @2013.6.30 :	change to Liunx style
 add init method
 use int32 to replace bool
 use int32 to replace uint32
 */
//------------------------------------------------------------------------------------------//
#include "PL_Hotplug.h"
#include "ODEV_Include.h"
#include "Commu_DBuf.h"
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_Linux
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
void PL_HOTPLUG::Init(void){
	rxFwSBufList = &cgInternalFwSBufList;
	handle = -1;
}
//------------------------------------------------------------------------------------------//
int32 PL_HOTPLUG::Open(const std::string &tCDBufName,int32 tCDBufPar,CSType tCSType,int32 blEnEcho){
	socklen_t	rcvbufsize;
	struct 	sockaddr_nl		client;
	int32 	ret;
	int     rcvbuf;
	struct  timeval timeout;
	
	
	handle = socket(AF_NETLINK, SOCK_RAW, NETLINK_KOBJECT_UEVENT);
	if (handle == -1)
		return 0;
	
	memset(&client, 0, sizeof(client));
	client.nl_family = AF_NETLINK;
	client.nl_pid = getpid();
	client.nl_groups = 1;								/* receive broadcast message*/
	
	rcvbufsize = sizeof(socklen_t);
	
	timeout.tv_sec = 0;
	timeout.tv_usec = 1000 * 100;//100ms
	setsockopt(handle,SOL_SOCKET,SO_RCVTIMEO,(char *)&timeout,sizeof(struct timeval));
	
	if(getsockopt(handle,SOL_SOCKET,SO_RCVBUF,(char*)&rcvbuf,&rcvbufsize) != -1){
		if(rcvbuf < (int32)cgRxBuffer.BufferMaxSize())
			rcvbuf = (int32)cgRxBuffer.BufferMaxSize();
		setsockopt(handle,SOL_SOCKET,SO_RCVBUF,(char*)&rcvbuf,rcvbufsize);
	}
	
	ret = bind(handle, (struct sockaddr*)&client, sizeof(client));
	if (ret == -1){
		Close(0);
		return 0;
	}
	
	cgCDBufName = tCDBufName;
	cgCDBufPar = tCDBufPar;
	cgCSType = tCSType;
	SetblConnected();
	ClrblSDC();
	ClrblAClose();
	Clean();
	DisableEcho();
	
	rxThread.ThreadRun();
	SetblConnected();
	return 1;
}
//------------------------------------------------------------------------------------------//
void PL_HOTPLUG::CloseSocket(void){
	if (handle != -1){
		shutdown(handle,SHUT_RDWR);
		close(handle);
	}
	handle = -1;
}
//------------------------------------------------------------------------------------------//
int32 PL_HOTPLUG::Close(int32 blClr){
	SetblAClose();
	CloseSocket();
	ClrblSDC();
	COMMU_DBUF::Close(blClr);
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 PL_HOTPLUG::ReadFromDevice(uint8 *buffer,uint32 length,uint32 *retNum){
	int64		retCode;
	*retNum = 0;
	
	retCode = recv(handle,(char*)buffer,length,0);
	if (retCode > 0){
		*retNum = (uint32)retCode;
		return 1;
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif