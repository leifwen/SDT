/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: BIC_CALC.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.01.25
*/
#ifndef BIC_CALCH
#define BIC_CALCH
//------------------------------------------------------------------------------------------//
#include "BIC_B.h"
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
class BIC_CALC_HEX2DEC : public BIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
				 BIC_CALC_HEX2DEC(void) : BIC_Node() {cgCommand = "hex2dec";cgReturnCode = BI_RETCODE_CALC_HEX2DEC;};
		virtual ~BIC_CALC_HEX2DEC(void){;};
	public:
		virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_CALC_DEC2HEX : public BIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
				 BIC_CALC_DEC2HEX(void) : BIC_Node() {cgCommand = "dec2hex";cgReturnCode = BI_RETCODE_CALC_DEC2HEX;};
		virtual ~BIC_CALC_DEC2HEX(void){;};
	public:
		virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_CALC_MD5 : public BIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
				 BIC_CALC_MD5(void) : BIC_Node() {cgCommand = "md5";cgReturnCode = BI_RETCODE_CALC_MD5;};
		virtual ~BIC_CALC_MD5(void){;};
	public:
		virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_CALC_SHA1 : public BIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
				 BIC_CALC_SHA1(void) : BIC_Node() {cgCommand = "sha1";cgReturnCode = BI_RETCODE_CALC_SHA1;};
		virtual ~BIC_CALC_SHA1(void){;};
	public:
		virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_CALC_SHA224 : public BIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
				 BIC_CALC_SHA224(void) : BIC_Node() {cgCommand = "sha224";cgReturnCode = BI_RETCODE_CALC_SHA224;};
		virtual ~BIC_CALC_SHA224(void){;};
	public:
		virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_CALC_SHA256 : public BIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
		public:
				 BIC_CALC_SHA256(void) : BIC_Node() {cgCommand = "sha256";cgReturnCode = BI_RETCODE_CALC_SHA256;};
		virtual ~BIC_CALC_SHA256(void){;};
	public:
		virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_CALC_SHA384 : public BIC_Node{
	public:
			enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
				 BIC_CALC_SHA384(void) : BIC_Node() {cgCommand = "sha384";cgReturnCode = BI_RETCODE_CALC_SHA384;};
		virtual ~BIC_CALC_SHA384(void){;};
	public:
		virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_CALC_SHA512 : public BIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
				 BIC_CALC_SHA512(void) : BIC_Node() {cgCommand = "sha512";cgReturnCode = BI_RETCODE_CALC_SHA512;};
		virtual ~BIC_CALC_SHA512(void){;};
	public:
		virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_CALC_BASE64 : public BIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
				 BIC_CALC_BASE64(void) : BIC_Node() {cgCommand = "base64";cgReturnCode = BI_RETCODE_CALC_BAS64;};
		virtual ~BIC_CALC_BASE64(void){;};
	public:
		virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_CALC : public BIC_Node_S{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node_S::RFLAG_S + BIC_Node_S::RFLAG_C};
	public:
				 BIC_CALC(void) : BIC_Node_S() {cgCommand = "calc";cgTitle = cgCommand;cgReturnCode = BI_RETCODE_CALC;Init();};
	    virtual ~BIC_CALC(void){;};
	public:
	    virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
	private:
	    void	Init(void){
	        AddNode(&cgSub_hex2dec);
			AddNode(&cgSub_dec2hex);
		#if (defined USE_OPENSSL) || (defined CommonDefH_MAC)
			AddNode(&cgSub_md5);
			AddNode(&cgSub_sha1);
			AddNode(&cgSub_sha224);
			AddNode(&cgSub_sha256);
			AddNode(&cgSub_sha384);
			AddNode(&cgSub_sha512);
			AddNode(&cgSub_base64);
		#endif
	    }
	    BIC_CALC_HEX2DEC		cgSub_hex2dec;
		BIC_CALC_DEC2HEX		cgSub_dec2hex;
	#if (defined USE_OPENSSL) || (defined CommonDefH_MAC)
		BIC_CALC_MD5			cgSub_md5;
		BIC_CALC_SHA1			cgSub_sha1;
		BIC_CALC_SHA224			cgSub_sha224;
		BIC_CALC_SHA256			cgSub_sha256;
		BIC_CALC_SHA384			cgSub_sha384;
		BIC_CALC_SHA512			cgSub_sha512;
	#endif
		BIC_CALC_BASE64			cgSub_base64;
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif