//
//  ADS_Crypto.cpp
//  SDT
//
//  Created by Leif Wen on 25/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "ADS_Crypto.h"
//------------------------------------------------------------------------------------------//
#ifdef ADS_Crypto_h
//------------------------------------------------------------------------------------------//
AES_ZLIB::AES_ZLIB(void) : DSTF_AB_FRAME<ALG_ZLIB,ALG_AES>(){
	TNFP::SetSelfName("AES_ZLIB");
	cgA.selfName = "ZLIB";
	cgB.selfName = "AES";
	SetUpName("");
};
//------------------------------------------------------------------------------------------//
AES_ZLIB& AES_ZLIB::InitCFG(uint32 cfg,const void* key){
	STDSTR	*sKey = (STDSTR*)key;

	if (B_ChkFLAG32(cgCTX.cfg,CFG_INIT) == G_FALSE)
		cfg |= (CFG_INIT_A | CFG_INIT_B);
	
	DSTF_DIRBASE::InitCFG(cfg,nullptr);
	B_SetFLAG32(cgCTX.cfg,CFG_INIT);

	if (B_ChkFLAG32(cfg, CFG_ATOB)){
		if (B_ChkFLAG32(cfg, CFG_INIT_A)){
			cgA.InitCFG((cfg >> 16) | ALG_ZLIB::CFG_Compress | CFG_INIT_CFG);
		}
		if (B_ChkFLAG32(cfg, CFG_INIT_B)){
			cgB.InitCFG((cfg >> 8)  | ALG_AES::CFG_Encrypt | CFG_INIT_CFGPAR,key);
		}
	}
	else{
		if (B_ChkFLAG32(cfg, CFG_INIT_A)){
			cgA.InitCFG(((cfg >> 16) & (~ALG_ZLIB::CFG_Compress)) | CFG_INIT_CFG);
		}
		if (B_ChkFLAG32(cfg, CFG_INIT_B)){
			cgB.InitCFG(((cfg >> 8) & (~ALG_AES::CFG_Encrypt)) | CFG_INIT_CFGPAR,key);
		}
	}
	
	if (key == nullptr){
		DisableB();
	}
	else if (sKey->length() == 0){
		DisableB();
	}
	return(*this);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif /* ADS_Crypto_h */
