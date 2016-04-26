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
#ifndef License_CheckerH
#define License_CheckerH
//------------------------------------------------------------------------------------------//
#include "Comm_PA.h"
#include "Comm_Crypto.h"
//------------------------------------------------------------------------------------------//
#define CHK_RSA_S_FN	"RSA_PrivateKey_SDT_0_3_0.pem"
#ifdef USE_OPENSSL
//------------------------------------------------------------------------------------------//
class Reg_SDT_PatchCode :public Field_Node{
	enum{PatchCodeSize = 1024 * 32};
	public:
		Reg_SDT_PatchCode(void) :Field_Node(){
			cgFIFO_Result.Init	(PatchCodeSize);
			Init				(FNT_MESSAGE,&cgFIFO_Result,G_LITTLE_ENDIAN);
		
			fn_SDT_SHA1.Init	(&cgFIFO_Result,2,G_LITTLE_ENDIAN);
			fn_PatchTime.Init	(&cgFIFO_Result,2,G_LITTLE_ENDIAN);
			fn_CYCode.Init		(&cgFIFO_Result,4,G_LITTLE_ENDIAN);
			fn_RSA_SDTPrk.Init	(&cgFIFO_Result,2,G_LITTLE_ENDIAN);
			fn_RSA_sPuk.Init	(&cgFIFO_Result,2,G_LITTLE_ENDIAN);
			fn_Length.Init		(FNT_CTRL,&cgFIFO_Result,4,G_LITTLE_ENDIAN);
		
			AddNode(&fn_SDT_SHA1);
			AddNode(&fn_PatchTime);
			AddNode(&fn_CYCode);
			AddNode(&fn_RSA_SDTPrk);
			AddNode(&fn_RSA_sPuk);
			AddNode(&fn_Length);
		};
		virtual	~Reg_SDT_PatchCode(void){;};
	public:
		FIFO_UINT8	cgFIFO_Result;
	
		FNode_LC	fn_SDT_SHA1;
		FNode_LC	fn_PatchTime;
		FNode_LC	fn_CYCode;
		FNode_LC	fn_RSA_SDTPrk;
		FNode_LC	fn_RSA_sPuk;
		Field_Node	fn_Length;
	public:
		int32	PatchToSDT			(const std::string &sFN_SDT,std::string *strStatus = nullptr);
		int32 	GetPatchCode		(void);
};
//------------------------------------------------------------------------------------------//
class SYS_DateTime;
const std::string&	CHK_Get_CYCode			(std::string *strCYCode);
const std::string&	CHK_GetSDT_Hash			(std::string *sHash);
const SYS_DateTime&	CHK_GetSDT_Time			(SYS_DateTime *patchTime);
int32				CHK_GetRSA_SPuk			(RSA **rsa_spuk);
int32				CHK_GetRSA_SDTPrk		(RSA **rsa_sdtsprk);
const std::string	&MakeMulitKey			(std::string *retMulitKey);
//------------------------------------------------------------------------------------------//
class Reg_Content :public Field_Node{
	public:
		Reg_Content(void) :Field_Node(){
			cgFIFO_Result.Init(1024 * 32);
			Init				(FNT_MESSAGE,&cgFIFO_Result,G_LITTLE_ENDIAN);
			
			fn_sdtPuKey.Init	(&cgFIFO_Result,2,G_LITTLE_ENDIAN);
			fn_CYCode.Init		(&cgFIFO_Result,4,G_LITTLE_ENDIAN);
			fn_MKEY.Init		(&cgFIFO_Result,2,G_LITTLE_ENDIAN);

			AddNode(&fn_sdtPuKey);
			AddNode(&fn_CYCode);
			AddNode(&fn_MKEY);
		};
		virtual	~Reg_Content(void){;};
	public:
		FIFO_UINT8	cgFIFO_Result;

