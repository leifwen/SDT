//
//  BIC_ConCtrl.cpp
//  SDT
//
//  Created by Leif Wen on 04/04/2017.
//  Copyright © 2017 leif wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "BIC_ConCtrl.h"
#include "AppLayer.h"
//------------------------------------------------------------------------------------------//
#ifdef BIC_ConCtrl_hpp
//------------------------------------------------------------------------------------------//
int32 BIC_DISPLAY::Command(BIC_ENV *env,const STDSTR &par,void *p)const{
	if (par == "on"){
		env->blDisplayAuto = 0;
		env->cstdout->PrintEnable();
	}
	else if (par == "off"){
		env->blDisplayAuto = 0;
		env->cstdout->PrintDisable();
	}
	else if (par == "auto"){
		env->blDisplayAuto = 1;
	}
	else if (par.length() > 0){
		return(Help(env));
	}
	if (env->blDisplayAuto != 0){
		PrintResult(env,"Display is in auto mode");
	}
	else if (env->cstdout->CheckPrintDisable()	!= 0){
		PrintResult(env,"Display is disable");
	}
	else{
		PrintResult(env,"Display is enable");
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_NEWRECORD::Command(BIC_ENV *env,const STDSTR &par,void *p)const{
	if (par.length() > 0)
		return(Help(env));
	
	env->cstdout->PrintEnable();
	env->cSDTApp->m_LogCache.CreateG2_FILE(ODEV_FILE::CreateNewLOGFileName());
	PrintMessage(env,"Set new record file:\n",env->cSDTApp->m_LogCache.GetG2_File()->GetFileName());
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif

