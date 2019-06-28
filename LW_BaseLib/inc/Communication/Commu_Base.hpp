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
inline CoupleType	COMMU_NODE::GetCType		(const COMMU_NODE* commu)	{return((commu == nullptr)?CT_None:commu->cgCType);};
inline COMMU_NODE*	COMMU_NODE::GetCouple		(const COMMU_NODE* commu)	{return((commu == nullptr)?nullptr:commu->cgCouple);};
inline COMMU_NODE*	COMMU_NODE::GetFather		(const COMMU_NODE* commu)	{return((commu == nullptr)?nullptr:static_cast<COMMU_NODE*>(GetUp(commu)));};
inline void 		COMMU_NODE::ClrSelChild		(void)						{ClrChildSelected(cgSelCommu);};
inline bool32		COMMU_NODE::IsSelected		(const COMMU_NODE* commu)	{return((commu == nullptr)?G_FALSE:commu->CheckSFlag(CN_blSelected));};
inline void 		COMMU_NODE::ReleaseCouple	(COMMU_NODE* commu)			{if (commu != nullptr) commu->cgCLock.R_clr();};
inline void 		COMMU_NODE::ReleaseSelected	(COMMU_NODE* commu)			{if (commu != nullptr) commu->cgCLock.R_clr();};
//------------------------------------------------------------------------------------------//
inline COMMU_NODE* COMMU_NODE::AcquireCouple(COMMU_NODE* commu){
	if (commu != nullptr){
		commu->cgCLock.R_set();
		return(commu->cgCouple);
	}
	return(nullptr);
};
//------------------------------------------------------------------------------------------//
inline COMMU_NODE* COMMU_NODE::AcquireSelected(COMMU_NODE* commu){
	if (commu != nullptr){
		commu->cgCLock.R_set();
		return(commu->cgSelCommu);
	}
	return(nullptr);
};
//------------------------------------------------------------------------------------------//
inline void COMMU_NODE::SetblSelected(COMMU_NODE* commu){
	COMMU_NODE *f = GetFather(commu);
	if (f != nullptr){
		f->SetChildSelected(commu);
	}
	else{
		if (commu != nullptr)
			commu->SetSFlag(CN_blSelected);
	}
};
//------------------------------------------------------------------------------------------//
inline void COMMU_NODE::ClrblSelected(COMMU_NODE* commu){
	COMMU_NODE *f = GetFather(commu);
	if (f != nullptr){
		f->ClrChildSelected(commu);
	}
	else{
		if (commu != nullptr)
			commu->ClrSFlag(CN_blSelected);
	}
};
//------------------------------------------------------------------------------------------//
inline COMMU_NODE* COMMU_NODE::GetNewChild(void){
	return(AppendChild(static_cast<COMMU_NODE*>(GetNewNode())));
};
//------------------------------------------------------------------------------------------//
inline COMMU_NODE* COMMU_NODE::AppendChild(COMMU_NODE* commu){
	AppendDownNode(commu);
	if (GetDRNodeID(commu) == 0)
		SetChildSelected(commu);
	return(commu);
};
//------------------------------------------------------------------------------------------//
inline void COMMU_NODE::JoinTo(COMMU_NODE* commu){
	if (commu != nullptr)
		commu->AppendChild(this);
};
//------------------------------------------------------------------------------------------//
inline void COMMU_NODE::RemoveSelf(void){
	ClrblSelected(this);
	TNF::RemoveSelf();
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
inline	ODEV_SYSTEM*	COMMU_LOGSYS::GetLogSystem	(COMMU_LOGSYS* _sys)	{return((_sys == nullptr)?nullptr:_sys->logSystem);};
inline	ODEV_VG3D*		COMMU_LOGSYS::GetVG3D		(COMMU_LOGSYS* _sys)	{return((_sys == nullptr)?nullptr:_sys->cgVG3D);};
//------------------------------------------------------------------------------------------//
template<typename... Args> inline void COMMU_LOGSYS::PrintStr(COMMU_LOGSYS* _sys,const Args&... args){
#ifdef ODEV_System_h
	if (_sys != nullptr)
		OUTPUT_NODE::PrintStr((OUTPUT_NODE*)_sys->cgVG3D,args...);
#endif
};
//------------------------------------------------------------------------------------------//
template<typename... Args> inline void COMMU_LOGSYS::PrintStrNL(COMMU_LOGSYS* _sys,const Args&... args){
#ifdef ODEV_System_h
	if (_sys != nullptr)
		OUTPUT_NODE::PrintStrNL((OUTPUT_NODE*)_sys->cgVG3D,args...);
#endif
};
//------------------------------------------------------------------------------------------//
template<typename... Args> inline void COMMU_LOGSYS::PrintALine(COMMU_LOGSYS* _sys,const Args&... args){
#ifdef ODEV_System_h
	if (_sys != nullptr)
		OUTPUT_NODE::PrintALine((OUTPUT_NODE*)_sys->cgVG3D,args...);
#endif
};
//------------------------------------------------------------------------------------------//
template<typename... Args> inline void COMMU_LOGSYS::PrintWithTime(COMMU_LOGSYS* _sys,const Args&... args){
#ifdef ODEV_System_h
	if (_sys != nullptr)
		OUTPUT_NODE::PrintWithTime((OUTPUT_NODE*)_sys->cgVG3D,args...);
#endif
};
//------------------------------------------------------------------------------------------//
template<typename... Args> inline void COMMU_LOGSYS::PrintWithDividingLine(COMMU_LOGSYS* _sys,const Args&... args){
#ifdef ODEV_System_h
	if (_sys != nullptr)
		OUTPUT_NODE::PrintWithDividingLine((OUTPUT_NODE*)_sys->cgVG3D,args...);
#endif
};
//------------------------------------------------------------------------------------------//
template<typename... Args> inline void COMMU_LOGSYS::PrintMessageDot(COMMU_LOGSYS* _sys,const Args&... args){
#ifdef ODEV_System_h
	if (_sys != nullptr)
		OUTPUT_NODE::PrintMessageDot((OUTPUT_NODE*)_sys->cgVG3D,args...);
#endif
};
//------------------------------------------------------------------------------------------//
template<typename... Args> inline void COMMU_LOGSYS::PrintWarningMessageDot(COMMU_LOGSYS* _sys,const Args&... args){
#ifdef ODEV_System_h
	if (_sys != nullptr)
		OUTPUT_NODE::PrintWarningMessageDot((OUTPUT_NODE*)_sys->cgVG3D,args...);
#endif
};
//------------------------------------------------------------------------------------------//
template<typename... Args> inline void COMMU_LOGSYS::PrintConnectionReport(COMMU_LOGSYS* _sys,const uint64& rxBytes,const uint64& txBytes,const uint64& fwBytes,const Args&... args){
#ifdef ODEV_System_h
	OUTPUT_NODE *node = nullptr;
	if (_sys != nullptr)
		node = (OUTPUT_NODE*)_sys->cgVG3D;
	if (node != nullptr){
		*node << Begin() << DefGroup() << NL()
		<< COL_DivLineTime
		<< COL_NormalMessage;
		
		DoPrintStr(node,args...)
		<< ".\n"
		<< COL_DB_Statistics
		<< "Received : " << Str_ToStr(rxBytes) << " bytes\n"
		<< "Echo     : " << Str_ToStr(fwBytes) << " bytes\n"
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
	COMMU_LOGSYS::PrintMessageDot(unitTeam->logSys,"Successfully connect to",strTitle,GetDevName());
};
//------------------------------------------------------------------------------------------//
inline void COMMU_CORE::PrintOpenFail(const STDSTR& strTitle){
	COMMU_LOGSYS::PrintMessageDot(unitTeam->logSys,"Failed connect to",strTitle,GetDevName());
};
//------------------------------------------------------------------------------------------//
inline void COMMU_CORE::PrintClose(const uint64& rxBytes,const uint64& txBytes,const uint64& fwBytes){
	COMMU_LOGSYS::PrintConnectionReport(unitTeam->logSys,rxBytes,txBytes,fwBytes,"User closed the connection");
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
void COMMU_BRIDGE::Init(const COMMU_TEAM* _team){
	COMMU_UNIT::Init(_team);
	unitTeam->commu->ThreadAccept(&rxThread);
	unitTeam->commu->ThreadAccept(&txThread);
	
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
void COMMU_FORWARD::Init(const COMMU_TEAM* _team){
	COMMU_UNIT::Init(_team);
	unitTeam->commu->ThreadAccept(&fwThread);
};
//------------------------------------------------------------------------------------------//
inline void COMMU_FORWARD::SetSelfName(const STDSTR& strName){
	selfName = strName;
	fwThread.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
inline void COMMU_FORWARD::SetUpName(const STDSTR& strName){
	fatherName = strName;
	fwThread.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
inline void COMMU_FORWARD::Empty(COMMU_FORWARD* forward){
	if (forward != nullptr){
		forward->cgFwBytes = 0;
	}
};
//------------------------------------------------------------------------------------------//
inline COMMU_FORWARD* COMMU_FORWARD::GetFWList(COMMU_FORWARD* forward){
	if (forward != nullptr)
		return(forward->cgFWList);
	return(nullptr);
};
//------------------------------------------------------------------------------------------//
inline void COMMU_FORWARD::UseSelfFwList(COMMU_FORWARD* forward){
	if (forward != nullptr){
		forward->rwlock.W_set();
		forward->cgFWList = forward;
		forward->rwlock.W_clr();
	}
};
//------------------------------------------------------------------------------------------//
inline void COMMU_FORWARD::UseExternalFwList(COMMU_FORWARD* forward,const COMMU_FORWARD* fwList){
	if (forward != nullptr){
		forward->rwlock.W_set();
		forward->cgFWList = (COMMU_FORWARD*)fwList;
		forward->rwlock.W_clr();
	}
};
//------------------------------------------------------------------------------------------//
inline void COMMU_FORWARD::ShareTo(COMMU_FORWARD* forward,SBUF* tSBUF){
	if (forward != nullptr){
		forward->rwlock.R_set();
		forward->cgFWList->AppendDownNode(tSBUF);
		forward->rwlock.R_clr();
	}
};
//------------------------------------------------------------------------------------------//
inline void COMMU_FORWARD::Enable(COMMU_FORWARD* forward,bool32 blEnable){
	if (forward != nullptr){
		if (blEnable == G_FALSE){
			TNF::Disable(&forward->fwThread);
			forward->fwThread.ThreadStop();
		}
		else{
			TNF::Enable(&forward->fwThread);
		}
	}
};
//------------------------------------------------------------------------------------------//
inline	uint64	COMMU_FORWARD::FwBytes		(COMMU_FORWARD* forward)	{return((forward == nullptr)?0:forward->cgFwBytes);};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
void COMMU_RECORD::Init(const COMMU_TEAM* _team){
	COMMU_UNIT::Init(_team);
	unitTeam->commu->ThreadAccept(&logThread);
};
//------------------------------------------------------------------------------------------//
inline void COMMU_RECORD::SetSelfName(const STDSTR& strName){
	selfName = strName;
	logThread.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
inline void COMMU_RECORD::SetUpName(const STDSTR& strName){
	fatherName = strName;
	logThread.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
inline void COMMU_RECORD::Enable(COMMU_RECORD* record,bool32 blEnable){
	if (record != nullptr){
		if (blEnable == G_FALSE){
			TNF::Disable(&record->logThread);
			record->logThread.ThreadStop();
		}
		else{
			TNF::Enable(&record->logThread);
		}
	}
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------/
template<typename T_UNIT> T_UNIT*	COMMU_TEAM::Create_unit		(T_UNIT* _unit)					 {if (_unit != nullptr) return(new T_UNIT);return(nullptr);};
template<typename T_UNIT> T_UNIT*	COMMU_TEAM::Create_core		(COMMU_TEAM* _team,T_UNIT* _unit){_team->core		= Create_unit(_unit);return((T_UNIT*)_team->core);};
template<typename T_UNIT> T_UNIT*	COMMU_TEAM::Create_birdge	(COMMU_TEAM* _team,T_UNIT* _unit){_team->bridge		= Create_unit(_unit);return((T_UNIT*)_team->bridge);};
template<typename T_UNIT> T_UNIT*	COMMU_TEAM::Create_mem		(COMMU_TEAM* _team,T_UNIT* _unit){_team->mem		= Create_unit(_unit);return((T_UNIT*)_team->mem);};
template<typename T_UNIT> T_UNIT*	COMMU_TEAM::Create_logSys	(COMMU_TEAM* _team,T_UNIT* _unit){_team->logSys		= Create_unit(_unit);return((T_UNIT*)_team->logSys);};
template<typename T_UNIT> T_UNIT*	COMMU_TEAM::Create_forward	(COMMU_TEAM* _team,T_UNIT* _unit){_team->forward	= Create_unit(_unit);return((T_UNIT*)_team->forward);};
template<typename T_UNIT> T_UNIT*	COMMU_TEAM::Create_record	(COMMU_TEAM* _team,T_UNIT* _unit){_team->record		= Create_unit(_unit);return((T_UNIT*)_team->record);};
//------------------------------------------------------------------------------------------/










//------------------------------------------------------------------------------------------//
inline			bool32	COMMU_FRAMEB::IsOpened			(void)const	{return(CheckSFlag(CF_blOpened));};
inline			bool32	COMMU_FRAMEB::IsCloseDueToOS	(void)const {return(CheckSFlag(CF_blCloseDueToOS));};
inline			void	COMMU_FRAMEB::SetblCloseCouple	(void)		{		SetSFlag(CF_blEnCloseCouple);};
inline			bool32	COMMU_FRAMEB::IsEnableEcho		(void)const {return(B_ChkFLAG32(unitTeam.core->GetOpenPar().cfg,CFG_blEnableEcho));};
inline	const	uint32&	COMMU_FRAMEB::RxArrayMaxSize	(void)const	{return(cgMaxRxArraySize);};
inline	const	uint32&	COMMU_FRAMEB::TxArrayMaxSize	(void)const	{return(cgMaxTxArraySize);};
//------------------------------------------------------------------------------------------//
inline COMMU_FRAME* COMMU_FRAMEB::Find(const STDSTR& name,uint32 port){
	COMMU_FRAME *retCommu;
	retCommu = nullptr;
	TREE_DownChain_Find(COMMU_FRAME,this,retCommu
						,((_opNode->unitTeam.core->GetOpenPar().name == name) && (_opNode->unitTeam.core->GetOpenPar().port == port)));
	return(retCommu);
};
//------------------------------------------------------------------------------------------//
inline SYS_AThread*		COMMU_FRAMEB::ThreadGet		(const STDSTR& name)			{return(cgThreadList.GetThread(name));};
inline void				COMMU_FRAMEB::ThreadAccept	(SYS_AThread * _thread)			{cgThreadList.AppendDownNode(_thread);};
inline void				COMMU_FRAMEB::RxDataShareTo	(SBUF* tSBUF)					{COMMU_FORWARD::ShareTo(unitTeam.forward, tSBUF);};
inline COMMU_CORE*		COMMU_FRAMEB::Core			(void)const						{return(unitTeam.core);};
//------------------------------------------------------------------------------------------/










//------------------------------------------------------------------------------------------//
template <typename T_FRAME> COMMU_NOSMC<T_FRAME>::COMMU_NOSMC(uint32 rxSize,uint32 txSize,const ODEV_SYSTEM* logSys) : T_FRAME(){
	this->Init(rxSize, txSize, logSys);
};
//------------------------------------------------------------------------------------------//
template <typename T_FRAME> void COMMU_NOSMC<T_FRAME>::DoThreadsStart(void){
	this->cgThreadList.ThreadRun(&this->unitTeam);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
template <typename T_FRAME,typename T_COMMU>
COMMU_POOL<T_FRAME,T_COMMU>::COMMU_POOL(void): T_FRAME(){
	COMMU_FORWARD::Enable(T_FRAME::unitTeam.forward, G_FALSE);
};
//------------------------------------------------------------------------------------------//
template <typename T_FRAME,typename T_COMMU>
COMMU_POOL<T_FRAME,T_COMMU>::COMMU_POOL(uint32 rxSize,uint32 txSize,const ODEV_SYSTEM* logSys) : COMMU_POOL(){
	this->Init(rxSize,txSize,logSys);
};
//------------------------------------------------------------------------------------------//
template <typename T_FRAME,typename T_COMMU>TNFP* COMMU_POOL<T_FRAME,T_COMMU>::CreateNode(void){
	return(TNFP::SetNodeUpName(new T_COMMU(T_FRAME::cgMaxRxArraySize,T_FRAME::cgMaxTxArraySize,COMMU_LOGSYS::GetLogSystem(T_FRAME::unitTeam.logSys))));
};
//------------------------------------------------------------------------------------------//
template <typename T_FRAME,typename T_COMMU>uint32 COMMU_POOL<T_FRAME,T_COMMU>::Send(const UVIn&  _in){
	COMMU_FRAME	*commu;
	uint32		ret = 0;
	
	commu = static_cast<COMMU_FRAME*>(T_FRAME::AcquireSelected(this));
	if (commu != nullptr){
		if (commu != this)
			ret = commu->Send(_in);
		T_FRAME::ReleaseSelected(this);
	}
	return(ret);
};
//------------------------------------------------------------------------------------------//
template <typename T_FRAME,typename T_COMMU>uint32 COMMU_POOL<T_FRAME,T_COMMU>::Read(const UVOut& _out,uint32 num){
	COMMU_FRAME	*commu;
	uint32		ret = 0;
	
	commu = static_cast<COMMU_FRAME*>(T_FRAME::AcquireSelected(this));
	if (commu != nullptr){
		if (commu != this)
			ret = commu->Read(_out,num);
		T_FRAME::ReleaseSelected(this);
	}
	return(ret);
};
//------------------------------------------------------------------------------------------//
template <typename T_FRAME,typename T_COMMU>uint64 COMMU_POOL<T_FRAME,T_COMMU>::RxBytes(void){
	COMMU_FRAME	*commu;
	uint64		ret = 0;
	
	commu = static_cast<COMMU_FRAME*>(T_FRAME::AcquireSelected(this));
	if (commu != nullptr){
		if (commu != this)
			ret = commu->RxBytes();
		T_FRAME::ReleaseSelected(this);
	}
	return(ret);
};
//------------------------------------------------------------------------------------------//
template <typename T_FRAME,typename T_COMMU>uint64 COMMU_POOL<T_FRAME,T_COMMU>::TxBytes(void){
	COMMU_FRAME	*commu;
	uint64		ret = 0;
	
	commu = static_cast<COMMU_FRAME*>(T_FRAME::AcquireSelected(this));
	if (commu != nullptr){
		if (commu != this)
			ret = commu->TxBytes();
		T_FRAME::ReleaseSelected(this);
	}
	return(ret);
};
//------------------------------------------------------------------------------------------//
template <typename T_FRAME,typename T_COMMU>uint64 COMMU_POOL<T_FRAME,T_COMMU>::FwBytes(void){
	COMMU_FRAME	*commu;
	uint64		ret = 0;
	
	commu = static_cast<COMMU_FRAME*>(T_FRAME::AcquireSelected(this));
	if (commu != nullptr){
		if (commu != this)
			ret = commu->FwBytes();
		T_FRAME::ReleaseSelected(this);
	}
	return(ret);
};
//------------------------------------------------------------------------------------------//
template <typename T_FRAME,typename T_COMMU>uint32 COMMU_POOL<T_FRAME,T_COMMU>::RxMaxSize(void){
	COMMU_FRAME	*commu;
	uint32		ret = 0;
	
	commu = static_cast<COMMU_FRAME*>(T_FRAME::AcquireSelected(this));
	if (commu != nullptr){
		if (commu != this)
			ret = commu->RxMaxSize();
		T_FRAME::ReleaseSelected(this);
	}
	return(ret);
};
//------------------------------------------------------------------------------------------//
template <typename T_FRAME,typename T_COMMU>uint32 COMMU_POOL<T_FRAME,T_COMMU>::TxMaxSize(void){
	COMMU_FRAME	*commu;
	uint32		ret = 0;
	
	commu = static_cast<COMMU_FRAME*>(T_FRAME::AcquireSelected(this));
	if (commu != nullptr){
		if (commu != this)
			ret = commu->TxMaxSize();
		T_FRAME::ReleaseSelected(this);
	}
	return(ret);
};
//------------------------------------------------------------------------------------------//
template <typename T_FRAME,typename T_COMMU>uint32 COMMU_POOL<T_FRAME,T_COMMU>::UnreadBytes(void){
	COMMU_FRAME	*commu;
	uint32		ret = 0;
	
	commu = static_cast<COMMU_FRAME*>(T_FRAME::AcquireSelected(this));
	if (commu != nullptr){
		if (commu != this)
			ret = commu->UnreadBytes();
		T_FRAME::ReleaseSelected(this);
	}
	return(ret);
};
//------------------------------------------------------------------------------------------//
template <typename T_FRAME,typename T_COMMU>uint32 COMMU_POOL<T_FRAME,T_COMMU>::UnsentBytes(void){
	COMMU_FRAME	*commu;
	uint32		ret = 0;
	
	commu = static_cast<COMMU_FRAME*>(T_FRAME::AcquireSelected(this));
	if (commu != nullptr){
		if (commu != this)
			ret = commu->UnsentBytes();
		T_FRAME::ReleaseSelected(this);
	}
	return(ret);
};
//------------------------------------------------------------------------------------------//
template <typename T_FRAME,typename T_COMMU>void COMMU_POOL<T_FRAME,T_COMMU>::Empty(void){
	COMMU_FRAME	*commu;
	
	commu = static_cast<COMMU_FRAME*>(T_FRAME::AcquireSelected(this));
	if (commu != nullptr){
		if (commu != this)
			commu->Empty();
		T_FRAME::ReleaseSelected(this);
	}
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
		COMMU_NODE::LinkCouple(*cmmuA,*cmmuB);
		return G_TRUE;
	}
	return G_FALSE;
};
//------------------------------------------------------------------------------------------//
#endif /* Commu_Base_h */
#endif /* Commu_Base_hpp */
