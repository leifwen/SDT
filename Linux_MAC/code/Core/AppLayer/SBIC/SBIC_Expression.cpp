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
#include "Comm_Buffer.h"
#include "CommandGList.h"
//------------------------------------------------------------------------------------------//
#ifdef SBIC_ExpressionH
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_Delay::Command(SBIC_ENV *env,const STDSTR &par,void *p)const{
	int32 	timeout;
	STDSTR	strPar1,strPar2;
	
	SplitPar1(strPar1, strPar2, par, " ");
	timeout = GetMS(strPar1);
	
	PrintExecute(env,"Delay",GetMSSTR(timeout));
	SYS_DelayMS(timeout,&env->blExit);
	
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_Synchronous::Command(SBIC_ENV *env,const STDSTR &par,void *p)const{
	double 	T1,T2,synchronousDelay;
	TIME	currentTime,dT;
	int32	hour, min, sec, msec;
	uint64	msecT2,dTime;
	
	STDSTR	strTempData,strTime;
	STDSTR	strPar1,strPar2;
	OUTPUT_NODE	*cOut;
	
	if ((env != nullptr) && (env->cgCommand != nullptr)){
		strPar2 = par;
		strPar1 = Str_LTrim(Str_ReadSubItem(&strPar2,","));
		strPar2 = Str_RTrim(strPar2);
		
		T1 = atof(strPar1.c_str());
		T2 = atof(strPar2.c_str());
		
		currentTime.Now();
		if (env->cgCommand->blFirstSynchronous == 0){
			env->cgCommand->blFirstSynchronous = 1;
			currentTime.DecodeTime(&hour,&min,&sec,&msec);
			SYS_EncodeTimeABS(&dT,0,0,sec,msec);
			env->cgCommand->timeST0 = currentTime - dT;
			msec = (int32)(T1 * 1000) % 1000;
			SYS_EncodeTimeABS(&dT,0,0,(int32)T1,msec);
			env->cgCommand->timeST0 += dT;
			env->cgCommand->timeST0 -= (60 * 60 * 24 * 2);
		}
		dT = currentTime - env->cgCommand->timeST0;
		
		dTime = (uint64)(dT.GetSec() * 1000 * 1000);
		msecT2 = (uint64)(T2 * 1000 * 1000);
		dTime %= msecT2;
		synchronousDelay = (double)(msecT2 - dTime) / 1000;
		
		if (CheckPrintSBICinfo(env) !=0 ){
			DeviceLock(env);
			cOut = GetDevOut(env);
			PrintNL(cOut)
			<< COL_DivLine_Maroon << COL_Time << SYS_MakeTime(currentTime)
			<< COL_MessageNormal(" Synchronous from ")
			<< COL_MessageImportant(env->cgCommand->timeST0.FormatDateTime(DEFAULT_TIMEFORMATE))
			<< COL_MessageNormal(" per ")
			<< COL_clBlue << Str_FloatToString(T2) << "s"
			<< COL_MessageNormal(". \n Next command will be send after ")
			<< COL_clRed << Str_FloatToString(synchronousDelay / 1000) << "s\n"
			<< Endl;
			DeviceUnlock(env);
		}
		SYS_DelayMS(synchronousDelay,&env->blExit);
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_Expression::Command(SBIC_ENV *env,const STDSTR &par,void *retTrue)const{
	STDSTR *ret = static_cast<STDSTR*>(retTrue);
	*ret = 'F';
	if ((env != nullptr) && (Expression(env,par) > 0))
		*ret = 'T';
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
void SBIC_Expression::FormatString(STDSTR *returnStr,const STDSTR &strIn){
	//control char :&& || & | [] !
	//formate to HEX without space
	//escape && || & | [] !
	
	STDSTR				strData,strResult;
	STDSTR::size_type	i,length;
	uint8				charData,charData1;
	
	length = strIn.length();
	i = 0;
	*returnStr = "[,";
	strData = "";
	while(i < length){
		charData = strIn[i];
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
				charData1 = strIn[i];
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
				charData = strIn[i];
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
void SBIC_Expression::GetPoland(STDSTR *polandQueue,const STDSTR &expressions){
	STDSTR		stackT,strExp;
	STDSTR		dataT,dataT2;
	
	*polandQueue = "";
	stackT = "";
	
	FormatString(&strExp,expressions);
	
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
int32 SBIC_Expression::Expression(SBIC_ENV *env,const STDSTR &condition) const{
	STDSTR	dataT,strPDQueue,strRet,fdata1,fdata2,strSub,strSR,caclRet;
	
	if (env != nullptr)
		env->cstdin->Get(&env->cstdin_Str,G_ESCAPE_OFF);
	
	GetPoland(&strPDQueue,condition);
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
			if (cgSubC_Condition.Command(env,dataT,&caclRet) == SBI_RETCODE_NO){
				cgSubC_Receive.Command(env,dataT,&caclRet);
			}
			strRet = caclRet + "," + strRet;
		}
	}
	fdata1 = Str_ReadSubItem(&strRet,",");
	return(fdata1 == "T");
}
//------------------------------------------------------------------------------------------//
int32 SBIC_Expression::CheckResult(SBUFFER *cstdin,const STDSTR &cCheckResult,int32 cTimeout){
	SBIC_ENV	env;

	SetDefAttr(&env);
	env.cfg = 0;
	env.cstdin = cstdin;
	SYS_Delay_SetTS(&env.cgTimeS,cTimeout);
	
	do{
		if (Expression(&env,cCheckResult) != 0)
			return 1;
	}while(SYS_Delay_CheckTS(&env.cgTimeS) == 0);
	return 0;
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_Wait::Command(SBIC_ENV *env,const STDSTR &par,void *p)const{
	int32 		timeout;
	STDSTR		strPar2,strRet;
	SYS_TIME_S	timeS;

	if (env != nullptr){
		strPar2 = par;
		timeout = GetMS(Str_Trim(Str_ReadSubItem(&strPar2,",")));
		Str_TrimSelf(strPar2);
		
		SYS_Delay_SetTS(&timeS,timeout);
		
		PrintExecute(env,"Wait, until {", strPar2, "} is true.\n Timeout is", GetMSSTR(timeout));
		while(!(env->blExit != 0 || SYS_Delay_CheckTS(&timeS) != 0 || cgSubC_Expression.Expression(env,strPar2) != 0));
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_Lable::Command(SBIC_ENV *env,const STDSTR &par,void *retStr)const{
	STDSTR *ret = static_cast<STDSTR*>(retStr);
	if (retStr != nullptr)
		*ret = (Str_Trim(par));
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_Define::Command(SBIC_ENV *env,const STDSTR &par,void *p)const{
	if (env != nullptr){
		env->cgDefine += par;
		env->cgDefine += ";";
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_GOTO::Command(SBIC_ENV *env,const STDSTR &par,void *p)const{
	STDSTR			strPar1,strPar2,retCMDLable;
	COMMAND_NODE	*cNode;
	int32			blJump;
	OUTPUT_NODE		*cOut;
	
	if (env != nullptr){
		DeviceLock(env);
		cOut = GetDevOut(env);
		
		strPar2 = par;
		strPar1 = Str_Trim(Str_ReadSubItem(&strPar2,","));
		Str_TrimSelf(strPar2);
		if (strPar2.length() == 0 || cgSubC_Expression.Expression(env,strPar2) != 0){
			if ((CheckPrintSBICinfo(env) != 0) && (CheckCommandExplain(env) != 0))
				PrintWithDividingLine(cOut,"Try to execute:",DelComment(env->cgCommand->StrCommand));
			
			blJump = 0;
			cNode = (COMMAND_NODE*)(GetcgDown(GetcgUp(env->cgCommand)));
			while(cNode != nullptr){
				if (cNode->blEnableSend != 0){
					if ((SBI_RETCODE_LABLE == cgSubC_Lable.Execute(env,DelComment(cNode->StrCommand),&retCMDLable))
						&& (retCMDLable == strPar1)){
						if ((CheckPrintSBICinfo(env) != 0) && (CheckCommandExplain(env) != 0))
							PrintStr(cOut,"\n Goto Lable:", retCMDLable);
						env->cgCommand = cNode;
						blJump = 1;
						break;
					}
				}
				cNode = (COMMAND_NODE*)(GetcgNext(cNode));
			}
			
			if ((blJump == 0) && ((CheckPrintSBICinfo(env) != 0) && (CheckCommandExplain(env) != 0)))
				PrintStr(cOut,"\n No find suitable label, script will continue, no jump");
		}
		DeviceUnlock(env);
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_STOP::Command(SBIC_ENV *env,const STDSTR &par,void *p)const{
	if (env != nullptr){
		if (par.length() == 0 || cgSubC_Expression.Expression(env,par) != 0){
			PrintExecute(env,"Expression matched, stop");
			env->blCMDRet = 6;
		}
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_BREAK::Command(SBIC_ENV *env,const STDSTR &par,void *p)const{
	if (env != nullptr){
		if (par.length() == 0 || cgSubC_Expression.Expression(env,par) != 0){
			PrintExecute(env,"Expression matched, break");
			env->blCMDRet = 3;
		}
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
#endif
