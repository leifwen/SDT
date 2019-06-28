//
//  SYS_Thread.hpp
//  SDT
//
//  Created by Leif Wen on 23/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#ifndef SYS_Thread_hpp
#define SYS_Thread_hpp
//------------------------------------------------------------------------------------------//
#include "SYS_Thread.h"
#ifdef SYS_Thread_h
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_VC
inline unsigned __stdcall SYS_AThread::CallExecute_VC(LPVOID par){
	CallExecute(par);
	return(1);
};
#endif
//------------------------------------------------------------------------------------------//
inline bool32 SYS_AThread::IsTerminated(void)const{
	return(CheckSFlag(SYS_blIsTerminated));
};
//------------------------------------------------------------------------------------------//
template <typename T_CLASS> SYS_Thread<T_CLASS>::SYS_Thread(void) : SYS_AThread(){
	cgFunClass = nullptr;
	cgExeFun = nullptr;
	cgOnStopFun = nullptr;
};
//------------------------------------------------------------------------------------------//
template <typename T_CLASS> void SYS_Thread<T_CLASS>::Execute(void* exep){
	if ((cgFunClass != nullptr) && (cgExeFun != nullptr))
		(cgFunClass->*cgExeFun)(exep);
};
//------------------------------------------------------------------------------------------//
template <typename T_CLASS>
void SYS_Thread<T_CLASS>::ThreadInit(T_CLASS *funClass,T_FUN exefun,const STDSTR &threadName){
	cgFunClass	= funClass;
	cgExeFun	= exefun;
	SetSFlag(SYS_blThreadFinish | SYS_blIsTerminated);
	selfName = threadName;
};
//------------------------------------------------------------------------------------------//
template <typename T_CLASS> void SYS_Thread<T_CLASS>::ThreadInit(T_FUN onstopfun){
	cgOnStopFun = onstopfun;
};
//------------------------------------------------------------------------------------------//
template <typename T_CLASS> void SYS_Thread<T_CLASS>::OnThreadStop(void* exep){
	if ((cgFunClass != nullptr) && (cgOnStopFun != nullptr))
		(cgFunClass->*cgOnStopFun)(exep);
};
//------------------------------------------------------------------------------------------//
#endif /* SYS_Thread_h */
#endif /* SYS_Thread_hpp */
