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
			PrintB(env,".CMD = RunEXE=<lable>,<timeout>,<command> -->Execute external command,only use in Linux.");
			PrintB(env,"  Command = <'RunEXE=<lable>,<timeout>,<command>>[//COMMENT]");
			PrintP(env,"  Notes:1.Can use 'lsc check the execution result.");
			PrintP(env,"        2.The lable is used for distinguishing the same command.");
			PrintP(env,"        3.Condition is used for checking whether matched EC output data.");
			PrintP(env,"     eg:");
			PrintP(env,"     Command = 'RunEXE= 001,10s,pppd file ./script");
			PrintP(env,"     Condition = \"local  IP address\"");
			PrintP(env,"     Execute pppd file ./script, and timeout is 10s.");
			PrintP(env,"     if the command output contain \"local  IP address\", the execution result is true.");
			return(cgReturnCode);
		};
	protected:
		SBIC_Expression	cgSubC_Expression;
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
			PrintB(env,".CMD = StopEXE=<lable>,<timeout>,<command> -->Stop external command,only use in Linux.");
			PrintB(env,"  Command = <'StopEXE=<lable>,<timeout>,<command>>[//COMMENT]");
			PrintP(env,"  Notes:1.Can use 'lsc check the execution result.");
			PrintP(env,"        2.The lable is uesd for distinguishing the same command.");
			PrintP(env,"     eg:");
			PrintP(env,"     Command = 'StopEXE=001,5s,pppd file ./script  // stop EC pppd file ./script which lable is 001");
			return(cgReturnCode);
		};
	protected:
		SBIC_Expression	cgSubC_Expression;
};
//------------------------------------------------------------------------------------------//
#endif
#endif
#endif

