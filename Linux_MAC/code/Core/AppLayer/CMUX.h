	/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: TelitCMUX.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2013.01.05
 *
*/

//------------------------------------------------------------------------------------------//
#include "Comm_PA.h"
#include "Commu_Com.h"
#include "Device.h"
//------------------------------------------------------------------------------------------//
#if defined Comm_PAH && defined Commu_ComH && defined DeviceH
#ifdef	SWVERSION_CMUX
#ifndef CMUXH
#define CMUXH
#ifdef CMUXH
//------------------------------------------------------------------------------------------//
#define CMUX_FLAG         ((uint8) 0xF9)

// Accepted values for Control field of the frame.
// P/F bit to be consider always = 0
enum{
	CMUX_CTRL_SABM	= 0x2F,
	CMUX_CTRL_UA	= 0x63,
	CMUX_CTRL_DM	= 0x0F,
	CMUX_CTRL_DISC	= 0x43,
	CMUX_CTRL_UIH	= 0xEF,
	CMUX_CTRL_UI	= 0x03,
};
enum{// Bit of ctrl and len fields
	CMUX_EA_BIT	= 0x01,
	CMUX_CR_BIT	= 0x02,
	CMUX_PF_BIT	= 0x10,
};
// Mask to filter only the cmd code. Use: 'CMD & CMUX_CMD_MASK'
//#define CMUX_CMD_MASK     (uint8) 0xFC

enum{// CMux command
	CMUX_CMD_PN		= 0x80,	// Parameter Negotiation
	CMUX_CMD_PSC	= 0x40,	// Power Saving Control
	CMUX_CMD_MUX_CLD= 0xC0,	// Multiplexer Close Down
	CMUX_CMD_TEST	= 0x20,	// Test command
	CMUX_CMD_FCON	= 0xA0,	// Flow Control ON
	CMUX_CMD_FCOFF	= 0x60,	// Flow Control OFF
	CMUX_CMD_MSC	= 0xE0,	// Modem Status
	CMUX_CMD_RPN	= 0x90,	// Remote Port Negotiation
	CMUX_CMD_RLS	= 0x50,	// Remote Line Status
	CMUX_CMD_SNC	= 0xD0,	// Service Negotiation
	
	CMUX_NSC_RES	= 0x10,	// Not Supported Command Response
	CMUX_BREAK_OCTET= 0x01,	// Break Octect used in MSC with break
};
enum{
	// Es: if it's 4 then permissible values are: 0,1,2,3,4 where 0 is control channel
	CMUX_MAX_DLCI_INDEX		= 4,	// Max value for dlci index.
	
	CMUX_CTRL_DLCI_INDEX	= 0,	// Control channel index
	CMUX_CTRL_DLCI_VPORT1	= 1,	// VP1
	CMUX_CTRL_DLCI_VPORT2	= 2,	// VP2
	CMUX_CTRL_DLCI_VPORT3	= 3,	// VP3
	CMUX_CTRL_DLCI_VPORT4	= 4,	// VP4
};
enum{// Various constants
	CMUX_MAX_INFO_SIZE			= 127,						// Max number of Information Field bytes
	CMUX_MIN_FRAME_SIZE			= 4,						// Min number of frame bytes
	CMUX_MAX_FRAME_SIZE			= CMUX_MAX_INFO_SIZE * 2,	// It is the MAX frame bytes
	CMUX_TWOBYTES_FRAME_SIZE	= 128,
};

// TEST command string
#define CMUX_TEST_CMD_STRING      "TEMUXVERSION"

// Utility macro
#define IsUAFrame(ctrl)       ((ctrl & ~CMUX_PF_BIT) == CMUX_CTRL_UA)
#define IsDMFrame(ctrl)       ((ctrl & ~CMUX_PF_BIT) == CMUX_CTRL_DM)

// modem Status
enum{
	CMUX_MODEMSTATUS_FC		= (((uint8) 0x01<<1)),
	CMUX_MODEMSTATUS_DTR	= (((uint8) 0x01<<2)),
	CMUX_MODEMSTATUS_RTS	= (((uint8) 0x01<<3)),
	CMUX_MODEMSTATUS_DSR	= (((uint8) 0x01<<2)),
	CMUX_MODEMSTATUS_CTS	= (((uint8) 0x01<<3)),
	CMUX_MODEMSTATUS_RING	= (((uint8) 0x01<<6)),
	CMUX_MODEMSTATUS_DCD	= (((uint8) 0x01<<7)),
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
class CMUX_LengthIndicator : public PROTOCOL_NODE{
	public:
				 CMUX_LengthIndicator(void) : PROTOCOL_NODE() {Add(pn_EA) < pn_Length;;};
		virtual ~CMUX_LengthIndicator(void){;};
	public:
		PROTOCOL_NODE	pn_EA;
		PROTOCOL_NODE	pn_Length;
	public:
		virtual void	ResetfnLength	(const FIFO8 *fifo){
			fnlength = 1;
			if (pn_EA.GetValueCalc(fifo) == 0)
				fnlength = 2;
			FillMaskField();
		};
	public:
		void	Init(const FIFO8 *fifoOut){
			PROTOCOL_NODE::Init(FNT_MASK,fifoOut,1,G_ENDIAN_LITTLE);
			
			pn_EA.Init		(FNT_CTRL,fifoOut);		pn_EA.SetMaskBit(0x01);
			pn_Length.Init	(FNT_LENGTH,fifoOut);	pn_Length.SetMaskBit(0xfffffffe);
			FillMaskField();
		}
		void	SetObyteNum(uint32 fObyteNum = 1){
			fnlength = fObyteNum;
			FillMaskField();
		};
};
//------------------------------------------------------------------------------------------//
class CMUX_UIH_SubFrame : public PROTOCOL_NODE{
	public:
		enum{RFLAG_C = 0, RFLAG_S = PROTOCOL_NODE::RFLAG_S + PROTOCOL_NODE::RFLAG_C};
	public:
				 CMUX_UIH_SubFrame(void) : PROTOCOL_NODE() {Add(pn_Type < pn_Type_EA < pn_Type_CR < pn_Type_Type) < pn_LengthIndicator < pn_Content;};
		virtual ~CMUX_UIH_SubFrame(void){;};
	public:
		PROTOCOL_NODE			pn_Type;
		PROTOCOL_NODE			pn_Type_EA;
		PROTOCOL_NODE			pn_Type_CR;
		PROTOCOL_NODE			pn_Type_Type;
	
		CMUX_LengthIndicator	pn_LengthIndicator;
		PROTOCOL_NODE			pn_Content;
	public:
		void Init(FIFO8 *tFIFO_Out){
			PROTOCOL_NODE::Init(FNT_MESSAGE,tFIFO_Out);
			pn_Type.Init		(FNT_MASK,tFIFO_Out,1);
			pn_Type_EA.Init		(FNT_CTRL,tFIFO_Out);pn_Type_EA.SetMaskBit(0x01);
			pn_Type_CR.Init		(FNT_CTRL,tFIFO_Out);pn_Type_CR.SetMaskBit(0x02);
			pn_Type_Type.Init	(FNT_CTRL,tFIFO_Out);pn_Type_Type.SetMaskBit(0xFC);
			pn_Type.FillMaskField();
		
			pn_LengthIndicator.Init	(tFIFO_Out);
			pn_Content.Init			(FNT_CONTENT,tFIFO_Out);pn_Content.linkNode = &pn_LengthIndicator.pn_Length;
		};
};
//------------------------------------------------------------------------------------------//
class CMUX_FRAME : public PROTOCOL_NODE{
	public:
		enum{RFLAG_C = 0, RFLAG_S = PROTOCOL_NODE::RFLAG_S + PROTOCOL_NODE::RFLAG_C};
	public:
				 CMUX_FRAME(void) : PROTOCOL_NODE() {
					 Add(pn_Head)
					 < (pn_Address < pn_Address_EA < pn_Address_CR < pn_Address_DLCI)
					 < (pn_Control < pn_Control_PF < pn_Control_Type)
					 < pn_LengthIndicator < pn_Info < pn_FCS < pn_Tail;
				 };
		virtual ~CMUX_FRAME(void){;};
	public:
		PROTOCOL_NODE			pn_Head;
		PROTOCOL_NODE			pn_Address;
		PROTOCOL_NODE			pn_Address_EA;
		PROTOCOL_NODE			pn_Address_CR;
		PROTOCOL_NODE			pn_Address_DLCI;
		PROTOCOL_NODE			pn_Control;
		PROTOCOL_NODE			pn_Control_PF;
		PROTOCOL_NODE			pn_Control_Type;
		CMUX_LengthIndicator	pn_LengthIndicator;
	
		PROTOCOL_NODE			pn_Info;
	
		PROTOCOL_NODE			pn_FCS;
		PROTOCOL_NODE			pn_Tail;
	public:
		void	Init	(FIFO8 *fifo){
			//("F9 01 F9 05 EF 27 0D 0A 30 37 2E 30 32 2E 35 30 34 0D 0A 0D 0A 4F 4B 0D 0A 80 F9");
			PROTOCOL_NODE::Init(FNT_MESSAGE,fifo);
		
			pn_Head.Init		(FNT_HEAD,fifo,1);	pn_Head.SetFixValue("\xf9");
			pn_Address.Init		(FNT_MASK,fifo,1);
			pn_Address_EA.Init	(FNT_CTRL,fifo);pn_Address_EA.SetMaskBit(0x01);
			pn_Address_CR.Init	(FNT_CTRL,fifo);pn_Address_CR.SetMaskBit(0x02);
			pn_Address_DLCI.Init(FNT_CTRL,fifo);pn_Address_DLCI.SetMaskBit(0xFC);
			pn_Address.FillMaskField();
		
			pn_Control.Init		(FNT_MASK,fifo,1);
			pn_Control_PF.Init	(FNT_CTRL,fifo);pn_Control_PF.SetMaskBit(0x10);
			pn_Control_Type.Init(FNT_CTRL,fifo);pn_Control_Type.SetMaskBit(0xEF);
			pn_Control.FillMaskField();
			
			pn_LengthIndicator.Init(fifo);
			pn_Info.Init		(FNT_MESSAGE,fifo);	pn_Info.linkNode = &pn_LengthIndicator.pn_Length;
			pn_FCS.Init			(FNT_CHECKSUM,fifo,1);
			pn_Tail.Init		(FNT_TAIL,fifo,1);pn_Tail.SetFixValue("\xf9");
		};
	public:
		virtual int32	ChecksumResult(const FIFO8 &fifo)const;
				uint8	CalcFCS(void);
	public:
				uint32		AddCMuxUIHFrame		(STDSTR *retStr,uint8 vPort,const uint8 *data,uint32 num,uint8 crBit);
		const	CMUX_FRAME& SetCMuxUIHFrame		(STDSTR *retStr,uint8 vPort,const uint8 *data,uint32 num,uint8 crBit,uint32 *retNum = nullptr);
		const	CMUX_FRAME& SetCMuxUIHFrame		(STDSTR *retStr,uint8 vPort,const FIFO8 &fifoIn,uint8 crBit,uint32 *retNum = nullptr);
		const	CMUX_FRAME& SetCMuxCtrlFrame	(STDSTR *retStr,uint8 vPort,uint8 ctrlType,uint8 crBit);
	
		const	CMUX_FRAME&	SetCMuxCLDFrame		(STDSTR *retStr);
		const	CMUX_FRAME&	SetCMuxFCONFrame	(STDSTR *retStr,uint8 crBit);
		const	CMUX_FRAME&	SetCMuxFCOFFFrame	(STDSTR *retStr,uint8 crBit);
		const	CMUX_FRAME&	SetCMuxStdPSCFrame	(STDSTR *retStr);
		const	CMUX_FRAME&	SetCMuxTPSCxFrame	(STDSTR *retStr,uint8 mode);
		const	CMUX_FRAME&	SetCMuxMSCCmd		(STDSTR *retStr,uint8 vPort,uint8 modemStatus,uint8 UINcrbit,uint8 crBit);
		const	CMUX_FRAME&	SetCMuxBRKCmd		(STDSTR *retStr,uint8 vPort,uint8 modemStatus,uint8 UINcrbit,uint8 crBit);
	
