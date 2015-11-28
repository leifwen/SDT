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
//------------------------------------------------------------------------------------------//
SBIC_Node	gSBIC_subCommandList;
SBIC_SQ		gSBIC_subC_SQ;
SBIC_HEX	gSBIC_subC_HEX;
SBIC_Time	gSBIC_subC_Time;
SBIC_STRING	gSBIC_subC_STRING;
SBIC_Build	gSBIC_subC_Build;
//------------------------------------------------------------------------------------------//
int32 SBIC_CreateHexCommand(
							SBICPAR *cSBICPAR,const std::string &inputCommand,
							int32 blHEX,int32 bl0x0D,G_ESCAPE_VAILD blEscape,
							std::string *retHexCommand,std::string *retPrintData){
	int32		eRetCode;
	std::string	strHexCommand,strPrintData,strOCommand;
	
	gSBIC_subCommandList.AddNode(&gSBIC_subC_SQ);
	gSBIC_subCommandList.AddNode(&gSBIC_subC_Time);
	gSBIC_subCommandList.AddNode(&gSBIC_subC_HEX);
	gSBIC_subCommandList.AddNode(&gSBIC_subC_STRING);
	gSBIC_subCommandList.AddNode(&gSBIC_subC_Build);
	
	strOCommand = SBIC_Node::DelComment(inputCommand);
	eRetCode = gSBIC_subCommandList.ExecuteLC(cSBICPAR,strOCommand,&strPrintData);
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
