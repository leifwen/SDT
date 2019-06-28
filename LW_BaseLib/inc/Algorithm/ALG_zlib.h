//
//  ALG_zlib.h
//  SDT
//
//  Created by Leif Wen on 20/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#include "DS_Transform.h"
//------------------------------------------------------------------------------------------//
#ifdef DS_Transform_h
//------------------------------------------------------------------------------------------//
#ifndef ALG_zlib_h
#define ALG_zlib_h
#ifdef ALG_zlib_h
#include "zlib.h"
//------------------------------------------------------------------------------------------//
struct ALG_ZLIB_CTX : public DSTF_DIR_CTX{
	z_stream ctx;
};
//------------------------------------------------------------------------------------------//
void	ALG_Zlib_Init			(ALG_ZLIB_CTX* ctx,uint32 cfg);
IOSE	ALG_Zlib_Update			(ALG_ZLIB_CTX* ctx,uint8* _out,const uint64& outSize,const uint8* data,const uint64& length);
IOSE	ALG_Zlib_Final			(ALG_ZLIB_CTX* ctx,uint8* _out,const uint64& outSize);
IOSE	ALG_Zlib_Release		(ALG_ZLIB_CTX* ctx);
IOSE	ALG_Zlib_ReInit			(ALG_ZLIB_CTX* ctx);
//------------------------------------------------------------------------------------------//
class ALG_ZLIB : public DSTF_DIR<ALG_ZLIB_CTX>{
	protected:
		enum	{CFG_C = 3, CFG_S = DSTF_DIR::CFG_S + DSTF_DIR::CFG_C};
	public:
		enum	{CFG_Compress				= CFG_INIT_CFG | CFG_DIR0
				,CFG_Uncompress				= CFG_INIT_CFG | CFG_DIR1
				,CFG_NO_COMPRESSION			= CFG_INIT_CFG | CGF_CREATE(0)
				,CFG_BEST_SPEED				= CFG_INIT_CFG | CGF_CREATE(1)
				,CFG_BEST_COMPRESSION		= CFG_INIT_CFG | CGF_CREATE(2)
				,CFG_DEFAULT_COMPRESSION	= CFG_INIT_CFG
		};
	public:
				 ALG_ZLIB(void);
		virtual ~ALG_ZLIB(void);
};
//------------------------------------------------------------------------------------------//
#endif /* ALG_zlib_h */
#endif /* ALG_zlib_h */
#endif
