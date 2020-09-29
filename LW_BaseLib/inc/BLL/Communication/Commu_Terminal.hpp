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
template <typename T_CDEV>
TServer_T<T_CDEV>::TServer_T(void) : CDRV_POOL_T<COMMU_DRV_T<0,COMMU_DRV,CORE_SOCKETSERVER>,T_CDEV>(){
	cgENV = nullptr;
	cgBIC = nullptr;
	
	TNFP::SetSelfName("TServer");
	this->SetSelfName(this->selfName);
	this->SetUpName("");
};
//------------------------------------------------------------------------------------------//
template <typename T_CDEV>
void TServer_T<T_CDEV>::InitBIC(uint32 size,const CMD_ENV* env,const BIC_BASE* bic){
	this->Init(size,size,nullptr);
	cgENV = (CMD_ENV*)env;
	cgBIC = (BIC_BASE*)bic;
};
//------------------------------------------------------------------------------------------//
template <typename T_CDEV>
TNF* TServer_T<T_CDEV>::CreateNode(void){
	T_CDEV	*cdrv;
	cdrv = static_cast<T_CDEV*>(TNFP::SetNodeUpName(new T_CDEV(this->cgMaxRxArraySize,this->cgMaxTxArraySize,nullptr)));
	if (cdrv != nullptr)
		static_cast<CORE_TSOCKET*>(cdrv->Core())->InitBIC(cgENV,cgBIC);
	return(cdrv);
};
//------------------------------------------------------------------------------------------//
#endif /* Commu_Terminal_h */
#endif /* Commu_Terminal_hpp */
