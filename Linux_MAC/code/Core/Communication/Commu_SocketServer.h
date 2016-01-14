/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: Commu_SocketServer.h
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
#ifndef Commu_SocketServerH
#define Commu_SocketServerH
//------------------------------------------------------------------------------------------//
#include "Commu_Socket.h"
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
class APISocketServer_Socket : public APISocket{
	public:
		enum{RFLAG_C = 0, RFLAG_S = APISocket::RFLAG_S + APISocket::RFLAG_C};
	public:
				 APISocketServer_Socket(const ODEV_LIST *tODEV_LIST,uint32 tSize) : APISocket(tODEV_LIST,tSize){selfName = "APISocketServer->Socket";};
		virtual ~APISocketServer_Socket(void){;};
	private:
				void	PrintRecDataTitle	(ODEV_LIST_POOL *output, uint32 byteNum);
		virtual int32	Ex2ThreadFun		(void);
	public:
		virtual void	CreateLogFile		(void);
};
//------------------------------------------------------------------------------------------//
class APISocketServer : public CDBUF_POOL{
	public:
		enum{RFLAG_C = 2, RFLAG_S = CDBUF_POOL::RFLAG_S + CDBUF_POOL::RFLAG_C};
	public:
				 APISocketServer(const ODEV_LIST *tODEV_LIST,uint32 tSize);
		virtual ~APISocketServer(void){CloseD();};
	protected:
		SYS_ThreadEx_List					cgThreadList;
		SYS_ThreadEx<APISocketServer>		disconnectThread;
		SYS_ThreadEx<APISocketServer>		tcplistionThread;
		SYS_ThreadEx<APISocketServer>		udplistionThread;
	private:
		virtual	int32		OnDoDisconnect(APISocket *delSocket){return 1;};
				int32		DisconnectThreadFun	(void);
		virtual int32		TCPThreadListionFun	(void);
		virtual int32		UDPThreadListionFun	(void);
		virtual APISocket*	CreateNewSocket_TCP(const ODEV_LIST *tODEV_LIST,uint32 tSize);
		virtual APISocket*	CreateNewSocket_UDP(const ODEV_LIST *tODEV_LIST,uint32 tSize){return(CreateNewSocket_TCP(tODEV_LIST,tSize));};
		virtual int32		OnOpenTCPSocket(APISocket *newSocket){return 1;};
		virtual int32		OnOpenUDPSocket(APISocket *newSocket){return(OnOpenTCPSocket(newSocket));};
	protected:
		void	DisconnectAll	(void);
		int32	OpenDev			(int32 port,COMMU_DBUF::CSType tStype,int32 blEnEcho);
		int32	Close_Do		(void);
	public:
		int32	OpenD			(int32 port,COMMU_DBUF::CSType tStype,int32 blEnEcho);
		int32	CloseD			(void);
	protected:
		APISocket			*cgNewSocket;
		COMMU_DBUF::CSType	cgCSType;
		SOCKET				listionSocket;
		uint32				cgBufMaxSize;
		int32				cgPort;
	protected:
		inline	void	SetblConnected		(void)		{SetSFlag(RFLAG_CREATE(0));};
		inline	void	ClrblConnected		(void)		{ClrSFlag(RFLAG_CREATE(0));};
		inline	void	SetblEcho			(void)		{SetSFlag(RFLAG_CREATE(1));};
		inline	void	ClrblEcho			(void)		{ClrSFlag(RFLAG_CREATE(1));};
		inline	int32	CheckblEcho			(void)const	{return(CheckSFlag(RFLAG_CREATE(1)));};
	public:
		inline	int32	CheckblConnected	(void)const	{return(CheckSFlag(RFLAG_CREATE(0)));};
		inline	void	LEnableEcho			(void){SetblEcho();CDBUF_POOL::LEnableEcho();};
		inline	void	LDisableEcho		(void){ClrblEcho();CDBUF_POOL::LDisableEcho();};
		inline	void	SEnableEcho			(void){SetblEcho();CDBUF_POOL::SEnableEcho();};
		inline	void	SDisableEcho		(void){ClrblEcho();CDBUF_POOL::SDisableEcho();};
	public:
		void	PrintOpenSuccessReport		(void);
		void	PrintOpenFailReport			(void);
		void	PrintUserDisconnectReport	(const std::string &strType);
	public:
		inline	const	COMMU_DBUF::CSType	&GetCSType	(void)const{return(cgCSType);};
		inline	int32	GetcgPort			(void){return(cgPort);};
				void	SetODEV_LIST		(const ODEV_LIST *tODEV_LIST);
	private:
    	ODEV_LIST		*cgODevList;	//used for external
		ODEV_LIST_POOL	*cgOutput;
	protected:
		inline	ODEV_LIST*		GetcgODevList	(void){return(cgODevList);};
		inline	ODEV_LIST_POOL*	GetcgOutput		(void){return(cgOutput);};
};
//------------------------------------------------------------------------------------------//
#endif