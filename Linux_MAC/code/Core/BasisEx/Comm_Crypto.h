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
//------------------------------------------------------------------------------------------//
enum CCT_Digest		{CCT_MD5 = 0,CCT_SHA1,CCT_SHA224,CCT_SHA256,CCT_SHA384,CCT_SHA512};
enum CCT_AES_KEYL	{CCT_AES128 = 0,CCT_AES192,CCT_AES256};
enum CCT_AES_MODE	{CCT_AES_CBC = 0,CCT_AES_CFB8};
enum CCT_BASE64_NL	{CCT_NL_NO = 0,CCT_NL_YES = 1};
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_MAC
#include <CommonCrypto/CommonDigest.h>
#include <CommonCrypto/CommonCryptor.h>
#else
#define CC_LONG					uint32

#define CC_SHA1_DIGEST_LENGTH	SHA_DIGEST_LENGTH
#define CC_SHA1_CTX				SHA_CTX
#define	CC_SHA1_Init			SHA1_Init
#define CC_SHA1_Update			SHA1_Update
#define CC_SHA1_Final			SHA1_Final
#define CC_SHA1					SHA1

#define CC_SHA224_DIGEST_LENGTH	SHA224_DIGEST_LENGTH
//#define CC_SHA256_CTX			SHA256_CTX
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
//#define CC_SHA512_CTX			SHA512_CTX
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
//------------------------------------------------------------------------------------------//
long double CCY_erf					(long double x,uint32 maxN);
long double CCY_Gaussian			(long double x,long double u,long double b);
//------------------------------------------------------------------------------------------//
#if (defined USE_OPENSSL) || (defined CommonDefH_MAC)
uint32		CCY_DigestSize			(CCT_Digest type);
std::string	CCY_Digest				(CCT_Digest type,const uint8 *data,uint64 length);
std::string	CCY_Digest_SHA1			(const uint8 *data,uint64 length);
std::string	CCY_Digest_SHA224		(const uint8 *data,uint64 length);
std::string	CCY_Digest_SHA256		(const uint8 *data,uint64 length);
std::string	CCY_Digest_SHA384		(const uint8 *data,uint64 length);
std::string	CCY_Digest_SHA512		(const uint8 *data,uint64 length);
std::string	CCY_Digest_MD5			(const uint8 *data,uint64 length);

std::string CCY_Digest				(CCT_Digest type,const FIFO_UINT8 &fifo,uint32 length = -1,uint32 offset = 0);
std::string CCY_Digest_SHA1			(const FIFO_UINT8 &fifo,uint32 length = -1,uint32 offset = 0);
std::string CCY_Digest_SHA224		(const FIFO_UINT8 &fifo,uint32 length = -1,uint32 offset = 0);
std::string CCY_Digest_SHA256		(const FIFO_UINT8 &fifo,uint32 length = -1,uint32 offset = 0);
std::string CCY_Digest_SHA384		(const FIFO_UINT8 &fifo,uint32 length = -1,uint32 offset = 0);
std::string CCY_Digest_SHA512		(const FIFO_UINT8 &fifo,uint32 length = -1,uint32 offset = 0);
std::string CCY_Digest_MD5			(const FIFO_UINT8 &fifo,uint32 length = -1,uint32 offset = 0);

std::string	CC_DigestFile			(CCT_Digest type,const std::string &sFN,uint64 length = -1,uint64 offset = 0);
std::string	CCY_DigestFile_SHA1		(const std::string &sFN,uint64 length = -1,uint64 offset = 0);
std::string	CCY_DigestFile_SHA224	(const std::string &sFN,uint64 length = -1,uint64 offset = 0);
std::string	CCY_DigestFile_SHA256	(const std::string &sFN,uint64 length = -1,uint64 offset = 0);
std::string	CCY_DigestFile_SHA384	(const std::string &sFN,uint64 length = -1,uint64 offset = 0);
std::string	CCY_DigestFile_SHA512	(const std::string &sFN,uint64 length = -1,uint64 offset = 0);
std::string	CCY_DigestFile_MD5		(const std::string &sFN,uint64 length = -1,uint64 offset = 0);

