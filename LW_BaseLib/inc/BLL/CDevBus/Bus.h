//
//  Bus.h
//  SDT
//
//  Created by Leif Wen on 13/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "Commu_Bus.h"
#include "Commu_Com.h"
#include "Commu_Socket.h"
#if defined Commu_Bus_h && (defined Commu_Com_h || defined Commu_Socket_h)
//------------------------------------------------------------------------------------------//
#ifndef Bus_h
#define Bus_h
#ifdef Bus_h
//------------------------------------------------------------------------------------------//
class CDEVBUS;
#ifndef Commu_Com_h
typedef void 	ACOM;
typedef void 	CORE_ACOM;
#endif
#ifndef Commu_Socket_h
typedef void 	ASOCKET;
typedef void 	ASOCKETSERVER;
#endif
//------------------------------------------------------------------------------------------//
class ExpandDevBusAttr{
	public:
		 ExpandDevBusAttr(void);
		~ExpandDevBusAttr(void){;};
	public:
		OPEN_PAR		aCOM;
		OPEN_PAR		aTCP;
		OPEN_PAR		aUDP;
		OPEN_PAR		aTCPS;
		OPEN_PAR		aUDPS;
		uint32			openType;
		STDSTR			name;
	public:
		CDEVBUS*		cdevbus;
		uint64*			envcfg;
	public:
		bool32			IsOpened		(void)const;
		bool32			SetConnectPar	(uint32 devType,const STDSTR& p1,const STDSTR& p2);
		bool32			SetConnectBR	(const STDSTR& br);
		bool32			Open			(const STDSTR& drvName,const OPEN_PAR& par)const;
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
class CDEVBUS : public COMMU_DEVBUS{
	public:
				 CDEVBUS(uint32 rxSize,uint32 txSize,const ODEV_CACHE* cache);
		virtual ~CDEVBUS(void);
	private:
		ExpandDevBusAttr	cgEDA;
	public:
		ExpandDevBusAttr*	EDA						(void)const;
	public:
		STDSTR				MakeFrmTitle			(void);
		STDSTR				MakeSendTitle			(uint32 num);
		STDSTR&				PrintSendCommand		(STDSTR* retForSend,const STDSTR& cmd,CMD_TAIL tail,G_ESCAPE blEscape);
		void				PrintSendCommand		(const STDSTR& strTitle1,const STDSTR& strTitle2,const STDSTR& strForPrint);
		void				PrintSendCommand		(uint32 num,const STDSTR& strForPrint);
		void				PrintSendStrWOG1		(const STDSTR& strPrintData);
		
		uint32				SendCommandWithPrint	(const STDSTR& cmd,CMD_TAIL tail,G_ESCAPE blEscape);
};
//------------------------------------------------------------------------------------------//
class CDEVBUS_APP : public TNF{
		typedef  SYS_Thread<CDEVBUS_APP>	ThreadType;
	public:
		enum	{RFLAG_C = 1, RFLAG_S = TNF::RFLAG_S + TNF::RFLAG_C};
		enum	{blIsTerminated = RFLAG_CREATE(0),};
	public:
				 CDEVBUS_APP(uint32 size);
		virtual ~CDEVBUS_APP(void);
	protected:
		uint32			cgMaxSize;
		ThreadType		executeThread;
		SBUF			cgSBUF;
		CDEVBUS*		cgCDevBus;
		DS_Lock			cgRunLock;
	protected:
				bool32	ExecuteThreadFunc	(void* p);
				bool32	IsExecuting			(const CDEVBUS* cDevBus);
		virtual	bool32	Execute				(const CDEVBUS* cDevBus,SYS_AThread* thread);
		virtual	void	Init				(const CDEVBUS* cDevBus);
		virtual	void	SetblIsTerminated	(void);
		virtual	void	ClrblIsTerminated	(void);
				bool32	IsTerminated		(void)const;
	public:
		virtual	void	Stop				(void);
				bool32 	IsStop				(void);
				bool32	IsServiceTo			(const CDEVBUS* cDevBus);
};
//------------------------------------------------------------------------------------------//
#define		DRV_COM				"DRV_COM"
#define		DRV_SOCKET			"DRV_SOCKET"
#define		DRV_SSERVER			"DRV_SSERVER"
#define		DRV_BLEPeripheral	"DRV_BLEPeripheral"
#define		DRV_BTSPP			"DRV_BTSPP"
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif /* Bus_h */
#endif /* Bus_h */
#endif /* Commu_Base_h */
