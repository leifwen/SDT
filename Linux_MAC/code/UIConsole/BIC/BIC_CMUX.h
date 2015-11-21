/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: BIC_CMUX.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.01.25
*/
#ifndef BIC_CMUXH
#define BIC_CMUXH
//------------------------------------------------------------------------------------------//
#include "BIC_B.h"
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
class BIC_CMUX_ON : public BIC_Node{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
			     BIC_CMUX_ON(void) : BIC_Node() {cgCommand = "on";cgReturnCode = BI_RETCODE_CMUX_ON;};
	    virtual ~BIC_CMUX_ON(void){;};
	public:
	    virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
	    virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_CMUX_OFF : public BIC_Node{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
		    	 BIC_CMUX_OFF(void) : BIC_Node() {cgCommand = "off";cgReturnCode = BI_RETCODE_CMUX_OFF;};
	    virtual ~BIC_CMUX_OFF(void){;};
	public:
	    virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
	    virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_CMUX_CLD : public BIC_Node{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
			     BIC_CMUX_CLD(void) : BIC_Node() {cgCommand = "cld";cgReturnCode = BI_RETCODE_CMUX_CLD;};
	    virtual ~BIC_CMUX_CLD(void){;};
	public:
	    virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
	    virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_CMUX_PSC : public BIC_Node{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
			     BIC_CMUX_PSC(void) : BIC_Node() {cgCommand = "psc";cgReturnCode = BI_RETCODE_CMUX_PSC;};
	    virtual ~BIC_CMUX_PSC(void){;};
	public:
	    virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
	    virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_CMUX_FC : public BIC_Node{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
			     BIC_CMUX_FC(void) : BIC_Node() {cgCommand = "fc";cgReturnCode = BI_RETCODE_CMUX_FC;};
	    virtual ~BIC_CMUX_FC(void){;};
	public:
	    virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
	    virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_DLCI : public BIC_Node{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
			     BIC_DLCI(void) : BIC_Node() {cgCommand = "dlci";cgReturnCode = BI_RETCODE_DLCI;};
	    virtual ~BIC_DLCI(void){;};
	public:
	    virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
	    virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_CMUX : public BIC_Node_S{
	public:
		enum{RFLAG_C = 0, RFLAG_S = BIC_Node_S::RFLAG_S + BIC_Node_S::RFLAG_C};
	public:
				 BIC_CMUX(void) : BIC_Node_S() {cgCommand = "cmux";cgTitle = cgCommand;cgReturnCode = BI_RETCODE_CMUX;Init();};
		virtual ~BIC_CMUX(void){;};
	public:
		virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
	    void	Init(void){
	        AddNode(&cgSub_on);
	        AddNode(&cgSub_off);
	        AddNode(&cgSub_cld);
	        AddNode(&cgSub_psc);
	        AddNode(&cgSub_fc);
            AddNode(&cgSub_dlci);
	    }
	    BIC_CMUX_ON		cgSub_on;
	    BIC_CMUX_OFF	cgSub_off;
	    BIC_CMUX_CLD	cgSub_cld;
	    BIC_CMUX_PSC	cgSub_psc;
	    BIC_CMUX_FC		cgSub_fc;
	    BIC_DLCI		cgSub_dlci;
};
//------------------------------------------------------------------------------------------//
#endif