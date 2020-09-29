//
//  CMUX_UIH.h
//  SDT
//
//  Created by Leif Wen on 25/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "ADS_Protocol.h"
#ifdef ADS_Protocol_h
//------------------------------------------------------------------------------------------//
#ifndef CMUX_UIH_h
#define CMUX_UIH_h
#ifdef CMUX_UIH_h
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
		INFO_SIZE_MAX			= 121,					// Max number of Information Field bytes
		FRAME_SIZE_MIN			= 4,					// Min number of frame bytes
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

	inline bool32 IsUAFrame(uint32 ctrl){return((ctrl & ~PF_BIT) == CTRL_UA);};
	inline bool32 IsDMFrame(uint32 ctrl){return((ctrl & ~PF_BIT) == CTRL_DM);};
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
				IOSE	WriteByte1		(IOS* _ios,uint32 data);
				IOSE	WriteByte2		(IOS* _ios,uint32 data);
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
		DS_SpinLock			cgLock;
	public:
				void		InitPN				(const ARRAY* _out,const ARRAY* _in);
		virtual bool32		ChecksumResult		(void)const;
		virtual	void		SetChecksum			(void);
		static	uint32& 	GetInfoSizeMax		(void);
	public:
	
				IOSE		AddUIHFrame			(uint32* sendNum,STDSTR* retHexFrame,uint8 vPort,const UVIn& _in,uint8 crBit);
				IOSE		SetUIHFrame			(uint32* sendNum,STDSTR* retHexFrame,uint8 vPort,const UVIn& _in,uint8 crBit);
				IOSE		SetCtrlFrame		(STDSTR* retHexFrame,uint8 vPort,uint8 ctrlType,uint8 crBit);
		
				IOSE		SetCLDFrame			(STDSTR* retHexFrame);
				IOSE		SetFCONFrame		(STDSTR* retHexFrame,uint8 crBit);
				IOSE		SetFCOFFFrame		(STDSTR* retHexFrame,uint8 crBit);
				IOSE		SetStdPSCFrame		(STDSTR* retHexFrame);
				IOSE		SetTPSCxFrame		(STDSTR* retHexFrame,uint8 mode);
				IOSE		SetMSCCmd			(STDSTR* retHexFrame,uint8 vPort,uint8 modemStatus,uint8 UINcrbit,uint8 crBit);
				IOSE		SetBRKCmd			(STDSTR* retHexFrame,uint8 vPort,uint8 modemStatus,uint8 UINcrbit,uint8 crBit);
	
				IOSE		SetSABMFrameD0		(STDSTR* retHexFrame);
				IOSE		SetSABMFrameV1		(STDSTR* retHexFrame);
				IOSE		SetSABMFrameV2		(STDSTR* retHexFrame);
				IOSE		SetSABMFrameV3		(STDSTR* retHexFrame);
				IOSE		SetSABMFrameV4		(STDSTR* retHexFrame);
		
				IOSE		SetDISCFrameD0		(STDSTR* retHexFrame);
				IOSE		SetDISCFrameV1		(STDSTR* retHexFrame);
				IOSE		SetDISCFrameV2		(STDSTR* retHexFrame);
				IOSE		SetDISCFrameV3		(STDSTR* retHexFrame);
				IOSE		SetDISCFrameV4		(STDSTR* retHexFrame);
		
				IOSE		SetDefaultMSV1		(STDSTR* retHexFrame);
				IOSE		SetDefaultMSV2		(STDSTR* retHexFrame);
				IOSE		SetDefaultMSV3		(STDSTR* retHexFrame);
				IOSE		SetDefaultMSV4		(STDSTR* retHexFrame);
	
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
#endif /* CMUX_UIH_h */
#endif /* CMUX_UIH_h */
#endif
