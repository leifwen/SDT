/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: PublicBuffer.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2012.02.09
 * @2013.6.30 :	change to Liunx style
 				add init method
 				use int32 to replace bool
 				use int32 to replace uint32
*/
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "Comm_Buffer.h"
//------------------------------------------------------------------------------------------//

#ifdef Comm_BufferH
//------------------------------------------------------------------------------------------//
void SBUFFER::Clean(G_LOCK blLock){
	OutLock_set(blLock);
	cgFIFO.Empty();
	OutLock_clr(blLock);
	cgAlreadyReadNum = 0;
}
//------------------------------------------------------------------------------------------//
void SBUFFER::Out(uint32 num,G_LOCK blLock){
	if (CheckEnable() != 0){
		OutLock_set(blLock);
		cgFIFO.Out(num);
		OutLock_clr(blLock);
	}
}
//------------------------------------------------------------------------------------------//
uint32 SBUFFER::Put(const uint8 *data,uint32 num,G_LOCK blLock){
	uint32	dataNum;
	dataNum = 0;
	if (CheckEnable() != 0){
		InLock_set(blLock);
		dataNum = cgFIFO.Put(data,num);
		InLock_clr(blLock);
	}
	return(dataNum);
}
//------------------------------------------------------------------------------------------//
uint32 SBUFFER::Put(const FIFO8 &fifoIn,uint32 num,uint32 offset,G_LOCK blLock){
	uint32	dataNum;
	dataNum = 0;
	if (CheckEnable() != 0){
		InLock_set(blLock);
		dataNum = cgFIFO.Put(fifoIn,num,offset);
		InLock_clr(blLock);
	}
	return(dataNum);
}
#ifdef Comm_PAH
//------------------------------------------------------------------------------------------//
uint32 SBUFFER::Put(const PROTOCOL_NODE &pnIn,G_LOCK blLock){
	uint32	dataNum;
	dataNum = 0;
	if (CheckEnable() != 0){
		InLock_set(blLock);
		dataNum = cgFIFO.Used();
		dataNum = pnIn.ReadAllContent(&cgFIFO).Used() - dataNum;
		InLock_clr(blLock);
	}
	return(dataNum);
}
#endif
//------------------------------------------------------------------------------------------//
uint32 SBUFFER::PutInHEX(const STDSTR &strIn,G_LOCK blLock){
	uint32	dataNum;
	dataNum = 0;
	if (CheckEnable() != 0){
		InLock_set(blLock);
		dataNum = cgFIFO.PutInHEX(strIn);
		InLock_clr(blLock);
	}
	return(dataNum);
}
//------------------------------------------------------------------------------------------//
uint32 SBUFFER::Put(const STDSTR &strIn,G_ESCAPE blEscape,G_LOCK blLock){
	uint32	dataNum;
	dataNum = 0;
	if (CheckEnable() != 0){
		InLock_set(blLock);
		dataNum = cgFIFO.Put(strIn,blEscape);
		InLock_clr(blLock);
	}
	return(dataNum);
}
//------------------------------------------------------------------------------------------//
uint32 SBUFFER::Get(uint8 *dataOut,uint32 num,G_LOCK blLock){
	uint32	dataNum;
	dataNum = 0;
	if (CheckEnable() != 0){
		OutLock_set(blLock);
		dataNum = cgFIFO.Get(dataOut,num);
		OutLock_clr(blLock);
	}
	return(dataNum);
}
//------------------------------------------------------------------------------------------//
uint32 SBUFFER::Get(FIFO8 *fifoOut,uint32 num,G_LOCK blLock){
	uint32	dataNum;
	dataNum = 0;
	if (CheckEnable() != 0){
		OutLock_set(blLock);
		dataNum = cgFIFO.Get(fifoOut,num);
		OutLock_clr(blLock);
	}
	return(dataNum);
}
//------------------------------------------------------------------------------------------//
const STDSTR& SBUFFER::Get(STDSTR *retStr,G_ESCAPE blEscape,G_LOCK blLock){
	if (CheckEnable() != 0){
		OutLock_set(blLock);
		if (blEscape == G_ESCAPE_OFF){
			cgFIFO.Get(retStr,cgFIFO.Used());
		}
		else{
			cgFIFO.GetUnEscape(retStr,cgFIFO.Used());
		}
		OutLock_clr(blLock);
	}
	return(*retStr);
}
//------------------------------------------------------------------------------------------//
const STDSTR& SBUFFER::GetInHEX(STDSTR *retStr,G_SPACE blSpace,G_LOCK blLock){
	if (CheckEnable() != 0){
		OutLock_set(blLock);
		if (blSpace == G_SPACE_OFF){
			cgFIFO.GetInHEX(retStr,cgFIFO.Used());
		}
		else{
			cgFIFO.GetInHEX_S(retStr,cgFIFO.Used());
		}
		OutLock_clr(blLock);
	}
	return(*retStr);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//









//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
void DBUFFER::Init(uint32 tSize){
	if (tSize > 0)
		DBUFFER_FRAME::Init(tSize);
	cgSelDB = this;
};
//------------------------------------------------------------------------------------------//
void DBUFFER::SetblSelected(void){
	if (GetFDB() != nullptr){
		GetFDB()->ChildSetSel(this);
	}
	else{
		TREE_NODE::SetblSelected();
	}
};
//------------------------------------------------------------------------------------------//
void DBUFFER::ClrblSelected(void){
	if (GetFDB() != nullptr){
		GetFDB()->ChildClrSel(this);
	}
	else{
		TREE_NODE::ClrblSelected();
	}
};
//------------------------------------------------------------------------------------------//
void DBUFFER::RemoveSelf(void){
	ClrblSelected();
	DBUFFER_FRAME::RemoveSelf();
};
//------------------------------------------------------------------------------------------//
DBUFFER* DBUFFER::AddSon(DBUFFER *sdb){
	AddNode(sdb);
	if (GetdRNodeID(sdb) == 0)
		ChildSetSel(sdb);
	return(sdb);
};
//------------------------------------------------------------------------------------------//
void DBUFFER::JoinTo(DBUFFER *fdb){
	if (fdb != nullptr)
		fdb->AddSon(this);
};
//------------------------------------------------------------------------------------------//
void DBUFFER::ChildSetSel(DBUFFER *sdb){
	TREE_NODE *cpNode;
	if ((sdb != nullptr) && (sdb != this)){
		sdb->Spin_InUse_set();
		if (sdb->CheckSelected() == 0){
			do{
				Spin_InUse_set();
				if (cgSelDB != sdb){
					if (cgSelDB != this){
						if (cgSelDB->Spin_InUse_try() == 0){
							Spin_InUse_clr();
							continue;
						}
					}
					cpNode = GetCoupleNode(cgSelDB);
					if (cpNode != nullptr){
						if (cpNode->Spin_InUse_try() == 0){
							Spin_InUse_clr();
							continue;
						}
						if (GetCNType(cgSelDB) == CN_M){
							sdb->LinkCoupleNode_nolock(cpNode);
						}
						else{
							cpNode->LinkCoupleNode_nolock(sdb);
						}
						cgSelDB->LinkCoupleNode_nolock(nullptr);
						cpNode->Spin_InUse_clr();
					}
					if (cgSelDB != this)
						cgSelDB->TREE_NODE::ClrblSelected();
					cgSelDB->Spin_InUse_clr((cgSelDB != this)?G_LOCK_ON:G_LOCK_OFF);
					cgSelDB = sdb;
					sdb->TREE_NODE::SetblSelected();
				}
				Spin_InUse_clr();
				break;
			}while(1);
		}
		sdb->Spin_InUse_clr();
	}
}
//------------------------------------------------------------------------------------------//
void DBUFFER::ChildClrSel(DBUFFER *sdb){
	TREE_NODE *cpNode;
	if ((sdb != nullptr) && (sdb != this)){
		do{
			sdb->Spin_InUse_set();
			if (sdb->CheckSelected() != 0){
				if (Spin_InUse_try() == 0){
					sdb->Spin_InUse_clr();
					continue;
				}
				cpNode = GetCoupleNode(sdb);
				if (cpNode != nullptr){
					if (cpNode->Spin_InUse_try() == 0){
						Spin_InUse_clr();
						sdb->Spin_InUse_clr();
						continue;
					}
					if (GetCNType(sdb) == CN_M){
						this->LinkCoupleNode_nolock(cpNode);
					}
					else{
						cpNode->LinkCoupleNode_nolock(this);
					}
					sdb->LinkCoupleNode_nolock(nullptr);
					cpNode->Spin_InUse_clr();
				}
				cgSelDB = this;
				sdb->TREE_NODE::ClrblSelected();
				Spin_InUse_clr();
			}
			sdb->Spin_InUse_clr();
			break;
		}while(1);
	}
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif
