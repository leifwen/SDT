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

#include "ODEV_LIST.h"
#include "Device.h"

#include "Script.h"
#include "ComEnum.h"
#include "CommandSList.h"
#include "CommandGList.h"
#include "ParRecord.h"

#include "TelitCMUX.h"
#include "SendFiles.h"
#include "Commu_NTPServer.h"

#include "BuildInCommand.h"
#include "Console.h"
#include "Terminal.h"

#ifdef CommonDefH_VC
#include "SDT.h"
#endif
//------------------------------------------------------------------------------------------//
class SDTAPP{
	public:
				 SDTAPP(void){ m_blExit = 0; cgBufMaxSize = 0; };
		virtual ~SDTAPP(void){;};
	public:
		SYS_DateTime	m_LoginTime;
		uint32			cgBufMaxSize;
		int32			m_blExit;
	public:
		SYS_ThreadEx<SDTAPP>		m_PrintThread;
		SYS_ThreadEx<SDTAPP>		m_ConsoleThread;
	private:
		int32	PrintThreadFun		(void);
		void	DoOutput			(void);
		int32	ConsoleThreadFun	(void);
	private:
		void	ParRecordLoad		(std::string strFileName);
		void	ParRecordSave		(std::string strFileName);
	public:
		ODEV_LIST		m_oDevList;
		ODEV_LIST_POOL	m_oDevOutputListPool;
		DEVICE			m_Device;		//Main
		DEVICE			m_Device2;		//Aux
	public:
		Script			m_Script;
		IPCOMLIST		m_IPComList;
		SC_LIST			m_SCList;
		GC_LIST			m_GCList;
		PR_Record		m_ParRecord;
	public:
		#ifdef	SWVERSION_CMUX
			CMUXDriver	m_CMUXDriver;
		#endif
		TFileSend		m_FileSend;
	    NTPServer		m_NTPServer;
	public:
	    TerminalServer	m_TS;
		BuildInCommand	m_BIC_Terminal;
	public:
		CON_ReadInLine	m_ReadInline;
		BICPAR			m_ConsoleBICPAR;
		BuildInCommand	m_BIC_Console;
	public:
		void		Init			(void);
		void		Exit			(void);
		int32		ExecBIC			(const std::string &tCommand);
};
//------------------------------------------------------------------------------------------//
extern	SDTAPP	GSDTApp;
//------------------------------------------------------------------------------------------//
#endif
