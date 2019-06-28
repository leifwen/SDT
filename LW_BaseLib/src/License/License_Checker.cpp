//
//  License_Checker.cpp
//  SDT
//
//  Created by Leif Wen on 05/03/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "License_Checker.h"
#ifdef License_Checker_h
//------------------------------------------------------------------------------------------//
#include "SYS_File.h"
#include "SYS_Time.h"
#include "ADS_SMC.h"
#include "PatchCode.h"
#include <math.h>
//------------------------------------------------------------------------------------------//
static long double		CHK_erf				(long double x,uint32 maxN);
static long double		CHK_Gaussian		(long double x,long double u,long double b);
static void				CHK_GetRATime		(DTIME* cRTime,DTIME* cATime);
static bool32			CHK_CheckATime		(void);
static bool32			CHK_CheckBTime		(void);
//------------------------------------------------------------------------------------------//
#ifdef PatchCode_h
static LINENSE_Content& CHK_GetLinense(void){
	static	LINENSE_Content	sgLinense;
	return (sgLinense);
}
#endif
//------------------------------------------------------------------------------------------//
static uint8& CHK_GetOverTime(void){
	static	uint8	sgOverTime = 0;
	return (sgOverTime);
};
//------------------------------------------------------------------------------------------//
void CHK_CheckerInit(void){
#ifdef PatchCode_h
	LINENSE_SIGN	tLS;
	
	RAND_bytes(&CHK_GetOverTime(),1);
	CHK_GetOverTime() = (uint8)sqrtl(CHK_GetOverTime()) + 5;
	
	tLS.Decode(&CHK_GetLinense(),IUD_FILE(PATCHCODE::GetFNLic()));
#endif
};
//------------------------------------------------------------------------------------------//
void CHK_CheckerInitClear(void){
#ifdef PatchCode_h
	RAND_bytes(&CHK_GetOverTime(),1);
	CHK_GetOverTime() = (uint8)sqrtl(CHK_GetOverTime()) + 5;
	CHK_GetLinense().FillZero();
	PATCHCODE::LoadPatchCode().Release();
#endif
};
//------------------------------------------------------------------------------------------//
bool32 CHK_CheckTime(void){
#ifdef PatchCode_h
	static int32	sgCheckTime = 0;
	uint8	rand[4];
	if (--sgCheckTime < 0){
		RAND_bytes(rand,4);
		rand[0] = (uint32)sqrtl(rand[0]) + 1;
		rand[1] = (uint32)sqrtl(rand[1]) + 1;
		rand[2] = (uint32)sqrtl(rand[0]) + 1;
		rand[3] = (uint32)sqrtl(rand[1]) + 1;
		sgCheckTime = 20 * 50 * 60 * (rand[0] * 2 + 10);
		try{
			rand[0] /= CHK_CheckATime();
			rand[1] /= CHK_CheckBTime();
			rand[3] += (1 + rand[0] + rand[1] + rand[2]);
		}
		catch(...){
			rand[3] = 0;
			CHK_GetLinense().FillZero();
		}
	}
	return(rand[3]);
#else
	return G_TRUE;
#endif
};
//------------------------------------------------------------------------------------------//
void CHK_GetCurrentTime(DTIME* cCTime){
	cCTime->Now();
};
//------------------------------------------------------------------------------------------//
static void CHK_GetRATime(DTIME* cRTime,DTIME* cATime){
#ifdef PatchCode_h
	STDSTR	strTime,sKey;
	
	if (CFS_CheckFile(PATCHCODE::GetFNLic())){
		if (DecryptTime(cRTime, cATime,&CHK_GetLinense()))
			return;
	}
	
	PATCHCODE::GetTime(cRTime);
#endif
	cATime->EncodeTimeABS(0, 3, 0, 0);
};
//------------------------------------------------------------------------------------------//
static bool32 CHK_CheckATime(void){
#ifdef PatchCode_h
	DTIME chk_cTime,chk_regTime,chk_aTime;
	
	CHK_GetRATime(&chk_regTime,&chk_aTime);
	CHK_GetCurrentTime(&chk_cTime);
	
	if ((CHK_Gaussian(chk_cTime.GetSec(),chk_regTime.GetSec(),chk_aTime.GetSec()) > sqrtl(0.9 * 0.9))){
		if (CHK_GetOverTime() > 0){
			if (CFS_CheckFile(PATCHCODE::GetFNLic()) > 0){
				std::fstream 	fileStream;
				uint8	offset;
				RAND_bytes(&offset,1);
				fileStream.open(PATCHCODE::GetFNLic().c_str(),std::ios::in|std::ios::out|std::ios::binary);
				fileStream.seekp(offset,std::ios::beg);
				fileStream << offset;
				fileStream.flush();
				fileStream.close();
				-- CHK_GetOverTime();
				return -1;
			}
		}
		return G_FALSE;
	}
#endif
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
static bool32 CHK_CheckBTime(void){
#ifdef PatchCode_h
	DTIME chk_cTime,chk_regTime,chk_aTime;
	
	CHK_GetRATime(&chk_regTime,&chk_aTime);
	CHK_GetCurrentTime(&chk_cTime);
	
	if ((CHK_Gaussian(chk_cTime.GetSec(),chk_regTime.GetSec(),chk_aTime.GetSec()) < sqrtl(0.48 * 0.48))){
		if (CHK_GetOverTime() > 0){
			if (CFS_CheckFile(PATCHCODE::GetFNLic()) > 0){
				std::fstream 	fileStream;
				uint8	offset;
				RAND_bytes(&offset,1);
				fileStream.open(PATCHCODE::GetFNLic().c_str(),std::ios::in|std::ios::out|std::ios::binary);
				fileStream.seekp(offset,std::ios::beg);
				fileStream << offset;
				fileStream.flush();
				fileStream.close();
				-- CHK_GetOverTime();
				return -1;
			}
		}
		return G_FALSE;
	}
#endif
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
bool32 CHK_GetSMC(STDSTR* smc,uint32 codeNo){
#ifdef PatchCode_h
	return(DecryptSMC(smc,&CHK_GetLinense(),codeNo));
#else
	return G_FALSE;
#endif
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
static const double CHK_PI = 3.1415926;
//------------------------------------------------------------------------------------------//
static long double CHK_erf(long double x,uint32 maxN){
	uint32		n;
	long double n2,ret,itemD,itemU;
	
	if (x > CHK_PI)
		x = CHK_PI;
	
	ret = x;
	itemU = x;
	n2 = x * x;
	n = 1;
	itemD = 1;
	do{
		itemU *= n2;
		itemD *= n;
		if ((n % 2) == 0){
			ret += (itemU / (itemD * (2 * n + 1)));
		}else{
			ret -= (itemU / (itemD * (2 * n + 1)));
		}
		++n;
	}while(n < maxN);
	
	ret = 2 * ret / sqrtl(CHK_PI);
	return(ret);
};
//------------------------------------------------------------------------------------------//
static long double CHK_Gaussian(long double x,long double u,long double b){
	uint32		n;
	long double n2,ret,itemD,itemU;
	
	x = (x - u) / (b * sqrtl(2));
	
	if (x > CHK_PI)
		x = CHK_PI;
	if (x < (- CHK_PI))
		x = - CHK_PI;
	
	ret = x;
	itemU = x;
	n2 = x * x;
	n = 1;
	itemD = 1.0;
	do{
		itemU *= n2;
		itemD *= n;
		if ((n % 2) == 0){
			ret += (itemU / (itemD * (2 * n + 1)));
		}else{
			ret -= (itemU / (itemD * (2 * n + 1)));
		}
		++n;
	}while(n < 100);
	
	ret = 0.5 + ret / sqrtl(CHK_PI);
	
	return(ret);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif /* License_Checker_h */
