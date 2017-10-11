/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: PA_B.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.08.12
 
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "Comm_PA.h"
#ifdef Comm_PAH
//------------------------------------------------------------------------------------------//
PROTOCOL_NODE::PROTOCOL_NODE(void) : TREE_NODE() {
	cgType = FNT_UNINIT;
	cgEndianType = G_ENDIAN_LITTLE;
	cgObyteNum = 0;
	
	cgDefFifo = nullptr;
	
	maskBit = 0xffffffff;
	
	fnOffset = 0;
	fnlength = 0;
	
	vaildAreaH = nullptr;
	vaildAreaT = nullptr;
	linkNode = nullptr;
	fixValue = "";
	moveBit = 0;
};
//------------------------------------------------------------------------------------------//
void PROTOCOL_NODE::Init(FNT_Type fType,const FIFO8 *defFifo,uint32 fObyteNum,G_ENDIAN fEndianType){
	cgType = fType;
	cgObyteNum = fObyteNum;
	cgEndianType = fEndianType;
	cgDefFifo = (FIFO8*)defFifo;
}
//------------------------------------------------------------------------------------------//
void PROTOCOL_NODE::SetFixValue(const STDSTR &str){
	fixValue = str;
	cgObyteNum = (uint32)fixValue.length();
}
//------------------------------------------------------------------------------------------//
int32 PROTOCOL_NODE::CheckFixValue(const FIFO8 &fifo)const{
	uint8	charP1,charP2;
	uint32	tOffset,fLength;
	
	tOffset = fnOffset + fnlength;
	fLength = fnlength;
	charP1 = 0;
	while(fLength-- > 0){
		fifo.Read(&charP1, 1, --tOffset);
		charP2 = fixValue[fLength];
		if (charP1 != charP2)
			return 0;
	}
	return 1;
}
//------------------------------------------------------------------------------------------//
void PROTOCOL_NODE::SetMaskBit(uint32 mask){
	uint32	tMask,i;
	
	maskBit = mask;
	i = 0;
	tMask = maskBit;
	moveBit = 0;
	do{
		if ((tMask & 0x00000001) > 0)
			break;
		tMask >>= 1;
		++ moveBit;
	}while(++ i < 32);
}
//------------------------------------------------------------------------------------------//
uint32 PROTOCOL_NODE::GetOriginalValue(const FIFO8 *fifo)const{
	uint32 ret,i;
	uint8 tData;
	
	if (fifo == nullptr)
		fifo = cgDefFifo;

	tData = 0;
	ret = 0;
	if (cgEndianType == G_ENDIAN_LITTLE){
		i = fnlength;
		while(i-- > 0){
			ret <<= 8;
			fifo->Read(&tData, 1, fnOffset + i);
			ret |= tData;
		};
	}
	else{
		i = 0;
		do{
			ret <<= 8;
			fifo->Read(&tData, 1, fnOffset + i);
			ret |= tData;
		}while(++i < fnlength);
	}
	return(ret);
}
//------------------------------------------------------------------------------------------//
uint32 PROTOCOL_NODE::GetValueAMask(const FIFO8 *fifo)const{
	if (fifo == nullptr)
		fifo = cgDefFifo;
	return(GetOriginalValue(fifo) & maskBit);
};
//------------------------------------------------------------------------------------------//
uint32 PROTOCOL_NODE::GetValueCalc(const FIFO8 *fifo)const{
	if (fifo == nullptr)
		fifo = cgDefFifo;
	return((GetOriginalValue(fifo) & maskBit) >> moveBit);
};
//------------------------------------------------------------------------------------------//
const STDSTR &PROTOCOL_NODE::ReadAllContentInHEXs(STDSTR *retStr,const FIFO8 *fifo)const{
	if (fifo == nullptr)
		fifo = cgDefFifo;
	*retStr = "";
	fifo->ReadInHEX_S(retStr,fnlength,fnOffset);
	return(*retStr);
};
//------------------------------------------------------------------------------------------//
const STDSTR &PROTOCOL_NODE::ReadAllContent(STDSTR *retStr,const FIFO8 *fifo)const{
	if (fifo == nullptr)
		fifo = cgDefFifo;
	*retStr = "";
	fifo->Read(retStr,fnlength,fnOffset);
	return(*retStr);
};
//------------------------------------------------------------------------------------------//
const FIFO8 &PROTOCOL_NODE::ReadAllContent(FIFO8 *retFifo,const FIFO8 *fifo)const{
	if (fifo == nullptr)
		fifo = cgDefFifo;

	if (retFifo != fifo)
		retFifo->Put(*fifo,fnlength,fnOffset);
	return(*retFifo);
};
//------------------------------------------------------------------------------------------//
const PROTOCOL_NODE &PROTOCOL_NODE::ReadAllContent(PROTOCOL_NODE *retPN,const FIFO8 *fifo)const{
	if (fifo == nullptr)
		fifo = cgDefFifo;
	retPN->Spin_InUse_set();
	retPN->cgDefFifo->Put(*fifo,fnlength,fnOffset);
	retPN->Spin_InUse_clr();
	return(*retPN);
};
//------------------------------------------------------------------------------------------//
void PROTOCOL_NODE::SetFIFO(void){
	fnOffset = cgDefFifo->GetPreInNum();
	fnlength = 0;
	if (cgType == FNT_MASK)
		FillMaskField();
};
//------------------------------------------------------------------------------------------//
void PROTOCOL_NODE::SetFIFO_4Byte(uint32 tInput){
	uint8 	tData;
	fnOffset = cgDefFifo->GetPreInNum();
	fnlength = 4;
	if (cgEndianType == G_ENDIAN_LITTLE){
		tData = (uint8)(tInput);
		cgDefFifo->Put(&tData, 1);
		tData = (uint8)(tInput >> 8);
		cgDefFifo->Put(&tData, 1);
		tData = (uint8)(tInput >> 16);
		cgDefFifo->Put(&tData, 1);
		tData = (uint8)(tInput >> 24);
		cgDefFifo->Put(&tData, 1);
	}
	else{
		tData = (uint8)(tInput >> 24);
		cgDefFifo->Put(&tData, 1);
		tData = (uint8)(tInput >> 16);
		cgDefFifo->Put(&tData, 1);
		tData = (uint8)(tInput >> 8);
		cgDefFifo->Put(&tData, 1);
		tData = (uint8)(tInput);
		cgDefFifo->Put(&tData, 1);
	}
	if (cgType == FNT_MASK)
		FillMaskField();
};
//------------------------------------------------------------------------------------------//
void PROTOCOL_NODE::SetFIFO_3Byte(uint32 tInput){
	uint8 	tData;
	fnOffset = cgDefFifo->GetPreInNum();
	fnlength = 3;
	if (cgEndianType == G_ENDIAN_LITTLE){
		tData = (uint8)(tInput);
		cgDefFifo->Put(&tData, 1);
		tData = (uint8)(tInput >> 8);
		cgDefFifo->Put(&tData, 1);
		tData = (uint8)(tInput >> 16);
		cgDefFifo->Put(&tData, 1);
	}
	else{
		tData = (uint8)(tInput >> 16);
		cgDefFifo->Put(&tData, 1);
		tData = (uint8)(tInput >> 8);
		cgDefFifo->Put(&tData, 1);
		tData = (uint8)(tInput);
		cgDefFifo->Put(&tData, 1);
	}
	if (cgType == FNT_MASK)
		FillMaskField();
};
//------------------------------------------------------------------------------------------//
void PROTOCOL_NODE::SetFIFO_2Byte(uint32 tInput){
	uint8 	tData;
	fnOffset = cgDefFifo->GetPreInNum();
	fnlength = 2;
	if (cgEndianType == G_ENDIAN_LITTLE){
		tData = (uint8)(tInput);
		cgDefFifo->Put(&tData, 1);
		tData = (uint8)(tInput >> 8);
		cgDefFifo->Put(&tData, 1);
	}
	else{
		tData = (uint8)(tInput >> 8);
		cgDefFifo->Put(&tData, 1);
		tData = (uint8)(tInput);
		cgDefFifo->Put(&tData, 1);
	}
	if (cgType == FNT_MASK)
		FillMaskField();
};
//------------------------------------------------------------------------------------------//
void PROTOCOL_NODE::SetFIFO_1Byte(uint32 tInput){
	uint8 	tData;
	fnOffset = cgDefFifo->GetPreInNum();
	fnlength = 1;
	tData = (uint8)(tInput);
	cgDefFifo->Put(&tData, 1);
	if (cgType == FNT_MASK)
		FillMaskField();
};
//------------------------------------------------------------------------------------------//
void PROTOCOL_NODE::SetFIFOByte(uint64 tInput){
	switch(cgObyteNum){
		case 1:
			SetFIFO_1Byte((uint32)tInput);break;
		case 2:
			SetFIFO_2Byte((uint32)tInput);break;
		case 3:
			SetFIFO_3Byte((uint32)tInput);break;
		case 4:
		default:
			SetFIFO_4Byte((uint32)tInput);break;
	}
};
//------------------------------------------------------------------------------------------//
void PROTOCOL_NODE::UpdateFIFO_4Byte(uint32 tInput){
	uint8 	tData;
	if (cgEndianType == G_ENDIAN_LITTLE){
		tData = (uint8)(tInput);
		cgDefFifo->UpdateOffsetByIn(&tData, 1,fnOffset);
		tData = (uint8)(tInput >> 8);
		cgDefFifo->UpdateOffsetByIn(&tData, 1,fnOffset + 1);
		tData = (uint8)(tInput >> 16);
		cgDefFifo->UpdateOffsetByIn(&tData, 1,fnOffset + 2);
		tData = (uint8)(tInput >> 24);
		cgDefFifo->UpdateOffsetByIn(&tData, 1,fnOffset + 3);
	}
	else{
		tData = (uint8)(tInput >> 24);
		cgDefFifo->UpdateOffsetByIn(&tData, 1,fnOffset);
		tData = (uint8)(tInput >> 16);
		cgDefFifo->UpdateOffsetByIn(&tData, 1,fnOffset + 1);
		tData = (uint8)(tInput >> 8);
		cgDefFifo->UpdateOffsetByIn(&tData, 1,fnOffset + 2);
		tData = (uint8)(tInput);
		cgDefFifo->UpdateOffsetByIn(&tData, 1,fnOffset + 3);
	}
	if (cgType == FNT_MASK)
		FillMaskField();
};
//------------------------------------------------------------------------------------------//
void PROTOCOL_NODE::UpdateFIFO_3Byte(uint32 tInput){
	uint8 	tData;
	if (cgEndianType == G_ENDIAN_LITTLE){
		tData = (uint8)(tInput);
		cgDefFifo->UpdateOffsetByIn(&tData, 1,fnOffset);
		tData = (uint8)(tInput >> 8);
		cgDefFifo->UpdateOffsetByIn(&tData, 1,fnOffset + 1);
		tData = (uint8)(tInput >> 16);
		cgDefFifo->UpdateOffsetByIn(&tData, 1,fnOffset + 2);
	}
	else{
		tData = (uint8)(tInput >> 16);
		cgDefFifo->UpdateOffsetByIn(&tData, 1,fnOffset);
		tData = (uint8)(tInput >> 8);
		cgDefFifo->UpdateOffsetByIn(&tData, 1,fnOffset + 1);
		tData = (uint8)(tInput);
		cgDefFifo->UpdateOffsetByIn(&tData, 1,fnOffset + 2);
	}
	if (cgType == FNT_MASK)
		FillMaskField();
};
//------------------------------------------------------------------------------------------//
void PROTOCOL_NODE::UpdateFIFO_2Byte(uint32 tInput){
	uint8 	tData;
	if (cgEndianType == G_ENDIAN_LITTLE){
		tData = (uint8)(tInput);
		cgDefFifo->UpdateOffsetByIn(&tData, 1,fnOffset);
		tData = (uint8)(tInput >> 8);
		cgDefFifo->UpdateOffsetByIn(&tData, 1,fnOffset + 1);
	}
	else{
		tData = (uint8)(tInput >> 8);
		cgDefFifo->UpdateOffsetByIn(&tData, 1,fnOffset);
		tData = (uint8)(tInput);
		cgDefFifo->UpdateOffsetByIn(&tData, 1,fnOffset + 1);
	}
	if (cgType == FNT_MASK)
		FillMaskField();
};
//------------------------------------------------------------------------------------------//
void PROTOCOL_NODE::UpdateFIFO_1Byte(uint32 tInput){
	uint8 	tData;
	tData = (uint8)(tInput);
	cgDefFifo->UpdateOffsetByIn(&tData, 1,fnOffset);
	if (cgType == FNT_MASK)
		FillMaskField();
};
//------------------------------------------------------------------------------------------//
void PROTOCOL_NODE::UpdateFIFOByte(uint64 tInput){
	switch(cgObyteNum){
		case 1:
			UpdateFIFO_1Byte((uint32)tInput);break;
		case 2:
			UpdateFIFO_2Byte((uint32)tInput);break;
		case 3:
			UpdateFIFO_3Byte((uint32)tInput);break;
		case 4:
		default:
			UpdateFIFO_4Byte((uint32)tInput);break;
	}
};
//------------------------------------------------------------------------------------------//
void PROTOCOL_NODE::SetFIFOFixValue(void){
	fnOffset = cgDefFifo->GetPreInNum();
	fnlength = cgDefFifo->Put((uint8*)fixValue.c_str(), (uint32)fixValue.length());
	if (cgType == FNT_MASK)
		FillMaskField();
};
//------------------------------------------------------------------------------------------//
void PROTOCOL_NODE::SetFIFO(const uint8 *data,uint32 num){
	fnOffset = cgDefFifo->GetPreInNum();
	fnlength = cgDefFifo->Put(data, num);
	if (cgType == FNT_MASK)
		FillMaskField();
};
//------------------------------------------------------------------------------------------//
void PROTOCOL_NODE::SetFIFO(std::stringstream &streamIn){
	fnOffset = cgDefFifo->GetPreInNum();
	fnlength = cgDefFifo->Put(streamIn);
	if (cgType == FNT_MASK)
		FillMaskField();
};
//------------------------------------------------------------------------------------------//
void PROTOCOL_NODE::SetFIFO(const FIFO8 &fifoIn,uint32 num,uint32 offset){
	fnOffset = cgDefFifo->GetPreInNum();
	fnlength = cgDefFifo->Put(fifoIn,num,offset);
	if (cgType == FNT_MASK)
		FillMaskField();
};
//------------------------------------------------------------------------------------------//
void PROTOCOL_NODE::SetFIFO(const PROTOCOL_NODE &fnIn){
	fnOffset = cgDefFifo->GetPreInNum();
	fnlength = cgDefFifo->Put(*fnIn.GetDefFifo(),fnIn.GetLength(),fnIn.GetOffset());
	if (cgType == FNT_MASK)
		FillMaskField();
};
//------------------------------------------------------------------------------------------//
void PROTOCOL_NODE::FillMaskField(void){
	if (cgType != FNT_MASK)
		return;
	
	TREE_LChildRChain_Traversal_LINE_nolock(PROTOCOL_NODE,this,
		operateNode_t->cgObyteNum = cgObyteNum;
		operateNode_t->cgEndianType = cgEndianType;
		operateNode_t->fnOffset = fnOffset;
		operateNode_t->fnlength = fnlength;
	);
}
//------------------------------------------------------------------------------------------//
uint32 PROTOCOL_NODE::CalcCLength(const FIFO8 &fifo)const{
	PROTOCOL_NODE	*fieldNode,*tNode;
	uint32		vaildAreaLength;
	
	if (cgType != FNT_LENGTH)
		return 0;
	if (vaildAreaH == nullptr)
		return(GetValueCalc(&fifo));
	
	tNode = vaildAreaT;
	if (tNode == nullptr)
		tNode = vaildAreaH;
	
	fieldNode = vaildAreaH;
	vaildAreaLength = fieldNode->fnlength;
	while((fieldNode != nullptr) && (fieldNode != tNode)){
		vaildAreaLength += fieldNode->fnlength;
		fieldNode = (PROTOCOL_NODE*)GetcgNext(fieldNode);
	};

	return(GetValueCalc(&fifo) - vaildAreaLength);
}
//------------------------------------------------------------------------------------------//
uint32 PROTOCOL_NODE::Out(FIFO8 *fifo){
	if (fifo == nullptr)
		fifo = cgDefFifo;
	return(fifo->Out(fnlength + fnOffset));
};
//------------------------------------------------------------------------------------------//
enum{
	RET_OK = 1,
	RET_AbandonFirstData = 0,
	RET_NoEnoughDataInFIFO = -1,
	RET_NotMessage = -2,
	RET_NoFirstChild = -3,
};
//------------------------------------------------------------------------------------------//
int32 PROTOCOL_NODE::TryGetFrame(const STDSTR &strIn){
	cgDefFifo->Out(fnOffset + fnlength);
	cgDefFifo->Put(strIn, G_ESCAPE_OFF);
	return(AnalysisFrame(cgDefFifo));
}
//------------------------------------------------------------------------------------------//
int32 PROTOCOL_NODE::AnalysisFrame(const FIFO8 *fifo,uint32 fifoOffset){
	int32 retTry;
	if (fifo == nullptr)
		fifo = cgDefFifo;
	retTry = AnalysisFrameR0(*fifo,fifoOffset);
	if (retTry == RET_OK)
		return(RET_OK);
	fnOffset = 0;
	fnlength = 0;
	return(retTry);
}
//------------------------------------------------------------------------------------------//
int32 PROTOCOL_NODE::TryGetFrame(FIFO8 *fifo){
	if (fifo == nullptr)
		fifo = cgDefFifo;
	
	while(fnOffset < fifo->Used()){
		fnOffset = 0;
		fnlength = 0;
		switch(AnalysisFrameR1(*fifo)){
			case RET_OK: return RET_OK;
			case RET_AbandonFirstData:fifo->Out(1);break;
			case RET_NoEnoughDataInFIFO:
				if (fifo->IsFull()){
					fifo->Out(1);
					break;
				}
				return RET_NoEnoughDataInFIFO;
			default:return 0;
		}
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
int32 PROTOCOL_NODE::AnalysisFrameR0(const FIFO8 &fifo,uint32 fifoOffset){
	int32 retTry;
	
	fnOffset = fifoOffset;
	fnlength = 0;
	while(fnOffset < fifo.Used()){
		retTry = AnalysisFrameR1(fifo);
		if (retTry != RET_AbandonFirstData)
			return(retTry);
		if (fifoOffset > 0)//only analysis once,
			return RET_AbandonFirstData;
		++ fnOffset;//abandon first byte data.
	}
	return RET_NoEnoughDataInFIFO;
}
//------------------------------------------------------------------------------------------//
int32 PROTOCOL_NODE::AnalysisFrameR1(const FIFO8 &fifoIn){
	PROTOCOL_NODE	*fieldNode;
	uint32		getNum;
	int32		retTry;

	if (cgType != FNT_MESSAGE)
		return RET_NotMessage;
	
	fieldNode = (PROTOCOL_NODE*)GetcgDown(this); //read Head;
	if (fieldNode == nullptr)
		return RET_NoFirstChild;
	
	getNum = 0;
	do{
	GET_DATA:;
		fieldNode->fnOffset = fnOffset + getNum;
		fieldNode->fnlength = 0;//Init fnlength
		if (fieldNode->cgObyteNum > 0){
			if ((fieldNode->fnOffset + fieldNode->cgObyteNum) > fifoIn.Used())
				return RET_AbandonFirstData;
			
			fieldNode->ResetfnLength(fifoIn);
			if ((fieldNode->fnOffset + fieldNode->fnlength) > fifoIn.Used())
				return RET_NoEnoughDataInFIFO;
			
			switch (fieldNode->cgType) {
				case FNT_HEAD:
					if (fieldNode->CheckFixValue(fifoIn) < 1)
						return RET_AbandonFirstData;
					break;
				case FNT_MESSAGE:
					if (fieldNode->fnlength > 0){
						retTry = fieldNode->AnalysisFrameR1(fifoIn);
						if ((retTry == RET_NoEnoughDataInFIFO) || (retTry == RET_AbandonFirstData))
							return(retTry);
					}
					break;
				case FNT_CONTENT:
				case FNT_LENGTH:
				case FNT_ADDR:
				case FNT_CTRL:
				case FNT_CHECKSUM:
					break;
				case FNT_MASK:
					fieldNode->FillMaskField();
					break;
				case FNT_TAIL:
					if (fieldNode->CheckFixValue(fifoIn) > 0){//tail field match
						if (fieldNode->linkNode != nullptr){//the FN_CONTENT length decide by FN_TAIL,set fieldNode->linkNode->length
							fieldNode->linkNode->fnlength = fieldNode->fnOffset - fieldNode->linkNode->fnOffset;
							if (fieldNode->linkNode->fnlength > 0){
								retTry = fieldNode->linkNode->AnalysisFrameR1(fifoIn);
								if ((retTry == RET_NoEnoughDataInFIFO) || (retTry == RET_AbandonFirstData))
									return(retTry);
							}
						}
					}
					else{//tail field no match
						if (fieldNode->linkNode != nullptr){//the FN_CONTENTlength decide by FN_TAIL
							++ getNum;//abandon first byte data.
							goto GET_DATA;
						}
						return RET_AbandonFirstData;//content field length is fix, so the frame receive error
					}
					break;
				default:
					break;
			}
		}
		else if ((fieldNode->cgType == FNT_CONTENT) || (fieldNode->cgType == FNT_MESSAGE)){
			if (fieldNode->linkNode != nullptr){//FN's length decide by FN_LENGTH
				fieldNode->fnlength = fieldNode->linkNode->CalcCLength(fifoIn);
				if ((fieldNode->fnOffset + fieldNode->fnlength) > fifoIn.Used())
					return RET_NoEnoughDataInFIFO;
			}
			retTry = fieldNode->AnalysisFrameR1(fifoIn);
			if ((retTry == RET_NoEnoughDataInFIFO) || (retTry == RET_AbandonFirstData))
				return(retTry);
		}
		getNum += fieldNode->fnlength;
		fieldNode = (PROTOCOL_NODE*)GetcgNext(fieldNode);//read next node;
	}while(fieldNode != nullptr);
	if (ChecksumResult(fifoIn) > 0){
		fnlength = getNum;
		return RET_OK;
	}
	return RET_AbandonFirstData;
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif
