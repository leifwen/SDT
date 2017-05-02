//
//  SBIC_Define.cpp
//  SDT
//
//  Created by Leif Wen on 02/04/2017.
//  Copyright © 2017 leif wen. All rights reserved.
//

#include "stdafx.h"
#include "SBIC_Define.h"
#include "Device.h"
//------------------------------------------------------------------------------------------//
void SetDefAttr(SBIC_ENV *env,const DEVICE *cDevice,SBUFFER *cstdin,OUTPUT_NODE *cstdout){
	SetDefAttr(env);
	if (cDevice != nullptr){
		#ifdef DeviceH
		env->cstdout = cDevice->GetLogSystem()->cache->GetG1_STDOUT();
		env->eda = &((DEVICE*)cDevice)->cgEDA;
		#endif
	}
	env->cstdin = cstdin;
	if (cstdout != nullptr)
		env->cstdout = cstdout;
}
//------------------------------------------------------------------------------------------//
