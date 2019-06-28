//
//  CMUX.cpp
//  SDT
//
//  Created by Leif Wen on 25/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "CMUX_UIH.h"
#ifdef CMUX_UIH_h
//------------------------------------------------------------------------------------------//
#include "DS_STRING.h"
namespace CMUX {
//------------------------------------------------------------------------------------------//
PN_LENI::PN_LENI(void) : PNF_MASK() {
	AppendDown(pn_EA) < pn_Length;
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
};
//------------------------------------------------------------------------------------------//
IOSE PN_LENI::WriteByte1(IOS* _ios,uint32 data){
	IOSIN	iosin;
	IOS_copy(&iosin, _ios);

	IOS_update(&iosin,_Begin(_ios));
	IOS_update(&iosin,SetByte1(_ios, data));
	IOS_update(&iosin,_Endl(_ios));
	FillMaskFieldWR();
	return(IOS_copy(_ios, iosin));
};
//------------------------------------------------------------------------------------------//
IOSE PN_LENI::WriteByte2(IOS* _ios,uint32 data){
	IOSIN	iosin;
	IOS_copy(&iosin, _ios);

	IOS_update(&iosin,_Begin(_ios));
	IOS_update(&iosin,SetByte2(_ios, data));
	IOS_update(&iosin,_Endl(_ios));
	FillMaskFieldWR();
	return(IOS_copy(_ios, iosin));
};
//------------------------------------------------------------------------------------------//

	







//------------------------------------------------------------------------------------------//
UIH_SUBFRAME::UIH_SUBFRAME(void) : PNF_BLOCK() {
	AppendDown(pn_Ctrl < pn_Ctrl_EA < pn_Ctrl_CR < pn_Ctrl_Type) < pn_Len < pn_Content;
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
uint32& UIH_FRAME::GetInfoSizeMax(void){
	static	uint32 infoSizeMax = INFO_SIZE_MAX;
	return(infoSizeMax);
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
};
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
};
//------------------------------------------------------------------------------------------//
IOSE UIH_FRAME::AddUIHFrame(uint32* sendNum,STDSTR* retHexFrame,uint8 vPort,const UVIn& _in,uint8 crBit){
	IOSE	rcode = IOS_OK;
	uint32	num;
	
	cgPosWR.array->Prepare_Set();
	IOS_update(&rcode,_Begin(nullptr));
	
	IOS_update(&rcode,pn_Addr.Write(nullptr,(vPort << 2) | crBit | EA_BIT));	//1:Address field
	IOS_update(&rcode,pn_Ctrl.Write(nullptr,CTRL_UIH & (~PF_BIT)));				//2:Ctrl field,Poll/Final BIT always 1
	
	num = (uint32)GetUVInLength(_in);
	if (num > GetInfoSizeMax())
		num = GetInfoSizeMax();
	if (num < FRAME_SIZE_TWOBYTES){
		IOS_update(&rcode,pn_Len.WriteByte1(nullptr,(num << 1) | EA_BIT));		//3:Length indicator
	}
	else{
		IOS_update(&rcode,pn_Len.WriteByte2(nullptr,(num << 1)));
	}
	
	IOS_update(&rcode,pn_Info.Write(nullptr,_in));								//4:Info

	IOS_update(&rcode,_Endl(nullptr));
	
	if ((retHexFrame != nullptr) && (rcode > 0)){
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
	if (sendNum != nullptr){
		if (rcode <= 0)
			num = 0;
		*sendNum = num;
	}
	return(rcode);
};
//------------------------------------------------------------------------------------------//
IOSE UIH_FRAME::SetUIHFrame(uint32* sendNum,STDSTR* retHexFrame,uint8 vPort,const UVIn& _in,uint8 crBit){
	IOSE	rcode = IOS_OK;
	uint32	snum,total,num;
	snum = 0;
	total = 0;
	num = (uint32)GetUVInLength(_in);
	do{
		snum = 0;
		IOS_update(&rcode,AddUIHFrame(&snum,retHexFrame,vPort,_in,crBit));
		AddUVInOffset(_in, snum);
		total += snum;
	}while((total < num) && (rcode > 0));
	if (sendNum != nullptr)
		*sendNum = total;
	return(rcode);
};
//------------------------------------------------------------------------------------------//
IOSE UIH_FRAME::SetCtrlFrame(STDSTR* retHexFrame,uint8 vPort,uint8 ctrlType,uint8 crBit){
	IOSE	rcode = IOS_OK;
	cgPosWR.array->Prepare_Set();
	IOS_update(&rcode,_Begin(nullptr));
		
	IOS_update(&rcode,pn_Addr.Write(nullptr,(vPort << 2) | crBit | EA_BIT));	//1:Address field
	IOS_update(&rcode,pn_Ctrl.Write(nullptr,ctrlType | PF_BIT));				//2:Ctrl field,Poll/Final BIT always 1
	IOS_update(&rcode,pn_Len.WriteByte1(nullptr,EA_BIT));						//3:Length indicator
	IOS_update(&rcode,pn_Info.WriteNone());										//4:Info
		
	IOS_update(&rcode,_Endl(nullptr));
		
	if ((retHexFrame != nullptr) && (rcode > 0)){
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
	return(rcode);
};
//------------------------------------------------------------------------------------------//
IOSE UIH_FRAME::SetCLDFrame(STDSTR* retHexFrame){
	uint8	data[2];
	
	data[0] = CMD_MUX_CLD | CR_BIT | EA_BIT;
	data[1] = EA_BIT;
	
	return(SetUIHFrame(nullptr,retHexFrame,DLCI0,IUD(data,2),CR_BIT));
};
//------------------------------------------------------------------------------------------//
IOSE UIH_FRAME::SetFCONFrame(STDSTR* retHexFrame,uint8 crBit){
	uint8	data[2];
	
	data[0] = CMD_FCON | crBit | EA_BIT;
	data[1] = EA_BIT;
	
	return(SetUIHFrame(nullptr,retHexFrame,DLCI0,IUD(data,2),CR_BIT));
};
//------------------------------------------------------------------------------------------//
IOSE UIH_FRAME::SetFCOFFFrame(STDSTR* retHexFrame,uint8 crBit){
	uint8	data[2];
	
	data[0] = CMD_FCOFF | crBit | EA_BIT;
	data[1] = EA_BIT;
	
	return(SetUIHFrame(nullptr,retHexFrame,DLCI0,IUD(data,2),CR_BIT));
};
//------------------------------------------------------------------------------------------//
IOSE UIH_FRAME::SetStdPSCFrame(STDSTR* retHexFrame){
	uint8	data[2];
	
	data[0] = CMD_PSC | CR_BIT | EA_BIT;
	data[1] = EA_BIT;
	
	return(SetUIHFrame(nullptr,retHexFrame,DLCI0,IUD(data,2),CR_BIT));
};
//------------------------------------------------------------------------------------------//
IOSE UIH_FRAME::SetTPSCxFrame(STDSTR* retHexFrame,uint8 mode){
	uint8	data[3];
	
	data[0] = CMD_PSC | CR_BIT | EA_BIT;
	data[1] = (0x01 << 1) | EA_BIT;
	data[2] = mode;
	
	return(SetUIHFrame(nullptr,retHexFrame,DLCI0,IUD(data,3),CR_BIT));
};
//------------------------------------------------------------------------------------------//
IOSE UIH_FRAME::SetMSCCmd(STDSTR* retHexFrame,uint8 vPort,uint8 modemStatus,uint8 UINcrbit,uint8 crBit){
	uint8	data[4];
	
	data[0] = CMD_MSC | crBit | EA_BIT;  		// Cmd type
	data[1] = (0x02 << 1) | EA_BIT;				// Cmd Len Ind
	data[2] = (vPort << 2) | 0x02 | EA_BIT;   	// valore 1
	data[3] = modemStatus | 0x01;
	
	return(SetUIHFrame(nullptr,retHexFrame,DLCI0,IUD(data,4),UINcrbit));
};
//------------------------------------------------------------------------------------------//
IOSE UIH_FRAME::SetBRKCmd(STDSTR* retHexFrame,uint8 vPort,uint8 modemStatus,uint8 UINcrbit,uint8 crBit){
	uint8	data[5];
	
	data[0] = CMD_MSC | crBit | EA_BIT;  		// Cmd type
	data[1] = (3 << 1) | EA_BIT;				// Cmd Len Ind
	data[2] = (vPort << 2) | 0x02 | EA_BIT;		// valore 1
	data[3] = modemStatus | 0x01;
	data[4] = BREAK_OCTET;
	
	return(SetUIHFrame(nullptr,retHexFrame,DLCI0,IUD(data,5),UINcrbit));
};
//------------------------------------------------------------------------------------------//
IOSE UIH_FRAME::SetSABMFrameD0(STDSTR* retHexFrame){return(SetCtrlFrame(retHexFrame,DLCI0,CTRL_SABM,CR_BIT));};
IOSE UIH_FRAME::SetSABMFrameV1(STDSTR* retHexFrame){return(SetCtrlFrame(retHexFrame,DLCI1,CTRL_SABM,CR_BIT));};
IOSE UIH_FRAME::SetSABMFrameV2(STDSTR* retHexFrame){return(SetCtrlFrame(retHexFrame,DLCI2,CTRL_SABM,CR_BIT));};
IOSE UIH_FRAME::SetSABMFrameV3(STDSTR* retHexFrame){return(SetCtrlFrame(retHexFrame,DLCI3,CTRL_SABM,CR_BIT));};
IOSE UIH_FRAME::SetSABMFrameV4(STDSTR* retHexFrame){return(SetCtrlFrame(retHexFrame,DLCI4,CTRL_SABM,CR_BIT));};

IOSE UIH_FRAME::SetDISCFrameD0(STDSTR* retHexFrame){return(SetCtrlFrame(retHexFrame,DLCI0,CTRL_DISC,CR_BIT));};
IOSE UIH_FRAME::SetDISCFrameV1(STDSTR* retHexFrame){return(SetCtrlFrame(retHexFrame,DLCI1,CTRL_DISC,CR_BIT));};
IOSE UIH_FRAME::SetDISCFrameV2(STDSTR* retHexFrame){return(SetCtrlFrame(retHexFrame,DLCI2,CTRL_DISC,CR_BIT));};
IOSE UIH_FRAME::SetDISCFrameV3(STDSTR* retHexFrame){return(SetCtrlFrame(retHexFrame,DLCI3,CTRL_DISC,CR_BIT));};
IOSE UIH_FRAME::SetDISCFrameV4(STDSTR* retHexFrame){return(SetCtrlFrame(retHexFrame,DLCI4,CTRL_DISC,CR_BIT));};

IOSE UIH_FRAME::SetDefaultMSV1(STDSTR* retHexFrame){return(SetMSCCmd(retHexFrame,DLCI1,MS_DTR | MS_RTS,CR_BIT,CR_BIT));};
IOSE UIH_FRAME::SetDefaultMSV2(STDSTR* retHexFrame){return(SetMSCCmd(retHexFrame,DLCI2,MS_DTR | MS_RTS,CR_BIT,CR_BIT));};
IOSE UIH_FRAME::SetDefaultMSV3(STDSTR* retHexFrame){return(SetMSCCmd(retHexFrame,DLCI3,MS_DTR | MS_RTS,CR_BIT,CR_BIT));};
IOSE UIH_FRAME::SetDefaultMSV4(STDSTR* retHexFrame){return(SetMSCCmd(retHexFrame,DLCI4,MS_DTR | MS_RTS,CR_BIT,CR_BIT));};
//------------------------------------------------------------------------------------------//
const STDSTR& UIH_FRAME::CreateFCONFrame(STDSTR* retHexFrame,uint8 crBit){
	cgLock.Set();
	SetFCONFrame(_EMPTY(retHexFrame),crBit);
	cgLock.Clr();
	return(*retHexFrame);
};
//------------------------------------------------------------------------------------------//
const STDSTR& UIH_FRAME::CreateFCOFFFrame(STDSTR* retHexFrame,uint8 crBit){
	cgLock.Set();
	SetFCOFFFrame(_EMPTY(retHexFrame),crBit);
	cgLock.Clr();
	return(*retHexFrame);
};
//------------------------------------------------------------------------------------------//
const STDSTR& UIH_FRAME::CreateTPSCxFrame(STDSTR* retHexFrame,uint8 mode){
	cgLock.Set();
	SetTPSCxFrame(_EMPTY(retHexFrame),mode);
	cgLock.Clr();
	return(*retHexFrame);
};
//------------------------------------------------------------------------------------------//
const STDSTR& UIH_FRAME::CreateMSCCmd(STDSTR* retHexFrame,uint8 vPort,uint8 modemStatus,uint8 UINcrbit,uint8 crBit){
	cgLock.Set();
	SetMSCCmd(_EMPTY(retHexFrame),vPort,modemStatus,UINcrbit,crBit);
	cgLock.Clr();
	return(*retHexFrame);
};
//------------------------------------------------------------------------------------------//
const STDSTR& UIH_FRAME::CreateBRKCmd(STDSTR* retHexFrame,uint8 vPort,uint8 modemStatus,uint8 UINcrbit,uint8 crBit){
	cgLock.Set();
	SetBRKCmd(_EMPTY(retHexFrame),vPort,modemStatus,UINcrbit,crBit);
	cgLock.Clr();
	return(*retHexFrame);
};
//------------------------------------------------------------------------------------------//
#define CMUXFUN(_name)\
const STDSTR& UIH_FRAME::Create##_name(STDSTR* retHexFrame){\
	cgLock.Set();\
	Set##_name(_EMPTY(retHexFrame));\
	cgLock.Clr();\
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
#endif /* CMUX_h */
