////
//  Commu_DBuf.cpp
//  SDT
//
//  Created by Leif Wen on 02/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
#include "Commu_DBuf.h"
//------------------------------------------------------------------------------------------//
#include "DS_STRING.h"
#include "ADS_SMC.h"
#include "SYS_Time.h"
#ifdef Commu_DBuf_h
//#define LOGPRINT_ENABLE
//#define LOGTHREAD_ENABLE
//#define LOGA_ENABLE
//#define LOG0_ENABLE

#ifdef LOG0_ENABLE
	#define ELog0(x) ELog(x);
#else
	#define ELog0(x);
#endif

#ifdef LOGA_ENABLE
	#define ELogA(x) ELog(x);
#else
	#define ELogA(x);
#endif
#include "SYS_Log.h"
//------------------------------------------------------------------------------------------//
COMMU_FRAME::COMMU_FRAME(uint32 size) : DBUF(){
	cgMaxSize = size;
	selfCloseThread.ThreadInit(this, &COMMU_FRAME::SelfCloseThreadFun,"selfClose");
	
	cgDevName = "";
	SetOpenPar(&cgOpenPar,OPEN_None,"",0,0);
	
	TNFP::SetSelfName("COMMU_FRAME");
	SetSelfName(selfName);
	SetFatherName("");
	ELogA(this);
};
//------------------------------------------------------------------------------------------//
COMMU_FRAME::~COMMU_FRAME(void){
	UnlinkCoupleNode();
	Close();
	selfCloseThread.ThreadStop();
	ELogA(this);
};
//------------------------------------------------------------------------------------------//
bool32 COMMU_FRAME::Open(const OPEN_PAR& par,G_LOCK blLock){
	
	ELog0(this << "COMMU_FRAME::Open()");
	
	bool32 err = G_FALSE;
	if (InDoing_try(blLock)){
		if (CheckSFlag(CF_blOpened))
			DoClose();
		err = DoOpen(par);
		if (err <= G_FALSE)
			DoClose();
		InDoing_clr(blLock);
	}
	return(err > 0 ? G_TRUE : G_FALSE);
}
//------------------------------------------------------------------------------------------//
bool32 COMMU_FRAME::DoOpen(const OPEN_PAR& par){

	ELog0(this << "COMMU_FRAME::DoOpen()");
	
	bool32 err = G_FALSE;
	
	Empty();
	SetOpenPar(&cgOpenPar, par);
	cgDevName = cgOpenPar.name + ":" + Str_ToStr(cgOpenPar.port);

	err = OpenDev(cgOpenPar);
	
	if (err > G_FALSE){
		DisableEcho();
		if (B_ChkFLAG32(par.cfg, CFG_blEnableEcho))
			EnableEcho();
		ClrSFlag(CF_blPushSend | CF_blCloseDueToOS);
		SetSFlag(CF_blOpened | CF_blConnected | CF_blEnableSelfClose);
		Enable();
		if (CheckSFlag(CF_blNoInitSize) == G_FALSE)
			InitSize(cgMaxSize);
		err = ThreadsStart();
	}
	return(err);
}
//------------------------------------------------------------------------------------------//
void COMMU_FRAME::Close(void){
	ELog0(this << "COMMU_FRAME::Close()");
	
	InDoing_set();
	DoClose();
	InDoing_clr();
	
	ELog0(this << "COMMU_FRAME::Close() finished");
}
//------------------------------------------------------------------------------------------//
void COMMU_FRAME::DoClose(void){
	ELog0(this << "COMMU_FRAME::DoClose()");

	ClrSFlag(CF_blEnableSelfClose);
	
	if (CheckSFlag(CF_blEnCloseCouple)){
		COMMU_FRAME	*coupleSocket;
		coupleSocket = static_cast<COMMU_FRAME*>(this->UnlinkCoupleNode());
		if (coupleSocket != nullptr){
			ELog0(this << "COMMU_FRAME::DoClose()::Call " << coupleSocket << "::Close()");
			coupleSocket->Close();
		}
	}
	if (IsOpened())
		PushSend(100);
	
	CloseDev();
	ClrSFlag(CF_blConnected);
	
	ELog0(this << "COMMU_FRAME::DoClose()::Call ThreadsStop()");
	ThreadsStop();
	ELog0(this << "COMMU_FRAME::DoClose()::Call CloseAllChild()");
	CloseAllChild();
	
	ClrSFlag(CF_blOpened);
	Disable();
	ELog0(this << "COMMU_FRAME::DoClose() finished");
}
//------------------------------------------------------------------------------------------//
void COMMU_FRAME::CloseAllChild(void){
	TREE_LChildRChain_Traversal_LINE(COMMU_FRAME, this,
		ELog0(this << "COMMU_FRAME::CloseAllChild(): " << _opNode);
		_opNode->Close();
	);
	ChildClrSel(GetSelDB());
	CleanChild(this,this);
	SetblUpdate();
};
//------------------------------------------------------------------------------------------//
void COMMU_FRAME::CloseChild(COMMU_FRAME* commu){
	if (commu != nullptr){
		ELog0(this << "COMMU_FRAME::CloseChild(): " << commu);
		commu->Close();
		ChildClrSel(commu);
		MoveNodesToTrash(this,commu);
		SetblUpdate();
	}
};
//------------------------------------------------------------------------------------------//
SYS_AThread* COMMU_FRAME::GetThread(const STDSTR& name){
	SYS_AThread* ret = nullptr;
	TREE_LChildRChain_Traversal_LINE_nolock(SYS_AThread, (&cgThreadList),
		if (_opNode->selfName == name){
			ret = _opNode;
			break;
		}
	);
	return(ret);
};
//------------------------------------------------------------------------------------------//
bool32 COMMU_FRAME::SelfCloseThreadFun(void* commu){
	
	ETLogThreadStart(selfCloseThread);
	SYS_DelayMS(2);
	if (commu == nullptr){
		Close();
	}
	else{
		static_cast<COMMU_FRAME*>(commu)->CloseChild(this);
	}
	
	ETLogThreadStop(selfCloseThread);
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
void COMMU_FRAME::DoSelfClose(void){
	if (CheckSFlag(CF_blEnableSelfClose)){
		ClrSFlag(CF_blEnableSelfClose);
		if (selfCloseThread.IsTerminated()){
			selfCloseThread.ThreadStop();
			SetSFlag(CF_blCloseDueToOS);
			ELog0(this << "COMMU_FRAME::DoSelfClose()::Call selfCloseThread.ThreadRun()");
			selfCloseThread.ThreadRun(GetFDB());
		}
	}
}
//------------------------------------------------------------------------------------------//
bool32 COMMU_FRAME::CloseSelf(uint32 timeoutMS){
	DoSelfClose();
	if (timeoutMS != 0){
		SYS_TIME_S	timeS;
		
		SYS_Delay_SetTS(&timeS,timeoutMS);
		while(SYS_Delay_CheckTS(&timeS) == G_FALSE){
			SYS_SleepMS(2);
			if (IsOpened() == G_FALSE)
				return G_TRUE;
		};
		return G_FALSE;
	}
	else{
		while(IsOpened())
			SYS_SleepMS(2);
	}
	return G_TRUE;
}
//------------------------------------------------------------------------------------------//
void COMMU_FRAME::DoThreadsStart(void){
	ELog0(this << "COMMU_FRAME::DoThreadsStart()");
	SMC_EncryptI(0)
	SMC_EncryptS(0)
	cgThreadList.ThreadRun(this);
	SMC_EncryptE(0)
}
//------------------------------------------------------------------------------------------//
void COMMU_FRAME::PushSend(uint32 timeMS){
	SYS_TIME_S		Timedly;
	SYS_Delay_SetTS(&Timedly, timeMS);
	SetSFlag(CF_blPushSend);
	while(ChkblPushSend() && (SYS_Delay_CheckTS(&Timedly) == 0))
		SYS_DelayMS(2);
};
//------------------------------------------------------------------------------------------//
COMMU_FRAME* COMMU_FRAME::Find(const STDSTR& name,uint32 port){
	COMMU_FRAME *retCommu;
	retCommu = nullptr;
	TREE_LChildRChain_Find(COMMU_FRAME,this,retCommu
						   ,((_opNode->GetOpenPar().name == name) && (_opNode->GetOpenPar().port == port)));
	return(retCommu);
}
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
COMMU_FRAME_FW::COMMU_FRAME_FW(uint32 size) : COMMU_FRAME(size){
	
	fwThread.ThreadInit(this, &COMMU_FRAME_FW::FwThreadFun,"fw");
	cgThreadList < fwThread;
	fwThread.Disable();
	
	rxFwSBufList = &cgInternalFwSBufList;
	
	TNFP::SetSelfName("COMMU_FRAME_FW");
	SetSelfName(selfName);
	SetFatherName("");
	ELogA(this);
};
//------------------------------------------------------------------------------------------//
COMMU_FRAME_FW::~COMMU_FRAME_FW(void){
	fwThread.ThreadStop();
	fwThread.RemoveSelf();
	ELogA(this);
};
//------------------------------------------------------------------------------------------//
uint32 COMMU_FRAME_FW::ForwardToCommu(COMMU_FRAME_FW* commu,uint32* outNum,const ARRAY& array,uint32 inNum,bool32 blInit){
	uint32 sendNum;
	
	sendNum = 0;
	if (commu != nullptr){
		if (commu->IsConnected()){
			if (commu->cgTxSBUF.cgAlreadyReadNum > inNum){
				commu->cgTxSBUF.cgAlreadyReadNum = inNum;
			}
			else{
				sendNum = commu->Send(IUD(array, inNum, commu->cgTxSBUF.cgAlreadyReadNum));
				commu->cgTxSBUF.cgAlreadyReadNum += sendNum;
			}
			if ((blInit == G_FALSE) || (*outNum > commu->cgTxSBUF.cgAlreadyReadNum))
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
static void ForwardToSBUFList(TNFP* list,uint32* outNum,const ARRAY& array,uint32 inNum,bool32 blInit){
	TREE_LChildRChain_Traversal_LINE(SBUF,list,
		if (_opNode->cgAlreadyReadNum > inNum){
			_opNode->cgAlreadyReadNum = inNum;
		}
		else{
			_opNode->cgAlreadyReadNum += _opNode->Put(nullptr,IUD(array,inNum,_opNode->cgAlreadyReadNum),G_LOCK_OFF);
				//one in one out mode, no need lock
		}
		if ((blInit == G_FALSE) || (*outNum > _opNode->cgAlreadyReadNum)){
			*outNum = _opNode->cgAlreadyReadNum;
			blInit = G_TRUE;
		}
	);
}
//------------------------------------------------------------------------------------------//
bool32 COMMU_FRAME_FW::FwThreadFun(void* commu){
	uint32	recNum,outNum;
	bool32  blInit;
	
	ETLogThreadStart(fwThread);
	InUse_set();
	cgTxSBUF.cgAlreadyReadNum = 0;
	{TREE_LChildRChain_Traversal_LINE(SBUF,(&cgInternalFwSBufList),_opNode->cgAlreadyReadNum = 0;);};
	if ((rxFwSBufList != nullptr) && (rxFwSBufList != &cgInternalFwSBufList)){
		TREE_LChildRChain_Traversal_LINE(SBUF,rxFwSBufList,_opNode->cgAlreadyReadNum = 0;);
	}
	InUse_clr();
	
	while(fwThread.IsTerminated() == 0){
		SYS_SleepMS(2);
		if (IsConnected() && (UnreadBytes() > 0)){
			blInit = G_FALSE;
			recNum = UnreadBytes();
			outNum = 0;
			if (CheckEcho() == G_FALSE){
				cgTxSBUF.cgAlreadyReadNum = 0;
			}
			else{
				cgFwBytes += ForwardToCommu(this,&outNum,cgRxSBUF.GetArray(), recNum, G_FALSE);
				cgTxSBUF.cgAlreadyReadNum += outNum;
				outNum = cgTxSBUF.cgAlreadyReadNum;
				blInit = G_TRUE;
			}
			InUse_set();
			ForwardToSBUFList(&cgInternalFwSBufList,&outNum,cgRxSBUF.GetArray(),recNum,blInit);
			if ((rxFwSBufList != nullptr) && (rxFwSBufList != &cgInternalFwSBufList))
				ForwardToSBUFList(rxFwSBufList,&outNum,cgRxSBUF.GetArray(),recNum,blInit);
			
			ForwardToCommu((COMMU_THREAD*)GetCoupleNode(this), &outNum, cgRxSBUF.GetArray(), recNum, blInit);
			
			if (CheckEcho())
				cgTxSBUF.cgAlreadyReadNum -= outNum;
			
			{TREE_LChildRChain_Traversal_LINE(SBUF,(&cgInternalFwSBufList),_opNode->cgAlreadyReadNum -= outNum;);};

			if ((rxFwSBufList != nullptr) && (rxFwSBufList != &cgInternalFwSBufList)){
				TREE_LChildRChain_Traversal_LINE(SBUF,rxFwSBufList,_opNode->cgAlreadyReadNum -= outNum;);
			}
			InUse_clr();
			cgRxSBUF.Out(outNum);
		}
	}
	ETLogThreadStop(fwThread);
	return G_TRUE;
}
/*//------------------------------------------------------------------------------------------//
bool32 COMMU_FRAME_FW::FwThreadFun(void* commu){
	uint32	recNum;
	
	ETLogThreadStart(fwThread);
	while(fwThread.IsTerminated() == 0){
		SYS_SleepMS(2);
		if (IsConnected() && (UnreadBytes() > 0)){
			recNum = UnreadBytes();
			
			if (CheckEcho())
				Send(IUD(cgRxSBUF.GetArray(), recNum, 0));
			
			InUse_set();
			{TREE_LChildRChain_Traversal_LINE(SBUF,(&cgInternalFwSBufList),_opNode->Put(nullptr,IUD(cgRxSBUF.GetArray(), recNum, 0),G_LOCK_OFF););};

			if ((rxFwSBufList != nullptr) && (rxFwSBufList != &cgInternalFwSBufList)){
				TREE_LChildRChain_Traversal_LINE(SBUF,rxFwSBufList,_opNode->Put(nullptr,IUD(cgRxSBUF.GetArray(), recNum, 0),G_LOCK_OFF););
			};
			if (((COMMU_THREAD*)GetCoupleNode(this)) != nullptr)
				((COMMU_THREAD*)GetCoupleNode(this))->Send(IUD(cgRxSBUF.GetArray(), recNum, 0));
			
			InUse_clr();
			cgRxSBUF.Out(recNum);
		}
	}
	ETLogThreadStop(fwThread);
	return G_TRUE;
}*/
//------------------------------------------------------------------------------------------//









#ifdef Output_h
//------------------------------------------------------------------------------------------//
class ODEV_COMMU_DBUF : public OUTPUT_NODE{
	public:
				 ODEV_COMMU_DBUF(COMMU_FRAME* commu,OUTPUT_CACHE* cache = nullptr,uint32 group = 0);
		virtual ~ODEV_COMMU_DBUF(void){;};
	private:
		COMMU_FRAME*	cgCommu;
	public:
		virtual 	void	Print		(uint32 ctrl,COLORENUM col,const uint8* data,uint32 num);
};
//------------------------------------------------------------------------------------------//
ODEV_COMMU_DBUF::ODEV_COMMU_DBUF(COMMU_FRAME* commu,OUTPUT_CACHE* cache,uint32 group) : OUTPUT_NODE(COLType_TXT,cache,group){
	SetSelfName("ODEV_COMMU_DBUF");
	cgCommu = commu;
	ELogA(this);
};
//------------------------------------------------------------------------------------------//
void ODEV_COMMU_DBUF::Print(uint32 ctrl,COLORENUM col,const uint8* data,uint32 num){
	ctrl &= COLRECORD::CRD_NL;
	if ((ctrl != 0) && (cgLastCR == 0))
		cgCommu->Send(IUD("\n"));
	cgCommu->Send(IUD(data,num));
};
//------------------------------------------------------------------------------------------//
#endif









//------------------------------------------------------------------------------------------//
COMMU_FRAME_LOGSYS::COMMU_FRAME_LOGSYS(uint32 size,const ODEV_SYSTEM* logSys) : COMMU_FRAME_FW(size){
#ifdef ODEV_System_h
	cgG2Self = nullptr;
	cgVG3D = nullptr;
	logSystem = nullptr;
	InitLogSys(logSys);
#endif
	cgAttrTitle = "";
	
	TNFP::SetSelfName("COMMU_FRAME_LOGSYS");
	SetSelfName(selfName);
	SetFatherName("");
	ELogA(this);
};
//------------------------------------------------------------------------------------------//
void COMMU_FRAME_LOGSYS::InitLogSys(const ODEV_SYSTEM* logSys){
#ifdef ODEV_System_h
	if (logSystem == nullptr){
		if (logSys != nullptr){
			logSystem = (ODEV_SYSTEM*)logSys;
			if (CreateODev_VG3D() != nullptr)
				cgVG3D->RegisterToCache(logSystem->cache);
		}
	}
#endif
};
//------------------------------------------------------------------------------------------//
COMMU_FRAME_LOGSYS::~COMMU_FRAME_LOGSYS(void){
#ifdef ODEV_System_h
	if (cgVG3D != nullptr){
		cgVG3D->UnregisterToCache();
		cgVG3D->UnregisterAll();
		DestroyTree(cgVG3D);
	}
	if (cgG2Self != nullptr)
		DestroyTree(cgG2Self);
#endif
};
//------------------------------------------------------------------------------------------//
bool32 COMMU_FRAME_LOGSYS::DoOpen(const OPEN_PAR& par){
	ELog0(this << "COMMU_FRAME_LOGSYS::DoOpen()");
#ifdef ODEV_System_h
	if (COMMU_FRAME::DoOpen(par) > 0){
		DoPrintOnOpenSuccess();
		return G_TRUE;
	}
	else{
		DoPrintOnOpenFail();
	}
	return G_FALSE;
#else
	return(COMMU_FRAME::DoOpen(par));
#endif
}
//------------------------------------------------------------------------------------------//
void COMMU_FRAME_LOGSYS::DoClose(void){
	ELog0(this << "COMMU_FRAME_LOGSYS::DoClose()");
	
#ifdef ODEV_System_h
	if (IsOpened()){
		COMMU_FRAME::DoClose();
		DoPrintOnClose();
	}
	else{
		COMMU_FRAME::DoClose();
	}
#else
	COMMU_FRAME::DoClose();
#endif
}
//------------------------------------------------------------------------------------------//
ODEV_VG3D* COMMU_FRAME_LOGSYS::CreateODev_VG3D(void){
#ifdef ODEV_System_h
	if ((logSystem != nullptr) && (cgVG3D == nullptr))
		cgVG3D = new ODEV_VG3D(logSystem->cache);
	return(cgVG3D);
#else
	return(nullptr);
#endif
}
//------------------------------------------------------------------------------------------//
ODEV_FILE* COMMU_FRAME_LOGSYS::CreateODev_G3File(const STDSTR &fName){
#ifdef ODEV_System_h
	if (cgVG3D != nullptr)
		cgVG3D->CreateG3D_FILE(fName,ODEV_FILE::ODEV_blRTF | ODEV_FILE::ODEV_blTXT);
	return(GetG3DefFile());
#else
	return(nullptr);
#endif
}
//------------------------------------------------------------------------------------------//
OUTPUT_NODE* COMMU_FRAME_LOGSYS::CreateODev_G2Self(void){
#ifdef ODEV_System_h
	cgG2Self = new ODEV_COMMU_DBUF(this);
	return(cgG2Self);
#else
	return(nullptr);
#endif
}
//------------------------------------------------------------------------------------------//
OUTPUT_NODE* COMMU_FRAME_LOGSYS::GetG2DefSelf(void)const{
#ifdef ODEV_System_h
	return(cgG2Self);
#else
	return(nullptr);
#endif
};
//------------------------------------------------------------------------------------------//
OUTPUT_NODE* COMMU_FRAME_LOGSYS::GetG3DefSelf(void)const{
#ifdef ODEV_System_h
	return((cgVG3D != nullptr) ? cgVG3D->GetDefODEV() : nullptr);
#else
	return(nullptr);
#endif
};
//------------------------------------------------------------------------------------------//
ODEV_STDOUT* COMMU_FRAME_LOGSYS::GetG3DefSTDOUT(void)const{
#ifdef ODEV_System_h
	return((cgVG3D != nullptr) ? cgVG3D->GetDefSTDOUT() : nullptr);
#else
	return(nullptr);
#endif
};
//------------------------------------------------------------------------------------------//
ODEV_FILE* COMMU_FRAME_LOGSYS::GetG3DefFile(void)const{
#ifdef ODEV_System_h
 	return((cgVG3D != nullptr) ? cgVG3D->GetDefFile() : nullptr);
#else
	return(nullptr);
#endif
};
//------------------------------------------------------------------------------------------//
OUTPUT_NODE* COMMU_FRAME_LOGSYS::AddG3D_Self(OUTPUT_NODE *oG3D){
#ifdef ODEV_System_h
	return((cgVG3D != nullptr) ? cgVG3D->AddG3D_ODEV(oG3D) : nullptr);
#else
	return(nullptr);
#endif
};
//------------------------------------------------------------------------------------------//
ODEV_STDOUT* COMMU_FRAME_LOGSYS::AddG3D_STDOUT(ODEV_STDOUT *oG3D){
#ifdef ODEV_System_h
	return((cgVG3D != nullptr) ? cgVG3D->AddG3D_STDOUT(oG3D) : nullptr);
#else
	return(nullptr);
#endif
};
//------------------------------------------------------------------------------------------//




















//------------------------------------------------------------------------------------------//
COMMU_THREAD::COMMU_THREAD(uint32 size,const ODEV_SYSTEM* logSys) : COMMU_FRAME_LOGSYS(size,logSys){

	rxThread.ThreadInit(this, &COMMU_THREAD::RxThreadFun,"rx");
	txThread.ThreadInit(this, &COMMU_THREAD::TxThreadFun,"tx");
	logThread.ThreadInit(this, &COMMU_THREAD::LogThreadFun,"log");
	
	cgThreadList < rxThread < txThread;
#ifdef ODEV_System_h
	cgThreadList < logThread;
	if (CreateODev_VG3D() == nullptr)
		logThread.Disable();
#endif

	cgArrayOut = &cgTxSBUF.GetArray();
	cgArrayIn = &cgRxSBUF.GetArray();
	
	SetGetDataByShare();

	TNFP::SetSelfName("COMMU_THREAD");
	SetSelfName(selfName);
	SetFatherName("");
	ELogA(this);
};
//------------------------------------------------------------------------------------------//
COMMU_THREAD::~COMMU_THREAD(void){
	Close();
	rxThread.ThreadStop();
	txThread.ThreadStop();
	logThread.ThreadStop();
	logThread.RemoveSelf();
	rxThread.RemoveSelf();
	txThread.RemoveSelf();
	ELogA(this);
};
//------------------------------------------------------------------------------------------//
void COMMU_THREAD::EnableLog(void){
	logThread.Enable();
};
//------------------------------------------------------------------------------------------//
void COMMU_THREAD::DisableLog(void){
	logThread.Disable();
};
//------------------------------------------------------------------------------------------//
bool32 COMMU_THREAD::RxThreadFun(void* commu){
	uint32	length,offset,num;
	bool32	err;
	
	ETLogThreadStart(rxThread);
	while(rxThread.IsTerminated() == 0){
		if (IsConnected()){
			offset = 0;
			length = 0;

			cgArrayIn->CalcInCapacity(length, offset);
			err = ReadFromDevice(&num,cgArrayIn->GetPointer(offset),length);
			cgArrayIn->In(num);
			if ((err > 0) && (num > 0)){
				cgRxBytes += num;
				AfterReadFromDevice();
				continue;
			}
			if (err == -1){
				ELog0(this << "RxThreadFun()::OS issue,Call DoSelfClose()");
				DoSelfClose();
				break;
			}
		}
		SYS_SleepMS(2);
	}
	ETLogThreadStop(rxThread);
	return G_TRUE;
}
//------------------------------------------------------------------------------------------//
bool32 COMMU_THREAD::TxThreadFun(void* commu){
	bool32	err;
	uint32	length,offset,num;
	
	ETLogThreadStart(txThread);
	while(txThread.IsTerminated() == 0){
		if (ChkblPushSend() || (IsConnected() && (cgArrayOut->Used() > 0))){
			do{
				offset = 0;
				cgArrayOut->CalcOutCapacity(length,offset);
				err = SendToDevice(&num,cgArrayOut->GetPointer(offset),length);
				cgArrayOut->Out(num);
				if (err == -1)
					break;
				if ((err > 0) && (num > 0))
					cgTxBytes += num;
			}while((length > 0) && IsConnected());
			ClrblPushSend();
			if (err == -1)
				break;
		}
		SYS_SleepMS(2);
	}
	ETLogThreadStop(txThread);
	return G_TRUE;
}
//------------------------------------------------------------------------------------------//
bool32 COMMU_THREAD::LogThreadFun(void* commu){
#ifdef ODEV_System_h
	int32	byteNum;
	uint64	envcfg = 0;
	STDSTR	strPrintdata = "";
	
	cgLogSBUF.InitSize(PACKAGE_MAX_SIZE);
	cgInternalFwSBufList.AddNode(&cgLogSBUF);
	ETLogThreadStart(logThread);
	while(logThread.IsTerminated() == 0){
		if (cgLogSBUF.Used() > 0){
			byteNum = cgLogSBUF.Used();
			strPrintdata = "";
			
			if (GetLogSystem() != nullptr)
				envcfg = GetLogSystem()->envcfg;
			if (B_ChkFLAG64(envcfg,ODEV_FLAG_EnHEXViewMode)){
				cgLogSBUF.Get(nullptr,OUD_HEXs(&strPrintdata),cgLogSBUF.Used());
				if (B_ChkFLAG64(envcfg,ODEV_FLAG_EnOSPMsgLine))
					strPrintdata = Str_Replace(strPrintdata," B0 B3 "," B0 B3\n");
			}
			else if (B_ChkFLAG64(envcfg, ODEV_FLAG_EnEscape)){
				cgLogSBUF.GetArray().GetUnEscape(&strPrintdata, cgLogSBUF.Used());
				
			}
			else{
				cgLogSBUF.Get(nullptr,&strPrintdata, cgLogSBUF.Used());
			}
			PrintRecData(strPrintdata,byteNum,B_ChkFLAG64(envcfg,ODEV_FLAG_EnRecMsg));
		}
		SYS_SleepMS(10);
	}
	ETLogThreadStop(logThread);
#endif
	cgLogSBUF.RemoveSelf();
	return G_TRUE;
}
//------------------------------------------------------------------------------------------//
void COMMU_THREAD::PrintRecData(const STDSTR& strData, uint32 byteNum, bool32 blEnRecMsg){
#ifdef ODEV_System_h
	STDSTR		strSelfName,strCoupleName;
	int32		blCon;
	COMMU_THREAD *cpNode;
	
	if (GetVG3D() != nullptr){
		strSelfName = GetDevName();
		strCoupleName = "";
		blCon = 0;
		
		InUse_set();
		cpNode = (COMMU_THREAD*)GetCoupleNode(this);
		if (cpNode != nullptr){
			strCoupleName = cpNode->GetDevName();
			blCon = cpNode->IsConnected();
		}
		InUse_clr();
		
		*GetVG3D() << Begin();
		if (CheckEcho()){
			*GetVG3D() << NL() << COL_DivLineTime << COL_DB_RxTitle << " " << MakeEchoTitle(strSelfName, byteNum, FwBytes()) << COL_DB_RxText;
			if (blCon != 0){
				if (GetCNType(this) == CN_M)
					*GetVG3D() << NL() << COL_DB_RxTitleM << " " << MakeTxTitle("", strCoupleName, byteNum, RxBytes()) << COL_DB_RxTextM;
				if (GetCNType(this) == CN_S)
					*GetVG3D() << NL() << COL_DB_RxTitleS << " " << MakeRxTitle(strCoupleName, "", byteNum, RxBytes()) << COL_DB_RxTextS;
			}
		}
		else if (GetCNType(this) == CN_None){
			if ((GetOpenPar().cfg == OPEN_TCPS) || (GetOpenPar().cfg == OPEN_UDPS)){
				*GetVG3D() << NL() << COL_DivLineTime << COL_DB_RxTitle << " " << MakeRxTitle("", strSelfName, byteNum, RxBytes()) << COL_DB_RxText;
			}
			else{
				if (blEnRecMsg != 0)
					*GetVG3D() << NL() << COL_DivLineTime << COL_DB_RxTitle << " " << MakeRxTitle(byteNum, RxBytes());
				*GetVG3D() << COL_DB_RxText;
			}
		}
		else if (GetCNType(this) == CN_M){
			if (blCon == 0){
				if ((GetOpenPar().cfg == OPEN_TCPS) || (GetOpenPar().cfg == OPEN_UDPS)){
					*GetVG3D() << NL() << COL_DivLineTime << COL_DB_RxTitle << " " << MakeRxTitle("", strSelfName, byteNum, RxBytes()) << COL_DB_RxText;
				}
				else{
					if (blEnRecMsg != 0)
						*GetVG3D() << NL() << COL_DivLineTime << COL_DB_RxTitle << " " << MakeRxTitle(byteNum, RxBytes());
					*GetVG3D() << COL_DB_RxText;
				}
			}
			else{
				*GetVG3D() << NL() << COL_DivLineTime << COL_DB_RxTitleM << " " << MakeTxTitle(strSelfName, strCoupleName, byteNum, RxBytes()) << COL_DB_RxTextM;
			}
		}
		else if (GetCNType(this) == CN_S){
			if (blCon == 0){
				*GetVG3D() << NL() << COL_DivLineTime << COL_DB_RxTitle << " " << MakeRxTitle("", strSelfName, byteNum, RxBytes()) << COL_DB_RxText;
			}
			else{
				*GetVG3D() << NL() << COL_DivLineTime << COL_DB_RxTitleS << " " << MakeRxTitle(strSelfName, strCoupleName, byteNum, RxBytes()) << COL_DB_RxTextS;
			}
		}
		if ((GetOpenPar().cfg == OPEN_TCPS) || (GetOpenPar().cfg == OPEN_UDPS)){
			if (CheckSelected() == 0)
				*GetVG3D() << ClrGroup(COLRECORD::CRD_G1);
		}
		*GetVG3D() << strData << Endl();
	}
#endif
}
//------------------------------------------------------------------------------------------//










#ifdef ADS_SSL_h
//------------------------------------------------------------------------------------------//
COMMU_SSL::COMMU_SSL(uint32 size,const ODEV_SYSTEM* logSys) :
COMMU_THREAD(size,logSys),cgMsgHandshake(size,(void*)logSys)
{
	cgMsgCenter.Init(&cgTxArray,&cgRxArray,&cgRxSBUF,&cgTxSBUF);
	cgMsgCenter.Init(&cgMSGENV,&cgThreadList);
	cgMsgCenter.TxBlock(G_FALSE);
	cgMsgCenter.RxBlock(G_FALSE);
	cgMsgCenter.SSLDisable();
	cgMsgCenter.Register(&cgMsgHandshake);

	TNFP::SetSelfName("COMMU_SSL");
	SetSelfName(selfName);
	cgMsgCenter.SetSelfName("SSL");
	cgMsgHandshake.SetSelfName("HS");
	SetFatherName("");
	ELogA(this);
};
//------------------------------------------------------------------------------------------//
void COMMU_SSL::Empty(void){
	COMMU_THREAD::Empty();
	cgTxArray.Empty();
	cgRxArray.Empty();
	cgMsgCenter.Empty();
};
//------------------------------------------------------------------------------------------//
bool32 COMMU_SSL::SelfCloseThreadFun(void* commu){
	cgMSGENV.SetblExit();
	return(COMMU_THREAD::SelfCloseThreadFun(commu));
};
//------------------------------------------------------------------------------------------//
bool32 COMMU_SSL::DoOpen(const OPEN_PAR& par){
	ELog0(this << "COMMU_SSL::DoOpen()");
	CMD_NODE::ClrblExit(&cgMSGENV);
	if (B_ChkFLAG32(par.cfg,CFG_blEnableSSL)){
		cgArrayOut = &cgTxArray;
		cgArrayIn = &cgRxArray;
		cgTxArray.InitSize(cgMaxSize);
		cgRxArray.InitSize(cgMaxSize);
		cgMsgCenter.Reset(this);
		cgMsgCenter.SSLEnable();
	}
	else{
		cgMsgCenter.SSLDisable();
		cgArrayOut = &cgTxSBUF.GetArray();
		cgArrayIn = &cgRxSBUF.GetArray();
	}
	return(COMMU_THREAD::DoOpen(par));
};
//------------------------------------------------------------------------------------------//
bool32 COMMU_SSL::ThreadsStart(void){
	ELog0(this << "COMMU_SSL::ThreadsStart()");
	
	if (COMMU_THREAD::ThreadsStart() == G_FALSE)
		return G_FALSE;
	if (B_ChkFLAG32(cgOpenPar.cfg,CFG_blEnableSSL) == G_FALSE)
		return G_TRUE;
	if (GetOpenPar().type == OPEN_UDPS)
		return G_TRUE;
	if (GetOpenPar().type != OPEN_TCPS){
		SYS_DelayMS(1);
		if (cgMsgHandshake.Send_REQ_Handshake() == G_FALSE){
			DoSelfClose();
			return G_FALSE;
		}
	}
	if (cgMsgHandshake.HandshakeCheck(&cgMSGENV))
		return G_TRUE;
	return G_FALSE;
}
//------------------------------------------------------------------------------------------//
void COMMU_SSL::DoClose(void){
	ELog0(this << "COMMU_SSL::DoClose()");
	ClrSFlag(CF_blEnableSelfClose);
	if (B_ChkFLAG32(cgOpenPar.cfg,CFG_blEnableSSL)){
		ELog0(this << "COMMU_SSL::IsConnected() = "			<< IsConnected());
		ELog0(this << "COMMU_SSL::CDF_blCloseDueToOS = "	<< CheckSFlag(CF_blCloseDueToOS));
		ELog0(this << "COMMU_SSL::HandshakeCheck() = "		<< cgMsgCenter.ChkblREQHS());
		ELog0(this << "COMMU_SSL::IsREQClose() = "			<< cgMsgCenter.ChkblREQClose());
		if (IsConnected() && (CheckSFlag(CF_blCloseDueToOS) == G_FALSE)
			&& cgMsgCenter.ChkblREQHS()
			&& (cgMsgCenter.ChkblREQClose() == G_FALSE)){
			ELog0(this << "COMMU_SSL::DoClose()::Call SendREQ_Close()");
			cgMsgHandshake.Send_REQ_Close(&cgMSGENV);
		}
		cgMSGENV.SetblExit();
		cgMsgCenter.Close(this);
	}
	COMMU_THREAD::DoClose();
};
//------------------------------------------------------------------------------------------//
void COMMU_SSL::AfterReadFromDevice(void){
	if (B_ChkFLAG32(cgOpenPar.cfg,CFG_blEnableSSL) == 0){
		COMMU_THREAD::AfterReadFromDevice();
	}
	else{
		cgMsgCenter.Delivery();
	}
};
//------------------------------------------------------------------------------------------//
#endif



















//------------------------------------------------------------------------------------------//
static const STDSTR& MakeByteNum(STDSTR* retStr,uint32 ByteNum, uint64 total, bool32 blCR){
	STDSTR	strNum;
	
	strNum = Str_ToStr(ByteNum);
	strNum += '/';
	strNum += Str_ToStr(total);
	
	if (strNum.length() < 13)
		retStr->insert(retStr->length(),13 - strNum.length(),' ');

	retStr->append(strNum);
	retStr->append(" Bytes: ");

	if (blCR != G_FALSE)
		retStr->append("\n");
	return(*retStr);
}
//------------------------------------------------------------------------------------------//
static inline const STDSTR& MakeTitle(STDSTR* retStr,const STDSTR& strTitle, uint32 ByteNum, uint64 total, bool32 blCR){
	retStr->append(strTitle);
	return(MakeByteNum(retStr,ByteNum, total, blCR));
}
//------------------------------------------------------------------------------------------//
STDSTR MakeTxTitle(uint32 ByteNum, uint64 total, bool32 blCR){
	STDSTR	strRet;
	return(MakeTitle(&strRet,"Send    ", ByteNum, total,blCR));
};
//------------------------------------------------------------------------------------------//
STDSTR MakeRxTitle(uint32 ByteNum, uint64 total, bool32 blCR){
	STDSTR	strRet;
	return(MakeTitle(&strRet,"Received", ByteNum, total,blCR));
};
//------------------------------------------------------------------------------------------//
STDSTR MakeEchoTitle(uint32 ByteNum, uint64 total, bool32 blCR){
	STDSTR	strRet;
	return(MakeTitle(&strRet,"Echo    ", ByteNum, total,blCR));
};
//------------------------------------------------------------------------------------------//
STDSTR MakeTxTitle(const STDSTR& strM, const STDSTR& strS, uint32 ByteNum, uint64 total, bool32 blCR){
	STDSTR	strRet;
	
	if (strM.length() == 0){
		strRet = "SDT ->> ";
		strRet += strS;
	}
	else if (strS.length() == 0){
		strRet = strM;
		strRet += " ->> SDT";
	}
	else{
		strRet = strM;
		strRet += " ->> ";
		strRet += strS;
	}
	return(MakeByteNum(&strRet,ByteNum, total,blCR));
}
//------------------------------------------------------------------------------------------//
STDSTR MakeRxTitle(const STDSTR& strM, const STDSTR& strS, uint32 ByteNum, uint64 total, bool32 blCR){
	STDSTR	strRet;
	
	if (strM.length() == 0){
		strRet = "SDT <<- ";
		strRet += strS;
	}
	else if (strS.length() == 0){
		strRet = strM;
		strRet += " <<- SDT";
	}
	else{
		strRet = strM;
		strRet += " <<- ";
		strRet += strS;
	}
	return(MakeByteNum(&strRet,ByteNum, total,blCR));
}
//------------------------------------------------------------------------------------------//
STDSTR MakeEchoTitle(const STDSTR& strM, uint32 ByteNum, uint64 total, bool32 blCR){
	STDSTR	strSendData;
	
	strSendData = "SDT <-> ";
	strSendData += strM;
	return(MakeByteNum(&strSendData,ByteNum, total,blCR));
}
//------------------------------------------------------------------------------------------//
#endif /* Commu_DBuf_h */
