//
//  Commu_Terminal.h
//  SDT
//
//  Created by Leif Wen on 17/03/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "BIC_Define.h"
#include "Commu_Socket.h"
#if defined BIC_Define_h && defined Commu_Socket_h
//------------------------------------------------------------------------------------------//
#ifndef Commu_Terminal_h
#define Commu_Terminal_h
#ifdef Commu_Terminal_h
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
template <typename T_COMMU> class COMMU_SOCKET2 : public COMMU_SOCKET<T_COMMU>{
	public:
				 COMMU_SOCKET2(uint32 size,const ODEV_SYSTEM* logSys);
				 COMMU_SOCKET2(uint32 size,const CMD_ENV* env,const BIC_BASE* bic);
		virtual ~COMMU_SOCKET2(void);
	public:
		virtual	void	SetSelfName			(const STDSTR& strName);
		virtual	void	SetFatherName		(const STDSTR& strName);
	protected:
		CMD_ENV			cgENV;
		BIC_BASE*		cgBIC;
		SYS_Thread<COMMU_SOCKET2>	BICThread;
	private:
		virtual	void	DoClose				(void);
	protected:
				bool32	BICThreadOnStop		(void* commu);
				bool32	DoBICThreadFun		(void* commu);
		virtual	bool32	BICThreadFun		(void* commu);
	public:
				void	Init				(const CMD_ENV* env);
};
//------------------------------------------------------------------------------------------//
template <typename T_COMMU> class COMMU_SOCKETSERVER2 : public COMMU_SOCKETSERVER<T_COMMU>{
	public:
				 COMMU_SOCKETSERVER2(uint32 size,const CMD_ENV* env,const BIC_BASE* bic);
		virtual ~COMMU_SOCKETSERVER2(void){;};
	protected:
		CMD_ENV*	cgENV;
		BIC_BASE*	cgBIC;
	protected:
		virtual	TNFP*	CreateNode		(void);
};
//------------------------------------------------------------------------------------------//
template <typename T_COMMU> class COMMU_NOSMC2 : public T_COMMU{
	public:
				 COMMU_NOSMC2(uint32 size,const ODEV_SYSTEM* logSys)				: T_COMMU(size,logSys){;};
				 COMMU_NOSMC2(uint32 size,const CMD_ENV* env,const BIC_BASE* bic)	: T_COMMU(size,env,bic){;};
		virtual ~COMMU_NOSMC2(void){;};
	public:
		virtual void	DoThreadsStart	(void){this->cgThreadList.ThreadRun(this);};
};
//------------------------------------------------------------------------------------------//
typedef COMMU_SOCKET2<COMMU_THREAD>			TerminalSocket;
typedef COMMU_SOCKETSERVER2<TerminalSocket>	TerminalServer;
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#include "Commu_Terminal.hpp"
#endif /* Commu_Terminal_h */
#endif /* Commu_Terminal_h */
#endif /* BIC_Define_h */
