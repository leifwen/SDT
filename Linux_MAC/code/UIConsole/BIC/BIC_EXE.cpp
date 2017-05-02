//
//  BIC_EXE.cpp
//  SDT
//
//  Created by Leif Wen on 04/04/2017.
//  Copyright © 2017 leif wen. All rights reserved.
//
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "BIC_EXE.h"
#include "AppLayer.h"
//------------------------------------------------------------------------------------------//
#ifdef BIC_EXE_hpp
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_EXEC::Command(BIC_ENV *env,const STDSTR &par,void *p)const{
	AEXE	*aexe;
	if (par.length() == 0)
		return(cgReturnCode);
	
	aexe = ExecuteCMD(env,par);
	
	if (aexe == nullptr){
		PrintFail(env);
		return(cgReturnCode);
	}
	
	if (OnlineMode(env,aexe,CMD_N) > 0)
		PrintSuccess(env,"Pressed double ESC key, stopped child process");
	env->cSDTApp->m_AExePool.ChildClose(aexe);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
AEXE *BIC_EXEC::ExecuteCMD(BIC_ENV *env,const STDSTR &cmd)const{
	return(env->cSDTApp->m_AExePool.Execute("BIC_EXEC", cmd));
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
AEXE *BIC_EXE_BASH::ExecuteCMD(BIC_ENV *env,const STDSTR &cmd)const{
	return(env->cSDTApp->m_AExePool.BASH("BIC_EXE_BASH", cmd));
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
AEXE *BIC_EXE_SH::ExecuteCMD(BIC_ENV *env,const STDSTR &cmd)const{
	return(env->cSDTApp->m_AExePool.SH("BIC_EXE_SH", cmd));
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif
