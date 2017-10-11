/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: Comm_Crypto.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.03.26
*/
#ifndef Comm_CryptoH
#define Comm_CryptoH
//------------------------------------------------------------------------------------------//
#include "Comm_PA.h"
#include "Comm_Convert.h"
#include "Comm_File.h"
//------------------------------------------------------------------------------------------//
#ifdef Comm_CryptoH
//------------------------------------------------------------------------------------------//
enum CCT_Digest		{CCT_MD5 = 0,CCT_SHA1,CCT_SHA224,CCT_SHA256,CCT_SHA384,CCT_SHA512};
enum CCT_AES_KEYL	{CCT_AES128 = 0,CCT_AES192,CCT_AES256};
enum CCT_AES_MODE	{CCT_AES_CBC = 0,CCT_AES_CFB8};
enum CCT_BASE64_NL	{CCT_NL_NO = 0,CCT_NL_YES = 1};
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_MAC
#include <CommonCrypto/CommonDigest.h>
#include <CommonCrypto/CommonCryptor.h>
#define CC_SHA224_CTX			CC_SHA256_CTX
#define CC_SHA384_CTX			CC_SHA512_CTX
#else
#define CC_LONG					uint32

#define CC_SHA1_DIGEST_LENGTH	SHA_DIGEST_LENGTH
#define CC_SHA1_CTX				SHA_CTX
#define	CC_SHA1_Init			SHA1_Init
#define CC_SHA1_Update			SHA1_Update
#define CC_SHA1_Final			SHA1_Final
#define CC_SHA1					SHA1

#define CC_SHA224_DIGEST_LENGTH	SHA224_DIGEST_LENGTH
#define CC_SHA224_CTX			SHA256_CTX
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
#define CC_SHA384_CTX			SHA512_CTX
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
#endif
#ifdef USE_OPENSSL
#include <openssl/crypto.h>
#include <openssl/md5.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#define	CY_Init()	(OpenSSL_add_all_algorithms())
#endif
//------------------------------------------------------------------------------------------//
long double CCY_erf					(long double x,uint32 maxN);
long double CCY_Gaussian			(long double x,long double u,long double b);
//------------------------------------------------------------------------------------------//
#if (defined USE_OPENSSL) || (defined CommonDefH_MAC)
union CCY_SHA1_CTX	{CC_SHA1_CTX c;};
union CCY_SHA224_CTX{CC_SHA256_CTX c;};
union CCY_SHA256_CTX{CC_SHA256_CTX c;};
union CCY_SHA384_CTX{CC_SHA512_CTX c;};
union CCY_SHA512_CTX{CC_SHA512_CTX c;};
union CCY_MD5_CTX	{CC_MD5_CTX c;};
//------------------------------------------------------------------------------------------//
inline uint32 CCY_Digest_Size	(CCY_SHA1_CTX *c)											{return(CC_SHA1_DIGEST_LENGTH);};
inline int CCY_Digest_Init		(CCY_SHA1_CTX *c)											{return(CC_SHA1_Init(&c->c));};
inline int CCY_Digest_Update	(CCY_SHA1_CTX *c, const void *data, CC_LONG len)			{return(CC_SHA1_Update(&c->c,data,len));};
inline int CCY_Digest_Final		(uint8 *md, CCY_SHA1_CTX *c)								{return(CC_SHA1_Final(md,&c->c));};
inline unsigned char *CCY_Digest(const uint8 *data, CC_LONG len, uint8 *md,CCY_SHA1_CTX *c)	{return(CC_SHA1(data,len,md));};

inline uint32 CCY_Digest_Size	(CCY_SHA224_CTX *c)											{return(CC_SHA224_DIGEST_LENGTH);};
inline int CCY_Digest_Init		(CCY_SHA224_CTX *c)											{return(CC_SHA224_Init(&c->c));};
inline int CCY_Digest_Update	(CCY_SHA224_CTX *c, const void *data, CC_LONG len)			{return(CC_SHA224_Update(&c->c,data,len));};
inline int CCY_Digest_Final		(uint8 *md, CCY_SHA224_CTX *c)								{return(CC_SHA224_Final(md,&c->c));};
inline unsigned char *CCY_Digest(const uint8 *data, CC_LONG len, uint8 *md,CCY_SHA224_CTX *c){return(CC_SHA224(data,len,md));};

inline uint32 CCY_Digest_Size	(CCY_SHA256_CTX *c)											{return(CC_SHA256_DIGEST_LENGTH);};
inline int CCY_Digest_Init		(CCY_SHA256_CTX *c)											{return(CC_SHA256_Init(&c->c));};
inline int CCY_Digest_Update	(CCY_SHA256_CTX *c, const void *data, CC_LONG len)			{return(CC_SHA256_Update(&c->c,data,len));};
inline int CCY_Digest_Final		(uint8 *md, CCY_SHA256_CTX *c)								{return(CC_SHA256_Final(md,&c->c));};
inline unsigned char *CCY_Digest(const uint8 *data, CC_LONG len, uint8 *md,CCY_SHA256_CTX *c){return(CC_SHA256(data,len,md));};

inline uint32 CCY_Digest_Size	(CCY_SHA384_CTX *c)											{return(CC_SHA384_DIGEST_LENGTH);};
inline int CCY_Digest_Init		(CCY_SHA384_CTX *c)											{return(CC_SHA384_Init(&c->c));};
inline int CCY_Digest_Update	(CCY_SHA384_CTX *c, const void *data, CC_LONG len)			{return(CC_SHA384_Update(&c->c,data,len));};
inline int CCY_Digest_Final		(uint8 *md, CCY_SHA384_CTX *c)								{return(CC_SHA384_Final(md,&c->c));};
inline unsigned char *CCY_Digest(const uint8 *data, CC_LONG len, uint8 *md,CCY_SHA384_CTX *c){return(CC_SHA384(data,len,md));};

inline uint32 CCY_Digest_Size	(CCY_SHA512_CTX *c)											{return(CC_SHA512_DIGEST_LENGTH);};
inline int CCY_Digest_Init		(CCY_SHA512_CTX *c)											{return(CC_SHA512_Init(&c->c));};
inline int CCY_Digest_Update	(CCY_SHA512_CTX *c, const void *data, CC_LONG len)			{return(CC_SHA512_Update(&c->c,data,len));};
inline int CCY_Digest_Final		(uint8 *md, CCY_SHA512_CTX *c)								{return(CC_SHA512_Final(md,&c->c));};
inline unsigned char *CCY_Digest(const uint8 *data, CC_LONG len, uint8 *md,CCY_SHA512_CTX *c){return(CC_SHA512(data,len,md));};

inline uint32 CCY_Digest_Size	(CCY_MD5_CTX *c)											{return(CC_MD5_DIGEST_LENGTH);};
inline int CCY_Digest_Init		(CCY_MD5_CTX *c)											{return(CC_MD5_Init(&c->c));};
inline int CCY_Digest_Update	(CCY_MD5_CTX *c, const void *data, CC_LONG len)				{return(CC_MD5_Update(&c->c,data,len));};
inline int CCY_Digest_Final		(uint8 *md, CCY_MD5_CTX *c)									{return(CC_MD5_Final(md,&c->c));};
inline unsigned char *CCY_Digest(const uint8 *data, CC_LONG len, uint8 *md,CCY_MD5_CTX *c)	{return(CC_MD5(data,len,md));};
//------------------------------------------------------------------------------------------//
template <typename CTX_TYPE>
STDSTR CCY_Digest(const uint8 *data,uint64 length,CTX_TYPE *t){
	CTX_TYPE	ctx;
	CC_LONG		packageLength;
	uint8		result[CC_SHA512_DIGEST_LENGTH];
	if (length == 0)
		return("");
	
	if (length > 0xffffffff){
		CCY_Digest_Init(&ctx);
		do{
			packageLength = (length > 0xffffffff) ? (0xffffffff): (CC_LONG)length;
			length -= packageLength;
			CCY_Digest_Update(&ctx,data,packageLength);
			data += packageLength;
		}while(length > 0);
		CCY_Digest_Final(result,&ctx);
	}
	else{
		CCY_Digest(data,(CC_LONG)length,result,&ctx);
	}
	return(Str_CharToASCIIStr(result,CCY_Digest_Size(&ctx),G_ESCAPE_OFF));
}

//------------------------------------------------------------------------------------------//
#ifdef Comm_FIFOH
template <typename CTX_TYPE>
STDSTR CCY_Digest(const FIFO8 &fifo,uint32 length,uint32 offset,CTX_TYPE *t){
	CTX_TYPE		ctx;
	CC_LONG			slength;
	uint8			result[CC_SHA512_DIGEST_LENGTH];
	
	slength = fifo.CalcOutLength(length,offset);
	if (length == 0)
		return("");
	
	CCY_Digest_Init(&ctx);
	CCY_Digest_Update(&ctx,fifo.GetPointer(offset),length);
	if (slength > 0)
		CCY_Digest_Update(&ctx,fifo.GetPointer(0),slength);
	CCY_Digest_Final(result,&ctx);
	return(Str_CharToASCIIStr(result,CCY_Digest_Size(&ctx),G_ESCAPE_OFF));
}
#endif
//------------------------------------------------------------------------------------------//
template <typename CTX_TYPE>
STDSTR CCY_DigestFile(const STDSTR &sFN,uint64 length,uint64 offset,CTX_TYPE *t){
	CTX_TYPE		ctx;
	CC_LONG			packageLength;
	uint8			result[CC_SHA512_DIGEST_LENGTH];
	uint8			data[1024 * 8];
	std::fstream	fileStream;
	
	if (CFS_CheckFile(sFN) == 0)
		return("");
	
	CCY_Digest_Init(&ctx);
	fileStream.open(sFN.c_str(),std::ios::in|std::ios::binary);
	if (offset > 0)
		fileStream.seekp(offset,std::ios::beg);
	do{
		packageLength = (length > sizeof(data))?sizeof(data):(CC_LONG)length;
		fileStream.read((char*)data,packageLength);
		packageLength = (CC_LONG)fileStream.gcount();
		length -= packageLength;
		CCY_Digest_Update(&ctx,data,packageLength);
	}while((!fileStream.eof()) && (length > 0));
	fileStream.close();
	CCY_Digest_Final(result,&ctx);
	return(Str_CharToASCIIStr(result,CCY_Digest_Size(&ctx),G_ESCAPE_OFF));
}
//------------------------------------------------------------------------------------------//
template <typename T,typename T2>static inline T& Ret(T* strRet,T2& _rn) {return(_rn._Ret(strRet));};
template <typename CTX_TYPE>
class CCY_DIGEST{
	public:
				 CCY_DIGEST(void){CCY_Digest_Init(&ctx);strResult = "";};
		virtual ~CCY_DIGEST(void){;};
	public:
		CTX_TYPE	ctx;
		STDSTR		strResult;
		uint8		result[1024];
	public:
		inline STDSTR& operator << (STDSTR&(*fun)(STDSTR*,CCY_DIGEST<CTX_TYPE>&))						{return((*fun)(&strResult,*this));};
		inline CCY_DIGEST<CTX_TYPE>& operator << (CCY_DIGEST<CTX_TYPE>&(*fun)(CCY_DIGEST<CTX_TYPE>&))	{return((*fun)(*this));};

		inline CCY_DIGEST<CTX_TYPE>& operator << (std::stringstream &streamIn)	{
			uint32			length;
			while(!streamIn.eof()){
				length = (uint32)streamIn.readsome((char*)result,sizeof(result));
				CCY_Digest_Update(&ctx,result,length);
			};
			return(*this);
		};
		#ifdef Comm_PAH
		inline CCY_DIGEST<CTX_TYPE>& operator << (const PROTOCOL_NODE &pnIn)		{
			uint32 length,offset,slength;
			length = pnIn.GetLength();
			offset = pnIn.GetOffset();
			slength = pnIn.GetDefFifo()->CalcOutLength(length,offset);
			if (length > 0){
				CCY_Digest_Update(&ctx,pnIn.GetDefFifo()->GetPointer(offset),length);
				if (slength > 0)
					CCY_Digest_Update(&ctx,pnIn.GetDefFifo()->GetPointer(0),slength);
			}
			return(*this);
		};
		#endif
		#ifdef Comm_FIFOH
		inline CCY_DIGEST<CTX_TYPE>& operator << (const _Data<const FIFO8*> &data)	{
			uint32 length,offset;
			uint32	slength;
			length = data.num;
			offset = data.offset;
			slength = data.p->CalcOutLength(length,offset);
			if (length > 0){
				CCY_Digest_Update(&ctx,data.p->GetPointer(offset),length);
				if (slength > 0)
					CCY_Digest_Update(&ctx,data.p->GetPointer(0),slength);
			}
			return(*this);
		};
		#endif
		inline CCY_DIGEST<CTX_TYPE>& operator << (const _Data<const uint8*> &data)	{CCY_Digest_Update(&ctx,data.p,data.num);return(*this);};
		inline CCY_DIGEST<CTX_TYPE>& operator << (const _Data<const char*> &data)	{CCY_Digest_Update(&ctx,data.p,data.num);return(*this);};
		inline CCY_DIGEST<CTX_TYPE>& operator << (const STDSTR &strIn)				{CCY_Digest_Update(&ctx,strIn.c_str(),(uint32)strIn.length());return(*this);};
		inline CCY_DIGEST<CTX_TYPE>& operator << (const char *data)					{CCY_Digest_Update(&ctx,data,(uint32)strlen(data));return(*this);};
	
