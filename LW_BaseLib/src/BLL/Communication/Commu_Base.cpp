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
DRV_NODE::DRV_NODE(void) : TNFP(){
	cgCType = CT_None;
	cgCouple = nullptr;
	cgSelDrv = this;
	
	TNFP::SetSelfName("DRV_NODE");
	SetSelfName(selfName);
	SetUpName("");
};
//------------------------------------------------------------------------------------------//
void DRV_NODE::LinkCouple_nolock(DRV_NODE* master,DRV_NODE* slave){
	if ((slave == nullptr) || (slave == master) || (master == nullptr))
		return;
	slave->cgCouple = master;
	slave->cgCType = CT_S;
	master->cgCouple = slave;
	master->cgCType = CT_M;
};
//------------------------------------------------------------------------------------------//
void DRV_NODE::LinkCouple(DRV_NODE* master,DRV_NODE* slave){
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
DRV_NODE* DRV_NODE::UnlinkCouple(DRV_NODE* cdrv){
	DRV_NODE *cpNode = nullptr;
	do{
		cdrv->cgCLock.W_set();
		if (cdrv->cgCouple != nullptr){
			if (cdrv->cgCouple->cgCLock.W_try() == G_FALSE){
				cdrv->cgCLock.W_clr();
				continue;
			}
			cpNode = cdrv->cgCouple;
			cdrv->cgCouple->cgCouple = nullptr;
			cdrv->cgCouple->cgCType = CT_None;
			cdrv->cgCouple->cgCLock.W_clr();
			cdrv->cgCouple = nullptr;
			cdrv->cgCType = CT_None;
		}
		cdrv->cgCLock.W_clr();
		break;
	}while(1);
	return(cpNode);
};
//------------------------------------------------------------------------------------------//
void DRV_NODE::SetChildSelected(DRV_NODE* son){
	DRV_NODE *cpNode;
	if ((son != nullptr) && (son != this)){
		if (IsSelected(son) == G_FALSE){
			son->cgCLock.W_set();
			do{
				cgCLock.W_set();
				if (cgSelDrv != son){
					if (cgSelDrv != this){
						if (cgSelDrv->cgCLock.W_try() == G_FALSE){
							cgCLock.W_clr();
							continue;
						}
					}
					cpNode = GetCouple(cgSelDrv);
					if (cpNode != nullptr){
						if (cpNode->cgCLock.W_try() == G_FALSE){
							cgCLock.W_clr();
							continue;
						}
						if (GetCType(cgSelDrv) == CT_M){
							LinkCouple_nolock(son,cpNode);
						}
						else{
							LinkCouple_nolock(cpNode,son);
						}
						cgSelDrv->cgCouple = nullptr;
						cgSelDrv->cgCType = CT_None;
						cpNode->cgCLock.W_clr();
					}
					if (cgSelDrv != this)
						cgSelDrv->ClrSFlag(CN_blSelected);
					cgSelDrv->cgCLock.W_clr((cgSelDrv != this)?G_LOCK_ON:G_LOCK_OFF);
					cgSelDrv = son;
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
void DRV_NODE::ClrChildSelected(DRV_NODE* son){
	DRV_NODE *cpNode;
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
				cgSelDrv = this;
				son->ClrSFlag(CN_blSelected);
				cgCLock.W_clr();
				son->cgCLock.W_clr();
				break;
			}while(1);
		}
	}
};
//------------------------------------------------------------------------------------------//










#ifdef ODEV_BUS_h
//------------------------------------------------------------------------------------------//
class ODEV_COMMU : public OUTPUT_NODE{
	public:
				 ODEV_COMMU(COMMU_DRV* cdrv,OUTPUT_CACHE* cache);
		virtual ~ODEV_COMMU(void){;};
	private:
		COMMU_DRV*			cgCommu;
		STDSTR				cgText;
		SYS_TIME_S			cgTimeS;
	public:
		virtual 	void	Print		(uint32 ctrl,COLORENUM col,const uint8* data,uint32 num);
};
//------------------------------------------------------------------------------------------//
ODEV_COMMU::ODEV_COMMU(COMMU_DRV* cdrv,OUTPUT_CACHE* cache) : OUTPUT_NODE(COLType_TXT,cache,COLRECORD::CRD_G2){
	SetSelfName("ODEV_COMMU");
	cgCommu = cdrv;
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
		cgCommu->Send(cgText,0);
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
COMMU_ODEVBUS::COMMU_ODEVBUS(void){
#ifdef ODEV_BUS_h
	cgG2Self = nullptr;
	cgVG3D = nullptr;
	cgODevBUS = nullptr;
#endif
	SetSelfName("LOGSYS");
	SetUpName("");
};
//------------------------------------------------------------------------------------------//
COMMU_ODEVBUS::~COMMU_ODEVBUS(void){
#ifdef ODEV_BUS_h
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
void COMMU_ODEVBUS::InitBus(COMMU_ODEVBUS* _bus,const ODEV_BUS* odevBus){
#ifdef ODEV_BUS_h
	if (_bus != nullptr){
		if (_bus->cgODevBUS == nullptr){
			if (odevBus != nullptr){
				_bus->cgODevBUS = (ODEV_BUS*)odevBus;
				if (CreateODev_VG3D(_bus) != nullptr)
					_bus->cgVG3D->RegisterToCache(_bus->cgODevBUS->cache);
			}
		}
	}
#endif
};
//------------------------------------------------------------------------------------------//
ODEV_VG3D* COMMU_ODEVBUS::CreateODev_VG3D(COMMU_ODEVBUS* _bus){
#ifdef ODEV_BUS_h
	if (_bus != nullptr){
		if ((_bus->cgODevBUS != nullptr) && (_bus->cgVG3D == nullptr))
			_bus->cgVG3D = new ODEV_VG3D(_bus->cgODevBUS->cache);
		return(_bus->cgVG3D);
	}
#endif
	return(nullptr);
};
//------------------------------------------------------------------------------------------//
ODEV_FILE* COMMU_ODEVBUS::CreateODev_G3File(COMMU_ODEVBUS* _bus,const STDSTR &fName){
#ifdef ODEV_BUS_h
	if (_bus != nullptr){
		if (_bus->cgVG3D != nullptr)
			_bus->cgVG3D->CreateG3D_FILE(fName,ODEV_FILE::ODEV_blRTF | ODEV_FILE::ODEV_blTXT);
		return(GetG3DefFile(_bus));
	}
#endif
	return(nullptr);
};
//------------------------------------------------------------------------------------------//
OUTPUT_NODE* COMMU_ODEVBUS::CreateODev_G2Self(COMMU_ODEVBUS* _bus,COMMU_DRV* cdrv,OUTPUT_CACHE* cache){
#ifdef ODEV_BUS_h
	if (_bus != nullptr){
		_bus->cgG2Self = new ODEV_COMMU(cdrv,cache);
		return(_bus->cgG2Self);
	}
#endif
	return(nullptr);
};
//------------------------------------------------------------------------------------------//
OUTPUT_NODE* COMMU_ODEVBUS::GetG2DefSelf(COMMU_ODEVBUS* _bus){
#ifdef ODEV_BUS_h
	if (_bus != nullptr)
		return(_bus->cgG2Self);
#endif
	return(nullptr);
};
//------------------------------------------------------------------------------------------//
OUTPUT_NODE* COMMU_ODEVBUS::GetG3DefSelf(COMMU_ODEVBUS* _bus){
#ifdef ODEV_BUS_h
	if (_bus != nullptr)
		return((_bus->cgVG3D != nullptr) ? _bus->cgVG3D->GetDefODEV() : nullptr);
#endif
	return(nullptr);
};
//------------------------------------------------------------------------------------------//
ODEV_STDOUT* COMMU_ODEVBUS::GetG3DefSTDOUT(COMMU_ODEVBUS* _bus){
#ifdef ODEV_BUS_h
	if (_bus != nullptr)
		return((_bus->cgVG3D != nullptr) ? _bus->cgVG3D->GetDefSTDOUT() : nullptr);
#endif
	return(nullptr);
};
//------------------------------------------------------------------------------------------//
ODEV_FILE* COMMU_ODEVBUS::GetG3DefFile(COMMU_ODEVBUS* _bus){
#ifdef ODEV_BUS_h
	if (_bus != nullptr)
		return((_bus->cgVG3D != nullptr) ? _bus->cgVG3D->GetDefFile() : nullptr);
#endif
	return(nullptr);
};
//------------------------------------------------------------------------------------------//
OUTPUT_NODE* COMMU_ODEVBUS::AddG3D_Self(COMMU_ODEVBUS* _bus,OUTPUT_NODE *oG3D){
#ifdef ODEV_BUS_h
	if (_bus != nullptr)
		return((_bus->cgVG3D != nullptr) ? _bus->cgVG3D->AddG3D_ODEV(oG3D) : nullptr);
#endif
	return(nullptr);
};
//------------------------------------------------------------------------------------------//
ODEV_STDOUT* COMMU_ODEVBUS::AddG3D_STDOUT(COMMU_ODEVBUS* _bus,ODEV_STDOUT *oG3D){
#ifdef ODEV_BUS_h
	if (_bus != nullptr)
		return((_bus->cgVG3D != nullptr) ? _bus->cgVG3D->AddG3D_STDOUT(oG3D) : nullptr);
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
	
	rxThread.ThreadInit(this, &COMMU_BRIDGE::RxThreadFunc, "rx");
	txThread.ThreadInit(this, &COMMU_BRIDGE::TxThreadFunc, "tx");
	
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
bool32 COMMU_BRIDGE::RxThreadFunc(void* _team){
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
					team->cdrv->CloseSelf(0);
					break;
				}
			}
			if ((err == G_FALSE) || cgArrayRx->IsFull() || cgArrayRx->IsEmpty())
				SYS_SleepMS(2);
		}
	}
	cgArrayRx->Empty();
	ETLogThreadStop(rxThread);
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
bool32 COMMU_BRIDGE::TxThreadFunc(void* _team){
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
	cgArrayTx->Empty();
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
uint32 COMMU_MEM::Send(const ABORTCHECK& _ac,const UVIn&  _in,uint32 timeoutMS){
	IOS		ios;
	uint64	num = 0;
	SYS_TIME_S	dtm;
	if (timeoutMS > 0)
		SYS_StopWatch_Start(&dtm);
	do{
		DS_IO_NODE::GetDSIOList().Save(IOS_clr(&ios), &arrayTx, _in);
		DS_IO_NODE::GetDSIOList().AddUVInOffset(_in, ios.total_in);
		num += ios.total_in;
		if (timeoutMS > 0){
			SYS_StopWatch_Stop(&dtm);
			if (dtm.timeMS > timeoutMS)
				break;
		}
	}while((ios.avail_in > 0) && (_ac.func != nullptr) && (_ac.func(_ac.p) == G_FALSE));
	
	return((uint32)num);
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
	}while((ios.avail_in > 0) && (_ac.func != nullptr) && (_ac.func(_ac.p) == G_FALSE));
	return((uint32)ios.total_out);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
COMMU_DISPOSE::COMMU_DISPOSE(void) : COMMU_UNIT(){

	dpThread.ThreadInit(this, &COMMU_DISPOSE::DPThreadFunc,"dp");
	
	cgDPList = this;
	cgDPBytes = 0;
	
	SetSelfName("DISPOSE");
	SetUpName("");
};
//------------------------------------------------------------------------------------------//
COMMU_DISPOSE::~COMMU_DISPOSE(void){
	dpThread.ThreadStop();
	dpThread.RemoveSelf();
};
//------------------------------------------------------------------------------------------//
static uint32 DisposeToCommu(COMMU_DRV* cdrv,uint32* outNum,ARRAY* array,uint32 inNum,bool32 blInit){
	uint32 sendNum;
	
	sendNum = 0;
	if (cdrv != nullptr){
		COMMU_TEAM*		team = &cdrv->unitTeam;
		COMMU_MEM*		mem = team->mem;

		if (cdrv->IsOpened()){
			if (COMMU_MEM::GetArrayTx(mem)->alreadyReadNum > inNum){
				COMMU_MEM::GetArrayTx(mem)->alreadyReadNum = inNum;
			}
			else{
				sendNum = cdrv->Send(IUD(array, inNum, COMMU_MEM::GetArrayTx(mem)->alreadyReadNum),0);
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
static void DisposeToSBUFList(TNFP* list,uint32* outNum,ARRAY* array,uint32 inNum,bool32 blInit){
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
bool32 COMMU_DISPOSE::DPThreadFunc(void* _team){
	COMMU_TEAM*		team = static_cast<COMMU_TEAM*>(_team);
	COMMU_MEM*		mem = team->mem;
	COMMU_DRV*		cdrv = nullptr;
	uint32			recNum,outNum;
	bool32  		blInit;
	
	ETLogThreadStart(dpThread);
	rwlock.R_set();
	COMMU_MEM::GetArrayTx(mem)->alreadyReadNum = 0;
	{TREE_DownChain_Traversal_LINE(SBUF,this,_opNode->array.alreadyReadNum = 0;);};
	if ((cgDPList != nullptr) && (cgDPList != this)){
		TREE_DownChain_Traversal_LINE(SBUF,cgDPList,_opNode->array.alreadyReadNum = 0;);
	}
	rwlock.R_clr();
	
	while(dpThread.IsTerminated() == G_FALSE){
		SYS_SleepMS(2);
		if (COMMU_MEM::GetArrayRx(mem)->Used() > 0){
			blInit = G_FALSE;
			recNum = COMMU_MEM::GetArrayRx(mem)->Used();
			outNum = 0;
			if (team->cdrv->IsEnableEcho() == G_FALSE){
				COMMU_MEM::GetArrayTx(mem)->alreadyReadNum = 0;
			}
			else{
				cgDPBytes += DisposeToCommu(team->cdrv,&outNum,COMMU_MEM::GetArrayRx(mem), recNum, G_FALSE);
				COMMU_MEM::GetArrayTx(mem)->alreadyReadNum += outNum;
				outNum = COMMU_MEM::GetArrayTx(mem)->alreadyReadNum;
				blInit = G_TRUE;
			}
			rwlock.R_set();
			DisposeToSBUFList(this,&outNum,COMMU_MEM::GetArrayRx(mem),recNum,blInit);
			if ((cgDPList != nullptr) && (cgDPList != this))
				DisposeToSBUFList(cgDPList,&outNum,COMMU_MEM::GetArrayRx(mem),recNum,blInit);
			
			cdrv = (COMMU_DRV*)DRV_NODE::GetCouple(team->cdrv);
			if (cdrv == nullptr){
				ELog0(ELogThread(dpThread,"Dispose:[nullptr]"));
			}
			else{
				ELog0(ELogThread(dpThread,"Dispose:" << cdrv));
			}
			DisposeToCommu(cdrv, &outNum, COMMU_MEM::GetArrayRx(mem), recNum, blInit);
			
			if (team->cdrv->IsEnableEcho())
				COMMU_MEM::GetArrayTx(mem)->alreadyReadNum -= outNum;
			
			{TREE_DownChain_Traversal_LINE(SBUF,this,_opNode->array.alreadyReadNum -= outNum;);};
			
			if ((cgDPList != nullptr) && (cgDPList != this)){
				TREE_DownChain_Traversal_LINE(SBUF,cgDPList,_opNode->array.alreadyReadNum -= outNum;);
			}
			rwlock.R_clr();
			COMMU_MEM::GetArrayRx(mem)->Out(outNum);
		}
	}
	ETLogThreadStop(dpThread);
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
COMMU_RECORD::COMMU_RECORD(void) : COMMU_UNIT(){
	
	recThread.ThreadInit(this, &COMMU_RECORD::RecThreadFunc,"log");
	
	SetSelfName("RECORD");
	SetUpName("");
};
//------------------------------------------------------------------------------------------//
COMMU_RECORD::~COMMU_RECORD(void){
	recThread.ThreadStop();
	recThread.RemoveSelf();
};
//------------------------------------------------------------------------------------------//
static void	PrintRecData(COMMU_TEAM* unit,const STDSTR& strData, uint32 byteNum, bool32 blEnRecMsg);
//------------------------------------------------------------------------------------------//
bool32 COMMU_RECORD::RecThreadFunc(void* _team){
#ifdef ODEV_BUS_h
	COMMU_TEAM*	team = static_cast<COMMU_TEAM*>(_team);
	int32		byteNum;
	uint64		envcfg = 0;
	STDSTR		strPrintdata = "";
	
	ETLogThreadStart(recThread);
	if ((team != nullptr) && (team->dispose != nullptr) && (team->odevBus != nullptr)){
		cgRecSBUF.array.InitSize(RECBUFFER_SIZE);
		team->dispose->AppendDownNode(&cgRecSBUF);
		
		while(recThread.IsTerminated() == G_FALSE){
			if (cgRecSBUF.array.Used() > 0){
				byteNum = cgRecSBUF.array.Used();
				strPrintdata = "";
				
				if (COMMU_ODEVBUS::GetODevBus(team->odevBus) != nullptr)
					envcfg = COMMU_ODEVBUS::GetODevBus(team->odevBus)->envcfg;
				if (B_ChkFLAG64(envcfg,ODEV_FLAG_EnHEXViewMode)){
					cgRecSBUF.array.GetInHEX_S(&strPrintdata, cgRecSBUF.array.Used());
					if (B_ChkFLAG64(envcfg,ODEV_FLAG_EnOSPMsgLine))
						strPrintdata = Str_Replace(strPrintdata," B0 B3 "," B0 B3\n");
				}
				else if (B_ChkFLAG64(envcfg, ODEV_FLAG_EnEscape)){
					cgRecSBUF.array.GetInASCII(&strPrintdata, cgRecSBUF.array.Used());
				}
				else{
					cgRecSBUF.array.Get(&strPrintdata, cgRecSBUF.array.Used());
				}
				PrintRecData(team,strPrintdata,byteNum,B_ChkFLAG64(envcfg,ODEV_FLAG_EnRecMsg));
			}
			SYS_SleepMS(10);
		}
	}
	ETLogThreadStop(recThread);
#endif
	cgRecSBUF.RemoveSelf();
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
static void PrintRecData(COMMU_TEAM* team,const STDSTR& strData, uint32 byteNum, bool32 blEnRecMsg){
#ifdef ODEV_BUS_h
	OUTPUT_NODE	*node = COMMU_ODEVBUS::GetVG3D(team->odevBus);
	int32		ctype = DRV_NODE::GetCType(team->cdrv);
	uint32		cfg = team->core->GetOpenPar().cfg;
	STDSTR		strSelfName = team->core->GetDevName();
	STDSTR		strCoupleName = "";
	COMMU_DRV	*cpNode = nullptr;
	bool32		blCon = 0;
	
	if (node != nullptr){
		cpNode = (COMMU_DRV*)DRV_NODE::AcquireCouple(team->cdrv);
		if (cpNode != nullptr){
			strCoupleName = cpNode->unitTeam.core->GetDevName();
			blCon = cpNode->IsOpened();
			DRV_NODE::ReleaseCouple(team->cdrv);
		}
		
		*node << Begin();
		if (team->cdrv->IsEnableEcho()){
			*node << NL() << COL_DivLineTime << COL_DB_RxTitle << " " << MakeEchoTitle(strSelfName, byteNum, team->cdrv->DPBytes()) << COL_DB_RxText;
			if (blCon != 0){
				if (ctype == DRV_NODE::CT_M)
					*node << NL() << COL_DB_RxTitleM << " " << MakeTxTitle("", strCoupleName, byteNum, team->cdrv->RxBytes()) << COL_DB_RxTextM;
				if (ctype == DRV_NODE::CT_S)
					*node << NL() << COL_DB_RxTitleS << " " << MakeRxTitle(strCoupleName, "", byteNum, team->cdrv->RxBytes()) << COL_DB_RxTextS;
			}
		}
		else if (ctype == DRV_NODE::CT_None){
			if ((cfg == OPEN_TCPS) || (cfg == OPEN_UDPS)){
				*node << NL() << COL_DivLineTime << COL_DB_RxTitle << " " << MakeRxTitle("", strSelfName, byteNum, team->cdrv->RxBytes()) << COL_DB_RxText;
			}
			else{
				if (blEnRecMsg != 0)
					*node << NL() << COL_DivLineTime << COL_DB_RxTitle << " " << MakeRxTitle(byteNum, team->cdrv->RxBytes());
				*node << COL_DB_RxText;
			}
		}
		else if (ctype == DRV_NODE::CT_M){
			if (blCon == 0){
				if ((cfg == OPEN_TCPS) || (cfg == OPEN_UDPS)){
					*node << NL() << COL_DivLineTime << COL_DB_RxTitle << " " << MakeRxTitle("", strSelfName, byteNum, team->cdrv->RxBytes()) << COL_DB_RxText;
				}
				else{
					if (blEnRecMsg != 0)
						*node << NL() << COL_DivLineTime << COL_DB_RxTitle << " " << MakeRxTitle(byteNum, team->cdrv->RxBytes());
					*node << COL_DB_RxText;
				}
			}
			else{
				*node << NL() << COL_DivLineTime << COL_DB_RxTitleM << " " << MakeTxTitle(strSelfName, strCoupleName, byteNum, team->cdrv->RxBytes()) << COL_DB_RxTextM;
			}
		}
		else if (ctype == DRV_NODE::CT_S){
			if (blCon == 0){
				*node << NL() << COL_DivLineTime << COL_DB_RxTitle << " " << MakeRxTitle("", strSelfName, byteNum, team->cdrv->RxBytes()) << COL_DB_RxText;
			}
			else{
				*node << NL() << COL_DivLineTime << COL_DB_RxTitleS << " " << MakeRxTitle(strSelfName, strCoupleName, byteNum, team->cdrv->RxBytes()) << COL_DB_RxTextS;
			}
		}
		if ((cfg == OPEN_TCPS) || (cfg == OPEN_UDPS)){
			if (DRV_NODE::IsSelected(team->cdrv) == G_FALSE)
				*node << ClrGroup(COLRECORD::CRD_G1);
		}
		*node << strData << Endl();
	}
#endif
};
//------------------------------------------------------------------------------------------//











//------------------------------------------------------------------------------------------//
COMMU_TEAM::COMMU_TEAM(void) : TNFP(){
	cdrv = nullptr;
	core = nullptr;
	bridge = nullptr;
	mem = nullptr;
	odevBus = nullptr;
	dispose = nullptr;
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
	AppendDownNode(dispose);
	AppendDownNode(odevBus);
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
COMMU_DRV::COMMU_DRV(void) : _COMMU_DRV(){
	selfCloseThread.ThreadInit(this, &COMMU_DRV::SelfCloseThreadFunc,"selfClose");
	
	unitTeam.cdrv = this;

	TNFP::SetSelfName("COMMU_DRV");
	SetSelfName(selfName);
	SetUpName("");
	cgMaxRxArraySize = 0;
	cgMaxTxArraySize = 0;
};
//------------------------------------------------------------------------------------------//
COMMU_DRV::COMMU_DRV(uint32 rxSize,uint32 txSize,const ODEV_BUS* odevBus) : COMMU_DRV(){
	Init(rxSize, txSize, odevBus);
};
//------------------------------------------------------------------------------------------//
COMMU_DRV::~COMMU_DRV(void){
	UnlinkCouple(this);
	Close();
	selfCloseThread.ThreadStop();
};
//------------------------------------------------------------------------------------------/
void COMMU_DRV::Init(uint32 rxSize,uint32 txSize,const ODEV_BUS* odevBus){
	cgMaxRxArraySize = rxSize;
	cgMaxTxArraySize = txSize;
	
	COMMU_ODEVBUS::InitBus(unitTeam.odevBus,odevBus);
	COMMU_MEM::InitArray(unitTeam.mem,rxSize,txSize);
	
	unitTeam.Init();
	unitTeam.Init();
	unitTeam.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
void COMMU_DRV::SetSelfName(const STDSTR& strName){
	selfName = strName;
	selfCloseThread.SetUpName(GetFullName(this));
	unitTeam.SetUpName(GetFullName(this));
	selfCloseThread.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
void COMMU_DRV::SetUpName(const STDSTR& strName){
	fatherName = strName;
	selfCloseThread.SetUpName(GetFullName(this));
	unitTeam.SetUpName(GetFullName(this));
	selfCloseThread.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
void COMMU_DRV::SetChildSelected(DRV_NODE* cdrv){
	COMMU_DISPOSE *dispose;
	if ((cdrv != nullptr) && (cdrv != this)){
		DRV_NODE* sel;
		sel = AcquireSelected(cdrv);
		if (sel != nullptr){
			if ((sel != this) && (sel != cdrv)){
				dispose = static_cast<COMMU_DRV*>(sel)->unitTeam.dispose;
				COMMU_DISPOSE::UseSelfDPList(dispose);
			}
			dispose = static_cast<COMMU_DRV*>(cdrv)->unitTeam.dispose;
			if (dispose != nullptr)
				COMMU_DISPOSE::UseExternalDPList(dispose,COMMU_DISPOSE::GetDPList(unitTeam.dispose));
			ReleaseSelected(cdrv);
		}
		DRV_NODE::SetChildSelected(cdrv);
	}
};
//------------------------------------------------------------------------------------------//
void COMMU_DRV::ClrChildSelected(DRV_NODE* cdrv){
	if ((cdrv != nullptr) && (cdrv != this)){
		COMMU_DISPOSE *dispose = static_cast<COMMU_DRV*>(cdrv)->unitTeam.dispose;
		if (dispose != nullptr)
			COMMU_DISPOSE::UseSelfDPList(dispose);
	}
	DRV_NODE::ClrChildSelected(cdrv);
};
//------------------------------------------------------------------------------------------//
void COMMU_DRV::EnableEcho(void){
	uint32 cfg = unitTeam.core->GetOpenPar().cfg;
	B_SetFLAG32(cfg,CFG_blEnableEcho);
	unitTeam.core->SetCfgPar(cfg);
	TREE_DownChain_Traversal_LINE(COMMU_DRV, this, _opNode->EnableEcho(););
};
//------------------------------------------------------------------------------------------//
void COMMU_DRV::DisableEcho(void){
	uint32 cfg = unitTeam.core->GetOpenPar().cfg;
	B_ClrFLAG32(cfg,CFG_blEnableEcho);
	unitTeam.core->SetCfgPar(cfg);
	TREE_DownChain_Traversal_LINE(COMMU_DRV, this, _opNode->DisableEcho(););
};
//------------------------------------------------------------------------------------------//
bool32 COMMU_DRV::Open(const OPEN_PAR& par){
	
	ELog0(LogTitle(this,"COMMU_DRV","Open","::") << par.name << "@" << par.port);
	
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
	
	ELog0(LogTitle(this,"COMMU_DRV","Open","::") << "Result=" << err);
	return(err > 0 ? G_TRUE : G_FALSE);
};
//------------------------------------------------------------------------------------------//
bool32 COMMU_DRV::DoOpen(const OPEN_PAR& par){
	bool32 err = G_FALSE;
	
	ELog0(LogTitle(this,"COMMU_DRV","DoOpen","::") << "Call OpenDev()");
	
	unitTeam.core->SetDevName(par);//used for print

	if (unitTeam.core->OpenDev(par) != G_FALSE){
		ELog0(LogTitle(this,"COMMU_DRV","DoOpen","::") << "unitTeam.core->OpenDev() = 1");
		unitTeam.Init();
		
		if (B_ChkFLAG32(par.cfg, CFG_blEnableEcho)){
			EnableEcho();
		}
		else{
			DisableEcho();
		}
		
		ClrSFlag(CF_blCloseDueToOS);
		SetSFlag(CF_blOpened | CF_blEnableSelfClose);

		ELog0(LogTitle(this,"COMMU_DRV","DoOpen","::") << "Call ThreadsStart()");
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
void COMMU_DRV::Close(void){
	ELog0(LogTitle(this,"COMMU_DRV","Close","::") << "Call DoClose()");
	
	cgOpenLock.Set();
	DoClose();
	ClrSFlag(CF_blOpened);
	cgOpenLock.Clr();

	ELog0(LogTitle(this,"COMMU_DRV","Close","::") << "finished");
};
//------------------------------------------------------------------------------------------//
void COMMU_DRV::DoClose(void){

	ClrSFlag(CF_blEnableSelfClose);
	
	COMMU_UNIT::StopUnit(unitTeam.mem);
	
	if (CheckSFlag(CF_blEnCloseCouple)){
		COMMU_DRV	*couple;
		couple = static_cast<COMMU_DRV*>(UnlinkCouple(this));
		if (couple != nullptr){
			ELog0(LogTitle(this,"COMMU_DRV","DoClose","::") << "Call " << couple << "::Close()");
			couple->Close();
		}
	}
	
	if (IsOpened() != G_FALSE)
		COMMU_BRIDGE::PushSend(unitTeam.bridge,100);
	
	if (unitTeam.core != nullptr)
		unitTeam.core->CloseDev();
	
	ELog0(LogTitle(this,"COMMU_DRV","DoClose","::") << "Call ThreadsStop()");
	cgThreadList.ThreadStop();
	
	ELog0(LogTitle(this,"COMMU_DRV","DoClose","::") << "Call CloseAllChild()");
	CloseAllChild();
	
	if (IsOpened() != G_FALSE)
		unitTeam.core->PrintClose(unitTeam.cdrv->RxBytes(),unitTeam.cdrv->TxBytes(),unitTeam.cdrv->DPBytes());
	
	ELog0(LogTitle(this,"COMMU_DRV","DoClose","::") << "finished");
};
//------------------------------------------------------------------------------------------//
void COMMU_DRV::CloseAllChild(void){
	TREE_DownChain_Traversal_LINE
	(COMMU_DRV, this,
	 	ELog0(LogTitle(this,"COMMU_DRV","CloseAllChild","::") << _opNode);
	 	_opNode->Close();
	);
	ClrSelChild();
	CleanDownTree(this,this);
	SetblUpdate(this);
};
//------------------------------------------------------------------------------------------//
void COMMU_DRV::CloseChild(COMMU_DRV* cdrv){
	if (cdrv != nullptr){
		ELog0(LogTitle(this,"COMMU_DRV","CloseChild","::") << cdrv);
		cdrv->Close();
		ClrChildSelected(cdrv);
		MoveNodesToTrash(this,cdrv,cdrv);
		SetblUpdate(this);
	}
};
//------------------------------------------------------------------------------------------//
bool32 COMMU_DRV::SelfCloseThreadFunc(void* cdrv){
	
	ETLogThreadStart(selfCloseThread);
	SYS_DelayMS(2);
	if (cdrv == nullptr){
		ELog0(ELogThread(selfCloseThread, "Call Close()"));
		Close();
	}
	else{
		ELog0(ELogThread(selfCloseThread, "Call" << static_cast<COMMU_DRV*>(cdrv) << "::CloseChild()"));
		static_cast<COMMU_DRV*>(cdrv)->CloseChild(this);
	}
	
	ETLogThreadStop(selfCloseThread);
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
bool32 COMMU_DRV::CloseSelf(uint32 timeoutMS){
	if (CheckSFlag(CF_blEnableSelfClose)){
		ClrSFlag(CF_blEnableSelfClose);
		if (selfCloseThread.IsTerminated()){
			selfCloseThread.ThreadStop();
			SetSFlag(CF_blCloseDueToOS);
			ELog0(LogTitle(this,"COMMU_DRV","CloseSelf","::") << "Call selfCloseThread.ThreadRun()");
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
void COMMU_DRV::DoThreadsStart(void){
	SMC_EncryptI(0)
	SMC_EncryptS(0)
	cgThreadList.ThreadRun(&unitTeam);
	SMC_EncryptE(0)
};
//------------------------------------------------------------------------------------------//
void COMMU_DRV::ThreadsStart(void){
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
uint32 COMMU_DRV::Send(const UVIn&  _in,uint32 timeoutMS){
	return(unitTeam.mem->Send(ACF(CommuIsClosed,&unitTeam),_in,timeoutMS));
};
//------------------------------------------------------------------------------------------//
uint32 COMMU_DRV::Read(const UVOut& _out,uint32 num){
	return(unitTeam.mem->Read(ACF(CommuIsClosed,&unitTeam),_out,num));
};
//------------------------------------------------------------------------------------------//
void COMMU_DRV::Empty(void){
	COMMU_BRIDGE::Empty(unitTeam.bridge);
	COMMU_MEM::Empty(unitTeam.mem);
	COMMU_DISPOSE::Empty(unitTeam.dispose);
};
//------------------------------------------------------------------------------------------//
uint64 COMMU_DRV::RxBytes(void){
	return(COMMU_BRIDGE::RxBytes(unitTeam.bridge));
};
//------------------------------------------------------------------------------------------//
uint64 COMMU_DRV::TxBytes(void){
	return(COMMU_BRIDGE::TxBytes(unitTeam.bridge));
};
//------------------------------------------------------------------------------------------//
uint64 COMMU_DRV::DPBytes(void){
	return(COMMU_DISPOSE::DPBytes(unitTeam.dispose));
};
//------------------------------------------------------------------------------------------//
uint32 COMMU_DRV::RxMaxSize(void){
	return(COMMU_BRIDGE::RxMaxSize(unitTeam.bridge));
};
//------------------------------------------------------------------------------------------//
uint32 COMMU_DRV::TxMaxSize(void){
	return(COMMU_BRIDGE::TxMaxSize(unitTeam.bridge));
};
//------------------------------------------------------------------------------------------//
uint32 COMMU_DRV::UnreadBytes(void){
	return(COMMU_BRIDGE::UnreadBytes(unitTeam.bridge));
};
//------------------------------------------------------------------------------------------//
uint32 COMMU_DRV::UnsentBytes(void){
	return(COMMU_BRIDGE::UnsentBytes(unitTeam.bridge));
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
COMMU_DRV_NOSMC::COMMU_DRV_NOSMC(uint32 rxSize,uint32 txSize,const ODEV_BUS* odevBus) : COMMU_DRV_NOSMC(){
	Init(rxSize, txSize, odevBus);
};
//------------------------------------------------------------------------------------------//
void COMMU_DRV_NOSMC::DoThreadsStart(void){
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
