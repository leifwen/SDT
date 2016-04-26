/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: Comm_Crypto.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.03.26
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "Comm_Crypto.h"
#include "Comm_File.h"
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_VC
	#pragma comment(lib, "ssleay32.lib")
	#pragma comment(lib, "libeay32.lib")
#endif
//------------------------------------------------------------------------------------------//
#define CCY_PI		(3.1415926)
#include <math.h>
//------------------------------------------------------------------------------------------//
long double CCY_erf(long double x,uint32 maxN){
	uint32		n;
	long double n2,ret,itemD,itemU;
	
	if (x > CCY_PI)
		x = CCY_PI;
	
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
	
	ret = 2 * ret / sqrtl(CCY_PI);
	return(ret);
}
//------------------------------------------------------------------------------------------//
long double CCY_Gaussian(long double x,long double u,long double b){
	uint32		n;
	long double n2,ret,itemD,itemU;
	
	x = (x - u) / (b * sqrtl(2));
	
	if (x > CCY_PI)
		x = CCY_PI;
	if (x < (- CCY_PI))
		x = - CCY_PI;
	
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
	
	ret = 0.5 + ret / sqrtl(CCY_PI);
	
	return(ret);
}
//------------------------------------------------------------------------------------------//
#if (defined USE_OPENSSL) || (defined CommonDefH_MAC)
//------------------------------------------------------------------------------------------//
uint32 CCY_DigestSize(CCT_Digest type){
	switch(type){
		case CCT_SHA1:
			return(CC_SHA1_DIGEST_LENGTH);
		case CCT_SHA224:
			return(CC_SHA224_DIGEST_LENGTH);
		case CCT_SHA256:
			return(CC_SHA256_DIGEST_LENGTH);
		case CCT_SHA384:
			return(CC_SHA384_DIGEST_LENGTH);
		case CCT_SHA512:
			return(CC_SHA512_DIGEST_LENGTH);
		case CCT_MD5:
		default:
			return(CC_MD5_DIGEST_LENGTH);
	}
}
//------------------------------------------------------------------------------------------//
std::string CCY_Digest(CCT_Digest type,const uint8 *data,uint64 length){
	switch(type){
		case CCT_SHA1:
			return(CCY_Digest_SHA1(data,length));
		case CCT_SHA224:
			return(CCY_Digest_SHA224(data,length));
		case CCT_SHA256:
			return(CCY_Digest_SHA256(data,length));
		case CCT_SHA384:
			return(CCY_Digest_SHA384(data,length));
		case CCT_SHA512:
			return(CCY_Digest_SHA512(data,length));
		case CCT_MD5:
		default:
			return(CCY_Digest_MD5(data,length));
	}
}
//------------------------------------------------------------------------------------------//
std::string CCY_Digest_SHA1(const uint8 *data,uint64 length){
	CC_SHA1_CTX		ctx;
	CC_LONG			packageLength;
	uint8			result[CC_SHA1_DIGEST_LENGTH];

	if (length > 0xffffffff){
		CC_SHA1_Init(&ctx);
		do{
			packageLength = (length > 0xffffffff) ? (0xffffffff): (CC_LONG)length;
			length -= packageLength;
			CC_SHA1_Update(&ctx,data,packageLength);
			data += packageLength;
		}while(length > 0);
		CC_SHA1_Final(result,&ctx);
	}
	else{
		CC_SHA1(data,(CC_LONG)length,result);
	}
	return(Str_CharToASCIIStr(result,CC_SHA1_DIGEST_LENGTH,G_ESCAPE_OFF));
}
//------------------------------------------------------------------------------------------//
std::string CCY_Digest_SHA224(const uint8 *data,uint64 length){
	CC_SHA256_CTX	ctx;
	CC_LONG			packageLength;
	uint8			result[CC_SHA224_DIGEST_LENGTH];
	
	if (length > 0xffffffff){
		CC_SHA224_Init(&ctx);
		do{
			packageLength = (length > 0xffffffff) ? (0xffffffff): (CC_LONG)length;
			length -= packageLength;
			CC_SHA224_Update(&ctx,data,packageLength);
			data += packageLength;
		}while(length > 0);
		CC_SHA224_Final(result,&ctx);
	}
	else{
		CC_SHA224(data,(CC_LONG)length,result);
	}
	return(Str_CharToASCIIStr(result,CC_SHA224_DIGEST_LENGTH,G_ESCAPE_OFF));
}
//------------------------------------------------------------------------------------------//
std::string CCY_Digest_SHA256(const uint8 *data,uint64 length){
	CC_SHA256_CTX	ctx;
	CC_LONG			packageLength;
	uint8			result[CC_SHA256_DIGEST_LENGTH];
	
	if (length > 0xffffffff){
		CC_SHA256_Init(&ctx);
		do{
			packageLength = (length > 0xffffffff) ? (0xffffffff): (CC_LONG)length;
			length -= packageLength;
			CC_SHA256_Update(&ctx,data,packageLength);
			data += packageLength;
		}while(length > 0);
		CC_SHA256_Final(result,&ctx);
	}
	else{
		CC_SHA256(data,(CC_LONG)length,result);
	}
	return(Str_CharToASCIIStr(result,CC_SHA256_DIGEST_LENGTH,G_ESCAPE_OFF));
}
//------------------------------------------------------------------------------------------//
std::string CCY_Digest_SHA384(const uint8 *data,uint64 length){
	CC_SHA512_CTX	ctx;
	CC_LONG			packageLength;
	uint8			result[CC_SHA384_DIGEST_LENGTH];
	
	if (length > 0xffffffff){
		CC_SHA384_Init(&ctx);
		do{
			packageLength = (length > 0xffffffff) ? (0xffffffff): (CC_LONG)length;
			length -= packageLength;
			CC_SHA384_Update(&ctx,data,packageLength);
			data += packageLength;
		}while(length > 0);
		CC_SHA384_Final(result,&ctx);
	}
	else{
		CC_SHA384(data,(CC_LONG)length,result);
	}
	return(Str_CharToASCIIStr(result,CC_SHA384_DIGEST_LENGTH,G_ESCAPE_OFF));
}
//------------------------------------------------------------------------------------------//
std::string CCY_Digest_SHA512(const uint8 *data,uint64 length){
	CC_SHA512_CTX	ctx;
	CC_LONG			packageLength;
	uint8			result[CC_SHA512_DIGEST_LENGTH];
	
	if (length > 0xffffffff){
		CC_SHA512_Init(&ctx);
		do{
			packageLength = (length > 0xffffffff) ? (0xffffffff): (CC_LONG)length;
			length -= packageLength;
			CC_SHA512_Update(&ctx,data,packageLength);
			data += packageLength;
		}while(length > 0);
		CC_SHA512_Final(result,&ctx);
	}
	else{
		CC_SHA512(data,(CC_LONG)length,result);
	}
	return(Str_CharToASCIIStr(result,CC_SHA512_DIGEST_LENGTH,G_ESCAPE_OFF));
}
//------------------------------------------------------------------------------------------//
std::string CCY_Digest_MD5(const uint8 *data,uint64 length){
	CC_MD5_CTX		ctx;
	CC_LONG			packageLength;
	uint8			result[CC_MD5_DIGEST_LENGTH];
	
	if (length > 0xffffffff){
		CC_MD5_Init(&ctx);
		do{
			packageLength = (length > 0xffffffff) ? (0xffffffff): (CC_LONG)length;
			length -= packageLength;
			CC_MD5_Update(&ctx,data,packageLength);
			data += packageLength;
		}while(length > 0);
		CC_MD5_Final(result,&ctx);
	}
	else{
		CC_MD5(data,(CC_LONG)length,result);
	}
	return(Str_CharToASCIIStr(result,CC_MD5_DIGEST_LENGTH,G_ESCAPE_OFF));
}
//------------------------------------------------------------------------------------------//
std::string CCY_Digest(CCT_Digest type,const FIFO_UINT8 &fifo,uint32 length,uint32 offset){
	switch(type){
		case CCT_SHA1:
			return(CCY_Digest_SHA1(fifo,length,offset));
		case CCT_SHA224:
			return(CCY_Digest_SHA224(fifo,length,offset));
		case CCT_SHA256:
			return(CCY_Digest_SHA256(fifo,length,offset));
		case CCT_SHA384:
			return(CCY_Digest_SHA384(fifo,length,offset));
		case CCT_SHA512:
			return(CCY_Digest_SHA512(fifo,length,offset));
		case CCT_MD5:
		default:
			return(CCY_Digest_MD5(fifo,length,offset));
	}
}
//------------------------------------------------------------------------------------------//
std::string CCY_Digest_SHA1(const FIFO_UINT8 &fifo,uint32 length,uint32 offset){
	CC_SHA1_CTX		ctx;
	CC_LONG			slength;
	uint8			result[CC_SHA1_DIGEST_LENGTH];
	std::string		retStr;

	retStr = "";
	slength = fifo.CalcOutLength(length,offset);
	if (length > 0){
		CC_SHA1_Init(&ctx);
		CC_SHA1_Update(&ctx,fifo.GetOutPointer(offset),length);
		if (slength > 0)
			CC_SHA1_Update(&ctx,fifo.GetOutPointer(0),slength);
		CC_SHA1_Final(result,&ctx);
		retStr = Str_CharToASCIIStr(result,CC_SHA1_DIGEST_LENGTH,G_ESCAPE_OFF);
	}
	return(retStr);
}
//------------------------------------------------------------------------------------------//
std::string CCY_Digest_SHA224(const FIFO_UINT8 &fifo,uint32 length,uint32 offset){
	CC_SHA256_CTX	ctx;
	CC_LONG			slength;
	uint8			result[CC_SHA224_DIGEST_LENGTH];
	std::string		retStr;
	
	retStr = "";
	slength = fifo.CalcOutLength(length,offset);
	if (length > 0){
		CC_SHA224_Init(&ctx);
		CC_SHA224_Update(&ctx,fifo.GetOutPointer(offset),length);
		if (slength > 0)
			CC_SHA224_Update(&ctx,fifo.GetOutPointer(0),slength);
		CC_SHA224_Final(result,&ctx);
		retStr = Str_CharToASCIIStr(result,CC_SHA224_DIGEST_LENGTH,G_ESCAPE_OFF);
	}
	return(retStr);
}
//------------------------------------------------------------------------------------------//
std::string CCY_Digest_SHA256(const FIFO_UINT8 &fifo,uint32 length,uint32 offset){
	CC_SHA256_CTX	ctx;
	CC_LONG			slength;
	uint8			result[CC_SHA256_DIGEST_LENGTH];
	std::string		retStr;
	
	retStr = "";
	slength = fifo.CalcOutLength(length,offset);
	if (length > 0){
		CC_SHA256_Init(&ctx);
		CC_SHA256_Update(&ctx,fifo.GetOutPointer(offset),length);
		if (slength > 0)
			CC_SHA256_Update(&ctx,fifo.GetOutPointer(0),slength);
		CC_SHA256_Final(result,&ctx);
		retStr = Str_CharToASCIIStr(result,CC_SHA256_DIGEST_LENGTH,G_ESCAPE_OFF);
	}
	return(retStr);
}
//------------------------------------------------------------------------------------------//
std::string CCY_Digest_SHA384(const FIFO_UINT8 &fifo,uint32 length,uint32 offset){
	CC_SHA512_CTX	ctx;
	CC_LONG			slength;
	uint8			result[CC_SHA384_DIGEST_LENGTH];
	std::string		retStr;
	
	retStr = "";
	slength = fifo.CalcOutLength(length,offset);
	if (length > 0){
		CC_SHA384_Init(&ctx);
		CC_SHA384_Update(&ctx,fifo.GetOutPointer(offset),length);
		if (slength > 0)
			CC_SHA384_Update(&ctx,fifo.GetOutPointer(0),slength);
		CC_SHA384_Final(result,&ctx);
		retStr = Str_CharToASCIIStr(result,CC_SHA384_DIGEST_LENGTH,G_ESCAPE_OFF);
	}
	return(retStr);
}
//------------------------------------------------------------------------------------------//
std::string CCY_Digest_SHA512(const FIFO_UINT8 &fifo,uint32 length,uint32 offset){
	CC_SHA512_CTX	ctx;
	CC_LONG			slength;
	uint8			result[CC_SHA512_DIGEST_LENGTH];
	std::string		retStr;
	
	retStr = "";
	slength = fifo.CalcOutLength(length,offset);
	if (length > 0){
		CC_SHA512_Init(&ctx);
		CC_SHA512_Update(&ctx,fifo.GetOutPointer(offset),length);
		if (slength > 0)
			CC_SHA512_Update(&ctx,fifo.GetOutPointer(0),slength);
		CC_SHA512_Final(result,&ctx);
		retStr = Str_CharToASCIIStr(result,CC_SHA512_DIGEST_LENGTH,G_ESCAPE_OFF);
	}
	return(retStr);
}
//------------------------------------------------------------------------------------------//
std::string CCY_Digest_MD5(const FIFO_UINT8 &fifo,uint32 length,uint32 offset){
	CC_MD5_CTX		ctx;
	CC_LONG			slength;
	uint8			result[CC_MD5_DIGEST_LENGTH];
	std::string		retStr;
	
	retStr = "";
	slength = fifo.CalcOutLength(length,offset);
	if (length > 0){
		CC_MD5_Init(&ctx);
		CC_MD5_Update(&ctx,fifo.GetOutPointer(offset),length);
		if (slength > 0)
			CC_MD5_Update(&ctx,fifo.GetOutPointer(0),slength);
		CC_MD5_Final(result,&ctx);
		retStr = Str_CharToASCIIStr(result,CC_MD5_DIGEST_LENGTH,G_ESCAPE_OFF);
	}
	return(retStr);
}
//------------------------------------------------------------------------------------------//
std::string CC_DigestFile(CCT_Digest type,const std::string &sFN,uint64 length,uint64 offset){
	switch(type){
		case CCT_SHA1:
			return(CCY_DigestFile_SHA1(sFN,length,offset));
		case CCT_SHA224:
			return(CCY_DigestFile_SHA224(sFN,length,offset));
		case CCT_SHA256:
			return(CCY_DigestFile_SHA256(sFN,length,offset));
		case CCT_SHA384:
			return(CCY_DigestFile_SHA384(sFN,length,offset));
		case CCT_SHA512:
			return(CCY_DigestFile_SHA512(sFN,length,offset));
		case CCT_MD5:
		default:
			return(CCY_DigestFile_MD5(sFN,length,offset));
	}
}
//------------------------------------------------------------------------------------------//
std::string CCY_DigestFile_SHA1(const std::string &sFN,uint64 length,uint64 offset){
	CC_SHA1_CTX		ctx;
	CC_LONG			packageLength;
	uint8			result[CC_SHA1_DIGEST_LENGTH];
	uint8			data[1024 * 8];
	std::fstream	fileStream;

	if (CFS_CheckFile(sFN) == 0)
		return("");
	
	CC_SHA1_Init(&ctx);
	fileStream.open(sFN.c_str(),std::ios::in|std::ios::binary);
	if (offset > 0)
		fileStream.seekp(offset,std::ios::beg);
	do{
		packageLength = (length > sizeof(data))?sizeof(data):(CC_LONG)length;
		fileStream.read((char*)data,packageLength);
		packageLength = (CC_LONG)fileStream.gcount();
		length -= packageLength;
		CC_SHA1_Update(&ctx,data,packageLength);
	}while((!fileStream.eof()) && (length > 0));
	fileStream.close();
	CC_SHA1_Final(result,&ctx);
	return(Str_CharToASCIIStr(result,CC_SHA1_DIGEST_LENGTH,G_ESCAPE_OFF));
}
//------------------------------------------------------------------------------------------//
std::string CCY_DigestFile_SHA224(const std::string &sFN,uint64 length,uint64 offset){
	CC_SHA256_CTX	ctx;
	CC_LONG			packageLength;
	uint8			result[CC_SHA224_DIGEST_LENGTH];
	uint8			data[1024 * 8];
	std::fstream	fileStream;
	
	if (CFS_CheckFile(sFN) == 0)
		return("");
	
	CC_SHA224_Init(&ctx);
	fileStream.open(sFN.c_str(),std::ios::in|std::ios::binary);
	if (offset > 0)
		fileStream.seekp(offset,std::ios::beg);
	do{
		packageLength = (length > sizeof(data))?sizeof(data):(CC_LONG)length;
		fileStream.read((char*)data,packageLength);
		packageLength = (CC_LONG)fileStream.gcount();
		length -= packageLength;
		CC_SHA224_Update(&ctx,data,packageLength);
	}while((!fileStream.eof()) && (length > 0));
	fileStream.close();
	CC_SHA224_Final(result,&ctx);
	return(Str_CharToASCIIStr(result,CC_SHA224_DIGEST_LENGTH,G_ESCAPE_OFF));
}
//------------------------------------------------------------------------------------------//
std::string CCY_DigestFile_SHA256(const std::string &sFN,uint64 length,uint64 offset){
	CC_SHA256_CTX	ctx;
	CC_LONG			packageLength;
	uint8			result[CC_SHA256_DIGEST_LENGTH];
	uint8			data[1024 * 8];
	std::fstream	fileStream;
	
	if (CFS_CheckFile(sFN) == 0)
		return("");
	
	CC_SHA256_Init(&ctx);
	fileStream.open(sFN.c_str(),std::ios::in|std::ios::binary);
	if (offset > 0)
		fileStream.seekp(offset,std::ios::beg);
	do{
		packageLength = (length > sizeof(data))?sizeof(data):(CC_LONG)length;
		fileStream.read((char*)data,packageLength);
		packageLength = (CC_LONG)fileStream.gcount();
		length -= packageLength;
		CC_SHA256_Update(&ctx,data,packageLength);
	}while((!fileStream.eof()) && (length > 0));
	fileStream.close();
	CC_SHA256_Final(result,&ctx);
	return(Str_CharToASCIIStr(result,CC_SHA256_DIGEST_LENGTH,G_ESCAPE_OFF));
}
//------------------------------------------------------------------------------------------//
std::string CCY_DigestFile_SHA384(const std::string &sFN,uint64 length,uint64 offset){
	CC_SHA512_CTX	ctx;
	CC_LONG			packageLength;
	uint8			result[CC_SHA384_DIGEST_LENGTH];
	uint8			data[1024 * 8];
	std::fstream	fileStream;
	
	if (CFS_CheckFile(sFN) == 0)
		return("");
	
	CC_SHA384_Init(&ctx);
	fileStream.open(sFN.c_str(),std::ios::in|std::ios::binary);
	if (offset > 0)
		fileStream.seekp(offset,std::ios::beg);
	do{
		packageLength = (length > sizeof(data))?sizeof(data):(CC_LONG)length;
		fileStream.read((char*)data,packageLength);
		packageLength = (CC_LONG)fileStream.gcount();
		length -= packageLength;
		CC_SHA384_Update(&ctx,data,packageLength);
	}while((!fileStream.eof()) && (length > 0));
	fileStream.close();
	CC_SHA384_Final(result,&ctx);
	return(Str_CharToASCIIStr(result,CC_SHA384_DIGEST_LENGTH,G_ESCAPE_OFF));
}
//------------------------------------------------------------------------------------------//
std::string CCY_DigestFile_SHA512(const std::string &sFN,uint64 length,uint64 offset){
	CC_SHA512_CTX	ctx;
	CC_LONG			packageLength;
	uint8			result[CC_SHA512_DIGEST_LENGTH];
	uint8			data[1024 * 8];
	std::fstream	fileStream;
	
	if (CFS_CheckFile(sFN) == 0)
		return("");
	
	CC_SHA512_Init(&ctx);
	fileStream.open(sFN.c_str(),std::ios::in|std::ios::binary);
	if (offset > 0)
		fileStream.seekp(offset,std::ios::beg);
	do{
		packageLength = (length > sizeof(data))?sizeof(data):(CC_LONG)length;
		fileStream.read((char*)data,packageLength);
		packageLength = (CC_LONG)fileStream.gcount();
		length -= packageLength;
		CC_SHA512_Update(&ctx,data,packageLength);
	}while((!fileStream.eof()) && (length > 0));
	fileStream.close();
	CC_SHA512_Final(result,&ctx);
	return(Str_CharToASCIIStr(result,CC_SHA512_DIGEST_LENGTH,G_ESCAPE_OFF));
}
//------------------------------------------------------------------------------------------//
std::string CCY_DigestFile_MD5(const std::string &sFN,uint64 length,uint64 offset){
	CC_MD5_CTX		ctx;
	CC_LONG			packageLength;
	uint8			result[CC_MD5_DIGEST_LENGTH];
	uint8			data[1024 * 8];
	std::fstream	fileStream;
	
	if (CFS_CheckFile(sFN) == 0)
		return("");
	
	CC_MD5_Init(&ctx);
	fileStream.open(sFN.c_str(),std::ios::in|std::ios::binary);
	if (offset > 0)
		fileStream.seekp(offset,std::ios::beg);
	do{
		packageLength = (length > sizeof(data))?sizeof(data):(CC_LONG)length;
		fileStream.read((char*)data,packageLength);
		packageLength = (CC_LONG)fileStream.gcount();
		length -= packageLength;
		CC_MD5_Update(&ctx,data,packageLength);
	}while((!fileStream.eof()) && (length > 0));
	fileStream.close();
	CC_MD5_Final(result,&ctx);
	return(Str_CharToASCIIStr(result,CC_MD5_DIGEST_LENGTH,G_ESCAPE_OFF));
}
//------------------------------------------------------------------------------------------//
inline std::string CCY_Create_IV(void){
	return(CCY_Digest_MD5("Written by Leif Wen,Script Debugging Tools"));
}
//------------------------------------------------------------------------------------------//
#endif
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_MAC
//------------------------------------------------------------------------------------------//
CCCryptorStatus CCY_AES_Init(CCCryptorRef &cryptorRef,CCOperation op,const std::string &sKey,CCT_AES_KEYL type,CCT_AES_MODE mode){
	size_t			KeySize;
	switch (type){
		case CCT_AES128:
			KeySize = kCCKeySizeAES128;
			break;
		case CCT_AES192:
			KeySize = kCCKeySizeAES192;
			break;
		case CCT_AES256:
			
		default:
			KeySize = kCCKeySizeAES256;
			break;
	}
	
	if (mode == CCT_AES_CBC)
		return(CCCryptorCreate(op,kCCAlgorithmAES,kCCOptionPKCS7Padding,sKey.c_str(),KeySize,CCY_Create_IV().c_str(),&cryptorRef));
	return(CCCryptorCreateWithMode(op,kCCModeCFB8,kCCAlgorithmAES,ccPKCS7Padding,CCY_Create_IV().c_str()
									  ,sKey.c_str(),KeySize,sKey.c_str(),KeySize,0,kCCModeOptionCTR_BE,&cryptorRef));
}
//------------------------------------------------------------------------------------------//
inline void CCY_AES_DataToStr(std::string &retStr,uint8 *buf,const uint64 &bufSize,const uint8 *data,uint64 length,CCCryptorRef &cryptorRef){
	size_t	dataNum,packageLength;
	do{
		packageLength = (length < (bufSize - 16)) ? (size_t)length : ((size_t)bufSize - 16);
		length -= packageLength;
		CCCryptorUpdate(cryptorRef, data, packageLength, buf, (size_t)bufSize, &dataNum);
		data += packageLength;
		retStr += Str_CharToASCIIStr(buf, dataNum, G_ESCAPE_OFF);
	}while(length > 0);
}
//------------------------------------------------------------------------------------------//
const std::string &CCY_Encrypt_AES(std::string *retStr,const uint8 *data,uint64 length,const std::string &sKey,CCT_AES_KEYL type,CCT_AES_MODE mode){
	CCCryptorRef	cryptorRef;
	uint8			dataOut[1024 * 8];
	size_t			dataNum;
	std::string		&resultStr = *retStr;
	
	resultStr = "";
	if (length > 0){
		if (CCY_AES_Init(cryptorRef,kCCEncrypt,sKey,type,mode) == kCCSuccess){
			CCY_AES_DataToStr(resultStr,dataOut,sizeof(dataOut),data,length,cryptorRef);
			CCCryptorFinal(cryptorRef, dataOut, sizeof(dataOut), &dataNum);
			resultStr += Str_CharToASCIIStr(dataOut, dataNum, G_ESCAPE_OFF);
			CCCryptorRelease(cryptorRef);
		}
	}
	return(resultStr);
}
//------------------------------------------------------------------------------------------//
const std::string &CCY_Decrypt_AES(std::string *retStr,const uint8 *data,uint64 length,const std::string &sKey,CCT_AES_KEYL type,CCT_AES_MODE mode){
	CCCryptorRef	cryptorRef;
	uint8			dataOut[1024 * 8];
	size_t			dataNum;
	std::string		&resultStr = *retStr;
	
	resultStr = "";
	if (length > 0){
		if (CCY_AES_Init(cryptorRef,kCCDecrypt,sKey,type,mode) == kCCSuccess){
			CCY_AES_DataToStr(resultStr,dataOut,sizeof(dataOut),data,length,cryptorRef);
			CCCryptorFinal(cryptorRef, dataOut, sizeof(dataOut), &dataNum);
			resultStr += Str_CharToASCIIStr(dataOut, dataNum, G_ESCAPE_OFF);
			CCCryptorRelease(cryptorRef);
		}
	}
	return(resultStr);
}
//------------------------------------------------------------------------------------------//
const std::string &CCY_Encrypt_AES(std::string *retStr,const FIFO_UINT8 &fifo,uint32 length,uint32 offset
								   ,const std::string &sKey,CCT_AES_KEYL type,CCT_AES_MODE mode){
	CCCryptorRef	cryptorRef;
	uint8			dataOut[1024 * 8];
	size_t			dataNum;
	std::string		&resultStr = *retStr;
	uint32			slength;
	
	resultStr = "";
	slength = fifo.CalcOutLength(length,offset);
	if (length > 0){
		if (CCY_AES_Init(cryptorRef,kCCEncrypt,sKey,type,mode) == kCCSuccess){
			CCY_AES_DataToStr(resultStr,dataOut,sizeof(dataOut),fifo.GetOutPointer(offset),length,cryptorRef);
			if (slength > 0)
				CCY_AES_DataToStr(resultStr,dataOut,sizeof(dataOut),fifo.GetOutPointer(0),slength,cryptorRef);
			CCCryptorFinal(cryptorRef, dataOut, sizeof(dataOut), &dataNum);
			resultStr += Str_CharToASCIIStr(dataOut, dataNum, G_ESCAPE_OFF);
			CCCryptorRelease(cryptorRef);
		}
	}
	return(resultStr);
}
//------------------------------------------------------------------------------------------//
const std::string &CCY_Decrypt_AES(std::string *retStr,const FIFO_UINT8 &fifo,uint32 length,uint32 offset
								   ,const std::string &sKey,CCT_AES_KEYL type,CCT_AES_MODE mode){
	CCCryptorRef	cryptorRef;
	uint8			dataOut[1024 * 8];
	size_t			dataNum;
	std::string		&resultStr = *retStr;
	uint32			slength;

	resultStr = "";
	slength = fifo.CalcOutLength(length,offset);
	if (length > 0){
		if (CCY_AES_Init(cryptorRef,kCCDecrypt,sKey,type,mode) == kCCSuccess){
			CCY_AES_DataToStr(resultStr,dataOut,sizeof(dataOut),fifo.GetOutPointer(offset),length,cryptorRef);
			if (slength > 0)
				CCY_AES_DataToStr(resultStr,dataOut,sizeof(dataOut),fifo.GetOutPointer(0),slength,cryptorRef);
			CCCryptorFinal(cryptorRef, dataOut, sizeof(dataOut), &dataNum);
			resultStr += Str_CharToASCIIStr(dataOut, dataNum, G_ESCAPE_OFF);
			CCCryptorRelease(cryptorRef);
		}
	}
	return(resultStr);
}
//------------------------------------------------------------------------------------------//
void CCY_AES_DataToFifo(FIFO_UINT8 *retfifo,uint8 *buf,const uint8 *data,uint32 length,CCCryptorRef &cryptorRef){
	uint32	offset,slength,capacity;
	size_t	dataNum;
	
	while(length > 0){
		offset = 0;
		retfifo->CalcInCapacity(capacity, offset);
		if (capacity == 0)
			return;
		if (capacity < 16){
			slength = (length < 16) ? length : 16;
			CCCryptorUpdate(cryptorRef, data, slength, buf, 16, &dataNum);
			data += slength;
			retfifo->Put(buf,(uint32)dataNum);
			length -= slength;
			if (length == 0)
				return;
		}
		offset = 0;
		retfifo->CalcInCapacity(capacity, offset);
		slength = (capacity >> 4) << 4;
		if (length > slength){
			slength = length - slength;
			length -= slength;
		}
		else{
			slength = 0;
		}
		dataNum = 0;
		if (length > 0){
			CCCryptorUpdate(cryptorRef, data, length, retfifo->GetOutPointer(offset), capacity, &dataNum);
			data += length;
			length = 0;
			retfifo->In((uint32)dataNum);
		}
		if (slength > 0)
			length = slength;
	}
}
//------------------------------------------------------------------------------------------//
const FIFO_UINT8 &CCY_Encrypt_AES(FIFO_UINT8 *retfifo,const uint8 *data,uint32 length,const std::string &sKey,CCT_AES_KEYL type,CCT_AES_MODE mode){
	CCCryptorRef	cryptorRef;
	size_t			dataNum;
	uint32			slength;
	uint8			buf[16];

	if (length > 0){
		if (CCY_AES_Init(cryptorRef,kCCEncrypt,sKey,type,mode) == kCCSuccess){
			slength = ((length + 15) >> 4) << 4;
			if (slength > retfifo->Unused())
				retfifo->Init(retfifo->MaxSize() + (slength - retfifo->Unused()));
			
			CCY_AES_DataToFifo(retfifo,buf,data,length,cryptorRef);
			
			CCCryptorFinal(cryptorRef, buf, 16, &dataNum);
			retfifo->Put(buf,(uint32)dataNum);
			CCCryptorRelease(cryptorRef);
		}
	}
	return(*retfifo);
}
//------------------------------------------------------------------------------------------//
const FIFO_UINT8 &CCY_Decrypt_AES(FIFO_UINT8 *retfifo,const uint8 *data,uint32 length,const std::string &sKey,CCT_AES_KEYL type,CCT_AES_MODE mode){
	CCCryptorRef	cryptorRef;
	size_t			dataNum;
	uint32			slength;
	uint8			buf[16];
	
	if (length > 0){
		if (CCY_AES_Init(cryptorRef,kCCDecrypt,sKey,type,mode) == kCCSuccess){
			slength = ((length + 15) >> 4) << 4;
			if (slength > retfifo->Unused())
				retfifo->Init(retfifo->MaxSize() + (slength - retfifo->Unused()));
			
			CCY_AES_DataToFifo(retfifo,buf,data,length,cryptorRef);
			
			CCCryptorFinal(cryptorRef, buf, 16, &dataNum);
			retfifo->Put(buf,(uint32)dataNum);
			CCCryptorRelease(cryptorRef);
		}
	}
	return(*retfifo);
}
//------------------------------------------------------------------------------------------//
const FIFO_UINT8 &CCY_Encrypt_AES(FIFO_UINT8 *retfifo,const FIFO_UINT8 &fifo,uint32 length,uint32 offset
								  ,const std::string &sKey,CCT_AES_KEYL type,CCT_AES_MODE mode){
	CCCryptorRef	cryptorRef;
	size_t			dataNum;
	uint32			slength,out_1,out_offset,out_2;
	uint8			buf[16];
	
	out_1 = length;
	out_offset = offset;
	fifo.CalcOutLength(out_1, out_offset);
	if (out_1 > 0){
		if (CCY_AES_Init(cryptorRef,kCCEncrypt,sKey,type,mode) == kCCSuccess){
			slength = ((length + 15) >> 4) << 4;
			if (slength > retfifo->Unused())
				retfifo->Init(retfifo->MaxSize() + (slength - retfifo->Unused()));

			out_1 = length;
			out_offset = offset;
			out_2 = fifo.CalcOutLength(out_1, out_offset);
			
			CCY_AES_DataToFifo(retfifo,buf,fifo.GetOutPointer(out_offset),out_1,cryptorRef);
			CCY_AES_DataToFifo(retfifo,buf,fifo.GetOutPointer(0),out_2,cryptorRef);
			
			CCCryptorFinal(cryptorRef, buf, 16, &dataNum);
			retfifo->Put(buf,(uint32)dataNum);
			CCCryptorRelease(cryptorRef);
		}
	}
	return(*retfifo);
}
//------------------------------------------------------------------------------------------//
const FIFO_UINT8 &CCY_Decrypt_AES(FIFO_UINT8 *retfifo,const FIFO_UINT8 &fifo,uint32 length,uint32 offset
								  ,const std::string &sKey,CCT_AES_KEYL type,CCT_AES_MODE mode){
	CCCryptorRef	cryptorRef;
	size_t			dataNum;
	uint32			slength,out_1,out_offset,out_2;
	uint8			buf[16];
	
	out_1 = length;
	out_offset = offset;
	fifo.CalcOutLength(out_1, out_offset);
	if (out_1 > 0){
		if (CCY_AES_Init(cryptorRef,kCCDecrypt,sKey,type,mode) == kCCSuccess){
			slength = ((length + 15) >> 4) << 4;
			if (slength > retfifo->Unused())
				retfifo->Init(retfifo->MaxSize() + (slength - retfifo->Unused()));

			out_1 = length;
			out_offset = offset;
			out_2 = fifo.CalcOutLength(out_1, out_offset);

			CCY_AES_DataToFifo(retfifo,buf,fifo.GetOutPointer(out_offset),out_1,cryptorRef);
			CCY_AES_DataToFifo(retfifo,buf,fifo.GetOutPointer(0),out_2,cryptorRef);
			
			CCCryptorFinal(cryptorRef, buf, 16, &dataNum);
			retfifo->Put(buf,(uint32)dataNum);
			CCCryptorRelease(cryptorRef);
		}
	}
	return(*retfifo);
}
//------------------------------------------------------------------------------------------//
#endif
#if (defined USE_OPENSSL) && ((defined CommonDefH_VC) || (defined CommonDefH_Linux))
enum{EncryptType = 0,DecryptType};
//------------------------------------------------------------------------------------------//
void CCY_AES_Init(EVP_CIPHER_CTX &ctx, int32 op, const std::string &sKey, CCT_AES_KEYL type, CCT_AES_MODE mode){
	EVP_CIPHER		*evpCipher;

	if (mode == CCT_AES_CBC){
		switch (type){
			case CCT_AES128:
				evpCipher = (EVP_CIPHER*)EVP_aes_128_cbc();
				break;
			case CCT_AES192:
				evpCipher = (EVP_CIPHER*)EVP_aes_192_cbc();
				break;
			case CCT_AES256:
			default:
				evpCipher = (EVP_CIPHER*)EVP_aes_256_cbc();
				break;
		}
	}
	else{
		switch (type){
			case CCT_AES128:
				evpCipher = (EVP_CIPHER*)EVP_aes_128_cfb8();
				break;
			case CCT_AES192:
				evpCipher = (EVP_CIPHER*)EVP_aes_192_cfb8();
				break;
			case CCT_AES256:
			default:
				evpCipher = (EVP_CIPHER*)EVP_aes_256_cfb8();
				break;
		}
	}
	
	EVP_CIPHER_CTX_init(&ctx);
	if (op == EncryptType){
		EVP_EncryptInit(&ctx,evpCipher,(uint8*)sKey.c_str(),(uint8*)CCY_Create_IV().c_str());
	}
	else{
		EVP_DecryptInit(&ctx,evpCipher,(uint8*)sKey.c_str(),(uint8*)CCY_Create_IV().c_str());
	}
}
//------------------------------------------------------------------------------------------//
inline void CCY_AES_DataToStr(std::string &retStr,uint8 *buf,const uint64 &bufSize,const uint8 *data,uint64 length,EVP_CIPHER_CTX &ctx,int32 op){
	int32	dataNum,packageLength;
	do{
		packageLength = (length < (bufSize - 16)) ? (int32)length : (int32)(bufSize - 16);
		length -= packageLength;
		if (op == EncryptType){
			EVP_EncryptUpdate(&ctx, buf, &dataNum, data, packageLength);
		}
		else{
			EVP_DecryptUpdate(&ctx, buf, &dataNum, data, packageLength);
		}
		data += packageLength;
		retStr += Str_CharToASCIIStr(buf, dataNum, G_ESCAPE_OFF);
	}while(length > 0);
}
//------------------------------------------------------------------------------------------//
const std::string &CCY_Encrypt_AES(std::string *retStr, const uint8 *data, uint64 length, const std::string &sKey, CCT_AES_KEYL type, CCT_AES_MODE mode){
	EVP_CIPHER_CTX	ctx;
	uint8			dataOut[1024 * 8];
	int32			dataNum;
	std::string		&resultStr = *retStr;
	
	resultStr = "";

	CCY_AES_Init(ctx,EncryptType,sKey,type,mode);
	CCY_AES_DataToStr(resultStr,dataOut,sizeof(dataOut),data,length,ctx,EncryptType);
	EVP_EncryptFinal(&ctx, dataOut, &dataNum);
	resultStr += Str_CharToASCIIStr(dataOut, dataNum, G_ESCAPE_OFF);
	EVP_CIPHER_CTX_cleanup(&ctx);
	return(resultStr);
}
//------------------------------------------------------------------------------------------//
const std::string &CCY_Decrypt_AES(std::string *retStr, const uint8 *data, uint64 length, const std::string &sKey, CCT_AES_KEYL type, CCT_AES_MODE mode){
	EVP_CIPHER_CTX	ctx;
	uint8			dataOut[1024 * 8];
	int32			dataNum;
	std::string		&resultStr = *retStr;
	
	resultStr = "";
	
	CCY_AES_Init(ctx,DecryptType,sKey,type,mode);
	CCY_AES_DataToStr(resultStr,dataOut,sizeof(dataOut),data,length,ctx,DecryptType);
	EVP_DecryptFinal(&ctx, dataOut, &dataNum);
	resultStr += Str_CharToASCIIStr(dataOut, dataNum, G_ESCAPE_OFF);
	EVP_CIPHER_CTX_cleanup(&ctx);
	return(resultStr);
}
//------------------------------------------------------------------------------------------//
const std::string &CCY_Encrypt_AES(std::string *retStr,const FIFO_UINT8 &fifo,uint32 length,uint32 offset
								   ,const std::string &sKey,CCT_AES_KEYL type, CCT_AES_MODE mode){
	EVP_CIPHER_CTX	ctx;
	uint8			dataOut[1024 * 8];
	int32			dataNum;
	std::string		&resultStr = *retStr;
	uint32			slength;

	
	resultStr = "";
	slength = fifo.CalcOutLength(length,offset);
	if (length > 0){
		CCY_AES_Init(ctx,EncryptType,sKey,type,mode);
		
		CCY_AES_DataToStr(resultStr,dataOut,sizeof(dataOut),fifo.GetOutPointer(offset),length,ctx,EncryptType);
		if (slength > 0)
			CCY_AES_DataToStr(resultStr,dataOut,sizeof(dataOut),fifo.GetOutPointer(0),slength,ctx,EncryptType);

		EVP_EncryptFinal(&ctx, dataOut, &dataNum);
		resultStr += Str_CharToASCIIStr(dataOut, dataNum, G_ESCAPE_OFF);
		EVP_CIPHER_CTX_cleanup(&ctx);
	}
	return(resultStr);
}
//------------------------------------------------------------------------------------------//
const std::string &CCY_Decrypt_AES(std::string *retStr,const FIFO_UINT8 &fifo,uint32 length,uint32 offset
								   ,const std::string &sKey,CCT_AES_KEYL type,CCT_AES_MODE mode){
	EVP_CIPHER_CTX	ctx;
	uint8			dataOut[1024 * 8];
	int32			dataNum;
	std::string		&resultStr = *retStr;
	uint32			slength;
	
	
	resultStr = "";
	slength = fifo.CalcOutLength(length,offset);
	if (length > 0){
		CCY_AES_Init(ctx,DecryptType,sKey,type,mode);
		
		CCY_AES_DataToStr(resultStr,dataOut,sizeof(dataOut),fifo.GetOutPointer(offset),length,ctx,DecryptType);
		if (slength > 0)
			CCY_AES_DataToStr(resultStr,dataOut,sizeof(dataOut),fifo.GetOutPointer(0),slength,ctx,DecryptType);
		
		EVP_DecryptFinal(&ctx, dataOut, &dataNum);
		resultStr += Str_CharToASCIIStr(dataOut, dataNum, G_ESCAPE_OFF);
		EVP_CIPHER_CTX_cleanup(&ctx);
	}
	return(resultStr);
}
//------------------------------------------------------------------------------------------//
void CCY_AES_DataToFifo(FIFO_UINT8 *retfifo,uint8 *buf,const uint8 *data,uint32 length,EVP_CIPHER_CTX &ctx,int32 op){
	uint32	offset,slength,capacity;
	int32	dataNum;

	while(length > 0){
		offset = 0;
		retfifo->CalcInCapacity(capacity, offset);
		if (capacity == 0)
			return;
		if (capacity < 16){
			slength = (length < 16) ? length : 16;
			if (op == EncryptType){
				EVP_EncryptUpdate(&ctx, buf, &dataNum, data, slength);
			}
			else{
				EVP_DecryptUpdate(&ctx, buf, &dataNum, data, slength);
			}
			data += slength;
			retfifo->Put(buf,(uint32)dataNum);
			length -= slength;
			if (length == 0)
				return;
		}
		offset = 0;
		retfifo->CalcInCapacity(capacity, offset);
		
		slength = (capacity >> 4) << 4;
		if (length > slength){
			slength = length - slength;
			length -= slength;
		}
		else{
			slength = 0;
		}
		dataNum = 0;
		if (length > 0){
			if (op == EncryptType){
				EVP_EncryptUpdate(&ctx, retfifo->GetOutPointer(offset), &dataNum, data, length);
			}
			else{
				EVP_DecryptUpdate(&ctx, retfifo->GetOutPointer(offset), &dataNum, data, length);
			}
			data += length;
			length = 0;
			retfifo->In((uint32)dataNum);
		}
		if (slength > 0)
			length = slength;
	}
}
//------------------------------------------------------------------------------------------//
const FIFO_UINT8 &CCY_Encrypt_AES(FIFO_UINT8 *retfifo,const uint8 *data,uint32 length,const std::string &sKey,CCT_AES_KEYL type,CCT_AES_MODE mode){
	EVP_CIPHER_CTX	ctx;
	int32			dataNum;
	uint32			slength;
	uint8			buf[16];
	
	if (length > 0){
		CCY_AES_Init(ctx,EncryptType,sKey,type,mode);
		slength = ((length + 15) >> 4) << 4;
		if (slength > retfifo->Unused())
			retfifo->Init(retfifo->MaxSize() + (slength - retfifo->Unused()));
		
		CCY_AES_DataToFifo(retfifo,buf,data,length,ctx,EncryptType);
		
		EVP_EncryptFinal(&ctx, buf, &dataNum);
		retfifo->Put(buf,(uint32)dataNum);
		EVP_CIPHER_CTX_cleanup(&ctx);
	}
	return(*retfifo);
}
//------------------------------------------------------------------------------------------//
const FIFO_UINT8 &CCY_Decrypt_AES(FIFO_UINT8 *retfifo,const uint8 *data,uint32 length,const std::string &sKey,CCT_AES_KEYL type,CCT_AES_MODE mode){
	EVP_CIPHER_CTX	ctx;
	int32			dataNum;
	uint32			slength;
	uint8			buf[16];
	
	if (length > 0){
		CCY_AES_Init(ctx,DecryptType,sKey,type,mode);
		slength = ((length + 15) >> 4) << 4;
		if (slength > retfifo->Unused())
			retfifo->Init(retfifo->MaxSize() + (slength - retfifo->Unused()));
		
		CCY_AES_DataToFifo(retfifo,buf,data,length,ctx,DecryptType);
		
		EVP_DecryptFinal(&ctx, buf, &dataNum);
		retfifo->Put(buf,(uint32)dataNum);
		EVP_CIPHER_CTX_cleanup(&ctx);
	}
	return(*retfifo);
}
//------------------------------------------------------------------------------------------//
const FIFO_UINT8 &CCY_Encrypt_AES(FIFO_UINT8 *retfifo,const FIFO_UINT8 &fifo,uint32 length,uint32 offset
								  ,const std::string &sKey,CCT_AES_KEYL type,CCT_AES_MODE mode){
	EVP_CIPHER_CTX	ctx;
	int32			dataNum;
	uint32			slength,out_1,out_offset,out_2;
	uint8			buf[16];
	
	out_1 = length;
	out_offset = offset;
	fifo.CalcOutLength(out_1, out_offset);
	if (out_1 > 0){
		CCY_AES_Init(ctx,EncryptType,sKey,type,mode);
		slength = ((length + 15) >> 4) << 4;
		if (slength > retfifo->Unused())
			retfifo->Init(retfifo->MaxSize() + (slength - retfifo->Unused()));
		
		out_1 = length;
		out_offset = offset;
		out_2 = fifo.CalcOutLength(out_1, out_offset);

		CCY_AES_DataToFifo(retfifo,buf,fifo.GetOutPointer(out_offset),out_1,ctx,EncryptType);
		CCY_AES_DataToFifo(retfifo,buf,fifo.GetOutPointer(0),out_2,ctx,EncryptType);
		
		EVP_EncryptFinal(&ctx, buf, &dataNum);
		retfifo->Put(buf,(uint32)dataNum);
		
		EVP_CIPHER_CTX_cleanup(&ctx);
	}
	return(*retfifo);
}
//------------------------------------------------------------------------------------------//
const FIFO_UINT8 &CCY_Decrypt_AES(FIFO_UINT8 *retfifo,const FIFO_UINT8 &fifo,uint32 length,uint32 offset
								  ,const std::string &sKey,CCT_AES_KEYL type,CCT_AES_MODE mode){
	EVP_CIPHER_CTX	ctx;
	int32			dataNum;
	uint32			slength,out_1,out_offset,out_2;
	uint8			buf[16];
	
	out_1 = length;
	out_offset = offset;
	fifo.CalcOutLength(out_1, out_offset);
	if (out_1 > 0){
		CCY_AES_Init(ctx,DecryptType,sKey,type,mode);
		slength = ((length + 15) >> 4) << 4;
		if (slength > retfifo->Unused())
			retfifo->Init(retfifo->MaxSize() + (slength - retfifo->Unused()));
		out_1 = length;
		out_offset = offset;
		out_2 = fifo.CalcOutLength(out_1, out_offset);
		
		CCY_AES_DataToFifo(retfifo,buf,fifo.GetOutPointer(out_offset),out_1,ctx,DecryptType);
		CCY_AES_DataToFifo(retfifo,buf,fifo.GetOutPointer(0),out_2,ctx,DecryptType);
		
		EVP_DecryptFinal(&ctx, buf, &dataNum);
		retfifo->Put(buf,(uint32)dataNum);
		
		EVP_CIPHER_CTX_cleanup(&ctx);
	}
	return(*retfifo);
}
//------------------------------------------------------------------------------------------//
#endif
//------------------------------------------------------------------------------------------//
enum{EncodeB64,DecodeB64};
//------------------------------------------------------------------------------------------//
inline void CCY_BASE64_DataToStr(std::string &retStr,uint8 *buf,const uint64 &bufSize,const uint8 *data,uint64 length
								 ,CCY_BASE64_CTX &ctx,int32 op,CCT_BASE64_NL blEnNL){
	uint64	dataNum,packageLength;
	do{
		packageLength = (length < bufSize) ? (int32)length : (int32)bufSize;
		length -= packageLength;
		if (op == EncodeB64){
			CCY_EnB64_Update(&ctx,buf,&dataNum,data,packageLength,blEnNL);
		}
		else{
			CCY_DeB64_Update(&ctx,buf,&dataNum,data,packageLength,blEnNL);
		}
		data += packageLength;
		retStr += Str_CharToASCIIStr(buf, dataNum, G_ESCAPE_OFF);
	}while(length > 0);
}
//------------------------------------------------------------------------------------------//
const std::string &CCY_Encode_BASE64(std::string *retStr,const uint8 *data,uint64 length,CCT_BASE64_NL blEnNL){
	CCY_BASE64_CTX	ctx;
	uint8			dataOut[1024 * 8];
	uint64			dataNum;
	std::string		&resultStr = *retStr;
	
	resultStr = "";
	CCY_B64_CTXInit(&ctx);
	CCY_BASE64_DataToStr(resultStr,dataOut,sizeof(dataOut),data,length,ctx,EncodeB64,blEnNL);
	CCY_EnB64_Final(&ctx, dataOut, &dataNum,blEnNL);
	resultStr += Str_CharToASCIIStr(dataOut, dataNum, G_ESCAPE_OFF);
	return(resultStr);
}
//------------------------------------------------------------------------------------------//
const std::string &CCY_Decode_BASE64(std::string *retStr,const uint8 *data,uint64 length,CCT_BASE64_NL blEnNL){
	CCY_BASE64_CTX	ctx;
	uint8			dataOut[1024 * 8];
	uint64			dataNum;
	std::string		&resultStr = *retStr;
	
	resultStr = "";
	CCY_B64_CTXInit(&ctx);
	CCY_BASE64_DataToStr(resultStr,dataOut,sizeof(dataOut),data,length,ctx,DecodeB64,blEnNL);
	CCY_DeB64_Final(&ctx, dataOut, &dataNum,blEnNL);
	resultStr += Str_CharToASCIIStr(dataOut, dataNum, G_ESCAPE_OFF);
	return(resultStr);
}
//------------------------------------------------------------------------------------------//
const std::string &CCY_Encode_BASE64(std::string *retStr,const FIFO_UINT8 &fifo,uint32 length,uint32 offset,CCT_BASE64_NL blEnNL){
	CCY_BASE64_CTX	ctx;
	uint8			dataOut[1024 * 8];
	uint64			dataNum;
	std::string		&resultStr = *retStr;
	uint32			slength;
	
	resultStr = "";
	slength = fifo.CalcOutCapacity(length,offset);
	if (length > 0){
		CCY_B64_CTXInit(&ctx);
		CCY_BASE64_DataToStr(resultStr,dataOut,sizeof(dataOut),fifo.GetOutPointer(offset),length,ctx,EncodeB64,blEnNL);
		if (slength)
			CCY_BASE64_DataToStr(resultStr,dataOut,sizeof(dataOut),fifo.GetOutPointer(0),slength,ctx,EncodeB64,blEnNL);
		
		CCY_DeB64_Final(&ctx, dataOut, &dataNum,blEnNL);
		resultStr += Str_CharToASCIIStr(dataOut, dataNum, G_ESCAPE_OFF);
	}
	return(resultStr);
}
//------------------------------------------------------------------------------------------//
const std::string &CCY_Decode_BASE64(std::string *retStr,const FIFO_UINT8 &fifo,uint32 length,uint32 offset,CCT_BASE64_NL blEnNL){
	CCY_BASE64_CTX	ctx;
	uint8			dataOut[1024 * 8];
	uint64			dataNum;
	std::string		&resultStr = *retStr;
	uint32			slength;
	
	resultStr = "";
	slength = fifo.CalcOutCapacity(length,offset);
	if (length > 0){
		CCY_B64_CTXInit(&ctx);
		CCY_BASE64_DataToStr(resultStr,dataOut,sizeof(dataOut),fifo.GetOutPointer(offset),length,ctx,DecodeB64,blEnNL);
		if (slength)
			CCY_BASE64_DataToStr(resultStr,dataOut,sizeof(dataOut),fifo.GetOutPointer(0),slength,ctx,DecodeB64,blEnNL);
		
		CCY_DeB64_Final(&ctx, dataOut, &dataNum,blEnNL);
		resultStr += Str_CharToASCIIStr(dataOut, dataNum, G_ESCAPE_OFF);
	}
	return(resultStr);
}
//------------------------------------------------------------------------------------------//
void CCY_EnB64_DataToFifo(FIFO_UINT8 *retfifo,uint8 *buf,const uint8 *data,uint32 length,CCY_BASE64_CTX &ctx,CCT_BASE64_NL blEnNL){
	uint32	offset,slength,capacity;
	uint64	dataNum;
	
	while(length > 0){
		offset = 0;
		retfifo->CalcInCapacity(capacity, offset);
		if (capacity == 0)
			return;
		if (capacity < 4){
			slength = (length < 3) ? length : 3;
			CCY_EnB64_Update(&ctx,buf,&dataNum,data,slength,blEnNL);
			data += slength;
			retfifo->Put(buf,(uint32)dataNum);
			length -= slength;
			if (length == 0)
				return;
		}
		offset = 0;
		retfifo->CalcInCapacity(capacity, offset);
		
		slength = (blEnNL == CCT_NL_NO) ? ((capacity >> 2) * 3) : (((capacity - (capacity + 64) / 65) >> 2) * 3);
		if (length > slength){
			slength = length - slength;
			length -= slength;
		}
		else{
			slength = 0;
		}
		dataNum = 0;
		if (length > 0){
			CCY_EnB64_Update(&ctx,retfifo->GetOutPointer(offset),&dataNum,data,length,blEnNL);
			data += length;
			length = 0;
			retfifo->In((uint32)dataNum);
		}
		if (slength > 0)
			length = slength;
	}
}
//------------------------------------------------------------------------------------------//
const FIFO_UINT8 &CCY_Encode_BASE64(FIFO_UINT8 *retfifo,const uint8 *data,uint32 length,CCT_BASE64_NL blEnNL){
	CCY_BASE64_CTX	ctx;
	uint64			dataNum;
	uint32			slength;
	uint8			buf[4];
	
	if (length > 0){
		CCY_B64_CTXInit(&ctx);
		slength = ((length + 2) / 3) << 2;
		if (blEnNL != CCT_NL_NO)
			slength = slength + ((slength + 63) >> 6);
		if (slength > retfifo->Unused())
			retfifo->Init(retfifo->MaxSize() + (slength - retfifo->Unused()));
		
		CCY_EnB64_DataToFifo(retfifo,buf,data,length,ctx,blEnNL);
		CCY_EnB64_Final(&ctx, buf, &dataNum, blEnNL);
		retfifo->Put(buf,(uint32)dataNum);
	}
	return(*retfifo);
}
//------------------------------------------------------------------------------------------//
void CCY_DeB64_DataToFifo(FIFO_UINT8 *retfifo,uint8* buf,const uint8 *data,uint32 length,CCY_BASE64_CTX &ctx,CCT_BASE64_NL blEnNL){
	uint32	offset,slength,capacity;
	uint64	dataNum;
	
	while(length > 0){
		offset = 0;
		retfifo->CalcInCapacity(capacity, offset);
		if (capacity == 0)
			return;
		if (capacity < 3){
			slength = (length < 4) ? length : 4;
			CCY_DeB64_Update(&ctx,buf,&dataNum,data,slength,blEnNL);
			data += slength;
			retfifo->Put(buf,(uint32)dataNum);
			length -= slength;
			if (length == 0)
				return;
		}
		offset = 0;
		retfifo->CalcInCapacity(capacity, offset);
		
		slength = ((capacity + 2) / 3) << 2;
		if (blEnNL != CCT_NL_NO)
			slength = slength + ((slength + 63) >> 6);

		if (length > slength){
			slength = length - slength;
			length -= slength;
		}
		else{
			slength = 0;
		}
		dataNum = 0;
		if (length > 0){
			CCY_DeB64_Update(&ctx,retfifo->GetOutPointer(offset),&dataNum,data,length,blEnNL);
			data += length;
			length = 0;
			retfifo->In((uint32)dataNum);
		}
		if (slength > 0)
			length = slength;
	}
}
//------------------------------------------------------------------------------------------//
const FIFO_UINT8 &CCY_Decode_BASE64(FIFO_UINT8 *retfifo,const uint8 *data,uint32 length,CCT_BASE64_NL blEnNL){
	CCY_BASE64_CTX	ctx;
	uint64			dataNum;
	uint32			slength;
	uint8			buf[4];
	
	if (length > 0){
		CCY_B64_CTXInit(&ctx);
		slength = (blEnNL == CCT_NL_NO) ? ((length >> 2) * 3) : (((length - (length + 64) / 65) >> 2) * 3);
		if (slength > retfifo->Unused())
			retfifo->Init(retfifo->MaxSize() + (slength - retfifo->Unused()));
		
		CCY_DeB64_DataToFifo(retfifo,buf,data,length,ctx,blEnNL);
		CCY_DeB64_Final(&ctx, buf, &dataNum, blEnNL);
		retfifo->Put(buf,(uint32)dataNum);
	}
	return(*retfifo);
}
//------------------------------------------------------------------------------------------//
const FIFO_UINT8 &CCY_Encode_BASE64(FIFO_UINT8 *retfifo,const FIFO_UINT8 &fifo,uint32 length,uint32 offset,CCT_BASE64_NL blEnNL){
	CCY_BASE64_CTX	ctx;
	uint64			dataNum;
	uint32			slength,out_1,out_offset,out_2;
	uint8			buf[4];
	
	out_1 = length;
	out_offset = offset;
	fifo.CalcOutLength(out_1, out_offset);
	if (out_1 > 0){
		CCY_B64_CTXInit(&ctx);
		slength = ((length + 2) / 3) << 2;
		if (blEnNL != CCT_NL_NO)
			slength = slength + ((slength + 63) >> 6);
		if (slength > retfifo->Unused())
			retfifo->Init(retfifo->MaxSize() + (slength - retfifo->Unused()));
		out_1 = length;
		out_offset = offset;
		out_2 = fifo.CalcOutLength(out_1, out_offset);
		
		CCY_EnB64_DataToFifo(retfifo,buf,fifo.GetOutPointer(out_offset),out_1,ctx,blEnNL);
		CCY_EnB64_DataToFifo(retfifo,buf,fifo.GetOutPointer(0),out_2,ctx,blEnNL);
		CCY_EnB64_Final(&ctx, buf, &dataNum, blEnNL);
		retfifo->Put(buf,(uint32)dataNum);
	}
	return(*retfifo);
}
//------------------------------------------------------------------------------------------//
const FIFO_UINT8 &CCY_Decode_BASE64(FIFO_UINT8 *retfifo,const FIFO_UINT8 &fifo,uint32 length,uint32 offset,CCT_BASE64_NL blEnNL){
	CCY_BASE64_CTX	ctx;
	uint64			dataNum;
	uint32			slength,out_1,out_offset,out_2;
	uint8			buf[4];
	
	out_1 = length;
	out_offset = offset;
	fifo.CalcOutLength(out_1, out_offset);
	if (out_1 > 0){
		CCY_B64_CTXInit(&ctx);
		slength = (blEnNL == CCT_NL_NO) ? ((length >> 2) * 3) : (((length - (length + 64) / 65) >> 2) * 3);
		if (slength > retfifo->Unused())
			retfifo->Init(retfifo->MaxSize() + (slength - retfifo->Unused()));
		out_1 = length;
		out_offset = offset;
		out_2 = fifo.CalcOutLength(out_1, out_offset);
		
		CCY_DeB64_DataToFifo(retfifo,buf,fifo.GetOutPointer(out_offset),out_1,ctx,blEnNL);
		CCY_DeB64_DataToFifo(retfifo,buf,fifo.GetOutPointer(0),out_2,ctx,blEnNL);
		CCY_DeB64_Final(&ctx, buf, &dataNum, blEnNL);
		retfifo->Put(buf,(uint32)dataNum);
	}
	return(*retfifo);
}
//------------------------------------------------------------------------------------------//
void CCY_Encode_File_BASE64(const std::string &outFN,const std::string &inFN,CCT_BASE64_NL blEnNL){
	CCY_BASE64_CTX		ctx;
	uint64			packageLength,dataNum;
	uint8			data[1024 * 8];
	uint8			dataOut[1024 * 8];
	std::fstream	fileStreamIn,fileStreamOut;
	std::string		resultStr;
	
	if (CFS_CheckFile(inFN) == 0)
		return;

	fileStreamOut.open(outFN.c_str(),std::ios::out|std::ios::trunc|std::ios::binary);
	fileStreamIn.open(inFN.c_str(),std::ios::in|std::ios::binary);
	CCY_B64_CTXInit(&ctx);
	do{
		packageLength = sizeof(data);
		fileStreamIn.read((char*)data,packageLength);
		packageLength = fileStreamIn.gcount();
		resultStr = "";
		CCY_BASE64_DataToStr(resultStr,dataOut,sizeof(dataOut),data,packageLength,ctx,EncodeB64,blEnNL);
		fileStreamOut << resultStr;
		fileStreamOut.flush();
	}while(!fileStreamIn.eof());
	fileStreamIn.close();
	CCY_EnB64_Final(&ctx, dataOut, &dataNum,blEnNL);
	resultStr = Str_CharToASCIIStr(dataOut, dataNum, G_ESCAPE_OFF);
	fileStreamOut << resultStr;
	fileStreamOut.flush();
	fileStreamOut.close();
	return;
}
//------------------------------------------------------------------------------------------//
void CCY_Decode_File_BASE64(const std::string &outFN,const std::string &inFN,CCT_BASE64_NL blEnNL){
	CCY_BASE64_CTX		ctx;
	uint64			packageLength,dataNum;
	uint8			data[1024 * 8];
	uint8			dataOut[1024 * 8];
	std::fstream	fileStreamIn,fileStreamOut;
	std::string		resultStr;
	
	if (CFS_CheckFile(inFN) == 0)
		return;
	
	fileStreamOut.open(outFN.c_str(),std::ios::out|std::ios::trunc|std::ios::binary);
	fileStreamIn.open(inFN.c_str(),std::ios::in|std::ios::binary);
	CCY_B64_CTXInit(&ctx);
	do{
		packageLength = sizeof(data);
		fileStreamIn.read((char*)data,packageLength);
		packageLength = fileStreamIn.gcount();
		resultStr = "";
		CCY_BASE64_DataToStr(resultStr,dataOut,sizeof(dataOut),data,packageLength,ctx,DecodeB64,blEnNL);
		fileStreamOut << resultStr;
		fileStreamOut.flush();
	}while(!fileStreamIn.eof());
	fileStreamIn.close();
	CCY_DeB64_Final(&ctx, dataOut, &dataNum,blEnNL);
	resultStr = Str_CharToASCIIStr(dataOut, dataNum, G_ESCAPE_OFF);
	fileStreamOut << resultStr;
	fileStreamOut.flush();
	fileStreamOut.close();
	return;
}
//------------------------------------------------------------------------------------------//

























