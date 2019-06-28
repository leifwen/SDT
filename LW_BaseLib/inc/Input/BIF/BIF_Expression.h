//
//  BIF_Expression.h
//  SDT
//
//  Created by Leif Wen on 09/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#include "BIF_Define.h"
//------------------------------------------------------------------------------------------//
#ifdef BIF_Define_h
//------------------------------------------------------------------------------------------//
#include "ADS_Buffer.h"
//------------------------------------------------------------------------------------------//
#ifndef BIF_Expression_h
#define BIF_Expression_h
#ifdef BIF_Expression_h
//------------------------------------------------------------------------------------------//
BIF_CLASSTYPE(RECEIVE,	"receive,==");
BIF_CLASSTYPE(TIMEOUT,	"timeout,==");
BIF_CLASSTYPE(NULLPTR,	"nullptr,==");
BIF_CLASSTYPE(RX_MORE,	"rx,>");
BIF_CLASSTYPE(RX_LESS,	"rx,<");
BIF_CLASSTYPE(RX_EQUAL,	"rx,==");
BIF_CLASSTYPE(LCS,		"lcs,==");
//------------------------------------------------------------------------------------------//
class BIF_CONDITION : public BIF_BASE{
	public:
				 BIF_CONDITION(void);
		virtual ~BIF_CONDITION(void){;};
	public:
		virtual	CMDID	Dispose		(CMD_ENV* env,const STDSTR& rawIn,void* retTrue)const;
		virtual	CMDID	Command		(CMD_ENV* env,const STDSTR& msg,void* retTrue)const;
		virtual	CMDID	Help		(CMD_ENV* env,uint32 flag = 0)const;
	protected:
		BIF_RECEIVE		cgSubC_RECEIVE;
		BIF_TIMEOUT		cgSubC_TIMEOUT;
		BIF_NULLPTR		cgSubC_NULLPTR;
		BIF_RX_MORE		cgSubC_RX_MORE;
		BIF_RX_LESS		cgSubC_RX_LESS;
		BIF_RX_EQUAL	cgSubC_RX_EQUAL;
		BIF_LCS			cgSubC_LCS;
	public:
		static	BIF_CONDITION& 	GetCondition	(void);
};
//------------------------------------------------------------------------------------------//
bool32 BIF_CheckResult		(ARRAY* _in,const STDSTR& expressions,int32 timeoutMS);
bool32 BIF_Expression		(CMD_ENV* env,const STDSTR& expressions);
//------------------------------------------------------------------------------------------//
#endif /* BIF_Expression_h */
#endif /* BIF_Expression_h */
#endif /* CMD_h */
#ifndef BIF_Expression_h
#define BIF_CheckResult(x,y,z)	0
#define BIF_Expression(x,y)		0
#endif
