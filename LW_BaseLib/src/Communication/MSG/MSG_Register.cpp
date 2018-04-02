//
//  MSG_Register.cpp
//  SDT
//
//  Created by Leif Wen on 17/03/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
#include "MSG_ID.h"
#include "MSG_Register.h"
#include "Commu_DBuf.h"
#include "SYS_File.h"
#ifdef MSG_Register_h
//------------------------------------------------------------------------------------------//
//#define LOGPRINT_ENABLE
//#define LOGMSG_ENABLE

#ifdef LOGMSG_ENABLE
#define ELogMSG(x) ELog(x);
#else
#define ELogMSG(x);
#endif

#include "SYS_Log.h"
//------------------------------------------------------------------------------------------//
MSG_Rerister::MSG_Rerister(uint32 size,void* p) : MSG_NODE(size,p){
	cgMSGID = MESG_ID_REG;

	selfName = "MSG_REG";
	SetFatherName("");
}
//------------------------------------------------------------------------------------------//
void MSG_Rerister::Reset(void* commu){
	ClrSFlag(REG_blApproveY | REG_blApproveN | REG_blReject);
};
//------------------------------------------------------------------------------------------//
bool32 MSG_Rerister::Approve(CMD_ENV* env,uint32 approveHours){
	bool32	blRet;
	ClrSFlag(REG_blApproveY | REG_blApproveN);
	cgApproveHours = approveHours;
	MSGSend2(env,MESG_REQ_Registration,IUD(""),REG_blApproveY,REG_blApproveN,MSG_CENTER::HandshakeTime,blRet,"");
	
	if ((blRet == 1) || (blRet == -3))
		return G_TRUE;
	return G_FALSE;
}
//------------------------------------------------------------------------------------------//
bool32 MSG_Rerister::Send_REQ_License(CMD_ENV* env,ARRAY *array,uint32 waitTimeS){
	bool32	blRet;

	ClrSFlag(REG_blApproveY | REG_blReject);
	MSGSend2(env,MESG_REQ_License,IUD(array),REG_blApproveY,REG_blReject,waitTimeS,blRet,"");

	return(blRet);
}
//------------------------------------------------------------------------------------------//
CMDID MSG_Rerister::MessageProcessing(CMD_ENV* env,const uint32& mID,const STDSTR& msg,void* commu){
	COMMU_THREAD*	_commu = static_cast<COMMU_THREAD*>(commu);
	bool32		blRet;
	CMDID		retCode = mID;
	ARRAY		*array;
	uint32		approveHours = 0;
	
#ifdef LOGMSG_ENABLE
	if ((mID >= MESG_REQ_Registration) && (mID <= MESG_ANS_Reject)){
		ELogMPRecMsg(mID);
	}
#endif
	
	switch (mID){
		case MESG_REQ_Registration:
			array = cgRegSign.Create(nullptr);
			if (array->Used() > 0){
				ELogMP("Create registration data successful, len = " << array->Used());
				MPSend(MESG_ANS_Registration,IUD(array),blRet);
			}
			else{
				ELogMP("Create array fail");
			}
			break;
		case MESG_ANS_Registration:
			array = cgLS.Create(cgApproveHours * 60 * 60,msg);
			if (array->Used() > 0){
				ELogMP("Create License data successful, len = " << array->Used());
				MPSend(MESG_REQ_WriteLicense,IUD(array),blRet);
			}
			else{
				ELogMP("Create Linense fail");
				SetSFlag(REG_blApproveN);
			}
			cgApproveHours = 0;
			break;
		case MESG_REQ_WriteLicense:
			ELogMP("Write to License.key");
			CFS_WriteFile(PATCHCODE::GetFNLic(), msg);
			MPSend(MESG_ANS_WriteLicense,IUD(""),blRet);
			break;
		case MESG_ANS_WriteLicense:
			SetSFlag(REG_blApproveY);
			break;
		case MESG_REQ_License:
			if (static_cast<LicenseBServer*>(_commu->GetFDB())->CheckApprove(_commu,&approveHours) > 0){
				if (approveHours == 0){
					MPSend(MESG_ANS_Reject,IUD(""),blRet);
				}
				else{
					array = cgLS.Create(approveHours * 60 * 60,msg);
				
		 			if (array->Used() > 0){
						MPSend(MESG_ANS_Approve,IUD(array),blRet);
					}
					else{
						ELogMP("Created linense fail");
						MPSend(MESG_ANS_Reject,IUD(""),blRet);
					}
				}
			}
			ELogMP("Call SelfClose()");
			cgMsgCenter->SetblREQHS();
			_commu->CloseSelf(1);
			break;
		case MESG_ANS_Approve:
			ELogMP("Write to License.key");
			CFS_WriteFile(PATCHCODE::GetFNLic(), msg);
			SetSFlag(REG_blApproveY);
			ELogMP("Call SelfClose()");
			cgMsgCenter->SetblREQHS();
			_commu->CloseSelf(1);
			break;
		case MESG_ANS_Reject:
			SetSFlag(REG_blReject);
			ELogMP("Call SelfClose()");
			cgMsgCenter->SetblREQHS();
			_commu->CloseSelf(1);
			break;
		default:
			retCode = MESG_NONE;
			break;
	}
	return(retCode);
}
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
LicenseBServer::LicenseBServer(int32 size) : LBServer(size,nullptr){
	ClrSFlag(LB_blAnswer | LB_blRequest);
	SetSelfName("LicenseBServer");
};
//------------------------------------------------------------------------------------------//
bool32 LicenseBServer::CheckblRequest(void)const{
	
	return(CheckSFlag(LB_blRequest));
};
//------------------------------------------------------------------------------------------//
bool32 LicenseBServer::CheckApprove(COMMU_THREAD* tSocket,uint32* retHours){
	while (cgRequestLock.TryLock() == G_FALSE){
		if (tSocket->IsOpened() == G_FALSE)
			return(CheckSFlag(LB_blAnswer));
	}
	ClrSFlag(LB_blAnswer);
	cgRequestSocket = tSocket;
	cgApproveHours = retHours;
	*cgApproveHours = 0;
	SetSFlag(LB_blRequest);
	while(tSocket->IsOpened() && (CheckSFlag(LB_blAnswer) == G_FALSE))
		SYS_SleepMS(10);
	if (tSocket->IsOpened() == G_FALSE)
		cgApproveHours = nullptr;
	return(CheckSFlag(LB_blAnswer));
};
//------------------------------------------------------------------------------------------//
void LicenseBServer::RejectRegistration(void){
	if (cgApproveHours != nullptr)
		*cgApproveHours = 0;
	ClrSFlag(LB_blRequest);
	SetSFlag(LB_blAnswer);
	cgRequestLock.Unlock();
};
//------------------------------------------------------------------------------------------//
void LicenseBServer::ApproveRegistration(const uint32& approveHours){
	if (cgApproveHours != nullptr)
		*cgApproveHours = approveHours;
	ClrSFlag(LB_blRequest);
	SetSFlag(LB_blAnswer);
	cgRequestLock.Unlock();
}
//------------------------------------------------------------------------------------------//
const STDSTR& LicenseBServer::RequestSocketInfo(STDSTR* strPrint){
	*strPrint  = SYS_MakeTimeNow();
	*strPrint += "Receive registration request from ";
	*strPrint += cgRequestSocket->GetDevName();
	return(*strPrint);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif /* MSG_Register_h */
