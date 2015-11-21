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
   @2015.09.08 : add SYS_Thread to SYS_ThreadEx
*/
#ifndef SYS_ThreadH
#define SYS_ThreadH
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#include "Comm_Tree.h"
#ifdef CommonDefH_Unix
#include <pthread.h>
#endif
//------------------------------------------------------------------------------------------//
template <typename T_FUN_CLASS>
class SYS_ThreadEx: public RTREE_NODE{
	typedef int32(T_FUN_CLASS::*T_CVoidFunP)(void);
	public:
		enum{RFLAG_C = 3, RFLAG_S = RTREE_NODE::RFLAG_S + RTREE_NODE::RFLAG_C};
	public:
				 SYS_ThreadEx(void) : RTREE_NODE(){thread_T_CLASS = nullptr;thread_CVoidFunP = nullptr;Enable();SetblIsTerminated();};
		virtual	~SYS_ThreadEx(void){ThreadStop();};
	private:
		#ifdef CommonDefH_Unix
			pthread_t		ThreadHandle;
			static 	void*	StaticThread(void *par){
				SYS_ThreadEx	*cSelf;
				cSelf = (SYS_ThreadEx*)par;
				cSelf->ClrblThreadFinish();
				cSelf->Execute();
				cSelf->SetblThreadFinish();
				return(nullptr);
			};
		#endif
		#ifdef CommonDefH_VC
			HANDLE			ThreadHandle;
			static unsigned __stdcall StaticThread(LPVOID par){
				SYS_ThreadEx	*cSelf;
				cSelf = (SYS_ThreadEx*)par;
				cSelf->ClrblThreadFinish();
				cSelf->Execute();
				cSelf->SetblThreadFinish();
				return(1);
			};
		#endif
	public:
		void		ClrblIsTerminated		(void)		{ClrSFlag(RFLAG_CREATE(0));};
	private:
		void		SetblIsTerminated		(void)		{SetSFlag(RFLAG_CREATE(0));};
		void		ClrblThreadCreated		(void)		{ClrSFlag(RFLAG_CREATE(1));};
		void		SetblThreadCreated		(void)		{SetSFlag(RFLAG_CREATE(1));};
		int32		CheckblThreadCreated	(void)const {return(CheckSFlag(RFLAG_CREATE(1)));};
		void		ClrblThreadFinish		(void)		{ClrSFlag(RFLAG_CREATE(2));};
		void		SetblThreadFinish		(void)		{SetSFlag(RFLAG_CREATE(2));};
		int32		CheckblThreadFinish		(void)const {return(CheckSFlag(RFLAG_CREATE(2)));};
	private:
		T_FUN_CLASS			*thread_T_CLASS;
		T_CVoidFunP			thread_CVoidFunP;
		virtual	void		Execute(void){
			if (thread_CVoidFunP != nullptr)
				(thread_T_CLASS->*thread_CVoidFunP)();
			};
	public:
		void	ThreadRun			(void){
			if ((CheckblThreadCreated() == 0) && (CheckblEnabled() != 0)){
				ClrblIsTerminated();
					#ifdef CommonDefH_Unix
						int32 ret;
						ret = pthread_create(&ThreadHandle, nullptr, StaticThread, this);
						if (ret == -1){
							SetblIsTerminated();
							ClrblThreadCreated();
						}
						else{
							SetblThreadCreated();
						}
					#endif
					#ifdef CommonDefH_VC
						ThreadHandle = (HANDLE)_beginthreadex(nullptr, 0, StaticThread, this, 0, nullptr);
						if (ThreadHandle == INVALID_HANDLE_VALUE){
							SetblIsTerminated();
							ClrblThreadCreated();
						}
						else{
							SetblThreadCreated();
						}
					#endif
			}
		};
		void	ThreadStop			(void){
			if (CheckblThreadCreated() != 0){
				SetblIsTerminated();
				while(CheckblThreadFinish() == 0){;};
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
				ClrblThreadCreated();
				}
		};
		int32	IsTerminated		(void)const {return(CheckSFlag(RFLAG_CREATE(0)));};
	public:
		void	ThreadInit			(T_FUN_CLASS *tT_CLASS,T_CVoidFunP tT_CVoidFunP){
			thread_T_CLASS = tT_CLASS;
			thread_CVoidFunP = tT_CVoidFunP;
			SetblIsTerminated();
		};
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
//template <typename T_FUN_CLASS>
class SYS_ThreadEx_List : public RTREE_NODE{
	public:
		enum{RFLAG_C = 0, RFLAG_S = RTREE_NODE::RFLAG_S + RTREE_NODE::RFLAG_C};
	public:
				 SYS_ThreadEx_List(void) : RTREE_NODE(){Enable();};
		virtual	~SYS_ThreadEx_List(void){;};
	public:
		void	LThreadRun		(void){RTREE_LChildRChain_T(SYS_ThreadEx<SYS_ThreadEx_List>,ThreadRun());};
		void	LThreadStop		(void){RTREE_LChildRChain_T(SYS_ThreadEx<SYS_ThreadEx_List>,ThreadStop());};
		void	LEnable			(void){RTREE_LChildRChain_T(SYS_ThreadEx<SYS_ThreadEx_List>,Enable());};
		void	LDisable		(void){RTREE_LChildRChain_T(SYS_ThreadEx<SYS_ThreadEx_List>,Disable());};
};
//------------------------------------------------------------------------------------------//
#endif