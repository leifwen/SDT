//
//  Output.cpp
//  SDT
//
//  Created by Leif Wen on 30/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
#include "Output.h"
//------------------------------------------------------------------------------------------//
#ifdef Output_h
#include "SYS_Time.h"
//#define LOGPRINT_ENABLE
#include "SYS_Log.h"
//------------------------------------------------------------------------------------------//
/*
 * __ffs - find first bit in word.
 * @word: The word to search
 *
 * Undefined if no bit exists, so code should check against 0 first.
 */
static uint32 FindFirstBit(uint64 word){
	uint32 num = 0;
	
	if ((word & 0xffffffff) == 0){
		num += 32;
		word >>= 32;
	}
	if ((word & 0xffff) == 0){
		num += 16;
		word >>= 16;
	}
	if ((word & 0xff) == 0){
		num += 8;
		word >>= 8;
	}
	if ((word & 0xf) == 0){
		num += 4;
		word >>= 4;
	}
	if ((word & 0x3) == 0){
		num += 2;
		word >>= 2;
	}
	if ((word & 0x1) == 0)
		num += 1;
	return(num);
}
//------------------------------------------------------------------------------------------//
static uint32 CheckVaildAddress(const uint64* addrM,uint32 size){
	uint32	ret,num;
	ret = 0;
	do{
		if (*addrM != (uint64)-1){
			num = FindFirstBit(~(*addrM));
			return(ret + num);
		}
		addrM++;
		ret += 64;
	}while(--size > 0);
	return 0;
};
//------------------------------------------------------------------------------------------//
static inline void CleanU64(const uint64* addrM,uint32 size){
	do{
		addrM = 0;
	}while(--size > 0);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
OUTPUT_CACHE::OUTPUT_CACHE(uint32 size) : CRDC(size){

	outCacheThread.ThreadInit(this, &OUTPUT_CACHE::OutCacheThreadFun,"OUTCACHE");
	
	CleanU64(g2AddrM,sizeof(g2AddrM)/64);
	CleanU64(g3AddrM,sizeof(g3AddrM)/64);
	g2AddrM[0] = 1;
	g2AddrM[sizeof(g2AddrM)/64 - 1] = ((uint64)0x1 << 63);
	g3AddrM[0] = 1;
	g3AddrM[sizeof(g3AddrM)/64 - 1] = ((uint64)0x1 << 63);
	cgLastCol = COL_NONE;
	cgLastCtrl = 0;
	
	SetSelfName("OUTPUT_CACHE");
};
//------------------------------------------------------------------------------------------//
OUTPUT_CACHE::~OUTPUT_CACHE(void){
	Stop();
	UnregisterAll();
	CleanChild(&cgONList,&cgONList);
	CleanTrash(&cgONList);
};
//------------------------------------------------------------------------------------------//
void OUTPUT_CACHE::Register(OUTPUT_NODE* oNode,uint32 group){
	uint32 addr,mask;
	
	InUse_set();
	addr = AssignAddress(group);
	mask = AssignMask(group);
	InUse_clr();
	
	oNode->SetAddress(this,addr,mask,(group == CRD_G2) ? 0 : CRD_DEFGROUP);
	
	cgONList.AddNode((TNF*)oNode);
}
//------------------------------------------------------------------------------------------//
void OUTPUT_CACHE::Unregister(OUTPUT_NODE* oNode){
	if (oNode != nullptr){
		InUse_set();
		RecycleAddress(oNode->GetAddress());
		InUse_clr();
		
		oNode->SetAddress(nullptr,0,0,0);
		if (oNode->IsDestoryByCache()){
			MoveNodesToTrash(&cgONList,oNode);
		}
		else{
			Remove(oNode);
		}
	}
}
//------------------------------------------------------------------------------------------//
void OUTPUT_CACHE::UnregisterAll(void){
	TREE_LChildRChain_Traversal_LINE_nolock(OUTPUT_NODE, this, Unregister(_opNode);)
}
//------------------------------------------------------------------------------------------//
void OUTPUT_CACHE::RecycleAddress(uint32 address){
	if (CheckGroup(address,CRD_G2)){
		address &= OC_G2AMASK;
		B_ClrFLAG64(g2AddrM[address >> 6],BD_FLAG64(address % 64));
	}
	else if (CheckGroup(address,CRD_G3)){
		address = (address & OC_G2AMASK) >> 3;
		B_ClrFLAG64(g3AddrM[address >> 6],BD_FLAG64(address % 64));
	}
}
//------------------------------------------------------------------------------------------//
uint32 OUTPUT_CACHE::AssignAddress(uint32 group){
	uint32	ret;
	
	ret = CRD_G1 | 0x0001;
	if (group == CRD_G2){
		ret = CheckVaildAddress(g2AddrM,sizeof(g2AddrM));
		if (ret > ((1 << 12) - 2))
			ret = 0;
		B_SetFLAG64(g2AddrM[ret >> 6],BD_FLAG64(ret % 64));
		ret = (ret & CRD_ADDRMASK) | CRD_G2;
	}
	else if (group == CRD_G3){
		ret = CheckVaildAddress(g3AddrM,sizeof(g3AddrM));
		if (ret > ((1 << 9) - 2))
			ret = 0;
		B_SetFLAG64(g3AddrM[ret >> 6],BD_FLAG64(ret % 64));
		ret = ((ret << 3) & CRD_ADDRMASK) | CRD_G3 | OC_G3Broadcast;
	}
	return(ret);
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
OUTPUT_CACHE& OUTPUT_CACHE::DoTransform(IOSTATUS* _ios,const UVOut& _out,const uint8* data,const uint64& length){
	if (_out.uvid == UVID_SELF){
		if (length == 0)
			return(*this);
		ELog("CACHE::Delivery()::ctrl:" << std::hex << cgLastCtrl << " col:" << std::dec << cgLastCol << " L:" << length << ".");
		TREE_LChildRChain_Traversal_LINE(OUTPUT_NODE,(&cgONList),
			if (_opNode->CheckPrint(cgLastCtrl)){
				_opNode->Print(cgLastCtrl,cgLastCol,data,(uint32)length);
				_opNode->UpdataLastStatus(cgLastCol,data + length - 1);
			}
		);
		B_ClrFLAG32(cgLastCtrl, CRD_NL);
	}
	else{
		CRDC::DoTransform(_ios,_out,data,length);
	}
	return(*this);
};
//------------------------------------------------------------------------------------------//
void OUTPUT_CACHE::Delivery(void){
	uint8	data;
	
	while(TryGetFrame() > 0){
		cgLastCol = (COLORENUM)ReadCOL();
		cgLastCtrl = ReadCtrl();
		Read(nullptr,OUD(this));
		Out();
	}
	TREE_LChildRChain_Traversal_LINE(OUTPUT_NODE,(&cgONList),_opNode->Print(0,COL_NONE,&data,0););
}
//------------------------------------------------------------------------------------------//
bool32 OUTPUT_CACHE::OutCacheThreadFun(void* p){
	while(outCacheThread.IsTerminated() == 0){
		Delivery();
		SYS_SleepMS(1);
	}
	return G_TRUE;
}
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
	InUse_set();
	cache = static_cast<OUTPUT_CACHE*>(cgCache);
	InUse_clr();
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
	g3AddrM = 0;
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
		InUse_set();
		TREE_LChildRChain_Traversal_LINE(OUTPUT_NODE,this,ShareAddressKeepDevID(_opNode,*this);)
		InUse_clr();
	}
};
//------------------------------------------------------------------------------------------//
void VG3D_POOL::UnregisterToCache(void){
	OUTPUT_CACHE* cache;
	InUse_set();
	cache = static_cast<OUTPUT_CACHE*>(cgCache);
	InUse_clr();
	if (cache != nullptr){
		cache->Unregister(this);
		InUse_set();
		TREE_LChildRChain_Traversal_LINE(OUTPUT_NODE,this,ShareAddressKeepDevID(_opNode,*this);)
		InUse_clr();
	}
};
//------------------------------------------------------------------------------------------//
uint32 VG3D_POOL::AssignDevID(void){
	uint32 devID;
	devID = FindFirstBit(~g3AddrM) & OUTPUT_CACHE::OC_DEVIDMASK;
	B_SetFLAG32(g3AddrM, BD_FLAG32(devID));
	return(devID);
}
//------------------------------------------------------------------------------------------//
OUTPUT_NODE* VG3D_POOL::AddG3D(OUTPUT_NODE* oG3D,G_LOCK blLock){
	if (oG3D != nullptr){
		oG3D->RemoveSelf();
		InUse_set(blLock);
		ShareAddressKeepDevID(oG3D, *this);
		oG3D->SetDevID(AssignDevID());
		AddNode(oG3D);
		InUse_clr(blLock);
	}
	return(oG3D);
};
//------------------------------------------------------------------------------------------//
void VG3D_POOL::UnregisterAll(void){
	TREE_LChildRChain_Traversal_LINE_nolock(OUTPUT_NODE, this,
		UnregisterChild(_opNode);
		if (_opNode->IsDestoryByCache())
			MoveNodesToTrash(this,_opNode);
	)
}
//------------------------------------------------------------------------------------------//
void VG3D_POOL::Print(uint32 ctrl,COLORENUM col,const uint8 *data,uint32 num){
	if (num > 0){
		TREE_LChildRChain_Traversal_LINE(OUTPUT_NODE,this,
			if (_opNode->CheckPrint(ctrl)){
				_opNode->Print(ctrl,col,data,num);
				_opNode->UpdataLastStatus(col,data + num - 1);
			}
		);
	}
	else{
		TREE_LChildRChain_Traversal_LINE(OUTPUT_NODE,this,_opNode->Print(ctrl,col,data,0););
	}
}
//------------------------------------------------------------------------------------------//
#endif /* Output_h */
