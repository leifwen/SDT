/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: BIC_B.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.01.25
*/
#ifndef BIC_BH
#define BIC_BH
//------------------------------------------------------------------------------------------//
#include "Comm_Tree.h"
#include "Device.h"
//------------------------------------------------------------------------------------------//
enum{
	BI_RETCODE_EXIT	= -1,
	BI_RETCODE_NO	= 0,
	BI_RETCODE_EXIT_HELP,
	BI_RETCODE_EXECUTE,
	BI_RETCODE_EXECUTE_BASH,
	BI_RETCODE_EXECUTE_SH,
	
	BI_RETCODE_RETURN,
	BI_RETCODE_HELP,
	BI_RETCODE_DISPLAY,
	
	BI_RETCODE_MAIN,
	BI_RETCODE_DEV,
	BI_RETCODE_ECHO,
	BI_RETCODE_REC,
	
	BI_RETCODE_COM,
	BI_RETCODE_COM_BR,
	BI_RETCODE_COM_MS,
	BI_RETCODE_COM_DTR,
	BI_RETCODE_COM_RTS,
	BI_RETCODE_COM_LS,
	
	BI_RETCODE_TCP,
	BI_RETCODE_UDP,
	BI_RETCODE_TCPS,
	BI_RETCODE_UDPS,
	BI_RETCODE_PORT,
	
	BI_RETCODE_SI,
	BI_RETCODE_SS,
	BI_RETCODE_SSD,
	
	BI_RETCODE_CONNECT,
	BI_RETCODE_DISCONNECT,
	BI_RETCODE_CR,
	
	BI_RETCODE_AUX,
	BI_RETCODE_AUX_COM,
	BI_RETCODE_AUX_BR,
	BI_RETCODE_AUX_MS,
	BI_RETCODE_AUX_OPEN,
	BI_RETCODE_AUX_CLOSE,
	BI_RETCODE_AUX_REC,
	BI_RETCODE_AUX_DTR,
	BI_RETCODE_AUX_RTS,
	
	BI_RETCODE_SC,
	BI_RETCODE_SC_EG,
	BI_RETCODE_SC_LOAD,
	BI_RETCODE_SC_SAVE,
	BI_RETCODE_SC_LS,
	BI_RETCODE_SC_SET,
	BI_RETCODE_SC_CLONE,
	BI_RETCODE_SC_DEL,
	BI_RETCODE_SC_MVUP,
	BI_RETCODE_SC_MVDN,
	BI_RETCODE_SC_COMMAND,
	BI_RETCODE_SC_SEND,
	
	BI_RETCODE_GC,
	BI_RETCODE_GC_EG,
	BI_RETCODE_GC_LOAD,
	BI_RETCODE_GC_SAVE,
	BI_RETCODE_GC_LS,
	BI_RETCODE_GC_SET,
	BI_RETCODE_GC_CLONE,
	BI_RETCODE_GC_DEL,
	BI_RETCODE_GC_MVUP,
	BI_RETCODE_GC_MVDN,
	BI_RETCODE_GC_GN,
	BI_RETCODE_GC_GROUP,
	BI_RETCODE_GC_GROUP_LS,
	BI_RETCODE_GC_GROUP_SET,
	BI_RETCODE_GC_GROUP_CLONE,
	BI_RETCODE_GC_GROUP_DEL,
	BI_RETCODE_GC_GROUP_MVUP,
	BI_RETCODE_GC_GROUP_MVDN,
	BI_RETCODE_GC_GROUP_COMMAND,
	BI_RETCODE_GC_GROUP_CONTINUE,
	BI_RETCODE_GC_GROUP_RESEND,
	BI_RETCODE_GC_GROUP_CSTOP,
	BI_RETCODE_GC_GROUP_CATCH,
	BI_RETCODE_GC_GROUP_SEND,
	
	BI_RETCODE_SCRIPT,
	BI_RETCODE_STOP,
	BI_RETCODE_RUN,
	BI_RETCODE_SEND,
	BI_RETCODE_SENDA,
	
