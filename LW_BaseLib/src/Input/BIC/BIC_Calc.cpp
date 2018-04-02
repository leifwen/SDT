//
//  BIC_Calc.cpp
//  SDT
//
//  Created by Leif Wen on 11/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
#include "BIC_Calc.h"
#ifdef BIC_Calc_h
//------------------------------------------------------------------------------------------//
#include "SYS_File.h"
#include "BIF_Transform.h"
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_CALC_HEX2DEC::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"HEX to DEC, 64bit unsigned");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[vaule]","HEX number");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_CALC_HEX2DEC::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	uint64		calcRet;
	calcRet = Str_HexToDec(msg);
	
	PrintALine(env,COL_clDefault
			   ,"\nHEX:",Str_DecToHex(calcRet)
			   ,"\nDEC:",Str_ToStr(calcRet));
	return(cgCommandID);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_CALC_DEC2HEX::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"DEC to HEX, 64bit unsigned");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[vaule]","DEC number");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_CALC_DEC2HEX::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	PrintALine(env,COL_clDefault
			   ,"\nDEC:",Str_Trim(msg)
			   ,"\nHEX:",Str_DecToHex(msg));
	return(cgCommandID);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#ifdef BIF_Transform_h
#define BIC_CALC_T(mode,text)\
CMDID BIC_CALC_##mode::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{\
	STDSTR	strPar;\
	strPar = msg;\
	strPar = Str_SplitSubItem(&strPar, ' ');\
	if (msg.length() > 0){\
		if (CFS_CheckFile(strPar) > 0){\
			PrintALine(env\
					   , "\n", COL_clBlue, "FileName:", COL_clCyan, msg\
					   , "\n", COL_clBlue, "FileSize:", COL_clCyan, Str_ToStr(CFS_CheckFileSize(strPar))\
					   , "\n", COL_clBlue, text		   , COL_clCyan, Str_ASCIIToHEX(ALG_Digest_##mode(IUD_FILE(strPar)),G_ESCAPE_OFF));\
		}\
		else{\
			BIF_Transform(nullptr, _EMPTY(&strPar), nullptr,msg,CMD_NONE,G_ESCAPE_ON);\
			PrintALine(env\
					   , "\n", COL_clBlue, "Input   :", COL_clCyan, msg\
					   , "\n", COL_clBlue, "Length  :", COL_clCyan, Str_ToStr(strPar.length())\
					   , "\n", COL_clBlue, text		 , COL_clCyan, Str_ASCIIToHEX(ALG_Digest_##mode(strPar),G_ESCAPE_OFF));\
		}\
	}\
	return(cgCommandID);\
}
#else
#define BIC_CALC_T(mode,text)\
CMDID BIC_CALC_##mode::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{\
	STDSTR	strPar;\
	strPar = msg;\
	strPar = Str_SplitSubItem(&strPar, ' ');\
	if (msg.length() > 0){\
		if (CFS_CheckFile(strPar) > 0){\
			PrintALine(env\
					   , "\n", COL_clBlue, "FileName:", COL_clCyan, msg\
					   , "\n", COL_clBlue, "FileSize:", COL_clCyan, Str_ToStr(CFS_CheckFileSize(strPar))\
					   , "\n", COL_clBlue, text		   , COL_clCyan, Str_ASCIIToHEX(ALG_Digest_##mode(IUD_FILE(strPar)),G_ESCAPE_OFF));\
		}\
		else{\
			PrintALine(env\
					   , "\n", COL_clBlue, "Input   :", COL_clCyan, msg\
					   , "\n", COL_clBlue, "Length  :", COL_clCyan, Str_ToStr(msg.length())\
					   , "\n", COL_clBlue, text		 , COL_clCyan, Str_ASCIIToHEX(ALG_Digest_##mode(msg),G_ESCAPE_OFF));\
		}\
	}\
	return(cgCommandID);\
}
#endif
//------------------------------------------------------------------------------------------//
#define BIC_CALC_TH(mode,text)\
CMDID	BIC_CALC_##mode::Help(CMD_ENV* env,uint32 flag)const{\
	PrintHelpItem(env,cgCommand,text);\
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))\
		return(cgCommandID);\
	PrintHelpSubItem(env,"<filename|input>","File name or input string");\
	return(cgCommandID);\
};
//------------------------------------------------------------------------------------------//
#ifdef ALG_Digest_h
BIC_CALC_T(MD5,		"MD5     :");
BIC_CALC_T(SHA1,	"SHA1    :");
BIC_CALC_T(SHA224,	"SHA224  :");
BIC_CALC_T(SHA256,	"SHA256  :");
BIC_CALC_T(SHA384,	"SHA384  :");
BIC_CALC_T(SHA512,	"SHA512  :");

BIC_CALC_TH(MD5,	"MD5");
BIC_CALC_TH(SHA1,	"SHA1");
BIC_CALC_TH(SHA224,	"SHA224");
BIC_CALC_TH(SHA256,	"SHA256");
BIC_CALC_TH(SHA384,	"SHA384");
BIC_CALC_TH(SHA512,	"SHA512");
#endif
//------------------------------------------------------------------------------------------//
#ifdef ALG_BASE64_h
//------------------------------------------------------------------------------------------//
CMDID BIC_CALC_BASE64::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"BASE64");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"<+|->"			,"Encode/Decode");
	PrintHelpSubItem(env,"<filename|input>"	,"File name or input string");
	PrintHelpSubItem(env,"[filename]"		,"Output file name");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_CALC_BASE64::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	STDSTR		par1,par2,par3,parR,strRet;
	ALG_BASE64	b64;
	
	b64.InitSize(512);
	par3 = msg;
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
					if (par1 == "+"){
						PrintALine(env
								   , "\n", COL_clBlue, "FileName:", COL_clCyan, par2
								   , "\n", COL_clBlue, "FileSize:", COL_clCyan, Str_ToStr(CFS_CheckFileSize(par2))
								   , "\n", COL_clBlue, "Encode  :", COL_clCyan, par3);
						
						b64.InitCFG(ALG_BASE64::CFG_Encode | ALG_BASE64::CFG_NL).AllIn(nullptr, OUD_FILEWR(par3), IUD_FILE(par2));
					}
					else{
						PrintALine(env
								   , "\n", COL_clBlue, "FileName:", COL_clCyan, par2
								   , "\n", COL_clBlue, "FileSize:", COL_clCyan, Str_ToStr(CFS_CheckFileSize(par2))
								   , "\n", COL_clBlue, "Decode  :", COL_clCyan, par3);
						b64.InitCFG(ALG_BASE64::CFG_Decode | ALG_BASE64::CFG_NL).AllIn(nullptr, OUD_FILEWR(par3), IUD_FILE(par2));
					}
				}
				else{
					PrintFail(env,"Input error");
				}
			}
			else{
			#ifdef BIF_Transform_h
				BIF_Transform(nullptr, _EMPTY(&par2), nullptr,parR,CMD_NONE,G_ESCAPE_ON);
			#else
				par2 = parR;
			#endif
				if (par1 == "+"){
					b64.InitCFG(ALG_BASE64::CFG_Encode | ALG_BASE64::CFG_NL).AllIn(nullptr, _EMPTY(&par3), par2);
					PrintALine(env
							   , "\n", COL_clBlue, "Input :", COL_clCyan, parR
							   , "\n", COL_clBlue, "Length:", COL_clCyan, Str_ToStr(par2)
							   , "\n", COL_clBlue, "Encode:", COL_clCyan, par3);
				}
				else{
					b64.InitCFG(ALG_BASE64::CFG_Decode | ALG_BASE64::CFG_NL).AllIn(nullptr, _EMPTY(&par3), par2);
					PrintALine(env
							   , "\n", COL_clBlue, "Input :", COL_clCyan, parR
							   , "\n", COL_clBlue, "Length:", COL_clCyan, Str_ToStr(par2)
							   , "\n", COL_clBlue, "Decode:", COL_clCyan, par3);
				}
			}
		}
	}
	else{
		PrintFail(env,"Input error");
	}
	return(cgCommandID);
}
//------------------------------------------------------------------------------------------//
#endif
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
BIC_CALC::BIC_CALC(void) : BIC_BASE_S(){
	SetSFlag(CMD_blTrySubCMD);
	cgCommandID = BIC_ID_CALC;
	cgCommand = "calc";
	cgConsoleName = cgCommand;
	cgHelpName = "Calculator";
	
	Add(cgSub_hex2dec) < cgSub_dec2hex
#ifdef ALG_Digest_h
	< cgSub_md5 < cgSub_sha1 < cgSub_sha224 < cgSub_sha256 < cgSub_sha384 < cgSub_sha512
#endif
#ifdef ALG_BASE64_h
	< cgSub_base64;
#endif
	;
};
//------------------------------------------------------------------------------------------//
#endif /* BIC_Calc_h */
