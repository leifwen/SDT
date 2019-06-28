//
//  ADS_Protocol.cpp
//  SDT
//
//  Created by Leif Wen on 24/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "ADS_Protocol.h"
//------------------------------------------------------------------------------------------//
#ifdef ADS_Protocol_h
//------------------------------------------------------------------------------------------//
ADS_FIFO::ADS_FIFO(void) : DSTF(){
	POS_reset(&cgPosWR);
	POS_reset(&cgPosRE);
	SetDefArrayWR(nullptr);
	SetDefArrayRE(nullptr);
};
//------------------------------------------------------------------------------------------//
IOSE ADS_FIFO::_Begin(IOS* _ios){
	DSTF::_Begin(_ios);
	_SetOut(cgPosWR.uvOut);
	POS_hold(&cgPosWR);
	return(IOS_update(_ios, IOS_OK));
};
//------------------------------------------------------------------------------------------//
IOSE ADS_FIFO::_Endl(IOS* _ios){
	uint32	gNum,preNum;
	IOSE 	rcode;
	rcode = DSTF::_Endl(_ios);
	gNum = cgPosWR.array->GetPreInNum();
	preNum = cgPosWR.array->Prepare_Clr(cgPosWR.array->IsNoMEM());
	if ((preNum == 0) && (rcode == IOS_NO_MEM) && (_ios != nullptr)){
		_ios->avail_out += gNum;
		_ios->total_out -= gNum;
	}
	return(rcode);
};
//------------------------------------------------------------------------------------------//
IOSE ADS_FIFO::AllIn(IOS* _ios,const UVIn& _in){
	IOSIN	iosin;
	IOS_copy(&iosin,_ios);
	
	IOS_update(&iosin, _Begin		(_ios));
	IOS_update(&iosin, Transform	(_ios, _in));
	IOS_update(&iosin, Final		(_ios));
	IOS_update(&iosin, _Endl		(_ios));
	
	return(IOS_copy(_ios,iosin));
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
enum{
	PNF_RET_OK = 1,
	PNF_RET_ERR = 0,
	PNF_RET_NoEnoughDataInArray = -1,
	PNF_RET_AbandonFirstData = -2,
	PNF_RET_NoFirstChild = -3,
	PNF_RET_CheckNextTail = -4,
};
//------------------------------------------------------------------------------------------//
PNF::PROTOCOL_NODE_FRAME(void) : ADS_FIFO(){
	cgFixedByte = 0;
	cgPNF = nullptr;
};
//------------------------------------------------------------------------------------------//
bool32 PNF::Analysis(uint32 startOffset){
	bool32	err = PNF_RET_OK;
	
	cgPosRE.offset = startOffset;
	cgPosRE.length = cgFixedByte;
	ResetPNLength();
	if (cgPosRE.length > 0){
		if ((cgPosRE.offset + cgPosRE.length) > cgPosRE.array->Used())
			err = PNF_RET_NoEnoughDataInArray;
	}
	return(err);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
PNF_VAR::PNF_VAR(void) : PNF(){
	cgMask = 0xffffffff;
	cgMovebit = 0;
};
//------------------------------------------------------------------------------------------//
uint8* CreateByte(uint8* buf,uint32 data,uint32 bytes,bool32 blEndianBig){
	if (blEndianBig){
		buf[0] = (uint8)(data >> 24);
		buf[1] = (uint8)(data >> 16);
		buf[2] = (uint8)(data >> 8);
		buf[3] = (uint8)(data);
		buf += (4 - bytes);
	}
	else{
		buf[0] = (uint8)(data);
		buf[1] = (uint8)(data >> 8);
		buf[2] = (uint8)(data >> 16);
		buf[3] = (uint8)(data >> 24);
	}
	return(buf);
};
//------------------------------------------------------------------------------------------//
IOSE PNF_VAR::SetByte4(IOS* _ios,uint32 data){
	uint8	buf[4];
	IOSE	rcode;
	uint64	total_in = 0;
	
	cgPosWR.length = 4;

	if (_ios != nullptr)
		total_in = _ios->total_in;

	rcode = Save(_ios, cgPosWR.uvOut, CreateByte(buf,data,cgPosWR.length,CheckSFlag(PNF_blEndianBig)), cgPosWR.length);
	
	if (_ios != nullptr)
		_ios->total_in = total_in;
	return(rcode);
};
//------------------------------------------------------------------------------------------//
IOSE PNF_VAR::SetByte3(IOS* _ios,uint32 data){
	uint8	buf[4];
	IOSE	rcode;
	uint64	total_in = 0;
	
	cgPosWR.length = 3;
	
	if (_ios != nullptr)
		total_in = _ios->total_in;
	
	rcode = Save(_ios, cgPosWR.uvOut, CreateByte(buf,data,cgPosWR.length,CheckSFlag(PNF_blEndianBig)), cgPosWR.length);
	
	if (_ios != nullptr)
		_ios->total_in = total_in;
	return(rcode);
};
//------------------------------------------------------------------------------------------//
IOSE PNF_VAR::SetByte2(IOS* _ios,uint32 data){
	uint8	buf[4];
	IOSE	rcode;
	uint64	total_in = 0;
	
	cgPosWR.length = 2;
	
	if (_ios != nullptr)
		total_in = _ios->total_in;
	
	rcode = Save(_ios, cgPosWR.uvOut, CreateByte(buf,data,cgPosWR.length,CheckSFlag(PNF_blEndianBig)), cgPosWR.length);
	
	if (_ios != nullptr)
		_ios->total_in = total_in;
	return(rcode);
};
//------------------------------------------------------------------------------------------//
IOSE PNF_VAR::SetByte1(IOS* _ios,uint32 data){
	uint8	buf[4];
	IOSE	rcode;
	uint64	total_in = 0;
	
	cgPosWR.length = 1;
	
	if (_ios != nullptr)
		total_in = _ios->total_in;
	
	rcode = Save(_ios, cgPosWR.uvOut, CreateByte(buf,data,cgPosWR.length,CheckSFlag(PNF_blEndianBig)), cgPosWR.length);
	
	if (_ios != nullptr)
		_ios->total_in = total_in;
	return(rcode);
};
//------------------------------------------------------------------------------------------//
IOSE PNF_VAR::SetByte(IOS* _ios,uint32 data){
	uint8	buf[4];
	IOSE	rcode;
	uint64	total_in = 0;

	cgPosWR.length = cgFixedByte;

	if (_ios != nullptr)
		total_in = _ios->total_in;
	
	rcode = Save(_ios, cgPosWR.uvOut, CreateByte(buf,data,cgPosWR.length,CheckSFlag(PNF_blEndianBig)), cgPosWR.length);
	
	if (_ios != nullptr)
		_ios->total_in = total_in;
	return(rcode);
};
//------------------------------------------------------------------------------------------//
IOSE PNF_VAR::UpdateByte4(uint32 data){
	uint8	buf[4];
	if (CheckSFlag(PNF_blEndianBig)){
		buf[0] = (uint8)(data >> 24);
		buf[1] = (uint8)(data >> 16);
		buf[2] = (uint8)(data >> 8);
		buf[3] = (uint8)(data);
	}
	else{
		buf[0] = (uint8)(data);
		buf[1] = (uint8)(data >> 8);
		buf[2] = (uint8)(data >> 16);
		buf[3] = (uint8)(data >> 24);
	}
	return((cgPosWR.array->UpdateByOffsetIn(buf, 4, cgPosWR.offset) == 4) ? IOS_OK : IOS_ERR);
};
//------------------------------------------------------------------------------------------//
IOSE PNF_VAR::UpdateByte3(uint32 data){
	uint8	buf[3];
	if (CheckSFlag(PNF_blEndianBig)){
		buf[0] = (uint8)(data >> 16);
		buf[1] = (uint8)(data >> 8);
		buf[2] = (uint8)(data);
	}
	else{
		buf[0] = (uint8)(data);
		buf[1] = (uint8)(data >> 8);
		buf[2] = (uint8)(data >> 16);
	}
	return((cgPosWR.array->UpdateByOffsetIn(buf, 3, cgPosWR.offset) == 3) ? IOS_OK : IOS_ERR);
};
//------------------------------------------------------------------------------------------//
IOSE PNF_VAR::UpdateByte2(uint32 data){
	uint8	buf[2];
	if (CheckSFlag(PNF_blEndianBig)){
		buf[0] = (uint8)(data >> 8);
		buf[1] = (uint8)(data);
	}
	else{
		buf[0] = (uint8)(data);
		buf[1] = (uint8)(data >> 8);
	}
	return((cgPosWR.array->UpdateByOffsetIn(buf, 2, cgPosWR.offset) == 2) ? IOS_OK : IOS_ERR);
};
//------------------------------------------------------------------------------------------//
IOSE PNF_VAR::UpdateByte1(uint32 data){
	uint8 	tData;
	tData = (uint8)(data);
	return((cgPosWR.array->UpdateByOffsetIn(&tData, 1, cgPosWR.offset) == 1) ? IOS_OK : IOS_ERR);
};
//------------------------------------------------------------------------------------------//
IOSE PNF_VAR::UpdateByte(uint32 data){
	IOSE	rcode;
	switch(cgFixedByte){
		case 1:
			rcode = UpdateByte1(data);break;
		case 2:
			rcode = UpdateByte2(data);break;
		case 3:
			rcode = UpdateByte3(data);break;
		case 4:
		default:
			rcode = UpdateByte4(data);break;
	}
	return(rcode);
};
//------------------------------------------------------------------------------------------//
uint32 PNF_VAR::GetOValueRE(void)const{
	uint32 ret,i;
	uint8 tData;
	
	tData = 0;
	ret = 0;
	if (CheckSFlag(PNF_blEndianBig)){
		i = 0;
		do{
			ret <<= 8;
			cgPosRE.array->Read(&tData, 1, cgPosRE.offset + i);
			ret |= tData;
		}while(++i < cgPosRE.length);
	}
	else{
		i = cgPosRE.length;
		while(i-- > 0){
			ret <<= 8;
			cgPosRE.array->Read(&tData, 1, cgPosRE.offset + i);
			ret |= tData;
		};
	}
	return(ret);
};
//------------------------------------------------------------------------------------------//
uint32 PNF_VAR::GetOValueWR(void)const{
	uint32 ret,i;
	uint8 tData;
	
	tData = 0;
	ret = 0;
	if (CheckSFlag(PNF_blEndianBig)){
		i = 0;
		do{
			ret <<= 8;
			cgPosWR.array->ReadByOffsetIn(&tData, 1, cgPosWR.offset + i);
			ret |= tData;
		}while(++i < cgPosWR.length);
	}
	else{
		i = cgPosWR.length;
		while(i-- > 0){
			ret <<= 8;
			cgPosWR.array->ReadByOffsetIn(&tData, 1, cgPosWR.offset + i);
			ret |= tData;
		};
	}
	return(ret);
};
//------------------------------------------------------------------------------------------//
void PNF_VAR::SetMaskBit(uint32 mask){
	uint32	tMask,i;
	
	cgMask = mask;
	i = 0;
	tMask = cgMask;
	cgMovebit = 0;
	do{
		if ((tMask & 0x00000001) > 0)
			break;
		tMask >>= 1;
		++ cgMovebit;
	}while(++ i < 32);
};
//------------------------------------------------------------------------------------------//
IOSE PNF_VAR::Write(IOS* _ios,uint32 data){
	IOSIN	iosin;
	IOS_copy(&iosin,_ios);
	
	IOS_update(&iosin, _Begin	(_ios));
	IOS_update(&iosin, SetByte	(_ios,data));
	IOS_update(&iosin, _Endl	(_ios));
	
	return(IOS_copy(_ios,iosin));
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
void PNF_MASK::FillMaskFieldRE(void){
	TREE_DownChain_Traversal_LINE_nolock(PNF_VAR,this,
		_opNode->SyncPosRE(&cgPosRE);
		_opNode->SetFixedByte(cgFixedByte);
		_opNode->SetEndian(CheckSFlag(PNF_blEndianBig) ? G_ENDIAN_BIG : G_ENDIAN_LITTLE);
	);
};
//------------------------------------------------------------------------------------------//
void PNF_MASK::FillMaskFieldWR(void){
	TREE_DownChain_Traversal_LINE_nolock(PNF_VAR,this,
		_opNode->SyncPosWR(&cgPosWR);
		_opNode->SetFixedByte(cgFixedByte);
		_opNode->SetEndian(CheckSFlag(PNF_blEndianBig) ? G_ENDIAN_BIG : G_ENDIAN_LITTLE);
	);
};
//------------------------------------------------------------------------------------------//
bool32 PNF_MASK::Analysis(uint32 startOffset){
	bool32 err;
	err = PNF_VAR::Analysis(startOffset);
	if (err == PNF_RET_OK)
		FillMaskFieldRE();
	return(err);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
PNF_HEAD::PNF_HEAD(void) : PNF(){
	cgFixedStr = "";
};
//------------------------------------------------------------------------------------------//
bool32 PNF_HEAD::CheckFixedStr(void)const{
	uint8	charP1,charP2;
	uint32	tOffset,fLength;
	
	tOffset = cgPosRE.offset + cgPosRE.length;
	fLength = cgPosRE.length;
	charP1 = 0;
	while(fLength-- > 0){
		cgPosRE.array->Read(&charP1, 1, --tOffset);
		charP2 = cgFixedStr[fLength];
		if (charP1 != charP2)
			return PNF_RET_AbandonFirstData;
	}
	return PNF_RET_OK;
};
//------------------------------------------------------------------------------------------//
bool32 PNF_HEAD::Analysis(uint32 startOffset){
	//PNF::Analysis return:PNF_RET_NoEnoughDataInArray,PNF_RET_OK
	//PNF_RET_NoEnoughDataInArray
	//PNF_RET_OK
	//PNF_RET_AbandonFirstData

	bool32 err;
	err = PNF::Analysis(startOffset);
	if (err == PNF_RET_OK)
		err = CheckFixedStr();
	return(err);
};
//------------------------------------------------------------------------------------------//
IOSE PNF_HEAD::Write(IOS* _ios){
	IOSE	rcode;
	uint64	total_in = 0;
	
	if (_ios != nullptr)
		total_in = _ios->total_in;
	
	rcode = AllIn(_ios,cgFixedStr);
	
	if (_ios != nullptr)
		_ios->total_in = total_in;
	return(rcode);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
bool32 PNF_TAIL::Analysis(uint32 startOffset){
	//PNF_HEAD::Analysis return:PNF_RET_NoEnoughDataInArray,PNF_RET_OK,PNF_RET_AbandonFirstData
	//PNF_RET_NoEnoughDataInArray
	//PNF_RET_OK
	//PNF_RET_AbandonFirstData

	bool32 err;
	err = PNF_HEAD::Analysis(startOffset);
	if (err == PNF_RET_OK){
		if (cgPNF != nullptr) //the PN_CONTENT length decide by PN_TAIL,set PN_CONTENT->length
			cgPNF->SetLengthRE(cgPosRE.offset - cgPNF->GetOffsetRE());
	}
	else if (err == PNF_RET_AbandonFirstData){//tail field no match
		if (cgPNF != nullptr) //the PN_CONTENT length decide by PN_TAIL
			err = PNF_RET_CheckNextTail;
	}
	return(err);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
void PNF_CONTENT::ResetPNLength(void){
	if (cgPNF != nullptr){//PN's length decide by PN_LENGTH
		cgPosRE.length = ((PNF_VAR*)cgPNF)->GetValueCalcRE();
	}
	else{
		cgPosRE.length = cgFixedByte;
	}
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
PNF_BLOCK::PNF_BLOCK(void): PNF(){
	cgPNF = this;
};
//------------------------------------------------------------------------------------------//
bool32 PNF_BLOCK::Analysis(uint32 startOffset){
	PNF		*fieldNode;
	uint32	getNum;
	int32	ret;
	
	cgPosRE.offset = startOffset;
	cgPosRE.length = 0;
	
	fieldNode = (PNF*)GetDown(this); //read Head;
	if (fieldNode == nullptr)
		return PNF_RET_NoFirstChild;
	
	getNum = 0;
	do{
	GET_DATA:;
		ret = fieldNode->Analysis(cgPosRE.offset + getNum);
		if (ret == PNF_RET_CheckNextTail){
			++ getNum;//abandon first byte data.
			goto GET_DATA;
		}
		if (ret != PNF_RET_OK)
			return ret;
		getNum += fieldNode->GetLengthRE();
		fieldNode = (PNF*)GetNext(fieldNode);//read next node;
	}while(fieldNode != nullptr);
	if (ChecksumResult() > 0){
		cgPosRE.length = getNum;
		return PNF_RET_OK;
	}
	return PNF_RET_AbandonFirstData;
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
PNFB_LC::PNFB_LC(void) : PNF_BLOCK(){
	AppendDown(pnlc_Len) < pnlc_Text;
};
//------------------------------------------------------------------------------------------//
IOSE PNFB_LC::_Begin(IOS* _ios){
	IOSE rcode = IOS_OK;
	
	IOS_update(&rcode, PNF_BLOCK::_Begin(_ios));
	IOS_update(&rcode, pnlc_Len.Write	(_ios,-1));
	IOS_update(&rcode, pnlc_Text._Begin	(_ios));
	
	return(rcode);
};
//------------------------------------------------------------------------------------------//
IOSE PNFB_LC::_Endl(IOS* _ios){
	IOSE rcode = IOS_OK;
	IOS_update(&rcode, pnlc_Text._Endl(_ios));
	IOS_update(&rcode, pnlc_Len.Update(pnlc_Text.GetLengthWR()));
	IOS_update(&rcode, PNF_BLOCK::_Endl(_ios));
	return(rcode);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
PNF_SCC::PNF_SCC(void) : PNFB_LC(){
	cgMaxSize = 0;
};
//------------------------------------------------------------------------------------------//
void PNF_SCC::ResetPosWR(void){
	POS_reset(&cgPosWR);
	pnlc_Len.ResetPosWR();
	pnlc_Text.ResetPosWR();
};
//------------------------------------------------------------------------------------------//
void PNF_SCC::ResetPosRE(void){
	POS_reset(&cgPosRE);
	pnlc_Len.ResetPosRE();
	pnlc_Text.ResetPosRE();
};
//------------------------------------------------------------------------------------------//
IOSE PNF_SCC::Write(IOS* _ios,uint32* retNum,const uint8 *data,uint32 num){
	uint32	size;
	IOSE	rcode;
	
	rcode = _Begin(_ios);
	size = cgMaxSize - pnlc_Text.GetLengthWR();
	if (num > size)
		num = size;
	IOS_update(&rcode, pnlc_Text.Transform(_ios, data, num));
	POS_update(&cgPosWR);
	if (IsFull())
		IOS_update(&rcode,_Endl(_ios));
	*retNum = num;
	return(rcode);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
PNF_SC::PNF_SC(void) : PNF(){
	cgEA = 0;
};
//------------------------------------------------------------------------------------------//
void PNF_SC::InitPN(const ARRAY* _out,const ARRAY* _in){
	if (_out == nullptr)
		_out = &cgArray;
	if (_in == nullptr)
		_in = &cgArray;
	
	PNF::InitPN	 (_out,_in, 0);
	pnsc_LC[0].InitPN(_out,_in, 0xff >> 2);
	pnsc_LC[1].InitPN(_out,_in, 0xffff >> 2);
	pnsc_LC[2].InitPN(_out,_in, 0xffffff >> 2);
	pnsc_LC[3].InitPN(_out,_in, 0xffffffff >> 2);
};
//------------------------------------------------------------------------------------------//
void PNF_SC::ResetPosWR(void){
	POS_reset(&cgPosWR);
	pnsc_LC[0].ResetPosWR();
	pnsc_LC[1].ResetPosWR();
	pnsc_LC[2].ResetPosWR();
	pnsc_LC[3].ResetPosWR();
};
//------------------------------------------------------------------------------------------//
void PNF_SC::ResetPosRE(void){
	POS_reset(&cgPosRE);
	pnsc_LC[0].ResetPosRE();
	pnsc_LC[1].ResetPosRE();
	pnsc_LC[2].ResetPosRE();
	pnsc_LC[3].ResetPosRE();
};
//------------------------------------------------------------------------------------------//
int32 PNF_SC::Analysis(uint32 startOffset){
	uint32	getNum;
	uint32	len;
	uint32	l0,l1,l2,l3,ea;
	
	ResetPosRE();
	cgPosRE.offset = startOffset;
	cgPosRE.length = 0;
	
	if (pnsc_LC[0].pnlc_Len.Analysis(cgPosRE.offset) != PNF_RET_OK)
		return PNF_RET_NoEnoughDataInArray;
	getNum = pnsc_LC[0].pnlc_Len.GetLengthRE();
	
	len = pnsc_LC[0].pnlc_Len.GetValueRE();
	ea = len & 0x03;
	
	l0 = (ea == 0) ? (len >> 2) : (0xff >> 2);
	pnsc_LC[0].pnlc_Text.SetFixedByte(l0);
	pnsc_LC[0].pnlc_Text.ResetPosRE();
	if (pnsc_LC[0].pnlc_Text.Analysis(cgPosRE.offset + getNum) != PNF_RET_OK)
		return PNF_RET_NoEnoughDataInArray;
	getNum += pnsc_LC[0].pnlc_Text.GetLengthRE();
	
	if (ea > 0){
		if (pnsc_LC[1].pnlc_Len.Analysis(cgPosRE.offset + getNum) != PNF_RET_OK)
			return PNF_RET_NoEnoughDataInArray;
		getNum += pnsc_LC[1].pnlc_Len.GetLengthRE();
		
		len |= (pnsc_LC[1].pnlc_Len.GetValueRE() << 8);
		
		l1 = (ea == 1) ? (len >> 2) : (0xffff >> 2);
		pnsc_LC[1].pnlc_Text.SetFixedByte(l1);
		pnsc_LC[1].pnlc_Text.ResetPosRE();
		if (pnsc_LC[1].pnlc_Text.Analysis(cgPosRE.offset + getNum) != PNF_RET_OK)
			return PNF_RET_NoEnoughDataInArray;
		getNum += pnsc_LC[1].pnlc_Text.GetLengthRE();
		
		if (ea > 1){
			if (pnsc_LC[2].pnlc_Len.Analysis(cgPosRE.offset + getNum) != PNF_RET_OK)
				return PNF_RET_NoEnoughDataInArray;
			getNum += pnsc_LC[2].pnlc_Len.GetLengthRE();
			
			len |= (pnsc_LC[2].pnlc_Len.GetValueRE() << 16);
			
			l2 = (ea == 2) ? (len >> 2) : (0xffffff >> 2);
			pnsc_LC[2].pnlc_Text.SetFixedByte(l2);
			pnsc_LC[2].pnlc_Text.ResetPosRE();
			if (pnsc_LC[2].pnlc_Text.Analysis(cgPosRE.offset + getNum) != PNF_RET_OK)
				return PNF_RET_NoEnoughDataInArray;
			getNum += pnsc_LC[2].pnlc_Text.GetLengthRE();
			
			if (ea > 2){
				if (pnsc_LC[3].pnlc_Len.Analysis(cgPosRE.offset + getNum) != PNF_RET_OK)
					return PNF_RET_NoEnoughDataInArray;
				getNum += pnsc_LC[3].pnlc_Len.GetLengthRE();
				
				len |= (pnsc_LC[3].pnlc_Len.GetValueRE() << 24);
				
				l3 = len >> 2;
				pnsc_LC[3].pnlc_Text.SetFixedByte(l3);
				pnsc_LC[3].pnlc_Text.ResetPosRE();
				if (pnsc_LC[3].pnlc_Text.Analysis(cgPosRE.offset + getNum) != PNF_RET_OK)
					return PNF_RET_NoEnoughDataInArray;
				getNum += pnsc_LC[3].pnlc_Text.GetLengthRE();
			}
		}
	}
	cgPosRE.length = getNum;
	return PNF_RET_OK;
};
//------------------------------------------------------------------------------------------//
IOSE PNF_SC::DoTransform(IOS* _ios,const UVOut& _out,const uint8* data,const uint64& length){
	IOSE	rcode = IOS_OK;
	uint32	ret;
	uint64  num = length;
	
	switch (cgEA) {
		case 0:;
			IOS_update(&rcode, pnsc_LC[0].Write(_ios, &ret, data, (uint32)num));
			num -= ret;
			data += ret;
			if (pnsc_LC[0].IsFull())
				cgEA ++;
		case 1:;
			if (num == 0)
				break;
			IOS_update(&rcode, pnsc_LC[1].Write(_ios, &ret, data, (uint32)num));
			num -= ret;
			data += ret;
			if (pnsc_LC[1].IsFull())
				cgEA ++;
		case 2:;
			if (num == 0)
				break;
			IOS_update(&rcode, pnsc_LC[2].Write(_ios, &ret, data, (uint32)num));
			num -= ret;
			data += ret;
			if (pnsc_LC[2].IsFull())
				cgEA ++;
		case 3:;
			if (num == 0)
				break;
			IOS_update(&rcode, pnsc_LC[3].Write(_ios, &ret, data, (uint32)num));
			num -= ret;
			if (pnsc_LC[3].IsFull())
				cgEA ++;
			if (num == 0)
				break;
		case 4:;
		default:;
			break;
	}
	POS_update(&cgPosWR);
	return(IOS_update(_ios, rcode));
};
//------------------------------------------------------------------------------------------//
IOSE PNF_SC::DoFinal(IOS* _ios,const UVOut& _out){
	IOSE	rcode = IOS_OK;
	uint32 len;
DOAGAIN:
	switch (cgEA) {
		case 0:;
			len = pnsc_LC[0].GetContentLengthIn();
			IOS_update(&rcode, pnsc_LC[0]._Endl(_ios));
			len <<= 2;
			len |= cgEA;
			IOS_update(&rcode, pnsc_LC[0].pnlc_Len.Update(len & 0xff));
			break;
		case 1:;
			len = pnsc_LC[1].GetContentLengthIn();
			IOS_update(&rcode, pnsc_LC[1]._Endl(_ios));
			if (len == 0){
				cgEA --;
				goto DOAGAIN;
			}
			len <<= 2;
			len |= cgEA;
			IOS_update(&rcode, pnsc_LC[0].pnlc_Len.Update(len & 0xff));
			IOS_update(&rcode, pnsc_LC[1].pnlc_Len.Update((len >> 8) & 0xff));
			break;
		case 2:;
			len = pnsc_LC[2].GetContentLengthIn();
			IOS_update(&rcode, pnsc_LC[2]._Endl(_ios));
			if (len == 0){
				cgEA --;
				goto DOAGAIN;
			}
			len <<= 2;
			len |= cgEA;
			IOS_update(&rcode, pnsc_LC[0].pnlc_Len.Update(len & 0xff));
			IOS_update(&rcode, pnsc_LC[1].pnlc_Len.Update((len >> 8) & 0xff));
			IOS_update(&rcode, pnsc_LC[2].pnlc_Len.Update((len >> 16) & 0xff));
			break;
		case 3:;
			len = pnsc_LC[3].GetContentLengthIn();
			IOS_update(&rcode, pnsc_LC[3]._Endl(_ios));
			
			if (len == 0){
				cgEA --;
				goto DOAGAIN;
			}
			len <<= 2;
			len |= cgEA;
			IOS_update(&rcode, pnsc_LC[0].pnlc_Len.Update(len & 0xff));
			IOS_update(&rcode, pnsc_LC[1].pnlc_Len.Update((len >> 8) & 0xff));
			IOS_update(&rcode, pnsc_LC[2].pnlc_Len.Update((len >> 16) & 0xff));
			IOS_update(&rcode, pnsc_LC[3].pnlc_Len.Update((len >> 24) & 0xff));
			break;
		case 4:;
			cgEA --;
			goto DOAGAIN;
		default:;
			break;
	}
	pnsc_LC[0]._Endl(nullptr);
	pnsc_LC[1]._Endl(nullptr);
	pnsc_LC[2]._Endl(nullptr);
	pnsc_LC[3]._Endl(nullptr);
	return(IOS_update(_ios, IOS_update(&rcode,IOS_FINISH)));
};
//------------------------------------------------------------------------------------------//









//------------------------------------------------------------------------------------------//
PNFB_SHELL::PNFB_SHELL(void) : PNF_BLOCK(){
	AppendDown(pns_Head) < pns_Block < pns_Checksum < pns_Tail;
	SetSFlag(PNF_blEnCRC);
	cgPNF = &pns_Block;
};
//------------------------------------------------------------------------------------------//
void PNFB_SHELL::InitPN(const ARRAY* _out,const ARRAY* _in,uint32 byteCRC,G_ENDIAN endian){
	if (_out == nullptr)
		_out = &cgArray;
	if (_in == nullptr)
		_in = &cgArray;

	PNF_BLOCK::InitPN	(_out,_in,&pns_Block);
	pns_Head.InitPN		(_out,_in,"\xE7");
	pns_Block.InitPN	(_out,_in);
	pns_Checksum.InitPN	(_out,_in,byteCRC,endian);
	pns_Tail.InitPN		(_out,_in,"\xE8");
	if (byteCRC == 0)
		DisableCRC();
};
//------------------------------------------------------------------------------------------//
IOSE PNFB_SHELL::_Begin(IOS* _ios){
	IOSE rcode = IOS_OK;
	
	IOS_update(&rcode, PNF_BLOCK::_Begin	(_ios));
	IOS_update(&rcode, pns_Head.Write		(_ios));
	IOS_update(&rcode, pns_Block._Begin	(_ios));
	
	return(rcode);
};
//------------------------------------------------------------------------------------------//
IOSE PNFB_SHELL::_Endl(IOS* _ios){
	IOSE rcode = IOS_OK;
	
	IOS_update(&rcode,pns_Block._Endl(_ios));
	if (CheckSFlag(PNF_blEnCRC))
		SetChecksum();
	IOS_update(&rcode,pns_Tail.Write(_ios));
	IOS_update(&rcode,PNF_BLOCK::_Endl(_ios));
	return(rcode);
};
//------------------------------------------------------------------------------------------//
IOSE PNFB_SHELL::Read(IOS* _ios,const UVOut& _out){
	if (ChecksumResult())
		return(PNF_BLOCK::Read(_ios,_out));
	return(IOS_update(_ios,IOS_ERR));
};
//------------------------------------------------------------------------------------------//
bool32 PNFB_SHELL::AnalysisR1(void){
	bool32	err = PNF_RET_ERR;
	uint32	offset;
	
	POS_reset(&cgPosRE);
	offset = 0;
	while(offset < cgPosRE.array->Used()){
		err = Analysis(offset);
		if (err == PNF_RET_OK)
			break;
		if (err == PNF_RET_AbandonFirstData){
			if (offset == 0)
				break;
		}
		err = PNF_RET_ERR;
		++ offset;//abandon first byte data.
	}
	return(err);
};
//------------------------------------------------------------------------------------------//
bool32 PNFB_SHELL::TryGetFrame(void){

	do{
		POS_reset(&cgPosRE);
		switch(AnalysisR1()){
			case PNF_RET_OK:
				return PNF_RET_OK;
			case PNF_RET_AbandonFirstData:
				cgPosRE.array->Out(1);
				break;
			case PNF_RET_NoEnoughDataInArray:
				if (cgPosRE.array->IsFull() != G_FALSE){
					cgPosRE.array->Out(1);
					break;
				}
				return PNF_RET_NoEnoughDataInArray;
			default:
				return G_FALSE;
		}
	}while(cgPosRE.offset < cgPosRE.array->Used());
	return G_FALSE;
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif /* ADS_Protocol_h */
