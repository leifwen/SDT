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
				 MSG_Rerister(uint32 size,void* p);
		virtual ~MSG_Rerister(void){;};
	private:
		uint32			cgApproveHours;
		REG_SIGN		cgRegSign;
		LINENSE_SIGN	cgLS;
	private:
		virtual	CMDID	MessageProcessing			(CMD_ENV* env,const uint32& mID,const STDSTR& msg,void* commu);
	public:
		virtual	void	Reset						(void* commu);
				bool32	Approve						(CMD_ENV* env,uint32 approveHours);
				bool32	Send_REQ_License			(CMD_ENV* env,ARRAY *array,uint32 waitTimeS);
};
//------------------------------------------------------------------------------------------//
typedef COMMU_NOSMC<COMMU_SOCKET<COMMU_MSG<COMMU_SSL,MSG_Rerister>>>	LicenseBSocket;
class LicenseBServer : public COMMU_NOSMC<COMMU_SOCKETSERVER<LicenseBSocket>>{
		typedef COMMU_NOSMC<COMMU_SOCKETSERVER<LicenseBSocket>> LBServer;
	protected:
		enum	{RFLAG_C = 2, RFLAG_S = LBServer::RFLAG_S + LBServer::RFLAG_C};
		enum	{LB_blAnswer = RFLAG_CREATE(0),LB_blRequest	= RFLAG_CREATE(1),};
	public:
				 LicenseBServer(int32 size);
		virtual ~LicenseBServer(void){;};
	private:
		DS_Lock				cgRequestLock;
		COMMU_THREAD*		cgRequestSocket;
		uint32*				cgApproveHours;
	public:
				bool32	CheckblRequest		(void)const;
				bool32	CheckApprove		(COMMU_THREAD* tSocket,uint32* retHours);
				void	ApproveRegistration	(const uint32& approveHours);
				void	RejectRegistration	(void);
		const	STDSTR&	RequestSocketInfo	(STDSTR* strPrint);
};
//------------------------------------------------------------------------------------------//
#endif /* MSG_Register_h */
#endif /* MSG_Register_h */
#endif /* MSG_Center_h */
