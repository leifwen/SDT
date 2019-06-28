//
//  ColorRecord.hpp
//  SDT
//
//  Created by Leif Wen on 29/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#include "ColorRecord.h"
//------------------------------------------------------------------------------------------//
#ifndef ColorRecord_hpp
#define ColorRecord_hpp
//------------------------------------------------------------------------------------------//
#ifdef ColorRecord_h
//------------------------------------------------------------------------------------------//
template <typename UV>	static inline _ColData COLOR	(COLORENUM col,const UV&	_in){UVIn uv(_in);return{col,uv};};
						static inline _ColData COLOR	(COLORENUM col,const UVIn&	_in){return{col,_in};};
//------------------------------------------------------------------------------------------//
inline uint32 CRD::CheckNL		(uint32 ctrl)					{return(B_ChkFLAG32(ctrl,CRD_NL));};
inline uint32 CRD::CheckGroup	(uint32 ctrl,uint32 group)		{return(B_ChkFLAG32(ctrl,group & CRD_GROUPMASK));};
//------------------------------------------------------------------------------------------//
inline IOSE CRD::DoTransform(IOS* _ios,const UVOut& _out,const uint8* data,const uint64& length){
	IOSE rcode;
	SetSFlag(CRD_blAddData);
	rcode = pn_Text.Transform(_ios,data,length);
	POS_update(&cgPosWR);
	return(rcode);
};
//------------------------------------------------------------------------------------------//
inline IOSE CRD::_Begin(IOS* _ios){
	SetS(_ios,-1,COL_FF);
	return(pn_Text._Begin(_ios));
};
//------------------------------------------------------------------------------------------//
inline IOSE CRD::_Endl(IOS* _ios){
	pn_Text._Endl(_ios);
	pn_CTRL.Update(cgDefCtrl);
	pn_COL.Update (cgDefCol);
	return(SetE());
};
//------------------------------------------------------------------------------------------//
inline CRD& CRD::SetS(IOS* _ios,uint32 ctrl,COLORENUM col){
	ClrSFlag(CRD_blAddData);
	PNFB_SHELL::_Begin(_ios);
	pn_CTRL.Write(_ios,ctrl);
	pn_COL.Write(_ios,col);
	return(*this);
};
//------------------------------------------------------------------------------------------//
inline IOSE CRD::SetE(void){
	return(PNFB_SHELL::_Endl(nullptr));
};
//------------------------------------------------------------------------------------------//
inline uint32 CRD::ReadCtrl	(void)const	{return(pn_CTRL.GetValueCalcRE());};
inline uint32 CRD::ReadCOL	(void)const	{return(pn_COL.GetValueCalcRE());};
//------------------------------------------------------------------------------------------//
inline IOSE CRD::Write(IOS* _ios,uint32 ctrl,COLORENUM col,const UVIn& _in){
	SetS(_ios,ctrl,col);
	pn_Text.Write(_ios,_in);
	return(SetE());
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
inline uint32 CRDC::MakeCtrl(uint32 ctrl,uint32 enforce){
	
	return(B_SetFLAG32(B_ClrFLAG32(ctrl,cgGroupDisableStatus),enforce & CRD_GNLMASK));
};
//------------------------------------------------------------------------------------------//
inline void CRDC::DisableGroup(uint32 group){
	cgPrintLock.Set();
	B_SetFLAG32(cgGroupDisableStatus,group & CRD_GROUPMASK);
	cgPrintLock.Clr();
};
//------------------------------------------------------------------------------------------//
inline void CRDC::EnableGroup(uint32 group){
	cgPrintLock.Set();
	B_ClrFLAG32(cgGroupDisableStatus,group & CRD_GROUPMASK);
	cgPrintLock.Clr();
};
//------------------------------------------------------------------------------------------//
inline bool32 CRDC::CheckDisableGroup(uint32 group){
	
	return(B_ChkFLAG32(cgGroupDisableStatus,group & CRD_GROUPMASK));
};
//------------------------------------------------------------------------------------------//
inline void CRDC::Write(IOS* _ios,COLORENUM col,const UVIn& _in,G_LOCK blLock,uint32 enforce,uint32 addr){
	cgPrintLock.Set(blLock);
	CRD::Write(_ios, MakeCtrl(addr,enforce), col, _in);
	cgPrintLock.Clr(blLock);
};
//------------------------------------------------------------------------------------------//
inline void CRDC::WriteNL(IOS* _ios,COLORENUM col,const UVIn& _in,G_LOCK blLock,uint32 enforce,uint32 addr){
	cgPrintLock.Set(blLock);
	CRD::Write(_ios, MakeCtrl(addr,enforce | CRD_NL), col, _in);
	cgPrintLock.Clr(blLock);
};
//------------------------------------------------------------------------------------------//
inline IOSE CRDC::_Begin(IOS* _ios){
	cgPrintLock.Set();
	cgDefCtrl = MakeCtrl(CRD_DEFGROUP,0);
	cgDefCol = COL_clDefault;
	return(CRD::_Begin(_ios));
};
//------------------------------------------------------------------------------------------//
inline IOSE CRDC::_Endl(IOS* _ios){
	IOSE rcode;
	rcode = CRD::_Endl(_ios);
	cgPrintLock.Clr();
	cgInNL = 0;
	cgInClrGroup = 0;
	return(rcode);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
inline void CRDN::SetAddress(const CRDC* cache,uint32 addr,uint32 mask,uint32 exGroup){
	cgCache = (CRDC*)cache;
	cgAddress = B_ClrFLAG32(addr, CRD::CRD_NL);
	cgMask = B_ClrFLAG32(mask, CRD::CRD_NL | CRD::CRD_GROUPMASK);
	cgExtraGroup = exGroup;
};
//------------------------------------------------------------------------------------------//
inline uint32 CRDN::GetAddress(void)const{
	
	return(cgAddress & cgMask);
};
//------------------------------------------------------------------------------------------//
inline uint32 CRDN::GetGroup(void)const{
	
	return(cgAddress & CRD::CRD_GROUPMASK);
};
//------------------------------------------------------------------------------------------//
inline void CRDN::SetExtraGroup(uint32 group){
	
	cgExtraGroup = (group & CRD::CRD_GROUPMASK);
};
//------------------------------------------------------------------------------------------//
inline void CRDN::SetDevID(uint32 devID){

	cgAddress = (cgAddress & (cgMask | CRD::CRD_GNLMASK)) | (devID & (~cgMask) & CRD::CRD_ADDRMASK);
};
//------------------------------------------------------------------------------------------//
inline uint32 CRDN::GetDevID(void){
	
	return(cgAddress & (cgMask | CRD::CRD_GNLMASK));
};
//------------------------------------------------------------------------------------------//
inline void CRDN::PrintDisable(void){
	if (cgCache != nullptr)
		cgCache->DisableGroup(cgAddress);
};
//------------------------------------------------------------------------------------------//
inline void CRDN::PrintEnable(void){
	if (cgCache != nullptr)
		cgCache->EnableGroup(cgAddress);
};
//------------------------------------------------------------------------------------------//
inline bool32 CRDN::CheckPrintDisable(void){
	if (cgCache != nullptr)
		return(cgCache->CheckDisableGroup(cgAddress));
	return G_FALSE;
};
//------------------------------------------------------------------------------------------//
inline IOSE CRDN::DoTransform(IOS* _ios,const UVOut& _out,const uint8* data,const uint64& length){
	if (cgCache != nullptr)
		return(cgCache->Transform(_ios, data, length));
	return IOS_OK;
};
//------------------------------------------------------------------------------------------//
inline IOSE CRDN::DoFinal(IOS* _ios,const UVOut& _out){
	if (cgCache != nullptr)
		return(cgCache->Final(_ios));
	return IOS_FINISH;
};
//------------------------------------------------------------------------------------------//
inline IOSE CRDN::_Begin(IOS* _ios){
	if (cgCache != nullptr){
		cgCache->_Begin(_ios);
		cgCache->SetAddr(cgAddress | cgExtraGroup);
		cgCache->SetEnforce(cgAddress);
	}
	return IOS_OK;
};
//------------------------------------------------------------------------------------------//
inline IOSE CRDN::_Endl(IOS* _ios) {
	if (cgCache != nullptr)
		return(cgCache->_Endl(_ios));
	return IOS_FINISH;
};
//------------------------------------------------------------------------------------------//
inline void CRDN::Write(IOS* _ios,COLORENUM col,const UVIn& _in,G_LOCK blLock,uint32 extraGroup)const{
	if (cgCache != nullptr)
		cgCache->Write(_ios, col, _in, blLock, cgAddress | extraGroup, cgAddress | cgExtraGroup);
};
//------------------------------------------------------------------------------------------//
inline void CRDN::WriteNL(IOS* _ios,COLORENUM col,const UVIn& _in,G_LOCK blLock,uint32 extraGroup)const{
	if (cgCache != nullptr)
		cgCache->WriteNL(_ios, col, _in, blLock, cgAddress | extraGroup, cgAddress | cgExtraGroup);
};
//------------------------------------------------------------------------------------------//
inline uint64 CRDN::GetUVInLength(const UVIn& _in){
	return (DSTF::GetUVInLength(_in));
};
//------------------------------------------------------------------------------------------//
inline uint64 CRDN::GetUVInLength(const _ColData& _in){
	return (DSTF::GetUVInLength(_in.uvin));
};
//------------------------------------------------------------------------------------------//
inline uint64 CRDN::GetUVInLength(const char* _in){
	if (_in != nullptr)
		return (strlen(_in));
	return 0;
};
//------------------------------------------------------------------------------------------//
inline uint64 CRDN::GetUVInLength(const COLORENUM col){
	return 0;
};
//------------------------------------------------------------------------------------------//
inline CRDN& CRDN::_NL(void){
	if (cgCache != nullptr)
		cgCache->NL();
	return(*this);
};
//------------------------------------------------------------------------------------------//
inline CRDN& CRDN::_DefGroup(void){
	if (cgCache != nullptr)
		cgCache->SetEnforce(cgAddress | cgExtraGroup);
	return(*this);
};
//------------------------------------------------------------------------------------------//
inline CRDN& CRDN::_G1(void){
	if (cgCache != nullptr)
		cgCache->SetEnforce(cgAddress | CRD::CRD_G1);
	return(*this);
};
//------------------------------------------------------------------------------------------//
inline CRDN& CRDN::_G2(void){
	if (cgCache != nullptr)
		cgCache->SetEnforce(cgAddress | CRD::CRD_G2);
	return(*this);
};
//------------------------------------------------------------------------------------------//
inline CRDN& CRDN::_G3(void){
	if (cgCache != nullptr)
		cgCache->SetEnforce(cgAddress | CRD::CRD_G3);
	return(*this);
};
//------------------------------------------------------------------------------------------//
inline CRDN& CRDN::SetColData(const _ColData& data){
	if (cgCache != nullptr){
		if (data.col != COL_FF)
			cgCache->SetCol(data.col);
		*this << data.uvin;
	}
	return(*this);
};
//------------------------------------------------------------------------------------------//
inline CRDN& CRDN::SetColData(const COLORENUM& col){
	if (cgCache != nullptr)
		cgCache->SetCol(col);
	return(*this);
};
//------------------------------------------------------------------------------------------//
inline CRDN& CRDN::SetGroup(const _Group& group){
	if (cgCache != nullptr)
		cgCache->SetEnforce(cgAddress | group.value);
	return(*this);
};
//------------------------------------------------------------------------------------------//
inline CRDN& CRDN::ClrGroup(const _ClrGroup& group){
	if (cgCache != nullptr)
		cgCache->ClrGroup(group.value);
	return(*this);
};
//------------------------------------------------------------------------------------------//
#endif /* ColorRecord_h */
#endif /* ColorRecord_hpp */
