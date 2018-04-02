//
//  ALG_zlib.cpp
//  SDT
//
//  Created by Leif Wen on 20/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
#include "Config.h"
//------------------------------------------------------------------------------------------//
//#define NOZLIBTEST
#ifndef	NOZLIBTEST
#ifdef CommonDefH_VC
	#define ZLIB_WINAPI
	//#pragma comment(lib, "zlibstat_d.lib")
	#pragma comment(lib, "zlibstat.lib")
#endif
#endif
#include "ALG_zlib.h"
#ifdef ALG_zlib_h
//------------------------------------------------------------------------------------------//
#define	uInt_MaxSize	((uInt)-1)

//#define LOGPRINT_ENABLE
#include "SYS_Log.h"
//------------------------------------------------------------------------------------------//
void ALG_Zlib_Init(ALG_ZLIB_CTX* ctx,uint32 cfg){
#ifndef	NOZLIBTEST
	B_ClrFLAG32(cfg, DSTF::CFG_DSTF_CTRL | DSTF::CFG_INIT);
	
	if (B_ChkFLAG32(ctx->cfg, DSTF::CFG_INIT) && (ctx->cfg == (cfg | DSTF::CFG_INIT)))
		return;

	ALG_Zlib_Release(ctx);
	
	ctx->cfg = cfg;
	
	ALG_Zlib_ReInit(ctx);
#endif
}
//------------------------------------------------------------------------------------------//
bool32 ALG_Zlib_ReInit(ALG_ZLIB_CTX* ctx){
#ifndef	NOZLIBTEST
	
	ALG_Zlib_Release(ctx);
	
	if (B_ChkFLAG32(ctx->cfg,(~DSTF::CFG_INIT_CFG) & ALG_ZLIB::CFG_Compress)){
		if (B_ChkFLAG32(ctx->cfg,(~DSTF::CFG_INIT_CFG) & ALG_ZLIB::CFG_NO_COMPRESSION)){
			deflateInit(&ctx->ctx, Z_NO_COMPRESSION);
		}
		else if (B_ChkFLAG32(ctx->cfg,(~DSTF::CFG_INIT_CFG) & ALG_ZLIB::CFG_BEST_SPEED)){
			deflateInit(&ctx->ctx, Z_BEST_SPEED);
		}
		else if (B_ChkFLAG32(ctx->cfg,(~DSTF::CFG_INIT_CFG) & ALG_ZLIB::CFG_BEST_COMPRESSION)){
			deflateInit(&ctx->ctx, Z_BEST_COMPRESSION);
		}
		else {
			deflateInit(&ctx->ctx, Z_DEFAULT_COMPRESSION);
		}
	}
	else{
		inflateInit(&ctx->ctx);
	}
	ctx->cfg |= DSTF::CFG_INIT;
#endif
	return G_TRUE;
}
//------------------------------------------------------------------------------------------//
bool32 ALG_Zlib_Update(ALG_ZLIB_CTX* ctx,uint8* _out,const uint64& outSize,const uint8* data,const uint64& length){
#ifdef NOZLIBTEST
	IOSTATUS ios;
	ctx->avail_in = length;
	ctx->avail_out = outSize;

	DS_IO_NODE::GetDSIOList().Save(IOSTATUS_Clr(&ios), OUD_CHARS(_out,outSize), IUD(data,length));
	
	ctx->avail_in	-= ios.total_in;
	ctx->total_in	+= ios.total_in;

	ctx->avail_out	-= ios.total_out;
	ctx->total_out	+= ios.total_out;
	
	ctx->next_in	= (uint8*)(data + ios.total_in);
	ctx->next_out	= (uint8*)(_out + ios.total_out);
	return 1;
#else
	bool32 err;

	ctx->ctx.next_in = (uint8*)data;
	ctx->ctx.next_out = _out;

	ctx->avail_in = length;
	ctx->avail_out = outSize;
	do{
		ctx->ctx.avail_in = (uint32)(ctx->avail_in < uInt32_MaxSize ? ctx->avail_in : uInt32_MaxSize);
		ctx->ctx.total_in = 0;
		
		ctx->ctx.avail_out = (uint32)(ctx->avail_out < uInt32_MaxSize ? ctx->avail_out : uInt32_MaxSize);
		ctx->ctx.total_out = 0;
		
		if (B_ChkFLAG32(ctx->cfg,(~DSTF::CFG_INIT_CFG) & ALG_ZLIB::CFG_Compress)){
			err = deflate(&ctx->ctx,Z_NO_FLUSH);
		}
		else{
			err = inflate(&ctx->ctx,Z_NO_FLUSH);
		}
		
		ctx->avail_in	-= ctx->ctx.total_in;
		ctx->total_in	+= ctx->ctx.total_in;
		
		ctx->avail_out	-= ctx->ctx.total_out;
		ctx->total_out	+= ctx->ctx.total_out;
	}while((ctx->avail_in) > 0 && (ctx->avail_out > 0) && (ctx->avail_out == 0));
	
	ctx->next_in	=  ctx->ctx.next_in;
	ctx->next_out	=  ctx->ctx.next_out;
	return(err + 1);
#endif
};
//------------------------------------------------------------------------------------------//
bool32 ALG_Zlib_Final(ALG_ZLIB_CTX* ctx,uint8* _out,const uint64& outSize){
#ifdef NOZLIBTEST
	ctx->avail_in = 0;
	ctx->total_in = 0;
	
	ctx->avail_out = 0;
	ctx->total_out = 0;
	
	ctx->next_in	= nullptr;
	ctx->next_out	= _out;
	return 2;
#else
	bool32 err;
	
	ctx->ctx.next_in = nullptr;
	ctx->ctx.next_out = _out;
	
	ctx->avail_in = 0;
	ctx->avail_out = outSize;
	do{
		ctx->ctx.avail_in = 0;
		ctx->ctx.total_in = 0;
		
		ctx->ctx.avail_out = (uint32)(ctx->avail_out < uInt32_MaxSize ? ctx->avail_out : uInt32_MaxSize);
		ctx->ctx.total_out = 0;
		
		if (B_ChkFLAG32(ctx->cfg,(~DSTF::CFG_INIT_CFG) & ALG_ZLIB::CFG_Compress)){
			err = deflate(&ctx->ctx,Z_FINISH);
		}
		else{
			err = inflate(&ctx->ctx,Z_FINISH);
		}
		
		ctx->avail_in	-= ctx->ctx.total_in;
		ctx->total_in	+= ctx->ctx.total_in;
		
		ctx->avail_out	-= ctx->ctx.total_out;
		ctx->total_out	+= ctx->ctx.total_out;
	}while((ctx->avail_out > 0) && (err == Z_OK));
	
	ctx->next_in	=  ctx->ctx.next_in;
	ctx->next_out	=  ctx->ctx.next_out;

	return(err + 1);
#endif
};
//------------------------------------------------------------------------------------------//
bool32 ALG_Zlib_Release(ALG_ZLIB_CTX* ctx){
#ifndef NOZLIBTEST
	if (B_ChkFLAG32(ctx->cfg, ALG_ZLIB::CFG_INIT)){
		if (B_ChkFLAG32(ctx->cfg, (~DSTF::CFG_INIT_CFG) & ALG_ZLIB::CFG_Compress)){
			deflateEnd(&ctx->ctx);
		}
		else{
			inflateEnd(&ctx->ctx);
		}
		B_ClrFLAG32(ctx->cfg, ALG_ZLIB::CFG_INIT);
	}
	ctx->ctx.zalloc = Z_NULL;
	ctx->ctx.zfree = Z_NULL;
	ctx->ctx.opaque = Z_NULL;
#endif
	return ZLIB_OK;
}
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
static inline void ALG_Zlib_Init(void* ctx,uint32 cfg,const void* p){
	ALG_Zlib_Init((ALG_ZLIB_CTX*)ctx,cfg);
	ELog(((ALG_ZLIB_CTX*)ctx)->dstf << "Init():  "
		 << (B_ChkFLAG32(((ALG_ZLIB_CTX*)ctx)->cfg, (~DSTF::CFG_INIT_CFG) & ALG_ZLIB::CFG_Compress) ? "Compress  " : "Uncompress"));
};
//------------------------------------------------------------------------------------------//
static inline bool32 ALG_Zlib_Update(void* ctx,uint8* _out,const uint64& outSize,const uint8* data,const uint64& length){
	ELog(((ALG_ZLIB_CTX*)ctx)->dstf << "Update():"
		 << (B_ChkFLAG32(((ALG_ZLIB_CTX*)ctx)->cfg, (~DSTF::CFG_INIT_CFG) & ALG_ZLIB::CFG_Compress) ? "Compress  " : "Uncompress") << ",len=" << length);
	return(ALG_Zlib_Update((ALG_ZLIB_CTX*)ctx,_out,outSize,data,length));
};
//------------------------------------------------------------------------------------------//
static inline bool32 ALG_Zlib_Final(void* ctx,uint8* _out,const uint64& outSize,const uint8* data,const uint64& length){
	ELog(((ALG_ZLIB_CTX*)ctx)->dstf << "Final(): "
		 << (B_ChkFLAG32(((ALG_ZLIB_CTX*)ctx)->cfg, (~DSTF::CFG_INIT_CFG) & ALG_ZLIB::CFG_Compress) ? "Compress  " : "Uncompress"));
	return(ALG_Zlib_Final((ALG_ZLIB_CTX*)ctx,_out,outSize));
};
//------------------------------------------------------------------------------------------//
static inline bool32 ALG_Zlib_Release(void* ctx){
	ELog(((ALG_ZLIB_CTX*)ctx)->dstf << "Release()");
	ALG_Zlib_Release((ALG_ZLIB_CTX*)ctx);
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
static inline bool32 ALG_Zlib_ReInit(void* ctx){
	ELog(((ALG_ZLIB_CTX*)ctx)->dstf << "ReInit()");
	ALG_Zlib_ReInit((ALG_ZLIB_CTX*)ctx);
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
ALG_ZLIB::ALG_ZLIB(void) : DSTF_DIR(){
	cgCTX.Init		= ALG_Zlib_Init;
	cgCTX.Update	= ALG_Zlib_Update;
	cgCTX.Final		= ALG_Zlib_Final;
	cgCTX.Release	= ALG_Zlib_Release;
	cgCTX.ReInit	= ALG_Zlib_ReInit;
	TNFP::SetSelfName("ZLIB");
}
//------------------------------------------------------------------------------------------//
ALG_ZLIB::~ALG_ZLIB(void){
	ALG_Zlib_Release(&cgCTX);
}
//------------------------------------------------------------------------------------------//
#endif
