////
//  Commu_DBuf.cpp
//  SDT
//
//  Created by Leif Wen on 02/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "Commu_Base.h"
//------------------------------------------------------------------------------------------//
#include "DS_STRING.h"
#include "ADS_SMC.h"
#include "SYS_Time.h"
#ifdef Commu_Base_h
//------------------------------------------------------------------------------------------//
//#define LOGPRINT_ENABLE
//#define LOGTHREAD_ENABLE
#include "SYS_Log.h"
#include "DS_STRING.h"
//------------------------------------------------------------------------------------------//
#define LOG0_ENABLE

#ifdef LOG0_ENABLE
	#define ELog0(x) ELog(x);
#else
	#define ELog0(x);
#endif
//------------------------------------------------------------------------------------------//
COMMU_NODE::COMMU_NODE(void) : TNFP(){
	cgCType = CT_None;
	cgCouple = nullptr;
	cgSelCommu = this;
	
	TNFP::SetSelfName("COMMU_NODE");
	SetSelfName(selfName);
	SetUpName("");
};
//------------------------------------------------------------------------------------------//
void COMMU_NODE::LinkCouple_nolock(COMMU_NODE* master,COMMU_NODE* slave){
	if ((slave == nullptr) || (slave == master) || (master == nullptr))
		return;
	slave->cgCouple = master;
	slave->cgCType = CT_S;
	master->cgCouple = slave;
	master->cgCType = CT_M;
};
//------------------------------------------------------------------------------------------//
void COMMU_NODE::LinkCouple(COMMU_NODE* master,COMMU_NODE* slave){
	if ((slave == nullptr) || (slave == master) || (master == nullptr))
		return;
	
	do{
		master->cgCLock.W_set();
		if (master->cgCouple != nullptr){
			if (master->cgCouple->cgCLock.W_try() == G_FALSE){
				master->cgCLock.W_clr();
				continue;
			}
			master->cgCouple->cgCouple = nullptr;
			master->cgCouple->cgCType = CT_None;
			master->cgCouple->cgCLock.W_clr();
			master->cgCouple = nullptr;
			master->cgCType = CT_None;
		}
		if (slave->cgCLock.W_try() == G_FALSE){
			master->cgCLock.W_clr();
			continue;
		}
		if (slave->cgCouple != nullptr){
			if (slave->cgCouple->cgCLock.W_try() == G_FALSE){
				slave->cgCLock.W_clr();
				master->cgCLock.W_clr();
				continue;
			}
			slave->cgCouple->cgCouple = nullptr;
			slave->cgCouple->cgCType = CT_None;
			slave->cgCouple->cgCLock.W_clr();
		}
		slave->cgCouple = master;
		slave->cgCType = CT_S;
		master->cgCouple = slave;
		master->cgCType = CT_M;
		
		slave->cgCLock.W_clr();
		master->cgCLock.W_clr();
		break;
	}while(1);
};
//------------------------------------------------------------------------------------------//
COMMU_NODE* COMMU_NODE::UnlinkCouple(COMMU_NODE* commu){
	COMMU_NODE *cpNode = nullptr;
	do{
		commu->cgCLock.W_set();
		if (commu->cgCouple != nullptr){
			if (commu->cgCouple->cgCLock.W_try() == G_FALSE){
				commu->cgCLock.W_clr();
				continue;
			}
			cpNode = commu->cgCouple;
			commu->cgCouple->cgCouple = nullptr;
			commu->cgCouple->cgCType = CT_None;
			commu->cgCouple->cgCLock.W_clr();
			commu->cgCouple = nullptr;
			commu->cgCType = CT_None;
		}
		commu->cgCLock.W_clr();
		break;
	}while(1);
	return(cpNode);
};
//------------------------------------------------------------------------------------------//
void COMMU_NODE::SetChildSelected(COMMU_NODE* son){
	COMMU_NODE *cpNode;
	if ((son != nullptr) && (son != this)){
		if (IsSelected(son) == G_FALSE){
			son->cgCLock.W_set();
			do{
				cgCLock.W_set();
				if (cgSelCommu != son){
					if (cgSelCommu != this){
						if (cgSelCommu->cgCLock.W_try() == G_FALSE){
							cgCLock.W_clr();
							continue;
						}
					}
					cpNode = GetCouple(cgSelCommu);
					if (cpNode != nullptr){
						if (cpNode->cgCLock.W_try() == G_FALSE){
							cgCLock.W_clr();
							continue;
						}
						if (GetCType(cgSelCommu) == CT_M){
							LinkCouple_nolock(son,cpNode);
						}
						else{
							LinkCouple_nolock(cpNode,son);
						}
						cgSelCommu->cgCouple = nullptr;
						cgSelCommu->cgCType = CT_None;
						cpNode->cgCLock.W_clr();
					}
					if (cgSelCommu != this)
						cgSelCommu->ClrSFlag(CN_blSelected);
					cgSelCommu->cgCLock.W_clr((cgSelCommu != this)?G_LOCK_ON:G_LOCK_OFF);
					cgSelCommu = son;
					son->SetSFlag(CN_blSelected);
				}
				cgCLock.W_clr();
				break;
			}while(1);
			son->cgCLock.W_clr();
		}
	}
};
//------------------------------------------------------------------------------------------//
void COMMU_NODE::ClrChildSelected(COMMU_NODE* son){
	COMMU_NODE *cpNode;
	if ((son != nullptr) && (son != this)){
		if (IsSelected(son) != G_FALSE){
			do{
				son->cgCLock.W_set();
				if (cgCLock.W_try() == G_FALSE){
					son->cgCLock.W_clr();
					continue;
				}
				cpNode = GetCouple(son);
				if (cpNode != nullptr){
					if (cpNode->cgCLock.W_try() == G_FALSE){
						cgCLock.W_clr();
						son->cgCLock.W_clr();
						continue;
					}
					if (GetCType(son) == CT_M){
						LinkCouple_nolock(this,cpNode);
					}
					else{
						LinkCouple_nolock(this,cpNode);
					}
					son->cgCouple = nullptr;
					son->cgCType = CT_None;
					cpNode->cgCLock.W_clr();
				}
				cgSelCommu = this;
				son->ClrSFlag(CN_blSelected);
				cgCLock.W_clr();
				son->cgCLock.W_clr();
				break;
			}while(1);
		}
	}
};
//------------------------------------------------------------------------------------------//