		const	CMUX_FRAME&	SetCMuxSABMFrameD0	(STDSTR *retStr){return(SetCMuxCtrlFrame(retStr,CMUX_CTRL_DLCI_INDEX,CMUX_CTRL_SABM,CMUX_CR_BIT));};
		const	CMUX_FRAME&	SetCMuxSABMFrameV1	(STDSTR *retStr){return(SetCMuxCtrlFrame(retStr,CMUX_CTRL_DLCI_VPORT1,CMUX_CTRL_SABM,CMUX_CR_BIT));};
		const	CMUX_FRAME&	SetCMuxSABMFrameV2	(STDSTR *retStr){return(SetCMuxCtrlFrame(retStr,CMUX_CTRL_DLCI_VPORT2,CMUX_CTRL_SABM,CMUX_CR_BIT));};
		const	CMUX_FRAME&	SetCMuxSABMFrameV3	(STDSTR *retStr){return(SetCMuxCtrlFrame(retStr,CMUX_CTRL_DLCI_VPORT3,CMUX_CTRL_SABM,CMUX_CR_BIT));};
		const	CMUX_FRAME&	SetCMuxSABMFrameV4	(STDSTR *retStr){return(SetCMuxCtrlFrame(retStr,CMUX_CTRL_DLCI_VPORT4,CMUX_CTRL_SABM,CMUX_CR_BIT));};
	
		const	CMUX_FRAME&	SetCMuxDISCFrameD0	(STDSTR *retStr){return(SetCMuxCtrlFrame(retStr,CMUX_CTRL_DLCI_INDEX,CMUX_CTRL_DISC,CMUX_CR_BIT));};
		const	CMUX_FRAME&	SetCMuxDISCFrameV1	(STDSTR *retStr){return(SetCMuxCtrlFrame(retStr,CMUX_CTRL_DLCI_VPORT1,CMUX_CTRL_DISC,CMUX_CR_BIT));};
		const	CMUX_FRAME&	SetCMuxDISCFrameV2	(STDSTR *retStr){return(SetCMuxCtrlFrame(retStr,CMUX_CTRL_DLCI_VPORT2,CMUX_CTRL_DISC,CMUX_CR_BIT));};
		const	CMUX_FRAME&	SetCMuxDISCFrameV3	(STDSTR *retStr){return(SetCMuxCtrlFrame(retStr,CMUX_CTRL_DLCI_VPORT3,CMUX_CTRL_DISC,CMUX_CR_BIT));};
		const	CMUX_FRAME&	SetCMuxDISCFrameV4	(STDSTR *retStr){return(SetCMuxCtrlFrame(retStr,CMUX_CTRL_DLCI_VPORT4,CMUX_CTRL_DISC,CMUX_CR_BIT));};
	
		const	CMUX_FRAME& SetCmuxDefaultMSV1	(STDSTR *retStr){return(SetCMuxMSCCmd(retStr,CMUX_CTRL_DLCI_VPORT1,CMUX_MODEMSTATUS_DTR | CMUX_MODEMSTATUS_RTS,CMUX_CR_BIT,CMUX_CR_BIT));};
		const	CMUX_FRAME& SetCmuxDefaultMSV2	(STDSTR *retStr){return(SetCMuxMSCCmd(retStr,CMUX_CTRL_DLCI_VPORT2,CMUX_MODEMSTATUS_DTR | CMUX_MODEMSTATUS_RTS,CMUX_CR_BIT,CMUX_CR_BIT));};
		const	CMUX_FRAME& SetCmuxDefaultMSV3	(STDSTR *retStr){return(SetCMuxMSCCmd(retStr,CMUX_CTRL_DLCI_VPORT3,CMUX_MODEMSTATUS_DTR | CMUX_MODEMSTATUS_RTS,CMUX_CR_BIT,CMUX_CR_BIT));};
		const	CMUX_FRAME& SetCmuxDefaultMSV4	(STDSTR *retStr){return(SetCMuxMSCCmd(retStr,CMUX_CTRL_DLCI_VPORT4,CMUX_MODEMSTATUS_DTR | CMUX_MODEMSTATUS_RTS,CMUX_CR_BIT,CMUX_CR_BIT));};
	
