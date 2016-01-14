/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: Commu_Socket.h
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
#ifndef Commu_SocketH
#define Commu_SocketH
//------------------------------------------------------------------------------------------//
#include "Commu_DBuf.h"
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_Unix
	typedef int SOCKET;
	#define SOCKET_ERROR	(-1)
	#define INVALID_SOCKET	(-1)
	#define	SD_BOTH			SHUT_RDWR
	#include <iostream>
	#include <netdb.h>
	#include <arpa/inet.h>
#endif
#ifdef CommonDefH_VC
	#include <WINSOCK2.H>
	int32	CheckWSAStartup(void);
#endif
//------------------------------------------------------------------------------------------//
class APISocket : public COMMU_DBUF{//default is TCP
	public:
		enum{RFLAG_C = 1, RFLAG_S = COMMU_DBUF::RFLAG_S + COMMU_DBUF::RFLAG_C};
	public:
				 APISocket(const ODEV_LIST *tODEV_LIST,uint32 tSize) : COMMU_DBUF(tODEV_LIST,tSize){Handle = INVALID_SOCKET;selfName = "APISocket";};
		virtual ~APISocket(void){CloseD();};
	protected:
		virtual	int32	ReadFromDevice	(uint32 *retNum,uint8 *buffer,uint32 length);
		virtual	int32	SendToDevice	(uint32 *retNum,const uint8 *buffer,uint32 length);
		virtual void	ThreadsStart	(void);
	protected:
		virtual	int32	OpenDev	(const std::string &tCDBufName,int32 tCDBufPar,CSType tCSType);
		virtual	void	CloseDev(void);
	public:
		virtual	void	PrintConnectInfo	(int32 blSendWelcome);
		virtual	void	PrintDisconnectInfo	(void);
	private:
		sockaddr_in		cgUDPS_RemoteAddr;
	protected:
		int32	Socket_OpenDev		(const std::string &tCDBufName,int32 tCDBufPar,CSType tCSType);
		void	Socket_CloseDev		(void);
		int32	Server_OpenDev		(const std::string &tCDBufName,int32 tCDBufPar,CSType tCSType);
		void	Server_CloseDev		(void);
		void	Socket_ThreadsStart	(void);
		void	UDPS_ThreadsStart	(void);
		int32	UDPS_SendToDevice	(uint32 *retNum,const uint8 *buffer,uint32 length);
	public:
		SOCKET	Handle;
		void	SetSocketBufferSize(void);
};
//------------------------------------------------------------------------------------------//
#endif