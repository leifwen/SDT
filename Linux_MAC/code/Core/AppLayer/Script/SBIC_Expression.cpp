/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: SBIC_Expression.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.8.15
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "SBIC_Expression.h"
#include "SYS_Time.h"
#include "Comm_Convert.h"
#include "Device.h"
#include "CommandGList.h"
//------------------------------------------------------------------------------------------//
int32 SBIC_Expression::Help(SBICPAR *tBICPAR,int32 blDetail)const{
	
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_Expression::Command(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	
	*ret = 'F';
	if ((tBICPAR != nullptr) && (Expression(tBICPAR,tBICPAR->cgRecvbuf,par) > 0))
		*ret = 'T';
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
void SBIC_Expression::FormatString(const std::string &strInput,std::string *returnStr) const{
	//control char :&& || & | [] !
	//formate to HEX without space
	//escape && || & | [] !
	
	std::string				strData,strResult;
	std::string::size_type	i,length;
	uint8					charData,charData1;
	
	length = strInput.length();
	i = 0;
	*returnStr = "[,";
	strData = "";
	while(i < length){
		charData = strInput[i];
		++ i;
		if ((charData == '[') || (charData == ']')){
			Str_TrimSelf(strData);
			if (strData.length() > 0){
				*returnStr += Str_ASCIIToHEX(strData,G_ESCAPE_OFF);
				*returnStr += ',';
			}
			*returnStr += charData;
			*returnStr += ',';
			strData = "";
		}
		else if ((charData == '&') || (charData == '|') || (charData == '!')){
			if (i < length){
				charData1 = strInput[i];
				if (charData1 != charData){
					Str_TrimSelf(strData);
					if (strData.length() > 0){
						*returnStr += Str_ASCIIToHEX(strData,G_ESCAPE_OFF);
						*returnStr += ',';
					}
					*returnStr += charData;
					*returnStr += ',';
					strData = "";
				}
			}
		}
		else{
			if ((charData == '\\') && (i < length)){
				charData = strInput[i];
				++ i;
				if ((charData != '&') && (charData != '|') && (charData != '[') && (charData != ']') && (charData != '!'))
					strData += '\\';
			}
			strData += charData;
		}
	}
	Str_TrimSelf(strData);
	if (strData.length() > 0){
		*returnStr += Str_ASCIIToHEX(strData,G_ESCAPE_OFF);
		*returnStr += ',';
	}
	*returnStr += ']';
}
//------------------------------------------------------------------------------------------//
void SBIC_Expression::GetPoland(const std::string &expressions,std::string *polandQueue) const{
	std::string		stackT,strExp;
	std::string		dataT,dataT2;
	
	*polandQueue = "";
	stackT = "";
	
	FormatString(expressions,&strExp);
	
	while(strExp.length() > 0){
		dataT = Str_ReadSubItem(&strExp,",");
		if (dataT == "!"){
			while(stackT.length() > 0){
				dataT2 = Str_ReadSubItem(&stackT, ",");
				if ((dataT2 == "[") || (dataT2 == "&") || (dataT2 == "|")){
					stackT = dataT + "," + dataT2 + "," + stackT;
					break;
				}
				*polandQueue += dataT2;
				*polandQueue += ',';
			}
		}
		else if ((dataT == "&") || (dataT == "|")){
			while(stackT.length() > 0){
				dataT2 = Str_ReadSubItem(&stackT, ",");
				if (dataT2 == "["){
					stackT = dataT + "," + dataT2 + "," + stackT;
					break;
				}
				*polandQueue += dataT2;
				*polandQueue += ',';
			}
		}
		else if (dataT == "]"){
			while(stackT.length() > 0){
				dataT2 = Str_ReadSubItem(&stackT, ",");
				if (dataT2 == "[")
					break;
				*polandQueue += dataT2;
				*polandQueue += ',';
			}
		}
		else {
			stackT = dataT + "," + stackT;
		}
	}
}
//------------------------------------------------------------------------------------------//
int32 SBIC_Expression::Expression(SBICPAR *tBICPAR,const std::string &strSource,const std::string &condition) const{
	std::string		dataT,strPDQueue,strRet,fdata1,fdata2,strSub,strSR,caclRet;
	
	GetPoland(condition,&strPDQueue);
	strRet = "";
	while(strPDQueue.length() > 0){
		dataT = Str_ReadSubItem(&strPDQueue, ",");
		if (dataT == "&"){
			fdata1 = Str_ReadSubItem(&strRet,",");
			fdata2 = Str_ReadSubItem(&strRet,",");
			if ((fdata1 == "T") && (fdata2 =="T")){
				strRet = "T," + strRet;
			}
			else{
				strRet = "F," + strRet;
			}
		}
		else if (dataT == "|"){
			fdata1 = Str_ReadSubItem(&strRet,",");
			fdata2 = Str_ReadSubItem(&strRet,",");
			if ((fdata1 == "T") || (fdata2 =="T")){
				strRet = "T," + strRet;
			}
			else{
				strRet = "F," + strRet;
			}
		}
		else if (dataT == "!"){
			fdata1 = Str_ReadSubItem(&strRet,",");
			if (fdata1 == "T"){
				strRet = "F," + strRet;
			}
			else{
				strRet = "T," + strRet;
			}
		}
		else{
			caclRet = 'F';
			dataT = Str_HEXToASCII(dataT);
			if (cgSubC_Condition.Command(tBICPAR,dataT,&caclRet) == SBI_RETCODE_NO){
				cgSubC_Receive.Command(tBICPAR,dataT,&caclRet);
			}
			strRet = caclRet + "," + strRet;
		}
	}
	fdata1 = Str_ReadSubItem(&strRet,",");
	return(fdata1 == "T");
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_Wait::Help(SBICPAR *tBICPAR,int32 blDetail)const{
	PrintB(tBICPAR,".CMD = Wait=<timeout>,<Expression> -->Wait until receive data contains expression,.");
	PrintB(tBICPAR,"  Command = <'Wait=<timeout>,<Expression>>[//COMMENT]");
	PrintP(tBICPAR,"  Notes:1.Expression is the same as Condition Expression.");
	PrintP(tBICPAR,"   eg:");
	PrintP(tBICPAR,"     Command = 'Wait=10.5s,'cts==H   //if CTS is high then goto send next \"Command\". Waiting timeout is 10.5s.");
	PrintP(tBICPAR,"     Command = 'Wait=500ms,'cts==L   //if CTS is low then goto send next \"Command\". Waiting timeout is 500ms.");
	PrintP(tBICPAR,"     Command = 'Wait=10.5s,'cts==H && 'dsr==H   //if CTS is high and DSR is high, then goto send next \"Command\".");
	PrintP(tBICPAR,"     Command = 'Wait=10.5s,'cts==H && OK        //if CTS is high and receive OK, then goto send next \"Command\".");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_Wait::Command(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	int32 		timeout;
	std::string	strPar2,strRet;
	SYS_TIME_S	timeS;
	
	*ret = "";
	if (tBICPAR != nullptr){
		strPar2 = par;
		timeout = GetMS(Str_Trim(Str_ReadSubItem(&strPar2,",")));
		Str_TrimSelf(strPar2);
		
		SYS_Delay_SetTS(&timeS,timeout);
		
		PrintExecute(tBICPAR,"Wait, until {" + strPar2 + "} is true.\r\n Timeout is " + GetMSSTR(timeout));
		do{
			if (tBICPAR->cgBuffer != nullptr)
				tBICPAR->cgBuffer->GetInHEX(&tBICPAR->cgRecvbuf,G_SPACE_OFF);
		}while(!(tBICPAR->blExit != 0 || SYS_Delay_CheckTS(&timeS) != 0 || cgSubC_Expression.Expression(tBICPAR,tBICPAR->cgRecvbuf,strPar2) != 0));
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_Lable::Help(SBICPAR *tBICPAR,int32 blDetail)const{
	PrintB(tBICPAR,".CMD = Lable:[PAR] -->Set a label. No execution command, used with CMD Goto.");
	PrintB(tBICPAR,"  Command = <'Lable:[PAR]>[//COMMENT]");
	PrintP(tBICPAR,"   eg:");
	PrintP(tBICPAR,"     Command = 'Lable:001      //used with CMD Goto, scrpit will jump to here");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_Lable::Command(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	*ret = (Str_Trim(par));
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_GOTO::Help(SBICPAR *tBICPAR,int32 blDetail)const{
	PrintB(tBICPAR,".CMD = Goto=<Lable>[,<Expression>] -->if receive data contains expression, then jump to Lable.");
	PrintB(tBICPAR,"  Command = <'Goto(<Lable>,<Expression>)>[//COMMENT]");
	PrintP(tBICPAR,"  Notes:Expression is the same as Condition Expression.");
	PrintP(tBICPAR,"   eg:");
	PrintP(tBICPAR,"     Command = 'Goto=001,Leif Wen  //if receive data contains Leif Wen, then goto 001");
	PrintP(tBICPAR,"     Command = 'Goto=001,'cts==h   //if CTS is high, then goto 001");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_GOTO::Command(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	std::string		strPar1,strPar2,strTempData,retCMDLable;
	COMMAND_NODE	*cNode;
	int32			blJump;
	
	if ((tBICPAR != nullptr) && (tBICPAR->cgDevice != nullptr) &&(tBICPAR->cgCommand != nullptr)){
		strPar2 = par;
		strPar1 = Str_Trim(Str_ReadSubItem(&strPar2,","));
		Str_TrimSelf(strPar2);
		if (strPar2.length() == 0 || cgSubC_Expression.Expression(tBICPAR,tBICPAR->cgRecvbuf,strPar2) != 0){
			if ((tBICPAR->cgDevice->cEDevFlag.blEnablePrintSBICinfo != 0) && (tBICPAR->cgDevice->cEDevFlag.blCommandExplain != 0)){
				strTempData = SYS_MakeTimeNow();
				strTempData += " Try to execute: ";
				strTempData += DelComment(tBICPAR->cgCommand->StrCommand);
				strTempData += "\r\n";
				if (tBICPAR->cgDevice->cgODevList.cgOutput != nullptr){
					tBICPAR->cgDevice->cgODevList.cgOutput->Spin_InUse_set();
					tBICPAR->cgDevice->cgODevList.cgOutput->WriteDividingLine(RICH_CF_clMaroon);
					tBICPAR->cgDevice->cgODevList.cgOutput->WriteStrN(strTempData,RICH_CF_clMaroon);
					tBICPAR->cgDevice->cgODevList.cgOutput->Spin_InUse_clr();
				}
			}
			
			blJump = 0;
			cNode = (COMMAND_NODE*)(GetFirstBrother_nolock(tBICPAR->cgCommand));
			while(cNode != nullptr){
				if (cNode->blEnableSend != 0){
					if (SBI_RETCODE_LABLE == cgSubC_Lable.Execute(tBICPAR,DelComment(cNode->StrCommand),&retCMDLable)
						&& (retCMDLable == strPar1)){
						if ((tBICPAR->cgDevice->cEDevFlag.blEnablePrintSBICinfo != 0) && (tBICPAR->cgDevice->cEDevFlag.blCommandExplain != 0)){
							strTempData = "Goto Lable:" + retCMDLable + "\r\n";
							if (tBICPAR->cgDevice->cgODevList.cgOutput != nullptr)
								tBICPAR->cgDevice->cgODevList.cgOutput->WriteStrN(strTempData, RICH_CF_clPurple);
						}
						tBICPAR->cgCommand = cNode;
						blJump = 1;
						break;
					}
				}
				cNode = (COMMAND_NODE*)(GetcgRChild(cNode));
			}
			
			if ((blJump == 0) && (tBICPAR->cgDevice->cEDevFlag.blEnablePrintSBICinfo != 0) && (tBICPAR->cgDevice->cEDevFlag.blCommandExplain != 0))
				if (tBICPAR->cgDevice->cgODevList.cgOutput != nullptr)
					tBICPAR->cgDevice->cgODevList.cgOutput->WriteStrN("No find suitable label, script will continue, no jump\r\n",RICH_CF_clPurple);
		}
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//



//------------------------------------------------------------------------------------------//
