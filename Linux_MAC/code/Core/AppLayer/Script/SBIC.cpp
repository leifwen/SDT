/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: SBIC.cpp
 * File ID	:
 * Remark	:
 * Writer   : Leif Wen
 * Date     : 2012.12.15
 * @2013.6.30 :	change to Liunx style
 use int32 to replace int32
 use int32 to replace int32
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "SBIC.h"
#include "Comm_Convert.h"
#include "Device.h"
//------------------------------------------------------------------------------------------//
int32 SBIC_Print::Help(SBICPAR *tBICPAR,int32 blDetail)const{
	PrintB(tBICPAR,".CMD = Print=<Expression> -->Print log.");
	PrintB(tBICPAR,"  Command = <'Print=<Expression>>[//COMMENT]");
	PrintP(tBICPAR,"  Notes:1.Expression Operators is +.");
	PrintP(tBICPAR,"        2.Can use \"\" set string. Use \\\", \\+ to escape \", +.");
	PrintP(tBICPAR,"        3.Support \\0xhh, \\0Xhh, \\a, \\b, \\f, \\n, \\r, \\t, \\v, \\\\, \\', \\\", \\0, \\/, \\*, \\?.");
	PrintP(tBICPAR,"        4.support sub command :'','hex,'ret,'time,'string.");
	PrintP(tBICPAR,"     eg:");
	PrintP(tBICPAR,"       Command = 'Print = Happy + New + Year //Print HappyNewYear.");
	
	const_cast<SBIC_Print *>(this)->HelpLC(tBICPAR,blDetail);	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_Print::Command(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	std::string 	strTempData,retHexCommand,retPrintData;
	
	*ret = "";
	CreateHexCommand(tBICPAR,par,0,0,G_ESCAPE_ON,&retHexCommand,&retPrintData);
	if ((tBICPAR != nullptr) && (tBICPAR->cgDevice != nullptr)
		&& (tBICPAR->cgDevice->cEDevFlag.blEnablePrintSBICinfo != 0)
		&& (tBICPAR->cgDevice->cEDevFlag.blCommandExplain != 0)){
		PrintExecute(tBICPAR,retPrintData);
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_Print::CreateHexCommand(SBICPAR *cSBICPAR,const std::string &inputCommand,
							int32 blHEX,int32 bl0x0D,G_ESCAPE_VAILD blEscape,
							std::string *retHexCommand,std::string *retPrintData)const{
	int32		eRetCode;
	std::string	strHexCommand,strPrintData,strOCommand;
	
	strOCommand = SBIC_Node::DelComment(inputCommand);
	eRetCode = ExecuteLC(cSBICPAR,strOCommand,&strPrintData);
	switch (eRetCode){
		case SBI_RETCODE_HEX:
			strHexCommand = strPrintData;
			if (bl0x0D != 0){
				strHexCommand += "0D";
				strPrintData += "0D";
			}
			break;
		case SBI_RETCODE_SQ:
		case SBI_RETCODE_TIME:
		case SBI_RETCODE_STRING:
		case SBI_RETCODE_BUILD:
			strHexCommand = Str_ASCIIToHEX(strPrintData,blEscape);
			if (bl0x0D != 0){
				strHexCommand += "0D";
				strPrintData += "\\r";
			}
			break;
		default:{
			if (blHEX != 0){
				strHexCommand = Str_HEXToHEXs(strOCommand);
				if (bl0x0D != 0)
					strHexCommand += "0D";
				strPrintData = strHexCommand;
			}
			else{
				strPrintData = strOCommand;
				strHexCommand = Str_ASCIIToHEX(strOCommand,blEscape);
				if (bl0x0D != 0){
					strHexCommand += "0D";
					strPrintData += "\\r";
				}
			}
			break;
		}
	}
	
	*retHexCommand = strHexCommand;
	*retPrintData = strPrintData + "\r\n";
	return(eRetCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
