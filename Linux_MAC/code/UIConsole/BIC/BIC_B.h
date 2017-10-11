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

//------------------------------------------------------------------------------------------//
#include "BIC_Define.h"
#include "Comm_Tree.h"
#include "Device.h"
//------------------------------------------------------------------------------------------//
#ifdef DeviceH
#ifndef BIC_BH
#define BIC_BH
#ifdef BIC_BH
//------------------------------------------------------------------------------------------//
enum{
	COL_Result	= COL_clYellow,
	COL_Sucess	= COL_NormalMessage,
	COL_Fail	= COL_ImportantMessage,
};
//------------------------------------------------------------------------------------------//
class BIC_Tools : public TREE_NODE{
	public:
				 BIC_Tools(void) : TREE_NODE()  {cgTitle = "";cgPrintTitle = "";};
		virtual ~BIC_Tools(void){;};
	public:
		inline	static	void	SetblExit			(BIC_ENV *env){if (env->blExit != -1) env->blExit = 1;};
		inline	static	void	ClrblExit			(BIC_ENV *env){if (env->blExit != -1) env->blExit = 0;};
	public:
				static	uint8	BI_ReadChar			(BIC_ENV *env,int32 blWait = 1);
				static	int32	BI_ReadCommand		(BIC_ENV *env,STDSTR *retCMD);
				static	uint8	PrintPressAnyKey	(BIC_ENV *env);
				static	uint8	PressAnyKey			(BIC_ENV *env,ExpandDeviceAttr *eda,SYS_TIME_S *dly = nullptr);
	public:
		inline	static	void	SplitPar1			(STDSTR &strPar1,STDSTR &strPar2,const STDSTR &par,const STDSTR &split = " "){
			strPar2 = Str_LTrim(par);
			strPar1 = Str_ReadSubItem(&strPar2,split);
			Str_LTrimSelf(strPar2);
		};
		inline	static	void	SplitPar1			(STDSTR *strPar1,STDSTR *strPar2,const STDSTR &split = " "){
			*strPar1 = Str_ReadSubItem(strPar2,split);
			Str_LTrimSelf(*strPar2);
		};
	public:
				STDSTR		cgTitle;
				STDSTR		cgPrintTitle;
	public:
				void		SetSLTitle	(const STDSTR &fTitle){cgPrintTitle = fTitle + "/" + cgTitle;UpdateTitle(cgPrintTitle);};
				void		UpdateTitle	(const STDSTR &fTitle){TREE_LChildRChain_Traversal_LINE_nolock(BIC_Tools,this,operateNode_t->SetSLTitle(fTitle););};
		virtual	TREE_NODE*	AddNode		(TNF *tTreeNode){((BIC_Tools*)tTreeNode)->SetSLTitle(cgTitle);return(TREE_NODE::AddNode(tTreeNode));};
	public:
		inline	static 	OUTPUT_NODE	&STDout	(BIC_ENV *env){return(*env->cstdout);};
	
