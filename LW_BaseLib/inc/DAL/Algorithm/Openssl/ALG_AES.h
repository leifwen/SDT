//
//  ALG_AES.h
//  SDT
//
//  Created by Leif Wen on 22/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#include "BasicDefine.h"
//------------------------------------------------------------------------------------------//
#if (defined USE_OPENSSL) || (defined CommonDefH_MAC)
//------------------------------------------------------------------------------------------//
#include "DS_Transform.h"
#ifdef DS_Transform_h
//------------------------------------------------------------------------------------------//
#ifndef ALG_AES_h
#define ALG_AES_h
#ifdef ALG_AES_h
//------------------------------------------------------------------------------------------//
#ifdef USE_MAC_Crypto
	#include <CommonCrypto/CommonCryptor.h>
#endif
#ifdef USE_OPENSSL
	#include <openssl/crypto.h>
	#include <openssl/md5.h>
	#include <openssl/bio.h>
	#include <openssl/evp.h>
	#include <openssl/ossl_typ.h>
	#include <openssl/pem.h>
	#include <openssl/rand.h>
	#ifndef ALG_Crypto_INIT
		#define ALG_Crypto_INIT
		#define	CY_Init()		(OpenSSL_add_all_algorithms())
	#endif
#endif
//------------------------------------------------------------------------------------------//
struct ALG_AES_CTX : public DSTF_DIR_CTX{
	STDSTR			key;
#ifdef USE_MAC_Crypto
	CCCryptorRef	ctx;
#else
#ifdef USE_OPENSSL
	EVP_CIPHER_CTX	ctx;
#endif
#endif
};
//------------------------------------------------------------------------------------------//
void	ALG_AES_Init	(ALG_AES_CTX* ctx,uint32 cfg,const STDSTR& sKey);
IOSE	ALG_AES_Update	(ALG_AES_CTX* ctx,uint8* _out,const uint64& outSize,const uint8* data,const uint64& length);
IOSE	ALG_AES_Final	(ALG_AES_CTX* ctx,uint8* _out,const uint64& outSize);
IOSE	ALG_AES_Release	(ALG_AES_CTX* ctx);
IOSE	ALG_AES_ReInit	(ALG_AES_CTX* ctx);
//------------------------------------------------------------------------------------------//
class ALG_AES : public DSTF_DIR<ALG_AES_CTX>{
	protected:
		enum	{CFG_C = 3, CFG_S = DSTF_DIR::CFG_S + DSTF_DIR::CFG_C};
	public:
		enum	{CFG_Encrypt	= CFG_INIT_CFG | CFG_DIR0
				,CFG_Decrypt	= CFG_INIT_CFG | CFG_DIR1
				,CFG_AES128		= CFG_INIT_CFG | CGF_CREATE(0)
				,CFG_AES192		= CFG_INIT_CFG | CGF_CREATE(1)
				,CFG_AES256		= CFG_INIT_CFG
				,CFG_AES_CBC	= CFG_INIT_CFG | CGF_CREATE(2)
				,CFG_AES_CFB8	= CFG_INIT_CFG
		};
	public:
				 ALG_AES(void);
		virtual ~ALG_AES(void);
};
//------------------------------------------------------------------------------------------//
#endif /* ALG_AES_h */
#endif /* ALG_AES_h */
#endif
#endif
