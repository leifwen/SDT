//
//  ALG_Digest.h
//  SDT
//
//  Created by Leif Wen on 23/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#include "BasicDefine.h"
//------------------------------------------------------------------------------------------//
#if (defined USE_OPENSSL) || (defined CommonDefH_MAC)
//------------------------------------------------------------------------------------------//
#ifndef ALG_Digest_h
#define ALG_Digest_h
#ifdef ALG_Digest_h
//------------------------------------------------------------------------------------------//
#include "DS_Transform.h"
//------------------------------------------------------------------------------------------//
#ifdef USE_MAC_Crypto
//------------------------------------------------------------------------------------------//
	#include <CommonCrypto/CommonDigest.h>
	#define CC_SHA224_CTX			CC_SHA256_CTX
	#define CC_SHA384_CTX			CC_SHA512_CTX
//------------------------------------------------------------------------------------------//
#else
#ifdef USE_OPENSSL
//------------------------------------------------------------------------------------------//
#include <openssl/crypto.h>
#include <openssl/md5.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#ifndef ALG_Crypto_INIT
	#define ALG_Crypto_INIT
	#define	CY_Init()	(OpenSSL_add_all_algorithms())
#endif
//------------------------------------------------------------------------------------------//
typedef uint32 CC_LONG;
#define SHA1_DIGEST_LENGTH		SHA_DIGEST_LENGTH
#define SHA1_CTX				SHA_CTX
#define SHA224_CTX				SHA256_CTX
#define SHA384_CTX				SHA512_CTX
//------------------------------------------------------------------------------------------//
#define CC_SHA1_DIGEST_LENGTH	SHA1_DIGEST_LENGTH
#define CC_SHA1_CTX				SHA1_CTX
#define	CC_SHA1_Init			SHA1_Init
#define CC_SHA1_Update			SHA1_Update
#define CC_SHA1_Final			SHA1_Final
#define CC_SHA1					SHA1

#define CC_SHA224_DIGEST_LENGTH	SHA224_DIGEST_LENGTH
#define CC_SHA224_CTX			SHA224_CTX
#define	CC_SHA224_Init			SHA224_Init
#define CC_SHA224_Update		SHA224_Update
#define CC_SHA224_Final			SHA224_Final
#define CC_SHA224				SHA224

#define CC_SHA256_DIGEST_LENGTH	SHA256_DIGEST_LENGTH
#define CC_SHA256_CTX			SHA256_CTX
#define	CC_SHA256_Init			SHA256_Init
#define CC_SHA256_Update		SHA256_Update
#define CC_SHA256_Final			SHA256_Final
#define CC_SHA256				SHA256

#define CC_SHA384_DIGEST_LENGTH	SHA384_DIGEST_LENGTH
#define CC_SHA384_CTX			SHA384_CTX
#define	CC_SHA384_Init			SHA384_Init
#define CC_SHA384_Update		SHA384_Update
#define CC_SHA384_Final			SHA384_Final
#define CC_SHA384				SHA384

#define CC_SHA512_DIGEST_LENGTH	SHA512_DIGEST_LENGTH
#define CC_SHA512_CTX			SHA512_CTX
#define	CC_SHA512_Init			SHA512_Init
#define CC_SHA512_Update		SHA512_Update
#define CC_SHA512_Final			SHA512_Final
#define CC_SHA512				SHA512

#define CC_MD5_DIGEST_LENGTH	MD5_DIGEST_LENGTH
#define CC_MD5_CTX				MD5_CTX
#define	CC_MD5_Init				MD5_Init
#define CC_MD5_Update			MD5_Update
#define CC_MD5_Final			MD5_Final
#define CC_MD5					MD5
//------------------------------------------------------------------------------------------//
#endif
#endif
//------------------------------------------------------------------------------------------//
#define Define_ALG_Method_CTX(md) \
	union ALG_##md##_CTX		{CC_##md##_CTX	ctx;};
