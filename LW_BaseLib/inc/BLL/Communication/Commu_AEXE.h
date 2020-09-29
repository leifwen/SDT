//
//  Commu_AEXE.h
//  SDT
//
//  Created by Leif Wen on 13/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "Commu_Base.h"
#if defined Commu_Base_h && defined CommonDefH_Unix
//------------------------------------------------------------------------------------------//
#ifndef Commu_AEXE_h
#define Commu_AEXE_h
#ifdef Commu_AEXE_h
//------------------------------------------------------------------------------------------//
class CORE_AEXE : public COMMU_CORE{
	public:
				 CORE_AEXE(void);
		virtual ~CORE_AEXE(void);
	public:
				virtual	void		Init				(const COMMU_TEAM* _team);
				virtual	void		SetSelfName			(const STDSTR& strName);
				virtual	void		SetUpName			(const STDSTR& strName);
	public:
				virtual	bool32		OpenDev				(const OPEN_PAR& par);
				virtual	void		CloseDev			(void);
	public:
				virtual	bool32		SendToDevice		(uint32* retNum,const uint8* buffer,uint32 length);
				virtual	bool32		ReadFromDevice		(uint32* retNum,	  uint8* buffer,uint32 length);
	private:
		SYS_Thread<CORE_AEXE>		monitorThread;
		bool32						MonitorThreadFunc	(void* _team);
	protected:
		pid_t			childpid;
		int				fd_pipeChildOut[2];
		int				fd_pipeChildIn[2];
		STDSTR			cgCommand;
		STDSTR			cgSH;
	public:
		static			bool32		ExecuteCommand		(const STDSTR& cmd);
				const	STDSTR&		GetCommand			(void)const;
						bool32		Execute				(const STDSTR& name,const STDSTR& cmd);
						bool32		SH					(const STDSTR& name,const STDSTR& cmd);
						bool32		BASH				(const STDSTR& name,const STDSTR& cmd);
};
//------------------------------------------------------------------------------------------//
typedef COMMU_DRV_T<TMEM|TBIRDGE,COMMU_DRV,CORE_AEXE>			AEXE;
typedef	CDRV_POOL_T<COMMU_DRV_T<0,COMMU_DRV,COMMU_CORE>,AEXE>	_AEXEPOOL;
//------------------------------------------------------------------------------------------//
class AEXEPOOL : public _AEXEPOOL{
	public:
				 AEXEPOOL(uint32 rxSize,uint32 txSize);
		virtual ~AEXEPOOL(void);
	public:
		virtual			void	CloseChild				(COMMU_DRV* cdrv);
						bool32	CloseChild				(const STDSTR& name,const STDSTR& cmd,uint32 timeoutMS);
						bool32	CloseChild				(const STDSTR& name,uint32 timeoutMS);
						AEXE*	Execute					(const STDSTR& name,const STDSTR& cmd);
						AEXE*	SH						(const STDSTR& name,const STDSTR& cmd);
						AEXE*	BASH					(const STDSTR& name,const STDSTR& cmd);
						AEXE*	Find					(const STDSTR& name,const STDSTR& cmd);
						AEXE*	Find					(const STDSTR& name);
};
//------------------------------------------------------------------------------------------//
#endif /* Commu_AEXE_h */
#endif /* Commu_AEXE_h */
#endif /* Commu_Base_h */
