//
//  ColorRecord.cpp
//  SDT
//
//  Created by Leif Wen on 29/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "ColorRecord.h"
//------------------------------------------------------------------------------------------//
#ifdef ColorRecord_h
//------------------------------------------------------------------------------------------//
CRD::COLRECORD(uint32 size) : PNFB_SHELL(){
	AddBlockSubPN(pn_CTRL < pn_NL < pn_Group < pn_Adress) < pn_COL < pn_Text;
	InitSize(size);
	InitPN();
	SetSelfName("COLRECORD");
};
//------------------------------------------------------------------------------------------//
void CRD::InitPN(void){
	PNFB_SHELL::InitPN	(&cgArray,&cgArray,0,G_ENDIAN_LITTLE);
	SetBlockPoint		(&pn_Text);
	
	pn_CTRL.InitPN		(&cgArray,&cgArray,2,G_ENDIAN_LITTLE);
	pn_NL.InitPN		(&cgArray,&cgArray,2,G_ENDIAN_LITTLE);
	pn_Group.InitPN		(&cgArray,&cgArray,2,G_ENDIAN_LITTLE);
	pn_Adress.InitPN	(&cgArray,&cgArray,2,G_ENDIAN_LITTLE);
	pn_NL.SetMaskBit	(CRD_NL);
	pn_Group.SetMaskBit	(CRD_GROUPMASK);
	pn_Adress.SetMaskBit(CRD_ADDRMASK);

	pn_CTRL.FillMaskFieldRE();
	
	pn_COL.InitPN		(&cgArray,&cgArray,1,G_ENDIAN_LITTLE);//max 255 color
	pn_Text.InitPN		(&cgArray,&cgArray);//max 2^30 + 2^22 + 2^14 + 2^6byte per message
	
	cgDefCtrl = CRD_GROUPMASK;
	cgDefCol = COL_NONE;
	ClrSFlag(CRD_blAddData);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
CRDC::COLRECORD_CACHE(uint32 size) : CRD(size){
	cgGroupDisableStatus = 0;
	SetSelfName("COLRECORD_CACHE");
	cgInAddr = CRD_DEFGROUP;
	cgInEnforce = 0;
	cgInNL = 0;
	cgInClrGroup = 0;
};
//------------------------------------------------------------------------------------------//
void CRDC::NL(void){
	if (CheckSFlag(CRD_blAddData)){
		pn_Text << "\n";
	}
	else{
		cgInNL = CRD_NL;
		B_SetFLAG32(cgDefCtrl, CRD_NL);
	}
};
//------------------------------------------------------------------------------------------//
void CRDC::SetCol(COLORENUM col){
	if (col == COL_FF)
		return;
	if (CheckSFlag(CRD_blAddData) == G_FALSE){
		cgDefCol = col;
	}
	else if (cgDefCol != col){
		CRD::_Endl(nullptr);
		CRD::_Begin(cgStartup.ios);
		cgDefCol = col;
		B_ClrFLAG32(cgDefCtrl, CRD_NL);
	}
};
//------------------------------------------------------------------------------------------//
void CRDC::SetAddr(uint32 addr){
	if (CheckSFlag(CRD_blAddData) == G_FALSE){
		cgInAddr = addr;
	}
	else if (addr != cgInAddr){
		CRD::_Endl(nullptr);
		CRD::_Begin(cgStartup.ios);
		cgInAddr = addr;
		cgInNL = 0;
	}
	cgDefCtrl = MakeCtrl(cgInAddr, cgInEnforce | cgInNL);
	B_ClrFLAG32(cgDefCtrl, cgInClrGroup & CRD_GROUPMASK);
};
//------------------------------------------------------------------------------------------//
void CRDC::SetEnforce(uint32 group){
	if (CheckSFlag(CRD_blAddData) == G_FALSE){
		cgInEnforce = group;
	}
	else if (group != cgInEnforce){
		CRD::_Endl(nullptr);
		CRD::_Begin(cgStartup.ios);
		cgInEnforce = group;
		cgInNL = 0;
	}
	cgDefCtrl = MakeCtrl(cgInAddr, cgInEnforce | cgInNL);
	B_ClrFLAG32(cgDefCtrl, cgInClrGroup & CRD_GROUPMASK);
};
//------------------------------------------------------------------------------------------//
void CRDC::ClrGroup(uint32 group){
	group &= CRD_GROUPMASK;
	if (CheckSFlag(CRD_blAddData) == G_FALSE){
		cgInClrGroup = group;
	}
	else if (group != cgInEnforce){
		CRD::_Endl(nullptr);
		CRD::_Begin(cgStartup.ios);
		cgInClrGroup = group;
		cgInNL = 0;
	}
	cgDefCtrl = MakeCtrl(cgInAddr, cgInEnforce | cgInNL);
	B_ClrFLAG32(cgDefCtrl, cgInClrGroup & CRD_GROUPMASK);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
CRDN::COLRECORD_NODE(void) : DSTF(){
	SetSelfName("COLRECORD_NODE");
	SetAddress(nullptr,0,0,0);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif
