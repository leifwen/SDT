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
				 BIC_NEWRECORD(void) : BIC_Node() {cgCommand = "nr/newrecord";cgReturnCode = BI_RETCODE_NEWRECORD;};
		virtual ~BIC_NEWRECORD(void){;};
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
		virtual	int32	NTPSync(BICPAR *tBICPAR, const std::string &IP, const int32 port, std::string *retPackage, double *dTCompensation, double *dT1Ret,double *dT4Ret)const;
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
#ifdef USE_OPENSSL
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
class BIC_APPROVE : public BIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
				 BIC_APPROVE(void) : BIC_Node() {cgCommand = "approve";cgReturnCode = BI_RETCODE_APPROVE;};
		virtual ~BIC_APPROVE(void){;};
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
class BIC_MSDT : public BIC_Node_S{
	public:
		enum{RFLAG_C = 0, RFLAG_S = BIC_Node_S::RFLAG_S + BIC_Node_S::RFLAG_C};
	public:
		BIC_MSDT(void) : BIC_Node_S() {cgCommand = "msdt";cgTitle = cgCommand;cgReturnCode = BI_RETCODE_MSDT;Init();};
		virtual ~BIC_MSDT(void){;};
	public:
		virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const{
			PrintHelpItem(tBICPAR, cgCommand, "->SDT manage.");
			if (blDetail == 0)
				return(cgReturnCode);
			return(cgReturnCode);
		};
	private:
		void	Init(void){
			AddNode(&cgC_PATCHSDT);
			AddNode(&cgC_APPROVE);
			AddNode(&cgC_LSERVER);
		}
		BIC_PATCHSDT	cgC_PATCHSDT;
		BIC_APPROVE		cgC_APPROVE;
		BIC_LSERVER		cgC_LSERVER;
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
class BIC_RST_ON : public BIC_Node{
	public:
		enum{ RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C };
	public:
				 BIC_RST_ON(void) : BIC_Node() { cgCommand = "on"; cgReturnCode = BI_RETCODE_RST_ON; };
		virtual ~BIC_RST_ON(void){ ; };
	public:
		virtual	int32	Command(BICPAR *tBICPAR, const std::string &par, std::string *ret)const;
		virtual	int32	Help(BICPAR *tBICPAR, int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_RST_OFF : public BIC_Node{
	public:
		enum{ RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C };
	public:
				 BIC_RST_OFF(void) : BIC_Node() { cgCommand = "off"; cgReturnCode = BI_RETCODE_RST_OFF; };
		virtual ~BIC_RST_OFF(void){ ; };
	public:
		virtual	int32	Command(BICPAR *tBICPAR, const std::string &par, std::string *ret)const;
		virtual	int32	Help(BICPAR *tBICPAR, int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_RST_LINK : public BIC_Node{
	public:
		enum{ RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C };
	public:
				 BIC_RST_LINK(void) : BIC_Node() { cgCommand = "link"; cgReturnCode = BI_RETCODE_RST_LINK; };
		virtual ~BIC_RST_LINK(void){ ; };
	public:
		virtual	int32	Command(BICPAR *tBICPAR, const std::string &par, std::string *ret)const;
		virtual	int32	Help(BICPAR *tBICPAR, int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_RST_MAPPING : public BIC_Node{
	public:
		enum{ RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C };
	public:
				 BIC_RST_MAPPING(void) : BIC_Node() { cgCommand = "mapping"; cgReturnCode = BI_RETCODE_RST_MAPPING; };
		virtual ~BIC_RST_MAPPING(void){ ; };
	public:
		virtual	int32	Command(BICPAR *tBICPAR, const std::string &par, std::string *ret)const;
		virtual	int32	Help(BICPAR *tBICPAR, int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_RST_LOGIN : public BIC_Node{
	public:
		enum{ RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C };
	public:
				 BIC_RST_LOGIN(void) : BIC_Node() { cgCommand = "login"; cgReturnCode = BI_RETCODE_RST_LOGIN; };
		virtual ~BIC_RST_LOGIN(void){ ; };
	public:
		virtual	int32	Command(BICPAR *tBICPAR, const std::string &par, std::string *ret)const;
		virtual	int32	Help(BICPAR *tBICPAR, int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
class BIC_RST : public BIC_Node_S{
	public:
		enum{RFLAG_C = 0, RFLAG_S = BIC_Node_S::RFLAG_S + BIC_Node_S::RFLAG_C};
	public:
			 BIC_RST(void) : BIC_Node_S() {cgCommand = "rst";cgTitle = cgCommand;cgReturnCode = BI_RETCODE_RST;Init();};
	virtual ~BIC_RST(void){;};
	public:
		virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const{
			PrintHelpItem(tBICPAR, cgCommand, "->Remote SSL terminal server.");
			if (blDetail == 0)
				return(cgReturnCode);
			return(cgReturnCode);
		};
	private:
		void	Init(void){
			AddNode(&cgSub_on);
			AddNode(&cgSub_off);
			AddNode(&cgSub_link);
			AddNode(&cgSub_mapping);
			AddNode(&cgSub_login);
		}
		BIC_RST_ON				cgSub_on;
		BIC_RST_OFF				cgSub_off;
		BIC_RST_LINK			cgSub_link;
		BIC_RST_MAPPING			cgSub_mapping;
		BIC_RST_LOGIN			cgSub_login;
};
//------------------------------------------------------------------------------------------//
#endif
//------------------------------------------------------------------------------------------//
#endif
