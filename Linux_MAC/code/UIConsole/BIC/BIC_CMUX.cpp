/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: BIC_CMUX.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.01.25
 
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "BIC_CMUX.h"
#include "Comm_Convert.h"
#include "AppLayer.h"
//------------------------------------------------------------------------------------------//
#ifdef BIC_CMUXH
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_CMUX_ON::Command(BIC_ENV *env, const STDSTR &par,void *p)const{
	COMMAND_GROUP	*group;
	uint32			gDID;
	
	if (env->eda->IsComOpened() == 0){
		PrintFail(env,"no COM device");
		return(cgReturnCode);
	}
	
	env->cstdout->PrintEnable();
	if (par.length() > 0){
		gDID = (uint32)strtoll(par.c_str(),nullptr,10);

		group = (COMMAND_GROUP*)FindInLChildRChainByDRNodeID(&env->cSDTApp->m_GCList, gDID);
		if (group != nullptr){
			group->autoRunTimes = 1;
			if (env->cSDTApp->m_Script.Execute(&env->cSDTApp->m_Device,group) == 0){
				PrintFail(env,"script is running");
				return(cgReturnCode);
			}
		}
		else{
			PrintFail(env);
			return(cgReturnCode);
		}
		
		if (InPressKeyMode(env) > 0){
			PrintFail(env,"Pressed ESC key, stopped CMUX");
			return(cgReturnCode);
		}
		env->cSDTApp->m_CMUXDriver.Open(0,0);
	}
	else{
		env->cSDTApp->m_CMUXDriver.Open(1,0);
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_CMUX_OFF::Command(BIC_ENV *env, const STDSTR &par,void *p)const{
	env->cstdout->PrintEnable();
	env->cSDTApp->m_CMUXDriver.CloseD();
	SYS_SleepMS(50);
	PrintSuccess(env,"CMUX stopped");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_CMUX_CLD::Command(BIC_ENV *env, const STDSTR &par,void *p)const{
	if (env->cSDTApp->m_CMUXDriver.IsConnected() == 0){
		PrintFail(env,"CMUX no start");
		return(cgReturnCode);
	}
	env->cstdout->PrintEnable();
	env->cSDTApp->m_CMUXDriver.SendCLD();
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_CMUX_PSC::Command(BIC_ENV *env, const STDSTR &par,void *p)const{
	int32	num;
	if (env->cSDTApp->m_CMUXDriver.IsConnected() == 0){
		PrintFail(env,"CMUX no start");
		return(cgReturnCode);
	}
	env->cstdout->PrintEnable();
	if (par.length() == 0){
		env->cSDTApp->m_CMUXDriver.SendStdPSC(0xff);
	}
	else{
		num = atoi(par.c_str());
		env->cSDTApp->m_CMUXDriver.SendStdPSC(num);
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_CMUX_FC::Command(BIC_ENV *env, const STDSTR &par,void *p)const{
	if (env->cSDTApp->m_CMUXDriver.IsConnected() == 0){
		PrintFail(env,"CMUX no start");
		return(cgReturnCode);
	}
	env->cstdout->PrintEnable();
	if (par.length() > 0){
		if (par == "on"){
			env->cSDTApp->m_CMUXDriver.SendFCC(1);
			return(cgReturnCode);
		}
		else if (par == "off"){
			env->cSDTApp->m_CMUXDriver.SendFCC(0);
			return(cgReturnCode);
		}
	}
	PrintFail(env);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_CMUX_DTR::Command(BIC_ENV *env, const STDSTR &par,void *p)const{
	STDSTR		strPar1,strPar2,strPar3;
	SYS_TIME_S	timeS;
	int32		bltime;
	int32		num;
	CMUXCOM		*tCMUXCOM;

	if (env->cSDTApp->m_CMUXDriver.IsConnected() == 0){
		PrintFail(env,"CMUX no start");
		return(cgReturnCode);
	}
	
	SplitPar1(strPar1,strPar3,par);
	
	num = atoi(strPar1.c_str());
	if ((num == 0) || (num > CMUX_MAX_DLCI_INDEX)){
		PrintFail(env);
		return(cgReturnCode);
	}
	tCMUXCOM = env->cSDTApp->m_CMUXDriver.GetVCOM(num);
	if (tCMUXCOM == nullptr){
		PrintFail(env);
		return(cgReturnCode);
	}

	SplitPar1(&strPar1, &strPar3);
	while(strPar1.length() > 0){
		bltime = 0;
		SplitPar1(&strPar2,&strPar3);
		if ((strPar2.length() > 0) && (strPar2 != "-H") && (strPar2 != "-L")){
			SYS_Delay_SetTS(&timeS,atof(strPar2.c_str()) * 1000);
			bltime = 1;
		}
		if (strPar1 == "-H"){
			env->cSDTApp->m_CMUXDriver.SendMSC(num,1,tCMUXCOM->Check_RTS());
		}
		else if (strPar1 == "-L"){
			env->cSDTApp->m_CMUXDriver.SendMSC(num,0,tCMUXCOM->Check_RTS());
		}
		else{
			break;
		}
		if (bltime > 0){
			if (PressAnyKey(env,env->eda,&timeS) == 27){
				PrintSuccess(env,"Pressed ESC key, stopped execute command");
				return(cgReturnCode);
			}
		}
		else{
			strPar1 = strPar2;
			continue;
		}
		SplitPar1(&strPar1,&strPar3);
	};
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_CMUX_RTS::Command(BIC_ENV *env, const STDSTR &par,void *p)const{
	STDSTR		strPar1,strPar2,strPar3;
	SYS_TIME_S	timeS;
	int32		bltime;
	int32		num;
	CMUXCOM		*tCMUXCOM;
	
	if (env->cSDTApp->m_CMUXDriver.IsConnected() == 0){
		PrintFail(env,"CMUX no start");
		return(cgReturnCode);
	}
	
	SplitPar1(strPar1,strPar3,par);
	
	num = atoi(strPar1.c_str());
	if ((num == 0) || (num > CMUX_MAX_DLCI_INDEX)){
		PrintFail(env);
		return(cgReturnCode);
	}
	tCMUXCOM = env->cSDTApp->m_CMUXDriver.GetVCOM(num);
	if (tCMUXCOM == nullptr){
		PrintFail(env);
		return(cgReturnCode);
	}
	
	SplitPar1(&strPar1, &strPar3);
	while(strPar1.length() > 0){
		bltime = 0;
		SplitPar1(&strPar2,&strPar3);
		if ((strPar2.length() > 0) && (strPar2 != "-H") && (strPar2 != "-L")){
			SYS_Delay_SetTS(&timeS,atof(strPar2.c_str()) * 1000);
			bltime = 1;
		}
		if (strPar1 == "-H"){
			env->cSDTApp->m_CMUXDriver.SendMSC(num,tCMUXCOM->Check_DTR(),1);
		}
		else if (strPar1 == "-L"){
			env->cSDTApp->m_CMUXDriver.SendMSC(num,tCMUXCOM->Check_DTR(),0);
		}
		else{
			break;
		}
		if (bltime > 0){
			if (PressAnyKey(env,env->eda,&timeS) == 27){
				PrintSuccess(env,"Pressed ESC key, stopped execute command");
				return(cgReturnCode);
			}
		}
		else{
			strPar1 = strPar2;
			continue;
		}
		SplitPar1(&strPar1,&strPar3);
	};
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_DLCI::Command(BIC_ENV *env, const STDSTR &par,void *p)const{
	int32		blcheck,num;
	STDSTR		strPar1,strPar2,strPrintData;
	CMUXCOM		*tCMUXCOM;

	if (env->cSDTApp->m_CMUXDriver.IsConnected() == 0){
		PrintFail(env,"CMUX no start");
		return(cgReturnCode);
	}
	
	if (par.length() == 0){
		num = 0;
		PrintStrNL(env,Data(COL_clDefault,DEV_LINE_START));
		while(num++ < CMUX_MAX_DLCI_INDEX){
			tCMUXCOM = env->cSDTApp->m_CMUXDriver.GetVCOM(num);
			if (tCMUXCOM == nullptr)
				continue;
			PrintStrNL(env,Data(COL_clCyan,&tCMUXCOM->DlciStatus(&strPrintData)));
		}
		PrintStrNL(env,Data(COL_clDefault,DEV_LINE_START));
		return(cgReturnCode);
	}
	
	SplitPar1(strPar1,strPar2,par);
	
	num = atoi(strPar1.c_str());
	if ((num == 0) || (num > CMUX_MAX_DLCI_INDEX)){
		PrintFail(env);
		return(cgReturnCode);
	}
	tCMUXCOM = env->cSDTApp->m_CMUXDriver.GetVCOM(num);
	if (tCMUXCOM == nullptr){
		PrintFail(env);
		return(cgReturnCode);
	}
	SplitPar1(&strPar1,&strPar2);
	blcheck = 0;
	while(strPar1.length() > 0){
		if (strPar1 == "-H"){
			tCMUXCOM->Update_HEX(1);
			tCMUXCOM->Update_Escape(0);
			blcheck = 1;
		}
		else if (strPar1 == "-A"){
			tCMUXCOM->Update_HEX(0);
			blcheck = 1;
		}
		else if (strPar1 == "-E"){
			SplitPar1(&strPar1,&strPar2);
			if (strPar1 == "on"){
				tCMUXCOM->Update_Escape(1);
				tCMUXCOM->Update_HEX(0);
				blcheck = 1;
			}
			else if (strPar1 == "off"){
				tCMUXCOM->Update_Escape(0);
				blcheck = 1;
			}
		}
		SplitPar1(&strPar1,&strPar2);
	};

	if ((par.length() > 0) && (blcheck == 0))
		return(Help(env));
	
	PrintResult(env,"DLCI",Str_ToString(num),":"
				,(tCMUXCOM->Check_HEX() == 0)?"ASCII mode,":"HEX mode,"
				,(tCMUXCOM->Check_Escape() == 0)?"disable escape":"enable escape");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif

