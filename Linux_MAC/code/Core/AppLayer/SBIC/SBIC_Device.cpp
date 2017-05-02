//
//  SBIC_Device.cpp
//  SDT
//
//  Created by Leif Wen on 04/04/2017.
//  Copyright © 2017 leif wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "SBIC_Device.h"
#include "Device.h"
#include "CommandGList.h"
//------------------------------------------------------------------------------------------//
#ifdef SBIC_Device_hpp
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_DTR::Command(SBIC_ENV *env,const STDSTR &par,void *p)const{
#ifdef DeviceH
	int32 	timeout;
	STDSTR	strPar1,strPar2,strTempData;
	
	if ((env != nullptr) && (env->eda->IsCom())){
		SplitPar1(strPar1, strPar2, par, ",");
		Str_LowerCaseSelf(strPar1);
		timeout = GetMS(strPar2);
		
		strTempData = "Set DTR to ";
		if ((strPar1 == "h") || (strPar1 == "1") || (strPar1 == "high")){
			strTempData += "high";
			if (timeout > 0){
				strTempData += ", and delay ";
				strTempData += GetMSSTR(timeout);
			}
			PrintExecute(env,strTempData);
			env->eda->ACom()->SetDTRToHigh();
			SYS_DelayMS(timeout,&env->blExit);
		}else if ((strPar1 == "l") || (strPar1 == "0") || (strPar1 == "low")){
			strTempData += "low";
			if (timeout > 0){
				strTempData += ", and delay ";
				strTempData += GetMSSTR(timeout);
			}
			PrintExecute(env,strTempData);
			env->eda->ACom()->SetDTRToLow();
			SYS_DelayMS(timeout,&env->blExit);
		}
	}
#endif
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_RTS::Command(SBIC_ENV *env,const STDSTR &par,void *p)const{
#ifdef DeviceH
	int32 	timeout;
	STDSTR	strPar1,strPar2,strTempData;
	
	if ((env != nullptr) && (env->eda->IsCom())){
		SplitPar1(strPar1, strPar2, par, ",");
		Str_LowerCaseSelf(strPar1);
		timeout = GetMS(strPar2);
		
		strTempData = "Set RTS to ";
		if ((strPar1 == "h") || (strPar1 == "1") || (strPar1 == "high")){
			strTempData += "high";
			if (timeout > 0){
				strTempData += ", and delay ";
				strTempData += GetMSSTR(timeout);
			}
			PrintExecute(env,strTempData);
			env->eda->ACom()->SetRTSToHigh();
			SYS_DelayMS(timeout,&env->blExit);
		}else if ((strPar1 == "l") || (strPar1 == "0") || (strPar1 == "low")){
			strTempData += "low";
			if (timeout > 0){
				strTempData += ", and delay ";
				strTempData += GetMSSTR(timeout);
			}
			PrintExecute(env,strTempData);
			env->eda->ACom()->SetRTSToLow();
			SYS_DelayMS(timeout,&env->blExit);
		}
	}
#endif
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_OpenCom::Command(SBIC_ENV *env,const STDSTR &par,void *p)const{
#ifdef DeviceH
	int32 	baudrate;
	STDSTR	strPar1,strPar2;
	
	if (env != nullptr){
		env->cgFunRet = 'F';
		env->eda->device->CloseD();
		
		SplitPar1(strPar1, strPar2, par, ",");
		baudrate = atoi(Str_Trim(strPar2).c_str());
		
		PrintExecute(env,"Open",strPar1 + "@" + Str_ToString(baudrate));
		if (env->eda->device->OpenD(strPar1,baudrate,CSType_COM,env->eda->device->CheckEcho()) != 0){
			env->cgFunRet = 'T';
		}
		else if (env->cgCommand != nullptr){
			env->cgCommand->StrCatch = "Fail";
			++ env->cgCommand->catchTimes;
		}
	}
#endif
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_ChangeBR::Command(SBIC_ENV *env,const STDSTR &par,void *p)const{
#ifdef DeviceH
	int32 	baudrate;
	
	if ((env != nullptr) && (env->eda->IsCom())){
		baudrate = atoi(Str_Trim(par).c_str());
		PrintExecute(env,"Change Baudrate to",Str_ToString(baudrate));
		env->eda->ACom()->SetBaudrate(baudrate);
	}
#endif
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_OpenTCPSocket::Command(SBIC_ENV *env,const STDSTR &par,void *p)const{
#ifdef DeviceH
	int32 	port;
	STDSTR	strPar1,strPar2;
	
	if (env != nullptr){
		env->cgFunRet = 'F';
		env->eda->device->CloseD();
		
		SplitPar1(strPar1, strPar2, par, ",");
		port = atoi(Str_Trim(strPar2).c_str());
		
		PrintExecute(env,"Open TCP",strPar1 + "@" + Str_ToString(port));
		if (env->eda->device->OpenD(strPar1,port,CSType_TCP,env->eda->device->CheckEcho()) != 0){
			env->cgFunRet = 'T';
		}
		else if (env->cgCommand != nullptr){
			env->cgCommand->StrCatch = "Fail";
			++ env->cgCommand->catchTimes;
		}
	}
#endif
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_CloseTCPSocket::Command(SBIC_ENV *env,const STDSTR &par,void *p)const{
#ifdef DeviceH
	if (env != nullptr){
		env->cgFunRet = 'F';
		if (env->eda->IsSocket()){
			PrintExecute(env,"Close TCP Socket");
			env->eda->ASocket()->CloseD();
			env->cgFunRet = 'T';
		}
	}
#endif
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_OpenUDPSocket::Command(SBIC_ENV *env,const STDSTR &par,void *p)const{
#ifdef DeviceH
	int32 	port;
	STDSTR	strPar1,strPar2;
	
	if (env != nullptr){
		env->cgFunRet = 'F';
		env->eda->device->CloseD();
		
		SplitPar1(strPar1, strPar2, par, ",");
		port = atoi(Str_Trim(strPar2).c_str());
		
		PrintExecute(env,"Open UDP",strPar1 + "@" + Str_ToString(port));
		if (env->eda->device->OpenD(strPar1,port,CSType_UDP,env->eda->device->CheckEcho()) != 0){
			env->cgFunRet = 'T';
		}
		else if (env->cgCommand != nullptr){
			env->cgCommand->StrCatch = "Fail";
			++ env->cgCommand->catchTimes;
		}
	}
#endif
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_CloseUDPSocket::Command(SBIC_ENV *env,const STDSTR &par,void *p)const{
#ifdef DeviceH
	if (env != nullptr){
		env->cgFunRet = 'F';
		if (env->eda->IsSocket()){
			PrintExecute(env,"Close UDP Socket");
			env->eda->ASocket()->CloseD();
			env->cgFunRet = 'T';
		}
	}
#endif
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_SetRecvDataMode::Command(SBIC_ENV *env,const STDSTR &par,void *p)const{
#ifdef DeviceH
	if (env != nullptr){
		if (Str_UpperCase(Str_Trim(par)) == "HEX"){
			B_SetFLAG64(*env->eda->envcfg, ODEV_FLAG_EnHEXViewMode);
		}else if (Str_UpperCase(Str_Trim(par)) == "ASCII"){
			B_ClrFLAG64(*env->eda->envcfg, ODEV_FLAG_EnHEXViewMode);
		}
	}
#endif
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_SetRecMsgReport::Command(SBIC_ENV *env,const STDSTR &par,void *p)const{
#ifdef DeviceH
	if (env != nullptr){
		if (Str_UpperCase(Str_Trim(par)) == "ENABLE"){
			B_SetFLAG64(*env->eda->envcfg, ODEV_FLAG_EnRecMsg);
		}else if (Str_UpperCase(Str_Trim(par)) == "DISABLE"){
			B_ClrFLAG64(*env->eda->envcfg, ODEV_FLAG_EnRecMsg);
		}
	}
#endif
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif
