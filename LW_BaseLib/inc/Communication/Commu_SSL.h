//
//  Commu_SSL.h
//  SDT
//
//  Created by Leif Wen on 13/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "Commu_Base.h"
#include "MSG_Center.h"
#include "MSG_Handshake.h"
#if defined Commu_Base_h && defined MSG_Center_h && defined MSG_Handshake_h
//------------------------------------------------------------------------------------------//
#ifndef Commu_SSL_h
#define Commu_SSL_h
#ifdef Commu_SSL_h
//------------------------------------------------------------------------------------------//
class MEM_SSL : public COMMU_MEM{
	public:
		enum	{MEM_MAXPACKAGE = 4,MEM_MAXPAGES = 4,MEM_MAXDLYMS = 10000,MEM_MAXTIMES = 10};
	public:
				 MEM_SSL(void);
		virtual	~MEM_SSL(void);
	public:
		virtual	void		InitArraySize			(uint32 rxSize,uint32 txSize);
		virtual	void		Init					(const COMMU_TEAM* _team);
		virtual	void		SetSelfName				(const STDSTR& strName);
		virtual	void		SetUpName				(const STDSTR& strName);
		virtual	void		AfterReadFromDevice		(void);
		virtual	bool32		Start					(void);
		virtual	void		Stop					(void);
	public:
		virtual	ARRAY*		GetArrayRx_original		(void);
		virtual	void		EmptyArray				(void);
		virtual	uint32		Send					(const ABORTCHECK& _ac,const UVIn&  _in);
		virtual	uint32		Read					(const ABORTCHECK& _ac,const UVOut& _out,uint32 num);
	protected:
		CSSL_DEV			cgSSLDev;
		CSSL_T1*			cgT1;
		ARRAY				cgSSLRx;
		MSG_CENTER			msgCenter;
		MSG_HANDSHAKE		msgHandshake;
		CMD_ENV				cgMSGENV;
	private:
		SYS_Thread<MEM_SSL>	t1DataRxThread;
				bool32		T1DataRxThreadFun	(void* _team);
	public:
				void		ResetT0T1			(void* _team);
				void		SetConfig			(uint32 cfg,const STDSTR& sKey);
				void		SetKey				(			const STDSTR& sKey);
	public:
				void		Reset				(void* _team);
				void		Empty				(void);
};
//------------------------------------------------------------------------------------------//
template <typename T_MSG> class MEM_MSG : public MEM_SSL{
	public:
				 MEM_MSG(void);
		virtual	~MEM_MSG(void){;};
	public:
		T_MSG	cgMsg;
	public:
		virtual	void		SetSelfName			(const STDSTR& strName);
		virtual	void		SetUpName			(const STDSTR& strName);
				MSG_NODE*	GetMSG				(CMDID mID);
};
//------------------------------------------------------------------------------------------//
#include "Commu_SSL.hpp"
#endif /* Commu_SSL_h */
#endif /* Commu_SSL_h */
#endif /* Commu_Base_h */