		inline const STDSTR &CreateCMuxCLDFrame(STDSTR *retStr){
			*retStr = "";
			Spin_InUse_set();
			SetCMuxCLDFrame(retStr);
			Spin_InUse_clr();
			return(*retStr);
		};
		inline const STDSTR &CreateCMuxFCONFrame(STDSTR *retStr,uint8 crBit){
			*retStr = "";
			Spin_InUse_set();
			SetCMuxFCONFrame(retStr,crBit);
			Spin_InUse_clr();
			return(*retStr);
		};
		inline const STDSTR &CreateCMuxFCOFFFrame(STDSTR *retStr,uint8 crBit){
			*retStr = "";
			Spin_InUse_set();
			SetCMuxFCOFFFrame(retStr,crBit);
			Spin_InUse_clr();
			return(*retStr);
		};
		inline const STDSTR &CreateCMuxStdPSCFrame(STDSTR *retStr){
			*retStr = "";
			Spin_InUse_set();
			SetCMuxStdPSCFrame(retStr);
			Spin_InUse_clr();
			return(*retStr);
		};
		inline const STDSTR &CreateCMuxTPSCxFrame(STDSTR *retStr,uint8 mode){
			*retStr = "";
			Spin_InUse_set();
			SetCMuxTPSCxFrame(retStr,mode);
			Spin_InUse_clr();
			return(*retStr);
		};
		inline const STDSTR &CreateCMuxMSCCmd(STDSTR *retStr,uint8 vPort,uint8 modemStatus,uint8 UINcrbit,uint8 crBit){
			*retStr = "";
			Spin_InUse_set();
			SetCMuxMSCCmd(retStr,vPort,modemStatus,UINcrbit,crBit);
			Spin_InUse_clr();
			return(*retStr);
		};
		inline const STDSTR &CreateCMuxBRKCmd(STDSTR *retStr,uint8 vPort,uint8 modemStatus,uint8 UINcrbit,uint8 crBit){
			*retStr = "";
			Spin_InUse_set();
			SetCMuxBRKCmd(retStr,vPort,modemStatus,UINcrbit,crBit);
			Spin_InUse_clr();
			return(*retStr);
		};
		inline const STDSTR &CreateCMuxSABMFrameD0(STDSTR *retStr){
			*retStr = "";
			Spin_InUse_set();
			SetCMuxSABMFrameD0(retStr);
			Spin_InUse_clr();
			return(*retStr);
		};
		inline const STDSTR &CreateCMuxSABMFrameV1(STDSTR *retStr){
			*retStr = "";
			Spin_InUse_set();
			SetCMuxSABMFrameV1(retStr);
			Spin_InUse_clr();
			return(*retStr);
		};
		inline const STDSTR &CreateCMuxSABMFrameV2(STDSTR *retStr){
			*retStr = "";
			Spin_InUse_set();
			SetCMuxSABMFrameV2(retStr);
			Spin_InUse_clr();
			return(*retStr);
		};
		inline const STDSTR &CreateCMuxSABMFrameV3(STDSTR *retStr){
			*retStr = "";
			Spin_InUse_set();
			SetCMuxSABMFrameV3(retStr);
			Spin_InUse_clr();
			return(*retStr);
		};
		inline const STDSTR &CreateCMuxSABMFrameV4(STDSTR *retStr){
			*retStr = "";
			Spin_InUse_set();
			SetCMuxSABMFrameV4(retStr);
			Spin_InUse_clr();
			return(*retStr);
		};
		inline const STDSTR &CreateCMuxDISCFrameD0(STDSTR *retStr){
			*retStr = "";
			Spin_InUse_set();
			SetCMuxDISCFrameD0(retStr);
			Spin_InUse_clr();
			return(*retStr);
		};
		inline const STDSTR &CreateCMuxDISCFrameV1(STDSTR *retStr){
			*retStr = "";
			Spin_InUse_set();
			SetCMuxDISCFrameV1(retStr);
			Spin_InUse_clr();
			return(*retStr);
		};
		inline const STDSTR &CreateCMuxDISCFrameV2(STDSTR *retStr){
			*retStr = "";
			Spin_InUse_set();
			SetCMuxDISCFrameV2(retStr);
			Spin_InUse_clr();
			return(*retStr);
		};
		inline const STDSTR &CreateCMuxDISCFrameV3(STDSTR *retStr){
			*retStr = "";
			Spin_InUse_set();
			SetCMuxDISCFrameV3(retStr);
			Spin_InUse_clr();
			return(*retStr);
		};
		inline const STDSTR &CreateCMuxDISCFrameV4(STDSTR *retStr){
			*retStr = "";
			Spin_InUse_set();
			SetCMuxDISCFrameV4(retStr);
			Spin_InUse_clr();
			return(*retStr);
		};
		inline const STDSTR &CreateCmuxDefaultMSV1(STDSTR *retStr){
			*retStr = "";
			Spin_InUse_set();
			SetCmuxDefaultMSV1(retStr);
			Spin_InUse_clr();
			return(*retStr);
		};
		inline const STDSTR &CreateCmuxDefaultMSV2(STDSTR *retStr){
			*retStr = "";
			Spin_InUse_set();
			SetCmuxDefaultMSV2(retStr);
			Spin_InUse_clr();
			return(*retStr);
		};
		inline const STDSTR &CreateCmuxDefaultMSV3(STDSTR *retStr){
			*retStr = "";
			Spin_InUse_set();
			SetCmuxDefaultMSV3(retStr);
			Spin_InUse_clr();
			return(*retStr);
		};
		inline const STDSTR &CreateCmuxDefaultMSV4(STDSTR *retStr){
			*retStr = "";
			Spin_InUse_set();
			SetCmuxDefaultMSV4(retStr);
			Spin_InUse_clr();
			return(*retStr);
		};
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
class CMUXCOM : public ACOM{
	public:
		enum{RFLAG_C = 8, RFLAG_S = ACOM::RFLAG_S + ACOM::RFLAG_C};
	private:
		enum{
			blV_DSR = RFLAG_CREATE(0),
			blV_CTS = RFLAG_CREATE(1),
			blV_DCD = RFLAG_CREATE(2),
			blV_RING = RFLAG_CREATE(3),
			blV_DTR = RFLAG_CREATE(4),
			blV_RTS = RFLAG_CREATE(5),
			blV_HEX = RFLAG_CREATE(6),
			blV_Escape = RFLAG_CREATE(7),
		};
	public:
				 CMUXCOM(uint32 tSize,const ODEV_SYSTEM *logSys = nullptr) : ACOM(tSize,logSys){Init();SetSelfName("CMUXCOM");};
		virtual ~CMUXCOM(void){CloseD();cmuxThread.RemoveSelf();};
	public:
		virtual	int32	DoOpen				(const STDSTR &tCDBufName,int32 tCDBufPar,CSType tCSType,int32 blEnEcho);
		virtual	void	DoPrintOnOpenSuccess(void);
		virtual	void	DoPrintOnClose		(void);
	private:
				void	Init(void);
	private:
		SYS_Thread<CMUXCOM>	cmuxThread;
		int32				cmuxThreadFun(void *p);
	public:
		int32	cgPortID;
	public:
		void	Update_DSR	(int32 bl);
		void	Update_CTS	(int32 bl);
		void	Update_DCD	(int32 bl);
		void	Update_RING	(int32 bl);
		void	Update_DTR	(int32 bl);
		void	Update_RTS	(int32 bl);
		void	Update_HEX	(int32 bl);
		void	Update_Escape(int32 bl);
	
