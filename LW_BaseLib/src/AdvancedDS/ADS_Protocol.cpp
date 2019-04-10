//
//  ADS_Protocol.cpp
//  SDT
//
//  Created by Leif Wen on 24/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
#include "ADS_Protocol.h"
//------------------------------------------------------------------------------------------//
#ifdef ADS_Protocol_h
//------------------------------------------------------------------------------------------//
ADS_FIFO::ADS_FIFO(void) : DSTF(){
	POS_Reset(&cgPosWR);
	POS_Reset(&cgPosRE);
};
//------------------------------------------------------------------------------------------//
ioss ADS_FIFO::_Begin(IOSTATUS* _ios){
	SetSFlag(DSTF_blStart);
	cgStartup.ios = (IOSTATUS*)_ios;
	cgStartup.uvOut = &cgPosWR.uvOut;
	cgPosWR.uvOut = cgPosWR.array;
	POS_Hold(&cgPosWR);
	return IOS_OK;
};
//------------------------------------------------------------------------------------------//
ioss ADS_FIFO::_Endl(void){
	Final(cgStartup.ios);
	if (cgPosWR.array->IsNoMEM())
		cgPosWR.array->Prepare_Giveup();
	cgPosWR.array->Prepare_Clr();
	return(cgPosWR.array->IsNoMEM() ? IOS_OK : IOS_NOMEM);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
ioss DS_IO_PNF::DoConvert(DSIO* ioNode,IOSTATUS* _ios,const UVOut& _out,const UVIn& _in){
	if (_in.uvid != DSIO_PNF_UVID)
		return IOS_NOCONVERT;
	PNF	*pnf = (PNF*)_in.uvp;
	return(ArrayConvert(ioNode,_ios,_out,pnf->GetDefArrayRE(),-1,0));
};
//------------------------------------------------------------------------------------------//
ioss DS_IO_PNF::DoSave(IOSTATUS* _ios,const UVOut& _out,const uint8* data,const uint64& length){
	if (_out.uvid != DSIO_PNF_UVID)
		return IOS_NOCONVERT;
	PNF	*pnf = (PNF*)_out.uvp;
	return(pnf->Write(_ios, IUD(data,length)));
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
enum{
	PNF_RET_OK = 1,
	PNF_RET_AbandonFirstData = 0,
	PNF_RET_NoEnoughDataInArray = -1,
	PNF_RET_NotMessage = -2,
	PNF_RET_NoFirstChild = -3,
	PNF_RET_CheckNextTail = -4,
};
//------------------------------------------------------------------------------------------//
PNF::PROTOCOL_NODE_FRAME(void) : ADS_FIFO(){
	static	DS_IO_PNF	sgDSIOPNF;
	cgFixedByte = 0;
	cgPNF = nullptr;
	GetDSIOList() < sgDSIOPNF;
};
//------------------------------------------------------------------------------------------//
bool32 PNF::Analysis(uint32 startOffset){
	
	cgPosRE.offset = startOffset;
	cgPosRE.length = cgFixedByte;
	ResetPNLength();
	if (cgPosRE.length > 0){
		if ((cgPosRE.offset + cgPosRE.length) > cgPosRE.array->Used())
			return PNF_RET_NoEnoughDataInArray;
	}
	return PNF_RET_OK;
}
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
ioss PNF_VAR::SetByte4(IOSTATUS* _ios,uint32 data){
	uint8		buf[4];
	IOSTATUS	ios;

	cgPosWR.length = 4;
	Save(IOSTATUS_Clr(&ios), cgPosWR.uvOut, CreateByte(buf,data,cgPosWR.length,CheckSFlag(PNF_blEndianBig)), cgPosWR.length);
	ios.total_in = 0;
	IOSTATUS_Add(_ios, ios);
	return(ios.status);
};
//------------------------------------------------------------------------------------------//
ioss PNF_VAR::SetByte3(IOSTATUS* _ios,uint32 data){
	uint8		buf[4];
	IOSTATUS	ios;
	
	cgPosWR.length = 3;
	Save(IOSTATUS_Clr(&ios), cgPosWR.uvOut, CreateByte(buf,data,cgPosWR.length,CheckSFlag(PNF_blEndianBig)), cgPosWR.length);
	ios.total_in = 0;
	IOSTATUS_Add(_ios, ios);
	return(ios.status);
};
//------------------------------------------------------------------------------------------//
ioss PNF_VAR::SetByte2(IOSTATUS* _ios,uint32 data){
	uint8		buf[4];
	IOSTATUS	ios;
	
	cgPosWR.length = 2;
	Save(IOSTATUS_Clr(&ios), cgPosWR.uvOut, CreateByte(buf,data,cgPosWR.length,CheckSFlag(PNF_blEndianBig)), cgPosWR.length);
	ios.total_in = 0;
	IOSTATUS_Add(_ios, ios);
	return(ios.status);
};
//------------------------------------------------------------------------------------------//
ioss PNF_VAR::SetByte1(IOSTATUS* _ios,uint32 data){
	uint8		buf[4];
	IOSTATUS	ios;
	
	cgPosWR.length = 1;
	Save(IOSTATUS_Clr(&ios), cgPosWR.uvOut, CreateByte(buf,data,cgPosWR.length,CheckSFlag(PNF_blEndianBig)), cgPosWR.length);
	ios.total_in = 0;
	IOSTATUS_Add(_ios, ios);
	return(ios.status);
};
//------------------------------------------------------------------------------------------//
ioss PNF_VAR::SetByte(IOSTATUS* _ios,uint32 data){
	ioss iossta;
	switch(cgFixedByte){
		case 1:;
			iossta = SetByte1(_ios,data);
			break;
		case 2:;
			iossta = SetByte2(_ios,data);
			break;
		case 3:;
			iossta = SetByte3(_ios,data);
			break;
		case 4:;
		default:
			iossta = SetByte4(_ios,data);break;
	}
	return(iossta);
};
//------------------------------------------------------------------------------------------//
ioss PNF_VAR::UpdateByte4(uint32 data){
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
	return(cgPosWR.array->UpdateByOffsetIn(buf, 4, cgPosWR.offset) == 4);
};
//------------------------------------------------------------------------------------------//
ioss PNF_VAR::UpdateByte3(uint32 data){
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
	return(cgPosWR.array->UpdateByOffsetIn(buf, 3, cgPosWR.offset) == 3);
};
//------------------------------------------------------------------------------------------//
ioss PNF_VAR::UpdateByte2(uint32 data){
	uint8	buf[2];
	if (CheckSFlag(PNF_blEndianBig)){
		buf[0] = (uint8)(data >> 8);
		buf[1] = (uint8)(data);
	}
	else{
		buf[0] = (uint8)(data);
		buf[1] = (uint8)(data >> 8);
	}
	return(cgPosWR.array->UpdateByOffsetIn(buf, 2, cgPosWR.offset) == 2);
};
//------------------------------------------------------------------------------------------//
ioss PNF_VAR::UpdateByte1(uint32 data){
	uint8 	tData;
	tData = (uint8)(data);
	return(cgPosWR.array->UpdateByOffsetIn(&tData, 1, cgPosWR.offset) == 1);
};
//------------------------------------------------------------------------------------------//
ioss PNF_VAR::UpdateByte(uint32 data){
	ioss	iossta;
	switch(cgFixedByte){
		case 1:
			iossta = UpdateByte1(data);break;
		case 2:
			iossta = UpdateByte2(data);break;
		case 3:
			iossta = UpdateByte3(data);break;
		case 4:
		default:
			iossta = UpdateByte4(data);break;
	}
	return(iossta);
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
}
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
}
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
}
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
void PNF_MASK::FillMaskFieldRE(void){
	TREE_LChildRChain_Traversal_LINE_nolock(PNF_VAR,this,
		_opNode->SyncPosRE(&cgPosRE);
		_opNode->SetFixedByte(cgFixedByte);
		_opNode->SetEndian(CheckSFlag(PNF_blEndianBig) ? G_ENDIAN_BIG : G_ENDIAN_LITTLE);
	);
}
//------------------------------------------------------------------------------------------//
void PNF_MASK::FillMaskFieldWR(void){
	TREE_LChildRChain_Traversal_LINE_nolock(PNF_VAR,this,
		_opNode->SyncPosWR(&cgPosWR);
		_opNode->SetFixedByte(cgFixedByte);
		_opNode->SetEndian(CheckSFlag(PNF_blEndianBig) ? G_ENDIAN_BIG : G_ENDIAN_LITTLE);
	);
}
//------------------------------------------------------------------------------------------//
ioss PNF_MASK::Write(IOSTATUS* _ios,uint32 data){
	ioss iossta;
	_Begin(_ios);
	SetByte(_ios,data);
	iossta = _Endl();
	FillMaskFieldWR();
	return(iossta);
};
//------------------------------------------------------------------------------------------//
bool32 PNF_MASK::Analysis(uint32 startOffset){
	bool32 err;
	err = PNF_VAR::Analysis(startOffset);
	if (err == PNF_RET_OK)
		FillMaskFieldRE();
	return(err);
}
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
PNF_FIXED::PNF_FIXED(void) : PNF(){
	cgFixedStr = "";
};
//------------------------------------------------------------------------------------------//
bool32 PNF_FIXED::CheckFixedStr(void)const{
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
}
//------------------------------------------------------------------------------------------//
bool32 PNF_FIXED::Analysis(uint32 startOffset){
	bool32 err;
	err = PNF::Analysis(startOffset);
	if (err == PNF_RET_OK)
		return(CheckFixedStr());
	return(err);
}
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
bool32 PNF_TAIL::Analysis(uint32 startOffset){
	bool32 err;
	err = PNF_FIXED::Analysis(startOffset);
	if (err == PNF_RET_OK){
		if (cgPNF != nullptr) //the PN_CONTENT length decide by PN_TAIL,set PN_CONTENT->length
			cgPNF->SetLengthRE(cgPosRE.offset - cgPNF->GetOffsetRE());
	}
	else if (err == PNF_RET_AbandonFirstData){//tail field no match
		if (cgPNF != nullptr) //the PN_CONTENT length decide by PN_TAIL
			return PNF_RET_CheckNextTail;
	}
	return(err);
}
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
void PNF_CONTENT::ResetPNLength(void){
	if (cgPNF != nullptr){//PN's length decide by PN_LENGTH
		cgPosRE.length = ((PNF_VAR*)cgPNF)->GetValueCalcRE();
	}
	else{
		cgPosRE.length = cgFixedByte;
	}
}
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
	Add(pnlc_Len) < pnlc_Text;
};
//------------------------------------------------------------------------------------------//
ioss PNFB_LC::_Begin(IOSTATUS* _ios){
	PNF_BLOCK::_Begin	(_ios);
	pnlc_Len.Write		(_ios,-1);
	return(pnlc_Text._Begin(_ios));
};
//------------------------------------------------------------------------------------------//
ioss PNFB_LC::_Endl(void){
	pnlc_Text._Endl();
	pnlc_Len.Update(pnlc_Text.GetLengthWR());
	return(PNF_BLOCK::_Endl());
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
PNF_SCC::PNF_SCC(void) : PNFB_LC(){
	cgMaxSize = 0;
};
//------------------------------------------------------------------------------------------//
void PNF_SCC::ResetPosWR(void){
	POS_Reset(&cgPosWR);
	pnlc_Len.ResetPosWR();
	pnlc_Text.ResetPosWR();
}
//------------------------------------------------------------------------------------------//
void PNF_SCC::ResetPosRE(void){
	POS_Reset(&cgPosRE);
	pnlc_Len.ResetPosRE();
	pnlc_Text.ResetPosRE();
}
//------------------------------------------------------------------------------------------//
uint32 PNF_SCC::Write(IOSTATUS* _ios,const uint8 *data,uint32 num){
	uint32 size;
	
	_Begin(_ios);
	size = cgMaxSize - pnlc_Text.GetLengthWR();
	if (num > size)
		num = size;
	pnlc_Text.Transform(_ios, data, num);
	POS_Update(&cgPosWR);
	if (IsFull())
		_Endl();
	return(num);
}
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
}
//------------------------------------------------------------------------------------------//
void PNF_SC::ResetPosWR(void){
	POS_Reset(&cgPosWR);
	pnsc_LC[0].ResetPosWR();
	pnsc_LC[1].ResetPosWR();
	pnsc_LC[2].ResetPosWR();
	pnsc_LC[3].ResetPosWR();
}
//------------------------------------------------------------------------------------------//
void PNF_SC::ResetPosRE(void){
	POS_Reset(&cgPosRE);
	pnsc_LC[0].ResetPosRE();
	pnsc_LC[1].ResetPosRE();
	pnsc_LC[2].ResetPosRE();
	pnsc_LC[3].ResetPosRE();
}
//------------------------------------------------------------------------------------------//
int32 PNF_SC::Analysis(uint32 startOffset){
	uint32	getNum;
	uint32	len;
	uint32	cgL0,cgL1,cgL2,cgL3;
	
	ResetPosRE();
	cgPosRE.offset = startOffset;
	cgPosRE.length = 0;
	
	if (pnsc_LC[0].pnlc_Len.Analysis(cgPosRE.offset) != PNF_RET_OK)
		return PNF_RET_NoEnoughDataInArray;
	getNum = pnsc_LC[0].pnlc_Len.GetLengthRE();
	
	len = pnsc_LC[0].pnlc_Len.GetValueRE();
	cgEA = len & 0x03;
	
	cgL0 = (cgEA == 0) ? (len >> 2) : (0xff >> 2);
	pnsc_LC[0].pnlc_Text.SetFixedByte(cgL0);
	pnsc_LC[0].pnlc_Text.ResetPosRE();
	if (pnsc_LC[0].pnlc_Text.Analysis(cgPosRE.offset + getNum) != PNF_RET_OK)
		return PNF_RET_NoEnoughDataInArray;
	getNum += pnsc_LC[0].pnlc_Text.GetLengthRE();
	
	if (cgEA > 0){
		if (pnsc_LC[1].pnlc_Len.Analysis(cgPosRE.offset + getNum) != PNF_RET_OK)
			return PNF_RET_NoEnoughDataInArray;
		getNum += pnsc_LC[1].pnlc_Len.GetLengthRE();
		
		len |= (pnsc_LC[1].pnlc_Len.GetValueRE() << 8);
		
		cgL1 = (cgEA == 1) ? (len >> 2) : (0xffff >> 2);
		pnsc_LC[1].pnlc_Text.SetFixedByte(cgL1);
		pnsc_LC[1].pnlc_Text.ResetPosRE();
		if (pnsc_LC[1].pnlc_Text.Analysis(cgPosRE.offset + getNum) != PNF_RET_OK)
			return PNF_RET_NoEnoughDataInArray;
		getNum += pnsc_LC[1].pnlc_Text.GetLengthRE();
		
		if (cgEA > 1){
			if (pnsc_LC[2].pnlc_Len.Analysis(cgPosRE.offset + getNum) != PNF_RET_OK)
				return PNF_RET_NoEnoughDataInArray;
			getNum += pnsc_LC[2].pnlc_Len.GetLengthRE();
			
			len |= (pnsc_LC[2].pnlc_Len.GetValueRE() << 16);
			
			cgL2 = (cgEA == 2) ? (len >> 2) : (0xffffff >> 2);
			pnsc_LC[2].pnlc_Text.SetFixedByte(cgL2);
			pnsc_LC[2].pnlc_Text.ResetPosRE();
			if (pnsc_LC[2].pnlc_Text.Analysis(cgPosRE.offset + getNum) != PNF_RET_OK)
				return PNF_RET_NoEnoughDataInArray;
			getNum += pnsc_LC[2].pnlc_Text.GetLengthRE();
			
			if (cgEA > 2){
				if (pnsc_LC[3].pnlc_Len.Analysis(cgPosRE.offset + getNum) != PNF_RET_OK)
					return PNF_RET_NoEnoughDataInArray;
				getNum += pnsc_LC[3].pnlc_Len.GetLengthRE();
				
				len |= (pnsc_LC[3].pnlc_Len.GetValueRE() << 24);
				
				cgL3 = len >> 2;
				pnsc_LC[3].pnlc_Text.SetFixedByte(cgL3);
				pnsc_LC[3].pnlc_Text.ResetPosRE();
				if (pnsc_LC[3].pnlc_Text.Analysis(cgPosRE.offset + getNum) != PNF_RET_OK)
					return PNF_RET_NoEnoughDataInArray;
				getNum += pnsc_LC[3].pnlc_Text.GetLengthRE();
			}
		}
	}
	cgPosRE.length = getNum;
	return PNF_RET_OK;
}
//------------------------------------------------------------------------------------------//
ioss PNF_SC::DoTransform(IOSTATUS* _ios,const UVOut& _out,const uint8* data,const uint64& length){
	uint32	ret;
	uint64  num = length;
	
	switch (cgEA) {
		case 0:;
			ret = pnsc_LC[0].Write(_ios, data, (uint32)num);
			num -= ret;
			data += ret;
			if (pnsc_LC[0].IsFull())
				cgEA ++;
			if (num == 0)
				break;
		case 1:;
			ret = pnsc_LC[1].Write(_ios, data, (uint32)num);
			num -= ret;
			data += ret;
			if (pnsc_LC[1].IsFull())
				cgEA ++;
			if (num == 0)
				break;
		case 2:;
			ret = pnsc_LC[2].Write(_ios, data, (uint32)num);
			num -= ret;
			data += ret;
			if (pnsc_LC[2].IsFull())
				cgEA ++;
			if (num == 0)
				break;
		case 3:;
			ret = pnsc_LC[3].Write(_ios, data, (uint32)num);
			num -= ret;
			if (pnsc_LC[3].IsFull())
				cgEA ++;
			if (num == 0)
				break;
		case 4:;
		default:;
			break;
	}
	POS_Update(&cgPosWR);
	return IOS_OK;
}
//------------------------------------------------------------------------------------------//
ioss PNF_SC::DoFinal(IOSTATUS* _ios,const UVOut& _out){
	uint32 len;
DOAGAIN:
	switch (cgEA) {
		case 0:;
			len = pnsc_LC[0].GetContentLengthIn();
			pnsc_LC[0]._Endl();
			len <<= 2;
			len |= cgEA;
			pnsc_LC[0].pnlc_Len.Update(len & 0xff);
			break;
		case 1:;
			len = pnsc_LC[1].GetContentLengthIn();
			pnsc_LC[1]._Endl();
			if (len == 0){
				cgEA --;
				goto DOAGAIN;
			}
			len <<= 2;
			len |= cgEA;
			pnsc_LC[0].pnlc_Len.Update(len & 0xff);
			pnsc_LC[1].pnlc_Len.Update((len >> 8) & 0xff);
			break;
		case 2:;
			len = pnsc_LC[2].GetContentLengthIn();
			pnsc_LC[2]._Endl();
			if (len == 0){
				cgEA --;
				goto DOAGAIN;
			}
			len <<= 2;
			len |= cgEA;
			pnsc_LC[0].pnlc_Len.Update(len & 0xff);
			pnsc_LC[1].pnlc_Len.Update((len >> 8) & 0xff);
			pnsc_LC[2].pnlc_Len.Update((len >> 16) & 0xff);
			break;
		case 3:;
			len = pnsc_LC[3].GetContentLengthIn();
			pnsc_LC[3]._Endl();
			
			if (len == 0){
				cgEA --;
				goto DOAGAIN;
			}
			len <<= 2;
			len |= cgEA;
			pnsc_LC[0].pnlc_Len.Update(len & 0xff);
			pnsc_LC[1].pnlc_Len.Update((len >> 8) & 0xff);
			pnsc_LC[2].pnlc_Len.Update((len >> 16) & 0xff);
			pnsc_LC[3].pnlc_Len.Update((len >> 24) & 0xff);
			break;
		case 4:;
			cgEA --;
			goto DOAGAIN;
		default:;
			break;
	}
	return IOS_OK;
}
//------------------------------------------------------------------------------------------//









