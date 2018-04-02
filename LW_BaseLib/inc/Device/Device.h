//
//  Device.h
//  SDT
//
//  Created by Leif Wen on 13/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "Commu_DBuf.h"
#include "Commu_Com.h"
#include "Commu_Socket.h"
#if defined ODEV_System_h && defined Commu_DBuf_h && (defined Commu_Com_h || defined Commu_Socket_h)
//------------------------------------------------------------------------------------------//
#ifndef Device_h
#define Device_h
#ifdef Device_h
//------------------------------------------------------------------------------------------//
class DEVICE;
class ODEV_CACHE;
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
class DEVICE : public COMMU_FRAME_LOGSYS{
	public:
				 DEVICE(uint32 size,const ODEV_CACHE* cache);
		virtual ~DEVICE(void);
	public:
		uint32	SSend			(const UVIn&  _in);
		uint32	SRead			(const UVOut& _out,uint32 num);
	public:
		void	SEmpty			(void);
		uint32	SRxSBUFMaxSize	(void);
		uint32	STxSBUFMaxSize	(void);
		uint32	SUnreadBytes	(void);
		uint32	SUnsentBytes	(void);
		uint64	SFwBytes		(void);
		uint64	SRxBytes		(void);
		uint64	STxBytes		(void);
	private:
				virtual	bool32	OpenDev					(const OPEN_PAR& par);
				virtual	void	CloseDev				(void);
	
				virtual	void	CloseChild				(COMMU_FRAME *commu);
		inline	virtual	void	CloseAllChild			(void){;};
		inline	virtual	void	DoPrintOnOpenSuccess	(void){;};
		inline	virtual	void	DoPrintOnOpenFail		(void){;};
		inline	virtual	void	DoPrintOnClose			(void){;};
	public:
				virtual	void	EnableEcho				(void);
				virtual	void	DisableEcho				(void);
	private:
						bool32	Open_Com				(const OPEN_PAR& par);
						bool32	Open_Socket				(const OPEN_PAR& par);
						bool32	Open_Server				(const OPEN_PAR& par);
						void	Close_Com				(void);
						void	Close_Socket			(void);
						void	Close_Server			(void);
	private:
				ODEV_SYSTEM		cgLogSystem;
				ACOM*			cgCom;
				ASOCKET*		cgSocket;
				ASOCKETSERVER*	cgServer;
	private:
		const	ACOM*			CreateCOM				(void);
		const	ASOCKET*		CreateSocket			(void);
		const	ASOCKETSERVER*	CreateServer			(void);
	public:
				ACOM* 			ACom					(void)const;
				ASOCKET*		ASocket					(void)const;
				ASOCKETSERVER*	AServer					(void)const;
	public:
		ExpandDeviceAttr		cgEDA;
		STDSTR					MakeFrmTitle			(void);
		OUTPUT_NODE* 			GetSelSTDOUT			(void);
	public:
				uint32			TransformCommand		(STDSTR* retForSend,STDSTR* retForPrint,const STDSTR& cmd,CMD_TAIL tail,G_ESCAPE blEscape);
				STDSTR			MakeSendTitle			(uint32 num);
				STDSTR&			PrintSendCommand		(STDSTR* retForSend,const STDSTR& cmd,CMD_TAIL tail,G_ESCAPE blEscape);
				void			PrintSendCommand		(const STDSTR& strTitle1,const STDSTR& strTitle2,const STDSTR& strForPrint);
				void			PrintSendCommand		(uint32 num,const STDSTR& strForPrint);
				void			PrintSendStrWOG1		(const STDSTR& strPrintData);

				uint32			SendCommand				(const STDSTR& cmd);
				bool32			CheckReceive			(const STDSTR& strCondition,SBUF* cSBUF,uint32 waitMS,uint32 delyMS);
				bool32			SendCommand				(const STDSTR& cmd,const STDSTR& strCondition,SBUF* cSBUF,uint32 waitMS,uint32 delyMS);
				uint32			SendCommand				(const STDSTR& cmd,CMD_TAIL tail,G_ESCAPE blEscape);
				uint32			SendCommandWithPrint	(const STDSTR& cmd,CMD_TAIL tail,G_ESCAPE blEscape);
};
//------------------------------------------------------------------------------------------//
class DEVICE_EXE_FRAME : public TREE_NODE{
		typedef  SYS_Thread<DEVICE_EXE_FRAME>	ThreadType;
	public:
		enum	{RFLAG_C = 1, RFLAG_S = TREE_NODE::RFLAG_S + TREE_NODE::RFLAG_C};
		enum	{blIsTerminated = RFLAG_CREATE(0),};
	public:
				 DEVICE_EXE_FRAME(uint32 size);
		virtual ~DEVICE_EXE_FRAME(void);
	protected:
		uint32			cgMaxSize;
		ThreadType		executeThread;
		SBUF			cgSBUF;
		DEVICE*			cgDevice;
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
				bool32 	IsStop				(void)const;
				bool32	IsServiceTo			(const DEVICE* dev);
};
//------------------------------------------------------------------------------------------//
#endif /* Device_h */
#endif /* Device_h */
#endif /* Commu_DBuf_h */