		inline const STDSTR& _Ret (STDSTR *strRet){
			CCY_Digest_Final(result,&ctx);
			CCY_Digest_Init(&ctx);
			return(Str_CharToASCIIStr(strRet,result,CCY_Digest_Size(&ctx),G_ESCAPE_OFF));
		};
};
//------------------------------------------------------------------------------------------//
#ifdef Comm_FIFOH
inline STDSTR CCY_Digest_SHA1	(const FIFO8 &fifo,uint32 length = -1,uint32 offset = 0){return(CCY_Digest(fifo,length,offset,(CCY_SHA1_CTX*)nullptr));};
inline STDSTR CCY_Digest_SHA224	(const FIFO8 &fifo,uint32 length = -1,uint32 offset = 0){return(CCY_Digest(fifo,length,offset,(CCY_SHA224_CTX*)nullptr));};
inline STDSTR CCY_Digest_SHA256	(const FIFO8 &fifo,uint32 length = -1,uint32 offset = 0){return(CCY_Digest(fifo,length,offset,(CCY_SHA256_CTX*)nullptr));};
inline STDSTR CCY_Digest_SHA384	(const FIFO8 &fifo,uint32 length = -1,uint32 offset = 0){return(CCY_Digest(fifo,length,offset,(CCY_SHA384_CTX*)nullptr));};
inline STDSTR CCY_Digest_SHA512	(const FIFO8 &fifo,uint32 length = -1,uint32 offset = 0){return(CCY_Digest(fifo,length,offset,(CCY_SHA512_CTX*)nullptr));};
inline STDSTR CCY_Digest_MD5	(const FIFO8 &fifo,uint32 length = -1,uint32 offset = 0){return(CCY_Digest(fifo,length,offset,(CCY_MD5_CTX*)nullptr));};
#endif
inline STDSTR CCY_Digest_SHA1	(const uint8 *data,uint64 length){return(CCY_Digest(data,length,(CCY_SHA1_CTX*)nullptr));};
inline STDSTR CCY_Digest_SHA224	(const uint8 *data,uint64 length){return(CCY_Digest(data,length,(CCY_SHA224_CTX*)nullptr));};
inline STDSTR CCY_Digest_SHA256	(const uint8 *data,uint64 length){return(CCY_Digest(data,length,(CCY_SHA256_CTX*)nullptr));};
inline STDSTR CCY_Digest_SHA384	(const uint8 *data,uint64 length){return(CCY_Digest(data,length,(CCY_SHA384_CTX*)nullptr));};
inline STDSTR CCY_Digest_SHA512	(const uint8 *data,uint64 length){return(CCY_Digest(data,length,(CCY_SHA512_CTX*)nullptr));};
inline STDSTR CCY_Digest_MD5	(const uint8 *data,uint64 length){return(CCY_Digest(data,length,(CCY_MD5_CTX*)nullptr));};

inline STDSTR CCY_Digest_SHA1	(const STDSTR &strData){return(CCY_Digest_SHA1  ((uint8*)strData.c_str(),strData.length()));};
inline STDSTR CCY_Digest_SHA224	(const STDSTR &strData){return(CCY_Digest_SHA224((uint8*)strData.c_str(),strData.length()));};
inline STDSTR CCY_Digest_SHA256	(const STDSTR &strData){return(CCY_Digest_SHA256((uint8*)strData.c_str(),strData.length()));};
inline STDSTR CCY_Digest_SHA384	(const STDSTR &strData){return(CCY_Digest_SHA384((uint8*)strData.c_str(),strData.length()));};
inline STDSTR CCY_Digest_SHA512	(const STDSTR &strData){return(CCY_Digest_SHA512((uint8*)strData.c_str(),strData.length()));};
inline STDSTR CCY_Digest_MD5	(const STDSTR &strData){return(CCY_Digest_MD5   ((uint8*)strData.c_str(),strData.length()));};

inline STDSTR CCY_DigestFile_SHA1	(const STDSTR &sFN,uint64 length = -1,uint64 offset = 0){return(CCY_DigestFile(sFN,length,offset,(CCY_SHA1_CTX*)nullptr));};
inline STDSTR CCY_DigestFile_SHA224	(const STDSTR &sFN,uint64 length = -1,uint64 offset = 0){return(CCY_DigestFile(sFN,length,offset,(CCY_SHA224_CTX*)nullptr));};
inline STDSTR CCY_DigestFile_SHA256	(const STDSTR &sFN,uint64 length = -1,uint64 offset = 0){return(CCY_DigestFile(sFN,length,offset,(CCY_SHA256_CTX*)nullptr));};
inline STDSTR CCY_DigestFile_SHA384	(const STDSTR &sFN,uint64 length = -1,uint64 offset = 0){return(CCY_DigestFile(sFN,length,offset,(CCY_SHA384_CTX*)nullptr));};
inline STDSTR CCY_DigestFile_SHA512	(const STDSTR &sFN,uint64 length = -1,uint64 offset = 0){return(CCY_DigestFile(sFN,length,offset,(CCY_SHA512_CTX*)nullptr));};
inline STDSTR CCY_DigestFile_MD5	(const STDSTR &sFN,uint64 length = -1,uint64 offset = 0){return(CCY_DigestFile(sFN,length,offset,(CCY_MD5_CTX*)nullptr));};

		uint32	CCY_DigestSize	(CCT_Digest type);
#ifdef Comm_FIFOH
		STDSTR	CCY_Digest		(CCT_Digest type,const FIFO8 &fifo,uint32 length = -1,uint32 offset = 0);
#endif
		STDSTR	CCY_Digest		(CCT_Digest type,const uint8 *data,uint64 length);
