/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: BuildInCommand.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.01.25
 */
#ifndef BuildInCommandH
#define BuildInCommandH
//------------------------------------------------------------------------------------------//
#include "BIC.h"
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
class BuildInCommand : public BIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
		BuildInCommand(void) : BIC_Node(){
			cgTitle = "SDT";
		
			AddNode(&cgC_EXIT);
			AddNode(&cgC_HELP);
		
		#ifdef CommonDefH_Unix
			AddNode(&cgC_EXEC);
			AddNode(&cgC_EXE_BASH);
			AddNode(&cgC_EXE_SH);
		#endif
		
			AddNode(&cgC_DISPLAY);
			AddNode(&cgC_NEWRECORD);
			
			AddNode(&cgC_MAIN);
			
		#ifdef SWVERSION_AUXCOM
			AddNode(&cgC_AUX);
		#endif
		
			AddNode(&cgC_CONNECT);
			AddNode(&cgC_DISCONNECT);
			AddNode(&cgC_CR);
			
		#ifdef SWVERSION_SCRIPT
			AddNode(&cgC_SCM);
			AddNode(&cgC_GCM);
			AddNode(&cgC_Script);
			AddNode(&cgC_STOP);
			AddNode(&cgC_RUN);
		
			AddNode(&cgC_SEND);
			AddNode(&cgC_SENDA);
		#endif
		
		#ifdef SWVERSION_CMUX
			AddNode(&cgC_CMUX);
		#endif
		#if (defined USE_OPENSSL) || (defined CommonDefH_MAC)
			AddNode(&cgC_Calc);
		#endif

		#ifdef SWVERSION_NTP
			AddNode(&cgC_NTP);
			AddNode(&cgC_NTPS);
		#endif

			AddNode(&cgC_TS);
			
		#ifdef USE_OPENSSL
			AddNode(&cgC_RST);
			AddNode(&cgC_CREGREPORT);
		#ifdef CommonDefH_MAC
			AddNode(&cgC_PATCHSDT);
			AddNode(&cgC_APPROVE);
			AddNode(&cgC_LSERVER);
		#endif
		#endif
		#ifdef SWVERSION_DICT
			AddNode(&cgC_DICT);
		#endif
	};
	virtual ~BuildInCommand(void){;};
	public:
		int32	Do		(BICPAR *tBICPAR);
	public:
		BIC_EXIT		cgC_EXIT;
		BIC_HELP		cgC_HELP;
	
	#ifdef CommonDefH_Unix
		BIC_EXEC		cgC_EXEC;
		BIC_EXE_BASH	cgC_EXE_BASH;
		BIC_EXE_SH		cgC_EXE_SH;
	#endif
	
		BIC_DISPLAY		cgC_DISPLAY;
		BIC_NEWRECORD	cgC_NEWRECORD;
	
		BIC_MAIN		cgC_MAIN;
	
	#ifdef SWVERSION_AUXCOM
		BIC_AUX			cgC_AUX;
	#endif
	
		BIC_CONNECT		cgC_CONNECT;
		BIC_DISCONNECT	cgC_DISCONNECT;
		BIC_CR			cgC_CR;
	
	#ifdef SWVERSION_SCRIPT
		BIC_SCM			cgC_SCM;
		BIC_GCM			cgC_GCM;
		BIC_SCRIPT		cgC_Script;
		BIC_STOP		cgC_STOP;
		BIC_RUN			cgC_RUN;
	
		BIC_SEND		cgC_SEND;
		BIC_SENDA		cgC_SENDA;
	#endif
	
	#ifdef SWVERSION_COM
		BIC_CMUX		cgC_CMUX;
	#endif
	
	#if (defined USE_OPENSSL) || (defined CommonDefH_MAC)
		BIC_CALC		cgC_Calc;
	#endif

	#ifdef SWVERSION_NTP
		BIC_NTP			cgC_NTP;
		BIC_NTPSERVER	cgC_NTPS;
	#endif
	
		BIC_TERMINALSERVER	cgC_TS;
	
	#ifdef USE_OPENSSL
		BIC_RST			cgC_RST;
		BIC_CREGREPORT	cgC_CREGREPORT;
	#ifdef CommonDefH_MAC
		BIC_PATCHSDT	cgC_PATCHSDT;
		BIC_APPROVE		cgC_APPROVE;
		BIC_LSERVER		cgC_LSERVER;
	#endif
	#endif
	#ifdef SWVERSION_DICT
		BIC_DICT		cgC_DICT;
	#endif
};
//------------------------------------------------------------------------------------------//
#endif