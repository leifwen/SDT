/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: Global.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2013.11.28
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "AppLayer.h"
#include "License_Checker.h"
#include "SMC.h"
#include <math.h>
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
SDTAPP	GSDTApp;
//------------------------------------------------------------------------------------------//
void SDTAPP::Init(void){
	m_blExit = 0;
#ifdef USE_OPENSSL
	CY_Init();
#endif
#ifdef CommonDefH_VC
	CheckWSAStartup();
#endif
	m_Script.StopRun();
	m_FileSend.StopSend();
#ifdef SWVERSION_SCRIPT
	ParRecordLoad("Default.ini");
#endif
	
	m_oDevOutputListPool.CreateODevFile(ODEV_CreateNewLOGFileName());
	m_oDevOutputListPool.CreateODevSDOUT(nullptr,nullptr,COLSTRING::COLType_COL);
	m_oDevList.cgOutput = &m_oDevOutputListPool;
	
	m_Device.Init(&m_oDevOutputListPool,cgBufMaxSize);
	m_Device2.Init(&m_oDevOutputListPool,cgBufMaxSize);
	
	B_SetFLAG64(m_Device.cgODevList.cgODevFlagU64, 0 | ODEV_FLAG_EnView | ODEV_FLAG_EnOSPMsgLine | ODEV_FLAG_EnMSReport);
	B_ClrFLAG64(m_Device.cgODevList.cgODevFlagU64, 0 | ODEV_FLAG_EnHEXViewMode | ODEV_FLAG_EnRecMsg | ODEV_FLAG_EnEscape);
	
	B_SetFLAG64(m_Device2.cgODevList.cgODevFlagU64, 0 | ODEV_FLAG_EnView | ODEV_FLAG_EnRecMsg | ODEV_FLAG_EnEscape);
	B_ClrFLAG64(m_Device2.cgODevList.cgODevFlagU64, 0 | ODEV_FLAG_EnHEXViewMode | ODEV_FLAG_EnOSPMsgLine | ODEV_FLAG_EnMSReport);
	
	m_Device.LinkCoupleNode(&m_Device2);
	m_Device2.cEDevFlag.com = "/dev/ttySDT";
	
#ifdef	SWVERSION_CMUX
	m_CMUXDriver.Init(&m_Device,cgBufMaxSize);
#endif

	m_ReadInline.Init(&m_ConsoleBICPAR,m_oDevOutputListPool.cODevSDOUT);

	m_PrintThread.ThreadInit(this,&SDTAPP::PrintThreadFun);
	m_ConsoleThread.ThreadInit(this,&SDTAPP::ConsoleThreadFun);

	m_PrintThread.ThreadRun();
	m_ConsoleThread.ThreadRun();
}
//------------------------------------------------------------------------------------------//
void SDTAPP::Exit(void){
#ifdef SWVERSION_SCRIPT
	ParRecordSave("Default.ini");
#endif
	m_TS.CloseD();
#ifdef USE_OPENSSL
	m_RSTCilent.CloseD();
	m_RST.CloseD();
#endif
	m_NTPServer.CloseD();
	m_Script.StopRun();
	m_FileSend.StopSend();
#ifdef	SWVERSION_CMUX
	m_CMUXDriver.CMUXStop();
#endif
	m_Device2.Close(0);
	m_Device.Close(0);
	m_ConsoleBICPAR.blExit = -1;
	m_ConsoleThread.ThreadStop();
	m_PrintThread.ThreadStop();
}
//------------------------------------------------------------------------------------------//
int32 SDTAPP::ExecBIC(const std::string &tCommand){
	std::string	ret;
	return(m_BIC_Console.ExecuteLC(&m_ConsoleBICPAR, tCommand, "", &ret));
}
//------------------------------------------------------------------------------------------//
void SDTAPP::ParRecordLoad(std::string strFileName){
#ifdef SWVERSION_SCRIPT
	uint32		readType;
	TExpandPar	tExpandPar;
	tExpandPar.g_IPComList = &m_IPComList;
	tExpandPar.g_SCList = &m_SCList;
	tExpandPar.g_GCList = &m_GCList;
	
	m_ParRecord.Init(&tExpandPar);
	readType = READINITYPE_NONE | READINITYPE_AddGroupCommand | READINITYPE_CoverGroupCommand;
	readType |= (READINITYPE_AddSingleCommand | READINITYPE_CoverSingleCommand);
	readType |= (READINITYPE_AddDeviceList | READINITYPE_CoverDeviceList);
	m_ParRecord.ReadScriptFromIniFile(strFileName,readType);
#endif
}
//------------------------------------------------------------------------------------------//
void SDTAPP::ParRecordSave(std::string strFileName){
#ifdef SWVERSION_SCRIPT
	uint32		readType;
	TExpandPar	tExpandPar;
	tExpandPar.g_IPComList = &m_IPComList;
	tExpandPar.g_SCList = &m_SCList;
	tExpandPar.g_GCList = &m_GCList;
	
	m_ParRecord.Init(&tExpandPar);
	readType = READINITYPE_NONE | READINITYPE_AddGroupCommand;
	readType |= (READINITYPE_AddSingleCommand);
	readType |= (READINITYPE_AddDeviceList);
	m_ParRecord.WriteScriptToIniFile(strFileName,readType);
#endif
}
//------------------------------------------------------------------------------------------//
int32 SDTAPP::PrintThreadFun(void){
#ifdef CommonDefH_EnableLicense
	uint32	checkTime;
	uint8	rand[4];
	rand[0] = 1;
	rand[1] = 1;
	rand[2] = 1;
	rand[3] = 1;
	CHK_CheckerInit();
	CHK_ReadLinenseFile();
	
	try{
		rand[0] = (uint8)sqrt(CHK_CheckATime());
		rand[1] /= (uint8)sqrt(CHK_CheckBTime());
	}
	catch(...){
		g_SMCCyCodeStream.FillZero();
		m_PrintThread.ClrblIsTerminated();
		m_Device.Close(0);
		m_Device2.Close(0);
	}
T_S:;
	RAND_bytes(rand,2);
	rand[0] = (uint32)sqrtl(rand[0]) + 1;
	rand[1] = (uint32)sqrtl(rand[1]) + 1;
	checkTime = 20 * 50 * 60 * (rand[0] * 2 + 10);
#endif
	
	while(m_PrintThread.IsTerminated() == 0){
		DoOutput();
		SYS_SleepMS(20);
		
#ifdef CommonDefH_EnableLicense
		if (--checkTime == 0){
			try{
				rand[0] /= CHK_CheckATime();
				rand[1] /= CHK_CheckBTime();
				rand[2] /= rand[2];
			}
			catch(...){
				g_SMCCyCodeStream.FillZero();
				m_PrintThread.ClrblIsTerminated();
				m_Device.Close(0);
			}
			goto T_S;
		}
#endif
	}
	return 1;
}
//------------------------------------------------------------------------------------------//
void SDTAPP::DoOutput(void){
	if (m_Device.cgODevList.cgOutput->CheckblEnabled() > 0)
		m_Device.UpdataUIRecord();
	if (m_Device2.cgODevList.cgOutput->CheckblEnabled() > 0)
		m_Device2.UpdataUIRecord();
#ifdef CommonDefH_Unix
	if ((m_Device.cgDevType == DEVICE::DEVID_TCPClient) && (m_Device.cgAPISocket != nullptr)){
		if (m_Device.cgAPISocket->CheckblSDC() > 0){
			if (m_Script.IsRunSocketCommand() == 0){
				m_Script.StopRun();
				m_Device.Close(0);
			}
		}
	}
	if ((m_Device.cgDevType == DEVICE::DEVID_APICOM) && (m_Device.cgAPIECom != nullptr)){
		if (m_Device.cgAPIECom->CheckblSDC() > 0){
			m_Script.StopRun();
			m_Device.Close(0);
		}
	}
	if ((m_Device2.cgDevType == DEVICE::DEVID_TCPClient) && (m_Device2.cgAPISocket != nullptr)){
		if (m_Device2.cgAPISocket->CheckblSDC() != 0)
			m_Device2.Close(0);
	}
	if ((m_Device2.cgDevType == DEVICE::DEVID_APICOM) && (m_Device2.cgAPIECom != nullptr)){
		if (m_Device2.cgAPIECom->CheckblSDC() != 0)
			m_Device2.Close(0);
	}
#endif
}
//------------------------------------------------------------------------------------------//
int32 SDTAPP::ConsoleThreadFun(void){
	PUB_SBUF	charBuf;
	
	charBuf.Init(1024 * 2);
	m_ConsoleBICPAR.sdtApp = this;
	m_ConsoleBICPAR.oDevNode = m_oDevOutputListPool.cODevSDOUT;
	m_ConsoleBICPAR.charSBUF = &charBuf;
	m_ConsoleBICPAR.validComList = &m_IPComList;
	m_ConsoleBICPAR.retCommand = "";
	m_ConsoleBICPAR.blExit = 0;
	m_ConsoleBICPAR.blInOnlineMode = 0;
	m_ConsoleBICPAR.blInPressKeyMode = 0;
	m_ConsoleBICPAR.blUseSecondLH = 0;
	m_ConsoleBICPAR.blDisplayAuto = 1;
	m_ConsoleBICPAR.gDID = 0;
	
	charBuf.Clean();
	
	while(m_ConsoleThread.IsTerminated() == 0){
		if (m_BIC_Console.Do(&m_ConsoleBICPAR) == -1)
			m_blExit = -1;
		if (m_ConsoleBICPAR.blExit == -1)
			break;
	}
	return 1;
}
//------------------------------------------------------------------------------------------//
