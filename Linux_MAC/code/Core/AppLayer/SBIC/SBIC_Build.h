/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: SBIC_Build.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.08.12
*/
//------------------------------------------------------------------------------------------//
#include "SBIC_B.h"
//------------------------------------------------------------------------------------------//
#ifndef SBIC_ENUMH
#define SBIC_ENUMH
enum{
	CMD_ASCII = 0,
	CMD_HEX,
	CMD_FILE,
};
#endif
//------------------------------------------------------------------------------------------//
#ifdef SBIC_BH
#ifndef SBIC_BuildH
#define SBIC_BuildH
#ifdef SBIC_BuildH
//------------------------------------------------------------------------------------------//
class SBIC_SQ : public SBIC_Node{
	public:
		SBIC_SQ(void) : SBIC_Node() {
			cgCommand = "'";
			cgReturnCode = SBI_RETCODE_COMBINE_SQ;
		};
		virtual ~SBIC_SQ(void){;};
	public:
		virtual	int32	Check	(SBIC_ENV *env,const STDSTR &rawIn,STDSTR *retPar)const;
		virtual	int32	Command	(SBIC_ENV *env,const STDSTR &par,void *retStr)const;
		virtual	int32	Help	(SBIC_ENV *env,int32 blDetail = 1)const{
			PrintB(env,".CMD = '[PAR] -->Send '[PAR].");
			PrintB(env,"  Command = <''[PAR]>[//COMMENT]");
			PrintP(env,"   eg:");
			PrintP(env,"     Command = ''       //2Bytes,send '");
			PrintP(env,"     Command = ''TEST   //4Bytes,send 'TEST");
			PrintP(env,"     Command = '' TEST  //5Bytes,send ' TEST");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class SBIC_HEX : public SBIC_Node{
	public:
		SBIC_HEX(void) : SBIC_Node() {
			cgCommand = "hex,:/hex,(,)";
			cgReturnCode = SBI_RETCODE_COMBINE_HEX;
		};
		virtual ~SBIC_HEX(void){;};
	public:
		virtual	int32	Command	(SBIC_ENV *env,const STDSTR &par,void *retStr)const;
		virtual	int32	Help	(SBIC_ENV *env,int32 blDetail = 1)const{
			PrintB(env,".CMD = HEX:[PAR] -->Send HEX format data.");
			PrintB(env,"  Command = <'HEX:[PAR]>[//COMMENT]");
			PrintP(env,"   eg:");
			PrintP(env,"     Command = 'HEX:35 36 37      //Send: 0x35,0x36,0x37,3Bytes");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class SBIC_VIEWINHEX : public SBIC_Node{
	public:
		SBIC_VIEWINHEX(void) : SBIC_Node() {
			cgCommand = "viewhex,:";
			cgReturnCode = SBI_RETCODE_COMBINE_VIEWHEX;
		};
		virtual ~SBIC_VIEWINHEX(void){;};
	public:
		virtual	int32	Command	(SBIC_ENV *env,const STDSTR &par,void *retStr)const;
		virtual	int32	Help	(SBIC_ENV *env,int32 blDetail = 1)const{
			PrintB(env,".CMD = viewhex:[PAR] -->View send data in HEX format.");
			PrintB(env,"  Command = <'viewhex:[PAR]>[//COMMENT]");
			PrintP(env,"   eg:");
			PrintP(env,"     Command = 'viewhex:ABCD      //Send: ABCD,4Bytes,but show [HEX:41 42 43 44]");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class SBIC_Time : public SBIC_Node{
	public:
		SBIC_Time(void) : SBIC_Node() {
			cgCommand = "time,(,)/time,=/now,(,)/now,=";
			cgReturnCode = SBI_RETCODE_COMBINE_TIME;
		};
		virtual ~SBIC_Time(void){;};
	public:
		virtual	int32	Command	(SBIC_ENV *env,const STDSTR &par,void *retStr)const;
		virtual	int32	Help	(SBIC_ENV *env,int32 blDetail = 1)const{
			PrintB(env,".CMD = Time/Now(<PAR>) -->Get current time.");
			PrintB(env,"  Command = <'Time/Now=(<PAR>)>[//COMMENT]");
			PrintP(env,"  Notes:1.PAR is style string, eg: YY/MM/DD,hh:mm:ss(zzz).");
			PrintP(env,"        2.YY:Year, MM:Month, DD:Date, hh:Hour, mm:Minute, ss:Sencond, zzz:Millisecond.");
			PrintP(env,"   eg:");
			PrintP(env,"     If PC time is 08:29:25");
			PrintP(env,"       Command = 'Time(hh:mm:ss) //Send: 08:29:25");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class SBIC_RET : public SBIC_Node{
	public:
		SBIC_RET(void) : SBIC_Node() {
			cgCommand = "ret,(,)";
			cgReturnCode = SBI_RETCODE_COMBINE_RET;
		};
		virtual ~SBIC_RET(void){;};
	public:
		virtual	int32	Command	(SBIC_ENV *env,const STDSTR &par,void *retStr)const;
		virtual	int32	Help	(SBIC_ENV *env,int32 blDetail = 1)const{
			PrintB(env,".CMD = RET(<PAR>) -->Get CMD Search's result.");
			PrintB(env,"  Command = <'RET(<PAR>)>");
			PrintP(env,"     eg:");
			PrintP(env,"     If last CMD is Search, and");
			PrintP(env,"       'RET(1) = Leif Wen");
			PrintP(env,"       'RET(2) = +86-138-6062-4143");
			PrintP(env,"       Command = 'Combine = \"Name: \" + 'RET(1) + \".MP: \" + 'RET(2) //New \"Command\" is Name: Leif Wen.MP: +86-138-6062-4143");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class SBIC_REPLACE : public SBIC_Node{
	public:
		SBIC_REPLACE(void) : SBIC_Node() {
			cgCommand = "rep,:/replace,:";
			cgReturnCode = SBI_RETCODE_COMBINE_REPLACE;
		};
		virtual ~SBIC_REPLACE(void){;};
	public:
		virtual	int32	Command	(SBIC_ENV *env,const STDSTR &par,void *retStr)const;
		virtual	int32	Help	(SBIC_ENV *env,int32 blDetail = 1)const{
			PrintB(env,".CMD = rep:<PAR> -->Replace PAR which define in 'define command.");
			PrintB(env,"  Command = <'rep:<PAR>>");
			PrintP(env,"     eg:");
			PrintP(env,"     If 'define: v1=Leif Wen;v2=+86-138-6062-4143;");
			PrintP(env,"       Command = 'rep:Name: v1.MP: v2 //New \"Command\" is Name: Leif Wen.MP: +86-138-6062-4143");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class SBIC_STRING : public SBIC_Node{
	public:
		SBIC_STRING(void) : SBIC_Node() {
			cgCommand = "string,(,)/str,(,)";
			cgReturnCode = SBI_RETCODE_COMBINE_STRING;
		};
		virtual ~SBIC_STRING(void){;};
	public:
		virtual	int32	Command	(SBIC_ENV *env,const STDSTR &par,void *retStr)const;
		virtual	int32	Help	(SBIC_ENV *env,int32 blDetail = 1)const{
			PrintB(env,".CMD = string(<len>) -->Create string, len > 0 and len < 64 * 512.");
			PrintB(env,"  Command = <'string(len)>");
			PrintP(env,"     eg:");
			PrintP(env,"       Command = 'string(10) //Send: 001>?@ABCD\\r");
			return(cgReturnCode);
		};
};
#if (defined USE_OPENSSL) || (defined CommonDefH_MAC)
//------------------------------------------------------------------------------------------//
class SBIC_CALC_MD5 : public SBIC_Node{
	public:
		SBIC_CALC_MD5(void) : SBIC_Node() {
			cgCommand = "MD5,(,)/MD5,=";
			cgReturnCode = SBI_RETCODE_COMBINE_CALC_MD5;
		};
		virtual ~SBIC_CALC_MD5(void){;};
	public:
		virtual	int32	Command	(SBIC_ENV *env,const STDSTR &par,void *retStr)const;
		virtual	int32	Help	(SBIC_ENV *env,int32 blDetail = 1)const{
			PrintB(env,".CMD = MD5(<Expression|FileName>) -->Calculate MD5.");
			PrintB(env,"  Command = <'MD5(<Expression|FileName>)>[//COMMENT]");
			PrintP(env,"   eg:");
			PrintP(env,"     Command = 'MD5(leif.wen)       //Reslut: 4A59D526EA4076DF64FF45AF8FF82621");
			PrintP(env,"     Command = 'MD5(./test.txt)     //Calculate file ./test.txt");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class SBIC_CALC_SHA1 : public SBIC_Node{
	public:
		SBIC_CALC_SHA1(void) : SBIC_Node() {
			cgCommand = "SHA1,(,)/SHA1,=";
			cgReturnCode = SBI_RETCODE_COMBINE_CALC_SHA1;
		};
		virtual ~SBIC_CALC_SHA1(void){;};
	public:
		virtual	int32	Command	(SBIC_ENV *env,const STDSTR &par,void *retStr)const;
		virtual	int32	Help	(SBIC_ENV *env,int32 blDetail = 1)const{
			PrintB(env,".CMD = SHA1(<Expression|FileName>) -->Calculate SHA1.");
			PrintB(env,"  Command = <'SHA1(<Expression|FileName>)>[//COMMENT]");
			PrintP(env,"   eg:");
			PrintP(env,"     Command = 'SHA1(leif.wen)       //Reslut: BD5EC1DE30861E6C1BF5FC7995C6CBAFAABF78E4");
			PrintP(env,"     Command = 'SHA1(./test.txt)     //Calculate file ./test.txt");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class SBIC_CALC_SHA224 : public SBIC_Node{
	public:
		SBIC_CALC_SHA224(void) : SBIC_Node() {
			cgCommand = "SHA224,(,)/SHA224,=";
			cgReturnCode = SBI_RETCODE_COMBINE_CALC_SHA224;
		};
		virtual ~SBIC_CALC_SHA224(void){;};
	public:
		virtual	int32	Command	(SBIC_ENV *env,const STDSTR &par,void *retStr)const;
		virtual	int32	Help	(SBIC_ENV *env,int32 blDetail = 1)const{
			PrintB(env,".CMD = SHA224(<Expression|FileName>) -->Calculate SHA224.");
			PrintB(env,"  Command = <'SHA224(<Expression|FileName>)>[//COMMENT]");
			PrintP(env,"   eg:");
			PrintP(env,"     Command = 'SHA224(leif.wen)       //Reslut: E97B789F4B626EDB61547D27FFC6E678E363F33C1F7295D76967E4F7");
			PrintP(env,"     Command = 'SHA224(./test.txt)     //Calculate file ./test.txt");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class SBIC_CALC_SHA256 : public SBIC_Node{
	public:
		SBIC_CALC_SHA256(void) : SBIC_Node() {
			cgCommand = "SHA256,(,)/SHA256,=";
			cgReturnCode = SBI_RETCODE_COMBINE_CALC_SHA256;
		};
		virtual ~SBIC_CALC_SHA256(void){;};
	public:
		virtual	int32	Command	(SBIC_ENV *env,const STDSTR &par,void *retStr)const;
		virtual	int32	Help	(SBIC_ENV *env,int32 blDetail = 1)const{
			PrintB(env,".CMD = SHA256(<Expression|FileName>) -->Calculate SHA256.");
			PrintB(env,"  Command = <'SHA256(<Expression|FileName>)>[//COMMENT]");
			PrintP(env,"   eg:");
			PrintP(env,"     Command = 'SHA256(leif.wen)       //Reslut: A33FD3E2A864E17F8BB87D23B774954D3194BED1C376BDD17B5965234759C780");
			PrintP(env,"     Command = 'SHA256(./test.txt)     //Calculate file ./test.txt");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class SBIC_CALC_SHA384 : public SBIC_Node{
	public:
		SBIC_CALC_SHA384(void) : SBIC_Node() {
			cgCommand = "SHA384,(,)/SHA384,=";
			cgReturnCode = SBI_RETCODE_COMBINE_CALC_SHA384;
		};
		virtual ~SBIC_CALC_SHA384(void){;};
	public:
		virtual	int32	Command	(SBIC_ENV *env,const STDSTR &par,void *retStr)const;
		virtual	int32	Help	(SBIC_ENV *env,int32 blDetail = 1)const{
			PrintB(env,".CMD = SHA384(<Expression|FileName>) -->Calculate SHA384.");
			PrintB(env,"  Command = <'SHA384(<Expression|FileName>)>[//COMMENT]");
			PrintP(env,"   eg:");
			PrintP(env,"     Command = 'SHA384(leif.wen)       //Reslut: 4DB8C402C13CC2F064ED6C619164CA79B4758572D594B35C3FE32BCD439C02B318A17D365DE50A01C75EF9D72E92CCD5");
			PrintP(env,"     Command = 'SHA384(./test.txt)     //Calculate file ./test.txt");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class SBIC_CALC_SHA512 : public SBIC_Node{
	public:
		SBIC_CALC_SHA512(void) : SBIC_Node() {
			cgCommand = "SHA512,(,)/SHA512,=";
			cgReturnCode = SBI_RETCODE_COMBINE_CALC_SHA512;
		};
		virtual ~SBIC_CALC_SHA512(void){;};
	public:
		virtual	int32	Command	(SBIC_ENV *env,const STDSTR &par,void *retStr)const;
		virtual	int32	Help	(SBIC_ENV *env,int32 blDetail = 1)const{
			PrintB(env,".CMD = SHA512(<Expression|FileName>) -->Calculate SHA512.");
			PrintB(env,"  Command = <'SHA512(<Expression|FileName>)>[//COMMENT]");
			PrintP(env,"   eg:");
			PrintP(env,"     Command = 'SHA512(leif.wen)       //Reslut:	33011AB95EE31DB208748AE7AB3F47FD1E6B51CC69FF855D8670066C259A55B5BA3F965E035F61A68A331BCE1CCD1317C857C0CBC3BBFA129D61E3A53A294392");
			PrintP(env,"     Command = 'SHA512(./test.txt)     //Calculate file ./test.txt");
			return(cgReturnCode);
		};
};
#endif
//------------------------------------------------------------------------------------------//
class SBIC_CALC_BASE64 : public SBIC_Node{
	public:
		SBIC_CALC_BASE64(void) : SBIC_Node() {
			cgCommand = "BASE64,(,)/BASE64,=";
			cgReturnCode = SBI_RETCODE_COMBINE_CALC_BASE64;
		};
		virtual ~SBIC_CALC_BASE64(void){;};
	public:
		virtual	int32	Command	(SBIC_ENV *env,const STDSTR &par,void *retStr)const;
		virtual	int32	Help	(SBIC_ENV *env,int32 blDetail = 1)const{
			PrintB(env,".CMD = BASE64(<E|D>,<Expression|FileName>) -->Calculate BASE64.");
			PrintB(env,"  Command = <'BASE64(<E|D>,<Expression|FileName>)>[//COMMENT]");
			PrintP(env,"   eg:");
			PrintP(env,"     Command = 'BASE64(E,leif.wen)          //Encode Reslut: bGVpZi53ZW4=");
			PrintP(env,"     Command = 'BASE64(D,bGVpZi53ZW4=)      //Decode Reslut: leif.wen");
			PrintP(env,"     Command = 'BASE64(E,./test.txt)        //Encode file ./test.txt");
			PrintP(env,"     Command = 'BASE64(D,./test.txt)        //Decode file ./test.txt");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class SBIC_SENDFILE : public SBIC_Node{
	public:
		SBIC_SENDFILE(void) : SBIC_Node() {
			cgCommand = "SendFile,(,)/SendFile,=";
			cgReturnCode = SBI_RETCODE_SENDFILE;
		};
		virtual ~SBIC_SENDFILE(void){;};
	public:
		virtual	int32	Command	(SBIC_ENV *env,const STDSTR &par,void *retStr)const;
		virtual	int32	Help	(SBIC_ENV *env,int32 blDetail = 1)const{
			PrintB(env,".CMD = SendFile(<FileName>) -->Send file.");
			PrintB(env,"  Command = <'SendFile(<FileName>)>[//COMMENT]");
			PrintP(env,"   eg:");
			PrintP(env,"     Command = 'SendFile(./test.txt)        //Send file ./test.txt");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
class SBIC_Combine : public SBIC_Node{
	public:
		SBIC_Combine(void) : SBIC_Node() {
			cgCommand = "Combine,=/build,=/send,=";
			cgReturnCode = SBI_RETCODE_COMBINE;
			Add(cgSubC_SQ) < cgSubC_VIEWHEX < cgSubC_HEX < cgSubC_Time < cgSubC_STRING < cgSubC_REPLACE
			#if (defined USE_OPENSSL) || (defined CommonDefH_MAC)
				< cgSubC_CALC_MD5 < cgSubC_CALC_SHA1 < cgSubC_CALC_SHA224 < cgSubC_CALC_SHA256 < cgSubC_CALC_SHA384 < cgSubC_CALC_SHA512
			#endif
			< cgSubC_CALC_BASE64
			< cgSubC_RET;
		};
		virtual ~SBIC_Combine(void){;};
	public:
		virtual	int32	Command	(SBIC_ENV *env,const STDSTR &par,void *retStr)const;
		virtual	int32	Help	(SBIC_ENV *env,int32 blDetail = 1)const{
			PrintB(env,".CMD = combine=<Expression> -->Create new \"Command\".");
			PrintB(env,"  Command = <'Combine=<Expression>>[//COMMENT]");
			PrintP(env,"  Notes:1.Expression Operators is +.");
			PrintP(env,"        2.Can use \"\" set string. Use \\\", \\+ to escape \", +.");
			PrintP(env,"        3.Support \\0xhh, \\0Xhh, \\a, \\b, \\f, \\n, \\r, \\t, \\v, \\\\, \\', \\\", \\0, \\/, \\*, \\?.");
			PrintP(env,"        4.support sub command :'','hex,'ret,'time,'string,'MD5,'SHA1,'SHA224,'SHA256,'SHA384,'SHA512,'BASE64.");
			PrintP(env,"     eg:");
			PrintP(env,"       Command = 'Combine = Happy + New + Year //new \"Command\" is HappyNewYear.");
			PrintP(env,"     If last CMD is Search, and");
			PrintP(env,"       'RET(1) = Leif Wen");
			PrintP(env,"       'RET(2) = +86-138-6062-4143");
			PrintP(env,"       Command = 'Combine = \"Name: \" + 'RET(1) + \".MP: \" + 'RET(2) //New \"Command\" is Name: Leif Wen.MP: +86-138-6062-4143");
		
			HelpTraversalChild(env,blDetail);
			return(cgReturnCode);
		};
	protected:
		SBIC_RET		cgSubC_RET;
	private:
		SBIC_SQ			cgSubC_SQ;
		SBIC_VIEWINHEX	cgSubC_VIEWHEX;
		SBIC_HEX		cgSubC_HEX;
		SBIC_Time		cgSubC_Time;
		SBIC_STRING		cgSubC_STRING;
		SBIC_REPLACE	cgSubC_REPLACE;
		#if (defined USE_OPENSSL) || (defined CommonDefH_MAC)
			SBIC_CALC_MD5		cgSubC_CALC_MD5;
			SBIC_CALC_SHA1		cgSubC_CALC_SHA1;
			SBIC_CALC_SHA224	cgSubC_CALC_SHA224;
			SBIC_CALC_SHA256	cgSubC_CALC_SHA256;
			SBIC_CALC_SHA384	cgSubC_CALC_SHA384;
			SBIC_CALC_SHA512	cgSubC_CALC_SHA512;
		#endif
		SBIC_CALC_BASE64	cgSubC_CALC_BASE64;
	
		static	void		FormatString(STDSTR *retStr,const STDSTR &strIn);
		static	void		SplitLetter	(STDSTR *retStr,STDSTR *strIn);
};
//------------------------------------------------------------------------------------------//
class SBIC_Print : public SBIC_Combine{
	public:
		SBIC_Print(void) : SBIC_Combine() {
			cgCommand = "print,=";
			cgReturnCode = SBI_RETCODE_PRINT;
			Add(cgSubC_SendFile) < cgSubC_Combine;
			cgSubC_RET.RemoveSelf();
		};
		virtual ~SBIC_Print(void){;};
	public:
		virtual	int32	Command	(SBIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(SBIC_ENV *env,int32 blDetail = 1)const;
	private:
		SBIC_SENDFILE		cgSubC_SendFile;
		SBIC_Combine		cgSubC_Combine;
	public:
		int32 CreateCommand(SBIC_ENV *env,STDSTR *retForSend,STDSTR *retForPrint,const STDSTR &inputCommand,CMD_TAIL tail,G_ESCAPE blEscape)const;
};
//------------------------------------------------------------------------------------------//
struct SBIC_RETSTR{
	STDSTR	forPrint;
	STDSTR	forSend;
	STDSTR	result;
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif
#endif
#endif
