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

//------------------------------------------------------------------------------------------//
#include "BIC_B.h"
//------------------------------------------------------------------------------------------//
#ifdef BIC_BH
#ifndef BIC_CALCH
#define BIC_CALCH
#ifdef BIC_CALCH
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
class BIC_CALC_HEX2DEC : public BIC_Node{
	public:
				 BIC_CALC_HEX2DEC(void) : BIC_Node() {cgCommand = "hex2dec";cgReturnCode = BI_RETCODE_CALC_HEX2DEC;};
		virtual ~BIC_CALC_HEX2DEC(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"HEX to DEC, 64bit unsigned");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"[vaule]","HEX number.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_CALC_DEC2HEX : public BIC_Node{
	public:
				 BIC_CALC_DEC2HEX(void) : BIC_Node() {cgCommand = "dec2hex";cgReturnCode = BI_RETCODE_CALC_DEC2HEX;};
		virtual ~BIC_CALC_DEC2HEX(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"DEC to HEX, 64bit unsigned");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"[vaule]","DEC number.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_CALC_MD5 : public BIC_Node{
	public:
				 BIC_CALC_MD5(void) : BIC_Node() {cgCommand = "md5";cgReturnCode = BI_RETCODE_CALC_MD5;};
		virtual ~BIC_CALC_MD5(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"MD5");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"<filename|input>","File name or input string.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_CALC_SHA1 : public BIC_Node{
	public:
				 BIC_CALC_SHA1(void) : BIC_Node() {cgCommand = "sha1";cgReturnCode = BI_RETCODE_CALC_SHA1;};
		virtual ~BIC_CALC_SHA1(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"SHA1");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"<filename|input>","File name or input string.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_CALC_SHA224 : public BIC_Node{
	public:
				 BIC_CALC_SHA224(void) : BIC_Node() {cgCommand = "sha224";cgReturnCode = BI_RETCODE_CALC_SHA224;};
		virtual ~BIC_CALC_SHA224(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"SHA224");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"<filename|input>","File name or input string.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_CALC_SHA256 : public BIC_Node{
		public:
				 BIC_CALC_SHA256(void) : BIC_Node() {cgCommand = "sha256";cgReturnCode = BI_RETCODE_CALC_SHA256;};
		virtual ~BIC_CALC_SHA256(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"SHA256");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"<filename|input>","File name or input string.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_CALC_SHA384 : public BIC_Node{
	public:
				 BIC_CALC_SHA384(void) : BIC_Node() {cgCommand = "sha384";cgReturnCode = BI_RETCODE_CALC_SHA384;};
		virtual ~BIC_CALC_SHA384(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"SHA384");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"<filename|input>","File name or input string.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_CALC_SHA512 : public BIC_Node{
	public:
				 BIC_CALC_SHA512(void) : BIC_Node() {cgCommand = "sha512";cgReturnCode = BI_RETCODE_CALC_SHA512;};
		virtual ~BIC_CALC_SHA512(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"SHA512");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"<filename|input>","File name or input string.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_CALC_BASE64 : public BIC_Node{
	public:
				 BIC_CALC_BASE64(void) : BIC_Node() {cgCommand = "base64";cgReturnCode = BI_RETCODE_CALC_BAS64;};
		virtual ~BIC_CALC_BASE64(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"BASE64");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"<+|->"			,"Encode/Decode.");
			PrintHelpSubItem(env,"<filename|input>"	,"File name or input string.");
			PrintHelpSubItem(env,"[filename]"		,"Output file name.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_CALC : public BIC_Node_S{
	public:
				 BIC_CALC(void) : BIC_Node_S() {cgblTrySubCMD = 1;cgCommand = "calc";cgTitle = cgCommand;cgReturnCode = BI_RETCODE_CALC;Init();};
	    virtual ~BIC_CALC(void){;};
	public:
	    virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			if (blDetail == 1){
				HelpTraversalChild(env,0);
				return(cgReturnCode);
			}
			PrintHelpItem(env, cgCommand, "-> Calculator.");
			if (blDetail == 0)
				return(cgReturnCode);
			return(cgReturnCode);
		};
	private:
	    void	Init(void){
	        Add(cgSub_hex2dec) < cgSub_dec2hex
		#if (defined USE_OPENSSL) || (defined CommonDefH_MAC)
			< cgSub_md5 < cgSub_sha1 < cgSub_sha224 < cgSub_sha256 < cgSub_sha384 < cgSub_sha512
		#endif
			 < cgSub_base64;
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
#endif
#endif
