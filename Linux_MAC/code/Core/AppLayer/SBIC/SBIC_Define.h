//
//  SBIC_Define.h
//  SDT
//
//  Created by Leif Wen on 02/04/2017.
//  Copyright © 2017 leif wen. All rights reserved.
//

#ifndef SBIC_Define_hpp
#define SBIC_Define_hpp
//------------------------------------------------------------------------------------------//
#include "BasicDef.h"
#include "SYS_Time.h"
//------------------------------------------------------------------------------------------//
enum{
	SBI_RETCODE_EXIT 	= -1,
	SBI_RETCODE_NO		= 0,
	SBI_RETCODE_HELP,
	SBI_RETCODE_EXPRESSION,
	
	SBI_RETCODE_COMBINE,
	SBI_RETCODE_COMBINE_VIEWHEX,
	SBI_RETCODE_COMBINE_RET,
	SBI_RETCODE_COMBINE_SQ,
	SBI_RETCODE_COMBINE_HEX,
	SBI_RETCODE_COMBINE_TIME,
	SBI_RETCODE_COMBINE_STRING,
	
	SBI_RETCODE_COMBINE_CALC_MD5,
	SBI_RETCODE_COMBINE_CALC_SHA1,
	SBI_RETCODE_COMBINE_CALC_SHA224,
	SBI_RETCODE_COMBINE_CALC_SHA256,
	SBI_RETCODE_COMBINE_CALC_SHA384,
	SBI_RETCODE_COMBINE_CALC_SHA512,
	SBI_RETCODE_COMBINE_CALC_BASE64,
	SBI_RETCODE_COMBINE_CALC_BASE64_FILE,
	
	SBI_RETCODE_PRINT,
	SBI_RETCODE_SENDFILE,
	
	SBI_RETCODE_DELAY,
	SBI_RETCODE_WAIT,
	SBI_RETCODE_DTR,
	SBI_RETCODE_RTS,
	SBI_RETCODE_OPENCOM,
	SBI_RETCODE_CHANGEBR,
	SBI_RETCODE_OPENTCPSOCKET,
	SBI_RETCODE_CLOSETCPSOCKET,
	SBI_RETCODE_OPENUDPSOCKET,
	SBI_RETCODE_CLOSEUDPSOCKET,
	SBI_RETCODE_SETRECVDATAMODE,
	SBI_RETCODE_SETRECMSGREPORT,
	SBI_RETCODE_SYNCHRONOUS,
	
	SBI_RETCODE_RUNEXE,
	SBI_RETCODE_STOPEXE,
	SBI_RETCODE_LABLE,
	SBI_RETCODE_GOTO,
	SBI_RETCODE_STOP,
	SBI_RETCODE_BREAK,
	SBI_RETCODE_SEARCH,
	
	SBI_RETCODE_RECEIVE,
	SBI_RETCODE_CTS,
	SBI_RETCODE_DSR,
	SBI_RETCODE_RING,
	SBI_RETCODE_DCD,
	SBI_RETCODE_TIMEOUT,
	SBI_RETCODE_nullptr,
	SBI_RETCODE_RX_B,
	SBI_RETCODE_RX_S,
	SBI_RETCODE_RX_E,
	SBI_RETCODE_LCS,
	SBI_RETCODE_CONDITION,
};
//------------------------------------------------------------------------------------------//
class SBUFFER;
class OUTPUT_NODE;
class DEVICE;
class ExpandDeviceAttr;
class COMMAND_NODE;
class AEXEPOOL;
//------------------------------------------------------------------------------------------//
struct SBIC_ENV{
	SBUFFER		*cstdin;
	STDSTR		cstdin_Str;
	OUTPUT_NODE *cstdout;
	int32		blExit;
	//-----------------------//
	ExpandDeviceAttr	*eda;
	//-----------------------//
	STDSTR			cgFunRet;
	STDSTR			cgSearchRet;
	
	int32			blCMDRet;
	COMMAND_NODE	*cgCommand;
	SYS_TIME_S		cgTimeS;
	AEXEPOOL		*cgAExePool;
	uint32			cfg;
};
//------------------------------------------------------------------------------------------//
		void SetDefAttr(SBIC_ENV *env,const DEVICE* device,SBUFFER *cstdin,OUTPUT_NODE *cstdout = nullptr);
inline	void SetDefAttr(SBIC_ENV *env){
	env->cstdin = nullptr;
	env->cstdin_Str = "";
	env->cstdout = nullptr;
	env->blExit = 0;
	env->eda = nullptr;
	
	env->cgFunRet = "";
	env->cgSearchRet = "";
	
	env->blCMDRet = 0;
	env->cgCommand = nullptr;
	SYS_Delay_SetTS(&env->cgTimeS,1000*60*60*24);
	env->cgAExePool = nullptr;
};
//------------------------------------------------------------------------------------------//
#endif /* SBIC_Define_h */
