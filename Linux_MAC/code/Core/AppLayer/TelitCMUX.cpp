/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: TelitCMUX.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2013.01.05
 *
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "TelitCMUX.h"
#include "Comm_Convert.h"
#include "Device.h"
#include "SMC.h"
//------------------------------------------------------------------------------------------//
#ifdef	SWVERSION_CMUX
//------------------------------------------------------------------------------------------//
static const uint8 CMUX_crctable[256] = { // reversed, 8-bit, poly=0x07
	0x00, 0x91, 0xE3, 0x72, 0x07, 0x96, 0xE4, 0x75, 0x0E, 0x9F, 0xED, 0x7C, 0x09, 0x98, 0xEA, 0x7B,
	0x1C, 0x8D, 0xFF, 0x6E, 0x1B, 0x8A, 0xF8, 0x69, 0x12, 0x83, 0xF1, 0x60, 0x15,	0x84, 0xF6, 0x67,
	0x38, 0xA9, 0xDB, 0x4A, 0x3F, 0xAE, 0xDC, 0x4D, 0x36, 0xA7, 0xD5, 0x44, 0x31,	0xA0, 0xD2, 0x43,
	0x24, 0xB5, 0xC7, 0x56, 0x23, 0xB2, 0xC0, 0x51, 0x2A, 0xBB, 0xC9, 0x58, 0x2D,	0xBC, 0xCE, 0x5F,
	0x70, 0xE1, 0x93, 0x02, 0x77, 0xE6, 0x94, 0x05, 0x7E, 0xEF, 0x9D, 0x0C, 0x79,	0xE8, 0x9A, 0x0B,
	0x6C, 0xFD, 0x8F, 0x1E, 0x6B, 0xFA, 0x88, 0x19, 0x62, 0xF3, 0x81, 0x10, 0x65,	0xF4, 0x86, 0x17,
	0x48, 0xD9, 0xAB, 0x3A, 0x4F, 0xDE, 0xAC, 0x3D, 0x46, 0xD7, 0xA5, 0x34, 0x41,	0xD0, 0xA2, 0x33,
	0x54, 0xC5, 0xB7, 0x26, 0x53, 0xC2, 0xB0, 0x21, 0x5A, 0xCB, 0xB9, 0x28, 0x5D,	0xCC, 0xBE, 0x2F,
	0xE0, 0x71, 0x03, 0x92, 0xE7, 0x76, 0x04, 0x95, 0xEE, 0x7F, 0x0D, 0x9C, 0xE9,	0x78, 0x0A, 0x9B,
	0xFC, 0x6D, 0x1F, 0x8E, 0xFB, 0x6A, 0x18, 0x89, 0xF2, 0x63, 0x11, 0x80, 0xF5,	0x64, 0x16, 0x87,
	0xD8, 0x49, 0x3B, 0xAA, 0xDF, 0x4E, 0x3C, 0xAD, 0xD6, 0x47, 0x35, 0xA4, 0xD1,	0x40, 0x32, 0xA3,
	0xC4, 0x55, 0x27, 0xB6, 0xC3, 0x52, 0x20, 0xB1, 0xCA, 0x5B, 0x29, 0xB8, 0xCD,	0x5C, 0x2E, 0xBF,
	0x90, 0x01, 0x73, 0xE2, 0x97, 0x06, 0x74, 0xE5, 0x9E, 0x0F, 0x7D, 0xEC, 0x99,	0x08, 0x7A, 0xEB,
	0x8C, 0x1D, 0x6F, 0xFE, 0x8B, 0x1A, 0x68, 0xF9, 0x82, 0x13, 0x61, 0xF0, 0x85,	0x14, 0x66, 0xF7,
	0xA8, 0x39, 0x4B, 0xDA, 0xAF, 0x3E, 0x4C, 0xDD, 0xA6, 0x37, 0x45, 0xD4, 0xA1,	0x30, 0x42, 0xD3,
	0xB4, 0x25, 0x57, 0xC6, 0xB3, 0x22, 0x50, 0xC1, 0xBA, 0x2B, 0x59, 0xC8, 0xBD,	0x2C, 0x5E, 0xCF
};
//------------------------------------------------------------------------------------------//
void CMUX_UIH_SubFrame::Init(FIFO_UINT8 *tFIFO_Out){
	Field_Node::Init(FNT_MESSAGE,tFIFO_Out);
		
	fn_Type.Init(FNT_MASK,tFIFO_Out,1);
	{
		fn_Type_EA.Init(FNT_CTRL,tFIFO_Out);
		fn_Type_EA.SetMaskBit(0x01);
		
		fn_Type_CR.Init(FNT_CTRL,tFIFO_Out);
		fn_Type_CR.SetMaskBit(0x02);
		
		fn_Type_Type.Init(FNT_CTRL,tFIFO_Out);
		fn_Type_Type.SetMaskBit(0xFC);
		
		fn_Type.AddNode(&fn_Type_EA);
		fn_Type.AddNode(&fn_Type_CR);
		fn_Type.AddNode(&fn_Type_Type);
		fn_Type.FillMaskField();
	}
	
	fn_LengthIndicator.Init(FNT_MASK,tFIFO_Out);
	
	fn_Content.Init(FNT_CONTENT,tFIFO_Out);
	fn_Content.linkNode = &fn_LengthIndicator.fn_Length;
	
	AddNode(&fn_Type);
	AddNode(&fn_LengthIndicator);
	AddNode(&fn_Content);
};
//------------------------------------------------------------------------------------------//
void CMUX_FRAME::Init(){
	//("F9 01 F9 05 EF 27 0D 0A 30 37 2E 30 32 2E 35 30 34 0D 0A 0D 0A 4F 4B 0D 0A 80 F9");
	
	Field_Node::Init(FNT_MESSAGE,&cgResultFIFO);
	
	fn_Head.Init(FNT_HEAD,&cgResultFIFO,1);
	fn_Head.SetFixValue("\xf9");
	
	fn_Address.Init(FNT_MASK,&cgResultFIFO,1);
	{
		fn_Address_EA.Init(FNT_CTRL,&cgResultFIFO);
		fn_Address_EA.SetMaskBit(0x01);
		
		fn_Address_CR.Init(FNT_CTRL,&cgResultFIFO);
		fn_Address_CR.SetMaskBit(0x02);
		
		fn_Address_DLCI.Init(FNT_CTRL,&cgResultFIFO);
		fn_Address_DLCI.SetMaskBit(0xFC);
		
		fn_Address.AddNode(&fn_Address_EA);
		fn_Address.AddNode(&fn_Address_CR);
		fn_Address.AddNode(&fn_Address_DLCI);
		
		fn_Address.FillMaskField();
	}
	fn_Control.Init(FNT_MASK, &cgResultFIFO, 1);
	{
		fn_Control_PF.Init(FNT_CTRL,&cgResultFIFO);
		fn_Control_PF.SetMaskBit(0x10);
		
		fn_Control_Type.Init(FNT_CTRL,&cgResultFIFO);
		fn_Control_Type.SetMaskBit(0xEF);
		
		fn_Control.AddNode(&fn_Control_PF);
		fn_Control.AddNode(&fn_Control_Type);

		fn_Control.FillMaskField();
	}
	
	fn_LengthIndicator.Init(FNT_MASK,&cgResultFIFO);
	
	fn_Info.Init(FNT_MESSAGE,&cgResultFIFO);
	fn_Info.linkNode = &fn_LengthIndicator.fn_Length;
	
	fn_FCS.Init(FNT_CHECKSUM,&cgResultFIFO,1);
	
	fn_Tail.Init(FNT_TAIL,&cgResultFIFO,1);
	fn_Tail.SetFixValue("\xf9");
	
	AddNode(&fn_Head);
	AddNode(&fn_Address);
	AddNode(&fn_Control);
	AddNode(&fn_LengthIndicator);
	AddNode(&fn_Info);
	AddNode(&fn_FCS);
	AddNode(&fn_Tail);
};
//------------------------------------------------------------------------------------------//
int32 CMUX_FRAME::ChecksumResult(const FIFO_UINT8 &fifobuf)const{
	uint8	FCS;
	uint32	len,i;
	uint8	charTemp;
	
	charTemp = 0;
	FCS = 0xFF;
	len = 0 + ((fn_Control_Type.GetValueAMask(&fifobuf) == CMUX_CTRL_UIH) ? fn_Info.GetOffset() : fn_FCS.GetOffset());
	i = fnOffset;
	while(++i < len){
		if (fifobuf.Read(&charTemp, 1, i) == 1)
			FCS = CMUX_crctable[FCS ^ charTemp];
	}
	FCS = CMUX_crctable[FCS ^ fn_FCS.GetOriginalValue(&fifobuf)];
	return(FCS == 0xCF);
}
//------------------------------------------------------------------------------------------//
uint8 CMUX_FRAME::CalcFCS(void){
	uint8	FCS;
	uint32	len,i;
	uint8	charTemp;
	
	FCS = 0xFF;
	charTemp = 0;
	len = (fn_Control_Type.GetValueAMask(GetcgDefFifo(this)) == CMUX_CTRL_UIH) ? fn_Info.GetOffset() : (fn_LengthIndicator.GetOffset() + fn_LengthIndicator.GetLength());
	i = fnOffset;
	while(++i < len){
		if (GetcgDefFifo(this)->Read(&charTemp,1,i) == 1)
			FCS = CMUX_crctable[FCS ^ charTemp];
	}
	return(0xFF - FCS);
}
//------------------------------------------------------------------------------------------//
std::string CMUX_FRAME::CreateCMuxCrtlFrame(uint8 vPort,uint8 ctrlType,uint8 crBit){
	std::string			strResult;

	GetcgDefFifo(this)->Empty();
	HoldOffset();

	fn_Head.SetFIFOFixValue();									//0:Head Flag
	fn_Address.SetFIFO_1Byte((vPort << 2) | crBit | CMUX_EA_BIT);//1:Address field
	fn_Control.SetFIFO_1Byte(ctrlType | CMUX_PF_BIT);			//2:Ctrl field,Poll/Final BIT always 1
	fn_LengthIndicator.SetFIFO_1Byte(0x00 | CMUX_EA_BIT);		//3:Length indicator
	fn_Info.SetFIFO();											//4:Info
	fn_FCS.SetFIFO_1Byte(CalcFCS());							//5:FCS field
	fn_Tail.SetFIFOFixValue();									//6:Tail Flag
	
	UpdateLength();
	return(ReadAllContent(&strResult));
}
//------------------------------------------------------------------------------------------//
std::string CMUX_FRAME::CreateCMuxUIHFrame(uint8 vPort,const FIFO_UINT8 &info,uint8 crBit){
	std::string			strResult;
	uint8	data[2];
	uint16	len;
	uint32	num;
	
	GetcgDefFifo(this)->Empty();
	HoldOffset();

	fn_Head.SetFIFOFixValue();									//0:Head Flag
	fn_Address.SetFIFO_1Byte((vPort << 2) | crBit | CMUX_EA_BIT);//1:Address field
	fn_Control.SetFIFO_1Byte(CMUX_CTRL_UIH & (~CMUX_PF_BIT));	//2:Ctrl field,Poll/Final BIT always 1

	num = (uint32)info.Used();
	if (num > CMUX_MAX_INFO_SIZE)
		num = CMUX_MAX_INFO_SIZE;
	if (num < CMUX_TWOBYTES_FRAME_SIZE){
		fn_LengthIndicator.SetFIFO_1Byte((num << 1) | CMUX_EA_BIT);//3:Length indicator
	}
	else{
		len = (num << 1);
		data[0] = (uint8)len;								//3:Length indicator
		data[1] = (uint8)(len >> 8);						//4:Length indicator
		fn_LengthIndicator.SetFIFO(data, 2);
	}

	fn_Info.SetFIFO(info, num);								//4:Info
	fn_FCS.SetFIFO_1Byte(CalcFCS());							//5:FCS field
	fn_Tail.SetFIFOFixValue();								//6:Tail Flag
	
	UpdateLength();
	return(ReadAllContent(&strResult));
}
//------------------------------------------------------------------------------------------//
std::string CMUX_FRAME::CreateCMuxUIHFrame(uint8 vPort,const std::string &info,uint8 crBit){
	std::string			strResult;
	uint8	data[2];
	uint16	len;
	uint32	num;
	
	GetcgDefFifo(this)->Empty();
	HoldOffset();
	fn_Head.SetFIFOFixValue();									//0:Head Flag
	fn_Address.SetFIFO_1Byte((vPort << 2) | crBit | CMUX_EA_BIT);//1:Address field
	fn_Control.SetFIFO_1Byte(CMUX_CTRL_UIH & (~CMUX_PF_BIT));	//2:Ctrl field,Poll/Final BIT always 1
	
	num = (uint32)info.length();
	if (num > CMUX_MAX_INFO_SIZE)
		num = CMUX_MAX_INFO_SIZE;
	if (num < CMUX_TWOBYTES_FRAME_SIZE){
		fn_LengthIndicator.SetFIFO_1Byte((num << 1) | CMUX_EA_BIT);//3:Length indicator
	}
	else{
		len = (num << 1);
		data[0] = (uint8)len;								//3:Length indicator
		data[1] = (uint8)(len >> 8);						//4:Length indicator
		fn_LengthIndicator.SetFIFO(data, 2);
	}

	fn_Info.SetFIFO((uint8*)info.c_str(), num);				//4:Info
	fn_FCS.SetFIFO_1Byte(CalcFCS());							//5:FCS field
	fn_Tail.SetFIFOFixValue();								//6:Tail Flag
	
	UpdateLength();
	return(ReadAllContent(&strResult));
}
//------------------------------------------------------------------------------------------//
std::string CMUX_FRAME::CreateCMuxCLDFrame(void){
	uint8			charTemp;
	std::string		strFrameInfo;
	
	charTemp = CMUX_MUX_CLD_CMD | CMUX_CR_BIT | CMUX_EA_BIT;
	strFrameInfo = ((uint8)charTemp);
	charTemp = 0x00 | CMUX_EA_BIT;
	strFrameInfo += ((uint8)charTemp);
	
	return(CreateCMuxUIHFrame(CMUX_CTRL_DLCI_INDEX,strFrameInfo,CMUX_CR_BIT));
}
//------------------------------------------------------------------------------------------//
std::string CMUX_FRAME::CreateCMuxFCONFrame(uint8 crBit){
	uint8			charTemp;
	std::string		strFrameInfo;
	
	charTemp = CMUX_FCON_CMD | crBit | CMUX_EA_BIT;
	strFrameInfo = ((uint8)charTemp);
	charTemp = 0x00 | CMUX_EA_BIT;
	strFrameInfo += ((uint8)charTemp);
	
	return(CreateCMuxUIHFrame(CMUX_CTRL_DLCI_INDEX,strFrameInfo,CMUX_CR_BIT));
}
//------------------------------------------------------------------------------------------//
std::string CMUX_FRAME::CreateCMuxFCOFFFrame(uint8 crBit){
	uint8			charTemp;
	std::string		strFrameInfo;
	
	charTemp = CMUX_FCOFF_CMD | crBit | CMUX_EA_BIT;
	strFrameInfo = ((uint8)charTemp);
	charTemp = 0x00 | CMUX_EA_BIT;
	strFrameInfo += ((uint8)charTemp);
	
	return(CreateCMuxUIHFrame(CMUX_CTRL_DLCI_INDEX,strFrameInfo,CMUX_CR_BIT));
}
//------------------------------------------------------------------------------------------//
std::string CMUX_FRAME::CreateCMuxStdPSCFrame(void){
	uint8			charTemp;
	std::string		strFrameInfo;
	
	charTemp = CMUX_PSC_CMD | CMUX_CR_BIT | CMUX_EA_BIT;	// Cmd type
	strFrameInfo = ((uint8)charTemp);
	charTemp = 0x00 | CMUX_EA_BIT;							// Cmd Len Ind
	strFrameInfo += ((uint8)charTemp);
	
	return(CreateCMuxUIHFrame(CMUX_CTRL_DLCI_INDEX,strFrameInfo,CMUX_CR_BIT));
}
//------------------------------------------------------------------------------------------//
std::string CMUX_FRAME::CreateCMuxTPSCxFrame(uint8 mode){
	uint8			charTemp;
	std::string		strFrameInfo;
	
	charTemp = CMUX_PSC_CMD | CMUX_CR_BIT | CMUX_EA_BIT;			// Cmd type
	strFrameInfo = ((uint8)charTemp);
	charTemp = (0x01 << 1) | CMUX_EA_BIT;             				// Cmd Len Ind
	strFrameInfo += ((uint8)charTemp);
	strFrameInfo += ((uint8)mode);
	
	return(CreateCMuxUIHFrame(CMUX_CTRL_DLCI_INDEX,strFrameInfo,CMUX_CR_BIT));
}
//------------------------------------------------------------------------------------------//
std::string CMUX_FRAME::CreateCMuxMSCCmd(uint8 vPort,uint8 modemStatus,uint8 UINcrbit,uint8 crBit){
	uint8			charTemp;
	std::string		strFrameInfo;
	
	charTemp = CMUX_MSC_CMD | crBit | CMUX_EA_BIT;  		// Cmd type
	strFrameInfo = ((uint8)charTemp);
	charTemp = (0x02 << 1) | CMUX_EA_BIT;					// Cmd Len Ind
	strFrameInfo += ((uint8)charTemp);
	charTemp = (vPort << 2) | 0x02 | CMUX_EA_BIT;   		// valore 1
	strFrameInfo += ((uint8)charTemp);
	charTemp = modemStatus | 0x01;
	strFrameInfo += ((uint8)charTemp);
	return(CreateCMuxUIHFrame(CMUX_CTRL_DLCI_INDEX,strFrameInfo,UINcrbit));
}
//------------------------------------------------------------------------------------------//
std::string CMUX_FRAME::CreateCMuxBRKCmd(uint8 vPort,uint8 modemStatus,uint8 UINcrbit,uint8 crBit){
	uint8			charTemp;
	std::string		strFrameInfo;
	
	charTemp = CMUX_MSC_CMD | crBit | CMUX_EA_BIT;  // Cmd type
	strFrameInfo = ((uint8)charTemp);
	charTemp = (3 << 1) | CMUX_EA_BIT;              // Cmd Len Ind
	strFrameInfo += ((uint8)charTemp);
	charTemp = (vPort << 2) | 0x02 | CMUX_EA_BIT;   // valore 1
	strFrameInfo += ((uint8)charTemp);
	charTemp = modemStatus | 0x01;
	strFrameInfo += ((uint8)charTemp);
	charTemp = CMUX_BREAK_OCTET;
	strFrameInfo += ((uint8)charTemp);
	
	return(CreateCMuxUIHFrame(CMUX_CTRL_DLCI_INDEX,strFrameInfo,UINcrbit));
}
//------------------------------------------------------------------------------------------//



