/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: SBIC_B.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.08.12
*/
#ifndef SBIC_BH
#define SBIC_BH
//------------------------------------------------------------------------------------------//
#include "Comm_Tree.h"
#include "ODEV_Node.h"
//------------------------------------------------------------------------------------------//
enum{
	SBI_RETCODE_EXIT 	= -1,
	SBI_RETCODE_NO		= 0,
	SBI_RETCODE_HELP,
	SBI_RETCODE_EXPRESSION,
	
	SBI_RETCODE_SQ,
	SBI_RETCODE_HEX,
	SBI_RETCODE_TIME,
	SBI_RETCODE_RET,
	SBI_RETCODE_STRING,
	SBI_RETCODE_COMBINE,
	SBI_RETCODE_PRINT,
	
	SBI_RETCODE_DELAY,
	SBI_RETCODE_WAIT,
	SBI_RETCODE_DTR,
	SBI_RETCODE_RTS,
	SBI_RETCODE_CHANGECOM,
	SBI_RETCODE_CHANGEBR,
	SBI_RETCODE_OPENTCPSOCKET,
	SBI_RETCODE_CLOSETCPSOCKET,
	SBI_RETCODE_OPENUDPSOCKET,
	SBI_RETCODE_CLOSEUDPSOCKET,
	SBI_RETCODE_SETRECVDATAMODE,
	SBI_RETCODE_SETRECMSGREPORT,
	SBI_RETCODE_SYNCHRONOUS,
	
	SBI_RETCODE_RUNEXTERNAL,
	SBI_RETCODE_STOPEXTERNAL,
	SBI_RETCODE_LABLE,
	SBI_RETCODE_GOTO,
	SBI_RETCODE_STOP,
	SBI_RETCODE_BREAK,
	SBI_RETCODE_SEARCH,
	
	SBI_RETCODE_RECEIVE,
	SBI_RETCODE_CTS,
	SBI_RETCODE_DSR,
	SBI_RETCODE_RING,
	SBI_RETCODE_DCD,
	SBI_RETCODE_TIMEOUT,
	SBI_RETCODE_nullptr,
	SBI_RETCODE_RX_B,
	SBI_RETCODE_RX_S,
	SBI_RETCODE_RX_E,
	SBI_RETCODE_LCS,
	SBI_RETCODE_CONDITION,
    
    SBI_RETCODE_CALC_MD5,
	SBI_RETCODE_CALC_SHA1,
	SBI_RETCODE_CALC_SHA224,
	SBI_RETCODE_CALC_SHA256,
	SBI_RETCODE_CALC_SHA384,
	SBI_RETCODE_CALC_SHA512,
	SBI_RETCODE_CALC_BASE64,
};
//------------------------------------------------------------------------------------------//
class DEVICE;
class PUB_SBUF;
class COMMAND_NODE;
struct SYS_TIME_S;
class SBIC_RE_LIST;
//------------------------------------------------------------------------------------------//
struct SBICPAR{
	ODEV_NODE		*cgODevNode;
	DEVICE			*cgDevice;
	PUB_SBUF		*cgBuffer;
	std::string		cgRecvbuf;
	std::string		cgSearchRet;
	std::string		cgFunRet;
	int32			blExit;
	int32			blRSC;
	int32			blCMDRet;
	COMMAND_NODE	*cgCommand;
	SYS_TIME_S		*cgTimeS;
	#ifdef CommonDefH_Unix
		SBIC_RE_LIST	*cgSBIC_RE_LIST;
	#endif
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
class SBIC_Node : public RTREE_NODE{
	public:
		enum{RFLAG_C = 0, RFLAG_S = RTREE_NODE::RFLAG_S + RTREE_NODE::RFLAG_C};
	public:
		SBIC_Node(void) : RTREE_NODE() {
			cgCommand = "";
			cgReturnCode = SBI_RETCODE_NO;
		};
		virtual ~SBIC_Node(void){;};
	public:
		static	void		SetblExit	(SBICPAR *tBICPAR){if ((tBICPAR != nullptr) && (tBICPAR->blExit != -1)) tBICPAR->blExit = 1;};
		static	void		ClrblExit	(SBICPAR *tBICPAR){if ((tBICPAR != nullptr) && (tBICPAR->blExit != -1)) tBICPAR->blExit = 0;};
		static	int32		ClkblExit	(SBICPAR *tBICPAR){return((tBICPAR != nullptr) && (tBICPAR->blExit != 0));};
		static	std::string	DelComment	(const std::string &strInput);
        static	int32		GetMS		(const std::string &par);
		static	std::string GetMSSTR	(int32 ms);
	public:
		static	void		PrintExecute(SBICPAR *tBICPAR,const std::string &strCommand);
		static	void		PrintB		(SBICPAR *tBICPAR,const std::string &str){
			if ((tBICPAR != nullptr) && (tBICPAR->cgODevNode != nullptr))
				tBICPAR->cgODevNode->WriteToStrN(str + "\r\n",RICH_CF_clBlue,COLSTRING::COL_EP_YES);
		};
		static	void		PrintP		(SBICPAR *tBICPAR,const std::string &str){
			if ((tBICPAR != nullptr) && (tBICPAR->cgODevNode != nullptr))
				tBICPAR->cgODevNode->WriteToStrN(str + "\r\n",RICH_CF_clPurple,COLSTRING::COL_EP_YES);
		};
		static	void		PrintM		(SBICPAR *tBICPAR,const std::string &strM,const std::string &strB = ""){
			if ((tBICPAR != nullptr) && (tBICPAR->cgODevNode != nullptr))
				tBICPAR->cgODevNode->Spin_InUse_set();
				tBICPAR->cgODevNode->WriteToStrN(strM,RICH_CF_clMaroon,COLSTRING::COL_EP_YES,G_LOCK_OFF);
				tBICPAR->cgODevNode->WriteToStr(strB + "\r\n",RICH_CF_clBlue,COLSTRING::COL_EP_YES,G_LOCK_OFF);
				tBICPAR->cgODevNode->Spin_InUse_clr();
		};
	protected:
		std::string		cgCommand;
		int32			cgReturnCode;
	public:
		int32	ExecuteLC		(SBICPAR *tBICPAR,const std::string &fullCMD,std::string *ret)const;
		int32	HelpLC			(SBICPAR *tBICPAR,int32 blDetail = 1)const;
	
