//
//  CMUX.cpp
//  SDT
//
//  Created by Leif Wen on 25/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
#include "CMUX.h"
#ifdef CMUX_h
//------------------------------------------------------------------------------------------//
#include "DS_STRING.h"
#include "ADS_SMC.h"
//#define LOGPRINT_ENABLE
//#define LOGTHREAD_ENABLE
#include "SYS_Log.h"
namespace CMUX {
//------------------------------------------------------------------------------------------//
PN_LENI::PN_LENI(void) : PNF_MASK() {
	Add(pn_EA) < pn_Length;
};
//------------------------------------------------------------------------------------------//
void PN_LENI::ResetPNLength(void){
	cgPosRE.length = 1;
	pn_EA.SyncPosRE(&cgPosRE);
	if (pn_EA.GetValueCalcRE() == 0)
		cgPosRE.length = 2;
	FillMaskFieldRE();
};
//------------------------------------------------------------------------------------------//
void PN_LENI::InitPN(const ARRAY* _out,const ARRAY* _in){
	
	PNF_MASK::InitPN(_out,_in,1,G_ENDIAN_LITTLE);
	
	pn_EA.InitPN	(_out,_in,1,G_ENDIAN_LITTLE);pn_EA.SetMaskBit(0x01);
	pn_Length.InitPN(_out,_in,1,G_ENDIAN_LITTLE);pn_Length.SetMaskBit(0xfffffffe);
	FillMaskFieldRE();
}
//------------------------------------------------------------------------------------------//
void PN_LENI::WriteByte1(IOSTATUS* _ios,uint32 data){
	_Begin(_ios);
	SetByte1(_ios, data);
	_Endl();
	FillMaskFieldWR();
};
//------------------------------------------------------------------------------------------//
void PN_LENI::WriteByte2(IOSTATUS* _ios,uint32 data){
	_Begin(_ios);
	SetByte2(_ios, data);
	_Endl();
	FillMaskFieldWR();
};
//------------------------------------------------------------------------------------------//

	







//------------------------------------------------------------------------------------------//
UIH_SUBFRAME::UIH_SUBFRAME(void) : PNF_BLOCK() {
	Add(pn_Ctrl < pn_Ctrl_EA < pn_Ctrl_CR < pn_Ctrl_Type) < pn_Len < pn_Content;
};
//------------------------------------------------------------------------------------------//
void UIH_SUBFRAME::InitPN(const ARRAY* _out,const ARRAY* _in){
	if (_out == nullptr)
		_out = &cgArray;
	if (_in == nullptr)
		_in = &cgArray;

	PNF_BLOCK::InitPN	(_out,_in);
	pn_Ctrl.InitPN		(_out,_in,1,G_ENDIAN_LITTLE);
	pn_Ctrl_EA.InitPN	(_out,_in,1,G_ENDIAN_LITTLE);pn_Ctrl_EA.SetMaskBit(0x01);
	pn_Ctrl_CR.InitPN	(_out,_in,1,G_ENDIAN_LITTLE);pn_Ctrl_CR.SetMaskBit(0x02);
	pn_Ctrl_Type.InitPN	(_out,_in,1,G_ENDIAN_LITTLE);pn_Ctrl_Type.SetMaskBit(0xFC);
	pn_Ctrl.FillMaskFieldRE();
	
	pn_Len.InitPN		(_out,_in);
	pn_Content.InitPN	(_out,_in,&pn_Len);
	SetBlockPoint(this);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
static const uint8 crctable[256] = { // reversed, 8-bit, poly=0x07
	0x00, 0x91, 0xE3, 0x72, 0x07, 0x96, 0xE4, 0x75, 0x0E, 0x9F, 0xED, 0x7C, 0x09, 0x98, 0xEA, 0x7B,
	0x1C, 0x8D, 0xFF, 0x6E, 0x1B, 0x8A, 0xF8, 0x69, 0x12, 0x83, 0xF1, 0x60, 0x15, 0x84, 0xF6, 0x67,
	0x38, 0xA9, 0xDB, 0x4A, 0x3F, 0xAE, 0xDC, 0x4D, 0x36, 0xA7, 0xD5, 0x44, 0x31, 0xA0, 0xD2, 0x43,
	0x24, 0xB5, 0xC7, 0x56, 0x23, 0xB2, 0xC0, 0x51, 0x2A, 0xBB, 0xC9, 0x58, 0x2D, 0xBC, 0xCE, 0x5F,
	0x70, 0xE1, 0x93, 0x02, 0x77, 0xE6, 0x94, 0x05, 0x7E, 0xEF, 0x9D, 0x0C, 0x79, 0xE8, 0x9A, 0x0B,
	0x6C, 0xFD, 0x8F, 0x1E, 0x6B, 0xFA, 0x88, 0x19, 0x62, 0xF3, 0x81, 0x10, 0x65, 0xF4, 0x86, 0x17,
	0x48, 0xD9, 0xAB, 0x3A, 0x4F, 0xDE, 0xAC, 0x3D, 0x46, 0xD7, 0xA5, 0x34, 0x41, 0xD0, 0xA2, 0x33,
	0x54, 0xC5, 0xB7, 0x26, 0x53, 0xC2, 0xB0, 0x21, 0x5A, 0xCB, 0xB9, 0x28, 0x5D, 0xCC, 0xBE, 0x2F,
	0xE0, 0x71, 0x03, 0x92, 0xE7, 0x76, 0x04, 0x95, 0xEE, 0x7F, 0x0D, 0x9C, 0xE9, 0x78, 0x0A, 0x9B,
	0xFC, 0x6D, 0x1F, 0x8E, 0xFB, 0x6A, 0x18, 0x89, 0xF2, 0x63, 0x11, 0x80, 0xF5, 0x64, 0x16, 0x87,
	0xD8, 0x49, 0x3B, 0xAA, 0xDF, 0x4E, 0x3C, 0xAD, 0xD6, 0x47, 0x35, 0xA4, 0xD1, 0x40, 0x32, 0xA3,
	0xC4, 0x55, 0x27, 0xB6, 0xC3, 0x52, 0x20, 0xB1, 0xCA, 0x5B, 0x29, 0xB8, 0xCD, 0x5C, 0x2E, 0xBF,
	0x90, 0x01, 0x73, 0xE2, 0x97, 0x06, 0x74, 0xE5, 0x9E, 0x0F, 0x7D, 0xEC, 0x99, 0x08, 0x7A, 0xEB,
	0x8C, 0x1D, 0x6F, 0xFE, 0x8B, 0x1A, 0x68, 0xF9, 0x82, 0x13, 0x61, 0xF0, 0x85, 0x14, 0x66, 0xF7,
	0xA8, 0x39, 0x4B, 0xDA, 0xAF, 0x3E, 0x4C, 0xDD, 0xA6, 0x37, 0x45, 0xD4, 0xA1, 0x30, 0x42, 0xD3,
	0xB4, 0x25, 0x57, 0xC6, 0xB3, 0x22, 0x50, 0xC1, 0xBA, 0x2B, 0x59, 0xC8, 0xBD, 0x2C, 0x5E, 0xCF
};
//------------------------------------------------------------------------------------------//
UIH_FRAME::UIH_FRAME(void) : PNFB_SHELL() {
	AddBlockSubPN(pn_Addr < pn_Addr_EA < pn_Addr_CR < pn_Addr_DLCI)
	< (pn_Ctrl < pn_Ctrl_PF < pn_Ctrl_Type)
	< pn_Len < pn_Info;
};
//------------------------------------------------------------------------------------------//
void UIH_FRAME::InitPN(const ARRAY* _out,const ARRAY* _in){
	//("F9 01 F9 05 EF 27 0D 0A 30 37 2E 30 32 2E 35 30 34 0D 0A 0D 0A 4F 4B 0D 0A 80 F9");
	if (_out == nullptr)
		_out = &cgArray;
	if (_in == nullptr)
		_in = &cgArray;
	
	PNFB_SHELL::InitPN	(_out,_in,1,G_ENDIAN_LITTLE);
	
	pns_Head.InitPN		(_out,_in,FRAME_FLAG_STR);
	pn_Addr.InitPN		(_out,_in,1,G_ENDIAN_LITTLE);
	pn_Addr_EA.InitPN	(_out,_in,1,G_ENDIAN_LITTLE);pn_Addr_EA.SetMaskBit(0x01);
	pn_Addr_CR.InitPN	(_out,_in,1,G_ENDIAN_LITTLE);pn_Addr_CR.SetMaskBit(0x02);
	pn_Addr_DLCI.InitPN	(_out,_in,1,G_ENDIAN_LITTLE);pn_Addr_DLCI.SetMaskBit(0xFC);
	pn_Addr.FillMaskFieldRE();
	
	pn_Ctrl.InitPN		(_out,_in,1,G_ENDIAN_LITTLE);
	pn_Ctrl_PF.InitPN	(_out,_in,1,G_ENDIAN_LITTLE);pn_Ctrl_PF.SetMaskBit(0x10);
	pn_Ctrl_Type.InitPN	(_out,_in,1,G_ENDIAN_LITTLE);pn_Ctrl_Type.SetMaskBit(0xEF);
	pn_Ctrl.FillMaskFieldRE();
	
	pn_Len.InitPN		(_out,_in);
	pn_Info.InitPN		(_out,_in,&pn_Len.pn_Length);
	pns_Tail.InitPN		(_out,_in,FRAME_FLAG_STR);
	SetBlockPoint(this);
};
//------------------------------------------------------------------------------------------//
bool32 UIH_FRAME::ChecksumResult(void)const{
	uint8	FCS = 0xFF;
	uint32	slength,length,offset;
	uint8*	data;
	
	length = (pn_Ctrl_Type.GetValueAMaskRE() == CTRL_UIH) ? pn_Info.GetOffsetRE() : pns_Checksum.GetOffsetRE();
	offset = GetOffsetRE();
	slength = cgPosRE.array->CalcOutLength(length,offset);
	
	if (length > 0){
		data = cgPosRE.array->GetPointer(offset);
		while(--length > 0){
			++ data;
			FCS = crctable[FCS ^ (*data)];
		}
		if (slength > 0){
			data = cgPosRE.array->GetPointer(0);
			while(slength-- > 0){
				FCS = crctable[FCS ^ (*data)];
				data ++;
			}
		}
	}

	FCS = crctable[FCS ^ pns_Checksum.GetValueRE()];
	
	return(FCS == 0xCF);
}
//------------------------------------------------------------------------------------------//
void UIH_FRAME::SetChecksum(void){
	uint8	FCS = 0xFF;
	uint32	slength,length,offset;
	uint8*	data;
	
	length = (pn_Ctrl_Type.GetValueAMaskWR() == CTRL_UIH) ? pn_Info.GetOffsetWR() : (pn_Info.GetOffsetWR() + pn_Info.GetLengthWR());
	offset = GetOffsetWR();
	slength = cgPosWR.array->CalcOutLengthInToPre(length,offset);

	if (length > 0){
		data = cgPosWR.array->GetPointer(offset);
		while(--length > 0){
			++ data;
			FCS = crctable[FCS ^ (*data)];

		}
		if (slength > 0){
			data = cgPosWR.array->GetPointer(0);
			while(slength-- > 0){
				FCS = crctable[FCS ^ (*data)];
				data ++;
			}
		}
	}
	pns_Checksum.Write(cgStartup.ios,0xFF - FCS);
}
//------------------------------------------------------------------------------------------//
UIH_FRAME& UIH_FRAME::AddUIHFrame(uint32* sendNum,STDSTR* retHexFrame,uint8 vPort,const uint8* data,uint32 num,uint8 crBit){
	cgPosWR.array->Prepare_Set();
	_Begin(nullptr);
	
	pn_Addr.Write(nullptr,(vPort << 2) | crBit | EA_BIT);	//1:Address field
	pn_Ctrl.Write(nullptr,CTRL_UIH & (~PF_BIT));			//2:Ctrl field,Poll/Final BIT always 1
	
	if (num > INFO_SIZE_MAX)
		num = INFO_SIZE_MAX;
	if (num < FRAME_SIZE_TWOBYTES){
		pn_Len.WriteByte1(nullptr,(num << 1) | EA_BIT);			//3:Length indicator
	}
	else{
		pn_Len.WriteByte2(nullptr,(num << 1) );
	}
	
	pn_Info.Write(nullptr,IUD(data, num));					//4:Info

	_Endl();
	
	if (retHexFrame != nullptr){
		uint32	slength,length,offset;
		uint8*	data;
		
		length = GetLengthWR();
		offset = GetOffsetWR();
		slength = cgPosWR.array->CalcOutLengthInToPre(length,offset);
		
		if (length > 0){
			data = cgPosWR.array->GetPointer(offset);
			Str_CharToHEX(retHexFrame, data, length, G_SPACE_ON);
			if (slength > 0){
				data = cgPosWR.array->GetPointer(0);
				Str_CharToHEX(retHexFrame, data, slength, G_SPACE_ON);
			}
		}
	}
	cgPosWR.array->Prepare_Clr();
	if (sendNum != nullptr)
		*sendNum = num;
	return(*this);
}
//------------------------------------------------------------------------------------------//
UIH_FRAME& UIH_FRAME::SetUIHFrame(uint32* sendNum,STDSTR* retHexFrame,uint8 vPort,const uint8* data,uint32 num,uint8 crBit){
	uint32 snum,total;
	snum = 0;
	total = 0;
	while((num > 0) && (cgPosWR.array->Unused() > FRAME_SIZE_MAX)){
		AddUIHFrame(&snum,retHexFrame,vPort,data,num,crBit);
		data += snum;
		total += snum;
		num -= snum;
	}
	if (sendNum != nullptr)
		*sendNum = total;
	return(*this);
}
//------------------------------------------------------------------------------------------//
UIH_FRAME& UIH_FRAME::SetUIHFrame(uint32* sendNum,STDSTR* retHexFrame,uint8 vPort,const ARRAY& _in,uint8 crBit){
	uint32	slength,num,offset,snum,total;
	
	snum = 0;
	total = 0;
	num = _in.Used();
	offset = 0;
	slength = _in.CalcOutLength(num, offset);
	
	do{
		if (num > 0){
			SetUIHFrame(&snum,retHexFrame,vPort,_in.GetPointer(offset),num,crBit);
			total += snum;
			if (num > snum)
				break;
		}
		if (slength > 0){
			SetUIHFrame(&snum,retHexFrame,vPort,_in.GetPointer(0),slength,crBit);
			total += snum;
		}
		break;
	}while(0);
	if (sendNum != nullptr)
		*sendNum = total;
	return(*this);
}
//------------------------------------------------------------------------------------------//
UIH_FRAME& UIH_FRAME::SetCtrlFrame(STDSTR* retHexFrame,uint8 vPort,uint8 ctrlType,uint8 crBit){
	cgPosWR.array->Prepare_Set();
	_Begin(nullptr);
	
	pn_Addr.Write(nullptr,(vPort << 2) | crBit | EA_BIT);	//1:Address field
	pn_Ctrl.Write(nullptr,ctrlType | PF_BIT);				//2:Ctrl field,Poll/Final BIT always 1
	pn_Len.WriteByte1(nullptr,EA_BIT);						//3:Length indicator
	pn_Info.WriteNone();									//4:Info
	
	_Endl();
	
	if (retHexFrame != nullptr){
		uint32	slength,length,offset;
		uint8*	data;
		
		length = GetLengthWR();
		offset = GetOffsetWR();
		slength = cgPosWR.array->CalcOutLengthInToPre(length,offset);
		
		if (length > 0){
			data = cgPosWR.array->GetPointer(offset);
			Str_CharToHEX(retHexFrame, data, length, G_SPACE_ON);
			if (slength > 0){
				data = cgPosWR.array->GetPointer(0);
				Str_CharToHEX(retHexFrame, data, slength, G_SPACE_ON);
			}
		}
	}
	cgPosWR.array->Prepare_Clr();
	return(*this);
}
//------------------------------------------------------------------------------------------//
UIH_FRAME& UIH_FRAME::SetCLDFrame(STDSTR* retHexFrame){
	uint8	data[2];
	
	data[0] = CMD_MUX_CLD | CR_BIT | EA_BIT;
	data[1] = EA_BIT;
	
	return(SetUIHFrame(nullptr,retHexFrame,DLCI0,data,2,CR_BIT));
}
//------------------------------------------------------------------------------------------//
UIH_FRAME& UIH_FRAME::SetFCONFrame(STDSTR* retHexFrame,uint8 crBit){
	uint8	data[2];
	
	data[0] = CMD_FCON | crBit | EA_BIT;
	data[1] = EA_BIT;
	
	return(SetUIHFrame(nullptr,retHexFrame,DLCI0,data,2,CR_BIT));
}
//------------------------------------------------------------------------------------------//
UIH_FRAME& UIH_FRAME::SetFCOFFFrame(STDSTR* retHexFrame,uint8 crBit){
	uint8	data[2];
	
	data[0] = CMD_FCOFF | crBit | EA_BIT;
	data[1] = EA_BIT;
	
	return(SetUIHFrame(nullptr,retHexFrame,DLCI0,data,2,CR_BIT));
}
//------------------------------------------------------------------------------------------//
UIH_FRAME& UIH_FRAME::SetStdPSCFrame(STDSTR* retHexFrame){
	uint8	data[2];
	
	data[0] = CMD_PSC | CR_BIT | EA_BIT;
	data[1] = EA_BIT;
	
	return(SetUIHFrame(nullptr,retHexFrame,DLCI0,data,2,CR_BIT));
}
//------------------------------------------------------------------------------------------//
UIH_FRAME& UIH_FRAME::SetTPSCxFrame(STDSTR* retHexFrame,uint8 mode){
	uint8	data[3];
	
	data[0] = CMD_PSC | CR_BIT | EA_BIT;
	data[1] = (0x01 << 1) | EA_BIT;
	data[2] = mode;
	
	return(SetUIHFrame(nullptr,retHexFrame,DLCI0,data,3,CR_BIT));
}
//------------------------------------------------------------------------------------------//
UIH_FRAME& UIH_FRAME::SetMSCCmd(STDSTR* retHexFrame,uint8 vPort,uint8 modemStatus,uint8 UINcrbit,uint8 crBit){
	uint8	data[4];
	
	data[0] = CMD_MSC | crBit | EA_BIT;  		// Cmd type
	data[1] = (0x02 << 1) | EA_BIT;				// Cmd Len Ind
	data[2] = (vPort << 2) | 0x02 | EA_BIT;   	// valore 1
	data[3] = modemStatus | 0x01;
	
	return(SetUIHFrame(nullptr,retHexFrame,DLCI0,data,4,UINcrbit));
}
//------------------------------------------------------------------------------------------//
UIH_FRAME& UIH_FRAME::SetBRKCmd(STDSTR* retHexFrame,uint8 vPort,uint8 modemStatus,uint8 UINcrbit,uint8 crBit){
	uint8	data[5];
	
	data[0] = CMD_MSC | crBit | EA_BIT;  		// Cmd type
	data[1] = (3 << 1) | EA_BIT;				// Cmd Len Ind
	data[2] = (vPort << 2) | 0x02 | EA_BIT;		// valore 1
	data[3] = modemStatus | 0x01;
	data[4] = BREAK_OCTET;
	
	return(SetUIHFrame(nullptr,retHexFrame,DLCI0,data,5,UINcrbit));
}
//------------------------------------------------------------------------------------------//
UIH_FRAME& UIH_FRAME::SetSABMFrameD0(STDSTR* retHexFrame){return(SetCtrlFrame(retHexFrame,DLCI0,CTRL_SABM,CR_BIT));};
UIH_FRAME& UIH_FRAME::SetSABMFrameV1(STDSTR* retHexFrame){return(SetCtrlFrame(retHexFrame,DLCI1,CTRL_SABM,CR_BIT));};
UIH_FRAME& UIH_FRAME::SetSABMFrameV2(STDSTR* retHexFrame){return(SetCtrlFrame(retHexFrame,DLCI2,CTRL_SABM,CR_BIT));};
UIH_FRAME& UIH_FRAME::SetSABMFrameV3(STDSTR* retHexFrame){return(SetCtrlFrame(retHexFrame,DLCI3,CTRL_SABM,CR_BIT));};
UIH_FRAME& UIH_FRAME::SetSABMFrameV4(STDSTR* retHexFrame){return(SetCtrlFrame(retHexFrame,DLCI4,CTRL_SABM,CR_BIT));};

UIH_FRAME& UIH_FRAME::SetDISCFrameD0(STDSTR* retHexFrame){return(SetCtrlFrame(retHexFrame,DLCI0,CTRL_DISC,CR_BIT));};
UIH_FRAME& UIH_FRAME::SetDISCFrameV1(STDSTR* retHexFrame){return(SetCtrlFrame(retHexFrame,DLCI1,CTRL_DISC,CR_BIT));};
UIH_FRAME& UIH_FRAME::SetDISCFrameV2(STDSTR* retHexFrame){return(SetCtrlFrame(retHexFrame,DLCI2,CTRL_DISC,CR_BIT));};
UIH_FRAME& UIH_FRAME::SetDISCFrameV3(STDSTR* retHexFrame){return(SetCtrlFrame(retHexFrame,DLCI3,CTRL_DISC,CR_BIT));};
UIH_FRAME& UIH_FRAME::SetDISCFrameV4(STDSTR* retHexFrame){return(SetCtrlFrame(retHexFrame,DLCI4,CTRL_DISC,CR_BIT));};

UIH_FRAME& UIH_FRAME::SetDefaultMSV1(STDSTR* retHexFrame){return(SetMSCCmd(retHexFrame,DLCI1,MS_DTR | MS_RTS,CR_BIT,CR_BIT));};
UIH_FRAME& UIH_FRAME::SetDefaultMSV2(STDSTR* retHexFrame){return(SetMSCCmd(retHexFrame,DLCI2,MS_DTR | MS_RTS,CR_BIT,CR_BIT));};
UIH_FRAME& UIH_FRAME::SetDefaultMSV3(STDSTR* retHexFrame){return(SetMSCCmd(retHexFrame,DLCI3,MS_DTR | MS_RTS,CR_BIT,CR_BIT));};
UIH_FRAME& UIH_FRAME::SetDefaultMSV4(STDSTR* retHexFrame){return(SetMSCCmd(retHexFrame,DLCI4,MS_DTR | MS_RTS,CR_BIT,CR_BIT));};
//------------------------------------------------------------------------------------------//
const STDSTR& UIH_FRAME::CreateFCONFrame(STDSTR* retHexFrame,uint8 crBit){
	*retHexFrame = "";
	InUse_set();
	SetFCONFrame(retHexFrame,crBit);
	InUse_clr();
	return(*retHexFrame);
};
//------------------------------------------------------------------------------------------//
const STDSTR& UIH_FRAME::CreateFCOFFFrame(STDSTR* retHexFrame,uint8 crBit){
	*retHexFrame = "";
	InUse_set();
	SetFCOFFFrame(retHexFrame,crBit);
	InUse_clr();
	return(*retHexFrame);
};
//------------------------------------------------------------------------------------------//
const STDSTR& UIH_FRAME::CreateTPSCxFrame(STDSTR* retHexFrame,uint8 mode){
	*retHexFrame = "";
	InUse_set();
	SetTPSCxFrame(retHexFrame,mode);
	InUse_clr();
	return(*retHexFrame);
};
//------------------------------------------------------------------------------------------//
const STDSTR& UIH_FRAME::CreateMSCCmd(STDSTR* retHexFrame,uint8 vPort,uint8 modemStatus,uint8 UINcrbit,uint8 crBit){
	*retHexFrame = "";
	InUse_set();
	SetMSCCmd(retHexFrame,vPort,modemStatus,UINcrbit,crBit);
	InUse_clr();
	return(*retHexFrame);
};
//------------------------------------------------------------------------------------------//
const STDSTR& UIH_FRAME::CreateBRKCmd(STDSTR* retHexFrame,uint8 vPort,uint8 modemStatus,uint8 UINcrbit,uint8 crBit){
	*retHexFrame = "";
	InUse_set();
	SetBRKCmd(retHexFrame,vPort,modemStatus,UINcrbit,crBit);
	InUse_clr();
	return(*retHexFrame);
};
//------------------------------------------------------------------------------------------//
#define CMUXFUN(_name)\
const STDSTR& UIH_FRAME::Create##_name(STDSTR* retHexFrame){\
	*retHexFrame = "";\
	InUse_set();\
	Set##_name(retHexFrame);\
	InUse_clr();\
	return(*retHexFrame);\
};
//------------------------------------------------------------------------------------------//
CMUXFUN(CLDFrame);
CMUXFUN(StdPSCFrame);

CMUXFUN(SABMFrameD0);
CMUXFUN(SABMFrameV1);
CMUXFUN(SABMFrameV2);
CMUXFUN(SABMFrameV3);
CMUXFUN(SABMFrameV4);

CMUXFUN(DISCFrameD0);
CMUXFUN(DISCFrameV1);
CMUXFUN(DISCFrameV2);
CMUXFUN(DISCFrameV3);
CMUXFUN(DISCFrameV4);

CMUXFUN(DefaultMSV1);
CMUXFUN(DefaultMSV2);
CMUXFUN(DefaultMSV3);
CMUXFUN(DefaultMSV4);
//------------------------------------------------------------------------------------------//
};










//------------------------------------------------------------------------------------------//
VCOM::VCOM(uint32 size,const ODEV_SYSTEM* logSys) : ACOM(size,logSys){
	cgPortID = 0;
	ClrSFlag(VCOM_blDSR | VCOM_blCTS | VCOM_blDCD | VCOM_blRING | VCOM_blDTR | VCOM_blRTS | VCOM_blHEX | VCOM_blEscape);
	SetSFlag(VCOM_blEscape);

	cmuxThread.ThreadInit(this, &VCOM::cmuxThreadFun,"cmux");
	
	cgThreadList < cmuxThread;
	modemStatusThread.Disable();
	DisableLog();
	
	TNFP::SetSelfName("VCOM");
	SetSelfName(selfName);
	SetFatherName("");
};
//------------------------------------------------------------------------------------------//
VCOM::~VCOM(void){
	Close();
	cmuxThread.ThreadStop();
	cmuxThread.RemoveSelf();
};
//------------------------------------------------------------------------------------------//
void VCOM::SetSelfName(const STDSTR& strName){
	ACOM::SetSelfName(strName);
	cmuxThread.SetFatherName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
void VCOM::SetFatherName(const STDSTR& strName){
	ACOM::SetFatherName(strName);
	cmuxThread.SetFatherName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
bool32 VCOM::DoOpen(const OPEN_PAR& par){
	ClrSFlag(VCOM_blDSR | VCOM_blCTS | VCOM_blDCD | VCOM_blRING | VCOM_blDTR | VCOM_blRTS | VCOM_blHEX | VCOM_blEscape);
	SetSFlag(VCOM_blEscape);

#ifdef CommonDefH_VC
	return(ACOM::DoOpen(par));
#endif
#ifdef CommonDefH_Unix
	OPEN_PAR	oPar;

	SetOpenPar(&oPar, par);
	oPar.type = OPEN_COMV;
	oPar.name = CMUX_COM_NAME;
	oPar.name += Str_ToStr(cgPortID);
	return(ACOM::DoOpen(oPar));
#endif
}
//------------------------------------------------------------------------------------------//
void VCOM::DoPrintOnOpenSuccess(void){
#ifdef CommonDefH_Unix
	PrintMessageDot("Open CMUX virtual port",Str_ToStr(cgPortID),":",vPortName);
#else
	PrintMessageDot("Open CMUX virtual port",Str_ToStr(cgPortID),":",GetOpenPar().name);
#endif
}
//------------------------------------------------------------------------------------------//
void VCOM::DoPrintOnClose(void){
#ifdef CommonDefH_Unix
	PrintMessageDot("Close CMUX virtual port",Str_ToStr(cgPortID),":",vPortName);
#else
	PrintMessageDot("Open CMUX virtual port",Str_ToStr(cgPortID),":",GetOpenPar().name);
#endif
}
//------------------------------------------------------------------------------------------//
void VCOM::Update_DSR(bool32 bl){
	if (bl > 0){
		SetSFlag(VCOM_blDSR);
	}
	else{
		ClrSFlag(VCOM_blDSR);
	}
}
//------------------------------------------------------------------------------------------//
void VCOM::Update_CTS(bool32 bl){
	if (bl > 0){
		SetSFlag(VCOM_blCTS);
	}
	else{
		ClrSFlag(VCOM_blCTS);
	}
}
//------------------------------------------------------------------------------------------//
void VCOM::Update_DCD(bool32 bl){
	if (bl > 0){
		SetSFlag(VCOM_blDCD);
	}
	else{
		ClrSFlag(VCOM_blDCD);
	}
}
//------------------------------------------------------------------------------------------//
void VCOM::Update_RING(bool32 bl){
	if (bl > 0){
		SetSFlag(VCOM_blRING);
	}
	else{
		ClrSFlag(VCOM_blRING);
	}
}
//------------------------------------------------------------------------------------------//
void VCOM::Update_DTR(bool32 bl){
	if (bl > 0){
		SetSFlag(VCOM_blDTR);
	}
	else{
		ClrSFlag(VCOM_blDTR);
	}
}
//------------------------------------------------------------------------------------------//
void VCOM::Update_RTS(bool32 bl){
	if (bl > 0){
		SetSFlag(VCOM_blRTS);
	}
	else{
		ClrSFlag(VCOM_blRTS);
	}
}
//------------------------------------------------------------------------------------------//
void VCOM::Update_HEX(bool32 bl){
	if (bl > 0){
		SetSFlag(VCOM_blHEX);
	}
	else{
		ClrSFlag(VCOM_blHEX);
	}
}
//------------------------------------------------------------------------------------------//
void VCOM::Update_Escape(bool32 bl){
	if (bl > 0){
		SetSFlag(VCOM_blEscape);
	}
	else{
		ClrSFlag(VCOM_blEscape);
	}
}
//------------------------------------------------------------------------------------------//
bool32	VCOM::Check_DSR		(void){return(CheckSFlag(VCOM_blDSR));};
bool32	VCOM::Check_CTS		(void){return(CheckSFlag(VCOM_blCTS));};
bool32	VCOM::Check_DCD		(void){return(CheckSFlag(VCOM_blDCD));};
bool32	VCOM::Check_RING	(void){return(CheckSFlag(VCOM_blRING));};
bool32	VCOM::Check_DTR		(void){return(CheckSFlag(VCOM_blDTR));};
bool32	VCOM::Check_RTS		(void){return(CheckSFlag(VCOM_blRTS));};
bool32	VCOM::Check_HEX		(void){return(CheckSFlag(VCOM_blHEX));};
bool32	VCOM::Check_Escape	(void){return(CheckSFlag(VCOM_blEscape));};
//------------------------------------------------------------------------------------------//
bool32 VCOM::cmuxThreadFun(void* commu){
	uint32		byteNum;
	STDSTR		strRecData;
	
	ETLogThreadStart(cmuxThread);
	SetGetDataByRead();
	while(cmuxThread.IsTerminated() == 0){
		byteNum = cgRxSBUF.Used();
		if (byteNum == 3){
			cgRxSBUF.cgArray.Read(_EMPTY(&strRecData),3,0);
			if (strRecData == "+++")
				static_cast<CMUXDriver*>(GetFDB())->Send3Pluse(this);
		}
		while(cgRxSBUF.Used() > 0){
			byteNum = static_cast<CMUXDriver*>(GetFDB())->Send(this,cgRxSBUF.cgArray);
			cgRxSBUF.Out(byteNum);
		}
		SYS_SleepMS(10);
	}
	ETLogThreadStart(cmuxThread);
	return G_TRUE;
}
//------------------------------------------------------------------------------------------//
const STDSTR& VCOM::DlciStatus(STDSTR* retStr){
	*retStr = " DLCI " + Str_ToStr(cgPortID) + " :";
	*retStr += " DTR=";	*retStr += (Check_DTR()  == G_FALSE)?"L,":"H,";
	*retStr += " RTS=";	*retStr += (Check_RTS()  == G_FALSE)?"L,":"H,";
	*retStr += " CTS=";	*retStr += (Check_CTS()  == G_FALSE)?"L,":"H,";
	*retStr += " DSR=";	*retStr += (Check_DSR()  == G_FALSE)?"L,":"H,";
	*retStr += " RING=";*retStr += (Check_RING() == G_FALSE)?"L,":"H,";
	*retStr += " DCD=";	*retStr += (Check_DCD()  == G_FALSE)?"L,":"H,";
	*retStr += (Check_HEX() == G_FALSE)?" ASCII mode,":" HEX mode,";
	*retStr += (Check_Escape() == G_FALSE)?" disable escape":" enable escape";
	return(*retStr);
}
//------------------------------------------------------------------------------------------//









using namespace CMUX;
//------------------------------------------------------------------------------------------//
CMUXDriver::CMUXDriver(uint32 size,const DEVICE* dev) : COMMU_THREAD(size,nullptr){
	Init(dev);

	cgTxUIH.InitPN(&cgTxSBUF.cgArray, &cgTxSBUF.cgArray);
	cgRxUIH.InitPN(&cgRxSBUF.cgArray, &cgRxSBUF.cgArray);
	
	SetSFlag(blStartWithAT);
	ClrSFlag(blInitWithThread);

	TNFP::SetSelfName("CMUXDriver");
	SetSelfName(selfName);
	SetFatherName("");
};
//------------------------------------------------------------------------------------------//
CMUXDriver::~CMUXDriver(void){
	Close();
	CleanChild(this, this);
};
//------------------------------------------------------------------------------------------//
void CMUXDriver::Init(const DEVICE* dev){
	cgDevice = (DEVICE*)dev;
	if (dev != nullptr)
		InitLogSys(dev->GetLogSystem());
};
//------------------------------------------------------------------------------------------//
void CMUXDriver::SetSelfName(const STDSTR& strName){
	COMMU_THREAD::SetSelfName(strName);
	commandThread.SetFatherName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
void CMUXDriver::SetFatherName(const STDSTR& strName){
	COMMU_THREAD::SetFatherName(strName);
	commandThread.SetFatherName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
TNFP* CMUXDriver::CreateNode(void){
	
	return(SetSubNodeFatherName(new VCOM(cgMaxSize,GetLogSystem())));
};
//------------------------------------------------------------------------------------------//
void CMUXDriver::DoPrintOnOpenSuccess(void){
	
	PrintMessageDot("CMUX driver start success");
};
//------------------------------------------------------------------------------------------//
void CMUXDriver::DoPrintOnOpenFail(void){
	
	PrintMessageDot("CMUX driver start fail");
};
//------------------------------------------------------------------------------------------//
void CMUXDriver::DoPrintOnClose(void){
	
	PrintMessageDot("CMUX driver stop");
};
//------------------------------------------------------------------------------------------//
VCOM* CMUXDriver::GetVCOM(int32 dlci){
	VCOM	*vcom;
	TREE_LChildRChain_Find(VCOM,this,vcom,(_opNode->cgPortID == dlci));
	return (vcom);
}
//------------------------------------------------------------------------------------------//
VCOM* CMUXDriver::OpenVCOM(int32 dlci,const STDSTR& name,uint32 baudrate){
	VCOM	*vcom;
	
	if ((dlci > Max_CMUXCOM) || (dlci == 0))
		return(nullptr);
	
	vcom = GetVCOM(dlci);
	if (vcom == nullptr){
		vcom = static_cast<VCOM*>(GetNewNode());
		if (vcom != nullptr){
			vcom->cgPortID = dlci;
			AddNode(vcom);
		}
	}
	if (vcom != nullptr){
		if (vcom->Open(SetOpenPar(OPEN_COM,name,baudrate,0)) == G_FALSE)
			vcom = nullptr;
	}
	return(vcom);
}
//------------------------------------------------------------------------------------------//
void CMUXDriver::CloseVCOM(int32 dlci){
	VCOM	*vcom;
	if (dlci > Max_CMUXCOM)
		return;
	InUse_set();
	vcom = GetVCOM(dlci);
	
	if (vcom != nullptr)
		vcom->Close();
	InUse_clr();
}
//------------------------------------------------------------------------------------------//
bool32 CMUXDriver::OpenDev(const OPEN_PAR& par){
#ifdef CommonDefH_Unix
	OpenVCOM(1,"",115200);
	OpenVCOM(2,"",115200);
	OpenVCOM(3,"",115200);
	OpenVCOM(4,"",115200);
#endif
	return G_TRUE;
}
//------------------------------------------------------------------------------------------//
bool32 CMUXDriver::DoOpen(const OPEN_PAR& par){
	ELog(this << "CMUXDriver::DoOpen()");
	if (COMMU_THREAD::DoOpen(par)){
		if (CheckSFlag(blInitWithThread)){
			if (CheckSFlag(blStartWithAT)){
				CMUXStartWithAT();
			}
			else{
				CMUXStart();
			}
			return G_TRUE;
		}
		if (CheckSFlag(blStartWithAT) == 0)
			return(CMUXStartCT());
		if (CMUXInitAT() > 0)
			return(CMUXStartCT());
	}
	return G_FALSE;
}
//------------------------------------------------------------------------------------------//
bool32 CMUXDriver::Open(bool32 blInitAT,bool32 blInitUseThread){
	bool32	ret;
	ret = 0;
	if (InDoing_try()){
		if (cgDevice->cgEDA.IsComOpened()){
			ClrSFlag(blStartWithAT | blInitWithThread);
			if (blInitAT)
				SetSFlag(blStartWithAT);
			if (blInitUseThread)
				SetSFlag(blInitWithThread);
			ret = COMMU_THREAD::Open(SetOpenPar(OPEN_None,"CMUX",999,0),G_LOCK_OFF);
		}
		InDoing_clr();
	}
	return(ret);
}
//------------------------------------------------------------------------------------------//
void CMUXDriver::CloseDev(void){
	if (cgDevice->IsOpened() && IsOpened())
		CMUXCloseCT();
	commandThread.ThreadStop();
	if (cgDevice->ACom() != nullptr)
		cgDevice->ACom()->EnableLog();
}
//------------------------------------------------------------------------------------------//
bool32 CMUXDriver::TxThreadFun(void* commu){
	STDSTR		strData,strFrame,strDataT;
	VCOM		*vcom;
	uint32		dlci;
	uint8		ctrlType;
	
	ETLogThreadStart(txThread);
	while(txThread.IsTerminated() == 0){
		while(cgTxSBUF.Used() > 0){
			while(cgTxUIH.TryGetFrame() > 0){
				cgTxUIH.Read(nullptr, _EMPTY(&strFrame));
				
				ctrlType = cgTxUIH.pn_Ctrl_Type.GetValueAMaskRE();
				dlci	 = cgTxUIH.pn_Addr_DLCI.GetValueCalcRE();
				
				if ((ctrlType == CTRL_UIH) && (dlci > 0)){
					vcom = GetVCOM(dlci);
					if (vcom != nullptr){
						if (vcom->Check_HEX()){
							cgTxUIH.pn_Info.Read(nullptr,OUD_HEXs(_EMPTY(&strData)));
							Str_RTrimSelf(strData);
						}
						else{
							cgTxUIH.pn_Info.Read(nullptr,_EMPTY(&strData));
							if (vcom->Check_Escape())
								strData = Str_UnEscapeToStr(_EMPTY(&strDataT),strData);
							
						}
					}
					PrintFrame(COLOR(COL_clDCyan,strData),"CMUXThread::Received data from DLCI",Str_ToStr(dlci));
					PrintWithTime(COL_clDCyan,"CMUXThread::Send CMUX frame:\n",COLOR(COL_clDCyan,Str_ASCIIToHEXs(strFrame, G_ESCAPE_OFF)),"\n");
				}
				cgDevice->SendCommand(strFrame, CMD_NONE, G_ESCAPE_OFF);
				cgTxUIH.Out();
			};
		}
		SYS_SleepMS(10);
	}
	ETLogThreadStop(txThread);
	return G_TRUE;
}
//------------------------------------------------------------------------------------------//
bool32 CMUXDriver::RxThreadFun(void* commu){
	STDSTR	strFrame,strInfo;
	SMC_EncryptI(0)
	SMC_EncryptS(0)
	cgDevice->RxDataShareTo(&cgRxSBUF);
	SMC_EncryptE(0)
	ETLogThreadStart(rxThread);
	while(rxThread.IsTerminated() == 0){
		SYS_SleepMS(10);
		while(cgRxSBUF.Used() > 0){
			if (cgRxUIH.TryGetFrame()){
				cgRxUIH.Read(nullptr,OUD_HEXs(_EMPTY(&strFrame)));
				DeliveryRxFrame(&cgRxUIH,strFrame,strInfo);
				cgRxUIH.Out();
			}
		}
	}
	cgRxSBUF.RemoveSelf();
	ETLogThreadStop(rxThread);
	return G_TRUE;
}
//------------------------------------------------------------------------------------------//
void CMUXDriver::DeliveryRxFrame(UIH_FRAME* rxUIH,const STDSTR& strFrame,STDSTR& strInfo){
	VCOM		*vcom;
	uint8		ctrlType;
	uint32		dlci;
	
	dlci = rxUIH->pn_Addr_DLCI.GetValueCalcRE();	// Check DLCI: Addr field:  |dlci|C/R|E/A|
	
	if (dlci > Max_CMUXCOM){
		PrintFrame(strFrame,"CMUXThread::DLCI vaule is invaild");
		return;
	}

	ctrlType = rxUIH->pn_Ctrl_Type.GetValueAMaskRE();//Check CTRL: CTRL field:  |X|X|X|X|P/F|X|X|X|,Suppose P/F bit in control field always = 0
	
	switch(ctrlType){
		case CTRL_SABM:
			PrintFrame(strFrame,"CMUXThread::Received SABM frame, MS wants connect to me, no implemented Ctrl Type");
			break;
		case CTRL_UA:
			PrintFrame(strFrame,"CMUXThread::Received UA response frame on DLCI",Str_ToStr(dlci));
			break;
		case CTRL_DM:
			PrintFrame(strFrame,"CMUXThread::Received DM response frame on DLCI",Str_ToStr(dlci));
			break;
		case CTRL_DISC:
			PrintFrame(strFrame,"CMUXThread::Received DISC frame, MS wants disconnect to me, no implemented Ctrl Type");
			break;
		case CTRL_UIH:
			if (dlci == DLCI0){
				CMuxCtrlCmdExec(rxUIH,strFrame,strInfo);
			}
			else{
				PrintFrame(strFrame,"CMUXThread::Received UIH frame on DLCI",Str_ToStr(dlci));
				vcom = GetVCOM(dlci);
				if (vcom != nullptr){
					rxUIH->pn_Info.Read(nullptr, _EMPTY(&strInfo));
					vcom->Send(strInfo);
				}
			}
			break;
		case CTRL_UI:
			PrintFrame(strFrame,"CMUXThread::Received UI frame received, no implemented Ctrl Type");
			break;
		default:
			PrintFrame(strFrame,"CMUXThread::CTRL Field |X|X|X|X|P/F|X|X|X| is invaild");
			break;
	}
}
//------------------------------------------------------------------------------------------//
void CMUXDriver::CMuxCtrlCmdExec(UIH_FRAME* rxUIH,const STDSTR& strFrame,STDSTR& strInfo){
	//There is no specified response to the UIH cpmmand/response
	//MSC Command Application->Module => C/R bit (RED) 1 and C/R bit (BLUE) 1
	//MSC Command Module->Application => C/R bit (RED) 0 and C/R bit (BLUE) 1
	//MSC Response Application->Module => C/R bit (RED) 1 and C/R bit (BLUE) 0
	//MSC Response Module->Application => C/R bit (RED) 0 and C/R bit (BLUE) 0
	
	uint8 	charTemp;
	STDSTR	strMSC;
	uint32	cMSCCRbit,cMSCDlci,cMS,frameCRbit,cmdType;
	
	rxUIH->pn_Info.Read(nullptr, _EMPTY(&strInfo));
	charTemp = strInfo[0];
	cmdType = charTemp & ~(CR_BIT | EA_BIT);
	switch(cmdType){
		case CMD_MSC:{		// Modem Status Command
			cMSCCRbit = charTemp & CR_BIT;
			cMSCDlci  = strInfo[2] >> 2;											// Get DLCI from frame
			cMS 	  = strInfo[3];
			frameCRbit = rxUIH->pn_Addr_CR.GetValueAMaskRE();						//addr & CMUX_CR_BIT;
			if ((frameCRbit == 0) && (cMSCCRbit != 0)){								//MSC Command Module->Application => C/R bit (RED) 0 and C/R bit (BLUE) 1
				
				PrintFrame(strFrame,"CMUXThread::Received MSC update frame on DLCI",Str_ToStr(cMSCDlci));
				PrintWithTime(COL_clDCyan,"CMUXThread::",UpdateModemStatus(&strMSC,cMSCDlci,cMS));
				
				cgTxUIH.CreateMSCCmd(&strMSC,cMSCDlci,cMS,CR_BIT,0);//MSC Response Application->Module => C/R bit (RED) 1 and C/R bit (BLUE) 0
				PrintWithTime(COL_clDCyan,"CMUXThread::Send MSC confirm frame:\n",COLOR(COL_clDCyan,strMSC),"\n");
			}
			else{
				PrintFrame(strFrame,"CMUXThread::Received MSC response on DLCI",Str_ToStr(cMSCDlci));
			}
			break;
		}
		case CMD_PSC:{		// Power Saving Control: Not Impl.
			cMSCCRbit = charTemp & CR_BIT;
			if (cMSCCRbit == 0){
				PrintFrame(strFrame,"CMUXThread::Received PSC response frame");
			}
			else{
				PrintFrame(strFrame,"CMUXThread::Received PSC frame, no supported in current version");
			}
			break;
		}
		case CMD_TEST:		// Test command
			PrintFrame(strFrame,"CMUXThread::Received Test Command, no supported in current version");
			break;
		case NSC_RES:		// Not Supported Command: Not Impl.
			PrintFrame(strFrame,"CMUXThread::Received NSC CMD, no supported CMD");
			break;
		case CMD_PN:		// Parameter Negotiation: Not Impl.
			PrintFrame(strFrame,"CMUXThread::Received Parameter Negotiation CMD, no supported CMD");
			break;
		case CMD_MUX_CLD:	// CMux Close Down: Not Impl.
			PrintFrame(strFrame,"CMUXThread::Received CMux Close Down CMD, no supported CMD");
			break;
		case CMD_FCON:		// Flow Control ON: Not Impl.
			PrintFrame(strFrame,"CMUXThread::Received Flow Control ON CMD, no supported CMD");
			break;
		case CMD_FCOFF:		// Flow Control OFF: Not Impl.
			PrintFrame(strFrame,"CMUXThread::Received Flow Control OFF CMD, no supported CMD");
			break;
		case CMD_RPN:		// Remote Port Negotiation: Not Impl.
			PrintFrame(strFrame,"CMUXThread::Received Remote Port Negotiation CMD, no supported CMD");
			break;
		case CMD_RLS:		// Remote Line Status: Not Impl.
			PrintFrame(strFrame,"CMUXThread::Received Remote Line Status CMD, no supported CMD");
			break;
		case CMD_SNC:		// Service Negotiation Command: Not Impl.
			PrintFrame(strFrame,"CMUXThread::Received Service Negotiation CMD, no supported CMD");
			break;
		default:
			PrintFrame(strFrame,"CMUXThread::Received Other no supported CMD");
	}
}
//------------------------------------------------------------------------------------------//
const STDSTR& CMUXDriver::UpdateModemStatus(STDSTR* strForPrint,uint32 cMSCDlci,uint32 cMS){
	VCOM* vcom;
	
	vcom = GetVCOM(cMSCDlci);
	
	if (cMS & MS_CTS){
		if (vcom != nullptr)
			vcom->Update_CTS(0);
		*strForPrint = "CTS=0";
	}
	else{
		if (vcom != nullptr)
			vcom->Update_CTS(1);
		*strForPrint = "CTS=1";
	}
	if (cMS & MS_DSR){
		if (vcom != nullptr)
			vcom->Update_DSR(0);
		*strForPrint += ",DSR=0";
	}
	else{
		if (vcom != nullptr)
			vcom->Update_DSR(1);
		*strForPrint += ",DSR=1";
	}
	if (cMS & MS_RING){
		if (vcom != nullptr)
			vcom->Update_RING(0);
		*strForPrint += ",RING=0";
	}
	else{
		if (vcom != nullptr)
			vcom->Update_RING(1);
		*strForPrint += ",RING=1";
	}
	if (cMS & MS_DCD){
		if (vcom != nullptr)
			vcom->Update_DCD(0);
		*strForPrint += ",DCD=0";
	}
	else{
		if (vcom != nullptr)
			vcom->Update_DCD(1);
		*strForPrint += ",DCD=1";
	}
	
	return(*strForPrint);
}
//------------------------------------------------------------------------------------------//
bool32 CMUXDriver::CMUXInitAT(void){
	bool32	blOK;
	SBUF	cBuffer;
	cBuffer.InitSize(128);
	cBuffer.Empty();
	cgDevice->RxDataShareTo(&cBuffer);
	blOK = G_FALSE;
	B_ClrFLAG64(cgDevice->GetLogSystem()->envcfg, ODEV_FLAG_EnHEXViewMode);
	do{
		if (SendATCMD(" ","AT"			,"OK",&cBuffer,1000,100) == G_FALSE) break;
		if (SendATCMD(" ","AT"			,"OK",&cBuffer,1000,100) == G_FALSE) break;
		if (SendATCMD(" ","AT"			,"OK",&cBuffer,1000,100) == G_FALSE) break;
		if (SendATCMD(" ","AT+IPR=115200","OK",&cBuffer,1000,100) == G_FALSE) break;
		if (SendATCMD(" ","ATE0V1&K3&D2","OK",&cBuffer,1000,100) == G_FALSE) break;
		if (SendATCMD(" ","AT+CMEE=2"	,"OK",&cBuffer,1000,100) == G_FALSE) break;
		if (SendATCMD(" ","AT+CGMI"		,"OK",&cBuffer,1000,100) == G_FALSE) break;
		if (SendATCMD(" ","AT+CGMM"		,"OK",&cBuffer,1000,100) == G_FALSE) break;
		if (SendATCMD(" ","AT+CGMR"		,"OK",&cBuffer,1000,100) == G_FALSE) break;
		SendATCMD(" ","AT#SELINT=2"		,"OK",&cBuffer,1000,100);
		SendATCMD(" ","AT#CMUXMODE=1"	,"OK",&cBuffer,1000,100);
		blOK = G_TRUE;
	}while(0);
	PrintWithDividingLine("CMUXDriver::Finish the external initialization");
	cBuffer.RemoveSelf();
	return(blOK);
}
//------------------------------------------------------------------------------------------//
bool32 CMUXDriver::CMUXStartCT(void){
	bool32	blOK;
	STDSTR	strCMD;
	SBUF	cBuffer;
	cBuffer.InitSize(128);
	cBuffer.Empty();
	cgDevice->RxDataShareTo(&cBuffer);
	blOK = G_FALSE;
	B_ClrFLAG64(cgDevice->GetLogSystem()->envcfg, ODEV_FLAG_EnHEXViewMode);
	if (SendATCMD(" ","AT+CMUX=0","OK",&cBuffer,1000,300) > 0){
		cgDevice->ACom()->DisableLog();
		B_SetFLAG64(cgDevice->GetLogSystem()->envcfg, ODEV_FLAG_EnHEXViewMode);
		do{
			if (SendCMUXCMD("SABM Frame. Enable DLCI 0.\n",				cgTxUIH.CreateSABMFrameD0(&strCMD), "!'nullptr", &cBuffer, 2000, 100) == 0) break;
			if (SendCMUXCMD("SABM Frame. Enable DLCI 1.\n",				cgTxUIH.CreateSABMFrameV1(&strCMD), "!'nullptr", &cBuffer, 2000, 100) == 0) break;
			if (SendCMUXCMD("MSCCmd. Set DTR=0 & RTS=0 on DLCI=1.\n", 	cgTxUIH.CreateDefaultMSV1(&strCMD), "!'nullptr", &cBuffer, 2000, 100) == 0) break;
			if (SendCMUXCMD("SABM Frame. Enable DLCI 2.\n",				cgTxUIH.CreateSABMFrameV2(&strCMD), "!'nullptr", &cBuffer, 2000, 100) == 0) break;
			if (SendCMUXCMD("MSCCmd. Set DTR=0 & RTS=0 on DLCI=2.\n", 	cgTxUIH.CreateDefaultMSV2(&strCMD), "!'nullptr", &cBuffer, 2000, 100) == 0) break;
			if (SendCMUXCMD("SABM Frame. Enable DLCI 3.\n",				cgTxUIH.CreateSABMFrameV3(&strCMD), "!'nullptr", &cBuffer, 2000, 100) == 0) break;
			if (SendCMUXCMD("MSCCmd. Set DTR=0 & RTS=0 on DLCI=3.\n",	cgTxUIH.CreateDefaultMSV3(&strCMD), "!'nullptr", &cBuffer, 2000, 100) == 0) break;
			if (SendCMUXCMD("SABM Frame. Enable DLCI 4.\n",				cgTxUIH.CreateSABMFrameV4(&strCMD), "!'nullptr", &cBuffer, 2000, 100) == 0) break;
			if (SendCMUXCMD("MSCCmd. Set DTR=0 & RTS=0 on DLCI=4.\n",	cgTxUIH.CreateDefaultMSV4(&strCMD), "!'nullptr", &cBuffer, 2000, 100) == 0) break;
			blOK = G_TRUE;
		} while (0);
	}
	PrintWithDividingLine("CMUXDriver::Finish the CMXU initialization");
	cBuffer.RemoveSelf();
	return(blOK);
}
//------------------------------------------------------------------------------------------//
void CMUXDriver::CMUXCloseCT(void){
	STDSTR	strCMD;
	SBUF	cBuffer;
	cBuffer.InitSize(128);
	cBuffer.Empty();
	cgDevice->RxDataShareTo(&cBuffer);
	B_SetFLAG64(cgDevice->GetLogSystem()->envcfg, ODEV_FLAG_EnHEXViewMode);
	do{
		if (SendCMUXCMD("DISC Frame. Close DLCI 4.\n", cgTxUIH.CreateDISCFrameV4(&strCMD), "!'nullptr", &cBuffer, 2000, 100) == 0) break;
		if (SendCMUXCMD("DISC Frame. Close DLCI 3.\n", cgTxUIH.CreateDISCFrameV3(&strCMD), "!'nullptr", &cBuffer, 2000, 100) == 0) break;
		if (SendCMUXCMD("DISC Frame. Close DLCI 2.\n", cgTxUIH.CreateDISCFrameV2(&strCMD), "!'nullptr", &cBuffer, 2000, 100) == 0) break;
		if (SendCMUXCMD("DISC Frame. Close DLCI 1.\n", cgTxUIH.CreateDISCFrameV1(&strCMD), "!'nullptr", &cBuffer, 2000, 100) == 0) break;
		SendCMUXCMD("DISC Frame. Close DLCI 0.\n", cgTxUIH.CreateDISCFrameD0(&strCMD), "!'nullptr", &cBuffer, 2000, 100);
	} while (0);
	B_ClrFLAG64(cgDevice->GetLogSystem()->envcfg, ODEV_FLAG_EnHEXViewMode);
	cBuffer.RemoveSelf();
	cgDevice->ACom()->EnableLog();
}
//------------------------------------------------------------------------------------------//
void CMUXDriver::CMUXCLDCT(void){
	STDSTR	strCMD;
	SBUF	cBuffer;
	cBuffer.InitSize(128);
	cBuffer.Empty();
	cgDevice->RxDataShareTo(&cBuffer);
	B_SetFLAG64(cgDevice->GetLogSystem()->envcfg, ODEV_FLAG_EnHEXViewMode);
	SendCMUXCMD("CLD frame. Exit CMUX mode:\n",cgTxUIH.CreateCLDFrame(&strCMD),"!'nullptr",&cBuffer,2000,100);
	B_ClrFLAG64(cgDevice->GetLogSystem()->envcfg, ODEV_FLAG_EnHEXViewMode);
	cBuffer.RemoveSelf();
	cgDevice->ACom()->EnableLog();
}
//------------------------------------------------------------------------------------------//
void CMUXDriver::CMUXStdPSCCT(uint8 cmode){
	STDSTR	strCMD;
	SBUF	cBuffer;
	cBuffer.InitSize(128);
	cBuffer.Empty();
	cgDevice->RxDataShareTo(&cBuffer);
	B_SetFLAG64(cgDevice->GetLogSystem()->envcfg, ODEV_FLAG_EnHEXViewMode);
	if (cmode == 0xff){
		SendCMUXCMD("standard PCS frame:\n",cgTxUIH.CreateStdPSCFrame(&strCMD),"!'nullptr",&cBuffer,2000,100);
	}
	else{
		SendCMUXCMD("Telit PCS frame, mode " + Str_ToStr(cmode) + ":\n",cgTxUIH.CreateTPSCxFrame(&strCMD,cmode),"!'nullptr",&cBuffer,2000,100);
	}
	cBuffer.RemoveSelf();
}
//------------------------------------------------------------------------------------------//
void CMUXDriver::CMUXFCCCT(bool32 blIsOn){
	STDSTR	strCMD;
	SBUF	cBuffer;
	cBuffer.InitSize(128);
	cBuffer.Empty();
	cgDevice->RxDataShareTo(&cBuffer);
	B_SetFLAG64(cgDevice->GetLogSystem()->envcfg, ODEV_FLAG_EnHEXViewMode);
	if (blIsOn){
		SendCMUXCMD("FCon frame:\n",cgTxUIH.CreateFCONFrame(&strCMD,CR_BIT),"!'nullptr",&cBuffer,2000,100);
	}
	else{
		SendCMUXCMD("FCoff frame:\n",cgTxUIH.CreateFCOFFFrame(&strCMD,CR_BIT),"!'nullptr",&cBuffer,2000,100);
	}
	cBuffer.RemoveSelf();
}
//------------------------------------------------------------------------------------------//
void CMUXDriver::CMUXMSCCT(int32 dlci,bool32 blDTR,bool32 blRTS){
	VCOM	*vcom;
	STDSTR	strCMD,strTitle;
	uint8	modemStatus;
	SBUF	cBuffer;
	cBuffer.InitSize(128);
	cBuffer.Empty();
	cgDevice->RxDataShareTo(&cBuffer);
	B_SetFLAG64(cgDevice->GetLogSystem()->envcfg, ODEV_FLAG_EnHEXViewMode);
	modemStatus = 0;
	
	vcom = GetVCOM(dlci);
	strTitle = "MSC CMD, Set DTR=";
	if (blDTR == 0){
		modemStatus |= MS_DTR;
		strTitle += '0';
		if (vcom != nullptr)
			vcom->Update_DTR(0);
	}
	else{
		strTitle += '1';
		if (vcom != nullptr)
			vcom->Update_DTR(1);
	}
	strTitle += " & RTS=";
	if (blRTS == 0)	{
		modemStatus |= MS_RTS;
		strTitle += '0';
		if (vcom != nullptr)
			vcom->Update_RTS(0);
	}
	else{
		strTitle += '1';
		if (vcom != nullptr)
			vcom->Update_RTS(1);
	}
	strTitle += " on DLCI=";
	strTitle += Str_ToStr(dlci);
	strTitle += "::\n";
	
	SendCMUXCMD(strTitle,cgTxUIH.CreateMSCCmd(&strCMD,dlci,modemStatus,CR_BIT,CR_BIT),"!'nullptr",&cBuffer,2000,300);
	cBuffer.RemoveSelf();
}
//------------------------------------------------------------------------------------------//
void CMUXDriver::SendCLD	(void)									{CMUXCLDCT();};
void CMUXDriver::SendStdPSC	(uint8 cmode)							{CMUXStdPSCCT(cmode);};
void CMUXDriver::SendFCC	(int32 blIsOn)							{CMUXFCCCT(blIsOn);};
void CMUXDriver::SendMSC	(int32 dlci,int32 blDTR,int32 blRTS)	{CMUXMSCCT(dlci,blDTR,blRTS);};
//------------------------------------------------------------------------------------------//
bool32 CMUXDriver::CmuxStartWithATThreadFun(void* p){
	if (CMUXInitAT() && CMUXStartCT())
		return G_TRUE;
	DoSelfClose();
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
bool32 CMUXDriver::CmuxStartThreadFun(void* p){
	CMUXStartCT();
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
bool32 CMUXDriver::CmuxStopThreadFun(void* p){
	CMUXCloseCT();
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
void CMUXDriver::CMUXStartWithAT(void){
	commandThread.InUse_set();
	commandThread.ThreadStop();
	commandThread.ThreadInit(this,&CMUXDriver::CmuxStartWithATThreadFun,"CMUXStartWithAT");
	commandThread.ThreadRun();
	commandThread.InUse_clr();
}
//------------------------------------------------------------------------------------------//
void CMUXDriver::CMUXStart(void){
	commandThread.InUse_set();
	commandThread.ThreadStop();
	commandThread.ThreadInit(this,&CMUXDriver::CmuxStartThreadFun,"CMUXStart");
	commandThread.ThreadRun();
	commandThread.InUse_clr();
}
//------------------------------------------------------------------------------------------//
void CMUXDriver::CMUXStop(void){
	commandThread.InUse_set();
	commandThread.ThreadStop();
	commandThread.ThreadInit(this,&CMUXDriver::CmuxStopThreadFun,"CMUXStop");
	commandThread.ThreadRun();
	commandThread.InUse_clr();
}
//------------------------------------------------------------------------------------------//
bool32 CMUXDriver::SendATCMD(const STDSTR& strTitle,const STDSTR& strCMD,const STDSTR& strCondition,SBUF* cSBUF,uint32 waitMS,uint32 delyMS){
	cgDevice->PrintSendCommand("CMUXDriver::Send",strTitle,strCMD);
	cgDevice->SendCommand(strCMD,CMD_R,G_ESCAPE_ON);
	return(cgDevice->CheckReceive(strCondition, cSBUF, waitMS, delyMS));
}
//------------------------------------------------------------------------------------------//
bool32 CMUXDriver::SendCMUXCMD(const STDSTR& strTitle,const STDSTR& strCMD,const STDSTR& strCondition,SBUF* cSBUF,uint32 waitMS,uint32 delyMS){
	cgDevice->PrintSendCommand("CMUXDriver::Send",strTitle,strCMD);
	return(cgDevice->CheckReceive(strCondition, cSBUF, waitMS, delyMS));
}
//------------------------------------------------------------------------------------------//
uint32 CMUXDriver::Send(VCOM* vcom,const ARRAY& _in){
	uint32	num;
	num = 0;
	cgTxUIH.InUse_set();
	cgTxUIH.SetUIHFrame(&num,nullptr,vcom->cgPortID,_in,CR_BIT);
	cgTxUIH.InUse_clr();
	return(num);
}
//------------------------------------------------------------------------------------------//
void CMUXDriver::Send3Pluse(VCOM* vcom){
	STDSTR	strHexFrame;
	uint8	modemStatus = 0;
	
	PrintWithDividingLine("CMUXDriver::Received +++ from DLCI",Str_ToStr(vcom->cgPortID),", switch to DTR control");
	
	if (vcom->Check_RTS()== 0)
		modemStatus |= MS_RTS;
	
	cgTxUIH.CreateMSCCmd(&strHexFrame,vcom->cgPortID,modemStatus,CR_BIT,CR_BIT);
	PrintWithTime(COL_clDCyan,"CMUXDriver::Send MSC confirm frame:\n",strHexFrame,"\n");
	
	SYS_SleepMS(200);
	
	modemStatus |= MS_DTR;
	
	cgTxUIH.CreateMSCCmd(&strHexFrame,vcom->cgPortID,modemStatus,CR_BIT,CR_BIT);
	PrintWithTime(COL_clDCyan,"CMUXDriver::Send MSC confirm frame:\n",strHexFrame,"\n");
	SYS_SleepMS(200);
}
//------------------------------------------------------------------------------------------//
DEVICE* CMUXDriver::GetDevice(void){
	return(cgDevice);
}
//------------------------------------------------------------------------------------------//
#endif /* CMUX_h */
