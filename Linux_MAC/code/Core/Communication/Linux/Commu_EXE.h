//
//  Commu_Exe.hpp
//  SDT
//
//  Created by Leif Wen on 23/04/2017.
//  Copyright © 2017 leif wen. All rights reserved.
//
//------------------------------------------------------------------------------------------//
#include "Commu_DBuf.h"
//------------------------------------------------------------------------------------------//
#if defined COMMU_DBUFH && defined CommonDefH_Unix
#ifndef Commu_EXE_hpp
#define Commu_EXE_hpp
#ifdef Commu_EXE_hpp
//------------------------------------------------------------------------------------------//
#include <sys/types.h>
#include <sys/wait.h>
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
class AEXE : public COMMU_DBUF{
	public:
		enum	{RFLAG_C = 2, RFLAG_S = COMMU_DBUF::RFLAG_S + COMMU_DBUF::RFLAG_C};
	private:
		enum	{blDTR = RFLAG_CREATE(0),blRTS = RFLAG_CREATE(1),};
	public:
				 AEXE(uint32 tSize,const ODEV_SYSTEM *logSys = nullptr) : COMMU_DBUF(tSize,logSys){Init();SetGetDataByRead();SetSelfName("AEXE");};
		virtual ~AEXE(void){CloseD();monitorThread.RemoveSelf();};
	private:
		void			Init(void);
	protected:
		virtual	int32	OpenDev				(const STDSTR &tCDBufName,int32 tCDBufPar,CSType tCSType,int32 blEnEcho);
		virtual	void	CloseDev			(void);
	private:
		virtual	int32	ReadFromDevice		(uint32 *retNum,uint8 *buffer,uint32 length);
		virtual	int32	SendToDevice		(uint32 *retNum,const uint8 *buffer,uint32 length);
	private:
		virtual void	DoPrintOnOpenSuccess(void){;};
		virtual	void	DoPrintOnOpenFail	(void){;};
		virtual void	DoPrintOnClose		(void){;};
	private:
		SYS_Thread<AEXE>	monitorThread;
		int32				monitorThreadFun(void *p);
	protected:
		pid_t	childpid;
		int		fd_pipeChildOut[2];
		int		fd_pipeChildIn[2];
		STDSTR	cgCommand;
		STDSTR	cgSH;
	public:
		static	int32	ExecuteConsoleCommand(const STDSTR &strCommand);
		const STDSTR&	GetCommand	(void)const{return(cgCommand);};
				int32	Execute		(const STDSTR &tCDBufName,const STDSTR &cmd);
				int32	SH			(const STDSTR &tCDBufName,const STDSTR &cmd);
				int32	BASH		(const STDSTR &tCDBufName,const STDSTR &cmd);
				int32	KillSelf	(void)const;
};
//------------------------------------------------------------------------------------------//
class AEXEPOOL : public COMMU_DBUF_FRAME_FW{
	public:
				 AEXEPOOL(void):COMMU_DBUF_FRAME_FW(){Init();};
				 AEXEPOOL(uint32 tSize,const ODEV_SYSTEM *logSys = nullptr):COMMU_DBUF_FRAME_FW(){Init();Init(tSize,logSys);};
		virtual ~AEXEPOOL(void){CloseD();};
	private:
		void	Init(void){
			CreateTrash(this);
			DisableLog();
			SetSelfName("AExePool");
		}
	public:
		void	Init(uint32 tSize,const ODEV_SYSTEM *logSys = nullptr);
	protected:
				virtual	void	DoClose		(void);
		inline	virtual	TNFP*	CreateNode	(void){return(SetSubNodeSelfName(new AEXE(cgBufMaxSize,GetLogSystem())));};
	private:
		virtual void	DoPrintOnOpenSuccess(void){;};
		virtual	void	DoPrintOnOpenFail	(void){;};
		virtual void	DoPrintOnClose		(void){;};
	public:
		virtual	void	ChildClose	(COMMU_DBUF_FRAME *commu){COMMU_DBUF_FRAME_FW::ChildClose(commu);};
				int32	ChildClose	(const STDSTR &tCDBufName,const STDSTR &cmd,int32 timeout);
				int32	ChildClose	(const STDSTR &tCDBufName,int32 timeout);
				AEXE*	Execute		(const STDSTR &tCDBufName,const STDSTR &cmd);
				AEXE*	SH			(const STDSTR &tCDBufName,const STDSTR &cmd);
				AEXE*	BASH		(const STDSTR &tCDBufName,const STDSTR &cmd);
				AEXE*	Find		(const STDSTR &tCDBufName,const STDSTR &cmd);
				AEXE*	Find		(const STDSTR &tCDBufName);
};
//------------------------------------------------------------------------------------------//
#endif
#endif
#endif /* Commu_Exe_hpp */