inline	STDSTR	CCY_Digest		(CCT_Digest type,const STDSTR &strData){return(CCY_Digest(type,(uint8*)strData.c_str(),strData.length()));};
		STDSTR	CCY_DigestFile	(CCT_Digest type,const STDSTR &sFN,uint64 length = -1,uint64 offset = 0);
//------------------------------------------------------------------------------------------//
class CCY_AES{
	public:
				 CCY_AES(void){;};
		virtual ~CCY_AES(void){;};
	private:
	#ifdef CommonDefH_MAC
		CCCryptorRef	cryptorRef;
	#else
		EVP_CIPHER_CTX	ctx;
		int32			cryptorRef;
	#endif
		uint8			dataOut[1024 * 8];
	public:
		CCY_AES& InitEncrypt	(const STDSTR &sKey,CCT_AES_KEYL type,CCT_AES_MODE mode);
		CCY_AES& InitDecrypt	(const STDSTR &sKey,CCT_AES_KEYL type,CCT_AES_MODE mode);
	
	#ifdef Comm_FIFOH
		CCY_AES& Update			(STDSTR *retStr,const FIFO8 &fifo,uint32 length,uint32 offset);
	#endif
		CCY_AES& Update			(STDSTR *retStr,const uint8 *data,uint64 length);
		CCY_AES& Final			(STDSTR *retStr);
	
	#ifdef Comm_FIFOH
		CCY_AES& Update			(FIFO8 *retFifo,const FIFO8 &fifo,uint32 length,uint32 offset);
		CCY_AES& Update			(FIFO8 *retFifo,const uint8 *data,uint32 length);
		CCY_AES& Final			(FIFO8 *retFifo);
	#endif
	
		inline CCY_AES& Update	(STDSTR *retStr,const STDSTR &strData){return(Update(retStr,(uint8*)strData.c_str(),strData.length()));};
	
	#ifdef Comm_FIFOH
		inline CCY_AES& Update	(FIFO8 *retFifo,const STDSTR &strData){return(Update(retFifo,(uint8*)strData.c_str(),(uint32)strData.length()));};
	#endif
};
inline	STDSTR	CCY_AESKey32Bye(const STDSTR &sKey)	{return(CCY_Digest_SHA256(sKey));}
const	STDSTR& CCY_Encrypt_AES(STDSTR *retStr,const uint8 *data,uint64 length,const STDSTR &sKey,CCT_AES_KEYL type,CCT_AES_MODE mode,G_APPEND blAppend = G_APPEND_OFF);
const	STDSTR& CCY_Decrypt_AES(STDSTR *retStr,const uint8 *data,uint64 length,const STDSTR &sKey,CCT_AES_KEYL type,CCT_AES_MODE mode,G_APPEND blAppend = G_APPEND_OFF);

#ifdef Comm_FIFOH
const	STDSTR& CCY_Encrypt_AES(STDSTR *retStr,const FIFO8 &fifo,uint32 length,uint32 offset,const STDSTR &sKey,CCT_AES_KEYL type,CCT_AES_MODE mode,G_APPEND blAppend = G_APPEND_OFF);
const	STDSTR& CCY_Decrypt_AES(STDSTR *retStr,const FIFO8 &fifo,uint32 length,uint32 offset,const STDSTR &sKey,CCT_AES_KEYL type,CCT_AES_MODE mode,G_APPEND blAppend = G_APPEND_OFF);

const	FIFO8&	CCY_Encrypt_AES(FIFO8 *retFifo,const uint8 *data,uint32 length,const STDSTR &sKey,CCT_AES_KEYL type,CCT_AES_MODE mode);
const	FIFO8&	CCY_Decrypt_AES(FIFO8 *retFifo,const uint8 *data,uint32 length,const STDSTR &sKey,CCT_AES_KEYL type,CCT_AES_MODE mode);
const	FIFO8&	CCY_Encrypt_AES(FIFO8 *retFifo,const FIFO8 &fifo,uint32 length,uint32 offset,const STDSTR &sKey,CCT_AES_KEYL type,CCT_AES_MODE mode);
const	FIFO8&	CCY_Decrypt_AES(FIFO8 *retFifo,const FIFO8 &fifo,uint32 length,uint32 offset,const STDSTR &sKey,CCT_AES_KEYL type,CCT_AES_MODE mode);
#endif
inline	const	STDSTR&	CCY_Encrypt_AES(STDSTR *retStr,const STDSTR &strData,const STDSTR &sKey,CCT_AES_KEYL type,CCT_AES_MODE mode,G_APPEND blAppend = G_APPEND_OFF){
	return(CCY_Encrypt_AES(retStr,(uint8*)strData.c_str(),strData.length(),sKey,type,mode,blAppend));
};
inline	const	STDSTR&	CCY_Decrypt_AES(STDSTR *retStr,const STDSTR &strData,const STDSTR &sKey,CCT_AES_KEYL type,CCT_AES_MODE mode,G_APPEND blAppend = G_APPEND_OFF){
	return(CCY_Decrypt_AES(retStr,(uint8*)strData.c_str(),strData.length(),sKey,type,mode,blAppend));
};
#endif
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
struct CCY_BASE64_CTX{
	int32			op;
	CCT_BASE64_NL	blEnNL;
	uint8	buf[4];
	uint32	unDoNum;
	int32	in64;
};
		void  CCY_B64_CTXInit	(CCY_BASE64_CTX *ctx,int32 op,CCT_BASE64_NL blEnNL);
const uint64& CCY_EnB64_Update	(CCY_BASE64_CTX *ctx,uint8 *retBuffer,uint64 *retNum,const uint8 *data,uint64 length);
const uint64& CCY_EnB64_Final	(CCY_BASE64_CTX *ctx,uint8 *retBuffer,uint64 *retNum);
const uint64& CCY_DeB64_Update	(CCY_BASE64_CTX *ctx,uint8 *retBuffer,uint64 *retNum,const uint8 *data,uint64 length);
const uint64& CCY_DeB64_Final	(CCY_BASE64_CTX *ctx,uint8 *retBuffer,uint64 *retNum);
//------------------------------------------------------------------------------------------//
class CCY_BASE64{
	public:
				 CCY_BASE64(void){;};
		virtual ~CCY_BASE64(void){;};
	private:
		CCY_BASE64_CTX	ctx;
		uint8			dataOut[1024 * 8];
	public:
		CCY_BASE64& InitEncode	(CCT_BASE64_NL blEnNL = CCT_NL_YES);
		CCY_BASE64& InitDecode	(CCT_BASE64_NL blEnNL = CCT_NL_YES);
	
