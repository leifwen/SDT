//
//  AppLayer.h
//  SDT
//
//  Created by Leif Wen on 20/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "Global.h"
//------------------------------------------------------------------------------------------//
#ifndef AppLayer_h
#define AppLayer_h
#ifdef AppLayer_h
//------------------------------------------------------------------------------------------//
class SDTAPP : public COMMU_FRAME{
	public:
				 SDTAPP(uint32 size);
		virtual ~SDTAPP(void);
	public:
#ifdef ODEV_System_h
		ODEV_CACHE		m_Cache;
#endif
#ifdef Commu_ComEnum_h
		IPCOMLIST		m_IPComList;
#endif
#if defined SWVERSION_SCRIPT && defined SList_h
		SC_LIST			m_SCList;
#endif
#if defined SWVERSION_SCRIPT && defined GList_h
		GC_LIST			m_GCList;
#endif
#ifdef Device_h
		DEVICE			m_Device1;		//Main
		DEVICE			m_Device2;		//Aux
#endif
#ifdef Commu_AEXE_h
		AEXEPOOL		m_AExePool;
#endif
#ifdef Commu_NTPServer_h
		NTPServer		m_NTPServer;
#endif
#ifdef Commu_Terminal_h
		TerminalServer	m_TS;
#endif
#ifdef Commu_RST_h
		RSTBServer		m_RSTServer;
		RSTBSocket		m_RSTClient;
#endif
#ifdef Console_h
		AppConsole		m_Console;
#endif
#ifdef CMD_h
		CMD_ENV			m_env;
#endif
#ifdef BIC_CONSOLE_h
		AppBIC			m_BIC;
#endif
#ifdef SWVERSION_SCRIPT
		SCRIPT			m_Script;
#endif
#ifdef SendFile_h
		TFileSend		m_FileSend;
#endif
#ifdef SWVERSION_CMUX
		CMUXDriver		m_CMUXDriver;
#endif
#ifdef ParRecord_h
		PR_Record		m_ParRecord;
#endif
	private:
				void	ParRecordLoad	(const STDSTR& fileName);
				void	ParRecordSave	(const STDSTR& fileName);
	public:
		virtual	void	CloseChild		(COMMU_FRAME* commu);
				void	Init			(const STDSTR& fileName);
				void	Run				(void);
				void	Exit			(const STDSTR& fileName);
				bool32	ExecBIC			(const STDSTR& cmd);
	public:
#ifdef CommonDefH_VC
				void	InitSTDOUT		(ODEV_STDOUT* oDevSTDOUT);
#endif
};
//------------------------------------------------------------------------------------------//
#endif /* AppLayer_h */
#endif /* AppLayer_h */
