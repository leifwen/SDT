//
//  CMUX.hpp
//  SDT
//
//  Created by Leif Wen on 25/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "ADS_Protocol.h"
#include "Commu_Com.h"
#include "Device.h"
#if defined ADS_Protocol_h && defined Commu_Com_h && defined Device_h
//------------------------------------------------------------------------------------------//
#ifndef CMUX_h
#define CMUX_h
#ifdef CMUX_h
//------------------------------------------------------------------------------------------//
namespace CMUX {
//------------------------------------------------------------------------------------------//
#define TEST_CMD_STRING		"TEMUXVERSION"
#define FRAME_FLAG_STR     	"\xf9"

	enum{FRAME_FLAG  = 0xF9};
	
	// Accepted values for Control field of the frame.
	// P/F bit to be consider always = 0
	enum{
		CTRL_SABM	= 0x2F,
		CTRL_UA		= 0x63,
		CTRL_DM		= 0x0F,
		CTRL_DISC	= 0x43,
		CTRL_UIH	= 0xEF,
		CTRL_UI		= 0x03,
	};
	enum{// Bit of ctrl and len fields
		EA_BIT		= 0x01,
		CR_BIT		= 0x02,
		PF_BIT		= 0x10,
	};
	enum{// CMux command
		CMD_PN		= 0x80,	// Parameter Negotiation
		CMD_PSC		= 0x40,	// Power Saving Control
		CMD_MUX_CLD	= 0xC0,	// Multiplexer Close Down
		CMD_TEST	= 0x20,	// Test command
		CMD_FCON	= 0xA0,	// Flow Control ON
		CMD_FCOFF	= 0x60,	// Flow Control OFF
		CMD_MSC		= 0xE0,	// Modem Status
		CMD_RPN		= 0x90,	// Remote Port Negotiation
		CMD_RLS		= 0x50,	// Remote Line Status
		CMD_SNC		= 0xD0,	// Service Negotiation
		
		NSC_RES		= 0x10,	// Not Supported Command Response
		BREAK_OCTET	= 0x01,	// Break Octect used in MSC with break
	};
	enum{// Es: if it's 4 then permissible values are: 0,1,2,3,4 where 0 is control channel
		DLCI_MAX 	= 4,	// Max value for dlci index.
		
		DLCI0		= 0,	// Control channel index
		DLCI1		= 1,	// VP1
		DLCI2		= 2,	// VP2
		DLCI3		= 3,	// VP3
		DLCI4		= 4,	// VP4
	};
	enum{// Various constants
		INFO_SIZE_MAX			= 127,					// Max number of Information Field bytes
		FRAME_SIZE_MIN			= 4,					// Min number of frame bytes
		FRAME_SIZE_MAX			= INFO_SIZE_MAX * 2,	// It is the MAX frame bytes
		FRAME_SIZE_TWOBYTES		= 128,
	};
	// modem Status
	enum{
		MS_FC	= (((uint8) 0x01<<1)),
		MS_DTR	= (((uint8) 0x01<<2)),
		MS_RTS	= (((uint8) 0x01<<3)),
		MS_DSR	= (((uint8) 0x01<<2)),
		MS_CTS	= (((uint8) 0x01<<3)),
		MS_RING	= (((uint8) 0x01<<6)),
		MS_DCD	= (((uint8) 0x01<<7)),
	};

#define IsUAFrame(ctrl)       ((ctrl & ~PF_BIT) == CTRL_UA)
#define IsDMFrame(ctrl)       ((ctrl & ~PF_BIT) == CTRL_DM)
//------------------------------------------------------------------------------------------//
class PN_LENI : public PNF_MASK{
	public:
				 PN_LENI(void);
		virtual ~PN_LENI(void){;};
	public:
		PNF_CTRL		pn_EA;
		PNF_LEN			pn_Length;
	public:
		virtual void	ResetPNLength	(void);
				void	InitPN			(const ARRAY* _out,const ARRAY* _in);
				void	WriteByte1		(IOSTATUS* _ios,uint32 data);
				void	WriteByte2		(IOSTATUS* _ios,uint32 data);
};
//------------------------------------------------------------------------------------------//
class UIH_SUBFRAME : public PNF_BLOCK{
	public:
				 UIH_SUBFRAME(void);
		virtual ~UIH_SUBFRAME(void){;};
	public:
		PNF_MASK	pn_Ctrl;
		PNF_CTRL	pn_Ctrl_EA;
		PNF_CTRL	pn_Ctrl_CR;
		PNF_CTRL	pn_Ctrl_Type;
	
		PN_LENI		pn_Len;
		PNF_CONTENT	pn_Content;
	public:
		void		InitPN			(const ARRAY* _out,const ARRAY* _in);
};
//------------------------------------------------------------------------------------------//
class UIH_FRAME : public PNFB_SHELL{
	public:
				 UIH_FRAME(void);
		virtual ~UIH_FRAME(void){;};
	public:
		PNF_MASK			pn_Addr;
		PNF_CTRL			pn_Addr_EA;
		PNF_CTRL			pn_Addr_CR;
		PNF_CTRL			pn_Addr_DLCI;
		PNF_MASK			pn_Ctrl;
		PNF_CTRL			pn_Ctrl_PF;
		PNF_CTRL			pn_Ctrl_Type;
		PN_LENI				pn_Len;
		
		PNF_CONTENT			pn_Info;
	public:
				void		InitPN				(const ARRAY* _out,const ARRAY* _in);
		virtual bool32		ChecksumResult		(void)const;
		virtual	void		SetChecksum			(void);
	public:
	
				UIH_FRAME&	AddUIHFrame			(uint32* sendNum,STDSTR* retHexFrame,uint8 vPort,const uint8* data,uint32 num,uint8 crBit);
				UIH_FRAME&	SetUIHFrame			(uint32* sendNum,STDSTR* retHexFrame,uint8 vPort,const uint8* data,uint32 num,uint8 crBit);
				UIH_FRAME&	SetUIHFrame			(uint32* sendNum,STDSTR* retHexFrame,uint8 vPort,const ARRAY& _in,uint8 crBit);
				UIH_FRAME&	SetCtrlFrame		(STDSTR* retHexFrame,uint8 vPort,uint8 ctrlType,uint8 crBit);
		
				UIH_FRAME&	SetCLDFrame			(STDSTR* retHexFrame);
				UIH_FRAME&	SetFCONFrame		(STDSTR* retHexFrame,uint8 crBit);
				UIH_FRAME&	SetFCOFFFrame		(STDSTR* retHexFrame,uint8 crBit);
				UIH_FRAME&	SetStdPSCFrame		(STDSTR* retHexFrame);
				UIH_FRAME&	SetTPSCxFrame		(STDSTR* retHexFrame,uint8 mode);
				UIH_FRAME&	SetMSCCmd			(STDSTR* retHexFrame,uint8 vPort,uint8 modemStatus,uint8 UINcrbit,uint8 crBit);
				UIH_FRAME&	SetBRKCmd			(STDSTR* retHexFrame,uint8 vPort,uint8 modemStatus,uint8 UINcrbit,uint8 crBit);
	
				UIH_FRAME&	SetSABMFrameD0		(STDSTR* retHexFrame);
				UIH_FRAME&	SetSABMFrameV1		(STDSTR* retHexFrame);
				UIH_FRAME&	SetSABMFrameV2		(STDSTR* retHexFrame);
				UIH_FRAME&	SetSABMFrameV3		(STDSTR* retHexFrame);
				UIH_FRAME&	SetSABMFrameV4		(STDSTR* retHexFrame);
		
				UIH_FRAME&	SetDISCFrameD0		(STDSTR* retHexFrame);
				UIH_FRAME&	SetDISCFrameV1		(STDSTR* retHexFrame);
				UIH_FRAME&	SetDISCFrameV2		(STDSTR* retHexFrame);
				UIH_FRAME&	SetDISCFrameV3		(STDSTR* retHexFrame);
				UIH_FRAME&	SetDISCFrameV4		(STDSTR* retHexFrame);
		
				UIH_FRAME&	SetDefaultMSV1		(STDSTR* retHexFrame);
				UIH_FRAME&	SetDefaultMSV2		(STDSTR* retHexFrame);
				UIH_FRAME&	SetDefaultMSV3		(STDSTR* retHexFrame);
				UIH_FRAME&	SetDefaultMSV4		(STDSTR* retHexFrame);
	