//------------------------------------------------------------------------------------------//
void CMUXCOM::Init(void){
	cgTrackingID = 0;
	vPortDSR = 0;
	vPortCTS = 0;
	vPortDCD = 0;
	vPortRING = 0;
	vPortDTR = 0;
	vPortRTS = 0;
	vPortHEX = 0;
	vPortEscape = 1;
	vPortName = "";
}
//------------------------------------------------------------------------------------------//
int32 CMUXCOM::PrintThreadFun(void){
	uint8				modemStatus;
	uint64				byteNum;
	std::string			strPrintdata,strRecData,strCommand,strTemp;
	CMUX_FRAME			tCMUXFrame;
	DEVICE				*tDevice;
	strPrintdata = "";
	tDevice = ((CMUXDriver*)GetFather(this))->cDevice;
	
	while(printThread.IsTerminated() == 0){
		strRecData = "";
		byteNum = cgRxBuffer.Used();
		if (byteNum == 3){
			cgRxBuffer.Spin_InUse_set();
			cgRxBuffer.cgBufFIFO.ReadInASCII(&strRecData,3,0);
			cgRxBuffer.Spin_InUse_clr();
			if (strRecData == "+++"){
				modemStatus = 0;
				if (vPortRTS == 0)
					modemStatus |= CMUX_MODEMSTATUS_RTS;
				strCommand = tCMUXFrame.CreateCMuxMSCCmd(cgTrackingID,modemStatus,CMUX_CR_BIT,CMUX_CR_BIT);
				strPrintdata = "CMUXFWThread::Receive +++ command from virtual port";
				strPrintdata += Str_IntToString(cgTrackingID);
				strPrintdata += ", switch to DTR control.\r\n";
				strPrintdata += "Set DTR=1::Send: ";
				strPrintdata += Str_ASCIIToHEXs(strCommand,G_ESCAPE_OFF);
				strPrintdata += "\r\n";
				if (GetcgOutput() != nullptr){
					GetcgOutput()->Spin_InUse_set();
					GetcgOutput()->WriteDividingLine(RICH_CF_clMaroon);
					GetcgOutput()->WriteStrN(strPrintdata,RICH_CF_clPurple);
					GetcgOutput()->WriteDividingLine(RICH_CF_clMaroon);
					GetcgOutput()->Spin_InUse_clr();
				}
				tDevice->WriteInASCII(strCommand,G_ESCAPE_OFF);
				SYS_SleepMS(200);
				modemStatus |= CMUX_MODEMSTATUS_DTR;
				strCommand = tCMUXFrame.CreateCMuxMSCCmd(cgTrackingID,modemStatus,CMUX_CR_BIT,CMUX_CR_BIT);
				strPrintdata = "Set DTR=0::Send: ";
				strPrintdata += Str_ASCIIToHEXs(strCommand,G_ESCAPE_OFF);
				strPrintdata += "\r\n";
				if (GetcgOutput() != nullptr){
					GetcgOutput()->Spin_InUse_set();
					GetcgOutput()->WriteDividingLine(RICH_CF_clMaroon);
					GetcgOutput()->WriteStrN(strPrintdata,RICH_CF_clPurple);
					GetcgOutput()->WriteDividingLine(RICH_CF_clMaroon);
					GetcgOutput()->Spin_InUse_clr();
				}
				tDevice->WriteInASCII(strCommand,G_ESCAPE_OFF);
				SYS_SleepMS(200);
			}
		}
		while(cgRxBuffer.Used() > 0){
			cgRxBuffer.Spin_InUse_set();
			strCommand = tCMUXFrame.CreateCMuxUIHFrame(cgTrackingID,cgRxBuffer.cgBufFIFO,CMUX_CR_BIT);
			cgRxBuffer.cgBufFIFO.Out(tCMUXFrame.fn_Info.GetLength());
			cgRxBuffer.Spin_InUse_clr();
			tDevice->WriteInASCII(strCommand,G_ESCAPE_OFF);
			strPrintdata = Str_ASCIIToHEXs(strCommand,G_ESCAPE_OFF);
			strPrintdata += '(';
			if (vPortHEX == 0){
				if (vPortEscape == 0){
					tCMUXFrame.fn_Info.ReadAllContent(&strPrintdata);
				}
				else{
					strTemp = "";
					tCMUXFrame.fn_Info.ReadAllContent(&strTemp);
					strPrintdata += Str_UnTransferChar(strTemp);
				}
			}
			else{
				strTemp = "";
				tCMUXFrame.fn_Info.ReadAllContent(&strTemp);
				strPrintdata += Str_RTrim(Str_ASCIIToHEXs(strTemp,G_ESCAPE_OFF));
			}
			strPrintdata += ")\r\n";
			
			strTemp = "Virtual Port ";
			strTemp += Str_IntToString(cgTrackingID);
			if (GetcgOutput() != nullptr){
				GetcgOutput()->Spin_InUse_set();
				GetcgOutput()->WriteDividingLine(RICH_CF_clMaroon);
				if (tDevice->cgCurrentDB != nullptr)
					GetcgOutput()->WriteRxTitle(tDevice->cgCurrentDB->GetBufName(), strTemp, (uint32)strCommand.length() - 6, RxBytes(), RICH_CF_clMaroon);
				GetcgOutput()->WriteStr(strPrintdata,RICH_CF_clBlack);
				GetcgOutput()->WriteDividingLine(RICH_CF_clMaroon);
				GetcgOutput()->Spin_InUse_clr();
			}
		}
		SYS_SleepMS(10);
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
int32 CMUXCOM::OpenDev(const std::string &tCDBufName,int32 tCDBufPar,CSType tCSType){
	if (IsConnected() > 0)
		return 1;
#ifdef CommonDefH_Unix
	std::string		strName;
	
	strName = CMUX_COM_NAME + Str_IntToString(cgTrackingID);
	
	if (APICOM::OpenVDev(strName,tCDBufPar,COMMU_DBUF::CSType_None) == 0)
		return 0;
	return 1;
#endif
#ifdef CommonDefH_VC
	std::string	strPrintData;
	
	if (APICOM::OpenDev(tCDBufName,tCDBufPar,COMMU_DBUF::CSType_None) == 0)
		return 0;
	
	strPrintData = SYS_MakeTimeNow();
	strPrintData += " Open CMUX virtual port";
	strPrintData += Str_IntToString(cgTrackingID);
	strPrintData += ' ';
	strPrintData += tCDBufName;
	strPrintData += ':';
	strPrintData += Str_IntToString(tCDBufPar);
	strPrintData += ".\r\n";
	if (GetcgOutput() != nullptr){
		GetcgOutput()->Spin_InUse_set();
		GetcgOutput()->WriteDividingLine(RICH_CF_clMaroon, COLSTRING::COL_EP_YES);
		GetcgOutput()->WriteStrN(strPrintData, RICH_CF_clPurple, COLSTRING::COL_EP_YES);
		GetcgOutput()->WriteDividingLine(RICH_CF_clMaroon, COLSTRING::COL_EP_YES);
		GetcgOutput()->Spin_InUse_clr();
	}
	return 1;
#endif	
}
//------------------------------------------------------------------------------------------//
void CMUXCOM::CloseDev(void){
	std::string		name;
	name = GetBufName();
	APICOM::CloseDev();
#ifdef CommonDefH_VC
	std::string	strPrintData;
	strPrintData = SYS_MakeTimeNow();
	strPrintData += " Close CMUX virtual port";
	strPrintData += Str_IntToString(cgTrackingID);
	strPrintData += ' ';
	strPrintData += name;
	strPrintData += ".\r\n";
	if (GetcgOutput() != nullptr){
		GetcgOutput()->Spin_InUse_set();
		GetcgOutput()->WriteDividingLine(RICH_CF_clMaroon, COLSTRING::COL_EP_YES);
		GetcgOutput()->WriteStrN(strPrintData, RICH_CF_clPurple, COLSTRING::COL_EP_YES);
		GetcgOutput()->WriteDividingLine(RICH_CF_clMaroon, COLSTRING::COL_EP_YES);
		GetcgOutput()->Spin_InUse_clr();
	}
#endif
}
//------------------------------------------------------------------------------------------//



//------------------------------------------------------------------------------------------//
CMUXDriver::CMUXDriver(void) : RTREE_NODE(){
	CreateTrash(this);
	deliveryThread.ThreadInit(this, &CMUXDriver::DeliveryThreadFun);
	commandThread.ThreadInit(this, &CMUXDriver::CommandThreadFun);
	ClrblStart();
	Init(nullptr,0);
}
//------------------------------------------------------------------------------------------//
CMUXDriver::CMUXDriver(const DEVICE *tDevice,uint32 tSize) : RTREE_NODE(){
	CreateTrash(this);
	deliveryThread.ThreadInit(this, &CMUXDriver::DeliveryThreadFun);
	commandThread.ThreadInit(this, &CMUXDriver::CommandThreadFun);
	ClrblStart();
	Init(tDevice,tSize);
}
//------------------------------------------------------------------------------------------//
void CMUXDriver::Init(const DEVICE *tDevice,uint32 tSize){
	if (CheckblStart() == 0){
		cgCommandMode = 0;
		cgBufMaxSize = tSize;
		cDevice = (DEVICE*)tDevice;
	}
}
//------------------------------------------------------------------------------------------//
CMUXCOM *CMUXDriver::Find_nolock(int32 tTrackingID){
	CMUXCOM		*retCOM;
	
	retCOM = nullptr;
	RTREE_LChildRChain_Find(CMUXCOM,this,retCOM,(operateNode_t->cgTrackingID == tTrackingID));
	return(retCOM);
}
//------------------------------------------------------------------------------------------//
CMUXCOM *CMUXDriver::SetSelect(int32 tTrackingID){
	CMUXCOM	*ret;
	
	ret = Find_nolock(tTrackingID);
	if (ret != nullptr){
		LClrblSelected();
		ret->SetblSelected();
	}
	return(ret);
}
//------------------------------------------------------------------------------------------//
CMUXCOM *CMUXDriver::VCOMOpen(int32 dlci,const std::string &tCOMName,int32 baudrate){
	CMUXCOM	*tCMUXCOM;
	
	if (dlci > Max_CMUXCOM)
		return(nullptr);
	Spin_InUse_set();
	tCMUXCOM = Find_nolock(dlci);
	if (tCMUXCOM == nullptr){
		tCMUXCOM = new CMUXCOM(&cDevice->cgODevList,cgBufMaxSize);
		if (tCMUXCOM != nullptr){
			tCMUXCOM->cgTrackingID = dlci;
			AddNode(tCMUXCOM);
		}
	}
	if (tCMUXCOM != nullptr){
		if (tCMUXCOM->OpenD(tCOMName,baudrate,COMMU_DBUF::CSType_None,0) == 0)
			tCMUXCOM = nullptr;
	}
	Spin_InUse_clr();
	return(tCMUXCOM);
}
//------------------------------------------------------------------------------------------//
int32 CMUXDriver::VCOMClose(int32 dlci,int32 blPrint){
	CMUXCOM	*tCMUXCOM;
	int32 	ret;
	
	if (dlci > Max_CMUXCOM)
		return 0;
	Spin_InUse_set();
	tCMUXCOM = Find_nolock(dlci);
	ret = 0;
	if (tCMUXCOM != nullptr){
		tCMUXCOM->CloseD(blPrint);
		MoveNodeToTrash(tCMUXCOM,this);
		CleanTrash(this);
		ret = 1;
	}
	Spin_InUse_clr();
	return(ret);
}
//------------------------------------------------------------------------------------------//
int32 CMUXDriver::DriverStart(void){
	int32	ret;
	ret = 0;
	if (InDoing_try() != 0){
		ret = DriverStart_Do();
		InDoing_clr();
	}
	return(ret);
}
//------------------------------------------------------------------------------------------//
int32 CMUXDriver::DriverStop(void){
	int32	ret;
	ret = 0;
	if (InDoing_try() != 0){
		ret = DriverStop_Do();
		InDoing_clr();
	}
	return(ret);
}
//------------------------------------------------------------------------------------------//
int32 CMUXDriver::DriverStart_Do(void){
	if (CheckblStart() == 0){
		deliveryThread.ThreadRun();
		SetblStart();
	}
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 CMUXDriver::DriverStop_Do(void){
	if (CheckblStart() != 0){
		CMUXClose();
		commandThread.ThreadStop();
		deliveryThread.ThreadStop();
		ClrblStart();
	}
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 CMUXDriver::CMUXStart(int32 blTelit){
#ifdef	SWVERSION_CMUX
	if (cDevice->cgDevType != DEVICE::DEVID_APICOM)
		return 0;
	if (InDoing_try() != 0){
		#ifdef CommonDefH_Unix
			cDevice->cgODevList.cgOutput ->WriteStrN(DEV_LINE_START,RICH_LIN_clDefault);
			VCOMOpen(1,"",115200);
			VCOMOpen(2,"",115200);
			VCOMOpen(3,"",115200);
			VCOMOpen(4,"",115200);
			cDevice->cgODevList.cgOutput ->WriteStrN(DEV_LINE_START,RICH_LIN_clDefault);
		#endif
		DriverStart_Do();
		if (blTelit != 0){
			CMUXInit();
		}
		else{
			CMUXInitWoAT();
		}
		InDoing_clr();
	}
#endif
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 CMUXDriver::CMUXStop(void){
#ifdef	SWVERSION_CMUX
	if (CheckblStart() == 0)
		return 1;
	if (InDoing_try() != 0){
		#ifdef CommonDefH_Unix
		cDevice->cgODevList.cgOutput ->WriteStrN(DEV_LINE_START,RICH_LIN_clDefault);
		VCOMClose(1,1);
		VCOMClose(2,1);
		VCOMClose(3,1);
		VCOMClose(4,1);
		cDevice->cgODevList.cgOutput ->WriteStrN(DEV_LINE_START,RICH_LIN_clDefault);
		#endif
		DriverStop_Do();
		InDoing_clr();
	}
#endif
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 CMUXDriver::DeliveryThreadFun(void){
	SMC_EncryptI(0)
	PUB_SBUF 			gBuffer;
	CMUX_FRAME			cCMUXFrame;

	SMC_EncryptS(0)
	gBuffer.Init(cgBufMaxSize);

	cDevice->rxBufferList.AddNode(&gBuffer);
	SMC_EncryptE(0)
	while(deliveryThread.IsTerminated() == 0){
		gBuffer.Spin_InUse_set();
		Spin_InUse_set();
		while(gBuffer.Used() > 0){
			if (cCMUXFrame.TryGetFrameAgain_if_NoEnoughDataInFIFO(&gBuffer.cgBufFIFO) < 1)
				break;
			DeliveryRxFrame(&cCMUXFrame);
		}
		gBuffer.Spin_InUse_clr();
		Spin_InUse_clr();
		SYS_SleepMS(10);
	}
	gBuffer.RemoveSelf();;
	return 1;
}
//------------------------------------------------------------------------------------------//
void CMUXDriver::DeliveryRxFrame(CMUX_FRAME *tCMUXFrame){
	CMUXCOM			*tCMUXCOM;
	uint8			ctrlType;
	uint32			vPortNumber;
	std::string		strPrintData;
	
	// Check DLCI: Addr field:  |dlci|C/R|E/A|
	vPortNumber = tCMUXFrame->fn_Address_DLCI.GetValueCalc();
	tCMUXCOM = nullptr;
	if (vPortNumber > Max_CMUXCOM){
		PrintERRORToMemoViewer("CMUXThread::DLCI vaule is invaild\r\n");
		return;
	}
	
	tCMUXCOM = Find_nolock(vPortNumber);
	
	// Check CTRL: CTRL field:  |X|X|X|X|P/F|X|X|X|
	// Suppose P/F bit in control field always = 0
	ctrlType = tCMUXFrame->fn_Control_Type.GetValueCalc();
	
	switch(ctrlType){
		case CMUX_CTRL_SABM:{
			PrintERRORToMemoViewer("CMUXThread::SABM frame received, MS wants connect to me, not implemented Ctrl Type\r\n");
			return;
		}
		case CMUX_CTRL_UA:{
			strPrintData = "CMUXThread::UA response frame recevied: ";
			if (vPortNumber > 0){
				strPrintData += "Port";
				strPrintData += Str_IntToString(vPortNumber);
			}
			else{
				strPrintData += "DLCI=0";
			}
			strPrintData += "\r\n";
			PrintERRORToMemoViewer(strPrintData);
			return;
		}
		case CMUX_CTRL_DM:{
			strPrintData = "CMUXThread::DM response frame recevied: ";
			if (vPortNumber > 0){
				strPrintData += "Port";
				strPrintData += Str_IntToString(vPortNumber);
			}
			else{
				strPrintData += "DLCI=0";
			}
			strPrintData += "\r\n";
			PrintERRORToMemoViewer(strPrintData);
			return;
		}
		case CMUX_CTRL_DISC:{
			PrintERRORToMemoViewer("CMUXThread::DISC frame received, MS wants disconnect to me, not implemented Ctrl Type\r\n");
			return;
		}
		case CMUX_CTRL_UIH:{
			if(vPortNumber == CMUX_CTRL_DLCI_INDEX){
				CMuxCtrlCmdExec(tCMUXFrame);
			}
			else if (tCMUXCOM != nullptr){
				Spin_InUse_set();
				strPrintData = "";
				tCMUXFrame->fn_Info.ReadAllContent(&strPrintData);
				tCMUXCOM->WriteInASCII(strPrintData,G_ESCAPE_OFF);
				Spin_InUse_clr();
			}
			return;
		}
		case CMUX_CTRL_UI:{
			PrintERRORToMemoViewer("CMUXThread::UI frame received, not implemented Ctrl Type\r\n");
			return;
		}
		default:{
			PrintERRORToMemoViewer("CMUXThread::CTRL Field |X|X|X|X|P/F|X|X|X| is invaild\r\n");
			return;
		}
	}
}
//------------------------------------------------------------------------------------------//
void CMUXDriver::CMuxCtrlCmdExec(CMUX_FRAME *tCMUXFrame){
	//There is no specified response to the UIH cpmmand/response
	//MSC Command Application->Module => C/R bit (RED) 1 and C/R bit (BLUE) 1
	//MSC Command Module->Application => C/R bit (RED) 0 and C/R bit (BLUE) 1
	//MSC Response Application->Module => C/R bit (RED) 1 and C/R bit (BLUE) 0
	//MSC Response Module->Application => C/R bit (RED) 0 and C/R bit (BLUE) 0
	
	uint8 				cmdType,frameCRbit,MSCTypeCRbit,charTemp;
	std::string			strMSCCommand,srtPrintData,strInfo;
	uint32				cMSCDlci,cModemStatus;
	
	strInfo = "";
	tCMUXFrame->fn_Info.ReadAllContent(&strInfo);
	charTemp = strInfo[0];
	cmdType = charTemp & ~(CMUX_CR_BIT | CMUX_EA_BIT);
	switch(cmdType){
		case CMUX_TEST_CMD:{ // Test command
			PrintERRORToMemoViewer("CMUXThread::Test Command, not support in current version.\r\n");
			break;
		}
		case CMUX_MSC_CMD:{ // Modem Status Command
			charTemp = strInfo[0];
			frameCRbit = tCMUXFrame->fn_Address_CR.GetValueAMask();//addr & CMUX_CR_BIT;
			MSCTypeCRbit = charTemp & CMUX_CR_BIT;
			
			charTemp = strInfo[2];
			cMSCDlci = (charTemp >> 2);// Get DLCI from frame
			charTemp = strInfo[3];
			cModemStatus = charTemp;
			if ((frameCRbit == 0) && (MSCTypeCRbit != 0)){//MSC Command Module->Application => C/R bit (RED) 0 and C/R bit (BLUE) 1
				srtPrintData = "CMUXThread::MSC update frame recevied: Port";
				srtPrintData += Str_IntToString(cMSCDlci);
				srtPrintData += "\r\n";
				PrintERRORToMemoViewer(srtPrintData);
				UpdateModemStatus(cMSCDlci,cModemStatus);
				
				strMSCCommand = tCMUXFrame->CreateCMuxMSCCmd(cMSCDlci,cModemStatus,CMUX_CR_BIT,0);
				//MSC Response Application->Module => C/R bit (RED) 1 and C/R bit (BLUE) 0
				cDevice->WriteInASCII(strMSCCommand,G_ESCAPE_OFF);
				srtPrintData = "CMUXThread::Send MSC confirm frame: \r\n" + Str_ASCIIToHEXs(strMSCCommand,G_ESCAPE_OFF) + "\r\n";
				if (cDevice->cgODevList.cgOutput != nullptr)
					cDevice->cgODevList.cgOutput->WriteStrN(srtPrintData,RICH_CF_clPurple,COLSTRING::COL_EP_YES);
			}
			else{
				srtPrintData = "CMUXThread::MSC response frame recevied: Port";
				srtPrintData += Str_IntToString(cMSCDlci);
				srtPrintData += "\r\n";
				PrintERRORToMemoViewer(srtPrintData);
			}
			break;
		}
		case CMUX_PSC_CMD:{ // Power Saving Control: Not Impl.
			charTemp = strInfo[0];
			//frameCRbit = CMUXFrame->addr & CMUX_CR_BIT;
			MSCTypeCRbit = charTemp & CMUX_CR_BIT;
			if (MSCTypeCRbit == 0){
				PrintERRORToMemoViewer("CMUXThread::PSC response frame received.\r\n");
			}
			else{
				PrintERRORToMemoViewer("CMUXThread::PSC frame received, Not support in current version.\r\n");
			}
			break;
		}
		case CMUX_NSC_RES:{ // Not Supported Command: Not Impl.
			PrintERRORToMemoViewer("CMUXThread::Not supported CMD received.\r\n");
			break;
		}
		case CMUX_PN_CMD:{      // Parameter Negotiation: Not Impl.
			PrintERRORToMemoViewer("CMUXThread::Parameter Negotiation CMD received, not supported CMD received.\r\n");
			break;
		}
		case CMUX_MUX_CLD_CMD:{ // CMux Close Down: Not Impl.
			PrintERRORToMemoViewer("CMUXThread::CMux Close Down CMD received, not supported CMD received.\r\n");
			break;
		}
		case CMUX_FCON_CMD:{    // Flow Control ON: Not Impl.
			PrintERRORToMemoViewer("CMUXThread::Flow Control ON CMD received, not supported CMD received.\r\n");
			break;
		}
		case CMUX_FCOFF_CMD:{   // Flow Control OFF: Not Impl.
			PrintERRORToMemoViewer("CMUXThread::Flow Control OFF CMD received, not supported CMD received.\r\n");
			break;
		}
		case CMUX_RPN_CMD:{     // Remote Port Negotiation: Not Impl.
			PrintERRORToMemoViewer("CMUXThread::Remote Port Negotiation CMD received, not supported CMD received.\r\n");
			break;
		}
		case CMUX_RLS_CMD:{     // Remote Line Status: Not Impl.
			PrintERRORToMemoViewer("CMUXThread::Remote Line Status CMD received, not supported CMD received.\r\n");
			break;
		}
		case CMUX_SNC_CMD:{     // Service Negotiation Command: Not Impl.
			PrintERRORToMemoViewer("CMUXThread::Service Negotiation CMD received, not supported CMD received.\r\n");
			break;
		}
		default:
			PrintERRORToMemoViewer("CMUXThread::Other not supported CMD received.\r\n");
	}
}
//------------------------------------------------------------------------------------------//
void CMUXDriver::UpdateModemStatus(uint32 cMSCDlci,uint32 cModemStatus){
	std::string		strPrintData;
	CMUXCOM			*tCMUXCOM;
	
	tCMUXCOM = Find_nolock(cMSCDlci);
	
	if (tCMUXCOM != nullptr){
		strPrintData = "CMUXThread::Received Modem Status: ";
		strPrintData += "Port";
		strPrintData += Str_IntToString(cMSCDlci);
		strPrintData += "\r\n";
		if (cModemStatus & CMUX_MODEMSTATUS_CTS){
			tCMUXCOM->vPortCTS = 0;
			strPrintData += "CTS=1";
		}
		else{
			tCMUXCOM->vPortCTS = 1;
			strPrintData += "CTS=0";
		}
		if (cModemStatus & CMUX_MODEMSTATUS_DSR){
			tCMUXCOM->vPortDSR = 0;
			strPrintData += ",DSR=1";
		}
		else{
			tCMUXCOM->vPortDSR = 1;
			strPrintData += ",DSR=0";
		}
		if (cModemStatus & CMUX_MODEMSTATUS_RING){
			tCMUXCOM->vPortRING = 0;
			strPrintData += ",RING=1";
		}
		else{
			tCMUXCOM->vPortRING = 1;
			strPrintData += ",RING=0";
		}
		if (cModemStatus & CMUX_MODEMSTATUS_DCD){
			tCMUXCOM->vPortDCD = 0;
			strPrintData += ",DCD=1";
		}
		else{
			tCMUXCOM->vPortDCD = 1;
			strPrintData += ",DCD=0";
		}
		strPrintData += "\r\n";
		if (cDevice->cgODevList.cgOutput != nullptr)
			cDevice->cgODevList.cgOutput ->WriteStrN(strPrintData,RICH_CF_clPurple,COLSTRING::COL_EP_YES);
	}
}
//------------------------------------------------------------------------------------------//
void CMUXDriver::PrintERRORToMemoViewer(const std::string &strError){
	if (cDevice->cgODevList.cgOutput != nullptr){
		cDevice->cgODevList.cgOutput->Spin_InUse_set();
		cDevice->cgODevList.cgOutput->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
		cDevice->cgODevList.cgOutput->WriteStrN(strError,RICH_CF_clRed,COLSTRING::COL_EP_YES);
		cDevice->cgODevList.cgOutput->Spin_InUse_clr();
	}
}
//------------------------------------------------------------------------------------------//
void CMUXDriver::PrintCMD(const std::string &title,const std::string &strOutput){
	cDevice->SendCommandOnly(strOutput,0,1,G_ESCAPE_OFF);
	if (cDevice->cgODevList.cgOutput != nullptr){
		cDevice->cgODevList.cgOutput->Spin_InUse_set();
		cDevice->cgODevList.cgOutput->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
		cDevice->cgODevList.cgOutput->WriteStrN(title,RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
		cDevice->cgODevList.cgOutput->WriteStr(strOutput + "\\r\r\n",RICH_CF_clBlack,COLSTRING::COL_EP_YES);
		cDevice->cgODevList.cgOutput->Spin_InUse_clr();
	}
	//SYS_SleepMS(200);
}
//------------------------------------------------------------------------------------------//
void CMUXDriver::PrintCMUXCMD(const std::string &title,const std::string &strOutput){
	std::string strPrintData;
	
	strPrintData = Str_ASCIIToHEXs(strOutput,G_ESCAPE_OFF) + "\r\n";
	cDevice->SendCommandOnly(strOutput,0,0,G_ESCAPE_OFF);
	if (cDevice->cgODevList.cgOutput != nullptr){
		cDevice->cgODevList.cgOutput->Spin_InUse_set();
		cDevice->cgODevList.cgOutput->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
		cDevice->cgODevList.cgOutput->WriteStrN(title,RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
		cDevice->cgODevList.cgOutput->WriteStr(strPrintData,RICH_CF_clBlack,COLSTRING::COL_EP_YES);
		cDevice->cgODevList.cgOutput->Spin_InUse_clr();
	}
	//SYS_SleepMS(200);
}
//------------------------------------------------------------------------------------------//
int32 CMUXDriver::CommandThreadFun(void){
	PUB_SBUF cBuffer;
	
	cBuffer.Init(cgBufMaxSize);
	
	cBuffer.Clean();
	cDevice->rxBufferList.AddNode(&cBuffer);
	switch(cgCommandMode){
		case 0:
			if (CMUXInitTelitAT(&cBuffer) != 0)
				CMUXInitCT(&cBuffer);
			break;
		case 1:
			CMUXInitCT(&cBuffer);
			break;
		case 2:
			CMUXCloseCT(&cBuffer);
			break;
		default:;
	}
	cgCommandMode = -1;
	cBuffer.RemoveSelf();
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 CMUXDriver::CMUXInitTelitAT(PUB_SBUF *cBuffer){
	CMUX_FRAME		cCMUXFrame;
	std::string		strOutput;
	int32			blOK;
	
	blOK = 0;
	B_ClrFLAG64(cDevice->cgODevList.cgODevFlagU64,ODEV_FLAG_EnHEXViewMode);
	do{
		PrintCMD("CMUXInitThread::Send:","AT");				if (cDevice->CheckResult(cBuffer,"OK",1000) == 0){SYS_SleepMS(300);break;}
		PrintCMD("CMUXInitThread::Send:","AT");				if (cDevice->CheckResult(cBuffer,"OK",1000) == 0){SYS_SleepMS(300);break;}
		PrintCMD("CMUXInitThread::Send:","AT");				if (cDevice->CheckResult(cBuffer,"OK",1000) == 0){SYS_SleepMS(300);break;}
		PrintCMD("CMUXInitThread::Send:","AT+IPR=115200");	if (cDevice->CheckResult(cBuffer,"OK",1000) == 0){SYS_SleepMS(300);break;}
		PrintCMD("CMUXInitThread::Send:","ATE0V1&K3&D2");	if (cDevice->CheckResult(cBuffer,"OK",1000) == 0){SYS_SleepMS(300);break;}
		PrintCMD("CMUXInitThread::Send:","AT+CMEE=2");		if (cDevice->CheckResult(cBuffer,"OK",1000) == 0){SYS_SleepMS(300);break;}
		PrintCMD("CMUXInitThread::Send:","AT+CGMI");		if (cDevice->CheckResult(cBuffer,"OK",1000) == 0){SYS_SleepMS(300);break;}
		PrintCMD("CMUXInitThread::Send:","AT+CGMM");		if (cDevice->CheckResult(cBuffer,"OK",1000) == 0){SYS_SleepMS(300);break;}
		PrintCMD("CMUXInitThread::Send:","AT+CGMR");		if (cDevice->CheckResult(cBuffer,"OK",1000) == 0){SYS_SleepMS(300);break;}
		PrintCMD("CMUXInitThread::Send:","AT#SELINT=2");	if (cDevice->CheckResult(cBuffer,"OK",1000) == 0){SYS_SleepMS(300);}
		PrintCMD("CMUXInitThread::Send:","AT#CMUXMODE=1");	if (cDevice->CheckResult(cBuffer,"OK",1000) == 0){SYS_SleepMS(300);}
		blOK = 1;
	}while(0);
	
	if (cDevice->cgODevList.cgOutput != nullptr){
		cDevice->cgODevList.cgOutput ->Spin_InUse_set();
		cDevice->cgODevList.cgOutput ->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
		cDevice->cgODevList.cgOutput ->WriteStrN("CMUXInitThread::Finish the external initialization.\r\n",RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
		cDevice->cgODevList.cgOutput ->Spin_InUse_clr();
	}
	return(blOK);
}
//------------------------------------------------------------------------------------------//
void CMUXDriver::CMUXInitCT(PUB_SBUF *cBuffer){
	CMUX_FRAME		cCMUXFrame;
	std::string		strOutput;
	
	B_ClrFLAG64(cDevice->cgODevList.cgODevFlagU64,ODEV_FLAG_EnHEXViewMode);
	PrintCMD("CMUXInitThread::Send:","AT+CMUX=0");
	if (cDevice->CheckResult(cBuffer,"OK",5000) != 0){
		SYS_SleepMS(200);
		B_SetFLAG64(cDevice->cgODevList.cgODevFlagU64,ODEV_FLAG_EnHEXViewMode);
		PrintCMUXCMD("CMUXInitThread::Send SABM Frame. DLCI=0::\r\n",cCMUXFrame.CreateCMuxSABMFrameD0());
		cDevice->CheckResult(cBuffer, "!'nullptr", 2000);SYS_SleepMS(300);
		PrintCMUXCMD("CMUXInitThread::Send SABM Frame. Enable virtual port1::\r\n",cCMUXFrame.CreateCMuxSABMFrameV1());
		cDevice->CheckResult(cBuffer, "!'nullptr", 2000);SYS_SleepMS(300);
		strOutput = cCMUXFrame.CreateCMuxMSCCmd(CMUX_CTRL_DLCI_VPORT1,CMUX_MODEMSTATUS_DTR | CMUX_MODEMSTATUS_RTS,CMUX_CR_BIT,CMUX_CR_BIT);
		PrintCMUXCMD("CMUXInitThread::Send MSCCmd. Set DTR=1&RTS=1 on DLCI=1::\r\n",strOutput);
		cDevice->CheckResult(cBuffer, "!'nullptr", 2000);SYS_SleepMS(300);
		PrintCMUXCMD("CMUXInitThread::Send SABM Frame. Enable virtual port2::\r\n",cCMUXFrame.CreateCMuxSABMFrameV2());
		cDevice->CheckResult(cBuffer, "!'nullptr", 2000);SYS_SleepMS(300);
		strOutput = cCMUXFrame.CreateCMuxMSCCmd(CMUX_CTRL_DLCI_VPORT2,CMUX_MODEMSTATUS_DTR | CMUX_MODEMSTATUS_RTS,CMUX_CR_BIT,CMUX_CR_BIT);
		PrintCMUXCMD("CMUXInitThread::Send MSCCmd. Set DTR=1&RTS=1 on DLCI=2::\r\n",strOutput);
		cDevice->CheckResult(cBuffer, "!'nullptr", 2000);SYS_SleepMS(300);
		PrintCMUXCMD("CMUXInitThread::Send SABM Frame. Enable virtual port3::\r\n",cCMUXFrame.CreateCMuxSABMFrameV3());
		cDevice->CheckResult(cBuffer, "!'nullptr", 2000);SYS_SleepMS(300);
		strOutput = cCMUXFrame.CreateCMuxMSCCmd(CMUX_CTRL_DLCI_VPORT3,CMUX_MODEMSTATUS_DTR | CMUX_MODEMSTATUS_RTS,CMUX_CR_BIT,CMUX_CR_BIT);
		PrintCMUXCMD("CMUXInitThread::Send MSCCmd. Set DTR=1&RTS=1 on DLCI=3::\r\n",strOutput);
		cDevice->CheckResult(cBuffer, "!'nullptr", 2000);SYS_SleepMS(300);
		PrintCMUXCMD("CMUXInitThread::Send SABM Frame. Enable virtual port4::\r\n",cCMUXFrame.CreateCMuxSABMFrameV4());
		cDevice->CheckResult(cBuffer, "!'nullptr", 2000);SYS_SleepMS(300);
		strOutput = cCMUXFrame.CreateCMuxMSCCmd(CMUX_CTRL_DLCI_VPORT4,CMUX_MODEMSTATUS_DTR | CMUX_MODEMSTATUS_RTS,CMUX_CR_BIT,CMUX_CR_BIT);
		PrintCMUXCMD("CMUXInitThread::Send MSCCmd. Set DTR=1&RTS=1 on DLCI=4::\r\n",strOutput);
		cDevice->CheckResult(cBuffer, "!'nullptr", 2000);SYS_SleepMS(300);
	}
	if (cDevice->cgODevList.cgOutput != nullptr){
		cDevice->cgODevList.cgOutput->Spin_InUse_set();
		cDevice->cgODevList.cgOutput->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
		cDevice->cgODevList.cgOutput->WriteStrN("CMUXInitThread::End of the execution.\r\n",RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
		cDevice->cgODevList.cgOutput->Spin_InUse_clr();
	}
}
//------------------------------------------------------------------------------------------//
void CMUXDriver::CMUXCloseCT(PUB_SBUF *cBuffer){
	CMUX_FRAME	cCMUXFrame;
	B_SetFLAG64(cDevice->cgODevList.cgODevFlagU64,ODEV_FLAG_EnHEXViewMode);
	do{
		PrintCMUXCMD("CMUXDriver::Send DISC Frame, close virtual port4::\r\n", cCMUXFrame.CreateCMuxDISCFrameV4());
		cDevice->CheckResult(cBuffer, "!'nullptr", 2000);SYS_SleepMS(300);
		PrintCMUXCMD("CMUXDriver::Send DISC Frame, close virtual port3::\r\n", cCMUXFrame.CreateCMuxDISCFrameV3());
		cDevice->CheckResult(cBuffer, "!'nullptr", 2000);SYS_SleepMS(300);
		PrintCMUXCMD("CMUXDriver::Send DISC Frame, close virtual port2::\r\n", cCMUXFrame.CreateCMuxDISCFrameV2());
		cDevice->CheckResult(cBuffer, "!'nullptr", 2000);SYS_SleepMS(300);
		PrintCMUXCMD("CMUXDriver::Send DISC Frame, close virtual port1::\r\n", cCMUXFrame.CreateCMuxDISCFrameV1());
		cDevice->CheckResult(cBuffer, "!'nullptr", 2000);SYS_SleepMS(300);
		PrintCMUXCMD("CMUXDriver::Send DISC Frame DLCI=0::\r\n", cCMUXFrame.CreateCMuxDISCFrameD0());
		cDevice->CheckResult(cBuffer, "!'nullptr", 2000);SYS_SleepMS(300);
	} while (0);
	deliveryThread.ThreadStop();
	B_ClrFLAG64(cDevice->cgODevList.cgODevFlagU64,ODEV_FLAG_EnHEXViewMode);
	ClrblStart();
}
//------------------------------------------------------------------------------------------//
void CMUXDriver::CMUXCLDCT(void){
	CMUX_FRAME	cCMUXFrame;
	
	B_SetFLAG64(cDevice->cgODevList.cgODevFlagU64,ODEV_FLAG_EnHEXViewMode);
	PrintCMUXCMD("CMUXDriver::Send CLD frame, exit CMUX mode::\r\n",cCMUXFrame.CreateCMuxCLDFrame());
	SYS_SleepMS(100);
	B_ClrFLAG64(cDevice->cgODevList.cgODevFlagU64,ODEV_FLAG_EnHEXViewMode);
}
//------------------------------------------------------------------------------------------//
void CMUXDriver::CMUXStdPSCCT(uint8 cmode){
	CMUX_FRAME		cCMUXFrame;
	std::string		strOutput;
	B_SetFLAG64(cDevice->cgODevList.cgODevFlagU64,ODEV_FLAG_EnHEXViewMode);
	if (cmode == 0xff){
		PrintCMUXCMD("CMUXDriver::Send standard PCS frame::\r\n",cCMUXFrame.CreateCMuxStdPSCFrame());
	}
	else{
		strOutput = "CMUXDriver::Send Telit PCS frame, mode";
		strOutput += Str_IntToString(cmode);
		strOutput += "::\r\n";
		PrintCMUXCMD(strOutput,cCMUXFrame.CreateCMuxTPSCxFrame(cmode));
	}
	SYS_SleepMS(100);
}
//------------------------------------------------------------------------------------------//
void CMUXDriver::CMUXFCCCT(int32 blIsOn){
	CMUX_FRAME	cCMUXFrame;
	B_SetFLAG64(cDevice->cgODevList.cgODevFlagU64,ODEV_FLAG_EnHEXViewMode);
	if (blIsOn){
		PrintCMUXCMD("CMUXDriver::Send FCon frame::\r\n",cCMUXFrame.CreateCMuxFCONFrame(CMUX_CR_BIT));
	}
	else{
		PrintCMUXCMD("CMUXDriver::Send FCoff frame::\r\n",cCMUXFrame.CreateCMuxFCOFFFrame(CMUX_CR_BIT));
	}
	SYS_SleepMS(100);
}
//------------------------------------------------------------------------------------------//
void CMUXDriver::CMUXMSCCT(int32 vPortNumber,int32 blDTR,int32 blRTS){
	CMUX_FRAME		cCMUXFrame;
	uint8			modemStatus;
	std::string		strOutput1,strOutput;
	
	B_SetFLAG64(cDevice->cgODevList.cgODevFlagU64,ODEV_FLAG_EnHEXViewMode);
	modemStatus = 0;
	strOutput1 = "CMUXDriver::Send MSC CMD, Set DTR=";
	if (blDTR == 0){
		modemStatus |= CMUX_MODEMSTATUS_DTR;
		strOutput1 += "1";
	}
	else{
		strOutput1 += "0";
	}
	strOutput1 += "&RTS=";
	if (blRTS == 0)	{
		modemStatus |= CMUX_MODEMSTATUS_RTS;
		strOutput1 += "1";
	}
	else{
		strOutput1 += "0";
	}
	strOutput1 += " on DLCI=";
	strOutput1 += Str_IntToString(vPortNumber);
	strOutput1 += "::\r\n";
	strOutput = cCMUXFrame.CreateCMuxMSCCmd(vPortNumber,modemStatus,CMUX_CR_BIT,CMUX_CR_BIT);
	
	PrintCMUXCMD(strOutput1,strOutput);
	SYS_SleepMS(100);
}
//------------------------------------------------------------------------------------------//
void CMUXDriver::CMUXInit(void){
	if (commandThread.InDoing_try() != 0){
		cgCommandMode = 0;
		commandThread.ThreadStop();
		commandThread.ThreadRun();
		commandThread.InDoing_clr();
	}
}
//------------------------------------------------------------------------------------------//
void CMUXDriver::CMUXInitWoAT(void){
	if (commandThread.InDoing_try() != 0){
		cgCommandMode = 1;
		commandThread.ThreadStop();
		commandThread.ThreadRun();
		commandThread.InDoing_clr();
	}
}
//------------------------------------------------------------------------------------------//
void CMUXDriver::CMUXClose(void){
	if (commandThread.InDoing_try() != 0){
		LVCOMClose();
		cgCommandMode = 2;
		commandThread.ThreadStop();
		commandThread.ThreadRun();
		commandThread.InDoing_clr();
	}
}
//------------------------------------------------------------------------------------------//
#endif
//------------------------------------------------------------------------------------------//