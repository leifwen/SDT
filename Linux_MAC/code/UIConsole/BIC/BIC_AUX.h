/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: BIC_AUX.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.01.25
*/
#ifndef BIC_AXUH
#define BIC_AXUH
//------------------------------------------------------------------------------------------//
#include "BIC_B.h"
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
class BIC_AUX_COM : public BIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
				 BIC_AUX_COM(void) : BIC_Node() {cgCommand = "com";cgReturnCode = BI_RETCODE_AUX_COM;};
		virtual ~BIC_AUX_COM(void){;};
	public:
		virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_AUX_BR : public BIC_AUX_COM{
	public:
		enum{RFLAG_C = 0, RFLAG_S = BIC_AUX_COM::RFLAG_S + BIC_AUX_COM::RFLAG_C};
	public:
				 BIC_AUX_BR(void) : BIC_AUX_COM() {cgCommand = "br";cgReturnCode = BI_RETCODE_AUX_BR;};
		virtual ~BIC_AUX_BR(void){;};
	public:
		virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_AUX_REC: public BIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
				 BIC_AUX_REC(void) : BIC_Node() {cgCommand = "rec";cgReturnCode = BI_RETCODE_AUX_REC;};
		virtual ~BIC_AUX_REC(void){;};
	public:
		virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_AUX_MS : public BIC_Node{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
			     BIC_AUX_MS(void) : BIC_Node() {cgCommand = "ms";cgReturnCode = BI_RETCODE_AUX_MS;};
	    virtual ~BIC_AUX_MS(void){;};
	public:
	    virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
	    virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
class BIC_AUX_OPEN : public BIC_AUX_COM{
	public:
		enum{RFLAG_C = 0, RFLAG_S = BIC_AUX_COM::RFLAG_S + BIC_AUX_COM::RFLAG_C};
	public:
				 BIC_AUX_OPEN(void) : BIC_AUX_COM() {cgCommand = "open";cgReturnCode = BI_RETCODE_AUX_OPEN;};
		virtual ~BIC_AUX_OPEN(void){;};
	public:
		virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
class BIC_AUX_CLOSE : public BIC_AUX_COM{
	public:
		enum{RFLAG_C = 0, RFLAG_S = BIC_AUX_COM::RFLAG_S + BIC_AUX_COM::RFLAG_C};
	public:
				 BIC_AUX_CLOSE(void) : BIC_AUX_COM() {cgCommand = "close";cgReturnCode = BI_RETCODE_AUX_CLOSE;};
		virtual ~BIC_AUX_CLOSE(void){;};
	public:
		virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_AUX_DTR : public BIC_Node{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
			     BIC_AUX_DTR(void) : BIC_Node() {cgCommand = "dtr";cgReturnCode = BI_RETCODE_AUX_DTR;};
	    virtual ~BIC_AUX_DTR(void){;};
	public:
	    virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
	    virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_AUX_RTS : public BIC_Node{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
			     BIC_AUX_RTS(void) : BIC_Node() {cgCommand = "rts";cgReturnCode = BI_RETCODE_AUX_RTS;};
	    virtual ~BIC_AUX_RTS(void){;};
	public:
	    virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
	    virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};

//------------------------------------------------------------------------------------------//
class BIC_AUX : public BIC_Node_S{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node_S::RFLAG_S + BIC_Node_S::RFLAG_C};
	public:
				 BIC_AUX(void) : BIC_Node_S() {cgCommand = "aux";cgTitle = cgCommand;cgReturnCode = BI_RETCODE_AUX;Init();};
	    virtual ~BIC_AUX(void){;};
	public:
		virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const{
			PrintHelpItem(tBICPAR, cgCommand, "->Auxiliary connection setting.");
			if (blDetail == 0)
				return(cgReturnCode);
			return(cgReturnCode);
		};
	private:
	    void	Init(void){
			AddNode(&cgSub_open);
			AddNode(&cgSub_close);
			AddNode(&cgSub_rec);
	        AddNode(&cgSub_com);
	        AddNode(&cgSub_br);
            AddNode(&cgSub_ms);
            AddNode(&cgSub_dtr);
            AddNode(&cgSub_rts);
	    }
		BIC_AUX_OPEN	cgSub_open;
		BIC_AUX_CLOSE	cgSub_close;
		BIC_AUX_REC		cgSub_rec;
	    BIC_AUX_COM		cgSub_com;
	    BIC_AUX_BR		cgSub_br;
    	BIC_AUX_MS		cgSub_ms;
	    BIC_AUX_DTR		cgSub_dtr;
	    BIC_AUX_RTS		cgSub_rts;
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif
