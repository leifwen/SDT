//
//  ALG_RSA.h
//  SDT
//
//  Created by Leif Wen on 23/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "BasicDefine.h"
//------------------------------------------------------------------------------------------//
#ifdef USE_OPENSSL
//------------------------------------------------------------------------------------------//
#ifndef ALG_RSA_h
#define ALG_RSA_h
#ifdef ALG_RSA_h
//------------------------------------------------------------------------------------------//
#include <openssl/crypto.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#ifndef ALG_Crypto_INIT
	#define ALG_Crypto_INIT
	#define	CY_Init()	(OpenSSL_add_all_algorithms())
#endif
//------------------------------------------------------------------------------------------//
STDSTR	ALG_CreateRandKey	(uint32 length);
bool32	ALG_RSA_CreateKey	(		RSA* _out	,int bits);
bool32	ALG_RSA_WR_Prk_PEM	(const	RSA* _in	,const STDSTR& fileName);
bool32	ALG_RSA_RD_Prk_PEM	(		RSA**_out	,const STDSTR& fileName);
bool32	ALG_RSA_WR_Puk_PEM	(const	RSA* _in	,const STDSTR& fileName);
bool32	ALG_RSA_RD_Puk_PEM	(		RSA**_out	,const STDSTR& fileName);
//------------------------------------------------------------------------------------------//
static inline	const	STDSTR& ALG_RSA_Encrypt_Prk	(STDSTR* _out,const RSA* _rsa,const uint8* data,uint32 length);
static inline	const	STDSTR& ALG_RSA_Decrypt_Prk	(STDSTR* _out,const RSA* _rsa,const uint8* data,uint32 length);
static inline	const	STDSTR& ALG_RSA_Encrypt_Puk	(STDSTR* _out,const RSA* _rsa,const uint8* data,uint32 length);
static inline	const	STDSTR& ALG_RSA_Decrypt_Puk	(STDSTR* _out,const RSA* _rsa,const uint8* data,uint32 length);

static inline	const	STDSTR& ALG_RSA_Encrypt_Prk	(STDSTR* _out,const RSA* _rsa,const STDSTR& _in);
static inline	const	STDSTR& ALG_RSA_Decrypt_Prk	(STDSTR* _out,const RSA* _rsa,const STDSTR& _in);
static inline	const	STDSTR& ALG_RSA_Encrypt_Puk	(STDSTR* _out,const RSA* _rsa,const STDSTR& _in);
static inline	const	STDSTR& ALG_RSA_Decrypt_Puk	(STDSTR* _out,const RSA* _rsa,const STDSTR& _in);

				const	STDSTR& ALG_RSA_Encode_Prk	(STDSTR* _out,const RSA* _in);
				const	STDSTR& ALG_RSA_Encode_Puk	(STDSTR* _out,const RSA* _in);
static inline			bool32	ALG_RSA_Decode_Prk	(RSA ** _out,const uint8* data,uint32 num);
static inline			bool32	ALG_RSA_Decode_Puk	(RSA ** _out,const uint8* data,uint32 num);
static inline			bool32	ALG_RSA_Decode_Prk	(RSA ** _out,const STDSTR& _in);
static inline			bool32	ALG_RSA_Decode_Puk	(RSA ** _out,const STDSTR& _in);
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#include "ALG_RSA.hpp"
#endif /* ALG_RSA_h */
#endif /* ALG_RSA_h */
#endif
