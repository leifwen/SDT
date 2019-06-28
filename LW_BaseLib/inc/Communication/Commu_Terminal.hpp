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
template <typename T_COMMU>
FRAME_TServer<T_COMMU>::FRAME_TServer(void) : COMMU_POOL<COMMU<0,COMMU_FRAME,CORE_SOCKETSERVER>,T_COMMU>(){
	cgENV = nullptr;
	cgBIC = nullptr;
	
	TNFP::SetSelfName("TServer");
	this->SetSelfName(this->selfName);
	this->SetUpName("");
};
//------------------------------------------------------------------------------------------//
template <typename T_COMMU>
void FRAME_TServer<T_COMMU>::InitBIC(uint32 size,const CMD_ENV* env,const BIC_BASE* bic){
	this->Init(size,size,nullptr);
	cgENV = (CMD_ENV*)env;
	cgBIC = (BIC_BASE*)bic;
};
//------------------------------------------------------------------------------------------//
template <typename T_COMMU>
TNFP* FRAME_TServer<T_COMMU>::CreateNode(void){
	T_COMMU	*commu;
	commu = static_cast<T_COMMU*>(TNFP::SetNodeUpName(new T_COMMU(this->cgMaxRxArraySize,this->cgMaxTxArraySize,nullptr)));
	if (commu != nullptr)
		static_cast<CORE_TSOCKET*>(commu->Core())->InitBIC(cgENV,cgBIC);
	return(commu);
};
//------------------------------------------------------------------------------------------//
#endif /* Commu_Terminal_h */
#endif /* Commu_Terminal_hpp */
