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
#ifndef SBIC_BuildH
#define SBIC_BuildH
//------------------------------------------------------------------------------------------//
#include "SBIC_B.h"
//------------------------------------------------------------------------------------------//
class SBIC_SQ : public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
		SBIC_SQ(void) : SBIC_Node() {
			cgCommand = "'";
			cgReturnCode = SBI_RETCODE_SQ;
		};
		virtual ~SBIC_SQ(void){;};
	public:
		virtual	int32	Check	(SBICPAR *tBICPAR,const std::string &fullCMD,std::string *retPar)const;
		virtual	int32	Command	(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(SBICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class SBIC_HEX : public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
		SBIC_HEX(void) : SBIC_Node() {
			cgCommand = "hex,:/hex,(,)";
			cgReturnCode = SBI_RETCODE_HEX;
		};
		virtual ~SBIC_HEX(void){;};
	public:
		virtual	int32	Command	(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(SBICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class SBIC_Time : public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
		SBIC_Time(void) : SBIC_Node() {
			cgCommand = "time,(,)/time,=/now,(,)/now,=";
			cgReturnCode = SBI_RETCODE_TIME;
		};
		virtual ~SBIC_Time(void){;};
	public:
		virtual	int32	Command	(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(SBICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class SBIC_RET : public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
		SBIC_RET(void) : SBIC_Node() {
			cgCommand = "ret,(,)";
			cgReturnCode = SBI_RETCODE_RET;
		};
		virtual ~SBIC_RET(void){;};
	public:
		virtual	int32	Command	(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(SBICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class SBIC_STRING : public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
		SBIC_STRING(void) : SBIC_Node() {
			cgCommand = "string,(,)/str,(,)";
			cgReturnCode = SBI_RETCODE_STRING;
		};
		virtual ~SBIC_STRING(void){;};
	public:
		virtual	int32	Command	(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(SBICPAR *tBICPAR,int32 blDetail = 1)const;
};
#if (defined USE_OPENSSL) || (defined CommonDefH_MAC)
//------------------------------------------------------------------------------------------//
class SBIC_CALC_MD5 : public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
		SBIC_CALC_MD5(void) : SBIC_Node() {
			cgCommand = "MD5,(,)/MD5,=";
			cgReturnCode = SBI_RETCODE_CALC_MD5;
		};
		virtual ~SBIC_CALC_MD5(void){;};
	public:
		virtual	int32	Command	(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(SBICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class SBIC_CALC_SHA1 : public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
		SBIC_CALC_SHA1(void) : SBIC_Node() {
			cgCommand = "SHA1,(,)/SHA1,=";
			cgReturnCode = SBI_RETCODE_CALC_SHA1;
		};
		virtual ~SBIC_CALC_SHA1(void){;};
	public:
		virtual	int32	Command	(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(SBICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class SBIC_CALC_SHA224 : public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
		SBIC_CALC_SHA224(void) : SBIC_Node() {
			cgCommand = "SHA224,(,)/SHA224,=";
			cgReturnCode = SBI_RETCODE_CALC_SHA224;
		};
		virtual ~SBIC_CALC_SHA224(void){;};
	public:
		virtual	int32	Command	(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(SBICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class SBIC_CALC_SHA256 : public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
		SBIC_CALC_SHA256(void) : SBIC_Node() {
			cgCommand = "SHA256,(,)/SHA256,=";
			cgReturnCode = SBI_RETCODE_CALC_SHA256;
		};
		virtual ~SBIC_CALC_SHA256(void){;};
	public:
		virtual	int32	Command	(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(SBICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class SBIC_CALC_SHA384 : public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
		SBIC_CALC_SHA384(void) : SBIC_Node() {
			cgCommand = "SHA384,(,)/SHA384,=";
			cgReturnCode = SBI_RETCODE_CALC_SHA384;
		};
		virtual ~SBIC_CALC_SHA384(void){;};
	public:
		virtual	int32	Command	(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(SBICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class SBIC_CALC_SHA512 : public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
		SBIC_CALC_SHA512(void) : SBIC_Node() {
			cgCommand = "SHA512,(,)/SHA512,=";
			cgReturnCode = SBI_RETCODE_CALC_SHA512;
		};
		virtual ~SBIC_CALC_SHA512(void){;};
	public:
		virtual	int32	Command	(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(SBICPAR *tBICPAR,int32 blDetail = 1)const;
};
#endif
//------------------------------------------------------------------------------------------//
class SBIC_CALC_BASE64 : public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
		SBIC_CALC_BASE64(void) : SBIC_Node() {
			cgCommand = "BASE64,(,)/BASE64,=";
			cgReturnCode = SBI_RETCODE_CALC_BASE64;
		};
		virtual ~SBIC_CALC_BASE64(void){;};
	public:
		virtual	int32	Command	(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(SBICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class SBIC_SENDFILE : public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
		SBIC_SENDFILE(void) : SBIC_Node() {
			cgCommand = "SendFile,(,)/SendFile,=";
			cgReturnCode = SBI_RETCODE_SENDFILE;
		};
		virtual ~SBIC_SENDFILE(void){;};
	public:
		virtual	int32	Command	(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(SBICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//






















//------------------------------------------------------------------------------------------//
class SBIC_Combine : public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
		SBIC_Combine(void) : SBIC_Node() {
			cgCommand = "Combine,=/build,=/send,=";
			cgReturnCode = SBI_RETCODE_COMBINE;
			AddNode(&cgSubC_SQ);
			AddNode(&cgSubC_HEX);
			AddNode(&cgSubC_Time);
			AddNode(&cgSubC_RET);
			AddNode(&cgSubC_STRING);
			#if (defined USE_OPENSSL) || (defined CommonDefH_MAC)
				AddNode(&cgSubC_CALC_MD5);
				AddNode(&cgSubC_CALC_SHA1);
				AddNode(&cgSubC_CALC_SHA224);
				AddNode(&cgSubC_CALC_SHA256);
				AddNode(&cgSubC_CALC_SHA384);
				AddNode(&cgSubC_CALC_SHA512);
			#endif
			AddNode(&cgSubC_CALC_BASE64);
			AddNode(&cgSubC_SendFile);
		};
		virtual ~SBIC_Combine(void){;};
	public:
		virtual	int32	Command	(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(SBICPAR *tBICPAR,int32 blDetail = 1)const;
	protected:
		SBIC_RET	cgSubC_RET;
	private:
		SBIC_Time	cgSubC_Time;
		SBIC_HEX	cgSubC_HEX;
		SBIC_SQ		cgSubC_SQ;
		SBIC_STRING	cgSubC_STRING;
		#if (defined USE_OPENSSL) || (defined CommonDefH_MAC)
			SBIC_CALC_MD5		cgSubC_CALC_MD5;
			SBIC_CALC_SHA1		cgSubC_CALC_SHA1;
			SBIC_CALC_SHA224	cgSubC_CALC_SHA224;
			SBIC_CALC_SHA256	cgSubC_CALC_SHA256;
			SBIC_CALC_SHA384	cgSubC_CALC_SHA384;
			SBIC_CALC_SHA512	cgSubC_CALC_SHA512;
		#endif
		SBIC_CALC_BASE64	cgSubC_CALC_BASE64;
		SBIC_SENDFILE		cgSubC_SendFile;
		static	void		FormatString(const std::string &strInput,std::string *returnStr);
		static	std::string	SplitLetter	(std::string *strInput);
};
//------------------------------------------------------------------------------------------//
class SBIC_Print : public SBIC_Combine{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Combine::RFLAG_S + SBIC_Combine::RFLAG_C};
	public:
		SBIC_Print(void) : SBIC_Combine() {
			cgCommand = "print,=";
			cgReturnCode = SBI_RETCODE_PRINT;
			AddNode(&cgSubC_Combine);
			cgSubC_RET.RemoveSelf();
		};
		virtual ~SBIC_Print(void){;};
	public:
		virtual	int32	Command	(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(SBICPAR *tBICPAR,int32 blDetail = 1)const;
	private:
		SBIC_Combine		cgSubC_Combine;
	public:
		int32 CreateHexCommand(SBICPAR *cSBICPAR,const std::string &inputCommand,int32 blHEX,int32 bl0x0D,G_ESCAPE_VAILD blEscape
							   ,std::string *retHexCommand,std::string *retPrintData)const;
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif
