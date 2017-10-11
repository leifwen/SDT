/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: BIC_CALC.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.01.25
 
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "BIC_Calc.h"
#include "Comm_Convert.h"
#include "Comm_File.h"
#include "Comm_Crypto.h"
//------------------------------------------------------------------------------------------//
#ifdef BIC_CALCH
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_CALC_HEX2DEC::Command(BIC_ENV *env, const STDSTR &par,void *p)const{
	uint64		calcRet;
	calcRet = Str_HexToDec(par);
	Print(env) << COL_clDefault
	<< NL << "HEX: " << Str_DecToHex(calcRet)
	<< NL << "DEC: " << Str_ToString(calcRet)
	<< Endl;
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_CALC_DEC2HEX::Command(BIC_ENV *env, const STDSTR &par,void *p)const{
	Print(env) << COL_clDefault
	<< NL << "DEC: " << Str_Trim(par)
	<< NL << "HEX: " << Str_DecToHex(par)
	<< Endl;
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
#if (defined USE_OPENSSL) || (defined CommonDefH_MAC)
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_CALC_MD5::Command(BIC_ENV *env, const STDSTR &par,void *p)const{
	SBIC_RETSTR		retCombine;
	SBIC_Combine	tSBIC_Combine;

	STDSTR	strPar;
	strPar = par;
	strPar = Str_SplitSubItem(&strPar, ' ');
	if (par.length() > 0){
		if (CFS_CheckFile(strPar) > 0){
			Print(env)
			<< NL << COL_clBlue << "FileName: " << COL_clCyan << par
			<< NL << COL_clBlue << "FileSize: " << COL_clCyan << Str_ToString(CFS_CheckFileSize(strPar))
			<< NL << COL_clBlue << "MD5:      " << COL_clCyan << Str_ASCIIToHEX(CCY_DigestFile_MD5(strPar),G_ESCAPE_OFF)
			<< Endl;
		}
		else{
			retCombine.result = "";
			retCombine.forPrint = "";
			retCombine.forSend = "";
			tSBIC_Combine.Command(nullptr, par, &retCombine);
			Print(env)
			<< NL << COL_clBlue << "Input:  " << COL_clCyan << par
			<< NL << COL_clBlue << "Length: " << COL_clCyan << Str_ToString(retCombine.forSend.length())
			<< NL << COL_clBlue << "MD5:    " << COL_clCyan << Str_ASCIIToHEX(CCY_Digest_MD5(retCombine.forSend),G_ESCAPE_OFF)
			<< Endl;
		}
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_CALC_SHA1::Command(BIC_ENV *env, const STDSTR &par,void *p)const{
	SBIC_RETSTR		retCombine;
	SBIC_Combine	tSBIC_Combine;
	
	STDSTR	strPar;
	strPar = par;
	strPar = Str_SplitSubItem(&strPar, ' ');
	if (par.length() > 0){
		if (CFS_CheckFile(strPar) > 0){
			Print(env)
			<< NL << COL_clBlue << "FileName: " << COL_clCyan << par
			<< NL << COL_clBlue << "FileSize: " << COL_clCyan << Str_ToString(CFS_CheckFileSize(strPar))
			<< NL << COL_clBlue << "SHA1:      " << COL_clCyan << Str_ASCIIToHEX(CCY_DigestFile_SHA1(strPar),G_ESCAPE_OFF)
			<< Endl;
		}
		else{
			retCombine.result = "";
			retCombine.forPrint = "";
			retCombine.forSend = "";
			tSBIC_Combine.Command(nullptr, par, &retCombine);
			Print(env)
			<< NL << COL_clBlue << "Input:  " << COL_clCyan << par
			<< NL << COL_clBlue << "Length: " << COL_clCyan << Str_ToString(retCombine.forSend.length())
			<< NL << COL_clBlue << "SHA1:    " << COL_clCyan << Str_ASCIIToHEX(CCY_Digest_SHA1(retCombine.forSend),G_ESCAPE_OFF)
			<< Endl;
		}
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_CALC_SHA224::Command(BIC_ENV *env, const STDSTR &par,void *p)const{
	SBIC_RETSTR		retCombine;
	SBIC_Combine	tSBIC_Combine;
	
	STDSTR	strPar;
	strPar = par;
	strPar = Str_SplitSubItem(&strPar, ' ');
	if (par.length() > 0){
		if (CFS_CheckFile(strPar) > 0){
			Print(env)
			<< NL << COL_clBlue << "FileName: " << COL_clCyan << par
			<< NL << COL_clBlue << "FileSize: " << COL_clCyan << Str_ToString(CFS_CheckFileSize(strPar))
			<< NL << COL_clBlue << "SHA224:   " << COL_clCyan << Str_ASCIIToHEX(CCY_DigestFile_SHA224(strPar),G_ESCAPE_OFF)
			<< Endl;
		}
		else{
			retCombine.result = "";
			retCombine.forPrint = "";
			retCombine.forSend = "";
			tSBIC_Combine.Command(nullptr, par, &retCombine);
			Print(env)
			<< NL << COL_clBlue << "Input:  " << COL_clCyan << par
			<< NL << COL_clBlue << "Length: " << COL_clCyan << Str_ToString(retCombine.forSend.length())
			<< NL << COL_clBlue << "SHA224: " << COL_clCyan << Str_ASCIIToHEX(CCY_Digest_SHA224(retCombine.forSend),G_ESCAPE_OFF)
			<< Endl;
		}
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_CALC_SHA256::Command(BIC_ENV *env, const STDSTR &par,void *p)const{
	SBIC_RETSTR		retCombine;
	SBIC_Combine	tSBIC_Combine;
	
	STDSTR	strPar;
	strPar = par;
	strPar = Str_SplitSubItem(&strPar, ' ');
	if (par.length() > 0){
		if (CFS_CheckFile(strPar) > 0){
			Print(env)
			<< NL << COL_clBlue << "FileName: " << COL_clCyan << par
			<< NL << COL_clBlue << "FileSize: " << COL_clCyan << Str_ToString(CFS_CheckFileSize(strPar))
			<< NL << COL_clBlue << "SHA256:   " << COL_clCyan << Str_ASCIIToHEX(CCY_DigestFile_SHA256(strPar),G_ESCAPE_OFF)
			<< Endl;
		}
		else{
			retCombine.result = "";
			retCombine.forPrint = "";
			retCombine.forSend = "";
			tSBIC_Combine.Command(nullptr, par, &retCombine);
			Print(env)
			<< NL << COL_clBlue << "Input:  " << COL_clCyan << par
			<< NL << COL_clBlue << "Length: " << COL_clCyan << Str_ToString(retCombine.forSend.length())
			<< NL << COL_clBlue << "SHA256: " << COL_clCyan << Str_ASCIIToHEX(CCY_Digest_SHA256(retCombine.forSend),G_ESCAPE_OFF)
			<< Endl;
		}
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_CALC_SHA384::Command(BIC_ENV *env, const STDSTR &par,void *p)const{
	SBIC_RETSTR		retCombine;
	SBIC_Combine	tSBIC_Combine;
	
	STDSTR	strPar;
	strPar = par;
	strPar = Str_SplitSubItem(&strPar, ' ');
	if (par.length() > 0){
		if (CFS_CheckFile(strPar) > 0){
			Print(env)
			<< NL << COL_clBlue << "FileName: " << COL_clCyan << par
			<< NL << COL_clBlue << "FileSize: " << COL_clCyan << Str_ToString(CFS_CheckFileSize(strPar))
			<< NL << COL_clBlue << "SHA384:   " << COL_clCyan << Str_ASCIIToHEX(CCY_DigestFile_SHA384(strPar),G_ESCAPE_OFF)
			<< Endl;
		}
		else{
			retCombine.result = "";
			retCombine.forPrint = "";
			retCombine.forSend = "";
			tSBIC_Combine.Command(nullptr, par, &retCombine);
			Print(env)
			<< NL << COL_clBlue << "Input:  " << COL_clCyan << par
			<< NL << COL_clBlue << "Length: " << COL_clCyan << Str_ToString(retCombine.forSend.length())
			<< NL << COL_clBlue << "SHA384: " << COL_clCyan << Str_ASCIIToHEX(CCY_Digest_SHA384(retCombine.forSend),G_ESCAPE_OFF)
			<< Endl;
		}
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_CALC_SHA512::Command(BIC_ENV *env, const STDSTR &par,void *p)const{
	SBIC_RETSTR		retCombine;
	SBIC_Combine	tSBIC_Combine;
	
	STDSTR	strPar;
	strPar = par;
	strPar = Str_SplitSubItem(&strPar, ' ');
    if (par.length() > 0){
		if (CFS_CheckFile(strPar) > 0){
			Print(env)
			<< NL << COL_clBlue << "FileName: " << COL_clCyan << par
			<< NL << COL_clBlue << "FileSize: " << COL_clCyan << Str_ToString(CFS_CheckFileSize(strPar))
			<< NL << COL_clBlue << "SHA512:   " << COL_clCyan << Str_ASCIIToHEX(CCY_DigestFile_SHA512(strPar),G_ESCAPE_OFF)
			<< Endl;
		}
		else{
			retCombine.result = "";
			retCombine.forPrint = "";
			retCombine.forSend = "";
			tSBIC_Combine.Command(nullptr, par, &retCombine);
			Print(env)
			<< NL << COL_clBlue << "Input:  " << COL_clCyan << par
			<< NL << COL_clBlue << "Length: " << COL_clCyan << Str_ToString(retCombine.forSend.length())
			<< NL << COL_clBlue << "SHA512: " << COL_clCyan << Str_ASCIIToHEX(CCY_Digest_SHA512(retCombine.forSend),G_ESCAPE_OFF)
			<< Endl;
		}
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
#endif
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_CALC_BASE64::Command(BIC_ENV *env, const STDSTR &par,void *p)const{
	SBIC_RETSTR		retCombine;
	SBIC_Combine	tSBIC_Combine;
	
	STDSTR		par1,par2,par3,parR,strRet;
	par3 = par;
    par1 = Str_ReadSubItem(&par3, " ");
	if ((par1 == "+") || (par1 == "-")){
        par3 = Str_LTrim(par3);
		if (par3.length() > 0){
            parR = par3;
			par2 = Str_Trim(Str_SplitSubItem(&par3, ' '));
			par3 = Str_Trim(par3);
			par3 = Str_SplitSubItem(&par3, ' ');
			
			if (CFS_CheckFile(par2) > 0){
				if (par3.length() > 0){
					Print(env)
					<< NL << COL_clBlue << "FileName: " << COL_clCyan << par
					<< NL << COL_clBlue << "FileSize: " << COL_clCyan << Str_ToString(CFS_CheckFileSize(par2));
					if (par1 == "+"){
						STDout(env) << NL << COL_clBlue << "Base64 Encode: " << COL_clCyan << par3 << Endl;
						CCY_Encode_File_BASE64(par3, par2);
					}
					else{
						STDout(env) << NL << COL_clBlue << "Base64 Decode: " << COL_clCyan << par3 << Endl;
						CCY_Decode_File_BASE64(par3, par2);
					}
				}
				else{
					PrintFail(env,"Input error");
				}
			}
			else{
				if (par1 == "+"){
					retCombine.result = "";
					retCombine.forPrint = "";
					retCombine.forSend = "";
					tSBIC_Combine.Command(nullptr, parR, &retCombine);
					Print(env)
					<< NL << COL_clBlue << "Input:  " << COL_clCyan << parR
					<< NL << COL_clBlue << "Length: " << COL_clCyan << Str_ToString(retCombine.forSend.length());
					STDout(env) << NL << COL_clBlue << "Base64 Encode: " << COL_clCyan << CCY_Encode_BASE64(&strRet, retCombine.forSend) << Endl;
				}
				else{
					Print(env)
					<< NL << COL_clBlue << "Input:  " << COL_clCyan << parR
					<< NL << COL_clBlue << "Length: " << COL_clCyan << Str_ToString(parR.length());
					STDout(env) << NL << COL_clBlue << "Base64 Decode: " << COL_clCyan << CCY_Decode_BASE64(&strRet, parR) << Endl;
				}
			}
		}
	}
	else{
		PrintFail(env,"Input error");
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
#endif
