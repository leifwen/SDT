/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: SBIC_Expression.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.08.12

*/
#ifndef SBIC_ExpressionH
#define SBIC_ExpressionH
//------------------------------------------------------------------------------------------//
#include "SBIC_B.h"
#include "SBIC_Condition.h"
//------------------------------------------------------------------------------------------//
class SBIC_Expression: public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
		SBIC_Expression(void) : SBIC_Node() {
			cgCommand = "Expression,=";
			cgReturnCode = SBI_RETCODE_EXPRESSION;
		};
		virtual ~SBIC_Expression(void){;};
	public:
		virtual	int32	Command	(SBICPAR *tBICPAR,const std::string &par,std::string *ret) const;
		virtual	int32	Help	(SBICPAR *tBICPAR,int32 blDetail = 1) const;

	private:
		SBIC_RECEIVE	cgSubC_Receive;
		SBIC_Conditon	cgSubC_Condition;
		void			FormatString	(const std::string &strInput,std::string *returnStr) const;
		void			GetPoland		(const std::string &expressions,std::string *polandQueue) const;
	public:
		int32			Expression		(SBICPAR *tBICPAR,const std::string &strSource,const std::string &condition) const;
};
//------------------------------------------------------------------------------------------//
class SBIC_Wait : public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
		SBIC_Wait(void) : SBIC_Node() {
			cgCommand = "wait,=";
			cgReturnCode = SBI_RETCODE_WAIT;
		};
		virtual ~SBIC_Wait(void){;};
	public:
		virtual	int32	Command	(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(SBICPAR *tBICPAR,int32 blDetail = 1)const;
	private:
		SBIC_Expression	cgSubC_Expression;
};
//------------------------------------------------------------------------------------------//
class SBIC_Lable : public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
		SBIC_Lable(void) : SBIC_Node() {
			cgCommand = "Lable,:";
			cgReturnCode = SBI_RETCODE_LABLE;
		};
		virtual ~SBIC_Lable(void){;};
	public:
		virtual	int32	Command	(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(SBICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class SBIC_GOTO : public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
		SBIC_GOTO(void) : SBIC_Node() {
			cgCommand = "goto,=";
			cgReturnCode = SBI_RETCODE_GOTO;
		};
		virtual ~SBIC_GOTO(void){;};
	public:
		virtual	int32	Command	(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(SBICPAR *tBICPAR,int32 blDetail = 1)const;
	private:
		SBIC_Lable		cgSubC_Lable;
		SBIC_Expression	cgSubC_Expression;
};
//------------------------------------------------------------------------------------------//
#endif
