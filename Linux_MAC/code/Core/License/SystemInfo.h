/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: SystemInfo.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2013.10.22
*/
#ifndef SystemInfoH
#define SystemInfoH
//------------------------------------------------------------------------------------------//
#include "BasicDef.h"
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_VC
//------------------------------------------------------------------------------------------//
void	APIComSignature_GetNetCardInfo	(STDSTR *retStr);
void	APIComSignature_GetHardiskInfo	(STDSTR *retStr);
void	APIComSignature_GetBOISInfo		(STDSTR *retStr);
void	SystemInfo_GetHardiskInfo		(STDSTR *retStr);
int32	SystemInfo_GetNetCardInfo		(STDSTR *retStr);
void	SystemInfo_GetCPUID				(STDSTR *retStr);
#endif
#ifdef CommonDefH_Unix//used for test in MAC
inline	const STDSTR&	SystemInfo_GetHardiskInfo		(STDSTR *retStr){
	*retStr += "HD1 is HD0001\r";
	*retStr += "HD2 is HD0002\r";
	*retStr += "HD3 is HD0003\r";
	*retStr += "HD4 is HD0004\r";
	return(*retStr);
};
		const STDSTR&	SystemInfo_GetNetCardInfo		(STDSTR *retStr);
inline	const STDSTR&	SystemInfo_GetCPUID				(STDSTR *retStr){*retStr += "CUPID is TESTCPU001";return(*retStr);};
#endif
//------------------------------------------------------------------------------------------//

#endif
