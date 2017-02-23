/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: BIC_Script.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.01.25
*/
#ifndef BIC_ScriptH
#define BIC_ScriptH
//------------------------------------------------------------------------------------------//
#include "BIC_B.h"
//------------------------------------------------------------------------------------------//
#define	BIC_GCM_LS_FLAG_GLshowSelect	B_FLAG64(0)
#define	BIC_GCM_LS_FLAG_GLshowCycle		B_FLAG64(1)
#define	BIC_GCM_LS_FLAG_GLshowInterval	B_FLAG64(2)
#define	BIC_GCM_LS_FLAG_GLshowCL		B_FLAG64(3)

#define	BIC_GCM_LS_FLAG_CLshowSelect	B_FLAG64(4)
#define	BIC_GCM_LS_FLAG_CLshowCycle		B_FLAG64(5)
#define	BIC_GCM_LS_FLAG_CLshowTimeout	B_FLAG64(6)
#define	BIC_GCM_LS_FLAG_CLshowDetail	B_FLAG64(7)

#define	BIC_GCM_LS_FLAG_GLSelect		B_FLAG64(8)
#define	BIC_GCM_LS_FLAG_GLsetSelect		B_FLAG64(9)
#define	BIC_GCM_LS_FLAG_GLsetCycle		B_FLAG64(10)
#define	BIC_GCM_LS_FLAG_GLsetInterval	B_FLAG64(11)

