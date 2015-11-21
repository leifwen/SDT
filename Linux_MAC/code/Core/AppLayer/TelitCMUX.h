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
#ifndef TelitCMUXH
#define TelitCMUXH
//------------------------------------------------------------------------------------------//
#include "Comm_PA.h"
#include "Commu_Com.h"
#ifdef	SWVERSION_CMUX
//------------------------------------------------------------------------------------------//
#define CMUX_FLAG         ((uint8) 0xF9)

// Accepted values for Control field of the frame.
// P/F bit to be consider always = 0
#define CMUX_CTRL_SABM    ((uint8) 0x2F)
#define CMUX_CTRL_UA      ((uint8) 0x63)
#define CMUX_CTRL_DM      ((uint8) 0x0F)
#define CMUX_CTRL_DISC    ((uint8) 0x43)
#define CMUX_CTRL_UIH     ((uint8) 0xEF)
#define CMUX_CTRL_UI      ((uint8) 0x03)

// Bit of ctrl and len fields
#define CMUX_EA_BIT       ((uint8) 0x01)
#define CMUX_CR_BIT       ((uint8) 0x02)
#define CMUX_PF_BIT       ((uint8) 0x10)

// Mask to filter only the cmd code. Use: 'CMD & CMUX_CMD_MASK'
//#define CMUX_CMD_MASK     (uint8) 0xFC

// CMux command
#define CMUX_PN_CMD       ((uint8) 0x80)   // Parameter Negotiation
#define CMUX_PSC_CMD      ((uint8) 0x40)   // Power Saving Control
#define CMUX_MUX_CLD_CMD  ((uint8) 0xC0)   // Multiplexer Close Down
#define CMUX_TEST_CMD     ((uint8) 0x20)   // Test command
#define CMUX_FCON_CMD     ((uint8) 0xA0)   // Flow Control ON
#define CMUX_FCOFF_CMD    ((uint8) 0x60)   // Flow Control OFF
#define CMUX_MSC_CMD      ((uint8) 0xE0)   // Modem Status
#define CMUX_RPN_CMD      ((uint8) 0x90)   // Remote Port Negotiation
#define CMUX_RLS_CMD      ((uint8) 0x50)   // Remote Line Status
#define CMUX_SNC_CMD      ((uint8) 0xD0)   // Service Negotiation

// Not Supported Command Response
#define CMUX_NSC_RES      ((uint8) 0x10)
// Break Octect used in MSC with break
#define CMUX_BREAK_OCTET  ((uint8) 0x01)

#define CMUX_CTRL_DLCI_INDEX      (0)			// Control channel index
#define CMUX_CTRL_DLCI_VPORT1     (1)			// VP1
#define CMUX_CTRL_DLCI_VPORT2     (2)			// VP2
#define CMUX_CTRL_DLCI_VPORT3     (3)			// VP3
#define CMUX_CTRL_DLCI_VPORT4     (4)			// VP4
// Es: if it's 4 then permissible values are: 0,1,2,3,4 where 0 is control channel
#define CMUX_MAX_DLCI_INDEX       (4)			// Max value for dlci index.

// Various constants
#define CMUX_MAX_INFO_SIZE			(127)		// Max number of Information Field bytes
#define CMUX_MIN_FRAME_SIZE			(4)			// Min number of frame bytes
#define CMUX_MAX_FRAME_SIZE			(CMUX_MAX_INFO_SIZE * 2)	// It is the MAX frame bytes
#define CMUX_TWOBYTES_FRAME_SIZE	(128)

// TEST command string
#define CMUX_TEST_CMD_STRING      "TEMUXVERSION"

// Utility macro
#define IsUAFrame(ctrl)       ((ctrl & ~CMUX_PF_BIT) == CMUX_CTRL_UA)
#define IsDMFrame(ctrl)       ((ctrl & ~CMUX_PF_BIT) == CMUX_CTRL_DM)

