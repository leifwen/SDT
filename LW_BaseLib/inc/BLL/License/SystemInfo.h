//
//  SystemInfo.h
//  SDT
//
//  Created by Leif Wen on 17/03/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#ifndef SystemInfo_h
#define SystemInfo_h
#ifdef SystemInfo_h
//------------------------------------------------------------------------------------------//
#include "Config.h"
#include "BasicDefine.h"
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_VC
//------------------------------------------------------------------------------------------//
void			SI_GetNetCardInfo			(STDSTR* retStr);
void			SI_GetHardiskInfo			(STDSTR* retStr);
void			SI_GetBOISInfo				(STDSTR* retStr);
const STDSTR&	SystemInfo_GetHardiskInfo	(STDSTR* retStr);
uint32			SystemInfo_GetNetCardInfo	(STDSTR* retStr);
const STDSTR&	SystemInfo_GetCPUID			(STDSTR* retStr);
#endif
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_Unix//used for test in MAC
const STDSTR&	SystemInfo_GetHardiskInfo	(STDSTR *retStr);
uint32			SystemInfo_GetNetCardInfo	(STDSTR *retStr);
const STDSTR&	SystemInfo_GetCPUID			(STDSTR *retStr);
#endif





























//------------------------------------------------------------------------------------------//
#endif /* SystemInfo_h */
#endif /* SystemInfo_h */