		FNode_LC	fn_sdtPuKey;
		FNode_LC	fn_CYCode;
		FNode_LC	fn_MKEY;
	public:
		const std::string&	Encode	(std::string *retStrWhole,RSA *rsa_sdtPuk){
			std::string	strContent;
			GetcgDefFifo()->Empty();
			HoldOffset();
			fn_sdtPuKey	= CCY_Encode_RSAPublicKey(&strContent, rsa_sdtPuk);
			fn_CYCode	= CHK_Get_CYCode(&strContent);
			fn_MKEY		= MakeMulitKey(&strContent);
			UpdateLength();
			return(ReadAllContent(retStrWhole));
		};
};
//------------------------------------------------------------------------------------------//
class Reg_Signature : public CCY_FR_Signature{
	public:
				 Reg_Signature(void) :CCY_FR_Signature(){CCY_FR_Signature::Init(1024 * 32,CCT_AES256,CCT_AES_CBC,CCT_SHA256,G_LITTLE_ENDIAN);};
		virtual	~Reg_Signature(void){;};
	public:
		Reg_Content regContent;
	public:
		int32	Encode(std::string *retStrWhole,std::string *retStrStatus = nullptr);
		int32 	Decode(RSA **rsa_sdtpuk,std::string *retStrCYCode,std::string *retStrMKEY,const std::string &strInput,RSA *rsa_sPrk);
		int32 	Decode(RSA **rsa_sdtpuk,std::string *retStrCYCode,std::string *retStrMKEY,const FIFO_UINT8 *fifoInput,RSA *rsa_sPrk);
};
//------------------------------------------------------------------------------------------//
class Linense_Content :public Field_Node{
	public:
		Linense_Content(void) :Field_Node(){
			cgFIFO_Result.Init(1024 * 32);
			Init				(FNT_MESSAGE,&cgFIFO_Result);
	
			fn_regTime.Init		(&cgFIFO_Result,CCT_AES256,CCT_AES_CBC,CCT_SHA256,G_LITTLE_ENDIAN);
			fn_approveTime.Init	(&cgFIFO_Result,CCT_AES256,CCT_AES_CBC,CCT_SHA256,G_LITTLE_ENDIAN);
			fn_CYCode.Init		(&cgFIFO_Result,4,G_LITTLE_ENDIAN);
	
			AddNode(&fn_regTime);
			AddNode(&fn_approveTime);
			AddNode(&fn_CYCode);
		};
		virtual	~Linense_Content(void){;};
	public:
		FIFO_UINT8	cgFIFO_Result;
	
		CCY_FN_AES_MK	fn_regTime;
		CCY_FN_AES_MK	fn_approveTime;
		FNode_LC		fn_CYCode;
	public:
		int32	Encode(std::string *retStrWhole,RSA **rsa_sdtpuk,const std::string &strRegSignature,uint64 approveSeconds,RSA *rsa_sprk);
};
//------------------------------------------------------------------------------------------//
class Linense_Signature : public CCY_FR_Signature{
	public:
				 Linense_Signature(void) :CCY_FR_Signature(){CCY_FR_Signature::Init(1024 * 32,CCT_AES256,CCT_AES_CBC,CCT_SHA256,G_LITTLE_ENDIAN);};
		virtual	~Linense_Signature(void){;};
	public:
		Linense_Content	linenseContent;
	public:
		int32	Encode(std::string *retStrWhole,const std::string &strRegSignature,uint64 approveSeconds);
		int32	Decode(const std::string &strInput);
		int32	Decode(const FIFO_UINT8 *fifoInput);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
std::string CHK_GetLinenseFN	(void);
void		CHK_CheckerInit		(void);
void		CHK_CheckerInitClear(void);
void		CHK_ReadLinenseFile	(void);
int32		CHK_CheckATime		(void);
int32		CHK_CheckBTime		(void);
//------------------------------------------------------------------------------------------//
int32		REG_CreateREGFILE	(std::string *retStatus = nullptr);
//------------------------------------------------------------------------------------------//
extern	std::string		g_StrTable[5];
extern	uint8			g_OverTime;
extern	Linense_Content	g_LinenseC;
#endif
//------------------------------------------------------------------------------------------//
#endif
