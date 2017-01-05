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
#include "Comm_File.h"
#include "Comm_Crypto.h"
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
	PrintP(tBICPAR,"     Command = 'HEX:35 36 37      //Send: 0x35,0x36,0x37,3Bytes");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_HEX::Command(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	SBIC_Print	tSBIC_Print;
	std::string	retHexCommand,retPrintData;
	tSBIC_Print.CreateHexCommand(tBICPAR, par, 0, 0, G_ESCAPE_ON, &retHexCommand,&retPrintData);
	*ret = Str_HEXToHEXs(Str_HEXToASCII(retHexCommand));
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_Time::Help(SBICPAR *tBICPAR,int32 blDetail)const{
	PrintB(tBICPAR,".CMD = Time/Now(<PAR>) -->Get current time.");
	PrintB(tBICPAR,"  Command = <'Time/Now=(<PAR>)>[//COMMENT]");
	PrintP(tBICPAR,"  Notes:1.PAR is style string, eg: YY/MM/DD,hh:mm:ss(zzz).");
	PrintP(tBICPAR,"        2.YY:Year, MM:Month, DD:Date, hh:Hour, mm:Minute, ss:Sencond, zzz:Millisecond.");
	PrintP(tBICPAR,"   eg:");
	PrintP(tBICPAR,"     If PC time is 08:29:25");
	PrintP(tBICPAR,"       Command = 'Time(hh:mm:ss) //Send: 08:29:25");
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
	PrintB(tBICPAR,".CMD = RET(<PAR>) -->Get CMD Search's result.");
	PrintB(tBICPAR,"  Command = <'RET(<PAR>)>");
	PrintP(tBICPAR,"     eg:");
	PrintP(tBICPAR,"     If last CMD is Search, and");
	PrintP(tBICPAR,"       'RET(1) = Leif Wen");
	PrintP(tBICPAR,"       'RET(2) = +86-138-6062-4143");
	PrintP(tBICPAR,"       Command = 'Combine = \"Name: \" + 'RET(1) + \".MP: \" + 'RET(2) //New \"Command\" is Name: Leif Wen.MP: +86-138-6062-4143");
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
	PrintB(tBICPAR,".CMD = string(len) -->Create string, len > 0 and len < 64 * 512.");
	PrintB(tBICPAR,"  Command = <'string(len)>");
	PrintP(tBICPAR,"     eg:");
	PrintP(tBICPAR,"       Command = 'string(10) //Send: 001>?@ABCD\\r");
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
#if (defined USE_OPENSSL) || (defined CommonDefH_MAC)
//------------------------------------------------------------------------------------------//
int32 SBIC_CALC_MD5::Help(SBICPAR *tBICPAR,int32 blDetail)const{
	PrintB(tBICPAR,".CMD = MD5(<Expression|FileName>) -->Calculate MD5.");
	PrintB(tBICPAR,"  Command = <'MD5(<Expression|FileName>)>[//COMMENT]");
	PrintP(tBICPAR,"   eg:");
	PrintP(tBICPAR,"     Command = 'MD5(leif.wen)       //Reslut: 4A59D526EA4076DF64FF45AF8FF82621");
	PrintP(tBICPAR,"     Command = 'MD5(./test.txt)     //Calculate file ./test.txt");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_CALC_MD5::Command(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	SBIC_Print	tSBIC_Print;
	std::string	retHexCommand,retPrintData;
	std::string	strPar;
	*ret = "";
	strPar = par;
	strPar = Str_SplitSubItem(&strPar, ' ');
	if (par.length() > 0){
		if (CFS_CheckFile(strPar) > 0){
			*ret = Str_ASCIIToHEX(CCY_DigestFile_MD5(strPar),G_ESCAPE_OFF);
		}
		else{
			tSBIC_Print.CreateHexCommand(tBICPAR, par, 0, 0, G_ESCAPE_ON, &retHexCommand,&retPrintData);
			*ret = Str_ASCIIToHEX(CCY_Digest_MD5(Str_HEXToASCII(retHexCommand)),G_ESCAPE_OFF);
		}
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_CALC_SHA1::Help(SBICPAR *tBICPAR,int32 blDetail)const{
	PrintB(tBICPAR,".CMD = SHA1(<Expression|FileName>) -->Calculate SHA1.");
	PrintB(tBICPAR,"  Command = <'SHA1(<Expression|FileName>)>[//COMMENT]");
	PrintP(tBICPAR,"   eg:");
	PrintP(tBICPAR,"     Command = 'SHA1(leif.wen)       //Reslut: BD5EC1DE30861E6C1BF5FC7995C6CBAFAABF78E4");
	PrintP(tBICPAR,"     Command = 'SHA1(./test.txt)     //Calculate file ./test.txt");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_CALC_SHA1::Command(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	SBIC_Print	tSBIC_Print;
	std::string	retHexCommand,retPrintData;
	std::string	strPar;
	*ret = "";
	strPar = par;
	strPar = Str_SplitSubItem(&strPar, ' ');
	if (par.length() > 0){
		if (CFS_CheckFile(strPar) > 0){
			*ret = Str_ASCIIToHEX(CCY_DigestFile_SHA1(strPar),G_ESCAPE_OFF);
		}
		else{
			tSBIC_Print.CreateHexCommand(tBICPAR, par, 0, 0, G_ESCAPE_ON, &retHexCommand,&retPrintData);
			*ret = Str_ASCIIToHEX(CCY_Digest_SHA1(Str_HEXToASCII(retHexCommand)),G_ESCAPE_OFF);
		}
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_CALC_SHA224::Help(SBICPAR *tBICPAR,int32 blDetail)const{
	PrintB(tBICPAR,".CMD = SHA224(<Expression|FileName>) -->Calculate SHA224.");
	PrintB(tBICPAR,"  Command = <'SHA224(<Expression|FileName>)>[//COMMENT]");
	PrintP(tBICPAR,"   eg:");
	PrintP(tBICPAR,"     Command = 'SHA224(leif.wen)       //Reslut: E97B789F4B626EDB61547D27FFC6E678E363F33C1F7295D76967E4F7");
	PrintP(tBICPAR,"     Command = 'SHA224(./test.txt)     //Calculate file ./test.txt");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_CALC_SHA224::Command(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	SBIC_Print	tSBIC_Print;
	std::string	retHexCommand,retPrintData;
	std::string	strPar;
	*ret = "";
	strPar = par;
	strPar = Str_SplitSubItem(&strPar, ' ');
	if (par.length() > 0){
		if (CFS_CheckFile(strPar) > 0){
			*ret = Str_ASCIIToHEX(CCY_DigestFile_SHA224(strPar),G_ESCAPE_OFF);
		}
		else{
			tSBIC_Print.CreateHexCommand(tBICPAR, par, 0, 0, G_ESCAPE_ON, &retHexCommand,&retPrintData);
			*ret = Str_ASCIIToHEX(CCY_Digest_SHA224(Str_HEXToASCII(retHexCommand)),G_ESCAPE_OFF);
		}
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_CALC_SHA256::Help(SBICPAR *tBICPAR,int32 blDetail)const{
	PrintB(tBICPAR,".CMD = SHA256(<Expression|FileName>) -->Calculate SHA256.");
	PrintB(tBICPAR,"  Command = <'SHA256(<Expression|FileName>)>[//COMMENT]");
	PrintP(tBICPAR,"   eg:");
	PrintP(tBICPAR,"     Command = 'SHA256(leif.wen)       //Reslut: A33FD3E2A864E17F8BB87D23B774954D3194BED1C376BDD17B5965234759C780");
	PrintP(tBICPAR,"     Command = 'SHA256(./test.txt)     //Calculate file ./test.txt");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_CALC_SHA256::Command(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	SBIC_Print	tSBIC_Print;
	std::string	retHexCommand,retPrintData;
	std::string	strPar;
	*ret = "";
	strPar = par;
	strPar = Str_SplitSubItem(&strPar, ' ');
	if (par.length() > 0){
		if (CFS_CheckFile(strPar) > 0){
			*ret = Str_ASCIIToHEX(CCY_DigestFile_SHA256(strPar),G_ESCAPE_OFF);
		}
		else{
			tSBIC_Print.CreateHexCommand(tBICPAR, par, 0, 0, G_ESCAPE_ON, &retHexCommand,&retPrintData);
			*ret = Str_ASCIIToHEX(CCY_Digest_SHA256(Str_HEXToASCII(retHexCommand)),G_ESCAPE_OFF);
		}
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_CALC_SHA384::Help(SBICPAR *tBICPAR,int32 blDetail)const{
	PrintB(tBICPAR,".CMD = SHA384(<Expression|FileName>) -->Calculate SHA384.");
	PrintB(tBICPAR,"  Command = <'SHA384(<Expression|FileName>)>[//COMMENT]");
	PrintP(tBICPAR,"   eg:");
	PrintP(tBICPAR,"     Command = 'SHA384(leif.wen)       //Reslut: 4DB8C402C13CC2F064ED6C619164CA79B4758572D594B35C3FE32BCD439C02B318A17D365DE50A01C75EF9D72E92CCD5");
	PrintP(tBICPAR,"     Command = 'SHA384(./test.txt)     //Calculate file ./test.txt");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_CALC_SHA384::Command(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	SBIC_Print	tSBIC_Print;
	std::string	retHexCommand,retPrintData;
	std::string	strPar;
	*ret = "";
	strPar = par;
	strPar = Str_SplitSubItem(&strPar, ' ');
	if (par.length() > 0){
		if (CFS_CheckFile(strPar) > 0){
			*ret = Str_ASCIIToHEX(CCY_DigestFile_SHA384(strPar),G_ESCAPE_OFF);
		}
		else{
			tSBIC_Print.CreateHexCommand(tBICPAR, par, 0, 0, G_ESCAPE_ON, &retHexCommand,&retPrintData);
			*ret = Str_ASCIIToHEX(CCY_Digest_SHA384(Str_HEXToASCII(retHexCommand)),G_ESCAPE_OFF);
		}
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_CALC_SHA512::Help(SBICPAR *tBICPAR,int32 blDetail)const{
	PrintB(tBICPAR,".CMD = SHA512(<Expression|FileName>) -->Calculate SHA512.");
	PrintB(tBICPAR,"  Command = <'SHA512(<Expression|FileName>)>[//COMMENT]");
	PrintP(tBICPAR,"   eg:");
	PrintP(tBICPAR,"     Command = 'SHA512(leif.wen)       //Reslut: 33011AB95EE31DB208748AE7AB3F47FD1E6B51CC69FF855D8670066C259A55B5BA3F965E035F61A68A331BCE1CCD1317C857C0CBC3BBFA129D61E3A53A294392");
	PrintP(tBICPAR,"     Command = 'SHA512(./test.txt)     //Calculate file ./test.txt");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_CALC_SHA512::Command(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	SBIC_Print	tSBIC_Print;
	std::string	retHexCommand,retPrintData;
	std::string	strPar;
	*ret = "";
	strPar = par;
	strPar = Str_SplitSubItem(&strPar, ' ');
	if (par.length() > 0){
		if (CFS_CheckFile(strPar) > 0){
			*ret = Str_ASCIIToHEX(CCY_DigestFile_SHA512(strPar),G_ESCAPE_OFF);
		}
		else{
			tSBIC_Print.CreateHexCommand(tBICPAR, par, 0, 0, G_ESCAPE_ON, &retHexCommand,&retPrintData);
			*ret = Str_ASCIIToHEX(CCY_Digest_SHA512(Str_HEXToASCII(retHexCommand)),G_ESCAPE_OFF);
		}
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
#endif
//------------------------------------------------------------------------------------------//
int32 SBIC_CALC_BASE64::Help(SBICPAR *tBICPAR,int32 blDetail)const{
	PrintB(tBICPAR,".CMD = BASE64(<E|D>,<Expression|FileName>) -->Calculate BASE64.");
	PrintB(tBICPAR,"  Command = <'BASE64(<E|D>,<Expression|FileName>)>[//COMMENT]");
	PrintP(tBICPAR,"   eg:");
	PrintP(tBICPAR,"     Command = 'BASE64(E,leif.wen)          //Encode Reslut: bGVpZi53ZW4=");
	PrintP(tBICPAR,"     Command = 'BASE64(D,bGVpZi53ZW4=)      //Decode Reslut: leif.wen");
	PrintP(tBICPAR,"     Command = 'BASE64(E,./test.txt)        //Encode file ./test.txt");
	PrintP(tBICPAR,"     Command = 'BASE64(D,./test.txt)        //Decode file ./test.txt");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_CALC_BASE64::Command(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	SBIC_Print	tSBIC_Print;
	std::string	retHexCommand,retPrintData;
	*ret = "";
	std::string		par1,par2,parR;
	par2 = par;
	par1 = Str_ReadSubItem(&par2, ",");
	if ((par1 == "E") || (par1 == "D") || (par1 == "e") || (par1 == "d")){
		par2 = Str_LTrim(par2);
		if (par2.length() > 0){
			parR = par2;
			par2 = Str_Trim(Str_SplitSubItem(&par2, ' '));
			
			if (CFS_CheckFile(par2) > 0){
				if (par1 == "E" || par1 == "e"){
					CCY_Encode_FileToStr_BASE64(ret, par2);
				}
				else{
					CCY_Decode_FileToStr_BASE64(ret, par2);
				}
			}
			else{
				tSBIC_Print.CreateHexCommand(tBICPAR, parR, 0, 0, G_ESCAPE_ON, &retHexCommand,&retPrintData);
				if (par1 == "E" || par1 == "e"){
					CCY_Encode_BASE64(ret, Str_HEXToASCII(retHexCommand));
				}
				else{
					CCY_Decode_BASE64(ret, Str_HEXToASCII(retHexCommand));
				}
			}
		}
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//





























//------------------------------------------------------------------------------------------//
int32 SBIC_Combine::Help(SBICPAR *tBICPAR,int32 blDetail)const{
	PrintB(tBICPAR,".CMD = combine=<Expression> -->Create new \"Command\".");
	PrintB(tBICPAR,"  Command = <'Combine=<Expression>>[//COMMENT]");
	PrintP(tBICPAR,"  Notes:1.Expression Operators is +.");
	PrintP(tBICPAR,"        2.Can use \"\" set string. Use \\\", \\+ to escape \", +.");
	PrintP(tBICPAR,"        3.Support \\0xhh, \\0Xhh, \\a, \\b, \\f, \\n, \\r, \\t, \\v, \\\\, \\', \\\", \\0, \\/, \\*, \\?.");
	PrintP(tBICPAR,"        4.support sub command :'','hex,'ret,'time,'string.");
	PrintP(tBICPAR,"     eg:");
	PrintP(tBICPAR,"       Command = 'Combine = Happy + New + Year //new \"Command\" is HappyNewYear.");
	PrintP(tBICPAR,"     If last CMD is Search, and");
	PrintP(tBICPAR,"       'RET(1) = Leif Wen");
	PrintP(tBICPAR,"       'RET(2) = +86-138-6062-4143");
	PrintP(tBICPAR,"       Command = 'Combine = \"Name: \" + 'RET(1) + \".MP: \" + 'RET(2) //New \"Command\" is Name: Leif Wen.MP: +86-138-6062-4143");
	
	const_cast<SBIC_Combine *>(this)->HelpLC(tBICPAR,blDetail);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_Combine::Command(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	std::string 	strTempData;
	
	*ret = "";
	Create(tBICPAR,par,ret);
	if ((tBICPAR != nullptr) && (tBICPAR->cgDevice != nullptr) && (tBICPAR->cgDevice->cEDevFlag.blEnablePrintSBICinfo != 0) && (tBICPAR->cgDevice->cEDevFlag.blCommandExplain != 0)){
		strTempData = SYS_MakeTimeNow();
		strTempData += " Execute: ";
		strTempData += "Combine:: ";
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
int32 SBIC_Combine::Create(SBICPAR *tBICPAR,const std::string &strInput,std::string *retStr)const{
	std::string		finalResult,strOInput,strRet,strSub;
	int32			retCode;

	FormatString(strInput,&strOInput);
	finalResult = "";
	while(strOInput.length() > 0){
		strSub = SplitLetter(&strOInput);
		retCode = const_cast<SBIC_Combine *>(this)->ExecuteLC(tBICPAR,strSub,&strRet);
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
void SBIC_Combine::FormatString(const std::string &strInput,std::string *returnStr){
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
std::string SBIC_Combine::SplitLetter(std::string *strInput){
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











//------------------------------------------------------------------------------------------//
int32 SBIC_Print::Help(SBICPAR *tBICPAR,int32 blDetail)const{
	PrintB(tBICPAR,".CMD = Print=<Expression> -->Print log.");
	PrintB(tBICPAR,"  Command = <'Print=<Expression>>[//COMMENT]");
	PrintP(tBICPAR,"  Notes:1.Expression Operators is +.");
	PrintP(tBICPAR,"        2.Can use \"\" set string. Use \\\", \\+ to escape \", +.");
	PrintP(tBICPAR,"        3.Support \\0xhh, \\0Xhh, \\a, \\b, \\f, \\n, \\r, \\t, \\v, \\\\, \\', \\\", \\0, \\/, \\*, \\?.");
	PrintP(tBICPAR,"        4.support sub command :same as 'combine.");
	PrintP(tBICPAR,"     eg:");
	PrintP(tBICPAR,"       Command = 'Print = Happy + New + Year //Print HappyNewYear.");
	
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_Print::Command(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	std::string 	strTempData,retHexCommand,retPrintData;
	
	*ret = "";
	CreateHexCommand(tBICPAR,par,0,0,G_ESCAPE_ON,&retHexCommand,&retPrintData);
	if ((tBICPAR != nullptr) && (tBICPAR->cgDevice != nullptr)
		&& (tBICPAR->cgDevice->cEDevFlag.blEnablePrintSBICinfo != 0)
		&& (tBICPAR->cgDevice->cEDevFlag.blCommandExplain != 0)){
		PrintExecute(tBICPAR,retPrintData);
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_Print::CreateHexCommand(SBICPAR *cSBICPAR,const std::string &inputCommand,
								   int32 blHEX,int32 bl0x0D,G_ESCAPE_VAILD blEscape,
								   std::string *retHexCommand,std::string *retPrintData)const{
	int32		eRetCode;
	std::string	strHexCommand,strPrintData,strOCommand;
	
	strOCommand = SBIC_Node::DelComment(inputCommand);
	eRetCode = ExecuteLC(cSBICPAR,strOCommand,&strPrintData);
	switch (eRetCode){
		case SBI_RETCODE_HEX:
			strHexCommand = strPrintData;
			if (bl0x0D != 0){
				strHexCommand += "0D";
				strPrintData += "0D";
			}
			break;
		case SBI_RETCODE_SQ:
		case SBI_RETCODE_TIME:
		case SBI_RETCODE_STRING:
		case SBI_RETCODE_CALC_MD5:
		case SBI_RETCODE_CALC_SHA1:
		case SBI_RETCODE_CALC_SHA224:
		case SBI_RETCODE_CALC_SHA256:
		case SBI_RETCODE_CALC_SHA384:
		case SBI_RETCODE_CALC_SHA512:
		case SBI_RETCODE_CALC_BASE64:
		case SBI_RETCODE_COMBINE:
			strHexCommand = Str_ASCIIToHEX(strPrintData,blEscape);
			if (bl0x0D != 0){
				strHexCommand += "0D";
				strPrintData += "\\r";
			}
			break;
		default:{
			if (blHEX != 0){
				strHexCommand = Str_HEXToHEXs(strOCommand);
				if (bl0x0D != 0)
					strHexCommand += "0D";
				strPrintData = strHexCommand;
			}
			else{
				strPrintData = strOCommand;
				strHexCommand = Str_ASCIIToHEX(strOCommand,blEscape);
				if (bl0x0D != 0){
					strHexCommand += "0D";
					strPrintData += "\\r";
				}
			}
			break;
		}
	}
	
	*retHexCommand = strHexCommand;
	*retPrintData = strPrintData + "\r\n";
	return(eRetCode);
}
//------------------------------------------------------------------------------------------//
