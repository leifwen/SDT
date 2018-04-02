//
//  Commu_Terminal.hpp
//  SDT
//
//  Created by Leif Wen on 17/03/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#ifndef Commu_Terminal_hpp
#define Commu_Terminal_hpp
//------------------------------------------------------------------------------------------//
#include "Commu_Terminal.h"
#ifdef Commu_Terminal_h
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
template <typename T_COMMU> COMMU_SOCKET2<T_COMMU>::COMMU_SOCKET2(uint32 size,const ODEV_SYSTEM* logSys)
: COMMU_SOCKET<T_COMMU>(size,logSys){
	BICThread.ThreadInit(this, &COMMU_SOCKET2::BICThreadFun,"BICThread");
	BICThread.ThreadInit(&COMMU_SOCKET2::BICThreadOnStop);
	this->cgThreadList < BICThread;
	this->SetGetDataByRead();
	
	cgBIC = nullptr;
	
	TNFP::SetSelfName("TSocket");
	SetSelfName(this->selfName);
	SetFatherName("");
};
//------------------------------------------------------------------------------------------//
template <typename T_COMMU> COMMU_SOCKET2<T_COMMU>::COMMU_SOCKET2(uint32 size,const CMD_ENV* env,const BIC_BASE* bic)
: COMMU_SOCKET<T_COMMU>(size,nullptr){
	BICThread.ThreadInit(this, &COMMU_SOCKET2::BICThreadFun,"BICThread");
	BICThread.ThreadInit(&COMMU_SOCKET2::BICThreadOnStop);
	this->cgThreadList < BICThread;
	this->SetGetDataByRead();
	
	if (env != nullptr)
		cgENV.CopyInit(env);
	cgBIC = (BIC_BASE*)bic;
	
	TNFP::SetSelfName("TSocket");
	SetSelfName(this->selfName);
	SetFatherName("");
};
//------------------------------------------------------------------------------------------//
template <typename T_COMMU> COMMU_SOCKET2<T_COMMU>::~COMMU_SOCKET2(void){
	BICThread.RemoveSelf();
};
//------------------------------------------------------------------------------------------//
template <typename T_COMMU> void COMMU_SOCKET2<T_COMMU>::Init(const CMD_ENV* env){
	if (env != nullptr)
		cgENV.CopyInit(env);
};
//------------------------------------------------------------------------------------------//
template <typename T_COMMU> void COMMU_SOCKET2<T_COMMU>::SetSelfName(const STDSTR& strName){
	COMMU_SOCKET<T_COMMU>::SetSelfName(strName);
	BICThread.SetFatherName(this->GetFullName(this));
};
//------------------------------------------------------------------------------------------//
template <typename T_COMMU> void COMMU_SOCKET2<T_COMMU>::SetFatherName(const STDSTR& strName){
	COMMU_SOCKET<T_COMMU>::SetFatherName(strName);
	BICThread.SetFatherName(this->GetFullName(this));
};
//------------------------------------------------------------------------------------------//
template <typename T_COMMU> void COMMU_SOCKET2<T_COMMU>::DoClose(void){
	CMD_BASE::SetblExit(&cgENV);
	COMMU_SOCKET<T_COMMU>::DoClose();
}
//------------------------------------------------------------------------------------------//
template <typename T_COMMU> bool32 COMMU_SOCKET2<T_COMMU>::DoBICThreadFun(void* commu){
#ifdef Output_h
	this->CreateODev_G2Self();
	this->GetG2DefSelf()->RegisterToCache(BIC_ENV::GetCache(&cgENV), COLRECORD::CRD_G2);
	
	CMD_ENV::SetSTDOUT(&cgENV, this->GetG2DefSelf());
	BIC_ENV::SetSTDIN(&cgENV, &this->cgRxSBUF);
	
	cgENV.ClearEnvAllFlag();
	cgBIC->Command(&cgENV,"",nullptr);
	
	this->GetG2DefSelf()->RemoveSelf();
#endif
	return G_TRUE;
}
//------------------------------------------------------------------------------------------//
template <typename T_COMMU> bool32 COMMU_SOCKET2<T_COMMU>::BICThreadOnStop(void* commu){
	CMD_NODE::SetblExit(&cgENV);
	return G_TRUE;
}
//------------------------------------------------------------------------------------------//
template <typename T_COMMU> bool32 COMMU_SOCKET2<T_COMMU>::BICThreadFun(void* commu){
	DoBICThreadFun(commu);
	if (BICThread.CheckEnable())
		this->DoSelfClose();
	return G_TRUE;
}
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
template <typename T_COMMU> COMMU_SOCKETSERVER2<T_COMMU>::COMMU_SOCKETSERVER2(uint32 size,const CMD_ENV* env,const BIC_BASE* bic)
: COMMU_SOCKETSERVER<T_COMMU>(size,nullptr){
	
	cgENV = (CMD_ENV*)env;
	cgBIC = (BIC_BASE*)bic;
	
	TNFP::SetSelfName("TServer");
	this->SetSelfName(this->selfName);
	this->SetFatherName("");
};
//------------------------------------------------------------------------------------------//
template <typename T_COMMU> TNFP* COMMU_SOCKETSERVER2<T_COMMU>::CreateNode(void){
	return(this->SetSubNodeFatherName(new T_COMMU(this->cgMaxSize,cgENV,cgBIC)));
};
//------------------------------------------------------------------------------------------//
#endif /* Commu_Terminal_h */
#endif /* Commu_Terminal_hpp */
