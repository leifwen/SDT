//
//  Device.cpp
//  SDT
//
//  Created by Leif Wen on 13/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "Device.h"
#ifdef Device_h
//------------------------------------------------------------------------------------------//
#include "ADS_SMC.h"
#include "BIF_Transform.h"
#include "BIF_Expression.h"
//------------------------------------------------------------------------------------------//
CORE_DEVICE::CORE_DEVICE(void) : COMMU_CORE(){
#ifdef Commu_Com_h
	cgCom = nullptr;
#endif
#ifdef Commu_Socket_h
	cgSocket = nullptr;
	cgServer = nullptr;
#endif

	SetSelfName("CORE_DEVICE");
	SetUpName("");
};
//------------------------------------------------------------------------------------------//
CORE_DEVICE::~CORE_DEVICE(void){
	CloseDev();
};
//------------------------------------------------------------------------------------------//
bool32 CORE_DEVICE::OpenDev(const OPEN_PAR& par){
	bool32	ret;
	SMC_EncryptI(0)
	SMC_EncryptI(1)
	SMC_EncryptI(2)
	SMC_EncryptI(3)
	SMC_EncryptI(4)
	
	ret = 0;
	static_cast<DEVICE*>(unitTeam->commu)->EDA()->openType = par.type;
	switch(par.type){
#ifdef SWVERSION_COM
		case OPEN_COM:
		case OPEN_COMV:
			SMC_EncryptS(0)
			ret = Open_Com(par);
			SMC_EncryptE(0)
			break;
#endif
#ifdef SWVERSION_TCPClient
		case OPEN_TCP:
			SMC_EncryptS(1)
			ret = Open_Socket(par);
			SMC_EncryptE(1)
			break;
#endif
#ifdef SWVERSION_UDPCLient
		case OPEN_UDP:
			SMC_EncryptS(2)
			ret = Open_Socket(par);
			SMC_EncryptE(2)
			break;
#endif
#ifdef SWVERSION_TCPServer
		case OPEN_TCPS:
			SMC_EncryptS(3)
			ret = Open_Server(par);
			SMC_EncryptE(3)
			break;
#endif
#ifdef SWVERSION_UDPServer
		case OPEN_UDPS:
			SMC_EncryptS(4)
			ret = Open_Server(par);
			SMC_EncryptE(4)
			break;
#endif
		default:
			break;
	}
	return ret;
};
//------------------------------------------------------------------------------------------//
void CORE_DEVICE::CloseDev(void){
	Close_Com();
	Close_Socket();
	Close_Server();
};
//------------------------------------------------------------------------------------------//
const ACOM* CORE_DEVICE::CreateCOM(void){
#ifdef Commu_Com_h
	if (cgCom == nullptr){
		cgCom = new ACOM(unitTeam->commu->RxArrayMaxSize(),unitTeam->commu->RxArrayMaxSize(),COMMU_LOGSYS::GetLogSystem(unitTeam->logSys));
		if (cgCom != nullptr){
			cgCom->SetSelfName("ACOM");
			cgCom->SetUpName(GetFullName(unitTeam->commu));
		}
		unitTeam->commu->AppendChild(cgCom);
	}
	return(cgCom);
#else
	return(nullptr);
#endif
};
//------------------------------------------------------------------------------------------//
const ASOCKET* CORE_DEVICE::CreateSocket(void){
#ifdef Commu_Socket_h
	if (cgSocket == nullptr){
		cgSocket = new ASOCKET(unitTeam->commu->RxArrayMaxSize(),unitTeam->commu->RxArrayMaxSize(),COMMU_LOGSYS::GetLogSystem(unitTeam->logSys));
		if (cgSocket != nullptr){
			cgSocket->SetSelfName("SOCKET");
			cgSocket->SetUpName(GetFullName(unitTeam->commu));
		}
		unitTeam->commu->AppendChild(cgSocket);
	}
	return(cgSocket);
#else
	return(nullptr);
#endif
};
//------------------------------------------------------------------------------------------//
const ASOCKETSERVER* CORE_DEVICE::CreateServer(void){
#ifdef Commu_Socket_h
	if (cgServer == nullptr){
		cgServer = new ASOCKETSERVER(unitTeam->commu->RxArrayMaxSize(),unitTeam->commu->RxArrayMaxSize(),COMMU_LOGSYS::GetLogSystem(unitTeam->logSys));
		if (cgServer != nullptr){
			cgServer->SetSelfName("SERVER");
			cgServer->SetUpName(GetFullName(unitTeam->commu));
		}
		unitTeam->commu->AppendChild(cgServer);
	}
	return(cgServer);
#else
	return(nullptr);
#endif
};
//------------------------------------------------------------------------------------------//
bool32 CORE_DEVICE::Open_Com(const OPEN_PAR& par){
#ifdef Commu_Com_h
	if (CreateCOM() == nullptr)
		return G_FALSE;
	unitTeam->commu->SetChildSelected(cgCom);
	SetOpenPar(&static_cast<DEVICE*>(unitTeam->commu)->EDA()->aCOM,par);
	return(cgCom->Open(par));
#else
	return G_FALSE;
#endif
};
//------------------------------------------------------------------------------------------//
void CORE_DEVICE::Close_Com(void){
#ifdef Commu_Com_h
	if (cgCom != nullptr)
		cgCom->Close();
#endif
};
//------------------------------------------------------------------------------------------//
bool32 CORE_DEVICE::Open_Socket(const OPEN_PAR& par){
#ifdef Commu_Socket_h
	if (CreateSocket() == nullptr)
		return G_FALSE;
	unitTeam->commu->SetChildSelected(cgSocket);
	if (par.type == OPEN_TCP){
		SetOpenPar(&static_cast<DEVICE*>(unitTeam->commu)->EDA()->aTCP,par);
	}
	else{
		SetOpenPar(&static_cast<DEVICE*>(unitTeam->commu)->EDA()->aUDP,par);
	}
	return(cgSocket->Open(par));
#else
	return G_FALSE;
#endif
};
//------------------------------------------------------------------------------------------//
void CORE_DEVICE::Close_Socket(void){
#ifdef Commu_Socket_h
	if (cgSocket != nullptr)
		cgSocket->Close();
#endif
};
//------------------------------------------------------------------------------------------//
bool32 CORE_DEVICE::Open_Server(const OPEN_PAR& par){
#ifdef Commu_Socket_h
	if (CreateServer() == nullptr)
		return G_FALSE;
	unitTeam->commu->SetChildSelected(cgServer);
	if (par.type == OPEN_TCPS){
		SetOpenPar(&static_cast<DEVICE*>(unitTeam->commu)->EDA()->aTCPS,par);
	}
	else{
		SetOpenPar(&static_cast<DEVICE*>(unitTeam->commu)->EDA()->aUDPS,par);
	}
	return(cgServer->Open(par));
#else
	return G_FALSE;
#endif
};
//------------------------------------------------------------------------------------------//
void CORE_DEVICE::Close_Server(void){
#ifdef Commu_Socket_h
	if (cgServer != nullptr)
		cgServer->Close();
#endif
};
//------------------------------------------------------------------------------------------//
ACOM* 			CORE_DEVICE::ACom			(void)const{return(cgCom);};
ASOCKET* 		CORE_DEVICE::ASocket		(void)const{return(cgSocket);};
ASOCKETSERVER*	CORE_DEVICE::AServer		(void)const{return(cgServer);};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
ExpandDeviceAttr::ExpandDeviceAttr(void){
	SetOpenPar(&aCOM	,OPEN_COM ,"/dev/ttyUSB0"	,115200	,0);
	SetOpenPar(&aTCP	,OPEN_TCP ,"leifwen.com"	,9527	,0);
	SetOpenPar(&aUDP	,OPEN_UDP ,"127.0.0.1"		,8192	,0);
	SetOpenPar(&aTCPS	,OPEN_TCPS,""				,9527	,0);
	SetOpenPar(&aUDPS	,OPEN_UDPS,""				,8192	,0);
	openType = OPEN_None;
	name = "";
	device = nullptr;
	envcfg = nullptr;
};
//------------------------------------------------------------------------------------------//
typedef ExpandDeviceAttr EDA;
bool32			EDA::IsOpened		(void)const{return(device->IsOpened());};
uint32			EDA::DevType		(void)const{return(device->Core()->GetOpenPar().type);};
#ifdef Commu_Com_h
ACOM* 			EDA::ACom			(void)const{return(device->ACom());};
CORE_ACOM* 		EDA::AComCore		(void)const{return((CORE_ACOM*)device->ACom()->Core());};
#endif
#ifdef Commu_Socket_h
ASOCKET*		EDA::ASocket		(void)const{return(device->ASocket());};
ASOCKETSERVER*	EDA::AServer		(void)const{return(device->AServer());};
#endif
bool32			EDA::IsCom			(void)const{return((device->Core()->GetOpenPar().type == OPEN_COM) || (device->Core()->GetOpenPar().type == OPEN_COMV));};
bool32			EDA::IsSocket		(void)const{return((device->Core()->GetOpenPar().type == OPEN_TCP) || (device->Core()->GetOpenPar().type == OPEN_UDP));};
bool32			EDA::IsServer		(void)const{return((device->Core()->GetOpenPar().type == OPEN_TCPS) || (device->Core()->GetOpenPar().type == OPEN_UDPS));};
bool32			EDA::IsComOpened	(void)const{return((device->IsOpened()) && IsCom());};
bool32			EDA::IsSocketOpened	(void)const{return((device->IsOpened()) && IsSocket());};
bool32			EDA::IsServerOpened	(void)const{return((device->IsOpened()) && IsServer());};
bool32			EDA::IsTCPOpened	(void)const{return((device->IsOpened()) && (device->Core()->GetOpenPar().type == OPEN_TCP));};
bool32			EDA::IsUCPOpened	(void)const{return((device->IsOpened()) && (device->Core()->GetOpenPar().type == OPEN_UDP));};
bool32			EDA::IsTCPSOpened	(void)const{return((device->IsOpened()) && (device->Core()->GetOpenPar().type == OPEN_TCPS));};
bool32			EDA::IsUDPSOpened	(void)const{return((device->IsOpened()) && (device->Core()->GetOpenPar().type == OPEN_UDPS));};
//------------------------------------------------------------------------------------------//
ODEV_VG3D& EDA::GetVG3D(void){
	COMMU_NODE 	*l1;
	ODEV_VG3D	*node;
	
	l1 = COMMU_NODE::AcquireSelected(device);
	
	node = COMMU_LOGSYS::GetVG3D(static_cast<COMMU_FRAME*>(l1)->unitTeam.logSys);
	
	COMMU_NODE::ReleaseSelected(device);
	
	return(*node);
};
//------------------------------------------------------------------------------------------//
bool32 EDA::Open(void)const{
	bool32	err = G_FALSE;
	switch(openType){
#ifdef SWVERSION_TCPClient
		case OPEN_TCP:
			err = device->Open(aTCP);
			break;
#endif
#ifdef SWVERSION_UDPCLient
		case OPEN_UDP:
			err = device->Open(aUDP);
			break;
#endif
#ifdef SWVERSION_TCPServer
		case OPEN_TCPS:
			err = device->Open(aTCPS);
			break;
#endif
#ifdef SWVERSION_UDPServer
		case OPEN_UDPS:
			err = device->Open(aUDPS);
			break;
#endif
#ifdef SWVERSION_COM
		case OPEN_COM:;
		case OPEN_COMV:
			err = device->Open(aCOM);
			break;
#endif
		default:
			break;
	}
	return(err);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
DEVICE::DEVICE(uint32 rxSize,uint32 txSize,const ODEV_CACHE* cache) : DEVICE_BASE(){
	cgLogSystem.cache = (ODEV_CACHE*)cache;
	
	DEVICE_BASE::Init(rxSize,txSize,&cgLogSystem);
	
	cgEDA.device = this;
	cgEDA.envcfg = &cgLogSystem.envcfg;
};
//------------------------------------------------------------------------------------------//
DEVICE::~DEVICE(void){
	Close();
	CleanDownTree(this, this);
};
//------------------------------------------------------------------------------------------//
void DEVICE::CloseChild(COMMU_FRAME *commu){
	CloseSelf(0);
};
//------------------------------------------------------------------------------------------//
ACOM* 				DEVICE::ACom			(void)const	{return(static_cast<CORE_DEVICE*>(Core())->ACom());};
ASOCKET* 			DEVICE::ASocket			(void)const	{return(static_cast<CORE_DEVICE*>(Core())->ASocket());};
ASOCKETSERVER*		DEVICE::AServer			(void)const	{return(static_cast<CORE_DEVICE*>(Core())->AServer());};
ODEV_SYSTEM*		DEVICE::GetLogSystem	(void)const	{return((ODEV_SYSTEM*)&cgLogSystem);};
ExpandDeviceAttr*	DEVICE::EDA				(void)const	{return((ExpandDeviceAttr*)&cgEDA);};
void				DEVICE::SetEnvCFG		(const uint64& flag){B_SetFLAG64(cgLogSystem.envcfg,flag);};
void				DEVICE::ClrEnvCFG		(const uint64& flag){B_ClrFLAG64(cgLogSystem.envcfg,flag);};
//------------------------------------------------------------------------------------------//
OUTPUT_NODE* DEVICE::GetSelSTDOUT(void){
	COMMU_NODE 	*l1;
	COMMU_NODE 	*l2;
	OUTPUT_NODE	*node;
	
	l1 = AcquireSelected(this);
	l2 = AcquireSelected(l1);

	node = COMMU_LOGSYS::GetVG3D(static_cast<COMMU_FRAME*>(l2)->unitTeam.logSys);
	
	ReleaseSelected(l1);
	ReleaseSelected(this);
	
	return(node);
};
//------------------------------------------------------------------------------------------//
uint32 DEVICE::TransformCommand(STDSTR* retForSend,STDSTR* retForPrint,const STDSTR& cmd,CMD_TAIL tail,G_ESCAPE blEscape){
#ifdef BIF_Transform_h
	BIF_Transform(nullptr,retForSend,retForPrint,cmd,tail,blEscape);
#else
	STDSTR	strT;
	Str_CharToASCII(_EMPTY(&strT), (uint8*)cmd.c_str(), cmd.length(), blEscape);
	switch (tail) {
		case CMD_R:
			strT.append("\r");
			break;
		case CMD_N:
			strT.append("\n");
			break;
		case CMD_RN:
			strT.append("\r\n");
		default:
			break;
	}
	
	if (retForPrint != nullptr)
		retForPrint->append(strT);
	retForSend->append(strT);
#endif
	return((uint32)retForSend->length());
};
//------------------------------------------------------------------------------------------//
uint32 DEVICE::SendCommand(const STDSTR& cmd){
	if (IsOpened() && (cmd.length() > 0))
		return(Send(cmd));
	return 0;
};
//------------------------------------------------------------------------------------------//
bool32 DEVICE::SendCommand(const STDSTR& cmd,const STDSTR& strCondition,ARRAY* array,uint32 waitMS,uint32 delyMS){
	
	array->Empty();
	if (IsOpened()){
		if (SendCommand(cmd) > 0)
			return(CheckReceive(strCondition,array,waitMS,delyMS));
	}
	return G_FALSE;
};
//------------------------------------------------------------------------------------------//
bool32 DEVICE::CheckReceive(const STDSTR& strCondition,ARRAY* array,uint32 waitMS,uint32 delyMS){
	
	if (IsOpened()){
#ifdef BIF_Expression_h
		if (BIF_CheckResult(array,strCondition,waitMS) > 0){
			SYS_SleepMS(delyMS);
			return G_TRUE;
		}
#else
		SYS_SleepMS(delyMS);
		return G_TRUE;
#endif
	}
	return G_FALSE;
};
//------------------------------------------------------------------------------------------//
uint32 DEVICE::SendCommand(const STDSTR& cmd,CMD_TAIL tail,G_ESCAPE blEscape){
	STDSTR	strForSend;
	TransformCommand(_EMPTY(&strForSend),nullptr,cmd,tail,blEscape);
	return(SendCommand(strForSend));
};
//------------------------------------------------------------------------------------------//
uint32 DEVICE::SendCommandWithPrint(const STDSTR& cmd,CMD_TAIL tail,G_ESCAPE blEscape){
	STDSTR	strForSend;
	PrintSendCommand(_EMPTY(&strForSend),cmd,tail,blEscape);
	return(SendCommand(strForSend));
};
//------------------------------------------------------------------------------------------//
STDSTR DEVICE::MakeSendTitle(uint32 num){
	STDSTR	title;

	COMMU_NODE 	*l1;
	COMMU_NODE 	*l2;
	
	l1 = AcquireSelected(this);
	l2 = AcquireSelected(l1);
#ifdef Commu_Socket_h
	if (l1 == AServer()){
		title = MakeTxTitle("",static_cast<COMMU_FRAME*>(l2)->Core()->GetDevName(),num
							  ,static_cast<COMMU_FRAME*>(l2)->TxBytes() + num, 0);
	}
	else{
		title = MakeTxTitle(num, static_cast<COMMU_FRAME*>(l2)->TxBytes() + num);
	}
#else
	title = MakeTxTitle(num, static_cast<COMMU_FRAME*>(l2)->TxBytes() + num);
#endif
	ReleaseSelected(l1);
	ReleaseSelected(this);

	return(title);
};
//------------------------------------------------------------------------------------------//
void DEVICE::PrintSendCommand(uint32 num,const STDSTR& strForPrint){
	if (num > 0)
		PrintSendCommand(MakeSendTitle(num),"",strForPrint);
};
//------------------------------------------------------------------------------------------//
STDSTR& DEVICE::PrintSendCommand(STDSTR* retForSend,const STDSTR& cmd,CMD_TAIL tail,G_ESCAPE blEscape){
	STDSTR	strForPrint;
	
	TransformCommand(_EMPTY(retForSend),_EMPTY(&strForPrint),cmd,tail,blEscape);
	if (retForSend->length() > 0)
		PrintSendCommand(MakeSendTitle((uint32)retForSend->length()),"",strForPrint);
	return(*retForSend);
};
//------------------------------------------------------------------------------------------//
void DEVICE::PrintSendCommand(const STDSTR& strTitle1,const STDSTR& strTitle2,const STDSTR& strForPrint){
	Traversal_set();
	OUTPUT_NODE::PrintWithDividingLine(GetSelSTDOUT(),COL_DB_TxTitle,strTitle1,strTitle2,COLOR(COL_DB_TxText,strForPrint));
	Traversal_clr();
};
//------------------------------------------------------------------------------------------//
void DEVICE::PrintSendStrWOG1(const STDSTR& strPrintData){
	Traversal_set();
	*GetSelSTDOUT() << Begin() << ClrGroup(COLRECORD::CRD_G1) << NL() << COL_DB_TxText << strPrintData << Endl();
	Traversal_clr();
};
//------------------------------------------------------------------------------------------//
STDSTR DEVICE::MakeFrmTitle(void){
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
DEVICE_EXE_FRAME::DEVICE_EXE_FRAME(uint32 size) : TNF(){
	cgMaxSize = size;
	Init(nullptr);
	executeThread.ThreadInit(this, &DEVICE_EXE_FRAME::ExecuteThreadFun,"executeThread");
};
//------------------------------------------------------------------------------------------//
DEVICE_EXE_FRAME:: ~DEVICE_EXE_FRAME(void){
	Stop();
};
//------------------------------------------------------------------------------------------//
void DEVICE_EXE_FRAME::Init(const DEVICE* dev){
	cgDevice = (DEVICE*)dev;
	cgSBUF.array.Empty();
	ClrblIsTerminated();
	Disable(this);
};
//------------------------------------------------------------------------------------------//
void DEVICE_EXE_FRAME::Stop(void){
	SetblIsTerminated();
	executeThread.ThreadStop();
	Init(nullptr);
};
//------------------------------------------------------------------------------------------//
bool32 DEVICE_EXE_FRAME::IsExecuting(const DEVICE* dev){
	if (dev == nullptr)
		return G_FALSE;
	if (IsStop() == G_FALSE)
		return G_FALSE;
	return(cgRunLock.TryLock());
};
//------------------------------------------------------------------------------------------//
bool32 DEVICE_EXE_FRAME::Execute(const DEVICE* dev,SYS_AThread* thread){
	SMC_EncryptI(0)
	Stop();
	Init(dev);
	Enable(this);
	
	SMC_EncryptS(0)
	executeThread.ThreadRun(thread);
	SMC_EncryptE(0)
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
bool32 DEVICE_EXE_FRAME::ExecuteThreadFun(void* p){
	ThreadType* thread = static_cast<ThreadType*>(p);
	cgSBUF.array.InitSize(cgMaxSize);
	cgDevice->RxDataShareTo(&cgSBUF);
	
	thread->Execute(nullptr);
	
	SYS_SleepMS(10);
	cgSBUF.RemoveSelf();
	Init(nullptr);
	cgRunLock.Unlock();
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
void DEVICE_EXE_FRAME::SetblIsTerminated(void){
	SetSFlag(blIsTerminated);
};
//------------------------------------------------------------------------------------------//
void DEVICE_EXE_FRAME::ClrblIsTerminated(void){
	ClrSFlag(blIsTerminated);
};
//------------------------------------------------------------------------------------------//
bool32 DEVICE_EXE_FRAME::IsTerminated(void)const{
	return(CheckSFlag(blIsTerminated));
};
//------------------------------------------------------------------------------------------//
bool32 DEVICE_EXE_FRAME::IsStop(void){
	return(IsEnable(this) == G_FALSE);
};
//------------------------------------------------------------------------------------------//
bool32 DEVICE_EXE_FRAME::IsServiceTo(const DEVICE* dev){
	return(dev == cgDevice);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif /* Device_h */
