//
//  ALG_AES.cpp
//  SDT
//
//  Created by Leif Wen on 22/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
#include "ALG_AES.h"
#include "ALG_Digest.h"
//------------------------------------------------------------------------------------------//
#ifdef ALG_AES_h
//#define NOAESTEST
//------------------------------------------------------------------------------------------//
#ifndef	NOAESTEST
#ifdef CommonDefH_VC
	#pragma comment(lib, "libeay32.lib")
	#pragma comment(lib, "ssleay32.lib")
#endif
#endif
//#define LOGPRINT_ENABLE
#include "SYS_Log.h"
//------------------------------------------------------------------------------------------//
static inline STDSTR AES_Create_IV(void)	{return(ALG_Digest_SHA256("leif"));};
//------------------------------------------------------------------------------------------//
STDSTR ALG_AES_FormatKey(uint32 cfg,const STDSTR& sKey){
	STDSTR key = "";
	if (B_ChkFLAG32(cfg, ALG_AES::CFG_AES128 & (~DSTF::CFG_INIT_CFG))){
		key = ALG_Digest_MD5(sKey);
	}
	else if (B_ChkFLAG32(cfg, ALG_AES::CFG_AES192 & (~DSTF::CFG_INIT_CFG))){
		key = ALG_Digest_SHA1(sKey) + "leif";
	}
	else{
		key = ALG_Digest_SHA256(sKey);
	}
	return(key);
}
//------------------------------------------------------------------------------------------//
void ALG_AES_Init(ALG_AES_CTX* ctx,uint32 cfg,const STDSTR& sKey){
#ifndef NOAESTEST
	
	B_ClrFLAG32(cfg, DSTF::CFG_DSTF_CTRL | DSTF::CFG_INIT);
	
	if (B_ChkFLAG32(ctx->cfg, DSTF::CFG_INIT) && (ctx->cfg == (cfg | DSTF::CFG_INIT)) && (ctx->key == sKey))
		return;
	
	ALG_AES_Release(ctx);
	
	ctx->cfg = cfg;
	ctx->key = ALG_AES_FormatKey(ctx->cfg,sKey);
	ctx->par = &ctx->key;
	
	ALG_AES_ReInit(ctx);
#endif
}
//------------------------------------------------------------------------------------------//
#ifdef USE_MAC_Crypto
//------------------------------------------------------------------------------------------//
bool32 ALG_AES_ReInit(ALG_AES_CTX* ctx){
#ifndef NOAESTEST
	size_t		KeySize;
	CCOperation op;

	ALG_AES_Release(ctx);

	op = B_ChkFLAG32(ctx->cfg, ALG_AES::CFG_Encrypt) ? kCCEncrypt : kCCDecrypt;
	
	if (B_ChkFLAG32(ctx->cfg, ALG_AES::CFG_AES128)){
		KeySize = kCCKeySizeAES128;
	}
	else if (B_ChkFLAG32(ctx->cfg, ALG_AES::CFG_AES192)){
		KeySize = kCCKeySizeAES192;
	}
	else{
		KeySize = kCCKeySizeAES256;
	}
	
	if (B_ChkFLAG32(ctx->cfg, ALG_AES::CFG_AES_CBC)){
		CCCryptorCreate(op
						,kCCAlgorithmAES
						,kCCOptionPKCS7Padding
						,ctx->key.c_str()
						,KeySize
						,AES_Create_IV().c_str()
						,&ctx->ctx
						);
	}
	else{
		CCCryptorCreateWithMode(op
								,kCCModeCFB8
								,kCCAlgorithmAES
								,ccPKCS7Padding
								,AES_Create_IV().c_str()
								,ctx->key.c_str()
								,KeySize,ctx->key.c_str()
								,KeySize
								,0
								,kCCModeOptionCTR_BE
								,&ctx->ctx
								);
	}
	ctx->cfg |= DSTF::CFG_INIT;
#endif
	return G_TRUE;
}
#else
#ifdef USE_OPENSSL
//------------------------------------------------------------------------------------------//
typedef int(*AES_UpdateFun)	(EVP_CIPHER_CTX *ctx, unsigned char *out, int *outl,const unsigned char *in, int inl);
//------------------------------------------------------------------------------------------//
bool32 ALG_AES_ReInit(ALG_AES_CTX* ctx){
#ifndef NOAESTEST
	EVP_CIPHER*		evpCipher;
	
	ALG_AES_Release(ctx);
	
	if (B_ChkFLAG32(ctx->cfg, ALG_AES::CFG_AES_CBC)){
		if (B_ChkFLAG32(ctx->cfg, ALG_AES::CFG_AES128)){
			evpCipher = (EVP_CIPHER*)EVP_aes_128_cbc();
		}
		else if (B_ChkFLAG32(ctx->cfg, ALG_AES::CFG_AES192)){
			evpCipher = (EVP_CIPHER*)EVP_aes_192_cbc();
		}
		else{
			evpCipher = (EVP_CIPHER*)EVP_aes_256_cbc();
		}
	}
	else{
		if (B_ChkFLAG32(ctx->cfg, ALG_AES::CFG_AES128)){
			evpCipher = (EVP_CIPHER*)EVP_aes_128_cfb8();
		}
		else if (B_ChkFLAG32(ctx->cfg, ALG_AES::CFG_AES192)){
			evpCipher = (EVP_CIPHER*)EVP_aes_192_cfb8();
		}
		else{
			evpCipher = (EVP_CIPHER*)EVP_aes_256_cfb8();
		}
	}
	EVP_CIPHER_CTX_init(&ctx->ctx);

	if (B_ChkFLAG32(ctx->cfg, ALG_AES::CFG_Encrypt)){
		EVP_EncryptInit_ex(&ctx->ctx,evpCipher,nullptr,(uint8*)ctx->key.c_str(),(uint8*)AES_Create_IV().c_str());
	}
	else{
		EVP_DecryptInit_ex(&ctx->ctx,evpCipher,nullptr,(uint8*)ctx->key.c_str(),(uint8*)AES_Create_IV().c_str());
	}
	ctx->cfg |= DSTF::CFG_INIT;
#endif
	return G_TRUE;
}
//------------------------------------------------------------------------------------------//
#endif
#endif
#ifdef USE_MAC_Crypto
typedef		size_t	DATA_LEN;
#else
#ifdef USE_OPENSSL
typedef		int32	DATA_LEN;
#endif
#endif
//------------------------------------------------------------------------------------------//
bool32 ALG_AES_Update(ALG_AES_CTX* ctx,uint8* _out,const uint64& outSize,const uint8* data,const uint64& length){
#ifdef NOAESTEST
	IOSTATUS ios;
	ctx->avail_in = length;
	ctx->avail_out = outSize;
	
	DS_IO_NODE::GetDSIOList().Save(IOSTATUS_Clr(&ios), OUD_CHARS(_out,outSize), IUD(data,length));
	
	ctx->avail_in	-= ios.total_in;
	ctx->total_in	+= ios.total_in;
	
	ctx->avail_out	-= ios.total_out;
	ctx->total_out	+= ios.total_out;
	
	ctx->next_in	= (uint8*)(data + ios.total_in);
	ctx->next_out	= (uint8*)(_out + ios.total_out);
#else
	const int32 package = 1024 * 16;
	DATA_LEN	total_out = 0,avail_in;

	ctx->next_in = (uint8*)data;
	ctx->avail_in = length;
	ctx->next_out = _out;
	ctx->avail_out = outSize;

	while(ctx->avail_in > 0){
		if (ctx->avail_out <= 16)
			return AES_NOMEM;
		avail_in = (ctx->avail_in < package) ? (int32)ctx->avail_in : package;

		if (avail_in > (int32)(ctx->avail_out - 16))
			avail_in = (int32)(ctx->avail_out - 16);
#ifdef USE_MAC_Crypto
		CCCryptorUpdate(ctx->ctx,  ctx->next_in, avail_in, ctx->next_out, ctx->avail_out, &total_out);
#else
#ifdef USE_OPENSSL
		if (B_ChkFLAG32(ctx->cfg, ALG_AES::CFG_Encrypt)){
			EVP_EncryptUpdate(&ctx->ctx, ctx->next_out, &total_out, ctx->next_in, avail_in);
		}
		else{
			EVP_DecryptUpdate(&ctx->ctx, ctx->next_out, &total_out, ctx->next_in, avail_in);
		}
#endif
#endif
		ctx->next_in += avail_in;
		ctx->total_in += avail_in;
		ctx->avail_in -= avail_in;
		
		ctx->next_out += total_out;
		ctx->avail_out -= total_out;
		ctx->total_out += total_out;
	};
#endif
	return AES_OK;
}
//------------------------------------------------------------------------------------------//
bool32 ALG_AES_Final(ALG_AES_CTX* ctx,uint8* _out,const uint64& outSize){
#ifdef NOAESTEST
	ctx->avail_in = 0;
	ctx->total_in = 0;
	
	ctx->avail_out = 0;
	ctx->total_out = 0;
	
	ctx->next_in	= nullptr;
	ctx->next_out	= _out;
#else

	DATA_LEN	total_out = 0;
	
	ctx->next_in = nullptr;
	ctx->avail_in = 0;
	ctx->next_out = _out;
	ctx->avail_out = outSize;

	if (ctx->avail_out < 16)
		return AES_NOMEM;
#ifdef USE_MAC_Crypto
	CCCryptorFinal(ctx->ctx, ctx->next_out, ctx->avail_out, &total_out);
#else
#ifdef USE_OPENSSL
	if (B_ChkFLAG32(ctx->cfg, ALG_AES::CFG_Encrypt)){
		EVP_EncryptFinal_ex(&ctx->ctx, ctx->next_out, &total_out);
	}
	else{
		EVP_DecryptFinal_ex(&ctx->ctx, ctx->next_out, &total_out);
	}
#endif
#endif
	ctx->total_out += total_out;
	ctx->avail_out -= total_out;
#endif
	return AES_FINISH;
}
//------------------------------------------------------------------------------------------//
bool32 ALG_AES_Release(ALG_AES_CTX* ctx){
#ifndef NOAESTEST
	if (B_ChkFLAG32(ctx->cfg, ALG_AES::CFG_INIT)){
#ifdef USE_MAC_Crypto
		CCCryptorRelease(ctx->ctx);
#else
#ifdef USE_OPENSSL
		EVP_CIPHER_CTX_cleanup(&ctx->ctx);
#endif
#endif
		B_ClrFLAG32(ctx->cfg, ALG_AES::CFG_INIT);
	}
#endif
	return AES_OK;
}
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
static inline void ALG_AES_Init(void* ctx,uint32 cfg,const void* key){
	STDSTR	*sKey = (STDSTR*)key;
	ALG_AES_Init((ALG_AES_CTX*)ctx,cfg,*sKey);
	ELog(((ALG_AES_CTX*)ctx)->dstf << "Init():   "
		 << (B_ChkFLAG32(((ALG_AES_CTX*)ctx)->cfg, ALG_AES::CFG_Encrypt) ? "Encrpyt" : "Decrpyt"));
};
//------------------------------------------------------------------------------------------//
static inline bool32 ALG_AES_Update(void* ctx,uint8* _out,const uint64& outSize,const uint8* data,const uint64& length){
	ELog(((ALG_AES_CTX*)ctx)->dstf << "Update(): "
		 << (B_ChkFLAG32(((ALG_AES_CTX*)ctx)->cfg, ALG_AES::CFG_Encrypt) ? "Encrpyt" : "Decrpyt") << "   ,len=" << length);
	return(ALG_AES_Update((ALG_AES_CTX*)ctx,_out,outSize,data,length));
};
//------------------------------------------------------------------------------------------//
static inline bool32 ALG_AES_Final(void* ctx,uint8* _out,const uint64& outSize,const uint8* data,const uint64& length){
	ELog(((ALG_AES_CTX*)ctx)->dstf << "Final():  "
		 << (B_ChkFLAG32(((ALG_AES_CTX*)ctx)->cfg, ALG_AES::CFG_Encrypt) ? "Encrpyt" : "Decrpyt"));
	return(ALG_AES_Final((ALG_AES_CTX*)ctx,_out,outSize));
};
//------------------------------------------------------------------------------------------//
static inline bool32 ALG_AES_Release(void* ctx){
	ELog(((ALG_AES_CTX*)ctx)->dstf << "Release()");
	ALG_AES_Release((ALG_AES_CTX*)ctx);
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
static inline bool32 ALG_AES_ReInit(void* ctx){
	ELog(((ALG_AES_CTX*)ctx)->dstf << "ReInit()");
	ALG_AES_ReInit((ALG_AES_CTX*)ctx);
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
ALG_AES::ALG_AES(void) : DSTF_DIR(){
	cgCTX.Init		= ALG_AES_Init;
	cgCTX.Update	= ALG_AES_Update;
	cgCTX.Final		= ALG_AES_Final;
	cgCTX.Release	= ALG_AES_Release;
	cgCTX.ReInit	= ALG_AES_ReInit;
	TNFP::SetSelfName("AES");
}
//------------------------------------------------------------------------------------------//
ALG_AES::~ALG_AES(void){
	ALG_AES_Release(&cgCTX);
}
//------------------------------------------------------------------------------------------//
#endif /* ALG_AES_h */
