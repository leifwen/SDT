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
#ifndef ScriptH
#define ScriptH
//------------------------------------------------------------------------------------------//
#include "SBIC.h"
#include "SYS_Thread.h"
#include "Comm_Buffer.h"
#include "CommandGList.h"
//------------------------------------------------------------------------------------------//
#define		TELIT_GPRS_RC_OK				(0)
#define		TELIT_GPRS_RC_CONNECT			(1)
#define		TELIT_GPRS_RC_RING				(2)
#define		TELIT_GPRS_RC_NOCARRIER			(3)
#define		TELIT_GPRS_RC_ERROR				(4)
#define		TELIT_GPRS_RC_CONNECT1200		(5)
#define		TELIT_GPRS_RC_NODIALTONE		(6)
#define		TELIT_GPRS_RC_BUSY				(7)
#define		TELIT_GPRS_RC_NOANSWER			(8)
#define		TELIT_GPRS_RC_NOCOMPLETE		(9)
#define		TELIT_GPRS_RC_NORESPONSE		(10)

#define		TELIT_GPRS_RESPONSE		\
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
class DEVICE;
//------------------------------------------------------------------------------------------//
class Script : public SBIC_Node{
	public:
		enum{
			#ifdef G_MAXBUFFER_SIZE
				BUF_MAX_SIZE = G_MAXBUFFER_SIZE
			#else
				BUF_MAX_SIZE = 1024 * 32
			#endif
		};
		enum{RFLAG_C = 2, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
				 Script(void);
		virtual ~Script(void);
	private:
		SBICPAR					cgSBICPAR;
		SBIC_Expression			cgSubC_Expression;

		SBIC_Delay				cgSubC_Delay;
		SBIC_DTR				cgSubC_DTR;
		SBIC_RTS				cgSubC_RTS;
		SBIC_ChangeCom			cgSubC_ChangeCom;
		SBIC_ChangeBR			cgSubC_ChangeBR;
		SBIC_OpenTCPSocket		cgSubC_OpenTCPSocket;
		SBIC_CloseTCPSocket		cgSubC_CloseTCPSocket;
		SBIC_OpenUDPSocket		cgSubC_OpenUDPSocket;
		SBIC_CloseUDPSocket		cgSubC_CloseUDPSocket;
		SBIC_SetRecvDataMode	cgSubC_SetRecvDataMode;
		SBIC_SetRecMsgReport	cgSubC_SetRecMsgReport;
		SBIC_Search				cgSubC_Search;
		SBIC_Synchronous		cgSubC_Synchronous;
		SBIC_Wait				cgSubC_Wait;
		SBIC_Lable				cgSubC_Lable;
		SBIC_GOTO				cgSubC_GOTO;
		SBIC_STOP				cgSubC_STOP;
		SBIC_BREAK				cgSubC_BREAK;
		SBIC_Print				cgSubC_PRINT;
		#ifdef CommonDefH_Unix
			SBIC_RE_LIST			cgSBIC_RE_LIST;
			SBIC_RunExternal		cgSubC_RunExternal;
			SBIC_StopExternal		cgSubC_StopExternal;
		#endif
	private:
		PUB_SBUF			cgBuffer;//receive buffer.
		int32				cgBufMaxSize;
		std::string			cgATCondition;
		void	PrintGroupResult			(COMMAND_GROUP *tGroup,const std::string &strGroupName);
        void    ShowResult                  (COMMAND_GROUP *tGroup,const std::string &tExecuteTime);
		void	ShowResult					(GC_LIST *tGroupList,const std::string &tExecuteTime);
		int32	ExecuteGroupList			(GC_LIST *tGroupList,const int32 &runTotalTimes);
		int32	ExecuteGroup				(COMMAND_GROUP *tGroup,const int32 &runTotalTimes,int32 blType);
		int32	ExecuteCommand				(COMMAND_NODE *tCommand,int32 frameTimeout);
	private:
		void	SetblIsTerminated	(void)		{SetSFlag(RFLAG_CREATE(0));SetblExit(&cgSBICPAR);};
		void	ClrblIsTerminated	(void)		{ClrSFlag(RFLAG_CREATE(0));ClrblExit(&cgSBICPAR);};
		int32	IsTerminated		(void)const	{return(CheckSFlag(RFLAG_CREATE(0)));};
		void	SetblRSC			(void)		{SetSFlag(RFLAG_CREATE(1));cgSBICPAR.blRSC = 1;};
		void	ClrblRSC			(void)		{ClrSFlag(RFLAG_CREATE(1));cgSBICPAR.blRSC = 0;};
		int32	CheckblRSC			(void)const	{return(CheckSFlag(RFLAG_CREATE(1)));};
	private:
		SYS_ThreadEx<Script>		eGroupListThread;
		SYS_ThreadEx<Script>		eGroupThread;
		SYS_ThreadEx<Script>		eCommandThread;
		int32	EGroupListThreadFun	(void);
		int32	EGroupThreadFun		(void);
		int32	ECommandThreadFun	(void);
		void	Init				(void);

		DEVICE			*cgDevice;
	    GC_LIST			cgMGroupList;
	    COMMAND_GROUP	cgMGroup;
	    COMMAND_NODE	cgMCommand;
	public:
		void	Help				(ODEV_NODE *tODevNode);
		int32 	IsRunSocketCommand	(void)const{return(CheckblRSC());};
		int32 	IsStop				(void)const{return((CheckblEnabled()) == 0);};
		void	StopRun				(void);
	
		int32	Execute(const DEVICE *tDevice,const GC_LIST *tGroupList);
		int32	Execute(const DEVICE *tDevice,const COMMAND_GROUP *tGroup);
		int32	Execute(const DEVICE *tDevice,const COMMAND_NODE *tCommand);
};
//------------------------------------------------------------------------------------------//
#endif