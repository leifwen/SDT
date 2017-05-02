//
//  BIC_GCM.h
//  SDT
//
//  Created by Leif Wen on 05/04/2017.
//  Copyright © 2017 leif wen. All rights reserved.
//
//------------------------------------------------------------------------------------------//
#include "BIC_B.h"
#include "BIC_Device.h"
//------------------------------------------------------------------------------------------//
#if defined BIC_BH && defined SWVERSION_SCRIPT
#ifndef BIC_GCM_hpp
#define BIC_GCM_hpp
#ifdef BIC_GCM_hpp
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
class BIC_GCM_LOAD : public BIC_Node{
	public:
				 BIC_GCM_LOAD(void) : BIC_Node() {cgCommand = "load";cgReturnCode = BI_RETCODE_GC_LOAD;};
		virtual ~BIC_GCM_LOAD(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env, cgCommand, "Load group from default <default.ini> file.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"[filename]","Filename.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_GCM_SAVE : public BIC_Node{
	public:
				 BIC_GCM_SAVE(void) : BIC_Node() {cgCommand = "save";cgReturnCode = BI_RETCODE_GC_SAVE;};
		virtual ~BIC_GCM_SAVE(void){;};
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
class GC_LIST;
class COMMAND_GROUP;
class COMMAND_NODE;
//------------------------------------------------------------------------------------------//
class BIC_GCM_LS : public BIC_Node{
	public:
				 BIC_GCM_LS(void) : BIC_Node() {cgCommand = "ls/list";cgReturnCode = BI_RETCODE_GC_LS;};
		virtual ~BIC_GCM_LS(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env, cgCommand, "List group.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"[-s]"		,"Show selected item.");
			PrintHelpSubItem(env,"[-c]"		,"Show Cycle column.");
			PrintHelpSubItem(env,"[-t]"		,"Show Timeout/interval column.");
			PrintHelpSubItem(env,"[-d]"		,"Show condition.");
			PrintHelpSubItem(env,"[-a]"		,"Show all.");
			PrintHelpSubItem(env,"[gID]"	,"Group gID, s/uns.");
			return(cgReturnCode);
		};
		
		static	int32	BIC_GCM_LS_PrintGroupList	(BIC_ENV *env,GC_LIST *tGroupList,uint32 flag);
		static	int32	BIC_GCM_LS_PringGroup	(BIC_ENV *env,COMMAND_GROUP *tGroup,uint32 flag);
		static	int32	BIC_GCM_LS_PringCommand		(BIC_ENV *env,COMMAND_NODE *tCommand,uint32 flag);
};
//------------------------------------------------------------------------------------------//
class BIC_GCM_SET : public BIC_Node{
	public:
				 BIC_GCM_SET(void) : BIC_Node() {cgCommand = "set";cgReturnCode = BI_RETCODE_GC_SET;};
		virtual ~BIC_GCM_SET(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env, cgCommand, "Set group.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"[+]"		,"Set group selected.");
			PrintHelpSubItem(env,"[-]"		,"Set group disselected.");
			PrintHelpSubItem(env,"[-cN]"	,"Set cycle, N is value.");
			PrintHelpSubItem(env,"[-tN]"	,"Set interval, N is value.");
			PrintHelpSubItem(env,"<gID>"	,"Group gID, s/uns.");
			return(cgReturnCode);
		};
		
		static	int32	SetPar(BIC_ENV *env, COMMAND_GROUP *tGroup, struct BIC_GCM_SET_PAR *tPar);
		static	int32	SetPar(BIC_ENV *env, GC_LIST *tGroupList, struct BIC_GCM_SET_PAR *tPar);
};
//------------------------------------------------------------------------------------------//
class BIC_GCM_CLONE : public BIC_Node{
	public:
				 BIC_GCM_CLONE(void) : BIC_Node() {cgCommand = "clone";cgReturnCode = BI_RETCODE_GC_CLONE;};
		virtual ~BIC_GCM_CLONE(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env, cgCommand, "Clone group by gID.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"[gID]","Group gID.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_GCM_DEL : public BIC_Node{
	public:
				 BIC_GCM_DEL(void) : BIC_Node() {cgCommand = "del";cgReturnCode = BI_RETCODE_GC_DEL;};
		virtual ~BIC_GCM_DEL(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env, cgCommand, "Delete group by gID.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"<gID>","Group gID.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_GCM_MVUP : public BIC_Node{
	public:
				 BIC_GCM_MVUP(void) : BIC_Node() {cgCommand = "mvup";cgReturnCode = BI_RETCODE_GC_MVUP;};
		virtual ~BIC_GCM_MVUP(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env, cgCommand, "Move group up.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"<gID>"	,"Group gID.");
			PrintHelpSubItem(env,"[step]"	,"Default value is 1.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_GCM_MVDN : public BIC_Node{
	public:
				 BIC_GCM_MVDN(void) : BIC_Node() {cgCommand = "mvdn";cgReturnCode = BI_RETCODE_GC_MVDN;};
		virtual ~BIC_GCM_MVDN(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env, cgCommand, "Move group down.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"<gID>"	,"Group gID.");
			PrintHelpSubItem(env,"[step]"	,"Default value is 1.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_GCM_EG : public BIC_Node{
	public:
				 BIC_GCM_EG(void) : BIC_Node() {cgCommand = "eg";cgReturnCode = BI_RETCODE_GC_EG;};
		virtual ~BIC_GCM_EG(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env, cgCommand, "Group record example.");
			if (blDetail == 0)
				return(cgReturnCode);
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_GCM_GN : public BIC_Node{
	public:
				 BIC_GCM_GN(void) : BIC_Node() {cgCommand = "gn";cgReturnCode = BI_RETCODE_GC_GN;};
		virtual ~BIC_GCM_GN(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env, cgCommand, "Set group name by gID.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"<gID>"	,"Group gID.");
			PrintHelpSubItem(env,"<name>"	,"Name.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_GCM_GROUP_LS : public BIC_Node{
	public:
				 BIC_GCM_GROUP_LS(void) : BIC_Node() {cgCommand = "ls/list";cgReturnCode = BI_RETCODE_GC_GROUP_LS;};
		virtual ~BIC_GCM_GROUP_LS(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env, cgCommand, "List command.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"[-s]"	,"Show selected item.");
			PrintHelpSubItem(env,"[-c]"	,"Show Cycle column.");
			PrintHelpSubItem(env,"[-t]"	,"Show Timeout column.");
			PrintHelpSubItem(env,"[-d]"	,"Show condition.");
			PrintHelpSubItem(env,"[-a]"	,"Show all.");
			PrintHelpSubItem(env,"[cID]","Command cID.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_GCM_GROUP_SET : public BIC_Node{
	public:
				 BIC_GCM_GROUP_SET(void) : BIC_Node() {cgCommand = "set";cgReturnCode = BI_RETCODE_GC_GROUP_SET;};
		virtual ~BIC_GCM_GROUP_SET(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env, cgCommand, "Set command.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"[+]"	,"Set command selected.");
			PrintHelpSubItem(env,"[-]"	,"Set command disselected.");
			PrintHelpSubItem(env,"[-cN]","Set cycle, N is value.");
			PrintHelpSubItem(env,"[-tN]","Set timeout, N is value.");
			PrintHelpSubItem(env,"[+r]","Enable send \"\\r\" in the end.");
			PrintHelpSubItem(env,"[-r]","Disable send \"\\r\" in the end.");
			PrintHelpSubItem(env,"[+n]","Enable send \"\\n\" in the end.");
			PrintHelpSubItem(env,"[-n]","Disable send \"\\n\" in the end.");
			PrintHelpSubItem(env,"<cID>","Command cID, s/uns.");
			return(cgReturnCode);
		};
		
		static	int32	SetPar(BIC_ENV *env, COMMAND_NODE *tCommand, struct BIC_GCM_GROUP_SET_PAR *tPar);
		static	int32	SetPar(BIC_ENV *env, COMMAND_GROUP *tGroup, struct BIC_GCM_GROUP_SET_PAR *tPar);
};
//------------------------------------------------------------------------------------------//
class BIC_GCM_GROUP_CLONE : public BIC_Node{
	public:
				 BIC_GCM_GROUP_CLONE(void) : BIC_Node() {cgCommand = "clone";cgReturnCode = BI_RETCODE_GC_GROUP_CLONE;};
		virtual ~BIC_GCM_GROUP_CLONE(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env, cgCommand, "Clone command by cID.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"[cID]","Command cID.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_GCM_GROUP_DEL : public BIC_Node{
	public:
				 BIC_GCM_GROUP_DEL(void) : BIC_Node() {cgCommand = "del";cgReturnCode = BI_RETCODE_GC_GROUP_DEL;};
		virtual ~BIC_GCM_GROUP_DEL(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env, cgCommand, "Delete command by cID.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"<cID>","Command cID.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_GCM_GROUP_MVUP : public BIC_Node{
	public:
				 BIC_GCM_GROUP_MVUP(void) : BIC_Node() {cgCommand = "mvup";cgReturnCode = BI_RETCODE_GC_GROUP_MVUP;};
		virtual ~BIC_GCM_GROUP_MVUP(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env, cgCommand, "Move command up.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"<cID>"	,"Command cID.");
			PrintHelpSubItem(env,"[step]"	,"Default value is 1.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_GCM_GROUP_MVDN : public BIC_Node{
	public:
				 BIC_GCM_GROUP_MVDN(void) : BIC_Node() {cgCommand = "mvdn";cgReturnCode = BI_RETCODE_GC_GROUP_MVDN;};
		virtual ~BIC_GCM_GROUP_MVDN(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env, cgCommand, "Move command down.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"<cID>"	,"Command cID.");
			PrintHelpSubItem(env,"[step]"	,"Default value is 1.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_GCM_GROUP_COMMAND : public BIC_Node{
	public:
				 BIC_GCM_GROUP_COMMAND(void) : BIC_Node() {cgCommand = "cmd/command";cgReturnCode = BI_RETCODE_GC_GROUP_COMMAND;};
		virtual ~BIC_GCM_GROUP_COMMAND(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env, cgCommand, "Set command by cID.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"<cID>"	,"Command cID.");
			PrintHelpSubItem(env,"<name>"	,"Name.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_GCM_GROUP_CONTINUE : public BIC_Node{
	public:
				 BIC_GCM_GROUP_CONTINUE(void) : BIC_Node() {cgCommand = "continue";cgReturnCode = BI_RETCODE_GC_GROUP_CONTINUE;};
		virtual ~BIC_GCM_GROUP_CONTINUE(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env, cgCommand, "Set continue by cID.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"<cID>"	,"Command cID.");
			PrintHelpSubItem(env,"<name>"	,"Name.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_GCM_GROUP_RESEND : public BIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
				 BIC_GCM_GROUP_RESEND(void) : BIC_Node() {cgCommand = "resend";cgReturnCode = BI_RETCODE_GC_GROUP_RESEND;};
		virtual ~BIC_GCM_GROUP_RESEND(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env, cgCommand, "Set resend by cID.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"<cID>"	,"Command cID.");
			PrintHelpSubItem(env,"<name>"	,"Name.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_GCM_GROUP_CSTOP : public BIC_Node{
	public:
				 BIC_GCM_GROUP_CSTOP(void) : BIC_Node() {cgCommand = "cstop";cgReturnCode = BI_RETCODE_GC_GROUP_CSTOP;};
		virtual ~BIC_GCM_GROUP_CSTOP(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env, cgCommand, "Set cstop by cID.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"<cID>"	,"Command cID.");
			PrintHelpSubItem(env,"<name>"	,"Name.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_GCM_GROUP_CATCH : public BIC_Node{
	public:
				 BIC_GCM_GROUP_CATCH(void) : BIC_Node() {cgCommand = "catch";cgReturnCode = BI_RETCODE_GC_GROUP_CATCH;};
		virtual ~BIC_GCM_GROUP_CATCH(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env, cgCommand, "Set catch by cID.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"<cID>"	,"Command cID.");
			PrintHelpSubItem(env,"<name>"	,"Name.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_GCM_GROUP_SEND : public BIC_Node{
	public:
				 BIC_GCM_GROUP_SEND(void) : BIC_Node() {cgCommand = "send";cgReturnCode = BI_RETCODE_GC_GROUP_SEND;};
		virtual ~BIC_GCM_GROUP_SEND(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *eda)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env, cgCommand, "Send command by cID.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"<cID>","Command cID.");
			return(cgReturnCode);
		};
	public:
		virtual	int32	InPressKeyModeExit	(BIC_ENV *env)const;
};
//------------------------------------------------------------------------------------------//
class BIC_GCM_GROUP : public BIC_Node_S{
	public:
				 BIC_GCM_GROUP(void) : BIC_Node_S() {cgCommand = "g/group";cgTitle = "group";cgReturnCode = BI_RETCODE_GC_GROUP;Init();};
		virtual ~BIC_GCM_GROUP(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *eda)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			if (blDetail == 1){
				HelpTraversalChild(env,0);
				return(cgReturnCode);
			}
			PrintHelpItem(env, cgCommand, "-> Into group by gID.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"<gID>","Group gID.");
			return(cgReturnCode);
		};
	private:
		void	Init(void){
			Add(cgSub_eg) < cgSub_load < cgSub_save
			< cgSub_script < cgSub_stop
			< cgSub_send
			< cgSub_ls < cgSub_set < cgSub_clone < cgSub_del < cgSub_mvup < cgSub_mvdn
			< cgSub_command < cgSub_continue < cgSub_resend < cgSub_cstop < cgSub_catch;
		}
		BIC_GCM_EG				cgSub_eg;
		BIC_GCM_LOAD			cgSub_load;
		BIC_GCM_SAVE			cgSub_save;
		BIC_GCM_GROUP_LS		cgSub_ls;
		BIC_GCM_GROUP_SET		cgSub_set;
		BIC_GCM_GROUP_CLONE		cgSub_clone;
		BIC_GCM_GROUP_DEL		cgSub_del;
		BIC_GCM_GROUP_MVUP		cgSub_mvup;
		BIC_GCM_GROUP_MVDN		cgSub_mvdn;
		BIC_GCM_GROUP_COMMAND	cgSub_command;
		BIC_GCM_GROUP_CONTINUE	cgSub_continue;
		BIC_GCM_GROUP_RESEND	cgSub_resend;
		BIC_GCM_GROUP_CSTOP		cgSub_cstop;
		BIC_GCM_GROUP_CATCH		cgSub_catch;
		BIC_SCRIPT				cgSub_script;
		BIC_STOP				cgSub_stop;
		BIC_GCM_GROUP_SEND		cgSub_send;
	};
//------------------------------------------------------------------------------------------//
class BIC_GCM : public BIC_Node_S{
	public:
				 BIC_GCM(void) : BIC_Node_S() {cgCommand = "gcm";cgTitle = cgCommand;cgReturnCode = BI_RETCODE_GC;Init();};
		virtual ~BIC_GCM(void){;};
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
			PrintHelpItem(env, cgCommand, "-> GCM.");
			if (blDetail == 0)
				return(cgReturnCode);
			return(cgReturnCode);
		};
	private:
		void	Init(void){
			Add(cgSub_eg) < cgSub_load < cgSub_save
			< cgSub_script < cgSub_stop < cgSub_run
			< cgSub_ls< cgSub_set< cgSub_clone< cgSub_del< cgSub_mvup< cgSub_mvdn
			< cgSub_gn < cgSub_group;
		}
		BIC_GCM_EG		cgSub_eg;
		BIC_GCM_LOAD	cgSub_load;
		BIC_GCM_SAVE	cgSub_save;
		BIC_GCM_LS		cgSub_ls;
		BIC_GCM_SET		cgSub_set;
		BIC_GCM_CLONE	cgSub_clone;
		BIC_GCM_DEL		cgSub_del;
		BIC_GCM_MVUP	cgSub_mvup;
		BIC_GCM_MVDN	cgSub_mvdn;
		BIC_GCM_GN		cgSub_gn;
		BIC_GCM_GROUP	cgSub_group;
		BIC_SCRIPT		cgSub_script;
		BIC_STOP		cgSub_stop;
		BIC_RUN			cgSub_run;
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif
#endif
#endif /* BIC_GCM_hpp */
