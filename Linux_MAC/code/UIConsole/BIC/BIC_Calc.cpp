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
//------------------------------------------------------------------------------------------//
int32 BIC_CALC::Help(BICPAR *tBICPAR, int32 blDetail)const{
	PrintHelpItem(tBICPAR, cgCommand, "->Calculator.");
	if (blDetail == 0)
		return(cgReturnCode);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_CALC_HEX2DEC::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"HEX to DEC, 64bit unsigned");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     [vaule]","HEX number.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_CALC_HEX2DEC::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	uint64		calcRet;
	
	calcRet = Str_HexToDec(par);
	PrintStrN(tBICPAR, "HEX: " + Str_DecToHex(calcRet), RICH_LIN_clDefault);
	PrintStrN(tBICPAR, "DEC: " + Str_UInt64ToString(calcRet), RICH_LIN_clDefault);
	
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_CALC_DEC2HEX::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"DEC to HEX, 64bit unsigned");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     [vaule]","DEC number.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_CALC_DEC2HEX::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{

	PrintStrN(tBICPAR, "DEC: " + Str_Trim(par), RICH_LIN_clDefault);
	PrintStrN(tBICPAR, "HEX: " + Str_DecToHex(par), RICH_LIN_clDefault);
	
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
#if (defined USE_OPENSSL) || (defined CommonDefH_MAC)
//------------------------------------------------------------------------------------------//
int32 BIC_CALC_MD5::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"MD5");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     <filename|input>","File name or input string.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_CALC_MD5::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	std::string	strPar;
	strPar = par;
	strPar = Str_SplitSubItem(&strPar, ' ');
	if (par.length() > 0){
		if (CFS_CheckFile(par) > 0){
			PrintStrN(tBICPAR, "FileName: ", RICH_LIN_clBlue);
			PrintStr(tBICPAR, par, RICH_LIN_clCyan);
			PrintStrN(tBICPAR, "FileSize: ", RICH_LIN_clBlue);
			PrintStr(tBICPAR, Str_Int64ToString(CFS_CheckFileSize(strPar)), RICH_LIN_clCyan);
			PrintStrN(tBICPAR, "MD5:      ", RICH_LIN_clBlue);
			PrintStr(tBICPAR, Str_ASCIIToHEX(CCY_DigestFile_MD5(strPar),G_ESCAPE_OFF), RICH_LIN_clCyan);
		}
		else{
			PrintStrN(tBICPAR, "Input:  ", RICH_LIN_clBlue);
			PrintStr(tBICPAR, par, RICH_LIN_clCyan);
			PrintStrN(tBICPAR, "Length: ", RICH_LIN_clBlue);
			PrintStr(tBICPAR, Str_Int64ToString(par.length()), RICH_LIN_clCyan);
			PrintStrN(tBICPAR, "MD5:    ", RICH_LIN_clBlue);
			PrintStr(tBICPAR, Str_ASCIIToHEX(CCY_Digest_MD5(par),G_ESCAPE_OFF), RICH_LIN_clCyan);
		}
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_CALC_SHA1::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"SHA1");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     <filename|input>","File name or input string.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_CALC_SHA1::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	std::string	strPar;
	strPar = par;
	strPar = Str_SplitSubItem(&strPar, ' ');
	if (par.length() > 0){
		if (CFS_CheckFile(par) > 0){
			PrintStrN(tBICPAR, "FileName: ", RICH_LIN_clBlue);
			PrintStr(tBICPAR, par, RICH_LIN_clCyan);
			PrintStrN(tBICPAR, "FileSize: ", RICH_LIN_clBlue);
			PrintStr(tBICPAR, Str_Int64ToString(CFS_CheckFileSize(strPar)), RICH_LIN_clCyan);
			PrintStrN(tBICPAR, "SHA1:     ", RICH_LIN_clBlue);
			PrintStr(tBICPAR, Str_ASCIIToHEX(CCY_DigestFile_SHA1(strPar),G_ESCAPE_OFF), RICH_LIN_clCyan);
		}
		else{
			PrintStrN(tBICPAR, "Input:  ", RICH_LIN_clBlue);
			PrintStr(tBICPAR, par, RICH_LIN_clCyan);
			PrintStrN(tBICPAR, "Length: ", RICH_LIN_clBlue);
			PrintStr(tBICPAR, Str_Int64ToString(par.length()), RICH_LIN_clCyan);
			PrintStrN(tBICPAR, "SHA1:   ", RICH_LIN_clBlue);
			PrintStr(tBICPAR, Str_ASCIIToHEX(CCY_Digest_SHA1(par),G_ESCAPE_OFF), RICH_LIN_clCyan);
		}
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_CALC_SHA224::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"SHA224");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     <filename|input>","File name or input string.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_CALC_SHA224::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	std::string	strPar;
	strPar = par;
	strPar = Str_SplitSubItem(&strPar, ' ');
	if (par.length() > 0){
		if (CFS_CheckFile(par) > 0){
			PrintStrN(tBICPAR, "FileName: ", RICH_LIN_clBlue);
			PrintStr(tBICPAR, par, RICH_LIN_clCyan);
			PrintStrN(tBICPAR, "FileSize: ", RICH_LIN_clBlue);
			PrintStr(tBICPAR, Str_Int64ToString(CFS_CheckFileSize(strPar)), RICH_LIN_clCyan);
			PrintStrN(tBICPAR, "SHA224:   ", RICH_LIN_clBlue);
			PrintStr(tBICPAR, Str_ASCIIToHEX(CCY_DigestFile_SHA224(strPar),G_ESCAPE_OFF), RICH_LIN_clCyan);
		}
		else{
			PrintStrN(tBICPAR, "Input:  ", RICH_LIN_clBlue);
			PrintStr(tBICPAR, par, RICH_LIN_clCyan);
			PrintStrN(tBICPAR, "Length: ", RICH_LIN_clBlue);
			PrintStr(tBICPAR, Str_Int64ToString(par.length()), RICH_LIN_clCyan);
			PrintStrN(tBICPAR, "SHA224: ", RICH_LIN_clBlue);
			PrintStr(tBICPAR, Str_ASCIIToHEX(CCY_Digest_SHA224(par),G_ESCAPE_OFF), RICH_LIN_clCyan);
		}
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_CALC_SHA256::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"SHA256");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     <filename|input>","File name or input string.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_CALC_SHA256::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	std::string	strPar;
	strPar = par;
	strPar = Str_SplitSubItem(&strPar, ' ');
	if (par.length() > 0){
		if (CFS_CheckFile(par) > 0){
			PrintStrN(tBICPAR, "FileName: ", RICH_LIN_clBlue);
			PrintStr(tBICPAR, par, RICH_LIN_clCyan);
			PrintStrN(tBICPAR, "FileSize: ", RICH_LIN_clBlue);
			PrintStr(tBICPAR, Str_Int64ToString(CFS_CheckFileSize(strPar)), RICH_LIN_clCyan);
			PrintStrN(tBICPAR, "SHA256:   ", RICH_LIN_clBlue);
			PrintStr(tBICPAR, Str_ASCIIToHEX(CCY_DigestFile_SHA256(strPar),G_ESCAPE_OFF), RICH_LIN_clCyan);
		}
		else{
			PrintStrN(tBICPAR, "Input:  ", RICH_LIN_clBlue);
			PrintStr(tBICPAR, par, RICH_LIN_clCyan);
			PrintStrN(tBICPAR, "Length: ", RICH_LIN_clBlue);
			PrintStr(tBICPAR, Str_Int64ToString(par.length()), RICH_LIN_clCyan);
			PrintStrN(tBICPAR, "SHA256: ", RICH_LIN_clBlue);
			PrintStr(tBICPAR, Str_ASCIIToHEX(CCY_Digest_SHA256(par),G_ESCAPE_OFF), RICH_LIN_clCyan);
		}
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_CALC_SHA384::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"SHA384");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     <filename|input>","File name or input string.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_CALC_SHA384::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	std::string	strPar;
	strPar = par;
	strPar = Str_SplitSubItem(&strPar, ' ');
	if (par.length() > 0){
		if (CFS_CheckFile(par) > 0){
			PrintStrN(tBICPAR, "FileName: ", RICH_LIN_clBlue);
			PrintStr(tBICPAR, par, RICH_LIN_clCyan);
			PrintStrN(tBICPAR, "FileSize: ", RICH_LIN_clBlue);
			PrintStr(tBICPAR, Str_Int64ToString(CFS_CheckFileSize(strPar)), RICH_LIN_clCyan);
			PrintStrN(tBICPAR, "SHA384:   ", RICH_LIN_clBlue);
			PrintStr(tBICPAR, Str_ASCIIToHEX(CCY_DigestFile_SHA384(strPar),G_ESCAPE_OFF), RICH_LIN_clCyan);
		}
		else{
			PrintStrN(tBICPAR, "Input:  ", RICH_LIN_clBlue);
			PrintStr(tBICPAR, par, RICH_LIN_clCyan);
			PrintStrN(tBICPAR, "Length: ", RICH_LIN_clBlue);
			PrintStr(tBICPAR, Str_Int64ToString(par.length()), RICH_LIN_clCyan);
			PrintStrN(tBICPAR, "SHA384: ", RICH_LIN_clBlue);
			PrintStr(tBICPAR, Str_ASCIIToHEX(CCY_Digest_SHA384(par),G_ESCAPE_OFF), RICH_LIN_clCyan);
		}
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_CALC_SHA512::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"SHA512");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     <filename|input>","File name or input string.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_CALC_SHA512::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	std::string	strPar;
	strPar = par;
	strPar = Str_SplitSubItem(&strPar, ' ');
		if (par.length() > 0){
		if (CFS_CheckFile(par) > 0){
			PrintStrN(tBICPAR, "FileName: ", RICH_LIN_clBlue);
			PrintStr(tBICPAR, par, RICH_LIN_clCyan);
			PrintStrN(tBICPAR, "FileSize: ", RICH_LIN_clBlue);
			PrintStr(tBICPAR, Str_Int64ToString(CFS_CheckFileSize(strPar)), RICH_LIN_clCyan);
			PrintStrN(tBICPAR, "SHA512:   ", RICH_LIN_clBlue);
			PrintStr(tBICPAR, Str_ASCIIToHEX(CCY_DigestFile_SHA512(strPar),G_ESCAPE_OFF), RICH_LIN_clCyan);
		}
		else{
			PrintStrN(tBICPAR, "Input:  ", RICH_LIN_clBlue);
			PrintStr(tBICPAR, par, RICH_LIN_clCyan);
			PrintStrN(tBICPAR, "Length: ", RICH_LIN_clBlue);
			PrintStr(tBICPAR, Str_Int64ToString(par.length()), RICH_LIN_clCyan);
			PrintStrN(tBICPAR, "SHA512: ", RICH_LIN_clBlue);
			PrintStr(tBICPAR, Str_ASCIIToHEX(CCY_Digest_SHA512(par),G_ESCAPE_OFF), RICH_LIN_clCyan);
		}
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
#endif
//------------------------------------------------------------------------------------------//
int32 BIC_CALC_BASE64::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"BASE64");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     <+|->"				,"Encode/Decode.");
	PrintHelpItem(tBICPAR,"     <filename|input>"	,"File name or input string.");
	PrintHelpItem(tBICPAR,"     [filename]"			,"Output file name.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_CALC_BASE64::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	std::string		par1,par2,par3,parR,strRet;
	std::string	strPar;
	parR = par;
	strPar = Str_SplitSubItem(&parR, ' ');
	if ((par1 == "+") || (par1 == "-")){
		if (parR.length() > 0){
			par3 = parR;
			par2 = Str_Trim(Str_SplitSubItem(&par3, ' '));
			par3 = Str_Trim(par3);
			par3 = Str_SplitSubItem(&par3, ' ');
			
			if (CFS_CheckFile(par2) > 0){
				if (par3.length() > 0){
					PrintStrN(tBICPAR, "FileName: ", RICH_LIN_clBlue);
					PrintStr(tBICPAR, par, RICH_LIN_clCyan);
					PrintStrN(tBICPAR, "FileSize: ", RICH_LIN_clBlue);
					PrintStr(tBICPAR, Str_Int64ToString(CFS_CheckFileSize(par2)), RICH_LIN_clCyan);
					if (par1 == "+"){
						PrintStrN(tBICPAR, "Base64 Encode: ", RICH_LIN_clBlue);
						PrintStrN(tBICPAR, par3, RICH_LIN_clCyan);
						CCY_Encode_File_BASE64(par3, par2,CCT_NL_YES);
					}
					else{
						PrintStrN(tBICPAR, "Base64 Decode: ", RICH_LIN_clBlue);
						PrintStrN(tBICPAR, par3, RICH_LIN_clCyan);
						CCY_Decode_File_BASE64(par3, par2,CCT_NL_YES);
					}
				}
				else{
					PrintDoRet(tBICPAR,"Input error");
				}
			}
			else{
				PrintStrN(tBICPAR, "Input:  ", RICH_LIN_clBlue);
				PrintStr(tBICPAR, parR, RICH_LIN_clCyan);
				PrintStrN(tBICPAR, "Length: ", RICH_LIN_clBlue);
				PrintStr(tBICPAR, Str_Int64ToString(parR.length()), RICH_LIN_clCyan);
				if (par1 == "+"){
					PrintStrN(tBICPAR, "Base64 Encode: ", RICH_LIN_clBlue);
					PrintStrN(tBICPAR, CCY_Encode_BASE64(&strRet, parR), RICH_LIN_clCyan);
				}
				else{
					PrintStrN(tBICPAR, "Base64 Decode: ", RICH_LIN_clBlue);
					PrintStrN(tBICPAR, CCY_Decode_BASE64(&strRet, parR), RICH_LIN_clCyan);
				}
			}
		}
	}
	else{
		PrintDoRet(tBICPAR,"Input error");
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//