/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: License_Checker.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.03.26
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "License_Checker.h"
#include "SYS_Time.h"
#include "Comm_Convert.h"
#include "Comm_File.h"
#include "Comm_Crypto.h"
#include "SMC.h"
#include "SystemInfo.h"
#include <math.h>
//------------------------------------------------------------------------------------------//
#ifdef License_CheckerH
//------------------------------------------------------------------------------------------//
int32 GetCYCodeStreamFormFile(FIFO8 *retFifo,const STDSTR &sFN_CYCode){
	if (CFS_CheckFile(sFN_CYCode) < 1)
		return 0;
	
	CFS_ReadFile(retFifo, sFN_CYCode);
	retFifo->Out((uint32)sFN_CYCode.length());
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 GetRSA_S(RSA **rsa_s){
	return(CCY_RSA_RD_PrivateKey_PEM(rsa_s, CHK_RSA_S_FN));
}
//------------------------------------------------------------------------------------------//
int32 Reg_SDT_PatchCode::PatchToSDT(const STDSTR &sFN_SDT,STDSTR *strStatus){
	RSA		*rsa_s,*rsa_sdt;
	int32	retCode;
	STDSTR	strContent,strCYCode,strFN_CYCode,strStatusT;
	TIME 	patchTime;
	CCY_FR_Signature	fnSignature;

	do{
		rsa_sdt = RSA_new();
		do{
			retCode = 0;
			if (CCY_RSA_CreateKey(rsa_sdt, 2048) < 1){
				strStatusT = "Create SDT RSA key fail.\n";
				break;
			}
			strStatusT = "Create SDT RSA key successful.\n";
			rsa_s = RSA_new();
			do{
				retCode = -1;
				if (GetRSA_S(&rsa_s) < 1){
					strStatusT += "Read server RSA public key fail.\n";
					break;
				}
				strStatusT += "Read server RSA public key successful.\n";
				
				retCode = -2;
				if (CFS_CheckFile(sFN_SDT) < 1){
					strStatusT += "Get SDT file fail.\n";
					break;
				}
				strStatusT += "Get SDT file successful.\n";
				retCode = -3;
				if (SMC_SourceCodeAnalysis(&strContent,sFN_SDT) < 1){
					strStatusT += "SDT source code analysis fail.\n";
					break;
				}
				strStatusT += "SDT source code analysis successful.\n";
				retCode = -4;
				if (SMC_CreateCYCodeStream(&strCYCode,sFN_SDT,strContent) < 1){
					strStatusT += "Create CYCode stream fail.\n";
					break;
				}
				strStatusT += "Create CYCode stream successful.\n";
				retCode = -5;
				if (SMC_SourceCodeReplace(sFN_SDT,strContent) < 1){
					strStatusT += "Source code replace fail.\n";
					break;
				}
				strStatusT += "Source code replace successful.\n";
				patchTime.Now();
				
				GetDefFifo()->Empty();
				_Start();
				fnSignature.Init(1024 * 32,CCT_AES256,CCT_AES_CBC,CCT_SHA512,G_ENDIAN_BIG);
				pn_SDT_SHA1		= CCY_Encrypt_RSAPublicKey	(&strContent, CCY_DigestFile_SHA1(sFN_SDT), rsa_sdt);
				pn_PatchTime	= CCY_Encrypt_RSAPublicKey	(&strContent, Str_DecToHex((uint64)patchTime.GetSec()), rsa_sdt);
				pn_CYCode		= fnSignature.Encode		(&strContent, strCYCode, rsa_s);
				pn_RSA_SDTPrk	= CCY_Encode_RSAPrivateKey	(&strContent, rsa_sdt);
				pn_RSA_sPuk		= CCY_Encode_RSAPublicKey	(&strContent, rsa_s);
				pn_Length.SetFIFOByte(GetDefFifo()->Used() + 4);
				_Endl();
				
				CFS_AddToFile(sFN_SDT,ReadAllContent(&strContent));
				strStatusT += "Patch to SDT ";
				strStatusT += Str_ToString(fnlength);
				strStatusT += " bytes,successful.\n";
				retCode = (int32)strContent.length();
			}while(0);
			RSA_free(rsa_s);
		}while(0);
		RSA_free(rsa_sdt);
	}while(0);
	if (strStatus != nullptr)
		*strStatus = strStatusT;
	return(retCode);
}
//------------------------------------------------------------------------------------------//
int32 Reg_SDT_PatchCode::GetPatchCode(void){
	uint8			buffer[1024 * 2];
	std::fstream	fileStream;
	uint64			offset,num;
#ifdef CommonDefH_VC
	CString			cName;
	std::wstring	strwName;
	HMODULE module = GetModuleHandle(0);
	TCHAR name[MAX_PATH];
	GetModuleFileName(module, name, sizeof(name));
	cName.Format(_T("%s"),name);
	strwName = cName;
	
	fileStream.open(Str_UnicodeToANSI(strwName).c_str(),std::ios::in|std::ios::binary);
#endif
#ifdef CommonDefH_Unix
	fileStream.open("VSDT.txt",std::ios::in|std::ios::binary);
#endif
	fileStream.seekp(-4,std::ios::end);
	fileStream.read((char*)buffer,4);
	
	offset = Str_CharToUint32(buffer,G_ENDIAN_LITTLE);
	
	fileStream.seekp(0 - offset,std::ios::end);

	GetDefFifo()->Empty();
	do{
		fileStream.read((char*)buffer, sizeof(buffer));
		num = fileStream.gcount();
		GetDefFifo()->Init(GetDefFifo()->Used() + sizeof(buffer));
		GetDefFifo()->Put(buffer, (uint32)num);
	}while(!fileStream.eof());
	fileStream.close();
	return(AnalysisFrame());
}
//------------------------------------------------------------------------------------------//
const STDSTR &CHK_GetSDT_Hash(STDSTR *sHash){
	Reg_SDT_PatchCode	tRSPC;
	STDSTR				strData;
	RSA					*rsa_sdtsprk;
	rsa_sdtsprk = RSA_new();
	
	if (tRSPC.GetPatchCode() != 0){
		if (CCY_Decode_RSAPrivateKey(&rsa_sdtsprk, tRSPC.pn_RSA_SDTPrk.ReadContent(&strData)) != 0)
			CCY_Decrypt_RSAPrivateKey(sHash,tRSPC.pn_SDT_SHA1.ReadContent(&strData),rsa_sdtsprk);
	}
	RSA_free(rsa_sdtsprk);
	return(*sHash);
}
//------------------------------------------------------------------------------------------//
const TIME &CHK_GetSDT_Time(TIME *patchTime){
	Reg_SDT_PatchCode	tRSPC;
	STDSTR				strData,strContent;
	RSA					*rsa_sdtsprk;
	rsa_sdtsprk = RSA_new();
	patchTime->Clear();
	if (tRSPC.GetPatchCode() != 0){
		if (CCY_Decode_RSAPrivateKey(&rsa_sdtsprk, tRSPC.pn_RSA_SDTPrk.ReadContent(&strData)) != 0)
			*patchTime = (double)Str_HexToDec((CCY_Decrypt_RSAPrivateKey(&strContent,tRSPC.pn_PatchTime.ReadContent(&strData),rsa_sdtsprk)));
	}
	RSA_free(rsa_sdtsprk);
	return(*patchTime);
}
//------------------------------------------------------------------------------------------//
const STDSTR &CHK_Get_CYCode(STDSTR *strCYCode){
	Reg_SDT_PatchCode	tRSPC;
	*strCYCode = "";
	if (tRSPC.GetPatchCode() > 0)
		tRSPC.pn_CYCode.ReadContent(strCYCode);
	return(*strCYCode);
}
//------------------------------------------------------------------------------------------//
int32 CHK_GetRSA_SDTPrk(RSA **rsa_sdtsprk){
	Reg_SDT_PatchCode	tRSPC;
	STDSTR				strData;
	
	if (tRSPC.GetPatchCode() > 0)
		return(CCY_Decode_RSAPrivateKey(rsa_sdtsprk, tRSPC.pn_RSA_SDTPrk.ReadContent(&strData)));
	return 0;
}
//------------------------------------------------------------------------------------------//
int32 CHK_GetRSA_SPuk(RSA **rsa_spuk){
	Reg_SDT_PatchCode	tRSPC;
	STDSTR			strData;
	
	if (tRSPC.GetPatchCode() > 0)
		return(CCY_Decode_RSAPublicKey(rsa_spuk, tRSPC.pn_RSA_sPuk.ReadContent(&strData)));
	return 0;
}
//------------------------------------------------------------------------------------------//











//------------------------------------------------------------------------------------------//
const STDSTR &MakeMulitKey(STDSTR *retMulitKey){
	STDSTR	strCPUHD,strMAC,strKey;

	CHK_GetSDT_Hash(&strCPUHD);
	SystemInfo_GetCPUID(&strCPUHD);
	SystemInfo_GetHardiskInfo(&strCPUHD);
	strMAC = "";
	SystemInfo_GetNetCardInfo(&strMAC);
	
	*retMulitKey = "";
	do{
		strKey = strCPUHD;
		strKey += Str_ReadSubItem(&strMAC, "\r");
		CCY_Encode_BASE64(&strKey,CCY_AESKey32Bye(strKey),CCT_NL_NO);
		
		*retMulitKey += strKey;
		*retMulitKey += '\r';
	}while(strMAC.length() > 0);
	return(*retMulitKey);
}
//------------------------------------------------------------------------------------------//
int32 Reg_Signature::Encode(STDSTR *retStrWhole,STDSTR *retStrStatus){
	STDSTR	strContent,retStatusT;
	int32 	retCode;
	RSA 	*rsa_sdtPrk,*rsa_sPuk;
	
	rsa_sPuk = RSA_new();
	rsa_sdtPrk = RSA_new();

	do{
		retCode = 0;
		if (CHK_GetRSA_SPuk(&rsa_sPuk) < 1){
			retStatusT = "read server public RSA key fail";
			break;
		}
		retCode = -1;
		if (CHK_GetRSA_SDTPrk(&rsa_sdtPrk) < 1){
			retStatusT = "read SDT privte RSA key fail";
			break;
		}
		retCode = 1;
		CCY_FR_Signature::Encode(retStrWhole,regContent.Encode(&strContent,rsa_sdtPrk), rsa_sPuk);
	}while(0);
	RSA_free(rsa_sdtPrk);
	RSA_free(rsa_sPuk);
	if (retStrStatus != nullptr)
		*retStrStatus = retStatusT;
	return(retCode);
}
//------------------------------------------------------------------------------------------//
int32 Reg_Signature::Decode(RSA **rsa_sdtpuk,STDSTR *retStrCYCode,STDSTR *retStrMKEY,const STDSTR &strIn,RSA *rsa_sPrk){
	STDSTR	strContent;

	*retStrCYCode = "";
	*retStrMKEY = "";
	
	if (CCY_FR_Signature::Decode(&strContent, strIn, rsa_sPrk) < 1)
		return 0;
	regContent.TryGetFrame(strContent);
	if (CCY_Decode_RSAPublicKey(rsa_sdtpuk, regContent.pn_sdtPuKey.ReadContent(&strContent)) < 1)
		return -1;
	regContent.pn_CYCode.ReadContent(retStrCYCode);
	regContent.pn_MKEY.ReadContent(retStrMKEY);
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 Reg_Signature::Decode(RSA **rsa_sdtpuk,STDSTR *retStrCYCode,STDSTR *retStrMKEY,const FIFO8 *fifoInput,RSA *rsa_sPrk){
	STDSTR	strContent;
	
	*retStrCYCode = "";
	*retStrMKEY = "";

	if (CCY_FR_Signature::Decode(&strContent, *fifoInput, rsa_sPrk) < 1)
		return 0;
	regContent.TryGetFrame(strContent);
	if (CCY_Decode_RSAPublicKey(rsa_sdtpuk, regContent.pn_sdtPuKey.ReadContent(&strContent)) < 1)
		return -1;
	regContent.pn_CYCode.ReadContent(retStrCYCode);
	regContent.pn_MKEY.ReadContent(retStrMKEY);
	return 1;
}
//------------------------------------------------------------------------------------------//









//------------------------------------------------------------------------------------------//
int32 Linense_Content::Encode(STDSTR *retStrWhole,RSA **rsa_sdtpuk,const STDSTR &strRegSignature,uint64 approveSeconds,RSA *rsa_sPrk){
	int32 	retCode;
	TIME	regTime;
	STDSTR	strContent,strCYCode,strMKEY;
	Reg_Signature	cRegSignature;
	CCY_FNLC_AES	cyBlock;
	CCY_FN_AES_MK	cyBlock_MK;
	FIFO8			fifo_CYCodeStream;
	uint32			cyCodeStreamOffset;
	CCY_FR_Signature	fnSignature;
	
	retCode = cRegSignature.Decode(rsa_sdtpuk,&strCYCode,&strMKEY,strRegSignature,rsa_sPrk);
	if (retCode < 1)
		return(retCode);

	fifo_CYCodeStream.Init(1024 * 32);
	
	fnSignature.Init(1024 * 32,CCT_AES256,CCT_AES_CBC,CCT_SHA512,G_ENDIAN_BIG);
	if (fnSignature.Decode(&strContent, strCYCode, rsa_sPrk) < 1)
		return -2;

	fifo_CYCodeStream.Put(strContent, G_ESCAPE_OFF);
	
	cyBlock.Init(&fifo_CYCodeStream);
	
	cyBlock_MK.Init(GetDefFifo(),CCT_AES256,CCT_AES_CBC,CCT_SHA256);
	regTime.Now();
	GetDefFifo()->Empty();
	_Start();
	pn_regTime.SetContent(Str_DecToHex((uint64)regTime.GetSec()), strMKEY);
	pn_approveTime.SetContent(Str_DecToHex(approveSeconds), strMKEY);
	
	pn_CYCode._Start();
	cyCodeStreamOffset = 0;
	do{
		cyBlock.AnalysisFrame(&fifo_CYCodeStream,cyCodeStreamOffset);
		cyCodeStreamOffset += cyBlock.GetLength();
		cyBlock_MK.SetContent(cyBlock.ReadContent(&strContent, CCY_AESKey32Bye("SMCEncrypt"),&fifo_CYCodeStream), strMKEY);
	}while(cyCodeStreamOffset < fifo_CYCodeStream.Used());
	pn_CYCode._Endl();

	_Endl();
	ReadAllContent(retStrWhole);
	
	retCode = 1;
	return(retCode);
}
//------------------------------------------------------------------------------------------//
int32 Linense_Signature::Encode(STDSTR *retStrWhole,const STDSTR &strRegSignature,uint64 approveSeconds){
	int32 	retCode;
	STDSTR	strContent;
	RSA 	*rsa_sdtPuk,*rsa_sPrk;

	rsa_sPrk = RSA_new();
	rsa_sdtPuk = RSA_new();
	
	do{
		retCode = 0;
		if (GetRSA_S(&rsa_sPrk) < 1)
			break;

		retCode = linenseContent.Encode(&strContent,&rsa_sdtPuk,strRegSignature,approveSeconds,rsa_sPrk);
		if (retCode > 0)
			CCY_FR_Signature::Encode(retStrWhole, strContent, rsa_sdtPuk);
	}while(0);
	RSA_free(rsa_sdtPuk);
	RSA_free(rsa_sPrk);
	return(retCode);
}
//------------------------------------------------------------------------------------------//
int32 Linense_Signature::Decode(const STDSTR &strIn){
	int32 	retCode;
	STDSTR	strContent;
	RSA 	*rsa_sdtPrk;
	
	rsa_sdtPrk = RSA_new();
	
	do{
		retCode = 0;
		if (CHK_GetRSA_SDTPrk(&rsa_sdtPrk) < 1)
			break;
		
		retCode = -1;
		if (CCY_FR_Signature::Decode(&strContent, strIn, rsa_sdtPrk) < 1)
			break;
		retCode = linenseContent.TryGetFrame(strContent);
	}while(0);
	RSA_free(rsa_sdtPrk);
	return(retCode);
}
//------------------------------------------------------------------------------------------//
int32 Linense_Signature::Decode(const FIFO8 *fifoInput){
	int32 	retCode;
	STDSTR	strContent;
	RSA 	*rsa_sdtPrk;
	
	rsa_sdtPrk = RSA_new();
	
	do{
		retCode = 0;
		if (CHK_GetRSA_SDTPrk(&rsa_sdtPrk) < 1)
			break;
		
		retCode = -1;
		if (CCY_FR_Signature::Decode(&strContent, *fifoInput, rsa_sdtPrk) < 1)
			break;
		retCode = linenseContent.TryGetFrame(strContent);
	}while(0);
	RSA_free(rsa_sdtPrk);
	return(retCode);
}
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_EnableSMC
STDSTR			g_StrTable[5];
uint8			g_OverTime;
Linense_Content	g_LinenseC;
//------------------------------------------------------------------------------------------//
void CHK_CheckerInit(void){
	//name = e7nYIuKfMuEweU8=
	//key = lytPIiDLSRBKP4vqsI1kjtRXcgxP9rG+OhuUXNF3i1YPDjG1p5uiR86vDfvMmgMBlcfKEiQx8Fy+7GmAYKHBvA==
	uint8	buf[32];
	CY_Init();
	g_StrTable[0] = "exOUwrqkxcYwH7A=";
	g_StrTable[1] = "lytPIiDLSRBKP4vqsI1kjtRXcgxP9rG+OhuUXNF3i1YP";
	g_StrTable[2] = "DjG1p5uiR86vDfvMmgMBlcfKEiQx8Fy+7GmAYKHBvA==";
	RAND_bytes(buf,sizeof(32));CCY_Encode_BASE64(&g_StrTable[3], buf, sizeof(32));
	RAND_bytes(buf,sizeof(32));CCY_Encode_BASE64(&g_StrTable[4], buf, sizeof(32));
	RAND_bytes(&g_OverTime,1);g_OverTime = (uint8)sqrtl(g_OverTime) + 5;
	g_SMCCyCodeStream.Init(1024 * 8);
}
//------------------------------------------------------------------------------------------//
void CHK_CheckerInitClear(void){
	uint8	buf[32];
	RAND_bytes(buf,sizeof(32));CCY_Encode_BASE64(&g_StrTable[3], buf, sizeof(32));
	RAND_bytes(buf,sizeof(32));CCY_Encode_BASE64(&g_StrTable[4], buf, sizeof(32));
	RAND_bytes(&g_OverTime,1);g_OverTime = (uint8)sqrtl(g_OverTime) + 5;
	g_LinenseC.FillZero();
}
//------------------------------------------------------------------------------------------//
STDSTR CHK_GetLinenseFN(void){
	STDSTR		name,strData;
	
	strData = g_StrTable[1];
	strData += g_StrTable[2];
	
	CCY_Decrypt_AES(&name, CCY_Decode_BASE64(g_StrTable[0],CCT_NL_NO), CCY_Decode_BASE64(strData,CCT_NL_NO), CCT_AES256, CCT_AES_CFB8);
	name.erase(11);
	return(name);
}
//------------------------------------------------------------------------------------------//
void CHK_ReadLinenseFile(void){
	STDSTR	sFN;
	Linense_Signature tLS;
	
	CFS_ReadFile(&tLS.cgFIFO_Result, CHK_GetLinenseFN());
	
	tLS.Decode(&tLS.cgFIFO_Result);
	
	g_LinenseC.cgFIFO_Result.Empty();
	g_LinenseC.cgFIFO_Result.Init(tLS.linenseContent.cgFIFO_Result.Used());
	g_LinenseC.cgFIFO_Result.Put(tLS.linenseContent.cgFIFO_Result,-1);
	g_LinenseC.AnalysisFrame(&g_LinenseC.cgFIFO_Result);
	g_LinenseC.pn_CYCode.ReadContent(&g_SMCCyCodeStream);
}
//------------------------------------------------------------------------------------------//
void CHK_GetRATime(TIME *cRTime,TIME *cATime){
	TIME	SDTTime;
	STDSTR	strTime,sKey;
	
	CHK_GetSDT_Time(cRTime);
	SYS_EncodeTimeABS(cATime,0, 3, 0, 0);
	
	if (CFS_CheckFile(CHK_GetLinenseFN()) > 0){
		if (g_LinenseC.pn_regTime.ReadContent(&strTime, MakeMulitKey(&sKey)) > 0)
			*cRTime = (double)Str_HexToDec(strTime);
		if (g_LinenseC.pn_approveTime.ReadContent(&strTime, MakeMulitKey(&sKey)) > 0)
			*cATime = (double)Str_HexToDec(strTime);
	}
}
//------------------------------------------------------------------------------------------//
void CHK_GetCurrentTime(TIME *cCTime){
	cCTime->Now();
}
//------------------------------------------------------------------------------------------//
int32 CHK_CheckATime(void){
	TIME chk_cTime,chk_regTime,chk_aTime;
	
	CHK_GetRATime(&chk_regTime,&chk_aTime);
	CHK_GetCurrentTime(&chk_cTime);
	
	if ((CCY_Gaussian(chk_cTime.GetSec(),chk_regTime.GetSec(),chk_aTime.GetSec()) > sqrtl(0.9 * 0.9))){
		if (g_OverTime > 0){
			if (CFS_CheckFile(CHK_GetLinenseFN()) > 0){
				uint8	rand;
				RAND_bytes(&rand,1);
				CFS_WriteFile(CHK_GetLinenseFN(), CCY_CreateRandKey(1), rand);
				-- g_OverTime;
				return -1;
			}
		}
		return 0;
	}
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 CHK_CheckBTime(void){
	TIME chk_cTime,chk_regTime,chk_aTime;
	
	CHK_GetRATime(&chk_regTime,&chk_aTime);
	CHK_GetCurrentTime(&chk_cTime);
	
	if ((CCY_Gaussian(chk_cTime.GetSec(),chk_regTime.GetSec(),chk_aTime.GetSec()) < sqrtl(0.48 * 0.48))){
		if (g_OverTime > 0){
			if (CFS_CheckFile(CHK_GetLinenseFN()) > 0){
				uint8	rand;
				RAND_bytes(&rand,1);
				CFS_WriteFile(CHK_GetLinenseFN(), CCY_CreateRandKey(1), rand);
				-- g_OverTime;
				return -1;
			}
		}
		return 0;
	}
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 REG_CreateREGFILE(STDSTR *retStatus){
	Reg_Signature	regS;
	STDSTR			strReg,retStatusT;
	int32			ret;

	ret = regS.Encode(&strReg,&retStatusT);
	if (ret > 0){
		CFS_WriteFile("PreRegistration.key", strReg);
		retStatusT = "create \"PreRegistration.key\" successful";
	}
	else{
		retStatusT = "create \"PreRegistration.key\" fail";
	}
	if (retStatus != nullptr)
		*retStatus = retStatusT;
	return(ret);
}
//------------------------------------------------------------------------------------------//
#endif
#endif