		inline	static	OUTPUT_NODE	&Print	(BIC_ENV *env){return(OUTPUT_NODE::Print(env->cstdout));};
		inline	static	OUTPUT_NODE	&PrintNL(BIC_ENV *env){return(OUTPUT_NODE::PrintNL(env->cstdout));};
		static	void	PrintStr			(BIC_ENV *env,const COLSTR &colStr1,const COLSTR &colStr2 = "",const COLSTR &colStr3 = "",const COLSTR &colStr4 = ""
											 ,const COLSTR &colStr5 = "",const COLSTR &colStr6 = "",const COLSTR &colStr7 = "",const COLSTR &colStr8 = "");
		static	void	PrintStrNL			(BIC_ENV *env,const COLSTR &colStr1,const COLSTR &colStr2 = "",const COLSTR &colStr3 = "",const COLSTR &colStr4 = ""
											 ,const COLSTR &colStr5 = "",const COLSTR &colStr6 = "",const COLSTR &colStr7 = "",const COLSTR &colStr8 = "");
		static	void	PrintWithTime		(BIC_ENV *env,const COLSTR &colStr1,const COLSTR &colStr2 = "",const COLSTR &colStr3 = "",const COLSTR &colStr4 = ""
											 ,const COLSTR &colStr5 = "",const COLSTR &colStr6 = "",const COLSTR &colStr7 = "",const COLSTR &colStr8 = "");
		static	void	PrintWithDividingLine(BIC_ENV *env,const COLSTR &colStr1,const COLSTR &colStr2 = "",const COLSTR &colStr3 = "",const COLSTR &colStr4 = ""
											 ,const COLSTR &colStr5 = "",const COLSTR &colStr6 = "",const COLSTR &colStr7 = "",const COLSTR &colStr8 = "");
		static	void	PrintMessage		(BIC_ENV *env,const COLSTR &colStr1,const COLSTR &colStr2 = "",const COLSTR &colStr3 = "",const COLSTR &colStr4 = ""
											 ,const COLSTR &colStr5 = "",const COLSTR &colStr6 = "",const COLSTR &colStr7 = "",const COLSTR &colStr8 = "");
		static	void	PrintNormalMessage	(BIC_ENV *env,const STDSTR &strData1,const STDSTR &strData2 = "",const STDSTR &strData3 = "",const STDSTR &strData4 = ""
											 ,const STDSTR &strData5 = "",const STDSTR &strData6 = "",const STDSTR &strData7 = "",const STDSTR &strData8 = "");
		static	void	PrintWarningMessage	(BIC_ENV *env,const STDSTR &strData1C1,const STDSTR &strData2C2 = "",const STDSTR &strData3C2 = "",const STDSTR &strData4C2 = ""
											 ,const STDSTR &strData5C2 = "",const STDSTR &strData6C2 = "",const STDSTR &strData7C2 = "",const STDSTR &strData8C2 = "");
	public:
		static	void	PrintALine			(BIC_ENV *env,const COLSTR &colStr1,const COLSTR &colStr2 = "",const COLSTR &colStr3 = "",const COLSTR &colStr4 = ""
											 ,const COLSTR &colStr5 = "",const COLSTR &colStr6 = "",const COLSTR &colStr7 = "",const COLSTR &colStr8 = "");
	public:
		static	void	PrintTitle			(BIC_ENV *env,CSType tDevType,int32 blPrintTail = 0);
		static	void	PrintHelpItem		(BIC_ENV *env,const STDSTR &command,const STDSTR &desp1,const STDSTR &desp2 = "",const STDSTR &desp3 = "",const STDSTR &desp4 = "");
		static	void	PrintHelpSubItem	(BIC_ENV *env,const STDSTR &command,const STDSTR &desp1,const STDSTR &desp2 = "",const STDSTR &desp3 = "",const STDSTR &desp4 = "");
	public:
		static	void	PrintFail			(BIC_ENV *env,const COLSTR &colStr1 = "",const COLSTR &colStr2 = "",const COLSTR &colStr3 = "",const COLSTR &colStr4 = "");
		static	void	PrintSuccess		(BIC_ENV *env,const COLSTR &colStr1 = "",const COLSTR &colStr2 = "",const COLSTR &colStr3 = "",const COLSTR &colStr4 = "");
		static	void	PrintResult			(BIC_ENV *env,const COLSTR &colStr1 = "",const COLSTR &colStr2 = "",const COLSTR &colStr3 = "",const COLSTR &colStr4 = ""
											 ,const COLSTR &colStr5 = "",const COLSTR &colStr6 = "",const COLSTR &colStr7 = "",const COLSTR &colStr8 = "");
	public:
		static	int32	BI_SetConnectPar	(ExpandDeviceAttr *eda,const STDSTR &par,CSType tDevType);
		static	int32	BI_SetConnectPar2	(ExpandDeviceAttr *eda,const STDSTR &par);
	public:
				int32	OnlineMode			(BIC_ENV *env,COMMU_DBUF_FRAME *cmmu,CMD_TAIL tail)const;
		virtual	int32	OnlineModeExit		(BIC_ENV *env)const{return 0;};
				int32	InPressKeyMode		(BIC_ENV *env)const;
		virtual	int32	InPressKeyModeExit	(BIC_ENV *env)const{return 0;};
};
//------------------------------------------------------------------------------------------//
class BIC_Node : public BIC_Tools{
	public:
				 BIC_Node(void) : BIC_Tools()  {selfName = "";cgCommand = "";cgReturnCode = BI_RETCODE_NO;cgblTrySubCMD = 0;};
		virtual ~BIC_Node(void){;};
	protected:
		STDSTR		cgCommand;
		int32		cgReturnCode;
		int32		cgblTrySubCMD;
	public:
		inline int32 ExecuteTraversalChild(BIC_ENV *env,const STDSTR &cmd,void *p)const{
			int32	retCode;
			retCode = BI_RETCODE_NO;
			TREE_LChildRChain_Traversal_LINE_nolock(BIC_Node,this,
				retCode = operateNode_t->Execute(env,cmd,p);
				if (retCode != BI_RETCODE_NO)
					break;
			);
			return(retCode);
		}
		int32 HelpTraversalChild(BIC_ENV *env,int32 blDetail = 1,int32 blPrintSubTitle = 1)const{
			if ((GetcgDown(this) != nullptr) && (blPrintSubTitle != 0))
				PrintALine(env,"Subcommand explain:");
			TREE_LChildRChain_Traversal_LINE_nolock(BIC_Node,this,operateNode_t->Help(env,blDetail););
			return(cgReturnCode);
		}
		inline	virtual void	SetSubNodeSelfName(const STDSTR &strName){
			TREE_LChildRChain_Traversal_LINE_nolock(BIC_Node,this,operateNode_t->SetSelfName(strName);operateNode_t->SetSubNodeSelfName(strName););
		}
	public:
		virtual	int32	Check		(BIC_ENV *env,const STDSTR &rawIn,STDSTR *retPar)const;
		virtual	int32	Execute		(BIC_ENV *env,const STDSTR &rawIn,void *p)const;
	public:
		virtual	int32	Help		(BIC_ENV *env,int32 blDetail = 1)const{return(cgReturnCode);};
		virtual	int32	Command		(BIC_ENV *env,const STDSTR &par,void *p)const;
	
