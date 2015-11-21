/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: BIC_COM.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.01.25
*/
#ifndef BIC_COMH
#define BIC_COMH
//------------------------------------------------------------------------------------------//
#include "BIC_B.h"
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
class BIC_COM : public BIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
				 BIC_COM(void) : BIC_Node() {cgCommand = "com";cgReturnCode = BI_RETCODE_COM;};
		virtual ~BIC_COM(void){;};
	public:
		virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_COM_BR : public BIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
				 BIC_COM_BR(void) : BIC_Node() {cgCommand = "br";cgReturnCode = BI_RETCODE_COM_BR;};
		virtual ~BIC_COM_BR(void){;};
	public:
		virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_COM_DTR : public BIC_Node{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
			     BIC_COM_DTR(void) : BIC_Node() {cgCommand = "dtr";cgReturnCode = BI_RETCODE_COM_DTR;};
	    virtual ~BIC_COM_DTR(void){;};
	public:
	    virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
	    virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_COM_RTS : public BIC_Node{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
		    	 BIC_COM_RTS(void) : BIC_Node() {cgCommand = "rts";cgReturnCode = BI_RETCODE_COM_RTS;};
	    virtual ~BIC_COM_RTS(void){;};
	public:
	    virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
	    virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_COM_MS : public BIC_Node{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
			     BIC_COM_MS(void) : BIC_Node() {cgCommand = "ms";cgReturnCode = BI_RETCODE_COM_MS;};
	    virtual ~BIC_COM_MS(void){;};
	public:
	    virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
	    virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_COM_LS : public BIC_Node{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
			     BIC_COM_LS(void) : BIC_Node() {cgCommand = "lscom";cgReturnCode = BI_RETCODE_COM_LS;};
	    virtual ~BIC_COM_LS(void){;};
	public:
	    virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
	    virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif