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
#include "SMC.h"
//------------------------------------------------------------------------------------------//
#define LOGPRINT_ENABLE
#define LOGPRINT_ENABLE2
#include "Comm_Log.h"
#ifdef COMMU_DBUFH
//------------------------------------------------------------------------------------------//
void COMMU_DBUF_FRAME::Init(uint32 tSize){
	DBUFFER::Init(tSize);
	selfCloseThread.ThreadInit(this, &COMMU_DBUF_FRAME::SelfCloseThreadFun,"selfClose");
	cgCSType = CSType_None;
	cgDevName = "";
	cgCDBufName = "";
	cgCDBufPar = 0;
}
//------------------------------------------------------------------------------------------//
int32 COMMU_DBUF_FRAME::OpenD(const STDSTR &tCDBufName,int32 tCDBufPar,CSType tCSType,int32 blEnEcho,G_LOCK blLock){
	int32 ret;
	ELog(this << "COMMU_DBUF_FRAME::OpenD()");
	ret = 0;
	if (InDoing_try(blLock) != 0){
		if ((IsConnected() != 0) || (ChkblOpened() != 0))
			DoClose();
		ret = DoOpen(tCDBufName,tCDBufPar,tCSType,blEnEcho);
		if (ret > 0){
			SetSFlag(cdf_blOpened);
		}
		else{
			SetSFlag(cdf_blCloseByUser);
			DoClose();
		}
		InDoing_clr(blLock);
	}
	return(ret > 0 ? 1 : 0);
}
//------------------------------------------------------------------------------------------//
int32 COMMU_DBUF_FRAME::DoOpen(const STDSTR &tCDBufName,int32 tCDBufPar,CSType tCSType,int32 blEnEcho){
	int32 ret;
	ELog(this << "COMMU_DBUF_FRAME::DoOpen()");
	Clean();
	ret = OpenDev(tCDBufName,tCDBufPar,tCSType,blEnEcho);
	
	cgDevName	= tCDBufName + ":" + Str_ToString(tCDBufPar);
	cgCDBufName = tCDBufName;
	cgCDBufPar	= tCDBufPar;
	cgCSType	= tCSType;
	
	if (ret > 0){
		DisableEcho();
		if (blEnEcho != 0)
			EnableEcho();
		ClrSFlag(cdf_blCloseDueToOS | cdf_blCloseByUser | cdf_blPushSend);
		SetSFlag(cdf_blConnected);
		Enable();
		ret = ThreadsStart();
	}
	return(ret);
}
//------------------------------------------------------------------------------------------//
void COMMU_DBUF_FRAME::CloseD(void){
	ELog(this << "COMMU_DBUF_FRAME::CloseD()");
	InDoing_set();
	SetSFlag(cdf_blCloseByUser);
	DoClose();
	ClrSFlag(cdf_blOpened | cdf_blCloseDueToOS);
	Disable();
	InDoing_clr();
	ELog(this << "COMMU_DBUF_FRAME::Finished close()");
}
//------------------------------------------------------------------------------------------//
void COMMU_DBUF_FRAME::DoClose(void){
	ELog(this << "COMMU_DBUF_FRAME::DoClose()");
	SetSFlag(cdf_blPushSend);
	CloseDev();
	ClrSFlag(cdf_blConnected);
	ELog(this << "COMMU_DBUF_FRAME::ThreadsStop()");
	ThreadsStop();
	ELog(this << "COMMU_DBUF_FRAME::ChildCloseAll()");
	ChildCloseAll();
}
//------------------------------------------------------------------------------------------//
void COMMU_DBUF_FRAME::ChildCloseAll(void){
	TREE_LChildRChain_Traversal_LINE(COMMU_DBUF_FRAME, this, operateNode_t->CloseD(););
	ChildClrSel(GetSelDB());
	CleanChild(this);
	SetblUpdate();
};
//------------------------------------------------------------------------------------------//
void COMMU_DBUF_FRAME::ChildClose(COMMU_DBUF_FRAME *commu){
	if (commu != nullptr){
		ELog(this << "ChildClose(): " << commu);
		commu->CloseD();
		ChildClrSel(commu);
		MoveToTrash(commu);
		SetblUpdate();
	}
};
//------------------------------------------------------------------------------------------//
int32 COMMU_DBUF_FRAME::SelfCloseThreadFun(void *p){
	ELog(this << "SelfCloseThreadFun()::Running");
	if (GetFDB() != nullptr){
		static_cast<COMMU_DBUF_FRAME*>(GetFDB())->ChildClose(this);
	}
	else{
		CloseD();
	}
	ELog(this << "SelfCloseThreadFun()::Stop");
	return 1;
};
//------------------------------------------------------------------------------------------//
void COMMU_DBUF_FRAME::SelfClose(void){
	if (ChkblCloseDueToOS() == 0){
		selfCloseThread.ThreadStop();
		if (ChkblCloseByUser() == 0){
			ELog(this << "SelfClose()::Call selfCloseThread.ThreadRun()");
			SetSFlag(cdf_blCloseDueToOS);
			selfCloseThread.ThreadRun();
		}
	}
}
//------------------------------------------------------------------------------------------//
void COMMU_DBUF_FRAME::DoThreadsStart(void){
	ELog(this << "COMMU_DBUF_FRAME::DoThreadsStart()");
	SMC_EncryptI(0)
	SMC_EncryptS(0)
	cgThreadList.LThreadRun();
	SMC_EncryptE(0)
}
//------------------------------------------------------------------------------------------//
void COMMU_DBUF_FRAME::PushSend(void){
	SYS_TIME_S		Timedly;
	SYS_Delay_SetTS(&Timedly, 100);
	SetSFlag(cdf_blPushSend);
	while((ChkblPushSend() != 0) && (SYS_Delay_CheckTS(&Timedly) == 0))
		SYS_DelayMS(2);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//











//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
class ODEV_COMMU_DBUF : public OUTPUT_NODE{
	public:
			 ODEV_COMMU_DBUF(COMMU_DBUF_FRAME *tCommu,OUTPUT_CACHE *tOutputCache = nullptr,uint32 group = 0) : OUTPUT_NODE(COLType_TXT,tOutputCache,group){commu = tCommu;};
	virtual ~ODEV_COMMU_DBUF(void){;};
		private:
			COMMU_DBUF_FRAME	*commu;
	public:
		virtual void	Print	(uint32 col,uint32 ctrl,const uint8 *data,uint32 num){
			ctrl &= COLRECORD::CRD_NL;
			if ((ctrl != 0) && (cgLastCR == 0))
				commu->Send('\n');
			commu->Send(data,num);
		};
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
void COMMU_DBUF_FRAME_LOG::Init(uint32 tSize,const ODEV_SYSTEM *logSys){
	COMMU_DBUF_FRAME::Init(tSize);
	
	logThread.ThreadInit(this, &COMMU_DBUF_FRAME_LOG::LogThreadFun,"log");
	cgThreadList < logThread;
	DisableLog();
	
	cgG2Self = nullptr;
	cgVG3D = nullptr;
	logSystem = (ODEV_SYSTEM*)logSys;
	if (CreateODev_VG3D() != nullptr){
		cgVG3D->RegisterToCache(logSystem->cache);
		EnableLog();
	}
	cgAttrTitle = "";
}
//------------------------------------------------------------------------------------------//
COMMU_DBUF_FRAME_LOG::~COMMU_DBUF_FRAME_LOG(void){
	logThread.ThreadStop();
	logThread.RemoveSelf();
	
	if (cgVG3D != nullptr){
		cgVG3D->UnregisterFromCache();
		cgVG3D->UnregisterAll();
		DestroyTree(cgVG3D);
	}
	if (cgG2Self != nullptr)
		DestroyTree(cgG2Self);
};
//------------------------------------------------------------------------------------------//
int32	COMMU_DBUF_FRAME_LOG::DoOpen(const STDSTR &tCDBufName,int32 tCDBufPar,CSType tCSType,int32 blEnEcho){
	ELog(this << "COMMU_DBUF_FRAME_LOG::DoOpen()");
	if (COMMU_DBUF_FRAME::DoOpen(tCDBufName, tCDBufPar, tCSType, blEnEcho) > 0){
		DoPrintOnOpenSuccess();
		return 1;
	}
	else{
		DoPrintOnOpenFail();
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
void COMMU_DBUF_FRAME_LOG::DoClose(void){
	ELog(this << "COMMU_DBUF_FRAME_LOG::DoClose()");
	COMMU_DBUF_FRAME::DoClose();
	if (ChkblOpened() != 0)
		DoPrintOnClose();
}
//------------------------------------------------------------------------------------------//
void COMMU_DBUF_FRAME_LOG::PrintStr(const COLSTR &colStr1,const COLSTR &colStr2,const COLSTR &colStr3,const COLSTR &colStr4
									,const COLSTR &colStr5,const COLSTR &colStr6,const COLSTR &colStr7,const COLSTR &colStr8){
	OUTPUT_NODE::PrintStr(cgVG3D,colStr1,colStr2,colStr3,colStr4,colStr5,colStr6,colStr7,colStr8);
}
//------------------------------------------------------------------------------------------//
void COMMU_DBUF_FRAME_LOG::PrintStrNL(const COLSTR &colStr1,const COLSTR &colStr2,const COLSTR &colStr3,const COLSTR &colStr4
									,const COLSTR &colStr5,const COLSTR &colStr6,const COLSTR &colStr7,const COLSTR &colStr8){
	OUTPUT_NODE::PrintStrNL(cgVG3D,colStr1,colStr2,colStr3,colStr4,colStr5,colStr6,colStr7,colStr8);
}
//------------------------------------------------------------------------------------------//
void COMMU_DBUF_FRAME_LOG::PrintWithTime(const COLSTR &colStr1,const COLSTR &colStr2,const COLSTR &colStr3,const COLSTR &colStr4
									  ,const COLSTR &colStr5,const COLSTR &colStr6,const COLSTR &colStr7,const COLSTR &colStr8){
	OUTPUT_NODE::PrintWithTime(cgVG3D,colStr1,colStr2,colStr3,colStr4,colStr5,colStr6,colStr7,colStr8);
}
//------------------------------------------------------------------------------------------//
void COMMU_DBUF_FRAME_LOG::PrintWithDividingLine(const COLSTR &colStr1,const COLSTR &colStr2,const COLSTR &colStr3,const COLSTR &colStr4
												 ,const COLSTR &colStr5,const COLSTR &colStr6,const COLSTR &colStr7,const COLSTR &colStr8){
	OUTPUT_NODE::PrintWithDividingLine(cgVG3D,colStr1,colStr2,colStr3,colStr4,colStr5,colStr6,colStr7,colStr8);
}
//------------------------------------------------------------------------------------------//
void COMMU_DBUF_FRAME_LOG::PrintMessage(const COLSTR &colStr1,const COLSTR &colStr2,const COLSTR &colStr3,const COLSTR &colStr4
										,const COLSTR &colStr5,const COLSTR &colStr6,const COLSTR &colStr7,const COLSTR &colStr8){
	OUTPUT_NODE::PrintMessage(cgVG3D,colStr1,colStr2,colStr3,colStr4,colStr5,colStr6,colStr7,colStr8);
}
//------------------------------------------------------------------------------------------//
void COMMU_DBUF_FRAME_LOG::PrintNormalMessage(const STDSTR &strData1,const STDSTR &strData2,const STDSTR &strData3,const STDSTR &strData4
											  ,const STDSTR &strData5,const STDSTR &strData6,const STDSTR &strData7,const STDSTR &strData8){
	OUTPUT_NODE::PrintNormalMessage(cgVG3D,strData1,strData2,strData3,strData4,strData5,strData6,strData7,strData8);
}
//------------------------------------------------------------------------------------------//
void COMMU_DBUF_FRAME_LOG::PrintWarningMessage(const STDSTR &strData1C1,const STDSTR &strData2C2,const STDSTR &strData3C2,const STDSTR &strData4C2
											   ,const STDSTR &strData5C2,const STDSTR &strData6C2,const STDSTR &strData7C2,const STDSTR &strData8C2){
	OUTPUT_NODE::PrintWarningMessage(cgVG3D,strData1C1,strData2C2,strData3C2,strData4C2,strData5C2,strData6C2,strData7C2,strData8C2);
}
//------------------------------------------------------------------------------------------//
void COMMU_DBUF_FRAME_LOG::PrintConnectionReport(const STDSTR &strData1,const STDSTR &strData2,const STDSTR &strData3,const STDSTR &strData4){
	PrintConnectionReport(cgVG3D,strData1,strData2,strData3,strData4);
}
//------------------------------------------------------------------------------------------//
void COMMU_DBUF_FRAME_LOG::PrintConnectionReport(OUTPUT_NODE *node,const STDSTR &strData1,const STDSTR &strData2,const STDSTR &strData3,const STDSTR &strData4){
	if (node != nullptr){
		*node << Start << EnforceDefGroup << NL
		<< COL_DivLineTime
		<< SpaceColStr(COL_NormalMessage,strData1)
		<< SpaceStr(strData2)
		<< SpaceStr(strData3)
		<< SpaceStr(strData4)
		<< ".\n"
		<< COL_DB_Statistics
		<<  "Received : " << Str_ToString(RxBytes()) << " bytes\n"
		<<  "Echo     : " << Str_ToString(FwBytes()) << " bytes\n"
		<<  "Sent     : " << Str_ToString(TxBytes()) << " bytes\n"
		<< COL_DivLine_Maroon
		<< Endl;
	}
}
//------------------------------------------------------------------------------------------//
ODEV_VG3D *COMMU_DBUF_FRAME_LOG::CreateODev_VG3D(void){
	if ((logSystem != nullptr) && (cgVG3D == nullptr))
		cgVG3D = new ODEV_VG3D(logSystem->cache);
	return(cgVG3D);
}
//------------------------------------------------------------------------------------------//
ODEV_FILE *COMMU_DBUF_FRAME_LOG::CreateODev_G3File(const STDSTR &fName){
	if (cgVG3D != nullptr)
		cgVG3D->CreateG3D_FILE(fName,ODEV_FILE::of_blRTF | ODEV_FILE::of_blTXT);
	return(GetG3DefFile());
}
//------------------------------------------------------------------------------------------//
OUTPUT_NODE *COMMU_DBUF_FRAME_LOG::CreateODev_G2Self(void){
	cgG2Self = new ODEV_COMMU_DBUF(this);
	return(cgG2Self);
}
//------------------------------------------------------------------------------------------//
OUTPUT_NODE	*COMMU_DBUF_FRAME_LOG::GetG2DefSelf		(void)const { return(cgG2Self); };
OUTPUT_NODE	*COMMU_DBUF_FRAME_LOG::GetG3DefSelf		(void)const { return((cgVG3D != nullptr) ? cgVG3D->GetDefODEV() : nullptr); };
ODEV_STDOUT	*COMMU_DBUF_FRAME_LOG::GetG3DefSTDOUT	(void)const { return((cgVG3D != nullptr) ? cgVG3D->GetDefSTDOUT() : nullptr); };
ODEV_FILE	*COMMU_DBUF_FRAME_LOG::GetG3DefFile		(void)const { return((cgVG3D != nullptr) ? cgVG3D->GetDefFile() : nullptr); };
//------------------------------------------------------------------------------------------//
OUTPUT_NODE	*COMMU_DBUF_FRAME_LOG::AddG3D_Self		(OUTPUT_NODE *oG3D)	{ return((cgVG3D != nullptr) ? cgVG3D->AddG3D_ODEV(oG3D) : nullptr); };
ODEV_STDOUT	*COMMU_DBUF_FRAME_LOG::AddG3D_STDOUT	(ODEV_STDOUT *oG3D)	{ return((cgVG3D != nullptr) ? cgVG3D->AddG3D_STDOUT(oG3D) : nullptr); };
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
void COMMU_DBUF_FRAME_FW::Init(uint32 tSize,const ODEV_SYSTEM *logSys){
	COMMU_DBUF_FRAME_LOG::Init(tSize,logSys);
	
	fwThread.ThreadInit(this, &COMMU_DBUF_FRAME_FW::FwThreadFun,"fw");
	cgThreadList < fwThread;
	fwThread.Disable();
	
	rxFwSBufList = &cgInternalFwSBufList;
}
//------------------------------------------------------------------------------------------//
COMMU_DBUF_FRAME_FW::~COMMU_DBUF_FRAME_FW(void){
	fwThread.ThreadStop();
	fwThread.RemoveSelf();
};
//------------------------------------------------------------------------------------------//
void COMMU_DBUF_FRAME_FW::UseSelfFwSBufList(void){
	Spin_InUse_set();
	rxFwSBufList = &cgInternalFwSBufList;
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void COMMU_DBUF_FRAME_FW::UseExternalFwSBufList(const SBUF_LIST *tRxFwSBufList){
	Spin_InUse_set();
	rxFwSBufList = (SBUF_LIST*)tRxFwSBufList;
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void COMMU_DBUF_FRAME_FW::RxDataShareTo(SBUFFER *tSBUF){
	Spin_InUse_set();
	if (rxFwSBufList != nullptr)
		rxFwSBufList->AddNode(tSBUF);
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
uint32 COMMU_DBUF_FRAME_FW::ForwardToSelf(COMMU_DBUF_FRAME_FW *commu,const FIFO8 &fifoIn,uint32 num,uint32 *outNum,int32 blInit){
	uint32 sendNum;
	
	sendNum = 0;
	if (commu != nullptr){
		if (commu->IsConnected() != 0){
			if (commu->cgTxSBUF.cgAlreadyReadNum > num){
				commu->cgTxSBUF.cgAlreadyReadNum = num;
			}
			else{
				sendNum = commu->Send(fifoIn, num, commu->cgTxSBUF.cgAlreadyReadNum);
				commu->cgTxSBUF.cgAlreadyReadNum += sendNum;
			}
			if ((blInit == 0) || (*outNum > commu->cgTxSBUF.cgAlreadyReadNum))
				*outNum = commu->cgTxSBUF.cgAlreadyReadNum;
			
			commu->cgTxSBUF.cgAlreadyReadNum -= *outNum;
		}
		else{
			commu->cgTxSBUF.cgAlreadyReadNum = 0;
		}
	}
	return(sendNum);
}
//------------------------------------------------------------------------------------------//
int32 COMMU_DBUF_FRAME_FW::FwThreadFun(void *p){
	uint32	recNum,outNum;
	uint32  blInit;
	
	ELog(this << "FwThreadFun()::Running");
	Spin_InUse_set();
	cgTxSBUF.cgAlreadyReadNum = 0;
	cgLogSBUF.cgAlreadyReadNum = 0;
	if (rxFwSBufList != nullptr){
		TREE_LChildRChain_Traversal_LINE(SBUFFER,rxFwSBufList,operateNode_t->cgAlreadyReadNum = 0;);
	}
	Spin_InUse_clr();
	
	while(fwThread.IsTerminated() == 0){
		SYS_SleepMS(2);
		if (IsConnected() != 0 && (UnreadBytes() > 0)){
			blInit = 0;
			recNum = UnreadBytes();
			outNum = 0;
			if (CheckEcho() == 0){
				cgTxSBUF.cgAlreadyReadNum = 0;
			}
			else{
				cgFwBytes += ForwardToSelf(this,cgRxSBUF.cgFIFO, recNum , &outNum, 0);
				cgTxSBUF.cgAlreadyReadNum += outNum;
				outNum = cgTxSBUF.cgAlreadyReadNum;
				blInit = 1;
			}
			if (CheckEnableLog() == 0){
				cgLogSBUF.cgAlreadyReadNum = 0;
			}
			else{
				cgLogSBUF.cgAlreadyReadNum += cgLogSBUF.Put(cgRxSBUF.cgFIFO,recNum,cgLogSBUF.cgAlreadyReadNum,G_LOCK_OFF);
				if ((blInit == 0) || (outNum > cgLogSBUF.cgAlreadyReadNum)){
					outNum = cgLogSBUF.cgAlreadyReadNum;
					blInit = 1;
				}
			}
			Spin_InUse_set();
			if (rxFwSBufList != nullptr){
				TREE_LChildRChain_Traversal_LINE(SBUFFER,rxFwSBufList,
					if (operateNode_t->cgAlreadyReadNum > recNum){
						operateNode_t->cgAlreadyReadNum = recNum;
					}
					else{
						operateNode_t->cgAlreadyReadNum += operateNode_t->Put(cgRxSBUF.cgFIFO,recNum,operateNode_t->cgAlreadyReadNum,G_LOCK_OFF);//one in one out mode, no need lock
					}
					if ((blInit == 0) || (outNum > operateNode_t->cgAlreadyReadNum)){
						outNum = operateNode_t->cgAlreadyReadNum;
						blInit = 1;
					}
				);
			}
			ForwardToSelf((COMMU_DBUF_THREAD*)GetCoupleNode(this),cgRxSBUF.cgFIFO, recNum, &outNum, blInit);
			if (CheckEcho() != 0)
				cgTxSBUF.cgAlreadyReadNum -= outNum;
			if (CheckEnableLog() != 0)
				cgLogSBUF.cgAlreadyReadNum -= outNum;
			if (rxFwSBufList != nullptr){
				TREE_LChildRChain_Traversal_LINE(SBUFFER,rxFwSBufList,operateNode_t->cgAlreadyReadNum -= outNum;);
			}
			Spin_InUse_clr();
			RxOut(outNum);
		}
	}
	ELog(this << "FwThreadFun()::Stop");
	return 1;
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//









//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
void COMMU_DBUF_THREAD::Init(uint32 tSize,const ODEV_SYSTEM *logSys){
	COMMU_DBUF_FRAME_FW::Init(tSize, logSys);
	rxThread.ThreadInit(this, &COMMU_DBUF_THREAD::RxThreadFun,"rx");
	txThread.ThreadInit(this, &COMMU_DBUF_THREAD::TxThreadFun,"tx");
	cgThreadList < rxThread < txThread;
}
//------------------------------------------------------------------------------------------//
COMMU_DBUF_THREAD::~COMMU_DBUF_THREAD(void){
	CloseD();
	rxThread.ThreadStop();
	txThread.ThreadStop();
	rxThread.RemoveSelf();
	txThread.RemoveSelf();
};
//------------------------------------------------------------------------------------------//
int32 COMMU_DBUF_THREAD::RxThreadFun(void *p){
	FIFO8	*fifo;
	uint32	length,offset,retNum;
	int32	errCode;
	
	ELog(this << "RxThreadFun()::Running");
	while(rxThread.IsTerminated() == 0){
		if ((IsConnected() != 0) && (ChkblCloseDueToOS() == 0)){
			offset = 0;
			length = 0;
			fifo = ReceiveFifo();
			fifo->CalcInCapacity(length, offset);
			errCode = ReadFromDevice(&retNum,fifo->GetPointer(offset),length);
			fifo->In(retNum);
			if ((errCode > 0) && (retNum > 0)){
				cgRxBytes += retNum;
				AfterReadFromDevice();
				continue;
			}
			if ((errCode == -1) && (ChkblCloseByUser() == 0)){
				ELog(this << "RxThreadFun()::OS issue,Call SelfClose()");
				SelfClose();
				break;
			}
		}
		SYS_SleepMS(2);
	}
	ELog(this << "RxThreadFun()::Stop");
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 COMMU_DBUF_THREAD::TxThreadFun(void *p){
	int32	errCode;
	uint32	length,offset,retNum;
	
	ELog(this << "TxThreadFun()::Running");
	while(txThread.IsTerminated() == 0){
		if ((ChkblPushSend() != 0) || ((IsConnected() != 0) && (ChkblCloseDueToOS() == 0) && (UnsentBytes() > 0))){
			do{
				offset = 0;
				cgTxSBUF.cgFIFO.CalcOutCapacity(length,offset);
				errCode = SendToDevice(&retNum,cgTxSBUF.cgFIFO.GetPointer(offset),length);
				TxOut(retNum);
				if (errCode == -1)
					break;
				if ((errCode > 0) && (retNum > 0))
					cgTxBytes += retNum;
			}while((length > 0) && (IsConnected() != 0));
			ClrblPushSend();
			if (errCode == -1)
				break;
		}
		SYS_SleepMS(2);
	}
	ELog(this << "TxThreadFun()::Stop");
	return 1;
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
#ifdef Commu_SSLH
//------------------------------------------------------------------------------------------//
STDSTR COMMU_DBUF_SSL::GetMesgText(uint32 mID){
	STDSTR retStr = "";
#ifdef LOGPRINT
	retStr = "(" + Str_ToString(mID)+ ")";
	switch(mID){
		case MESG_REQ_Close				:retStr += "MESG_REQ_Close ";break;
		case MESG_ANS_Close				:retStr += "MESG_ANS_Close ";break;
		default							:retStr = "";break;
	}
#endif
	return(retStr);
}
//------------------------------------------------------------------------------------------//
void COMMU_DBUF_SSL::Init(CCT_AES_KEYL type,CCT_AES_MODE mode,CCT_Digest digestType,G_ENDIAN tEV){
	cgSSLT2.Init(this,&cgRxSBUF, &cgTxSBUF,type,mode,digestType,tEV);
	SetSelfName(selfName);
}
//------------------------------------------------------------------------------------------//
void COMMU_DBUF_SSL::SetConfig(CCT_AES_KEYL type,CCT_AES_MODE mode,CCT_Digest digestType,G_ENDIAN tEV){
	cgSSLT2.SetConfig(type,mode,digestType,tEV);
}
//------------------------------------------------------------------------------------------//
FIFO8 *COMMU_DBUF_SSL::ReceiveFifo(void){
	if (CheckSFlag(cds_blUseSSL) == 0){
		return(COMMU_DBUF_THREAD::ReceiveFifo());
	}
	else{
		return(cgSSLT2.GetT0RxFifo());
	}
}
//------------------------------------------------------------------------------------------//
void COMMU_DBUF_SSL::AfterReadFromDevice(void){
	if (CheckSFlag(cds_blUseSSL) == 0){
		COMMU_DBUF_THREAD::AfterReadFromDevice();
	}
	else{
		cgSSLT2.AfterReadFromDevice();
	}
};
//------------------------------------------------------------------------------------------//
void COMMU_DBUF_SSL::SSLEnable(void){
	ELog(this << "SSLEnable");
	InDoing_set();
	cgSSLT2.SSLEnable(this);
	cgSSLT2.SSLThreadRun();
	SetblUseSSL();
	InDoing_clr();
};
//------------------------------------------------------------------------------------------//
void COMMU_DBUF_SSL::SSLDisable(void){
	ELog(this << "SSLDisable");
	InDoing_set();
	ClrSFlag(cds_blUseSSL);
	cgSSLT2.SSLDisable();
	InDoing_clr();
};
//------------------------------------------------------------------------------------------//
int32 COMMU_DBUF_SSL::ThreadsStart(void){
	ELog(this << "COMMU_DBUF_SSL::ThreadsStart()");
	if (CheckSFlag(cds_blUseSSL) != 0)
		cgSSLT2.SSLEnable(this);
	if (COMMU_DBUF_THREAD::ThreadsStart() == 0)
		return 0;
	if (CheckSFlag(cds_blUseSSL) == 0)
		return 1;
	if (cgSSLT2.Handshake(this) == 0){
		SetSFlag(cds_blClose);
		return 0;
	}
	return 1;
}
//------------------------------------------------------------------------------------------//
void COMMU_DBUF_SSL::DoClose(void){
	ELog(this << "COMMU_DBUF_SSL::DoClose()");
	if (CheckSFlag(cds_blUseSSL) != 0){
		if ((IsConnected() != 0) && (cgSSLT2.ChkblClose() == 0) && (CheckSFlag(cds_blClose) == 0) && (ChkblCloseDueToOS() == 0)){
			ELog(this << "COMMU_DBUF_SSL::DoClose()::Call SendREQ_Close()");
			SendREQ_Close();
		}
	}
	ClrSFlag(cds_blClose);
	COMMU_DBUF_THREAD::DoClose();
};
//------------------------------------------------------------------------------------------//
void COMMU_DBUF_SSL::CloseDev(void){
	ELog(this << "COMMU_DBUF_SSL::CloseDev()");
	COMMU_DBUF_THREAD::CloseDev();
	if (CheckSFlag(cds_blUseSSL) != 0){
		ELog(this << "COMMU_DBUF_SSL::CloseDev()::Call cgSSLT2.Reset()");
		cgSSLT2.Reset();
	}
};
//------------------------------------------------------------------------------------------//
#define CHSendInMP(_mID,_strMesg,_blRet)	CHSend(_mID,_strMesg,_blRet,"MessageProcessing()::")
#define CHRecInMP(_mID)						CHRec(_mID,"MessageProcessing()::");
#define E2LogInMP(_strMesg)					E2Log(this << "MessageProcessing()::" << _strMesg);
//------------------------------------------------------------------------------------------//
int32 COMMU_DBUF_SSL::MessageProcessing(const uint32 &mID,const STDSTR &strMesg){
	STDSTR	strMesgT,strContent;
	int32	blDo,blRet;
	
	if (mID == MESG_NONE)
		return 0;
	
	blDo = 1;
	if ((mID >= MESG_NEXT) && (mID < MESG_NEXT_SSL)){
		CHRecInMP(mID);
	}
	switch (mID){
		case MESG_REQ_Close:
			CHSendInMP(MESG_ANS_Close,"",blRet);
			E2LogInMP("Call SelfClose()");
			SetSFlag(cds_blClose);
			SelfClose();
			break;
		case MESG_ANS_Close:
			PushSend();
			SetSFlag(cds_blClose);
			break;
		default:
			blDo = 0;
			break;
	}
	return(blDo);
}
//------------------------------------------------------------------------------------------//
int32 COMMU_DBUF_SSL::SendCHMesg(uint32 mID,const STDSTR &strMesg,const uint64 &flagY,const uint64 &flagN,const uint32 &waitTimeS){
	SYS_TIME_S		timedly;
	int32			blRet;
	cgSendCHMesgLock.Lock();
	if (IsConnected() == 0){
		blRet = -1;
	}
	else{
		SYS_Delay_SetTS(&timedly, waitTimeS << 10);
		ClrSFlag(flagY | flagN);
		
		CHSend(mID,strMesg,blRet,"");
		if (blRet == 0){
			blRet = -3;
		}
		else{
			blRet = CheckFlagStatus(flagY,flagN,timedly);
		}
	}
	cgSendCHMesgLock.Unlock();
	return(blRet);
}
//------------------------------------------------------------------------------------------//
int32 COMMU_DBUF_SSL::CheckFlagStatus(const uint64 &flagY,const uint64 &flagN,SYS_TIME_S &timedly){
	
	while((CheckSFlag(flagY | flagN) == 0) && (IsConnected() != 0) && (SYS_Delay_CheckTS(&timedly) == 0))
		SYS_SleepMS(100);
	if (CheckSFlag(flagY) > 0){
		return 1;
	}
	else if (CheckSFlag(flagN) > 0){
		return 0;
	}
	else if (IsConnected() == 0){
		return -1;
	}
	return -2;
}
//------------------------------------------------------------------------------------------//
int32 COMMU_DBUF_SSL::SendREQ_Close(void){
	int32	blRet;
	blRet = SendCHMesg(MESG_REQ_Close,"",cds_blClose,cds_blClose,1);
	if ((blRet == 1) || (blRet == -3))
		return 1;
	return 0;
};
//------------------------------------------------------------------------------------------//
uint32 COMMU_DBUF_SSL::Send(const PROTOCOL_NODE &pnIn){
	if (CheckSFlag(cds_blUseSSL) == 0){
		return(COMMU_DBUF_THREAD::Send(pnIn));
	}
	else{
		return(DataCHWrite(pnIn));
	}
};
//------------------------------------------------------------------------------------------//
uint32 COMMU_DBUF_SSL::Send(const FIFO8 &fifoIn,uint32 num,uint32 offset){
	if (CheckSFlag(cds_blUseSSL) == 0){
		return(COMMU_DBUF_THREAD::Send(fifoIn,num,offset));
	}
	else{
		return(DataCHWrite(fifoIn,num,offset));
	}
};
//------------------------------------------------------------------------------------------//
uint32 COMMU_DBUF_SSL::Send(const uint8 *data,uint32 num){
	if (CheckSFlag(cds_blUseSSL) == 0){
		return(COMMU_DBUF_THREAD::Send(data,num));
	}
	else{
		return(DataCHWrite(data,num));
	}
};
//------------------------------------------------------------------------------------------//
uint32 COMMU_DBUF_SSL::Send(const uint8 data){
	if (CheckSFlag(cds_blUseSSL) == 0){
		return(COMMU_DBUF_THREAD::Send(data));
	}
	else{
		return(DataCHWrite(data));
	}
};
//------------------------------------------------------------------------------------------//
uint32 COMMU_DBUF_SSL::Send(const STDSTR &strIn,G_ESCAPE blEscape){
	if (CheckSFlag(cds_blUseSSL) == 0){
		return(COMMU_DBUF_THREAD::Send(strIn,blEscape));
	}
	else{
		return(DataCHWrite(strIn));
	}
};
//------------------------------------------------------------------------------------------//
uint32 COMMU_DBUF_SSL::SendInHEX(const STDSTR &strIn){
	if (CheckSFlag(cds_blUseSSL) == 0){
		return(COMMU_DBUF_THREAD::SendInHEX(strIn));
	}
	else{
		return(DataCHWriteInHex(strIn));
	}
};
//------------------------------------------------------------------------------------------//
#endif
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 COMMU_DBUF::LogThreadFun(void *p){
	int32	byteNum;
	uint64	envcfg;
	STDSTR	strPrintdata;
	
	ELog(this << "LogThreadFun()::Running");
	COMMU_DBUF_FRAME_LOG::LogThreadFun(p);
	strPrintdata = "";
	while(logThread.IsTerminated() == 0){
		if (cgLogSBUF.Used() > 0){
			byteNum = cgLogSBUF.Used();
			strPrintdata = "";
			
			envcfg = GetLogSystem()->envcfg;
			if (B_ChkFLAG64(envcfg,ODEV_FLAG_EnHEXViewMode) != 0){
				cgLogSBUF.GetInHEX(&strPrintdata,G_SPACE_ON);
				if (B_ChkFLAG64(envcfg,ODEV_FLAG_EnOSPMsgLine) != 0)
					strPrintdata = Str_Replace(strPrintdata," B0 B3 "," B0 B3\n");
			}
			else{
				cgLogSBUF.Get(&strPrintdata, (B_ChkFLAG64(envcfg, ODEV_FLAG_EnEscape) != 0) ? G_ESCAPE_ON : G_ESCAPE_OFF);
			}
			PrintRecData(strPrintdata,byteNum,B_ChkFLAG64(envcfg,ODEV_FLAG_EnRecMsg));
		}
		SYS_SleepMS(10);
	}
	ELog(this << "LogThreadFun()::Stop");
	return 1;
}
//------------------------------------------------------------------------------------------//
void COMMU_DBUF::PrintRecData(const STDSTR &strData, uint32 byteNum, int32 blEnRecMsg){
	STDSTR	strSelfName,strCoupleName;
	int32	blCon;
	COMMU_DBUF *cpNode;

	if (GetVG3D() != nullptr){
		strSelfName = GetDevName();
		strCoupleName = "";
		blCon = 0;
		
		Spin_InUse_set();
		cpNode = (COMMU_DBUF*)GetCoupleNode(this);
		if (cpNode != nullptr){
			strCoupleName = cpNode->GetDevName();
			blCon = cpNode->IsConnected();
		}
		Spin_InUse_clr();
		
		*GetVG3D() << Start;
		if (CheckEcho() != 0){
			*GetVG3D() << NL << COL_DivLineTime << COL_DB_RxTitle << MakeEchoTitle(strSelfName, byteNum, FwBytes()) << COL_DB_RxText;
			if (blCon != 0){
				if (GetCNType(this) == CN_M)
					*GetVG3D() << NL << COL_DB_RxTitleM << MakeTxTitle("", strCoupleName, byteNum, RxBytes()) << COL_DB_RxTextM;
				if (GetCNType(this) == CN_S)
					*GetVG3D() << NL << COL_DB_RxTitleS << MakeRxTitle(strCoupleName, "", byteNum, RxBytes()) << COL_DB_RxTextS;
			}
		}
		else if (GetCNType(this) == CN_None){
			if ((GetCSType() == CSType_TCPS) || (GetCSType() == CSType_UDPS)){
				*GetVG3D() << NL << COL_DivLineTime << COL_DB_RxTitle << MakeRxTitle("", strSelfName, byteNum, RxBytes()) << COL_DB_RxText;
			}
			else{
				if (blEnRecMsg != 0)
					*GetVG3D() << NL << COL_DivLineTime << COL_DB_RxTitle << MakeRxTitle(byteNum, RxBytes());
				*GetVG3D() << COL_DB_RxText;
			}
		}
		else if (GetCNType(this) == CN_M){
			if (blCon == 0){
				if ((GetCSType() == CSType_TCPS) || (GetCSType() == CSType_UDPS)){
					*GetVG3D() << NL << COL_DivLineTime << COL_DB_RxTitle << MakeRxTitle("", strSelfName, byteNum, RxBytes()) << COL_DB_RxText;
				}
				else{
					if (blEnRecMsg != 0)
						*GetVG3D() << NL << COL_DivLineTime << COL_DB_RxTitle << MakeRxTitle(byteNum, RxBytes());
					*GetVG3D() << COL_DB_RxText;
				}
			}
			else{
				*GetVG3D() << NL << COL_DivLineTime << COL_DB_RxTitleM << MakeTxTitle(strSelfName, strCoupleName, byteNum, RxBytes()) << COL_DB_RxTextM;
			}
		}
		else if (GetCNType(this) == CN_S){
			if (blCon == 0){
				*GetVG3D() << NL << COL_DivLineTime << COL_DB_RxTitle << MakeRxTitle("", strSelfName, byteNum, RxBytes()) << COL_DB_RxText;
			}
			else{
				*GetVG3D() << NL << COL_DivLineTime << COL_DB_RxTitleS << MakeRxTitle(strSelfName, strCoupleName, byteNum, RxBytes()) << COL_DB_RxTextS;
			}
		}
		if ((GetCSType() == CSType_TCPS) || (GetCSType() == CSType_UDPS)){
			if (CheckSelected() == 0)
				*GetVG3D() << ClrGroup(COLRECORD::CRD_G1);
		}
		*GetVG3D() << strData << Endl;
	}
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
STDSTR	MakeByteNum(uint32 ByteNum, uint64 tTotal, uint32 blCR){
	STDSTR	strSendData;
	
	strSendData = Str_ToString(ByteNum);
	strSendData += '/';
	strSendData += Str_ToString(tTotal);
	
	if (strSendData.length() < 12)
		strSendData.insert(0,12 - strSendData.length(),' ');
	
	strSendData += " Bytes: ";
	
	if (blCR != 0)
		strSendData += "\n";
	return(strSendData);
}
//------------------------------------------------------------------------------------------//
STDSTR	MakeTitle(const STDSTR &strTitle, uint32 ByteNum, uint64 tTotal, uint32 blCR){
	STDSTR	strSendData;
	
	strSendData = strTitle;
	strSendData += MakeByteNum(ByteNum, tTotal, blCR);
	return(strSendData);
}
//------------------------------------------------------------------------------------------//
STDSTR	MakeTxTitle(const STDSTR &strM, const STDSTR &strS, uint32 ByteNum, uint64 tTotal, uint32 blCR){
	STDSTR	strSendData;
	
	strSendData = "";
	if (strM.length() == 0){
		strSendData += " SDT ->> ";
		strSendData += strS;
	}
	else if (strS.length() == 0){
		strSendData += ' ';
		strSendData += strM;
		strSendData += " ->> SDT";
	}
	else{
		strSendData += ' ';
		strSendData += strM;
		strSendData += " ->> ";
		strSendData += strS;
	}
	strSendData += ' ';
	strSendData += MakeByteNum(ByteNum, tTotal,blCR);
	return(strSendData);
}
//------------------------------------------------------------------------------------------//
STDSTR	MakeRxTitle(const STDSTR &strM, const STDSTR &strS, uint32 ByteNum, uint64 tTotal, uint32 blCR){
	STDSTR	strSendData;
	
	strSendData = "";
	if (strM.length() == 0){
		strSendData += " SDT <<- ";
		strSendData += strS;
	}
	else if (strS.length() == 0){
		strSendData += ' ';
		strSendData += strM;
		strSendData += " <<- SDT";
	}
	else{
		strSendData += ' ';
		strSendData += strM;
		strSendData += " <<- ";
		strSendData += strS;
	}
	strSendData += ' ';
	strSendData += MakeByteNum(ByteNum, tTotal,blCR);
	return(strSendData);
}
//------------------------------------------------------------------------------------------//
STDSTR	MakeEchoTitle(const STDSTR &strM, uint32 ByteNum, uint64 tTotal, uint32 blCR){
	STDSTR	strSendData;
	
	strSendData = "";
	strSendData += " SDT <-> ";
	strSendData += strM;
	strSendData += ' ';
	strSendData += MakeByteNum(ByteNum, tTotal, blCR);
	return(strSendData);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif

