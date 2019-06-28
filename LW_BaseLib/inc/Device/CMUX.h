//
//  CMUX.h
//  SDT
//
//  Created by Leif Wen on 25/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "Commu_Com.h"
#include "Device.h"
#include "CMUX_UIH.h"
#if defined Commu_Com_h && defined Device_h && defined CMUX_UIH_h
//------------------------------------------------------------------------------------------//
#ifndef CMUX_h
#define CMUX_h
#ifdef CMUX_h
//------------------------------------------------------------------------------------------//
class CORE_VCOM : public CORE_ACOM{
	protected:
		enum	{RFLAG_C = 8, RFLAG_S = CORE_ACOM::RFLAG_S + CORE_ACOM::RFLAG_C};
		enum	{VCOM_blDSR		= RFLAG_CREATE(0)
				,VCOM_blCTS		= RFLAG_CREATE(1)
				,VCOM_blDCD		= RFLAG_CREATE(2)
				,VCOM_blRING	= RFLAG_CREATE(3)
				,VCOM_blDTR		= RFLAG_CREATE(4)
				,VCOM_blRTS		= RFLAG_CREATE(5)
				,VCOM_blHEX		= RFLAG_CREATE(6)
				,VCOM_blEscape	= RFLAG_CREATE(7)
		};
	public:
				 CORE_VCOM(void);
		virtual ~CORE_VCOM(void);
	public:
		int32	cgPortID;
	protected:
		virtual	void	Init			(const COMMU_TEAM* _team);
		virtual	bool32	OpenDev			(const OPEN_PAR& par);
		virtual void	PrintOpenSuccess(const STDSTR& strTitle = "");
		virtual void	PrintClose		(const uint64& rxBytes,const uint64& txBytes,const uint64& fwBytes);
		static	void	Update			(COMMU_FRAME* vcom,bool32 bl,uint32 flag);
	public:
		static	void	Update_DSR		(COMMU_FRAME* vcom,bool32 bl);
		static	void	Update_CTS		(COMMU_FRAME* vcom,bool32 bl);
		static	void	Update_DCD		(COMMU_FRAME* vcom,bool32 bl);
		static	void	Update_RING		(COMMU_FRAME* vcom,bool32 bl);
		static	void	Update_DTR		(COMMU_FRAME* vcom,bool32 bl);
		static	void	Update_RTS		(COMMU_FRAME* vcom,bool32 bl);
		static	void	Update_HEX		(COMMU_FRAME* vcom,bool32 bl);
		static	void	Update_Escape	(COMMU_FRAME* vcom,bool32 bl);
		
		static	bool32	Check_DSR		(COMMU_FRAME* vcom);
		static	bool32	Check_CTS		(COMMU_FRAME* vcom);
		static	bool32	Check_DCD		(COMMU_FRAME* vcom);
		static	bool32	Check_RING		(COMMU_FRAME* vcom);
		static	bool32	Check_DTR		(COMMU_FRAME* vcom);
		static	bool32	Check_RTS		(COMMU_FRAME* vcom);
		static	bool32	Check_HEX		(COMMU_FRAME* vcom);
		static	bool32	Check_Escape	(COMMU_FRAME* vcom);
	public:
		static	STDSTR&	DlciStatus		(COMMU_FRAME* vcom,STDSTR* retStr);
};
//------------------------------------------------------------------------------------------//
class FORWARD_VCOM : public COMMU_FORWARD{
	public:
				 FORWARD_VCOM(void):COMMU_FORWARD(){;};
		virtual	~FORWARD_VCOM(void){;};
	public:
		virtual	bool32			FwThreadFun			(void* _team);
};
//------------------------------------------------------------------------------------------//
typedef  COMMU<TMEM|TBIRDGE|TFORWARD|TLOGSYS,COMMU_FRAME,CORE_VCOM,COMMU_MEM,COMMU_BRIDGE,FORWARD_VCOM>	VCOM;
//------------------------------------------------------------------------------------------//
class CORE_CMUXDriver : public COMMU_CORE{
	public:
				 CORE_CMUXDriver(void);
		virtual ~CORE_CMUXDriver(void);
	public:
		virtual	void		SetSelfName			(const STDSTR& strName);
		virtual	void		SetUpName			(const STDSTR& strName);
	private:
		virtual	bool32		OpenDev				(const OPEN_PAR& par);
		virtual	void		CloseDev			(void);
	protected:
		virtual void		PrintOpenSuccess	(const STDSTR& strTitle = "");
		virtual void		PrintOpenFail		(const STDSTR& strTitle = "");
		virtual void		PrintClose			(const uint64& rxBytes,const uint64& txBytes,const uint64& fwBytes);
	private:
		SYS_Thread<CORE_CMUXDriver>	commandThread;
		STDSTR			cgCMDsInit;
		bool32			CmuxStartThreadFun		(void* _team);
		bool32			CmuxStopThreadFun		(void* _team);
	public:
		void			CMUXStartInThread		(const STDSTR& cmdsInit);
		void			CMUXStopInThread		(void);
		bool32			CMUXStart				(STDSTR cmdsInit);
		void			CMUXStop				(void);
};
//------------------------------------------------------------------------------------------//
class BRIDGE_CMUXDriver : public COMMU_BRIDGE{
	public:
				 BRIDGE_CMUXDriver(void) : COMMU_BRIDGE(){;};
		virtual	~BRIDGE_CMUXDriver(void){;};
	private:
		virtual	bool32	RxThreadFun			(void* _team);
		virtual	bool32	TxThreadFun			(void* _team);
	protected:
				void	DeliveryRxFrame		(COMMU_TEAM* team,CMUX::UIH_FRAME* rxUIH,const STDSTR& strFrame,STDSTR& strInfo);
				void	CMuxCtrlCmdExec		(COMMU_TEAM* team,CMUX::UIH_FRAME* rxUIH,const STDSTR& strFrame,STDSTR& strInfo);
		const	STDSTR&	UpdateModemStatus	(STDSTR* strForPrint,uint32 cMSCDlci,uint32 cModemStatus);
};
//------------------------------------------------------------------------------------------//
class FRAME_CMUXDriver : public COMMU_FRAME{
	protected:
		enum	{RFLAG_C = 1, RFLAG_S = COMMU_FRAME::RFLAG_S + COMMU_FRAME::RFLAG_C};
	public:
		enum	{Max_CMUXCOM = CMUX::DLCI_MAX};
		enum	{CMUX_blInitInThread = RFLAG_CREATE(0)};
	public:
				 FRAME_CMUXDriver(void) : COMMU_FRAME(){;};
		virtual	~FRAME_CMUXDriver(void){;};
	private:
		STDSTR				cgCMDsInit;
		DEVICE*				cgDevice;
	public:
		CMUX::UIH_FRAME		cgTxUIH;
		ARRAY				cgArray;
	protected:
		virtual	bool32		DoOpen				(const OPEN_PAR& par);
	public:
				void		Init				(uint32 rxSize,uint32 txSize,const DEVICE* dev);
				bool32		Open				(const STDSTR& cmdsInit,uint64 blInitInThread = 0);
				DEVICE*		GetDevice			(void);
	public:
				VCOM* 		GetVCOM				(int32 dlci);
				VCOM*		OpenVCOM			(int32 dlci,const STDSTR& name,uint32 baudrate);
				void		CloseVCOM			(int32 dlci);
	public:
				bool32		SendATCMD	(const STDSTR& strTitle,const STDSTR& strCMD,const STDSTR& strCondition,ARRAY* array,uint32 waitMS = 1000,uint32 delyMS = 300);
				bool32		SendCMUXCMD	(const STDSTR& strTitle,const STDSTR& strCMD,const STDSTR& strCondition,ARRAY* array,uint32 waitMS = 1000,uint32 delyMS = 300);
	public:
				uint32		Send				(VCOM* vcom,const UVIn& _in);
				void		Send3Pluse			(VCOM* vcom);
				void		SendCLD				(void);
				void		SendStdPSC			(uint8 cmode);
				void		SendFCC				(bool32 blIsOn);
				void		SendMSC				(int32 dlci,bool32 blDTR,bool32 blRTS);
};
//------------------------------------------------------------------------------------------//
typedef	COMMU_POOL<COMMU<TBIRDGE|TLOGSYS,FRAME_CMUXDriver,CORE_CMUXDriver,COMMU_MEM,BRIDGE_CMUXDriver>,VCOM>	CMUXDriver;
//------------------------------------------------------------------------------------------//
extern const STDSTR CMUX_DEFATCMDS;
//------------------------------------------------------------------------------------------//
#endif /* CMUX_h */
#endif /* CMUX_h */
#endif
#ifndef CMUX_h
#undef SWVERSION_CMUX
#endif
