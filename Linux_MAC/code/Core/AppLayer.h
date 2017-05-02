/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: AppLayer.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2013.11.28
*/
#ifndef AppLayerH
#define AppLayerH
//------------------------------------------------------------------------------------------//
#include "BasicDef.h"
#include "BasicClass.h"
#include "SYS_Thread.h"

#include "ODEV_System.h"
#include "Device.h"

#include "Script.h"
#include "ComEnum.h"
#include "CommandSList.h"
#include "CommandGList.h"
#include "ParRecord.h"

#include "CMUX.h"
#include "SendFiles.h"
#include "Commu_RMS.h"
#include "Commu_NTPServer.h"

#include "BuildInCommand.h"
#include "Console.h"
#include "Terminal.h"

#ifdef CommonDefH_VC
#include "SDT.h"
#endif
//------------------------------------------------------------------------------------------//
#ifdef ConsoleH
class CON_CONSOLE : public CON_ReadInLine{
	public:
				 CON_CONSOLE(void):CON_ReadInLine(){;};
		virtual ~CON_CONSOLE(void){;};
	public:
		virtual	int32	BICThreadFun(void *p);
};
#endif
//------------------------------------------------------------------------------------------//
class SDTAPP : public COMMU_DBUF_FRAME{
	public:
		virtual ~SDTAPP(void){;};
				 SDTAPP(int32 tSize = G_MAXBUFFER_SIZE)
					: COMMU_DBUF_FRAME(0)
				#ifdef ODEV_SystemH
					,m_LogCache(1024 * 1024 * 32)
				#endif
				#ifdef DeviceH
					,m_Device(tSize,&m_LogCache)
					,m_Device2(tSize,&m_LogCache)
				#endif
				#ifdef Commu_EXE_hpp
					,m_AExePool(tSize,nullptr)
				#endif
				#ifdef	CMUXH
					,m_CMUXDriver(tSize,&m_Device)
				#endif
				#ifdef TerminalH
					,m_TS(tSize,this)
				#endif
				#ifdef Terminal_RSTH
					,m_RST(tSize,this)
					,m_RSTCilent(tSize,this)
				#endif
				{m_blExit = 0; cgBufMaxSize = tSize; m_CheckTime = 0;};
	public:
	#ifdef ODEV_SystemH
		ODEV_CACHE		m_LogCache;
	#endif
	#ifdef ComEnumH
		IPCOMLIST		m_IPComList;
	#endif
	#ifdef CommandSlistH
		SC_LIST			m_SCList;
	#endif
	#ifdef CommandGListH
		GC_LIST			m_GCList;
	#endif
	#ifdef ParRecordH
		PR_Record		m_ParRecord;
	#endif
	#ifdef DeviceH
		DEVICE			m_Device;		//Main
		DEVICE			m_Device2;		//Aux
	#endif
	#ifdef Commu_EXE_hpp
		AEXEPOOL		m_AExePool;
	#endif
	#ifdef ScriptH
		Script			m_Script;
	#endif
	#ifdef SendFilesH
		TFileSend		m_FileSend;
	#endif
	#ifdef	CMUXH
		CMUXDriver		m_CMUXDriver;
	#endif
	#ifdef Commu_NTPServerH
		NTPServer		m_NTPServer;
	#endif
	#ifdef TerminalH
		TerminalServer	m_TS;
	#endif
	#ifdef Terminal_RSTH
		RSTSocket		m_RSTCilent;
		RSTServer		m_RST;
	#endif
	#ifdef BIC_CMDH
		BuildInCommand	m_BIC;
	#endif
	#ifdef ConsoleH
		CON_CONSOLE		m_Console;
	#endif
	public:
		#ifdef BIC_DICTH
			DICTWORD_NODE	m_Dict;
			COCAWORD_NODE	m_COCA;
		#endif
	public:
		TIME	m_LoginTime;
		int32	m_blExit;
		int32	m_CheckTime;
	public:
		virtual	void	ChildClose	(COMMU_DBUF_FRAME *commu);
	private:
		void	ParRecordLoad	(STDSTR strFileName);
		void	ParRecordSave	(STDSTR strFileName);
	public:
		int32	CheckTime		(void);
		int32	ExecBIC			(BIC_ENV *env);
		void	ExecBIC			(const STDSTR &cmd);
	public:
		void	Init			(void);
		void	Exit			(void);
};

//------------------------------------------------------------------------------------------//
extern	SDTAPP	GSDTApp;
//------------------------------------------------------------------------------------------//
#endif
