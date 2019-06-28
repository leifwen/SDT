//
//  Commu_Terminal.cpp
//  SDT
//
//  Created by Leif Wen on 2019/5/15.
//  Copyright © 2019 Leif Wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "Commu_Terminal.h"
#ifdef Commu_Terminal_h
//------------------------------------------------------------------------------------------//
CORE_TSOCKET::CORE_TSOCKET(void) : CORE_SOCKET(){
	BICThread.ThreadInit(this, &CORE_TSOCKET::BICThreadFun,"BICThread");
	BICThread.ThreadInit(&CORE_TSOCKET::BICThreadOnStop);
	
	cgBIC = nullptr;
	
	SetSelfName("CORE_TSOCKET");
	SetUpName("");
};
//------------------------------------------------------------------------------------------//
CORE_TSOCKET::~CORE_TSOCKET(void){
	CloseDev();
	BICThread.RemoveSelf();
};
//------------------------------------------------------------------------------------------//
void CORE_TSOCKET::SetSelfName(const STDSTR& strName){
	CORE_SOCKET::SetSelfName(strName);
	BICThread.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
void CORE_TSOCKET::SetUpName(const STDSTR& strName){
	CORE_SOCKET::SetUpName(strName);
	BICThread.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
void CORE_TSOCKET::Init(const COMMU_TEAM* _team){
	CORE_SOCKET::Init(_team);
	unitTeam->commu->ThreadAccept(&BICThread);
};
//------------------------------------------------------------------------------------------//
void CORE_TSOCKET::InitBIC(const CMD_ENV* env,const BIC_BASE* bic){
	if (env != nullptr)
		cgENV.CopyInit(env);
	cgBIC = (BIC_BASE*)bic;
};
//------------------------------------------------------------------------------------------//
void CORE_TSOCKET::CloseDev(void){
	CMD_BASE::SetblExit(&cgENV);
	CORE_SOCKET::CloseDev();
};
//------------------------------------------------------------------------------------------//
bool32 CORE_TSOCKET::DoBICThreadFun(void* _team){
	COMMU_TEAM*		team = static_cast<COMMU_TEAM*>(_team);
	COMMU_FRAME*	commu = team->commu;
	COMMU_LOGSYS*	logSys = team->logSys;
	COMMU_MEM*		mem = team->mem;

#ifdef Output_h
	COMMU_LOGSYS::CreateODev_G2Self(logSys,commu,BIC_ENV::GetCache(&cgENV));

	CMD_ENV::SetSTDOUT(&cgENV, COMMU_LOGSYS::GetG2DefSelf(logSys));
	BIC_ENV::SetSTDIN(&cgENV, COMMU_MEM::GetArrayRx(mem));
	
	cgENV.ClearEnvAllFlag();
	cgBIC->Command(&cgENV,"",nullptr);
	
	COMMU_LOGSYS::GetG2DefSelf(logSys)->RemoveSelf();
#endif
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
bool32 CORE_TSOCKET::BICThreadOnStop(void* _team){
	CMD_NODE::SetblExit(&cgENV);
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
bool32 CORE_TSOCKET::BICThreadFun(void* _team){
	COMMU_TEAM*		team = static_cast<COMMU_TEAM*>(_team);
	COMMU_FRAME*	commu = team->commu;
	
	DoBICThreadFun(_team);
	if (TNF::IsEnable(&BICThread))
		commu->CloseSelf(0);
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
#endif