inline	std::string	CCY_Digest			(CCT_Digest type,const std::string &strData){return(CCY_Digest(type,(uint8*)strData.c_str(),strData.length()));};
inline	std::string	CCY_Digest_SHA1		(const std::string &strData)				{return(CCY_Digest_SHA1((uint8*)strData.c_str(),strData.length()));};
inline	std::string	CCY_Digest_SHA224	(const std::string &strData)				{return(CCY_Digest_SHA224((uint8*)strData.c_str(),strData.length()));};
inline	std::string	CCY_Digest_SHA256	(const std::string &strData)				{return(CCY_Digest_SHA256((uint8*)strData.c_str(),strData.length()));};
inline	std::string	CCY_Digest_SHA384	(const std::string &strData)				{return(CCY_Digest_SHA384((uint8*)strData.c_str(),strData.length()));};
inline	std::string	CCY_Digest_SHA512	(const std::string &strData)				{return(CCY_Digest_SHA512((uint8*)strData.c_str(),strData.length()));};
inline	std::string	CCY_Digest_MD5		(const std::string &strData)				{return(CCY_Digest_MD5((uint8*)strData.c_str(),strData.length()));};
//------------------------------------------------------------------------------------------//
inline			std::string		CCY_AESKey32Bye(const std::string &sKey)	{return(CCY_Digest_SHA256(sKey));}
const std::string&	CCY_Encrypt_AES(std::string *retStr,const uint8 *data,uint64 length,const std::string &sKey,CCT_AES_KEYL type,CCT_AES_MODE mode);
const std::string&	CCY_Decrypt_AES(std::string *retStr,const uint8 *data,uint64 length,const std::string &sKey,CCT_AES_KEYL type,CCT_AES_MODE mode);
const std::string&	CCY_Encrypt_AES(std::string *retStr,const FIFO_UINT8 &fifo,uint32 length,uint32 offset,const std::string &sKey,CCT_AES_KEYL type,CCT_AES_MODE mode);
const std::string&	CCY_Decrypt_AES(std::string *retStr,const FIFO_UINT8 &fifo,uint32 length,uint32 offset,const std::string &sKey,CCT_AES_KEYL type,CCT_AES_MODE mode);

const FIFO_UINT8&	CCY_Encrypt_AES(FIFO_UINT8 *retfifo,const uint8 *data,uint32 length,const std::string &sKey,CCT_AES_KEYL type,CCT_AES_MODE mode);
const FIFO_UINT8&	CCY_Decrypt_AES(FIFO_UINT8 *retfifo,const uint8 *data,uint32 length,const std::string &sKey,CCT_AES_KEYL type,CCT_AES_MODE mode);
const FIFO_UINT8&	CCY_Encrypt_AES(FIFO_UINT8 *retfifo,const FIFO_UINT8 &fifo,uint32 length,uint32 offset,const std::string &sKey,CCT_AES_KEYL type,CCT_AES_MODE mode);
const FIFO_UINT8&	CCY_Decrypt_AES(FIFO_UINT8 *retfifo,const FIFO_UINT8 &fifo,uint32 length,uint32 offset,const std::string &sKey,CCT_AES_KEYL type,CCT_AES_MODE mode);

inline	const	std::string&	CCY_Encrypt_AES(std::string *retStr,const std::string &strData,const std::string &sKey,CCT_AES_KEYL type,CCT_AES_MODE mode){
	return(CCY_Encrypt_AES(retStr,(uint8*)strData.c_str(),strData.length(),sKey,type,mode));
};
inline	const	std::string&	CCY_Decrypt_AES(std::string *retStr,const std::string &strData,const std::string &sKey,CCT_AES_KEYL type,CCT_AES_MODE mode){
	return(CCY_Decrypt_AES(retStr,(uint8*)strData.c_str(),strData.length(),sKey,type,mode));
};
#endif
//------------------------------------------------------------------------------------------//
const std::string&	CCY_Encode_BASE64(std::string *retStr,const uint8 *data,uint64 length,CCT_BASE64_NL blEnNL = CCT_NL_YES);
const std::string&	CCY_Decode_BASE64(std::string *retStr,const uint8 *data,uint64 length,CCT_BASE64_NL blEnNL = CCT_NL_YES);
const std::string&	CCY_Encode_BASE64(std::string *retStr,FIFO_UINT8 &fifo,uint32 length,uint32 offset,CCT_BASE64_NL blEnNL = CCT_NL_YES);
const std::string&	CCY_Decode_BASE64(std::string *retStr,FIFO_UINT8 &fifo,uint32 length,uint32 offset,CCT_BASE64_NL blEnNL = CCT_NL_YES);

const FIFO_UINT8&	CCY_Encode_BASE64(FIFO_UINT8 *retfifo,const uint8 *data,uint32 length,CCT_BASE64_NL blEnNL = CCT_NL_YES);
const FIFO_UINT8&	CCY_Decode_BASE64(FIFO_UINT8 *retfifo,const uint8 *data,uint32 length,CCT_BASE64_NL blEnNL = CCT_NL_YES);
const FIFO_UINT8&	CCY_Encode_BASE64(FIFO_UINT8 *retfifo,const FIFO_UINT8 &fifo,uint32 length,uint32 offset,CCT_BASE64_NL blEnNL = CCT_NL_YES);
const FIFO_UINT8&	CCY_Decode_BASE64(FIFO_UINT8 *retfifo,const FIFO_UINT8 &fifo,uint32 length,uint32 offset,CCT_BASE64_NL blEnNL = CCT_NL_YES);
void CCY_Encode_File_BASE64(const std::string &outFN,const std::string &inFN,CCT_BASE64_NL blEnNL);
void CCY_Decode_File_BASE64(const std::string &outFN,const std::string &inFN,CCT_BASE64_NL blEnNL);
inline	const	std::string&	CCY_Encode_BASE64(std::string *retStr,const std::string &strData,CCT_BASE64_NL blEnNL = CCT_NL_YES){
	return(CCY_Encode_BASE64(retStr,(uint8*)strData.c_str(),strData.length(),blEnNL));
};
inline	const	std::string&	CCY_Decode_BASE64(std::string *retStr,const std::string &strData,CCT_BASE64_NL blEnNL = CCT_NL_YES){
	return(CCY_Decode_BASE64(retStr,(uint8*)strData.c_str(),strData.length(),blEnNL));
};
inline			std::string		CCY_Encode_BASE64(const std::string &data,CCT_BASE64_NL blEnNL = CCT_NL_YES){
	std::string	result;
	return(CCY_Encode_BASE64(&result,(uint8*)data.c_str(),data.length(),blEnNL));
};
inline			std::string		CCY_Decode_BASE64(const std::string &data,CCT_BASE64_NL blEnNL = CCT_NL_YES){
	std::string	result;
	return(CCY_Decode_BASE64(&result,(uint8*)data.c_str(),data.length(),blEnNL));
};
//------------------------------------------------------------------------------------------//
struct CCY_BASE64_CTX{
	uint8	buf[4];
	uint32	unDoNum;
	int32	in64;
};
void CCY_B64_CTXInit(CCY_BASE64_CTX *ctx);
const uint64& CCY_EnB64_Update	(CCY_BASE64_CTX *ctx,uint8 *retBuffer,uint64 *retNum,const uint8 *data,uint64 length,CCT_BASE64_NL blEnNL);
const uint64& CCY_EnB64_Final	(CCY_BASE64_CTX *ctx,uint8 *retBuffer,uint64 *retNum,CCT_BASE64_NL blEnNL);
const uint64& CCY_DeB64_Update	(CCY_BASE64_CTX *ctx,uint8 *retBuffer,uint64 *retNum,const uint8 *data,uint64 length,CCT_BASE64_NL blEnNL);
const uint64& CCY_DeB64_Final	(CCY_BASE64_CTX *ctx,uint8 *retBuffer,uint64 *retNum,CCT_BASE64_NL blEnNL);
//------------------------------------------------------------------------------------------//
#ifdef USE_OPENSSL
#include <openssl/crypto.h>
#include <openssl/md5.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#define	CY_Init()	(OpenSSL_add_all_algorithms())
//------------------------------------------------------------------------------------------//
std::string	CCY_CreateRandKey		(int32 length);
int32	CCY_RSA_CreateKey			(RSA *retRSA	,int bits);
int32	CCY_RSA_WR_PrivateKey_PEM	(RSA *tRSA		,const std::string &sFN);
int32	CCY_RSA_RD_PrivateKey_PEM	(RSA **retRSA	,const std::string &sFN);
int32	CCY_RSA_WR_PublicKey_PEM	(RSA *tRSA		,const std::string &sFN);
int32	CCY_RSA_RD_PublicKey_PEM	(RSA **retRSA	,const std::string &sFN);

