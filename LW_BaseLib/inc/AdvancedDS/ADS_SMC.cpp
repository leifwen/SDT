//
//  ADS_SMC.cpp
//  SDT
//
//  Created by Leif Wen on 27/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
#include "ADS_SMC.h"
#ifdef ADS_SMC_h
//------------------------------------------------------------------------------------------//
#include "DS_STRING.h"
#include "License_Checker.h"
//------------------------------------------------------------------------------------------//
SMC_BLOCK::SMC_BLOCK(void){
	blInit = G_FALSE;
	cgMemOffset = nullptr;
	cgCodeSize = 0;
	cgOldMemCode = "";
};
//------------------------------------------------------------------------------------------//
bool32 SMC_BLOCK::Init(uint8* offset,uint32 codeSize){
	bool32	blRet;
	
	blRet = G_FALSE;
	if (blInit == G_FALSE){
		try{
			cgOldMemCode = Str_CharToASCII(offset,codeSize,G_ESCAPE_OFF);
			cgMemOffset = offset;
			cgCodeSize = codeSize;
			blInit = G_TRUE;
			blRet = G_TRUE;
		}
		catch(...){
			blInit = G_FALSE;
			blRet = -1;
		}
	}
	return(blRet);
}
//------------------------------------------------------------------------------------------//
bool32 SMC_BLOCK::Decrypt(void){
	bool32	blRet;
	uint32	cyCodeNo;
	STDSTR	runCode;
	
	blRet = G_FALSE;
	if (blInit){
		cyCodeNo = Str_CharToUint16((uint8*)cgOldMemCode.c_str());
		if (CHK_GetSMC(&runCode,cyCodeNo)){
			if (cgCodeSize == runCode.length()){
				try{
#ifdef CommonDefH_VC
					CopyMemory(cgMemOffset,runCode.c_str(),cgCodeSize);
#endif
					blRet = G_TRUE;
				}
				catch(...){
					blRet = -1;
				}
			}
		}
	}
	return(blRet);
}
//------------------------------------------------------------------------------------------//
bool32 SMC_BLOCK::Resume(void){
	bool32	blRet;
	
	blRet = G_FALSE;
	if (blInit){
		try{
#ifdef CommonDefH_VC
			CopyMemory(cgMemOffset,cgOldMemCode.c_str(),cgCodeSize);
#endif
			blRet = G_TRUE;
		}
		catch(...){
			blRet = -1;
		}
	}
	return(blRet);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif /* ADS_SMC_h */
