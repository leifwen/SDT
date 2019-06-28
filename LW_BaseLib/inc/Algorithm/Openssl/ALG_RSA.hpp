//
//  ALG_RSA.hpp
//  SDT
//
//  Created by Leif Wen on 23/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#ifndef ALG_RSA_hpp
#define ALG_RSA_hpp
//------------------------------------------------------------------------------------------//
#include "ALG_RSA.h"
#ifdef ALG_RSA_h
//------------------------------------------------------------------------------------------//
typedef  int(*ALG_RSA_FUN)(int flen, const unsigned char *from,unsigned char *to, RSA *rsa, int padding);
const STDSTR& ALG_RSA_DO(STDSTR* _out,ALG_RSA_FUN rsaFun,const RSA* _rsa,const uint8* data,uint32 length);
//------------------------------------------------------------------------------------------//
static inline const STDSTR& ALG_RSA_Encrypt_Prk(STDSTR* _out,const RSA* _rsa,const uint8* data,uint32 length){
	
	return(ALG_RSA_DO(_out,RSA_private_encrypt,_rsa,data,length));
};
//------------------------------------------------------------------------------------------//
static inline const STDSTR& ALG_RSA_Decrypt_Prk(STDSTR* _out,const RSA* _rsa,const uint8* data,uint32 length){
	
	return(ALG_RSA_DO(_out,RSA_private_decrypt,_rsa,data,length));
};
//------------------------------------------------------------------------------------------//
static inline const STDSTR& ALG_RSA_Encrypt_Puk(STDSTR* _out,const RSA* _rsa,const uint8* data,uint32 length){
	
	return(ALG_RSA_DO(_out,RSA_public_encrypt,_rsa,data,length));
};
//------------------------------------------------------------------------------------------//
static inline const STDSTR& ALG_RSA_Decrypt_Puk(STDSTR* _out,const RSA* _rsa,const uint8* data,uint32 length){
	
	return(ALG_RSA_DO(_out,RSA_public_decrypt,_rsa,data,length));
};
//------------------------------------------------------------------------------------------//
static inline const STDSTR&	ALG_RSA_Encrypt_Prk(STDSTR* _out,const RSA* _rsa,const STDSTR& _in){

	return(ALG_RSA_DO(_out,RSA_private_encrypt,_rsa,(uint8*)_in.c_str(),(uint32)_in.length()));
};
//------------------------------------------------------------------------------------------//
static inline const STDSTR&	ALG_RSA_Decrypt_Prk(STDSTR* _out,const RSA* _rsa,const STDSTR& _in){
	
	return(ALG_RSA_DO(_out,RSA_private_decrypt,_rsa,(uint8*)_in.c_str(),(uint32)_in.length()));
};
//------------------------------------------------------------------------------------------//
static inline const STDSTR&	ALG_RSA_Encrypt_Puk(STDSTR* _out,const RSA* _rsa,const STDSTR& _in){
	
	return(ALG_RSA_DO(_out,RSA_public_encrypt,_rsa,(uint8*)_in.c_str(),(uint32)_in.length()));
};
//------------------------------------------------------------------------------------------//
static inline const STDSTR&	ALG_RSA_Decrypt_Puk(STDSTR* _out,const RSA* _rsa,const STDSTR& _in){
	
	return(ALG_RSA_DO(_out,RSA_public_decrypt,_rsa,(uint8*)_in.c_str(),(uint32)_in.length()));
};
//------------------------------------------------------------------------------------------//
static inline bool32 ALG_RSA_Decode_Prk(RSA** _out,const uint8* data,uint32 num){

	return(d2i_RSAPrivateKey(_out,&data,(uint32)num) != nullptr);
};
//------------------------------------------------------------------------------------------//
static inline bool32 ALG_RSA_Decode_Puk(RSA** _out,const uint8* data,uint32 num){

	return(d2i_RSAPublicKey(_out,&data,(uint32)num) != nullptr);
};
//------------------------------------------------------------------------------------------//
static inline bool32 ALG_RSA_Decode_Prk(RSA** _out,const STDSTR& _in){
	
	return(ALG_RSA_Decode_Prk(_out,(uint8*)_in.c_str(),(uint32)_in.length()));
};
//------------------------------------------------------------------------------------------//
static inline bool32 ALG_RSA_Decode_Puk(RSA** _out,const STDSTR& _in){
	
	return(ALG_RSA_Decode_Puk(_out,(uint8*)_in.c_str(),(uint32)_in.length()));
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif /* ALG_RSA_h */
#endif /* ALG_RSA_hpp */
