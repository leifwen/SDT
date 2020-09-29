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
				bool32	DoBICThreadFunc		(void* _team);
		virtual	bool32	BICThreadFunc		(void* _team);
};
//------------------------------------------------------------------------------------------//
template <typename T_CDEV>
class TServer_T : public CDRV_POOL_T<COMMU_DRV_T<0,COMMU_DRV,CORE_SOCKETSERVER>,T_CDEV>{
	public:
				 TServer_T(void);
		virtual ~TServer_T(void){;};
	protected:
		CMD_ENV*		cgENV;
		BIC_BASE*		cgBIC;
	protected:
		virtual	TNF*	CreateNode		(void);
	public:
				void	InitBIC			(uint32 size,const CMD_ENV* env,const BIC_BASE* bic);
};
//------------------------------------------------------------------------------------------//
typedef COMMU_DRV_T<TMEM|TBIRDGE|TODEVBUS,COMMU_DRV,CORE_TSOCKET>	TerminalSocket;
typedef	TServer_T<TerminalSocket>									TerminalServer;
//------------------------------------------------------------------------------------------//
#include "Commu_Terminal.hpp"
#endif /* Commu_Terminal_h */
#endif /* Commu_Terminal_h */
#endif /* BIC_Define_h */
