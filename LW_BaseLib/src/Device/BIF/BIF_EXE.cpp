//
//  BIF_EXE.cpp
//  SDT
//
//  Created by Leif Wen on 18/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "BIF_EXE.h"
#ifdef BIF_EXE_h
#include "BIF_Expression.h"
#include "Commu_AEXE.h"
//------------------------------------------------------------------------------------------//
CMDID BIF_RUNEXE::Help(CMD_ENV* env,uint32 flag)const{
	PrintB(env,".CMD = RunEXE=<name>,<command> -->Execute external command,only use in Linux.");
	PrintB(env,"  Command = <'RunEXE=<name>,<command>>[//COMMENT]");
	PrintP(env,"  Notes:1.Can use 'lcs check the execution result.");
	PrintP(env,"        2.The name is used for distinguishing the same command.");
	PrintP(env,"     eg:");
	PrintP(env,"     Command = 'RunEXE=CMD1,pppd file ./script    //define EC name is CMD1,and EC is \"pppd file ./script\"");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIF_RUNEXE::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	STDSTR		strPar1,strPar2;
	
	if (env != nullptr){
		BIF_ENV::RetFun(env) = 'F';
#ifdef Commu_AEXE_h
		SplitPar1(strPar1, strPar2, msg, ",");
		
		PrintExecute(env,"Exexture external command:",strPar1,"::",strPar2);
		
		if (strPar2.length() > 0){
			if (BIF_ENV_DEV::GetAExePool(env)->Execute(strPar1, strPar2) != nullptr)
				BIF_ENV::RetFun(env) = 'T';
		}
#endif
	}
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIF_STOPEXE::Help(CMD_ENV* env,uint32 flag)const{
	PrintB(env,".CMD = StopEXE=<name>,<timeout> -->Stop external command,only use in Linux.");
	PrintB(env,"  Command = <'StopEXE=<name>,<timeout>>[//COMMENT]");
	PrintP(env,"  Notes:1.Can use 'lcs check the execution result.");
	PrintP(env,"        2.The name is defined in RunEXE.");
	PrintP(env,"     eg:");
	PrintP(env,"     Command = 'StopEXE=CMD1,5s  // stop EC which name is CMD1");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIF_STOPEXE::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	if (env != nullptr){
		BIF_ENV::RetFun(env) = 'F';
#ifdef Commu_AEXE_h
		STDSTR		strPar1, strPar2;
		int32		timeout;
		AEXE	*aexe;
		SplitPar1(strPar1, strPar2, msg, ",");
		timeout = GetMS(strPar2);
		aexe = BIF_ENV_DEV::GetAExePool(env)->Find(strPar1);
		if (aexe != nullptr){
			PrintExecute(env,"Stop external command:",strPar1,"::",static_cast<CORE_AEXE*>(aexe->Core())->GetCommand());
			if (BIF_ENV_DEV::GetAExePool(env)->CloseChild(strPar1,timeout))
				BIF_ENV::RetFun(env) = 'T';
		}
#endif
	}
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
namespace BIF {
	static void PrintLineWithTime(CMD_ENV* env,STDSTR* strIn);
};
//------------------------------------------------------------------------------------------//
CMDID BIF_WAITEXE::Help(CMD_ENV* env,uint32 flag)const{
	PrintB(env,".CMD = WaitEXE=<name>,<timeout>,<condition> -->Wait external command executing,");
	PrintB(env,"                                               only use in Linux.");
	PrintB(env,"  Command = <'WaitEXE=<name>,<timeout>,<condition>>[//COMMENT]");
	PrintP(env,"  Notes:1.Can use 'lcs check the execution result.");
	PrintP(env,"        2.The name is used for distinguishing the same command.");
	PrintP(env,"     eg:");
	PrintP(env,"     Command = 'WaitEXE=CMD1,5s,local  IP address");
	PrintP(env,"               // WaitEXE EC executing which name is CMD1,and check whether contain \"local  IP address\"");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIF_WAITEXE::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	if (env != nullptr){
		BIF_ENV::RetFun(env) = 'F';
#ifdef Commu_AEXE_h
		STDSTR		strPar1, strPar3, strPrint;
		int32		timeout;
		SYS_TIME_S	timeS;
		AEXE		*aexe;
		
		strPar3 = Str_Trim(msg);
		strPar1 = Str_RTrim(Str_ReadSubItem(&strPar3,","));
		timeout = GetMS(Str_Trim(Str_ReadSubItem(&strPar3,",")));
		Str_TrimSelf(strPar3);
		
		SYS_Delay_SetTS(&timeS,timeout);
		
		aexe = BIF_ENV_DEV::GetAExePool(env)->Find(strPar1);
		if (aexe != nullptr){
			BIF_ENV::STDIN(env) = "";
			do{
				SYS_Delay_IsTimeout(&timeS);
				aexe->Read(_EMPTY(&strPrint), -1);
				BIF_ENV::STDIN(env) += strPrint;
				BIF::PrintLineWithTime(env,&strPrint);
				if (BIF_Expression(env,strPar3) != G_FALSE)
					BIF_ENV::RetFun(env) = 'T';
				if (aexe->IsOpened() == G_FALSE){
					if (strPar3.length() == 0)
						BIF_ENV::RetFun(env) = 'T';
					break;
				}
			}while(!(IsExit(env) || SYS_Delay_IsTimeout(&timeS) || (BIF_ENV::RetFun(env) == "T")));
		}
#ifdef GList_h
		if ((BIF_ENV::GetCommandNode(env) != nullptr) && (BIF_ENV::RetFun(env) == "F")){
			BIF_ENV::GetCommandNode(env)->StrCatch = "Fail";
			++ BIF_ENV::GetCommandNode(env)->catchTimes;
		}
#endif
		BIF_ENV::RetCMD(env) = 2;
#endif
	}
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
static void BIF::PrintLineWithTime(CMD_ENV* env,STDSTR* strIn){
	STDSTR	strT;
	
	do{
		strT = Str_ReadSubItem(strIn,"\n",G_TRUE);
		if (strT.length() > 0)
			CMD_BASE::PrintWithTime(env,COL_DB_RxText,&strT);
	}while(strIn->length() > 0);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIF_GETEXE::Help(CMD_ENV* env,uint32 flag)const{
	PrintB(env,".CMD = GetEXE=<name> -->Get external command status,only use in Linux.");
	PrintB(env,"  Command = <'GetEXE=<name>,<timeout>,<condition>>[//COMMENT]");
	PrintP(env,"  Notes:1.Can use 'lcs check the execution result.");
	PrintP(env,"        2.The name is used for distinguishing the same command.");
	PrintP(env,"        3.Set result to true if EC is in running.");
	PrintP(env,"     eg:");
	PrintP(env,"     Command = 'GetEXE=CMD1  // Get EC status which name is CMD1");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIF_GETEXE::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	if (env != nullptr){
		BIF_ENV::RetFun(env) = 'F';
#ifdef Commu_AEXE_h
		if (BIF_ENV_DEV::GetAExePool(env)->Find(msg) != nullptr)
			BIF_ENV::RetFun(env) = 'T';
#endif
	}
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif /* BIF_EXE_h */
