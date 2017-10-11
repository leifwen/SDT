/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: BIC_CMD.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.01.25
 */

//------------------------------------------------------------------------------------------//
#include "BIC.h"
//------------------------------------------------------------------------------------------//
#ifdef BIC_BH
#ifndef BIC_CMDH
#define BIC_CMDH
#ifdef BIC_CMDH
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
class BIC_MAIN : public BIC_Node_S{
	public:
				 BIC_MAIN(void) : BIC_Node_S() {cgblTrySubCMD = 1;cgCommand = "main";cgTitle = cgCommand;cgReturnCode = BI_RETCODE_MAIN;Init();};
		virtual ~BIC_MAIN(void){;};
	public:
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			if (blDetail == 1){
				HelpTraversalChild(env,0);
				return(cgReturnCode);
			}
			PrintHelpItem(env, cgCommand, "-> Main connection setting.");
			if (blDetail == 0)
				return(cgReturnCode);
			return(cgReturnCode);
		};
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const{return(BIC_Node_S::Command(env, par, env->eda));};
	private:
		void	Init(void){
			Add(cgC_CONNECT) < cgC_DISCONNECT < cgC_ONLINE < cgC_CR
			< cgC_DEV < cgC_ECHO < cgC_REC
		#ifdef SWVERSION_COM
			< cgC_ACOM
		#endif
		#if (defined SWVERSION_PORT) || (defined SWVERSION_Server)
			< cgC_ASOCKET
		#endif
			< cgC_SEND < cgC_SENDA < cgC_SENDFILE < cgC_STOP
		#ifdef SWVERSION_SCRIPT
			< cgC_RUN < cgC_SCRIPT
		#endif
			;
		}
		BIC_CONNECT		cgC_CONNECT;
		BIC_DISCONNECT	cgC_DISCONNECT;
		BIC_ONLINE		cgC_ONLINE;
		BIC_CR			cgC_CR;
		
		BIC_ECHO		cgC_ECHO;
		BIC_REC			cgC_REC;
		BIC_DEV			cgC_DEV;
		#ifdef SWVERSION_COM
		BIC_ACOM		cgC_ACOM;
		#endif
		#if (defined SWVERSION_PORT) || (defined SWVERSION_Server)
		BIC_ASOCKET		cgC_ASOCKET;
		#endif
		BIC_SEND		cgC_SEND;
		BIC_SENDA		cgC_SENDA;
		BIC_SENDFILE	cgC_SENDFILE;
		BIC_STOP		cgC_STOP;
		#ifdef SWVERSION_SCRIPT
		BIC_RUN			cgC_RUN;
		BIC_SCRIPT		cgC_SCRIPT;
		#endif
};
//------------------------------------------------------------------------------------------//
class BIC_AUX : public BIC_MAIN{
	public:
				 BIC_AUX(void) : BIC_MAIN() {cgblTrySubCMD = 0;cgCommand = "aux";cgTitle = cgCommand;cgReturnCode = BI_RETCODE_AUX;Init();SetSubNodeSelfName("Auxiliary");};
		virtual ~BIC_AUX(void){;};
	public:
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			if (blDetail == 1){
				HelpTraversalChild(env,0);
				return(cgReturnCode);
			}
			PrintHelpItem(env, cgCommand, "-> Auxiliary connection setting.");
			if (blDetail == 0)
				return(cgReturnCode);
			return(cgReturnCode);
		};
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const{return(BIC_Node_S::Command(env, par, env->eda2));};
	private:
		void	Init(void){
		#ifdef BIC_SCM_hpp
			Add(cgC_SCM);
		#endif
		#ifdef BIC_GCM_hpp
			Add(cgC_GCM)
		#endif
		;
	}
	private:
	#ifdef BIC_SCM_hpp
		BIC_SCM			cgC_SCM;
	#endif
	#ifdef BIC_GCM_hpp
		BIC_GCM			cgC_GCM;
	#endif
};
//------------------------------------------------------------------------------------------//
class BIC_CMD : public BIC_Node{
	public:
		virtual ~BIC_CMD(void){;};
		BIC_CMD(void) : BIC_Node(){
			cgTitle = "SDT";
			
			Add(cgC_EXIT) < cg_VERSION < cgC_HELP
		#ifdef BIC_EXE_hpp
			< cgC_EXEC < cgC_EXE_BASH < cgC_EXE_SH
		#endif
			< cgC_NEWRECORD
			< cgC_DISPLAY
		#ifdef BIC_MSDT_ENABLE
			< cgC_MSDT
		#endif
		#ifdef BIC_CALCH
			< cgC_Calc
		#endif
		#ifdef BIC_RSTH
			< cgC_RST
		#endif
			< cgC_MAIN
		#ifdef SWVERSION_AUXDEVICE
			< cgC_AUX
		#endif
		#ifdef BIC_SCM_hpp
			< cgC_SCM
		#endif
		#ifdef BIC_GCM_hpp
			< cgC_GCM
		#endif
		#ifdef BIC_CMUXH
			< cgC_CMUX
		#endif
		#ifdef BIC_NTPH
			< cgC_NTP < cgC_NTPS
		#endif
		#ifdef BIC_CREGH
			< cgC_CREGREPORT
		#endif
		#ifdef BIC_DICTH
			< cgC_DICT
		#endif
			;
	};
	public:
		BIC_VERSION		cg_VERSION;
		BIC_EXIT		cgC_EXIT;
		BIC_HELP		cgC_HELP;
	#ifdef BIC_EXE_hpp
		BIC_EXEC		cgC_EXEC;
		BIC_EXE_BASH	cgC_EXE_BASH;
		BIC_EXE_SH		cgC_EXE_SH;
	#endif
		BIC_NEWRECORD	cgC_NEWRECORD;
		BIC_DISPLAY		cgC_DISPLAY;
	#ifdef BIC_MSDT_ENABLE
		BIC_MSDT		cgC_MSDT;
	#endif
	#ifdef BIC_CALCH
		BIC_CALC		cgC_Calc;
	#endif
	#ifdef BIC_RSTH
		BIC_RST			cgC_RST;
	#endif
		BIC_MAIN		cgC_MAIN;
	#ifdef SWVERSION_AUXDEVICE
		BIC_AUX			cgC_AUX;
	#endif
	#ifdef BIC_SCM_hpp
		BIC_SCM			cgC_SCM;
	#endif
	#ifdef BIC_GCM_hpp
		BIC_GCM			cgC_GCM;
	#endif
	#ifdef BIC_CMUXH
		BIC_CMUX		cgC_CMUX;
	#endif
	#ifdef BIC_NTPH
		BIC_NTP			cgC_NTP;
		BIC_NTPSERVER	cgC_NTPS;
	#endif
	#ifdef BIC_CREGH
		BIC_CREGREPORT	cgC_CREGREPORT;
	#endif
	#ifdef BIC_DICTH
		BIC_DICT		cgC_DICT;
	#endif
};
//------------------------------------------------------------------------------------------//
#endif
#endif
#endif
