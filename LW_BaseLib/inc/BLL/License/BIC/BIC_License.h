//
//  BIC_License.h
//  SDT
//
//  Created by Leif Wen on 06/03/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "BIC_Define.h"
#include "PatchCode.h"
#if defined BIC_Define_h && defined PatchCode_h
//------------------------------------------------------------------------------------------//
#ifndef BIC_License_h
#define BIC_License_h
#ifdef BIC_License_h
//------------------------------------------------------------------------------------------//
BIC_CLASSTYPE(PATCH,		"patch");
BIC_CLASSTYPE(APPROVE,		"approve");
BIC_CLASSTYPE(LSERVER,		"lserver");
BIC_CLASSTYPE(REGISTRATION,	"reg");
//------------------------------------------------------------------------------------------//
class BIC_BIN : public BIC_BASE_S{
	public:
				 BIC_BIN(void);
		virtual ~BIC_BIN(void){;};
	public:
		
	public:
		BIC_PATCH			cgSub_patch;
		BIC_APPROVE			cgSub_approve;
		BIC_LSERVER			cgSub_lserver;
		BIC_REGISTRATION	cgSub_registraion;
};
//------------------------------------------------------------------------------------------//
#endif /* BIC_License_h */
#endif /* BIC_License_h */
#endif /* BIC_Define_h */
