/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: Commu_SSL_License.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2015.10.26
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "License_SSLSocket.h"
#include "Comm_Convert.h"
#include "SYS_Time.h"
#include "Comm_File.h"
#define LOGPRINT_ENABLE
#define LOGPRINT_ENABLE2
#include "Comm_Log.h"
//------------------------------------------------------------------------------------------//
#ifdef License_SSLSocketH
//------------------------------------------------------------------------------------------//
enum{
	MESG_REQ_License = MESG_NEXT_SSL,
	MESG_ANS_Approve,
	MESG_ANS_Reject,
};
//------------------------------------------------------------------------------------------//
STDSTR LicenseSSLSocket::GetMesgText(uint32 mID){
	STDSTR retStr = "";
#ifdef LOGPRINT
	retStr = COMMU_DBUF_SSL::GetMesgText(mID);
	if (retStr.length() > 0)
		return(retStr);
	
	retStr = "(" + Str_ToString(mID)+ ")";
	switch(mID){
		case MESG_REQ_License				:retStr += "MESG_REQ_Close ";break;
		case MESG_ANS_Approve				:retStr += "MESG_ANS_Approve ";break;
		case MESG_ANS_Reject				:retStr += "MESG_ANS_Reject ";break;
		default:;
	}
#endif
	return(retStr);
}
//------------------------------------------------------------------------------------------//
#define CHSendInMP(_mID,_strMesg,_blRet)	CHSend(_mID,_strMesg,_blRet,"MessageProcessing()::")
#define CHRecInMP(_mID)						CHRec(_mID,"MessageProcessing()::");
#define E2LogInMP(_strMesg)					E2Log(this << "MessageProcessing()::" << _strMesg);
//------------------------------------------------------------------------------------------//
int32 LicenseSSLSocket::MessageProcessing(const uint32 &mID,const STDSTR &strMesg){
	STDSTR	strMesgT,strContent;
	int32	blDo,blRet;
	uint64	approveSecond = 0;
	
	if (BSOCKET::MessageProcessing(mID,strMesg) > 0)
		return 1;
	
	if (mID == MESG_NONE)
		return 0;
	
	blDo = 1;
	if ((mID >= MESG_NEXT_SSL) && (mID <= MESG_ANS_Reject)){
		CHRecInMP(mID);
	}
	switch (mID){
		case MESG_REQ_License:
			if (static_cast<LicenseSSLServer*>(GetFDB())->CheckApprove(this,&approveSecond) > 0){
				if (approveSecond == 0){
					CHSendInMP(MESG_ANS_Reject,"",blRet);
				}
				else if (gLS.Encode(&strContent, strMesg, approveSecond) > 0){
					CHSendInMP(MESG_ANS_Approve,strContent,blRet);
				}
				else{
					E2LogInMP("Created linense fail");
					CHSendInMP(MESG_ANS_Reject,"",blRet);
				}
			}
			SetblNoSendCloseMesg();
			SelfClose();
			break;
		case MESG_ANS_Approve:
			E2LogInMP("Write to License.key");
			CFS_WriteFile("License.key", strMesg);
			SetSFlag(blApprove);
			SetblNoSendCloseMesg();
			SelfClose();
			break;
		case MESG_ANS_Reject:
			SetSFlag(blReject);
			SetblNoSendCloseMesg();
			SelfClose();
			break;
		default:
			blDo = 0;
		break;
	}
	return(blDo);
}
//------------------------------------------------------------------------------------------//
int32 LicenseSSLSocket::CreateRegSignature(STDSTR *strReg,STDSTR *retStatus){
	Reg_Signature	regS;
	return(regS.Encode(strReg,retStatus));
}
//------------------------------------------------------------------------------------------//
int32 LicenseSSLSocket::SendRequestLicense(const STDSTR &regSignature,uint32 waitTimeS){
	return((SendCHMesg(MESG_REQ_License,regSignature,blApprove,blReject,waitTimeS) > 0)?1:0);
}
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
int32 LicenseSSLServer::CheckApprove(LicenseSSLSocket *tSocket,uint64 *retSecond){
	while (Spin_Request_Try() == 0){
		if (tSocket->IsConnected() == 0)
			return(CheckSFlag(blAnswer));
	}
	ClrSFlag(blAnswer);
	cgRequestSocket = tSocket;
	cgApproveSecond = retSecond;
	*cgApproveSecond = 0;
	SetSFlag(blRequest);
	while((tSocket->IsConnected() != 0) && (CheckSFlag(blAnswer) == 0))
		SYS_SleepMS(10);
	if (tSocket->IsConnected() == 0)
		cgApproveSecond = nullptr;
	return(CheckSFlag(blAnswer));
};
//------------------------------------------------------------------------------------------//
void LicenseSSLServer::RejectRegistration(void){
	if (cgApproveSecond != nullptr)
		*cgApproveSecond = 0;
	ClrSFlag(blRequest);
	SetSFlag(blAnswer);
	Spin_Request_Unlock();
};
//------------------------------------------------------------------------------------------//
void LicenseSSLServer::ApproveRegistration(const uint64 &approveSecond){
	if (cgApproveSecond != nullptr)
		*cgApproveSecond = approveSecond;
	ClrSFlag(blRequest);
	SetSFlag(blAnswer);
	Spin_Request_Unlock();
}
//------------------------------------------------------------------------------------------//
const STDSTR &LicenseSSLServer::RequestSocketInfo(STDSTR *strPrint){
	*strPrint  = SYS_MakeTimeNow();
	*strPrint += "Receive registration request from ";
	*strPrint += cgRequestSocket->GetDevName();
	return(*strPrint);
}
//------------------------------------------------------------------------------------------//
#endif








