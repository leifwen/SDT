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
class CORE_TSOCKET : public CORE_SOCKET{//default is TCP
	public:
		enum	{PACKAGE_MAX_SIZE = 1024 * 8};
	public:
				 CORE_TSOCKET(void);
		virtual ~CORE_TSOCKET(void);
	public:
				void	InitBIC				(const CMD_ENV* env,const BIC_BASE* bic);
		virtual	void	Init				(const COMMU_TEAM* _team);
		virtual	void	SetSelfName			(const STDSTR& strName);
		virtual	void	SetUpName			(const STDSTR& strName);
		virtual	void	CloseDev			(void);
	protected:
		CMD_ENV						cgENV;
		BIC_BASE*					cgBIC;
		SYS_Thread<CORE_TSOCKET>	BICThread;
	protected:
				bool32	BICThreadOnStop		(void* _team);
				bool32	DoBICThreadFun		(void* _team);
		virtual	bool32	BICThreadFun		(void* _team);
};
//------------------------------------------------------------------------------------------//
template <typename T_COMMU>
class FRAME_TServer : public COMMU_POOL<COMMU<0,COMMU_FRAME,CORE_SOCKETSERVER>,T_COMMU>{
	public:
				 FRAME_TServer(void);
		virtual ~FRAME_TServer(void){;};
	protected:
		CMD_ENV*		cgENV;
		BIC_BASE*		cgBIC;
	protected:
		virtual	TNFP*	CreateNode		(void);
	public:
				void	InitBIC			(uint32 size,const CMD_ENV* env,const BIC_BASE* bic);
};
//------------------------------------------------------------------------------------------//
typedef COMMU<TMEM|TBIRDGE|TLOGSYS,COMMU_FRAME,CORE_TSOCKET>	TerminalSocket;
typedef	FRAME_TServer<TerminalSocket>							TerminalServer;
//------------------------------------------------------------------------------------------//
#include "Commu_Terminal.hpp"
#endif /* Commu_Terminal_h */
#endif /* Commu_Terminal_h */
#endif /* BIC_Define_h */
