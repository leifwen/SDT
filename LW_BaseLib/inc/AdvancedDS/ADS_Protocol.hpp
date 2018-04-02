//
//  ADS_Protocol.hpp
//  SDT
//
//  Created by Leif Wen on 24/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#ifndef ADS_Protocol_hpp
#define ADS_Protocol_hpp
//------------------------------------------------------------------------------------------//
#include "ADS_Protocol.h"
#ifdef ADS_Protocol_h
//------------------------------------------------------------------------------------------//
static inline void POS_Reset(ADS_POSITION* pos){
	pos->offset = 0;
	pos->length = 0;
};
//------------------------------------------------------------------------------------------//
static inline void POS_Hold(ADS_POSITION* pos){
	pos->array->Prepare_Set();
	pos->offset = pos->array->GetPreInNum();
	pos->length = 0;
};
//------------------------------------------------------------------------------------------//
static inline void POS_Update(ADS_POSITION* pos){
	pos->length = pos->array->GetPreInNum() - pos->offset;
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
template <typename T> static inline T& Start(T& _tn){
	_tn._Begin(nullptr);
	return(_tn);
};
//------------------------------------------------------------------------------------------//
static inline _UVBASE	OUDMesg	(PNF* p){return{(void*)p,	DS_IO_PNF::DSIO_PNF_UVID};};
static inline _UVBASE	IUDMesg	(PNF* p){return{(void*)p,	DS_IO_PNF::DSIO_PNF_UVID};};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
template <typename T_DSTF> inline T_DSTF& operator << (T_DSTF& _dstf,T_DSTF&(*fun)(T_DSTF&)){
	return((*fun)(_dstf));
};
//------------------------------------------------------------------------------------------//
template <typename T_DSTF> inline T_DSTF& operator << (T_DSTF& _dstf,const _BeginIOS& _sios){
	_dstf._Begin(_sios.value);
	return(_dstf);
};
//------------------------------------------------------------------------------------------//
template <typename T_DSTF> inline T_DSTF& operator << (T_DSTF& _dstf,const _BeginF& _fun){
	_dstf._Begin(nullptr);;
	return(_dstf);
};
//------------------------------------------------------------------------------------------//
template <typename T_DSTF> inline T_DSTF& operator << (T_DSTF& _dstf,const _EndlF& _fun){
	_dstf._Endl();
	return(_dstf);
};
//------------------------------------------------------------------------------------------//
inline void ADS_FIFO::SetDefArrayWR(const ARRAY* array){
	cgPosWR.array = (array == nullptr)? &cgArray : (ARRAY*)array;
};
//------------------------------------------------------------------------------------------//
inline void ADS_FIFO::SetDefArrayRE(const ARRAY* array){
	cgPosRE.array = (array == nullptr)? &cgArray : (ARRAY*)array;
};
//------------------------------------------------------------------------------------------//
inline ARRAY* ADS_FIFO::GetDefArrayWR	(void)const{return(cgPosWR.array);};
inline ARRAY* ADS_FIFO::GetDefArrayRE	(void)const{return(cgPosRE.array);};
//------------------------------------------------------------------------------------------//
inline ADS_FIFO& ADS_FIFO::DoTransform(IOSTATUS* _ios,const UVOut& _out,const uint8* data,const uint64& length){
	Save(_ios,_out,data,length);
	POS_Update(&cgPosWR);
	return(*this);
};
//------------------------------------------------------------------------------------------//
inline ADS_FIFO& ADS_FIFO::DoFinal(IOSTATUS* _ios,const UVOut& _out){
	Save(_ios,_out,nullptr,0);
	POS_Update(&cgPosWR);
	return(*this);
};
//------------------------------------------------------------------------------------------//
inline ADS_FIFO& ADS_FIFO::Transform(IOSTATUS* _ios,const UVIn& _in){
	DSTF::Transform(_ios,cgPosWR.uvOut,_in);
	return(*this);
};
//------------------------------------------------------------------------------------------//
inline ADS_FIFO& ADS_FIFO::Transform(IOSTATUS* _ios,const uint8* data,const uint64& length){
	DSTF::Transform(_ios,cgPosWR.uvOut,data,length);
	return(*this);
};
//------------------------------------------------------------------------------------------//
inline ADS_FIFO& ADS_FIFO::Final(IOSTATUS* _ios){
	DSTF::Final(_ios,cgPosWR.uvOut);
	return(*this);
};
//------------------------------------------------------------------------------------------//
inline ADS_FIFO& ADS_FIFO::AllIn(IOSTATUS* _ios,const UVIn& _in){
	_Begin(_ios);
	Transform(_ios, _in);
	Final(_ios);
	_Endl();
	return(*this);
};
//------------------------------------------------------------------------------------------//
inline ADS_FIFO& ADS_FIFO::AllOut(IOSTATUS* _ios,const UVOut& _out){
	Save(_ios,_out,IUD(cgPosRE.array,cgPosRE.length,cgPosRE.offset));
	return(*this);
};
//------------------------------------------------------------------------------------------//









//------------------------------------------------------------------------------------------//
inline TNF* PNF::AddNode(TNF* pn){
	((PNF*)pn)->SetDefArrayRE(cgPosRE.array);
	((PNF*)pn)->SetDefArrayWR(cgPosWR.array);
	return(ADS_FIFO::AddNode(pn));
};
//------------------------------------------------------------------------------------------//
inline void PNF::InitPN(const ARRAY* _out,const ARRAY* _in,uint32 fixedNum){
	SetDefArrayRE(_in);
	SetDefArrayWR(_out);
	cgFixedByte = fixedNum;
};
//------------------------------------------------------------------------------------------//
inline void PNF::SetFixedByte(uint32 fixedNum){
	cgFixedByte = fixedNum;
};
//------------------------------------------------------------------------------------------//
inline uint32& PNF::GetFixedByte(void){
	return(cgFixedByte);
};
//------------------------------------------------------------------------------------------//
inline void PNF::SyncPosRE(const ADS_POSITION* pos){
	cgPosRE.offset = pos->offset;
	cgPosRE.length = pos->length;
};
//------------------------------------------------------------------------------------------//
inline void PNF::SyncPosWR(const ADS_POSITION* pos){
	cgPosWR.offset = pos->offset;
	cgPosWR.length = pos->length;
};
//------------------------------------------------------------------------------------------//
inline	const	uint32&	PNF::GetOffsetWR	(void)const			{return(cgPosWR.offset);};
inline	const	uint32&	PNF::GetLengthWR	(void)const			{return(cgPosWR.length);};
inline	const	uint32&	PNF::GetOffsetRE	(void)const			{return(cgPosRE.offset);};
inline	const	uint32&	PNF::GetLengthRE	(void)const			{return(cgPosRE.length);};
inline	const	void	PNF::SetOffsetRE	(uint32 offset)		{cgPosRE.offset = offset;};
inline	const	void	PNF::SetLengthRE	(uint32 length)		{cgPosRE.length = length;};
inline			void	PNF::ResetPosWR		(void)				{POS_Reset(&cgPosWR);};
inline			void	PNF::ResetPosRE		(void)				{POS_Reset(&cgPosRE);};
//------------------------------------------------------------------------------------------//
inline void PNF::ResetPNLength(void){
	cgPosRE.length = cgFixedByte;
};
//------------------------------------------------------------------------------------------//
inline bool32 PNF::Read(IOSTATUS* _ios,const UVOut& _out){
	AllOut(_ios, _out);
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
inline PNF& PNF::Write(IOSTATUS* _ios,const UVIn& _in){
	AllIn(_ios, _in);
	return(*this);
};
//------------------------------------------------------------------------------------------//
inline PNF& PNF::WriteNone(void){
	POS_Hold(&cgPosWR);
	POS_Update(&cgPosWR);
	cgPosWR.array->Prepare_Clr();
	return(*this);
};
//------------------------------------------------------------------------------------------//
inline PNF&	PNF::CleanWR(void)	{cgPosWR.array->Reset();return(*this);};
inline PNF&	PNF::CleanRE(void)	{cgPosRE.array->Reset();return(*this);};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
inline void PNF_VAR::InitPN(const ARRAY* _out,const ARRAY* _in,uint32 fixedNum,G_ENDIAN endian){
	PNF::InitPN(_out,_in,fixedNum);
	SetEndian(endian);
};
//------------------------------------------------------------------------------------------//
inline void PNF_VAR::SetEndian(G_ENDIAN endian){
	if (endian == G_ENDIAN_LITTLE){
		ClrSFlag(PNF_blEndianBig);
	}
	else{
		SetSFlag(PNF_blEndianBig);
	}
};
//------------------------------------------------------------------------------------------//
inline uint32 PNF_VAR::GetValueAMaskWR	(void)const{return(GetOValueWR() & cgMask);};
inline uint32 PNF_VAR::GetValueAMaskRE	(void)const{return(GetOValueRE() & cgMask);};
inline uint32 PNF_VAR::GetValueCalcWR	(void)const{return(GetValueAMaskWR() >> cgMovebit);};
inline uint32 PNF_VAR::GetValueCalcRE	(void)const{return(GetValueAMaskRE() >> cgMovebit);};
inline uint32 PNF_VAR::GetValueWR		(void)const{return(GetOValueWR());};
inline uint32 PNF_VAR::GetValueRE		(void)const{return(GetOValueRE());};
//------------------------------------------------------------------------------------------//
inline	const PNF_VAR& PNF_VAR::Update(uint32 data){
	UpdateByte(data);
	return(*this);
};
//------------------------------------------------------------------------------------------//
inline PNF_VAR& PNF_VAR::Write(IOSTATUS* _ios,uint32 data){
	_Begin(_ios);
	SetByte(_ios,data);
	_Endl();
	return(*this);
};
//------------------------------------------------------------------------------------------//
inline const PNF_VAR& PNF_VAR::operator = (uint32 data){
	Write(nullptr,data);
	return(*this);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
inline void PNF_FIXED::InitPN(const ARRAY* _out,const ARRAY* _in,const STDSTR& fixedStr){
	cgFixedStr = fixedStr;
	PNF::InitPN(_out,_in,(uint32)cgFixedStr.length());
};
//------------------------------------------------------------------------------------------//
inline PNF_FIXED& PNF_FIXED::Write(IOSTATUS* _ios){
	IOSTATUS	ios;
	IOSTATUS_Clr(&ios);
	AllIn(&ios,cgFixedStr);
	ios.total_in = 0;
	IOSTATUS_Add(_ios,ios);
	return(*this);
};
//------------------------------------------------------------------------------------------//











//------------------------------------------------------------------------------------------//
inline void PNF_TAIL::InitPN(const ARRAY* _out,const ARRAY* _in,const STDSTR& fixedStr,PNF* pnContent){
	PNF_HEAD::InitPN(_out,_in,fixedStr);
	cgPNF = pnContent;
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
inline void	PNF_CONTENT::InitPN(const ARRAY* _out,const ARRAY* _in,PNF_VAR* pnLength){
	PNF::InitPN(_out,_in,0);
	cgPNF = pnLength;
};
//------------------------------------------------------------------------------------------//
inline void	PNF_CONTENT::InitPN(const ARRAY* _out,const ARRAY* _in,uint32 fixedNum){
	PNF::InitPN(_out,_in,fixedNum);
	cgPNF = nullptr;
};
//------------------------------------------------------------------------------------------//
inline const PNF_CONTENT& PNF_CONTENT::operator = (const UVIn& _in){
	Write(nullptr, _in);
	return(*this);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
inline void	PNF_BLOCK::InitPN(const ARRAY* _out,const ARRAY* _in,const PNF* pn){
	PNF::InitPN(_out,_in,0);
	SetBlockPoint(pn);
};
//------------------------------------------------------------------------------------------//
inline TNF* PNF_BLOCK::AddNode(TNF *pn){
	SetBlockPoint((PNF*)pn);
	return(PNF::AddNode(pn));
};
//------------------------------------------------------------------------------------------//
inline void PNF_BLOCK::SetBlockPoint(const PNF* pn){
	cgPNF = (pn == nullptr) ? this : (PNF*)pn;
};
//------------------------------------------------------------------------------------------//
inline bool32 PNF_BLOCK::Read(IOSTATUS* _ios,const UVOut& _out){
	if (cgPNF == this)
		return(PNF::Read(_ios,_out));
	return(cgPNF->Read(_ios,_out));
};
//------------------------------------------------------------------------------------------//
inline bool32 PNF_BLOCK::ChecksumResult(void)const{
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
inline PNF_BLOCK& PNF_BLOCK::DoTransform(IOSTATUS* _ios,const UVOut& _out,const uint8* data,const uint64& length){
	if (cgPNF == this){
		PNF::DoTransform(_ios,_out,data,length);
	}
	else{
		cgPNF->Transform(_ios,data,length);
		POS_Update(&cgPosWR);
	}
	return(*this);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
inline void PNFB_LC::InitPN(const ARRAY* _out,const ARRAY* _in,uint32 fixedNum,G_ENDIAN endian){
	if (_out == nullptr)
		_out = &cgArray;
	if (_in == nullptr)
		_in = &cgArray;

	PNF_BLOCK::InitPN	(_out,_in,&pnlc_Text);
	pnlc_Len.InitPN		(_out,_in,fixedNum,endian);
	pnlc_Text.InitPN	(_out,_in,&pnlc_Len);
};
//------------------------------------------------------------------------------------------//
inline			void	PNFB_LC::SetEndian(G_ENDIAN endian)		{pnlc_Len.SetEndian(endian);};
inline	const	uint32&	PNFB_LC::GetContentOffsetIn(void)const	{return(pnlc_Text.GetOffsetWR());};
inline	const	uint32&	PNFB_LC::GetContentLengthIn(void)const	{return(pnlc_Text.GetLengthWR());};
inline	const	uint32&	PNFB_LC::GetContentOffsetOut(void)const	{return(pnlc_Text.GetOffsetRE());};
inline	const	uint32&	PNFB_LC::GetContentLengthOut(void)const	{return(pnlc_Text.GetLengthRE());};
//------------------------------------------------------------------------------------------//









//------------------------------------------------------------------------------------------//
inline void PNF_SCC::InitPN(const ARRAY* _out,const ARRAY* _in,uint32 maxSize){
	PNF_BLOCK::InitPN	(_out,_in,&pnlc_Text);
	pnlc_Len.InitPN		(_out,_in,1,G_ENDIAN_LITTLE);
	pnlc_Text.InitPN	(_out,_in,1);
	cgMaxSize = maxSize;
};
//------------------------------------------------------------------------------------------//
inline bool32 PNF_SCC::IsFull(void){
	return(cgMaxSize <= pnlc_Text.GetLengthWR());
};
//------------------------------------------------------------------------------------------//
inline PNF_SCC& PNF_SCC::_Begin(IOSTATUS* _ios){
	if (CheckSFlag(DSTF_blStart) == G_FALSE){
		PNFB_LC::_Begin(_ios);
	}
	else{
		cgStartup.ios = _ios;
		pnlc_Text._Startup(_ios, cgStartup.uvOut);
	}
	return(*this);
}
//------------------------------------------------------------------------------------------//
inline PNF_SCC& PNF_SCC::_Endl(void){
	if (CheckSFlag(DSTF_blStart))
		PNFB_LC::_Endl();
	return(*this);
}
//------------------------------------------------------------------------------------------//









//------------------------------------------------------------------------------------------//
inline PNF_SC& PNF_SC::_Begin(IOSTATUS* _ios){
	ResetPosWR();
	PNF::_Begin(_ios);
	cgEA = 0;
	return(*this);
};
//------------------------------------------------------------------------------------------//
inline bool32 PNF_SC::Read(IOSTATUS* _ios,const UVOut& _out){
	pnsc_LC[0].Read(_ios,_out);
	pnsc_LC[1].Read(_ios,_out);
	pnsc_LC[2].Read(_ios,_out);
	pnsc_LC[3].Read(_ios,_out);
	return G_TRUE;
}
//------------------------------------------------------------------------------------------//
inline const PNF_SC& PNF_SC::operator = (const UVIn& _in){
	Write(nullptr, _in);
	return(*this);
};
//------------------------------------------------------------------------------------------//









//------------------------------------------------------------------------------------------//
inline PNF_BLOCK& PNFB_SHELL::AddBlockSubPN(PNF& subPN){
	pns_Block.Add(subPN);
	return(pns_Block);
};
//------------------------------------------------------------------------------------------//
inline void	PNFB_SHELL::DisableCRC(void){
	pns_Checksum.RemoveSelf();
	ClrSFlag(PNF_blEnCRC);
};
//------------------------------------------------------------------------------------------//
inline void	PNFB_SHELL::SetChecksum(void){
	pns_Checksum.Write(cgStartup.ios,0);
};
//------------------------------------------------------------------------------------------//
inline uint32 PNFB_SHELL::Out(void){
	return(cgPosRE.array->Out(cgPosRE.length + cgPosRE.offset));
};
//------------------------------------------------------------------------------------------//











//------------------------------------------------------------------------------------------//
template <typename T_PNF> PNFB_LIST<T_PNF>::PNFB_LIST(void) : PNF_BLOCK(){
	cgNum = 0;
	TNFP::SetSelfName("PNF_LIST");
	SetFatherName("");
};
//------------------------------------------------------------------------------------------//
template <typename T_PNF> inline void PNFB_LIST<T_PNF>::SetSelfName(const STDSTR& strName){
	selfName = strName;
	pnl_Qty.SetFatherName(GetFullName(this));
	pnl_Text.SetFatherName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
template <typename T_PNF> inline void PNFB_LIST<T_PNF>::SetFatherName(const STDSTR& strName){
	fatherName = strName;
	pnl_Qty.SetFatherName(GetFullName(this));
	pnl_Text.SetFatherName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
template <typename T_PNF> void PNFB_LIST<T_PNF>::InitPN(const ARRAY* _out,const ARRAY* _in){
	if (_out == nullptr)
		_out = &cgArray;
	if (_in == nullptr)
		_in = &cgArray;
	
	PNF_BLOCK::InitPN(_out, _in, &pnl_Text);
	pnl_Qty.InitPN	 (_out, _in, 2, G_ENDIAN_LITTLE);
	pnl_Text.InitPN	 (_out, _in);
};
//------------------------------------------------------------------------------------------//
template <typename T_PNF> inline PNFB_LIST<T_PNF>& PNFB_LIST<T_PNF>::InitCFG(uint32 cfg,const void* par){
	pnl_Text.InitCFG(cfg,par);
	return(*this);
};
//------------------------------------------------------------------------------------------//
template <typename T_PNF> bool32 PNFB_LIST<T_PNF>::Analysis(uint32 startOffset){
	uint32	num,offset;
	
	cgPosRE.offset = 0;
	cgPosRE.length = 0;
	offset = startOffset;
	if (pnl_Qty.Analysis(offset) > 0){
		num = pnl_Qty.GetValueRE();
		offset = pnl_Qty.GetOffsetRE() + pnl_Qty.GetLengthRE();
		
		while ((num > 0) && (pnl_Text.Analysis(offset) > 0)){
			offset = pnl_Text.GetOffsetRE() + pnl_Text.GetLengthRE();
			-- num;
		};
		if (num == 0){
			cgPosRE.offset = pnl_Qty.GetOffsetRE();
			cgPosRE.length = offset - cgPosRE.offset;
			return G_TRUE;
		}
	}
	return G_FALSE;
};
//------------------------------------------------------------------------------------------//
template <typename T_PNF> PNFB_LIST<T_PNF>& PNFB_LIST<T_PNF>::_Begin(IOSTATUS* _ios){
	cgNum = 0;
	PNF_BLOCK::_Begin(_ios);
	pnl_Qty.Write(_ios,0);
	return(*this);
};
//------------------------------------------------------------------------------------------//
template <typename T_PNF> PNFB_LIST<T_PNF>& PNFB_LIST<T_PNF>::_Endl(void){
	if (cgNum > 0)
		pnl_Text._Endl();
	pnl_Qty.Update(cgNum);
	PNF_BLOCK::_Endl();
	return(*this);
};
//------------------------------------------------------------------------------------------//
template <typename T_PNF>
PNFB_LIST<T_PNF>& PNFB_LIST<T_PNF>::DoTransform(IOSTATUS* _ios,const UVOut& _out,const uint8* data,const uint64& length){
	if (cgNum == 0){
		++ cgNum;
		pnl_Text._Begin(_ios);
	}
	pnl_Text.Transform(_ios,data,length);
	return(*this);
};
//------------------------------------------------------------------------------------------//
template <typename T_PNF> PNFB_LIST<T_PNF>& PNFB_LIST<T_PNF>::NewText(void){
	if (cgNum > 0)
		pnl_Text._Endl();
	
	++ cgNum;
	pnl_Text._Begin(cgStartup.ios);
	return(*this);
};
//------------------------------------------------------------------------------------------//
namespace PNFLISTNAMESPACE {
	template <typename T_PNF,typename T> uint32 AddText(IOSTATUS* _ios,T_PNF* _pnf){
		return 0;
	};
	
	template <typename T_PNF,typename T> uint32 AddText(IOSTATUS* _ios,T_PNF* _pnf,const T& _in){
		_pnf->Write(_ios,_in);
		return 1;
	};
	
	template<typename T_PNF,typename T,typename... Args>
	static inline uint32 AddText(IOSTATUS* _ios,T_PNF* _pnf,const T& first,const Args&... args){
		_pnf->Write(_ios,first);
		return(AddText(_ios,_pnf,args...) + 1);
	};
};
//------------------------------------------------------------------------------------------//
template <typename T_PNF>
template<typename... Args> PNFB_LIST<T_PNF>& PNFB_LIST<T_PNF>::Write(IOSTATUS* _ios,const Args&... args){
	_Begin(_ios);
	cgNum = PNFLISTNAMESPACE::AddText(_ios,&pnl_Text,args...);
	pnl_Qty.Update(cgNum);
	PNF_BLOCK::_Endl();
	return(*this);
};
//------------------------------------------------------------------------------------------//
template <typename T_PNF> uint32 PNFB_LIST<T_PNF>::ReadQty(void)const{
	return(pnl_Qty.GetValueRE());
};
//------------------------------------------------------------------------------------------//
template <typename T_PNF> bool32 PNFB_LIST<T_PNF>::Read(IOSTATUS* _ios,const UVOut& _out,uint32 order){
	uint32	num,offset;
	
	num = pnl_Qty.GetValueRE();
	if ((order <= num) && (order > 0)){
		offset = pnl_Qty.GetOffsetRE() + pnl_Qty.GetLengthRE();
		while (order-- > 0){
			if (pnl_Text.Analysis(offset) > 0){
				offset = pnl_Text.GetOffsetRE() + pnl_Text.GetLengthRE();
			}
			else{
				offset = 0;
				break;
			}
		};
		if (offset != 0)
			return(pnl_Text.Read(_ios,_out));
	}
	return G_FALSE;
};
//------------------------------------------------------------------------------------------//










#define TPNFM PNFS_MESG<T_PNF>
//------------------------------------------------------------------------------------------//
template <typename T_PNF> TPNFM::PNFS_MESG(void) : PNFB_SHELL(){
	AddBlockSubPN(pnm_ID) < pnm_Text;
	
	TNFP::SetSelfName("PNFS_MESG");
	pnm_Text.SetSelfName("Text");
	SetFatherName("");
};
//------------------------------------------------------------------------------------------//
template <typename T_PNF> inline void TPNFM::SetSelfName(const STDSTR& strName){
	selfName = strName;
	pnm_Text.SetFatherName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
template <typename T_PNF> inline void TPNFM::SetFatherName(const STDSTR& strName){
	fatherName = strName;
	pnm_Text.SetFatherName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
template <typename T_PNF> void TPNFM::InitPN(const ARRAY* _out,const ARRAY* _in,uint32 byteID,uint32 byteCRC,G_ENDIAN endian){
	if (_out == nullptr)
		_out = &cgArray;
	if (_in == nullptr)
		_in = &cgArray;

	PNFB_SHELL::InitPN	(_out,_in,byteCRC,endian);
	pnm_ID.InitPN		(_out,_in,byteID,endian);
	pnm_Text.InitPN		(_out,_in);
	SetBlockPoint		(&pnm_Text);
	if (byteCRC == 0)
		DisableCRC();
}
//------------------------------------------------------------------------------------------//
template <typename T_PNF> inline TPNFM& TPNFM::InitCFG(uint32 cfg,const void* par){
	PNFB_SHELL::InitCFG(cfg,par);
	pnm_Text.InitCFG(cfg,par);
	return(*this);
};
//------------------------------------------------------------------------------------------//
template <typename T_PNF> inline uint32 TPNFM::ReadID(void)const{
	return(pnm_ID.GetValueCalcRE());
};
//------------------------------------------------------------------------------------------//
template <typename T_PNF> inline TPNFM& TPNFM::Write(IOSTATUS* _ios,uint32 mID,const UVIn& _in){
	*this << Begin(_ios) << SetmID(mID) << _in << Endl();
	return(*this);
};
//------------------------------------------------------------------------------------------//
template <typename T_PNF> TPNFM& TPNFM::_Begin(IOSTATUS* _ios){
	PNFB_SHELL::_Begin	(_ios);
	pnm_ID.Write		(_ios,0);
	pnm_Text._Begin		(_ios);
	return(*this);
};
//------------------------------------------------------------------------------------------//
template <typename T_PNF> TPNFM& TPNFM::_Endl(void){
	pnm_Text._Endl();
	PNFB_SHELL::_Endl();
	return(*this);
};
//------------------------------------------------------------------------------------------//








#ifdef ALG_CRC_h
#define TPNMFCRC PNFS_MESG_CRC<T_PNF,T_CRC>
//------------------------------------------------------------------------------------------//
template <typename T_PNF,typename T_CRC> TPNMFCRC::PNFS_MESG_CRC(void) : PNFS_MESG<T_PNF>(){
	cgCRCCFG = -1;
	TPNFM::SetSelfName("PNFS_MESG_CRC");
	cgCRC0.selfName = "CRC0";
	cgCRC1.selfName = "CRC1";
	SetFatherName("");
};
//------------------------------------------------------------------------------------------//
template <typename T_PNF,typename T_CRC> inline void TPNMFCRC::SetSelfName(const STDSTR& strName){
	TPNFM::SetSelfName(strName);
	SetFatherName(TPNFM::fatherName);
};
//------------------------------------------------------------------------------------------//
template <typename T_PNF,typename T_CRC> inline void TPNMFCRC::SetFatherName(const STDSTR& strName){
	TPNFM::SetFatherName(strName);
	cgCRC0.SetFatherName(TPNFM::GetFullName(this));
	cgCRC1.SetFatherName(TPNFM::GetFullName(this));
};
//------------------------------------------------------------------------------------------//
template <typename T_PNF,typename T_CRC> void TPNMFCRC::InitPN(const ARRAY* _out
																 ,const ARRAY* _in
																 ,uint32 byteID
																 ,uint32 crcCFG
																 ,G_ENDIAN endian){
	InitCFG_CRC(crcCFG);
	cgCRC0.InitCFG(cgCRCCFG);
	cgCRC1.InitCFG(cgCRCCFG);
	TPNFM::InitPN(_out,_in,byteID,cgCRC0.GetCRCBit(),endian);
};
//------------------------------------------------------------------------------------------//
template <typename T_PNF,typename T_CRC> void TPNMFCRC::InitCFG_CRC(uint32 crcCFG){
	cgCRCCFG = crcCFG;
};
//------------------------------------------------------------------------------------------//
template <typename T_PNF,typename T_CRC> void TPNMFCRC::SetChecksum(void){
	uint32	slength,length,offset;
	length = this->pnm_Text.GetLengthWR();
	offset = this->pnm_Text.GetOffsetWR();
	slength = this->cgPosWR.array->CalcOutLengthInToPre(length,offset);
	cgCRC0.InitCFG(cgCRCCFG);
	if (length > 0){
		cgCRC0.Transform(nullptr,_NONE(),this->cgPosWR.array->GetPointer(offset),length);
		if (slength > 0)
			cgCRC0.Transform(nullptr,_NONE(),this->cgPosWR.array->GetPointer(0),slength);
	}
	cgCRC0.Final(nullptr, _NONE());
	this->pns_Checksum.Write(this->cgStartup.ios,cgCRC0.GetCRCReasult());
};
//------------------------------------------------------------------------------------------//
template <typename T_PNF,typename T_CRC> bool32 TPNMFCRC::ChecksumResult(void)const{
	ALG_CRC16 *crc = (ALG_CRC16*)&cgCRC1;
	if (this->CheckSFlag(TPNFM::PNF_blEnCRC) == G_FALSE)
		return G_TRUE;
	crc->InitCFG(cgCRCCFG).Calc(nullptr
								,_NONE()
								,IUD(this->cgPosRE.array,this->pnm_Text.GetLengthRE(),this->pnm_Text.GetOffsetRE()));
	return(crc->GetCRCReasult() == this->pns_Checksum.GetValueCalcRE());
};
//------------------------------------------------------------------------------------------//
#undef TPNMFCRC
#endif
#undef TPNMF
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
template <typename T_SHELL> T_SHELL* DSTF_TEST::SHELL_Create(const STDSTR& name,T_SHELL* _nullptr,ARRAY* _out,ARRAY* _in,uint32 cfg,void* p){
	T_SHELL *_shell = new T_SHELL[2];
	
	_shell[0].SetSelfName(name + "_0");
	_shell[0].InitPN(_out, _in);
	_shell[0].InitCFG(DSTF::CFG_INIT_WR_CFGPAR | DSTF::CFG_INIT_RE_CFGPAR | cfg, p);
	
	_shell[1].SetSelfName(name + "_1");
	_shell[1].InitPN(_out, _in);
	_shell[1].InitCFG(DSTF::CFG_INIT_WR_CFGPAR | DSTF::CFG_INIT_RE_CFGPAR | cfg, p);
	
	return(_shell);
};
//------------------------------------------------------------------------------------------//
template <typename T_SHELL> void DSTF_TEST::SHELL_Test(IOSTATUS* _ios,T_SHELL* _shell,STDSTR* _out,const STDSTR& _in){
	bool32 retbl;
	STDSTR _strIn;
	_shell[0].GetDefArrayWR()->Reset();
	_shell[0].GetDefArrayRE()->Reset();
	_shell[1].GetDefArrayWR()->Reset();
	_shell[1].GetDefArrayRE()->Reset();

	printf("----------------------------------------------------------\n");
	_shell[0].Write(IOSTATUS_Clr(_ios), _in);
	PrintResult(_shell[0].selfName + " Write   ",ShowINOUT(_ios),1);
	PrintResult(_shell[0].selfName + " Analysis","",_shell[0].Analysis(0));
	retbl = _shell[0].Read(IOSTATUS_Clr(_ios), _EMPTY(_out));
	PrintResult(_shell[0].selfName + " Read    ",ShowINOUT(_ios),retbl);
	PrintResult(_shell[0].selfName + " in==out ",ShowINOUT("",_in.length(),_out->length()),(*_out == _in));
	
	_shell[0].GetDefArrayWR()->Reset();
	_shell[0].GetDefArrayRE()->Reset();
	_shell[1].GetDefArrayWR()->Reset();
	_shell[1].GetDefArrayRE()->Reset();
	
	_strIn = "1234567890";
	_shell[0] << Begin(IOSTATUS_Clr(_ios)) << _strIn << _strIn << Endl;
	_strIn += _strIn;
	
	PrintResult(_shell[1].selfName + " Analysis","",_shell[1].Analysis(0));
	retbl = _shell[1].Read(IOSTATUS_Clr(_ios), _EMPTY(_out));
	PrintResult(_shell[1].selfName + " Read    ",ShowINOUT(_ios),retbl);
	PrintResult(_shell[1].selfName + " in==out ",ShowINOUT("",_strIn.length(),_out->length()),(*_out == _strIn));
	
	_shell[0].GetDefArrayWR()->Reset();
	_shell[0].GetDefArrayRE()->Reset();
	_shell[1].GetDefArrayWR()->Reset();
	_shell[1].GetDefArrayRE()->Reset();
}
//------------------------------------------------------------------------------------------//
template <typename T_SHELL> void DSTF_TEST::SHELL_Test(IOSTATUS* _ios,T_SHELL* _shell,STDSTR* _out,uint32 mID,const STDSTR& _in){
	uint32 _mID;
	bool32 retbl;
	STDSTR strMID;
	
	_shell[0].GetDefArrayWR()->Reset();
	_shell[0].GetDefArrayRE()->Reset();
	_shell[1].GetDefArrayWR()->Reset();
	_shell[1].GetDefArrayRE()->Reset();

	printf("----------------------------------------------------------\n");
	_shell[0].Write(IOSTATUS_Clr(_ios), mID, _in);
	PrintResult(_shell[0].selfName + " Write   ",ShowINOUT(_ios),1);
	
	PrintResult(_shell[0].selfName + " Analysis","",_shell[0].Analysis(0));
	_mID = _shell[0].ReadID();
	PrintResult(_shell[0].selfName + " mID     ",ShowINOUT("W/R:",mID,_mID),(mID == _mID));
	
	retbl = _shell[0].Read(IOSTATUS_Clr(_ios), _EMPTY(_out));
	PrintResult(_shell[0].selfName + " Read    ",ShowINOUT(_ios),retbl);
	PrintResult(_shell[0].selfName + " in==out ",ShowINOUT("",_in.length(),_out->length()),(*_out == _in));
	
	
	
	PrintResult(_shell[1].selfName + " Analysis","",_shell[1].Analysis(0));
	_mID = _shell[1].ReadID();
	PrintResult(_shell[1].selfName + " mID     ",ShowINOUT("W/R:",mID,_mID),(mID == _mID));
	
	retbl = _shell[1].Read(IOSTATUS_Clr(_ios), _EMPTY(_out));
	PrintResult(_shell[1].selfName + " Read    ",ShowINOUT(_ios),retbl);
	PrintResult(_shell[1].selfName + " in==out ",ShowINOUT("",_in.length(),_out->length()),(*_out == _in));
	
	_shell[0].GetDefArrayWR()->Reset();
	_shell[0].GetDefArrayRE()->Reset();
	_shell[1].GetDefArrayWR()->Reset();
	_shell[1].GetDefArrayRE()->Reset();
}
//------------------------------------------------------------------------------------------//
static inline STDSTR DSTF_TEST::ShowINOUT(IOSTATUS* _ios){
	return(ShowINOUT("In/Out=",_ios->total_in,_ios->total_out));
}
//------------------------------------------------------------------------------------------//
static inline STDSTR DSTF_TEST::ShowINOUT(const STDSTR& name,uint64 _in,uint64 _out){
	STDSTR ret;
	ret = name;
	ret += Str_ToStr(_in);
	ret += "/";
	ret += Str_ToStr(_out);
	return(ret);
}
//------------------------------------------------------------------------------------------//
static inline void DSTF_TEST::PrintResult(const STDSTR& name,const STDSTR& method,bool32 blok){
	if (blok){
		printf("MESG_Test< %s > is ok. %s\r\n",name.c_str(),method.c_str());
	}
	else{
		printf("MESG_Test< %s > is fail.\r\n",name.c_str());
	}
}
//------------------------------------------------------------------------------------------//
#endif /* ADS_Protocol_h */
#endif /* ADS_Protocol_hpp */