	#ifdef Comm_FIFOH
		CCY_BASE64& Update		(STDSTR *retStr,const FIFO8 &fifo,uint32 length,uint32 offset);
	#endif
		CCY_BASE64& Update		(STDSTR *retStr,const uint8 *data,uint64 length);
		CCY_BASE64& Final		(STDSTR *retStr);
	
	#ifdef Comm_FIFOH
		CCY_BASE64& Update		(FIFO8 *retFifo,const FIFO8 &fifo,uint32 length,uint32 offset);
		CCY_BASE64& Update		(FIFO8 *retFifo,const uint8 *data,uint32 length);
		CCY_BASE64& Final		(FIFO8 *retFifo);
	#endif
		inline CCY_BASE64& Update	(STDSTR *retStr,const STDSTR &strData){return(Update(retStr,(uint8*)strData.c_str(),strData.length()));};
	
	#ifdef Comm_FIFOH
		inline CCY_BASE64& Update	(FIFO8 *retFifo,const STDSTR &strData){return(Update(retFifo,(uint8*)strData.c_str(),(uint32)strData.length()));};
	#endif
};
const STDSTR& CCY_Encode_BASE64(STDSTR *retStr,const uint8 *data,uint64 length,CCT_BASE64_NL blEnNL = CCT_NL_YES,G_APPEND blAppend = G_APPEND_OFF);
const STDSTR& CCY_Decode_BASE64(STDSTR *retStr,const uint8 *data,uint64 length,CCT_BASE64_NL blEnNL = CCT_NL_YES,G_APPEND blAppend = G_APPEND_OFF);
#ifdef Comm_FIFOH
const STDSTR& CCY_Encode_BASE64(STDSTR *retStr,FIFO8 &fifo,uint32 length,uint32 offset,CCT_BASE64_NL blEnNL = CCT_NL_YES,G_APPEND blAppend = G_APPEND_OFF);
const STDSTR& CCY_Decode_BASE64(STDSTR *retStr,FIFO8 &fifo,uint32 length,uint32 offset,CCT_BASE64_NL blEnNL = CCT_NL_YES,G_APPEND blAppend = G_APPEND_OFF);

const FIFO8& CCY_Encode_BASE64(FIFO8 *retFifo,const uint8 *data,uint32 length,CCT_BASE64_NL blEnNL = CCT_NL_YES);
const FIFO8& CCY_Decode_BASE64(FIFO8 *retFifo,const uint8 *data,uint32 length,CCT_BASE64_NL blEnNL = CCT_NL_YES);
const FIFO8& CCY_Encode_BASE64(FIFO8 *retFifo,const FIFO8 &fifo,uint32 length,uint32 offset,CCT_BASE64_NL blEnNL = CCT_NL_YES);
const FIFO8& CCY_Decode_BASE64(FIFO8 *retFifo,const FIFO8 &fifo,uint32 length,uint32 offset,CCT_BASE64_NL blEnNL = CCT_NL_YES);
#endif
void CCY_Encode_File_BASE64(const STDSTR &outFN,const STDSTR &inFN,CCT_BASE64_NL blEnNL = CCT_NL_YES);
void CCY_Decode_File_BASE64(const STDSTR &outFN,const STDSTR &inFN,CCT_BASE64_NL blEnNL = CCT_NL_YES);
const STDSTR& CCY_Encode_FileToStr_BASE64(STDSTR *retStr,const STDSTR &inFN,CCT_BASE64_NL blEnNL = CCT_NL_YES,G_APPEND blAppend = G_APPEND_OFF);
const STDSTR& CCY_Decode_FileToStr_BASE64(STDSTR *retStr,const STDSTR &inFN,CCT_BASE64_NL blEnNL = CCT_NL_YES,G_APPEND blAppend = G_APPEND_OFF);

inline	const	STDSTR&	CCY_Encode_BASE64(STDSTR *retStr,const STDSTR &strData,CCT_BASE64_NL blEnNL = CCT_NL_YES,G_APPEND blAppend = G_APPEND_OFF){
	return(CCY_Encode_BASE64(retStr,(uint8*)strData.c_str(),strData.length(),blEnNL,blAppend));
};
inline	const	STDSTR&	CCY_Decode_BASE64(STDSTR *retStr,const STDSTR &strData,CCT_BASE64_NL blEnNL = CCT_NL_YES,G_APPEND blAppend = G_APPEND_OFF){
	return(CCY_Decode_BASE64(retStr,(uint8*)strData.c_str(),strData.length(),blEnNL,blAppend));
};
inline			STDSTR		CCY_Encode_BASE64(const STDSTR &data,CCT_BASE64_NL blEnNL = CCT_NL_YES){
	STDSTR	result;
	return(CCY_Encode_BASE64(&result,(uint8*)data.c_str(),data.length(),blEnNL));
};
inline			STDSTR		CCY_Decode_BASE64(const STDSTR &data,CCT_BASE64_NL blEnNL = CCT_NL_YES){
	STDSTR	result;
	return(CCY_Decode_BASE64(&result,(uint8*)data.c_str(),data.length(),blEnNL));
};
//------------------------------------------------------------------------------------------//
#ifdef USE_OPENSSL
//------------------------------------------------------------------------------------------//
STDSTR	CCY_CreateRandKey			(int32 length);
int32	CCY_RSA_CreateKey			(RSA *retRSA	,int bits);
int32	CCY_RSA_WR_PrivateKey_PEM	(RSA *tRSA		,const STDSTR &sFN);
int32	CCY_RSA_RD_PrivateKey_PEM	(RSA **retRSA	,const STDSTR &sFN);
int32	CCY_RSA_WR_PublicKey_PEM	(RSA *tRSA		,const STDSTR &sFN);
int32	CCY_RSA_RD_PublicKey_PEM	(RSA **retRSA	,const STDSTR &sFN);

