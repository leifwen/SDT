/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: Commu_DBuf.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2012.02.09
 * @2013.6.30 :	change to Liunx style
 add init method
 use int32 to replace bool
 use int32 to replace uint32
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "Commu_DBuf.h"
#include "Comm_Convert.h"
#include "ODEV_Include.h"
#include "SMC.h"
//------------------------------------------------------------------------------------------//
class COMMU_DBUF_ODEV : public ODEV_NODE{
	public:
		enum{RFLAG_C = 0, RFLAG_S = ODEV_NODE::RFLAG_S + ODEV_NODE::RFLAG_C};
	public:
				 COMMU_DBUF_ODEV(void) : ODEV_NODE(){cgDBuffer = nullptr;};
				 COMMU_DBUF_ODEV(const COMMU_DBUF_B *tODEV,COLType tCOLType = COLType_TXT) : ODEV_NODE(){Init(tODEV,tCOLType);};
		virtual ~COMMU_DBUF_ODEV(void){;};
	public:
		virtual int32	Print		(G_LOCK_VAILD blLock = G_LOCK_ON){
			std::string		strT;
			if (cgDBuffer != nullptr)
				cgDBuffer->WriteInASCII(ReadStr(&strT,blLock),G_ESCAPE_OFF);
			return 1;
		};
		virtual void	UnInit		(G_LOCK_VAILD blLock = G_LOCK_ON){Spin_InUse_set(blLock);cgDBuffer = nullptr;Spin_InUse_clr(blLock);};
	private:
		COMMU_DBUF_B	*cgDBuffer;
	public:
		void			Init(const COMMU_DBUF_B *tODEV,COLType tCOLType = COLType_TXT){
			Spin_InUse_set();
			ChageType(tCOLType,G_LOCK_OFF);
			cgDBuffer = (COMMU_DBUF_B*)tODEV;
			Spin_InUse_clr();
		};
};
//------------------------------------------------------------------------------------------//

