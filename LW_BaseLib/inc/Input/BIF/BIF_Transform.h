//
//  BIF_Transform.h
//  SDT
//
//  Created by Leif Wen on 09/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "BIF_Define.h"
//------------------------------------------------------------------------------------------//
#ifdef BIF_Define_h
//------------------------------------------------------------------------------------------//
#ifndef BIF_Transform_h
#define BIF_Transform_h
#ifdef BIF_Transform_h
#include "ALG_Digest.h"
#include "ALG_BASE64.h"
#include "ALG_zlib.h"
//------------------------------------------------------------------------------------------//
struct BIF_RETSTR{
	STDSTR	forPrint;
	STDSTR	forSend;
	STDSTR	result;
};
//------------------------------------------------------------------------------------------//
class BIF_COMBINE_BASE : public BIF_BASE{
	public:
				 BIF_COMBINE_BASE(void) : BIF_BASE() {;};
		virtual ~BIF_COMBINE_BASE(void){;};
	public:
		virtual	CMDID Execute(CMD_ENV* env,const uint32& mID,const STDSTR& msg,void* bif_retstr)const;
};
#define BIF_COMBINETYPE(_NAME,_CMD)	CMD_CLASSTYPE(BIF_##_NAME,BIF_COMBINE_BASE,BIF_ID_COMBINE_##_NAME,_CMD);
//------------------------------------------------------------------------------------------//
class BIF_SQ : public BIF_BASE{
	public:
				 BIF_SQ(void) : BIF_BASE() {cgCommandID = BIF_ID_COMBINE_SQ;cgCommand = "'";};
		virtual ~BIF_SQ(void){;};
	public:
		virtual	CMDID	ResolveID	(STDSTR** retMsg,const STDSTR& rawIn)const;
		virtual	CMDID	Command		(CMD_ENV* env,const STDSTR& msg,void* bif_retstr)const;
		virtual	CMDID	Help		(CMD_ENV* env,uint32 flag = 0)const;
};
//------------------------------------------------------------------------------------------//
BIF_COMBINETYPE(HEX,		"hex,:/hex,(,)");
BIF_COMBINETYPE(VIEWINHEX,	"viewhex,:");
BIF_COMBINETYPE(TIME,		"time,(,)/time,=/now,(,)/now,=");
BIF_COMBINETYPE(STRING,		"str,(,)/string,(,)");
BIF_COMBINETYPE(FILE,		"file,(,)/file,=");

BIF_COMBINETYPE(DEFINE,		"def,:/define,:");
BIF_COMBINETYPE(REPLACE,	"rep,:/replace,:");

BIF_CLASSTYPE  (SEARCH,		"search,=");
BIF_COMBINETYPE(RET,		"ret,(,)");
//------------------------------------------------------------------------------------------//
#ifdef ALG_Digest_h
	BIF_COMBINETYPE(CALC_MD5,	"MD5,(,)/MD5,=");
	BIF_COMBINETYPE(CALC_SHA1,	"SHA1,(,)/SHA1,=");
	BIF_COMBINETYPE(CALC_SHA224,"SHA224,(,)/SHA224,=");
	BIF_COMBINETYPE(CALC_SHA256,"SHA256,(,)/SHA256,=");
	BIF_COMBINETYPE(CALC_SHA384,"SHA384,(,)/SHA384,=");
	BIF_COMBINETYPE(CALC_SHA512,"SHA512,(,)/SHA512,=");
#endif
#ifdef ALG_BASE64_h
	BIF_COMBINETYPE(CALC_BASE64,"BASE64,(,)/BASE64,=");
#endif
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
class BIF_COMBINE : public BIF_BASE{
	public:
				 BIF_COMBINE(void);
		virtual ~BIF_COMBINE(void){;};
	public:
		virtual	CMDID			Command		(CMD_ENV* env,const STDSTR& msg,void* bif_retstr)const;
		virtual	CMDID			Help		(CMD_ENV* env,uint32 flag = 0)const;
		static	BIF_COMBINE&	GetCombine	(void);
	protected:
		BIF_SQ			cgSubC_SQ;
		BIF_HEX			cgSubC_HEX;
		BIF_VIEWINHEX	cgSubC_VIEWINHEX;
		BIF_TIME		cgSubC_Time;
		BIF_STRING		cgSubC_STRING;
	
		BIF_DEFINE		cgSubC_DEFINE;
		BIF_REPLACE		cgSubC_REPLACE;
	
		BIF_RET			cgSubC_RET;
#ifdef ALG_Digest_h
		BIF_CALC_MD5	cgSubC_CALC_MD5;
		BIF_CALC_SHA1	cgSubC_CALC_SHA1;
		BIF_CALC_SHA224	cgSubC_CALC_SHA224;
		BIF_CALC_SHA256	cgSubC_CALC_SHA256;
		BIF_CALC_SHA384	cgSubC_CALC_SHA384;
		BIF_CALC_SHA512	cgSubC_CALC_SHA512;
#endif
#ifdef ALG_BASE64_h
		BIF_CALC_BASE64	cgSubC_CALC_BASE64;
#endif
};
//------------------------------------------------------------------------------------------//
BIF_COMBINETYPE(PRINT,"print,=");
//------------------------------------------------------------------------------------------//
CMDID	BIF_Transform	(CMD_ENV* env,STDSTR* retForSend,STDSTR* retForPrint,const STDSTR& inputCommand,CMD_TAIL tail,G_ESCAPE blEscape);
//------------------------------------------------------------------------------------------//
#endif /* BIF_Transform_h */
#endif /* BIF_Transform_h */
#endif /* BIF_Define_h */
