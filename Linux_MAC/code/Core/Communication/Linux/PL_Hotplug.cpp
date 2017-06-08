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
//------------------------------------------------------------------------------------------//
#ifdef PL_HotplugH
#include <linux/netlink.h>
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 PL_HOTPLUG::OpenDev(const STDSTR &tCDBufName,int32 tCDBufPar,CSType tCSType,int32 blEnEcho){
	struct 	sockaddr_nl		client;
	struct  timeval timeout;
	
	osHandle = socket(AF_NETLINK, SOCK_RAW, NETLINK_KOBJECT_UEVENT);
	if (osHandle == INVALID_SOCKET)
		return 0;
	
	memset(&client, 0, sizeof(client));
	client.nl_family = AF_NETLINK;
	client.nl_pid = getpid();
	client.nl_groups = 1;								/* receive broadcast message*/
	
	
	timeout.tv_sec = 0;
	timeout.tv_usec = 1000 * 100;//100ms
	setsockopt(osHandle,SOL_SOCKET,SO_RCVTIMEO,(char *)&timeout,sizeof(struct timeval));
	
	SetSocketBufferSize();
	
	if (bind(osHandle, (struct sockaddr*)&client, sizeof(client) == SOCKET_ERROR))
		return 0;
	return 1;
}
//------------------------------------------------------------------------------------------//
void PL_HOTPLUG::CloseDev(void){
	Socket_CloseDev();
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif
