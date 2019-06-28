//
//  BIF_Transform.cpp
//  SDT
//
//  Created by Leif Wen on 09/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "BIF_Transform.h"
//------------------------------------------------------------------------------------------//
#ifdef BIF_Transform_h
#include "SYS_File.h"
#include "SYS_Time.h"
#include "ALG_Math.h"
//------------------------------------------------------------------------------------------//
CMDID BIF_COMBINE_BASE::Execute(CMD_ENV* env,const uint32& mID,const STDSTR& msg,void* bif_retstr)const{
	//if mID == CMD_ID_NO, means msg is rawIn
	if (mID == (uint32)cgCommandID){
		if (bif_retstr == nullptr)
			return(mID);
		return(Command(env,msg,bif_retstr));
	}
	if ((CheckSFlag(CMD_blTrySubCMD)) && (msg.length() > 0))
		return(TraversalChildExecute(env,mID,msg,bif_retstr));
	return(CMD_ID_NO);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
CMDID BIF_SQ::Help(CMD_ENV* env,uint32 flag)const{
	PrintB(env,".CMD = '[PAR] -->Send '[PAR].");
	PrintB(env,"  Command = <''[PAR]>[//COMMENT]");
	PrintP(env,"   eg:");
	PrintP(env,"     Command = ''       //2Bytes,send '");
	PrintP(env,"     Command = ''TEST   //4Bytes,send 'TEST");
	PrintP(env,"     Command = '' TEST  //5Bytes,send ' TEST");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIF_SQ::ResolveID(STDSTR** retMsg,const STDSTR& rawIn)const{
	STDSTR	oFullCMD;
	
	oFullCMD = Str_LTrim(rawIn);
	
	if (oFullCMD.length() > 1){
		if (oFullCMD.substr(0,2) == "''"){
			**retMsg = oFullCMD.substr(1);
			return(cgCommandID);
		}
	}
	**retMsg = "";
	return(BIF_ID_NO);
};
//------------------------------------------------------------------------------------------//
CMDID BIF_SQ::Command(CMD_ENV* env,const STDSTR& msg,void* bif_retstr)const{
	BIF_RETSTR *ret = static_cast<BIF_RETSTR*>(bif_retstr);
	ret->result = Str_Trim(msg);
	ret->forPrint += ret->result;
	ret->forSend += ret->result;
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIF_HEX::Help(CMD_ENV* env,uint32 flag)const{
	PrintB(env,".CMD = HEX:[PAR] -->Send HEX format data.");
	PrintB(env,"  Command = <'HEX:[PAR]>[//COMMENT]");
	PrintP(env,"   eg:");
	PrintP(env,"     Command = 'HEX:35 36 37      //Send: 0x35,0x36,0x37,3Bytes");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIF_HEX::Command(CMD_ENV* env,const STDSTR& msg,void* bif_retstr)const{
	BIF_RETSTR *ret = static_cast<BIF_RETSTR*>(bif_retstr);
	ret->result = Str_HEXToASCII(msg);
	ret->forPrint += ("[HEX:" + Str_RTrim(Str_ASCIIToHEXs(ret->result, G_ESCAPE_OFF)) +"]");
	ret->forSend += ret->result;
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIF_VIEWINHEX::Help(CMD_ENV* env,uint32 flag)const{
	PrintB(env,".CMD = Viewhex:[PAR] -->View send data in HEX format.");
	PrintB(env,"  Command = <'Viewhex:[PAR]>[//COMMENT]");
	PrintP(env,"   eg:");
	PrintP(env,"     Command = 'Viewhex:ABCD      //Send: ABCD,4Bytes,but show [HEX:41 42 43 44]");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIF_VIEWINHEX::Command(CMD_ENV* env,const STDSTR& msg,void* bif_retstr)const{
	BIF_RETSTR *ret = static_cast<BIF_RETSTR*>(bif_retstr);

	if (msg.length() > 0){
		BIF_Transform(nullptr, _EMPTY(&ret->result), nullptr,msg,CMD_NONE,G_ESCAPE_ON);
		ret->forPrint += ("[HEX:" + Str_RTrim(Str_ASCIIToHEXs(ret->result, G_ESCAPE_OFF)) +"]");
		ret->forSend += ret->result;
	}	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
#ifndef DEFAULT_TIMEFORMATE
	#define	DEFAULT_TIMEFORMATE	"[hh:mm:ss.zzz]"
#endif
//------------------------------------------------------------------------------------------//
CMDID BIF_TIME::Help(CMD_ENV* env,uint32 flag)const{
	PrintB(env,".CMD = Time/Now(<PAR>) -->Get current time.");
	PrintB(env,"  Command = <'Time/Now(<PAR>)>[//COMMENT]");
	PrintP(env,"  Notes:1.PAR is style string, eg: YY/MM/DD,hh:mm:ss(zzz).");
	PrintP(env,"        2.YY:Year, MM:Month, DD:Date, hh:Hour, mm:Minute, ss:Sencond, zzz:Millisecond.");
	PrintP(env,"   eg:");
	PrintP(env,"     If PC time is 08:29:25");
	PrintP(env,"       Command = 'Time(hh:mm:ss) //Send: 08:29:25");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIF_TIME::Command(CMD_ENV* env,const STDSTR& msg,void* bif_retstr)const{
	BIF_RETSTR *ret = static_cast<BIF_RETSTR*>(bif_retstr);
	DTIME	eTime;
	
	if (msg.length() == 0){
		eTime.Now();
		ret->result = eTime.Format(DEFAULT_TIMEFORMATE);
	}
	else{
		eTime.Now();
		ret->result = eTime.Format(Str_Trim(msg));
	}
	
	ret->forPrint += ret->result;
	Str_CharToStr(&ret->forSend, (uint8*)ret->result.c_str(), ret->result.length(), G_ASCII, G_SPACE_OFF, G_ESCAPE_ON);
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIF_STRING::Help(CMD_ENV* env,uint32 flag)const{
	PrintB(env,".CMD = Str/String(<len>) -->Create string, support four arithmetic operation.");
	PrintB(env,"  Command = <'Str/String(len)>");
	PrintP(env,"     eg:");
	PrintP(env,"       Command = 'String(5+5) //Send: 001?@ABCDE\\r");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIF_STRING::Command(CMD_ENV* env,const STDSTR& msg,void* bif_retstr)const{
	BIF_RETSTR *ret = static_cast<BIF_RETSTR*>(bif_retstr);
	STDSTR		strT;
	int32		num,line;
	char		charT;
	
	num = ALG_FAO32Calc(nullptr,msg);
	ret->forPrint += "\n";
	ret->result = "";
	line = 0;
	//if (num > 64 * 512)
	//	num = 64 * 512;
	while(num > 4){
		++ line;
		strT = Str_ToStr(line);
		if (strT.length() < 4)
			strT.insert(0,4 - strT.length(),'0');
		ret->forPrint += strT;
		ret->result += strT;
		num -= 4;
		charT = 63;
		while((charT < 92) && (num > 1)){
			strT = charT;
			ret->forPrint += strT;
			ret->result += strT;
			++ charT;
			-- num;
		}
		charT = 93;
		while((charT < 123) && (num > 1)){
			strT = charT;
			ret->forPrint += strT;
			ret->result += strT;
			++ charT;
			-- num;
		}
		if (num > 0){
			ret->forPrint += "\\n\n";
			ret->result += "\n";
			-- num;
		}
	}
	if (num == 4){
		ret->forPrint += "?@A\\n\n";
		ret->result += "?@A\n";
	}
	else if (num == 3){
		ret->forPrint += "?@\\n\n";
		ret->result += "?@\n";
	}
	else if (num == 2){
		ret->forPrint += "?\\n\n";
		ret->result += "?\n";
	}
	else if (num == 1){
		ret->forPrint += "\\n\n";
		ret->result += "\n";
	}
	ret->forSend += ret->result;
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIF_FILE::Help(CMD_ENV* env,uint32 flag)const{
	PrintB(env,".CMD = File(<FileName>) -->Read file.");
	PrintB(env,"  Command = <'File(<FileName>)>[//COMMENT]");
	PrintP(env,"   eg:");
	PrintP(env,"     Command = 'File(./test.txt)        //Read file ./test.txt");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIF_FILE::Command(CMD_ENV* env,const STDSTR& msg,void* bif_retstr)const{
	BIF_RETSTR *ret = static_cast<BIF_RETSTR*>(bif_retstr);

	ret->result = msg;
	ret->result = Str_SplitSubItem(&ret->result, ' ');
	if (msg.length() > 0){
#ifdef DS_Transform_h
		if (CFS_CheckFile(ret->result) > 0)
			DS_IO_NODE::GetDSIOList().Transform(nullptr, &ret->forSend, IUD_FILE(ret->result));
#endif
		ret->forPrint += "[File Size:";
		ret->forPrint += Str_ToStr(CFS_CheckFileSize(ret->result));
		ret->forPrint += ",Name:";
		ret->forPrint += ret->result;
		ret->forPrint += "]";
	}
	ret->result = "";
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIF_DEFINE::Help(CMD_ENV* env,uint32 flag)const{
	PrintB(env,".CMD = Def/Define:<PAR> -->Set a define string. No execution command, used with CMD Replace.");
	PrintB(env,"  Command = <'Def/Define:<PAR>>[//COMMENT]");
	PrintP(env,"   eg:");
	PrintP(env,"     Command = 'Define:v1=Leif Wen;v2=+86-180-8888-0966;      //used with CMD Replace");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIF_DEFINE::Command(CMD_ENV* env,const STDSTR& msg,void* bif_retstr)const{
	BIF_RETSTR *ret = static_cast<BIF_RETSTR*>(bif_retstr);
	if (env != nullptr){
		BIF_ENV::RetDefine(env) += msg;
		BIF_ENV::RetDefine(env) += ";";
	}
	ret->result = "";
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIF_REPLACE::Help(CMD_ENV* env,uint32 flag)const{
	PrintB(env,".CMD = Rep/Replace:<PAR> -->Replace PAR which define in 'define command.");
	PrintB(env,"  Command = <'Rep/Replace:<PAR>>");
	PrintP(env,"     eg:");
	PrintP(env,"     If 'define: v1=Leif Wen;v2=+86-180-8888-0966;");
	PrintP(env,"       Command = 'Rep:Name: v1.MP: v2 //New \"Command\" is Name: Leif Wen.MP: +86-180-8888-0966");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIF_REPLACE::Command(CMD_ENV* env,const STDSTR& msg,void* bif_retstr)const{
	BIF_RETSTR *ret = static_cast<BIF_RETSTR*>(bif_retstr);
	STDSTR	strDefine,strVar,strValue,strT;
	
	if (env != nullptr){
		ret->result = msg;
		strDefine = BIF_ENV::RetDefine(env);
		while(strDefine.length() > 0){
			strT = Str_ReadSubItem(&strDefine,";");
			if (strT.length() > 0){
				SplitPar1(strVar,strValue,strT,"=");
				Str_TrimSelf(strVar);
				Str_TrimSelf(strValue);
				ret->result = Str_Replace(ret->result, strVar, strValue);
			}
		}
		ret->forPrint += ret->result;
		ret->forSend += ret->result;
	}
	else{
		ret->forPrint += msg;
		ret->forSend += msg;
	}
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIF_SEARCH::Help(CMD_ENV* env,uint32 flag)const{
	PrintB(env,".CMD = Search=<Expression> -->Search expression in received data, and save the result.");
	PrintB(env,"  Command = <'Search=<Expression>>[//COMMENT]");
	PrintP(env,"  Notes: Expression's operators is * and ?, and only support ASCII format.");
	PrintP(env,"     1.* means any character, ? means one character, support multiple * and ? combination.");
	PrintP(env,"     2.The results returned are the data replaced by * and ?.");
	PrintP(env,"     3.Only record the first match of the results.");
	PrintP(env,"     4.Use 'RET (x) to get the results.");
	PrintP(env,"     5.support \\0xhh, \\0Xhh, \\a, \\b, \\f, \\n, \\r, \\t, \\v, \\\\, \\', \\\", \\0, \\/, \\*, \\?.");
	PrintP(env,"   eg:");
	PrintP(env,"    Received data:HappyNewYear");
	PrintP(env,"      Command = 'Search=Happy*Year     //Search Happy*Year, and record New as the result");
	PrintP(env,"      'RET(1)=New");
	PrintP(env,"    Received data:Name: Leif Wen.MP: +86-180-8888-0966.");
	PrintP(env,"      Command = 'Search=Name: *.MP: *. //Search Name: *.MP: *., and record Leif Wen and +86-180-8888-0966 as result");
	PrintP(env,"      'RET(1)=Leif Wen");
	PrintP(env,"      'RET(2)=+86-180-8888-0966");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIF_SEARCH::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	STDSTR	strTempData,strSearchRet;
	
	if (env != nullptr){
		BIF_ENV::RetFun(env) = 'F';
		if (Str_Find(&BIF_ENV::RetSearch(env),BIF_ENV::STDIN(env),msg)){
			BIF_ENV::RetFun(env) = 'T';
		}
#ifdef GList_h
		else if (BIF_ENV::GetCommandNode(env) != nullptr){
			BIF_ENV::GetCommandNode(env)->StrCatch = "Fail";
			++ BIF_ENV::GetCommandNode(env)->catchTimes;
		}
#endif
#ifdef Output_h
		if (CheckEnvFlag(env,BIF_ENV::ENV_blPrintBIFInfo) && CheckEnvFlag(env,BIF_ENV::ENV_blPrintBIFExplain) && (GetSTDOUT(env) != nullptr)){
			int32 i = 0;
			strSearchRet = BIF_ENV::RetSearch(env);
			*GetSTDOUT(env) << Begin() << NL()
			<< COL_DivLineTime << COL_NormalMessage << "Execute: Search::" << msg << "\n"
			<< COL_clRed;
			while(strSearchRet.length() > 0){
				*GetSTDOUT(env) << " RET(" << Str_ToStr(++i) << ") =" << Str_HEXToASCII(Str_ReadSubItem(&strSearchRet,",")) << "\n";
			};
			*GetSTDOUT(env) << Endl();
		}
#endif
	}
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIF_RET::Help(CMD_ENV* env,uint32 flag)const{
	PrintB(env,".CMD = RET(<PAR>) -->Get CMD Search's result.");
	PrintB(env,"  Command = <'RET(<PAR>)>");
	PrintP(env,"     eg:");
	PrintP(env,"     If last CMD is Search, and");
	PrintP(env,"       'RET(1) = Leif Wen");
	PrintP(env,"       'RET(2) = +86-180-8888-0966");
	PrintP(env,"       Command = 'Combine = \"Name: \" + 'RET(1) + \".MP: \" + 'RET(2)");
	PrintP(env,"                          //New \"Command\" is Name: Leif Wen.MP: +86-180-8888-0966");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIF_RET::Command(CMD_ENV* env,const STDSTR& msg,void* bif_retstr)const{
	BIF_RETSTR *ret = static_cast<BIF_RETSTR*>(bif_retstr);
	STDSTR	strT,retSearch;
	int32	num;
	
	ret->result = "";
	if (env != nullptr){
		retSearch = BIF_ENV::RetSearch(env);
		num = atoi(Str_Trim(msg).c_str());
		strT = "";
		while(num > 0){
			strT = Str_ReadSubItem(&retSearch,",");
			-- num;
		}
		ret->result = Str_HEXToASCII(strT);
		ret->forPrint += ret->result;
		ret->forSend += ret->result;
	}
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#define BIF_CALC_T(mode,text) \
CMDID BIF_CALC_##mode::Command(CMD_ENV* env,const STDSTR& msg,void* bif_retstr)const{\
	BIF_RETSTR *ret = static_cast<BIF_RETSTR*>(bif_retstr);\
	\
	ret->result = msg;\
	ret->result = Str_SplitSubItem(&ret->result, ' ');\
	if (msg.length() > 0){\
		if (CFS_CheckFile(ret->result) > 0){\
			ret->result = ALG_Digest_##mode(IUD_FILE(ret->result));\
		}\
		else{\
			BIF_Transform(nullptr, _EMPTY(&ret->result),nullptr,msg,CMD_NONE,G_ESCAPE_ON);\
			ret->result = ALG_Digest_##mode(ret->result);\
		}\
		ret->forPrint += "[";\
		ret->forPrint += text;\
		ret->forPrint += ":";\
		ret->forPrint += Str_RTrim(Str_ASCIIToHEXs(ret->result, G_ESCAPE_OFF));\
		ret->forPrint += "]";\
		ret->forSend += ret->result;\
	}\
	return(cgCommandID);\
};
//------------------------------------------------------------------------------------------//
#ifdef ALG_DS_DIGEST
BIF_CALC_T(MD5,		"MD5");
BIF_CALC_T(SHA1,	"SHA1");
BIF_CALC_T(SHA224,	"SHA224");
BIF_CALC_T(SHA256,	"SHA256");
BIF_CALC_T(SHA384,	"SHA384");
BIF_CALC_T(SHA512,	"SHA512");
//------------------------------------------------------------------------------------------//
CMDID BIF_CALC_MD5::Help(CMD_ENV* env,uint32 flag)const{
	PrintB(env,".CMD = MD5(<Expression|FileName>) -->Calculate MD5.");
	PrintB(env,"  Command = <'MD5(<Expression|FileName>)>[//COMMENT]");
	PrintP(env,"   eg:");
	PrintP(env,"     Command = 'MD5(leif.wen)       //Result: 4A59D526EA4076DF64FF45AF8FF82621");
	PrintP(env,"     Command = 'MD5(./test.txt)     //Calculate file ./test.txt");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIF_CALC_SHA1::Help(CMD_ENV* env,uint32 flag)const{
	PrintB(env,".CMD = SHA1(<Expression|FileName>) -->Calculate SHA1.");
	PrintB(env,"  Command = <'SHA1(<Expression|FileName>)>[//COMMENT]");
	PrintP(env,"   eg:");
	PrintP(env,"     Command = 'SHA1(leif.wen)       //Result: BD5EC1DE30861E6C1BF5FC7995C6CBAFAABF78E4");
	PrintP(env,"     Command = 'SHA1(./test.txt)     //Calculate file ./test.txt");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIF_CALC_SHA224::Help(CMD_ENV* env,uint32 flag)const{
	PrintB(env,".CMD = SHA224(<Expression|FileName>) -->Calculate SHA224.");
	PrintB(env,"  Command = <'SHA224(<Expression|FileName>)>[//COMMENT]");
	PrintP(env,"   eg:");
	PrintP(env,"     Command = 'SHA224(leif.wen)      //Result: E97B789F4B626EDB61547D27FFC6E678E363F33C1F7295D76967E4F7");
	PrintP(env,"     Command = 'SHA224(./test.txt)    //Calculate file ./test.txt");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIF_CALC_SHA256::Help(CMD_ENV* env,uint32 flag)const{
	PrintB(env,".CMD = SHA256(<Expression|FileName>) -->Calculate SHA256.");
	PrintB(env,"  Command = <'SHA256(<Expression|FileName>)>[//COMMENT]");
	PrintP(env,"   eg:");
	PrintP(env,"     Command = 'SHA256(leif.wen)      //Result: A33FD3E2A864E17F8BB87D23B774954D3194BED1C376BDD17B5965234759C780");
	PrintP(env,"     Command = 'SHA256(./test.txt)    //Calculate file ./test.txt");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIF_CALC_SHA384::Help(CMD_ENV* env,uint32 flag)const{
	PrintB(env,".CMD = SHA384(<Expression|FileName>) -->Calculate SHA384.");
	PrintB(env,"  Command = <'SHA384(<Expression|FileName>)>[//COMMENT]");
	PrintP(env,"   eg:");
	PrintP(env,"     Command = 'SHA384(leif.wen)    //Result: 4DB8C402C13CC2F064ED6C619164CA79B4758572D594B35C3FE32BCD439C02B318A17D365DE50A01C75EF9D72E92CCD5");
	PrintP(env,"     Command = 'SHA384(./test.txt)  //Calculate file ./test.txt");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIF_CALC_SHA512::Help(CMD_ENV* env,uint32 flag)const{
	PrintB(env,".CMD = SHA512(<Expression|FileName>) -->Calculate SHA512.");
	PrintB(env,"  Command = <'SHA512(<Expression|FileName>)>[//COMMENT]");
	PrintP(env,"   eg:");
	PrintP(env,"     Command = 'SHA512(leif.wen)      //Result:	33011AB95EE31DB208748AE7AB3F47FD1E6B51CC69FF855D8670066C259A55B5BA3F965E035F61A68A331BCE1CCD1317C857C0CBC3BBFA129D61E3A53A294392");
	PrintP(env,"     Command = 'SHA512(./test.txt)    //Calculate file ./test.txt");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
#endif
#ifdef ALG_BASE64_h
//------------------------------------------------------------------------------------------//
CMDID BIF_CALC_BASE64::Help(CMD_ENV* env,uint32 flag)const{
	PrintB(env,".CMD = BASE64(<E|D>,<Expression|FileName>) -->Calculate BASE64.");
	PrintB(env,"  Command = <'BASE64(<E|D>,<Expression|FileName>)>[//COMMENT]");
	PrintP(env,"   eg:");
	PrintP(env,"     Command = 'BASE64(E,leif.wen)          //Encode Reslut: bGVpZi53ZW4=");
	PrintP(env,"     Command = 'BASE64(D,bGVpZi53ZW4=)      //Decode Reslut: leif.wen");
	PrintP(env,"     Command = 'BASE64(E,./test.txt)        //Encode file ./test.txt");
	PrintP(env,"     Command = 'BASE64(D,./test.txt)        //Decode file ./test.txt");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIF_CALC_BASE64::Command(CMD_ENV* env,const STDSTR& msg,void* bif_retstr)const{
	BIF_RETSTR	*ret = static_cast<BIF_RETSTR*>(bif_retstr);
	ALG_BASE64	b64;
	STDSTR		par1,par2;
	
	b64.InitSize(512);
	SplitPar1(par1, par2, msg, ",");
	Str_LowerCaseSelf(par1);
	
	ret->result = "";
	if ((par1 == "e") || (par1 == "d")){
		if (par2.length() > 0){
			par2 = Str_Trim(Str_SplitSubItem(&par2, ' '));
			
			if (CFS_CheckFile(par2) > 0){
				if (par1 == "e"){
					b64.InitCFG(ALG_BASE64::CFG_Encode | ALG_BASE64::CFG_NL).AllIn(nullptr, &ret->forSend, IUD_FILE(par2));
					ret->forPrint += "[BASE64 encode file:";
				}
				else{
					b64.InitCFG(ALG_BASE64::CFG_Decode | ALG_BASE64::CFG_NL).AllIn(nullptr, &ret->forSend, IUD_FILE(par2));
					ret->forPrint += "[BASE64 decode file:";
				}
				ret->forPrint += par2;
				ret->forPrint += "]";
				return(BIF_ID_COMBINE_CALC_BASE64_FILE);
			}
			else{
				if (par1 == "e"){
					BIF_Transform(nullptr, _EMPTY(&par1), nullptr,par2,CMD_NONE,G_ESCAPE_ON);
					b64.InitCFG(ALG_BASE64::CFG_Encode | ALG_BASE64::CFG_NL).AllIn(nullptr, &ret->result, par1);
				}
				else{
					BIF_Transform(nullptr, _EMPTY(&par1), nullptr,par2,CMD_NONE,G_ESCAPE_ON);
					b64.InitCFG(ALG_BASE64::CFG_Decode | ALG_BASE64::CFG_NL).AllIn(nullptr, &ret->result, par1);
				}
				ret->forPrint += ret->result;
				ret->forSend += ret->result;
			}
		}
	}
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
#endif
//------------------------------------------------------------------------------------------//











//------------------------------------------------------------------------------------------//
BIF_COMBINE::BIF_COMBINE(void) : BIF_BASE() {
	cgCommandID = BIF_ID_COMBINE;
	cgCommand = "combine,=/build,=/send,=";
	
	AppendDown(cgSubC_SQ) < cgSubC_HEX < cgSubC_VIEWINHEX < cgSubC_TIME < cgSubC_STRING < cgSubC_FILE < cgSubC_DEFINE < cgSubC_REPLACE
#ifdef ALG_DS_DIGEST
	< cgSubC_CALC_MD5 < cgSubC_CALC_SHA1 < cgSubC_CALC_SHA224 < cgSubC_CALC_SHA256 < cgSubC_CALC_SHA384 < cgSubC_CALC_SHA512
#endif
#ifdef ALG_BASE64_h
	< cgSubC_CALC_BASE64
#endif
	< cgSubC_RET;
	
	SetSFlag(CMD_blTrySubCMD);
};
//------------------------------------------------------------------------------------------//
CMDID BIF_COMBINE::Help(CMD_ENV* env,uint32 flag)const{
	PrintB(env,".CMD = Combine=<Expression> -->Create new \"Command\".");
	PrintB(env,"  Command = <'Combine=<Expression>>[//COMMENT]");
	PrintP(env,"  Notes:1.Expression Operators is +.");
	PrintP(env,"        2.Can use \"\" set string. Use \\\", \\+ to escape \", +.");
	PrintP(env,"        3.Support \\0xhh, \\0Xhh, \\a, \\b, \\f, \\n, \\r, \\t, \\v, \\\\, \\', \\\", \\0, \\/, \\*, \\?.");
	PrintP(env,"        4.support sub command :'','hex,'viewhex,'ret,'time,'string,'file,'replace,");
	PrintP(env,"          'MD5,'SHA1,'SHA224,'SHA256,'SHA384,'SHA512,'BASE64.");
	PrintP(env,"     eg:");
	PrintP(env,"       Command = 'Combine = Happy + New + Year //new \"Command\" is HappyNewYear.");
	PrintP(env,"     If last CMD is Search, and");
	PrintP(env,"       'RET(1) = Leif Wen");
	PrintP(env,"       'RET(2) = +86-180-8888-0966");
	PrintP(env,"       Command = 'Combine = \"Name: \" + 'RET(1) + \".MP: \" + 'RET(2)");
	PrintP(env,"                       //New \"Command\" is Name: Leif Wen.MP: +86-180-8888-0966");
	TraversalChildHelp(env,flag);
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
BIF_COMBINE& BIF_COMBINE::GetCombine(void){
	static BIF_COMBINE sgCombine;
	return(sgCombine);
};
//------------------------------------------------------------------------------------------//
namespace BIF {
	static void FormatString	(STDSTR* retStr,const STDSTR& strIn);
	static void SplitLetter		(STDSTR* retStr,STDSTR* strIn);
};
//------------------------------------------------------------------------------------------//
CMDID BIF_COMBINE::Command(CMD_ENV* env,const STDSTR& msg,void* bif_retstr)const{
	BIF_RETSTR *ret = static_cast<BIF_RETSTR*>(bif_retstr);
	STDSTR	strIn,strSub;
	
	BIF::FormatString(&strIn,msg);

	while(strIn.length() > 0){
		BIF::SplitLetter(&strSub,&strIn);
		if (Dispose(env,strSub,bif_retstr) == BIF_ID_NO){
			ret->result = strSub;
			ret->forPrint += ret->result;
			Str_CharToStr(&ret->forSend, (uint8*)ret->result.c_str(), ret->result.length(), G_ASCII, G_SPACE_OFF, G_ESCAPE_ON);
		}
	}
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
static void BIF::FormatString(STDSTR* retStr,const STDSTR& strIn){
	//formate to HEX without space
	//escape +,"
	STDSTR	strData,strResult;
	STRSIZE	i,length;
	uint8	charData;

	length = strIn.length();
	i = 0;
	*retStr = "";
	strData = "";
	
	while(i < length){
		charData = strIn[i];
		++ i;
		if (charData == '+'){
			Str_ASCIIToHEX(retStr, Str_Trim(strData), G_ESCAPE_OFF);
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
	Str_ASCIIToHEX(retStr, Str_Trim(strData), G_ESCAPE_OFF);
};
//------------------------------------------------------------------------------------------//
static void BIF::SplitLetter(STDSTR* retStr,STDSTR* strIn){
	// remove " in front and back.
	
	Str_HEXToASCII(_EMPTY(retStr), Str_ReadSubItem(strIn, ","));
	if (retStr->length() > 1){
		if (((*retStr)[0] == '"') && ((*retStr)[retStr->length() - 1] == '"'))
			*retStr = retStr->substr(1,retStr->length() - 2);
	}
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
CMDID BIF_PRINT::Help(CMD_ENV* env,uint32 flag)const{
	PrintB(env,".CMD = Print=<Expression> -->Print log.");
	PrintB(env,"  Command = <'Print=<Expression>>[//COMMENT]");
	PrintP(env,"  Notes:1.Expression Operators is +.");
	PrintP(env,"        2.Can use \"\" set string. Use \\\", \\+ to escape \", +.");
	PrintP(env,"        3.Support \\0xhh, \\0Xhh, \\a, \\b, \\f, \\n, \\r, \\t, \\v, \\\\, \\', \\\", \\0, \\/, \\*, \\?.");
	PrintP(env,"        4.support sub command :same as 'combine.");
	PrintP(env,"     eg:");
	PrintP(env,"       Command = 'Print = Happy + New + Year //Print HappyNewYear.");
	
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIF_PRINT::Command(CMD_ENV* env,const STDSTR& msg,void* bif_retstr)const{
	STDSTR 	retForPrint;
	
	BIF_Transform(env,nullptr,_EMPTY(&retForPrint),msg,CMD_NONE,G_ESCAPE_ON);
	PrintWithDividingLine(env,"Print:",retForPrint);
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
CMDID BIF_Transform(CMD_ENV* env,STDSTR* retForSend,STDSTR* retForPrint,const STDSTR& inputCommand,CMD_TAIL tail,G_ESCAPE blEscape){
	BIF_RETSTR			ret;
	CMDID 	eIDCode;
	STDSTR				strOCommand;
	
	
	ret.result = "";
	ret.forPrint = "";
	ret.forSend = "";
	strOCommand = CMD_NODE::DelComment(inputCommand);
	eIDCode = BIF_COMBINE::GetCombine().Dispose(env,strOCommand,&ret);
	switch (eIDCode){
		case BIF_ID_COMBINE_FILE:
		case BIF_ID_COMBINE_CALC_BASE64_FILE:
		case BIF_ID_COMBINE_DEFINE:
			break;
		case BIF_ID_NO:
			ret.result = "";
			ret.forPrint = strOCommand;
			Str_CharToStr(&ret.forSend, (uint8*)strOCommand.c_str(), strOCommand.length(), G_ASCII, G_SPACE_OFF, blEscape);
		default:
			switch (tail) {
				case CMD_R:
					ret.forPrint += "\\r";
					ret.forSend += "\r";
					break;
				case CMD_N:
					ret.forPrint += "\\n";
					ret.forSend += "\n";
					break;
				case CMD_RN:
					ret.forPrint += "\\r\\n";
					ret.forSend += "\r\n";
					break;
				default:
					break;
			}
			break;
	}
	if (retForSend != nullptr)
		*retForSend += ret.forSend;
	if (retForPrint != nullptr)
		*retForPrint += ret.forPrint;
	return(eIDCode);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif /* BIF_Transform_h */

