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
#include "CMUX.h"
#include "Comm_Convert.h"
#include "SMC.h"
//------------------------------------------------------------------------------------------//
#define LOGPRINT_ENABLE
#define LOGPRINT_ENABLE2
#include "Comm_Log.h"
#ifdef	CMUXH
//------------------------------------------------------------------------------------------//
static const uint8 CMUX_crctable[256] = { // reversed, 8-bit, poly=0x07
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
int32 CMUX_FRAME::ChecksumResult(const FIFO8 &fifo)const{
	uint8	FCS;
	uint32	len,i;
	uint8	charTemp;
	
	charTemp = 0;
	FCS = 0xFF;
	len = 0 + ((pn_Control_Type.GetValueAMask(&fifo) == CMUX_CTRL_UIH) ? pn_Info.GetOffset() : pn_FCS.GetOffset());
	i = fnOffset;
	while(++i < len){
		if (fifo.Read(&charTemp, 1, i) == 1)
			FCS = CMUX_crctable[FCS ^ charTemp];
	}
	FCS = CMUX_crctable[FCS ^ pn_FCS.GetOriginalValue(&fifo)];
	return(FCS == 0xCF);
}
//------------------------------------------------------------------------------------------//
uint8 CMUX_FRAME::CalcFCS(void){
	uint8	FCS;
	uint32	len,i;
	uint8	charTemp;
	
	FCS = 0xFF;
	charTemp = 0;
	len = (pn_Control_Type.GetValueAMask(GetDefFifo()) == CMUX_CTRL_UIH) ? pn_Info.GetOffset() : (pn_LengthIndicator.GetOffset() + pn_LengthIndicator.GetLength());
	i = fnOffset;
	while(++i < len){
		if (GetDefFifo()->ReadOffsetByIn(&charTemp,1,i) == 1)
			FCS = CMUX_crctable[FCS ^ charTemp];
	}
	return(0xFF - FCS);
}
//------------------------------------------------------------------------------------------//
uint32 CMUX_FRAME::AddCMuxUIHFrame(STDSTR *retStr,uint8 vPort,const uint8 *data,uint32 num,uint8 crBit){
	_Start();
	
	pn_Head.SetFIFOFixValue();										//0:Head Flag
	pn_Address.SetFIFO_1Byte((vPort << 2) | crBit | CMUX_EA_BIT);	//1:Address field
	pn_Control.SetFIFO_1Byte(CMUX_CTRL_UIH & (~CMUX_PF_BIT));		//2:Ctrl field,Poll/Final BIT always 1
	
	if (num > CMUX_MAX_INFO_SIZE)
		num = CMUX_MAX_INFO_SIZE;
	if (num < CMUX_TWOBYTES_FRAME_SIZE){
		pn_LengthIndicator.SetObyteNum(1);
		pn_LengthIndicator.SetFIFO_1Byte((num << 1) | CMUX_EA_BIT);//3:Length indicator
	}
	else{
		pn_LengthIndicator.SetObyteNum(2);
		pn_LengthIndicator.SetFIFO_2Byte((num << 1) );
	}
	
	pn_Info.SetFIFO(data, num);								//4:Info
	pn_FCS.SetFIFO_1Byte(CalcFCS());						//5:FCS field
	pn_Tail.SetFIFOFixValue();								//6:Tail Flag
	
	if (retStr != nullptr){
		uint32	len,i;
		uint8	charTemp;
		i = fnOffset;
		len = pn_Tail.GetOffset() + pn_Tail.GetLength();
		while(i < len){
			if (GetDefFifo()->ReadOffsetByIn(&charTemp,1,i++) == 1){
				FIFO8::CharToHex(retStr, charTemp);
				*retStr += ' ';
			}
		}
	}
	_Endl();
	return(num);
}
//------------------------------------------------------------------------------------------//
const CMUX_FRAME &CMUX_FRAME::SetCMuxUIHFrame(STDSTR *retStr,uint8 vPort,const uint8 *data,uint32 num,uint8 crBit,uint32 *sendNum){
	uint32 ret;
	ret = 0;
	while((num > 0) && (GetDefFifo()->Unused() > CMUX_MAX_FRAME_SIZE)){
		ret += AddCMuxUIHFrame(retStr,vPort,data,num,crBit);
		num -= ret;
	}
	if (sendNum != nullptr)
		*sendNum = ret;
	return(*this);
}
//------------------------------------------------------------------------------------------//
const CMUX_FRAME &CMUX_FRAME::SetCMuxUIHFrame(STDSTR *retStr,uint8 vPort,const FIFO8 &fifoIn,uint8 crBit,uint32 *sendNum){
	uint32	slength,num,offset,ret;

	ret = 0;
	num = fifoIn.Used();
	offset = 0;
	slength = fifoIn.CalcOutLength(num, offset);
	
	if (num > 0){
		SetCMuxUIHFrame(retStr,vPort,fifoIn.GetPointer(offset),num,crBit,&ret);
		if (sendNum != nullptr)
			*sendNum = ret;
		if (num > ret)
			return(*this);
	}
	if (slength > 0){
		SetCMuxUIHFrame(retStr,vPort,fifoIn.GetPointer(0),slength,crBit,&ret);
		if (sendNum != nullptr)
			*sendNum = +ret;
	}
	return(*this);
}
//------------------------------------------------------------------------------------------//
const CMUX_FRAME& CMUX_FRAME::SetCMuxCtrlFrame(STDSTR *retStr,uint8 vPort,uint8 ctrlType,uint8 crBit){
	_Start();
	
	pn_Head.SetFIFOFixValue			();									//0:Head Flag
	pn_Address.SetFIFO_1Byte		((vPort << 2) | crBit | CMUX_EA_BIT);//1:Address field
	pn_Control.SetFIFO_1Byte		(ctrlType | CMUX_PF_BIT);			//2:Ctrl field,Poll/Final BIT always 1
	pn_LengthIndicator.SetObyteNum	(1);
	pn_LengthIndicator.SetFIFO_1Byte(CMUX_EA_BIT);						//3:Length indicator
	pn_Info.SetFIFO					();									//4:Info
	pn_FCS.SetFIFO_1Byte			(CalcFCS());						//5:FCS field
	pn_Tail.SetFIFOFixValue			();									//6:Tail Flag
	
	if (retStr != nullptr){
		uint32	len,i;
		uint8	charTemp;
		i = fnOffset;
		len = pn_Tail.GetOffset() + pn_Tail.GetLength();
		while(i < len){
			if (GetDefFifo()->ReadOffsetByIn(&charTemp,1,i++) == 1){
				FIFO8::CharToHex(retStr, charTemp);
				*retStr += ' ';
			}
		}
	}
	_Endl();
	return(*this);
}
//------------------------------------------------------------------------------------------//
const CMUX_FRAME& CMUX_FRAME::SetCMuxCLDFrame(STDSTR *retStr){
	uint8	data[2];
	
	data[0] = CMUX_CMD_MUX_CLD | CMUX_CR_BIT | CMUX_EA_BIT;
	data[1] = CMUX_EA_BIT;
	
	return(SetCMuxUIHFrame(retStr,CMUX_CTRL_DLCI_INDEX,data,2,CMUX_CR_BIT));
}
//------------------------------------------------------------------------------------------//
const CMUX_FRAME& CMUX_FRAME::SetCMuxFCONFrame(STDSTR *retStr,uint8 crBit){
	uint8	data[2];
	
	data[0] = CMUX_CMD_FCON | crBit | CMUX_EA_BIT;
	data[1] = CMUX_EA_BIT;
	
	return(SetCMuxUIHFrame(retStr,CMUX_CTRL_DLCI_INDEX,data,2,CMUX_CR_BIT));
}
//------------------------------------------------------------------------------------------//
const CMUX_FRAME& CMUX_FRAME::SetCMuxFCOFFFrame(STDSTR *retStr,uint8 crBit){
	uint8	data[2];
	
	data[0] = CMUX_CMD_FCOFF | crBit | CMUX_EA_BIT;
	data[1] = CMUX_EA_BIT;
	
	return(SetCMuxUIHFrame(retStr,CMUX_CTRL_DLCI_INDEX,data,2,CMUX_CR_BIT));
}
//------------------------------------------------------------------------------------------//
const CMUX_FRAME& CMUX_FRAME::SetCMuxStdPSCFrame(STDSTR *retStr){
	uint8	data[2];
	
	data[0] = CMUX_CMD_PSC | CMUX_CR_BIT | CMUX_EA_BIT;
	data[1] = CMUX_EA_BIT;
	
	return(SetCMuxUIHFrame(retStr,CMUX_CTRL_DLCI_INDEX,data,2,CMUX_CR_BIT));
}
//------------------------------------------------------------------------------------------//
const CMUX_FRAME& CMUX_FRAME::SetCMuxTPSCxFrame(STDSTR *retStr,uint8 mode){
	uint8	data[3];
	
	data[0] = CMUX_CMD_PSC | CMUX_CR_BIT | CMUX_EA_BIT;
	data[1] = (0x01 << 1) | CMUX_EA_BIT;
	data[2] = mode;
	
	return(SetCMuxUIHFrame(retStr,CMUX_CTRL_DLCI_INDEX,data,3,CMUX_CR_BIT));
}
//------------------------------------------------------------------------------------------//
const CMUX_FRAME& CMUX_FRAME::SetCMuxMSCCmd(STDSTR *retStr,uint8 vPort,uint8 modemStatus,uint8 UINcrbit,uint8 crBit){
	uint8	data[4];
	
	data[0] = CMUX_CMD_MSC | crBit | CMUX_EA_BIT;  		// Cmd type
	data[1] = (0x02 << 1) | CMUX_EA_BIT;				// Cmd Len Ind
	data[2] = (vPort << 2) | 0x02 | CMUX_EA_BIT;   		// valore 1
	data[3] = modemStatus | 0x01;
	
	return(SetCMuxUIHFrame(retStr,CMUX_CTRL_DLCI_INDEX,data,4,UINcrbit));
}
//------------------------------------------------------------------------------------------//
const CMUX_FRAME& CMUX_FRAME::SetCMuxBRKCmd(STDSTR *retStr,uint8 vPort,uint8 modemStatus,uint8 UINcrbit,uint8 crBit){
	uint8	data[5];
	
	data[0] = CMUX_CMD_MSC | crBit | CMUX_EA_BIT;  // Cmd type
	data[1] = (3 << 1) | CMUX_EA_BIT;              // Cmd Len Ind
	data[2] = (vPort << 2) | 0x02 | CMUX_EA_BIT;   // valore 1
	data[3] = modemStatus | 0x01;
	data[4] = CMUX_BREAK_OCTET;
	
	return(SetCMuxUIHFrame(retStr,CMUX_CTRL_DLCI_INDEX,data,5,UINcrbit));
}
//------------------------------------------------------------------------------------------//













//------------------------------------------------------------------------------------------//
void CMUXCOM::Init(void){
	cgPortID = 0;
	ClrSFlag(blV_DSR | blV_CTS | blV_DCD | blV_RING | blV_DTR | blV_RTS | blV_HEX | blV_Escape);
	SetSFlag(blV_Escape);
	cmuxThread.ThreadInit(this, &CMUXCOM::cmuxThreadFun,"cmux");
	modemStatusThread.Disable();
	cgThreadList < cmuxThread;
}
//------------------------------------------------------------------------------------------//
int32 CMUXCOM::DoOpen(const STDSTR &tCDBufName,int32 tCDBufPar,CSType tCSType,int32 blEnEcho){
#ifdef CommonDefH_VC
	return(ACOM::DoOpen(tCDBufName,tCDBufPar,CSType_COM,blEnEcho));
#endif
#ifdef CommonDefH_Unix
	STDSTR		strName;
	strName = CMUX_COM_NAME;
	strName	+= Str_ToString(cgPortID);
	return(ACOM::DoOpen(strName,tCDBufPar,CSType_COMV,blEnEcho));
#endif
}
//------------------------------------------------------------------------------------------//
void CMUXCOM::DoPrintOnOpenSuccess(void){
	PrintNormalMessage("Open CMUX virtual port",Str_ToString(cgPortID),":",GetBufName());
}
//------------------------------------------------------------------------------------------//
void CMUXCOM::DoPrintOnClose(void){
	PrintNormalMessage("Close CMUX virtual port",Str_ToString(cgPortID),":",GetBufName());
}
//------------------------------------------------------------------------------------------//
void CMUXCOM::Update_DSR(int32 bl){
	if (bl > 0){
		SetSFlag(blV_DSR);
	}
	else{
		ClrSFlag(blV_DSR);
	}
}
//------------------------------------------------------------------------------------------//
void CMUXCOM::Update_CTS(int32 bl){
	if (bl > 0){
		SetSFlag(blV_CTS);
	}
	else{
		ClrSFlag(blV_CTS);
	}
}
//------------------------------------------------------------------------------------------//
void CMUXCOM::Update_DCD(int32 bl){
	if (bl > 0){
		SetSFlag(blV_DCD);
	}
	else{
		ClrSFlag(blV_DCD);
	}
}
//------------------------------------------------------------------------------------------//
void CMUXCOM::Update_RING(int32 bl){
	if (bl > 0){
		SetSFlag(blV_RING);
	}
	else{
		ClrSFlag(blV_RING);
	}
}
//------------------------------------------------------------------------------------------//
void CMUXCOM::Update_DTR(int32 bl){
	if (bl > 0){
		SetSFlag(blV_DTR);
	}
	else{
		ClrSFlag(blV_DTR);
	}
}
//------------------------------------------------------------------------------------------//
void CMUXCOM::Update_RTS(int32 bl){
	if (bl > 0){
		SetSFlag(blV_RTS);
	}
	else{
		ClrSFlag(blV_RTS);
	}
}
//------------------------------------------------------------------------------------------//
void CMUXCOM::Update_HEX(int32 bl){
	if (bl > 0){
		SetSFlag(blV_HEX);
	}
	else{
		ClrSFlag(blV_HEX);
	}
}
//------------------------------------------------------------------------------------------//
void CMUXCOM::Update_Escape(int32 bl){
	if (bl > 0){
		SetSFlag(blV_Escape);
	}
	else{
		ClrSFlag(blV_Escape);
	}
}
//------------------------------------------------------------------------------------------//
int32 CMUXCOM::cmuxThreadFun(void *p){
	uint32		byteNum;
	STDSTR		strRecData;
	
	SetGetDataByRead();
	while(cmuxThread.IsTerminated() == 0){
		strRecData = "";
		byteNum = cgRxSBUF.Used();
		if (byteNum == 3){
			cgRxSBUF.cgFIFO.Read(&strRecData,3,0);
			if (strRecData == "+++")
				static_cast<CMUXDriver*>(GetFDB())->Send3P(this);
		}
		while(cgRxSBUF.Used() > 0){
			byteNum = static_cast<CMUXDriver*>(GetFDB())->Send(this,&cgRxSBUF.cgFIFO);
			cgRxSBUF.Out(byteNum);
		}
		SYS_SleepMS(10);
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
const STDSTR &CMUXCOM::DlciStatus(STDSTR *retStr){
	*retStr = " DLCI " + Str_ToString(cgPortID) + " :";
	*retStr += " DTR=";	*retStr += (Check_DTR() == 0)?"L,":"H,";
	*retStr += " RTS=";	*retStr += (Check_DTR() == 0)?"L,":"H,";
	*retStr += " CTS=";	*retStr += (Check_DTR() == 0)?"L,":"H,";
	*retStr += " DSR=";	*retStr += (Check_DTR() == 0)?"L,":"H,";
	*retStr += " RING=";*retStr += (Check_DTR() == 0)?"L,":"H,";
	*retStr += " DCD=";	*retStr += (Check_DTR() == 0)?"L,":"H,";
	*retStr += (Check_HEX() == 0)?" ASCII mode,":" HEX mode,";
	*retStr += (Check_Escape() == 0)?" disable escape":" enable escape";
	return(*retStr);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
void CMUXDriver::Init(uint32 tSize,const DEVICE *dev){
	COMMU_DBUF_THREAD::Init(tSize,dev->GetLogSystem());
	
	cDevice = (DEVICE*)dev;
	cmux_txO.Init(&cgTxSBUF.cgFIFO);
	cmux_txI.Init(&cgTxSBUF.cgFIFO);
	cmux_rx.Init(&cgRxSBUF.cgFIFO);
	SetSFlag(blStartWithAT);
	ClrSFlag(blInitWithThread);
	CreateTrash(this);
}
//------------------------------------------------------------------------------------------//
void CMUXDriver::PrintFrame(const COLSTR &strFrame
							,const COLSTR &colStr1,const COLSTR &colStr2,const COLSTR &colStr3,const COLSTR &colStr4
							,const COLSTR &colStr5,const COLSTR &colStr6,const COLSTR &colStr7,const COLSTR &colStr8){
	if (GetVG3D() != nullptr){
		*GetVG3D() << Start << EnforceDefGroup << NL
		<< COL_DivLineTime
		<< COL_NormalMessage
		<< ColStr(colStr1)
		<< ColStr(colStr2)
		<< ColStr(colStr3)
		<< ColStr(colStr4)
		<< ColStr(colStr5)
		<< ColStr(colStr6)
		<< ColStr(colStr7)
		<< ColStr(colStr8)
		<< ColData(COL_NormalMessage, colStr1.col) << ".\n"
		<< COL_DB_RxText << strFrame
		<< "\n"
		<< Endl;
	}
}
//------------------------------------------------------------------------------------------//
CMUXCOM *CMUXDriver::GetVCOM(int32 dlci){
	CMUXCOM	*tCMUXCOM;
	TREE_LChildRChain_Find(CMUXCOM,this,tCMUXCOM,(operateNode_t->cgPortID == dlci));
	return (tCMUXCOM);
}
//------------------------------------------------------------------------------------------//
CMUXCOM *CMUXDriver::VCOMOpen(int32 dlci,const STDSTR &tCOMName,int32 baudrate){
	CMUXCOM	*tCMUXCOM;
	
	if (dlci > Max_CMUXCOM)
		return(nullptr);
	tCMUXCOM = GetVCOM(dlci);
	if (tCMUXCOM == nullptr){
		tCMUXCOM = static_cast<CMUXCOM*>(GetNewNode(this));
		if (tCMUXCOM != nullptr){
			tCMUXCOM->cgPortID = dlci;
			AddNode(tCMUXCOM);
		}
	}
	if (tCMUXCOM != nullptr){
		if (tCMUXCOM->OpenD(tCOMName,baudrate,CSType_COM,0) == 0)
			tCMUXCOM = nullptr;
	}
	return(tCMUXCOM);
}
//------------------------------------------------------------------------------------------//
void CMUXDriver::VCOMClose(int32 dlci){
	CMUXCOM	*tCMUXCOM;
	if (dlci > Max_CMUXCOM)
		return;
	Spin_InUse_set();
	tCMUXCOM = GetVCOM(dlci);
	
	if (tCMUXCOM != nullptr)
		tCMUXCOM->CloseD();
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
int32 CMUXDriver::OpenDev(const STDSTR &tCDBufName,int32 tCDBufPar,CSType tCSType,int32 blEnEcho){
	#ifdef CommonDefH_Unix
		VCOMOpen(1,"",115200);
		VCOMOpen(2,"",115200);
		VCOMOpen(3,"",115200);
		VCOMOpen(4,"",115200);
	#endif
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 CMUXDriver::DoOpen(const STDSTR &tCDBufName,int32 tCDBufPar,CSType tCSType,int32 blEnEcho){
	ELog(this << "CMUXDriver::DoOpen()");
	if (COMMU_DBUF_FRAME_FW::DoOpen(tCDBufName, tCDBufPar, tCSType, blEnEcho) > 0){
		if (CheckSFlag(blInitWithThread) != 0){
			if (CheckSFlag(blStartWithAT) != 0){
				CMUXStartWithAT();
			}
			else{
				CMUXStart();
			}
			return 1;
		}
		if (CheckSFlag(blStartWithAT) == 0)
			return(CMUXStartCT());
		if (CMUXInitAT() > 0)
			return(CMUXStartCT());
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
int32 CMUXDriver::Open(int32 blInitAT,int32 blInitUseThread){
	int32	ret;
	ret = 0;
	if (InDoing_try() != 0){
		if (cDevice->IsConnected() != 0){
			ClrSFlag(blStartWithAT | blInitWithThread);
			if (blInitAT != 0)
				SetSFlag(blStartWithAT);
			if (blInitUseThread != 0)
				SetSFlag(blInitWithThread);
			ret = OpenD("CMUX",999,CSType_None,0,G_LOCK_OFF);
		}
		InDoing_clr();
	}
	return(ret);
}
//------------------------------------------------------------------------------------------//
void CMUXDriver::CloseDev(void){
	if ((cDevice->IsConnected() != 0) && (IsConnected() != 0))
		CMUXCloseCT();
	commandThread.ThreadStop();
	if (cDevice->ACom() != nullptr)
		cDevice->ACom()->EnableLog();
}
//------------------------------------------------------------------------------------------//
int32 CMUXDriver::TxThreadFun(void *p){
	STDSTR		strData,strFrameHex,strDataT;
	CMUXCOM		*cmuxcom;
	uint32		dlci;
	uint8		ctrlType;
	
	ELog(this << "TxThreadFun()::Running");
	while(txThread.IsTerminated() == 0){
		while(cgTxSBUF.Used() > 0){
			while(cmux_txO.TryGetFrame() > 0){
				strFrameHex = "";
				cmux_txO.ReadAllContentInHEXs(&strFrameHex);
				
				ctrlType = cmux_txO.pn_Control_Type.GetValueCalc();
				dlci = cmux_txO.pn_Address_DLCI.GetValueCalc();
				
				if ((ctrlType == CMUX_CTRL_UIH) && (dlci > 0)){
					cmuxcom = GetVCOM(dlci);
					if (cmuxcom->Check_HEX() == 0){
						cmux_txO.pn_Info.ReadAllContent(&strData);
						if (cmuxcom->Check_Escape() != 0)
							strDataT = "";
							strData = Str_UnTransferChar(&strDataT,strData);
					}
					else{
						cmux_txO.pn_Info.ReadAllContentInHEXs(&strData);
						Str_RTrimSelf(strData);
					}
					PrintFrame(Data(COL_clDCyan,&strData),"CMUXThread::Received:","data from virtual Port",Str_ToString(dlci));
					PrintWithTime(Data(COL_clDCyan,"CMUXThread::Send:"),"CMUX frame:");
					PrintStrNL(Data(COL_clDCyan,&strFrameHex),"\n");
				}
				cDevice->SendHexCommand(strFrameHex);
				cmux_txO.Out();
			};
		}
		SYS_SleepMS(10);
	}
	ELog(this << "TxThreadFun()::Stop");
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 CMUXDriver::RxThreadFun(void *p){
	STDSTR	strFrame;
	SMC_EncryptI(0)
	SMC_EncryptS(0)
	cDevice->RxDataShareTo(&cgRxSBUF);
	SMC_EncryptE(0)
	ELog(this << "RxThreadFun()::Running");
	while(rxThread.IsTerminated() == 0){
		SYS_SleepMS(10);
		while(cgRxSBUF.Used() > 0){
			if (cmux_rx.TryGetFrame() > 0){
				DeliveryRxFrame(&cmux_rx,cmux_rx.ReadAllContentInHEXs(&strFrame));
				cmux_rx.Out();
			}
		}
	}
	cgRxSBUF.RemoveSelf();
	ELog(this << "RxThreadFun()::Stop");
	return 1;
}
//------------------------------------------------------------------------------------------//
void CMUXDriver::DeliveryRxFrame(CMUX_FRAME *tCMUXFrame,const STDSTR &strFrame){
	CMUXCOM		*tCMUXCOM;
	uint8		ctrlType;
	uint32		vPortNumber;
	
	// Check DLCI: Addr field:  |dlci|C/R|E/A|
	vPortNumber = tCMUXFrame->pn_Address_DLCI.GetValueCalc();
	
	if (vPortNumber > Max_CMUXCOM){
		PrintFrame(strFrame,"CMUXThread::DLCI vaule is invaild");
		return;
	}
	
	// Check CTRL: CTRL field:  |X|X|X|X|P/F|X|X|X|
	// Suppose P/F bit in control field always = 0
	ctrlType = tCMUXFrame->pn_Control_Type.GetValueCalc();
	
	switch(ctrlType){
		case CMUX_CTRL_SABM:
			PrintFrame(strFrame,"CMUXThread::Received:","SABM frame,","MS wants connect to me, no implemented Ctrl Type");
			break;
		case CMUX_CTRL_UA:
			PrintFrame(strFrame,"CMUXThread::Received:","UA response frame on"
								  ,(vPortNumber > 0)?"Port":""
								  ,(vPortNumber > 0)?Str_ToString(vPortNumber):""
								  ,(vPortNumber == 0)?"DLCI=0":"");
			break;
		case CMUX_CTRL_DM:
			PrintFrame(strFrame,"CMUXThread::Received:","DM response frame on"
								  ,(vPortNumber > 0)?"Port":""
								  ,(vPortNumber > 0)?Str_ToString(vPortNumber):""
								  ,(vPortNumber == 0)?"DLCI=0":"");
			break;
		case CMUX_CTRL_DISC:
			PrintFrame(strFrame,"CMUXThread::Received:","DISC frame,","MS wants disconnect to me, no implemented Ctrl Type");
			break;
		case CMUX_CTRL_UIH:
			if (vPortNumber == CMUX_CTRL_DLCI_INDEX){
				CMuxCtrlCmdExec(tCMUXFrame,strFrame);
			}
			else{
				PrintFrame(strFrame,"CMUXThread::Received:","UIH frame on","Port",Str_ToString(vPortNumber));
				tCMUXCOM = GetVCOM(vPortNumber);
				if (tCMUXCOM != nullptr)
					tCMUXCOM->Send(tCMUXFrame->pn_Info);
			}
			break;
		case CMUX_CTRL_UI:
			PrintFrame(strFrame,"CMUXThread::Received:","UI frame received,","no implemented Ctrl Type");
			break;
		default:
			PrintFrame(strFrame,"CMUXThread::CTRL Field |X|X|X|X|P/F|X|X|X| is invaild");
			break;
	}
}
//------------------------------------------------------------------------------------------//
void CMUXDriver::CMuxCtrlCmdExec(CMUX_FRAME *tCMUXFrame,const STDSTR &strFrame){
	//There is no specified response to the UIH cpmmand/response
	//MSC Command Application->Module => C/R bit (RED) 1 and C/R bit (BLUE) 1
	//MSC Command Module->Application => C/R bit (RED) 0 and C/R bit (BLUE) 1
	//MSC Response Application->Module => C/R bit (RED) 1 and C/R bit (BLUE) 0
	//MSC Response Module->Application => C/R bit (RED) 0 and C/R bit (BLUE) 0
	
	uint8 	cmdType,frameCRbit,MSCTypeCRbit,charTemp;
	STDSTR	strMSCCommand,strInfo;
	uint32	cMSCDlci,cModemStatus;
	
	strInfo = "";
	tCMUXFrame->pn_Info.ReadAllContent(&strInfo);
	charTemp = strInfo[0];
	cmdType = charTemp & ~(CMUX_CR_BIT | CMUX_EA_BIT);
	switch(cmdType){
		case CMUX_CMD_MSC:{																// Modem Status Command
			MSCTypeCRbit = charTemp & CMUX_CR_BIT;
			cMSCDlci = (strInfo[2] >> 2);												// Get DLCI from frame
			cModemStatus = strInfo[3];
			frameCRbit = tCMUXFrame->pn_Address_CR.GetValueAMask();						//addr & CMUX_CR_BIT;
			if ((frameCRbit == 0) && (MSCTypeCRbit != 0)){								//MSC Command Module->Application => C/R bit (RED) 0 and C/R bit (BLUE) 1
				
				PrintFrame(strFrame,"CMUXThread::Received:","MSC update frame on","Port",Str_ToString(cMSCDlci));
				PrintWithTime(Data(COL_clDCyan,"CMUXThread::"),UpdateModemStatus(&strMSCCommand,cMSCDlci,cModemStatus));
				
				cmux_txI.CreateCMuxMSCCmd(&strMSCCommand,cMSCDlci,cModemStatus,CMUX_CR_BIT,0);//MSC Response Application->Module => C/R bit (RED) 1 and C/R bit (BLUE) 0
				PrintWithTime(Data(COL_clDCyan,"CMUXThread::Send:"),"MSC confirm frame:");
				PrintStrNL(Data(COL_clDCyan,&strMSCCommand),"\n");
			}
			else{
				PrintFrame(strFrame,"CMUXThread::Received:","MSC response on","Port",Str_ToString(cMSCDlci));
			}
			break;
		}
		case CMUX_CMD_PSC:{ // Power Saving Control: Not Impl.
			MSCTypeCRbit = charTemp & CMUX_CR_BIT;
			if (MSCTypeCRbit == 0){
				PrintFrame(strFrame,"CMUXThread::Received:","PSC response frame");
			}
			else{
				PrintFrame(strFrame,"CMUXThread::Received:","PSC frame,","no supported in current version");
			}
			break;
		}
		case CMUX_CMD_TEST:		// Test command
			PrintFrame(strFrame,"CMUXThread::Received:","Test Command,","no supported in current version");
			break;
		case CMUX_NSC_RES:		// Not Supported Command: Not Impl.
			PrintFrame(strFrame,"CMUXThread::Received:","NSC CMD","no supported CMD");
			break;
		case CMUX_CMD_PN:		// Parameter Negotiation: Not Impl.
			PrintFrame(strFrame,"CMUXThread::Received:","Parameter Negotiation CMD,","no supported CMD");
			break;
		case CMUX_CMD_MUX_CLD:	// CMux Close Down: Not Impl.
			PrintFrame(strFrame,"CMUXThread::Received:","CMux Close Down CMD,","no supported CMD");
			break;
		case CMUX_CMD_FCON:		// Flow Control ON: Not Impl.
			PrintFrame(strFrame,"CMUXThread::Received:","Flow Control ON CMD,","no supported CMD");
			break;
		case CMUX_CMD_FCOFF:	// Flow Control OFF: Not Impl.
			PrintFrame(strFrame,"CMUXThread::Received:","Flow Control OFF CMD,","no supported CMD");
			break;
		case CMUX_CMD_RPN:		// Remote Port Negotiation: Not Impl.
			PrintFrame(strFrame,"CMUXThread::Received:","Remote Port Negotiation CMD,","no supported CMD");
			break;
		case CMUX_CMD_RLS:		// Remote Line Status: Not Impl.
			PrintFrame(strFrame,"CMUXThread::Received:","Remote Line Status CMD,","no supported CMD");
			break;
		case CMUX_CMD_SNC:		// Service Negotiation Command: Not Impl.
			PrintFrame(strFrame,"CMUXThread::Received:","Service Negotiation CMD,","no supported CMD");
			break;
		default:
			PrintFrame(strFrame,"CMUXThread::Received:","Other no supported CMD");
	}
}
//------------------------------------------------------------------------------------------//
const STDSTR &CMUXDriver::UpdateModemStatus(STDSTR *strPrintData,uint32 cMSCDlci,uint32 cModemStatus){
	CMUXCOM		*tCMUXCOM;
	
	tCMUXCOM = GetVCOM(cMSCDlci);

	if (cModemStatus & CMUX_MODEMSTATUS_CTS){
		if (tCMUXCOM != nullptr)
			tCMUXCOM->Update_CTS(0);
		*strPrintData = "CTS=0";
	}
	else{
		if (tCMUXCOM != nullptr)
			tCMUXCOM->Update_CTS(1);
		*strPrintData = "CTS=1";
	}
	if (cModemStatus & CMUX_MODEMSTATUS_DSR){
		if (tCMUXCOM != nullptr)
			tCMUXCOM->Update_DSR(0);
		*strPrintData += ",DSR=0";
	}
	else{
		if (tCMUXCOM != nullptr)
			tCMUXCOM->Update_DSR(1);
		*strPrintData += ",DSR=1";
	}
	if (cModemStatus & CMUX_MODEMSTATUS_RING){
		if (tCMUXCOM != nullptr)
			tCMUXCOM->Update_RING(0);
		*strPrintData += ",RING=0";
	}
	else{
		if (tCMUXCOM != nullptr)
			tCMUXCOM->Update_RING(1);
		*strPrintData += ",RING=1";
	}
	if (cModemStatus & CMUX_MODEMSTATUS_DCD){
		if (tCMUXCOM != nullptr)
			tCMUXCOM->Update_DCD(0);
		*strPrintData += ",DCD=0";
	}
	else{
		if (tCMUXCOM != nullptr)
			tCMUXCOM->Update_DCD(1);
		*strPrintData += ",DCD=1";
	}
	
	return(*strPrintData);
}
//------------------------------------------------------------------------------------------//
int32 CMUXDriver::CMUXInitAT(void){
	int32	blOK;
	SBUFFER cBuffer;
	cBuffer.Init(128);
	cBuffer.Clean();
	cDevice->RxDataShareTo(&cBuffer);
	blOK = 0;
	B_ClrFLAG64(cDevice->GetLogSystem()->envcfg, ODEV_FLAG_EnHEXViewMode);
	do{
		if (SendATCMD("","AT"			,"OK",&cBuffer,1000,100) == 0) break;
		if (SendATCMD("","AT"			,"OK",&cBuffer,1000,100) == 0) break;
		if (SendATCMD("","AT"			,"OK",&cBuffer,1000,100) == 0) break;
		if (SendATCMD("","AT+IPR=115200","OK",&cBuffer,1000,100) == 0) break;
		if (SendATCMD("","ATE0V1&K3&D2"	,"OK",&cBuffer,1000,100) == 0) break;
		if (SendATCMD("","AT+CMEE=2"	,"OK",&cBuffer,1000,100) == 0) break;
		if (SendATCMD("","AT+CGMI"		,"OK",&cBuffer,1000,100) == 0) break;
		if (SendATCMD("","AT+CGMM"		,"OK",&cBuffer,1000,100) == 0) break;
		if (SendATCMD("","AT+CGMR"		,"OK",&cBuffer,1000,100) == 0) break;
		SendATCMD("","AT#SELINT=2"		,"OK",&cBuffer,1000,100);
		SendATCMD("","AT#CMUXMODE=1"	,"OK",&cBuffer,1000,100);
		blOK = 1;
	}while(0);
	PrintWithDividingLine("CMUXDriver::Finish the external initialization");
	cBuffer.RemoveSelf();
	return(blOK);
}
//------------------------------------------------------------------------------------------//
int32 CMUXDriver::CMUXStartCT(void){
	int32	blOK;
	STDSTR	strCMD;
	SBUFFER cBuffer;
	cBuffer.Init(128);
	cBuffer.Clean();
	cDevice->RxDataShareTo(&cBuffer);
	cDevice->ACom()->DisableLog();
	blOK = 0;
	B_ClrFLAG64(cDevice->GetLogSystem()->envcfg, ODEV_FLAG_EnHEXViewMode);
	if (SendATCMD("","AT+CMUX=0","OK",&cBuffer,1000,300) > 0){
		B_SetFLAG64(cDevice->GetLogSystem()->envcfg, ODEV_FLAG_EnHEXViewMode);
		do{
			if (SendCMUXCMD("SABM Frame. Enable DLCI=0:\n", cmux_txI.CreateCMuxSABMFrameD0(&strCMD), "!'nullptr", &cBuffer, 2000, 100) == 0) break;
			if (SendCMUXCMD("SABM Frame. Enable virtual port1:\n", cmux_txI.CreateCMuxSABMFrameV1(&strCMD), "!'nullptr", &cBuffer, 2000, 100) == 0) break;
			if (SendCMUXCMD("MSCCmd. Set DTR=0&RTS=0 on DLCI=1:\n", cmux_txI.CreateCmuxDefaultMSV1(&strCMD), "!'nullptr", &cBuffer, 2000, 100) == 0) break;
			if (SendCMUXCMD("SABM Frame. Enable virtual port2:\n", cmux_txI.CreateCMuxSABMFrameV2(&strCMD), "!'nullptr", &cBuffer, 2000, 100) == 0) break;
			if (SendCMUXCMD("MSCCmd. Set DTR=0&RTS=0 on DLCI=2:\n", cmux_txI.CreateCmuxDefaultMSV2(&strCMD), "!'nullptr", &cBuffer, 2000, 100) == 0) break;
			if (SendCMUXCMD("SABM Frame. Enable virtual port3:\n", cmux_txI.CreateCMuxSABMFrameV3(&strCMD), "!'nullptr", &cBuffer, 2000, 100) == 0) break;
			if (SendCMUXCMD("MSCCmd. Set DTR=0&RTS=0 on DLCI=3:\n", cmux_txI.CreateCmuxDefaultMSV3(&strCMD), "!'nullptr", &cBuffer, 2000, 100) == 0) break;
			if (SendCMUXCMD("SABM Frame. Enable virtual port4:\n", cmux_txI.CreateCMuxSABMFrameV4(&strCMD), "!'nullptr", &cBuffer, 2000, 100) == 0) break;
			if (SendCMUXCMD("MSCCmd. Set DTR=0&RTS=0 on DLCI=4:\n", cmux_txI.CreateCmuxDefaultMSV4(&strCMD), "!'nullptr", &cBuffer, 2000, 100) == 0) break;
			blOK = 1;
		} while (0);
	}
	PrintWithDividingLine("CMUXDriver::Finish the CMXU initialization");
	cBuffer.RemoveSelf();
	return(blOK);
}
//------------------------------------------------------------------------------------------//
void CMUXDriver::CMUXCloseCT(void){
	STDSTR	strCMD;
	SBUFFER cBuffer;
	cBuffer.Init(128);
	cBuffer.Clean();
	cDevice->RxDataShareTo(&cBuffer);
	B_SetFLAG64(cDevice->GetLogSystem()->envcfg, ODEV_FLAG_EnHEXViewMode);
	do{
		if (SendCMUXCMD("DISC Frame. Close virtual port4:\n", cmux_txI.CreateCMuxDISCFrameV4(&strCMD), "!'nullptr", &cBuffer, 2000, 100) == 0) break;
		if (SendCMUXCMD("DISC Frame. Close virtual port3:\n", cmux_txI.CreateCMuxDISCFrameV3(&strCMD), "!'nullptr", &cBuffer, 2000, 100) == 0) break;
		if (SendCMUXCMD("DISC Frame. Close virtual port2:\n", cmux_txI.CreateCMuxDISCFrameV2(&strCMD), "!'nullptr", &cBuffer, 2000, 100) == 0) break;
		if (SendCMUXCMD("DISC Frame. Close virtual port1:\n", cmux_txI.CreateCMuxDISCFrameV1(&strCMD), "!'nullptr", &cBuffer, 2000, 100) == 0) break;
		SendCMUXCMD("DISC Frame. Close DLCI=0::\n", cmux_txI.CreateCMuxDISCFrameD0(&strCMD), "!'nullptr", &cBuffer, 2000, 100);
	} while (0);
	B_ClrFLAG64(cDevice->GetLogSystem()->envcfg, ODEV_FLAG_EnHEXViewMode);
	cBuffer.RemoveSelf();
	cDevice->ACom()->EnableLog();
}
//------------------------------------------------------------------------------------------//
void CMUXDriver::CMUXCLDCT(void){
	STDSTR	strCMD;
	SBUFFER cBuffer;
	cBuffer.Init(128);
	cBuffer.Clean();
	cDevice->RxDataShareTo(&cBuffer);
	B_SetFLAG64(cDevice->GetLogSystem()->envcfg, ODEV_FLAG_EnHEXViewMode);
	SendCMUXCMD("CLD frame. Exit CMUX mode:\n",cmux_txI.CreateCMuxCLDFrame(&strCMD),"!'nullptr",&cBuffer,2000,100);
	B_ClrFLAG64(cDevice->GetLogSystem()->envcfg, ODEV_FLAG_EnHEXViewMode);
	cBuffer.RemoveSelf();
	cDevice->ACom()->EnableLog();
}
//------------------------------------------------------------------------------------------//
void CMUXDriver::CMUXStdPSCCT(uint8 cmode){
	STDSTR	strCMD;
	SBUFFER cBuffer;
	cBuffer.Init(128);
	cBuffer.Clean();
	cDevice->RxDataShareTo(&cBuffer);
	B_SetFLAG64(cDevice->GetLogSystem()->envcfg, ODEV_FLAG_EnHEXViewMode);
	if (cmode == 0xff){
		SendCMUXCMD("standard PCS frame:\n",cmux_txI.CreateCMuxStdPSCFrame(&strCMD),"!'nullptr",&cBuffer,2000,100);
	}
	else{
		SendCMUXCMD("Telit PCS frame, mode " + Str_ToString(cmode) + ":\n",cmux_txI.CreateCMuxTPSCxFrame(&strCMD,cmode),"!'nullptr",&cBuffer,2000,100);
	}
	cBuffer.RemoveSelf();
}
//------------------------------------------------------------------------------------------//
void CMUXDriver::CMUXFCCCT(int32 blIsOn){
	STDSTR	strCMD;
	SBUFFER cBuffer;
	cBuffer.Init(128);
	cBuffer.Clean();
	cDevice->RxDataShareTo(&cBuffer);
	B_SetFLAG64(cDevice->GetLogSystem()->envcfg, ODEV_FLAG_EnHEXViewMode);
	if (blIsOn){
		SendCMUXCMD("FCon frame:\n",cmux_txI.CreateCMuxFCONFrame(&strCMD,CMUX_CR_BIT),"!'nullptr",&cBuffer,2000,100);
	}
	else{
		SendCMUXCMD("FCoff frame:\n",cmux_txI.CreateCMuxFCOFFFrame(&strCMD,CMUX_CR_BIT),"!'nullptr",&cBuffer,2000,100);
	}
	cBuffer.RemoveSelf();
}
//------------------------------------------------------------------------------------------//
void CMUXDriver::CMUXMSCCT(int32 vPortNumber,int32 blDTR,int32 blRTS){
	CMUXCOM	*tCMUXCOM;
	STDSTR	strCMD,strTitle;
	uint8	modemStatus;
	SBUFFER cBuffer;
	cBuffer.Init(128);
	cBuffer.Clean();
	cDevice->RxDataShareTo(&cBuffer);
	B_SetFLAG64(cDevice->GetLogSystem()->envcfg, ODEV_FLAG_EnHEXViewMode);
	modemStatus = 0;
	
	tCMUXCOM = GetVCOM(vPortNumber);
	strTitle = "MSC CMD, Set DTR=";
	if (blDTR == 0){
		modemStatus |= CMUX_MODEMSTATUS_DTR;
		strTitle += '0';
		if (tCMUXCOM != nullptr)
			tCMUXCOM->Update_DTR(0);
	}
	else{
		strTitle += '1';
		if (tCMUXCOM != nullptr)
			tCMUXCOM->Update_DTR(1);
	}
	strTitle += " & RTS=";
	if (blRTS == 0)	{
		modemStatus |= CMUX_MODEMSTATUS_RTS;
		strTitle += '0';
		if (tCMUXCOM != nullptr)
			tCMUXCOM->Update_RTS(0);
	}
	else{
		strTitle += '1';
		if (tCMUXCOM != nullptr)
			tCMUXCOM->Update_RTS(1);
	}
	strTitle += " on DLCI=";
	strTitle += Str_ToString(vPortNumber);
	strTitle += "::\n";
	
	SendCMUXCMD(strTitle,cmux_txI.CreateCMuxMSCCmd(&strCMD,vPortNumber,modemStatus,CMUX_CR_BIT,CMUX_CR_BIT),"!'nullptr",&cBuffer,2000,300);
	cBuffer.RemoveSelf();
}
//------------------------------------------------------------------------------------------//
int32 CMUXDriver::CmuxStartWithATThreadFun(void *p){
	if ((CMUXInitAT() != 0) && (CMUXStartCT() != 0))
		return 1;
	SelfClose();
	return 1;
};
//------------------------------------------------------------------------------------------//
void CMUXDriver::CMUXStartWithAT(void){
	commandThread.Spin_InUse_set();
	commandThread.ThreadStop();
	commandThread.ThreadInit(this,&CMUXDriver::CmuxStartWithATThreadFun,"CMUXStartWithAT");
	commandThread.ThreadRun();
	commandThread.Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void CMUXDriver::CMUXStart(void){
	commandThread.Spin_InUse_set();
	commandThread.ThreadStop();
	commandThread.ThreadInit(this,&CMUXDriver::CmuxStartThreadFun,"CMUXStart");
	commandThread.ThreadRun();
	commandThread.Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void CMUXDriver::CMUXStop(void){
	commandThread.Spin_InUse_set();
	commandThread.ThreadStop();
	commandThread.ThreadInit(this,&CMUXDriver::CmuxStopThreadFun,"CMUXStop");
	commandThread.ThreadRun();
	commandThread.Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
int32 CMUXDriver::SendATCMD(const STDSTR strTitle,const STDSTR &strCMD,const STDSTR &strCheck,SBUFFER *cSBUF,uint32 waitMS,uint32 delyMS){
	cDevice->PrintSendCommand("CMUXDriver::Send: ",strTitle,strCMD);
	return(cDevice->SendCommand(strCMD,CMD_R,G_ESCAPE_ON,strCheck,cSBUF,waitMS,delyMS));
}
//------------------------------------------------------------------------------------------//
int32 CMUXDriver::SendCMUXCMD(const STDSTR strTitle,const STDSTR &strCMD,const STDSTR &strCheck,SBUFFER *cSBUF,uint32 waitMS,uint32 delyMS){
	cDevice->PrintSendCommand("CMUXDriver::Send:",strTitle,strCMD);
	cSBUF->Clean();
	if (cSBUF == nullptr){
		SYS_SleepMS(delyMS);
		return 1;
	}
#ifdef SBIC_ExpressionH
	if (cgSBIC_Exp.CheckResult(cSBUF,strCheck,waitMS) > 0){
		SYS_SleepMS(delyMS);
		return 1;
	}
#else
	return 1;
#endif
	return 0;
}
//------------------------------------------------------------------------------------------//
uint32 CMUXDriver::Send(CMUXCOM *ccom,FIFO8 *fifoIn){
	uint32	num;
	num = 0;
	cmux_txI.Spin_InUse_set();
	cmux_txI.SetCMuxUIHFrame(nullptr,ccom->cgPortID,*fifoIn,CMUX_CR_BIT,&num);
	cmux_txI.Spin_InUse_clr();
	return(num);
}
//------------------------------------------------------------------------------------------//
void CMUXDriver::Send3P(CMUXCOM *ccom){
	STDSTR	strFrameHex;
	uint8	modemStatus = 0;

	PrintWithDividingLine("CMUXDriver::Received:","+++ from virtual port",Str_ToString(ccom->cgPortID),"switch to DTR control");
	
	if (ccom->Check_RTS()== 0)
		modemStatus |= CMUX_MODEMSTATUS_RTS;

	cmux_txI.CreateCMuxMSCCmd(&strFrameHex,ccom->cgPortID,modemStatus,CMUX_CR_BIT,CMUX_CR_BIT);
	PrintWithTime(Data(COL_clDCyan,"CMUXDriver::Send:"),"MSC confirm frame:");
	PrintStrNL(Data(COL_clDCyan,&strFrameHex));
	
	SYS_SleepMS(200);
	
	modemStatus |= CMUX_MODEMSTATUS_DTR;

	cmux_txI.CreateCMuxMSCCmd(&strFrameHex,ccom->cgPortID,modemStatus,CMUX_CR_BIT,CMUX_CR_BIT);
	PrintWithTime(Data(COL_clDCyan,"CMUXDriver::Send:"),"MSC confirm frame:");
	PrintStrNL(Data(COL_clDCyan,&strFrameHex));
	SYS_SleepMS(200);
}
//------------------------------------------------------------------------------------------//
#endif