const STDSTR&	CCY_Encrypt_RSAPrivateKey	(STDSTR *retStr,const uint8 *data,uint64 num,RSA *tRSA);
const STDSTR&	CCY_Decrypt_RSAPrivateKey	(STDSTR *retStr,const uint8 *data,uint64 num,RSA *tRSA);
const STDSTR&	CCY_Encrypt_RSAPublicKey	(STDSTR *retStr,const uint8 *data,uint64 num,RSA *tRSA);
const STDSTR&	CCY_Decrypt_RSAPublicKey	(STDSTR *retStr,const uint8 *data,uint64 num,RSA *tRSA);
//------------------------------------------------------------------------------------------//
inline const STDSTR&	CCY_Encrypt_RSAPrivateKey	(STDSTR *retStr,const STDSTR &strData,RSA *tRSA){
	return(CCY_Encrypt_RSAPrivateKey(retStr,(uint8*)strData.c_str(),strData.length(),tRSA));
};
inline const STDSTR&	CCY_Decrypt_RSAPrivateKey	(STDSTR *retStr,const STDSTR &strData,RSA *tRSA){
	return(CCY_Decrypt_RSAPrivateKey(retStr,(uint8*)strData.c_str(),strData.length(),tRSA));
};
inline const STDSTR&	CCY_Encrypt_RSAPublicKey	(STDSTR *retStr,const STDSTR &strData,RSA *tRSA){
	return(CCY_Encrypt_RSAPublicKey(retStr,(uint8*)strData.c_str(),strData.length(),tRSA));
};
inline const STDSTR&	CCY_Decrypt_RSAPublicKey	(STDSTR *retStr,const STDSTR &strData,RSA *tRSA){
	return(CCY_Decrypt_RSAPublicKey(retStr,(uint8*)strData.c_str(),strData.length(),tRSA));
};

const STDSTR&	CCY_Encode_RSAPrivateKey	(STDSTR *retStr,const RSA *tRSA);
const STDSTR&	CCY_Encode_RSAPublicKey		(STDSTR *retStr,const RSA *tRSA);
inline int32	CCY_Decode_RSAPrivateKey	(RSA **retRSA,const uint8 *data,uint64 num)	{return(d2i_RSAPrivateKey(retRSA,&data,(uint32)num) != nullptr);};
inline int32	CCY_Decode_RSAPublicKey		(RSA **retRSA,const uint8 *data,uint64 num)	{return(d2i_RSAPublicKey(retRSA,&data,(uint32)num) != nullptr);};
inline int32	CCY_Decode_RSAPrivateKey	(RSA **retRSA,const STDSTR &strData)		{return(CCY_Decode_RSAPrivateKey(retRSA,(uint8*)strData.c_str(),strData.length()));};
inline int32	CCY_Decode_RSAPublicKey		(RSA **retRSA,const STDSTR &strData)		{return(CCY_Decode_RSAPublicKey(retRSA,(uint8*)strData.c_str(),strData.length()));};
//------------------------------------------------------------------------------------------//
#endif
#ifdef Comm_PAH
#if (defined USE_OPENSSL) || (defined CommonDefH_MAC)
//------------------------------------------------------------------------------------------//
class CCY_FN_Hash :public PROTOCOL_NODE{
	public:
				 CCY_FN_Hash(void) : PROTOCOL_NODE(){Add(pnlc_Substance) < pnlc_Hash;cgDigestType = CCT_MD5;};
		virtual	~CCY_FN_Hash(void){;};
	private:
		CCT_Digest 	cgDigestType;
		PNODE_LC	pnlc_Substance;
		PNODE_LC	pnlc_Hash;
	public:
		void	Init(const FIFO8 *tfifo,CCT_Digest digestType = CCT_MD5,G_ENDIAN tEV = G_ENDIAN_LITTLE){
			PROTOCOL_NODE::Init	(FNT_MESSAGE,tfifo,0,tEV);
			pnlc_Substance.Init	(tfifo,4,tEV);
			pnlc_Hash.Init		(tfifo,1,tEV);

			cgDigestType = digestType;
		};
	public:
		virtual 		int32			ChecksumResult	(const FIFO8 &fifo)const{
			STDSTR	strHash;
			if (pnlc_Hash.ReadContent(&strHash,&fifo) == CCY_Digest(cgDigestType,fifo,pnlc_Substance.GetContentLength(),pnlc_Substance.GetContentOffset()))
				return 1;
			return 0;
		};
		inline			int32			ReadContent	(STDSTR *retStrSubstance,const FIFO8 *fifo = nullptr)const{
			*retStrSubstance = "";
			if (fifo == nullptr)
				fifo = GetDefFifo();
			if (ChecksumResult(*fifo) > 0){
				pnlc_Substance.ReadContent(retStrSubstance,fifo);
				return 1;
			}
			return 0;
		};
		inline			int32			ReadContent	(FIFO8 *retfifoSubstance,const FIFO8 *fifo = nullptr)const{
			if (fifo == nullptr)
				fifo = GetDefFifo();
			if (ChecksumResult(*fifo) > 0){
				pnlc_Substance.ReadContent(retfifoSubstance,fifo);
				return 1;
			}
			return 0;
		};
		inline	const	CCY_FN_Hash&	SetContent	(const FIFO8 &fifoIn,uint32 num,uint32 offset = 0){
			PROTOCOL_NODE::_Start();
			pnlc_Substance.SetContent(fifoIn,num,offset);
			pnlc_Hash = CCY_Digest(cgDigestType,fifoIn,num,offset);
			PROTOCOL_NODE::_Endl();
			return(*this);
		};
		inline	const	CCY_FN_Hash&	SetContent	(const uint8 *data,uint32 num){
			PROTOCOL_NODE::_Start();
			pnlc_Substance.SetContent(data,num);
			pnlc_Hash = CCY_Digest(cgDigestType,data,num);
			PROTOCOL_NODE::_Endl();
			return(*this);
		};
		inline	const	CCY_FN_Hash&	SetContent	(const STDSTR &strIn){
			return(CCY_FN_Hash::SetContent((uint8*)strIn.c_str(),(uint32)strIn.length()));
		};
		inline	const	CCY_FN_Hash&	SetContent	(const PROTOCOL_NODE &pnIn){
			PROTOCOL_NODE::_Start();
			pnlc_Substance.SetContent(pnIn);
			pnlc_Hash = CCY_Digest(cgDigestType,*pnIn.GetDefFifo(),pnIn.GetLength(),pnIn.GetOffset());
			PROTOCOL_NODE::_Endl();
			return(*this);
		};