const std::string&	CCY_Encrypt_RSAPrivateKey	(std::string *retStr,const uint8 *data,uint64 num,RSA *tRSA);
const std::string&	CCY_Decrypt_RSAPrivateKey	(std::string *retStr,const uint8 *data,uint64 num,RSA *tRSA);
const std::string&	CCY_Encrypt_RSAPublicKey	(std::string *retStr,const uint8 *data,uint64 num,RSA *tRSA);
const std::string&	CCY_Decrypt_RSAPublicKey	(std::string *retStr,const uint8 *data,uint64 num,RSA *tRSA);

inline const std::string&	CCY_Encrypt_RSAPrivateKey	(std::string *retStr,const std::string &strData,RSA *tRSA){
	return(CCY_Encrypt_RSAPrivateKey(retStr,(uint8*)strData.c_str(),strData.length(),tRSA));
};
inline const std::string&	CCY_Decrypt_RSAPrivateKey	(std::string *retStr,const std::string &strData,RSA *tRSA){
	return(CCY_Decrypt_RSAPrivateKey(retStr,(uint8*)strData.c_str(),strData.length(),tRSA));
};
inline const std::string&	CCY_Encrypt_RSAPublicKey	(std::string *retStr,const std::string &strData,RSA *tRSA){
	return(CCY_Encrypt_RSAPublicKey(retStr,(uint8*)strData.c_str(),strData.length(),tRSA));
};
inline const std::string&	CCY_Decrypt_RSAPublicKey	(std::string *retStr,const std::string &strData,RSA *tRSA){
	return(CCY_Decrypt_RSAPublicKey(retStr,(uint8*)strData.c_str(),strData.length(),tRSA));
};

const std::string&	CCY_Encode_RSAPrivateKey	(std::string *retStr,const RSA *tRSA);
const std::string&	CCY_Encode_RSAPublicKey		(std::string *retStr,const RSA *tRSA);
inline int32		CCY_Decode_RSAPrivateKey	(RSA **retRSA,const uint8 *data,uint64 num){return(d2i_RSAPrivateKey(retRSA,&data,(uint32)num) != nullptr);};
inline int32		CCY_Decode_RSAPublicKey		(RSA **retRSA,const uint8 *data,uint64 num){return(d2i_RSAPublicKey(retRSA,&data,(uint32)num) != nullptr);};
inline int32		CCY_Decode_RSAPrivateKey	(RSA **retRSA,const std::string &strData){
	return(CCY_Decode_RSAPrivateKey(retRSA,(uint8*)strData.c_str(),strData.length()));
};
inline int32		CCY_Decode_RSAPublicKey		(RSA **retRSA,const std::string &strData){
	return(CCY_Decode_RSAPublicKey(retRSA,(uint8*)strData.c_str(),strData.length()));
};
//------------------------------------------------------------------------------------------//
#endif
#if (defined USE_OPENSSL) || (defined CommonDefH_MAC)
//------------------------------------------------------------------------------------------//
class CCY_FNLC_Hash :public FNode_LC{
	public:
				 CCY_FNLC_Hash(void) : FNode_LC(){AddSubNode(&fnlc_Hash);AddSubNode(&fnlc_Substance);cgDigestType = CCT_MD5;};
		virtual	~CCY_FNLC_Hash(void){;};
	private:
		CCT_Digest 	cgDigestType;
		FNode_LC	fnlc_Hash;
		FNode_LC	fnlc_Substance;
	public:
		void	Init(const FIFO_UINT8 *tfifo,CCT_Digest digestType = CCT_MD5,G_Endian_VAILD tEV = G_LITTLE_ENDIAN){
			FNode_LC::Init		(tfifo,4,tEV);
			fnlc_Hash.Init		(tfifo,1,tEV);
			fnlc_Substance.Init	(tfifo,4,tEV);

			cgDigestType = digestType;
		};
	public:
		virtual 		int32			ChecksumResult	(const FIFO_UINT8 &fifobuf){
			std::string	strHash;
			if (fnlc_Hash.ReadContent(&strHash,&fifobuf) == CCY_Digest(cgDigestType,fifobuf,fnlc_Substance.GetContentLength(),fnlc_Substance.GetContentOffset()))
				return 1;
			return 0;
		};
		inline			int32			ReadContent	(std::string *retStrSubstance,const FIFO_UINT8 *fifobuf = nullptr){
			*retStrSubstance = "";
			if (fifobuf == nullptr)
				fifobuf = GetcgDefFifo(this);
			if (ChecksumResult(*fifobuf) > 0){
				fnlc_Substance.ReadContent(retStrSubstance,fifobuf);
				return 1;
			}
			return 0;
		};
		inline			int32			ReadContent	(FIFO_UINT8 *retfifoSubstance,const FIFO_UINT8 *fifobuf = nullptr){
			if (fifobuf == nullptr)
				fifobuf = GetcgDefFifo(this);
			if (ChecksumResult(*fifobuf) > 0){
				fnlc_Substance.ReadContent(retfifoSubstance,fifobuf);
				return 1;
			}
			return 0;
		};
		inline	const	CCY_FNLC_Hash&	SetContent	(const FIFO_UINT8 &fifoIn,uint32 num,uint32 offset = 0){
			FNode_LC::HoldOffset();
			fnlc_Hash = CCY_Digest(cgDigestType,fifoIn,num,offset);
			fnlc_Substance.SetContent(fifoIn,num,offset);
			FNode_LC::UpdateLength();
			return(*this);
		};
		inline	const	CCY_FNLC_Hash&	SetContent	(const uint8 *data,uint32 num){
			FNode_LC::HoldOffset();
			fnlc_Hash = CCY_Digest(cgDigestType,data,num);
			fnlc_Substance.SetContent(data,num);
			FNode_LC::UpdateLength();
			return(*this);
		};
		inline	const	CCY_FNLC_Hash&	SetContent	(const std::string &strInput){
			return(CCY_FNLC_Hash::SetContent((uint8*)strInput.c_str(),(uint32)strInput.length()));
		};
		inline	const	CCY_FNLC_Hash&	SetContent	(const Field_Node &fnInput){
			FNode_LC::HoldOffset();
			fnlc_Hash = CCY_Digest(cgDigestType,*GetcgDefFifo(&fnInput),fnInput.GetLength(),fnInput.GetOffset());
			fnlc_Substance.SetContent(fnInput);
			FNode_LC::UpdateLength();
			return(*this);
		};