//------------------------------------------------------------------------------------------//
void COMMU_DBUF_B::Init(void){
	cgCSType = CSType_None;
	cgCDBufName = "";
	cgCDBufPar = 0;
	cgFNode_TX = nullptr;
	
	rxThread.ThreadInit(this, &COMMU_DBUF_B::RxThreadFun);
	txThread.ThreadInit(this, &COMMU_DBUF_B::TxThreadFun);
	
	cgThreadList.AddNode(&rxThread);
	cgThreadList.AddNode(&txThread);
	
	rxFwSBufList = &cgInternalFwSBufList;
	cgSelfODdev = nullptr;
}
//------------------------------------------------------------------------------------------//
void COMMU_DBUF_B::UseSelfFwSBL(void){
	Spin_InUse_set();
	rxFwSBufList = &cgInternalFwSBufList;
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void COMMU_DBUF_B::UseExternalFwSBL(const PUB_SBUF_LIST *tRxFwSBufList){
	Spin_InUse_set();
	rxFwSBufList = (PUB_SBUF_LIST*)tRxFwSBufList;
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
int32 COMMU_DBUF_B::OpenD(const std::string &tCDBufName,int32 tCDBufPar,CSType tCSType,int32 blEnEcho){
	int32 ret;
	ret = 0;
	if (InDoing_try() > 0){
		if (IsConnected() > 0)
			Close_Do();
		Clean();
		ret = OpenDev(tCDBufName,tCDBufPar,tCSType);
		if (ret > 0){
			Run(tCDBufName,tCDBufPar,tCSType,blEnEcho);
			ret = DoAfterOpen();
			if (ret == 0)
				Close_Do();
		}
		InDoing_clr();
	}
	return(ret);
}
//------------------------------------------------------------------------------------------//
void COMMU_DBUF_B::Run(const std::string &tCDBufName,int32 tCDBufPar,CSType tCSType,int32 blEnEcho){
	DisableEcho();
	if (blEnEcho != 0)
		EnableEcho();
	cgCDBufName = tCDBufName;
	cgCDBufPar = tCDBufPar;
	cgCSType = tCSType;
	ClrblSDC();
	ClrblAClose();
	Clean();
	
	ThreadsStart();
	SetblConnected();
	Enable();
}
//------------------------------------------------------------------------------------------//
int32 COMMU_DBUF_B::DoAfterOpen(void){
	return 1;
};
//------------------------------------------------------------------------------------------//
void COMMU_DBUF_B::ThreadsStart(void){
	SMC_EncryptI(0)
	SMC_EncryptS(0)
	cgThreadList.LThreadRun();
	SMC_EncryptE(0)
}
//------------------------------------------------------------------------------------------//
int32 COMMU_DBUF_B::CloseD(int32 blClrBufName){
	int32 ret;
	InDoing_set();
	ret = Close_Do(blClrBufName);
	Disable();
	InDoing_clr();
	return(ret);
}
//------------------------------------------------------------------------------------------//
int32 COMMU_DBUF_B::Close_Do(int32 blClrBufName){
	OnCloseDev();
	CloseDev();
	cgThreadList.LThreadStop();
	if (blClrBufName != 0){
		cgCDBufName = "";
		cgCDBufPar = 0;
	}
	ClrblSDC();
	return 1;
}
//------------------------------------------------------------------------------------------//
void COMMU_DBUF_B::OnCloseDev(void){
	PushSend();
	SetblPushSend();
};
//------------------------------------------------------------------------------------------//
void COMMU_DBUF_B::CloseDev(void){
	SetblAClose();
	ClrblConnected();
};
//------------------------------------------------------------------------------------------//
void COMMU_DBUF_B::DoAfterReadFromDevice(const uint8 *databuf,int32 num){
	if (CheckEnableEcho() != 0)
		cgFwBytes += cgTxBuffer.Put(databuf,num);//Echo
	Spin_InUse_set();
	if (GetcgCoupleNode(this) != nullptr)
		ForwardToCouple(databuf,num);
	if (rxFwSBufList != nullptr)
		rxFwSBufList->LPut(databuf,num);//used for internal AP
	Spin_InUse_clr();
	if (CheckblcgRxBufferUsed() > 0)
		cgRxBuffer.Put(databuf,num);
}
//------------------------------------------------------------------------------------------//
void COMMU_DBUF_B::ForwardToCouple(const uint8 *databuf,int32 num){
	if (num > 0)
		((COMMU_DBUF_B*)GetcgCoupleNode(this))->Write(databuf,num);
}
//------------------------------------------------------------------------------------------//
int32 COMMU_DBUF_B::RxThreadFun(void){
	uint8	buffer[PACKAGE_MAX_SIZE];
	uint32	retNum;
	int32	errCode;
	
	if (GetCSType() == CSType_UDPS)
		return 1;
	while(rxThread.IsTerminated() == 0){
		if ((IsConnected() > 0) && (CheckblHold() == 0)){
			errCode = ReadFromDevice(&retNum,buffer,PACKAGE_MAX_SIZE);
			if (errCode == -1)
				break;
			if ((errCode > 0) && (retNum > 0)){
				cgRxBytes += retNum;
				DoAfterReadFromDevice(buffer,retNum);
				continue;
			}
		}
		SYS_SleepMS(2);
	}
	return 1;
}
//------------------------------------------------------------------------------------------//
void COMMU_DBUF_B::PushSend(void){
	SYS_TIME_S		Timedly;
	SYS_Delay_SetTS(&Timedly, 100);
	SetblPushSend();
	while((CheckblPushSend() > 0) && (SYS_Delay_CheckTS(&Timedly) == 0))
		SYS_DelayMS(2);
};
//------------------------------------------------------------------------------------------//
int32 COMMU_DBUF_B::TxThreadFun(void){
	int32	errCode;
	uint32	length,offset,retNum;
	std::string	strBuf;
	
	while(txThread.IsTerminated() == 0){
		if (((IsConnected() > 0) && (CheckblHold() == 0) && (cgTxBuffer.Used() > 0))
			|| (CheckblPushSend() > 0)){
			if (CheckblPushSend() > 0)
				ClrblPushSend();
			if (cgFNode_TX != nullptr){//only in UDP mode need usd this
				cgFNode_TX->Spin_InUse_set();
				cgTxBuffer.Spin_InUse_set();
				if (cgFNode_TX->AnalysisFrame(cgTxBuffer.cgBufFIFO) > 0){
					cgFNode_TX->ReadAllContent(&strBuf);
					cgTxBuffer.Out(cgFNode_TX->GetOffset() + cgFNode_TX->GetLength(),G_LOCK_OFF);
				}
				cgTxBuffer.Spin_InUse_clr();
				cgFNode_TX->Spin_InUse_clr();
				errCode = SendToDevice(&retNum,(uint8*)strBuf.c_str(),(uint32)strBuf.length());
				if ((errCode > 0) && (retNum > 0))
					cgTxBytes += retNum;
			}
			else{
				cgTxBuffer.Spin_InUse_set();
				do{
					offset = 0;
					cgTxBuffer.cgBufFIFO.CalcOutCapacity(length,offset);
					errCode = SendToDevice(&retNum,cgTxBuffer.cgBufFIFO.GetOutPointer(offset),length);
					cgTxBuffer.Out(retNum,G_LOCK_OFF);
					if (errCode == -1)
						break;
					if ((errCode > 0) && (retNum > 0))
						cgTxBytes += retNum;
				}while((length > 0) && (CheckblAClose() == 0));
				cgTxBuffer.Spin_InUse_clr();
				if (errCode == -1)
					break;
			}
		}
		SYS_SleepMS(2);
	}
	return 1;
}
//------------------------------------------------------------------------------------------//
ODEV_NODE *COMMU_DBUF_B::CreateSelfODev(COLSTRING::COLType tCOLType){
	Spin_InUse_set();
	if (cgSelfODdev == nullptr)
		cgSelfODdev = new COMMU_DBUF_ODEV(this,tCOLType);
	Spin_InUse_clr();
	return(cgSelfODdev);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//



















//------------------------------------------------------------------------------------------//
void COMMU_DBUF::Init(const ODEV_LIST *tODEV_LIST){
	exThread.ThreadInit(this, &COMMU_DBUF::ExThreadFun);
	ex2Thread.ThreadInit(this, &COMMU_DBUF::Ex2ThreadFun);
	cgThreadList.AddNode(&exThread);
	cgThreadList.AddNode(&ex2Thread);
	
	SetODEV_LIST(tODEV_LIST);
	
	cgODevPool = nullptr;
}
//------------------------------------------------------------------------------------------//
void COMMU_DBUF::SetODEV_LIST(const ODEV_LIST *tODEV_LIST){
	Spin_InUse_set();
	cgODevList = (ODEV_LIST*)tODEV_LIST;
	cgOutput = nullptr;
	if (cgODevList != nullptr)
		cgOutput = cgODevList->cgOutput;
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void COMMU_DBUF::PrintOpenSuccessReport(const std::string &strData){
	std::string strPrintData;
	Spin_InUse_set();
	if (cgOutput != nullptr){
		strPrintData = SYS_MakeTimeNow();
		strPrintData += " Successfully connect to ";
		if (strData.length() > 0){
			strPrintData += strData;
			strPrintData += ' ';
		}
		strPrintData += GetBufName();
		strPrintData += ":";
		strPrintData += Str_IntToString(GetBufPar());
		strPrintData += ".\r\n";
		cgOutput->Spin_InUse_set();
		cgOutput->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
		cgOutput->WriteStrN(strPrintData,RICH_CF_clPurple,COLSTRING::COL_EP_YES);
		cgOutput->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
		cgOutput->Spin_InUse_clr();
	}
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void COMMU_DBUF::PrintOpenFailReport(const std::string &strDevName){
	Spin_InUse_set();
	if (cgOutput != nullptr){
		cgOutput->Spin_InUse_set();
		cgOutput->WriteDividingLine(RICH_CF_clBlack,COLSTRING::COL_EP_YES);
		cgOutput->WriteStrN(SYS_MakeTimeNow(),RICH_CF_clRed,COLSTRING::COL_EP_YES);
		cgOutput->WriteStr(" Failed connect to ",RICH_CF_clRed,COLSTRING::COL_EP_YES);
		cgOutput->WriteStr(strDevName,RICH_CF_clBlue,COLSTRING::COL_EP_YES);
		cgOutput->WriteStr(".\r\n",RICH_CF_clRed,COLSTRING::COL_EP_YES);
		cgOutput->WriteDividingLine(RICH_CF_clBlack,COLSTRING::COL_EP_YES);
		cgOutput->Spin_InUse_clr();
	}
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void COMMU_DBUF::PrintUserDisconnectReport(const std::string &strDevName){
	std::string		strPrintData;
	Spin_InUse_set();
	if (cgOutput != nullptr){
		strPrintData = SYS_MakeTimeNow();
		strPrintData += strDevName;
		strPrintData += ("Received : " + Str_UInt64ToString(RxBytes()) + " bytes\r\n");
		strPrintData += ("Echo     : " + Str_UInt64ToString(FwBytes()) + " bytes\r\n");
		strPrintData += ("Sent     : " + Str_UInt64ToString(TxBytes()) + " bytes\r\n");
		cgOutput->Spin_InUse_set();
		cgOutput->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
		cgOutput->WriteStrN(strPrintData,RICH_CF_clPurple,COLSTRING::COL_EP_YES);
		cgOutput->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
		cgOutput->Spin_InUse_clr();
	}
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void COMMU_DBUF::PrintRecData(ODEV_LIST_POOL *output, const std::string &strData, uint32 byteNum, int32 blEnRecMsg){
	std::string	strName,strSelfName;
	
	if (output == nullptr)
		return;
	
	strSelfName = GetBufName() + ":" + Str_IntToString(GetBufPar());
	
	if (GetcgCoupleNode(this) != nullptr)
		strName = ((COMMU_DBUF*)GetcgCoupleNode(this))->GetBufName() + ":" + Str_IntToString(((COMMU_DBUF*)GetcgCoupleNode(this))->GetBufPar());
	if (CheckEnableEcho() != 0){
		output->WriteDividingLine(RICH_CF_clMaroon);
		output->WriteEchoTitle(strSelfName, byteNum, FwBytes(),RICH_CF_clMaroon);
		output->WriteStr(strData,RICH_CF_clPurple);
		if (GetcgCNType(this) == CN_1){
			output->WriteDividingLine(RICH_CF_clMaroon);
			output->WriteTxTitle(strSelfName, strName, byteNum, RxBytes(), RICH_CF_clRed);
			output->WriteStr(strData,RICH_CF_clBlue);
		}
	}
	else if (GetcgCNType(this) == CN_None){
		if (blEnRecMsg == 1){
			output->WriteDividingLine(RICH_CF_clMaroon);
			output->WriteRxTitle(byteNum, RxBytes(), RICH_CF_clRed);
		}
		if (blEnRecMsg == 2){
			output->WriteDividingLine(RICH_CF_clMaroon);
			output->WriteRxTitle(strSelfName, "", byteNum, RxBytes(), RICH_CF_clRed);
		}
		output->WriteStr(strData,RICH_CF_clBlue);
	}
	else if (GetcgCNType(this) == CN_1){
		output->WriteDividingLine(RICH_CF_clMaroon);
		output->WriteTxTitle(strSelfName, strName, byteNum, RxBytes(), RICH_CF_clRed);
		output->WriteStr(strData,RICH_CF_clBlue);
	}
	else if (GetcgCNType(this) == CN_0){
		output->WriteDividingLine(RICH_CF_clMaroon);
		output->WriteRxTitle(strName, strSelfName, byteNum, RxBytes(), RICH_CF_clMaroon);
		output->WriteStr(strData,RICH_CF_clBlack);
	}
}
//------------------------------------------------------------------------------------------//
void COMMU_DBUF::PrintRecData_lock(ODEV_LIST_POOL *output, const std::string &strData, uint32 byteNum, int32 blEnRecMsg){
	std::string	strName,strSelfName;
	
	if (output == nullptr)
		return;
	output->Spin_InUse_set();
	PrintRecData(output,strData,byteNum,blEnRecMsg);
	output->Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
int32 COMMU_DBUF::Ex2ThreadFun(void){
	int32		byteNum;
	uint64		oDevFlagU64;
	std::string	strPrintdata;
	
	strPrintdata = "";
	SetblcgRxBufferUsed();
	while(ex2Thread.IsTerminated() == 0){
		oDevFlagU64 = 0;
		if (cgRxBuffer.Used() > 0){
			Spin_InUse_set();
			strPrintdata = "";
			if (cgODevList != nullptr)
				oDevFlagU64 = cgODevList->cgODevFlagU64;
			
			if (B_ChkFLAG64(oDevFlagU64,ODEV_FLAG_EnHEXViewMode) != 0){
				byteNum = cgRxBuffer.GetInHEX(&strPrintdata,G_SPACE_ON);
				if (B_ChkFLAG64(oDevFlagU64,ODEV_FLAG_EnOSPMsgLine) != 0)
					strPrintdata = Str_Replace(strPrintdata," B0 B3 "," B0 B3\r\n");
			}
			else{
				byteNum = cgRxBuffer.GetInASCII(&strPrintdata, (B_ChkFLAG64(oDevFlagU64, ODEV_FLAG_EnEscape) != 0) ? G_ESCAPE_ON : G_ESCAPE_OFF);
			}
			
			if (cgOutput != nullptr)
				PrintRecData_lock(cgOutput,strPrintdata,byteNum,B_ChkFLAG64(oDevFlagU64,ODEV_FLAG_EnRecMsg));
			if (cgODevPool != nullptr)
				PrintRecData_lock(cgODevPool,strPrintdata,byteNum,B_ChkFLAG64(oDevFlagU64,ODEV_FLAG_EnRecMsg));
			if ((GetcgCoupleNode(this) != nullptr) && (((COMMU_DBUF*)GetcgCoupleNode(this))->cgODevPool != nullptr))
				PrintRecData_lock(((COMMU_DBUF*)GetcgCoupleNode(this))->cgODevPool,strPrintdata,byteNum,B_ChkFLAG64(oDevFlagU64,ODEV_FLAG_EnRecMsg));
			Spin_InUse_clr();
		}
		SYS_SleepMS(10);
	}
	return 1;
}
//------------------------------------------------------------------------------------------//
ODEV_NODE_SDOUT *COMMU_DBUF::CreateODevSDOUT(const void *tRichEdit,const void *tCFrm){
	ODEV_NODE_SDOUT *ret;
	ret = nullptr;
	Spin_InUse_set();
	if (CreateOutputList() != nullptr){
		cgODevPool->CreateODevSDOUT(tRichEdit, tCFrm);
		ret = cgODevPool->cODevSDOUT;
	}
	Spin_InUse_clr();
	return(ret);
}
//------------------------------------------------------------------------------------------//
ODEV_NODE_FILE *COMMU_DBUF::CreateODevFile(const std::string &tfileName){
	ODEV_NODE_FILE *ret;
	ret = nullptr;
	Spin_InUse_set();
	if (CreateOutputList() != nullptr){
		cgODevPool->CreateODevFile(tfileName);
		ret = cgODevPool->cODevFileTXT;
	}
	Spin_InUse_clr();
	return(ret);
}
//------------------------------------------------------------------------------------------//
ODEV_LIST_POOL *COMMU_DBUF::CreateOutputList(void){
	if (cgODevPool == nullptr){
		cgODevPool = new ODEV_LIST_POOL;
		AddNode(cgODevPool);
	}
	return(cgODevPool);
}
//------------------------------------------------------------------------------------------//
void COMMU_DBUF::UpdataRecordUI(void){
	Spin_InUse_set();
	if (cgODevPool != nullptr)
		cgODevPool->Print();
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_VC
//------------------------------------------------------------------------------------------//
void COMMU_DBUF::UpdataRecordUISDOUT(void){
	Spin_InUse_set();
	if (cgODevPool != nullptr)
		cgODevPool->PrintSDOUT();
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------/
#endif
//------------------------------------------------------------------------------------------//






//------------------------------------------------------------------------------------------//
COMMU_DBUF *CDBUF_POOL::Find(const std::string &tDBufName,int32 tDBufPar){
	COMMU_DBUF *retCOMMU_DBUF;
	retCOMMU_DBUF = nullptr;
	RTREE_LChildRChain_Find(COMMU_DBUF,this,retCOMMU_DBUF,((operateNode_t->GetBufName() == tDBufName) && (operateNode_t->GetBufPar() == tDBufPar)));
	return(retCOMMU_DBUF);
}
//------------------------------------------------------------------------------------------//
COMMU_DBUF *CDBUF_POOL::SetSelect(const std::string &tDBufName,int32 tDBufPar){
	COMMU_DBUF *retCOMMU_DBUF;
	
	retCOMMU_DBUF = Find(tDBufName,tDBufPar);
	if (retCOMMU_DBUF != nullptr){
		LClrblSelected();
		retCOMMU_DBUF->SetblSelected();
	}
	return(retCOMMU_DBUF);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//

