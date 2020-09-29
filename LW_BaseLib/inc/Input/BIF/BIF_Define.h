//
//  BIF_Define.h
//  SDT
//
//  Created by Leif Wen on 09/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#include "CMD.h"
#ifdef CMD_h
//------------------------------------------------------------------------------------------//
#ifndef BIF_Define_h
#define BIF_Define_h
#ifdef BIF_Define_h
//------------------------------------------------------------------------------------------//
enum{
	BIF_ID_EXIT 		= CMD_NODE::CMD_ID_EXIT,
	BIF_ID_NO			= CMD_NODE::CMD_ID_NO,
	BIF_ID_HELP			= CMD_NODE::CMD_ID_HELP,
	
	BIF_ID_BIC			= CMD_NODE::CMD_ID_NEXT,
	
	BIF_ID_COMBINE,
	BIF_ID_COMBINE_SQ,
	BIF_ID_COMBINE_HEX,
	BIF_ID_COMBINE_VIEWINHEX,
	BIF_ID_COMBINE_TIME,
	BIF_ID_COMBINE_STRING,
	BIF_ID_COMBINE_FILE,
	
	BIF_ID_COMBINE_DEFINE,
	BIF_ID_COMBINE_REPLACE,
	
	BIF_ID_COMBINE_CALC_MD5,
	BIF_ID_COMBINE_CALC_SHA1,
	BIF_ID_COMBINE_CALC_SHA224,
	BIF_ID_COMBINE_CALC_SHA256,
	BIF_ID_COMBINE_CALC_SHA384,
	BIF_ID_COMBINE_CALC_SHA512,
	BIF_ID_COMBINE_CALC_BASE64,
	BIF_ID_COMBINE_CALC_BASE64_FILE,
	
	BIF_ID_COMBINE_PRINT,
	
	BIF_ID_COMBINE_RET,
	BIF_ID_SEARCH,
	
	BIF_ID_RECEIVE,
	BIF_ID_TIMEOUT,
	BIF_ID_NULLPTR,
	BIF_ID_RX_MORE,
	BIF_ID_RX_LESS,
	BIF_ID_RX_EQUAL,
	BIF_ID_LCS,
	BIF_ID_CONDITION,
	BIF_ID_EXPRESSION,
	
	BIF_ID_DELAY,
	BIF_ID_WAIT,
	BIF_ID_SYNCHRONOUS,
	
	BIF_ID_LABLE,
	BIF_ID_GOTO,
	BIF_ID_STOP,
	BIF_ID_BREAK,

	BIF_ID_OPEN,
	BIF_ID_CLOSE,
	BIF_ID_BR,
	BIF_ID_DTR,
	BIF_ID_RTS,
	BIF_ID_CTS,
	BIF_ID_DSR,
	BIF_ID_RING,
	BIF_ID_DCD,

	BIF_ID_SETRECVDATAMODE,
	BIF_ID_SETRECMSGREPORT,

	BIF_ID_RUNEXE,
	BIF_ID_STOPEXE,
	BIF_ID_GETEXE,
	BIF_ID_WAITEXE,
	
	BIF_ID_CMD,
};
class COMMAND_NODE;
struct SYS_TIME_S;
//------------------------------------------------------------------------------------------//
namespace BIF_ENV{
	enum	{CMD_VID_STDIN  = CMD_ENV::CMD_VID_NEXT
			,CMD_VID_retDefine
			,CMD_VID_retSearch
			,CMD_VID_retFun
			,CMD_VID_retDTime
			,CMD_VID_retCMD
			,CMD_VID_commandNode
			,CMD_VID_arrayIn
			,CMD_VID_NEXT
	};
	enum	{RFLAG_C = 2, RFLAG_S = CMD_ENV::RFLAG_S + CMD_ENV::RFLAG_C};
	enum	{ENV_blPrintBIFInfo		= RFLAG_CREATE(0)
			,ENV_blPrintBIFExplain	= RFLAG_CREATE(1)};
	
	static	inline	void		InitQuantity	(CMD_ENV* env);
	static	inline	void		Init			(CMD_ENV* env);
	static	inline	void		DeInit			(CMD_ENV* env);

	CMD_VAR		(STDIN,			CMD_VID_STDIN,		STDSTR);
	CMD_VAR		(RetDefine,		CMD_VID_retDefine,	STDSTR);//used for CMD define & replace
	CMD_VAR		(RetSearch,		CMD_VID_retSearch,	STDSTR);//used for CMD search & ret
	CMD_VAR		(RetFun,		CMD_VID_retFun,		STDSTR);//T or F
	CMD_VAR		(RetCMD,		CMD_VID_retCMD,		bool32);//used for CMD stop & break
	CMD_VAR		(RetDTime,		CMD_VID_retDTime,	SYS_TIME_S);//used for CMD timeout
	CMD_SETVAR	(SetCommandNode,CMD_VID_commandNode,COMMAND_NODE*);
	CMD_SETVAR	(SetArrayIn,	CMD_VID_arrayIn,	ARRAY*);//used for CMD wait
	
	static	inline	COMMAND_NODE*	GetCommandNode	(CMD_ENV* env);
	static	inline	ARRAY*			GetArrayIn		(CMD_ENV* env);
};
//------------------------------------------------------------------------------------------//
class BIF_BASE : public CMD_NODE{
	public:
				 BIF_BASE(void) : CMD_NODE(){;};
		virtual ~BIF_BASE(void){;};
	public:
		inline	virtual	CMDID	ResolveID	(STDSTR** retMsg,const STDSTR& rawIn)const;
	public:
									static	inline	void	PrintB			(CMD_ENV* env,const STDSTR& str);
									static	inline	void	PrintP			(CMD_ENV* env,const STDSTR& str);
									static	inline	void	PrintM			(CMD_ENV* env,const STDSTR& strM,const STDSTR& strB = "");
		template<typename... Args>	static	inline	void 	PrintExecute	(CMD_ENV* env,const Args&... args);
};
//------------------------------------------------------------------------------------------//
#define BIF_CLASSTYPE(_NAME,_CMD)	CMD_CLASSTYPE(BIF_##_NAME,BIF_BASE,BIF_ID_##_NAME,_CMD);
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#include "BIF_Define.hpp"
#endif /* BIF_Define_h */
#endif /* BIF_Define_h */
#endif
