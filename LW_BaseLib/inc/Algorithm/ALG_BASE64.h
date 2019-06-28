//
//  ALG_BASE64.h
//  SDT
//
//  Created by Leif Wen on 21/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#include "DS_Transform.h"
//------------------------------------------------------------------------------------------//
#ifdef DS_Transform_h
//------------------------------------------------------------------------------------------//
#ifndef ALG_BASE64_h
#define ALG_BASE64_h
#ifdef ALG_BASE64_h
//------------------------------------------------------------------------------------------//
struct ALG_BASE64_CTX : public DSTF_DIR_CTX{	
	uint32	outMinSize;
	uint8	buf[4];
	uint32	unDoNum;
	int32	in64;
};
//------------------------------------------------------------------------------------------//
void	ALG_B64_Init	(ALG_BASE64_CTX* ctx,uint32 cfg);
IOSE	ALG_B64_Update	(ALG_BASE64_CTX* ctx,uint8* _out,const uint64& outSize,const uint8* data,const uint64& length);
IOSE	ALG_B64_Final	(ALG_BASE64_CTX* ctx,uint8* _out,const uint64& outSize);
IOSE	ALG_B64_Release	(ALG_BASE64_CTX* ctx);
IOSE	ALG_B64_ReInit	(ALG_BASE64_CTX* ctx);
//------------------------------------------------------------------------------------------//
class ALG_BASE64 : public DSTF_DIR<ALG_BASE64_CTX>{
	protected:
		enum	{CFG_C = 1, CFG_S = DSTF_DIR::CFG_S + DSTF_DIR::CFG_C};
	public:
		enum	{CFG_Encode = CFG_INIT_CFG | CFG_DIR0
				,CFG_Decode = CFG_INIT_CFG |  CFG_DIR1
				,CFG_NL		= CFG_INIT_CFG | CGF_CREATE(0)
				,CFG_NONL 	= CFG_INIT_CFG
		};
	public:
				 ALG_BASE64(void);
		virtual ~ALG_BASE64(void);
};
//------------------------------------------------------------------------------------------//
#endif /* ALG_BASE64_h */
#endif /* ALG_BASE64_h */
#endif
