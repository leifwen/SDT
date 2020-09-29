//
//  Commu_AEXE.cpp
//  SDT
//
//  Created by Leif Wen on 13/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "Commu_SSL.h"
#ifdef Commu_SSL_h
//------------------------------------------------------------------------------------------//
//#define LOGPRINT_ENABLE
//#define LOGTHREAD_ENABLE
#include "SYS_Log.h"
//------------------------------------------------------------------------------------------//
MEM_SSL::MEM_SSL(void) : COMMU_MEM(){
	t1DataRxThread.ThreadInit(this,&MEM_SSL::T1DataRxThreadFunc	,"T1DRx");
	msgCenter.Register(&msgHandshake);
	
	cgSSLDev.cgT0.SetPackageDlyMS(CSSL_T0::MAX_PACKAGE_DEALYMS * 3);
	cgT1 = nullptr;
	
	SetSelfName				("MEM_SSL");
	cgSSLDev.SetSelfName	("DEV");
	msgCenter.SetSelfName	("msg");
	msgHandshake.SetSelfName("HS");
	SetUpName("");
};
//------------------------------------------------------------------------------------------//
MEM_SSL::~MEM_SSL(void){
	t1DataRxThread.RemoveSelf();
};
//------------------------------------------------------------------------------------------//
void MEM_SSL::SetSelfName(const STDSTR& strName){
	COMMU_MEM::SetSelfName(strName);
	cgSSLDev.SetUpName(GetFullName(this));
	t1DataRxThread.SetUpName(GetFullName(this));
	msgCenter.SetUpName(GetFullName(this));
	msgHandshake.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
void MEM_SSL::SetUpName(const STDSTR& strName){
	COMMU_MEM::SetUpName(strName);
	cgSSLDev.SetUpName(GetFullName(this));
	t1DataRxThread.SetUpName(GetFullName(this));
	msgCenter.SetUpName(GetFullName(this));
	msgHandshake.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
void MEM_SSL::Init(const COMMU_TEAM* _team){
	COMMU_MEM::Init(_team);
	unitTeam->cdrv->ThreadAccept(&t1DataRxThread);
	msgCenter.Init((void*)_team,&cgMSGENV);
	cgMSGENV.ClrblExit();
	msgCenter.Reset((void*)_team);
};
//------------------------------------------------------------------------------------------//
void MEM_SSL::InitArraySize(uint32 rxSize,uint32 txSize){
	arrayRx.InitSize(CSSL_T0::MAX_PACKAGE_SIZE * 16);
	arrayTx.InitSize(txSize);
	cgSSLRx.InitSize(rxSize);

	cgSSLDev.Init(&arrayTx,&arrayRx);
	if (cgT1 == nullptr){
		cgT1 = cgSSLDev.AcquireT1(MEM_MAXPACKAGE, MEM_MAXPAGES, MEM_MAXDLYMS, MEM_MAXTIMES);
		cgT1->SetSelfName("T1D");
	}

	msgCenter.Init(rxSize, txSize);
	msgCenter.Init(&cgSSLDev, MEM_MAXPACKAGE, MEM_MAXPAGES);
	msgCenter.SetDelayPar(MEM_MAXDLYMS, MEM_MAXTIMES);
};
//------------------------------------------------------------------------------------------//
void MEM_SSL::AfterReadFromDevice(void){
	cgSSLDev.cgT0.Delivery();
};
//------------------------------------------------------------------------------------------//
bool32 MEM_SSL::Start(void){
	if ((unitTeam->core->GetOpenPar().type == OPEN_TCP) || (unitTeam->core->GetOpenPar().type == OPEN_UDP)){
		if (msgHandshake.Send_REQ_Handshake() == G_FALSE)
			return G_FALSE;
	}
	return(msgHandshake.HandshakeCheck(&cgMSGENV));
};
//------------------------------------------------------------------------------------------//
void MEM_SSL::Stop(void){
	ELogLock(1);
	ELog(LogTitle(this,"MEM_SSL","Stop","::") << "IsConnected() = "		<< unitTeam->core->IsConnected());
	ELog(LogTitle(this,"MEM_SSL","Stop","::") << "IsCloseDueToOS() = "	<< unitTeam->cdrv->IsCloseDueToOS());
	ELog(LogTitle(this,"MEM_SSL","Stop","::") << "HandshakeCheck() = "	<< msgCenter.ChkblREQHS());
	ELog(LogTitle(this,"MEM_SSL","Stop","::") << "IsREQClose() = "		<< msgCenter.ChkblREQClose());
	ELogLock(0);
	
	if (unitTeam->core->IsConnected() && (unitTeam->cdrv->IsCloseDueToOS() == G_FALSE)
		&& msgCenter.ChkblREQHS()
		&& (msgCenter.ChkblREQClose() == G_FALSE)){
		ELog(LogTitle(this,"MEM_SSL","Stop","::") << "Call SendREQ_Close()");
		msgHandshake.Send_REQ_Close(&cgMSGENV);
	}
	cgMSGENV.SetblExit();
	msgCenter.Close(unitTeam);
	if (cgT1 != nullptr)
		cgT1->Close();
};
//------------------------------------------------------------------------------------------//
ARRAY* MEM_SSL::GetArrayRx_original(void){
	return(&cgSSLRx);
};
//------------------------------------------------------------------------------------------//
uint32 MEM_SSL::Send(const ABORTCHECK& _ac,const UVIn&  _in,uint32 timeoutMS){
	IOS	ios;
	IOS_clr(&ios);
	if (cgT1 != nullptr)
		cgT1->Send(_ac, &ios, _in);
	return((uint32)ios.total_in);
};
//------------------------------------------------------------------------------------------//
uint32 MEM_SSL::Read(const ABORTCHECK& _ac,const UVOut& _out,uint32 num){
	IOS	ios;
	IOS_clr(&ios);
	do{
		if (cgSSLRx.Used() > 0){
			DS_IO_NODE::GetDSIOList().Save(&ios, _out, IUD(cgSSLRx,num,0));
			cgSSLRx.Out(num - (uint32)ios.avail_in);
			num = (uint32)ios.avail_in;
		}
	}while((ios.avail_in > 0) && (_ac.func != nullptr) && (_ac.func(_ac.p) == G_FALSE));
	return((uint32)ios.total_out);
};
//------------------------------------------------------------------------------------------//
bool32 MEM_SSL::T1DataRxThreadFunc(void* _team){
	ETLogThreadStart(t1DataRxThread);
#ifdef LOGTHREAD_ENABLE
	IOS		ios;
	ALG_MD5	md5;
	STDSTR	str_md5;
#endif

	while(t1DataRxThread.IsTerminated() == G_FALSE){
#ifndef LOGTHREAD_ENABLE
		if (cgT1 != nullptr)
			cgT1->Read(ACF(CommuIsClosed,_team),nullptr,&cgSSLRx);
#else
		md5.InitCFG();
		if ((cgT1 != nullptr) && (cgT1->Read(ACF(CommuIsClosed,_team),IOS_clr(&ios),OUD(&md5,&cgSSLRx)) > 0)){
			md5.GetResult(nullptr, OUD_HEXs(_EMPTY(&str_md5)));
			ELogLock(1);
			ELog(ELogThread(t1DataRxThread,"Data length=" << ios.total_out << " ,Receive MD5=" << str_md5));
			ELog(ELogThread(t1DataRxThread,"ios:in/out=" << ios.total_in << "/" << ios.total_out));
			ELogLock(0);
		}
#endif
		SYS_SleepMS(2);
	};
	ETLogThreadStop(t1DataRxThread);
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
void MEM_SSL::ResetT0T1(void* _team){
	cgSSLDev.cgT0.SetPackageDlyMS(CSSL_T0::MAX_PACKAGE_DEALYMS * 3);
	cgSSLDev.cgT0.Reset();
	if (cgT1 != nullptr)
		cgT1->Reset();
};
//------------------------------------------------------------------------------------------//
void MEM_SSL::EmptyArray(void){
	COMMU_MEM::EmptyArray();
	cgSSLRx.Empty();
	if (cgT1 != nullptr)
		cgT1->Empty();
};
//------------------------------------------------------------------------------------------//
void MEM_SSL::Reset(void* _team){
	msgCenter.Reset(_team);
};
//------------------------------------------------------------------------------------------//
void MEM_SSL::Empty(void){
	msgCenter.Empty();
};
//------------------------------------------------------------------------------------------//
void MEM_SSL::SetConfig(uint32 cfg,const STDSTR& sKey){
	cgSSLDev.cgT0.SetConfig(cfg,sKey);
};
//------------------------------------------------------------------------------------------//
void MEM_SSL::SetKey(const STDSTR& sKey){
	cgSSLDev.cgT0.SetKey(sKey);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif /* Commu_SSL_h */
