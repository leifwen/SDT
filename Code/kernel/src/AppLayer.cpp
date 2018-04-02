//
//  AppLayer.cpp
//  SDT
//
//  Created by Leif Wen on 20/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
#include "AppLayer.h"
#ifdef AppLayer_h
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
SDTAPP::SDTAPP(uint32 size) : COMMU_FRAME(size)
#ifdef ODEV_System_h
			,m_Cache(1024 * 1024 * 32)
#endif
#ifdef Device_h
			,m_Device1(size,&m_Cache)
			,m_Device2(size,&m_Cache)
#endif
#ifdef Commu_AEXE_h
			,m_AExePool(1024 * 8,nullptr)
#endif
#ifdef Commu_Terminal_h
			,m_TS(size,&m_env,&m_BIC)
#endif
#ifdef Commu_RST_h
			,m_RSTServer(size,&m_env,&m_BIC)
			,m_RSTClient(size,nullptr,&m_BIC)
#endif
#ifdef Console_h
			,m_Console(1024 * 8)
#endif
#ifdef SWVERSION_SCRIPT
			,m_Script(1024 * 16)
#endif
#ifdef SWVERSION_CMUX
			,m_CMUXDriver(size,nullptr)
#endif
{
	SetSFlag(CF_blNoInitSize);
	SetSelfName("SDT");
};
//------------------------------------------------------------------------------------------//
SDTAPP::~SDTAPP(void){
#ifdef Device_h
	m_Device1.RemoveSelf();
	m_Device2.RemoveSelf();
#endif
#ifdef USE_OPENSSL
	CRYPTO_cleanup_all_ex_data();
	EVP_cleanup();
#endif
};
//------------------------------------------------------------------------------------------//
void SDTAPP::Init(const STDSTR& fileName){
#ifdef USE_OPENSSL
	CY_Init();
#endif
#if defined CommonDefH_VC && defined Commu_Socket_h
	CheckWSAStartup();
#endif
#ifdef SWVERSION_SCRIPT
	ParRecordLoad(fileName);
#endif
#ifdef BIC_Device_h
	m_Device1.SetSelfName("Device1");
	B_SetFLAG64(m_Device1.GetLogSystem()->envcfg, 0 | ODEV_FLAG_EnView | ODEV_FLAG_EnOSPMsgLine | ODEV_FLAG_EnMSReport);
	B_ClrFLAG64(m_Device1.GetLogSystem()->envcfg, 0 | ODEV_FLAG_EnHEXViewMode | ODEV_FLAG_EnRecMsg | ODEV_FLAG_EnEscape);
	
	m_Device2.SetSelfName("Device2");
	B_SetFLAG64(m_Device2.GetLogSystem()->envcfg, 0 | ODEV_FLAG_EnView | ODEV_FLAG_EnRecMsg | ODEV_FLAG_EnEscape);
	B_ClrFLAG64(m_Device2.GetLogSystem()->envcfg, 0 | ODEV_FLAG_EnHEXViewMode | ODEV_FLAG_EnOSPMsgLine | ODEV_FLAG_EnMSReport);
	m_Device2.cgEDA.aCOM.name = "/dev/ttySDT";
	
	m_Device1.LinkCoupleNode(&m_Device2);
	
	Add(m_Device1) < m_Device2;
#endif
#if defined	SWVERSION_CMUX && defined BIC_Device_h
	m_CMUXDriver.Init(&m_Device1);
#endif
#ifndef CommonDefH_VC
	#ifdef ODEV_System_h
	m_Cache.CreateG1_STDOUT(OUTPUT_NODE::COLType_COL);
	m_Cache.Start();										//call in MFC timer thread
	#ifdef Console_h
		m_Console.Init(m_Cache.GetG1_STDOUT());
	#endif
	#endif
#endif

#ifdef BIC_Dev_h
	BIC_ENV_DEV::InitQuantity(&m_env);
	BIC_ENV_DEV::Init(&m_env);
#else
	#ifdef BIC_Commu_h
		BIC_ENV_COMMU::InitQuantity(&m_env);
		BIC_ENV_COMMU::Init(&m_env);
	#else
		#ifdef BIC_Define_h
			BIC_ENV::InitQuantity(&m_env);
			BIC_ENV::Init(&m_env);
		#else
			#ifdef CMD_h
			m_env.InitQuantity(CMD_ENV::CMD_VID_NEXT);
			#endif
		#endif
	#endif
#endif
#if defined CMD_h && defined ODEV_System_h
	CMD_ENV::SetSTDOUT			(&m_env,m_Cache.GetG1_STDOUT());
#endif
#ifdef BIC_Define_h
	#ifdef ODEV_System_h
	BIC_ENV::SetCache			(&m_env,&m_Cache);
	#endif
	#if defined SWVERSION_SCRIPT && defined SList_h
	BIC_ENV::SetSCList			(&m_env,&m_SCList);
	#endif
	#if defined SWVERSION_SCRIPT && defined GList_h
	BIC_ENV::SetGCList			(&m_env,&m_GCList);
	#endif
#endif
#ifdef BIC_Commu_h
	#ifdef Commu_AEXE_h
	BIC_ENV_COMMU::SetAExePool	(&m_env,&m_AExePool);
	#endif
	#ifdef Commu_NTPServer_h
	BIC_ENV_COMMU::SetNTPS		(&m_env,&m_NTPServer);
	#endif
	#ifdef Commu_Terminal_h
	BIC_ENV_COMMU::SetTS		(&m_env,&m_TS);
	#endif
	#ifdef Commu_RST_h
	BIC_ENV_COMMU::SetRSTServer	(&m_env,&m_RSTServer);
	BIC_ENV_COMMU::SetRSTClient	(&m_env,&m_RSTClient);
	#endif
#endif
#ifdef BIC_Dev_h
	#ifdef Device_h
	BIC_ENV_DEV::SetEDA			(&m_env,&m_Device1.cgEDA);
	BIC_ENV_DEV::SetEDA1		(&m_env,&m_Device1.cgEDA);
	BIC_ENV_DEV::SetEDA2		(&m_env,&m_Device2.cgEDA);
	#endif

	#ifdef Commu_ComEnum_h
	BIC_ENV_DEV::SetValidComList(&m_env,&m_IPComList);
	#endif
	#ifdef Script_h
	BIC_ENV_DEV::SetScript		(&m_env,&m_Script);
	#endif
	#ifdef SendFile_h
	BIC_ENV_DEV::SetFileSend	(&m_env,&m_FileSend);
	#endif
	#ifdef SWVERSION_CMUX
	BIC_ENV_DEV::SetCMUX		(&m_env,&m_CMUXDriver);
	#endif
#endif
#ifdef Commu_RST_h
	m_RSTClient.Init(&m_env);
#endif
}
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_VC
//------------------------------------------------------------------------------------------//
void SDTAPP::InitSTDOUT(ODEV_STDOUT* oDevSTDOUT){
#ifdef ODEV_System_h
		m_Cache.AddG1_STDOUT(oDevSTDOUT);
	#ifdef Console_h
		m_Console.Init(oDevSTDOUT);
	#endif
	#ifdef CMD_h
		CMD_ENV::SetSTDOUT(&m_env,oDevSTDOUT);
	#endif
#endif
};
//------------------------------------------------------------------------------------------//
#endif
//------------------------------------------------------------------------------------------//
void SDTAPP::Run(void){
#if defined Console_h && defined BIC_CONSOLE_h
	m_BIC.Dispose(&m_env,"main",nullptr);
	m_Console.LoadDefault("console.ini");
	m_Console.StartWithBIC(&m_env,&m_BIC);
#endif
}
//------------------------------------------------------------------------------------------//
bool32 SDTAPP::ExecBIC(const STDSTR& cmd){
#ifdef CommonDefH_VC
	CHK_CheckTime();
#endif
#ifdef BIC_CONSOLE_h
	return(m_BIC.Command(&m_env,cmd,nullptr));
#endif
	return -1;
};
//------------------------------------------------------------------------------------------//
void SDTAPP::Exit(const STDSTR& fileName){
#ifdef Commu_AEXE_h
	m_AExePool.Close();
#endif
#ifdef Commu_NTPServer_h
	m_NTPServer.Close();
#endif
#ifdef Commu_Terminal_h
	m_TS.Close();
#endif
#ifdef Commu_RST_h
	m_RSTServer.Close();
	m_RSTClient.Close();
#endif
#ifdef Script_h
	m_Script.Stop();
#endif
#ifdef SendFile_h
	m_FileSend.Stop();
#endif
#ifdef SWVERSION_CMUX
	m_CMUXDriver.Close();
#endif
#ifdef BIC_Dev_h
	m_Device2.RemoveSelf();
	m_Device1.RemoveSelf();
	m_Device2.Close();
	m_Device1.Close();
	m_Device1.UnlinkCoupleNode();
#endif
#ifdef ODEV_System_h
	m_Cache.Stop();
#endif
#if defined Console_h
	m_Console.Stop();
#endif
#ifdef BIC_Dev_h
	BIC_ENV_DEV::DeInit(&m_env);
#else
	#ifdef BIC_Commu_h
		BIC_ENV_COMMU::DeInit(&m_env);
	#else
		#ifdef BIC_Define_h
			BIC_ENV::DeInit(&m_env);
		#endif
	#endif
#endif
#ifdef SWVERSION_SCRIPT
	ParRecordSave(fileName);
#endif
#ifdef Commu_ComEnum_h
	m_IPComList.Empty();
#endif
#if defined SWVERSION_SCRIPT && defined SList_h
	m_SCList.Empty();
#endif
#if defined SWVERSION_SCRIPT && defined GList_h
	m_GCList.Empty();
#endif
#ifdef CommonDefH_VC
	CHK_CheckerInitClear();
#endif
#ifdef USE_OPENSSL
	CRYPTO_cleanup_all_ex_data();
	EVP_cleanup();
#endif
}
//------------------------------------------------------------------------------------------//
void SDTAPP::CloseChild(COMMU_FRAME* commu){
	commu->Close();
#ifdef SWVERSION_SCRIPT
	if (m_Script.IsServiceTo((DEVICE*)commu))
		m_Script.Stop();
#endif
#ifdef SendFile_h
	if (m_FileSend.IsServiceTo((DEVICE*)commu))
		m_FileSend.Stop();
#endif
};
//------------------------------------------------------------------------------------------//
void SDTAPP::ParRecordLoad(const STDSTR& fileName){
#ifdef SWVERSION_SCRIPT
#ifdef ParRecord_h
	uint32		cfg;
	
	m_ParRecord.Init(&m_IPComList,&m_SCList,&m_GCList);
	cfg = READINITYPE_NONE;
	cfg |= (READINITYPE_AddGroupCommand | READINITYPE_CoverGroupCommand);
	cfg |= (READINITYPE_AddSingleCommand | READINITYPE_CoverSingleCommand);
	cfg |= (READINITYPE_AddDeviceList | READINITYPE_CoverDeviceList);
	m_ParRecord.Load(fileName,cfg);
#endif
#endif
}
//------------------------------------------------------------------------------------------//
void SDTAPP::ParRecordSave(const STDSTR& fileName){
#ifdef SWVERSION_SCRIPT
#ifdef ParRecord_h
	uint32		cfg;
	
	m_ParRecord.Init(&m_IPComList,&m_SCList,&m_GCList);
	cfg = READINITYPE_NONE | READINITYPE_AddGroupCommand;
	cfg |= (READINITYPE_AddSingleCommand);
	cfg |= (READINITYPE_AddDeviceList);
	m_ParRecord.Save(fileName,cfg);
#endif
#endif
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif /* AppLayer_h */