		inline			void			HoldOffset	(void){
			FNode_LC::HoldOffset();
			fnlc_Hash = CCY_Digest(cgDigestType,"usedforhold");
			fnlc_Substance.HoldOffset();
		};
		inline			void			UpdateLength(void){
			std::string strTemp;
			fnlc_Substance.UpdateLength();
			strTemp = CCY_Digest(cgDigestType,*GetcgDefFifo(this),fnlc_Substance.GetContentLength(),fnlc_Substance.GetContentOffset());
			GetcgDefFifo(this)->Update((uint8*)strTemp.c_str(), fnlc_Hash.GetContentLength(), fnlc_Hash.GetContentOffset());
			FNode_LC::HoldOffset();
		};
};
//------------------------------------------------------------------------------------------//
class CCY_FNLC_AES :public FNode_LC{
	public:
				 CCY_FNLC_AES(void) : FNode_LC(){cgAES_type = CCT_AES128;cgAES_mode = CCT_AES_CBC;};
		virtual	~CCY_FNLC_AES(void){;};
	private:
		CCT_AES_KEYL	cgAES_type;
		CCT_AES_MODE	cgAES_mode;
		uint32			cgInvalidLength;
	public:
		inline 	const uint32&	GetcgInvalidLength()const{return(cgInvalidLength);};
		void	Init(const FIFO_UINT8 *tfifo,CCT_AES_KEYL type = CCT_AES128,CCT_AES_MODE mode = CCT_AES_CBC,G_Endian_VAILD tEV = G_LITTLE_ENDIAN){
			FNode_LC::Init(tfifo,4,tEV);
			cgAES_type = type;
			cgAES_mode = mode;
			cgInvalidLength = 4;
		};
		void	SetConfig(CCT_AES_KEYL type = CCT_AES128,CCT_AES_MODE mode = CCT_AES_CBC,G_Endian_VAILD tEV = G_LITTLE_ENDIAN){
			FNode_LC::SetEndianType(this,tEV);
			cgAES_type = type;
			cgAES_mode = mode;
		}
	public:
		inline	const std::string	&ReadContent(std::string *retStrOriginal,const std::string &strKey,const FIFO_UINT8 *fifobuf = nullptr){
			if (strKey.length() == 0)
				return(FNode_LC::ReadContent(retStrOriginal,fifobuf));
			if (fifobuf == nullptr)
				fifobuf = GetcgDefFifo(this);
			return(CCY_Decrypt_AES(retStrOriginal, *fifobuf, GetContentLength(), GetContentOffset() , strKey, cgAES_type, cgAES_mode));
		};
		inline	const FIFO_UINT8	&ReadContent(FIFO_UINT8 *retfifoOriginal,const std::string &strKey,const FIFO_UINT8 *fifobuf = nullptr){
			if (strKey.length() == 0)
				return(FNode_LC::ReadContent(retfifoOriginal,fifobuf));
			if (fifobuf == nullptr)
				fifobuf = GetcgDefFifo(this);
			return(CCY_Decrypt_AES(retfifoOriginal, *fifobuf, GetContentLength(), GetContentOffset(), strKey, cgAES_type, cgAES_mode));
		};
		inline	const PUB_SBUF		&ReadContent(PUB_SBUF *retPSBUF,const std::string &strKey,const FIFO_UINT8 *fifobuf = nullptr){
			if (strKey.length() == 0)
				return(FNode_LC::ReadContent(retPSBUF,fifobuf));
			if (fifobuf == nullptr)
				fifobuf = GetcgDefFifo(this);
			retPSBUF->Spin_InUse_set();
			CCY_Decrypt_AES(&retPSBUF->cgBufFIFO, *fifobuf, GetContentLength(), GetContentOffset(), strKey, cgAES_type, cgAES_mode);
			retPSBUF->Spin_InUse_clr();
			return(*retPSBUF);
		};
		inline	const CCY_FNLC_AES	&SetContent	(const FIFO_UINT8 &fifoIn,uint32 num,uint32 offset,const std::string &strKey){
			if (strKey.length() == 0){
				FNode_LC::SetContent(fifoIn, num, offset);
			}
			else{
				FNode_LC::HoldOffset();
				CCY_Encrypt_AES(GetcgDefFifo(this), fifoIn, num, offset, strKey, cgAES_type, cgAES_mode);
				FNode_LC::UpdateLength();
			}
			return(*this);
		};
		inline	const CCY_FNLC_AES	&SetContent	(const uint8 *data,uint32 num,const std::string &strKey){
			if (strKey.length() == 0){
				FNode_LC::SetContent(data, num);
			}
			else{
				FNode_LC::HoldOffset();
				CCY_Encrypt_AES(GetcgDefFifo(this), data, num, strKey, cgAES_type, cgAES_mode);
				FNode_LC::UpdateLength();
			}
			return(*this);
		};
		inline	const CCY_FNLC_AES	&SetContent	(const std::string &strInput,const std::string &strKey){
			return(CCY_FNLC_AES::SetContent((uint8*)strInput.c_str(), (uint32)strInput.length(), strKey));
		};
		inline	const CCY_FNLC_AES	&SetContent	(const Field_Node &fnInput,const std::string &strKey){
			if (strKey.length() == 0){
				FNode_LC::SetContent(fnInput);
			}
			else{
				FNode_LC::HoldOffset();
				CCY_Encrypt_AES(GetcgDefFifo(this), *GetcgDefFifo(&fnInput), fnInput.GetLength(),fnInput.GetOffset(), strKey, cgAES_type, cgAES_mode);
				FNode_LC::UpdateLength();
			}
			return(*this);
		};
};
//------------------------------------------------------------------------------------------//
class CCY_FN_AES :public FNode_LC{
	public:
				 CCY_FN_AES(void) :FNode_LC(){AddSubNode(&fnlc_Hash);AddSubNode(&fnlc_Crypto);cgDigestType = CCT_MD5;};
		virtual	~CCY_FN_AES(void){;};
	private:
		CCT_Digest		cgDigestType;
	
