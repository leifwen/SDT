//
//  BIC_GCM.h
//  SDT
//
//  Created by Leif Wen on 19/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#include "BIC_Define.h"
#if defined BIC_Define_h && defined GList_h
//------------------------------------------------------------------------------------------//
#ifndef BIC_GCM_h
#define BIC_GCM_h
#ifdef BIC_GCM_h
//------------------------------------------------------------------------------------------//
BIC_CLASSTYPE(GC_EG,		"eg");
BIC_CLASSTYPE(GC_LOAD,		"load");
BIC_CLASSTYPE(GC_SAVE,		"save");
BIC_CLASSTYPE(GC_LS,		"ls/list");
BIC_CLASSTYPE(GC_SET,		"set");
BIC_CLASSTYPE(GC_CLONE,		"clone");
BIC_CLASSTYPE(GC_DEL,		"del");
BIC_CLASSTYPE(GC_MVUP,		"mvup");
BIC_CLASSTYPE(GC_MVDN,		"mvdn");
BIC_CLASSTYPE(GC_NAME,		"name");
//------------------------------------------------------------------------------------------//
class GC_GROUP_BASE : public BIC_BASE{
	public:
				 GC_GROUP_BASE(void) : BIC_BASE(){;};
		virtual ~GC_GROUP_BASE(void){;};
	public:
		virtual	CMDID	Execute		(CMD_ENV* env,const uint32& mID,const STDSTR& msg,void* gID)const;
};
//------------------------------------------------------------------------------------------//
#define BIC_GROUPCLASSTYPE(_NAME,_CMD)		CMD_CLASSTYPE(BIC_##_NAME,GC_GROUP_BASE,BIC_ID_##_NAME,_CMD);
//------------------------------------------------------------------------------------------//
BIC_GROUPCLASSTYPE(GC_GROUP_LS,			"ls/list");
BIC_GROUPCLASSTYPE(GC_GROUP_SET,		"set");
BIC_GROUPCLASSTYPE(GC_GROUP_CLONE,		"clone");
BIC_GROUPCLASSTYPE(GC_GROUP_DEL,		"del");
BIC_GROUPCLASSTYPE(GC_GROUP_MVUP,		"mvup");
BIC_GROUPCLASSTYPE(GC_GROUP_MVDN,		"mvdn");
BIC_GROUPCLASSTYPE(GC_GROUP_COMMAND,	"cmd/command");
BIC_GROUPCLASSTYPE(GC_GROUP_CONTINUE,	"continue");
BIC_GROUPCLASSTYPE(GC_GROUP_RESEND,		"resend");
BIC_GROUPCLASSTYPE(GC_GROUP_CSTOP,		"cstop");
BIC_GROUPCLASSTYPE(GC_GROUP_CATCH,		"catch");
//------------------------------------------------------------------------------------------//
class BIC_GC_GROUP : public BIC_BASE_S{
	public:
				 BIC_GC_GROUP(void);
		virtual ~BIC_GC_GROUP(void){;};
	public:
		virtual	CMDID	MessageProcessing		(CMD_ENV* env,const uint32& mID,const STDSTR& msg,void* p)const;
		virtual	CMDID	Help					(CMD_ENV* env,uint32 flag)const;
	public:
		BIC_GC_GROUP_LS			cgSub_ls;
		BIC_GC_GROUP_SET		cgSub_set;
		BIC_GC_GROUP_CLONE		cgSub_clone;
		BIC_GC_GROUP_DEL		cgSub_del;
		BIC_GC_GROUP_MVUP		cgSub_mvup;
		BIC_GC_GROUP_MVDN		cgSub_mvdn;
		BIC_GC_GROUP_COMMAND	cgSub_command;
		BIC_GC_GROUP_CONTINUE	cgSub_continue;
		BIC_GC_GROUP_RESEND		cgSub_resend;
		BIC_GC_GROUP_CSTOP		cgSub_cstop;
		BIC_GC_GROUP_CATCH		cgSub_catch;
};
//------------------------------------------------------------------------------------------//
class BIC_GCM : public BIC_BASE_S{
	public:
				 BIC_GCM(void);
		virtual ~BIC_GCM(void){;};
	public:
		BIC_GC_EG		cgSub_eg;
		BIC_GC_LOAD		cgSub_load;
		BIC_GC_SAVE		cgSub_save;
		BIC_GC_LS		cgSub_ls;
		BIC_GC_SET		cgSub_set;
		BIC_GC_CLONE	cgSub_clone;
		BIC_GC_DEL		cgSub_del;
		BIC_GC_MVUP		cgSub_mvup;
		BIC_GC_MVDN		cgSub_mvdn;
		BIC_GC_NAME		cgSub_name;
		BIC_GC_GROUP	cgSub_group;
};
//------------------------------------------------------------------------------------------//
#endif /* BIC_GCM_h */
#endif /* BIC_GCM_h */
#endif /* BIC_Define_h */
