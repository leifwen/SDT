//
//  ADS_Buffer.cpp
//  SDT
//
//  Created by Leif Wen on 01/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
#include "ADS_Buffer.h"
//------------------------------------------------------------------------------------------//
#ifdef ADS_Buffer_h
//------------------------------------------------------------------------------------------//
SBUF::SBUF(uint32 size) : TREE_NODE(){
	InitSize(size);
};
//------------------------------------------------------------------------------------------//
uint32 SBUF::Put(IOSTATUS* _ios,const UVIn& _in,G_LOCK blLock){
	IOSTATUS	ios;
	IOSTATUS_Clr(&ios);
	
	if (CheckEnable()){
		InLock_set(blLock);
		DS_IO_NODE::GetDSIOList().Save(&ios, &cgArray, _in);
		IOSTATUS_Add(_ios, ios);
		InLock_clr(blLock);
	}
	return((uint32)ios.total_in);
};
//------------------------------------------------------------------------------------------//
uint32 SBUF::Get(IOSTATUS* _ios,const UVOut& _out,uint32 length,G_LOCK blLock){
	IOSTATUS	ios;
	IOSTATUS_Clr(&ios);
	
	if (CheckEnable()){
		OutLock_set(blLock);
		DS_IO_NODE::GetDSIOList().Save(&ios, _out, IUD(cgArray,length,0));
		cgArray.Out((uint32)ios.total_in);
		IOSTATUS_Add(_ios, ios);
		OutLock_clr(blLock);
	}
	return((uint32)ios.total_out);
};
//------------------------------------------------------------------------------------------//
void SBUF::Out(uint32 num,G_LOCK blLock){
	OutLock_set(blLock);
	cgArray.Out(num);
	OutLock_clr(blLock);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
DBUF_FRAME::DBUF_FRAME(void) : TREE_NODE(){
	InitSize(0);
};
//------------------------------------------------------------------------------------------//
DBUF_FRAME::DBUF_FRAME(uint32 size) : TREE_NODE(){
	InitSize(size);
};
//------------------------------------------------------------------------------------------//
DBUF_FRAME& DBUF_FRAME::InitSize(uint32 size){
	if (size > 0){
		cgRxSBUF.InitSize(size);
		cgTxSBUF.InitSize(size);
	}
	cgMaxSize = size;
	cgRxBytes = 0;
	cgTxBytes = 0;
	cgFwBytes = 0;
	return(*this);
};
//------------------------------------------------------------------------------------------//
void DBUF_FRAME::Empty(void){
	cgRxBytes = 0;
	cgTxBytes = 0;
	cgFwBytes = 0;
	cgRxSBUF.Empty();
	cgTxSBUF.Empty();
};
//------------------------------------------------------------------------------------------//











//------------------------------------------------------------------------------------------//
DBUF::DBUF(void) : DBUF_FRAME(){
	cgSelDB = this;
};
//------------------------------------------------------------------------------------------//
DBUF::DBUF(uint32 size) : DBUF_FRAME(size){
	cgSelDB = this;
};
//------------------------------------------------------------------------------------------//
void DBUF::ChildSetSel(DBUF* sdb){
	TREE_NODE *cpNode;
	if ((sdb != nullptr) && (sdb != this)){
		sdb->InUse_set();
		if (sdb->CheckSelected() == G_FALSE){
			do{
				InUse_set();
				if (cgSelDB != sdb){
					if (cgSelDB != this){
						if (cgSelDB->InUse_try() == G_FALSE){
							InUse_clr();
							continue;
						}
					}
					cpNode = GetCoupleNode(cgSelDB);
					if (cpNode != nullptr){
						if (cpNode->InUse_try() == G_FALSE){
							InUse_clr();
							continue;
						}
						if (GetCNType(cgSelDB) == CN_M){
							sdb->LinkCoupleNode_nolock(cpNode);
						}
						else{
							cpNode->LinkCoupleNode_nolock(sdb);
						}
						cgSelDB->LinkCoupleNode_nolock(nullptr);
						cpNode->InUse_clr();
					}
					if (cgSelDB != this)
						cgSelDB->TREE_NODE::ClrblSelected();
					cgSelDB->InUse_clr((cgSelDB != this)?G_LOCK_ON:G_LOCK_OFF);
					cgSelDB = sdb;
					sdb->TREE_NODE::SetblSelected();
				}
				InUse_clr();
				break;
			}while(1);
		}
		sdb->InUse_clr();
	}
}
//------------------------------------------------------------------------------------------//
void DBUF::ChildClrSel(DBUF* sdb){
	TREE_NODE *cpNode;
	if ((sdb != nullptr) && (sdb != this)){
		do{
			sdb->InUse_set();
			if (sdb->CheckSelected() != G_FALSE){
				if (InUse_try() == G_FALSE){
					sdb->InUse_clr();
					continue;
				}
				cpNode = GetCoupleNode(sdb);
				if (cpNode != nullptr){
					if (cpNode->InUse_try() == G_FALSE){
						InUse_clr();
						sdb->InUse_clr();
						continue;
					}
					if (GetCNType(sdb) == CN_M){
						this->LinkCoupleNode_nolock(cpNode);
					}
					else{
						cpNode->LinkCoupleNode_nolock(this);
					}
					sdb->LinkCoupleNode_nolock(nullptr);
					cpNode->InUse_clr();
				}
				cgSelDB = this;
				sdb->TREE_NODE::ClrblSelected();
				InUse_clr();
			}
			sdb->InUse_clr();
			break;
		}while(1);
	}
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif /* ADS_Buffer_h */
