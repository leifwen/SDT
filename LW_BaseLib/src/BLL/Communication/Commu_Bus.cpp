//
//  Commu_Bus.cpp
//  SDT
//
//  Created by Leif Wen on 13/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "Commu_Bus.h"
#ifdef Commu_Bus_h
//------------------------------------------------------------------------------------------//
#include "ADS_SMC.h"
#include "BIF_Transform.h"
#include "BIF_Expression.h"
//------------------------------------------------------------------------------------------//
CORE_DEVBUS::CORE_DEVBUS(void) : COMMU_CORE(){
	SetSelfName("CORE_DEVBUS");
	SetUpName("");
};
//------------------------------------------------------------------------------------------//
CORE_DEVBUS::~CORE_DEVBUS(void){
	CloseDev();
};
//------------------------------------------------------------------------------------------//
TNF* CORE_DEVBUS::CreateNode(void){
	return(new COMMU_DEV());
}
//------------------------------------------------------------------------------------------//
COMMU_DEV* CORE_DEVBUS::IsRegistered(const STDSTR &name){
	COMMU_DEV *dev = nullptr;

	{TREE_DownChain_Find(COMMU_DEV,this,dev,(_opNode->name == name));}

	return(dev);
};
//------------------------------------------------------------------------------------------//
bool32 CORE_DEVBUS::Register(const STDSTR &name, TypeConstructor func){
	COMMU_DEV *dev = nullptr;
	
	dev = IsRegistered(name);
	if (dev == nullptr)
		dev = static_cast<COMMU_DEV*>(GetNewNode());
	if (dev != nullptr){
		dev->name = name;
		dev->Constructor = func;
		AppendDownNode(dev);
	}
	
	return(dev != nullptr);
};
//------------------------------------------------------------------------------------------//
static bool32 DEVBUS_OPEN(COMMU_DRV *drv,const OPEN_PAR& par){
	return(drv->Open(par));
};
//------------------------------------------------------------------------------------------//
bool32 CORE_DEVBUS::OpenDev(const OPEN_PAR& par){
	bool32	ret = G_FALSE;
	SMC_EncryptI(0)
	
	COMMU_DEV *dev = nullptr;
	COMMU_DRV *drv = nullptr;
	
	do{
		{TREE_DownChain_Find(COMMU_DEV,this,dev,(_opNode->name == selfName));}
		if (dev == nullptr)
			break;
		
		drv = static_cast<COMMU_DRV*>(FindInDownChainBySelfName(unitTeam->cdrv, selfName));
		if (drv == nullptr){//create new if here no item in the list
			drv = static_cast<COMMU_DRV*>((*dev->Constructor)	(unitTeam->cdrv->RxArrayMaxSize()
																,unitTeam->cdrv->RxArrayMaxSize()
																,COMMU_ODEVBUS::GetODevBus(unitTeam->odevBus))
										  );
			if (drv != nullptr){
				drv->SetSelfName(selfName);
				drv->SetUpName(GetFullName(unitTeam->cdrv));
				unitTeam->cdrv->AppendChild(drv);
			}
		}
		
		if (drv == nullptr)
			break;
		unitTeam->cdrv->SetChildSelected(drv);
		SMC_EncryptS(0)
		ret = DEVBUS_OPEN(drv,par);
		SMC_EncryptE(0)
	}while(0);
	return ret;
};
//------------------------------------------------------------------------------------------//
void CORE_DEVBUS::CloseDev(void){
	COMMU_DRV *drv = nullptr;
	drv = static_cast<COMMU_DRV*>(FindInDownChainBySelfName(unitTeam->cdrv, selfName));
	if (drv != nullptr)
		drv->Close();
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
COMMU_DEVBUS::COMMU_DEVBUS(uint32 rxSize,uint32 txSize,const ODEV_CACHE* cache) : _COMMU_DEVBUS(){
	cgODevBus.cache = (ODEV_CACHE*)cache;
	
	_COMMU_DEVBUS::Init(rxSize,txSize,&cgODevBus);
};
//------------------------------------------------------------------------------------------//
COMMU_DEVBUS::~COMMU_DEVBUS(void){
	Close();
	CleanDownTree(this, this);
};
//------------------------------------------------------------------------------------------//
bool32 COMMU_DEVBUS::Open(const STDSTR& drvName,const OPEN_PAR& par){
	bool32 err = G_FALSE;

	if (cgOpenLock.Try()){
		if (SetTo(drvName) != G_FALSE)
			err = _COMMU_DEVBUS::Open(par);
		cgOpenLock.Clr();
	}
	return err;
};
//------------------------------------------------------------------------------------------//
void COMMU_DEVBUS::CloseChild(COMMU_DRV *cdrv){
	CloseSelf(0);
};
//------------------------------------------------------------------------------------------//
bool32 COMMU_DEVBUS::Register(const STDSTR &name, TypeConstructor func){
	CORE_DEVBUS *_core = static_cast<CORE_DEVBUS*>(Core());
	return(_core->Register(name, func));
};
//------------------------------------------------------------------------------------------//
bool32 COMMU_DEVBUS::SetTo(const STDSTR& drvName){
	while(IsOpened() == G_FALSE){
		if (static_cast<CORE_DEVBUS*>(Core())->IsRegistered(drvName) == nullptr)
			break;
		Core()->SetSelfName(drvName);
		return G_TRUE;
	};
	return G_FALSE;
};
//------------------------------------------------------------------------------------------//
COMMU_DRV* COMMU_DEVBUS::IsFor(const STDSTR& drvName){
	if (Core()->selfName == drvName){
		DRV_NODE * drv;
		drv = AcquireSelected(this);
		ReleaseSelected(this);
		return (static_cast<COMMU_DRV*>(drv));
	}
	return nullptr;
};
//------------------------------------------------------------------------------------------//
COMMU_DRV* COMMU_DEVBUS::GetDrv(const STDSTR& drvName){
	return (static_cast<COMMU_DRV*>(FindInDownChainBySelfName(this, drvName)));
};
//------------------------------------------------------------------------------------------//
ODEV_BUS*	COMMU_DEVBUS::GetODevBus	(void)const	{return((ODEV_BUS*)&cgODevBus);};
void		COMMU_DEVBUS::SetEnvCFG		(const uint64& flag){B_SetFLAG64(cgODevBus.envcfg,flag);};
void		COMMU_DEVBUS::ClrEnvCFG		(const uint64& flag){B_ClrFLAG64(cgODevBus.envcfg,flag);};
//------------------------------------------------------------------------------------------//
OUTPUT_NODE* COMMU_DEVBUS::GetSelSTDOUT(void){
	DRV_NODE 	*l1;
	DRV_NODE 	*l2;
	OUTPUT_NODE	*node;
	
	l1 = AcquireSelected(this);
	l2 = AcquireSelected(l1);

	node = COMMU_ODEVBUS::GetVG3D(static_cast<COMMU_DRV*>(l2)->unitTeam.odevBus);
	
	ReleaseSelected(l1);
	ReleaseSelected(this);
	
	return(node);
};
//------------------------------------------------------------------------------------------//
uint32 COMMU_DEVBUS::TransformCommand(STDSTR* retForSend,STDSTR* retForPrint,const STDSTR& cmd,CMD_TAIL tail,G_ESCAPE blEscape){
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
uint32 COMMU_DEVBUS::SendCommand(const STDSTR& cmd){
	if (IsOpened() && (cmd.length() > 0))
		return(Send(cmd,0));
	return 0;
};
//------------------------------------------------------------------------------------------//
bool32 COMMU_DEVBUS::SendCommand(const STDSTR& cmd,const STDSTR& strCondition,ARRAY* array,uint32 waitMS,uint32 delyMS){
	
	array->Empty();
	if (IsOpened()){
		if (SendCommand(cmd) > 0)
			return(CheckReceive(strCondition,array,waitMS,delyMS));
	}
	return G_FALSE;
};
//------------------------------------------------------------------------------------------//
bool32 COMMU_DEVBUS::CheckReceive(const STDSTR& strCondition,ARRAY* array,uint32 waitMS,uint32 delyMS){
	
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
uint32 COMMU_DEVBUS::SendCommand(const STDSTR& cmd,CMD_TAIL tail,G_ESCAPE blEscape){
	STDSTR	strForSend;
	TransformCommand(_EMPTY(&strForSend),nullptr,cmd,tail,blEscape);
	return(SendCommand(strForSend));
};
//------------------------------------------------------------------------------------------//
#endif /* Commu_Bus_h */
