//
//  Device.cpp
//  SDT
//
//  Created by Leif Wen on 13/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
#include "Device.h"
#ifdef Device_h
//------------------------------------------------------------------------------------------//
#include "ADS_SMC.h"
#include "BIF_Transform.h"
#include "BIF_Expression.h"
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
uint32			EDA::DevType		(void)const{return(device->GetOpenPar().type);};
ACOM* 			EDA::ACom			(void)const{return(device->ACom());};
ASOCKET*		EDA::ASocket		(void)const{return(device->ASocket());};
ASOCKETSERVER*	EDA::AServer		(void)const{return(device->AServer());};
bool32			EDA::IsCom			(void)const{return((device->GetOpenPar().type == OPEN_COM) || (device->GetOpenPar().type == OPEN_COMV));};
bool32			EDA::IsSocket		(void)const{return((device->GetOpenPar().type == OPEN_TCP) || (device->GetOpenPar().type == OPEN_UDP));};
bool32			EDA::IsServer		(void)const{return((device->GetOpenPar().type == OPEN_TCPS) || (device->GetOpenPar().type == OPEN_UDPS));};
bool32			EDA::IsComOpened	(void)const{return((device->IsOpened()) && IsCom());};
bool32			EDA::IsSocketOpened	(void)const{return((device->IsOpened()) && IsSocket());};
bool32			EDA::IsServerOpened	(void)const{return((device->IsOpened()) && IsServer());};
bool32			EDA::IsTCPOpened	(void)const{return((device->IsOpened()) && (device->GetOpenPar().type == OPEN_TCP));};
bool32			EDA::IsUCPOpened	(void)const{return((device->IsOpened()) && (device->GetOpenPar().type == OPEN_UDP));};
bool32			EDA::IsTCPSOpened	(void)const{return((device->IsOpened()) && (device->GetOpenPar().type == OPEN_TCPS));};
bool32			EDA::IsUDPSOpened	(void)const{return((device->IsOpened()) && (device->GetOpenPar().type == OPEN_UDPS));};
ODEV_VG3D&		EDA::GetVG3D		(void){return(*static_cast<COMMU_FRAME_LOGSYS*>(device->GetSelDB())->GetVG3D());};
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
namespace _DF {
	template <typename T_FUN,typename T_RET> static inline	T_RET& RunFunInLock(DBUF* db,T_FUN fun,T_RET* ret){
		db->InUse_set();
		*ret = (db->GetSelDB()->GetSelDB()->*fun)();
		db->InUse_clr();
		return(*ret);
	};
	//------------------------------------------------------------------------------------------//
	template <typename T_FUN> static inline void RunFunInLock(DBUF* db,T_FUN fun){
		db->InUse_set();
		(db->GetSelDB()->GetSelDB()->*fun)();
		db->InUse_clr();
	};
};
//------------------------------------------------------------------------------------------//
inline uint32 DEVICE::SSend(const UVIn& _in){
	uint32 ret;
	InUse_set();
	ret = GetSelDB()->GetSelDB()->Put(nullptr,_in);
	InUse_clr();
	return(ret);
};
//------------------------------------------------------------------------------------------//
inline uint32 DEVICE::SRead(const UVOut& _out,uint32 num){
	uint32 ret;
	InUse_set();
	ret = GetSelDB()->GetSelDB()->Get(nullptr,_out,num);
	InUse_clr();
	return(ret);
};
//------------------------------------------------------------------------------------------//
void	DEVICE::SEmpty			(void){return(_DF::RunFunInLock(GetSelDB(),&DEVICE::Empty));};
uint32	DEVICE::SRxSBUFMaxSize	(void){uint32 ret;return(_DF::RunFunInLock(this,&DEVICE::RxSBUFMaxSize,&ret));};
uint32	DEVICE::STxSBUFMaxSize	(void){uint32 ret;return(_DF::RunFunInLock(this,&DEVICE::TxSBUFMaxSize,&ret));};
uint32	DEVICE::SUnreadBytes	(void){uint32 ret;return(_DF::RunFunInLock(this,&DEVICE::UnreadBytes,&ret));};
uint32	DEVICE::SUnsentBytes	(void){uint32 ret;return(_DF::RunFunInLock(this,&DEVICE::UnsentBytes,&ret));};
uint64	DEVICE::SFwBytes		(void){uint64 ret;return(_DF::RunFunInLock(this,&DEVICE::FwBytes,&ret));};
uint64	DEVICE::SRxBytes		(void){uint64 ret;return(_DF::RunFunInLock(this,&DEVICE::RxBytes,&ret));};
uint64	DEVICE::STxBytes		(void){uint64 ret;return(_DF::RunFunInLock(this,&DEVICE::TxBytes,&ret));};
//------------------------------------------------------------------------------------------//
DEVICE::DEVICE(uint32 size,const ODEV_CACHE* cache) : COMMU_FRAME_LOGSYS(size,nullptr){
	SetSFlag(CF_blNoInitSize);

	cgLogSystem.cache = (ODEV_CACHE*)cache;
	InitLogSys(&cgLogSystem);
	
	cgCom = nullptr;
	cgSocket = nullptr;
	cgServer = nullptr;
	cgEDA.device = this;
	cgEDA.envcfg = &cgLogSystem.envcfg;

	TNFP::SetSelfName("DEVICE");
	SetFatherName("");
};
//------------------------------------------------------------------------------------------//
DEVICE::~DEVICE(void){
	Close();
	CleanChild(this, this);
};
//------------------------------------------------------------------------------------------//
bool32 DEVICE::OpenDev(const OPEN_PAR& par){
	bool32	ret;
	SMC_EncryptI(0)
	SMC_EncryptI(1)
	SMC_EncryptI(2)
	SMC_EncryptI(3)
	SMC_EncryptI(4)
	
	ret = 0;
	cgEDA.openType = par.type;
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
}
//------------------------------------------------------------------------------------------//
void DEVICE::CloseDev(void){
	Close_Com();
	Close_Socket();
	Close_Server();
	ChildClrSel(GetSelDB());
}
//------------------------------------------------------------------------------------------//
void DEVICE::CloseChild(COMMU_FRAME *commu){
	DoSelfClose();
};
//------------------------------------------------------------------------------------------//
void DEVICE::EnableEcho(void){
	COMMU_FRAME_LOGSYS::EnableEcho();
	if (GetSelDB() != this)
		static_cast<COMMU_FRAME*>(GetSelDB())->EnableEcho();
};
//------------------------------------------------------------------------------------------//
void DEVICE::DisableEcho(void){
	COMMU_FRAME_LOGSYS::DisableEcho();
	if (GetSelDB() != this)
		static_cast<COMMU_FRAME*>(GetSelDB())->DisableEcho();
};
//------------------------------------------------------------------------------------------//
const ACOM* DEVICE::CreateCOM(void){
#ifdef Commu_Com_h
	if (cgCom == nullptr){
		cgCom = new ACOM(cgMaxSize,&cgLogSystem);
		if (cgCom != nullptr){
			cgCom->UseExternalFwSBufList(&cgInternalFwSBufList);
			cgCom->SetFatherName(fatherName);
		}
		AddChild(cgCom);
	}
	return(cgCom);
#else
	return(nullptr);
#endif
}
//------------------------------------------------------------------------------------------//
const ASOCKET* DEVICE::CreateSocket(void){
#ifdef Commu_Socket_h
	if (cgSocket == nullptr){
		cgSocket = new ASOCKET(cgMaxSize,&cgLogSystem);
		if (cgSocket != nullptr){
			cgSocket->UseExternalFwSBufList(&cgInternalFwSBufList);
			cgSocket->SetFatherName(fatherName);
		}
		AddChild(cgSocket);
	}
	return(cgSocket);
#else
	return(nullptr);
#endif
}
//------------------------------------------------------------------------------------------//
const ASOCKETSERVER* DEVICE::CreateServer(void){
#ifdef Commu_Socket_h
	if (cgServer == nullptr){
		cgServer = new ASOCKETSERVER(cgMaxSize,&cgLogSystem);
		if (cgServer != nullptr){
			cgServer->UseExternalFwSBufList(&cgInternalFwSBufList);
			cgServer->SetFatherName(fatherName);
		}
		AddChild(cgServer);
	}
	return(cgServer);
#else
	return(nullptr);
#endif
}
//------------------------------------------------------------------------------------------//
bool32 DEVICE::Open_Com(const OPEN_PAR& par){
#ifdef Commu_Com_h
	if (CreateCOM() == nullptr)
		return G_FALSE;
	ChildSetSel(cgCom);
	SetOpenPar(&cgEDA.aCOM,par);
	return(cgCom->Open(par));
#else
	return G_FALSE;
#endif
}
//------------------------------------------------------------------------------------------//
void DEVICE::Close_Com(void){
#ifdef Commu_Com_h
	if (cgCom != nullptr)
		cgCom->Close();
#endif
}
//------------------------------------------------------------------------------------------//
bool32 DEVICE::Open_Socket(const OPEN_PAR& par){
#ifdef Commu_Socket_h
	if (CreateSocket() == nullptr)
		return G_FALSE;
	ChildSetSel(cgSocket);
	if (par.type == OPEN_TCP){
		SetOpenPar(&cgEDA.aTCP,par);
	}
	else{
		SetOpenPar(&cgEDA.aUDP,par);
	}
	return(cgSocket->Open(par));
#else
	return G_FALSE;
#endif
}
//------------------------------------------------------------------------------------------//
void DEVICE::Close_Socket(void){
#ifdef Commu_Socket_h
	if (cgSocket != nullptr)
		cgSocket->Close();
#endif
}
//------------------------------------------------------------------------------------------//
bool32 DEVICE::Open_Server(const OPEN_PAR& par){
#ifdef Commu_Socket_h
	if (CreateServer() == nullptr)
		return G_FALSE;
	ChildSetSel(cgServer);
	if (par.type == OPEN_TCPS){
		SetOpenPar(&cgEDA.aTCPS,par);
	}
	else{
		SetOpenPar(&cgEDA.aUDPS,par);
	}
	return(cgServer->Open(par));
#else
	return G_FALSE;
#endif
}
//------------------------------------------------------------------------------------------//
void DEVICE::Close_Server(void){
#ifdef Commu_Socket_h
	if (cgServer != nullptr)
		cgServer->Close();
#endif
}
//------------------------------------------------------------------------------------------//
ACOM* 			DEVICE::ACom	(void)const{return(cgCom);};
ASOCKET* 		DEVICE::ASocket	(void)const{return(cgSocket);};
ASOCKETSERVER*	DEVICE::AServer	(void)const{return(cgServer);};
//------------------------------------------------------------------------------------------//
OUTPUT_NODE* DEVICE::GetSelSTDOUT(void){
	return(static_cast<COMMU_FRAME_LOGSYS*>(GetSelDB()->GetSelDB())->GetVG3D());
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
}
//------------------------------------------------------------------------------------------//
uint32 DEVICE::SendCommand(const STDSTR& cmd){
	if (IsOpened() && (cmd.length() > 0))
		return(SSend(cmd));
	return 0;
}
//------------------------------------------------------------------------------------------//
bool32 DEVICE::SendCommand(const STDSTR& cmd,const STDSTR& strCondition,SBUF* cSBUF,uint32 waitMS,uint32 delyMS){
	
	cSBUF->Empty();
	if (IsOpened()){
		if (SendCommand(cmd) > 0)
			return(CheckReceive(strCondition,cSBUF,waitMS,delyMS));
	}
	return G_FALSE;
}
//------------------------------------------------------------------------------------------//
bool32 DEVICE::CheckReceive(const STDSTR& strCondition,SBUF* cSBUF,uint32 waitMS,uint32 delyMS){
	
	if (IsOpened()){
#ifdef BIF_Expression_h
		if (BIF_CheckResult(cSBUF,strCondition,waitMS) > 0){
			SYS_SleepMS(delyMS);
			return G_TRUE;
		}
#else
		SYS_SleepMS(delyMS);
		return G_TRUE;
#endif
	}
	return G_FALSE;
}
//------------------------------------------------------------------------------------------//
uint32 DEVICE::SendCommand(const STDSTR& cmd,CMD_TAIL tail,G_ESCAPE blEscape){
	STDSTR	strForSend;
	TransformCommand(_EMPTY(&strForSend),nullptr,cmd,tail,blEscape);
	return(SendCommand(strForSend));
}
//------------------------------------------------------------------------------------------//
uint32 DEVICE::SendCommandWithPrint(const STDSTR& cmd,CMD_TAIL tail,G_ESCAPE blEscape){
	STDSTR	strForSend;
	PrintSendCommand(_EMPTY(&strForSend),cmd,tail,blEscape);
	return(SendCommand(strForSend));
};
//------------------------------------------------------------------------------------------//
STDSTR DEVICE::MakeSendTitle(uint32 num){
	STDSTR	title;
#ifdef Commu_Socket_h
	if (GetSelDB() == AServer()){
		title = MakeTxTitle("",static_cast<COMMU_THREAD*>(GetSelDB()->GetSelDB())->GetDevName(),num, STxBytes() + num, 0);
	}
	else{
		title = MakeTxTitle(num, STxBytes() + num);
	}
#else
	title = MakeTxTitle(num, STxBytes() + num);
#endif
	return(title);
}
//------------------------------------------------------------------------------------------//
void DEVICE::PrintSendCommand(uint32 num,const STDSTR& strForPrint){
	if (num > 0)
		PrintSendCommand(MakeSendTitle(num),"",strForPrint);
}
//------------------------------------------------------------------------------------------//
STDSTR& DEVICE::PrintSendCommand(STDSTR* retForSend,const STDSTR& cmd,CMD_TAIL tail,G_ESCAPE blEscape){
	STDSTR	strForPrint;
	
	TransformCommand(_EMPTY(retForSend),_EMPTY(&strForPrint),cmd,tail,blEscape);
	if (retForSend->length() > 0)
		PrintSendCommand(MakeSendTitle((uint32)retForSend->length()),"",strForPrint);
	return(*retForSend);
}
//------------------------------------------------------------------------------------------//
void DEVICE::PrintSendCommand(const STDSTR& strTitle1,const STDSTR& strTitle2,const STDSTR& strForPrint){
	LockChild_set();
	OUTPUT_NODE::PrintWithDividingLine(GetSelSTDOUT(),COL_DB_TxTitle,strTitle1,strTitle2,COLOR(COL_DB_TxText,strForPrint));
	LockChild_clr();
}
//------------------------------------------------------------------------------------------//
void DEVICE::PrintSendStrWOG1(const STDSTR& strPrintData){
	LockChild_set();
	*GetSelSTDOUT() << Begin() << ClrGroup(COLRECORD::CRD_G1) << NL() << COL_DB_TxText << strPrintData << Endl();
	LockChild_clr();
}
//------------------------------------------------------------------------------------------//
STDSTR DEVICE::MakeFrmTitle(void){
	STDSTR	strRet;
	strRet = "";
	switch (GetOpenPar().type){
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
}
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
DEVICE_EXE_FRAME::DEVICE_EXE_FRAME(uint32 size){
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
	cgSBUF.Empty();
	ClrblIsTerminated();
	Disable();
}
//------------------------------------------------------------------------------------------//
void DEVICE_EXE_FRAME::Stop(void){
	SetblIsTerminated();
	executeThread.ThreadStop();
	Init(nullptr);
}
//------------------------------------------------------------------------------------------//
bool32 DEVICE_EXE_FRAME::IsExecuting(const DEVICE* dev){
	if (dev == nullptr)
		return G_FALSE;
	if (IsStop() == G_FALSE)
		return G_FALSE;
	return(InDoing_try());
}
//------------------------------------------------------------------------------------------//
bool32 DEVICE_EXE_FRAME::Execute(const DEVICE* dev,SYS_AThread* thread){
	SMC_EncryptI(0)
	Stop();
	Init(dev);
	Enable();
	
	SMC_EncryptS(0)
	executeThread.ThreadRun(thread);
	SMC_EncryptE(0)
	return G_TRUE;
}
//------------------------------------------------------------------------------------------//
bool32 DEVICE_EXE_FRAME::ExecuteThreadFun(void* p){
	ThreadType* thread = static_cast<ThreadType*>(p);
	cgSBUF.InitSize(cgMaxSize);
	cgDevice->RxDataShareTo(&cgSBUF);
	
	thread->Execute(nullptr);
	
	SYS_SleepMS(10);
	cgSBUF.RemoveSelf();
	Init(nullptr);
	InDoing_clr();
	return G_TRUE;
}
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
bool32 DEVICE_EXE_FRAME::IsStop(void)const{
	return((CheckEnable()) == G_FALSE);
};
//------------------------------------------------------------------------------------------//
bool32 DEVICE_EXE_FRAME::IsServiceTo(const DEVICE* dev){
	return(dev == cgDevice);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif /* Device_h */
