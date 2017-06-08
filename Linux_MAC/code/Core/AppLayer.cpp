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
#ifdef SWVERSION_SCRIPT
	ParRecordLoad("Default.ini");
#endif
#ifdef DeviceH
	//-----------------------------------//
	m_Device.SetSelfName("Device1");
	B_SetFLAG64(m_Device.GetLogSystem()->envcfg, 0 | ODEV_FLAG_EnView | ODEV_FLAG_EnOSPMsgLine | ODEV_FLAG_EnMSReport);
	B_ClrFLAG64(m_Device.GetLogSystem()->envcfg, 0 | ODEV_FLAG_EnHEXViewMode | ODEV_FLAG_EnRecMsg | ODEV_FLAG_EnEscape);
	
	m_Device2.SetSelfName("Device2");
	B_SetFLAG64(m_Device2.GetLogSystem()->envcfg, 0 | ODEV_FLAG_EnView | ODEV_FLAG_EnRecMsg | ODEV_FLAG_EnEscape);
	B_ClrFLAG64(m_Device2.GetLogSystem()->envcfg, 0 | ODEV_FLAG_EnHEXViewMode | ODEV_FLAG_EnOSPMsgLine | ODEV_FLAG_EnMSReport);
	m_Device2.cgEDA.aCOM.name = "/dev/ttySDT";
	
	m_Device.LinkCoupleNode(&m_Device2);
	
	Add(m_Device) < m_Device2;
	//-----------------------------------//
#endif
	//m_LogCache.CreateG2_FILE(ODEV_FILE::CreateNewLOGFileName());
#ifndef CommonDefH_VC
	#ifdef ODEV_SystemH
	m_LogCache.CreateG1_STDOUT(OUTPUT_NODE::COLType_COL);
	m_LogCache.Start();//call in MFC timer thread
	#endif
#endif
#ifdef ConsoleH
	m_Console.Init(this);
#endif
}
//------------------------------------------------------------------------------------------//
void SDTAPP::Exit(void){
#ifdef SWVERSION_SCRIPT
	ParRecordSave("Default.ini");
#endif
#ifdef Commu_EXE_hpp
	m_AExePool.CloseD();
#endif
#ifdef TerminalH
	m_TS.CloseD();
#endif
#ifdef Terminal_RSTH
	m_RSTCilent.CloseD();
	m_RST.CloseD();
#endif
#ifdef Commu_NTPServerH
	m_NTPServer.CloseD();
#endif
#ifdef ScriptH
	m_Script.Stop();
#endif
#ifdef SendFilesH
	m_FileSend.Stop();
#endif
#ifdef CMUXH
	m_CMUXDriver.CloseD();
#endif
#ifdef DeviceH
	m_Device.RemoveSelf();
	m_Device2.RemoveSelf();
	m_Device2.CloseD();
	m_Device.CloseD();
#endif
#ifdef ConsoleH
	m_Console.Exit();
#endif
}
//------------------------------------------------------------------------------------------//
int32 SDTAPP::ExecBIC(BIC_ENV *env){
	CheckTime();
#ifdef BuildInCommandH
	return(m_BIC.Do(env));
#else
	return -1;
#endif
};
//------------------------------------------------------------------------------------------//
void SDTAPP::ExecBIC(const STDSTR &cmd){
	m_Console.ExecuteBICCMD(cmd);
};
//------------------------------------------------------------------------------------------//
void SDTAPP::ChildClose(COMMU_DBUF_FRAME *commu){
	commu->CloseD();
	if (m_Script.CheckServiceTo((DEVICE*)commu) != 0)
		m_Script.Stop();
	if (m_FileSend.CheckServiceTo((DEVICE*)commu) != 0)
		m_FileSend.Stop();
}
//------------------------------------------------------------------------------------------//
void SDTAPP::ParRecordLoad(STDSTR strFileName){
#ifdef SWVERSION_SCRIPT
#ifdef ParRecordH
	uint32		readType;
	
	m_ParRecord.Init(&m_IPComList,&m_SCList,&m_GCList);
	readType = READINITYPE_NONE | READINITYPE_AddGroupCommand | READINITYPE_CoverGroupCommand;
	readType |= (READINITYPE_AddSingleCommand | READINITYPE_CoverSingleCommand);
	readType |= (READINITYPE_AddDeviceList | READINITYPE_CoverDeviceList);
	m_ParRecord.ReadScriptFromIniFile(strFileName,readType);
#endif
#endif
}
//------------------------------------------------------------------------------------------//
void SDTAPP::ParRecordSave(STDSTR strFileName){
#ifdef SWVERSION_SCRIPT
#ifdef ParRecordH
	uint32		readType;
	
	m_ParRecord.Init(&m_IPComList,&m_SCList,&m_GCList);
	readType = READINITYPE_NONE | READINITYPE_AddGroupCommand;
	readType |= (READINITYPE_AddSingleCommand);
	readType |= (READINITYPE_AddDeviceList);
	m_ParRecord.WriteScriptToIniFile(strFileName,readType);
#endif
#endif
}
//------------------------------------------------------------------------------------------//
int32 SDTAPP::CheckTime(void){
#ifdef CommonDefH_EnableLicense
	uint8	rand[4];
	if (--m_CheckTime < 0){
		RAND_bytes(rand,4);
		rand[0] = (uint32)sqrtl(rand[0]) + 1;
		rand[1] = (uint32)sqrtl(rand[1]) + 1;
		rand[2] = (uint32)sqrtl(rand[0]) + 1;
		rand[3] = (uint32)sqrtl(rand[1]) + 1;
		m_CheckTime = 20 * 50 * 60 * (rand[0] * 2 + 10);
		try{
			rand[0] /= CHK_CheckATime();
			rand[1] /= CHK_CheckBTime();
			rand[3] += rand[0],rand[1],rand[2];
		}
		catch(...){
			g_SMCCyCodeStream.FillZero();
			m_Device.CloseD();
			m_Device2.CloseD();
		}
	}
	return(rand[3]);
#else
	return 1;
#endif
}
//------------------------------------------------------------------------------------------//











//------------------------------------------------------------------------------------------//
#ifdef ConsoleH
int32 CON_CONSOLE::BICThreadFun(void *p){
#ifdef CommonDefH_EnableLicense
	SDTAPP	*sdt = static_cast<SDTAPP*>(p);
	CHK_CheckerInit();
	CHK_ReadLinenseFile();
	sdt->CheckTime();
#endif
	return(CON_ReadInLine::BICThreadFun(p));
}
#endif
//------------------------------------------------------------------------------------------//

