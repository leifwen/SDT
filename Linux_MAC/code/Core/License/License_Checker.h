/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: License_Checker.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.03.26
*/

//------------------------------------------------------------------------------------------//
#include "Comm_PA.h"
#include "Comm_Crypto.h"
#if defined USE_OPENSSL && defined Comm_PAH
#ifndef License_CheckerH
#define License_CheckerH
#ifdef License_CheckerH
//------------------------------------------------------------------------------------------//
#define CHK_RSA_S_FN	"RSA_PrivateKey_SDT_0_3_0.pem"
//------------------------------------------------------------------------------------------//
class Reg_SDT_PatchCode :public PROTOCOL_NODE{
	enum{PatchCodeSize = 1024 * 32};
	public:
		Reg_SDT_PatchCode(void) :PROTOCOL_NODE(){
			cgFIFO_Result.Init	(PatchCodeSize);
			Init				(FNT_MESSAGE,&cgFIFO_Result,G_ENDIAN_LITTLE);
		
			pn_SDT_SHA1.Init	(&cgFIFO_Result,2,G_ENDIAN_LITTLE);
			pn_PatchTime.Init	(&cgFIFO_Result,2,G_ENDIAN_LITTLE);
			pn_CYCode.Init		(&cgFIFO_Result,4,G_ENDIAN_LITTLE);
			pn_RSA_SDTPrk.Init	(&cgFIFO_Result,2,G_ENDIAN_LITTLE);
			pn_RSA_sPuk.Init	(&cgFIFO_Result,2,G_ENDIAN_LITTLE);
			pn_Length.Init		(FNT_CTRL,&cgFIFO_Result,4,G_ENDIAN_LITTLE);
		
			Add(pn_SDT_SHA1) < pn_PatchTime < pn_CYCode < pn_RSA_SDTPrk < pn_RSA_sPuk < pn_Length;
		};
		virtual	~Reg_SDT_PatchCode(void){;};
	public:
		FIFO8	cgFIFO_Result;
	
		PNODE_LC	pn_SDT_SHA1;
		PNODE_LC	pn_PatchTime;
		PNODE_LC	pn_CYCode;
		PNODE_LC	pn_RSA_SDTPrk;
		PNODE_LC	pn_RSA_sPuk;
		PROTOCOL_NODE	pn_Length;
	public:
		int32	PatchToSDT			(const STDSTR &sFN_SDT,STDSTR *strStatus = nullptr);
		int32 	GetPatchCode		(void);
};
//------------------------------------------------------------------------------------------//
class TIME;
const STDSTR&	CHK_Get_CYCode			(STDSTR *strCYCode);
const STDSTR&	CHK_GetSDT_Hash			(STDSTR *sHash);
const TIME&		CHK_GetSDT_Time			(TIME *patchTime);
int32			CHK_GetRSA_SPuk			(RSA **rsa_spuk);
int32			CHK_GetRSA_SDTPrk		(RSA **rsa_sdtsprk);
const STDSTR&	MakeMulitKey			(STDSTR *retMulitKey);
//------------------------------------------------------------------------------------------//
class Reg_Content :public PROTOCOL_NODE{
	public:
		Reg_Content(void) :PROTOCOL_NODE(){
			cgFIFO_Result.Init(1024 * 32);
			Init				(FNT_MESSAGE,&cgFIFO_Result,G_ENDIAN_LITTLE);
			
			pn_sdtPuKey.Init	(&cgFIFO_Result,2,G_ENDIAN_LITTLE);
			pn_CYCode.Init		(&cgFIFO_Result,4,G_ENDIAN_LITTLE);
			pn_MKEY.Init		(&cgFIFO_Result,2,G_ENDIAN_LITTLE);

			Add(pn_sdtPuKey) < pn_CYCode < pn_MKEY;
		};
		virtual	~Reg_Content(void){;};
	public:
		FIFO8	cgFIFO_Result;