		FNode_LC		fnlc_Hash;
		CCY_FNLC_AES	fnlc_Crypto;
	
		uint32			cgInvalidLength;
	public:
		inline 	const uint32&	GetcgInvalidLength(void)const{return(cgInvalidLength);};
		void	Init(const FIFO_UINT8 *tfifo,CCT_AES_KEYL type = CCT_AES128,CCT_AES_MODE mode = CCT_AES_CBC
					 ,CCT_Digest digestType = CCT_MD5,G_Endian_VAILD tEV = G_LITTLE_ENDIAN){
			FNode_LC::Init	(tfifo,4,tEV);
			fnlc_Hash.Init	(tfifo,1,tEV);
			fnlc_Crypto.Init(tfifo,type,mode,tEV);
			cgDigestType = digestType;
			cgInvalidLength = 4 + 1 + CCY_DigestSize(digestType) + fnlc_Crypto.GetcgInvalidLength();
		};
		void	SetConfig(CCT_AES_KEYL type = CCT_AES128,CCT_AES_MODE mode = CCT_AES_CBC
						  ,CCT_Digest digestType = CCT_MD5,G_Endian_VAILD tEV = G_LITTLE_ENDIAN){
			FNode_LC::SetEndianType(this,tEV);
			FNode_LC::SetEndianType(&fnlc_Hash,tEV);
			fnlc_Crypto.SetConfig(type,mode,tEV);
			cgDigestType = digestType;
		}
	public:
		inline	int32	CheckContent(std::string *retStrOriginal,const std::string &strKey,const FIFO_UINT8 *fifobuf = nullptr){
			std::string	strHash;
			*retStrOriginal = "";
			if (fnlc_Hash.ReadContent(&strHash,fifobuf) == CCY_Digest(cgDigestType,fnlc_Crypto.ReadContent(retStrOriginal,strKey,fifobuf)))
				return 1;
			return 0;
		}
		inline	int32	ReadContent	(std::string *retStrOriginal,const std::string &strKey,const FIFO_UINT8 *fifobuf = nullptr){
			return(CheckContent(retStrOriginal,strKey,fifobuf));
		};
		inline	int32	ReadContent	(FIFO_UINT8 *retfifoOriginal,const std::string &strKey,const FIFO_UINT8 *fifobuf = nullptr){
			std::string	retStrOriginal;
			if (CheckContent(&retStrOriginal,strKey,fifobuf) > 0){
				retfifoOriginal->PutInASCII(retStrOriginal, G_ESCAPE_OFF);
				return 1;
			}
			return 0;
		};
		inline	int32	ReadContent	(PUB_SBUF *retPSBUFOriginal,const std::string &strKey,const FIFO_UINT8 *fifobuf = nullptr){
			std::string	retStrOriginal;
			if (CheckContent(&retStrOriginal,strKey,fifobuf) > 0){
				retPSBUFOriginal->PutInASCII(retStrOriginal, G_ESCAPE_OFF);
				return 1;
			}
			return 0;
		};
		inline	const CCY_FN_AES&	SetContent	(const FIFO_UINT8 &fifoIn,uint32 num,uint32 offset,const std::string &strKey){
			FNode_LC::HoldOffset();
			fnlc_Hash = CCY_Digest(cgDigestType, fifoIn, num, offset);
			fnlc_Crypto.SetContent(fifoIn, num, offset, strKey);
			FNode_LC::UpdateLength();
			return(*this);
		};
		inline	const CCY_FN_AES&	SetContent	(const uint8 *data,uint32 num,const std::string &strKey){
			FNode_LC::HoldOffset();
			fnlc_Hash = CCY_Digest(cgDigestType, data, num);
			fnlc_Crypto.SetContent(data, num, strKey);
			FNode_LC::UpdateLength();
			return(*this);
		};
		inline	const CCY_FN_AES&	SetContent	(const std::string &strInput,const std::string &strKey){
			return(CCY_FN_AES::SetContent((uint8*)strInput.c_str(),(uint32)strInput.length(),strKey));
		};
		inline	const CCY_FN_AES&	SetContent	(const Field_Node &fnInput,const std::string &strKey){
			FNode_LC::HoldOffset();
			fnlc_Hash = CCY_Digest(cgDigestType,*GetcgDefFifo(&fnInput),fnInput.GetLength(),fnInput.GetOffset());
			fnlc_Crypto.SetContent(fnInput, strKey);
			FNode_LC::UpdateLength();
			return(*this);
		};
};
//------------------------------------------------------------------------------------------//
class CCY_FN_AES_MK :public FNode_LC{
	public:
				 CCY_FN_AES_MK(void) :FNode_LC(){;};
		virtual	~CCY_FN_AES_MK(void){;};
	private:
		CCY_FN_AES		fn_AES;
	public:
		void	Init(const FIFO_UINT8 *tfifo,CCT_AES_KEYL type = CCT_AES128,CCT_AES_MODE mode = CCT_AES_CBC
					 ,CCT_Digest digestType = CCT_MD5,G_Endian_VAILD tEV = G_LITTLE_ENDIAN){
			FNode_LC::Init	(tfifo,4,tEV);
			fn_AES.Init		(tfifo,type,mode,digestType,tEV);
		};
	public:
				int32	ReadContent	(std::string *retStr,const std::string &strMulitKey,const FIFO_UINT8 *fifobuf = nullptr);
				const CCY_FN_AES_MK&	SetContent	(const FIFO_UINT8 &fifoIn,uint32 num,uint32 offset,std::string strMulitKey);
				const CCY_FN_AES_MK&	SetContent	(const uint8 *data,uint32 num,std::string strMulitKey);
		inline	const CCY_FN_AES_MK&	SetContent	(const std::string &strInput,const std::string &strMulitKey){
			return(CCY_FN_AES_MK::SetContent((uint8*)strInput.c_str(),(uint32)strInput.length(),strMulitKey));
		};
		inline	const CCY_FN_AES_MK&	SetContent	(const Field_Node &fnInput,const std::string &strMulitKey){
			return(CCY_FN_AES_MK::SetContent(*GetcgDefFifo(&fnInput),fnInput.GetLength(),fnInput.GetOffset(),strMulitKey));
		};
};
//------------------------------------------------------------------------------------------//
#endif
#ifdef USE_OPENSSL
//------------------------------------------------------------------------------------------//
class CCY_FR_Signature :public Field_Node{
	public:
				 CCY_FR_Signature(void) :Field_Node(){AddNode(&fnlc_sKey);AddNode(&fnlc_signature);AddNode(&fnlcAES_packet);};
		virtual	~CCY_FR_Signature(void){;};
	public:
		void	Init(uint32 fifoSize = 1024 * 8,CCT_AES_KEYL type = CCT_AES128,CCT_AES_MODE mode = CCT_AES_CBC
					 ,CCT_Digest digestType = CCT_MD5,G_Endian_VAILD tEV = G_LITTLE_ENDIAN);
		FIFO_UINT8	cgFIFO_Result;
	private:
		CCT_Digest 		cgDigestType;
		FNode_LC		fnlc_sKey;
		FNode_LC		fnlc_signature;
		CCY_FNLC_AES	fnlcAES_packet;
	public:
			const CCY_FR_Signature&	Encode(const FIFO_UINT8 &fifoIn,uint32 num,uint32 offset,RSA *rsa_puk);
			const CCY_FR_Signature&	Encode(const uint8 *data,uint32 num,RSA *rsa_puk);
	inline	const CCY_FR_Signature&	Encode(const std::string &strInput,RSA *rsa_puk){
		return(CCY_FR_Signature::Encode((uint8*)strInput.c_str(),(uint32)strInput.length(),rsa_puk));
	};
	inline	const CCY_FR_Signature&	Encode(const Field_Node &fnInput,RSA *rsa_puk){
		return(CCY_FR_Signature::Encode(*GetcgDefFifo(&fnInput),fnInput.GetLength(),fnInput.GetOffset(),rsa_puk));
	};
	
	inline	const std::string&	Encode(std::string *retStrWhole,const uint8 *data,uint32 num,RSA *rsa_puk){
		CCY_FR_Signature::Encode(retStrWhole,data,num,rsa_puk);
		return(ReadAllContent(retStrWhole));
	};
	inline	const std::string&	Encode(std::string *retStrWhole,const std::string &strInput,RSA *rsa_puk){
		CCY_FR_Signature::Encode((uint8*)strInput.c_str(),(uint32)strInput.length(),rsa_puk);
		return(ReadAllContent(retStrWhole));
	};
			int32				Decode(std::string *retStrOriginal,const uint8 *data,uint32 num,RSA *rsa_prk);
			int32				Decode(std::string *retStrOriginal,const FIFO_UINT8 &fifoIn,RSA *rsa_prk);
	inline	int32				Decode(std::string *retStrOriginal,const std::string &strInput,RSA *rsa_prk){
		return(CCY_FR_Signature::Decode(retStrOriginal,(uint8*)strInput.c_str(),(uint32)strInput.length(),rsa_prk));
	};
};
//------------------------------------------------------------------------------------------//
#endif
//------------------------------------------------------------------------------------------//
#endif
