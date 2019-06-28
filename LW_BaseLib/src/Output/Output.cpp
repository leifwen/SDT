//
//  Output.cpp
//  SDT
//
//  Created by Leif Wen on 30/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "Output.h"
//------------------------------------------------------------------------------------------//
#ifdef Output_h
#include "SYS_Time.h"
//#define LOGPRINT_ENABLE
#include "SYS_Log.h"
//------------------------------------------------------------------------------------------//
OUTPUT_CACHE::OUTPUT_CACHE(uint32 size) : CRDC(size){

	outCacheThread.ThreadInit(this, &OUTPUT_CACHE::OutCacheThreadFun,"OUTCACHE");
	
	g2AddrM.InitSize((1 << 12) - 1);
	g3AddrM.InitSize((1 << 9) - 1);
	
	g2AddrM.ClrAllBit();
	g3AddrM.ClrAllBit();
	
	cgLastCol = COL_NONE;
	cgLastCtrl = 0;
	
	SetSelfName("OUTPUT_CACHE");
};
//------------------------------------------------------------------------------------------//
OUTPUT_CACHE::~OUTPUT_CACHE(void){
	Stop();
	UnregisterAll();
	CleanDownTree(&cgONList,&cgONList);
	CleanTrash(&cgONList);
};
//------------------------------------------------------------------------------------------//
void OUTPUT_CACHE::Register(OUTPUT_NODE* oNode,uint32 group){
	uint32 addr,mask;
	
	cgMapLock.Set();
	addr = AssignAddress(group);
	mask = AssignMask(group);
	oNode->SetAddress(this,addr,mask,(group == CRD_G2) ? 0 : CRD_DEFGROUP);
	cgMapLock.Clr();
	
	cgONList.AppendDownNode((TNF*)oNode);
};
//------------------------------------------------------------------------------------------//
void OUTPUT_CACHE::Unregister(OUTPUT_NODE* oNode){
	if (oNode != nullptr){
		cgMapLock.Set();
		RecycleAddress(oNode->GetAddress());
		oNode->SetAddress(nullptr,0,0,0);
		cgMapLock.Clr();
		
		if (oNode->IsDestoryByCache()){
			MoveNodesToTrash(&cgONList,oNode,oNode);
		}
		else{
			DetachUpPriorNext(oNode,oNode);
		}
	}
};
//------------------------------------------------------------------------------------------//
void OUTPUT_CACHE::UnregisterAll(void){
	TREE_DownChain_Traversal_LINE_nolock(OUTPUT_NODE, this, Unregister(_opNode);)
};
//------------------------------------------------------------------------------------------//
void OUTPUT_CACHE::RecycleAddress(uint32 address){
	if (CheckGroup(address,CRD_G2)){
		address &= OC_G2AMASK;
		g2AddrM.ClrOneBit(address);
	}
	else if (CheckGroup(address,CRD_G3)){
		address = (address & OC_G2AMASK) >> 3;
		g3AddrM.ClrOneBit(address);
	}
};
//------------------------------------------------------------------------------------------//
uint32 OUTPUT_CACHE::AssignAddress(uint32 group){
	uint64	ret;
	
	ret = CRD_G1 | 0x0001;
	if (group == CRD_G2){
		ret = g2AddrM.FindFirstZeroBit();
		g2AddrM.SetOneBit(ret);
		ret = (ret & CRD_ADDRMASK) | CRD_G2;
	}
	else if (group == CRD_G3){
		ret = g3AddrM.FindFirstZeroBit();
		g3AddrM.SetOneBit(ret);
		ret = ((ret << 3) & CRD_ADDRMASK) | CRD_G3 | OC_G3Broadcast;
	}
	return((uint32)ret);
};
//------------------------------------------------------------------------------------------//
uint32 OUTPUT_CACHE::AssignMask(uint32 group)const{
	if (group == COLRECORD::CRD_G1)
		return(OC_G1AMASK);
	if (group == COLRECORD::CRD_G2)
		return(OC_G2AMASK);
	return(OC_G3AMASK);
};
//------------------------------------------------------------------------------------------//
IOSE OUTPUT_CACHE::DoTransform(IOS* _ios,const UVOut& _out,const uint8* data,const uint64& length){
	if (_out.uvid == UVID_SELF){
		ELog(LogTitle(this,"OUTPUT_CACHE","Delivery","::") << "ctrl:" << std::hex << cgLastCtrl << " col:" << std::dec << cgLastCol << " L:" << length << ".");
		TREE_DownChain_Traversal_LINE
		(OUTPUT_NODE,&cgONList,
			if (_opNode->CheckPrint(cgLastCtrl)){
				_opNode->Print(cgLastCtrl,cgLastCol,data,(uint32)length);
				_opNode->UpdataLastStatus(cgLastCol,data + length - 1);
			}
		);
		B_ClrFLAG32(cgLastCtrl, CRD_NL);

	}
	else{
		return(CRDC::DoTransform(_ios,_out,data,length));
	}
	return IOS_OK;
};
//------------------------------------------------------------------------------------------//
void OUTPUT_CACHE::Delivery(void){
	uint8	data;
	
	while(TryGetFrame() > 0){
		cgLastCol = (COLORENUM)ReadCOL();
		cgLastCtrl = ReadCtrl();
		Read(nullptr,OUD(this));
		OutRE();
	}
	TREE_DownChain_Traversal_LINE(OUTPUT_NODE,&cgONList,_opNode->Print(0,COL_NONE,&data,0););
};
//------------------------------------------------------------------------------------------//
bool32 OUTPUT_CACHE::OutCacheThreadFun(void* p){
	while(outCacheThread.IsTerminated() == 0){
		Delivery();
		SYS_SleepMS(1);
	}
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
OUTPUT_NODE::OUTPUT_NODE(COLType colType,OUTPUT_CACHE* cache,uint32 group) : CRDN(){
	Init(colType,group);
	RegisterToCache(cache,group);
	SetSelfName("OUTPUT_NODE");
};
//------------------------------------------------------------------------------------------//
void OUTPUT_NODE::Init(COLType colType,uint32 group){
	cgLastCOL = COL_NONE;
	cgLastCR = 0;
	cgColType = colType;
};
//------------------------------------------------------------------------------------------//
void OUTPUT_NODE::RegisterToCache(OUTPUT_CACHE* cache,uint32 group){
	if (cache != nullptr)
		cache->Register(this, group);
};
//------------------------------------------------------------------------------------------//
void OUTPUT_NODE::UnregisterToCache(void){
	OUTPUT_CACHE *cache;
	cache = static_cast<OUTPUT_CACHE*>(cgCache);
	if (cache != nullptr)
		cache->Unregister(this);
};
//------------------------------------------------------------------------------------------//
void OUTPUT_NODE::RemoveSelf(void){
	OUTPUT_NODE *father = static_cast<OUTPUT_NODE*>(GetUp(this));
	if (GetGroup() == COLRECORD::CRD_G3){
		if (father != nullptr)
			father->UnregisterChild(this);
	}
	else{
		UnregisterToCache();
	}
};
//------------------------------------------------------------------------------------------//
void OUTPUT_NODE::UpdataLastStatus(COLORENUM col,const uint8* data){
	cgLastCOL = col;
	cgLastCR = (*data == '\r' || *data == '\n') ? 1 : 0;
};
//------------------------------------------------------------------------------------------//
bool32 OUTPUT_NODE::CheckPrint(uint32 addr)const{
	if (CRD::CheckGroup(cgAddress,addr) == G_FALSE)
		return G_FALSE;
	
	if (((addr & cgMask) == GetAddress()) || ((addr & cgMask) == cgMask))
		return G_TRUE;

	switch (GetGroup()) {
		case CRD::CRD_G1:;
			return G_TRUE;
		case CRD::CRD_G2:;
			if ((addr & CRD::CRD_GROUPMASK) != CRD::CRD_G2)
				return G_TRUE;
		case CRD::CRD_G3:;
		default:;
			break;
	}
	return G_FALSE;
};
//------------------------------------------------------------------------------------------//
void OUTPUT_NODE::ShareAddressKeepDevID(OUTPUT_NODE *node1,const OUTPUT_NODE &node2){
	node1->cgCache = node2.cgCache;
	if (node1->cgMask == node2.cgMask){
		B_ClrFLAG32(node1->cgAddress,node1->cgMask | CRD::CRD_GROUPMASK);
		B_SetFLAG32(node1->cgAddress,node2.cgAddress & (~(node2.cgMask | CRD::CRD_GROUPMASK)));
	}
	else{
		node1->cgAddress = node2.cgAddress;
		node1->cgMask = node2.cgMask;
	}
	node1->cgExtraGroup = node2.cgExtraGroup;
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
VG3D_POOL::VG3D_POOL(OUTPUT_CACHE* cache) : OUTPUT_NODE(COLType_RAW){
	SetSelfName("VG3D_POOL");
	RegisterToCache(cache);
	g3AddrM.InitSize((1 << 3) - 1);
};
//------------------------------------------------------------------------------------------//
VG3D_POOL::~VG3D_POOL(void){
	UnregisterAll();
	UnregisterToCache();
};
//------------------------------------------------------------------------------------------//
void VG3D_POOL::RegisterToCache(OUTPUT_CACHE* cache,uint32 group){
	group = COLRECORD::CRD_G3;
	if (cache != nullptr){
		cache->Register(this, group);
		TREE_DownChain_Traversal_LINE(OUTPUT_NODE,this,ShareAddressKeepDevID(_opNode,*this);)
	}
};
//------------------------------------------------------------------------------------------//
void VG3D_POOL::UnregisterToCache(void){
	OUTPUT_CACHE* cache;
	cache = static_cast<OUTPUT_CACHE*>(cgCache);
	if (cache != nullptr){
		cache->Unregister(this);
		TREE_DownChain_Traversal_LINE(OUTPUT_NODE,this,ShareAddressKeepDevID(_opNode,*this);)
	}
};
//------------------------------------------------------------------------------------------//
uint32 VG3D_POOL::AssignDevID(void){
	uint32 devID;
	g3AddrM.FindFirstZeroBit();
	devID = (uint32)g3AddrM.FindFirstZeroBit();
	g3AddrM.SetOneBit(devID);
	return(devID);
};
//------------------------------------------------------------------------------------------//
OUTPUT_NODE* VG3D_POOL::AddG3D(OUTPUT_NODE* oG3D,G_LOCK blLock){
	if (oG3D != nullptr){
		oG3D->RemoveSelf();
		ShareAddressKeepDevID(oG3D, *this);
		oG3D->SetDevID(AssignDevID());
		AppendDownNode(oG3D);
	}
	return(oG3D);
};
//------------------------------------------------------------------------------------------//
void VG3D_POOL::UnregisterAll(void){
	TREE_DownChain_Traversal_LINE_nolock(OUTPUT_NODE, this,
		UnregisterChild(_opNode);
		if (_opNode->IsDestoryByCache())
			MoveNodesToTrash(this,_opNode,_opNode);
	)
};
//------------------------------------------------------------------------------------------//
void VG3D_POOL::Print(uint32 ctrl,COLORENUM col,const uint8 *data,uint32 num){
	if (num > 0){
		TREE_DownChain_Traversal_LINE(OUTPUT_NODE,this,
			if (_opNode->CheckPrint(ctrl)){
				_opNode->Print(ctrl,col,data,num);
				_opNode->UpdataLastStatus(col,data + num - 1);
			}
		);
	}
	else{
		TREE_DownChain_Traversal_LINE(OUTPUT_NODE,this,_opNode->Print(ctrl,col,data,0););
	}
};
//------------------------------------------------------------------------------------------//
#endif /* Output_h */
