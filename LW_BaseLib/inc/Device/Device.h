//
//  Device.h
//  SDT
//
//  Created by Leif Wen on 13/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "Commu_Base.h"
#include "Commu_Com.h"
#include "Commu_Socket.h"
#if defined ODEV_System_h && defined Commu_Base_h && (defined Commu_Com_h || defined Commu_Socket_h)
//------------------------------------------------------------------------------------------//
#ifndef Device_h
#define Device_h
#ifdef Device_h
//------------------------------------------------------------------------------------------//
class DEVICE;
class ODEV_CACHE;
#ifndef Commu_Com_h
typedef void 	ACOM;
typedef void 	CORE_ACOM;
#endif
#ifndef Commu_Socket_h
typedef void 	ASOCKET;
typedef void 	ASOCKETSERVER;
#endif
//------------------------------------------------------------------------------------------//
class ExpandDeviceAttr{
	public:
		 ExpandDeviceAttr(void);
		~ExpandDeviceAttr(void){;};
	public:
		OPEN_PAR		aCOM;
		OPEN_PAR		aTCP;
		OPEN_PAR		aUDP;
		OPEN_PAR		aTCPS;
		OPEN_PAR		aUDPS;
		uint32			openType;
		STDSTR			name;
	public:
		DEVICE*			device;
		uint64*			envcfg;
	public:
		bool32			IsOpened		(void)const;
		bool32			Open			(void)const;
		uint32 			DevType			(void)const;
		ACOM* 			ACom			(void)const;
		CORE_ACOM* 		AComCore		(void)const;
		ASOCKET*		ASocket			(void)const;
		ASOCKETSERVER*	AServer			(void)const;
		bool32			IsCom			(void)const;
		bool32			IsSocket		(void)const;
		bool32			IsServer		(void)const;
		bool32			IsComOpened		(void)const;
		bool32			IsSocketOpened	(void)const;
		bool32			IsServerOpened	(void)const;
		bool32			IsTCPOpened		(void)const;
		bool32			IsUCPOpened		(void)const;
		bool32			IsTCPSOpened	(void)const;
		bool32			IsUDPSOpened	(void)const;
		
		ODEV_VG3D&		GetVG3D			(void);
};
//------------------------------------------------------------------------------------------//
class CORE_DEVICE : public COMMU_CORE{
	public:
				 CORE_DEVICE(void);
		virtual ~CORE_DEVICE(void);
	protected:
		virtual	bool32			OpenDev					(const OPEN_PAR& par);
		virtual	void			CloseDev				(void);
	private:
		virtual void			PrintOpenSuccess		(const STDSTR& strTitle = ""){;};
		virtual void			PrintOpenFail			(const STDSTR& strTitle = ""){;};
		virtual void			PrintClose				(const uint64& rxBytes,const uint64& txBytes,const uint64& fwBytes){;};
	private:
		ACOM*					cgCom;
		ASOCKET*				cgSocket;
		ASOCKETSERVER*			cgServer;
	private:
		const	ACOM*			CreateCOM				(void);
		const	ASOCKET*		CreateSocket			(void);
		const	ASOCKETSERVER*	CreateServer			(void);
				bool32			Open_Com				(const OPEN_PAR& par);
				bool32			Open_Socket				(const OPEN_PAR& par);
				bool32			Open_Server				(const OPEN_PAR& par);
				void			Close_Com				(void);
				void			Close_Socket			(void);
				void			Close_Server			(void);
	public:
				ACOM* 			ACom					(void)const;
				ASOCKET*		ASocket					(void)const;
				ASOCKETSERVER*	AServer					(void)const;
};
//------------------------------------------------------------------------------------------//
typedef  COMMU_POOL<COMMU<TLOGSYS|TFORWARD,COMMU_FRAME,CORE_DEVICE>,COMMU_FRAME>	DEVICE_BASE;
//------------------------------------------------------------------------------------------//
class DEVICE : public DEVICE_BASE{
	public:
				 DEVICE(uint32 rxSize,uint32 txSize,const ODEV_CACHE* cache);
		virtual ~DEVICE(void);
	private:
				virtual	void	CloseChild			(COMMU_FRAME *commu);
		inline	virtual	void	CloseAllChild		(void){;};
	private:
		ODEV_SYSTEM			cgLogSystem;
		ExpandDeviceAttr	cgEDA;
	public:
		ACOM* 				ACom					(void)const;
		ASOCKET*			ASocket					(void)const;
		ASOCKETSERVER*		AServer					(void)const;
		ExpandDeviceAttr*	EDA						(void)const;
	public:
		STDSTR				MakeFrmTitle			(void);
		OUTPUT_NODE* 		GetSelSTDOUT			(void);
		ODEV_SYSTEM*		GetLogSystem			(void)const;
		void				SetEnvCFG				(const uint64& flag);
		void				ClrEnvCFG				(const uint64& flag);
	public:
		uint32				TransformCommand		(STDSTR* retForSend,STDSTR* retForPrint,const STDSTR& cmd,CMD_TAIL tail,G_ESCAPE blEscape);
		STDSTR				MakeSendTitle			(uint32 num);
		STDSTR&				PrintSendCommand		(STDSTR* retForSend,const STDSTR& cmd,CMD_TAIL tail,G_ESCAPE blEscape);
		void				PrintSendCommand		(const STDSTR& strTitle1,const STDSTR& strTitle2,const STDSTR& strForPrint);
		void				PrintSendCommand		(uint32 num,const STDSTR& strForPrint);
		void				PrintSendStrWOG1		(const STDSTR& strPrintData);
		
		uint32				SendCommand				(const STDSTR& cmd);
		bool32				CheckReceive			(const STDSTR& strCondition,ARRAY* array,uint32 waitMS,uint32 delyMS);
		bool32				SendCommand				(const STDSTR& cmd,const STDSTR& strCondition,ARRAY* array,uint32 waitMS,uint32 delyMS);
		uint32				SendCommand				(const STDSTR& cmd,CMD_TAIL tail,G_ESCAPE blEscape);
		uint32				SendCommandWithPrint	(const STDSTR& cmd,CMD_TAIL tail,G_ESCAPE blEscape);
};
//------------------------------------------------------------------------------------------//
class DEVICE_EXE_FRAME : public TNF{
		typedef  SYS_Thread<DEVICE_EXE_FRAME>	ThreadType;
	public:
		enum	{RFLAG_C = 1, RFLAG_S = TNF::RFLAG_S + TNF::RFLAG_C};
		enum	{blIsTerminated = RFLAG_CREATE(0),};
	public:
				 DEVICE_EXE_FRAME(uint32 size);
		virtual ~DEVICE_EXE_FRAME(void);
	protected:
		uint32			cgMaxSize;
		ThreadType		executeThread;
		SBUF			cgSBUF;
		DEVICE*			cgDevice;
		DS_Lock			cgRunLock;
	protected:
				bool32	ExecuteThreadFun	(void* p);
				bool32	IsExecuting			(const DEVICE* dev);
		virtual	bool32	Execute				(const DEVICE* dev,SYS_AThread* thread);
		virtual	void	Init				(const DEVICE* dev);
		virtual	void	SetblIsTerminated	(void);
		virtual	void	ClrblIsTerminated	(void);
				bool32	IsTerminated		(void)const;
	public:
		virtual	void	Stop				(void);
				bool32 	IsStop				(void);
				bool32	IsServiceTo			(const DEVICE* dev);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif /* Device_h */
#endif /* Device_h */
#endif /* Commu_Base_h */
