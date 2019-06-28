//
//  BIC_Calc.h
//  SDT
//
//  Created by Leif Wen on 11/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#include "BIC_Define.h"
#include "ALG_Digest.h"
#include "ALG_BASE64.h"
//------------------------------------------------------------------------------------------//
#ifdef BIC_Define_h
//------------------------------------------------------------------------------------------//
#ifndef BIC_Calc_h
#define BIC_Calc_h
#ifdef BIC_Calc_h
//------------------------------------------------------------------------------------------//
BIC_CLASSTYPE(CALC_HEX2DEC,	"hex2dec");
BIC_CLASSTYPE(CALC_DEC2HEX,	"dec2hex");
#ifdef ALG_DS_DIGEST
	BIC_CLASSTYPE(CALC_MD5,		"md5");
	BIC_CLASSTYPE(CALC_SHA1,	"sha1");
	BIC_CLASSTYPE(CALC_SHA224,	"sha224");
	BIC_CLASSTYPE(CALC_SHA256,	"sha256");
	BIC_CLASSTYPE(CALC_SHA384,	"sha384");
	BIC_CLASSTYPE(CALC_SHA512,	"sha512");
#endif
#ifdef ALG_BASE64_h
	BIC_CLASSTYPE(CALC_BASE64,	"base64");
#endif
//------------------------------------------------------------------------------------------//
class BIC_CALC : public BIC_BASE_S{
	public:
				 BIC_CALC(void);
		virtual ~BIC_CALC(void){;};
	public:
		
	public:
		BIC_CALC_HEX2DEC		cgSub_hex2dec;
		BIC_CALC_DEC2HEX		cgSub_dec2hex;
#ifdef ALG_DS_DIGEST
		BIC_CALC_MD5			cgSub_md5;
		BIC_CALC_SHA1			cgSub_sha1;
		BIC_CALC_SHA224			cgSub_sha224;
		BIC_CALC_SHA256			cgSub_sha256;
		BIC_CALC_SHA384			cgSub_sha384;
		BIC_CALC_SHA512			cgSub_sha512;
#endif
#ifdef ALG_BASE64_h
		BIC_CALC_BASE64			cgSub_base64;
#endif
};
//------------------------------------------------------------------------------------------//
#endif /* BIC_Calc_h */
#endif /* BIC_Calc_h */
#endif /* BIC_Define_h */
