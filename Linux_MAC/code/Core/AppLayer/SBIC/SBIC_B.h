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

//------------------------------------------------------------------------------------------//
#include "SBIC_Define.h"
#include "Comm_Convert.h"
#include "Comm_Tree.h"
#include "OUTPUT.h"
//------------------------------------------------------------------------------------------//
#if defined Comm_TreeH && defined OUTPUTH 
#ifndef SBIC_BH
#define SBIC_BH
#ifdef SBIC_BH
//------------------------------------------------------------------------------------------//
class SBIC_TOOLS : public TREE_NODE{
	public:
		enum{
			st_blCommandExplain	= BD_FLAG32(0),
			st_blATResponse		= BD_FLAG32(1),
			st_blPrintSBICinfo	= BD_FLAG32(2),
		};
	public:
				 SBIC_TOOLS(void) : TREE_NODE() {;};
		virtual ~SBIC_TOOLS(void){;};
	public:
		static	void		SetblExit	(SBIC_ENV *env){if ((env != nullptr) && (env->blExit != -1)) env->blExit = 1;};
		static	void		ClrblExit	(SBIC_ENV *env){if ((env != nullptr) && (env->blExit != -1)) env->blExit = 0;};
		static	int32		ClkblExit	(SBIC_ENV *env){return((env != nullptr) && (env->blExit != 0));};
	public:
				static	STDSTR	DelComment	(const STDSTR &strIn);
				static	int32	GetMS		(const STDSTR &par);
				static	STDSTR	GetMSSTR	(int32 ms);
		inline	static	void	SplitPar1	(STDSTR &strPar1,STDSTR &strPar2,const STDSTR &par,const STDSTR &split){
			strPar2 = Str_LTrim(par);
			strPar1 = Str_ReadSubItem(&strPar2,split);
			Str_LTrimSelf(strPar2);
		};
	public:
		inline	static	OUTPUT_NODE	&Print	(OUTPUT_NODE *node){return(OUTPUT_NODE::Print(node));};
		inline	static	OUTPUT_NODE	&PrintNL(OUTPUT_NODE *node){return(OUTPUT_NODE::PrintNL(node));};
		static	void PrintStr				(OUTPUT_NODE *node,const COLSTR &colStr1,const COLSTR &colStr2 = "",const COLSTR &colStr3 = "",const COLSTR &colStr4 = ""
											,const COLSTR &colStr5 = "",const COLSTR &colStr6 = "",const COLSTR &colStr7 = "",const COLSTR &colStr8 = "");
		static	void PrintStrNL				(OUTPUT_NODE *node,const COLSTR &colStr1,const COLSTR &colStr2 = "",const COLSTR &colStr3 = "",const COLSTR &colStr4 = ""
											,const COLSTR &colStr5 = "",const COLSTR &colStr6 = "",const COLSTR &colStr7 = "",const COLSTR &colStr8 = "");
		static	void PrintWithTime			(OUTPUT_NODE *node,const COLSTR &colStr1,const COLSTR &colStr2 = "",const COLSTR &colStr3 = "",const COLSTR &colStr4 = ""
											,const COLSTR &colStr5 = "",const COLSTR &colStr6 = "",const COLSTR &colStr7 = "",const COLSTR &colStr8 = "");
		static	void PrintWithDividingLine	(OUTPUT_NODE *node,const COLSTR &colStr1,const COLSTR &colStr2 = "",const COLSTR &colStr3 = "",const COLSTR &colStr4 = ""
											,const COLSTR &colStr5 = "",const COLSTR &colStr6 = "",const COLSTR &colStr7 = "",const COLSTR &colStr8 = "");
		static	void PrintMessage			(OUTPUT_NODE *node,const COLSTR &colStr1,const COLSTR &colStr2 = "",const COLSTR &colStr3 = "",const COLSTR &colStr4 = ""
											,const COLSTR &colStr5 = "",const COLSTR &colStr6 = "",const COLSTR &colStr7 = "",const COLSTR &colStr8 = "");
		static	void PrintNormalMessage		(OUTPUT_NODE *node,const STDSTR &strData1,const STDSTR &strData2 = "",const STDSTR &strData3 = "",const STDSTR &strData4 = ""
											,const STDSTR &strData5 = "",const STDSTR &strData6 = "",const STDSTR &strData7 = "",const STDSTR &strData8 = "");
		static	void PrintWarningMessage	(OUTPUT_NODE *node,const STDSTR &strData1C1,const STDSTR &strData2C2 = "",const STDSTR &strData3C2 = "",const STDSTR &strData4C2 = ""
											 ,const STDSTR &strData5C2 = "",const STDSTR &strData6C2 = "",const STDSTR &strData7C2 = "",const STDSTR &strData8C2 = "");
	public:
		static 	OUTPUT_NODE	*GetDevOut		(SBIC_ENV *env);
		static	void		DeviceLock		(SBIC_ENV *env);
		static	void		DeviceUnlock	(SBIC_ENV *env);
		static	void		PrintExecute	(SBIC_ENV *env,const COLSTR &colStr2 = "",const COLSTR &colStr3 = "",const COLSTR &colStr4 = ""
											,const COLSTR &colStr5 = "",const COLSTR &colStr6 = "",const COLSTR &colStr7 = "",const COLSTR &colStr8 = "");
	public:
		inline	static	void	PrintB		(SBIC_ENV *env,const STDSTR &str){PrintStrNL(env->cstdout,Data(COL_clBlue,&str), "\n");};
		inline	static	void	PrintP		(SBIC_ENV *env,const STDSTR &str){PrintStrNL(env->cstdout,Data(COL_clPurple,&str),"\n");};
		inline	static	void	PrintM		(SBIC_ENV *env,const STDSTR &strM,const STDSTR &strB = ""){PrintStrNL(env->cstdout, Data(COL_clMaroon,&strM),Data(COL_clBlue,&strB),"\n");};
	public:
		inline	static	void	SetblCommandExplain	(SBIC_ENV *env){B_SetFLAG32(env->cfg,st_blCommandExplain);};
		inline	static	void	ClrblCommandExplain	(SBIC_ENV *env){B_ClrFLAG32(env->cfg,st_blCommandExplain);};
		inline	static	int32	CheckCommandExplain	(SBIC_ENV *env){return(B_ChkFLAG32(env->cfg,st_blCommandExplain));};
	