		int32	Check_DSR	(void){return(CheckSFlag(blV_DSR));};
		int32	Check_CTS	(void){return(CheckSFlag(blV_CTS));};
		int32	Check_DCD	(void){return(CheckSFlag(blV_DCD));};
		int32	Check_RING	(void){return(CheckSFlag(blV_RING));};
		int32	Check_DTR	(void){return(CheckSFlag(blV_DTR));};
		int32	Check_RTS	(void){return(CheckSFlag(blV_RTS));};
		int32	Check_HEX	(void){return(CheckSFlag(blV_HEX));};
		int32	Check_Escape(void){return(CheckSFlag(blV_Escape));};
	public:
		const	STDSTR		&DlciStatus	(STDSTR *retStr);
};
//------------------------------------------------------------------------------------------//
class DEVICE;
//------------------------------------------------------------------------------------------//
class CMUXDriver : public COMMU_DBUF_THREAD{
	public:
		enum{RFLAG_C = 2, RFLAG_S = COMMU_DBUF_THREAD::RFLAG_S + COMMU_DBUF_THREAD::RFLAG_C};
		enum{Max_CMUXCOM = CMUX_MAX_DLCI_INDEX};
	private:
		enum{
			blStartWithAT = RFLAG_CREATE(0),
			blInitWithThread = RFLAG_CREATE(1),
		};
	public:
				 CMUXDriver(void):COMMU_DBUF_THREAD(){;};
				 CMUXDriver(uint32 tSize,const DEVICE *dev) : COMMU_DBUF_THREAD(){Init(tSize,dev);SetSelfName("CMUXDriver");};
		virtual ~CMUXDriver(void){CloseD();DestroyAll();};
	public:
		void	Init	(uint32 tSize,const DEVICE *dev);
	private:
		virtual	int32	OpenDev				(const STDSTR &tCDBufName,int32 tCDBufPar,CSType tCSType,int32 blEnEcho);
		virtual	void	CloseDev			(void);
		inline	virtual	TNFP*	CreateNode	(void){return(SetSubNodeSelfName(new CMUXCOM(cgBufMaxSize,GetLogSystem())));};
	protected:
		virtual	int32	DoOpen				(const STDSTR &tCDBufName,int32 tCDBufPar,CSType tCSType,int32 blEnEcho);
		virtual	void	DoPrintOnOpenSuccess(void){PrintNormalMessage("CMUX driver start success");};
		virtual	void	DoPrintOnOpenFail	(void){PrintNormalMessage("CMUX driver start fail");};
		virtual	void	DoPrintOnClose		(void){PrintNormalMessage("CMUX driver stop");};
	
