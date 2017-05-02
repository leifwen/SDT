//
//  BIC_SCM.h
//  SDT
//
//  Created by Leif Wen on 05/04/2017.
//  Copyright © 2017 leif wen. All rights reserved.
//
//------------------------------------------------------------------------------------------//
#include "BIC_B.h"
//------------------------------------------------------------------------------------------//
#if defined BIC_BH && defined SWVERSION_SCRIPT
#ifndef BIC_SCM_hpp
#define BIC_SCM_hpp
#ifdef BIC_SCM_hpp
//------------------------------------------------------------------------------------------//
class BIC_SCM_EG : public BIC_Node{
	public:
				 BIC_SCM_EG(void) : BIC_Node() {cgCommand = "eg";cgReturnCode = BI_RETCODE_SC_EG;};
		virtual ~BIC_SCM_EG(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env, cgCommand, "Single command record example.");
			if (blDetail == 0)
				return(cgReturnCode);
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_SCM_LOAD : public BIC_Node{
	public:
				 BIC_SCM_LOAD(void) : BIC_Node() {cgCommand = "load";cgReturnCode = BI_RETCODE_SC_LOAD;};
		virtual ~BIC_SCM_LOAD(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env, cgCommand, "Load single command from default <default.ini> file.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"[filename]","Filename.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_SCM_SAVE : public BIC_Node{
	public:
				 BIC_SCM_SAVE(void) : BIC_Node() {cgCommand = "save";cgReturnCode = BI_RETCODE_SC_SAVE;};
		virtual ~BIC_SCM_SAVE(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env, cgCommand, "Save to file.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"<filename>","Filename.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_SCM_LS : public BIC_Node{
	public:
				 BIC_SCM_LS(void) : BIC_Node() {cgCommand = "ls/list";cgReturnCode = BI_RETCODE_SC_LS;};
		virtual ~BIC_SCM_LS(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env, cgCommand, "List single command.");
			if (blDetail == 0)
				return(cgReturnCode);
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_SCM_SET : public BIC_Node{
	public:
				 BIC_SCM_SET(void) : BIC_Node() {cgCommand = "set";cgReturnCode = BI_RETCODE_SC_SET;};
		virtual ~BIC_SCM_SET(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env, cgCommand, "Set command.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"[+cr]"	,"Enable send \"\\r\" in the end.");
			PrintHelpSubItem(env,"[-cr]"	,"Disable send \"\\r\" in the end.");
			PrintHelpSubItem(env,"<sID>"	,"Command sID, all.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_SCM_CLONE : public BIC_Node{
	public:
				 BIC_SCM_CLONE(void) : BIC_Node() {cgCommand = "clone";cgReturnCode = BI_RETCODE_SC_CLONE;};
		virtual ~BIC_SCM_CLONE(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env, cgCommand, "Clone command by sID.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"[sID]","Command sID.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_SCM_DEL : public BIC_Node{
	public:
				 BIC_SCM_DEL(void) : BIC_Node() {cgCommand = "del";cgReturnCode = BI_RETCODE_SC_DEL;};
		virtual ~BIC_SCM_DEL(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env, cgCommand, "Delete command by sID.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"<sID>","Command sID.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_SCM_MVUP : public BIC_Node{
	public:
				 BIC_SCM_MVUP(void) : BIC_Node() {cgCommand = "mvup";cgReturnCode = BI_RETCODE_SC_MVUP;};
		virtual ~BIC_SCM_MVUP(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env, cgCommand, "Move command up.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"<sID>"	,"Command sID.");
			PrintHelpSubItem(env,"[step]"	,"Default value is 1.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_SCM_MVDN : public BIC_Node{
	public:
				 BIC_SCM_MVDN(void) : BIC_Node() {cgCommand = "mvdn";cgReturnCode = BI_RETCODE_SC_MVDN;};
		virtual ~BIC_SCM_MVDN(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env, cgCommand, "Move command down.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"<sID>"	,"Command sID.");
			PrintHelpSubItem(env,"[step]"	,"Default value is 1.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_SCM_COMMAND : public BIC_Node{
	public:
				 BIC_SCM_COMMAND(void) : BIC_Node() {cgCommand = "cmd/command";cgReturnCode = BI_RETCODE_SC_COMMAND;};
		virtual ~BIC_SCM_COMMAND(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env, cgCommand, "Set command content by sID.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"<sID>"		,"Command sID.");
			PrintHelpSubItem(env,"<content>"	,"Content.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_SCM_SEND : public BIC_Node{
	public:
				 BIC_SCM_SEND(void) : BIC_Node() {cgCommand = "send";cgReturnCode = BI_RETCODE_SC_SEND;};
		virtual ~BIC_SCM_SEND(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *eda)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env, cgCommand, "Send command by sID if connected.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"<sID>","Command sID.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_SCM : public BIC_Node_S{
	public:
				 BIC_SCM(void) : BIC_Node_S() {cgCommand = "scm";cgTitle = cgCommand;cgReturnCode = BI_RETCODE_SC;Init();};
		virtual ~BIC_SCM(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *eda)const{
			int32	retCode;
			retCode = BIC_Node_S::Command(env, par, eda);
			if (retCode == BI_RETCODE_NO)
				return(cgSub_ls.Command(env, par, eda));
			return(retCode);
		};
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			if (blDetail == 1){
				HelpTraversalChild(env,0);
				return(cgReturnCode);
			}
			PrintHelpItem(env, cgCommand, "-> SCM.");
			if (blDetail == 0)
				return(cgReturnCode);
			return(cgReturnCode);
		};
	private:
		void	Init(void){Add(cgSub_eg) < cgSub_load < cgSub_save < cgSub_send < cgSub_ls < cgSub_set < cgSub_clone < cgSub_del < cgSub_mvup < cgSub_mvdn < cgSub_command;};
		BIC_SCM_EG		cgSub_eg;
		BIC_SCM_LOAD	cgSub_load;
		BIC_SCM_SAVE	cgSub_save;
		BIC_SCM_LS		cgSub_ls;
		BIC_SCM_SET		cgSub_set;
		BIC_SCM_CLONE	cgSub_clone;
		BIC_SCM_DEL		cgSub_del;
		BIC_SCM_MVUP	cgSub_mvup;
		BIC_SCM_MVDN	cgSub_mvdn;
		BIC_SCM_COMMAND	cgSub_command;
		BIC_SCM_SEND	cgSub_send;
	};
//------------------------------------------------------------------------------------------//
#endif
#endif
#endif /* BIC_SCM_hpp */
