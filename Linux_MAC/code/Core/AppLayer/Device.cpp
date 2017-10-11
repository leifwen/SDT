/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: Device.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2012.12.12
 * @2013.7.05 :	use PUB_RECORD to replace Memo,RichMemo
 * @2013.6.30 :	change to Liunx style
 use int32 to replace bool
 use int32 to replace uint32
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "Device.h"
#include "Comm_Convert.h"
#include "SBIC.h"
#include "SMC.h"
//------------------------------------------------------------------------------------------//
#ifdef DeviceH
//------------------------------------------------------------------------------------------//
int32 DEVICE::OpenDev(const STDSTR &tCDBufName,int32 tCDBufPar,CSType tCSType,int32 blEnEcho){
	int32	ret;
	SMC_EncryptI(0)
	SMC_EncryptI(1)
	SMC_EncryptI(2)
	SMC_EncryptI(3)
	SMC_EncryptI(4)
	
	ret = 0;
	cgEDA.csType = tCSType;
	switch(tCSType){
#ifdef SWVERSION_COM
		case CSType_COM:
		case CSType_COMV:
			SMC_EncryptS(0)
			ret = Open_Com(tCDBufName,tCDBufPar,tCSType,blEnEcho);
			SMC_EncryptE(0)
			break;
#endif
#ifdef SWVERSION_TCPClient
		case CSType_TCP:
			SMC_EncryptS(1)
			ret = Open_Socket(tCDBufName,tCDBufPar,tCSType,blEnEcho);
			SMC_EncryptE(1)
			break;
#endif
#ifdef SWVERSION_UDPCLient
		case CSType_UDP:
			SMC_EncryptS(2)
			ret = Open_Socket(tCDBufName,tCDBufPar,tCSType,blEnEcho);
			SMC_EncryptE(2)
			break;
#endif
#ifdef SWVERSION_TCPServer
		case CSType_TCPS:
			SMC_EncryptS(3)
			ret = Open_Server(tCDBufName,tCDBufPar,tCSType,blEnEcho);
			SMC_EncryptE(3)
			break;
#endif
#ifdef SWVERSION_UDPServer
		case CSType_UDPS:
			SMC_EncryptS(4)
			ret = Open_Server(tCDBufName,tCDBufPar,tCSType,blEnEcho);
			SMC_EncryptE(4)
			break;
#endif
		default:
			break;
	}
	return ret;
}
//------------------------------------------------------------------------------------------//
void DEVICE::CloseDev(void){
	Close_Com();
	Close_Socket();
	Close_Server();
	ChildClrSel(GetSelDB());
}
//------------------------------------------------------------------------------------------//
void DEVICE::EnableEcho(void){
	COMMU_DBUF_FRAME_FW::EnableEcho();
	if (GetSelDB() != this)
		static_cast<COMMU_DBUF_FRAME*>(GetSelDB())->EnableEcho();
};
//------------------------------------------------------------------------------------------//
void DEVICE::DisableEcho(void){
	COMMU_DBUF_FRAME_FW::DisableEcho();
	if (GetSelDB() != this)
		static_cast<COMMU_DBUF_FRAME*>(GetSelDB())->DisableEcho();
};
//------------------------------------------------------------------------------------------//
const ACOM *DEVICE::CreateCOM(void){
	if (cgCom == nullptr){
		cgCom = new ACOM(cgBufMaxSize,&cgLogSystem);
		if (cgCom != nullptr){
			cgCom->UseExternalFwSBufList(&cgInternalFwSBufList);
			cgCom->SetSelfName(selfName + "::ACOM");
		}
		AddSon(cgCom);
	}
	return(cgCom);
}
//------------------------------------------------------------------------------------------//
const ASOCKET *DEVICE::CreateSocket(void){
	if (cgSocket == nullptr){
		cgSocket = new ASOCKET(cgBufMaxSize,&cgLogSystem);
		if (cgSocket != nullptr){
			cgSocket->UseExternalFwSBufList(&cgInternalFwSBufList);
			cgSocket->SetSelfName(selfName + "::ASOCKET");
		}
		AddSon(cgSocket);
	}
	return(cgSocket);
}
//------------------------------------------------------------------------------------------//
const ASOCKETSERVER *DEVICE::CreateServer(void){
	if (cgServer == nullptr){
		cgServer = new ASOCKETSERVER(cgBufMaxSize,&cgLogSystem);
		if (cgServer != nullptr){
			cgServer->UseExternalFwSBufList(&cgInternalFwSBufList);
			cgServer->SetSelfName(selfName + "::ASERVER");
		}
		AddSon(cgServer);
	}
	return(cgServer);
}
//------------------------------------------------------------------------------------------//
int32 DEVICE::Open_Com(const STDSTR &tCDBufName,int32 tCDBufPar,CSType tCSType,int32 blEnEcho){

	if (CreateCOM() == nullptr)
		return 0;
	ChildSetSel(cgCom);
	SetOpenPar(cgEDA.aCOM,tCDBufName,tCDBufPar,tCSType,blEnEcho);
	return(cgCom->OpenD(tCDBufName,tCDBufPar,tCSType,blEnEcho));
}
//------------------------------------------------------------------------------------------//
void DEVICE::Close_Com(void){
	if (cgCom != nullptr)
		cgCom->CloseD();
}
//------------------------------------------------------------------------------------------//
int32 DEVICE::Open_Socket(const STDSTR &tCDBufName,int32 tCDBufPar,CSType tCSType,int32 blEnEcho){
	
	if (CreateSocket() == nullptr)
		return 0;
	ChildSetSel(cgSocket);
	if (tCSType == CSType_TCP){
		SetOpenPar(cgEDA.aTCP,tCDBufName,tCDBufPar,tCSType,blEnEcho);
	}
	else{
		SetOpenPar(cgEDA.aUDP,tCDBufName,tCDBufPar,tCSType,blEnEcho);
	}
	return(cgSocket->OpenD(tCDBufName,tCDBufPar,tCSType,blEnEcho));
}
//------------------------------------------------------------------------------------------//
void DEVICE::Close_Socket(void){
	if (cgSocket != nullptr)
		cgSocket->CloseD();
}
//------------------------------------------------------------------------------------------//
int32 DEVICE::Open_Server(const STDSTR &tCDBufName,int32 tCDBufPar,CSType tCSType,int32 blEnEcho){
	if (CreateServer() == nullptr)
		return 0;
	ChildSetSel(cgServer);
	if (tCSType == CSType_TCPS){
		SetOpenPar(cgEDA.aTCPS,tCDBufName,tCDBufPar,tCSType,blEnEcho);
	}
	else{
		SetOpenPar(cgEDA.aUDPS,tCDBufName,tCDBufPar,tCSType,blEnEcho);
	}
	return(cgServer->OpenD(tCDBufName,tCDBufPar,tCSType,blEnEcho));
}
//------------------------------------------------------------------------------------------//
void DEVICE::Close_Server(void){
	if (cgServer != nullptr)
		cgServer->CloseD();
}
//------------------------------------------------------------------------------------------//
uint32 DEVICE::SendHexCommand(const STDSTR &hexcmd){
	uint32		ByteNum;
	ByteNum = 0;
	
	if (IsConnected() != 0)
		ByteNum = SSendInHEX(hexcmd);
	return(ByteNum);
}
//------------------------------------------------------------------------------------------//
uint32 DEVICE::SendHexCommandWithPrint(const STDSTR &hexcmd){
	STDSTR		strForPrint;
	uint32		ByteNum;
	ByteNum = 0;
	
	if (IsConnected() != 0){
		ByteNum = SSendInHEX(hexcmd);
		strForPrint = Str_HEXToHEXs(hexcmd);
		PrintSendCommand(strForPrint, ByteNum);
	}
	return(ByteNum);
}
//------------------------------------------------------------------------------------------//
int32 DEVICE::SendHexCommand(const STDSTR &hexcmd,const STDSTR &strCheck,SBUFFER *cSBUF,uint32 waitMS,uint32 delyMS){
	cSBUF->Clean();
	if (SendHexCommand(hexcmd) > 0){
		if (cSBUF == nullptr){
			SYS_SleepMS(delyMS);
			return 1;
		}
#ifdef SBIC_ExpressionH
		if (cgSBIC_Exp.CheckResult(cSBUF,strCheck,waitMS) > 0){
			SYS_SleepMS(delyMS);
			return 1;
		}
#else
		return 1;
#endif
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
uint32 DEVICE::SendCommand(const STDSTR &cmd,CMD_TAIL tail,G_ESCAPE blEscape){
	STDSTR		strForSend;
	uint32		ByteNum;
	ByteNum = 0;
	
	if (IsConnected() != 0){
#ifdef SBIC_BuildH
		cgSBIC_PRINT.CreateCommand(nullptr,&strForSend,nullptr,cmd,tail,blEscape);
#endif
		ByteNum = SSend(strForSend,G_ESCAPE_OFF);
	}
	return(ByteNum);
}
//------------------------------------------------------------------------------------------//
int32 DEVICE::SendCommand(const STDSTR &cmd,CMD_TAIL tail,G_ESCAPE blEscape,const STDSTR &strCheck,SBUFFER *cSBUF,uint32 waitMS,uint32 delyMS){
	cSBUF->Clean();
	if (SendCommand(cmd,tail,blEscape) > 0){
		if (cSBUF == nullptr){
			SYS_SleepMS(delyMS);
			return 1;
		}
		#ifdef SBIC_ExpressionH
		if (cgSBIC_Exp.CheckResult(cSBUF,strCheck,waitMS) > 0){
			SYS_SleepMS(delyMS);
			return 1;
		}
		#else
		return 1;
		#endif
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
uint32 DEVICE::SendCommandWithPrint(const STDSTR &cmd,CMD_TAIL tail,G_ESCAPE blEscape){
	STDSTR		strForSend,strForPrint;
	int32		ByteNum;
	ByteNum = 0;
	
	if (IsConnected() != 0){
	#ifdef SBIC_BuildH
		cgSBIC_PRINT.CreateCommand(nullptr,&strForSend,&strForPrint,cmd,tail,blEscape);
	#endif
		ByteNum = SSend(strForSend,G_ESCAPE_OFF);
		PrintSendCommand(strForPrint, ByteNum);
	}
	return(ByteNum);
}
//------------------------------------------------------------------------------------------//
int32 DEVICE::SendCommandWithPrint(const STDSTR &cmd,CMD_TAIL tail,G_ESCAPE blEscape,const STDSTR &strCheck,SBUFFER *cSBUF,uint32 waitMS,uint32 delyMS){
	cSBUF->Clean();
	if (SendCommandWithPrint(cmd,tail,blEscape) > 0){
		if (cSBUF == nullptr){
			SYS_SleepMS(delyMS);
			return 1;
		}
	#ifdef SBIC_ExpressionH
		if (cgSBIC_Exp.CheckResult(cSBUF,strCheck,waitMS) > 0){
			SYS_SleepMS(delyMS);
			return 1;
		}
	#else
		return 1;
	#endif
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
void DEVICE::PrintSendCommand(const STDSTR &strPrintData,uint32 num){
	OUTPUT_NODE	*selStdout;
	Spin_Child_set();
	selStdout = GetSelStdout();
	if (GetSelDB() == AServer()){
		*selStdout << Start << NL
		<< COL_DivLineTime
		<< COL_DB_TxTitle << MakeTxTitle("",static_cast<COMMU_DBUF*>(GetSelDB()->GetSelDB())->GetDevName(),num, STxBytes()+ num, 0) << COL_DB_TxText << strPrintData << Endl;
	}
	else{
		*selStdout << Start << NL
		<< COL_DivLineTime
		<< COL_DB_TxTitle << MakeTxTitle(num, STxBytes()+ num) << COL_DB_TxText << SpaceStr(strPrintData) << NL << Endl;
	}
	Spin_Child_clr();
}
//------------------------------------------------------------------------------------------//
void DEVICE::PrintSendCommand(const STDSTR &strTitle1,const STDSTR &strTitle2,const STDSTR &strPrintData){
	OUTPUT_NODE	*selStdout;
	Spin_Child_set();
	selStdout = GetSelStdout();
	*selStdout << Start << NL
	<< COL_DivLineTime
	<< COL_DB_TxTitle << SpaceStr(strTitle1) << SpaceStr(strTitle2) << COL_DB_TxText << strPrintData << NL << Endl;
	Spin_Child_clr();
}
//------------------------------------------------------------------------------------------//
void DEVICE::PrintSendStrWOG1(const STDSTR &strPrintData){
	OUTPUT_NODE	*selStdout;
	Spin_Child_set();
	selStdout = GetSelStdout();
	*selStdout << Start << ClrGroup(COLRECORD::CRD_G1) << NL << COL_DB_TxText << strPrintData << Endl;
	Spin_Child_clr();
}
//------------------------------------------------------------------------------------------//
STDSTR DEVICE::MakeFrmTitle(void){
	STDSTR	strRet;
	strRet = "";
	switch (GetCSType()){
		case CSType_TCP:
			strRet = "TCP:";
			strRet += cgEDA.aTCP.name;
			strRet += "@";
			strRet += Str_ToString(cgEDA.aTCP.port);
			break;
		case CSType_UDP:
			strRet = "UDP:";
			strRet += cgEDA.aUDP.name;
			strRet += "@";
			strRet += Str_ToString(cgEDA.aUDP.port);
			break;
		case CSType_TCPS:
			strRet = "TCP SERVER";
			strRet += "@";
			strRet += Str_ToString(cgEDA.aTCPS.port);
			break;
		case CSType_UDPS:
			strRet = "UDP SERVER";
			strRet += "@";
			strRet += Str_ToString(cgEDA.aUDPS.port);
			break;
		case CSType_COM:
		case CSType_COMV:
			strRet = cgEDA.aCOM.name;
			strRet += "@";
			strRet += Str_ToString(cgEDA.aCOM.port);
			break;
		default:
			break;
	}
	Str_UpperCaseSelf(strRet);
	return(strRet);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
DEVICE_EXE_FRAME::DEVICE_EXE_FRAME(void) : TREE_NODE(){
	cgSBUF.Init(BUF_MAX_SIZE);
	Init(nullptr);
}
//------------------------------------------------------------------------------------------//
void DEVICE_EXE_FRAME::Init(const DEVICE *tDevice){
	cgDevice = (DEVICE*)tDevice;
	cgSBUF.Clean();
	ClrblIsTerminated();
	Disable();
}
//------------------------------------------------------------------------------------------//
void DEVICE_EXE_FRAME::Stop(void){
	SetblIsTerminated();
	executeThread.ThreadStop();
	Init(nullptr);
}
//------------------------------------------------------------------------------------------//
int32 DEVICE_EXE_FRAME::CheckExecute(const DEVICE *tDevice){
	if (tDevice == nullptr)
		return 0;
	if (IsStop() == 0)
		return 0;
	if (InDoing_try() == 0)
		return 0;
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 DEVICE_EXE_FRAME::Execute(const DEVICE *tDevice,SYS_AThread *threadP){
	SMC_EncryptI(0)
	Stop();
	Init(tDevice);
	Enable();
	executeThread.ThreadInit(this, &DEVICE_EXE_FRAME::ExecuteThreadFun,"executeThread",threadP);
	SMC_EncryptS(0)
	executeThread.ThreadRun();
	SMC_EncryptE(0)
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 DEVICE_EXE_FRAME::ExecuteThreadFun(void *p){
	ThreadType	*threadP = static_cast<ThreadType*>(p);
	
	cgDevice->RxDataShareTo(&cgSBUF);
	
	threadP->Execute();
	
	SYS_SleepMS(10);
	cgSBUF.RemoveSelf();
	Init(nullptr);
	InDoing_clr();
	return 1;
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif

