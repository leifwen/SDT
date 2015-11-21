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
#include "Device.h"
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_SQ::Check(SBICPAR *tBICPAR,const std::string &fullCMD,std::string *retPar)const{
	std::string	oFullCMD;
	
	oFullCMD = Str_LTrim(fullCMD);
	
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
int32 SBIC_SQ::Help(SBICPAR *tBICPAR,int32 blDetail)const{
	PrintB(tBICPAR,".CMD = '[PAR] -->Send '[PAR].");
	PrintB(tBICPAR,"  Command = <''[PAR]>[//COMMENT]");
	PrintP(tBICPAR,"   eg:");
	PrintP(tBICPAR,"     Command = ''       //2Bytes,send '");
	PrintP(tBICPAR,"     Command = ''TEST   //4Bytes,send 'TEST");
	PrintP(tBICPAR,"     Command = '' TEST  //5Bytes,send ' TEST");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_SQ::Command(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	*ret = (Str_Trim(par));
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_HEX::Help(SBICPAR *tBICPAR,int32 blDetail)const{
	PrintB(tBICPAR,".CMD = HEX:[PAR] -->Send HEX format data [PAR].");
	PrintB(tBICPAR,"  Command = <'HEX:[PAR]>[//COMMENT]");
	PrintP(tBICPAR,"   eg:");
	PrintP(tBICPAR,"     Command = 'HEX:35 36 37      //send 0x35,0x36,0x37,3Bytes");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_HEX::Command(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	*ret = Str_HEXToHEXs(par);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_Time::Help(SBICPAR *tBICPAR,int32 blDetail)const{
	PrintB(tBICPAR,".CMD = Time/Now(<PAR>); -->Get current time.");
	PrintB(tBICPAR,"  Command = <'Time/Now=(<PAR>)>[//COMMENT]");
	PrintP(tBICPAR,"  Notes:1.PAR is style string, eg: YY/MM/DD,hh:mm:ss(zzz).");
	PrintP(tBICPAR,"        2.YY:Year, MM:Month, DD:Date, hh:Hour, mm:Minute, ss:Sencond, zzz:Millisecond.");
	PrintP(tBICPAR,"   eg:");
	PrintP(tBICPAR,"     If PC time is 08:29:25");
	PrintP(tBICPAR,"       Command = 'Build = \"Time is \" + 'Time(hh:mm:ss) //New \"Command\" is Time is 08:29:25");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_Time::Command(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	SYS_DateTime	eTime;
	if (par.length() == 0){
		eTime.Now();
		*ret = eTime.FormatDateTime(DEFAULT_TIMEFORMATE);
		
	}
	else{
		eTime.Now();
		*ret = eTime.FormatDateTime(Str_Trim(par));
		
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_RET::Help(SBICPAR *tBICPAR,int32 blDetail)const{
	PrintB(tBICPAR,".CMD = RET(<PAR>); -->Get CMD Search's result.");
	PrintB(tBICPAR,"  Command = <'RET(<PAR>)>");
	PrintP(tBICPAR,"     eg:");
	PrintP(tBICPAR,"     If last CMD is Search, and");
	PrintP(tBICPAR,"       'RET(1) = Leif Wen");
	PrintP(tBICPAR,"       'RET(2) = +86-138-6062-4143");
	PrintP(tBICPAR,"       Command = 'Build = \"Name: \" + 'RET(1) + \".MP: \" + 'RET(2) //New \"Command\" is Name: Leif Wen.MP: +86-138-6062-4143");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_RET::Command(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	std::string	strT,strSearchRet;
	int32		num;
	
	*ret = "";
	if (tBICPAR != nullptr){
		strSearchRet = tBICPAR->cgSearchRet;
		num = atoi(Str_Trim(par).c_str());
		strT = "";
		while(num > 0){
			strT = Str_ReadSubItem(&strSearchRet,",");
			-- num;
		}
		*ret = Str_HEXToASCII(strT);
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_STRING::Help(SBICPAR *tBICPAR,int32 blDetail)const{
	PrintB(tBICPAR,".CMD = string(len); -->Create string, len > 0 and len < 64 * 512.");
	PrintB(tBICPAR,"  Command = <'string(len)>");
	PrintP(tBICPAR,"     eg:");
	PrintP(tBICPAR,"       Command = 'Build = \"String is \" + 'string(10) //New \"Command\" is String is 001>?@ABCD\\r");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_STRING::Command(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	std::string	strT,strOut;
	int32		num,line;
	char		charT;
	
	*ret = "";
	num = atoi(Str_Trim(par).c_str());
	strOut = "";
	line = 0;
	if (num > 64 * 512)
		num = 64 * 512;
	while(num > 3){
		++ line;
		strT = Str_IntToString(line);
		if (strT.length() < 3)
			strT.insert(0,3 - strT.length(),'0');
		strOut += strT;
		num -= 3;
		charT = 62;
		while((charT < 92) && (num > 1)){
			strT = charT;
			strOut += strT;
			++ charT;
			-- num;
		}
		charT = 93;
		while((charT < 123) && (num > 1)){
			strT = charT;
			strOut += strT;
			++ charT;
			-- num;
		}
		if (num > 0){
			strOut += "\\r";
			-- num;
		}
	}
	if (num == 3)
		strOut += ">?\\r";
	if (num == 2)
		strOut += "?\\r";
	if (num == 1)
		strOut += "\\r";
	*ret = strOut;
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_Build::Help(SBICPAR *tBICPAR,int32 blDetail)const{
	PrintB(tBICPAR,".CMD = Build/Send=<Expression> -->Create new \"Command\".");
	PrintB(tBICPAR,"  Command = <'Build/Send=<Expression>>[//COMMENT]");
	PrintP(tBICPAR,"  Notes:1.Expression Operators is +.");
	PrintP(tBICPAR,"        2.Can use \"\" set string. Use \\\", \\+ to escape \", +.");
	PrintP(tBICPAR,"        3.Support \\0xhh, \\0Xhh, \\a, \\b, \\f, \\n, \\r, \\t, \\v, \\\\, \\', \\\", \\0, \\/, \\*, \\?.");
	PrintP(tBICPAR,"        4.support sub command :'','hex,'ret,'time,'string.");
	PrintP(tBICPAR,"     eg:");
	PrintP(tBICPAR,"       Command = 'Build = Happy + New + Year //new \"Command\" is HappyNewYear.");
	PrintP(tBICPAR,"     If last CMD is Search, and");
	PrintP(tBICPAR,"       'RET(1) = Leif Wen");
	PrintP(tBICPAR,"       'RET(2) = +86-138-6062-4143");
	PrintP(tBICPAR,"       Command = 'Build = \"Name: \" + 'RET(1) + \".MP: \" + 'RET(2) //New \"Command\" is Name: Leif Wen.MP: +86-138-6062-4143");
	
	const_cast<SBIC_Build *>(this)->HelpLC(tBICPAR,blDetail);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_Build::Command(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	std::string 	strTempData;
	
	*ret = "";
	Build(tBICPAR,par,ret);
	if ((tBICPAR != nullptr) && (tBICPAR->cgDevice != nullptr) && (tBICPAR->cgDevice->cEDevFlag.blEnablePrintSBICinfo != 0) && (tBICPAR->cgDevice->cEDevFlag.blCommandExplain != 0)){
		strTempData = SYS_MakeTimeNow();
		strTempData += " Execute: ";
		strTempData += "Build:: ";
		strTempData += par;
		strTempData += "\r\n";
		if (tBICPAR->cgDevice->cgODevList.cgOutput != nullptr){
			tBICPAR->cgDevice->cgODevList.cgOutput->Spin_InUse_set();
			tBICPAR->cgDevice->cgODevList.cgOutput->WriteDividingLine(RICH_CF_clMaroon);
			tBICPAR->cgDevice->cgODevList.cgOutput->WriteStrN(strTempData,RICH_CF_clMaroon);
			strTempData = "This new command will be send:\r\n";
			strTempData += *ret;
			strTempData += "\r\n";
			tBICPAR->cgDevice->cgODevList.cgOutput->WriteStr(strTempData,RICH_CF_clPurple);
			tBICPAR->cgDevice->cgODevList.cgOutput->Spin_InUse_clr();
		}
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_Build::Build(SBICPAR *tBICPAR,const std::string &strInput,std::string *retStr)const{
	std::string		finalResult,strOInput,strRet,strSub;
	int32			retCode;

	FormatString(strInput,&strOInput);
	finalResult = "";
	while(strOInput.length() > 0){
		strSub = SplitLetter(&strOInput);
		retCode = const_cast<SBIC_Build *>(this)->ExecuteLC(tBICPAR,strSub,&strRet);
		if (retCode == SBI_RETCODE_HEX){
			strRet = " " + Str_Trim(strRet);
			strRet = Str_Replace(strRet," ","\\0x");
			finalResult += strRet;
		}
		else if (retCode != SBI_RETCODE_NO){
			finalResult += strRet;
		}
		else{
			finalResult += strSub;
		}
	}
	*retStr = finalResult;
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
void SBIC_Build::FormatString(const std::string &strInput,std::string *returnStr){
	//formate to HEX without space
	//escape +,"
	std::string				strData,strResult;
	std::string::size_type	i,length;
	uint8					charData;
	
	length = strInput.length();
	i = 0;
	*returnStr = "";
	strData = "";
	
	while(i < length){
		charData = strInput[i];
		++ i;
		if (charData == '+'){
			*returnStr += Str_ASCIIToHEX(Str_Trim(strData),G_ESCAPE_OFF);
			*returnStr += charData;
			strData = "";
		}
		else{
			if ((charData == '\\') && (i < length)){	//   (\+)
				charData = strInput[i];
				++ i;
				if (charData != '+')
					strData += "\\";
			}
			strData += charData;
		}
	}
	*returnStr += Str_ASCIIToHEX(Str_Trim(strData),G_ESCAPE_OFF);
}
//------------------------------------------------------------------------------------------//
std::string SBIC_Build::SplitLetter(std::string *strInput){
	// remove " in front and back.
	std::string		strResult;
	
	strResult = Str_HEXToASCII(Str_ReadSubItem(strInput, "+"));
	
	if (strResult.length() > 1){
		if ((strResult[0] == '"') && (strResult[strResult.length() - 1] == '"'))
			strResult = strResult.substr(1,strResult.length() - 2);
	}
	return(strResult);
}
//------------------------------------------------------------------------------------------//
