//
//  ALG_Digest.hpp
//  SDT
//
//  Created by Leif Wen on 23/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#ifndef ALG_Digest_hpp
#define ALG_Digest_hpp
//------------------------------------------------------------------------------------------//
#include "ALG_Digest.h"
#ifdef ALG_Digest_h
//------------------------------------------------------------------------------------------//
Define_ALG_FUN(SHA1);
Define_ALG_FUN(SHA224);
Define_ALG_FUN(SHA256);
Define_ALG_FUN(SHA384);
Define_ALG_FUN(SHA512);
Define_ALG_FUN(MD5);

Define_ALG_Digest0(SHA1);
Define_ALG_Digest0(SHA224);
Define_ALG_Digest0(SHA256);
Define_ALG_Digest0(SHA384);
Define_ALG_Digest0(SHA512);
Define_ALG_Digest0(MD5);

Define_ALG_Digest1(SHA1);
Define_ALG_Digest1(SHA224);
Define_ALG_Digest1(SHA256);
Define_ALG_Digest1(SHA384);
Define_ALG_Digest1(SHA512);
Define_ALG_Digest1(MD5);

Define_ALG_Digest2(SHA1);
Define_ALG_Digest2(SHA224);
Define_ALG_Digest2(SHA256);
Define_ALG_Digest2(SHA384);
Define_ALG_Digest2(SHA512);
Define_ALG_Digest2(MD5);

Define_ALG_Digest3(SHA1);
Define_ALG_Digest3(SHA224);
Define_ALG_Digest3(SHA256);
Define_ALG_Digest3(SHA384);
Define_ALG_Digest3(SHA512);
Define_ALG_Digest3(MD5);
//------------------------------------------------------------------------------------------//
template <typename ALGCTX> static inline void ALG_Digest_Update64(ALGCTX* ctx,const uint8* data,uint64 length){
	CC_LONG		packageLength;
	while(length > 0){
		packageLength = (length > 0xffffffff) ? (0xffffffff): (CC_LONG)length;
		length -= packageLength;
		ALG_Digest_Update32(ctx,data,packageLength);
		data += packageLength;
	};
}
//------------------------------------------------------------------------------------------//
template <typename ALGCTX> static inline STDSTR ALG_Digest64(ALGCTX* ctxnullptr,const uint8* data,uint64 length){
	ALGCTX	ctx;
	uint8	result[CC_SHA512_DIGEST_LENGTH];
	STDSTR	retStr = "";
	if (length > 0){
		if (length > 0xffffffff){
			ALG_Digest_Update64(&ctx,data,length);
			ALG_Digest_Final(&ctx,result);
		}
		else{
			ALG_Digest32(&ctx,result,data,(CC_LONG)length);
		}
		retStr.append((char*)result,ALG_Digest_Size(&ctx));
	}
	return(retStr);
}
//------------------------------------------------------------------------------------------//
#ifdef DS_Transform_h
//------------------------------------------------------------------------------------------//
template <typename ALGCTX> ALG_DIGEST_T<ALGCTX>::ALG_DIGEST_T(void) : DSTF(){
	InitSize(ALG_Digest_Size(&cgCTX));
	TNFP::SetSelfName("DIGEST");
};
//------------------------------------------------------------------------------------------//
template <typename ALGCTX> inline ALG_DIGEST_T<ALGCTX>& ALG_DIGEST_T<ALGCTX>::InitCFG(uint32 cfg,const void* p){
	DSTF::InitCFG(cfg,p);
	ALG_Digest_Init(&cgCTX);
	return(*this);
};
//------------------------------------------------------------------------------------------//
template <typename ALGCTX> inline ALG_DIGEST_T<ALGCTX>& ALG_DIGEST_T<ALGCTX>::InitSize(uint32 size){
	DSTF::InitSize(size);
	return(*this);
};
//------------------------------------------------------------------------------------------//
template <typename ALGCTX>
inline ioss ALG_DIGEST_T<ALGCTX>::DoTransform(IOSTATUS* _ios,const UVOut& _out,const uint8* data,const uint64& length){
	ALG_Digest_Update64(&cgCTX,data,length);
	return(Save(_ios,_out,data,length));
};
//------------------------------------------------------------------------------------------//
template <typename ALGCTX>
inline ioss ALG_DIGEST_T<ALGCTX>::DoFinal(IOSTATUS* _ios,const UVOut& _out){
	cgArray.Reset();
	ALG_Digest_Final(&cgCTX,cgArray.GetPointer(0));
	return(DSTF::DoFinal(_ios,_out));
};
//------------------------------------------------------------------------------------------//
template <typename ALGCTX>
inline ALG_DIGEST_T<ALGCTX>& ALG_DIGEST_T<ALGCTX>::GetResult(IOSTATUS* _ios,const UVOut& _result){
	Save(_ios,_result,cgArray.GetPointer(0),ALG_Digest_Size(&cgCTX));
	return(*this);
};
//------------------------------------------------------------------------------------------//
template <typename ALGCTX>
inline ALG_DIGEST_T<ALGCTX>& ALG_DIGEST_T<ALGCTX>::Calc(IOSTATUS* _ios,const UVOut& _result,const UVOut& _out,const UVIn& _in){
	ALG_Digest_Init(&cgCTX);
	AllIn(_ios, _out, _in);
	GetResult(nullptr,_result);
	return(*this);
};
//------------------------------------------------------------------------------------------//
template <typename ALGCTX>
inline uint32 ALG_DIGEST_T<ALGCTX>::GetResultSize(void){
	return(ALG_Digest_Size(&cgCTX));
};
//------------------------------------------------------------------------------------------//
#endif
//------------------------------------------------------------------------------------------//
#endif /* ALG_Digest_h */
#endif /* ALG_Digest_hpp */