		const	STDSTR&		CreateCLDFrame		(STDSTR* retHexFrame);
		const	STDSTR&		CreateFCONFrame		(STDSTR* retHexFrame,uint8 crBit);
		const	STDSTR&		CreateFCOFFFrame	(STDSTR* retHexFrame,uint8 crBit);
		const	STDSTR&		CreateStdPSCFrame	(STDSTR* retHexFrame);
		const	STDSTR&		CreateTPSCxFrame	(STDSTR* retHexFrame,uint8 mode);
		const	STDSTR&		CreateMSCCmd		(STDSTR* retHexFrame,uint8 vPort,uint8 modemStatus,uint8 UINcrbit,uint8 crBit);
		const	STDSTR&		CreateBRKCmd		(STDSTR* retHexFrame,uint8 vPort,uint8 modemStatus,uint8 UINcrbit,uint8 crBit);
	
		const	STDSTR&		CreateSABMFrameD0	(STDSTR* retHexFrame);
		const	STDSTR&		CreateSABMFrameV1	(STDSTR* retHexFrame);
		const	STDSTR&		CreateSABMFrameV2	(STDSTR* retHexFrame);
		const	STDSTR&		CreateSABMFrameV3	(STDSTR* retHexFrame);
		const	STDSTR&		CreateSABMFrameV4	(STDSTR* retHexFrame);

		const	STDSTR&		CreateDISCFrameD0	(STDSTR* retHexFrame);
		const	STDSTR&		CreateDISCFrameV1	(STDSTR* retHexFrame);
		const	STDSTR&		CreateDISCFrameV2	(STDSTR* retHexFrame);
		const	STDSTR&		CreateDISCFrameV3	(STDSTR* retHexFrame);
		const	STDSTR&		CreateDISCFrameV4	(STDSTR* retHexFrame);
	
		const	STDSTR&		CreateDefaultMSV1	(STDSTR* retHexFrame);
		const	STDSTR&		CreateDefaultMSV2	(STDSTR* retHexFrame);
		const	STDSTR&		CreateDefaultMSV3	(STDSTR* retHexFrame);
		const	STDSTR&		CreateDefaultMSV4	(STDSTR* retHexFrame);
};
};
//------------------------------------------------------------------------------------------//
class VCOM : public ACOM{
	protected:
		enum	{RFLAG_C = 8, RFLAG_S = ACOM::RFLAG_S + ACOM::RFLAG_C};
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
				 VCOM(uint32 size,const ODEV_SYSTEM* logSys);
		virtual ~VCOM(void);
	public:
		virtual	void	SetSelfName			(const STDSTR& strName);
		virtual	void	SetFatherName		(const STDSTR& strName);
	protected:
		virtual	int32	DoOpen				(const OPEN_PAR& par);
		virtual	void	DoPrintOnOpenSuccess(void);
		virtual	void	DoPrintOnClose		(void);
	private:
		SYS_Thread<VCOM>	cmuxThread;
		bool32				cmuxThreadFun	(void* commu);
	public:
		int32	cgPortID;
	public:
				void	Update_DSR		(bool32 bl);
				void	Update_CTS		(bool32 bl);
				void	Update_DCD		(bool32 bl);
				void	Update_RING		(bool32 bl);
				void	Update_DTR		(bool32 bl);
				void	Update_RTS		(bool32 bl);
				void	Update_HEX		(bool32 bl);
				void	Update_Escape	(bool32 bl);
		
				bool32	Check_DSR		(void);
				bool32	Check_CTS		(void);
				bool32	Check_DCD		(void);
				bool32	Check_RING		(void);
				bool32	Check_DTR		(void);
				bool32	Check_RTS		(void);
				bool32	Check_HEX		(void);
				bool32	Check_Escape	(void);
	public:
		const	STDSTR&	DlciStatus		(STDSTR* retStr);
};
//------------------------------------------------------------------------------------------//
class CMUXDriver : public COMMU_THREAD{
	protected:
		enum	{RFLAG_C = 2, RFLAG_S = COMMU_THREAD::RFLAG_S + COMMU_THREAD::RFLAG_C};
		enum	{Max_CMUXCOM = CMUX::DLCI_MAX};
		enum	{blStartWithAT = RFLAG_CREATE(0),blInitWithThread = RFLAG_CREATE(1),};
	public:
				 CMUXDriver(uint32 size,const DEVICE* dev);
		virtual ~CMUXDriver(void);
	public:
		virtual	void		SetSelfName			(const STDSTR& strName);
		virtual	void		SetFatherName		(const STDSTR& strName);
	private:
		virtual	bool32		OpenDev				(const OPEN_PAR& par);
		virtual	void		CloseDev			(void);
		virtual	TNFP*		CreateNode			(void);
	protected:
		virtual	bool32		DoOpen				(const OPEN_PAR& par);
		virtual	void		DoPrintOnOpenSuccess(void);
		virtual	void		DoPrintOnOpenFail	(void);
		virtual	void		DoPrintOnClose		(void);
		
		template<typename... Args> inline void 	PrintFrame	(const STDSTR&	 strFrame,const Args&... args);
		template<typename... Args> inline void 	PrintFrame	(const _ColData& strFrame,const Args&... args);
	private:
		DEVICE*				cgDevice;
		CMUX::UIH_FRAME		cgTxUIH;
		CMUX::UIH_FRAME		cgRxUIH;
	private:
		virtual	bool32		RxThreadFun			(void* commu);
		virtual	bool32		TxThreadFun			(void* commu);
				void		DeliveryRxFrame		(CMUX::UIH_FRAME* rxUIH,const STDSTR& strFrame,STDSTR& strInfo);
				void		CMuxCtrlCmdExec		(CMUX::UIH_FRAME* rxUIH,const STDSTR& strFrame,STDSTR& strInfo);
		const	STDSTR&		UpdateModemStatus	(STDSTR* strForPrint,uint32 cMSCDlci,uint32 cModemStatus);
	private:
				bool32		SendATCMD			(const STDSTR& strTitle,const STDSTR& strCMD,const STDSTR& strCondition,SBUF* cSBUF
												 ,uint32 waitMS = 1000,uint32 delyMS = 300);
				bool32		SendCMUXCMD			(const STDSTR& strTitle,const STDSTR& strCMD,const STDSTR& strCondition,SBUF* cSBUF
												 ,uint32 waitMS = 1000,uint32 delyMS = 300);
				bool32		CMUXInitAT			(void);
				bool32		CMUXStartCT			(void);
				void		CMUXCloseCT			(void);
				void		CMUXCLDCT			(void);
				void		CMUXStdPSCCT		(uint8 cmode);
				void		CMUXFCCCT			(bool32 blIsOn);
				void		CMUXMSCCT			(bool32 dlci,bool32 blDTR,bool32 blRTS);
	private:
		SYS_Thread<CMUXDriver>	commandThread;
	private:
				bool32		CmuxStartWithATThreadFun(void* p);
				bool32		CmuxStartThreadFun		(void* p);
				bool32		CmuxStopThreadFun		(void* p);
	public:
				void		CMUXStart				(void);
				void		CMUXStartWithAT			(void);
				void		CMUXStop				(void);
				void		SendCLD					(void);
				void		SendStdPSC				(uint8 cmode);
				void		SendFCC					(bool32 blIsOn);
				void		SendMSC					(int32 dlci,bool32 blDTR,bool32 blRTS);
	public:
				bool32		Open					(bool32 blInitAT = G_TRUE,bool32 blInitWihtThread = G_FALSE);
	public:
				void		Init					(const DEVICE* dev);
				VCOM* 		GetVCOM					(int32 dlci);
				VCOM*		OpenVCOM				(int32 dlci,const STDSTR& name,uint32 baudrate);
				void		CloseVCOM				(int32 dlci);
				uint32		Send					(VCOM* vcom,const ARRAY& _in);
				void		Send3Pluse				(VCOM* vcom);
				DEVICE*		GetDevice				(void);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
template<typename... Args> inline void CMUXDriver::PrintFrame(const STDSTR& strFrame,const Args&... args){
#ifdef ODEV_System_h
	if (GetVG3D() != nullptr){
		*GetVG3D() << Begin() << DefGroup() << NL()
		<< COL_DivLineTime
		<< COL_NormalMessage;
		
		DoPrintStr(GetVG3D(),args...)
		<< ":\n"
		<< COL_DB_RxText << strFrame
		<< "\n"
		<< Endl();
	}
#endif
}
//------------------------------------------------------------------------------------------//
template<typename... Args> inline void CMUXDriver::PrintFrame(const _ColData& strFrame,const Args&... args){
#ifdef ODEV_System_h
	if (GetVG3D() != nullptr){
		*GetVG3D() << Begin() << DefGroup() << NL()
		<< COL_DivLineTime
		<< COL_NormalMessage;
		
		DoPrintStr(GetVG3D(),args...)
		<< ":\n"
		<< strFrame
		<< "\n"
		<< Endl();
	}
#endif
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#else
#undef SWVERSION_CMUX
#endif /* CMUX_h */
#endif /* CMUX_h */
#else
#undef SWVERSION_CMUX
#endif
