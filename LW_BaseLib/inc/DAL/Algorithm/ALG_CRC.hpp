//
//  ALG_CRC.hpp
//  SDT
//
//  Created by Leif Wen on 23/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#ifndef ALG_CRC_hpp
#define ALG_CRC_hpp
//------------------------------------------------------------------------------------------//
#include "ALG_CRC.hpp"
#ifdef ALG_CRC_h
//------------------------------------------------------------------------------------------//
enum {
	CRC_REFIN = BD_FLAG32(0),
	CRC_REFOUT = BD_FLAG32(1),
};
//------------------------------------------------------------------------------------------//
template <typename CRC_BIT> CRC_BIT Reflect(CRC_BIT crc, uint32 bitnum) {
	// reflects the lower 'bitnum' bits of 'crc'
	
	CRC_BIT i, j = 1, crcout = 0;
	
	for (i = (CRC_BIT)1 << (bitnum - 1); i; i >>= 1) {
		if (crc & i)
			crcout |= j;
		j <<= 1;
	}
	return (crcout);
};
//------------------------------------------------------------------------------------------//
template <typename CRC_BIT> void GenerateCRCTable(ALG_CRC_CTX<CRC_BIT>* ctx){
	// make CRC lookup table used by table algorithms
	
	uint32 i, j;
	CRC_BIT bit, crc;
	
	for (i=0; i<256; i++) {
		crc = (CRC_BIT)i;
		if (B_ChkFLAG32(ctx->ref,CRC_REFIN))
			crc = Reflect(crc, 8);
		crc <<= (ctx->width - 8);
		
		for (j=0; j<8; j++) {
			bit = crc & ctx->highbit;
			crc <<= 1;
			if (bit != 0)
				crc ^= ctx->polynomial;
		}
		
		if (B_ChkFLAG32(ctx->ref,CRC_REFIN))
			crc = Reflect(crc, ctx->width);
		ctx->crcTable[i] = crc;
	}
};
//------------------------------------------------------------------------------------------//
template <typename CRC_BIT> void Init(ALG_CRC_CTX<CRC_BIT>* ctx,uint32 polynomial, uint32 initValue, uint32 xorValue,uint32 reflect){
	ctx->polynomial = (CRC_BIT)polynomial;
	ctx->initValue = (CRC_BIT)initValue;
	ctx->xorValue = (CRC_BIT)xorValue;
	
	ctx->ref = 0;
	B_SetFLAG32(ctx->ref,reflect);
	
	ctx->width = 8 * sizeof(CRC_BIT);
	ctx->highbit = 1 << (ctx->width - 1);

	GenerateCRCTable(ctx);
	ctx->result = 0;
	Reset(ctx);
};
//------------------------------------------------------------------------------------------//
template <typename CRC_BIT> void Reset(ALG_CRC_CTX<CRC_BIT>* ctx){
	ctx->remainder = ctx->initValue;
	if (B_ChkFLAG32(ctx->ref,CRC_REFIN))
		ctx->remainder = Reflect(ctx->remainder, ctx->width);
};
//------------------------------------------------------------------------------------------//
static inline void ALG_CRC_Init (ALG_CRC_CTX<uint8>* ctx,uint32 cfg){
	if (ctx->cfg != cfg){
		ALG_CRC8_Init(ctx,cfg);
	}
	else{
		Reset(ctx);
	}
};
//------------------------------------------------------------------------------------------//
static inline void ALG_CRC_Init (ALG_CRC_CTX<uint16>* ctx,uint32 cfg){
	if (ctx->cfg != cfg){
		ALG_CRC16_Init(ctx,cfg);
	}
	else{
		Reset(ctx);
	}
};
//------------------------------------------------------------------------------------------//
static inline void ALG_CRC_Init (ALG_CRC_CTX<uint32>* ctx,uint32 cfg){
	if (ctx->cfg != cfg){
		ALG_CRC32_Init(ctx,cfg);
	}
	else{
		Reset(ctx);
	}
};
//------------------------------------------------------------------------------------------//
template <typename CRC_BIT> static inline void ALG_CRC_Update(ALG_CRC_CTX<CRC_BIT>* ctx,const uint8* data,const uint64& length){
	// fast lookup table algorithm without augmented zero bytes, e.g. used in pkzip.
	// only usable with polynom orders of 8, 16, 24 or 32.
	uint64 num = length;
	
	if (B_ChkFLAG32(ctx->ref,CRC_REFIN) == G_FALSE){
		while (num --)
			ctx->remainder = (ctx->remainder << 8) ^ ctx->crcTable[((ctx->remainder >> (ctx->width - 8)) & 0xff) ^ *data++];
	}
	else {
		while (num --)
			ctx->remainder = (ctx->remainder >> 8) ^ ctx->crcTable[ (ctx->remainder & 0xff) ^ *data++];
	}
};
//------------------------------------------------------------------------------------------//
template <typename CRC_BIT> static inline CRC_BIT ALG_CRC_Final(ALG_CRC_CTX<CRC_BIT>* ctx){
	
	if ((B_ChkFLAG32(ctx->ref,CRC_REFIN) ^ B_ChkFLAG32(ctx->ref,CRC_REFOUT)))
		ctx->remainder = Reflect(ctx->remainder, ctx->width);
	ctx->remainder ^= ctx->xorValue;
	ctx->result = ctx->remainder;
	Reset(ctx);
	return(ctx->result);
};
//------------------------------------------------------------------------------------------//
template <typename CRC_BIT> uint64 CalcBitByBit(ALG_CRC_CTX<CRC_BIT>* ctx,uint32 bitnum,uint8 *data, uint64 length){
	// fast bit by bit algorithm without augmented zero bytes.
	// does not use lookup table, suited for polynom orders between 1...32.
	
	uint64 i, j, c, bit;
	uint64 crc,crchighbit,crcmask;
	
	crcmask = ((((unsigned long)1 << (bitnum - 1)) - 1) << 1) | 1;
	crchighbit = (unsigned long)1 << (bitnum - 1);
	
	crc = ctx->nitValue;
	
	for (i=0; i<length; i++) {
		
		c = (unsigned long)*data++;
		if (B_ChkFLAG32(ctx->ref,CRC_REFIN))
			c = Reflect(c, 8);
		
		for (j=0x80; j; j>>=1) {
			bit = crc & crchighbit;
			crc <<= 1;
			if (c & j)
				bit ^= crchighbit;
			if (bit)
				crc ^= ctx->polynomial;
		}
	}
	
	if (B_ChkFLAG32(ctx->ref,CRC_REFOUT))
		crc = Reflect(crc, bitnum);
	crc ^= ctx->xorValue;
	crc &= crcmask;
	
	return(crc);
};
#ifdef ALG_DS_CRC
//------------------------------------------------------------------------------------------//
template <typename ALGCTX> ALG_CRC_T<ALGCTX>::ALG_CRC_T(void) : DSTF(){
	cgCTX.cfg = 0;
	TNFP::SetSelfName("CRC");
};
//------------------------------------------------------------------------------------------//
template <typename ALGCTX> inline ALG_CRC_T<ALGCTX>& ALG_CRC_T<ALGCTX>::InitCFG(uint32 cfg,const void* p){
	DSTF::InitCFG(cfg,p);
	ALG_CRC_Init(&cgCTX,cfg);
	return(*this);
};
//------------------------------------------------------------------------------------------//
template <typename ALGCTX> inline ALG_CRC_T<ALGCTX>& ALG_CRC_T<ALGCTX>::InitSize(uint32 size){
	return(*this);
};
//------------------------------------------------------------------------------------------//
template <typename ALGCTX>
inline IOSE ALG_CRC_T<ALGCTX>::DoTransform(IOS* _ios,const UVOut& _out,const uint8* data,const uint64& length){
	ALG_CRC_Update(&cgCTX,data,length);
	return(Save(_ios,_out,data,length));
};
//------------------------------------------------------------------------------------------//
template <typename ALGCTX>
inline IOSE ALG_CRC_T<ALGCTX>::DoFinal(IOS* _ios,const UVOut& _out){
	ALG_CRC_Final(&cgCTX);
	return(DSTF::DoFinal(_ios,_out));
};
//------------------------------------------------------------------------------------------//
template <typename ALGCTX> inline uint32 ALG_CRC_T<ALGCTX>::Calc(IOS* _ios,const UVOut& _out,const UVIn& _in){
	AllIn(_ios,_out,_in);
	return(GetCRCReasult());
};
//------------------------------------------------------------------------------------------//
template <typename ALGCTX> inline uint32 ALG_CRC_T<ALGCTX>::GetCRCReasult(void){
	return(cgCTX.result);
};
//------------------------------------------------------------------------------------------//
template <typename ALGCTX> inline uint32 ALG_CRC_T<ALGCTX>::GetCRCBit(void){
	return(cgCTX.width / 8);
};
//------------------------------------------------------------------------------------------//
#endif
#endif /* ALG_CRC_h */
#endif /* ALG_CRC_hpp */
