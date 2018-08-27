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
KERNEL::KERNEL(uint32 sizeCache,uint32 sizeBuffer) : COMMU_FRAME(sizeBuffer)
#ifdef ODEV_System_h
			,m_Cache(sizeCache)
#endif
#ifdef Device_h
			,m_DeviceM(sizeBuffer,&m_Cache)
			,m_DeviceA(sizeBuffer,&m_Cache)
			,m_DeviceS(sizeBuffer,&m_Cache)
#endif
#ifdef Commu_AEXE_h
			,m_AExePool(sizeBuffer < 1024 * 8 ? sizeBuffer : 1024 * 8,nullptr)
#endif
#ifdef Commu_Terminal_h
			,m_TS(sizeBuffer,&m_env,&m_BIC)
#endif
#ifdef Commu_RST_h
			,m_RSTServer(sizeBuffer,&m_env,&m_BIC)
			,m_RSTClient(sizeBuffer,nullptr,&m_BIC)
#endif
#ifdef Console_h
			,m_Console(sizeBuffer < 1024 * 8 ? sizeBuffer : 1024 * 8)
#endif
#ifdef SWVERSION_SCRIPT
			,m_Script(sizeBuffer < 1024 * 16 ? sizeBuffer : 1024 * 16)
#endif
#ifdef SWVERSION_CMUX
			,m_CMUXDriver(sizeBuffer,nullptr)
#endif
{
	SetSFlag(CF_blNoInitSize);
	SetSelfName("SDT");
};
//------------------------------------------------------------------------------------------//
KERNEL::~KERNEL(void){
#ifdef Device_h
	m_DeviceM.RemoveSelf();
	m_DeviceA.RemoveSelf();
#endif
#ifdef USE_OPENSSL
	CRYPTO_cleanup_all_ex_data();
	EVP_cleanup();
#endif
};
//------------------------------------------------------------------------------------------//
void KERNEL::Init(const STDSTR& fileName){
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
	m_DeviceM.SetSelfName("DeviceM");
	B_SetFLAG64(m_DeviceM.GetLogSystem()->envcfg, 0 | ODEV_FLAG_EnView | ODEV_FLAG_EnOSPMsgLine | ODEV_FLAG_EnMSReport);
	B_ClrFLAG64(m_DeviceM.GetLogSystem()->envcfg, 0 | ODEV_FLAG_EnHEXViewMode | ODEV_FLAG_EnRecMsg | ODEV_FLAG_EnEscape);
	
	m_DeviceA.SetSelfName("DeviceA");
	B_SetFLAG64(m_DeviceA.GetLogSystem()->envcfg, 0 | ODEV_FLAG_EnView | ODEV_FLAG_EnRecMsg | ODEV_FLAG_EnEscape);
	B_ClrFLAG64(m_DeviceA.GetLogSystem()->envcfg, 0 | ODEV_FLAG_EnHEXViewMode | ODEV_FLAG_EnOSPMsgLine | ODEV_FLAG_EnMSReport);
	m_DeviceA.cgEDA.aCOM.name = "/dev/ttySDT";
	
	m_DeviceM.LinkCoupleNode(&m_DeviceA);
	
	Add(m_DeviceM) < m_DeviceA;
	
	m_DeviceS.SetSelfName("DeviceS");
	B_SetFLAG64(m_DeviceS.GetLogSystem()->envcfg, 0 | ODEV_FLAG_EnView | ODEV_FLAG_EnOSPMsgLine | ODEV_FLAG_EnMSReport);
	B_ClrFLAG64(m_DeviceS.GetLogSystem()->envcfg, 0 | ODEV_FLAG_EnHEXViewMode | ODEV_FLAG_EnRecMsg | ODEV_FLAG_EnEscape);

#endif
#if defined	SWVERSION_CMUX && defined BIC_Device_h
	m_CMUXDriver.Init(&m_DeviceM);
#endif
#ifndef CommonDefH_VC
	#ifdef ODEV_System_h
	m_Cache.CreateG1_STDOUT(OUTPUT_NODE::COLType_COL);
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
	BIC_ENV_DEV::SetEDA			(&m_env,&m_DeviceM.cgEDA);
	BIC_ENV_DEV::SetEDA_M		(&m_env,&m_DeviceM.cgEDA);
	BIC_ENV_DEV::SetEDA_A		(&m_env,&m_DeviceA.cgEDA);
	BIC_ENV_DEV::SetEDA_S		(&m_env,&m_DeviceS.cgEDA);
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
void KERNEL::InitSTDOUT(ODEV_STDOUT* oDevSTDOUT){
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
void KERNEL::Run(const STDSTR& cmd){
#ifndef CommonDefH_VC
	#ifdef ODEV_System_h
		m_Cache.Start();	//call in MFC timer thread
	#endif
#endif
#if defined Console_h && defined BIC_CONSOLE_h
	m_BIC.Dispose(&m_env,"main",nullptr);
	m_Console.LoadDefault("console.ini");
	if (cmd.length() > 0)
		m_Console.ExecBIC(cmd);
	m_Console.StartWithBIC(&m_env,&m_BIC);
#endif
}
//------------------------------------------------------------------------------------------//
bool32 KERNEL::ExecBIC(const STDSTR& cmd){
#ifdef CommonDefH_VC
	CHK_CheckTime();
#endif
#ifdef BIC_CONSOLE_h
	m_Console.ExecBIC(cmd);
	return G_TRUE;
#else
	return G_FALSE;
#endif
};
//------------------------------------------------------------------------------------------//
void KERNEL::Exit(const STDSTR& fileName){
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
	m_DeviceA.RemoveSelf();
	m_DeviceM.RemoveSelf();
	m_DeviceA.Close();
	m_DeviceM.Close();
	m_DeviceM.UnlinkCoupleNode();
	m_DeviceS.Close();
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
void KERNEL::CloseChild(COMMU_FRAME* commu){
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
void KERNEL::ParRecordLoad(const STDSTR& fileName){
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
void KERNEL::ParRecordSave(const STDSTR& fileName){
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