		inline			void			HoldOffset	(void){
			PROTOCOL_NODE::_Start();
			pnlc_Substance._Start();
		};
		inline			void			UpdateLength(void){
			pnlc_Substance._Endl();
			pnlc_Hash = CCY_Digest(cgDigestType,*GetDefFifo(),pnlc_Substance.GetContentLength(),pnlc_Substance.GetContentOffset());
			PROTOCOL_NODE::_Endl();
		};
};
//------------------------------------------------------------------------------------------//
class CCY_FNLC_AES :public PNODE_LC{
	public:
				 CCY_FNLC_AES(void) : PNODE_LC(){cgAES_type = CCT_AES128;cgAES_mode = CCT_AES_CBC;};
		virtual	~CCY_FNLC_AES(void){;};
	private:
		CCT_AES_KEYL	cgAES_type;
		CCT_AES_MODE	cgAES_mode;
	public:
		inline  uint32	GetInvalidLength(void)const{return(4);};
		void	Init(const FIFO8 *tfifo,CCT_AES_KEYL type = CCT_AES128,CCT_AES_MODE mode = CCT_AES_CBC,G_ENDIAN tEV = G_ENDIAN_LITTLE){
			PNODE_LC::Init(tfifo,4,tEV);
			cgAES_type = type;
			cgAES_mode = mode;
		};
		void	SetConfig(CCT_AES_KEYL type = CCT_AES128,CCT_AES_MODE mode = CCT_AES_CBC,G_ENDIAN tEV = G_ENDIAN_LITTLE){
			PNODE_LC::SetEndianType(tEV);
			cgAES_type = type;
			cgAES_mode = mode;
		}
	public:
		inline	const STDSTR	&ReadContent(STDSTR *retStrOriginal,const STDSTR &strKey,const FIFO8 *fifo = nullptr)const{
			if (strKey.length() == 0)
				return(PNODE_LC::ReadContent(retStrOriginal,fifo));
			if (fifo == nullptr)
				fifo = GetDefFifo();
			return(CCY_Decrypt_AES(retStrOriginal, *fifo, GetContentLength(), GetContentOffset() , strKey, cgAES_type, cgAES_mode));
		};
		inline	const FIFO8	&ReadContent(FIFO8 *retfifoOriginal,const STDSTR &strKey,const FIFO8 *fifo = nullptr)const{
			if (strKey.length() == 0)
				return(PNODE_LC::ReadContent(retfifoOriginal,fifo));
			if (fifo == nullptr)
				fifo = GetDefFifo();
			return(CCY_Decrypt_AES(retfifoOriginal, *fifo, GetContentLength(), GetContentOffset(), strKey, cgAES_type, cgAES_mode));
		};
		inline	const CCY_FNLC_AES	&SetContent	(const FIFO8 &fifoIn,uint32 num,uint32 offset,const STDSTR &strKey){
			if (strKey.length() == 0){
				PNODE_LC::SetContent(fifoIn, num, offset);
			}
			else{
				PNODE_LC::_Start();
				CCY_Encrypt_AES(GetDefFifo(), fifoIn, num, offset, strKey, cgAES_type, cgAES_mode);
				PNODE_LC::_Endl();
			}
			return(*this);
		};
		inline	const CCY_FNLC_AES	&SetContent	(const uint8 *data,uint32 num,const STDSTR &strKey){
			if (strKey.length() == 0){
				PNODE_LC::SetContent(data, num);
			}
			else{
				PNODE_LC::_Start();
				CCY_Encrypt_AES(GetDefFifo(), data, num, strKey, cgAES_type, cgAES_mode);
				PNODE_LC::_Endl();
			}
			return(*this);
		};
		inline	const CCY_FNLC_AES	&SetContent	(const STDSTR &strIn,const STDSTR &strKey){
			return(CCY_FNLC_AES::SetContent((uint8*)strIn.c_str(), (uint32)strIn.length(), strKey));
		};
		inline	const CCY_FNLC_AES	&SetContent	(const PROTOCOL_NODE &pnIn,const STDSTR &strKey){
			if (strKey.length() == 0){
				PNODE_LC::SetContent(pnIn);
			}
			else{
				PNODE_LC::_Start();
				CCY_Encrypt_AES(GetDefFifo(), *pnIn.GetDefFifo(), pnIn.GetLength(),pnIn.GetOffset(), strKey, cgAES_type, cgAES_mode);
				PNODE_LC::_Endl();
			}
			return(*this);
		};
};
//------------------------------------------------------------------------------------------//
class CCY_FN_AES :public PROTOCOL_NODE{
	public:
				 CCY_FN_AES(void) :PROTOCOL_NODE(){Add(pnlc_Hash) < pnlc_AES;cgDigestType = CCT_MD5;};
		virtual	~CCY_FN_AES(void){;};
	private:
		CCT_Digest		cgDigestType;
	
