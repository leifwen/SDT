//
//  ColorRecord.hpp
//  SDT
//
//  Created by Leif Wen on 29/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#ifndef ColorRecord_hpp
#define ColorRecord_hpp
//------------------------------------------------------------------------------------------//
#include "ColorRecord.h"
//------------------------------------------------------------------------------------------//
template <typename UV>	static inline _ColData COLOR	(COLORENUM col,const UV&	_in){UVIn uv(_in);return{col,uv};};
						static inline _ColData COLOR	(COLORENUM col,const UVIn&	_in){return{col,_in};};
//------------------------------------------------------------------------------------------//
#ifdef ColorRecord_h
//------------------------------------------------------------------------------------------//
inline uint32 CRD::CheckNL		(uint32 ctrl)					{return(B_ChkFLAG32(ctrl,CRD_NL));};
inline uint32 CRD::CheckGroup	(uint32 ctrl,uint32 group)		{return(B_ChkFLAG32(ctrl,group & CRD_GROUPMASK));};
//------------------------------------------------------------------------------------------//
inline CRD& CRD::DoTransform(IOSTATUS* _ios,const UVOut& _out,const uint8* data,const uint64& length){
	SetSFlag(CRD_blAddData);
	pn_Text.Transform(_ios,data,length);
	POS_Update(&cgPosWR);
	return(*this);
};
//------------------------------------------------------------------------------------------//
inline CRD& CRD::_Begin(IOSTATUS* _ios){
	SetS(_ios,-1,COL_FF);
	pn_Text._Begin(_ios);
	return(*this);
};
//------------------------------------------------------------------------------------------//
inline CRD& CRD::_Endl(void){
	pn_Text._Endl();
	pn_CTRL.Update(cgDefCtrl);
	pn_COL.Update (cgDefCol);
	return(SetE());
};
//------------------------------------------------------------------------------------------//
inline CRD& CRD::SetS(IOSTATUS* _ios,uint32 ctrl,COLORENUM col){
	ClrSFlag(CRD_blAddData);
	PNFB_SHELL::_Begin(_ios);
	pn_CTRL.Write(_ios,ctrl);
	pn_COL.Write(_ios,col);
	return(*this);
};
//------------------------------------------------------------------------------------------//
inline CRD& CRD::SetE(void){
	PNFB_SHELL::_Endl();
	return(*this);
};
//------------------------------------------------------------------------------------------//
inline uint32 CRD::ReadCtrl	(void)const	{return(pn_CTRL.GetValueCalcRE());};
inline uint32 CRD::ReadCOL	(void)const	{return(pn_COL.GetValueCalcRE());};
//------------------------------------------------------------------------------------------//
inline CRD& CRD::Write(IOSTATUS* _ios,uint32 ctrl,COLORENUM col,const UVIn& _in){
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
	InUse_set();
	B_SetFLAG32(cgGroupDisableStatus,group & CRD_GROUPMASK);
	InUse_clr();
};
//------------------------------------------------------------------------------------------//
inline void CRDC::EnableGroup(uint32 group){
	InUse_set();
	B_ClrFLAG32(cgGroupDisableStatus,group & CRD_GROUPMASK);
	InUse_clr();
};
//------------------------------------------------------------------------------------------//
inline bool32 CRDC::CheckDisableGroup(uint32 group){
	
	return(B_ChkFLAG32(cgGroupDisableStatus,group & CRD_GROUPMASK));
};
//------------------------------------------------------------------------------------------//
inline CRDC& CRDC::Write(IOSTATUS* _ios,COLORENUM col,const UVIn& _in,G_LOCK blLock,uint32 enforce,uint32 addr){
	InUse_set(blLock);
	CRD::Write(_ios, MakeCtrl(addr,enforce), col, _in);
	InUse_clr(blLock);
	return(*this);
};
//------------------------------------------------------------------------------------------//
inline CRDC& CRDC::WriteNL(IOSTATUS* _ios,COLORENUM col,const UVIn& _in,G_LOCK blLock,uint32 enforce,uint32 addr){
	InUse_set(blLock);
	CRD::Write(_ios, MakeCtrl(addr,enforce | CRD_NL), col, _in);
	InUse_clr(blLock);
	return(*this);
};
//------------------------------------------------------------------------------------------//
inline CRDC& CRDC::_Begin(IOSTATUS* _ios){
	InUse_set();
	cgDefCtrl = MakeCtrl(CRD_DEFGROUP,0);
	cgDefCol = COL_clDefault;
	CRD::_Begin(_ios);
	return(*this);
};
//------------------------------------------------------------------------------------------//
inline CRDC& CRDC::_Endl(void){
	CRD::_Endl();
	InUse_clr();
	cgInNL = 0;
	cgInClrGroup = 0;
	return(*this);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
inline void CRDN::SetAddress(const CRDC* cache,uint32 addr,uint32 mask,uint32 exGroup){
	InUse_set();
	cgCache = (CRDC*)cache;
	cgAddress = B_ClrFLAG32(addr, CRD::CRD_NL);
	cgMask = B_ClrFLAG32(mask, CRD::CRD_NL | CRD::CRD_GROUPMASK);
	cgExtraGroup = exGroup;
	InUse_clr();
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
inline CRDN& CRDN::DoTransform(IOSTATUS* _ios,const UVOut& _out,const uint8* data,const uint64& length){
	if (cgCache != nullptr)
		cgCache->Transform(_ios, data, length);
	return(*this);
};
//------------------------------------------------------------------------------------------//
inline CRDN& CRDN::DoFinal(IOSTATUS* _ios,const UVOut& _out){
	if (cgCache != nullptr)
		cgCache->Final(_ios);
	return(*this);
};
//------------------------------------------------------------------------------------------//
inline CRDN& CRDN::_Begin(IOSTATUS* _ios){
	if (cgCache != nullptr){
		cgCache->_Begin(_ios);
		cgCache->SetAddr(cgAddress | cgExtraGroup);
		cgCache->SetEnforce(cgAddress);
	}
	return(*this);
};
//------------------------------------------------------------------------------------------//
inline CRDN& CRDN::_Endl(void) {
	if (cgCache != nullptr)
		cgCache->_Endl();
	return(*this);
};
//------------------------------------------------------------------------------------------//
inline void CRDN::Write(IOSTATUS* _ios,COLORENUM col,const UVIn& _in,G_LOCK blLock,uint32 extraGroup)const{
	if (cgCache != nullptr)
		cgCache->Write(_ios, col, _in, blLock, cgAddress | extraGroup, cgAddress | cgExtraGroup);
};
//------------------------------------------------------------------------------------------//
inline void CRDN::WriteNL(IOSTATUS* _ios,COLORENUM col,const UVIn& _in,G_LOCK blLock,uint32 extraGroup)const{
	if (cgCache != nullptr)
		cgCache->WriteNL(_ios, col, _in, blLock, cgAddress | extraGroup, cgAddress | cgExtraGroup);
};
//------------------------------------------------------------------------------------------//
inline uint64 CRDN::GetInLength(const UVIn& _in){
	return (DSTF::GetInLength(_in));
};
//------------------------------------------------------------------------------------------//
inline uint64 CRDN::GetInLength(const _ColData& _in){
	return (DSTF::GetInLength(_in.uvin));
};
//------------------------------------------------------------------------------------------//
inline uint64 CRDN::GetInLength(const char* _in){
	if (_in != nullptr)
		return (strlen(_in));
	return 0;
};
//------------------------------------------------------------------------------------------//
inline uint64 CRDN::GetInLength(const COLORENUM col){
	return 0;
}
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