//------------------------------------------------------------------------------------------//
Define_ALG_Method_CTX(SHA1);
Define_ALG_Method_CTX(SHA224);
Define_ALG_Method_CTX(SHA256);
Define_ALG_Method_CTX(SHA384);
Define_ALG_Method_CTX(SHA512);
Define_ALG_Method_CTX(MD5);
//------------------------------------------------------------------------------------------//
#define Define_ALG_FUN(md) \
static inline uint32 ALG_Digest_Size	(ALG_##md##_CTX* ctx)											{return(CC_##md##_DIGEST_LENGTH);};\
static inline int	 ALG_Digest_Init	(ALG_##md##_CTX* ctx)											{return(CC_##md##_Init	(&ctx->ctx));};\
static inline int	 ALG_Digest_Update32(ALG_##md##_CTX* ctx, const void *data, CC_LONG len)			{return(CC_##md##_Update(&ctx->ctx,data,len));};\
static inline int	 ALG_Digest_Final	(ALG_##md##_CTX* ctx, uint8* ret)								{return(CC_##md##_Final	(ret,&ctx->ctx));};\
static inline uint8* ALG_Digest32		(ALG_##md##_CTX* ctx, uint8* ret,const uint8* data, CC_LONG len){return(CC_##md			(data,len,ret));};\
//------------------------------------------------------------------------------------------//
template <typename ALGCTX> static inline STDSTR	ALG_Digest64		(ALGCTX* ctxnullptr,const uint8* data,uint64 length);
template <typename ALGCTX> static inline void	ALG_Digest_Update64	(ALGCTX* ctx,		const uint8* data,uint64 length);
//------------------------------------------------------------------------------------------//
#define Define_ALG_Digest0(md) \
	static inline STDSTR ALG_Digest_##md (const uint8* data,uint64 length){\
		return(ALG_Digest64((ALG_##md##_CTX*)nullptr,data,length));\
	};\

#define Define_ALG_Digest1(md) \
	static inline STDSTR ALG_Digest_##md (const STDSTR& _in){\
		return(ALG_Digest64((ALG_##md##_CTX*)nullptr,(uint8*)_in.c_str(),_in.length()));\
	};

#define Define_ALG_Digest2(md) \
	static inline STDSTR ALG_Digest_##md (const char* _in){\
		return(ALG_Digest64((ALG_##md##_CTX*)nullptr,(uint8*)_in,strlen(_in)));\
	};
//------------------------------------------------------------------------------------------//
#ifdef DS_Transform_h
#ifndef ALG_DS_DIGEST
#define ALG_DS_DIGEST
#ifdef ALG_DS_DIGEST
//------------------------------------------------------------------------------------------//
#define Define_ALG_Digest3(md)\
	static inline STDSTR ALG_Digest_##md (const UVIn& _in){\
		ALG_DIGEST_T<ALG_##md##_CTX>	digest;\
		STDSTR	ret;\
		digest.InitCFG().Calc(nullptr,_EMPTY(&ret),_NONE(),_in);\
		return(ret);\
	};
template <typename ALGCTX> class ALG_DIGEST_T : public DSTF{
	public:
				 ALG_DIGEST_T(void);
		virtual ~ALG_DIGEST_T(void){;};
	public:
		ALGCTX	cgCTX;
	public:
		inline	virtual	ALG_DIGEST_T&	InitCFG		(uint32 cfg = 0,const void* p = nullptr);
	protected:
		inline	virtual	ALG_DIGEST_T&	InitSize	(uint32 size);
		inline	virtual	IOSE			DoTransform	(IOS* _ios,const UVOut& _out,const uint8* data,const uint64& length);
		inline	virtual	IOSE			DoFinal		(IOS* _ios,const UVOut& _out);
	public:
		inline			ALG_DIGEST_T&	GetResult	(IOS* _ios,const UVOut& _result);
		inline			ALG_DIGEST_T&	Calc		(IOS* _ios,const UVOut& _result,const UVOut& _out,const UVIn& _in);
		inline			uint32			GetResultSize(void);
};
//------------------------------------------------------------------------------------------//
typedef ALG_DIGEST_T<ALG_SHA1_CTX>	 	ALG_SHA1;
typedef ALG_DIGEST_T<ALG_SHA224_CTX>	ALG_SHA224;
typedef ALG_DIGEST_T<ALG_SHA256_CTX>	ALG_SHA256;
typedef ALG_DIGEST_T<ALG_SHA384_CTX>	ALG_SHA384;
typedef ALG_DIGEST_T<ALG_SHA512_CTX>	ALG_SHA512;
typedef ALG_DIGEST_T<ALG_MD5_CTX>	 	ALG_MD5;
#else
#define Define_ALG_Digest3(md);
#endif	/* ALG_DS_DIGEST */
#endif	/* ALG_DS_DIGEST */
#endif	/* DS_Transform_h */
//------------------------------------------------------------------------------------------//
#include "ALG_Digest.hpp"
#endif /* ALG_Digest_h */
#endif /* ALG_Digest_h */
#endif
