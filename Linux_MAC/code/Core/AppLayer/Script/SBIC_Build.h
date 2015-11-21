/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: SBIC_Build.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.08.12
*/
#ifndef SBIC_BuildH
#define SBIC_BuildH
//------------------------------------------------------------------------------------------//
#include "SBIC_B.h"
//------------------------------------------------------------------------------------------//
class SBIC_SQ : public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
		SBIC_SQ(void) : SBIC_Node() {
			cgCommand = "'";
			cgReturnCode = SBI_RETCODE_SQ;
		};
		virtual ~SBIC_SQ(void){;};
	public:
		virtual	int32	Check	(SBICPAR *tBICPAR,const std::string &fullCMD,std::string *retPar)const;
		virtual	int32	Command	(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(SBICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class SBIC_HEX : public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
		SBIC_HEX(void) : SBIC_Node() {
			cgCommand = "hex,:";
			cgReturnCode = SBI_RETCODE_HEX;
		};
		virtual ~SBIC_HEX(void){;};
	public:
		virtual	int32	Command	(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(SBICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class SBIC_Time : public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
		SBIC_Time(void) : SBIC_Node() {
			cgCommand = "time,(,)/now,(,)";
			cgReturnCode = SBI_RETCODE_TIME;
		};
		virtual ~SBIC_Time(void){;};
	public:
		virtual	int32	Command	(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(SBICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class SBIC_RET : public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
		SBIC_RET(void) : SBIC_Node() {
			cgCommand = "ret,(,)";
			cgReturnCode = SBI_RETCODE_RET;
		};
		virtual ~SBIC_RET(void){;};
	public:
		virtual	int32	Command	(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(SBICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class SBIC_STRING : public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
		SBIC_STRING(void) : SBIC_Node() {
			cgCommand = "string,(,)/str,(,)";
			cgReturnCode = SBI_RETCODE_STRING;
		};
		virtual ~SBIC_STRING(void){;};
	public:
		virtual	int32	Command	(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(SBICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class SBIC_Build : public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
		SBIC_Build(void) : SBIC_Node() {
			cgCommand = "build,=/send,=";
			cgReturnCode = SBI_RETCODE_BUILD;
			AddNode(&cgSubC_SQ);
			AddNode(&cgSubC_HEX);
			AddNode(&cgSubC_Time);
			AddNode(&cgSubC_RET);
			AddNode(&cgSubC_STRING);
		};
		virtual ~SBIC_Build(void){;};
	public:
		virtual	int32	Command	(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(SBICPAR *tBICPAR,int32 blDetail = 1)const;
	private:
		SBIC_RET	cgSubC_RET;
		SBIC_Time	cgSubC_Time;
		SBIC_HEX	cgSubC_HEX;
		SBIC_SQ		cgSubC_SQ;
		SBIC_STRING	cgSubC_STRING;
		static	void		FormatString(const std::string &strInput,std::string *returnStr);
		static	std::string	SplitLetter	(std::string *strInput);
	public:
		int32		Build		(SBICPAR *tBICPAR,const std::string &strInput,std::string *retStr)const;
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif
