/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: SBIC_Condition.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.08.12
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "SBIC_Condition.h"
#include "Comm_Convert.h"
//------------------------------------------------------------------------------------------//
#ifdef SBIC_ConditionH
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_RECEIVE::Command(SBIC_ENV *env,const STDSTR &par,void *retTrue)const{
	STDSTR *ret = static_cast<STDSTR*>(retTrue);
	SBIC_RETSTR		strRETSTR;
	SBIC_Combine	sCombine;
	SBIC_Search		sSearch;
	
	*ret = 'F';
	if (env != nullptr){
		sCombine.Command(env,par,&strRETSTR);
		if (sSearch.Search(nullptr,env->cstdin_Str,strRETSTR.forSend) != 0){
			*ret = 'T';
		}
		else if (env->cstdin_Str.find(strRETSTR.forSend) != STDSTR::npos){
			*ret = 'T';
		}
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_CTS::Command(SBIC_ENV *env,const STDSTR &par,void *retTrue)const{
#ifdef DeviceH
	STDSTR *ret = static_cast<STDSTR*>(retTrue);
	STDSTR	strPar;
	
	*ret = 'F';
	strPar = Str_LowerCase(Str_Trim(par));
	if ((env != nullptr) && (env->eda->IsCom())){
		if (env->eda->ACom()->GetDSRStatus() == "H"){
			if ((strPar == "h") || (strPar == "1") || (strPar == "high"))
				*ret = 'T';
		}
		else if ((strPar == "l") || (strPar == "0") || (strPar == "low")){
			*ret = 'T';
		}
	}
#endif
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_DSR::Command(SBIC_ENV *env,const STDSTR &par,void *retTrue)const{
#ifdef DeviceH
	STDSTR *ret = static_cast<STDSTR*>(retTrue);
	STDSTR	strPar;
	
	*ret = 'F';
	strPar = Str_LowerCase(Str_Trim(par));
	if ((env != nullptr) && (env->eda->IsCom())){
		if (env->eda->ACom()->GetDSRStatus() == "H"){
			if ((strPar == "h") || (strPar == "1") || (strPar == "high"))
				*ret = 'T';
		}
		else if ((strPar == "l") || (strPar == "0") || (strPar == "low")){
			*ret = 'T';
		}
	}
#endif
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_RING::Command(SBIC_ENV *env,const STDSTR &par,void *retTrue)const{
#ifdef DeviceH
	STDSTR *ret = static_cast<STDSTR*>(retTrue);
	STDSTR	strPar;
	
	*ret = 'F';
	strPar = Str_LowerCase(Str_Trim(par));
	if ((env != nullptr) && (env->eda->IsCom())){
		if (env->eda->ACom()->GetRINGStatus() == "H"){
			if ((strPar == "h") || (strPar == "1") || (strPar == "high"))
				*ret = 'T';
		}
		else if ((strPar == "l") || (strPar == "0") || (strPar == "low")){
			*ret = 'T';
		}
	}
#endif
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_DCD::Command(SBIC_ENV *env,const STDSTR &par,void *retTrue)const{
#ifdef DeviceH
	STDSTR *ret = static_cast<STDSTR*>(retTrue);
	STDSTR	strPar;
	
	*ret = 'F';
	strPar = Str_LowerCase(Str_Trim(par));
	if ((env != nullptr) && (env->eda->IsCom())){
		if (env->eda->ACom()->GetDCDStatus() == "H"){
			if ((strPar == "h") || (strPar == "1") || (strPar == "high"))
				*ret = 'T';
		}
		else if ((strPar == "l") || (strPar == "0") || (strPar == "low")){
			*ret = 'T';
		}
	}
#endif
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_TIMEOUT::Command(SBIC_ENV *env,const STDSTR &par,void *retTrue)const{
	STDSTR *ret = static_cast<STDSTR*>(retTrue);
	*ret = 'F';
	if (env != nullptr){
		if (SYS_Delay_CheckTS(&env->cgTimeS) != 0)
			*ret = 'T';
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_nullptr::Command(SBIC_ENV *env,const STDSTR &par,void *retTrue)const{
	STDSTR *ret = static_cast<STDSTR*>(retTrue);
	*ret = 'F';
	if ((env != nullptr) && (env->cstdin_Str.length() == 0))
		*ret = 'T';
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_RX_B::Command(SBIC_ENV *env,const STDSTR &par,void *retTrue)const{
	STDSTR *ret = static_cast<STDSTR*>(retTrue);
	*ret = 'F';
	if ((env != nullptr) && (env->cstdin_Str.length() > (STDSTR::size_type)strtoll(par.c_str(),nullptr,10)))
		*ret = 'T';
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_RX_S::Command(SBIC_ENV *env,const STDSTR &par,void *retTrue)const{
	STDSTR *ret = static_cast<STDSTR*>(retTrue);
	*ret = 'F';
	if ((env != nullptr) && (env->cstdin_Str.length() < (STDSTR::size_type)strtoll(par.c_str(),nullptr,10)))
		*ret = 'T';
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_RX_E::Command(SBIC_ENV *env,const STDSTR &par,void *retTrue)const{
	STDSTR *ret = static_cast<STDSTR*>(retTrue);
	*ret = 'F';
	if ((env != nullptr) && (env->cstdin_Str.length() == (STDSTR::size_type)strtoll(par.c_str(),nullptr,10)))
		*ret = 'T';
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_LCS::Command(SBIC_ENV *env,const STDSTR &par,void *retTrue)const{
	STDSTR *ret = static_cast<STDSTR*>(retTrue);
	STDSTR	strPar;
	
	*ret = 'F';
	strPar = Str_LowerCase(Str_Trim(par));
	if (env != nullptr){
		if (env->cgFunRet == "T"){
			if ((strPar == "t") || (strPar == "1") || (strPar == "true"))
				*ret = 'T';
		}
		else if ((strPar == "f") || (strPar == "0") || (strPar == "false")){
			*ret = 'T';
		}
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_Conditon::Execute(SBIC_ENV *env,const STDSTR &fullCMD,void *retTrue)const{
	STDSTR *ret = static_cast<STDSTR*>(retTrue);
	int32	retCode;
	STDSTR	retPar;
	
	if (Check(env,fullCMD,&retPar) == cgReturnCode){
		retCode = Command(env,retPar,ret);
		if (retCode == SBI_RETCODE_NO)
			*ret = 'F';
		return(cgReturnCode);
	}
	return(SBI_RETCODE_NO);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_Conditon::Help(SBIC_ENV *env,int32 blDetail )const{
	HelpTraversalChild(env,blDetail);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
#endif
