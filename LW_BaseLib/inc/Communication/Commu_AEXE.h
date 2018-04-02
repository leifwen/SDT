//
//  Commu_AEXE.h
//  SDT
//
//  Created by Leif Wen on 13/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "Commu_DBuf.h"
#if defined Commu_DBuf_h && defined CommonDefH_Unix
//------------------------------------------------------------------------------------------//
#ifndef Commu_AEXE_h
#define Commu_AEXE_h
#ifdef Commu_AEXE_h
//------------------------------------------------------------------------------------------//
class AEXE : public COMMU_THREAD{
	public:
		enum	{RFLAG_C = 2, RFLAG_S = COMMU_THREAD::RFLAG_S + COMMU_THREAD::RFLAG_C};
	private:
		enum	{blDTR = RFLAG_CREATE(0),blRTS = RFLAG_CREATE(1),};
	public:
				 AEXE(uint32 size,const ODEV_SYSTEM* logSys);
		virtual ~AEXE(void);
	public:
				virtual	void		SetSelfName			(const STDSTR& strName);
				virtual	void		SetFatherName		(const STDSTR& strName);
	protected:
				virtual	bool32		OpenDev				(const OPEN_PAR& par);
				virtual	void		CloseDev			(void);
	private:
				virtual	bool32		SendToDevice		(uint32* retNum,const uint8* buffer,uint32 length);
				virtual	bool32		ReadFromDevice		(uint32* retNum,	  uint8* buffer,uint32 length);
	private:
		inline	virtual void		DoPrintOnOpenSuccess(void){;};
		inline	virtual	void		DoPrintOnOpenFail	(void){;};
		inline	virtual void		DoPrintOnClose		(void){;};
	private:
		SYS_Thread<AEXE>			monitorThread;
		bool32						MonitorThreadFun	(void* commu);
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
class AEXEPOOL : public COMMU_FRAME_LOGSYS{
	public:
				 AEXEPOOL(uint32 size,const ODEV_SYSTEM* logSys);
		virtual ~AEXEPOOL(void);
	protected:
				virtual	void	DoClose					(void);
		inline	virtual	TNFP*	CreateNode				(void);
	private:
		virtual 		void	DoPrintOnOpenSuccess	(void){;};
		virtual			void	DoPrintOnOpenFail		(void){;};
		virtual 		void	DoPrintOnClose			(void){;};
	public:
		virtual			void	CloseChild				(COMMU_FRAME* commu);
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
#endif /* Commu_DBuf_h */