		PNODE_LC		pnlc_Hash;
		CCY_FNLC_AES	pnlc_AES;
	public:
		inline	uint32	GetInvalidLength(void)const{return(1 + CCY_DigestSize(cgDigestType) + pnlc_AES.GetInvalidLength());};
		void	Init(const FIFO8 *tfifo,CCT_AES_KEYL type = CCT_AES128,CCT_AES_MODE mode = CCT_AES_CBC
					 ,CCT_Digest digestType = CCT_MD5,G_ENDIAN tEV = G_ENDIAN_LITTLE){
			PROTOCOL_NODE::Init(FNT_MESSAGE,tfifo,0,tEV);
			pnlc_Hash.Init	(tfifo,1,tEV);
			pnlc_AES.Init	(tfifo,type,mode,tEV);
			cgDigestType = digestType;
		};
		void	SetConfig(CCT_AES_KEYL type = CCT_AES128,CCT_AES_MODE mode = CCT_AES_CBC
						  ,CCT_Digest digestType = CCT_MD5,G_ENDIAN tEV = G_ENDIAN_LITTLE){
			PROTOCOL_NODE::SetEndianType(tEV);
			pnlc_Hash.SetEndianType(tEV);
			pnlc_AES.SetConfig(type,mode,tEV);
			cgDigestType = digestType;
		}
	public:
		inline	int32	CheckContent(STDSTR *retStrOriginal,const STDSTR &strKey,const FIFO8 *fifo = nullptr)const{
			STDSTR	strHash;
			*retStrOriginal = "";
			if (pnlc_Hash.ReadContent(&strHash,fifo) == CCY_Digest(cgDigestType,pnlc_AES.ReadContent(retStrOriginal,strKey,fifo)))
				return 1;
			return 0;
		}
		inline	int32	ReadContent	(STDSTR *retStrOriginal,const STDSTR &strKey,const FIFO8 *fifo = nullptr)const{
			return(CheckContent(retStrOriginal,strKey,fifo));
		};
		inline	int32	ReadContent	(FIFO8 *retfifoOriginal,const STDSTR &strKey,const FIFO8 *fifo = nullptr)const{
			STDSTR	retStrOriginal;
			if (CheckContent(&retStrOriginal,strKey,fifo) > 0){
				retfifoOriginal->Put(retStrOriginal, G_ESCAPE_OFF);
				return 1;
			}
			return 0;
		};
		inline	const CCY_FN_AES&	SetContent	(const FIFO8 &fifoIn,uint32 num,uint32 offset,const STDSTR &strKey){
			PROTOCOL_NODE::_Start();
			pnlc_Hash = CCY_Digest(cgDigestType, fifoIn, num, offset);
			pnlc_AES.SetContent(fifoIn, num, offset, strKey);
			PROTOCOL_NODE::_Endl();
			return(*this);
		};
		inline	const CCY_FN_AES&	SetContent	(const uint8 *data,uint32 num,const STDSTR &strKey){
			PROTOCOL_NODE::_Start();
			pnlc_Hash = CCY_Digest(cgDigestType, data, num);
			pnlc_AES.SetContent(data, num, strKey);
			PROTOCOL_NODE::_Endl();
			return(*this);
		};
		inline	const CCY_FN_AES&	SetContent	(const STDSTR &strIn,const STDSTR &strKey){
			return(CCY_FN_AES::SetContent((uint8*)strIn.c_str(),(uint32)strIn.length(),strKey));
		};
		inline	const CCY_FN_AES&	SetContent	(const PROTOCOL_NODE &pnIn,const STDSTR &strKey){
			PROTOCOL_NODE::_Start();
			pnlc_Hash = CCY_Digest(cgDigestType,*pnIn.GetDefFifo(),pnIn.GetLength(),pnIn.GetOffset());
			pnlc_AES.SetContent(pnIn, strKey);
			PROTOCOL_NODE::_Endl();
			return(*this);
		};
};
//------------------------------------------------------------------------------------------//
class CCY_FN_AES_MK :public PNODE_LC{
	public:
				 CCY_FN_AES_MK(void) :PNODE_LC(){;};
		virtual	~CCY_FN_AES_MK(void){;};
	private:
		CCY_FN_AES		pn_AES;
	public:
		void	Init(const FIFO8 *tfifo,CCT_AES_KEYL type = CCT_AES128,CCT_AES_MODE mode = CCT_AES_CBC
					 ,CCT_Digest digestType = CCT_MD5,G_ENDIAN tEV = G_ENDIAN_LITTLE){
			PNODE_LC::Init	(tfifo,4,tEV);
			pn_AES.Init		(tfifo,type,mode,digestType,tEV);
		};
	public:
				int32	ReadContent	(STDSTR *retStr,const STDSTR &strMulitKey,const FIFO8 *fifo = nullptr);
				const CCY_FN_AES_MK&	SetContent	(const FIFO8 &fifoIn,uint32 num,uint32 offset,STDSTR strMulitKey);
				const CCY_FN_AES_MK&	SetContent	(const uint8 *data,uint32 num,STDSTR strMulitKey);
		inline	const CCY_FN_AES_MK&	SetContent	(const STDSTR &strIn,const STDSTR &strMulitKey){
			return(CCY_FN_AES_MK::SetContent((uint8*)strIn.c_str(),(uint32)strIn.length(),strMulitKey));
		};
		inline	const CCY_FN_AES_MK&	SetContent	(const PROTOCOL_NODE &pnIn,const STDSTR &strMulitKey){
			return(CCY_FN_AES_MK::SetContent(*pnIn.GetDefFifo(),pnIn.GetLength(),pnIn.GetOffset(),strMulitKey));
		};
};
//------------------------------------------------------------------------------------------//
#endif
#ifdef USE_OPENSSL
//------------------------------------------------------------------------------------------//
class CCY_FR_Signature :public PROTOCOL_NODE{
	public:
				 CCY_FR_Signature(void) :PROTOCOL_NODE(){Add(pnlc_sKey) < pnlc_signature < fnlcAES_packet;};
		virtual	~CCY_FR_Signature(void){;};
	public:
		void	Init(uint32 fifoSize = 1024 * 8,CCT_AES_KEYL type = CCT_AES128,CCT_AES_MODE mode = CCT_AES_CBC
					 ,CCT_Digest digestType = CCT_MD5,G_ENDIAN tEV = G_ENDIAN_LITTLE);
		FIFO8	cgFIFO_Result;
	private:
		CCT_Digest 		cgDigestType;
		PNODE_LC		pnlc_sKey;
		PNODE_LC		pnlc_signature;
		CCY_FNLC_AES	fnlcAES_packet;
	public:
			const CCY_FR_Signature&	Encode(const FIFO8 &fifoIn,uint32 num,uint32 offset,RSA *rsa_puk);
			const CCY_FR_Signature&	Encode(const uint8 *data,uint32 num,RSA *rsa_puk);
	inline	const CCY_FR_Signature&	Encode(const STDSTR &strIn,RSA *rsa_puk){
		return(CCY_FR_Signature::Encode((uint8*)strIn.c_str(),(uint32)strIn.length(),rsa_puk));
	};
	inline	const CCY_FR_Signature&	Encode(const PROTOCOL_NODE &pnIn,RSA *rsa_puk){
		return(CCY_FR_Signature::Encode(*pnIn.GetDefFifo(),pnIn.GetLength(),pnIn.GetOffset(),rsa_puk));
	};
	
	inline	const STDSTR&	Encode(STDSTR *retStrWhole,const uint8 *data,uint32 num,RSA *rsa_puk){
		CCY_FR_Signature::Encode(data,num,rsa_puk);
		return(ReadAllContent(retStrWhole));
	};
	inline	const STDSTR&	Encode(STDSTR *retStrWhole,const STDSTR &strIn,RSA *rsa_puk){
		CCY_FR_Signature::Encode((uint8*)strIn.c_str(),(uint32)strIn.length(),rsa_puk);
		return(ReadAllContent(retStrWhole));
	};
			int32	Decode(STDSTR *retStrOriginal,const uint8 *data,uint32 num,RSA *rsa_prk);
			int32	Decode(STDSTR *retStrOriginal,const FIFO8 &fifoIn,RSA *rsa_prk);
	inline	int32	Decode(STDSTR *retStrOriginal,const STDSTR &strIn,RSA *rsa_prk){
		return(CCY_FR_Signature::Decode(retStrOriginal,(uint8*)strIn.c_str(),(uint32)strIn.length(),rsa_prk));
	};
};
//------------------------------------------------------------------------------------------//
#endif
#endif
//------------------------------------------------------------------------------------------//
#endif
//------------------------------------------------------------------------------------------//
#endif