#ifdef USE_OPENSSL
//------------------------------------------------------------------------------------------//
std::string CCY_CreateRandKey(int32 length){
	std::string	ret;
	int32		rLength;
	char 		buf[64];
	if (RAND_status() != 1){
		RAND_bytes((uint8*)buf,64);
		RAND_add(buf,sizeof(buf),strlen(buf));
		RAND_seed(buf,sizeof(buf));
		while(RAND_status() != 1)
			RAND_poll();
	}
	ret = "";
	do{
		rLength = (length > sizeof(buf)) ? sizeof(buf) : length;
		RAND_bytes((uint8*)buf,rLength);
		ret += Str_CharToASCIIStr((uint8*)buf,rLength,G_ESCAPE_OFF);
		length -= rLength;
	}while(length > 0);
	return(ret);
}
//------------------------------------------------------------------------------------------//
int32 CCY_RSA_CreateKey(RSA *retRSA,int bits){
	int32		ret;
	BN_ULONG	w;
	BIGNUM		*bne;
	
	ret = 0;
	bne = nullptr;
	w = RSA_3;
	bne = BN_new();
	if (bne != nullptr){
		if (BN_set_word(bne,w) != 0){
			ret = RSA_generate_key_ex(retRSA,bits,bne,nullptr);
		}
		BN_free(bne);
	}
	return(ret);
}
//------------------------------------------------------------------------------------------//
int32 CCY_RSA_WR_PrivateKey_PEM(RSA *tRSA,const std::string &sFN){
	int32	ret;
	BIO		*tBIO;
	
	ret = 0;
	tBIO = BIO_new_file(sFN.c_str(),"w");
	if (tBIO != nullptr){
		ret = PEM_write_bio_RSAPrivateKey(tBIO,tRSA,nullptr,nullptr,0,nullptr,nullptr);
		BIO_flush(tBIO);
		BIO_free(tBIO);
	}
	return(ret);
}
//------------------------------------------------------------------------------------------//
int32 CCY_RSA_RD_PrivateKey_PEM(RSA **retRSA,const std::string &sFN){
	BIO *tBIO;
	RSA	*readRet;

	readRet = nullptr;
	tBIO = nullptr;
	tBIO = BIO_new_file(sFN.c_str(),"rb");
	if (tBIO != nullptr){
		readRet = PEM_read_bio_RSAPrivateKey(tBIO,retRSA,nullptr,nullptr);
		BIO_free(tBIO);
	}
	return(readRet != nullptr);
}
//------------------------------------------------------------------------------------------//
int32 CCY_RSA_WR_PublicKey_PEM(RSA *tRSA,const std::string &sFN){
	int32	ret;
	BIO		*tBIO;
	
	ret = 0;
	tBIO = BIO_new_file(sFN.c_str(),"w");
	if (tBIO != nullptr){
		ret = PEM_write_bio_RSAPublicKey(tBIO,tRSA);
		BIO_flush(tBIO);
		BIO_free(tBIO);
	}
	return(ret);
}
//------------------------------------------------------------------------------------------//
int32 CCY_RSA_RD_PublicKey_PEM(RSA **retRSA,const std::string &sFN){
	BIO *tBIO;
	RSA	*readRet;
	
	readRet = nullptr;
	tBIO = BIO_new_file(sFN.c_str(),"rb");
	if (tBIO != nullptr){
		readRet = PEM_read_bio_RSAPublicKey(tBIO,retRSA,nullptr,nullptr);
		BIO_free(tBIO);
	}
	return(readRet != nullptr);
}
//------------------------------------------------------------------------------------------//
const std::string &CCY_Encrypt_RSAPrivateKey(std::string *retStr,const uint8 *data,uint64 num,RSA *tRSA){
	uint8	*bufout;
	int32	len;
	*retStr = "";
	len = RSA_size(tRSA);
	bufout = new uint8[len];
	if (bufout!= nullptr){
		len = RSA_private_encrypt((uint32)num,data,bufout,tRSA,RSA_PKCS1_PADDING);
		if (len > 0)
			*retStr = Str_CharToASCIIStr(bufout,len,G_ESCAPE_OFF);
		delete []bufout;
	}
	return(*retStr);
}
//------------------------------------------------------------------------------------------//
const std::string &CCY_Decrypt_RSAPrivateKey(std::string *retStr,const uint8 *data,uint64 num,RSA *tRSA){
	uint8	*bufout;
	int32	len;
	*retStr = "";
	len = RSA_size(tRSA);
	bufout = new uint8[len];
	if (bufout!= nullptr){
		len = RSA_private_decrypt((uint32)num,data,bufout,tRSA,RSA_PKCS1_PADDING);
		if (len > 0)
			*retStr = Str_CharToASCIIStr(bufout,len,G_ESCAPE_OFF);
		delete []bufout;
	}
	return(*retStr);
}
//------------------------------------------------------------------------------------------//
const std::string &CCY_Encrypt_RSAPublicKey(std::string *retStr,const uint8 *data,uint64 num,RSA *tRSA){
	uint8	*bufout;
	int32	len;
	*retStr = "";
	len = RSA_size(tRSA);
	bufout = new uint8[len];
	if (bufout!= nullptr){
		len = RSA_public_encrypt((uint32)num,data,bufout,tRSA,RSA_PKCS1_PADDING);
		if (len > 0)
			*retStr = Str_CharToASCIIStr(bufout,len,G_ESCAPE_OFF);
		delete []bufout;
	}
	return(*retStr);
}
//------------------------------------------------------------------------------------------//
const std::string &CCY_Decrypt_RSAPublicKey(std::string *retStr,const uint8 *data,uint64 num,RSA *tRSA){
	uint8	*bufout;
	int32	len;
	*retStr = "";
	len = RSA_size(tRSA);
	bufout = new uint8[len];
	if (bufout!= nullptr){
		len = RSA_public_decrypt((uint32)num,data,bufout,tRSA,RSA_PKCS1_PADDING);
		if (len > 0)
			*retStr = Str_CharToASCIIStr(bufout,len,G_ESCAPE_OFF);
		delete []bufout;
	}
	return(*retStr);
}
//------------------------------------------------------------------------------------------//
const std::string &CCY_Encode_RSAPrivateKey(std::string *retStr,const RSA *tRSA){
	uint8	bufout[1024 * 2],*p;
	int32	len;
	*retStr = "";
	p = bufout;
	len = i2d_RSAPrivateKey(tRSA,&p);
	if (len > 0)
		*retStr = Str_CharToASCIIStr(bufout,len,G_ESCAPE_OFF);
	return(*retStr);
}
//------------------------------------------------------------------------------------------//
const std::string &CCY_Encode_RSAPublicKey(std::string *retStr,const RSA *tRSA){
	uint8	bufout[1024 * 2],*p;
	int32	len;
	*retStr = "";
	p = bufout;
	len = i2d_RSAPublicKey(tRSA,&p);
	if (len > 0)
		*retStr = Str_CharToASCIIStr(bufout,len,G_ESCAPE_OFF);
	return(*retStr);
}
//------------------------------------------------------------------------------------------//
#endif











#if (defined USE_OPENSSL) || (defined CommonDefH_MAC)
//------------------------------------------------------------------------------------------//
int32 CCY_FN_AES_MK::ReadContent(std::string *retStr,const std::string &strMulitKey,const FIFO_UINT8 *fifobuf){
	//strMulitKey is in BASE64, used '\r' to split
	std::string	strKey,strMKey;
	uint32		tOffset;
	
	if (fifobuf == nullptr)
		fifobuf = GetcgDefFifo();
	
	tOffset = GetContentOffset();
	while(tOffset < fifobuf->Used()){
		tOffset += fn_AES.AnalysisFrame(*fifobuf,tOffset);
		strMKey = strMulitKey;
		do{
			CCY_Decode_BASE64(&strKey,Str_ReadSubItem(&strMKey, "\r"),CCT_NL_NO);
			if (strKey.length() > 0){
				if (fn_AES.ReadContent(retStr, strKey, fifobuf) != 0)
					return 1;
			}
		}while(strMKey.length() > 0);
	}
	*retStr = "";
	return 0;
}
//------------------------------------------------------------------------------------------//
const CCY_FN_AES_MK& CCY_FN_AES_MK::SetContent(const uint8 *data,uint32 num,std::string strMulitKey){
	//strMulitKey is in BASE64, used '\r' to split
	std::string	strContent,strKey;

	FNode_LC::HoldOffset();
	do{
		strKey = Str_ReadSubItem(&strMulitKey, "\r");
		if (strKey.length() > 0)
			fn_AES.SetContent(data, num, CCY_Decode_BASE64(strKey,CCT_NL_NO));
	}while(strMulitKey.length() > 0);
	
	FNode_LC::UpdateLength();
	return(*this);
}
//------------------------------------------------------------------------------------------//
const CCY_FN_AES_MK& CCY_FN_AES_MK::SetContent(const FIFO_UINT8 &fifoIn,uint32 num,uint32 offset,std::string strMulitKey){
	//strMulitKey is in BASE64, used '\r' to split
	std::string	strContent,strKey;
	
	FNode_LC::HoldOffset();
	do{
		strKey = Str_ReadSubItem(&strMulitKey, "\r");
		if (strKey.length() > 0)
			fn_AES.SetContent(fifoIn, num, offset, CCY_Decode_BASE64(strKey,CCT_NL_NO));
	}while(strMulitKey.length() > 0);
	
	FNode_LC::UpdateLength();
	return(*this);
}
//------------------------------------------------------------------------------------------//
#endif








