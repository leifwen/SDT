//
//  Commu_Socket.h
//  SDT
//
//  Created by Leif Wen on 05/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "Commu_DBuf.h"
#ifdef Commu_DBuf_h
//------------------------------------------------------------------------------------------//
#ifndef Commu_Socket_h
#define Commu_Socket_h
#ifdef Commu_Socket_h
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
	static inline bool32	CheckWSAStartup(void);
#endif
//------------------------------------------------------------------------------------------//
template <typename T_COMMU> class COMMU_SOCKET : public T_COMMU{//default is TCP
	public:
				 COMMU_SOCKET(uint32 size,const ODEV_SYSTEM* logSys);
		virtual ~COMMU_SOCKET(void);
	protected:
		virtual	bool32	OpenDev				(const OPEN_PAR& par);
		virtual	void	CloseDev			(void);
	private:
		virtual	bool32	SendToDevice		(uint32* retNum,const uint8 *buffer,uint32 length);
		virtual	bool32	ReadFromDevice		(uint32* retNum,	  uint8 *buffer,uint32 length);
	private:
		virtual void	DoPrintOnOpenSuccess(void);
		virtual void	DoPrintOnClose		(void);
	private:
		SOCKET			osHandle;
		sockaddr_in		cgUDPS_RemoteAddr;
	protected:
				bool32	Socket_OpenDev		(const OPEN_PAR& par);
				void	Socket_CloseDev		(void);
				bool32	Server_OpenDev		(const OPEN_PAR& par);
				void	Server_CloseDev		(void);
				bool32	UDPS_SendToDevice	(uint32* retNum,const uint8* buffer,uint32 length);
	public:
				bool32	OpenD				(const SOCKET& sID,const sockaddr_in& addr,uint32 type,uint32 cfg);
		
				void	SetSocketBufferSize	(void);
				void	UDPS_ReadFromDevice	(const uint8* buffer,uint32 num);
};
//------------------------------------------------------------------------------------------//
template <typename T_COMMU> class COMMU_SOCKETSERVER : public COMMU_FRAME_LOGSYS{
	public:
				 COMMU_SOCKETSERVER(uint32 size,const ODEV_SYSTEM* logSys);
		virtual ~COMMU_SOCKETSERVER(void);
	public:
				virtual	void	SetSelfName			(const STDSTR& strName);
				virtual	void	SetFatherName		(const STDSTR& strName);
	public:
				virtual	void	ChildSetSel			(DBUF* sdb);
				virtual	void	ChildClrSel			(DBUF* sdb);
	protected:
				virtual	bool32	OpenDev				(const OPEN_PAR& par);
				virtual	void	CloseDev			(void);
				virtual	void	DoClose				(void);
	private:
				virtual void	DoPrintOnClose		(void);
				virtual void	DoPrintOnOpenSuccess(void);
				virtual void	DoPrintOnOpenFail	(void);
	public:
				virtual	void	EnableEcho			(void);
				virtual	void	DisableEcho			(void);
	protected:
		SYS_Thread<COMMU_SOCKETSERVER>	listionThread;
	private:
						bool32	ListionThreadFun	(void* commu);
				virtual bool32	ListionTCP			(void* commu);
				virtual bool32	ListionUDP			(void* commu);
	
				virtual	bool32	OnDoDisconnect		(COMMU_FRAME* delSocket);
				virtual bool32	OnOpenTCPSocket		(COMMU_FRAME* newSocket);
				virtual bool32	OnOpenUDPSocket		(COMMU_FRAME* newSocket);
				virtual	TNFP*	CreateNode			(void);
	protected:
		SOCKET			listionSocket;
};
//------------------------------------------------------------------------------------------//
typedef COMMU_SOCKET<COMMU_THREAD>						ASOCKET;
typedef COMMU_SOCKETSERVER<ASOCKET>						ASOCKETSERVER;

#ifdef ADS_SSL_h
typedef COMMU_SOCKET<COMMU_SSL>							ASOCKETSSL;
typedef COMMU_SOCKETSERVER<ASOCKETSSL>					ASOCKETSERVERSSL;
#endif

typedef COMMU_NOSMC<COMMU_SOCKET<COMMU_THREAD>> 		BSOCKET;
typedef COMMU_NOSMC<COMMU_SOCKETSERVER<BSOCKET>>		BSOCKETSERVER;
//------------------------------------------------------------------------------------------//
#include "Commu_Socket.hpp"
#endif /* Commu_Socket_h */
#endif /* Commu_Socket_h */
#endif /* Commu_DBuf_h */