#define	BIC_GCM_LS_FLAG_CLSelect		B_FLAG64(12)
#define	BIC_GCM_LS_FLAG_CLsetSelect		B_FLAG64(13)
#define	BIC_GCM_LS_FLAG_CLsetCycle		B_FLAG64(14)
#define	BIC_GCM_LS_FLAG_CLsetTimeout	B_FLAG64(15)
#define	BIC_GCM_LS_FLAG_CLsetHEX		B_FLAG64(16)
#define	BIC_GCM_LS_FLAG_CLsetCR			B_FLAG64(17)
//------------------------------------------------------------------------------------------//
class BIC_RUN : public BIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
				 BIC_RUN(void) : BIC_Node() {cgCommand = "run";cgReturnCode = BI_RETCODE_RUN;};
		virtual ~BIC_RUN(void){;};
	public:
		virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_STOP : public BIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
				 BIC_STOP(void) : BIC_Node() {cgCommand = "stop";cgReturnCode = BI_RETCODE_STOP;};
		virtual ~BIC_STOP(void){;};
	public:
		virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_SEND : public BIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
				 BIC_SEND(void) : BIC_Node() {cgCommand = "send";cgReturnCode = BI_RETCODE_SEND;};
		virtual ~BIC_SEND(void){;};
	public:
		virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_SENDA : public BIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
				 BIC_SENDA(void) : BIC_Node() {cgCommand = "senda";cgReturnCode = BI_RETCODE_SENDA;};
		virtual ~BIC_SENDA(void){;};
	public:
		virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_SENDFILE : public BIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
		BIC_SENDFILE(void) : BIC_Node() {cgCommand = "sendfile";cgReturnCode = BI_RETCODE_SENDFILE;};
		virtual ~BIC_SENDFILE(void){;};
	public:
		virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_SCRIPT : public BIC_Node{
	public:
		enum{ RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C };
	public:
				 BIC_SCRIPT(void) : BIC_Node() { cgCommand = "script"; cgReturnCode = BI_RETCODE_SCRIPT; };
		virtual ~BIC_SCRIPT(void){ ; };
	public:
		virtual	int32	Command(BICPAR *tBICPAR, const std::string &par, std::string *ret)const;
		virtual	int32	Help(BICPAR *tBICPAR, int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//



















//------------------------------------------------------------------------------------------//
class BIC_SCM_LOAD : public BIC_Node{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
			     BIC_SCM_LOAD(void) : BIC_Node() {cgCommand = "load";cgReturnCode = BI_RETCODE_SC_LOAD;};
	    virtual ~BIC_SCM_LOAD(void){;};
	public:
	    virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
	    virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_SCM_SAVE : public BIC_Node{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
			     BIC_SCM_SAVE(void) : BIC_Node() {cgCommand = "save";cgReturnCode = BI_RETCODE_SC_SAVE;};
	    virtual ~BIC_SCM_SAVE(void){;};
	public:
	    virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
	    virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_SCM_LS : public BIC_Node{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
	    		 BIC_SCM_LS(void) : BIC_Node() {cgCommand = "ls/list";cgReturnCode = BI_RETCODE_SC_LS;};
	    virtual ~BIC_SCM_LS(void){;};
	public:
	    virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
	    virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_SCM_SET : public BIC_Node{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
	    		 BIC_SCM_SET(void) : BIC_Node() {cgCommand = "set";cgReturnCode = BI_RETCODE_SC_SET;};
	    virtual ~BIC_SCM_SET(void){;};
	public:
	    virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
	    virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_SCM_CLONE : public BIC_Node{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
    			 BIC_SCM_CLONE(void) : BIC_Node() {cgCommand = "clone";cgReturnCode = BI_RETCODE_SC_CLONE;};
	    virtual ~BIC_SCM_CLONE(void){;};
	public:
	    virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
	    virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_SCM_DEL : public BIC_Node{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
			     BIC_SCM_DEL(void) : BIC_Node() {cgCommand = "del";cgReturnCode = BI_RETCODE_SC_DEL;};
	    virtual ~BIC_SCM_DEL(void){;};
	public:
	    virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
	    virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_SCM_MVUP : public BIC_Node{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
			     BIC_SCM_MVUP(void) : BIC_Node() {cgCommand = "mvup";cgReturnCode = BI_RETCODE_SC_MVUP;};
	    virtual ~BIC_SCM_MVUP(void){;};
	public:
	    virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
	    virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_SCM_MVDN : public BIC_Node{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
			     BIC_SCM_MVDN(void) : BIC_Node() {cgCommand = "mvdn";cgReturnCode = BI_RETCODE_SC_MVDN;};
	    virtual ~BIC_SCM_MVDN(void){;};
	public:
	    virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
	    virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_SCM_EG : public BIC_Node{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
			     BIC_SCM_EG(void) : BIC_Node() {cgCommand = "eg";cgReturnCode = BI_RETCODE_SC_EG;};
	    virtual ~BIC_SCM_EG(void){;};
	public:
	    virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
	    virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_SCM_SEND : public BIC_Node{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
			     BIC_SCM_SEND(void) : BIC_Node() {cgCommand = "send";cgReturnCode = BI_RETCODE_SC_SEND;};
	    virtual ~BIC_SCM_SEND(void){;};
	public:
	    virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
	    virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_SCM_COMMAND : public BIC_Node{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
			     BIC_SCM_COMMAND(void) : BIC_Node() {cgCommand = "cmd/command";cgReturnCode = BI_RETCODE_SC_COMMAND;};
	    virtual ~BIC_SCM_COMMAND(void){;};
	public:
	    virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
	    virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_SCM : public BIC_Node_S{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node_S::RFLAG_S + BIC_Node_S::RFLAG_C};
	public:
				 BIC_SCM(void) : BIC_Node_S() {cgCommand = "scm";cgTitle = cgCommand;cgReturnCode = BI_RETCODE_SC;Init();};
	    virtual ~BIC_SCM(void){;};
	public:
	    virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
	    virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
	private:
	    void	Init(void){
            AddNode(&cgSub_eg);
	        AddNode(&cgSub_load);
	        AddNode(&cgSub_save);
	        AddNode(&cgSub_ls);
	        AddNode(&cgSub_set);
	        AddNode(&cgSub_clone);
	        AddNode(&cgSub_del);
	        AddNode(&cgSub_mvup);
	        AddNode(&cgSub_mvdn);
            AddNode(&cgSub_command);
            AddNode(&cgSub_send);
	    }
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







//------------------------------------------------------------------------------------------//
class BIC_GCM_LOAD : public BIC_Node{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
	   			 BIC_GCM_LOAD(void) : BIC_Node() {cgCommand = "load";cgReturnCode = BI_RETCODE_GC_LOAD;};
	    virtual ~BIC_GCM_LOAD(void){;};
	public:
	    virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
	    virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_GCM_SAVE : public BIC_SCM_SAVE{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_SCM_SAVE::RFLAG_S + BIC_SCM_SAVE::RFLAG_C};
	public:
	    		 BIC_GCM_SAVE(void) : BIC_SCM_SAVE() {cgCommand = "save";cgReturnCode = BI_RETCODE_GC_SAVE;};
	    virtual ~BIC_GCM_SAVE(void){;};
	public:
	    //virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
	    //virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class GC_LIST;
class COMMAND_GROUP;
class COMMAND_NODE;
//------------------------------------------------------------------------------------------//
class BIC_GCM_LS : public BIC_Node{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
	    		 BIC_GCM_LS(void) : BIC_Node() {cgCommand = "ls/list";cgReturnCode = BI_RETCODE_GC_LS;};
	    virtual ~BIC_GCM_LS(void){;};
	public:
	    virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
	    virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
    
        static	int32	BIC_GCM_LS_PrintGroupList	(BICPAR *tBICPAR,GC_LIST *tGroupList,uint64 flag);
		static	int32	BIC_GCM_LS_PringCommandList	(BICPAR *tBICPAR,COMMAND_GROUP *tGroup,uint64 flag);
	    static	int32	BIC_GCM_LS_PringCommandTitle(BICPAR *tBICPAR,uint64 flag);
	    static	int32	BIC_GCM_LS_PringCommand		(BICPAR *tBICPAR,COMMAND_NODE *tCommand,uint64 flag);
};
//------------------------------------------------------------------------------------------//
class BIC_GCM_SET : public BIC_Node{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
	   			 BIC_GCM_SET(void) : BIC_Node() {cgCommand = "set";cgReturnCode = BI_RETCODE_GC_SET;};
	    virtual ~BIC_GCM_SET(void){;};
	public:
	    virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
	    virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
    
    	static	int32	SetPar(BICPAR *tBICPAR, COMMAND_GROUP *tGroup, struct BIC_GCM_SET_PAR *tPar);
    	static	int32	SetPar(BICPAR *tBICPAR, GC_LIST *tGroupList, struct BIC_GCM_SET_PAR *tPar);
};
//------------------------------------------------------------------------------------------//
class BIC_GCM_CLONE : public BIC_Node{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
	    		 BIC_GCM_CLONE(void) : BIC_Node() {cgCommand = "clone";cgReturnCode = BI_RETCODE_GC_CLONE;};
	    virtual ~BIC_GCM_CLONE(void){;};
	public:
	    virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
	    virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_GCM_DEL : public BIC_Node{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
			     BIC_GCM_DEL(void) : BIC_Node() {cgCommand = "del";cgReturnCode = BI_RETCODE_GC_DEL;};
	    virtual ~BIC_GCM_DEL(void){;};
	public:
	    virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
	    virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_GCM_MVUP : public BIC_Node{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
				 BIC_GCM_MVUP(void) : BIC_Node() {cgCommand = "mvup";cgReturnCode = BI_RETCODE_GC_MVUP;};
	    virtual ~BIC_GCM_MVUP(void){;};
	public:
	    virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
	    virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_GCM_MVDN : public BIC_Node{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
			     BIC_GCM_MVDN(void) : BIC_Node() {cgCommand = "mvdn";cgReturnCode = BI_RETCODE_GC_MVDN;};
	    virtual ~BIC_GCM_MVDN(void){;};
	public:
	    virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
	    virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_GCM_EG : public BIC_Node{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
			     BIC_GCM_EG(void) : BIC_Node() {cgCommand = "eg";cgReturnCode = BI_RETCODE_GC_EG;};
	    virtual ~BIC_GCM_EG(void){;};
	public:
	    virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
	    virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_GCM_GN : public BIC_Node{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
			     BIC_GCM_GN(void) : BIC_Node() {cgCommand = "gn";cgReturnCode = BI_RETCODE_GC_GN;};
	    virtual ~BIC_GCM_GN(void){;};
	public:
	    virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
	    virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_GCM_GROUP_LS : public BIC_Node{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
	    		 BIC_GCM_GROUP_LS(void) : BIC_Node() {cgCommand = "ls/list";cgReturnCode = BI_RETCODE_GC_GROUP_LS;};
	    virtual ~BIC_GCM_GROUP_LS(void){;};
	public:
	    virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
	    virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_GCM_GROUP_SET : public BIC_Node{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
			     BIC_GCM_GROUP_SET(void) : BIC_Node() {cgCommand = "set";cgReturnCode = BI_RETCODE_GC_GROUP_SET;};
	    virtual ~BIC_GCM_GROUP_SET(void){;};
	public:
	    virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
	    virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
    
	    static	int32	SetPar(BICPAR *tBICPAR, COMMAND_NODE *tCommand, struct BIC_GCM_GROUP_SET_PAR *tPar);
	    static	int32	SetPar(BICPAR *tBICPAR, COMMAND_GROUP *tGroup, struct BIC_GCM_GROUP_SET_PAR *tPar);
};
//------------------------------------------------------------------------------------------//
class BIC_GCM_GROUP_CLONE : public BIC_Node{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
			     BIC_GCM_GROUP_CLONE(void) : BIC_Node() {cgCommand = "clone";cgReturnCode = BI_RETCODE_GC_GROUP_CLONE;};
	    virtual ~BIC_GCM_GROUP_CLONE(void){;};
	public:
	    virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
	    virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_GCM_GROUP_DEL : public BIC_Node{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
			     BIC_GCM_GROUP_DEL(void) : BIC_Node() {cgCommand = "del";cgReturnCode = BI_RETCODE_GC_GROUP_DEL;};
	    virtual ~BIC_GCM_GROUP_DEL(void){;};
	public:
	    virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
	    virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_GCM_GROUP_MVUP : public BIC_Node{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
		    	 BIC_GCM_GROUP_MVUP(void) : BIC_Node() {cgCommand = "mvup";cgReturnCode = BI_RETCODE_GC_GROUP_MVUP;};
	    virtual ~BIC_GCM_GROUP_MVUP(void){;};
	public:
	    virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
	    virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_GCM_GROUP_MVDN : public BIC_Node{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
		    	 BIC_GCM_GROUP_MVDN(void) : BIC_Node() {cgCommand = "mvdn";cgReturnCode = BI_RETCODE_GC_GROUP_MVDN;};
	    virtual ~BIC_GCM_GROUP_MVDN(void){;};
	public:
	    virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
	    virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_GCM_GROUP_COMMAND : public BIC_Node{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
		    	 BIC_GCM_GROUP_COMMAND(void) : BIC_Node() {cgCommand = "cmd/command";cgReturnCode = BI_RETCODE_GC_GROUP_COMMAND;};
	    virtual ~BIC_GCM_GROUP_COMMAND(void){;};
	public:
	    virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
	    virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_GCM_GROUP_CONTINUE : public BIC_Node{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
		    	 BIC_GCM_GROUP_CONTINUE(void) : BIC_Node() {cgCommand = "continue";cgReturnCode = BI_RETCODE_GC_GROUP_CONTINUE;};
	    virtual ~BIC_GCM_GROUP_CONTINUE(void){;};
	public:
	    virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
	    virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_GCM_GROUP_RESEND : public BIC_Node{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
			     BIC_GCM_GROUP_RESEND(void) : BIC_Node() {cgCommand = "resend";cgReturnCode = BI_RETCODE_GC_GROUP_RESEND;};
	    virtual ~BIC_GCM_GROUP_RESEND(void){;};
	public:
	    virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
	    virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_GCM_GROUP_CSTOP : public BIC_Node{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
			     BIC_GCM_GROUP_CSTOP(void) : BIC_Node() {cgCommand = "cstop";cgReturnCode = BI_RETCODE_GC_GROUP_CSTOP;};
	    virtual ~BIC_GCM_GROUP_CSTOP(void){;};
	public:
	    virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
	    virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_GCM_GROUP_CATCH : public BIC_Node{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
			     BIC_GCM_GROUP_CATCH(void) : BIC_Node() {cgCommand = "catch";cgReturnCode = BI_RETCODE_GC_GROUP_CATCH;};
	    virtual ~BIC_GCM_GROUP_CATCH(void){;};
	public:
	    virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
	    virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_GCM_GROUP_SEND : public BIC_Node{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
			     BIC_GCM_GROUP_SEND(void) : BIC_Node() {cgCommand = "send";cgReturnCode = BI_RETCODE_GC_GROUP_SEND;};
	    virtual ~BIC_GCM_GROUP_SEND(void){;};
	public:
	    virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
	    virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_GCM_GROUP : public BIC_Node_S{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node_S::RFLAG_S + BIC_Node_S::RFLAG_C};
	public:
                 BIC_GCM_GROUP(void) : BIC_Node_S() {cgCommand = "g/group";cgTitle = "group";cgReturnCode = BI_RETCODE_GC_GROUP;Init();};
	    virtual ~BIC_GCM_GROUP(void){;};
	public:
	    virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
	    virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
	private:
	    void	Init(void){
			AddNode(&cgSub_script);
			AddNode(&cgSub_stop);
			AddNode(&cgSub_send);
            AddNode(&cgSub_load);
            AddNode(&cgSub_save);
			AddNode(&cgSub_ls);
    		AddNode(&cgSub_set);
            AddNode(&cgSub_clone);
            AddNode(&cgSub_del);
            AddNode(&cgSub_mvup);
            AddNode(&cgSub_mvdn);
            AddNode(&cgSub_command);
            AddNode(&cgSub_continue);
            AddNode(&cgSub_resend);
            AddNode(&cgSub_cstop);
            AddNode(&cgSub_catch);
    	}
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
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node_S::RFLAG_S + BIC_Node_S::RFLAG_C};
	public:
			     BIC_GCM(void) : BIC_Node_S() {cgCommand = "gcm";cgTitle = cgCommand;cgReturnCode = BI_RETCODE_GC;Init();};
	    virtual ~BIC_GCM(void){;};
	public:
	    virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
	    virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
	private:
    	void	Init(void){
			AddNode(&cgSub_script);
			AddNode(&cgSub_stop);
			AddNode(&cgSub_run);
            AddNode(&cgSub_eg);
            AddNode(&cgSub_load);
            AddNode(&cgSub_save);
            AddNode(&cgSub_ls);
            AddNode(&cgSub_set);
            AddNode(&cgSub_clone);
            AddNode(&cgSub_del);
            AddNode(&cgSub_mvup);
            AddNode(&cgSub_mvdn);
            AddNode(&cgSub_gn);
            AddNode(&cgSub_group);
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
#endif
