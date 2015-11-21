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
#include "License_Checker.h"
//------------------------------------------------------------------------------------------//
#ifdef USE_OPENSSL
//------------------------------------------------------------------------------------------//
enum{
	MESG_Request_License,
	MESG_ANS_License,
	MESG_LoopBack_ATime,
};
//------------------------------------------------------------------------------------------//
int32 LicenseSSLSocket::PrintThreadFun(void){
	std::string			strMesg,strContent;
	uint32 				mID,kbps;
	CCY_FNLC_MESG		fnMesg;
	FIFO_UINT8			rxfifo;
	Linense_Signature	tLS;
	
	kbps = 8;
	rxfifo.Init(1024 * 16);
	fnMesg.Init(&rxfifo);
	while((printThread.IsTerminated() == 0) && (IsConnected() > 0)){
		rxfifo.Empty();
		if (CSSL_T1D_CH1.RX_Packaging(&rxfifo,kbps) > 0){
			if (fnMesg.AnalysisFrame(rxfifo) > 0){
				fnMesg.ReadContent(&strMesg, &mID,&rxfifo);
				rxfifo.Empty();
				switch (mID){
					case MESG_Request_License:
						//MyLogPrint(this, "Rec MESG_Request_License");
						((LicenseSSLServer*)GetFather(this))->SendRegistrationMesg(strMesg,GetdRNodeID(this));
						break;
					case MESG_ANS_License:
						//MyLogPrint(this, "Rec MESG_ANS_License");
						CFS_WriteFile("License.key", strMesg);
						SetblANSLicense();
						break;
					default:
						break;
				}
			}
		}
		SYS_SleepMS(10);
	}
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 LicenseSSLSocket::CheckANSLicense(uint32 waitTimeS){
	SYS_TIME_S	Timedly;
	SYS_Delay_SetTS(&Timedly, waitTimeS << 10);
	while((CheckblANSLicense() == 0) && (IsConnected() > 0) && (SYS_Delay_CheckTS(&Timedly) == 0))
		SYS_DelayMS(100);
	return(CheckblANSLicense());
}
//------------------------------------------------------------------------------------------//
int32 LicenseSSLSocket::GetLicense(uint32 waitTimeS,std::string *retStatus){
	CCY_FNLC_MESG	fnMesg;
	FIFO_UINT8		rxfifo;
	Reg_Signature	regS;
	std::string		strReg,retStatusT;
	int32			ret;
	
	regS.selfName = "GetLicense";
	rxfifo.Init(1024 * 16);
	fnMesg.Init(&rxfifo);
	
	ret = regS.Encode(&strReg,&retStatusT);
	while(ret > 0){
		retStatusT = "Create registration data successful.\r\n";
		retStatusT += "Send registration data to server.\r\n";
		CSSL_T1D_CH1.TX_Packaging(fnMesg.SetContent(strReg, MESG_Request_License),8);
		ret = 0;
		if (CheckANSLicense(waitTimeS) == 0){
			retStatusT += "Timeout for getting License.key successful.\r\n";
			break;
		}
		ret = 1;
		retStatusT += "Update License.key successful.\r\n";
		break;
	}
	if (retStatus != nullptr)
		*retStatus = retStatusT;
	return(ret);
}
//------------------------------------------------------------------------------------------//








//------------------------------------------------------------------------------------------//
void LicenseSSLServer::RejectRegistration(void){
	uint32	mID;
	LicenseSSLSocketS	*lSocket;

	mID = cgfnMesg.fn_MesgID.GetValueCalc();
	cgfnMesg.Out();

	Spin_InUse_set();
	lSocket = (LicenseSSLSocketS*)FindInLChildRChainByDRNodeID(this, mID);
	if (lSocket != nullptr)
		lSocket->CloseD();
	Spin_InUse_clr();
};
//------------------------------------------------------------------------------------------//
void LicenseSSLServer::ApproveRegistration(const uint64 &approveSecond){
	std::string			strContent,strMesg;
	uint32				mID;
	LicenseSSLSocketS	*lSocket;
	Linense_Signature	tLS;
	CCY_FNLC_MESG		fnMesg;
	FIFO_UINT8			rxfifo;
	
	rxfifo.Init(1024 * 16);
	fnMesg.Init(&rxfifo);
	
	Spin_InUse_set();
	cgfnMesg.ReadContent(&strMesg, &mID);
	cgfnMesg.Out();
	lSocket = (LicenseSSLSocketS*)FindInLChildRChainByDRNodeID(this, mID);
	if (lSocket != nullptr){
		if (tLS.Encode(&strContent, strMesg, Str_DecToHex(approveSecond)) > 0)
			lSocket->SSLWrite(fnMesg.SetContent(strContent,MESG_ANS_License),8);
		lSocket->CloseD();
	}
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
int32 LicenseSSLServer::GetRegistration(uint32 *mID){
	if (cgLicenseSBUF.cgBufFIFO.Used() > 0){
		if (cgfnMesg.AnalysisFrame(cgLicenseSBUF.cgBufFIFO) > 0){
			*mID = cgfnMesg.fn_MesgID.GetValueCalc();
			return 1;
		}
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
#endif







