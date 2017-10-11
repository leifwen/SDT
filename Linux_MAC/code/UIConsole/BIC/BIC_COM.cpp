/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: BIC_COM.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.01.25
 
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "BIC_COM.h"
#include "Comm_Convert.h"
#include "AppLayer.h"
//------------------------------------------------------------------------------------------//
#if defined BIC_COMH
//------------------------------------------------------------------------------------------//
int32 BIC_COM::Command(BIC_ENV *env,const STDSTR &par,void *eda)const{
	if (BI_SetConnectPar(static_cast<ExpandDeviceAttr*>(eda),par,CSType_COM) == 0)
		PrintFail(env,"already connected");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_COM_BR::Command(BIC_ENV *env,const STDSTR &par,void *eda)const{
	ExpandDeviceAttr* attr = static_cast<ExpandDeviceAttr*>(eda);
	
	if (par.length() == 0)
		return(Help(env));
	
	if (attr->IsCom()){
		BI_SetConnectPar2(attr,par);
	}
	else{
		PrintFail(env,"no COM device");
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_COM_DTR::Command(BIC_ENV *env,const STDSTR &par,void *eda)const{
	ExpandDeviceAttr* attr = static_cast<ExpandDeviceAttr*>(eda);
	
	STDSTR		strPar1,strPar2,strPar3;
	SYS_TIME_S	timeS;
	int32		bltime;
	
	if (attr->IsComOpened()){
		if (par.length() == 0){
			PrintResult(env,selfName,"COM DTR is",((attr->ACom()->GetDTRStatus() != 0)?"high":"low"));
		}
		else{
			SplitPar1(strPar1, strPar3, par);
			do{
				bltime = 0;
				SplitPar1(&strPar2,&strPar3);
				if ((strPar2.length() > 0) && (strPar2 != "-H") && (strPar2 != "-L")){
					SYS_Delay_SetTS(&timeS,atof(strPar2.c_str()) * 1000);
					bltime = 1;
				}
				if (strPar1 == "-H"){
					attr->ACom()->SetDTR(1);
				}
				else if (strPar1 == "-L"){
					attr->ACom()->SetDTR(0);
				}
				else{
					break;
				}
				if (bltime > 0){
					if (PressAnyKey(env,attr,&timeS) == 27){
						PrintSuccess(env,"Pressed ESC key, stopped execute command");
						return(cgReturnCode);
					}
				}
				else{
					strPar1 = strPar2;
					continue;
				}
				SplitPar1(&strPar1,&strPar3);
			}while(strPar1.length() > 0);
		}
	}
	else{
		PrintFail(env,"no COM connected");
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_COM_RTS::Command(BIC_ENV *env,const STDSTR &par,void *eda)const{
	ExpandDeviceAttr* attr = static_cast<ExpandDeviceAttr*>(eda);
	
	STDSTR		strPar1,strPar2,strPar3;
	SYS_TIME_S	timeS;
	int32		bltime;
	
	if (attr->IsComOpened()){
		if (par.length() == 0){
			PrintResult(env,selfName,"COM RTS is",((attr->ACom()->GetRTSStatus() != 0)?"high":"low"));
		}
		else{
			SplitPar1(strPar1, strPar3, par);
			do{
				bltime = 0;
				SplitPar1(&strPar2,&strPar3);
				if ((strPar2.length() > 0) && (strPar2 != "-H") && (strPar2 != "-L")){
					SYS_Delay_SetTS(&timeS,atof(strPar2.c_str()) * 1000);
					bltime = 1;
				}
				if (strPar1 == "-H"){
					attr->ACom()->SetRTS(1);
				}
				else if (strPar1 == "-L"){
					attr->ACom()->SetRTS(0);
				}
				else{
					break;
				}
				if (bltime > 0){
					if (PressAnyKey(env,attr,&timeS) == 27){
						PrintSuccess(env,"Pressed ESC key, stopped execute command");
						return(cgReturnCode);
					}
				}
				else{
					strPar1 = strPar2;
					continue;
				}
				SplitPar1(&strPar1,&strPar3);
			}while(strPar1.length() > 0);
		}
	}
	else{
		PrintFail(env,"no COM connected");
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_COM_MS::Command(BIC_ENV *env,const STDSTR &par,void *eda)const{
	ExpandDeviceAttr* attr = static_cast<ExpandDeviceAttr*>(eda);
	
	if (attr->IsComOpened()){
		if (par == "on"){
			B_SetFLAG64(*attr->envcfg, ODEV_FLAG_EnMSReport);
			PrintResult(env,"Enable",Str_LowerCase(selfName),"modem status report");
		}
		else if (par == "off"){
			B_ClrFLAG64(*attr->envcfg, ODEV_FLAG_EnMSReport);
			PrintResult(env,"Disable",Str_LowerCase(selfName),"modem status report");
		}
		else if (par.length() == 0){
			PrintResult(env,attr->ACom()->GetFullModemStatus());
		}
		else{
			Help(env);
		}
	}
	else{
		PrintFail(env,"no COM connected");
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_COM_LS::Command(BIC_ENV *env,const STDSTR &par,void *p)const{
	env->validComList->Refresh();
	
	PrintNL(env) << COL_DivStar_Default << COL_Result;
	TREE_LChildRChain_Traversal_LINE(IPCOMNAME,env->validComList,
		if ((operateNode_t->typeID == PublicDevice_DEVID_APICOM) && (operateNode_t->blAvailable != 0))
			STDout(env) << operateNode_t->strShowName << "\n";
	);
	STDout(env) << COL_DivStar_Default << Endl;
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_COM_OPEN::Command(BIC_ENV *env, const STDSTR &par,void *eda)const{
	ExpandDeviceAttr* attr = static_cast<ExpandDeviceAttr*>(eda);

	if (par.length() == 0){
		if (attr->IsConnected() == 0){
			attr->aCOM.type = CSType_COM;
			attr->device->Open(attr->aCOM);
		}
		else{
			BIC_COM::Command(env,"",eda);
		}
	}
#ifdef CommonDefH_Unix
	else if (par == "-v"){
		if (attr->IsConnected() == 0){
			attr->aCOM.type = CSType_COMV;
			attr->device->Open(attr->aCOM);
		}
		else{
			BIC_COM::Command(env,"",eda);
		}
	}
#endif
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif
