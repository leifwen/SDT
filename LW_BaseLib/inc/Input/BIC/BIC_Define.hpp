//
//  BIC_Define.hpp
//  SDT
//
//  Created by Leif Wen on 11/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#ifndef BIC_Define_hpp
#define BIC_Define_hpp
//------------------------------------------------------------------------------------------//
#include "BIC_Define.h"
#ifdef BIC_Define_h
#include "ADS_Buffer.h"
#include "Console.h"
#include "SList.h"
#include "GList.h"
#include "ODEV_System.h"
//------------------------------------------------------------------------------------------//
inline void BIC_ENV::InitQuantity(CMD_ENV* env){
	if (env != nullptr)
		env->InitQuantity(CMD_VID_NEXT);
};
//------------------------------------------------------------------------------------------//
inline void BIC_ENV::Init(CMD_ENV* env)		{;};
inline void BIC_ENV::DeInit(CMD_ENV* env)	{;};
//------------------------------------------------------------------------------------------//
inline SBUF* BIC_ENV::GetSTDIN(CMD_ENV* env){
#ifdef Console_h
	return(CMD_ENV::GetVar(env,CMD_VID_STDIN,(SBUF*)nullptr));
#else
	return(nullptr);
#endif
};
//------------------------------------------------------------------------------------------//
inline ODEV_CACHE*	BIC_ENV::GetCache(CMD_ENV* env){
#ifdef ODEV_System_h
	return(CMD_ENV::GetVar(env,CMD_VID_CACHE,(ODEV_CACHE*)nullptr));
#else
	return(nullptr);
#endif
};
//------------------------------------------------------------------------------------------//
inline CONSOLE*	BIC_ENV::GetConsole(CMD_ENV* env){
#ifdef Console_h
	return(CMD_ENV::GetVar(env,CMD_VID_CONSOLE,(CONSOLE*)nullptr));
#else
	return(nullptr);
#endif
};
//------------------------------------------------------------------------------------------//
inline SC_LIST*	BIC_ENV::GetSCList(CMD_ENV* env){
#ifdef SList_h
	return(CMD_ENV::GetVar(env,CMD_VID_SCLIST,(SC_LIST*)nullptr));
#else
	return(nullptr);
#endif
};
//------------------------------------------------------------------------------------------//
inline GC_LIST*	BIC_ENV::GetGCList(CMD_ENV* env){
#ifdef GList_h
	return(CMD_ENV::GetVar(env,CMD_VID_GCLIST,(GC_LIST*)nullptr));
#else
	return(nullptr);
#endif
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
inline CMDID BIC_BASE::ResolveID(STDSTR** retMsg,const STDSTR& rawIn)const{
	return(ResolveIDCMD(retMsg,rawIn));
};
//------------------------------------------------------------------------------------------//
template<typename... Args> void BIC_BASE::PrintHelpItem(CMD_ENV* env,const STDSTR& command,const Args&... args){
	STDSTR		space;
	space = "";
	if (command.length() < 22)
		space.insert(space.length(),22 - command.length(),' ');
	
	PrintALineDot(env,COL_clBlue,command,space,COL_clDefault,args...);
};
//------------------------------------------------------------------------------------------//
template<typename... Args> void BIC_BASE::PrintHelpSubItem(CMD_ENV* env,const STDSTR& command,const Args&... args){
	STDSTR		space;
	space = "";
	if (command.length() < 17)
		space.insert(space.length(),17 - command.length(),' ');
	
	PrintALineDot(env,COL_clBlue,"    ",command,space,COL_clDefault,args...);
}
//------------------------------------------------------------------------------------------//
template<typename... Args> inline void BIC_BASE::PrintFail(CMD_ENV* env,const Args&... args){
	
	PrintALineDot(env,COL_Fail,"[BIC] Execute fail. Due to",args...);
}
//------------------------------------------------------------------------------------------//
inline void BIC_BASE::PrintFail(CMD_ENV* env){
	PrintALineDot(env,COL_Fail,"[BIC] Execute fail");
}
//------------------------------------------------------------------------------------------//
template<typename... Args> inline void BIC_BASE::PrintSuccess(CMD_ENV* env,const Args&... args){
	
	PrintALineDot(env,COL_Sucess,"[BIC] Execute success.",args...);
}
//------------------------------------------------------------------------------------------//
inline void BIC_BASE::PrintSuccess(CMD_ENV* env){
	
	PrintALineDot(env,COL_Sucess,"[BIC] Execute success");
}
//------------------------------------------------------------------------------------------//
template<typename... Args> void BIC_BASE::PrintResult(CMD_ENV* env,const Args&... args){
	
	PrintALineDot(env,COL_Result,"[BIC]",args...);
}
//------------------------------------------------------------------------------------------//
inline void	BIC_BASE::SetInPressAnyKeyMode(CMD_ENV* env){
#ifdef Console_h
	if (BIC_ENV::GetConsole(env) != nullptr)
		BIC_ENV::GetConsole(env)->SetInPressAnyKeyMode();
#endif
};
//------------------------------------------------------------------------------------------//
inline void	BIC_BASE::ClrInPressAnyKeyMode(CMD_ENV* env){
#ifdef Console_h
	if (BIC_ENV::GetConsole(env) != nullptr)
		BIC_ENV::GetConsole(env)->ClrInPressAnyKeyMode();
#endif
};
//------------------------------------------------------------------------------------------//
inline void	BIC_BASE::SetInOnlineMode(CMD_ENV* env){
#ifdef Console_h
	if (BIC_ENV::GetConsole(env) != nullptr)
		BIC_ENV::GetConsole(env)->SetInOnlineMode();
#endif
};
//------------------------------------------------------------------------------------------//
inline void	BIC_BASE::ClrInOnlineMode(CMD_ENV* env){
#ifdef Console_h
	if (BIC_ENV::GetConsole(env) != nullptr)
		BIC_ENV::GetConsole(env)->ClrInOnlineMode();
#endif
};
//------------------------------------------------------------------------------------------//
inline void	BIC_BASE::SetblDisplayAuto(CMD_ENV* env){
#ifdef Console_h
	if (BIC_ENV::GetConsole(env) != nullptr)
		BIC_ENV::GetConsole(env)->SetblDisplayAuto();
#endif
};
//------------------------------------------------------------------------------------------//
inline void	BIC_BASE::ClrblDisplayAuto(CMD_ENV* env){
#ifdef Console_h
	if (BIC_ENV::GetConsole(env) != nullptr)
		BIC_ENV::GetConsole(env)->ClrblDisplayAuto();
#endif
};
//------------------------------------------------------------------------------------------//
inline bool32 BIC_BASE::IsDisplayAuto(CMD_ENV* env){
#ifdef Console_h
	if (BIC_ENV::GetConsole(env) != nullptr)
		return(BIC_ENV::GetConsole(env)->IsDisplayAuto());
#endif
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
inline void BIC_BASE::SwitchToMain(CMD_ENV* env){
#ifdef Console_h
	if (BIC_ENV::GetConsole(env) != nullptr)
		return(BIC_ENV::GetConsole(env)->SwitchToMain());
#endif
};
//------------------------------------------------------------------------------------------//
inline void BIC_BASE::SwitchToAux(CMD_ENV* env){
#ifdef Console_h
	if (BIC_ENV::GetConsole(env) != nullptr)
		return(BIC_ENV::GetConsole(env)->SwitchToAux());
#endif
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
inline void BIC_BASE_S::SetFatherName(const STDSTR& strName){
	fatherName = strName;
	TREE_LChildRChain_Traversal_LINE_nolock(CMD_NODE,this,
		_opNode->SetFatherName(fatherName +  ((fatherName.length() > 0) ? "/" : "") + Str_UpperCase((cgConsoleName.length() > 0)?cgConsoleName:cgCommand))
	);
};
//------------------------------------------------------------------------------------------//
#endif /* BIC_Define_h */
#endif /* BIC_Define_hpp */