		virtual	int32	Check	(SBICPAR *tBICPAR,const std::string &fullCMD,std::string *retPar)const;
		virtual	int32	Execute	(SBICPAR *tBICPAR,const std::string &fullCMD,std::string *ret)const;
		virtual	int32	Help	(SBICPAR *tBICPAR,int32 blDetail = 1)const;
		virtual	int32	Command	(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const;
};
//------------------------------------------------------------------------------------------//
class SBIC_Delay : public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
		SBIC_Delay(void) : SBIC_Node() {
			cgCommand = "delay,=";
			cgReturnCode = SBI_RETCODE_DELAY;
		};
		virtual ~SBIC_Delay(void){;};
	public:
		virtual	int32	Command	(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(SBICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class SBIC_DTR : public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
		SBIC_DTR(void) : SBIC_Node() {
			cgCommand = "dtr,=";
			cgReturnCode = SBI_RETCODE_DTR;
		};
		virtual ~SBIC_DTR(void){;};
	public:
		virtual	int32	Command	(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(SBICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class SBIC_RTS : public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
		SBIC_RTS(void) : SBIC_Node() {
			cgCommand = "rts,=";
			cgReturnCode = SBI_RETCODE_RTS;
		};
		virtual ~SBIC_RTS(void){;};
	public:
		virtual	int32	Command	(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(SBICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class SBIC_ChangeCom : public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
		SBIC_ChangeCom(void) : SBIC_Node() {
			cgCommand = "ChangeCom,=";
			cgReturnCode = SBI_RETCODE_CHANGECOM;
		};
		virtual ~SBIC_ChangeCom(void){;};
	public:
		virtual	int32	Command	(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(SBICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class SBIC_ChangeBR : public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
		SBIC_ChangeBR(void) : SBIC_Node() {
			cgCommand = "ChangeBR,=";
			cgReturnCode = SBI_RETCODE_CHANGEBR;
		};
		virtual ~SBIC_ChangeBR(void){;};
	public:
		virtual	int32	Command	(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(SBICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class SBIC_OpenTCPSocket : public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
		SBIC_OpenTCPSocket(void) : SBIC_Node() {
			cgCommand = "OpenTCPSocket,=/OTCP,=";
			cgReturnCode = SBI_RETCODE_OPENTCPSOCKET;
		};
		virtual ~SBIC_OpenTCPSocket(void){;};
	public:
		virtual	int32	Command	(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(SBICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class SBIC_CloseTCPSocket : public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
		SBIC_CloseTCPSocket(void) : SBIC_Node() {
			cgCommand = "CloseTCPSocket,=/CTCP,=";
			cgReturnCode = SBI_RETCODE_CLOSETCPSOCKET;
		};
		virtual ~SBIC_CloseTCPSocket(void){;};
	public:
		virtual	int32	Command	(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(SBICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class SBIC_OpenUDPSocket : public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C };
	public:
		SBIC_OpenUDPSocket(void) : SBIC_Node() {
			cgCommand = "OpenUDPSocket,=/OUDP,=";
			cgReturnCode = SBI_RETCODE_OPENUDPSOCKET;
		};
		virtual ~SBIC_OpenUDPSocket(void){ ; };
	public:
		virtual	int32	Command(SBICPAR *tBICPAR, const std::string &par, std::string *ret)const;
		virtual	int32	Help(SBICPAR *tBICPAR, int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class SBIC_CloseUDPSocket : public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C };
	public:
		SBIC_CloseUDPSocket(void) : SBIC_Node() {
			cgCommand = "CloseUDPSocket,=/CUDP,=";
			cgReturnCode = SBI_RETCODE_CLOSEUDPSOCKET;
		};
		virtual ~SBIC_CloseUDPSocket(void){ ; };
	public:
		virtual	int32	Command(SBICPAR *tBICPAR, const std::string &par, std::string *ret)const;
		virtual	int32	Help(SBICPAR *tBICPAR, int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class SBIC_SetRecvDataMode : public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
		SBIC_SetRecvDataMode(void) : SBIC_Node() {
			cgCommand = "SetRecvDataMode,=";
			cgReturnCode = SBI_RETCODE_SETRECVDATAMODE;
		};
		virtual ~SBIC_SetRecvDataMode(void){;};
	public:
		virtual	int32	Command	(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(SBICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class SBIC_SetRecMsgReport : public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
		SBIC_SetRecMsgReport(void) : SBIC_Node() {
			cgCommand = "SetRecMsgReport,=";
			cgReturnCode = SBI_RETCODE_SETRECMSGREPORT;
		};
		virtual ~SBIC_SetRecMsgReport(void){;};
	public:
		virtual	int32	Command	(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(SBICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class SBIC_Synchronous : public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
		SBIC_Synchronous(void) : SBIC_Node() {
			cgCommand = "Synchronous,=/Sync,=";
			cgReturnCode = SBI_RETCODE_SYNCHRONOUS;
		};
		virtual ~SBIC_Synchronous(void){;};
	public:
		virtual	int32	Command	(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(SBICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif
