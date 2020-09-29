//
//  ALG_RSA.cpp
//  SDT
//
//  Created by Leif Wen on 23/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "ALG_RSA.h"
//------------------------------------------------------------------------------------------//
#ifdef ALG_RSA_h
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_VC
	#pragma comment(lib, "libeay32.lib")
	#pragma comment(lib, "ssleay32.lib")
#endif
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
STDSTR ALG_CreateRandKey(uint32 length){
	STDSTR	ret;
	uint32	rLength;
	char 	buf[64];
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
		ret.append(buf,rLength);
		length -= rLength;
	}while(length > 0);
	return(ret);
};
//------------------------------------------------------------------------------------------//
bool32 ALG_RSA_CreateKey(RSA* _out,int bits){
	bool32		ret;
	BN_ULONG	w;
	BIGNUM		*bne;
	
	ret = 0;
	bne = nullptr;
	w = RSA_3;
	bne = BN_new();
	if (bne != nullptr){
		if (BN_set_word(bne,w) != 0){
			ret = RSA_generate_key_ex(_out,bits,bne,nullptr);
		}
		BN_free(bne);
	}
	return(ret);
};
//------------------------------------------------------------------------------------------//
bool32 ALG_RSA_WR_Prk_PEM(RSA* _in,const STDSTR& fileName){
	bool32	ret;
	BIO		*tBIO;
	
	ret = 0;
	tBIO = BIO_new_file(fileName.c_str(),"w");
	if (tBIO != nullptr){
		ret = PEM_write_bio_RSAPrivateKey(tBIO,_in,nullptr,nullptr,0,nullptr,nullptr);
		ret = BIO_flush(tBIO);
		BIO_free(tBIO);
	}
	return(ret);
};
//------------------------------------------------------------------------------------------//
bool32 ALG_RSA_RD_Prk_PEM(RSA** _out,const STDSTR& fileName){
	BIO *tBIO;
	RSA	*readRet;
	
	readRet = nullptr;
	tBIO = nullptr;
	tBIO = BIO_new_file(fileName.c_str(),"rb");
	if (tBIO != nullptr){
		readRet = PEM_read_bio_RSAPrivateKey(tBIO,_out,nullptr,nullptr);
		BIO_free(tBIO);
	}
	return(readRet != nullptr);
};
//------------------------------------------------------------------------------------------//
bool32 ALG_RSA_WR_Puk_PEM(RSA* _in,const STDSTR& fileName){
	bool32	ret;
	BIO		*tBIO;
	
	ret = 0;
	tBIO = BIO_new_file(fileName.c_str(),"w");
	if (tBIO != nullptr){
		ret = PEM_write_bio_RSAPublicKey(tBIO,_in);
		ret = BIO_flush(tBIO);
		BIO_free(tBIO);
	}
	return(ret);
};
//------------------------------------------------------------------------------------------//
bool32 ALG_RSA_RD_Puk(RSA** _out,const STDSTR& fileName){
	BIO *tBIO;
	RSA	*readRet;
	
	readRet = nullptr;
	tBIO = BIO_new_file(fileName.c_str(),"rb");
	if (tBIO != nullptr){
		readRet = PEM_read_bio_RSAPublicKey(tBIO,_out,nullptr,nullptr);
		BIO_free(tBIO);
	}
	return(readRet != nullptr);
};
//------------------------------------------------------------------------------------------//
const STDSTR& ALG_RSA_DO(STDSTR* _out,ALG_RSA_FUN rsaFun,const RSA* _rsa,const uint8* data,uint32 length){
	uint8	*bufout;
	uint32	len;

	len = RSA_size(_rsa);
	bufout = new uint8[len];
	if (bufout!= nullptr){
		len = rsaFun(length,data,bufout,(RSA*)_rsa,RSA_PKCS1_PADDING);
		if (len > 0)
			_out->append((char*)bufout,len);
		delete []bufout;
	}
	return(*_out);
};
//------------------------------------------------------------------------------------------//
const STDSTR& ALG_RSA_Encode_Prk(STDSTR* _out,const RSA* _in){
	uint8	bufout[1024 * 2],*p;
	uint32	len;

	p = bufout;
	len = i2d_RSAPrivateKey(_in,&p);
	if (len > 0)
		_out->append((char*)bufout,len);
	return(*_out);
};
//------------------------------------------------------------------------------------------//
const STDSTR& ALG_RSA_Encode_Puk(STDSTR* _out,const RSA* _in){
	uint8	bufout[1024 * 2],*p;
	uint32	len;

	p = bufout;
	len = i2d_RSAPublicKey(_in,&p);
	if (len > 0)
		_out->append((char*)bufout,len);
	return(*_out);
};
//------------------------------------------------------------------------------------------//
#endif /* ALG_RSA_h */