// modem Status
#define CMUX_MODEMSTATUS_FC		(((uint8) 0x01<<1))
#define CMUX_MODEMSTATUS_DTR	(((uint8) 0x01<<2))
#define CMUX_MODEMSTATUS_RTS	(((uint8) 0x01<<3))
#define CMUX_MODEMSTATUS_DSR	(((uint8) 0x01<<2))
#define CMUX_MODEMSTATUS_CTS	(((uint8) 0x01<<3))
#define CMUX_MODEMSTATUS_RING	(((uint8) 0x01<<6))
#define CMUX_MODEMSTATUS_DCD	(((uint8) 0x01<<7))
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
class CMUX_LengthIndicator : public Field_Node{
	public:
				 CMUX_LengthIndicator(void) : Field_Node() {;};
		virtual ~CMUX_LengthIndicator(void){;};
	public:
		Field_Node	fn_EA;
		Field_Node	fn_Length;
	public:
		void	Init(FNT_Type fType,const FIFO_UINT8 *fifoOut){
			Field_Node::Init(fType,fifoOut,1,G_LITTLE_ENDIAN);
			
			fn_EA.Init(FNT_CTRL,fifoOut);
			fn_EA.SetMaskBit(0x01);
		
			fn_Length.Init(FNT_LENGTH,fifoOut);
			fn_Length.SetMaskBit(0xfffffffe);
		
			AddNode(&fn_EA);
			AddNode(&fn_Length);
		
			FillMaskField();
		}
		virtual void	ResetfnLength	(const FIFO_UINT8 *fifobuf){
			fnlength = 1;
			FillMaskField();
			if (fn_EA.GetValueCalc(fifobuf) == 0)
				fnlength = 2;
			FillMaskField();
		};
};
//------------------------------------------------------------------------------------------//
class CMUX_UIH_SubFrame : public Field_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = Field_Node::RFLAG_S + Field_Node::RFLAG_C};
	public:
				 CMUX_UIH_SubFrame(void) : Field_Node() {;};
		virtual ~CMUX_UIH_SubFrame(void){;};
	public:
		Field_Node				fn_Type;
		Field_Node				fn_Type_EA;
		Field_Node				fn_Type_CR;
		Field_Node				fn_Type_Type;
	
		CMUX_LengthIndicator	fn_LengthIndicator;
		Field_Node				fn_Content;
	public:
		void Init(FIFO_UINT8 *tFIFO_Out);
	public:
};
//------------------------------------------------------------------------------------------//
class CMUX_FRAME : public Field_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = Field_Node::RFLAG_S + Field_Node::RFLAG_C};
	public:
				 CMUX_FRAME(void) : Field_Node() {cgResultFIFO.Init(CMUX_MAX_FRAME_SIZE);Init();};
		virtual ~CMUX_FRAME(void){;};
	public:
		Field_Node				fn_Head;
		Field_Node				fn_Address;
		Field_Node				fn_Address_EA;
		Field_Node				fn_Address_CR;
		Field_Node				fn_Address_DLCI;
		Field_Node				fn_Control;
		Field_Node				fn_Control_PF;
		Field_Node				fn_Control_Type;
		CMUX_LengthIndicator	fn_LengthIndicator;
	
		Field_Node				fn_Info;
	
		Field_Node				fn_FCS;
		Field_Node				fn_Tail;
		FIFO_UINT8				cgResultFIFO;
	private:
		void	Init	(void);
	public:
		virtual int32	ChecksumResult(const FIFO_UINT8 &fifobuf)const;
				uint8	CalcFCS(void);
	public:
		std::string		CreateCMuxCrtlFrame		(uint8 vPort,uint8 ctrlType,uint8 crBit);
		std::string		CreateCMuxUIHFrame		(uint8 vPort,const FIFO_UINT8 &info,uint8 crBit);
		std::string		CreateCMuxUIHFrame		(uint8 vPort,const std::string &info,uint8 crBit);
		std::string		CreateCMuxCLDFrame		(void);
		std::string		CreateCMuxFCONFrame		(uint8 crBit);
		std::string		CreateCMuxFCOFFFrame	(uint8 crBit);
		std::string		CreateCMuxStdPSCFrame	(void);
		std::string		CreateCMuxTPSCxFrame	(uint8 mode);
		std::string		CreateCMuxSABMFrameD0	(void){return(CreateCMuxCrtlFrame(CMUX_CTRL_DLCI_INDEX,CMUX_CTRL_SABM,CMUX_CR_BIT));};
		std::string		CreateCMuxSABMFrameV1	(void){return(CreateCMuxCrtlFrame(CMUX_CTRL_DLCI_VPORT1,CMUX_CTRL_SABM,CMUX_CR_BIT));};
		std::string		CreateCMuxSABMFrameV2	(void){return(CreateCMuxCrtlFrame(CMUX_CTRL_DLCI_VPORT2,CMUX_CTRL_SABM,CMUX_CR_BIT));};
		std::string		CreateCMuxSABMFrameV3	(void){return(CreateCMuxCrtlFrame(CMUX_CTRL_DLCI_VPORT3,CMUX_CTRL_SABM,CMUX_CR_BIT));};
		std::string		CreateCMuxSABMFrameV4	(void){return(CreateCMuxCrtlFrame(CMUX_CTRL_DLCI_VPORT4,CMUX_CTRL_SABM,CMUX_CR_BIT));};
		std::string		CreateCMuxDISCFrameD0	(void){return(CreateCMuxCrtlFrame(CMUX_CTRL_DLCI_INDEX,CMUX_CTRL_DISC,CMUX_CR_BIT));};
		std::string		CreateCMuxDISCFrameV1	(void){return(CreateCMuxCrtlFrame(CMUX_CTRL_DLCI_VPORT1,CMUX_CTRL_DISC,CMUX_CR_BIT));};
		std::string		CreateCMuxDISCFrameV2	(void){return(CreateCMuxCrtlFrame(CMUX_CTRL_DLCI_VPORT2,CMUX_CTRL_DISC,CMUX_CR_BIT));};
		std::string		CreateCMuxDISCFrameV3	(void){return(CreateCMuxCrtlFrame(CMUX_CTRL_DLCI_VPORT3,CMUX_CTRL_DISC,CMUX_CR_BIT));};
		std::string		CreateCMuxDISCFrameV4	(void){return(CreateCMuxCrtlFrame(CMUX_CTRL_DLCI_VPORT4,CMUX_CTRL_DISC,CMUX_CR_BIT));};
		std::string		CreateCMuxMSCCmd		(uint8 vPort,uint8 modemStatus,uint8 UINcrbit,uint8 crBit);
		std::string		CreateCMuxBRKCmd		(uint8 vPort,uint8 modemStatus,uint8 UINcrbit,uint8 crBit);
};
//------------------------------------------------------------------------------------------//

//------------------------------------------------------------------------------------------//
class CMUXCOM : public APICOM{
	public:
		enum{RFLAG_C = 0, RFLAG_S = APICOM::RFLAG_S + APICOM::RFLAG_C};
	public:
				 CMUXCOM(const ODEV_LIST *tODEV_LIST,uint32 tSize) : APICOM(tODEV_LIST,tSize){Init();exThread.Disable();};
		virtual ~CMUXCOM(void){CloseD(0);};
	private:
		virtual	int32	PrintThreadFun	(void);
		void	Init	(void);
	public:
		int32	cgTrackingID;
		int32	vPortDSR;
		int32	vPortCTS;
		int32	vPortDCD;
		int32	vPortRING;
		int32	vPortDTR;
		int32	vPortRTS;
		int32	vPortHEX;
		int32	vPortEscape;
	public:
		virtual	int32		OpenDev		(const std::string &tCDBufName,int32 tCDBufPar,CSType tCSType);
		virtual	void		CloseDev	(void);
};
//------------------------------------------------------------------------------------------//
class DEVICE;
//------------------------------------------------------------------------------------------//
class CMUXDriver : public RTREE_NODE{
	public:
		enum{PackageMaxSize = 1024 * 8,Max_CMUXCOM = CMUX_MAX_DLCI_INDEX};
		enum{RFLAG_C = 0, RFLAG_S = RTREE_NODE::RFLAG_S + RTREE_NODE::RFLAG_C};
	public:
				 CMUXDriver(const DEVICE *tDevice,uint32 tSize);
				 CMUXDriver(void);
		virtual ~CMUXDriver(void){
			DriverStop();
			DestroyAll();
		};
	private:
		int32		cgBufMaxSize;
	private:
		void		SetblStart		(void)		{SetSFlag(RFLAG_CREATE(0));};
		void		ClrblStart		(void)		{ClrSFlag(RFLAG_CREATE(0));};
	private:
		int32		cgCommandMode;