	BI_RETCODE_CMUX,
	BI_RETCODE_CMUX_ON,
	BI_RETCODE_CMUX_OFF,
	BI_RETCODE_CMUX_CLD,
	BI_RETCODE_CMUX_PSC,
	BI_RETCODE_CMUX_FC,
	BI_RETCODE_CMUX_DTR,
	BI_RETCODE_CMUX_RTS,
	BI_RETCODE_DLCI,
	
	BI_RETCODE_NEWRECORD,
	BI_RETCODE_PATCHSDT,
	BI_RETCODE_APPROVE,
	BI_RETCODE_LSERVER,
	BI_RETCODE_CREGREPORT,
	BI_RETCODE_NTP,
	BI_RETCODE_NTPSERVER,
	BI_RETCODE_TERMINALSERVER,
		
	BI_RETCODE_RST,
	BI_RETCODE_RST_ON,
	BI_RETCODE_RST_OFF,
	BI_RETCODE_RST_LINK,
	BI_RETCODE_RST_MAPPING,
	BI_RETCODE_RST_LOGIN,
	
	BI_RETCODE_CALC,
	BI_RETCODE_CALC_HEX2DEC,
	BI_RETCODE_CALC_DEC2HEX,
	BI_RETCODE_CALC_MD5,
	BI_RETCODE_CALC_SHA1,
	BI_RETCODE_CALC_SHA224,
	BI_RETCODE_CALC_SHA256,
	BI_RETCODE_CALC_SHA384,
	BI_RETCODE_CALC_SHA512,
	BI_RETCODE_CALC_BAS64,
};
//------------------------------------------------------------------------------------------//
class SDTAPP;
class PUB_SBUF;
class IPCOMLIST;
struct BICPAR{
    SDTAPP		*sdtApp;
	ODEV_NODE	*oDevNode;
	PUB_SBUF	*charSBUF;
	IPCOMLIST	*validComList;
	std::string	retCommand;
	int32		blExit;
	int32		blInOnlineMode;
	int32		blInPressKeyMode;
	int32		blUseSecondLH;
	int32		blDisplayAuto;
    uint32		gDID;
};
//------------------------------------------------------------------------------------------//
class BIC_Node : public RTREE_NODE{
	public:
		enum{RFLAG_C = 0, RFLAG_S = RTREE_NODE::RFLAG_S + RTREE_NODE::RFLAG_C};
	public:
				 BIC_Node(void) : RTREE_NODE()  {cgTitle = "";cgPrintTitle = "";cgCommand = "";cgReturnCode = BI_RETCODE_NO;};
		virtual ~BIC_Node(void){;};
	public:
		static	void	SetblExit			(BICPAR *tBICPAR){if (tBICPAR->blExit != -1) tBICPAR->blExit = 1;};
		static	void	ClrblExit			(BICPAR *tBICPAR){if (tBICPAR->blExit != -1) tBICPAR->blExit = 0;};
		static	uint8	BI_ReadChar			(BICPAR *tBICPAR,int32 blWait = 1);
		static	int32	BI_ReadCommand		(BICPAR *tBICPAR);
	public:
		static	void	PrintTitle			(BICPAR *tBICPAR,DEVICE::DEVID_TYPE tDevType,int32 blPrintTail = 0);
		static	void	PrintDoRet			(BICPAR *tBICPAR,const std::string &strData)
												{tBICPAR->oDevNode->WriteToStrN(strData + "\n",RICH_LIN_clLightBlue,COLSTRING::COL_EP_YES);};
		static	void	PrintStr			(BICPAR *tBICPAR,const std::string &strData,const std::string &strColor)
												{tBICPAR->oDevNode->WriteToStr(strData,strColor,COLSTRING::COL_EP_YES);};
        static	void	PrintStrN			(BICPAR *tBICPAR,const std::string &strData,const std::string &strColor)
												{tBICPAR->oDevNode->WriteToStrN(strData,strColor,COLSTRING::COL_EP_YES);};
		static	void	PrintHelpItem		(BICPAR *tBICPAR,const std::string &command,const std::string &desp);
		static	uint8	PrintPressAnyKey	(BICPAR *tBICPAR);
		static	uint8	PressAnyKey			(BICPAR *tBICPAR);
	public:
		static	int32	BI_SET_ConnectPar	(BICPAR *tBICPAR,const std::string &par,DEVICE::DEVID_TYPE tDevType);
		static	int32	BI_SET_ConnectPar2	(BICPAR *tBICPAR,const std::string &par);
	public:
		std::string		cgPrintTitle;
		std::string		cgTitle;
		void 			SetSLTitle			(const std::string &fTitle);
		int32			UpdateTitle			(const std::string &fTitle);
	protected:
		std::string		cgCommand;
		int32			cgReturnCode;
	protected:
		virtual	int32	ExecLC		(BICPAR *tBICPAR,const std::string &fullCMD,std::string *ret)const;
		virtual	int32	ExecLC_T	(BICPAR *tBICPAR,const std::string &exTitle,std::string *ret)const;
	public:
		virtual	int32	ExecuteLC	(BICPAR *tBICPAR,const std::string &par,const std::string &exTitle,std::string *ret)const;
		virtual	int32	HelpLC		(BICPAR *tBICPAR,int32 blDetail = 1,int32 blPrintSubTitle = 1)const;

