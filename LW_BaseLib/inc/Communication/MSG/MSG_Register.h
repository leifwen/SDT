//
//  MSG_Register.h
//  SDT
//
//  Created by Leif Wen on 17/03/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "MSG_Center.h"
#include "PatchCode.h"
#include "Commu_Socket.h"
#include "Commu_SSL.h"
#if defined MSG_Center_h && defined PatchCode_h
//------------------------------------------------------------------------------------------//
#ifndef MSG_Register_h
#define MSG_Register_h
#ifdef MSG_Register_h
//------------------------------------------------------------------------------------------//
class MSG_Rerister : public MSG_NODE{
	protected:
		enum	{RFLAG_C = 3, RFLAG_S = MSG_NODE::RFLAG_S + MSG_NODE::RFLAG_C};
		enum	{REG_blApproveY = RFLAG_CREATE(0)
				,REG_blApproveN = RFLAG_CREATE(1)
				,REG_blReject	= RFLAG_CREATE(2)
		};
	public:
				 MSG_Rerister(void);
		virtual ~MSG_Rerister(void){;};
	private:
		uint32			cgApproveHours;
		REG_SIGN		cgRegSign;
		LINENSE_SIGN	cgLS;
	private:
		virtual	CMDID	MessageProcessing			(CMD_ENV* env,const uint32& mID,const STDSTR& msg,void* _team);
	public:
		virtual	void	Reset						(void* commu);
				bool32	Approve						(CMD_ENV* env,uint32 approveHours);
				bool32	Send_REQ_License			(CMD_ENV* env,ARRAY *array,uint32 waitTimeS);
};
//------------------------------------------------------------------------------------------//
#if defined Commu_Socket_h && defined Commu_SSL_h
#ifndef Commu_License_h
#define Commu_License_h
#ifdef Commu_License_h
typedef COMMU<TMEM|TBIRDGE,COMMU_FRAME_NOSMC,CORE_SOCKET,MEM_MSG<MSG_Rerister>>	LicSocket;
typedef	COMMU_POOL<COMMU<0,COMMU_FRAME_NOSMC,CORE_SOCKETSERVER>,LicSocket>		LicServer_BASE;
class LicServer : public LicServer_BASE{
	protected:
		enum	{RFLAG_C = 2, RFLAG_S = LicServer_BASE::RFLAG_S + LicServer_BASE::RFLAG_C};
		enum	{LB_blAnswer = RFLAG_CREATE(0),LB_blRequest	= RFLAG_CREATE(1),};
	public:
				 LicServer(uint32 size);
		virtual ~LicServer(void){;};
	private:
		DS_Lock				cgRequestLock;
		COMMU_FRAME*		cgRequestSocket;
		uint32*				cgApproveHours;
	public:
				bool32	CheckblRequest		(void)const;
				bool32	CheckApprove		(COMMU_FRAME* tSocket,uint32* retHours);
				void	ApproveRegistration	(const uint32& approveHours);
				void	RejectRegistration	(void);
		const	STDSTR&	RequestSocketInfo	(STDSTR* strPrint);
};
#endif
#endif
#endif
//------------------------------------------------------------------------------------------//
#endif /* MSG_Register_h */
#endif /* MSG_Register_h */
#endif /* MSG_Center_h */