#ifdef USE_OPENSSL
//------------------------------------------------------------------------------------------//
void CCY_FR_Signature::Init(uint32 fifoSize,CCT_AES_KEYL type,CCT_AES_MODE mode,CCT_Digest digestType,G_Endian_VAILD tEV){
	cgFIFO_Result.Init(fifoSize);
	
	Field_Node::Init	(FNT_MESSAGE,&cgFIFO_Result,tEV);
	fnlc_sKey.Init		(&cgFIFO_Result,2,tEV);
	fnlc_signature.Init	(&cgFIFO_Result,2,tEV);
	fnlcAES_packet.Init	(&cgFIFO_Result,type,mode,tEV);
	cgDigestType = digestType;
}
//------------------------------------------------------------------------------------------//
const CCY_FR_Signature&	CCY_FR_Signature::Encode(const FIFO_UINT8 &fifoIn,uint32 num,uint32 offset,RSA *rsa_puk){
	std::string	strContent,sKey;
	GetcgDefFifo()->Empty();
	HoldOffset();
	sKey			= CCY_CreateRandKey(32);
	fnlc_sKey		= CCY_Encrypt_RSAPublicKey(&strContent, sKey, rsa_puk);
	fnlc_signature	= CCY_Encrypt_RSAPublicKey(&strContent, CCY_Digest(cgDigestType, fifoIn, num,offset), rsa_puk);
	fnlcAES_packet.SetContent(fifoIn, num, offset, sKey);
	UpdateLength();
	return(*this);
}
//------------------------------------------------------------------------------------------//
const CCY_FR_Signature& CCY_FR_Signature::Encode(const uint8 *data,uint32 num,RSA *rsa_puk){
	std::string	strContent,sKey;
	GetcgDefFifo()->Empty();
	HoldOffset();
	sKey			= CCY_CreateRandKey(32);
	fnlc_sKey		= CCY_Encrypt_RSAPublicKey(&strContent, sKey, rsa_puk);
	fnlc_signature	= CCY_Encrypt_RSAPublicKey(&strContent, CCY_Digest(cgDigestType, data, num), rsa_puk);
	fnlcAES_packet.SetContent(data, num, sKey);
	UpdateLength();
	return(*this);
}
//------------------------------------------------------------------------------------------//
int32 CCY_FR_Signature::Decode(std::string *retStrOriginal,const FIFO_UINT8 &fifoIn,RSA *rsa_prk){
	std::string	strContent,sKey;
	
	if (AnalysisFrame(fifoIn) < 1)
		return 0;
	
	CCY_Decrypt_RSAPrivateKey(&sKey, fnlc_sKey.ReadContent(&strContent,&fifoIn), rsa_prk);
	
	fnlcAES_packet.ReadContent(retStrOriginal,sKey,&fifoIn);
	
	if (CCY_Decrypt_RSAPrivateKey(&sKey, fnlc_signature.ReadContent(&strContent,&fifoIn), rsa_prk) == CCY_Digest(cgDigestType,*retStrOriginal))
		return 1;
	*retStrOriginal = "";
	return 0;
}
//------------------------------------------------------------------------------------------//
int32 CCY_FR_Signature::Decode(std::string *retStrOriginal,const uint8 *data,uint32 num,RSA *rsa_prk){
	std::string	strContent,sKey;

	GetcgDefFifo()->Empty();
	GetcgDefFifo()->Init(num);
	GetcgDefFifo()->Put(data,num);
	
	if (AnalysisFrame(*GetcgDefFifo()) < 1)
		return 0;

	CCY_Decrypt_RSAPrivateKey(&sKey, fnlc_sKey.ReadContent(&strContent), rsa_prk);
	
	fnlcAES_packet.ReadContent(retStrOriginal,sKey);
	
	if (CCY_Decrypt_RSAPrivateKey(&sKey, fnlc_signature.ReadContent(&strContent), rsa_prk) == CCY_Digest(cgDigestType,*retStrOriginal))
		return 1;
	*retStrOriginal = "";
	return 0;
}
//------------------------------------------------------------------------------------------//
#endif

















































//------------------------------------------------------------------------------------------//
static const char e0[256] = {
 'A',  'A',  'A',  'A',  'B',  'B',  'B',  'B',  'C',  'C',
 'C',  'C',  'D',  'D',  'D',  'D',  'E',  'E',  'E',  'E',
 'F',  'F',  'F',  'F',  'G',  'G',  'G',  'G',  'H',  'H',
 'H',  'H',  'I',  'I',  'I',  'I',  'J',  'J',  'J',  'J',
 'K',  'K',  'K',  'K',  'L',  'L',  'L',  'L',  'M',  'M',
 'M',  'M',  'N',  'N',  'N',  'N',  'O',  'O',  'O',  'O',
 'P',  'P',  'P',  'P',  'Q',  'Q',  'Q',  'Q',  'R',  'R',
 'R',  'R',  'S',  'S',  'S',  'S',  'T',  'T',  'T',  'T',
 'U',  'U',  'U',  'U',  'V',  'V',  'V',  'V',  'W',  'W',
 'W',  'W',  'X',  'X',  'X',  'X',  'Y',  'Y',  'Y',  'Y',
 'Z',  'Z',  'Z',  'Z',  'a',  'a',  'a',  'a',  'b',  'b',
 'b',  'b',  'c',  'c',  'c',  'c',  'd',  'd',  'd',  'd',
 'e',  'e',  'e',  'e',  'f',  'f',  'f',  'f',  'g',  'g',
 'g',  'g',  'h',  'h',  'h',  'h',  'i',  'i',  'i',  'i',
 'j',  'j',  'j',  'j',  'k',  'k',  'k',  'k',  'l',  'l',
 'l',  'l',  'm',  'm',  'm',  'm',  'n',  'n',  'n',  'n',
 'o',  'o',  'o',  'o',  'p',  'p',  'p',  'p',  'q',  'q',
 'q',  'q',  'r',  'r',  'r',  'r',  's',  's',  's',  's',
 't',  't',  't',  't',  'u',  'u',  'u',  'u',  'v',  'v',
 'v',  'v',  'w',  'w',  'w',  'w',  'x',  'x',  'x',  'x',
 'y',  'y',  'y',  'y',  'z',  'z',  'z',  'z',  '0',  '0',
 '0',  '0',  '1',  '1',  '1',  '1',  '2',  '2',  '2',  '2',
 '3',  '3',  '3',  '3',  '4',  '4',  '4',  '4',  '5',  '5',
 '5',  '5',  '6',  '6',  '6',  '6',  '7',  '7',  '7',  '7',
 '8',  '8',  '8',  '8',  '9',  '9',  '9',  '9',  '+',  '+',
 '+',  '+',  '/',  '/',  '/',  '/'
};
static const char e1[256] = {
 'A',  'B',  'C',  'D',  'E',  'F',  'G',  'H',  'I',  'J',
 'K',  'L',  'M',  'N',  'O',  'P',  'Q',  'R',  'S',  'T',
 'U',  'V',  'W',  'X',  'Y',  'Z',  'a',  'b',  'c',  'd',
 'e',  'f',  'g',  'h',  'i',  'j',  'k',  'l',  'm',  'n',
 'o',  'p',  'q',  'r',  's',  't',  'u',  'v',  'w',  'x',
 'y',  'z',  '0',  '1',  '2',  '3',  '4',  '5',  '6',  '7',
 '8',  '9',  '+',  '/',  'A',  'B',  'C',  'D',  'E',  'F',
 'G',  'H',  'I',  'J',  'K',  'L',  'M',  'N',  'O',  'P',
 'Q',  'R',  'S',  'T',  'U',  'V',  'W',  'X',  'Y',  'Z',
 'a',  'b',  'c',  'd',  'e',  'f',  'g',  'h',  'i',  'j',
 'k',  'l',  'm',  'n',  'o',  'p',  'q',  'r',  's',  't',
 'u',  'v',  'w',  'x',  'y',  'z',  '0',  '1',  '2',  '3',
 '4',  '5',  '6',  '7',  '8',  '9',  '+',  '/',  'A',  'B',
 'C',  'D',  'E',  'F',  'G',  'H',  'I',  'J',  'K',  'L',
 'M',  'N',  'O',  'P',  'Q',  'R',  'S',  'T',  'U',  'V',
 'W',  'X',  'Y',  'Z',  'a',  'b',  'c',  'd',  'e',  'f',
 'g',  'h',  'i',  'j',  'k',  'l',  'm',  'n',  'o',  'p',
 'q',  'r',  's',  't',  'u',  'v',  'w',  'x',  'y',  'z',
 '0',  '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9',
 '+',  '/',  'A',  'B',  'C',  'D',  'E',  'F',  'G',  'H',
 'I',  'J',  'K',  'L',  'M',  'N',  'O',  'P',  'Q',  'R',
 'S',  'T',  'U',  'V',  'W',  'X',  'Y',  'Z',  'a',  'b',
 'c',  'd',  'e',  'f',  'g',  'h',  'i',  'j',  'k',  'l',
 'm',  'n',  'o',  'p',  'q',  'r',  's',  't',  'u',  'v',
 'w',  'x',  'y',  'z',  '0',  '1',  '2',  '3',  '4',  '5',
 '6',  '7',  '8',  '9',  '+',  '/'
};
static const uint32_t d0[256] = {
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,	0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff,	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,	0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff,	0x01ffffff, 0x000000f8, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x000000fc,
	0x000000d0, 0x000000d4, 0x000000d8, 0x000000dc, 0x000000e0, 0x000000e4,	0x000000e8, 0x000000ec,
	0x000000f0, 0x000000f4, 0x01ffffff, 0x01ffffff,	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x00000000,	0x00000004, 0x00000008, 0x0000000c, 0x00000010, 0x00000014, 0x00000018,
	0x0000001c, 0x00000020, 0x00000024, 0x00000028, 0x0000002c, 0x00000030,	0x00000034, 0x00000038,
	0x0000003c, 0x00000040, 0x00000044, 0x00000048,	0x0000004c, 0x00000050, 0x00000054, 0x00000058,
	0x0000005c, 0x00000060,	0x00000064, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x00000068, 0x0000006c, 0x00000070, 0x00000074, 0x00000078,	0x0000007c, 0x00000080,
	0x00000084, 0x00000088, 0x0000008c, 0x00000090, 0x00000094, 0x00000098, 0x0000009c, 0x000000a0,
	0x000000a4, 0x000000a8, 0x000000ac, 0x000000b0, 0x000000b4, 0x000000b8, 0x000000bc, 0x000000c0,
	0x000000c4, 0x000000c8, 0x000000cc, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff
};
static const uint32_t d1[256] = {
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x0000e003, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x0000f003,
	0x00004003, 0x00005003, 0x00006003, 0x00007003, 0x00008003, 0x00009003, 0x0000a003, 0x0000b003,
	0x0000c003, 0x0000d003, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x00000000, 0x00001000, 0x00002000, 0x00003000, 0x00004000, 0x00005000, 0x00006000,
	0x00007000, 0x00008000, 0x00009000, 0x0000a000, 0x0000b000, 0x0000c000, 0x0000d000, 0x0000e000,
	0x0000f000, 0x00000001, 0x00001001, 0x00002001, 0x00003001, 0x00004001, 0x00005001, 0x00006001,
	0x00007001, 0x00008001, 0x00009001, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x0000a001, 0x0000b001, 0x0000c001, 0x0000d001, 0x0000e001, 0x0000f001, 0x00000002,
	0x00001002, 0x00002002, 0x00003002, 0x00004002, 0x00005002, 0x00006002, 0x00007002, 0x00008002,
	0x00009002, 0x0000a002, 0x0000b002, 0x0000c002, 0x0000d002, 0x0000e002, 0x0000f002, 0x00000003,
	0x00001003, 0x00002003, 0x00003003, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff
};
static const uint32_t d2[256] = {
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
 	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x00800f00, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x00c00f00,
	0x00000d00, 0x00400d00, 0x00800d00, 0x00c00d00, 0x00000e00, 0x00400e00, 0x00800e00, 0x00c00e00,
	0x00000f00, 0x00400f00, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x00000000, 0x00400000, 0x00800000, 0x00c00000, 0x00000100, 0x00400100, 0x00800100,
	0x00c00100, 0x00000200, 0x00400200, 0x00800200, 0x00c00200, 0x00000300, 0x00400300, 0x00800300,
	0x00c00300, 0x00000400, 0x00400400, 0x00800400, 0x00c00400, 0x00000500, 0x00400500, 0x00800500,
	0x00c00500, 0x00000600, 0x00400600, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x00800600, 0x00c00600, 0x00000700, 0x00400700, 0x00800700, 0x00c00700, 0x00000800,
	0x00400800, 0x00800800, 0x00c00800, 0x00000900, 0x00400900, 0x00800900, 0x00c00900, 0x00000a00,
	0x00400a00, 0x00800a00, 0x00c00a00, 0x00000b00, 0x00400b00, 0x00800b00, 0x00c00b00, 0x00000c00,
	0x00400c00, 0x00800c00, 0x00c00c00, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff
};
static const uint32_t d3[256] = {
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x003e0000, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x003f0000,
	0x00340000, 0x00350000, 0x00360000, 0x00370000, 0x00380000, 0x00390000, 0x003a0000, 0x003b0000,
	0x003c0000, 0x003d0000, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x00000000, 0x00010000, 0x00020000, 0x00030000, 0x00040000, 0x00050000, 0x00060000,
	0x00070000, 0x00080000, 0x00090000, 0x000a0000, 0x000b0000, 0x000c0000, 0x000d0000, 0x000e0000,
	0x000f0000, 0x00100000, 0x00110000, 0x00120000, 0x00130000, 0x00140000, 0x00150000, 0x00160000,
	0x00170000, 0x00180000, 0x00190000, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x001a0000, 0x001b0000, 0x001c0000, 0x001d0000, 0x001e0000, 0x001f0000, 0x00200000,
	0x00210000, 0x00220000, 0x00230000, 0x00240000, 0x00250000, 0x00260000, 0x00270000, 0x00280000,
	0x00290000, 0x002a0000, 0x002b0000, 0x002c0000, 0x002d0000, 0x002e0000, 0x002f0000, 0x00300000,
	0x00310000, 0x00320000, 0x00330000, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff,
	0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff, 0x01ffffff
};
//------------------------------------------------------------------------------------------//
void CCY_B64_CTXInit(CCY_BASE64_CTX *ctx){
	ctx->unDoNum = 0;
	ctx->in64 = 0;
}
//------------------------------------------------------------------------------------------//
inline void CCY_EnB64_Calc(CCY_BASE64_CTX *ctx,uint8 *&retBuffer,uint64 *retNum,const uint8 *&data,CCT_BASE64_NL blEnNL){
	uint8	t1, t2, t3;
	t1 = *data++;
	t2 = *data++;
	t3 = *data++;
	*retBuffer++ = e0[t1];
	*retBuffer++ = e1[((t1 & 0x03) << 4) | ((t2 >> 4) & 0x0F)];
	*retBuffer++ = e1[((t2 & 0x0F) << 2) | ((t3 >> 6) & 0x03)];
	*retBuffer++ = e1[t3];
	*retNum += 4;
	if (blEnNL != CCT_NL_NO){
		ctx->in64 += 4;
		if (ctx->in64 == 64){
			*retBuffer++ = '\n';
			ctx->in64 = 0;
			*retNum += 1;
		}
	}
}
//------------------------------------------------------------------------------------------//
const uint64 &CCY_EnB64_Update(CCY_BASE64_CTX *ctx,uint8 *retBuffer,uint64 *retNum,const uint8 *data,uint64 length,CCT_BASE64_NL blEnNL){
	const 	uint8	*dp;
	
	*retNum = 0;
	do{
		while((length > 0) && ((length < 3) || (ctx->unDoNum > 0))){
			ctx->buf[ctx->unDoNum++] = *data++;
			--length;
			if (ctx->unDoNum == 3){
				ctx->unDoNum = 0;
				dp = ctx->buf;
				CCY_EnB64_Calc(ctx,retBuffer,retNum,dp,blEnNL);
			}
		};
		while(length > 2){
			CCY_EnB64_Calc(ctx,retBuffer,retNum,data,blEnNL);
			length -= 3;
		};
	}while(length > 0);
	return(*retNum);
}
//------------------------------------------------------------------------------------------//
const uint64 &CCY_EnB64_Final(CCY_BASE64_CTX *ctx,uint8 *retBuffer,uint64 *retNum,CCT_BASE64_NL blEnNL){
	*retNum = 0;
	switch (ctx->unDoNum) {
		case 0:
			if ((blEnNL != CCT_NL_NO) && (ctx->in64 < 64))
				*retBuffer = '\n';
			break;
		case 1:
			*retBuffer++ = e0[ctx->buf[0]];
			*retBuffer++ = e1[(ctx->buf[0] & 0x03) << 4];
			*retBuffer++ = '=';
			*retBuffer++ = '=';
			*retNum += 4;
			if (blEnNL != CCT_NL_NO){
				*retBuffer = '\n';
				*retNum += 1;
			}
			break;
		case 2:;
		default:
			*retBuffer++ = e0[ctx->buf[0]];
			*retBuffer++ = e1[((ctx->buf[0] & 0x03) << 4) | ((ctx->buf[1] >> 4) & 0x0F)];
			*retBuffer++ = e1[(ctx->buf[1] & 0x0F) << 2];
			*retBuffer++ = '=';
			*retNum += 4;
			if (blEnNL != CCT_NL_NO){
				*retBuffer = '\n';
				*retNum += 1;
			}
	}
	return(*retNum);
}
//------------------------------------------------------------------------------------------//
inline void CCY_DeB64_Calc(CCY_BASE64_CTX *ctx,uint8 *&retBuffer,uint64 *retNum,const uint8 *&data,CCT_BASE64_NL blEnNL){
	uint32	x;
	uint8	x2,x3;

	x  = d0[*data++];
	x |= d1[*data++];
	x2 = *data++;
	x3 = *data++;
	if (x2 == '='){
		*retBuffer++ = (uint8)x;
		++ *retNum;
	}
	else if (x3 == '='){
		x |= d3[x2];
		*retBuffer++ = (uint8)x;
		*retBuffer++ = (uint8)(x >> 8);
		*retNum += 2;
	}
	else{
		x |= d2[x2];
		x |= d3[x3];
		*retBuffer++ = (uint8)x;
		*retBuffer++ = (uint8)(x >> 8);
		*retBuffer++ = (uint8)(x >> 16);
		*retNum += 3;
	}
}
//------------------------------------------------------------------------------------------//
const uint64 &CCY_DeB64_Update(CCY_BASE64_CTX *ctx,uint8 *retBuffer,uint64 *retNum,const uint8 *data,uint64 length,CCT_BASE64_NL blEnNL){
	const 	uint8	*dp;
	
	*retNum = 0;
	do{
		while((length > 0) && ((length < 4) || (ctx->unDoNum > 0))){
			ctx->buf[ctx->unDoNum++] = *data++;
			--length;
			if (ctx->unDoNum == 4){
				ctx->unDoNum = 0;
				dp = ctx->buf;
				CCY_DeB64_Calc(ctx,retBuffer,retNum,dp,blEnNL);
				if ((blEnNL != CCT_NL_NO) && (*data == '\n') && (length > 0)){
					-- length;
					++ data;
				}
			}
		};
		while(length > 3){
			CCY_DeB64_Calc(ctx,retBuffer,retNum,data,blEnNL);
			length -= 4;
			if ((blEnNL != CCT_NL_NO) && (*data == '\n') && (length > 0)){
				-- length;
				++ data;
			}
		};
	}while(length > 0);
	return(*retNum);
}
//------------------------------------------------------------------------------------------//
const uint64& CCY_DeB64_Final(CCY_BASE64_CTX *ctx,uint8 *retBuffer,uint64 *retNum,CCT_BASE64_NL blEnNL){
	const 	uint8	*dp;
	*retNum = 0;
	while(ctx->unDoNum > 1){
		ctx->buf[ctx->unDoNum++] = '=';
		if (ctx->unDoNum == 4){
			ctx->unDoNum = 0;
			dp = ctx->buf;
			CCY_DeB64_Calc(ctx,retBuffer,retNum,dp,blEnNL);
		}
	};
	return(*retNum);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