				void	PrintFrame			(const COLSTR &strFrame,const COLSTR &colStr1,const COLSTR &colStr2 = "",const COLSTR &colStr3 = "",const COLSTR &colStr4 = ""
											 ,const COLSTR &colStr5 = "",const COLSTR &colStr6 = "",const COLSTR &colStr7 = "",const COLSTR &colStr8 = "");
	private:
		DEVICE		*cDevice;
		CMUX_FRAME	cmux_txO;
		CMUX_FRAME	cmux_txI;
		CMUX_FRAME	cmux_rx;
	#ifdef SBIC_ExpressionH
		SBIC_Expression cgSBIC_Exp;
	#endif
	private:
		virtual	int32	RxThreadFun			(void *p);
		virtual	int32	TxThreadFun			(void *p);
				void	DeliveryRxFrame		(CMUX_FRAME *CMUXFrame,const STDSTR &strFrame);
				void	CMuxCtrlCmdExec		(CMUX_FRAME *CMUXFrame,const STDSTR &strFrame);
		const	STDSTR&	UpdateModemStatus	(STDSTR *strPrintData,uint32 cMSCDlci,uint32 cModemStatus);
	public:
		CMUXCOM	*VCOMOpen		(int32 dlci,const STDSTR &tCOMName,int32 baudrate);
		void	VCOMClose		(int32 dlci);
	private:
		int32	SendATCMD	(const STDSTR strTitle,const STDSTR &strCMD,const STDSTR &strCheck,SBUFFER *cSBUF,uint32 waitMS = 1000,uint32 delyMS = 300);
		int32	SendCMUXCMD	(const STDSTR strTitle,const STDSTR &strCMD,const STDSTR &strCheck,SBUFFER *cSBUF,uint32 waitMS = 1000,uint32 delyMS = 300);
	
		int32	CMUXInitAT		(void);
		int32	CMUXStartCT		(void);
		void	CMUXCloseCT		(void);
		void	CMUXCLDCT		(void);
		void	CMUXStdPSCCT	(uint8 cmode);
		void	CMUXFCCCT		(int32 blIsOn);
		void	CMUXMSCCT		(int32 vPortNumber,int32 blDTR,int32 blRTS);
	private:
		SYS_Thread<CMUXDriver>	commandThread;
	private:
				int32	CmuxStartWithATThreadFun(void *p);
		inline	int32	CmuxStartThreadFun	(void *p){CMUXStartCT();return 1;};
		inline	int32	CmuxStopThreadFun	(void *p){CMUXCloseCT();return 1;};
	public:
				void	CMUXStart		(void);
				void	CMUXStartWithAT	(void);
				void	CMUXStop		(void);
		inline	void	SendCLD			(void)										{CMUXCLDCT();};
		inline	void	SendStdPSC		(uint8 cmode)								{CMUXStdPSCCT(cmode);};
		inline	void	SendFCC			(int32 blIsOn)								{CMUXFCCCT(blIsOn);};
		inline	void	SendMSC			(int32 vPortNumber,int32 blDTR,int32 blRTS)	{CMUXMSCCT(vPortNumber,blDTR,blRTS);};
	public:
		uint32	Send		(CMUXCOM *ccom,FIFO8 *fifoIn);
		void	Send3P		(CMUXCOM *ccom);
	public:
		int32	Open		(int32 blInitAT = 1,int32 blInitWihtThread = 0);
		CMUXCOM	*GetVCOM	(int32 dlci);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif
#endif
#endif
#endif
