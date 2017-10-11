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
//------------------------------------------------------------------------------------------//
#include "SBIC_B.h"
#include "SBIC_Condition.h"
//------------------------------------------------------------------------------------------//
#include "SBIC_B.h"
#if defined SBIC_BH && defined SBIC_ConditionH
#ifndef SBIC_ExpressionH
#define SBIC_ExpressionH
#ifdef SBIC_ExpressionH
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
class SBIC_Delay : public SBIC_Node{
	public:
		SBIC_Delay(void) : SBIC_Node() {
			cgCommand = "delay,=";
			cgReturnCode = SBI_RETCODE_DELAY;
		};
		virtual ~SBIC_Delay(void){;};
	public:
		virtual	int32	Command	(SBIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(SBIC_ENV *env,int32 blDetail = 1)const{
			PrintB(env,".CMD = Delay=<PAR> -->Delay PAR time, then goto send next \"Command\". Default unit is second.");
			PrintB(env,"  Command = <'Delay=<PAR>>[//COMMENT]");
			PrintP(env,"   eg:");
			PrintP(env,"     Command = 'Delay=1.2      //delay 1.2s");
			PrintP(env,"     Command = 'Delay=1200ms   //delay 1200ms");
			PrintP(env,"     Command = 'Delay=1.2s     //delay 1.2s");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class SBIC_Synchronous : public SBIC_Node{
	public:
		SBIC_Synchronous(void) : SBIC_Node() {
			cgCommand = "Synchronous,=/Sync,=";
			cgReturnCode = SBI_RETCODE_SYNCHRONOUS;
		};
		virtual ~SBIC_Synchronous(void){;};
	public:
		virtual	int32	Command	(SBIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(SBIC_ENV *env,int32 blDetail = 1)const{
			PrintB(env,".CMD = Synchronous=<PAR1>,<PAR2> -->Time synchronization, PAR1 is offset of zone second, PAR2 is synchronization interval, uint is second.");
			PrintB(env,"  Command = <'Synchronous=<PAR1>,<PAR2>>[//COMMENT]");
			PrintP(env,"  Notes:");
			PrintP(env,"     1.If this is the first implementation, T0=TIME(HOUR(Tnow),MINUTE(Tnow),0)+PAR1.");
			PrintP(env,"        eg:Time is 10:12:45, record T0=10:12:00 + PAR1.");
			PrintP(env,"     2.According to PAR2 to calculate the execution time of next \"Command\". Tnext=T0+INT((Tnow-T0)/Timeout(ms)+1)*Timeout(ms).");
			PrintP(env,"     3.If this is not the first time implementation, T0 has no changed, re-calculate Tnext.");
			PrintP(env,"   eg:Time is 10:12:14");
			PrintP(env,"     Command = 'Synchronous=0,10      //T0=10:12:00, the execution time of next \"Command\" is 10:12:20, 10:12:30, 10:12:40,...");
			PrintP(env,"     Command = 'Synchronous=5,10      //T0=10:12:05, the execution time of next \"Command\" is 10:12:15, 10:12:25, 10:12:35,...");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class SBIC_Expression: public SBIC_Node{
	public:
		SBIC_Expression(void) : SBIC_Node() {
			cgCommand = "Expression,=";
			cgReturnCode = SBI_RETCODE_EXPRESSION;
		};
		virtual ~SBIC_Expression(void){;};
	public:
		virtual	int32	Command	(SBIC_ENV *env,const STDSTR &par,void *p) const;
		virtual	int32	Help	(SBIC_ENV *env,int32 blDetail = 1) const{return(cgReturnCode);};

	private:
		SBIC_RECEIVE	cgSubC_Receive;
		SBIC_Conditon	cgSubC_Condition;
		static	void	FormatString	(STDSTR *returnStr,const STDSTR &strIn);
		static	void	GetPoland		(STDSTR *polandQueue,const STDSTR &expressions);
	public:
		int32			Expression		(SBIC_ENV *env,const STDSTR &condition) const;
		int32			CheckResult		(SBUFFER *cstdin,const STDSTR &cCheckResult,int32 cTimeout);
};
//------------------------------------------------------------------------------------------//
class SBIC_Wait : public SBIC_Node{
	public:
		SBIC_Wait(void) : SBIC_Node() {
			cgCommand = "wait,=";
			cgReturnCode = SBI_RETCODE_WAIT;
		};
		virtual ~SBIC_Wait(void){;};
	public:
		virtual	int32	Command	(SBIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(SBIC_ENV *env,int32 blDetail = 1)const{
			PrintB(env,".CMD = Wait=<timeout>,<Expression> -->Wait until receive data contains expression,.");
			PrintB(env,"  Command = <'Wait=<timeout>,<Expression>>[//COMMENT]");
			PrintP(env,"  Notes:1.Expression is the same as Condition Expression.");
			PrintP(env,"   eg:");
			PrintP(env,"     Command = 'Wait=10.5s,'cts==H   //if CTS is high then goto send next \"Command\". Waiting timeout is 10.5s.");
			PrintP(env,"     Command = 'Wait=500ms,'cts==L   //if CTS is low then goto send next \"Command\". Waiting timeout is 500ms.");
			PrintP(env,"     Command = 'Wait=10.5s,'cts==H && 'dsr==H   //if CTS is high and DSR is high, then goto send next \"Command\".");
			PrintP(env,"     Command = 'Wait=10.5s,'cts==H && OK        //if CTS is high and receive OK, then goto send next \"Command\".");
			return(cgReturnCode);
		};
	private:
		SBIC_Expression	cgSubC_Expression;
};
//------------------------------------------------------------------------------------------//
class SBIC_Define : public SBIC_Node{
	public:
		SBIC_Define(void) : SBIC_Node() {
			cgCommand = "Define,:/Def,:";
			cgReturnCode = SBI_RETCODE_DEFINE;
		};
		virtual ~SBIC_Define(void){;};
	public:
		virtual	int32	Command	(SBIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(SBIC_ENV *env,int32 blDetail = 1)const{
			PrintB(env,".CMD = Define:<PAR> -->Set a define string. No execution command, used with CMD Replace.");
			PrintB(env,"  Command = <'Define:<PAR>>[//COMMENT]");
			PrintP(env,"   eg:");
			PrintP(env,"     Command = 'Define:v1=Leif Wen;v2=+86-138-6062-4143;      //used with CMD Replace");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class SBIC_Lable : public SBIC_Node{
	public:
		SBIC_Lable(void) : SBIC_Node() {
			cgCommand = "Lable,:";
			cgReturnCode = SBI_RETCODE_LABLE;
		};
		virtual ~SBIC_Lable(void){;};
	public:
		virtual	int32	Command	(SBIC_ENV *env,const STDSTR &par,void *retStr)const;
		virtual	int32	Help	(SBIC_ENV *env,int32 blDetail = 1)const{
			PrintB(env,".CMD = Lable:[PAR] -->Set a label. No execution command, used with CMD Goto.");
			PrintB(env,"  Command = <'Lable:[PAR]>[//COMMENT]");
			PrintP(env,"   eg:");
			PrintP(env,"     Command = 'Lable:001      //used with CMD Goto, scrpit will jump to here");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class SBIC_GOTO : public SBIC_Node{
	public:
		SBIC_GOTO(void) : SBIC_Node() {
			cgCommand = "goto,=";
			cgReturnCode = SBI_RETCODE_GOTO;
		};
		virtual ~SBIC_GOTO(void){;};
	public:
		virtual	int32	Command	(SBIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(SBIC_ENV *env,int32 blDetail = 1)const{
			PrintB(env,".CMD = Goto=<Lable>[,<Expression>] -->if receive data contains expression, then jump to Lable.");
			PrintB(env,"  Command = <'Goto=<Lable>[,<Expression>]>[//COMMENT]");
			PrintP(env,"  Notes:Expression is the same as Condition Expression.");
			PrintP(env,"   eg:");
			PrintP(env,"     Command = 'Goto=001,Leif Wen  //if receive data contains Leif Wen, then goto 001");
			PrintP(env,"     Command = 'Goto=001,'cts==h   //if CTS is high, then goto 001");
			return(cgReturnCode);
		};
	private:
		SBIC_Lable		cgSubC_Lable;
		SBIC_Expression	cgSubC_Expression;
};
//------------------------------------------------------------------------------------------//
class SBIC_STOP : public SBIC_Node{
	public:
		SBIC_STOP(void) : SBIC_Node() {
			cgCommand = "stop,=";
			cgReturnCode = SBI_RETCODE_STOP;
		};
		virtual ~SBIC_STOP(void){;};
	public:
		virtual	int32	Command	(SBIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(SBIC_ENV *env,int32 blDetail = 1)const{
			PrintB(env,".CMD = Stop=<Expression> -->if receive data contains expression, then stop to run.");
			PrintB(env,"  Command = <'Stop=<Expression>>[//COMMENT]");
			PrintP(env,"  Notes:Expression is the same as Condition Expression.");
			PrintP(env,"   eg:");
			PrintP(env,"     Command = 'Stop=Leif Wen  //if receive data contains Leif Wen, then stop");
			PrintP(env,"     Command = 'Stop='cts==h   //if CTS is high, then stop");
			return(cgReturnCode);
		};
	private:
		SBIC_Expression	cgSubC_Expression;
};
//------------------------------------------------------------------------------------------//
class SBIC_BREAK : public SBIC_Node{
	public:
		SBIC_BREAK(void) : SBIC_Node() {
			cgCommand = "break,=";
			cgReturnCode = SBI_RETCODE_BREAK;
		};
		virtual ~SBIC_BREAK(void){;};
	public:
		virtual	int32	Command	(SBIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(SBIC_ENV *env,int32 blDetail = 1)const{
			PrintB(env,".CMD = Break=<Expression> -->if receive data contains expression, then break to run.");
			PrintB(env,"  Command = <'Break=<Expression>>[//COMMENT]");
			PrintP(env,"  Notes:Expression is the same as Condition Expression.");
			PrintP(env,"   eg:");
			PrintP(env,"     Command = 'Break=Leif Wen  //if receive data contains Leif Wen, then break");
			PrintP(env,"     Command = 'Break='cts==h   //if CTS is high, then break");
			return(cgReturnCode);
		};
	private:
		SBIC_Expression	cgSubC_Expression;
};
//------------------------------------------------------------------------------------------//
#endif
#endif
#endif