		int32	ExecuteAsConsole	(BIC_ENV *env,const STDSTR &cmd,const STDSTR &exTitle,void *p)const;
};
//------------------------------------------------------------------------------------------//
class BIC_VERSION : public BIC_Node{
	public:
				 BIC_VERSION(void) : BIC_Node() {cgCommand = "ver";cgReturnCode = BI_RETCODE_VERSION;};
		virtual ~BIC_VERSION(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const{PrintResult(env, SWVERSION_SHORTNAME, SWVERSION_VER);return(cgReturnCode);};
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"SDT Version.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_HELP : public BIC_Node{
	public:
				 BIC_HELP(void) : BIC_Node() {cgCommand = "?/help";cgReturnCode = BI_RETCODE_HELP;};
		virtual ~BIC_HELP(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"Help information.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"[-d]","List command detail.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_EXIT : public BIC_Node{
	public:
				 BIC_EXIT(void) : BIC_Node() {cgCommand = "exit";cgReturnCode = BI_RETCODE_EXIT;};
		virtual ~BIC_EXIT(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const{
			#ifdef CommonDefH_VC
				return(BI_RETCODE_NO);
			#endif
			#ifdef CommonDefH_Unix
				env->blExit = cgReturnCode;
				PrintStr(env,"\n");
				return(cgReturnCode);
			#endif
		};
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"Exit SDT.");
			return(BI_RETCODE_EXIT_HELP);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_RETURN : public BIC_Node{
	public:
				 BIC_RETURN(void) : BIC_Node() {cgCommand = "r/return";cgReturnCode = BI_RETCODE_RETURN;};
		virtual ~BIC_RETURN(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const{PrintStr(env,"\n");return(cgReturnCode);};
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"Return to the upper menu.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_Node_S : public BIC_Node{
	public:
				 BIC_Node_S(void) : BIC_Node() {InitSubNode();};
		virtual ~BIC_Node_S(void){;};
	protected:
		void	InitSubNode(void){Add(cgSub_EXIT) < cgSub_VERSION < cgSub_HELP < cgSub_RETURN;};
	protected:
		BIC_EXIT	cgSub_EXIT;
		BIC_VERSION cgSub_VERSION;
		BIC_HELP	cgSub_HELP;
		BIC_RETURN	cgSub_RETURN;
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif
#endif
#endif
