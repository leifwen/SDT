//
//  Commu_SSL.hpp
//  SDT
//
//  Created by Leif Wen on 13/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#ifndef Commu_SSL_hpp
#define Commu_SSL_hpp
//------------------------------------------------------------------------------------------//
#include "Commu_SSL.h"
#ifdef Commu_SSL_h
//------------------------------------------------------------------------------------------//
template <typename T_MSG> MEM_MSG<T_MSG>::MEM_MSG(void) : MEM_SSL(){
	
	msgCenter.Register(&cgMsg);
	SetUpName("");
};
//------------------------------------------------------------------------------------------//
template <typename T_MSG> inline void MEM_MSG<T_MSG>::SetSelfName(const STDSTR& strName){
	MEM_SSL::SetSelfName(strName);
	cgMsg.SetUpName(GetFullName(this));};
//------------------------------------------------------------------------------------------//
template <typename T_MSG> inline void MEM_MSG<T_MSG>::SetUpName(const STDSTR& strName){
	MEM_SSL::SetUpName(strName);
	cgMsg.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
template <typename T_MSG> MSG_NODE* MEM_MSG<T_MSG>::GetMSG(CMDID mID){
	MSG_NODE* ret = nullptr;
	if (cgMsg.GetMSGID() == mID){
		ret = &cgMsg;
	}
	else{
		TREE_DownChain_Traversal_LINE_nolock
		(MSG_NODE,(&cgMsg),
			if (_opNode->GetMSGID() == mID){
				ret = _opNode;
				break;
			}
		);
	}
	return(ret);
};
//------------------------------------------------------------------------------------------//
#endif /* Commu_SSL_h */
#endif /* Commu_SSL_hpp */
