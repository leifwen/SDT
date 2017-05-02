/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: SYS_Thread.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2012.02.09
 * @2013.12.11 : update Thread to make more safe.
 * @2013.06.30 : change to Liunx style
 				add init method
 				use int32 to replace bool
 				use int32 to replace uint32
   @2014.05.24 : form PublicSYS.h
				change PUB_Clock to SYS_ThreadLock
				change PUB_Thread to SYS_Thread
				change PUB_WaitMS to SYS_WaitMS
   @2015.09.08 : add SYS_Thread to SYS_Thread
*/

//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#include "Comm_Tree.h"
//------------------------------------------------------------------------------------------//
#ifdef Comm_TreeH
#ifndef SYS_ThreadH
#define SYS_ThreadH
#ifdef SYS_ThreadH
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_Unix
#include <pthread.h>
#endif
//------------------------------------------------------------------------------------------//
class SYS_AThread : public TREE_NODE{
	public:
		enum{RFLAG_C = 3, RFLAG_S = TREE_NODE::RFLAG_S + TREE_NODE::RFLAG_C};
	protected:
		enum{blIsTerminated = RFLAG_CREATE(0),blThreadCreated = RFLAG_CREATE(1),blThreadFinish = RFLAG_CREATE(2),};
	public:
				 SYS_AThread(void) : TREE_NODE(){Enable(); SetSFlag(blIsTerminated | blThreadFinish);};
		virtual	~SYS_AThread(void){ThreadStop();};
	private:
#ifdef CommonDefH_Unix
		pthread_t		ThreadHandle;
		static 	void*	StaticThread(void *par){
			SYS_AThread	*cSelf = (SYS_AThread*)par;
			cSelf->ClrSFlag(blThreadFinish | blIsTerminated);
			cSelf->Execute();
			cSelf->SetSFlag(blThreadFinish | blIsTerminated);
			return(nullptr);
		};
#endif
#ifdef CommonDefH_VC
		HANDLE			ThreadHandle;
		static unsigned __stdcall StaticThread(LPVOID par){
			SYS_AThread	*cSelf = (SYS_AThread*)par;
			cSelf->ClrSFlag(blThreadFinish | blIsTerminated);
			cSelf->Execute();
			cSelf->SetSFlag(blThreadFinish | blIsTerminated);
			return(1);
		};
#endif
	public:
		void	ThreadRun(void){
			if ((CheckSFlag(blThreadCreated) == 0) && (CheckEnable() != 0)){
#ifdef CommonDefH_Unix
				int32 ret;
				ret = pthread_create(&ThreadHandle, nullptr, StaticThread, this);
				if (ret == -1){
					SetSFlag(blIsTerminated);
					ClrSFlag(blThreadCreated);
				}
				else{
					SetSFlag(blThreadCreated);
				}
#endif
#ifdef CommonDefH_VC
				ThreadHandle = (HANDLE)_beginthreadex(nullptr, 0, StaticThread, this, 0, nullptr);
				if (ThreadHandle == INVALID_HANDLE_VALUE){
					SetSFlag(blIsTerminated);
					ClrSFlag(blThreadCreated);
				}
				else{
					SetSFlag(blThreadCreated);
				}
#endif
			}
		};
		void	ThreadStop(void){
			if (CheckSFlag(blThreadCreated) != 0){
				SetSFlag(blIsTerminated);
				while(CheckSFlag(blThreadFinish) == 0){SetSFlag(blIsTerminated);};
#ifdef CommonDefH_Unix
				void *t;
				pthread_join(ThreadHandle, &t);
#endif
#ifdef CommonDefH_VC
				if (ThreadHandle != INVALID_HANDLE_VALUE){
					WaitForSingleObject(ThreadHandle,INFINITE);
					try{
						CloseHandle(ThreadHandle);
					}
					catch(...){};
					ThreadHandle = INVALID_HANDLE_VALUE;
				}
#endif
				ClrSFlag(blThreadCreated);
			}
		};
		int32	IsTerminated	(void)const {return(CheckSFlag(blIsTerminated));};
	public:
		virtual	void	Execute	(void){;};
};
//------------------------------------------------------------------------------------------//
template <typename T_FUN_CLASS>
class SYS_Thread: public SYS_AThread{
		typedef int32(T_FUN_CLASS::*T_CVoidFunP)(void *);
	public:
				 SYS_Thread(void) : SYS_AThread(){thread_T_CLASS = nullptr; thread_CVoidFunP = nullptr;};
		virtual	~SYS_Thread(void){;};
	private:
		T_FUN_CLASS			*thread_T_CLASS;
		T_CVoidFunP			thread_CVoidFunP;
		void				*threadParVoidP;
	public:
		virtual	void		Execute(void){
			if (thread_CVoidFunP != nullptr)
				(thread_T_CLASS->*thread_CVoidFunP)(threadParVoidP);
		};
	public:
		void	ThreadInit	(T_FUN_CLASS *tT_CLASS,T_CVoidFunP tT_CVoidFunP,const STDSTR &selfname,void *tVoid = nullptr){
			thread_T_CLASS = tT_CLASS;
			thread_CVoidFunP = tT_CVoidFunP;
			threadParVoidP = tVoid;
			SetSFlag(blThreadFinish | blIsTerminated);
			SetSelfName(selfname);
		};
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
class SYS_Thread_List : public TREE_NODE{
	public:
				 SYS_Thread_List(void) : TREE_NODE(){Enable();};
		virtual	~SYS_Thread_List(void){;};
	public:
		void	LThreadRun		(void){TREE_LChildRChain_T(SYS_AThread,ThreadRun());};
		void	LThreadStop		(void){TREE_LChildRChain_T(SYS_AThread,ThreadStop());};
		void	LEnable			(void){TREE_LChildRChain_T(SYS_AThread,Enable());};
		void	LDisable		(void){TREE_LChildRChain_T(SYS_AThread,Disable());};
};
//------------------------------------------------------------------------------------------//
#endif
#endif
#endif
