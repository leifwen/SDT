/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: SBIC_Condition.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.08.12
*/
#ifndef SBIC_ConditionH
#define SBIC_ConditionH
//------------------------------------------------------------------------------------------//
#include "SBIC_B.h"
#include "SBIC_Search.h"
#include "SBIC_Build.h"
//------------------------------------------------------------------------------------------//
class SBIC_CTS : public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
		SBIC_CTS(void) : SBIC_Node() {
			cgCommand = "cts,==";
			cgReturnCode = SBI_RETCODE_CTS;
		};
		virtual ~SBIC_CTS(void){;};
	public:
		virtual	int32	Command	(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(SBICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class SBIC_DSR : public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
		SBIC_DSR(void) : SBIC_Node() {
			cgCommand = "dsr,==";
			cgReturnCode = SBI_RETCODE_DSR;
		};
		virtual ~SBIC_DSR(void){;};
	public:
		virtual	int32	Command	(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(SBICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class SBIC_RING : public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
		SBIC_RING(void) : SBIC_Node() {
			cgCommand = "ring,==";
			cgReturnCode = SBI_RETCODE_RING;
		};
		virtual ~SBIC_RING(void){;};
	public:
		virtual	int32	Command	(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(SBICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class SBIC_DCD : public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
		SBIC_DCD(void) : SBIC_Node() {
			cgCommand = "dcd,==";
			cgReturnCode = SBI_RETCODE_DCD;
		};
		virtual ~SBIC_DCD(void){;};
	public:
		virtual	int32	Command	(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(SBICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class SBIC_RECEIVE : public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
		SBIC_RECEIVE(void) : SBIC_Node() {
			cgCommand = "receive,==";
			cgReturnCode = SBI_RETCODE_RECEIVE;
		};
		virtual ~SBIC_RECEIVE(void){;};
	public:
		virtual	int32	Command	(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(SBICPAR *tBICPAR,int32 blDetail = 1)const;
	private:
		SBIC_Build		cgSubC_Build;
		SBIC_Search		cgSubC_Search;
};
//------------------------------------------------------------------------------------------//
class SBIC_TIMEOUT : public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
		SBIC_TIMEOUT(void) : SBIC_Node() {
			cgCommand = "timeout";
			cgReturnCode = SBI_RETCODE_TIMEOUT;
		};
		virtual ~SBIC_TIMEOUT(void){;};
	public:
		virtual	int32	Command	(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(SBICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class SBIC_nullptr : public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
		SBIC_nullptr(void) : SBIC_Node() {
			cgCommand = "nullptr";
			cgReturnCode = SBI_RETCODE_nullptr;
		};
		virtual ~SBIC_nullptr(void){;};
	public:
		virtual	int32	Command	(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(SBICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class SBIC_RX_B : public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
		SBIC_RX_B(void) : SBIC_Node() {
			cgCommand = "rx,>";
			cgReturnCode = SBI_RETCODE_RX_B;
		};
		virtual ~SBIC_RX_B(void){;};
	public:
		virtual	int32	Command	(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(SBICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class SBIC_RX_S : public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
		SBIC_RX_S(void) : SBIC_Node() {
			cgCommand = "rx,<";
			cgReturnCode = SBI_RETCODE_RX_S;
		};
		virtual ~SBIC_RX_S(void){;};
	public:
		virtual	int32	Command	(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(SBICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class SBIC_RX_E : public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
		SBIC_RX_E(void) : SBIC_Node() {
			cgCommand = "rx,==";
			cgReturnCode = SBI_RETCODE_RX_E;
		};
		virtual ~SBIC_RX_E(void){;};
	public:
		virtual	int32	Command	(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(SBICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class SBIC_LCS : public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
		SBIC_LCS(void) : SBIC_Node() {
			cgCommand = "lcs,==";
			cgReturnCode = SBI_RETCODE_LCS;
		};
		virtual ~SBIC_LCS(void){;};
	public:
		virtual	int32	Command	(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(SBICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class SBIC_Conditon: public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
		SBIC_Conditon(void) : SBIC_Node() {
			cgCommand = "Condition,=";
			cgReturnCode = SBI_RETCODE_CONDITION;
			AddNode(&cgSubC_Receive);
			AddNode(&cgSubC_CTS);
			AddNode(&cgSubC_DSR);
			AddNode(&cgSubC_RING);
			AddNode(&cgSubC_DCD);
			AddNode(&cgSubC_Timeout);
			AddNode(&cgSubC_nullptr);
			AddNode(&cgSubC_RX_B);
			AddNode(&cgSubC_RX_S);
			AddNode(&cgSubC_RX_E);
			AddNode(&cgSubC_LCS);
		};
		virtual ~SBIC_Conditon(void){;};
	public:
		virtual	int32	Execute	(SBICPAR *tBICPAR,const std::string &fullCMD,std::string *ret)const;
		virtual	int32	Help	(SBICPAR *tBICPAR,int32 blDetail = 1) const;
	private:
		SBIC_RECEIVE	cgSubC_Receive;
		SBIC_CTS		cgSubC_CTS;
		SBIC_DSR		cgSubC_DSR;
		SBIC_RING		cgSubC_RING;
		SBIC_DCD		cgSubC_DCD;
		SBIC_TIMEOUT	cgSubC_Timeout;
		SBIC_nullptr	cgSubC_nullptr;
		SBIC_RX_B		cgSubC_RX_B;
		SBIC_RX_S		cgSubC_RX_S;
		SBIC_RX_E		cgSubC_RX_E;
		SBIC_LCS		cgSubC_LCS;
};
//------------------------------------------------------------------------------------------//
#endif