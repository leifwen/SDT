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
//#define LOGPRINT_ENABLE
#include "License_SSLSocket.h"
#include "Comm_Convert.h"
#include "SYS_Time.h"
#include "Comm_File.h"
#include "License_Checker.h"
//------------------------------------------------------------------------------------------//
#ifdef USE_OPENSSL
//------------------------------------------------------------------------------------------//
enum{
	MESG_REQ_License = MESG_NEXT_SSL,
	MESG_ANS_Approve,
	MESG_ANS_Reject,
};
//------------------------------------------------------------------------------------------//
int32 LicenseSSLSocket::MessageProcessing(FNode_MESG *RecMesg,int32 blReady){
	std::string		strMesg,strContent;
	uint32 			mID;
	int32			blDo;
	static	Linense_Signature	tLS;
	static	uint64	approveSecond = 0;
	
	if (SSLSocket::MessageProcessing(RecMesg,blReady) > 0)
		return 1;
	
	if (blReady == 0)
		return 0;
	
	blDo = 1;
	RecMesg->ReadContent(&strMesg,&mID);
	switch (mID){
		case MESG_REQ_License:
			ELogPrint(this, "MessageProcessing()::Rec  MESG_REQ_License");
			if (((LicenseSSLServer*)GetFather(this))->CheckApprove(this,&approveSecond) > 0){
				if (approveSecond == 0){
					ELogPrint(this, "MessageProcessing()::Send MESG_ANS_Reject");
					CtrlCHWrite("",MESG_ANS_Reject);
				}
				else if (tLS.Encode(&strContent, strMesg, approveSecond) > 0){
					ELogPrint(this, "MessageProcessing()::Send MESG_ANS_Approve");
					CtrlCHWrite(strContent,MESG_ANS_Approve);
				}
				else{
					ELogPrint(this, "MessageProcessing()::Send MESG_ANS_Reject ,linense create fail");
					CtrlCHWrite("",MESG_ANS_Reject);
				}
			}
			//SendREQ_CloseSocket();
			PushSend();
			ClrblConnected();
			break;
		case MESG_ANS_Approve:
			ELogPrint(this, "MessageProcessing()::Rec  MESG_ANS_Approve");
			ELogPrint(this, "MessageProcessing()::Write to License.key");
			CFS_WriteFile("License.key", strMesg);
			SetblANSLicenseOK();
			SetblANSLicense();
			ClrblConnected();
			break;
		case MESG_ANS_Reject:
			ELogPrint(this, "MessageProcessing()::Rec  MESG_ANS_Reject");
			ClrblANSLicenseOK();
			SetblANSLicense();
			ClrblConnected();
			break;
		default:
			blDo = 0;
		break;
	}
	return(blDo);
}
//------------------------------------------------------------------------------------------//
int32 LicenseSSLSocket::CreateRegSignature(std::string *strReg,std::string *retStatus){
	Reg_Signature	regS;
	return(regS.Encode(strReg,retStatus));
}
//------------------------------------------------------------------------------------------//
int32 LicenseSSLSocket::SendRequestLicense(const std::string &regSignature,uint32 waitTimeS){
	SYS_TIME_S		Timedly;
	
	SYS_Delay_SetTS(&Timedly, waitTimeS << 10);	
	ClrblANSLicense();
	ClrblANSLicenseOK();
	ELogPrint(this, "Send MESG_Request_License");
	if (CtrlCHWrite(regSignature,MESG_REQ_License) > 0){
		while((CheckblANSLicense() == 0) && (IsConnected() > 0) && (SYS_Delay_CheckTS(&Timedly) == 0))
			SYS_SleepMS(100);
	}
	if (CheckblANSLicenseOK() > 0)
		return 1;
	if (CheckblANSLicense() > 0)
		return 0;
	return -1;
}
//------------------------------------------------------------------------------------------//








//------------------------------------------------------------------------------------------//
int32 LicenseSSLServer::CheckApprove(LicenseSSLSocket *tSocket,uint64 *retSecond){
	while (Spin_Request_Try() == 0){
		if (tSocket->IsConnected() == 0)
			return(CheckblApprove());
	}
	ClrblApprove();
	cgRequestSocket = tSocket;
	cgApproveSecond = retSecond;
	*cgApproveSecond = 0;
	SetblRequest();
	while((tSocket->IsConnected() > 0) && (CheckblApprove() == 0))
		SYS_SleepMS(10);
	if (tSocket->IsConnected() == 0)
		cgApproveSecond = nullptr;
	return(CheckblApprove());
};
//------------------------------------------------------------------------------------------//
void LicenseSSLServer::RejectRegistration(void){
	if (cgApproveSecond != nullptr)
		*cgApproveSecond = 0;
	ClrblRequset();
	SetblApprove();
	Spin_Request_Unlock();
};
//------------------------------------------------------------------------------------------//
void LicenseSSLServer::ApproveRegistration(const uint64 &approveSecond){
	if (cgApproveSecond != nullptr)
		*cgApproveSecond = approveSecond;
	ClrblRequset();
	SetblApprove();
	Spin_Request_Unlock();
}
//------------------------------------------------------------------------------------------//
const std::string &LicenseSSLServer::RequestSocketInfo(std::string *strPrint){
	SYS_DateTime	tNow;
	tNow.Now();
	*strPrint  = tNow.FormatDateTime("[hh:mm:ss.zzz]  ");
	*strPrint += "Receive registration request from ";
	*strPrint += cgRequestSocket->GetBufName();
	*strPrint += '@';
	*strPrint += Str_IntToString(cgRequestSocket->GetBufPar());
	*strPrint += ".\r\n";
	return(*strPrint);
}
//------------------------------------------------------------------------------------------//
#endif