		SYS_ThreadEx<CMUXDriver>		deliveryThread;
		SYS_ThreadEx<CMUXDriver>		commandThread;

		int32	CommandThreadFun		(void);
		int32	DeliveryThreadFun		(void);
		void	DeliveryRxFrame			(CMUX_FRAME *CMUXFrame);
		void	CMuxCtrlCmdExec			(CMUX_FRAME *CMUXFrame);
		void	PrintERRORToMemoViewer	(const std::string &strError);
		void	UpdateModemStatus		(uint32 cMSCDlci,uint32 cModemStatus);
	private:
		void	PrintCMD		(const std::string &title,const std::string &strOutput);
		void	PrintCMUXCMD	(const std::string &title,const std::string &strOutput);

		int32	CMUXInitTelitAT	(PUB_SBUF *cBuffer);
		void	CMUXInitCT		(PUB_SBUF *cBuffer);
		void	CMUXCloseCT		(PUB_SBUF *cBuffer);
		void	CMUXCLDCT		(void);
		void	CMUXStdPSCCT	(uint8 cmode);
		void	CMUXFCCCT		(int32 blIsOn);
		void	CMUXMSCCT		(int32 vPortNumber,int32 blDTR,int32 blRTS);
		int32	DriverStart_Do	(void);
		int32	DriverStop_Do	(void);
	public:
		DEVICE	*cDevice;
		void	CMUXInit		(void);
		void	CMUXInitWoAT	(void);
		void	CMUXClose		(void);
		void	SendCMUXCLD		(void)			{CMUXCLDCT();};
		void	SendCMUXStdPSC	(uint8 cmode)	{CMUXStdPSCCT(cmode);};
		void	SendFCC			(int32 blIsOn)	{CMUXFCCCT(blIsOn);};
		void	SendMSC			(int32 vPortNumber,int32 blDTR,int32 blRTS){CMUXMSCCT(vPortNumber,blDTR,blRTS);};
	public:
		int32	DriverStart		(void);
		int32	DriverStop		(void);
		int32	CheckblStart	(void)const	{return(CheckSFlag(RFLAG_CREATE(0)));};
		CMUXCOM *Find_nolock	(int32 dlci);
		CMUXCOM *SetSelect		(int32 dlci);
	
		CMUXCOM	*VCOMOpen		(int32 dlci,const std::string &tCOMName,int32 baudrate);
		int32	VCOMClose		(int32 dlci,int32 blPrint);
		void	LVCOMClose		(void){RTREE_LChildRChain_T(CMUXCOM,CloseD(1));};
	public:
		void	Init			(const DEVICE *tDevice,uint32 tSize);
		int32	CMUXStart		(int32 blTelit);
		int32	CMUXStop		(void);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif
#endif