#ifdef ODEV_System_h
//------------------------------------------------------------------------------------------//
class ODEV_COMMU : public OUTPUT_NODE{
	public:
				 ODEV_COMMU(COMMU_FRAME* commu,OUTPUT_CACHE* cache);
		virtual ~ODEV_COMMU(void){;};
	private:
		COMMU_FRAME*		cgCommu;
		STDSTR				cgText;
		SYS_TIME_S			cgTimeS;
	public:
		virtual 	void	Print		(uint32 ctrl,COLORENUM col,const uint8* data,uint32 num);
};
//------------------------------------------------------------------------------------------//
ODEV_COMMU::ODEV_COMMU(COMMU_FRAME* commu,OUTPUT_CACHE* cache) : OUTPUT_NODE(COLType_TXT,cache,COLRECORD::CRD_G2){
	SetSelfName("ODEV_COMMU");
	cgCommu = commu;
	cgText = "";
	SYS_Delay_SetTS(&cgTimeS, 100);
};
//------------------------------------------------------------------------------------------//
void ODEV_COMMU::Print(uint32 ctrl,COLORENUM col,const uint8* data,uint32 num){
	if (num == 0)
		return;
	
	ctrl &= COLRECORD::CRD_NL;
	if ((ctrl != 0) && (cgLastCR == 0))
		cgText.append("\n");
	cgText.append((char*)data, num);
	if ((cgText.length() > 1024 * 2) || (SYS_Delay_IsTimeout(&cgTimeS) != G_FALSE)){
		cgCommu->Send(cgText);
		cgText = "";
		SYS_Delay_SetTS(&cgTimeS, 100);
	}
};
//------------------------------------------------------------------------------------------//
#endif










//------------------------------------------------------------------------------------------//
COMMU_UNIT::COMMU_UNIT(void) : TNFP(){
	unitTeam = nullptr;
};
//------------------------------------------------------------------------------------------//
void COMMU_UNIT::Init(const COMMU_TEAM* _team){
	unitTeam = (COMMU_TEAM*)_team;
};
//------------------------------------------------------------------------------------------//
bool32 COMMU_UNIT::Start(void){
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
bool32 COMMU_UNIT::StartUnit(COMMU_UNIT* _unit){
	if (_unit != nullptr)
		return(_unit->Start());
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
void COMMU_UNIT::Stop(void){
};
//------------------------------------------------------------------------------------------//
void COMMU_UNIT::StopUnit(COMMU_UNIT* _unit){
	if (_unit != nullptr)
		_unit->Stop();
};
//------------------------------------------------------------------------------------------//











//------------------------------------------------------------------------------------------//
COMMU_LOGSYS::COMMU_LOGSYS(void){
#ifdef ODEV_System_h
	cgG2Self = nullptr;
	cgVG3D = nullptr;
	logSystem = nullptr;
#endif
	SetSelfName("LOGSYS");
	SetUpName("");
};
//------------------------------------------------------------------------------------------//
COMMU_LOGSYS::~COMMU_LOGSYS(void){
#ifdef ODEV_System_h
	if (cgVG3D != nullptr){
		cgVG3D->UnregisterToCache();
		cgVG3D->UnregisterAll();
		TNFP::DestroyTree(cgVG3D);
	}
	if (cgG2Self != nullptr)
		TNFP::DestroyTree(cgG2Self);
#endif
};
//------------------------------------------------------------------------------------------//
void COMMU_LOGSYS::InitSys(COMMU_LOGSYS* _sys,const ODEV_SYSTEM* logSys){
#ifdef ODEV_System_h
	if (_sys != nullptr){
		if (_sys->logSystem == nullptr){
			if (logSys != nullptr){
				_sys->logSystem = (ODEV_SYSTEM*)logSys;
				if (CreateODev_VG3D(_sys) != nullptr)
					_sys->cgVG3D->RegisterToCache(_sys->logSystem->cache);
			}
		}
	}
#endif
};
//------------------------------------------------------------------------------------------//
ODEV_VG3D* COMMU_LOGSYS::CreateODev_VG3D(COMMU_LOGSYS* _sys){
#ifdef ODEV_System_h
	if (_sys != nullptr){
		if ((_sys->logSystem != nullptr) && (_sys->cgVG3D == nullptr))
			_sys->cgVG3D = new ODEV_VG3D(_sys->logSystem->cache);
		return(_sys->cgVG3D);
	}
#endif
	return(nullptr);
};
//------------------------------------------------------------------------------------------//
ODEV_FILE* COMMU_LOGSYS::CreateODev_G3File(COMMU_LOGSYS* _sys,const STDSTR &fName){
#ifdef ODEV_System_h
	if (_sys != nullptr){
		if (_sys->cgVG3D != nullptr)
			_sys->cgVG3D->CreateG3D_FILE(fName,ODEV_FILE::ODEV_blRTF | ODEV_FILE::ODEV_blTXT);
		return(GetG3DefFile(_sys));
	}
#endif
	return(nullptr);
};
//------------------------------------------------------------------------------------------//
OUTPUT_NODE* COMMU_LOGSYS::CreateODev_G2Self(COMMU_LOGSYS* _sys,COMMU_FRAME* commu,OUTPUT_CACHE* cache){
#ifdef ODEV_System_h
	if (_sys != nullptr){
		_sys->cgG2Self = new ODEV_COMMU(commu,cache);
		return(_sys->cgG2Self);
	}
#endif
	return(nullptr);
};
//------------------------------------------------------------------------------------------//
OUTPUT_NODE* COMMU_LOGSYS::GetG2DefSelf(COMMU_LOGSYS* _sys){
#ifdef ODEV_System_h
	if (_sys != nullptr)
		return(_sys->cgG2Self);
#endif
	return(nullptr);
};
//------------------------------------------------------------------------------------------//
OUTPUT_NODE* COMMU_LOGSYS::GetG3DefSelf(COMMU_LOGSYS* _sys){
#ifdef ODEV_System_h
	if (_sys != nullptr)
		return((_sys->cgVG3D != nullptr) ? _sys->cgVG3D->GetDefODEV() : nullptr);
#endif
	return(nullptr);
};
//------------------------------------------------------------------------------------------//
ODEV_STDOUT* COMMU_LOGSYS::GetG3DefSTDOUT(COMMU_LOGSYS* _sys){
#ifdef ODEV_System_h
	if (_sys != nullptr)
		return((_sys->cgVG3D != nullptr) ? _sys->cgVG3D->GetDefSTDOUT() : nullptr);
#endif
	return(nullptr);
};
//------------------------------------------------------------------------------------------//
ODEV_FILE* COMMU_LOGSYS::GetG3DefFile(COMMU_LOGSYS* _sys){
#ifdef ODEV_System_h
	if (_sys != nullptr)
		return((_sys->cgVG3D != nullptr) ? _sys->cgVG3D->GetDefFile() : nullptr);
#endif
	return(nullptr);
};
//------------------------------------------------------------------------------------------//
OUTPUT_NODE* COMMU_LOGSYS::AddG3D_Self(COMMU_LOGSYS* _sys,OUTPUT_NODE *oG3D){
#ifdef ODEV_System_h
	if (_sys != nullptr)
		return((_sys->cgVG3D != nullptr) ? _sys->cgVG3D->AddG3D_ODEV(oG3D) : nullptr);
#endif
	return(nullptr);
};
//------------------------------------------------------------------------------------------//
ODEV_STDOUT* COMMU_LOGSYS::AddG3D_STDOUT(COMMU_LOGSYS* _sys,ODEV_STDOUT *oG3D){
#ifdef ODEV_System_h
	if (_sys != nullptr)
		return((_sys->cgVG3D != nullptr) ? _sys->cgVG3D->AddG3D_STDOUT(oG3D) : nullptr);
#endif
	return(nullptr);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
COMMU_CORE::COMMU_CORE(void) : COMMU_UNIT(){
	cgDevName = "";
	SetOpenPar(&cgOpenPar,OPEN_None,"",0,0);
	
	SetSelfName("CORE");
	SetUpName("");
};
//------------------------------------------------------------------------------------------//
COMMU_CORE::~COMMU_CORE(void){
	CloseDev();
};
//------------------------------------------------------------------------------------------//
STDSTR COMMU_CORE::GetCoreName(void)const{
	STDSTR	name;
	name = selfName;
	Str_ReadSubItem(&name, "_");
	return(name);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
COMMU_BRIDGE::COMMU_BRIDGE(void) : COMMU_UNIT(){
	
	rxThread.ThreadInit(this, &COMMU_BRIDGE::RxThreadFun, "rx");
	txThread.ThreadInit(this, &COMMU_BRIDGE::TxThreadFun, "tx");
	
	cgArrayRx = nullptr;
	cgArrayTx = nullptr;
	
	SetSelfName("BRIDGE");
	SetUpName("");
};
//------------------------------------------------------------------------------------------//
COMMU_BRIDGE::~COMMU_BRIDGE(void){
	rxThread.ThreadStop();
	txThread.ThreadStop();
	rxThread.RemoveSelf();
	txThread.RemoveSelf();
};
//------------------------------------------------------------------------------------------//
bool32 COMMU_BRIDGE::RxThreadFun(void* _team){
	COMMU_TEAM*	team = static_cast<COMMU_TEAM*>(_team);
	COMMU_CORE*	core = team->core;
	COMMU_MEM*	mem = team->mem;
	uint32		length,offset,num;
	bool32		err = 0;
	
	ETLogThreadStart(rxThread);
	if (core != nullptr){
		while(rxThread.IsTerminated() == G_FALSE){
			if (core->IsConnected()){
				offset = 0;
				length = 0;
				num = 0;
				cgArrayRx->CalcInCapacity(length, offset);
				err = core->ReadFromDevice(&num,cgArrayRx->GetPointer(offset),length);
				cgArrayRx->In(num);
				if ((err > 0) && (num > 0)){
					ELog0(ELogThread(rxThread,"Rec:" << num));
					cgRxBytes += num;
					mem->AfterReadFromDevice();
					continue;
				}
				if (err == -1){
					ELog0(ELogThread(rxThread,"OS issue,Call CloseSelf()"));
					SYS_SleepMS(5);
					team->commu->CloseSelf(0);
					break;
				}
			}
			if ((err == G_FALSE) || cgArrayRx->IsFull() || cgArrayRx->IsEmpty())
				SYS_SleepMS(2);
		}
	}
	ETLogThreadStop(rxThread);
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
bool32 COMMU_BRIDGE::TxThreadFun(void* _team){
	COMMU_TEAM*	team = static_cast<COMMU_TEAM*>(_team);
	COMMU_CORE*	core = team->core;
	uint32	length,offset,num;
	bool32	err = 0;
	
	ETLogThreadStart(txThread);
	if (core != nullptr){
		while(txThread.IsTerminated() == G_FALSE){
			if (CheckSFlag(CF_blPushSend) || (core->IsConnected() && (cgArrayTx->Used() > 0))){
				do{
					offset = 0;
					num = 0;
					cgArrayTx->CalcOutCapacity(length,offset);
					if (length > cgArrayTx->MaxSize() / 4)
						length = cgArrayTx->MaxSize() / 4 - 1;
					err = core->SendToDevice(&num,cgArrayTx->GetPointer(offset),length);
					cgArrayTx->Out(num);
					ELog0(ELogThread(txThread,"Out:" << num));
					cgTxBytes += num;
					if (err == G_FALSE)
						break;
				}while((length > 0) && core->IsConnected());
				ClrSFlag(CF_blPushSend);
			}
			SYS_SleepMS(2);
		}
	}
	ETLogThreadStop(txThread);
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
void COMMU_BRIDGE::PushSend(COMMU_BRIDGE* bridge,uint32 timeMS){
	if (bridge != nullptr){
		SYS_TIME_S		Timedly;
		SYS_Delay_SetTS(&Timedly, timeMS);
		bridge->SetSFlag(CF_blPushSend);
		while(bridge->CheckSFlag(CF_blPushSend) && (SYS_Delay_IsTimeout(&Timedly) == G_FALSE))
			SYS_DelayMS(2);
	}
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
COMMU_MEM::COMMU_MEM(void) : COMMU_UNIT(){
	
	SetSelfName("MEM");
	SetUpName("");
};
//------------------------------------------------------------------------------------------//
uint32 COMMU_MEM::Send(const ABORTCHECK& _ac,const UVIn&  _in){
	IOS		ios;
	uint32	num = 0;
	do{
		DS_IO_NODE::GetDSIOList().Save(IOS_clr(&ios), &arrayTx, _in);
		DS_IO_NODE::GetDSIOList().AddUVInOffset(_in, ios.total_in);
		num += ios.total_in;
	}while((ios.avail_in > 0) && (_ac.fun != nullptr) && (_ac.fun(_ac.p) == G_FALSE));
	
	return(num);
};
//------------------------------------------------------------------------------------------//
uint32 COMMU_MEM::Read(const ABORTCHECK& _ac,const UVOut& _out,uint32 num){
	IOS	ios;
	IOS_clr(&ios);
	do{
		if (arrayRx.Used() > 0){
			DS_IO_NODE::GetDSIOList().Save(&ios, _out, IUD(arrayRx,num,0));
			arrayRx.Out(num - (uint32)ios.avail_in);
			num = (uint32)ios.avail_in;
		}
	}while((ios.avail_in > 0) && (_ac.fun != nullptr) && (_ac.fun(_ac.p) == G_FALSE));
	return((uint32)ios.total_out);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
COMMU_FORWARD::COMMU_FORWARD(void) : COMMU_UNIT(){

	fwThread.ThreadInit(this, &COMMU_FORWARD::FwThreadFun,"fw");
	
	cgFWList = this;
	cgFwBytes = 0;
	
	SetSelfName("FORWARD");
	SetUpName("");
};
//------------------------------------------------------------------------------------------//
COMMU_FORWARD::~COMMU_FORWARD(void){
	fwThread.ThreadStop();
	fwThread.RemoveSelf();
};
//------------------------------------------------------------------------------------------//
static uint32 ForwardToCommu(COMMU_FRAME* commu,uint32* outNum,ARRAY* array,uint32 inNum,bool32 blInit){
	uint32 sendNum;
	
	sendNum = 0;
	if (commu != nullptr){
		COMMU_TEAM*		team = &commu->unitTeam;
		COMMU_MEM*		mem = team->mem;

		if (commu->IsOpened()){
			if (COMMU_MEM::GetArrayTx(mem)->alreadyReadNum > inNum){
				COMMU_MEM::GetArrayTx(mem)->alreadyReadNum = inNum;
			}
			else{
				sendNum = commu->Send(IUD(array, inNum, COMMU_MEM::GetArrayTx(mem)->alreadyReadNum));
				COMMU_MEM::GetArrayTx(mem)->alreadyReadNum += sendNum;
			}
			if ((blInit == G_FALSE) || (*outNum > COMMU_MEM::GetArrayTx(mem)->alreadyReadNum))
				*outNum = COMMU_MEM::GetArrayTx(mem)->alreadyReadNum;
			
			COMMU_MEM::GetArrayTx(mem)->alreadyReadNum -= *outNum;
		}
		else{
			if (mem != nullptr)
				COMMU_MEM::GetArrayTx(mem)->alreadyReadNum = 0;
		}
	}
	return(sendNum);
};
//------------------------------------------------------------------------------------------//
static void ForwardToSBUFList(TNFP* list,uint32* outNum,ARRAY* array,uint32 inNum,bool32 blInit){
	TREE_DownChain_Traversal_LINE
	(SBUF,list,
		if (_opNode->array.alreadyReadNum > inNum){
			_opNode->array.alreadyReadNum = inNum;
		}
		else{
			_opNode->array.alreadyReadNum += _opNode->array.Put(*array,inNum,_opNode->array.alreadyReadNum);
			//one in one out mode, no need lock
		}
		if ((blInit == G_FALSE) || (*outNum > _opNode->array.alreadyReadNum)){
			*outNum = _opNode->array.alreadyReadNum;
			blInit = G_TRUE;
		}
	);
};
//------------------------------------------------------------------------------------------//
bool32 COMMU_FORWARD::FwThreadFun(void* _team){
	COMMU_TEAM*		team = static_cast<COMMU_TEAM*>(_team);
	COMMU_MEM*		mem = team->mem;
	COMMU_FRAME*	commu = nullptr;
	uint32			recNum,outNum;
	bool32  		blInit;
	
	ETLogThreadStart(fwThread);
	rwlock.R_set();
	COMMU_MEM::GetArrayTx(mem)->alreadyReadNum = 0;
	{TREE_DownChain_Traversal_LINE(SBUF,this,_opNode->array.alreadyReadNum = 0;);};
	if ((cgFWList != nullptr) && (cgFWList != this)){
		TREE_DownChain_Traversal_LINE(SBUF,cgFWList,_opNode->array.alreadyReadNum = 0;);
	}
	rwlock.R_clr();
	
	while(fwThread.IsTerminated() == G_FALSE){
		SYS_SleepMS(2);
		if (COMMU_MEM::GetArrayRx(mem)->Used() > 0){
			blInit = G_FALSE;
			recNum = COMMU_MEM::GetArrayRx(mem)->Used();
			outNum = 0;
			if (team->commu->IsEnableEcho() == G_FALSE){
				COMMU_MEM::GetArrayTx(mem)->alreadyReadNum = 0;
			}
			else{
				cgFwBytes += ForwardToCommu(team->commu,&outNum,COMMU_MEM::GetArrayRx(mem), recNum, G_FALSE);
				COMMU_MEM::GetArrayTx(mem)->alreadyReadNum += outNum;
				outNum = COMMU_MEM::GetArrayTx(mem)->alreadyReadNum;
				blInit = G_TRUE;
			}
			rwlock.R_set();
			ForwardToSBUFList(this,&outNum,COMMU_MEM::GetArrayRx(mem),recNum,blInit);
			if ((cgFWList != nullptr) && (cgFWList != this))
				ForwardToSBUFList(cgFWList,&outNum,COMMU_MEM::GetArrayRx(mem),recNum,blInit);
			
			commu = (COMMU_FRAME*)COMMU_NODE::GetCouple(team->commu);
			if (commu == nullptr){
				ELog0(ELogThread(fwThread,"Forward:[nullprt]"));
			}
			else{
				ELog0(ELogThread(fwThread,"Forward:" << commu));
			}
			ForwardToCommu(commu, &outNum, COMMU_MEM::GetArrayRx(mem), recNum, blInit);
			
			if (team->commu->IsEnableEcho())
				COMMU_MEM::GetArrayTx(mem)->alreadyReadNum -= outNum;
			
			{TREE_DownChain_Traversal_LINE(SBUF,this,_opNode->array.alreadyReadNum -= outNum;);};
			
			if ((cgFWList != nullptr) && (cgFWList != this)){
				TREE_DownChain_Traversal_LINE(SBUF,cgFWList,_opNode->array.alreadyReadNum -= outNum;);
			}
			rwlock.R_clr();
			COMMU_MEM::GetArrayRx(mem)->Out(outNum);
		}
	}
	ETLogThreadStop(fwThread);
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
COMMU_RECORD::COMMU_RECORD(void) : COMMU_UNIT(){
	
	logThread.ThreadInit(this, &COMMU_RECORD::LogThreadFun,"log");
	
	SetSelfName("RECORD");
	SetUpName("");
};
//------------------------------------------------------------------------------------------//
COMMU_RECORD::~COMMU_RECORD(void){
	logThread.ThreadStop();
	logThread.RemoveSelf();
};
//------------------------------------------------------------------------------------------//
static void	PrintRecData(COMMU_TEAM* unit,const STDSTR& strData, uint32 byteNum, bool32 blEnRecMsg);
//------------------------------------------------------------------------------------------//
bool32 COMMU_RECORD::LogThreadFun(void* _team){
#ifdef ODEV_System_h
	COMMU_TEAM*	team = static_cast<COMMU_TEAM*>(_team);
	int32		byteNum;
	uint64		envcfg = 0;
	STDSTR		strPrintdata = "";
	
	ETLogThreadStart(logThread);
	if ((team != nullptr) && (team->forward != nullptr) && (team->logSys != nullptr)){
		cgLogSBUF.array.InitSize(LOGBUFFER_SIZE);
		team->forward->AppendDownNode(&cgLogSBUF);
		
		while(logThread.IsTerminated() == G_FALSE){
			if (cgLogSBUF.array.Used() > 0){
				byteNum = cgLogSBUF.array.Used();
				strPrintdata = "";
				
				if (COMMU_LOGSYS::GetLogSystem(team->logSys) != nullptr)
					envcfg = COMMU_LOGSYS::GetLogSystem(team->logSys)->envcfg;
				if (B_ChkFLAG64(envcfg,ODEV_FLAG_EnHEXViewMode)){
					cgLogSBUF.array.GetInHEX_S(&strPrintdata, cgLogSBUF.array.Used());
					if (B_ChkFLAG64(envcfg,ODEV_FLAG_EnOSPMsgLine))
						strPrintdata = Str_Replace(strPrintdata," B0 B3 "," B0 B3\n");
				}
				else if (B_ChkFLAG64(envcfg, ODEV_FLAG_EnEscape)){
					cgLogSBUF.array.GetInASCII(&strPrintdata, cgLogSBUF.array.Used());
				}
				else{
					cgLogSBUF.array.Get(&strPrintdata, cgLogSBUF.array.Used());
				}
				PrintRecData(team,strPrintdata,byteNum,B_ChkFLAG64(envcfg,ODEV_FLAG_EnRecMsg));
			}
			SYS_SleepMS(10);
		}
	}
	ETLogThreadStop(logThread);
#endif
	cgLogSBUF.RemoveSelf();
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
static void PrintRecData(COMMU_TEAM* team,const STDSTR& strData, uint32 byteNum, bool32 blEnRecMsg){
#ifdef ODEV_System_h
	OUTPUT_NODE	*node = COMMU_LOGSYS::GetVG3D(team->logSys);
	int32		ctype = COMMU_NODE::GetCType(team->commu);
	uint32		cfg = team->core->GetOpenPar().cfg;
	STDSTR		strSelfName = team->core->GetDevName();
	STDSTR		strCoupleName = "";
	COMMU_FRAME	*cpNode = nullptr;
	bool32		blCon = 0;
	
	if (node != nullptr){
		cpNode = (COMMU_FRAME*)COMMU_NODE::AcquireCouple(team->commu);
		if (cpNode != nullptr){
			strCoupleName = cpNode->unitTeam.core->GetDevName();
			blCon = cpNode->IsOpened();
			COMMU_NODE::ReleaseCouple(team->commu);
		}
		
		*node << Begin();
		if (team->commu->IsEnableEcho()){
			*node << NL() << COL_DivLineTime << COL_DB_RxTitle << " " << MakeEchoTitle(strSelfName, byteNum, team->commu->FwBytes()) << COL_DB_RxText;
			if (blCon != 0){
				if (ctype == COMMU_NODE::CT_M)
					*node << NL() << COL_DB_RxTitleM << " " << MakeTxTitle("", strCoupleName, byteNum, team->commu->RxBytes()) << COL_DB_RxTextM;
				if (ctype == COMMU_NODE::CT_S)
					*node << NL() << COL_DB_RxTitleS << " " << MakeRxTitle(strCoupleName, "", byteNum, team->commu->RxBytes()) << COL_DB_RxTextS;
			}
		}
		else if (ctype == COMMU_NODE::CT_None){
			if ((cfg == OPEN_TCPS) || (cfg == OPEN_UDPS)){
				*node << NL() << COL_DivLineTime << COL_DB_RxTitle << " " << MakeRxTitle("", strSelfName, byteNum, team->commu->RxBytes()) << COL_DB_RxText;
			}
			else{
				if (blEnRecMsg != 0)
					*node << NL() << COL_DivLineTime << COL_DB_RxTitle << " " << MakeRxTitle(byteNum, team->commu->RxBytes());
				*node << COL_DB_RxText;
			}
		}
		else if (ctype == COMMU_NODE::CT_M){
			if (blCon == 0){
				if ((cfg == OPEN_TCPS) || (cfg == OPEN_UDPS)){
					*node << NL() << COL_DivLineTime << COL_DB_RxTitle << " " << MakeRxTitle("", strSelfName, byteNum, team->commu->RxBytes()) << COL_DB_RxText;
				}
				else{
					if (blEnRecMsg != 0)
						*node << NL() << COL_DivLineTime << COL_DB_RxTitle << " " << MakeRxTitle(byteNum, team->commu->RxBytes());
					*node << COL_DB_RxText;
				}
			}
			else{
				*node << NL() << COL_DivLineTime << COL_DB_RxTitleM << " " << MakeTxTitle(strSelfName, strCoupleName, byteNum, team->commu->RxBytes()) << COL_DB_RxTextM;
			}
		}
		else if (ctype == COMMU_NODE::CT_S){
			if (blCon == 0){
				*node << NL() << COL_DivLineTime << COL_DB_RxTitle << " " << MakeRxTitle("", strSelfName, byteNum, team->commu->RxBytes()) << COL_DB_RxText;
			}
			else{
				*node << NL() << COL_DivLineTime << COL_DB_RxTitleS << " " << MakeRxTitle(strSelfName, strCoupleName, byteNum, team->commu->RxBytes()) << COL_DB_RxTextS;
			}
		}
		if ((cfg == OPEN_TCPS) || (cfg == OPEN_UDPS)){
			if (COMMU_NODE::IsSelected(team->commu) == G_FALSE)
				*node << ClrGroup(COLRECORD::CRD_G1);
		}
		*node << strData << Endl();
	}
#endif
};
//------------------------------------------------------------------------------------------//











//------------------------------------------------------------------------------------------//
COMMU_TEAM::COMMU_TEAM(void) : TNFP(){
	commu = nullptr;
	core = nullptr;
	bridge = nullptr;
	mem = nullptr;
	logSys = nullptr;
	forward = nullptr;
	record = nullptr;
};
//------------------------------------------------------------------------------------------//
COMMU_TEAM::~COMMU_TEAM(void){
	CleanDownTree(this, this);
	CleanTrash(this);
};
//------------------------------------------------------------------------------------------//
void COMMU_TEAM::Init(void){
	AppendDownNode(bridge);
	AppendDownNode(mem);
	AppendDownNode(forward);
	AppendDownNode(logSys);
	AppendDownNode(record);
	AppendDownNode(core);
	TREE_DownChain_Traversal_LINE(COMMU_UNIT, this, _opNode->Init(this));
};
//------------------------------------------------------------------------------------------//
void COMMU_TEAM::SetSelfName(const STDSTR& strName){
	TREE_DownChain_Traversal_LINE(COMMU_UNIT, this, _opNode->SetSelfName(strName));
};
//------------------------------------------------------------------------------------------//
void COMMU_TEAM::SetUpName(const STDSTR& strName){
	TREE_DownChain_Traversal_LINE(COMMU_UNIT, this, _opNode->SetUpName(strName));
};
//------------------------------------------------------------------------------------------//











//------------------------------------------------------------------------------------------//
COMMU_FRAME::COMMU_FRAME(void) : COMMU_FRAMEB(){
	selfCloseThread.ThreadInit(this, &COMMU_FRAME::SelfCloseThreadFun,"selfClose");
	
	unitTeam.commu = this;

	TNFP::SetSelfName("COMMU_FRAME");
	SetSelfName(selfName);
	SetUpName("");
	cgMaxRxArraySize = 0;
	cgMaxTxArraySize = 0;
};
//------------------------------------------------------------------------------------------//
COMMU_FRAME::COMMU_FRAME(uint32 rxSize,uint32 txSize,const ODEV_SYSTEM* logSys) : COMMU_FRAME(){
	Init(rxSize, txSize, logSys);
};
//------------------------------------------------------------------------------------------//
COMMU_FRAME::~COMMU_FRAME(void){
	UnlinkCouple(this);
	Close();
	selfCloseThread.ThreadStop();
};
//------------------------------------------------------------------------------------------/
void COMMU_FRAME::Init(uint32 rxSize,uint32 txSize,const ODEV_SYSTEM* logSys){
	cgMaxRxArraySize = rxSize;
	cgMaxTxArraySize = txSize;
	
	COMMU_LOGSYS::InitSys(unitTeam.logSys,logSys);
	COMMU_MEM::InitArray(unitTeam.mem,rxSize,txSize);
	
	unitTeam.Init();
	unitTeam.Init();
	unitTeam.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
void COMMU_FRAME::SetSelfName(const STDSTR& strName){
	selfName = strName;
	selfCloseThread.SetUpName(GetFullName(this));
	unitTeam.SetUpName(GetFullName(this));
	selfCloseThread.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
void COMMU_FRAME::SetUpName(const STDSTR& strName){
	fatherName = strName;
	selfCloseThread.SetUpName(GetFullName(this));
	unitTeam.SetUpName(GetFullName(this));
	selfCloseThread.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
void COMMU_FRAME::SetChildSelected(COMMU_NODE* commu){
	COMMU_FORWARD *forward;
	if ((commu != nullptr) && (commu != this)){
		COMMU_NODE* sel;
		sel = AcquireSelected(commu);
		if (sel != nullptr){
			if ((sel != this) && (sel != commu)){
				forward = static_cast<COMMU_FRAME*>(sel)->unitTeam.forward;
				COMMU_FORWARD::UseSelfFwList(forward);
			}
			forward = static_cast<COMMU_FRAME*>(commu)->unitTeam.forward;
			if (forward != nullptr)
				COMMU_FORWARD::UseExternalFwList(forward,COMMU_FORWARD::GetFWList(unitTeam.forward));
			ReleaseSelected(commu);
		}
		COMMU_NODE::SetChildSelected(commu);
	}
};
//------------------------------------------------------------------------------------------//
void COMMU_FRAME::ClrChildSelected(COMMU_NODE* commu){
	if ((commu != nullptr) && (commu != this)){
		COMMU_FORWARD *forward = static_cast<COMMU_FRAME*>(commu)->unitTeam.forward;
		if (forward != nullptr)
			COMMU_FORWARD::UseSelfFwList(forward);
	}
	COMMU_NODE::ClrChildSelected(commu);
};
//------------------------------------------------------------------------------------------//
void COMMU_FRAME::EnableEcho(void){
	uint32 cfg = unitTeam.core->GetOpenPar().cfg;
	B_SetFLAG32(cfg,CFG_blEnableEcho);
	unitTeam.core->SetCfgPar(cfg);
	TREE_DownChain_Traversal_LINE(COMMU_FRAME, this, _opNode->EnableEcho(););
};
//------------------------------------------------------------------------------------------//
void COMMU_FRAME::DisableEcho(void){
	uint32 cfg = unitTeam.core->GetOpenPar().cfg;
	B_ClrFLAG32(cfg,CFG_blEnableEcho);
	unitTeam.core->SetCfgPar(cfg);
	TREE_DownChain_Traversal_LINE(COMMU_FRAME, this, _opNode->DisableEcho(););
};
//------------------------------------------------------------------------------------------//
bool32 COMMU_FRAME::Open(const OPEN_PAR& par){
	
	ELog0(LogTitle(this,"COMMU_FRAME","Open","::") << par.name << "@" << par.port);
	
	bool32 err = G_FALSE;
	
	if (cgOpenLock.Try()){
		if (IsOpened())
			Close();
		err = DoOpen(par);
		if (err == G_FALSE){
			Close();
		}
		else{
			SetSFlag(CF_blOpened);
		}
		cgOpenLock.Clr();
	}
	
	ELog0(LogTitle(this,"COMMU_FRAME","Open","::") << "Result=" << err);
	return(err > 0 ? G_TRUE : G_FALSE);
};
//------------------------------------------------------------------------------------------//
bool32 COMMU_FRAME::DoOpen(const OPEN_PAR& par){
	bool32 err = G_FALSE;
	
	ELog0(LogTitle(this,"COMMU_FRAME","DoOpen","::") << "Call OpenDev()");
	
	unitTeam.core->SetDevName(par);//used for print

	if (unitTeam.core->OpenDev(par) != G_FALSE){
		ELog0(LogTitle(this,"COMMU_FRAME","DoOpen","::") << "unitTeam.core->OpenDev() = 1");
		unitTeam.Init();
		
		if (B_ChkFLAG32(par.cfg, CFG_blEnableEcho)){
			EnableEcho();
		}
		else{
			DisableEcho();
		}
		
		ClrSFlag(CF_blCloseDueToOS);
		SetSFlag(CF_blOpened | CF_blEnableSelfClose);

		ELog0(LogTitle(this,"COMMU_FRAME","DoOpen","::") << "Call ThreadsStart()");
		ThreadsStart();
		SYS_SleepMS(10);
		
		err = COMMU_UNIT::StartUnit(unitTeam.mem);
	}
	if (err == G_FALSE){
		unitTeam.core->PrintOpenFail();
	}
	else{
		unitTeam.core->PrintOpenSuccess();
	}
	return(err);
};
//------------------------------------------------------------------------------------------//
void COMMU_FRAME::Close(void){
	ELog0(LogTitle(this,"COMMU_FRAME","Close","::") << "Call DoClose()");
	
	cgOpenLock.Set();
	DoClose();
	ClrSFlag(CF_blOpened);
	cgOpenLock.Clr();

	ELog0(LogTitle(this,"COMMU_FRAME","Close","::") << "finished");
};
//------------------------------------------------------------------------------------------//
void COMMU_FRAME::DoClose(void){

	ClrSFlag(CF_blEnableSelfClose);
	
	COMMU_UNIT::StopUnit(unitTeam.mem);
	
	if (CheckSFlag(CF_blEnCloseCouple)){
		COMMU_FRAME	*couple;
		couple = static_cast<COMMU_FRAME*>(UnlinkCouple(this));
		if (couple != nullptr){
			ELog0(LogTitle(this,"COMMU_FRAME","DoClose","::") << "Call " << couple << "::Close()");
			couple->Close();
		}
	}
	
	if (IsOpened() != G_FALSE)
		COMMU_BRIDGE::PushSend(unitTeam.bridge,100);
	
	if (unitTeam.core != nullptr)
		unitTeam.core->CloseDev();
	
	ELog0(LogTitle(this,"COMMU_FRAME","DoClose","::") << "Call ThreadsStop()");
	cgThreadList.ThreadStop();
	
	ELog0(LogTitle(this,"COMMU_FRAME","DoClose","::") << "Call CloseAllChild()");
	CloseAllChild();
	
	if (IsOpened() != G_FALSE)
		unitTeam.core->PrintClose(unitTeam.commu->RxBytes(),unitTeam.commu->TxBytes(),unitTeam.commu->FwBytes());
	
	ELog0(LogTitle(this,"COMMU_FRAME","DoClose","::") << "finished");
};
//------------------------------------------------------------------------------------------//
void COMMU_FRAME::CloseAllChild(void){
	TREE_DownChain_Traversal_LINE
	(COMMU_FRAME, this,
	 	ELog0(LogTitle(this,"COMMU_FRAME","CloseAllChild","::") << _opNode);
	 	_opNode->Close();
	);
	ClrSelChild();
	CleanDownTree(this,this);
	SetblUpdate(this);
};
//------------------------------------------------------------------------------------------//
void COMMU_FRAME::CloseChild(COMMU_FRAME* commu){
	if (commu != nullptr){
		ELog0(LogTitle(this,"COMMU_FRAME","CloseChild","::") << commu);
		commu->Close();
		ClrChildSelected(commu);
		MoveNodesToTrash(this,commu,commu);
		SetblUpdate(this);
	}
};
//------------------------------------------------------------------------------------------//
bool32 COMMU_FRAME::SelfCloseThreadFun(void* commu){
	
	ETLogThreadStart(selfCloseThread);
	SYS_DelayMS(2);
	if (commu == nullptr){
		ELog0(ELogThread(selfCloseThread, "Call Close()"));
		Close();
	}
	else{
		ELog0(ELogThread(selfCloseThread, "Call" << static_cast<COMMU_FRAME*>(commu) << "::CloseChild()"));
		static_cast<COMMU_FRAME*>(commu)->CloseChild(this);
	}
	
	ETLogThreadStop(selfCloseThread);
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
bool32 COMMU_FRAME::CloseSelf(uint32 timeoutMS){
	if (CheckSFlag(CF_blEnableSelfClose)){
		ClrSFlag(CF_blEnableSelfClose);
		if (selfCloseThread.IsTerminated()){
			selfCloseThread.ThreadStop();
			SetSFlag(CF_blCloseDueToOS);
			ELog0(LogTitle(this,"COMMU_FRAME","CloseSelf","::") << "Call selfCloseThread.ThreadRun()");
			selfCloseThread.ThreadRun(GetFather(this));
		}
	}
	if (timeoutMS != 0){
		SYS_TIME_S	timeS;
		
		SYS_Delay_SetTS(&timeS,timeoutMS);
		while(SYS_Delay_IsTimeout(&timeS) == G_FALSE){
			SYS_SleepMS(2);
			if (IsOpened() == G_FALSE)
				return G_TRUE;
		};
		return G_FALSE;
	}
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
void COMMU_FRAME::DoThreadsStart(void){
	SMC_EncryptI(0)
	SMC_EncryptS(0)
	cgThreadList.ThreadRun(&unitTeam);
	SMC_EncryptE(0)
};
//------------------------------------------------------------------------------------------//
void COMMU_FRAME::ThreadsStart(void){
	DoThreadsStart();
};
//------------------------------------------------------------------------------------------//
bool32 CommuIsClosed(void* _team){
	COMMU_TEAM*	team = static_cast<COMMU_TEAM*>(_team);
	COMMU_CORE*	core = team->core;
	
	if (core->IsConnected() == G_FALSE)
		SYS_SleepMS(1);
	return(core->IsConnected() == G_FALSE);
};
//------------------------------------------------------------------------------------------//
uint32 COMMU_FRAME::Send(const UVIn&  _in){
	return(unitTeam.mem->Send(ACF(CommuIsClosed,&unitTeam),_in));
};
//------------------------------------------------------------------------------------------//
uint32 COMMU_FRAME::Read(const UVOut& _out,uint32 num){
	return(unitTeam.mem->Read(ACF(CommuIsClosed,&unitTeam),_out,num));
};
//------------------------------------------------------------------------------------------//
void COMMU_FRAME::Empty(void){
	COMMU_BRIDGE::Empty(unitTeam.bridge);
	COMMU_MEM::Empty(unitTeam.mem);
	COMMU_FORWARD::Empty(unitTeam.forward);
};
//------------------------------------------------------------------------------------------//
uint64 COMMU_FRAME::RxBytes(void){
	return(COMMU_BRIDGE::RxBytes(unitTeam.bridge));
};
//------------------------------------------------------------------------------------------//
uint64 COMMU_FRAME::TxBytes(void){
	return(COMMU_BRIDGE::TxBytes(unitTeam.bridge));
};
//------------------------------------------------------------------------------------------//
uint64 COMMU_FRAME::FwBytes(void){
	return(COMMU_FORWARD::FwBytes(unitTeam.forward));
};
//------------------------------------------------------------------------------------------//
uint32 COMMU_FRAME::RxMaxSize(void){
	return(COMMU_BRIDGE::RxMaxSize(unitTeam.bridge));
};
//------------------------------------------------------------------------------------------//
uint32 COMMU_FRAME::TxMaxSize(void){
	return(COMMU_BRIDGE::TxMaxSize(unitTeam.bridge));
};
//------------------------------------------------------------------------------------------//
uint32 COMMU_FRAME::UnreadBytes(void){
	return(COMMU_BRIDGE::UnreadBytes(unitTeam.bridge));
};
//------------------------------------------------------------------------------------------//
uint32 COMMU_FRAME::UnsentBytes(void){
	return(COMMU_BRIDGE::UnsentBytes(unitTeam.bridge));
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
COMMU_FRAME_NOSMC::COMMU_FRAME_NOSMC(uint32 rxSize,uint32 txSize,const ODEV_SYSTEM* logSys) : COMMU_FRAME_NOSMC(){
	Init(rxSize, txSize, logSys);
};
//------------------------------------------------------------------------------------------//
void COMMU_FRAME_NOSMC::DoThreadsStart(void){
	cgThreadList.ThreadRun(&unitTeam);
};
//------------------------------------------------------------------------------------------//










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
};
//------------------------------------------------------------------------------------------//
static inline const STDSTR& MakeTitle(STDSTR* retStr,const STDSTR& strTitle, uint32 ByteNum, uint64 total, bool32 blCR){
	retStr->append(strTitle);
	return(MakeByteNum(retStr,ByteNum, total, blCR));
};
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
};
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
};
//------------------------------------------------------------------------------------------//
STDSTR MakeEchoTitle(const STDSTR& strM, uint32 ByteNum, uint64 total, bool32 blCR){
	STDSTR	strSendData;
	
	strSendData = "SDT <-> ";
	strSendData += strM;
	return(MakeByteNum(&strSendData,ByteNum, total,blCR));
};
//------------------------------------------------------------------------------------------//
#endif /* Commu_Base_h */
