//
//  CMD.hpp
//  SDT
//
//  Created by Leif Wen on 06/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#ifndef CMD_hpp
#define CMD_hpp
//------------------------------------------------------------------------------------------//
#include "CMD.h"
#ifdef CMD_h
#include "DS_STRING.h"
#include "Output.h"
//------------------------------------------------------------------------------------------//
inline	void	CMD_ENV::ClearEnvAllFlag(void)							{ClearAllFlag();};
inline	void	CMD_ENV::SetEnvFlag		(const uint64& rStatus)			{SetSFlag(rStatus);};
inline	void	CMD_ENV::ClrEnvFlag		(const uint64& rStatus)			{ClrSFlag(rStatus);};
inline	bool32	CMD_ENV::CheckEnvFlag	(const uint64& rStatus)const 	{return(CheckSFlag(rStatus));};
//------------------------------------------------------------------------------------------//
inline	void	CMD_ENV::SetblExitForce	(void)		{SetSFlag(ENV_blExit | ENV_blExitLock);};
inline	void	CMD_ENV::SetblExit		(void)		{if (CheckSFlag(ENV_blExitLock) == G_FALSE) SetSFlag(ENV_blExit);};
inline	void	CMD_ENV::ClrblExit		(void)		{if (CheckSFlag(ENV_blExitLock) == G_FALSE) ClrSFlag(ENV_blExit);};
inline	bool32	CMD_ENV::ChkblExit		(void)const	{return(CheckSFlag(ENV_blExitLock) || CheckSFlag(ENV_blExit));};
//------------------------------------------------------------------------------------------//
inline uint32 CMD_ENV::GetQuantity(void){
	return(CMD_ENV::CMD_VID_NEXT);
};
//------------------------------------------------------------------------------------------//
inline void CMD_ENV::SetVar(CMD_ENV* env,uint32 vid,void* var){
	if (env != nullptr)
		env->buffer[vid] = var;
};
//------------------------------------------------------------------------------------------//
template<typename VART> inline VART* CMD_ENV::GetVar(CMD_ENV* env,uint32 vid,VART* varnullptr){
	if (env != nullptr)
		//return(static_cast<VART*>(env->buffer[vid]));
		return((VART*)(env->buffer[vid]));
	return(nullptr);
};
//------------------------------------------------------------------------------------------//
template<typename VART>	inline void CMD_ENV::NewVar(CMD_ENV* env,uint32 vid,VART* varnullptr){
	if (env != nullptr)
		SetVar(env,vid,new VART);
};
//------------------------------------------------------------------------------------------//
template<typename VART>	inline void CMD_ENV::DelVar(CMD_ENV* env,uint32 vid,VART* varnullptr){
	if (env != nullptr){
		delete GetVar(env,vid,varnullptr);
		SetVar(env, vid, nullptr);
	}
};
//------------------------------------------------------------------------------------------//
inline void CMD_ENV::SetSTDOUT(CMD_ENV* env,OUTPUT_NODE* var){
	SetVar(env,CMD_VID_STDOUT,var);
};
//------------------------------------------------------------------------------------------//
inline OUTPUT_NODE* CMD_ENV::GetSTDOUT(CMD_ENV* env){
#ifdef Output_h
	return(GetVar(env,CMD_VID_STDOUT,(OUTPUT_NODE*)nullptr));
#else
	return(nullptr);
#endif
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
inline	void	CMD_BASE::ClearEnvAllFlag	(CMD_ENV* env)							{if (env != nullptr) env->ClearEnvAllFlag();};
inline	void	CMD_BASE::SetEnvFlag		(CMD_ENV* env,const uint64& rStatus)	{if (env != nullptr) env->SetEnvFlag(rStatus);};
inline	void	CMD_BASE::ClrEnvFlag		(CMD_ENV* env,const uint64& rStatus)	{if (env != nullptr) env->ClrEnvFlag(rStatus);};
inline	bool32	CMD_BASE::CheckEnvFlag		(CMD_ENV* env,const uint64& rStatus) 	{return(env != nullptr ? env->CheckEnvFlag(rStatus) : G_FALSE);};
//------------------------------------------------------------------------------------------//
inline	void	CMD_BASE::SetblExitForce	(CMD_ENV* env){if (env != nullptr) env->SetblExitForce();};
inline	void	CMD_BASE::SetblExit			(CMD_ENV* env){if (env != nullptr) env->SetblExit();};
inline	void	CMD_BASE::ClrblExit			(CMD_ENV* env){if (env != nullptr) env->ClrblExit();};
inline	bool32	CMD_BASE::ChkblExit			(CMD_ENV* env){return(env != nullptr ? env->ChkblExit() : G_FALSE);};
//------------------------------------------------------------------------------------------//
inline OUTPUT_NODE* CMD_BASE::GetSTDOUT(CMD_ENV* env){
	return(CMD_ENV::GetSTDOUT(env));
};
//------------------------------------------------------------------------------------------//
inline void	CMD_BASE::SplitPar1(STDSTR& strPar1,STDSTR& strPar2,const STDSTR& par,const STDSTR& split){
	strPar2 = Str_LTrim(par);
	strPar1 = Str_ReadSubItem(&strPar2,split);
	Str_LTrimSelf(strPar2);
};
//------------------------------------------------------------------------------------------//
inline void	CMD_BASE::SplitPar1(STDSTR* strPar1,STDSTR* strPar2,const STDSTR& split){
	*strPar1 = Str_ReadSubItem(strPar2,split);
	Str_LTrimSelf(*strPar2);
};
//------------------------------------------------------------------------------------------//
template<typename... Args> void CMD_BASE::PrintStr(CMD_ENV* env,const Args&... args){
#ifdef Output_h
	OUTPUT_NODE::PrintStr(GetSTDOUT(env),args...);
#endif
};
//------------------------------------------------------------------------------------------//
template<typename... Args> void CMD_BASE::PrintStrNL(CMD_ENV* env,const Args&... args){
#ifdef Output_h
	OUTPUT_NODE::PrintStrNL(GetSTDOUT(env),args...);
#endif
};
//------------------------------------------------------------------------------------------//
template<typename... Args> void CMD_BASE::PrintALine(CMD_ENV* env,const Args&... args){
#ifdef Output_h
	OUTPUT_NODE::PrintALine(GetSTDOUT(env),args...);
#endif
};
//------------------------------------------------------------------------------------------//
template<typename... Args> void CMD_BASE::PrintALineDot(CMD_ENV* env,const Args&... args){
#ifdef Output_h
	OUTPUT_NODE::PrintALineDot(GetSTDOUT(env),args...);
#endif
};
template<typename... Args> void CMD_BASE::PrintWithTime(CMD_ENV* env,const Args&... args){
#ifdef Output_h
	OUTPUT_NODE::PrintWithTime(GetSTDOUT(env),args...);
#endif
};
//------------------------------------------------------------------------------------------//
template<typename... Args> void CMD_BASE::PrintWithDividingLine(CMD_ENV* env,const Args&... args){
#ifdef Output_h
	OUTPUT_NODE::PrintWithDividingLine(CMD_ENV::GetSTDOUT(env),args...);
#endif
};
//------------------------------------------------------------------------------------------//
inline void CMD_BASE::PrintEnable(CMD_ENV* env){
#ifdef Output_h
	if (GetSTDOUT(env) != nullptr)
		GetSTDOUT(env)->PrintEnable();
#endif
};
//------------------------------------------------------------------------------------------//
inline void CMD_BASE::PrintDisable(CMD_ENV* env){
#ifdef Output_h
	if (GetSTDOUT(env) != nullptr)
		GetSTDOUT(env)->PrintDisable();
#endif
};
//------------------------------------------------------------------------------------------//
inline bool32 CMD_BASE::IsPrintEnable(CMD_ENV* env){
#ifdef Output_h
	if (GetSTDOUT(env) != nullptr)
		return(GetSTDOUT(env)->CheckPrintDisable() == G_FALSE);
#endif
	return(G_FALSE);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
template <typename T_FUN_CLASS> CMD_NODE_T<T_FUN_CLASS>::CMD_NODE_T(void) : CMD_NODE(){
	cgFunClass = nullptr;
	cgFun = nullptr;
};
//------------------------------------------------------------------------------------------//
template <typename T_FUN_CLASS> void CMD_NODE_T<T_FUN_CLASS>::InitFun(T_FUN_CLASS *funClass,T_FUN fun){
	cgFunClass = funClass;
	cgFun = fun;
};
//------------------------------------------------------------------------------------------//
template <typename T_FUN_CLASS>
CMDID CMD_NODE_T<T_FUN_CLASS>::MessageProcessing(CMD_ENV* env,const uint32& mID,const STDSTR& msg,void* p)const{
	
	if ((cgFunClass != nullptr ) && (cgFun != nullptr))
		return((cgFunClass->*cgFun)(env,mID,msg,p));
	return(Execute(env,mID,msg,p));
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif /* CMD_h */
#endif /* CMD_hpp */