		PNODE_LC	pn_sdtPuKey;
		PNODE_LC	pn_CYCode;
		PNODE_LC	pn_MKEY;
	public:
		const STDSTR&	Encode	(STDSTR *retStrWhole,RSA *rsa_sdtPuk){
			STDSTR	strContent;
			GetDefFifo()->Empty();
			_Start();
			pn_sdtPuKey	= CCY_Encode_RSAPublicKey(&strContent, rsa_sdtPuk);
			pn_CYCode	= CHK_Get_CYCode(&strContent);
			pn_MKEY		= MakeMulitKey(&strContent);
			_Endl();
			return(ReadAllContent(retStrWhole));
		};
};
//------------------------------------------------------------------------------------------//
class Reg_Signature : public CCY_FR_Signature{
	public:
				 Reg_Signature(void) :CCY_FR_Signature(){CCY_FR_Signature::Init(1024 * 32,CCT_AES256,CCT_AES_CBC,CCT_SHA256,G_ENDIAN_LITTLE);};
		virtual	~Reg_Signature(void){;};
	public:
		Reg_Content regContent;
	public:
		int32	Encode(STDSTR *retStrWhole,STDSTR *retStrStatus = nullptr);
		int32 	Decode(RSA **rsa_sdtpuk,STDSTR *retStrCYCode,STDSTR *retStrMKEY,const STDSTR &strIn,RSA *rsa_sPrk);
		int32 	Decode(RSA **rsa_sdtpuk,STDSTR *retStrCYCode,STDSTR *retStrMKEY,const FIFO8 *fifoInput,RSA *rsa_sPrk);
};
//------------------------------------------------------------------------------------------//
class Linense_Content :public PROTOCOL_NODE{
	public:
		Linense_Content(void) :PROTOCOL_NODE(){
			cgFIFO_Result.Init(1024 * 32);
			Init				(FNT_MESSAGE,&cgFIFO_Result);
	
			pn_regTime.Init		(&cgFIFO_Result,CCT_AES256,CCT_AES_CBC,CCT_SHA256,G_ENDIAN_LITTLE);
			pn_approveTime.Init	(&cgFIFO_Result,CCT_AES256,CCT_AES_CBC,CCT_SHA256,G_ENDIAN_LITTLE);
			pn_CYCode.Init		(&cgFIFO_Result,4,G_ENDIAN_LITTLE);
	
			Add(pn_regTime) < pn_approveTime < pn_CYCode;
		};
		virtual	~Linense_Content(void){;};
	public:
		FIFO8	cgFIFO_Result;
	
		CCY_FN_AES_MK	pn_regTime;
		CCY_FN_AES_MK	pn_approveTime;
		PNODE_LC		pn_CYCode;
	public:
		int32	Encode(STDSTR *retStrWhole,RSA **rsa_sdtpuk,const STDSTR &strRegSignature,uint64 approveSeconds,RSA *rsa_sprk);
};
//------------------------------------------------------------------------------------------//
class Linense_Signature : public CCY_FR_Signature{
	public:
				 Linense_Signature(void) :CCY_FR_Signature(){CCY_FR_Signature::Init(1024 * 32,CCT_AES256,CCT_AES_CBC,CCT_SHA256,G_ENDIAN_LITTLE);};
		virtual	~Linense_Signature(void){;};
	public:
		Linense_Content	linenseContent;
	public:
		int32	Encode(STDSTR *retStrWhole,const STDSTR &strRegSignature,uint64 approveSeconds);
		int32	Decode(const STDSTR &strIn);
		int32	Decode(const FIFO8 *fifoInput);
};
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_EnableSMC
//------------------------------------------------------------------------------------------//
STDSTR	CHK_GetLinenseFN	(void);
void	CHK_CheckerInit		(void);
void	CHK_CheckerInitClear(void);
void	CHK_ReadLinenseFile	(void);
int32	CHK_CheckATime		(void);
int32	CHK_CheckBTime		(void);
//------------------------------------------------------------------------------------------//
int32	REG_CreateREGFILE	(STDSTR *retStatus = nullptr);
//------------------------------------------------------------------------------------------//
extern	STDSTR			g_StrTable[5];
extern	uint8			g_OverTime;
extern	Linense_Content	g_LinenseC;
#endif
#endif
//------------------------------------------------------------------------------------------//
#endif
#endif
