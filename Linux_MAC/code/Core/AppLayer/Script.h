/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: Script.h
 * File ID	:
 * Remark	:
 * Writer   : Leif Wen
 * Date     : 2012.12.15
 * @2013.6.30 :	change to Liunx style
 				use int32 to replace int32
 				use int32 to replace int32
*/

//------------------------------------------------------------------------------------------//
#include "SBIC_CMD.h"
#include "Device.h"
#include "CommandGList.h"
//------------------------------------------------------------------------------------------//
#if defined SBIC_CMD_hpp && defined DeviceH
#ifndef ScriptH
#define ScriptH
#ifdef ScriptH
//------------------------------------------------------------------------------------------//
#define		TELIT_AT_RESPONSE		\
"[\r\nOK\r\n\
 || \r\nCONNECT\r\n\
 || \r\nRING\r\n\
 || \r\nNO CARRIER\r\n\
 || \r\nERROR\r\n\
 || +CME ERROR:\
 || \r\n+CME ERROR:\
 || \r\nCONNECT *\r\n\
 || \r\nNO DIALTONE\r\n\
 || \r\nBUSY\r\n\
 || \r\nNO ANSWER\r\n]"
//------------------------------------------------------------------------------------------//
class Script : public DEVICE_EXE_FRAME{
	public:
				 Script(void);
		virtual ~Script(void){;};
	private:
		virtual	void	Init				(const DEVICE *tDevice);
		virtual	void	SetblIsTerminated	(void){DEVICE_EXE_FRAME::SetblIsTerminated();SBIC_CMD::SetblExit(&cgSBICENV);};
		virtual	void	ClrblIsTerminated	(void){DEVICE_EXE_FRAME::ClrblIsTerminated();SBIC_CMD::ClrblExit(&cgSBICENV);};
		inline	void	PrintB				(SBIC_ENV *env,const STDSTR &str){SBIC_CMD::PrintB(env,str);};
		inline	void	PrintP				(SBIC_ENV *env,const STDSTR &str){SBIC_CMD::PrintP(env,str);};
		inline	void	PrintM				(SBIC_ENV *env,const STDSTR &strM,const STDSTR &strB = ""){SBIC_CMD::PrintM(env,strM,strB);};
	private:
		SYS_Thread<Script>	eGroupListThread;
		SYS_Thread<Script>	eGroupThread;
		SYS_Thread<Script>	eCommandThread;
	private:
		int32	EGroupListThreadFun	(void *p);
		int32	EGroupThreadFun		(void *p);
		int32	ECommandThreadFun	(void *p);
	private:
		SBIC_ENV		cgSBICENV;
		SBIC_CMD		cgSBICCMD;
		SBIC_REPLACE	cgSubC_REPLACE;
		#ifdef SBIC_RunEXEH
		AEXEPOOL		cgAExePool;
		#endif
		STDSTR			cgATCondition;
		GC_LIST			cgMGroupList;
		COMMAND_GROUP	cgMGroup;
		COMMAND_NODE	cgMCommand;
	public:
		void	SetblCommandExplain	(void){SBIC_CMD::SetblCommandExplain(&cgSBICENV);};
		void	ClrblCommandExplain	(void){SBIC_CMD::ClrblCommandExplain(&cgSBICENV);};
		int32	CheckCommandExplain	(void){return(SBIC_CMD::CheckCommandExplain(&cgSBICENV));};
	
		void	SetblATResponse		(void){SBIC_CMD::SetblATResponse(&cgSBICENV);};
		void	ClrblATResponse		(void){SBIC_CMD::ClrblATResponse(&cgSBICENV);};
		int32	CheckATResponse		(void){return(SBIC_CMD::CheckATResponse(&cgSBICENV));};
	
		void	SetblPrintSBICinfo	(void){SBIC_CMD::SetblPrintSBICinfo(&cgSBICENV);};
		void	ClrblPrintSBICinfo	(void){SBIC_CMD::ClrblPrintSBICinfo(&cgSBICENV);};
		int32	CheckPrintSBICinfo	(void){return(SBIC_CMD::CheckPrintSBICinfo(&cgSBICENV));};
	private:
		void	PrintGroupResult	(OUTPUT_NODE* cOut,COMMAND_GROUP *tGroup,const STDSTR &strGroupName);
		void    ShowResult			(COMMAND_GROUP *tGroup,const STDSTR &tExecuteTime);
		void	ShowResult			(GC_LIST *tGroupList,const STDSTR &tExecuteTime);
		int32	ExecuteGroupList	(GC_LIST *tGroupList,const int32 &runTotalTimes);
		int32	ExecuteGroup		(COMMAND_GROUP *tGroup,const int32 &runTotalTimes,int32 blType);
		int32	ExecuteCommand		(COMMAND_NODE *tCommand,int32 frameTimeout);
	public:
		virtual	void	Stop		(void);
	public:
		void	Help				(OUTPUT_NODE *oDevNode);
	public:
		int32	Execute			(const DEVICE *tDevice,const GC_LIST *tGroupList);
		int32	Execute			(const DEVICE *tDevice,const COMMAND_GROUP *tGroup);
		int32	Execute			(const DEVICE *tDevice,const COMMAND_NODE *tCommand);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif
#endif
#endif
