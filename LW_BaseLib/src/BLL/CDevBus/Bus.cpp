//
//  Bus.cpp
//  SDT
//
//  Created by Leif Wen on 13/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "Bus.h"
#ifdef Bus_h
//------------------------------------------------------------------------------------------//
#include "ADS_SMC.h"
#include "Commu_BLE.h"
#include "Commu_BT_SPP.h"
//------------------------------------------------------------------------------------------//
DEF_CONSTRUCTOR_FUNC(ACOM			,DRV_COM)
DEF_CONSTRUCTOR_FUNC(ASOCKET		,DRV_SOCKET)
DEF_CONSTRUCTOR_FUNC(ASOCKETSERVER	,DRV_SSERVER)
DEF_CONSTRUCTOR_FUNC(BLEPeripheral	,DRV_BLEPeripheral)
DEF_CONSTRUCTOR_FUNC(BTSPP			,DRV_BTSPP)

//------------------------------------------------------------------------------------------//
typedef ExpandDevBusAttr EDA;
//------------------------------------------------------------------------------------------//
EDA::ExpandDevBusAttr(void){
	SetOpenPar(&aCOM	,OPEN_COM ,"/dev/ttyUSB0"	,115200	,0);
	SetOpenPar(&aTCP	,OPEN_TCP ,"leifwen.com"	,9527	,0);
	SetOpenPar(&aUDP	,OPEN_UDP ,"127.0.0.1"		,8192	,0);
	SetOpenPar(&aTCPS	,OPEN_TCPS,""				,9527	,0);
	SetOpenPar(&aUDPS	,OPEN_UDPS,""				,8192	,0);
	openType = OPEN_None;
	name = "";
	cdevbus = nullptr;
	envcfg = nullptr;
};
//------------------------------------------------------------------------------------------//
static inline int32 ToInt(const STDSTR& str){
	int32 ret = 0;
	if (str.length() > 0){
		ret = atoi(str.c_str());
		if (ret < 0)
			ret = 0;
	}
	return(ret);
}
static inline void Update(OPEN_PAR* par,const STDSTR& p1,const STDSTR& p2){
	if (p1.length() > 0)
		par->name = p1;
	int32 port = ToInt(p2);
	if (port > 0)
		par->port =port;
}
static inline void Update_COM(EDA* eda,const STDSTR& p1,const STDSTR& p2){
	if ((eda->openType == OPEN_COM) || (eda->openType == OPEN_COMV)){
		eda->aCOM.type = eda->openType;
		Update(&eda->aCOM,p1,p2);
	}
};
static inline void Update_TCP(EDA* eda,const STDSTR& p1,const STDSTR& p2){
	if (eda->openType == OPEN_TCP){
		eda->aTCP.type = eda->openType;
		Update(&eda->aTCP,p1,p2);
	}
};
static inline void Update_UDP(EDA* eda,const STDSTR& p1,const STDSTR& p2){
	if (eda->openType == OPEN_UDP){
		eda->aUDP.type = eda->openType;
		Update(&eda->aUDP,p1,p2);
	}
};
static inline void Update_TCPs(EDA* eda,const STDSTR& p1,const STDSTR& p2){
	if (eda->openType == OPEN_TCPS){
		eda->aTCPS.type = eda->openType;
		Update(&eda->aTCPS,p1,p2);
	}
};
static inline void Update_UDPs(EDA* eda,const STDSTR& p1,const STDSTR& p2){
	if (eda->openType == OPEN_UDPS){
		eda->aUDPS.type = eda->openType;
		Update(&eda->aUDPS,p1,p2);
	}
};
//------------------------------------------------------------------------------------------//
bool32 EDA::SetConnectPar(uint32 devType,const STDSTR& strPar1,const STDSTR& strPar2){
	do{
		if (IsOpened() != G_FALSE)
			break;
		openType = devType;
		Update_COM(this,strPar1,strPar2);
		Update_TCP(this,strPar1,strPar2);
		Update_UDP(this,strPar1,strPar2);
		Update_TCPs(this,strPar1,strPar2);
		Update_UDPs(this,strPar1,strPar2);
		return G_TRUE;
	}while(0);
	return G_FALSE;
};
//------------------------------------------------------------------------------------------//
bool32 EDA::SetConnectBR(const STDSTR& br){
	#ifdef Commu_Com_h
	if (IsCom() != G_FALSE){
		Update_COM(this,"",br);
		AComCore()->SetBaudrate(aCOM.port);
		return G_TRUE;
	}
	#endif
	return G_FALSE;
};
//------------------------------------------------------------------------------------------//
bool32			EDA::IsOpened		(void)const{return(cdevbus->IsOpened());};
uint32			EDA::DevType		(void)const{return(cdevbus->Core()->GetOpenPar().type);};
#ifdef Commu_Com_h
ACOM* 			EDA::ACom			(void)const{return(static_cast<ACOM*>(cdevbus->IsFor(DRV_COM)));};
CORE_ACOM* 		EDA::AComCore		(void)const{
	if (ACom() != nullptr)
		return(static_cast<CORE_ACOM*>(ACom()->Core()));
	return nullptr;
};
#endif
#ifdef Commu_Socket_h
ASOCKET*		EDA::ASocket		(void)const{return(static_cast<ASOCKET*>(cdevbus->IsFor(DRV_SOCKET)));};
ASOCKETSERVER*	EDA::AServer		(void)const{return(static_cast<ASOCKETSERVER*>(cdevbus->IsFor(DRV_SSERVER)));};
#endif
bool32			EDA::IsCom			(void)const{return(cdevbus->Core()->selfName == DRV_COM);};
bool32			EDA::IsSocket		(void)const{return(cdevbus->Core()->selfName == DRV_SOCKET);};
bool32			EDA::IsServer		(void)const{return(cdevbus->Core()->selfName == DRV_SSERVER);};

bool32			EDA::IsComOpened	(void)const{return((cdevbus->IsOpened()) && IsCom());};
bool32			EDA::IsSocketOpened	(void)const{return((cdevbus->IsOpened()) && IsSocket());};
bool32			EDA::IsServerOpened	(void)const{return((cdevbus->IsOpened()) && IsServer());};
bool32			EDA::IsTCPOpened	(void)const{return((cdevbus->IsOpened()) && (cdevbus->Core()->GetOpenPar().type == OPEN_TCP));};
bool32			EDA::IsUCPOpened	(void)const{return((cdevbus->IsOpened()) && (cdevbus->Core()->GetOpenPar().type == OPEN_UDP));};
bool32			EDA::IsTCPSOpened	(void)const{return((cdevbus->IsOpened()) && (cdevbus->Core()->GetOpenPar().type == OPEN_TCPS));};
bool32			EDA::IsUDPSOpened	(void)const{return((cdevbus->IsOpened()) && (cdevbus->Core()->GetOpenPar().type == OPEN_UDPS));};
//------------------------------------------------------------------------------------------//
ODEV_VG3D& EDA::GetVG3D(void){
	DRV_NODE 	*l1;
	ODEV_VG3D	*node;
	
	l1 = DRV_NODE::AcquireSelected(cdevbus);
	
	node = COMMU_ODEVBUS::GetVG3D(static_cast<COMMU_DRV*>(l1)->unitTeam.odevBus);
	
	DRV_NODE::ReleaseSelected(cdevbus);
	
	return(*node);
};
//------------------------------------------------------------------------------------------//
bool32 EDA::Open(const STDSTR& drvName,const OPEN_PAR& par)const{
	return (cdevbus->Open(drvName,par));
}
//------------------------------------------------------------------------------------------//
bool32 EDA::Open(void)const{
	bool32	err = G_FALSE;
	switch(openType){
#ifdef SWVERSION_TCPClient
		case OPEN_TCP:
			err = cdevbus->Open(DRV_SOCKET,aTCP);
			break;
#endif
#ifdef SWVERSION_UDPCLient
		case OPEN_UDP:
			err = cdevbus->Open(DRV_SOCKET,aUDP);
			break;
#endif
#ifdef SWVERSION_TCPServer
		case OPEN_TCPS:
			err = cdevbus->Open(DRV_SSERVER,aTCPS);
			break;
#endif
#ifdef SWVERSION_UDPServer
		case OPEN_UDPS:
			err = cdevbus->Open(DRV_SSERVER,aUDPS);
			break;
#endif
#ifdef SWVERSION_COM
		case OPEN_COM:;
		case OPEN_COMV:
			err = cdevbus->Open(DRV_COM,aCOM);
			break;
#endif
		default:
			break;
	}
	return(err);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
CDEVBUS::CDEVBUS(uint32 rxSize,uint32 txSize,const ODEV_CACHE* cache)
	: COMMU_DEVBUS(rxSize,txSize,cache){

	Register(DRV_COM			, CONSTRUCTOR_FUNC(DRV_COM));
	Register(DRV_SOCKET			, CONSTRUCTOR_FUNC(DRV_SOCKET));
	Register(DRV_SSERVER		, CONSTRUCTOR_FUNC(DRV_SSERVER));
	Register(DRV_BLEPeripheral	, CONSTRUCTOR_FUNC(DRV_BLEPeripheral));
	Register(DRV_BTSPP			, CONSTRUCTOR_FUNC(DRV_BTSPP));

	cgEDA.cdevbus = this;
	cgEDA.envcfg = &GetODevBus()->envcfg;
};
//------------------------------------------------------------------------------------------//
CDEVBUS::~CDEVBUS(void){
	Close();
	CleanDownTree(this, this);
};
//------------------------------------------------------------------------------------------//
ExpandDevBusAttr*	CDEVBUS::EDA	(void)const	{return((ExpandDevBusAttr*)&cgEDA);};
//------------------------------------------------------------------------------------------//
uint32 CDEVBUS::SendCommandWithPrint(const STDSTR& cmd,CMD_TAIL tail,G_ESCAPE blEscape){
	STDSTR	strForSend;
	PrintSendCommand(_EMPTY(&strForSend),cmd,tail,blEscape);
	return(SendCommand(strForSend));
};
//------------------------------------------------------------------------------------------//
STDSTR CDEVBUS::MakeSendTitle(uint32 num){
	STDSTR	title;

	DRV_NODE 	*l1;
	DRV_NODE 	*l2;
	
	l1 = AcquireSelected(this);
	l2 = AcquireSelected(l1);
#ifdef Commu_Socket_h
	if (l1 == IsFor(DRV_SSERVER)){
		title = MakeTxTitle("",static_cast<COMMU_DRV*>(l2)->Core()->GetDevName(),num
							  ,static_cast<COMMU_DRV*>(l2)->TxBytes() + num, 0);
	}
	else{
		title = MakeTxTitle(num, static_cast<COMMU_DRV*>(l2)->TxBytes() + num);
	}
#else
	title = MakeTxTitle(num, static_cast<COMMU_DRV*>(l2)->TxBytes() + num);
#endif
	ReleaseSelected(l1);
	ReleaseSelected(this);

	return(title);
};
//------------------------------------------------------------------------------------------//
void CDEVBUS::PrintSendCommand(uint32 num,const STDSTR& strForPrint){
	if (num > 0)
		PrintSendCommand(MakeSendTitle(num),"",strForPrint);
};
//------------------------------------------------------------------------------------------//
STDSTR& CDEVBUS::PrintSendCommand(STDSTR* retForSend,const STDSTR& cmd,CMD_TAIL tail,G_ESCAPE blEscape){
	STDSTR	strForPrint;
	
	TransformCommand(_EMPTY(retForSend),_EMPTY(&strForPrint),cmd,tail,blEscape);
	if (retForSend->length() > 0)
		PrintSendCommand(MakeSendTitle((uint32)retForSend->length()),"",strForPrint);
	return(*retForSend);
};
//------------------------------------------------------------------------------------------//
void CDEVBUS::PrintSendCommand(const STDSTR& strTitle1,const STDSTR& strTitle2,const STDSTR& strForPrint){
	Traversal_set();
	OUTPUT_NODE::PrintWithDividingLine(GetSelSTDOUT(),COL_DB_TxTitle,strTitle1,strTitle2,COLOR(COL_DB_TxText,strForPrint));
	Traversal_clr();
};
//------------------------------------------------------------------------------------------//
void CDEVBUS::PrintSendStrWOG1(const STDSTR& strPrintData){
	Traversal_set();
	*GetSelSTDOUT() << Begin() << ClrGroup(COLRECORD::CRD_G1) << NL() << COL_DB_TxText << strPrintData << Endl();
	Traversal_clr();
};
//------------------------------------------------------------------------------------------//
STDSTR CDEVBUS::MakeFrmTitle(void){
	STDSTR	strRet;
	strRet = "";
	switch (Core()->GetOpenPar().type){
		case OPEN_TCP:
			strRet = "TCP:";
			strRet += cgEDA.aTCP.name;
			strRet += "@";
			strRet += Str_ToStr(cgEDA.aTCP.port);
			break;
		case OPEN_UDP:
			strRet = "UDP:";
			strRet += cgEDA.aUDP.name;
			strRet += "@";
			strRet += Str_ToStr(cgEDA.aUDP.port);
			break;
		case OPEN_TCPS:
			strRet = "TCP SERVER";
			strRet += "@";
			strRet += Str_ToStr(cgEDA.aTCPS.port);
			break;
		case OPEN_UDPS:
			strRet = "UDP SERVER";
			strRet += "@";
			strRet += Str_ToStr(cgEDA.aUDPS.port);
			break;
		case OPEN_COM:
		case OPEN_COMV:
			strRet = cgEDA.aCOM.name;
			strRet += "@";
			strRet += Str_ToStr(cgEDA.aCOM.port);
			break;
		default:
			break;
	}
	Str_UpperCaseSelf(strRet);
	return(strRet);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
CDEVBUS_APP::CDEVBUS_APP(uint32 size) : TNF(){
	cgMaxSize = size;
	Init(nullptr);
	executeThread.ThreadInit(this, &CDEVBUS_APP::ExecuteThreadFunc,"executeThread");
};
//------------------------------------------------------------------------------------------//
CDEVBUS_APP:: ~CDEVBUS_APP(void){
	Stop();
};
//------------------------------------------------------------------------------------------//
void CDEVBUS_APP::Init(const CDEVBUS* cDevBus){
	cgCDevBus = (CDEVBUS*)cDevBus;
	cgSBUF.array.Empty();
	ClrblIsTerminated();
	Disable(this);
};
//------------------------------------------------------------------------------------------//
void CDEVBUS_APP::Stop(void){
	SetblIsTerminated();
	executeThread.ThreadStop();
	Init(nullptr);
};
//------------------------------------------------------------------------------------------//
bool32 CDEVBUS_APP::IsExecuting(const CDEVBUS* cDevBus){
	if (cDevBus == nullptr)
		return G_FALSE;
	if (IsStop() == G_FALSE)
		return G_FALSE;
	return(cgRunLock.TryLock());
};
//------------------------------------------------------------------------------------------//
bool32 CDEVBUS_APP::Execute(const CDEVBUS* cDevBus,SYS_AThread* thread){
	SMC_EncryptI(0)
	Stop();
	Init(cDevBus);
	Enable(this);
	
	SMC_EncryptS(0)
	executeThread.ThreadRun(thread);
	SMC_EncryptE(0)
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
bool32 CDEVBUS_APP::ExecuteThreadFunc(void* p){
	ThreadType* thread = static_cast<ThreadType*>(p);
	cgSBUF.array.InitSize(cgMaxSize);
	cgCDevBus->RxDataShareTo(&cgSBUF);
	
	thread->Execute(nullptr);
	
	SYS_SleepMS(10);
	cgSBUF.RemoveSelf();
	Init(nullptr);
	cgRunLock.Unlock();
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
void CDEVBUS_APP::SetblIsTerminated(void){
	SetSFlag(blIsTerminated);
};
//------------------------------------------------------------------------------------------//
void CDEVBUS_APP::ClrblIsTerminated(void){
	ClrSFlag(blIsTerminated);
};
//------------------------------------------------------------------------------------------//
bool32 CDEVBUS_APP::IsTerminated(void)const{
	return(CheckSFlag(blIsTerminated));
};
//------------------------------------------------------------------------------------------//
bool32 CDEVBUS_APP::IsStop(void){
	return(IsEnable(this) == G_FALSE);
};
//------------------------------------------------------------------------------------------//
bool32 CDEVBUS_APP::IsServiceTo(const CDEVBUS* cDevBus){
	return(cDevBus == cgCDevBus);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif /* Bus_h */