//------------------------------------------------------------------------------------------//
PNFB_SHELL::PNFB_SHELL(void) : PNF_BLOCK(){
	Add(pns_Head) < pns_Block < pns_Checksum < pns_Tail;
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
}
//------------------------------------------------------------------------------------------//
ioss PNFB_SHELL::_Begin(IOSTATUS* _ios){
	PNF_BLOCK::_Begin	(_ios);
	pns_Head.Write		(_ios);
	return(pns_Block._Begin(_ios));
};
//------------------------------------------------------------------------------------------//
ioss PNFB_SHELL::_Endl(void){
	pns_Block._Endl();
	if (CheckSFlag(PNF_blEnCRC))
		SetChecksum();
	pns_Tail.Write(cgStartup.ios);
	return(PNF_BLOCK::_Endl());
};
//------------------------------------------------------------------------------------------//
ioss PNFB_SHELL::Read(IOSTATUS* _ios,const UVOut& _out){
	if (ChecksumResult())
		return(PNF_BLOCK::Read(_ios,_out));
	return IOS_ERR;
};
//------------------------------------------------------------------------------------------//
bool32 PNFB_SHELL::AnalysisR1(uint32 startOffset){
	int32 ret;
	
	cgPosRE.offset = startOffset;
	
	while(cgPosRE.offset < cgPosRE.array->Used()){
		ret = Analysis(cgPosRE.offset);
		if (ret != PNF_RET_AbandonFirstData)
			return(ret);
		if (startOffset > 0)//only analysis once,
			return PNF_RET_AbandonFirstData;
		++ cgPosRE.offset;//abandon first byte data.
	}
	return PNF_RET_NoEnoughDataInArray;
}
//------------------------------------------------------------------------------------------//
bool32 PNFB_SHELL::AnalysisFrame(uint32 startOffset){
	bool32 retTry;

	retTry = AnalysisR1(startOffset);
	if (retTry == PNF_RET_OK)
		return PNF_RET_OK;
	POS_Reset(&cgPosRE);
	return(retTry);
}
//------------------------------------------------------------------------------------------//
bool32 PNFB_SHELL::TryGetFrame(void){
	
	do{
		POS_Reset(&cgPosRE);
		switch(Analysis(0)){
			case PNF_RET_OK:
				return PNF_RET_OK;
			case PNF_RET_AbandonFirstData:
				cgPosRE.array->Out(1);
				break;
			case PNF_RET_NoEnoughDataInArray:
				if (cgPosRE.array->IsFull() == G_FALSE){
					cgPosRE.array->Out(1);
					break;
				}
				return PNF_RET_NoEnoughDataInArray;
			default:
				return G_FALSE;
		}
	}while(cgPosRE.offset < cgPosRE.array->Used());
	return G_FALSE;
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif /* ADS_Protocol_h */
