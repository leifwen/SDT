/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: BIC_Other.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.01.25
*/
#ifndef BIC_OtherH
#define BIC_OtherH
//------------------------------------------------------------------------------------------//
#include "BIC_B.h"
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
class BIC_NEWRECORD : public BIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
				 BIC_NEWRECORD(void) : BIC_Node() {cgCommand = "newrecord/nr";cgReturnCode = BI_RETCODE_NEWRECORD;};
		virtual ~BIC_NEWRECORD(void){;};
	public:
		virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_PATCHSDT : public BIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
				 BIC_PATCHSDT(void) : BIC_Node() {cgCommand = "patch";cgReturnCode = BI_RETCODE_PATCHSDT;};
		virtual ~BIC_PATCHSDT(void){;};
	public:
		virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_LSERVER : public BIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
				 BIC_LSERVER(void) : BIC_Node() {cgCommand = "lserver";cgReturnCode = BI_RETCODE_LSERVER;};
		virtual ~BIC_LSERVER(void){;};
	public:
		virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_CREGREPORT : public BIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
				 BIC_CREGREPORT(void) : BIC_Node() {cgCommand = "creg";cgReturnCode = BI_RETCODE_CREGREPORT;};
		virtual ~BIC_CREGREPORT(void){;};
	public:
		virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_NTP : public BIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
				 BIC_NTP(void) : BIC_Node() {cgCommand = "ntp";cgReturnCode = BI_RETCODE_NTP;};
		virtual ~BIC_NTP(void){;};
	public:
		virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
		virtual	int32	NTPSync(BICPAR *tBICPAR, const std::string &IP, const int32 port, std::string *retPackage, double *dTCompensation, double *dT1Ret, double *dT4Ret)const;
};
//------------------------------------------------------------------------------------------//
class BIC_NTPSERVER : public BIC_Node{
	public:
		enum{ RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C };
	public:
				 BIC_NTPSERVER(void) : BIC_Node() { cgCommand = "ntps"; cgReturnCode = BI_RETCODE_NTPSERVER; };
		virtual ~BIC_NTPSERVER(void){ ; };
	public:
		virtual	int32	Command(BICPAR *tBICPAR, const std::string &par, std::string *ret)const;
		virtual	int32	Help(BICPAR *tBICPAR, int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_TERMINALSERVER : public BIC_Node{
	public:
	    enum{ RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C };
	public:
			     BIC_TERMINALSERVER(void) : BIC_Node() { cgCommand = "ts"; cgReturnCode = BI_RETCODE_TERMINALSERVER; };
	    virtual ~BIC_TERMINALSERVER(void){ ; };
	public:
	    virtual	int32	Command(BICPAR *tBICPAR, const std::string &par, std::string *ret)const;
	    virtual	int32	Help(BICPAR *tBICPAR, int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif