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
//------------------------------------------------------------------------------------------//
#include "Commu_DBuf.h"
#ifdef COMMU_DBUFH
#ifndef Commu_SocketH
#define Commu_SocketH
#ifdef Commu_SocketH
#ifdef Commu_SSLH
#define Commu_SSLSocketH
#endif
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_Unix
	typedef int SOCKET;
	#define SOCKET_ERROR	(-1)
	#define INVALID_SOCKET	(-1)
	#define	SD_BOTH			SHUT_RDWR
	#include <netdb.h>
	#include <arpa/inet.h>
#endif
#ifdef CommonDefH_VC
	#include <WINSOCK2.H>
	int32	CheckWSAStartup(void);
#endif
//------------------------------------------------------------------------------------------//
class ASOCKET : public COMMU_DBUF{//default is TCP
	public:
				 ASOCKET(uint32 tSize,const ODEV_SYSTEM *logSys = nullptr) : COMMU_DBUF(tSize,logSys){Init();SetSelfName("ASOCKET");};
		virtual ~ASOCKET(void){CloseD();};
	private:
		void			Init(void);
	protected:
		virtual	int32	OpenDev				(const STDSTR &tCDBufName,int32 tCDBufPar,CSType tCSType,int32 blEnEcho);
		virtual	void	CloseDev			(void);
	private:
		virtual	int32	ReadFromDevice		(uint32 *retNum,uint8 *buffer,uint32 length);
		virtual	int32	SendToDevice		(uint32 *retNum,const uint8 *buffer,uint32 length);
	private:
		virtual void	DoPrintOnOpenSuccess(void);
		virtual void	DoPrintOnClose		(void);
	private:
		SOCKET			osHandle;
		sockaddr_in		cgUDPS_RemoteAddr;
	protected:
		int32	Socket_OpenDev		(const STDSTR &tCDBufName,int32 tCDBufPar,CSType tCSType);
		void	Socket_CloseDev		(void);
		int32	Server_OpenDev		(const STDSTR &tCDBufName,int32 tCDBufPar,CSType tCSType);
		void	Server_CloseDev		(void);
		int32	UDPS_SendToDevice	(uint32 *retNum,const uint8 *buffer,uint32 length);
	public:
		int32	Open				(const SOCKET &sID,const sockaddr_in &addr,CSType tCSType,int32 blEnEcho);
	
		void	SetSocketBufferSize	(void);
		void	UDPS_ReadFromDevice	(const uint8 *buffer,uint32 num);
};
//------------------------------------------------------------------------------------------//
class ASOCKETSERVER : public COMMU_DBUF_FRAME_FW{
	public:
				 ASOCKETSERVER(uint32 tSize,const ODEV_SYSTEM *logSys = nullptr):COMMU_DBUF_FRAME_FW(0,logSys){Init(tSize);DisableLog();SetSelfName("ASocketServer");};
		virtual ~ASOCKETSERVER(void);
	private:
		void	Init(uint32 tSize);
	public:
		virtual	void	ChildSetSel	(DBUFFER *sdb);
		virtual	void	ChildClrSel	(DBUFFER *sdb);
	protected:
		virtual	int32	OpenDev				(const STDSTR &tCDBufName,int32 tCDBufPar,CSType tCSType,int32 blEnEcho);
		virtual	void	DoClose				(void);
		virtual	void	CloseDev			(void);
	private:
		virtual void	DoPrintOnClose		(void){PrintNormalMessage("User stopped the server");};
		virtual void	DoPrintOnOpenSuccess(void);
		virtual void	DoPrintOnOpenFail	(void);
	public:
		virtual	void	EnableEcho			(void);
		virtual	void	DisableEcho			(void);
	protected:
		SYS_Thread<ASOCKETSERVER>	monitorThread;
		SYS_Thread<ASOCKETSERVER>	listionThread;
	private:
		int32		MonitorThreadFun	(void *p);
		int32		ListionThreadFun	(void *p);
		virtual int32	ListionTCP		(void *p);
		virtual int32	ListionUDP		(void *p);
	
		inline	virtual	int32		OnDoDisconnect		(ASOCKET *delSocket)	{return 1;};
		inline	virtual int32		OnOpenTCPSocket		(ASOCKET *newSocket)	{return 1;};
		inline	virtual int32		OnOpenUDPSocket		(ASOCKET *newSocket)	{return(OnOpenTCPSocket(newSocket));};
		inline	virtual	TNFP*		CreateNode			(void){return(SetSubNodeSelfName(new ASOCKET(cgBufMaxSize,GetLogSystem())));};
	protected:
		SOCKET	listionSocket;
};
//------------------------------------------------------------------------------------------//
class BSOCKET : public ASOCKET{
	public:
				 BSOCKET(uint32 tSize,const ODEV_SYSTEM *logSys = nullptr) : ASOCKET(tSize,logSys){SetSelfName("BSOCKET");};
		virtual ~BSOCKET(void){;};
	public:
		inline	virtual void	DoThreadsStart	(void){cgThreadList.LThreadRun();};
};
//------------------------------------------------------------------------------------------//
class BSOCKETSERVER : public ASOCKETSERVER{
	public:
				 BSOCKETSERVER(uint32 tSize,const ODEV_SYSTEM *logSys = nullptr) : ASOCKETSERVER(tSize,nullptr){SetSelfName("BSocketServer");};
		virtual ~BSOCKETSERVER(void){;};
	private:
		inline	virtual void	DoThreadsStart	(void){cgThreadList.LThreadRun();};
		inline	virtual	TNFP*	CreateNode		(void){return(SetSubNodeSelfName(new BSOCKET(cgBufMaxSize,GetLogSystem())));};
};
//------------------------------------------------------------------------------------------//
#endif
#endif
#endif