		virtual	int32	Check		(BICPAR *tBICPAR,const std::string &fullCMD,std::string *retPar)const;
		virtual	int32	Execute		(BICPAR *tBICPAR,const std::string &fullCMD,std::string *ret)const;
		virtual	int32	Help		(BICPAR *tBICPAR,int32 blDetail = 1)const;
		virtual	int32	Command		(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	AddNode		(RTREE_NODE *tTreeNode){RTREE_NODE::AddNode(tTreeNode);((BIC_Node*)tTreeNode)->SetSLTitle(cgTitle);return 1;};
};
//------------------------------------------------------------------------------------------//
class BIC_HELP : public BIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
				 BIC_HELP(void) : BIC_Node() {cgCommand = "?/help";cgReturnCode = BI_RETCODE_HELP;};
		virtual ~BIC_HELP(void){;};
	public:
		virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
		virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
};
//------------------------------------------------------------------------------------------//
class BIC_EXIT : public BIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
				 BIC_EXIT(void) : BIC_Node() {cgCommand = "exit";cgReturnCode = BI_RETCODE_EXIT;};
		virtual ~BIC_EXIT(void){;};
	public:
		virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
		virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
};
//------------------------------------------------------------------------------------------//
class BIC_EXEC : public BIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
				 BIC_EXEC(void) : BIC_Node() {cgCommand = "ex/execute";cgReturnCode = BI_RETCODE_EXECUTE;};
		virtual ~BIC_EXEC(void){;};
	public:
		virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
		virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
};
//------------------------------------------------------------------------------------------//
class BIC_EXE_BASH : public BIC_Node{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
			     BIC_EXE_BASH(void) : BIC_Node() {cgCommand = "bash";cgReturnCode = BI_RETCODE_EXECUTE_BASH;};
	    virtual ~BIC_EXE_BASH(void){;};
	public:
	    virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
	    virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
};
//------------------------------------------------------------------------------------------//
class BIC_EXE_SH : public BIC_Node{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
			     BIC_EXE_SH(void) : BIC_Node() {cgCommand = "exsh";cgReturnCode = BI_RETCODE_EXECUTE_SH;};
	    virtual ~BIC_EXE_SH(void){;};
	public:
	    virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
	    virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
};
//------------------------------------------------------------------------------------------//
class BIC_RETURN : public BIC_Node{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
                 BIC_RETURN(void) : BIC_Node() {cgCommand = "r/return";cgReturnCode = BI_RETCODE_RETURN;};
	    virtual ~BIC_RETURN(void){;};
	public:
	    virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
	    virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
};
//------------------------------------------------------------------------------------------//
class BIC_Node_S : public BIC_Node{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
			     BIC_Node_S(void) : BIC_Node() {InitSubNode();};
	    virtual ~BIC_Node_S(void){;};
	protected:
		void	InitSubNode(void){
            AddNode(&cgSub_EXIT);
			AddNode(&cgSub_HELP);
            AddNode(&cgSub_RETURN);
        }
	    BIC_HELP	cgSub_HELP;
    	BIC_RETURN	cgSub_RETURN;
	    BIC_EXIT	cgSub_EXIT;
};
//------------------------------------------------------------------------------------------//
#endif