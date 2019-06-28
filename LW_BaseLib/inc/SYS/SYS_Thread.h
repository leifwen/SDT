//
//  SYS_Thread.h
//  SDT
//
//  Created by Leif Wen on 23/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#include "DS_Tree.h"
#ifdef DS_Tree_h
//------------------------------------------------------------------------------------------//
#ifndef SYS_Thread_h
#define SYS_Thread_h
#ifdef SYS_Thread_h
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_Unix
#include <pthread.h>
typedef pthread_t	THREADHANDLE;
#endif
#ifdef CommonDefH_VC
typedef HANDLE		THREADHANDLE;
#endif
//------------------------------------------------------------------------------------------//
class SYS_AThread : public TNFP{
	protected:
		enum{RFLAG_C = 3, RFLAG_S = TNF::RFLAG_S + TNF::RFLAG_C};
		enum{SYS_blIsTerminated		= RFLAG_CREATE(0),
			 SYS_blThreadCreated	= RFLAG_CREATE(1),
			 SYS_blThreadFinish		= RFLAG_CREATE(2),
			 SYS_blThreadStartup	= RFLAG_CREATE(3),
		};
	public:
				 SYS_AThread(void);
		virtual	~SYS_AThread(void);
	private:
		void*			cgExecutePar;
		THREADHANDLE	cgThreadHandle;
	private:
		static 	void*	CallExecute		(void* exep);
#ifdef CommonDefH_VC
		static inline unsigned __stdcall 	CallExecute_VC(LPVOID par);
#endif
	public:
				void	ThreadRun		(void* exep = nullptr);
				void	ThreadStop		(void* exep = nullptr);
		virtual	void	OnThreadStop	(void* exep){;};
		inline	bool32	IsTerminated	(void)const;
	public:
		virtual	void	Execute			(void* exep = nullptr){;};
};
//------------------------------------------------------------------------------------------//
template <typename T_CLASS> class SYS_Thread: public SYS_AThread{
		typedef bool32(T_CLASS::*T_FUN)(void *);
	public:
				 SYS_Thread(void);
		virtual	~SYS_Thread(void){;};
	private:
		T_CLASS		*cgFunClass;
		T_FUN		cgExeFun;
		T_FUN		cgOnStopFun;
	public:
		virtual	void	Execute			(void* exep);
	public:
				void	ThreadInit		(T_CLASS* funClass,T_FUN exefun,const STDSTR& threadName);
				void	ThreadInit		(T_FUN stopfun);
		virtual	void	OnThreadStop	(void* exep);
};
//------------------------------------------------------------------------------------------//
class SYS_Thread_List : public TNF{
	public:
				 SYS_Thread_List(void){;};
		virtual	~SYS_Thread_List(void){;};
	public:
		void			ThreadRun	(void* exep);
		void			ThreadStop	(void);
		SYS_AThread*	GetThread	(const STDSTR& name);
		void			Enable		(void);
		void			Disable		(void);
	};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#include "SYS_Thread.hpp"
#endif /* SYS_Thread_h */
#endif /* SYS_Thread_h */
#endif /* DS_Tree_h */