		inline	static	void	SetblATResponse		(SBIC_ENV *env){B_SetFLAG32(env->cfg,st_blATResponse);};
		inline	static	void	ClrblATResponse		(SBIC_ENV *env){B_ClrFLAG32(env->cfg,st_blATResponse);};
		inline	static	int32	CheckATResponse		(SBIC_ENV *env){return(B_ChkFLAG32(env->cfg,st_blATResponse));};
	
		inline	static	void	SetblPrintSBICinfo	(SBIC_ENV *env){B_SetFLAG32(env->cfg,st_blPrintSBICinfo);};
		inline	static	void	ClrblPrintSBICinfo	(SBIC_ENV *env){B_ClrFLAG32(env->cfg,st_blPrintSBICinfo);};
		inline	static	int32	CheckPrintSBICinfo	(SBIC_ENV *env){return(B_ChkFLAG32(env->cfg,st_blPrintSBICinfo));};
};
//------------------------------------------------------------------------------------------//
class SBIC_Node : public SBIC_TOOLS{
	public:
				 SBIC_Node(void) : SBIC_TOOLS() {cgblTrySubCMD = 0;cgCommand = "";cgReturnCode = SBI_RETCODE_NO;};
		virtual ~SBIC_Node(void){;};
	protected:
		STDSTR		cgCommand;
		int32		cgReturnCode;
		int32		cgblTrySubCMD;
	public:
		int32	ExecuteTraversalChild		(SBIC_ENV *env,const STDSTR &cmd,void *p)const{
			int32	retCode;
			retCode = SBI_RETCODE_NO;
			TREE_LChildRChain_Traversal_LINE_nolock(SBIC_Node,this,
				retCode = operateNode_t->Execute(env,cmd,p);
				if (retCode != SBI_RETCODE_NO)
					break;
			);
			return(retCode);
		};
		int32	HelpTraversalChild			(SBIC_ENV *env,int32 blDetail = 1)const{
			TREE_LChildRChain_Traversal_LINE_nolock(SBIC_Node,this,operateNode_t->Help(env,blDetail));
			return(cgReturnCode);
		};
	public:
		virtual	int32	Check	(SBIC_ENV *env,const STDSTR &rawIn,STDSTR *retPar)const;
		virtual	int32	Execute	(SBIC_ENV *env,const STDSTR &rawIn,void *p)const;
	public:
		virtual	int32	Help	(SBIC_ENV *env,int32 blDetail = 1)const			{return(cgReturnCode);};
		virtual	int32	Command	(SBIC_ENV *env,const STDSTR &par,void *p)const	{return(ExecuteTraversalChild(env, par, p));};
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif
#endif
#endif

