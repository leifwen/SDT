/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: SBIC_RunExternal.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.8.15
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "SBIC_RunEXE.h"
#include "SYS_Time.h"
#include "Comm_Convert.h"
#include "Device.h"
#include "CommandGList.h"
//------------------------------------------------------------------------------------------//
#ifdef SBIC_RunEXEH
//------------------------------------------------------------------------------------------//
int32 SBIC_RunEXE::Command(SBIC_ENV *env,const STDSTR &par,void *retTrue)const{
	STDSTR		strPar1,strPar3,strContinue;
	AEXE		*aexe;
	int32		timeout;
	SYS_TIME_S	timeS;
	
	env->cgFunRet = 'F';
	if (env != nullptr){
		strPar3 = Str_Trim(par);
		strPar1 = Str_RTrim(Str_ReadSubItem(&strPar3,","));
		timeout = GetMS(Str_Trim(Str_ReadSubItem(&strPar3,",")));
		Str_TrimSelf(strPar3);
		strContinue = "";
		if (env->cgCommand != nullptr)
			strContinue = env->cgCommand->StrContinue;
		
		PrintExecute(env,"Exexture external command:\n"
					 ," Command is " + strPar1 + ":" + strPar3 + "\n"
					 ,"Timeout is",GetMSSTR(timeout)
					 ,(strContinue.length() == 0)?"":"True condition is"
					 ,(strContinue.length() == 0)?"":strContinue);
		
		SYS_Delay_SetTS(&timeS,timeout);
		
		if (strPar3.length() > 0){
			aexe = env->cgAExePool->Execute(strPar1, strPar3);
			if (aexe != nullptr){
				env->cstdin_Str = "";
				do{
					timeout = SYS_Delay_CheckTS(&timeS);
					aexe->Read(&env->cstdin_Str, G_ESCAPE_OFF);
					if (cgSubC_Expression.Expression(env,strContinue) != 0)
						env->cgFunRet = 'T';
					if (aexe->IsConnected() == 0){
						if (strContinue.length() == 0)
							env->cgFunRet = 'T';
						break;
					}
				}while(env->blExit == 0 && timeout == 0 && (env->cgFunRet == "F"));
			}
		}
		if ((env->cgCommand != nullptr) && (env->cgFunRet == "F")){
			env->cgCommand->StrCatch = "Fail";
			++ env->cgCommand->catchTimes;
		}
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_StopEXE::Command(SBIC_ENV *env,const STDSTR &par,void *retTrue)const{
	STDSTR		strPar1,strPar3;
	int32		timeout;
	env->cgFunRet = 'F';
	if (env != nullptr){
		strPar3 = Str_Trim(par);
		strPar1 = Str_RTrim(Str_ReadSubItem(&strPar3,","));
		timeout = GetMS(Str_Trim(Str_ReadSubItem(&strPar3,",")));
		Str_TrimSelf(strPar3);
		
		PrintExecute(env,"Stop external command:",strPar1,":",strPar3);
		if (env->cgAExePool->ChildClose(strPar1,strPar3,timeout) != 0)
			env->cgFunRet = 'T';
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
#endif

