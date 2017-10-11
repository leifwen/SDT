/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: SBIC_RunEXE.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.08.12

*/
//------------------------------------------------------------------------------------------//
#include "SBIC_B.h"
#include "SBIC_Expression.h"
#include "Comm_FIFO.h"
#include "SYS_Thread.h"
#include "Commu_EXE.h"
//------------------------------------------------------------------------------------------//
#if defined Commu_EXE_hpp && defined SBIC_BH && defined SBIC_ExpressionH
#ifndef SBIC_RunEXEH
#define SBIC_RunEXEH
#ifdef SBIC_RunEXEH
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
class SBIC_RunEXE : public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
		SBIC_RunEXE(void) : SBIC_Node() {
			cgCommand = "RunEXE,=";
			cgReturnCode = SBI_RETCODE_RUNEXE;
		};
		virtual ~SBIC_RunEXE(void){;};
	public:
		virtual	int32	Command	(SBIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(SBIC_ENV *env,int32 blDetail = 1)const{
			PrintB(env,".CMD = RunEXE=<name>,<command> -->Execute external command,only use in Linux.");
			PrintB(env,"  Command = <'RunEXE=<name>,<command>>[//COMMENT]");
			PrintP(env,"  Notes:1.Can use 'lcs check the execution result.");
			PrintP(env,"        2.The name is used for distinguishing the same command.");
			PrintP(env,"     eg:");
			PrintP(env,"     Command = 'RunEXE=CMD1,pppd file ./script    //define EC name is CMD1,and EC is \"pppd file ./script\"");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class SBIC_StopEXE : public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
		SBIC_StopEXE(void) : SBIC_Node() {
			cgCommand = "StopEXE,=";
			cgReturnCode = SBI_RETCODE_STOPEXE;
		};
		virtual ~SBIC_StopEXE(void){;};
	public:
		virtual	int32	Command	(SBIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(SBIC_ENV *env,int32 blDetail = 1)const{
			PrintB(env,".CMD = StopEXE=<name>,<timeout> -->Stop external command,only use in Linux.");
			PrintB(env,"  Command = <'StopEXE=<name>,<timeout>>[//COMMENT]");
			PrintP(env,"  Notes:1.Can use 'lcs check the execution result.");
			PrintP(env,"        2.The name is defined in RunEXE.");
			PrintP(env,"     eg:");
			PrintP(env,"     Command = 'StopEXE=CMD1,5s  // stop EC which name is CMD1");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class SBIC_WaitEXE : public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
		SBIC_WaitEXE(void) : SBIC_Node() {
			cgCommand = "WaitEXE,=";
			cgReturnCode = SBI_RETCODE_WAITEXE;
		};
		virtual ~SBIC_WaitEXE(void){;};
	public:
		virtual	int32	Command	(SBIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(SBIC_ENV *env,int32 blDetail = 1)const{
			PrintB(env,".CMD = WaitEXE=<name>,<timeout>,<condition> -->Wait external command executing,only use in Linux.");
			PrintB(env,"  Command = <'WaitEXE=<name>,<timeout>,<condition>>[//COMMENT]");
			PrintP(env,"  Notes:1.Can use 'lcs check the execution result.");
			PrintP(env,"        2.The name is uesd for distinguishing the same command.");
			PrintP(env,"     eg:");
			PrintP(env,"     Command = 'WaitEXE=CMD1,5s,local  IP address  // WaitEXE EC executing which name is CMD1,and check whether contain \"local  IP address\"");
			return(cgReturnCode);
		};
	protected:
		SBIC_Expression	cgSubC_Expression;
	private:
		void PrintLineWithTime(SBIC_ENV *env,STDSTR *strIn)const;
};
//------------------------------------------------------------------------------------------//
class SBIC_GetEXE : public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
		SBIC_GetEXE(void) : SBIC_Node() {
			cgCommand = "GetEXE,=";
			cgReturnCode = SBI_RETCODE_GETEXE;
		};
		virtual ~SBIC_GetEXE(void){;};
	public:
		virtual	int32	Command	(SBIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(SBIC_ENV *env,int32 blDetail = 1)const{
			PrintB(env,".CMD = GetEXE=<name> -->Get external command status,only use in Linux.");
			PrintB(env,"  Command = <'GetEXE=<name>,<timeout>,<condition>>[//COMMENT]");
			PrintP(env,"  Notes:1.Can use 'lcs check the execution result.");
			PrintP(env,"        2.The name is uesd for distinguishing the same command.");
			PrintP(env,"        3.Set result to true if EC is in running.");
			PrintP(env,"     eg:");
			PrintP(env,"     Command = 'GetEXE=CMD1  // Get EC status which name is CMD1");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
#endif
#endif
#endif

