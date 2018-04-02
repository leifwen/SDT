//
//  BIF_Define.hpp
//  SDT
//
//  Created by Leif Wen on 11/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#ifndef BIF_Define_hpp
#define BIF_Define_hpp
//------------------------------------------------------------------------------------------//
#include "BIF_Define.h"
#ifdef BIF_Define_h
#include "GList.h"
#include "SYS_Time.h"
//------------------------------------------------------------------------------------------//
inline void BIF_ENV::InitQuantity(CMD_ENV* env){
	if (env != nullptr)
		env->InitQuantity(CMD_VID_NEXT);
};
//------------------------------------------------------------------------------------------//
inline void BIF_ENV::Init(CMD_ENV* env){
	if (env != nullptr){
		CMD_ENV::NewVar(env,CMD_VID_STDIN,(STDSTR*)nullptr);
		CMD_ENV::NewVar(env,CMD_VID_retDefine,(STDSTR*)nullptr);
		CMD_ENV::NewVar(env,CMD_VID_retSearch,(STDSTR*)nullptr);
		CMD_ENV::NewVar(env,CMD_VID_retFun,(STDSTR*)nullptr);
		CMD_ENV::NewVar(env,CMD_VID_retCMD,(bool32*)nullptr);
		CMD_ENV::NewVar(env,CMD_VID_retDTime,(SYS_TIME_S*)nullptr);
		STDIN(env) = "";
		RetDefine(env) = "";
		RetSearch(env) = "";
		RetFun(env) = "";
		RetCMD(env) = 0;
		SYS_Delay_SetTS(&RetDTime(env),1000 * 60 * 60 * 24);
	}
};
//------------------------------------------------------------------------------------------//
inline void BIF_ENV::DeInit(CMD_ENV* env){
	CMD_ENV::DelVar(env,CMD_VID_STDIN,(STDSTR*)nullptr);
	CMD_ENV::DelVar(env,CMD_VID_retDefine,(STDSTR*)nullptr);
	CMD_ENV::DelVar(env,CMD_VID_retSearch,(STDSTR*)nullptr);
	CMD_ENV::DelVar(env,CMD_VID_retFun,(STDSTR*)nullptr);
	CMD_ENV::DelVar(env,CMD_VID_retCMD,(bool32*)nullptr);
	CMD_ENV::DelVar(env,CMD_VID_retDTime,(SYS_TIME_S*)nullptr);
};
//------------------------------------------------------------------------------------------//
inline COMMAND_NODE* BIF_ENV::GetCommandNode(CMD_ENV* env){
#ifdef GList_h
	return(CMD_ENV::GetVar(env,CMD_VID_commandNode,(COMMAND_NODE*)nullptr));
#else
	return(nullptr);
#endif
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
CMDID BIF_BASE::ResolveID(STDSTR** retMsg,const STDSTR& rawIn)const{
	return(ResolveIDFun(retMsg,rawIn));
};
//------------------------------------------------------------------------------------------//
inline void BIF_BASE::PrintB(CMD_ENV* env,const STDSTR& str){
	PrintALine(env,COLOR(COL_clBlue,str));
};
//------------------------------------------------------------------------------------------//
inline void BIF_BASE::PrintP(CMD_ENV* env,const STDSTR& str){
	PrintALine(env,COLOR(COL_clPurple,str));
};
//------------------------------------------------------------------------------------------//
inline void BIF_BASE::PrintM(CMD_ENV* env,const STDSTR& strM,const STDSTR& strB){
	PrintALine(env,COLOR(COL_clMaroon,strM),COLOR(COL_clBlue,strB));
};
//------------------------------------------------------------------------------------------//
template<typename... Args> inline void BIF_BASE::PrintExecute(CMD_ENV* env,const Args&... args){
	PrintWithDividingLine(env,"Execute:",args...);
};
//------------------------------------------------------------------------------------------//
#endif /* BIF_Define_h */
#endif /* BIF_Define_hpp */
