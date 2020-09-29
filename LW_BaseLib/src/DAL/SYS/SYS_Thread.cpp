//
//  SYS_Thread.cpp
//  SDT
//
//  Created by Leif Wen on 23/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "SYS_Thread.h"
#include "SYS_Time.h"
//------------------------------------------------------------------------------------------//
#ifdef SYS_Thread_h
//------------------------------------------------------------------------------------------//
SYS_AThread::SYS_AThread(void) : TNFP(){
	Enable(this);
	SetSFlag(SYS_blIsTerminated | SYS_blThreadFinish);
	cgExecutePar = nullptr;
	cgThreadFunc = nullptr;
};
//------------------------------------------------------------------------------------------//
SYS_AThread::~SYS_AThread(void){
	
	ThreadStop();
};
//------------------------------------------------------------------------------------------//
void* SYS_AThread::CallExecute(void* p){
	SYS_AThread	*cSelf = (SYS_AThread*)p;
	cSelf->ClrSFlag(SYS_blThreadFinish | SYS_blIsTerminated);
	cSelf->SetSFlag(SYS_blThreadStartup);
	cSelf->Execute(cSelf->cgExecutePar);
	cSelf->SetSFlag(SYS_blThreadFinish | SYS_blIsTerminated);
	return(nullptr);
};
//------------------------------------------------------------------------------------------//
void SYS_AThread::ThreadRun(void* exep){
	if ((CheckSFlag(SYS_blThreadCreated) == G_FALSE) && IsEnable(this)){
		if (exep != nullptr)
			cgExecutePar = exep;
		if (CheckSFlag(SYS_blThreadFinish) == G_FALSE)
			return;
		ClrSFlag(SYS_blThreadStartup);
#ifdef CommonDefH_Unix
		int32 ret;
		ret = pthread_create(&cgThreadHandle, nullptr, CallExecute, this);
		if (ret == -1){
			SetSFlag(SYS_blIsTerminated);
			ClrSFlag(SYS_blThreadCreated);
		}
		else{
			SetSFlag(SYS_blThreadCreated);
			while(CheckSFlag(SYS_blThreadStartup) == G_FALSE);
		}
#endif
#ifdef CommonDefH_VC
		cgThreadHandle = (HANDLE)_beginthreadex(nullptr, 0, CallExecute_VC, this, 0, nullptr);
		if (cgThreadHandle == INVALID_HANDLE_VALUE){
			SetSFlag(SYS_blIsTerminated);
			ClrSFlag(SYS_blThreadCreated);
		}
		else{
			SetSFlag(SYS_blThreadCreated);
			while(CheckSFlag(SYS_blThreadStartup) == G_FALSE);
		}
#endif
	}
};
//------------------------------------------------------------------------------------------//
void SYS_AThread::ThreadRun(ThreadFunc _func,void* exep){
	cgThreadFunc = _func;
	ThreadRun(exep);
}
//------------------------------------------------------------------------------------------//
void SYS_AThread::ThreadStop(void* exep){
	OnThreadStop(exep);
	SetSFlag(SYS_blIsTerminated);
	if (CheckSFlag(SYS_blThreadCreated)){
		while(CheckSFlag(SYS_blThreadFinish) == G_FALSE)
			SetSFlag(SYS_blIsTerminated);
#ifdef CommonDefH_Unix
		void *t;
		pthread_join(cgThreadHandle, &t);
#endif
#ifdef CommonDefH_VC
		if (cgThreadHandle != INVALID_HANDLE_VALUE){
			WaitForSingleObject(cgThreadHandle,INFINITE);
			try{
				CloseHandle(cgThreadHandle);
			}
			catch(...){};
			cgThreadHandle = INVALID_HANDLE_VALUE;
		}
#endif
		ClrSFlag(SYS_blThreadCreated);
	}
};
//------------------------------------------------------------------------------------------//
void SYS_AThread::Execute(void* exep){
	if (cgThreadFunc != nullptr)
		(*cgThreadFunc)(exep);
};

//------------------------------------------------------------------------------------------//
void SYS_Thread_List::ThreadRun		(void* exep){TREE_DownChain_Traversal(SYS_AThread,ThreadRun(exep));};
void SYS_Thread_List::ThreadStop	(void)		{TREE_DownChain_Traversal(SYS_AThread,ThreadStop());};
void SYS_Thread_List::EnableAll		(void)		{TREE_DownChain_Traversal(SYS_AThread,Enable(_opNode));};
void SYS_Thread_List::DisableAll	(void)		{TREE_DownChain_Traversal(SYS_AThread,Disable(_opNode));};
//------------------------------------------------------------------------------------------//
SYS_AThread* SYS_Thread_List::GetThread(const STDSTR& name){
	SYS_AThread* ret = nullptr;
	TREE_DownChain_Traversal_LINE
	(SYS_AThread, this,
		if (_opNode->selfName == name){
			ret = _opNode;
			break;
		}
	);
	return(ret);
};
//------------------------------------------------------------------------------------------//
void SYS_Thread_List::Enable(const STDSTR& name){
	TNF::Enable(GetThread(name));
};
//------------------------------------------------------------------------------------------//
void SYS_Thread_List::Disable(const STDSTR& name){
	TNF::Enable(GetThread(name));
};
//------------------------------------------------------------------------------------------//
#endif /* SYS_Thread_h */

