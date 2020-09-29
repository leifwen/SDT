//
//  Commu_DBuf.hpp
//  SDT
//
//  Created by Leif Wen on 02/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#ifndef Commu_Base_hpp
#define Commu_Base_hpp
//------------------------------------------------------------------------------------------//
#include "Commu_Base.h"
#ifdef Commu_Base_h
//------------------------------------------------------------------------------------------//
inline CoupleType	DRV_NODE::GetCType			(const DRV_NODE* cdrv)	{return((cdrv == nullptr)?CT_None:cdrv->cgCType);};
inline DRV_NODE*	DRV_NODE::GetCouple			(const DRV_NODE* cdrv)	{return((cdrv == nullptr)?nullptr:cdrv->cgCouple);};
inline DRV_NODE*	DRV_NODE::GetFather			(const DRV_NODE* cdrv)	{return((cdrv == nullptr)?nullptr:static_cast<DRV_NODE*>(GetUp(cdrv)));};
inline void 		DRV_NODE::ClrSelChild		(void)					{ClrChildSelected(cgSelDrv);};
inline bool32		DRV_NODE::IsSelected		(const DRV_NODE* cdrv)	{return((cdrv == nullptr)?G_FALSE:cdrv->CheckSFlag(CN_blSelected));};
inline void 		DRV_NODE::ReleaseCouple		(DRV_NODE* cdrv)		{if (cdrv != nullptr) cdrv->cgCLock.R_clr();};
inline void 		DRV_NODE::ReleaseSelected	(DRV_NODE* cdrv)		{if (cdrv != nullptr) cdrv->cgCLock.R_clr();};
//------------------------------------------------------------------------------------------//
inline DRV_NODE* DRV_NODE::AcquireCouple(DRV_NODE* cdrv){
	if (cdrv != nullptr){
		cdrv->cgCLock.R_set();
		return(cdrv->cgCouple);
	}
	return(nullptr);
};
//------------------------------------------------------------------------------------------//
inline DRV_NODE* DRV_NODE::AcquireSelected(DRV_NODE* cdrv){
	if (cdrv != nullptr){
		cdrv->cgCLock.R_set();
		return(cdrv->cgSelDrv);
	}
	return(nullptr);
};
//------------------------------------------------------------------------------------------//
inline void DRV_NODE::SetblSelected(DRV_NODE* cdrv){
	DRV_NODE *f = GetFather(cdrv);
	if (f != nullptr){
		f->SetChildSelected(cdrv);
	}
	else{
		if (cdrv != nullptr)
			cdrv->SetSFlag(CN_blSelected);
	}
};
//------------------------------------------------------------------------------------------//
inline void DRV_NODE::ClrblSelected(DRV_NODE* cdrv){
	DRV_NODE *f = GetFather(cdrv);
	if (f != nullptr){
		f->ClrChildSelected(cdrv);
	}
	else{
		if (cdrv != nullptr)
			cdrv->ClrSFlag(CN_blSelected);
	}
};
//------------------------------------------------------------------------------------------//
inline DRV_NODE* DRV_NODE::GetNewChild(void){
	return(AppendChild(static_cast<DRV_NODE*>(GetNewNode())));
};
//------------------------------------------------------------------------------------------//
inline DRV_NODE* DRV_NODE::AppendChild(DRV_NODE* cdrv){
	AppendDownNode(cdrv);
	if (GetDRNodeID(cdrv) == 0)
		SetChildSelected(cdrv);
	return(cdrv);
};
//------------------------------------------------------------------------------------------//
inline void DRV_NODE::JoinTo(DRV_NODE* cdrv){
	if (cdrv != nullptr)
		cdrv->AppendChild(this);
};
//------------------------------------------------------------------------------------------//
inline void DRV_NODE::RemoveSelf(void){
	ClrblSelected(this);
	TNF::RemoveSelf();
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
inline	ODEV_BUS*		COMMU_ODEVBUS::GetODevBus	(COMMU_ODEVBUS* _bus)	{return((_bus == nullptr)?nullptr:_bus->cgODevBUS);};
inline	ODEV_VG3D*		COMMU_ODEVBUS::GetVG3D		(COMMU_ODEVBUS* _bus)	{return((_bus == nullptr)?nullptr:_bus->cgVG3D);};
//------------------------------------------------------------------------------------------//
template<typename... Args> inline void COMMU_ODEVBUS::PrintStr(COMMU_ODEVBUS* _bus,const Args&... args){
#ifdef ODEV_BUS_h
	if (_bus != nullptr)
		OUTPUT_NODE::PrintStr((OUTPUT_NODE*)_bus->cgVG3D,args...);
#endif
};
//------------------------------------------------------------------------------------------//
template<typename... Args> inline void COMMU_ODEVBUS::PrintStrNL(COMMU_ODEVBUS* _bus,const Args&... args){
#ifdef ODEV_BUS_h
	if (_bus != nullptr)
		OUTPUT_NODE::PrintStrNL((OUTPUT_NODE*)_bus->cgVG3D,args...);
#endif
};
//------------------------------------------------------------------------------------------//
template<typename... Args> inline void COMMU_ODEVBUS::PrintALine(COMMU_ODEVBUS* _bus,const Args&... args){
#ifdef ODEV_BUS_h
	if (_bus != nullptr)
		OUTPUT_NODE::PrintALine((OUTPUT_NODE*)_bus->cgVG3D,args...);
#endif
};
//------------------------------------------------------------------------------------------//
template<typename... Args> inline void COMMU_ODEVBUS::PrintWithTime(COMMU_ODEVBUS* _bus,const Args&... args){
#ifdef ODEV_BUS_h
	if (_bus != nullptr)
		OUTPUT_NODE::PrintWithTime((OUTPUT_NODE*)_bus->cgVG3D,args...);
#endif
};
//------------------------------------------------------------------------------------------//
template<typename... Args> inline void COMMU_ODEVBUS::PrintWithDividingLine(COMMU_ODEVBUS* _bus,const Args&... args){
#ifdef ODEV_BUS_h
	if (_bus != nullptr)
		OUTPUT_NODE::PrintWithDividingLine((OUTPUT_NODE*)_bus->cgVG3D,args...);
#endif
};
//------------------------------------------------------------------------------------------//
template<typename... Args> inline void COMMU_ODEVBUS::PrintMessageDot(COMMU_ODEVBUS* _bus,const Args&... args){
#ifdef ODEV_BUS_h
	if (_bus != nullptr)
		OUTPUT_NODE::PrintMessageDot((OUTPUT_NODE*)_bus->cgVG3D,args...);
#endif
};
//------------------------------------------------------------------------------------------//
template<typename... Args> inline void COMMU_ODEVBUS::PrintWarningMessageDot(COMMU_ODEVBUS* _bus,const Args&... args){
#ifdef ODEV_BUS_h
	if (_bus != nullptr)
		OUTPUT_NODE::PrintWarningMessageDot((OUTPUT_NODE*)_bus->cgVG3D,args...);
#endif
};
//------------------------------------------------------------------------------------------//
template<typename... Args> inline void COMMU_ODEVBUS::PrintConnectionReport(COMMU_ODEVBUS* _bus,const uint64& rxBytes,const uint64& txBytes,const uint64& dpBytes,const Args&... args){
#ifdef ODEV_BUS_h
	OUTPUT_NODE *node = nullptr;
	if (_bus != nullptr)
		node = (OUTPUT_NODE*)_bus->cgVG3D;
	if (node != nullptr){
		*node << Begin() << DefGroup() << NL()
		<< COL_DivLineTime
		<< COL_NormalMessage;
		
		DoPrintStr(node,args...)
		<< ".\n"
		<< COL_DB_Statistics
		<< "Received : " << Str_ToStr(rxBytes) << " bytes\n"
		<< "Echo     : " << Str_ToStr(dpBytes) << " bytes\n"
		<< "Sent     : " << Str_ToStr(txBytes) << " bytes\n"
		<< COL_DivLine << DIVIDING_LINE
		<< Endl();
	}
#endif
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
static inline OPEN_PAR& SetOpenPar(OPEN_PAR* op,uint32 type,const STDSTR& name,uint32 port,uint32 cfg){
	op->type = type;
	op->name = name;
	op->port = port;
	op->cfg = cfg;
	return(*op);
};
//------------------------------------------------------------------------------------------//
static inline OPEN_PAR SetOpenPar(uint32 type,const STDSTR& name,uint32 port,uint32 cfg){
	return{type,name,port,cfg};
};
//------------------------------------------------------------------------------------------//
static inline OPEN_PAR& SetOpenPar(OPEN_PAR* op,const OPEN_PAR& op2){
	op->type = op2.type;
	op->name = op2.name;
	op->port = op2.port;
	op->cfg = op2.cfg;
	return(*op);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
inline	const	STDSTR&		COMMU_CORE::GetDevName	(void)const		{return(cgDevName);};
inline	const	OPEN_PAR&	COMMU_CORE::GetOpenPar	(void)const		{return(cgOpenPar);};
inline			void		COMMU_CORE::SetTypePar	(uint32 type)	{cgOpenPar.type = type;};
inline			void		COMMU_CORE::SetCfgPar	(uint32 cfg)	{cgOpenPar.cfg = cfg;};
inline			bool32		COMMU_CORE::IsConnected	(void)const		{return(CheckSFlag(CF_blConnected));};
//------------------------------------------------------------------------------------------//
inline	void	COMMU_CORE::SetSelfName		(const STDSTR& strName)	{selfName = strName;};
inline	void	COMMU_CORE::SetUpName		(const STDSTR& strName)	{fatherName = strName;;};
inline	void	COMMU_CORE::CloseDev		(void)					{ClrSFlag(CF_blConnected);};
//------------------------------------------------------------------------------------------//
inline void COMMU_CORE::SetDevName(const OPEN_PAR& par){
	SetOpenPar(&cgOpenPar, par);
	cgDevName = cgOpenPar.name + ":" + Str_ToStr(cgOpenPar.port);
};
//------------------------------------------------------------------------------------------//
inline bool32 COMMU_CORE::OpenDev(const OPEN_PAR& par){
	SetDevName(par);
	SetSFlag(CF_blConnected);
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
inline bool32 COMMU_CORE::SendToDevice(uint32* retNum,const uint8* buffer,uint32 length){
	*retNum = 0;
	return 0;
};
//------------------------------------------------------------------------------------------//
inline bool32 COMMU_CORE::ReadFromDevice(uint32* retNum,uint8* buffer,uint32 length){
	*retNum = 0;
	return 0;
};
//------------------------------------------------------------------------------------------//
inline void COMMU_CORE::PrintOpenSuccess(const STDSTR& strTitle){
	COMMU_ODEVBUS::PrintMessageDot(unitTeam->odevBus,"Successfully connect to",strTitle,GetDevName());
};
//------------------------------------------------------------------------------------------//
inline void COMMU_CORE::PrintOpenFail(const STDSTR& strTitle){
	COMMU_ODEVBUS::PrintMessageDot(unitTeam->odevBus,"Failed connect to",strTitle,GetDevName());
};
//------------------------------------------------------------------------------------------//
inline void COMMU_CORE::PrintClose(const uint64& rxBytes,const uint64& txBytes,const uint64& dpBytes){
	COMMU_ODEVBUS::PrintConnectionReport(unitTeam->odevBus,rxBytes,txBytes,dpBytes,"User closed the connection");
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
void COMMU_BRIDGE::Init(const COMMU_TEAM* _team){
	COMMU_UNIT::Init(_team);
	unitTeam->cdrv->ThreadAccept(&rxThread);
	unitTeam->cdrv->ThreadAccept(&txThread);
	
	Empty(this);
	ClrSFlag(CF_blPushSend);
};
//------------------------------------------------------------------------------------------//
inline void COMMU_BRIDGE::InitArray(COMMU_BRIDGE* bridge,const ARRAY* _rx, const ARRAY* _tx){
	if (bridge != nullptr){
		bridge->cgArrayRx = (ARRAY*)_rx;
		bridge->cgArrayTx = (ARRAY*)_tx;
	}
};
//------------------------------------------------------------------------------------------//
inline void COMMU_BRIDGE::SetSelfName(const STDSTR& strName){
	selfName = strName;
	rxThread.SetUpName(GetFullName(this));
	txThread.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
inline void COMMU_BRIDGE::SetUpName(const STDSTR& strName){
	fatherName = strName;
	rxThread.SetUpName(GetFullName(this));
	txThread.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
inline	uint32	COMMU_BRIDGE::RxMaxSize		(COMMU_BRIDGE* bridge)	{return((bridge == nullptr)?0:bridge->cgArrayRx->MaxSize());};
inline	uint32	COMMU_BRIDGE::TxMaxSize		(COMMU_BRIDGE* bridge)	{return((bridge == nullptr)?0:bridge->cgArrayTx->MaxSize());};
inline	uint32	COMMU_BRIDGE::UnreadBytes	(COMMU_BRIDGE* bridge)	{return((bridge == nullptr)?0:bridge->cgArrayRx->Used());};
inline	uint32	COMMU_BRIDGE::UnsentBytes	(COMMU_BRIDGE* bridge)	{return((bridge == nullptr)?0:bridge->cgArrayTx->Used());};
inline	uint64	COMMU_BRIDGE::RxBytes		(COMMU_BRIDGE* bridge)	{return((bridge == nullptr)?0:bridge->cgRxBytes);};
inline	uint64	COMMU_BRIDGE::TxBytes		(COMMU_BRIDGE* bridge)	{return((bridge == nullptr)?0:bridge->cgTxBytes);};
inline	ARRAY*	COMMU_BRIDGE::GetArrayRx	(COMMU_BRIDGE* bridge)	{return((bridge == nullptr)?0:bridge->cgArrayRx);};
inline	ARRAY*	COMMU_BRIDGE::GetArrayTx	(COMMU_BRIDGE* bridge)	{return((bridge == nullptr)?0:bridge->cgArrayTx);};
inline	void	COMMU_BRIDGE::EnableRxThread(COMMU_BRIDGE *bridge)	{if (bridge != nullptr) TNF::Enable(&bridge->rxThread);};
inline	void	COMMU_BRIDGE::DisableRxThread(COMMU_BRIDGE* bridge)	{if (bridge != nullptr) TNF::Disable(&bridge->rxThread);};
//------------------------------------------------------------------------------------------//
void COMMU_BRIDGE::Empty(COMMU_BRIDGE* bridge){
	if (bridge != nullptr){
		bridge->cgRxBytes = 0;
		bridge->cgTxBytes = 0;
		if (bridge->cgArrayRx != nullptr)
			bridge->cgArrayRx->Empty();
		if (bridge->cgArrayTx != nullptr)
			bridge->cgArrayTx->Empty();
	}
};
//------------------------------------------------------------------------------------------//











//------------------------------------------------------------------------------------------//
inline void COMMU_MEM::Init(const COMMU_TEAM* _team){
	COMMU_UNIT::Init(_team);
	COMMU_BRIDGE::InitArray(_team->bridge,&arrayRx,&arrayTx);
	arrayRx.Empty();
	arrayTx.Empty();
};
//------------------------------------------------------------------------------------------//
inline void	COMMU_MEM::InitArray(COMMU_MEM *mem,uint32 rxSize,uint32 txSize){
	if (mem != nullptr)
		mem->InitArraySize(rxSize,txSize);
};
//------------------------------------------------------------------------------------------//
inline void	COMMU_MEM::InitArraySize(uint32 rxSize,uint32 txSize){
	arrayRx.InitSize(rxSize);
	arrayTx.InitSize(txSize);
};
//------------------------------------------------------------------------------------------//
inline void COMMU_MEM::SetSelfName(const STDSTR& strName){
	selfName = strName;
};
//------------------------------------------------------------------------------------------//
inline void COMMU_MEM::SetUpName(const STDSTR& strName){
	fatherName = strName;
};
//------------------------------------------------------------------------------------------//
inline	ARRAY*	COMMU_MEM::GetArrayRx			(COMMU_MEM* mem)	{return((mem == nullptr)?nullptr:mem->GetArrayRx_original());};
inline	ARRAY*	COMMU_MEM::GetArrayTx			(COMMU_MEM* mem)	{return((mem == nullptr)?nullptr:mem->GetArrayTx_original());};
inline	ARRAY*	COMMU_MEM::GetArrayRx_original	(void)				{return(&arrayRx);};
inline	ARRAY*	COMMU_MEM::GetArrayTx_original	(void)				{return(&arrayTx);};
//------------------------------------------------------------------------------------------//
void COMMU_MEM::Empty(COMMU_MEM* mem){
	if (mem != nullptr)
		mem->EmptyArray();
};
//------------------------------------------------------------------------------------------//
inline void COMMU_MEM::EmptyArray(void){
	arrayRx.Empty();
	arrayRx.Empty();
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
void COMMU_DISPOSE::Init(const COMMU_TEAM* _team){
	COMMU_UNIT::Init(_team);
	unitTeam->cdrv->ThreadAccept(&dpThread);
};
//------------------------------------------------------------------------------------------//
inline void COMMU_DISPOSE::SetSelfName(const STDSTR& strName){
	selfName = strName;
	dpThread.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
inline void COMMU_DISPOSE::SetUpName(const STDSTR& strName){
	fatherName = strName;
	dpThread.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
inline void COMMU_DISPOSE::Empty(COMMU_DISPOSE* dispose){
	if (dispose != nullptr){
		dispose->cgDPBytes = 0;
	}
};
//------------------------------------------------------------------------------------------//
inline COMMU_DISPOSE* COMMU_DISPOSE::GetDPList(COMMU_DISPOSE* dispose){
	if (dispose != nullptr)
		return(dispose->cgDPList);
	return(nullptr);
};
//------------------------------------------------------------------------------------------//
inline void COMMU_DISPOSE::UseSelfDPList(COMMU_DISPOSE* dispose){
	if (dispose != nullptr){
		dispose->rwlock.W_set();
		dispose->cgDPList = dispose;
		dispose->rwlock.W_clr();
	}
};
//------------------------------------------------------------------------------------------//
inline void COMMU_DISPOSE::UseExternalDPList(COMMU_DISPOSE* dispose,const COMMU_DISPOSE* dpList){
	if (dispose != nullptr){
		dispose->rwlock.W_set();
		dispose->cgDPList = (COMMU_DISPOSE*)dpList;
		dispose->rwlock.W_clr();
	}
};
//------------------------------------------------------------------------------------------//
inline void COMMU_DISPOSE::ShareTo(COMMU_DISPOSE* dispose,SBUF* tSBUF){
	if (dispose != nullptr){
		dispose->rwlock.R_set();
		dispose->cgDPList->AppendDownNode(tSBUF);
		dispose->rwlock.R_clr();
	}
};
//------------------------------------------------------------------------------------------//
inline void COMMU_DISPOSE::Enable(COMMU_DISPOSE* dispose,bool32 blEnable){
	if (dispose != nullptr){
		if (blEnable == G_FALSE){
			TNF::Disable(&dispose->dpThread);
			dispose->dpThread.ThreadStop();
		}
		else{
			TNF::Enable(&dispose->dpThread);
		}
	}
};
//------------------------------------------------------------------------------------------//
inline	uint64	COMMU_DISPOSE::DPBytes		(COMMU_DISPOSE* dispose)	{return((dispose == nullptr)?0:dispose->cgDPBytes);};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
void COMMU_RECORD::Init(const COMMU_TEAM* _team){
	COMMU_UNIT::Init(_team);
	unitTeam->cdrv->ThreadAccept(&recThread);
};
//------------------------------------------------------------------------------------------//
inline void COMMU_RECORD::SetSelfName(const STDSTR& strName){
	selfName = strName;
	recThread.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
inline void COMMU_RECORD::SetUpName(const STDSTR& strName){
	fatherName = strName;
	recThread.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
inline void COMMU_RECORD::Enable(COMMU_RECORD* record,bool32 blEnable){
	if (record != nullptr){
		if (blEnable == G_FALSE){
			TNF::Disable(&record->recThread);
			record->recThread.ThreadStop();
		}
		else{
			TNF::Enable(&record->recThread);
		}
	}
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------/
template<typename T_UNIT> T_UNIT*	COMMU_TEAM::Create_unit		(T_UNIT* _unit)					 {if (_unit != nullptr) return(new T_UNIT);return(nullptr);};
template<typename T_UNIT> T_UNIT*	COMMU_TEAM::Create_core		(COMMU_TEAM* _team,T_UNIT* _unit){_team->core		= Create_unit(_unit);return((T_UNIT*)_team->core);};
template<typename T_UNIT> T_UNIT*	COMMU_TEAM::Create_birdge	(COMMU_TEAM* _team,T_UNIT* _unit){_team->bridge		= Create_unit(_unit);return((T_UNIT*)_team->bridge);};
template<typename T_UNIT> T_UNIT*	COMMU_TEAM::Create_mem		(COMMU_TEAM* _team,T_UNIT* _unit){_team->mem		= Create_unit(_unit);return((T_UNIT*)_team->mem);};
template<typename T_UNIT> T_UNIT*	COMMU_TEAM::Create_odecBus	(COMMU_TEAM* _team,T_UNIT* _unit){_team->odevBus	= Create_unit(_unit);return((T_UNIT*)_team->odevBus);};
template<typename T_UNIT> T_UNIT*	COMMU_TEAM::Create_dispose	(COMMU_TEAM* _team,T_UNIT* _unit){_team->dispose	= Create_unit(_unit);return((T_UNIT*)_team->dispose);};
template<typename T_UNIT> T_UNIT*	COMMU_TEAM::Create_record	(COMMU_TEAM* _team,T_UNIT* _unit){_team->record		= Create_unit(_unit);return((T_UNIT*)_team->record);};
//------------------------------------------------------------------------------------------/










//------------------------------------------------------------------------------------------//
inline			bool32	_COMMU_DRV::IsOpened			(void)const	{return(CheckSFlag(CF_blOpened));};
inline			bool32	_COMMU_DRV::IsCloseDueToOS		(void)const {return(CheckSFlag(CF_blCloseDueToOS));};
inline			void	_COMMU_DRV::SetblCloseCouple	(void)		{		SetSFlag(CF_blEnCloseCouple);};
inline			bool32	_COMMU_DRV::IsEnableEcho		(void)const {return(B_ChkFLAG32(unitTeam.core->GetOpenPar().cfg,CFG_blEnableEcho));};
inline	const	uint32&	_COMMU_DRV::RxArrayMaxSize		(void)const	{return(cgMaxRxArraySize);};
inline	const	uint32&	_COMMU_DRV::TxArrayMaxSize		(void)const	{return(cgMaxTxArraySize);};
//------------------------------------------------------------------------------------------//
inline COMMU_DRV* _COMMU_DRV::Find(const STDSTR& name,uint32 port){
	COMMU_DRV *retCommu;
	retCommu = nullptr;
	TREE_DownChain_Find(COMMU_DRV,this,retCommu
						,((_opNode->unitTeam.core->GetOpenPar().name == name) && (_opNode->unitTeam.core->GetOpenPar().port == port)));
	return(retCommu);
};
//------------------------------------------------------------------------------------------//
inline SYS_AThread*		_COMMU_DRV::ThreadGet		(const STDSTR& name)			{return(cgThreadList.GetThread(name));};
inline void				_COMMU_DRV::ThreadAccept	(SYS_AThread * _thread)			{cgThreadList.AppendDownNode(_thread);};
inline void				_COMMU_DRV::RxDataShareTo	(SBUF* tSBUF)					{COMMU_DISPOSE::ShareTo(unitTeam.dispose, tSBUF);};
inline COMMU_CORE*		_COMMU_DRV::Core			(void)const						{return(unitTeam.core);};
//------------------------------------------------------------------------------------------/










//------------------------------------------------------------------------------------------//
template <typename T_CDRV> COMMU_NOSMC_T<T_CDRV>::COMMU_NOSMC_T(uint32 rxSize,uint32 txSize,const ODEV_BUS* odevBus) : T_CDRV(){
	this->Init(rxSize, txSize, odevBus);
};
//------------------------------------------------------------------------------------------//
template <typename T_CDRV> void COMMU_NOSMC_T<T_CDRV>::DoThreadsStart(void){
	this->cgThreadList.ThreadRun(&this->unitTeam);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
template <typename T_CDRV,typename T_CDEV>
CDRV_POOL_T<T_CDRV,T_CDEV>::CDRV_POOL_T(void): T_CDRV(){
	COMMU_DISPOSE::Enable(T_CDRV::unitTeam.dispose, G_FALSE);
};
//------------------------------------------------------------------------------------------//
template <typename T_CDRV,typename T_CDEV>
CDRV_POOL_T<T_CDRV,T_CDEV>::CDRV_POOL_T(uint32 rxSize,uint32 txSize,const ODEV_BUS* odevBus) : CDRV_POOL_T(){
	this->Init(rxSize,txSize,odevBus);
};
//------------------------------------------------------------------------------------------//
template <typename T_CDRV,typename T_CDEV>TNF* CDRV_POOL_T<T_CDRV,T_CDEV>::CreateNode(void){
	return(TNFP::SetNodeUpName(new T_CDEV(T_CDRV::cgMaxRxArraySize,T_CDRV::cgMaxTxArraySize,COMMU_ODEVBUS::GetODevBus(T_CDRV::unitTeam.odevBus))));
};
//------------------------------------------------------------------------------------------//
template <typename T_CDRV,typename T_CDEV>uint32 CDRV_POOL_T<T_CDRV,T_CDEV>::Send(const UVIn&  _in,uint32 timeoutMS){
	COMMU_DRV	*cdrv;
	uint32		ret = 0;
	
	cdrv = static_cast<COMMU_DRV*>(T_CDRV::AcquireSelected(this));
	if (cdrv != nullptr){
		if (cdrv != this)
			ret = cdrv->Send(_in,timeoutMS);
		T_CDRV::ReleaseSelected(this);
	}
	return(ret);
};
//------------------------------------------------------------------------------------------//
template <typename T_CDRV,typename T_CDEV>uint32 CDRV_POOL_T<T_CDRV,T_CDEV>::Read(const UVOut& _out,uint32 num){
	COMMU_DRV	*cdrv;
	uint32		ret = 0;
	
	cdrv = static_cast<COMMU_DRV*>(T_CDRV::AcquireSelected(this));
	if (cdrv != nullptr){
		if (cdrv != this)
			ret = cdrv->Read(_out,num);
		T_CDRV::ReleaseSelected(this);
	}
	return(ret);
};
//------------------------------------------------------------------------------------------//
template <typename T_CDRV,typename T_CDEV>uint64 CDRV_POOL_T<T_CDRV,T_CDEV>::RxBytes(void){
	COMMU_DRV	*cdrv;
	uint64		ret = 0;
	
	cdrv = static_cast<COMMU_DRV*>(T_CDRV::AcquireSelected(this));
	if (cdrv != nullptr){
		if (cdrv != this)
			ret = cdrv->RxBytes();
		T_CDRV::ReleaseSelected(this);
	}
	return(ret);
};
//------------------------------------------------------------------------------------------//
template <typename T_CDRV,typename T_CDEV>uint64 CDRV_POOL_T<T_CDRV,T_CDEV>::TxBytes(void){
	COMMU_DRV	*cdrv;
	uint64		ret = 0;
	
	cdrv = static_cast<COMMU_DRV*>(T_CDRV::AcquireSelected(this));
	if (cdrv != nullptr){
		if (cdrv != this)
			ret = cdrv->TxBytes();
		T_CDRV::ReleaseSelected(this);
	}
	return(ret);
};
//------------------------------------------------------------------------------------------//
template <typename T_CDRV,typename T_CDEV>uint64 CDRV_POOL_T<T_CDRV,T_CDEV>::DPBytes(void){
	COMMU_DRV	*cdrv;
	uint64		ret = 0;
	
	cdrv = static_cast<COMMU_DRV*>(T_CDRV::AcquireSelected(this));
	if (cdrv != nullptr){
		if (cdrv != this)
			ret = cdrv->DPBytes();
		T_CDRV::ReleaseSelected(this);
	}
	return(ret);
};
//------------------------------------------------------------------------------------------//
template <typename T_CDRV,typename T_CDEV>uint32 CDRV_POOL_T<T_CDRV,T_CDEV>::RxMaxSize(void){
	COMMU_DRV	*cdrv;
	uint32		ret = 0;
	
	cdrv = static_cast<COMMU_DRV*>(T_CDRV::AcquireSelected(this));
	if (cdrv != nullptr){
		if (cdrv != this)
			ret = cdrv->RxMaxSize();
		T_CDRV::ReleaseSelected(this);
	}
	return(ret);
};
//------------------------------------------------------------------------------------------//
template <typename T_CDRV,typename T_CDEV>uint32 CDRV_POOL_T<T_CDRV,T_CDEV>::TxMaxSize(void){
	COMMU_DRV	*cdrv;
	uint32		ret = 0;
	
	cdrv = static_cast<COMMU_DRV*>(T_CDRV::AcquireSelected(this));
	if (cdrv != nullptr){
		if (cdrv != this)
			ret = cdrv->TxMaxSize();
		T_CDRV::ReleaseSelected(this);
	}
	return(ret);
};
//------------------------------------------------------------------------------------------//
template <typename T_CDRV,typename T_CDEV>uint32 CDRV_POOL_T<T_CDRV,T_CDEV>::UnreadBytes(void){
	COMMU_DRV	*cdrv;
	uint32		ret = 0;
	
	cdrv = static_cast<COMMU_DRV*>(T_CDRV::AcquireSelected(this));
	if (cdrv != nullptr){
		if (cdrv != this)
			ret = cdrv->UnreadBytes();
		T_CDRV::ReleaseSelected(this);
	}
	return(ret);
};
//------------------------------------------------------------------------------------------//
template <typename T_CDRV,typename T_CDEV>uint32 CDRV_POOL_T<T_CDRV,T_CDEV>::UnsentBytes(void){
	COMMU_DRV	*cdrv;
	uint32		ret = 0;
	
	cdrv = static_cast<COMMU_DRV*>(T_CDRV::AcquireSelected(this));
	if (cdrv != nullptr){
		if (cdrv != this)
			ret = cdrv->UnsentBytes();
		T_CDRV::ReleaseSelected(this);
	}
	return(ret);
};
//------------------------------------------------------------------------------------------//
template <typename T_CDRV,typename T_CDEV>void CDRV_POOL_T<T_CDRV,T_CDEV>::Empty(void){
	COMMU_DRV	*cdrv;
	
	cdrv = static_cast<COMMU_DRV*>(T_CDRV::AcquireSelected(this));
	if (cdrv != nullptr){
		if (cdrv != this)
			cdrv->Empty();
		T_CDRV::ReleaseSelected(this);
	}
};
//------------------------------------------------------------------------------------------//
template <typename T_CDRV,typename T_CDEV>T_CDEV* CDRV_POOL_T<T_CDRV,T_CDEV>::Attach(uint32 index){
	DRV_NODE	*cdrv = nullptr;
	DRV_NODE	*oldSel = nullptr;
	
	if (this->IsOpened()){
		cdrv = static_cast<T_CDEV*>(this->FindInDownChainByDRNodeID(this, index));
		oldSel = DRV_NODE::AcquireSelected(this);
		if (oldSel != nullptr){
			DRV_NODE::ReleaseSelected(this);
			if (oldSel == cdrv){
				this->ClrChildSelected(cdrv);
			}
			else{
				this->SetChildSelected(cdrv);
			}
		}
	}
	return(static_cast<T_CDEV*>(cdrv));
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
template <typename T_COMMU_A,typename T_COMMU_B>
COMMU_CPPOOL<T_COMMU_A,T_COMMU_B>::COMMU_CPPOOL(void) : TNFP(){
	TNFP::SetSelfName("COMMU_CPPOOL");
	SetSelfName(selfName);
	cgPoolA.selfName = "PoolA";
	cgPoolB.selfName = "PoolB";
	SetUpName("");
};
//------------------------------------------------------------------------------------------//
template <typename T_COMMU_A,typename T_COMMU_B>
COMMU_CPPOOL<T_COMMU_A,T_COMMU_B>::~COMMU_CPPOOL(void){
	Close();
};
//------------------------------------------------------------------------------------------//
template <typename T_COMMU_A,typename T_COMMU_B>
void COMMU_CPPOOL<T_COMMU_A,T_COMMU_B>::Init(uint32 rxSize,uint32 txSize){
	cgPoolA.Init(rxSize,txSize,nullptr);
	cgPoolB.Init(rxSize,txSize,nullptr);
};
//------------------------------------------------------------------------------------------//
template <typename T_COMMU_A,typename T_COMMU_B>
void COMMU_CPPOOL<T_COMMU_A,T_COMMU_B>::SetSelfName(const STDSTR& strName){
	selfName = strName;
	cgPoolA.SetUpName(GetFullName(this));
	cgPoolB.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
template <typename T_COMMU_A,typename T_COMMU_B>
void COMMU_CPPOOL<T_COMMU_A,T_COMMU_B>::SetUpName(const STDSTR& strName){
	fatherName = strName;
	cgPoolA.SetUpName(GetFullName(this));
	cgPoolB.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
template <typename T_COMMU_A,typename T_COMMU_B>
void COMMU_CPPOOL<T_COMMU_A,T_COMMU_B>::Close(void){
	cgPoolA.Close();
	cgPoolB.Close();
};
//------------------------------------------------------------------------------------------//
template <typename T_COMMU_A,typename T_COMMU_B>
void COMMU_CPPOOL<T_COMMU_A,T_COMMU_B>::CloseChild(T_COMMU_A* cmmuA,T_COMMU_B* cmmuB){
	cgPoolA.CloseChild(cmmuA);
	cgPoolB.CloseChild(cmmuB);
};
//------------------------------------------------------------------------------------------//
template <typename T_COMMU_A,typename T_COMMU_B>
bool32 COMMU_CPPOOL<T_COMMU_A,T_COMMU_B>::GetCP(T_COMMU_A** cmmuA,T_COMMU_B** cmmuB){
	*cmmuA = (T_COMMU_A*)cgPoolA.GetNewChild();
	*cmmuB = (T_COMMU_B*)cgPoolB.GetNewChild();
	if (*cmmuA == nullptr){
		cgPoolB.CloseChild(*cmmuB);
		cmmuB = nullptr;
	}
	else if (cmmuB == nullptr){
		cgPoolA.CloseChild(*cmmuA);
		cmmuA = nullptr;
	}
	else{
		(*cmmuA)->SetblCloseCouple();
		(*cmmuB)->SetblCloseCouple();
		DRV_NODE::LinkCouple(*cmmuA,*cmmuB);
		return G_TRUE;
	}
	return G_FALSE;
};
//------------------------------------------------------------------------------------------//
#endif /* Commu_Base_h */
#endif /* Commu_Base_hpp */
