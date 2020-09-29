//
//  ADS_Protocol.hpp
//  SDT
//
//  Created by Leif Wen on 24/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#ifndef ADS_Protocol_hpp
#define ADS_Protocol_hpp
//------------------------------------------------------------------------------------------//
#include "ADS_Protocol.h"
#ifdef ADS_Protocol_h
//------------------------------------------------------------------------------------------//
static inline void POS_reset(ADS_POSITION* pos){
	pos->offset = 0;
	pos->length = 0;
};
//------------------------------------------------------------------------------------------//
static inline void POS_hold(ADS_POSITION* pos){
	pos->array->Prepare_Set();
	pos->offset = pos->array->GetPreInNum();
	pos->length = 0;
};
//------------------------------------------------------------------------------------------//
static inline void POS_update(ADS_POSITION* pos){
	pos->length = pos->array->GetPreInNum() - pos->offset;
};
//------------------------------------------------------------------------------------------//
static inline const void IOS_copy(IOSIN* _iosin,IOS* _ios){
	_iosin->rcode = IOS_OK;
	if (_ios != nullptr){
		_iosin->avail_in = _ios->avail_in;
		_iosin->total_in = _ios->total_in;
	}
};
//------------------------------------------------------------------------------------------//
static inline const IOSE& IOS_copy(IOS* _ios,const IOSIN& _iosin){
	if ((_ios != nullptr) && (_iosin.rcode == IOS_NO_MEM)){
		_ios->avail_in = _iosin.avail_in;
		_ios->total_in = _iosin.total_in;
	}
	return(IOS_update(_ios, _iosin.rcode));
};
//------------------------------------------------------------------------------------------//
static inline const void IOS_hdcopy(IOS* _ios,const IOSIN& _iosin){
	if (_ios != nullptr){
		_ios->avail_in = _iosin.avail_in;
		_ios->total_in = _iosin.total_in;
	}
};
//------------------------------------------------------------------------------------------//
static inline const IOSE& IOS_update(IOSIN* _iosin,const IOSE& _rcode){
	if (_iosin->rcode > 0)
		_iosin->rcode = _rcode;
	return(_iosin->rcode);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
inline ADS_FIFO& ADS_FIFO::_SetOut(const UVOut& _out){
	cgStartup.uvOut = &cgPosWR.uvOut;
	return(*this);
};
//------------------------------------------------------------------------------------------//
inline ADS_FIFO& ADS_FIFO::_SetIOS(IOS* _ios){
	cgStartup.ios = (IOS*)_ios;
	cgStartup.uvOut = &cgPosWR.uvOut;
	return(*this);
};
//------------------------------------------------------------------------------------------//
inline void ADS_FIFO::SetDefArrayWR(const ARRAY* array){
	cgPosWR.array = (array == nullptr)? &cgArray : (ARRAY*)array;
	cgPosWR.uvOut = cgPosWR.array;
	_SetOut(cgPosWR.uvOut);
};
//------------------------------------------------------------------------------------------//
inline void ADS_FIFO::SetDefArrayRE(const ARRAY* array){
	cgPosRE.array = (array == nullptr)? &cgArray : (ARRAY*)array;
	cgPosRE.uvOut = cgPosRE.array;
};
//------------------------------------------------------------------------------------------//
inline ARRAY* ADS_FIFO::GetDefArrayWR	(void)const{return(cgPosWR.array);};
inline ARRAY* ADS_FIFO::GetDefArrayRE	(void)const{return(cgPosRE.array);};
//------------------------------------------------------------------------------------------//
inline IOSE ADS_FIFO::DoTransform(IOS* _ios,const UVOut& _out,const uint8* data,const uint64& length){
	IOSE rcode;
	rcode = Save(_ios,_out,data,length);
	POS_update(&cgPosWR);
	return(rcode);
};
//------------------------------------------------------------------------------------------//
inline IOSE ADS_FIFO::DoFinal(IOS* _ios,const UVOut& _out){
	IOSE rcode;
	rcode = DSTF::DoFinal(_ios,_out);
	POS_update(&cgPosWR);
	return(rcode);
};
//------------------------------------------------------------------------------------------//
inline IOSE ADS_FIFO::Transform(IOS* _ios,const UVIn& _in){
	return(DSTF::Transform(_ios,cgPosWR.uvOut,_in));
};
//------------------------------------------------------------------------------------------//
inline IOSE ADS_FIFO::Transform(IOS* _ios,const uint8* data,const uint64& length){
	return(DSTF::Transform(_ios,cgPosWR.uvOut,data,length));
};
//------------------------------------------------------------------------------------------//
inline IOSE ADS_FIFO::Final(IOS* _ios){
	return(DSTF::Final(_ios,cgPosWR.uvOut));
};
//------------------------------------------------------------------------------------------//
inline IOSE ADS_FIFO::AllOut(IOS* _ios,const UVOut& _out){
	return(Save(_ios,_out,IUD(cgPosRE.array,cgPosRE.length,cgPosRE.offset)));
};
//------------------------------------------------------------------------------------------//
inline IOSE ADS_FIFO::FinalOut(IOS* _ios,const UVOut& _out){
	return(Save(_ios,_out,_NONE()));
};	
//------------------------------------------------------------------------------------------//









//------------------------------------------------------------------------------------------//
inline TNF* PNF::AppendDownNode(TNF* pn){
	((PNF*)pn)->SetDefArrayRE(cgPosRE.array);
	((PNF*)pn)->SetDefArrayWR(cgPosWR.array);
	return(ADS_FIFO::AppendDownNode(pn));
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
inline const uint32& PNF::GetFixedByte(void)const{
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
inline			void	PNF::ResetPosWR		(void)				{POS_reset(&cgPosWR);};
inline			void	PNF::ResetPosRE		(void)				{POS_reset(&cgPosRE);};
//------------------------------------------------------------------------------------------//
inline void PNF::ResetPNLength(void){
	cgPosRE.length = cgFixedByte;
};
//------------------------------------------------------------------------------------------//
inline IOSE PNF::FinalRead(IOS* _ios,const UVOut& _out){
	return(FinalOut(_ios, _out));
};
//------------------------------------------------------------------------------------------//
inline IOSE PNF::Read(IOS* _ios,const UVOut& _out){
	return(AllOut(_ios, _out));
};
//------------------------------------------------------------------------------------------//
inline IOSE PNF::Write(IOS* _ios,const UVIn& _in){
	return(AllIn(_ios, _in));
};
//------------------------------------------------------------------------------------------//
inline IOSE PNF::WriteNone(void){
	POS_hold(&cgPosWR);
	POS_update(&cgPosWR);
	cgPosWR.array->Prepare_Clr();
	return IOS_FINISH;
};
//------------------------------------------------------------------------------------------//
inline void	PNF::CleanWR(void)	{cgPosWR.array->Reset();};
inline void	PNF::CleanRE(void)	{cgPosRE.array->Reset();};
//------------------------------------------------------------------------------------------//
inline uint32 PNF::OutRE(void){
	return(cgPosRE.array->Out(cgPosRE.length + cgPosRE.offset));
};
//------------------------------------------------------------------------------------------//
inline uint32 PNF::OutWR(void){
	return(cgPosWR.array->Out(cgPosWR.length + cgPosWR.offset));
};
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
inline G_ENDIAN PNF_VAR::GetEndianType(void){
	return(CheckSFlag(PNF_blEndianBig) ? G_ENDIAN_BIG : G_ENDIAN_LITTLE);
};
//------------------------------------------------------------------------------------------//
inline uint32 PNF_VAR::GetValueAMaskWR	(void)const{return(GetOValueWR() & cgMask);};
inline uint32 PNF_VAR::GetValueAMaskRE	(void)const{return(GetOValueRE() & cgMask);};
inline uint32 PNF_VAR::GetValueCalcWR	(void)const{return(GetValueAMaskWR() >> cgMovebit);};
inline uint32 PNF_VAR::GetValueCalcRE	(void)const{return(GetValueAMaskRE() >> cgMovebit);};
inline uint32 PNF_VAR::GetValueWR		(void)const{return(GetOValueWR());};
inline uint32 PNF_VAR::GetValueRE		(void)const{return(GetOValueRE());};
//------------------------------------------------------------------------------------------//
inline IOSE PNF_VAR::Update(uint32 data){
	return(UpdateByte(data));
};
//------------------------------------------------------------------------------------------//
inline uint32 PNF_VAR::GetMaxValue(void)const{
	return((uint32)-1 >> ((4 - GetFixedByte()) * 8));
};
//------------------------------------------------------------------------------------------//
inline bool32 PNF_VAR::SetMaxValueRE(void)const{
	uint8	buf[4];
	buf[0] = 0xff;
	buf[1] = 0xff;
	buf[2] = 0xff;
	buf[3] = 0xff;
	return((cgPosRE.array->UpdateByOffsetOut(buf, GetLengthRE(), GetOffsetRE()) == GetLengthRE()));
};
//------------------------------------------------------------------------------------------//
inline bool32 PNF_VAR::IsMaxValueRE(void)const{
	uint32 mask;
	mask = (uint32)-1 >> ((4 - GetFixedByte()) * 8);
	return(GetValueCalcRE() == mask);
};
//------------------------------------------------------------------------------------------//
inline bool32 PNF_VAR::SetMaxValueWR(void)const{
	uint8	buf[4];
	buf[0] = 0xff;
	buf[1] = 0xff;
	buf[2] = 0xff;
	buf[3] = 0xff;
	return((cgPosWR.array->UpdateByOffsetIn(buf, GetLengthWR(), GetOffsetWR()) == GetLengthWR()));
};
//------------------------------------------------------------------------------------------//
inline bool32 PNF_VAR::IsMaxValueWR(void)const{
	uint32 mask;
	mask = (uint32)-1 >> ((4 - GetFixedByte()) * 8);
	return(GetValueCalcWR() == mask);
};
//------------------------------------------------------------------------------------------//
inline const PNF_VAR& PNF_VAR::operator = (uint32 data){
	Write(cgStartup.ios,data);
	return(*this);
};
//------------------------------------------------------------------------------------------//









//------------------------------------------------------------------------------------------//
inline IOSE PNF_MASK::Write(IOS* _ios,uint32 data){
	IOSE rcode;
	rcode = PNF_VAR::Write(_ios,data);
	FillMaskFieldWR();
	return(rcode);
};
//------------------------------------------------------------------------------------------//
inline const PNF_MASK& PNF_MASK::operator = (uint32 data){
	Write(cgStartup.ios,data);
	return(*this);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
inline void PNF_HEAD::InitPN(const ARRAY* _out,const ARRAY* _in,const STDSTR& fixedStr){
	cgFixedStr = fixedStr;
	PNF::InitPN(_out,_in,(uint32)cgFixedStr.length());
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
	Write(cgStartup.ios, _in);
	return(*this);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
inline void	PNF_BLOCK::InitPN(const ARRAY* _out,const ARRAY* _in,const PNF* pn){
	PNF::InitPN(_out,_in,0);
	SetBlockPoint(pn);
};
//------------------------------------------------------------------------------------------//
inline TNF* PNF_BLOCK::AppendDownNode(TNF *pn){
	SetBlockPoint((PNF*)pn);
	return(PNF::AppendDownNode(pn));
};
//------------------------------------------------------------------------------------------//
inline void PNF_BLOCK::SetBlockPoint(const PNF* pn){
	cgPNF = (pn == nullptr) ? this : (PNF*)pn;
};
//------------------------------------------------------------------------------------------//
inline IOSE PNF_BLOCK::Read(IOS* _ios,const UVOut& _out){
	if (cgPNF == this)
		return(PNF::Read(_ios,_out));
	return(cgPNF->Read(_ios,_out));
};
//------------------------------------------------------------------------------------------//
inline bool32 PNF_BLOCK::ChecksumResult(void)const{
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
inline IOSE PNF_BLOCK::DoTransform(IOS* _ios,const UVOut& _out,const uint8* data,const uint64& length){
	IOSE rcode;
	if (cgPNF == this){
		rcode = PNF::DoTransform(_ios,_out,data,length);
	}
	else{
		rcode = cgPNF->Transform(_ios,data,length);
		POS_update(&cgPosWR);
	}
	return(rcode);
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
inline const PNFB_LC& PNFB_LC::operator = (const UVIn& _in){
	Write(cgStartup.ios,_in);
	return(*this);
};
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
inline IOSE PNF_SCC::_Begin(IOS* _ios){
	if (CheckSFlag(DSTF_blStart) == G_FALSE)
		return(PNFB_LC::_Begin(_ios));

	cgStartup.ios = _ios;
	pnlc_Len._SetIOS(_ios);
	pnlc_Text._SetIOS(_ios);
	return(IOS_update(_ios,IOS_OK));
};
//------------------------------------------------------------------------------------------//
inline IOSE PNF_SCC::_Endl(IOS* _ios){
	if (CheckSFlag(DSTF_blStart))
		return(PNFB_LC::_Endl(_ios));
	return(IOS_update(_ios,IOS_FINISH));
};
//------------------------------------------------------------------------------------------//









//------------------------------------------------------------------------------------------//
inline IOSE PNF_SC::_Begin(IOS* _ios){
	ResetPosWR();
	cgEA = 0;
	return(PNF::_Begin(_ios));
};
//------------------------------------------------------------------------------------------//
inline IOSE PNF_SC::Read(IOS* _ios,const UVOut& _out){
	IOSE rcode = IOS_OK;
	
	IOS_update(&rcode, pnsc_LC[0].Read(_ios,_out));
	if (rcode > 0)
		IOS_update(&rcode, pnsc_LC[1].Read(_ios,_out));
	if (rcode > 0)
		IOS_update(&rcode, pnsc_LC[2].Read(_ios,_out));
	if (rcode > 0)
		IOS_update(&rcode, pnsc_LC[3].Read(_ios,_out));
	return(rcode);
};
//------------------------------------------------------------------------------------------//
inline const PNF_SC& PNF_SC::operator = (const UVIn& _in){
	Write(cgStartup.ios, _in);
	return(*this);
};
//------------------------------------------------------------------------------------------//









//------------------------------------------------------------------------------------------//
inline PNF_BLOCK& PNFB_SHELL::AddBlockSubPN(PNF& subPN){
	pns_Block.AppendDown(subPN);
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











//------------------------------------------------------------------------------------------//
template <typename T_PNF> PNFB_LIST<T_PNF>::PNFB_LIST(void) : PNF_BLOCK(){
	cgNum = 0;
	TNFP::SetSelfName("PNF_LIST");
	SetUpName("");
};
//------------------------------------------------------------------------------------------//
template <typename T_PNF> inline void PNFB_LIST<T_PNF>::SetSelfName(const STDSTR& strName){
	selfName = strName;
	pnl_Qty.SetUpName(GetFullName(this));
	pnl_Text.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
template <typename T_PNF> inline void PNFB_LIST<T_PNF>::SetUpName(const STDSTR& strName){
	fatherName = strName;
	pnl_Qty.SetUpName(GetFullName(this));
	pnl_Text.SetUpName(GetFullName(this));
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
template <typename T_PNF> IOSE PNFB_LIST<T_PNF>::_Begin(IOS* _ios){
	IOSE	rcode = IOS_OK;

	cgNum = 0;
	IOS_update(&rcode, PNF_BLOCK::_Begin(_ios));
	IOS_update(&rcode, pnl_Qty.Write(_ios,0));
	
	return(rcode);
};
//------------------------------------------------------------------------------------------//
template <typename T_PNF> IOSE PNFB_LIST<T_PNF>::_Endl(IOS* _ios){
	IOSE	rcode = IOS_OK;
	if (cgNum > 0)
		IOS_update(&rcode, pnl_Text._Endl(_ios));
	IOS_update(&rcode, pnl_Qty.Update(cgNum));
	IOS_update(&rcode, PNF_BLOCK::_Endl(_ios));
	return(rcode);
};
//------------------------------------------------------------------------------------------//
template <typename T_PNF>
IOSE PNFB_LIST<T_PNF>::DoTransform(IOS* _ios,const UVOut& _out,const uint8* data,const uint64& length){
	IOSE	rcode = IOS_OK;
	if (cgNum == 0){
		++ cgNum;
		IOS_update(&rcode,pnl_Text._Begin(_ios));
	}
	return(IOS_update(&rcode,pnl_Text.Transform(_ios,data,length)));
};
//------------------------------------------------------------------------------------------//
template <typename T_PNF> PNFB_LIST<T_PNF>& PNFB_LIST<T_PNF>::NewText(void){
	if (cgNum > 0)
		pnl_Text._Endl(cgStartup.ios);
	
	++ cgNum;
	pnl_Text._Begin(cgStartup.ios);
	return(*this);
};
//------------------------------------------------------------------------------------------//
namespace PNFLISTNAMESPACE {
	template <typename T_PNF,typename T> IOSE AddText(IOS* _ios,uint32* num,T_PNF* _pnf){
		return(IOS_update(_ios, IOS_OK));
	};
	
	template <typename T_PNF,typename T> IOSE AddText(IOS* _ios,uint32* num,T_PNF* _pnf,const T& _in){
		*num += 1;
		return(_pnf->Write(_ios,_in));
	};
	
	template<typename T_PNF,typename T,typename... Args>
	static inline IOSE AddText(IOS* _ios,uint32* num,T_PNF* _pnf,const T& first,const Args&... args){
		IOSE rcode = IOS_OK;
		*num += 1;
		IOS_update(&rcode,_pnf->Write(_ios,first));
		IOS_update(&rcode,AddText(_ios,num,_pnf,args...));
		return(rcode);
	};
};
//------------------------------------------------------------------------------------------//
template <typename T_PNF>
template<typename... Args> IOSE PNFB_LIST<T_PNF>::Write(IOS* _ios,const Args&... args){
	IOSIN	iosin;
	IOS_copy(&iosin, _ios);
	
	cgNum = 0;
	IOS_update(&iosin, _Begin(_ios));
	IOS_update(&iosin, PNFLISTNAMESPACE::AddText(_ios,&cgNum,&pnl_Text,args...));
	IOS_update(&iosin, pnl_Qty.Update(cgNum));
	IOS_update(&iosin, PNF_BLOCK::_Endl(_ios));
	return(IOS_copy(_ios, iosin));
};
//------------------------------------------------------------------------------------------//
template <typename T_PNF> uint32 PNFB_LIST<T_PNF>::ReadQty(void)const{
	return(pnl_Qty.GetValueRE());
};
//------------------------------------------------------------------------------------------//
template <typename T_PNF> IOSE PNFB_LIST<T_PNF>::Read(IOS* _ios,const UVOut& _out,uint32 order){
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
	return(IOS_update(_ios,IOS_ERR));
};
//------------------------------------------------------------------------------------------//










#define TPNFM PNFS_MESG<T_PNF>
//------------------------------------------------------------------------------------------//
template <typename T_PNF> TPNFM::PNFS_MESG(void) : PNFB_SHELL(){
	AddBlockSubPN(pnm_ID) < pnm_Text;
	
	TNFP::SetSelfName("PNFS_MESG");
	pnm_Text.SetSelfName("Text");
	SetUpName("");
};
//------------------------------------------------------------------------------------------//
template <typename T_PNF> inline void TPNFM::SetSelfName(const STDSTR& strName){
	selfName = strName;
	pnm_Text.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
template <typename T_PNF> inline void TPNFM::SetUpName(const STDSTR& strName){
	fatherName = strName;
	pnm_Text.SetUpName(GetFullName(this));
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
};
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
template <typename T_PNF> inline bool32 TPNFM::MarkedRead(void)const{
	return(pnm_ID.SetMaxValueRE());
};
//------------------------------------------------------------------------------------------//
template <typename T_PNF> inline bool32 TPNFM::IsUnread(void)const{
	return(pnm_ID.IsMaxValueRE() == G_FALSE);
};
//------------------------------------------------------------------------------------------//
template <typename T_PNF> inline IOSE TPNFM::Write(IOS* _ios,uint32 mID,const UVIn& _in){
	IOSIN	iosin;
	IOS_copy(&iosin,_ios);

	IOS_update(&iosin, _Begin				(_ios));
	IOS_update(&iosin, pnm_ID.Update		(mID));
	IOS_update(&iosin, pnm_Text.Transform	(_ios, _in));
	IOS_update(&iosin, _Endl				(_ios));
	
	return(IOS_copy(_ios, iosin));
};
//------------------------------------------------------------------------------------------//
template <typename T_PNF> IOSE TPNFM::_Begin(IOS* _ios){
	IOSE rcode = IOS_OK;

	IOS_update(&rcode, PNFB_SHELL::_Begin	(_ios));
	IOS_update(&rcode, pnm_ID.Write			(_ios,0));
	IOS_update(&rcode, pnm_Text._Begin		(_ios));
	return(rcode);
};
//------------------------------------------------------------------------------------------//
template <typename T_PNF> IOSE TPNFM::_Endl(IOS* _ios){
	IOSE	rcode = IOS_OK;
	IOS_update(&rcode,pnm_Text._Endl		(_ios));
	IOS_update(&rcode,PNFB_SHELL::_Endl		(_ios));
	return(rcode);
};
//------------------------------------------------------------------------------------------//








#ifdef ALG_DS_CRC
#define TPNMFCRC PNFS_MESG_CRC<T_PNF,T_CRC>
//------------------------------------------------------------------------------------------//
template <typename T_PNF,typename T_CRC> TPNMFCRC::PNFS_MESG_CRC(void) : PNFS_MESG<T_PNF>(){
	cgCRCCFG = -1;
	TPNFM::SetSelfName("PNFS_MESG_CRC");
	cgCRC0.selfName = "CRC0";
	cgCRC1.selfName = "CRC1";
	SetUpName("");
};
//------------------------------------------------------------------------------------------//
template <typename T_PNF,typename T_CRC> inline void TPNMFCRC::SetSelfName(const STDSTR& strName){
	TPNFM::SetSelfName(strName);
	SetUpName(TPNFM::fatherName);
};
//------------------------------------------------------------------------------------------//
template <typename T_PNF,typename T_CRC> inline void TPNMFCRC::SetUpName(const STDSTR& strName){
	TPNFM::SetUpName(strName);
	cgCRC0.SetUpName(TPNFM::GetFullName(this));
	cgCRC1.SetUpName(TPNFM::GetFullName(this));
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
template <typename T_SHELL> bool32 DSTF_TEST::SHELL_Test(IOS* _ios,T_SHELL* _shell,STDSTR* _out,const STDSTR& _in){
	bool32 retbl,result;
	STDSTR _strIn;
	_shell[0].GetDefArrayWR()->Reset();
	_shell[0].GetDefArrayRE()->Reset();
	_shell[1].GetDefArrayWR()->Reset();
	_shell[1].GetDefArrayRE()->Reset();

	result = G_TRUE;
	printf("----------------------------------------------------------\n");
	_shell[0].Write(IOS_clr(_ios), _in);
	PrintResult(_shell[0].selfName + " Write   ",ShowINOUT(_ios),1);
	PrintResult(_shell[0].selfName + " Analysis","",_shell[0].Analysis(0));
	retbl = _shell[0].Read(IOS_clr(_ios), _EMPTY(_out)) > 0;
	result &= retbl;
	PrintResult(_shell[0].selfName + " Read    ",ShowINOUT(_ios),retbl);
	PrintResult(_shell[0].selfName + " in==out ",ShowINOUT("",_in.length(),_out->length()),(*_out == _in));
	result &= (*_out == _in);
	
	_shell[0].GetDefArrayWR()->Reset();
	_shell[0].GetDefArrayRE()->Reset();
	_shell[1].GetDefArrayWR()->Reset();
	_shell[1].GetDefArrayRE()->Reset();
	
	_strIn = "1234567890";
	_shell[0] << Begin(IOS_clr(_ios)) << _strIn << _strIn << Endl;
	_strIn += _strIn;
	
	PrintResult(_shell[1].selfName + " Analysis","",_shell[1].Analysis(0));
	retbl = _shell[1].Read(IOS_clr(_ios), _EMPTY(_out)) > 0;
	result &= retbl;
	PrintResult(_shell[1].selfName + " Read    ",ShowINOUT(_ios),retbl);
	PrintResult(_shell[1].selfName + " in==out ",ShowINOUT("",_strIn.length(),_out->length()),(*_out == _strIn));
	result &= (*_out == _strIn);
	
	_shell[0].GetDefArrayWR()->Reset();
	_shell[0].GetDefArrayRE()->Reset();
	_shell[1].GetDefArrayWR()->Reset();
	_shell[1].GetDefArrayRE()->Reset();
	return(result);
};
//------------------------------------------------------------------------------------------//
template <typename T_SHELL> bool32 DSTF_TEST::SHELL_Test(IOS* _ios,T_SHELL* _shell,STDSTR* _out,uint32 mID,const STDSTR& _in){
	uint32 _mID;
	bool32 retbl,result;
	STDSTR strMID;
	
	_shell[0].GetDefArrayWR()->Reset();
	_shell[0].GetDefArrayRE()->Reset();
	_shell[1].GetDefArrayWR()->Reset();
	_shell[1].GetDefArrayRE()->Reset();

	result = G_TRUE;
	printf("----------------------------------------------------------\n");
	_shell[0].Write(IOS_clr(_ios), mID, _in);
	PrintResult(_shell[0].selfName + " Write   ",ShowINOUT(_ios),1);
	
	retbl = _shell[0].Analysis(0) > 0;
	result &= retbl;
	PrintResult(_shell[0].selfName + " Analysis","",retbl);
	_mID = _shell[0].ReadID();
	PrintResult(_shell[0].selfName + " mID     ",ShowINOUT("W/R:",mID,_mID),(mID == _mID));
	result &= (mID == _mID);
	
	retbl = _shell[0].Read(IOS_clr(_ios), _EMPTY(_out)) > 0;
	result &= retbl;
	PrintResult(_shell[0].selfName + " Read    ",ShowINOUT(_ios),retbl);
	PrintResult(_shell[0].selfName + " in==out ",ShowINOUT("",_in.length(),_out->length()),(*_out == _in));
	result &= (*_out == _in);
	
	
	
	retbl = _shell[1].Analysis(0) > 0;
	result &= retbl;
	PrintResult(_shell[1].selfName + " Analysis","",retbl);
	_mID = _shell[1].ReadID();
	PrintResult(_shell[1].selfName + " mID     ",ShowINOUT("W/R:",mID,_mID),(mID == _mID));
	result &= (mID == _mID);
	
	retbl = _shell[1].Read(IOS_clr(_ios), _EMPTY(_out)) > 0;
	result &= retbl;
	PrintResult(_shell[1].selfName + " Read    ",ShowINOUT(_ios),retbl);
	PrintResult(_shell[1].selfName + " in==out ",ShowINOUT("",_in.length(),_out->length()),(*_out == _in));
	result &= (*_out == _in);
	
	_shell[0].GetDefArrayWR()->Reset();
	_shell[0].GetDefArrayRE()->Reset();
	_shell[1].GetDefArrayWR()->Reset();
	_shell[1].GetDefArrayRE()->Reset();
	return(result);
};
//------------------------------------------------------------------------------------------//
static inline STDSTR DSTF_TEST::ShowINOUT(IOS* _ios){
	return(ShowINOUT("In/Out=",_ios->total_in,_ios->total_out));
};
//------------------------------------------------------------------------------------------//
static inline STDSTR DSTF_TEST::ShowINOUT(const STDSTR& name,uint64 _in,uint64 _out){
	STDSTR ret;
	ret = name;
	ret += Str_ToStr(_in);
	ret += "/";
	ret += Str_ToStr(_out);
	return(ret);
};
//------------------------------------------------------------------------------------------//
static inline void DSTF_TEST::PrintResult(const STDSTR& name,const STDSTR& method,bool32 blok){
	if (blok){
		printf("MESG_Test< %s > is ok. %s\r\n",name.c_str(),method.c_str());
	}
	else{
		printf("MESG_Test< %s > is fail.\r\n",name.c_str());
	}
};
//------------------------------------------------------------------------------------------//
#endif /* ADS_Protocol_h */
#endif /* ADS_Protocol_hpp */
