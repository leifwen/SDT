/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: SBIC.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.08.12
*/
#ifndef SBICH
#define SBICH
//------------------------------------------------------------------------------------------//
#include "SBIC_B.h"
#include "SBIC_Build.h"
#include "SBIC_Search.h"
#include "SBIC_Condition.h"
#include "SBIC_Expression.h"
#include "SBIC_RunExternal.h"
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
class SBIC_Print : public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
		SBIC_Print(void) : SBIC_Node() {
			cgCommand = "print,=";
			cgReturnCode = SBI_RETCODE_PRINT;
			AddNode(&cgSubC_SQ);
			AddNode(&cgSubC_HEX);
			AddNode(&cgSubC_Time);
			AddNode(&cgSubC_STRING);
			AddNode(&cgSubC_Build);
		};
		virtual ~SBIC_Print(void){;};
	public:
		virtual	int32	Command	(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(SBICPAR *tBICPAR,int32 blDetail = 1)const;
	private:
		SBIC_SQ		cgSubC_SQ;
		SBIC_HEX	cgSubC_HEX;
		SBIC_Time	cgSubC_Time;
		SBIC_STRING	cgSubC_STRING;
		SBIC_Build	cgSubC_Build;
	public:
		int32 CreateHexCommand(SBICPAR *cSBICPAR,const std::string &inputCommand,int32 blHEX,int32 bl0x0D,G_ESCAPE_VAILD blEscape
							   ,std::string *retHexCommand,std::string *retPrintData)const;
};
//------------------------------------------------------------------------------------------//
#endif
