//
//  Commu_Socket.h
//  SDT
//
//  Created by Leif Wen on 05/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "Commu_Base.h"
#ifdef Commu_Base_h
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
	bool32	CheckWSAStartup(void);
#endif
//------------------------------------------------------------------------------------------//
class CORE_SOCKET : public COMMU_CORE{//default is TCP
	public:
		enum	{PACKAGE_MAX_SIZE = 1024 * 8};
	public:
				 CORE_SOCKET(void);
		virtual ~CORE_SOCKET(void);
	protected:
		virtual	bool32	OpenDev				(const OPEN_PAR& par);
		virtual	void	CloseDev			(void);
	private:
		virtual	bool32	SendToDevice		(uint32* retNum,const uint8 *buffer,uint32 length);
		virtual	bool32	ReadFromDevice		(uint32* retNum,	  uint8 *buffer,uint32 length);
	private:
		virtual void	PrintOpenSuccess	(const STDSTR& strTitle = "");
		virtual void	PrintClose			(const uint64& rxBytes,const uint64& txBytes,const uint64& fwBytes);
	private:
		SOCKET			osHandle;
		sockaddr_in		cgUDPS_RemoteAddr;
		ARRAY			cgArrayUDPSrx;
	protected:
				bool32	OpenDev_socket		(const OPEN_PAR& par);
				bool32	OpenDev_server		(const OPEN_PAR& par);
				void	CloseDev_socket		(void);
				void	CloseDev_server		(void);
	public:
				bool32	Open				(const SOCKET& sID,const sockaddr_in& addr,uint32 type,uint32 cfg);
		
				void	SetSocketBufferSize	(void);
				void	UDPS_recv			(const uint8* buffer,uint32 num);
};
//------------------------------------------------------------------------------------------//
class CORE_SOCKETSERVER : public COMMU_CORE{
	public:
				 CORE_SOCKETSERVER(void);
		virtual ~CORE_SOCKETSERVER(void);
	public:
		virtual	void	Init				(const COMMU_TEAM* _team);
		virtual	void	SetSelfName			(const STDSTR& strName);
		virtual	void	SetUpName			(const STDSTR& strName);
	protected:
		virtual	bool32	OpenDev				(const OPEN_PAR& par);
		virtual	void	CloseDev			(void);
	private:
		virtual void	PrintOpenSuccess	(const STDSTR& strTitle = "");
		virtual void	PrintOpenFail		(const STDSTR& strTitle = "");
		virtual void	PrintClose			(const uint64& rxBytes,const uint64& txBytes,const uint64& fwBytes);
	protected:
		SYS_Thread<CORE_SOCKETSERVER>		listionThread;
		SOCKET								listionSocket;
	private:
				bool32	ListionThreadFun	(void* _team);
		virtual bool32	ListionTCP			(void* _team);
		virtual bool32	ListionUDP			(void* _team);
		
		virtual bool32	OnOpenTCPSocket		(COMMU_FRAME* newSocket);
		virtual bool32	OnOpenUDPSocket		(COMMU_FRAME* newSocket);
};
//------------------------------------------------------------------------------------------//
typedef COMMU<TDEFALL,COMMU_FRAME,CORE_SOCKET>										ASOCKET;
typedef	COMMU_POOL<COMMU<TLOGSYS|TFORWARD,COMMU_FRAME,CORE_SOCKETSERVER>,ASOCKET>	ASOCKETSERVER;
//------------------------------------------------------------------------------------------//
#endif /* Commu_Socket_h */
#endif /* Commu_Socket_h */
#endif /* Commu_Base_h */
