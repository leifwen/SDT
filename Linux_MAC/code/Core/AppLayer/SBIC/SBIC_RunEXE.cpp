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
int32 SBIC_RunEXE::Command(SBIC_ENV *env,const STDSTR &par,void *p)const{
	STDSTR		strPar1,strPar2;
	AEXE		*aexe;
	
	env->cgFunRet = 'F';
	if (env != nullptr){
		SplitPar1(strPar1, strPar2, par, ",");
		
		PrintExecute(env,"Exexture external command:",strPar1,"::",strPar2);
		
		if (strPar2.length() > 0){
			aexe = env->cgAExePool->Execute(strPar1, strPar2);
			if (aexe != nullptr)
				env->cgFunRet = 'T';
		}
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_StopEXE::Command(SBIC_ENV *env,const STDSTR &par,void *p)const{
	STDSTR		strPar1,strPar2;
	int32		timeout;
	AEXE		*aexe;
	
	env->cgFunRet = 'F';
	if (env != nullptr){
		SplitPar1(strPar1, strPar2, par, ",");
		timeout = GetMS(strPar2);
		aexe = env->cgAExePool->Find(strPar1);
		if (aexe != nullptr){
			PrintExecute(env,"Stop external command:",strPar1,"::",aexe->GetCommand());
			if (env->cgAExePool->ChildClose(strPar1,timeout) != 0)
				env->cgFunRet = 'T';
		}
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
void SBIC_WaitEXE::PrintLineWithTime(SBIC_ENV *env,STDSTR *strIn)const{
	STDSTR	strT;
	
	do{
		strT = Str_ReadSubItem(strIn,"\n",1);
		if (strT.length() > 0)
			PrintWithTime(env->cstdout,Data(COL_DB_RxText,&strT));
	}while(strIn->length() > 0);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_WaitEXE::Command(SBIC_ENV *env,const STDSTR &par,void *p)const{
	STDSTR		strPar1,strPar3,strPrint;
	int32		timeout;
	SYS_TIME_S	timeS;
	AEXE		*aexe;
	
	env->cgFunRet = 'F';
	if (env != nullptr){
		strPar3 = Str_Trim(par);
		strPar1 = Str_RTrim(Str_ReadSubItem(&strPar3,","));
		timeout = GetMS(Str_Trim(Str_ReadSubItem(&strPar3,",")));
		Str_TrimSelf(strPar3);
		
		SYS_Delay_SetTS(&timeS,timeout);
		
		aexe = env->cgAExePool->Find(strPar1);
		if (aexe != nullptr){
			env->cstdin_Str = "";
			do{
				timeout = SYS_Delay_CheckTS(&timeS);
				strPrint = "";
				aexe->Read(&strPrint, G_ESCAPE_OFF);
				env->cstdin_Str += strPrint;
				PrintLineWithTime(env,&strPrint);
				if (cgSubC_Expression.Expression(env,strPar3) != 0)
					env->cgFunRet = 'T';
				if (aexe->IsConnected() == 0){
					if (strPar3.length() == 0)
						env->cgFunRet = 'T';
					break;
				}
			}while(env->blExit == 0 && timeout == 0 && (env->cgFunRet == "F"));
		}
		if ((env->cgCommand != nullptr) && (env->cgFunRet == "F")){
			env->cgCommand->StrCatch = "Fail";
			++ env->cgCommand->catchTimes;
		}
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_GetEXE::Command(SBIC_ENV *env,const STDSTR &par,void *p)const{
	env->cgFunRet = 'F';
	if (env != nullptr){
		if (env->cgAExePool->Find(par) != nullptr)
			env->cgFunRet = 'T';
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
#endif

