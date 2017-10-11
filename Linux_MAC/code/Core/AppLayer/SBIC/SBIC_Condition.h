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
//------------------------------------------------------------------------------------------//
#include "SBIC_B.h"
#include "SBIC_Search.h"
#include "SBIC_Build.h"
//------------------------------------------------------------------------------------------//
#include "SBIC_B.h"
#if defined SBIC_BH && defined SBIC_SearchH && defined SBIC_BuildH
#ifndef SBIC_ConditionH
#define SBIC_ConditionH
#ifdef SBIC_ConditionH
//------------------------------------------------------------------------------------------//
class SBIC_RECEIVE : public SBIC_Node{
	public:
		SBIC_RECEIVE(void) : SBIC_Node() {
			cgCommand = "receive,==";
			cgReturnCode = SBI_RETCODE_RECEIVE;
		};
		virtual ~SBIC_RECEIVE(void){;};
	public:
		virtual	int32	Command	(SBIC_ENV *env,const STDSTR &par,void *retTrue)const;
		virtual	int32	Help	(SBIC_ENV *env,int32 blDetail = 1)const{
			PrintB(env,".CMD = receive==<par> -->check recevie buffer status.Used as sub-command in Condition Expression.");
			PrintB(env,"  Command = <'receive==<par>>");
			PrintP(env,"  Notes:1.support * and ?.");
			PrintP(env,"        2.Expression Operators is +.");
			PrintP(env,"        3.Can use \"\" set string. Use \\\", \\+ to escape \", +.");
			PrintP(env,"        4.Support \\0xhh, \\0Xhh, \\a, \\b, \\f, \\n, \\r, \\t, \\v, \\\\, \\', \\\", \\0, \\/, \\*, \\?.");
			PrintP(env,"        5.support sub command :same as 'combine.");
			PrintP(env,"   eg:");
			PrintP(env,"     Condition Expression = 'receive== OK   //if receive buffer contain OK, the Condition Expression is true");
			PrintP(env,"     Condition Expression = 'receive== O+K  //if receive buffer contain OK, the Condition Expression is true");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class SBIC_CTS : public SBIC_Node{
	public:
		SBIC_CTS(void) : SBIC_Node() {
			cgCommand = "cts,==";
			cgReturnCode = SBI_RETCODE_CTS;
		};
		virtual ~SBIC_CTS(void){;};
	public:
		virtual	int32	Command	(SBIC_ENV *env,const STDSTR &par,void *retTrue)const;
		virtual	int32	Help	(SBIC_ENV *env,int32 blDetail = 1)const{
			PrintB(env,".CMD = CTS==<H|L> -->check CTS status.Used as sub-command in Condition Expression.");
			PrintB(env,"  Command = <'CTS==<H|L>>");
			PrintP(env,"   eg:");
			PrintP(env,"     Condition Expression = 'CTS==H      //if CTS is high, the Condition Expression is true");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class SBIC_DSR : public SBIC_Node{
	public:
		SBIC_DSR(void) : SBIC_Node() {
			cgCommand = "dsr,==";
			cgReturnCode = SBI_RETCODE_DSR;
		};
		virtual ~SBIC_DSR(void){;};
	public:
		virtual	int32	Command	(SBIC_ENV *env,const STDSTR &par,void *retTrue)const;
		virtual	int32	Help	(SBIC_ENV *env,int32 blDetail = 1)const{
			PrintB(env,".CMD = DSR==<H|L> -->check DSR status.Used as sub-command in Condition Expression.");
			PrintB(env,"  Command = <'DSR==<H|L>>");
			PrintP(env,"   eg:");
			PrintP(env,"     Condition Expression = 'DST==H      //if DSR is high, the Condition Expression is true");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class SBIC_RING : public SBIC_Node{
	public:
		SBIC_RING(void) : SBIC_Node() {
			cgCommand = "ring,==";
			cgReturnCode = SBI_RETCODE_RING;
		};
		virtual ~SBIC_RING(void){;};
	public:
		virtual	int32	Command	(SBIC_ENV *env,const STDSTR &par,void *retTrue)const;
		virtual	int32	Help	(SBIC_ENV *env,int32 blDetail = 1)const{
			PrintB(env,".CMD = RING==<H|L> -->check RING status.Used as sub-command in Condition Expression.");
			PrintB(env,"  Command = <'RING==<H|L>>");
			PrintP(env,"   eg:");
			PrintP(env,"     Condition Expression = 'RING==H      //if RING is high, the Condition Expression is true");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class SBIC_DCD : public SBIC_Node{
	public:
		SBIC_DCD(void) : SBIC_Node() {
			cgCommand = "dcd,==";
			cgReturnCode = SBI_RETCODE_DCD;
		};
		virtual ~SBIC_DCD(void){;};
	public:
		virtual	int32	Command	(SBIC_ENV *env,const STDSTR &par,void *retTrue)const;
		virtual	int32	Help	(SBIC_ENV *env,int32 blDetail = 1)const{
			PrintB(env,".CMD = DCD==<H|L> -->check DCD status.Used as sub-command in Condition Expression.");
			PrintB(env,"  Command = <'DCD==<H|L>>");
			PrintP(env,"   eg:");
			PrintP(env,"     Condition Expression = 'DCD==H      //if DCD is high, the Condition Expression is true");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class SBIC_TIMEOUT : public SBIC_Node{
	public:
		SBIC_TIMEOUT(void) : SBIC_Node() {
			cgCommand = "timeout";
			cgReturnCode = SBI_RETCODE_TIMEOUT;
		};
		virtual ~SBIC_TIMEOUT(void){;};
	public:
		virtual	int32	Command	(SBIC_ENV *env,const STDSTR &par,void *retTrue)const;
		virtual	int32	Help	(SBIC_ENV *env,int32 blDetail = 1)const{
			PrintB(env,".CMD = Timeout -->check timeout status.Used as sub-command in Condition Expression.");
			PrintB(env,"  Command = <'Timeout>");
			PrintP(env,"   eg:");
			PrintP(env,"     Condition Expression = 'Timeout      //if Timeout is true, the Condition Expression is true");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class SBIC_nullptr : public SBIC_Node{
	public:
		SBIC_nullptr(void) : SBIC_Node() {
			cgCommand = "nullptr";
			cgReturnCode = SBI_RETCODE_nullptr;
		};
		virtual ~SBIC_nullptr(void){;};
	public:
		virtual	int32	Command	(SBIC_ENV *env,const STDSTR &par,void *retTrue)const;
		virtual	int32	Help	(SBIC_ENV *env,int32 blDetail = 1)const{
			PrintB(env,".CMD = nullptr -->check receive buffer status.Used as sub-command in Condition Expression.");
			PrintB(env,"  Command = <'nullptr>");
			PrintP(env,"   eg:");
			PrintP(env,"     Condition Expression = 'nullptr      //if nothing received, the Condition Expression is true");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class SBIC_RX_B : public SBIC_Node{
	public:
		SBIC_RX_B(void) : SBIC_Node() {
			cgCommand = "rx,>";
			cgReturnCode = SBI_RETCODE_RX_B;
		};
		virtual ~SBIC_RX_B(void){;};
	public:
		virtual	int32	Command	(SBIC_ENV *env,const STDSTR &par,void *retTrue)const;
		virtual	int32	Help	(SBIC_ENV *env,int32 blDetail = 1)const{
			PrintB(env,".CMD = RX> <num> -->check receive buffer status.Used as sub-command in Condition Expression.");
			PrintB(env,"  Command = <'RX> <num>>");
			PrintP(env,"   eg:");
			PrintP(env,"     Condition Expression = 'RX>10      //if the bytes received is more then 10, the Condition Expression is true");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class SBIC_RX_S : public SBIC_Node{
	public:
		SBIC_RX_S(void) : SBIC_Node() {
			cgCommand = "rx,<";
			cgReturnCode = SBI_RETCODE_RX_S;
		};
		virtual ~SBIC_RX_S(void){;};
	public:
		virtual	int32	Command	(SBIC_ENV *env,const STDSTR &par,void *retTrue)const;
		virtual	int32	Help	(SBIC_ENV *env,int32 blDetail = 1)const{
			PrintB(env,".CMD = RX< <num> -->check receive buffer status.Used as sub-command in Condition Expression.");
			PrintB(env,"  Command = <'RX< <num>>");
			PrintP(env,"   eg:");
			PrintP(env,"     Condition Expression = 'RX<10      //if the bytes received is less then 10, the Condition Expression is true");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class SBIC_RX_E : public SBIC_Node{
	public:
		SBIC_RX_E(void) : SBIC_Node() {
			cgCommand = "rx,==";
			cgReturnCode = SBI_RETCODE_RX_E;
		};
		virtual ~SBIC_RX_E(void){;};
	public:
		virtual	int32	Command	(SBIC_ENV *env,const STDSTR &par,void *retTrue)const;
		virtual	int32	Help	(SBIC_ENV *env,int32 blDetail = 1)const{
			PrintB(env,".CMD = RX==<num> -->check receive buffer status.Used as sub-command in Condition Expression.");
			PrintB(env,"  Command = <'RX==<num>>");
			PrintP(env,"   eg:");
			PrintP(env,"     Condition Expression = 'RX==10      //if the bytes received equal to 10, the Condition Expression is true");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class SBIC_LCS : public SBIC_Node{
	public:
		SBIC_LCS(void) : SBIC_Node() {
			cgCommand = "lcs,==";
			cgReturnCode = SBI_RETCODE_LCS;
		};
		virtual ~SBIC_LCS(void){;};
	public:
		virtual	int32	Command	(SBIC_ENV *env,const STDSTR &par,void *retTrue)const;
		virtual	int32	Help	(SBIC_ENV *env,int32 blDetail = 1)const{
			PrintB(env,".CMD = lcs==<T|F> -->check last command execution status.Used as sub-command in Condition Expression.");
			PrintB(env,"  Command = <'lcs==<T|F>>");
			PrintP(env,"  Notes:1.T is true,F is false.");
			PrintP(env,"        2.support command,'Search, 'ChangeCom, 'OpenTCPSocket, 'CloseTCPSocket 'OpenUDPSocket, 'CloseUDPSocket.");
			PrintP(env,"   eg:");
			PrintP(env,"     Last command is 'ChangeCom and execute successful.");
			PrintP(env,"     Condition Expression = 'lcs==T      //if 'ChangeCom execute successful the Condition Expression is true");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class SBIC_Conditon: public SBIC_Node{
	public:
		SBIC_Conditon(void) : SBIC_Node() {
			cgCommand = "Condition,=";
			cgReturnCode = SBI_RETCODE_CONDITION;
			Add(cgSubC_Receive) < cgSubC_CTS <cgSubC_DSR < cgSubC_RING < cgSubC_DCD
			< cgSubC_Timeout
			< cgSubC_nullptr < cgSubC_RX_B < cgSubC_RX_S < cgSubC_RX_E
			< cgSubC_LCS;
		};
		virtual ~SBIC_Conditon(void){;};
	public:
		virtual	int32	Execute	(SBIC_ENV *env,const STDSTR &fullCMD,void *retTrue)const;
		virtual	int32	Help	(SBIC_ENV *env,int32 blDetail = 1) const;
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
#endif
#endif
