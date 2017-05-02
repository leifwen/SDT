/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: OUTPUT.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2013.7.5
 *			: 2014.1.13 remove support BCB2012
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "OUTPUT.h"
//------------------------------------------------------------------------------------------//
#ifdef OUTPUTH
//------------------------------------------------------------------------------------------//
#include "SYS_Time.h"
#include "Comm_Convert.h"
//#define LOGPRINT_ENABLE
//#define LOGPRINT_ENABLE2
#include "Comm_Log.h"
//------------------------------------------------------------------------------------------//
#ifndef DIVIDING_LINE
#define	DIVIDING_LINE	"----------------------------------------------------------------------\n"
#endif
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
/*
 * __ffs - find first bit in word.
 * @word: The word to search
 *
 * Undefined if no bit exists, so code should check against 0 first.
 */
inline uint32 FindFirstBit(uint64 word){
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
inline uint32 CheckVaildAddress(const uint64 *addrM,uint32 size){
	uint32	ret,num;
	ret = 0;
	do{
		if (*addrM != -1){
			num = FindFirstBit(~(*addrM));
			return(ret + num);
		}
		addrM++;
		ret += 64;
	}while(--size > 0);
	return 0;
};
//------------------------------------------------------------------------------------------//
inline void CleanU64(const uint64 *addrM,uint32 size){
	do{
		addrM = 0;
	}while(--size > 0);
};
//------------------------------------------------------------------------------------------//
void OUTPUT_CACHE::Init(uint32 tSize){
	COLRECORD_CACHE::Init(tSize);

	outThread.ThreadInit(this, &OUTPUT_CACHE::OutThreadFun,"out");
	
	CleanU64(g2AddrM,sizeof(g2AddrM)/64);
	CleanU64(g3AddrM,sizeof(g3AddrM)/64);
	g2AddrM[0] = 1;
	g2AddrM[sizeof(g2AddrM)/64 - 1] = ((uint64)0x1 << 63);
	g3AddrM[0] = 1;
	g3AddrM[sizeof(g3AddrM)/64 - 1] = ((uint64)0x1 << 63);
};
//------------------------------------------------------------------------------------------//
void OUTPUT_CACHE::RecycleAddress(uint32 address){
	if (CheckGroup(address,CRD_G2) != 0){
		address = GetAddress(address);
		B_ClrFLAG64(g2AddrM[address >> 6],BD_FLAG64(address % 64));
	}
	else if (CheckGroup(address,CRD_G3) != 0){
		address = GetAddress(address) >> 3;
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
void OUTPUT_CACHE::Delivery(void){
	uint32	slength,num,offset,col,ctrl;
	uint8	data;
	
	while(fnRecordOut.TryGetFrame() > 0){
		num = fnRecordOut.GetTextLength();
		offset = fnRecordOut.GetTextOffset();
		slength = fnRecordOut.GetDefFifo()->CalcOutLength(num, offset);
		col = fnRecordOut.ReadCOL();
		ctrl = fnRecordOut.ReadCtrl();
		ELog("CACHE::Out()::col:" << col << " ctrl:" << std::hex << ctrl << " L:" << fnRecordOut.GetTextLength());
		TREE_LChildRChain_Traversal_LINE(OUTPUT_NODE,(&cgSubNodeList),
			if (operateNode_t->CheckPrint(ctrl) != 0){
				if (num > 0){
					operateNode_t->Print(col,ctrl,fnRecordOut.GetDefFifo()->GetPointer(offset),num);
					operateNode_t->UpdataLastStatus(fnRecordOut.GetDefFifo()->GetPointer(offset + num - 1),col);
				}
				if (slength > 0){
					operateNode_t->Print(col,(ctrl & (~COLRECORD::CRD_NL)),fnRecordOut.GetDefFifo()->GetPointer(0),slength);
					operateNode_t->UpdataLastStatus(fnRecordOut.GetDefFifo()->GetPointer(0 + slength - 1),col);
				}
			}
		);
		fnRecordOut.Out();
	}
	TREE_LChildRChain_Traversal_LINE(OUTPUT_NODE,(&cgSubNodeList),operateNode_t->Print(0,0,&data,0););
}
//------------------------------------------------------------------------------------------//
int32 OUTPUT_CACHE::OutThreadFun(void *p){
	while(outThread.IsTerminated() == 0){
		Delivery();
		SYS_SleepMS(1);
	}
	return 1;
}
//------------------------------------------------------------------------------------------//
void OUTPUT_CACHE::Register(OUTPUT_NODE *oNode,uint32 group){
	uint32 addr,mask;
	
	Spin_InUse_set();
	addr = AssignAddress(group);
	mask = AssignMask(group);
	Spin_InUse_clr();
	
	if (group == CRD_G2){
		oNode->SetAddress(this,addr,mask,0);
	}
	else{
		oNode->SetAddress(this,addr,mask,CRD_DEFGROUP);
	}
	AddToCache(oNode);
}
//------------------------------------------------------------------------------------------//
void OUTPUT_CACHE::Unregister(OUTPUT_NODE *oNode){
	if (oNode != nullptr){
		Spin_InUse_set();
		RecycleAddress(oNode->GetAddress());
		Spin_InUse_clr();
		
		oNode->SetAddress(this,0,0,0);
		Remove(oNode);
	}
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//









//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
void OUTPUT_NODE::Init(COLType tCOLType,OUTPUT_CACHE *cache,uint32 group){
	cgLastCOL = 0;
	cgLastCR = 0;
	cgColType = tCOLType;
};
//------------------------------------------------------------------------------------------//
void OUTPUT_NODE::RegisterToCache(OUTPUT_CACHE *cache,uint32 group){
	if (cache != nullptr)
		cache->Register(this, group);
};
//------------------------------------------------------------------------------------------//
void OUTPUT_NODE::UnregisterFromCache(void){
	OUTPUT_CACHE *cache;
	Spin_InUse_set();
	cache = static_cast<OUTPUT_CACHE*>(cgRecordCache);
	Spin_InUse_clr();
	if (cache != nullptr)
		cache->Unregister(this);
};
//------------------------------------------------------------------------------------------//
void OUTPUT_NODE::RemoveSelf(void){
	if (GetFatherNode() != nullptr){
		if (COLRECORD::GetGroup(cgAddress) == COLRECORD::CRD_G3){
			GetFatherNode()->Unregister(this);
		}
		else{
			UnregisterFromCache();
		}
	}
	COLRECORD_NODE::RemoveSelf();
};
//------------------------------------------------------------------------------------------//
void OUTPUT_NODE::UpdataLastStatus(const uint8 *data,uint32 tCol){
	cgLastCOL = tCol;
	cgLastCR = (*data == '\r' || *data == '\n') ? 1 : 0;
};
//------------------------------------------------------------------------------------------//
uint32 OUTPUT_NODE::CheckPrint(uint32 addr)const{
	if (COLRECORD::CheckGroup(cgAddress,addr) == 0)
		return 0;
	switch (cgAddress & COLRECORD::CRD_GROUPMASK) {
		case COLRECORD::CRD_G3:;
			addr = COLRECORD::GetAddress(addr);
			if ((addr == COLRECORD::GetAddress(cgAddress)) || (addr == COLRECORD::GetAddress(cgBroadcast)))
				return 1;
			return 0;
		case COLRECORD::CRD_G2:;
			if (COLRECORD::GetGroup(addr) != COLRECORD::CRD_G2)
				return 1;
			addr = COLRECORD::GetAddress(addr);
			if ((addr == COLRECORD::GetAddress(cgAddress)) || (addr == COLRECORD::GetAddress(cgBroadcast)))
				return 1;
			return 0;
		case COLRECORD::CRD_G1:;
			return 1;
		default:;
			break;
	}
	return 0;
};
//------------------------------------------------------------------------------------------//
void OUTPUT_NODE::ShareAddressKeepDevID(OUTPUT_NODE *node1,const OUTPUT_NODE &node2){
	node1->cgRecordCache = node2.cgRecordCache;
	if (node1->cgMask == node2.cgMask){
		node1->cgBroadcast = node1->cgAddress;
		node1->cgAddress = node2.cgAddress;
		node1->SetDevID(node2.cgAddress);
	}
	else{
		node1->cgAddress = node2.cgAddress;
	}
	node1->cgMask = node2.cgMask;
	node1->cgBroadcast = node2.cgBroadcast;
	node1->cgExtraGroup = node2.cgExtraGroup;
};
//------------------------------------------------------------------------------------------//
void OUTPUT_NODE::PrintStr(OUTPUT_NODE *node
						   ,const COLSTR &colStr1,const COLSTR &colStr2,const COLSTR &colStr3,const COLSTR &colStr4
						   ,const COLSTR &colStr5,const COLSTR &colStr6,const COLSTR &colStr7,const COLSTR &colStr8){
	if (node != nullptr){
		*node << Start
		<< COL_NormalMessage
		<< colStr1
		<< ColStr(colStr2)
		<< ColStr(colStr3)
		<< ColStr(colStr4)
		<< ColStr(colStr5)
		<< ColStr(colStr6)
		<< ColStr(colStr7)
		<< ColStr(colStr8)
		<< Endl;
	}
}
//------------------------------------------------------------------------------------------//
void OUTPUT_NODE::PrintStrNL(OUTPUT_NODE *node
						   ,const COLSTR &colStr1,const COLSTR &colStr2,const COLSTR &colStr3,const COLSTR &colStr4
						   ,const COLSTR &colStr5,const COLSTR &colStr6,const COLSTR &colStr7,const COLSTR &colStr8){
	if (node != nullptr){
		*node << Start << NL
		<< COL_NormalMessage
		<< colStr1
		<< ColStr(colStr2)
		<< ColStr(colStr3)
		<< ColStr(colStr4)
		<< ColStr(colStr5)
		<< ColStr(colStr6)
		<< ColStr(colStr7)
		<< ColStr(colStr8)
		<< Endl;
	}
}
//------------------------------------------------------------------------------------------//
void OUTPUT_NODE::PrintWithTime(OUTPUT_NODE *node
							 ,const COLSTR &colStr1,const COLSTR &colStr2,const COLSTR &colStr3,const COLSTR &colStr4
							 ,const COLSTR &colStr5,const COLSTR &colStr6,const COLSTR &colStr7,const COLSTR &colStr8){
	if (node != nullptr){
		*node << Start << NL
		<< COL_Time_DCyan
		<< COL_NormalMessage
		<< ColStr(colStr1)
		<< ColStr(colStr2)
		<< ColStr(colStr3)
		<< ColStr(colStr4)
		<< ColStr(colStr5)
		<< ColStr(colStr6)
		<< ColStr(colStr7)
		<< ColStr(colStr8)
		<< Endl;
	}
}
//------------------------------------------------------------------------------------------//
void OUTPUT_NODE::PrintWithDividingLine(OUTPUT_NODE *node
										,const COLSTR &colStr1,const COLSTR &colStr2,const COLSTR &colStr3,const COLSTR &colStr4
										,const COLSTR &colStr5,const COLSTR &colStr6,const COLSTR &colStr7,const COLSTR &colStr8){
	if (node != nullptr){
		*node << Start << NL
		<< COL_DivLineTime
		<< COL_NormalMessage
		<< ColStr(colStr1)
		<< ColStr(colStr2)
		<< ColStr(colStr3)
		<< ColStr(colStr4)
		<< ColStr(colStr5)
		<< ColStr(colStr6)
		<< ColStr(colStr7)
		<< ColStr(colStr8)
		<< ColData(COL_NormalMessage, colStr1.col) << ".\n"
		<< Endl;
	}
}
//------------------------------------------------------------------------------------------//
void OUTPUT_NODE::PrintMessage(OUTPUT_NODE *node
							   ,const COLSTR &colStr1,const COLSTR &colStr2,const COLSTR &colStr3,const COLSTR &colStr4
							   ,const COLSTR &colStr5,const COLSTR &colStr6,const COLSTR &colStr7,const COLSTR &colStr8){
	if (node != nullptr){
		*node << Start << EnforceDefGroup << NL
		<< COL_DivLineTime
		<< COL_NormalMessage
		<< ColStr(colStr1)
		<< ColStr(colStr2)
		<< ColStr(colStr3)
		<< ColStr(colStr4)
		<< ColStr(colStr5)
		<< ColStr(colStr6)
		<< ColStr(colStr7)
		<< ColStr(colStr8)
		<< ColData(COL_NormalMessage, colStr1.col) << ".\n"
		<< COL_DivLine_Maroon
		<< Endl;
	}
}
//------------------------------------------------------------------------------------------//
void OUTPUT_NODE::PrintNormalMessage(OUTPUT_NODE *node
									 ,const STDSTR &strData1,const STDSTR &strData2,const STDSTR &strData3,const STDSTR &strData4
									 ,const STDSTR &strData5,const STDSTR &strData6,const STDSTR &strData7,const STDSTR &strData8){
	if (node != nullptr){
		*node << Start << EnforceDefGroup << NL
		<< COL_DivLineTime
		<< SpaceColStr(COL_NormalMessage,strData1)
		<< SpaceStr(strData2)
		<< SpaceStr(strData3)
		<< SpaceStr(strData4)
		<< SpaceStr(strData5)
		<< SpaceStr(strData6)
		<< SpaceStr(strData7)
		<< SpaceStr(strData8)
		<< ".\n"
		<< COL_DivLine_Maroon
		<< Endl;
	}
}
//------------------------------------------------------------------------------------------//
void OUTPUT_NODE::PrintWarningMessage(OUTPUT_NODE *node
									  ,const STDSTR &strData1C1,const STDSTR &strData2C2,const STDSTR &strData3C2,const STDSTR &strData4C2
									  ,const STDSTR &strData5C2,const STDSTR &strData6C2,const STDSTR &strData7C2,const STDSTR &strData8C2){
	if (node != nullptr){
		*node << Start << EnforceDefGroup << NL
		<< COL_DivLineTime
		<< SpaceColStr(COL_WarningMessage1,strData1C1)
		<< SpaceColStr(COL_WarningMessage2,strData2C2)
		<< SpaceStr(strData3C2)
		<< SpaceStr(strData4C2)
		<< SpaceStr(strData5C2)
		<< SpaceStr(strData6C2)
		<< SpaceStr(strData7C2)
		<< SpaceStr(strData8C2)
		<< COL_WarningMessage1 << ".\n"
		<< COL_DivLine_Maroon
		<< Endl;
	}
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//









//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
void VG3D_POOL::RegisterToCache(OUTPUT_CACHE *cache,uint32 group){
	group = COLRECORD::CRD_G3;
	if (cache != nullptr){
		cache->Register(this, group);
		Spin_InUse_set();
		NextDevID = 1;
		TREE_LChildRChain_Traversal_LINE(OUTPUT_NODE,this,ShareAddressKeepDevID(operateNode_t,*this);)
		Spin_InUse_clr();
	}
};
//------------------------------------------------------------------------------------------//
void VG3D_POOL::UnregisterFromCache(void){
	OUTPUT_CACHE *cache;
	Spin_InUse_set();
	cache = static_cast<OUTPUT_CACHE*>(cgRecordCache);
	Spin_InUse_clr();
	if (cache != nullptr){
		cache->Unregister(this);
		Spin_InUse_set();
		NextDevID = 1;
		TREE_LChildRChain_Traversal_LINE(OUTPUT_NODE,this,ShareAddressKeepDevID(operateNode_t,*this);)
		Spin_InUse_clr();
	}
};
//------------------------------------------------------------------------------------------//
void VG3D_POOL::AddG3D(OUTPUT_NODE *oG3D,uint32 devID,G_LOCK blLock){
	if (oG3D != nullptr){
		oG3D->RemoveSelf();
		Spin_InUse_set(blLock);
		ShareAddressKeepDevID(oG3D, *this);
		oG3D->SetDevID(devID);
		if (devID == 0)
			oG3D->SetDevID(NextDevID++);
		AddNode(oG3D);
		Spin_InUse_clr(blLock);
	}
};
//------------------------------------------------------------------------------------------//
void VG3D_POOL::UnregisterAll(void){
	Spin_InUse_set();
	TREE_LChildRChain_Traversal_LINE(OUTPUT_NODE, this, OUTPUT_NODE::Unregister(operateNode_t);)
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
uint32 VG3D_POOL::CheckPrint(uint32 addr)const{
	if ((COLRECORD::CheckGroup(cgAddress, addr) != 0) && ((addr & cgMask) == (cgAddress & cgMask)))
		return 1;
	return 0;
};
//------------------------------------------------------------------------------------------//
void VG3D_POOL::Print(uint32 col,uint32 ctrl,const uint8 *data,uint32 num){
	if (num > 0){
		TREE_LChildRChain_Traversal_LINE(OUTPUT_NODE,this,
			if (operateNode_t->CheckPrint(ctrl) != 0){
				operateNode_t->Print(col,ctrl,data,num);
				operateNode_t->UpdataLastStatus(data + num - 1,col);
			}
		);
	}
	else{
		TREE_LChildRChain_Traversal_LINE(OUTPUT_NODE,this,operateNode_t->Print(col,ctrl,data,0););
	}
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif
