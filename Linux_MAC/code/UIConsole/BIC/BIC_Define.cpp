//
//  BIC_Define.cpp
//  SDT
//
//  Created by Leif Wen on 29/03/2017.
//  Copyright © 2017 leif wen. All rights reserved.
//
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "BIC_Define.h"
#include "AppLayer.h"
//------------------------------------------------------------------------------------------//
void SetDefConsoleAttr(BIC_ENV *env,SDTAPP *cSDTApp,SBUFFER *cstdin,OUTPUT_NODE *cstdout){
	SetDefConsoleAttr(env);
	env->cSDTApp = cSDTApp;
	#ifdef ODEV_SystemH
	env->cstdout = cSDTApp->m_LogCache.GetG1_STDOUT();
	#endif
	#ifdef ParRecordH
	env->validComList =  &cSDTApp->m_IPComList;
	#endif
	#ifdef DeviceH
	env->eda = &cSDTApp->m_Device.cgEDA;
	env->eda2 = &cSDTApp->m_Device2.cgEDA;
	#endif
	env->cstdin = cstdin;
	if (cstdout != nullptr)
		env->cstdout = cstdout;
}
//------------------------------------------------------------------------------------------//
