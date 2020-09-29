//
//  MSG_RMS.h
//  SDT
//
//  Created by Leif Wen on 15/03/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "Commu_Socket.h"
#include "MSG_Center.h"
#include "Commu_SSL.h"
#if defined MSG_Center_h && defined Commu_Socket_h && defined Commu_SSL_h
//------------------------------------------------------------------------------------------//
#ifndef MSG_RMS_h
#define MSG_RMS_h
#ifdef MSG_RMS_h
//------------------------------------------------------------------------------------------//
/*
			RSTBServer<========LINK=========>RSTBSocket
			  /    \							|
			 /      \							|
			/        \							|
		   /		  \							|
	  MAPBSERVER	   \						|
		  |				\						|
		  |				 \						|
	   BSocket<----->RSTBSocket<============>RSocket<----->BScoket
 		  |													 |
		  |													 |
		Socket											Local Server
*/
//------------------------------------------------------------------------------------------//
class MSG_RMS;
class CORE_MAPBSERVER : public CORE_SOCKETSERVER{
	public:
				 CORE_MAPBSERVER(void);
		virtual ~CORE_MAPBSERVER(void){;};
	private:
		OPEN_PAR			cgMapPar;
		MSG_RMS*			cgMsgRMS;
		CMD_ENV*			cgENV;
	private:
		virtual bool32		OnOpenTCPSocket	(COMMU_DRV* newSocket);
	public:
		const	OPEN_PAR&	GetMapPar		(void);
				bool32		Open			(CMD_ENV* env,MSG_RMS* msg,uint32 localPort,const OPEN_PAR& mapPar);
};
//------------------------------------------------------------------------------------------//
typedef COMMU_DRV_T<TMEM|TBIRDGE|TDISPOSE,COMMU_DRV_NOSMC,CORE_SOCKET>		BSOCKET;
typedef	CDRV_POOL_T<COMMU_DRV_T<0,COMMU_DRV_NOSMC,CORE_MAPBSERVER>,BSOCKET>	MAPBSERVER_BASE;
//------------------------------------------------------------------------------------------//
class MAPBSERVER : public MAPBSERVER_BASE{
	public:
				 MAPBSERVER(uint32 rxSize,uint32 txSize,const ODEV_BUS* odevBus);
		virtual ~MAPBSERVER(void){;};
	public:
		const	OPEN_PAR&	GetMapPar		(void);
				bool32		Open			(CMD_ENV* env,MSG_RMS* msg,uint32 localPort,const OPEN_PAR& mapPar);
};
//------------------------------------------------------------------------------------------//
class MSG_RMS : public MSG_NODE{
	public:
		typedef COMMU_DRV_T<TMEM|TBIRDGE|TDISPOSE,COMMU_DRV_NOSMC,CORE_SOCKET,MEM_MSG<MSG_RMS>>	RMSBSOCKET;
	protected:
		enum	{RFLAG_C = 8, RFLAG_S = MSG_NODE::RFLAG_S + MSG_NODE::RFLAG_C};
		enum	{RMS_blSetupMServerY	= RFLAG_CREATE(0)
				,RMS_blSetupMServerN	= RFLAG_CREATE(1)
				,RMS_blCloseMServerY	= RFLAG_CREATE(2)
				,RMS_blCloseMServerN	= RFLAG_CREATE(3)
				,RMS_blListMServerY		= RFLAG_CREATE(4)
				,RMS_blListMServerN		= RFLAG_CREATE(5)
				,RMS_blLinkY			= RFLAG_CREATE(6)
				,RMS_blLinkN			= RFLAG_CREATE(7)
		};
	public:
				 MSG_RMS(void);
		virtual	~MSG_RMS(void){;};
	protected:
		CDRV_POOL_T<COMMU_DRV_T<0,COMMU_DRV,COMMU_CORE>,MAPBSERVER>	cgMSPool;
		COMMU_CPPOOL<RMSBSOCKET,BSOCKET>							cgCPPool;
	private:
		virtual	void	Init						(uint32 rxSize,uint32 txSize);
		virtual	CMDID	MessageProcessing			(CMD_ENV* env,const uint32& mID,const STDSTR& msg,void* _team);
	public:
		virtual	void	SetSelfName					(const STDSTR& strName);
		virtual	void	SetUpName					(const STDSTR& strName);
		virtual	void	Reset						(void* _team);
		virtual	void	Close						(void* _team);
	private:
				bool32	OpenMServer					(CMD_ENV* env,MAPBSERVER** mServer, const STDSTR& strMesg);
				bool32	CloseMServer				(									const STDSTR& strMesg);
				bool32	ListMappingServer			(STDSTR* strMesg);
				bool32	OpenCoupleSocket			(CMD_ENV* env,RMSBSOCKET** rmssocket,BSOCKET** bsocket,STDSTR strMesg,const OPEN_PAR& par);
				bool32	Send_REG_Link				(CMD_ENV* env,const STDSTR& strMesg);
	public:
				bool32	Send_REQ_SetupMServer		(CMD_ENV* env,uint32 remotePort,const OPEN_PAR& mapPar);
				bool32	Send_REQ_CloseMServer		(CMD_ENV* env,uint32 remotePort);
				bool32	Send_REG_ListMServer		(CMD_ENV* env,STDSTR* retStr);
				bool32	Send_REQ_SetupDataChannel	(CMD_ENV* env,COMMU_DRV* mSocket,const OPEN_PAR& mapPar);
};
//------------------------------------------------------------------------------------------//
#endif /* MSG_RMS_h */
#endif /* MSG_RMS_h */
#endif /* MSG_Center_h */
