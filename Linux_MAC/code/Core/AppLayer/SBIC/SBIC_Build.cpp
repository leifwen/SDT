/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: SBIC_Build.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.08.12
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "SBIC_Build.h"
#include "SYS_Time.h"
#include "Comm_Convert.h"
#include "Comm_File.h"
#include "Comm_Crypto.h"
//------------------------------------------------------------------------------------------//
#ifdef SBIC_BuildH
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_SQ::Check(SBIC_ENV *env,const STDSTR &rawIn,STDSTR *retPar)const{
	STDSTR	oFullCMD;
	
	oFullCMD = Str_LTrim(rawIn);
	
	if (oFullCMD.length() > 1){
		if (oFullCMD.substr(0,2) == "''"){
			*retPar = oFullCMD.substr(1);
			return(cgReturnCode);
		}
	}
	*retPar = "";
	return(SBI_RETCODE_NO);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_SQ::Command(SBIC_ENV *env,const STDSTR &par,void *retStr)const{
	SBIC_RETSTR *ret = static_cast<SBIC_RETSTR*>(retStr);
	ret->result = Str_Trim(par);
	ret->forPrint += ret->result;
	ret->forSend += ret->result;
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//--------------------------------------------- ---------------------------------------------//
int32 SBIC_HEX::Command(SBIC_ENV *env,const STDSTR &par,void *retStr)const{
	SBIC_RETSTR *ret = static_cast<SBIC_RETSTR*>(retStr);
	ret->result = Str_HEXToASCII(par);
	ret->forPrint += ("[HEX:" + Str_RTrim(Str_ASCIIToHEXs(ret->result, G_ESCAPE_OFF)) +"]");
	ret->forSend += ret->result;
	return(cgReturnCode);
}//------------------------------------------------------------------------------------------//
//--------------------------------------------- ---------------------------------------------//
int32 SBIC_VIEWINHEX::Command(SBIC_ENV *env,const STDSTR &par,void *retStr)const{
	SBIC_RETSTR		*ret = static_cast<SBIC_RETSTR*>(retStr);
	SBIC_RETSTR		retCombine;
	SBIC_Combine	tSBIC_Combine;
	
	if (par.length() > 0){
		retCombine.result = "";
		retCombine.forPrint = "";
		retCombine.forSend = "";
		tSBIC_Combine.Command(env, par, &retCombine);
		ret->forPrint += ("[HEX:" + Str_RTrim(Str_ASCIIToHEXs(retCombine.forSend, G_ESCAPE_OFF)) +"]");
		ret->forSend += retCombine.forSend;
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_Time::Command(SBIC_ENV *env,const STDSTR &par,void *retStr)const{
	SBIC_RETSTR *ret = static_cast<SBIC_RETSTR*>(retStr);
	
	TIME	eTime;
	if (par.length() == 0){
		eTime.Now();
		ret->result = eTime.FormatDateTime(DEFAULT_TIMEFORMATE);
	}
	else{
		eTime.Now();
		ret->result = eTime.FormatDateTime(Str_Trim(par));
	}
	
	ret->forPrint += ret->result;
	Str_CharToStr(&ret->forSend, (uint8*)ret->result.c_str(), ret->result.length(), G_ASCII, G_ESCAPE_ON, G_SPACE_OFF, G_APPEND_ON);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_RET::Command(SBIC_ENV *env,const STDSTR &par,void *retStr)const{
	SBIC_RETSTR *ret = static_cast<SBIC_RETSTR*>(retStr);
	
	STDSTR	strT,strSearchRet;
	int32	num;
	
	ret->result = "";
	if (env != nullptr){
		strSearchRet = env->cgSearchRet;
		num = atoi(Str_Trim(par).c_str());
		strT = "";
		while(num > 0){
			strT = Str_ReadSubItem(&strSearchRet,",");
			-- num;
		}
		ret->result = Str_HEXToASCII(strT);
		ret->forPrint += ret->result;
		ret->forSend += ret->result;
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_REPLACE::Command(SBIC_ENV *env,const STDSTR &par,void *retStr)const{
	SBIC_RETSTR *ret = static_cast<SBIC_RETSTR*>(retStr);
	
	STDSTR	strDefine,strVar,strValue,strT;
	
	ret->result = par;
	if (env != nullptr){
		strDefine = env->cgDefine;
		while(strDefine.length() > 0){
			strT = Str_ReadSubItem(&strDefine,";");
			if (strT.length() > 0){
				SplitPar1(strVar,strValue,strT,"=");
				ret->result = Str_Replace(ret->result, strVar, strValue);
			}
		}
		ret->forPrint += ret->result;
		ret->forSend += ret->result;
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_STRING::Command(SBIC_ENV *env,const STDSTR &par,void *retStr)const{
	SBIC_RETSTR *ret = static_cast<SBIC_RETSTR*>(retStr);
	
	STDSTR		strT;
	int32		num,line;
	char		charT;
	
	num = atoi(Str_Trim(par).c_str());
	ret->forPrint += "\n";
	line = 0;
	if (num > 64 * 512)
		num = 64 * 512;
	while(num > 3){
		++ line;
		strT = Str_ToString(line);
		if (strT.length() < 3)
			strT.insert(0,3 - strT.length(),'0');
		ret->forPrint += strT;
		ret->forSend += strT;
		num -= 3;
		charT = 62;
		while((charT < 92) && (num > 1)){
			strT = charT;
			ret->forPrint += strT;
			ret->forSend += strT;
			++ charT;
			-- num;
		}
		charT = 93;
		while((charT < 123) && (num > 1)){
			strT = charT;
			ret->forPrint += strT;
			ret->forSend += strT;
			++ charT;
			-- num;
		}
		if (num > 0){
			ret->forPrint += "\\n\n";
			ret->forSend += "\n";
			-- num;
		}
	}
	if (num == 3){
		ret->forPrint += ">?\\n\n";
		ret->forSend += ">?\n";
	}
	else if (num == 2){
		ret->forPrint += "?\\n\n";
		ret->forSend += "?\n";
	}
	else if (num == 1){
		ret->forPrint += "\\n\n";
		ret->forSend += "\n";
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
#if (defined USE_OPENSSL) || (defined CommonDefH_MAC)
//------------------------------------------------------------------------------------------//
int32 SBIC_CALC_MD5::Command(SBIC_ENV *env,const STDSTR &par,void *retStr)const{
	SBIC_RETSTR		*ret = static_cast<SBIC_RETSTR*>(retStr);
	SBIC_RETSTR		retCombine;
	SBIC_Combine	tSBIC_Combine;
	
	ret->result = par;
	ret->result = Str_SplitSubItem(&ret->result, ' ');
	if (par.length() > 0){
		if (CFS_CheckFile(ret->result) > 0){
			 ret->result = CCY_DigestFile_MD5(ret->result);
		}
		else{
			retCombine.result = "";
			retCombine.forPrint = "";
			retCombine.forSend = "";
			tSBIC_Combine.Command(env, par, &retCombine);
			ret->result = CCY_Digest_MD5(retCombine.forSend);
		}
		ret->forPrint += ("[MD5:" + Str_RTrim(Str_ASCIIToHEXs(ret->result, G_ESCAPE_OFF)) +"]");
		ret->forSend += ret->result;
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_CALC_SHA1::Command(SBIC_ENV *env,const STDSTR &par,void *retStr)const{
	SBIC_RETSTR		*ret = static_cast<SBIC_RETSTR*>(retStr);
	SBIC_RETSTR		retCombine;
	SBIC_Combine	tSBIC_Combine;
	
	ret->result = par;
	ret->result = Str_SplitSubItem(&ret->result, ' ');
	if (par.length() > 0){
		if (CFS_CheckFile(ret->result) > 0){
			ret->result = CCY_DigestFile_SHA1(ret->result);
		}
		else{
			retCombine.result = "";
			retCombine.forPrint = "";
			retCombine.forSend = "";
			tSBIC_Combine.Command(env, par, &retCombine);
			ret->result = CCY_Digest_SHA1(retCombine.forSend);
		}
		ret->forPrint += ("[SHA1:" + Str_RTrim(Str_ASCIIToHEXs(ret->result, G_ESCAPE_OFF)) +"]");
		ret->forSend += ret->result;
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_CALC_SHA224::Command(SBIC_ENV *env,const STDSTR &par,void *retStr)const{
	SBIC_RETSTR		*ret = static_cast<SBIC_RETSTR*>(retStr);
	SBIC_RETSTR		retCombine;
	SBIC_Combine	tSBIC_Combine;
	
	ret->result = par;
	ret->result = Str_SplitSubItem(&ret->result, ' ');
	if (par.length() > 0){
		if (CFS_CheckFile(ret->result) > 0){
			ret->result = CCY_DigestFile_SHA224(ret->result);
		}
		else{
			retCombine.result = "";
			retCombine.forPrint = "";
			retCombine.forSend = "";
			tSBIC_Combine.Command(env, par, &retCombine);
			ret->result = CCY_Digest_SHA224(retCombine.forSend);
		}
		ret->forPrint += ("[SHA224:" + Str_RTrim(Str_ASCIIToHEXs(ret->result, G_ESCAPE_OFF)) +"]");
		ret->forSend += ret->result;
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_CALC_SHA256::Command(SBIC_ENV *env,const STDSTR &par,void *retStr)const{
	SBIC_RETSTR		*ret = static_cast<SBIC_RETSTR*>(retStr);
	SBIC_RETSTR		retCombine;
	SBIC_Combine	tSBIC_Combine;
	
	ret->result = par;
	ret->result = Str_SplitSubItem(&ret->result, ' ');
	if (par.length() > 0){
		if (CFS_CheckFile(ret->result) > 0){
			ret->result = CCY_DigestFile_SHA256(ret->result);
		}
		else{
			retCombine.result = "";
			retCombine.forPrint = "";
			retCombine.forSend = "";
			tSBIC_Combine.Command(env, par, &retCombine);
			ret->result = CCY_Digest_SHA256(retCombine.forSend);
		}
		ret->forPrint += ("[SHA224:" + Str_RTrim(Str_ASCIIToHEXs(ret->result, G_ESCAPE_OFF)) +"]");
		ret->forSend += ret->result;
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_CALC_SHA384::Command(SBIC_ENV *env,const STDSTR &par,void *retStr)const{
	SBIC_RETSTR		*ret = static_cast<SBIC_RETSTR*>(retStr);
	SBIC_RETSTR		retCombine;
	SBIC_Combine	tSBIC_Combine;
	
	ret->result = par;
	ret->result = Str_SplitSubItem(&ret->result, ' ');
	if (par.length() > 0){
		if (CFS_CheckFile(ret->result) > 0){
			ret->result = CCY_DigestFile_SHA384(ret->result);
		}
		else{
			retCombine.result = "";
			retCombine.forPrint = "";
			retCombine.forSend = "";
			tSBIC_Combine.Command(env, par, &retCombine);
			ret->result = CCY_Digest_SHA384(retCombine.forSend);
		}
		ret->forPrint += ("[SHA384:" + Str_RTrim(Str_ASCIIToHEXs(ret->result, G_ESCAPE_OFF)) +"]");
		ret->forSend += ret->result;
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_CALC_SHA512::Command(SBIC_ENV *env,const STDSTR &par,void *retStr)const{
	SBIC_RETSTR		*ret = static_cast<SBIC_RETSTR*>(retStr);
	SBIC_RETSTR		retCombine;
	SBIC_Combine	tSBIC_Combine;
	
	ret->result = par;
	ret->result = Str_SplitSubItem(&ret->result, ' ');
	if (par.length() > 0){
		if (CFS_CheckFile(ret->result) > 0){
			ret->result = CCY_DigestFile_SHA512(ret->result);
		}
		else{
			retCombine.result = "";
			retCombine.forPrint = "";
			retCombine.forSend = "";
			tSBIC_Combine.Command(env, par, &retCombine);
			ret->result = CCY_Digest_SHA512(retCombine.forSend);
		}
		ret->forPrint += ("[SHA512:" + Str_RTrim(Str_ASCIIToHEXs(ret->result, G_ESCAPE_OFF)) +"]");
		ret->forSend += ret->result;
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
#endif
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_CALC_BASE64::Command(SBIC_ENV *env,const STDSTR &par,void *retStr)const{
	SBIC_RETSTR		*ret = static_cast<SBIC_RETSTR*>(retStr);
	SBIC_RETSTR		retCombine;
	SBIC_Combine	tSBIC_Combine;
	
	STDSTR		par1,par2,parR;
	
	SplitPar1(par1, par2, par, ",");
	
	Str_LowerCaseSelf(par1);
	if ((par1 == "e") || (par1 == "d")){
		if (par2.length() > 0){
			parR = par2;
			par2 = Str_Trim(Str_SplitSubItem(&par2, ' '));
			ret->result = "";
			if (CFS_CheckFile(par2) > 0){
				if (par1 == "e"){
					CCY_Encode_FileToStr_BASE64(&ret->forSend, par2,CCT_NL_YES,G_APPEND_ON);
					ret->forPrint += "[BASE64 encode file:";
				}
				else{
					CCY_Decode_FileToStr_BASE64(&ret->forSend, par2,CCT_NL_YES,G_APPEND_ON);
					ret->forPrint += "[BASE64 decode file:";
				}
				ret->forPrint += par2;
				ret->forPrint += "]";
				return(SBI_RETCODE_COMBINE_CALC_BASE64_FILE);
			}
			else{
				retCombine.result = "";
				retCombine.forPrint = "";
				retCombine.forSend = "";
				tSBIC_Combine.Command(env, par2, &retCombine);
				
				if (par1 == "e"){
					CCY_Encode_BASE64(&ret->result, retCombine.forSend);
				}
				else{
					CCY_Decode_BASE64(&ret->result, retCombine.forSend);
				}
				ret->forPrint += ret->result;
				ret->forSend += ret->result;
			}
		}
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_SENDFILE::Command(SBIC_ENV *env,const STDSTR &par,void *retStr)const{
	SBIC_RETSTR		*ret = static_cast<SBIC_RETSTR*>(retStr);

	ret->result = par;
	ret->result = Str_SplitSubItem(&ret->result, ' ');
	if (par.length() > 0){
		if (CFS_CheckFile(ret->result) > 0)
			CFS_ReadFile(&ret->forSend,ret->result,-1,0,G_APPEND_ON);
		ret->forPrint += "[Send file:";
		ret->forPrint += ret->result;
		ret->forPrint += "]";
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_Combine::Command(SBIC_ENV *env,const STDSTR &par,void *retStr)const{
	SBIC_RETSTR *ret = static_cast<SBIC_RETSTR*>(retStr);
	
	STDSTR	finalResult,strOInput,strRet,strSub;
	int32	retCode;
	
	FormatString(&strOInput,par);
	finalResult = "";
	while(strOInput.length() > 0){
		SplitLetter(&strSub,&strOInput);
		retCode = ExecuteTraversalChild(env,strSub,retStr);
		if (retCode == SBI_RETCODE_NO)
			retCode = Execute(env,strSub,retStr);
		if (retCode == SBI_RETCODE_NO){
			ret->result = strSub;
			ret->forPrint += ret->result;
			Str_CharToStr(&ret->forSend, (uint8*)ret->result.c_str(), ret->result.length(), G_ASCII, G_ESCAPE_ON, G_SPACE_OFF, G_APPEND_ON);
		}
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
void SBIC_Combine::FormatString(STDSTR *retStr,const STDSTR &strIn){
	//formate to HEX without space
	//escape +,"
	STDSTR				strData,strResult;
	STDSTR::size_type	i,length;
	uint8				charData;
	
	length = strIn.length();
	i = 0;
	*retStr = "";
	strData = "";
	
	while(i < length){
		charData = strIn[i];
		++ i;
		if (charData == '+'){
			*retStr += CCY_Encode_BASE64(Str_Trim(strData),CCT_NL_NO);
			*retStr += ',';
			strData = "";
		}
		else{
			if ((charData == '\\') && (i < length)){	//   (\+)
				charData = strIn[i];
				++ i;
				if (charData != '+')
					strData += "\\";
			}
			strData += charData;
		}
	}
	*retStr += CCY_Encode_BASE64(Str_Trim(strData),CCT_NL_NO);
}
//------------------------------------------------------------------------------------------//
void SBIC_Combine::SplitLetter(STDSTR *retStr,STDSTR *strIn){
	// remove " in front and back.
	
	*retStr = CCY_Decode_BASE64(Str_ReadSubItem(strIn, ","));
	
	if (retStr->length() > 1){
		if (((*retStr)[0] == '"') && ((*retStr)[retStr->length() - 1] == '"'))
			*retStr = retStr->substr(1,retStr->length() - 2);
	}
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_Print::Help(SBIC_ENV *env,int32 blDetail )const{
	PrintB(env,".CMD = Print=<Expression> -->Print log.");
	PrintB(env,"  Command = <'Print=<Expression>>[//COMMENT]");
	PrintP(env,"  Notes:1.Expression Operators is +.");
	PrintP(env,"        2.Can use \"\" set string. Use \\\", \\+ to escape \", +.");
	PrintP(env,"        3.Support \\0xhh, \\0Xhh, \\a, \\b, \\f, \\n, \\r, \\t, \\v, \\\\, \\', \\\", \\0, \\/, \\*, \\?.");
	PrintP(env,"        4.support sub command :same as 'combine.");
	PrintP(env,"     eg:");
	PrintP(env,"       Command = 'Print = Happy + New + Year //Print HappyNewYear.");
	
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_Print::Command(SBIC_ENV *env,const STDSTR &par,void *p)const{
	STDSTR 	strTempData,retForSend,retForPrint;
	
	CreateCommand(env,nullptr,&retForPrint,par,CMD_NONE,G_ESCAPE_ON);
	if ((env != nullptr) && (CheckPrintSBICinfo(env) != 0) && (CheckCommandExplain(env) != 0))
		PrintExecute(env,retForPrint);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_Print::CreateCommand(SBIC_ENV *env,STDSTR *retForSend,STDSTR *retForPrint,const STDSTR &inputCommand,CMD_TAIL tail,G_ESCAPE blEscape)const{
	SBIC_RETSTR	retStr;
	int32		eRetCode;
	STDSTR		strOCommand;
	
	retStr.result = "";
	retStr.forPrint = "";
	retStr.forSend = "";
	strOCommand = SBIC_Node::DelComment(inputCommand);
	eRetCode = ExecuteTraversalChild(env,strOCommand,&retStr);
	switch (eRetCode){
		case SBI_RETCODE_COMBINE_CALC_BASE64_FILE:
		case SBI_RETCODE_SENDFILE:
			break;
		case SBI_RETCODE_NO:
			retStr.result = "";
			retStr.forPrint = strOCommand;
			Str_CharToStr(&retStr.forSend, (uint8*)strOCommand.c_str(), strOCommand.length(), G_ASCII, blEscape, G_SPACE_OFF, G_APPEND_OFF);
		default:
			switch (tail) {
				case CMD_R:
					retStr.forPrint += "\\r";
					retStr.forSend += "\r";
					break;
				case CMD_N:
					retStr.forPrint += "\\n";
					retStr.forSend += "\n";
					break;
				case CMD_RN:
					retStr.forPrint += "\\r\\n";
					retStr.forSend += "\r\n";
					break;
				default:
					break;
			}
			break;
	}
	if (retForSend != nullptr)
		*retForSend = retStr.forSend;
	if (retForPrint != nullptr)
		*retForPrint = retStr.forPrint;
	return(eRetCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif
