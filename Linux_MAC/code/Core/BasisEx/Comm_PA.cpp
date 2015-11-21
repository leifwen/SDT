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
//------------------------------------------------------------------------------------------//
Field_Node::Field_Node(void) : RTREE_NODE() {
	cgType = FNT_UNINIT;
	cgEndianType = G_LITTLE_ENDIAN;
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
void Field_Node::Init(FNT_Type fType,const FIFO_UINT8 *tfifo,uint32 fbyteNum,G_Endian_VAILD tEV){
	cgType = fType;
	cgObyteNum = fbyteNum;
	cgEndianType = tEV;
	cgDefFifo = (FIFO_UINT8*)tfifo;
}
//------------------------------------------------------------------------------------------//
void Field_Node::SetFixValue(const std::string &str){
	fixValue = str;
	cgObyteNum = (uint32)fixValue.length();
}
//------------------------------------------------------------------------------------------//
int32 Field_Node::CheckFixValue(const FIFO_UINT8 &fifobuf)const{
	uint8	charP1,charP2;
	uint32	tOffset,fLength;
	
	tOffset = fnOffset + fnlength;
	fLength = fnlength;
	charP1 = 0;
	while(fLength-- > 0){
		fifobuf.Read(&charP1, 1, --tOffset);
		charP2 = fixValue[fLength];
		if (charP1 != charP2)
			return 0;
	}
	return 1;
}
//------------------------------------------------------------------------------------------//
void Field_Node::SetMaskBit(uint32 mask){
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
uint32 Field_Node::GetOriginalValue(const FIFO_UINT8 *fifobuf)const{
	uint32 ret,i;
	uint8 tData;
	
	if (fifobuf == nullptr)
		fifobuf = cgDefFifo;

	tData = 0;
	ret = 0;
	if (cgEndianType == G_LITTLE_ENDIAN){
		i = fnlength;
		while(i-- > 0){
			ret <<= 8;
			fifobuf->Read(&tData, 1, fnOffset + i);
			ret |= tData;
		};
	}
	else{
		i = 0;
		do{
			ret <<= 8;
			fifobuf->Read(&tData, 1, fnOffset + i);
			ret |= tData;
		}while(++i < fnlength);
	}
	return(ret);
}
//------------------------------------------------------------------------------------------//
uint32 Field_Node::GetValueAMask(const FIFO_UINT8 *fifobuf)const{
	if (fifobuf == nullptr)
		fifobuf = cgDefFifo;
	return(GetOriginalValue(fifobuf) & maskBit);
};
//------------------------------------------------------------------------------------------//
uint32 Field_Node::GetValueCalc(const FIFO_UINT8 *fifobuf)const{
	if (fifobuf == nullptr)
		fifobuf = cgDefFifo;
	return((GetOriginalValue(fifobuf) & maskBit) >> moveBit);
};
//------------------------------------------------------------------------------------------//
const std::string &Field_Node::ReadAllContent(std::string *retStr,const FIFO_UINT8 *fifobuf)const{
	if (fifobuf == nullptr)
		fifobuf = cgDefFifo;
	*retStr = "";
	fifobuf->ReadInASCII(retStr,fnlength,fnOffset);
	return(*retStr);
};
//------------------------------------------------------------------------------------------//
const FIFO_UINT8 &Field_Node::ReadAllContent(FIFO_UINT8 *retfifo,const FIFO_UINT8 *fifobuf)const{
	if (fifobuf == nullptr)
		fifobuf = cgDefFifo;

	if (retfifo != fifobuf)
		retfifo->Put(*fifobuf,fnlength,fnOffset);
	return(*retfifo);
};
//------------------------------------------------------------------------------------------//
const PUB_SBUF &Field_Node::ReadAllContent(PUB_SBUF *retPSBUF,const FIFO_UINT8 *fifobuf)const{
	if (fifobuf == nullptr)
		fifobuf = cgDefFifo;
	retPSBUF->Put(*fifobuf,fnlength,fnOffset);
	return(*retPSBUF);
};
//------------------------------------------------------------------------------------------//
const Field_Node &Field_Node::ReadAllContent(Field_Node *retFN,const FIFO_UINT8 *fifobuf)const{
	if (fifobuf == nullptr)
		fifobuf = cgDefFifo;
	retFN->Spin_InUse_set();
	retFN->cgDefFifo->Put(*fifobuf,fnlength,fnOffset);
	retFN->Spin_InUse_clr();
	return(*retFN);
};
//------------------------------------------------------------------------------------------//
void Field_Node::SetFIFO(void){
	fnOffset = cgDefFifo->Used();
	fnlength = 0;
	if (cgType == FNT_MASK)
		FillMaskField();
};
//------------------------------------------------------------------------------------------//
void Field_Node::SetFIFO_4Byte(uint32 tInput){
	uint8 	tData;
	fnOffset = cgDefFifo->Used();
	fnlength = 4;
	if (cgEndianType == G_LITTLE_ENDIAN){
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
void Field_Node::SetFIFO_3Byte(uint32 tInput){
	uint8 	tData;
	fnOffset = cgDefFifo->Used();
	fnlength = 3;
	if (cgEndianType == G_LITTLE_ENDIAN){
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
void Field_Node::SetFIFO_2Byte(uint32 tInput){
	uint8 	tData;
	fnOffset = cgDefFifo->Used();
	fnlength = 2;
	if (cgEndianType == G_LITTLE_ENDIAN){
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
void Field_Node::SetFIFO_1Byte(uint32 tInput){
	uint8 	tData;
	fnOffset = cgDefFifo->Used();
	fnlength = 1;
	tData = (uint8)(tInput);
	cgDefFifo->Put(&tData, 1);
	if (cgType == FNT_MASK)
		FillMaskField();
};
//------------------------------------------------------------------------------------------//
void Field_Node::SetFIFOByte(uint64 tInput){
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
void Field_Node::UpdateFIFO_4Byte(uint32 tInput){
	uint8 	tData;
	if (cgEndianType == G_LITTLE_ENDIAN){
		tData = (uint8)(tInput);
		cgDefFifo->Update(&tData, 1,fnOffset);
		tData = (uint8)(tInput >> 8);
		cgDefFifo->Update(&tData, 1,fnOffset + 1);
		tData = (uint8)(tInput >> 16);
		cgDefFifo->Update(&tData, 1,fnOffset + 2);
		tData = (uint8)(tInput >> 24);
		cgDefFifo->Update(&tData, 1,fnOffset + 3);
	}
	else{
		tData = (uint8)(tInput >> 24);
		cgDefFifo->Update(&tData, 1,fnOffset);
		tData = (uint8)(tInput >> 16);
		cgDefFifo->Update(&tData, 1,fnOffset + 1);
		tData = (uint8)(tInput >> 8);
		cgDefFifo->Update(&tData, 1,fnOffset + 2);
		tData = (uint8)(tInput);
		cgDefFifo->Update(&tData, 1,fnOffset + 3);
	}
	if (cgType == FNT_MASK)
		FillMaskField();
};
//------------------------------------------------------------------------------------------//
void Field_Node::UpdateFIFO_3Byte(uint32 tInput){
	uint8 	tData;
	if (cgEndianType == G_LITTLE_ENDIAN){
		tData = (uint8)(tInput);
		cgDefFifo->Update(&tData, 1,fnOffset);
		tData = (uint8)(tInput >> 8);
		cgDefFifo->Update(&tData, 1,fnOffset + 1);
		tData = (uint8)(tInput >> 16);
		cgDefFifo->Update(&tData, 1,fnOffset + 2);
	}
	else{
		tData = (uint8)(tInput >> 16);
		cgDefFifo->Update(&tData, 1,fnOffset);
		tData = (uint8)(tInput >> 8);
		cgDefFifo->Update(&tData, 1,fnOffset + 1);
		tData = (uint8)(tInput);
		cgDefFifo->Update(&tData, 1,fnOffset + 2);
	}
	if (cgType == FNT_MASK)
		FillMaskField();
};
//------------------------------------------------------------------------------------------//
void Field_Node::UpdateFIFO_2Byte(uint32 tInput){
	uint8 	tData;
	if (cgEndianType == G_LITTLE_ENDIAN){
		tData = (uint8)(tInput);
		cgDefFifo->Update(&tData, 1,fnOffset);
		tData = (uint8)(tInput >> 8);
		cgDefFifo->Update(&tData, 1,fnOffset + 1);
	}
	else{
		tData = (uint8)(tInput >> 8);
		cgDefFifo->Update(&tData, 1,fnOffset);
		tData = (uint8)(tInput);
		cgDefFifo->Update(&tData, 1,fnOffset + 1);
	}
	if (cgType == FNT_MASK)
		FillMaskField();
};
//------------------------------------------------------------------------------------------//
void Field_Node::UpdateFIFO_1Byte(uint32 tInput){
	uint8 	tData;
	tData = (uint8)(tInput);
	cgDefFifo->Update(&tData, 1,fnOffset);
	if (cgType == FNT_MASK)
		FillMaskField();
};
//------------------------------------------------------------------------------------------//
void Field_Node::UpdateFIFOByte(uint64 tInput){
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
void Field_Node::SetFIFOFixValue(void){
	fnOffset = cgDefFifo->Used();
	fnlength = cgDefFifo->Put((uint8*)fixValue.c_str(), (uint32)fixValue.length());
	if (cgType == FNT_MASK)
		FillMaskField();
};
//------------------------------------------------------------------------------------------//
void Field_Node::SetFIFO(const uint8 *tInput,uint32 num){
	fnOffset = cgDefFifo->Used();
	fnlength = cgDefFifo->Put(tInput, num);
	if (cgType == FNT_MASK)
		FillMaskField();
};
//------------------------------------------------------------------------------------------//
void Field_Node::SetFIFO(const FIFO_UINT8 &fifoIn,uint32 num,uint32 offset){
	fnOffset = cgDefFifo->Used();
	fnlength = cgDefFifo->Put(fifoIn,num,offset);
	if (cgType == FNT_MASK)
		FillMaskField();
};
//------------------------------------------------------------------------------------------//
void Field_Node::SetFIFO(const Field_Node &fnIn){
	fnOffset = cgDefFifo->Used();
	fnlength = cgDefFifo->Put(*GetcgDefFifo(&fnIn),fnIn.GetLength(),fnIn.GetOffset());
	if (cgType == FNT_MASK)
		FillMaskField();
};
//------------------------------------------------------------------------------------------//
void Field_Node::FillMaskField(void){
	Field_Node	*fieldNode;

	if (cgType != FNT_MASK)
		return;
	
	fieldNode = (Field_Node*)GetFirstChild_nolock(this);

	while(fieldNode != nullptr){
		fieldNode->cgObyteNum = cgObyteNum;
		fieldNode->cgEndianType = cgEndianType;
		fieldNode->fnOffset = fnOffset;
		fieldNode->fnlength = fnlength;
		fieldNode = (Field_Node*)GetNextBrother_nolock(fieldNode);
	}
}
//------------------------------------------------------------------------------------------//
uint32 Field_Node::CalcCLength(const FIFO_UINT8 &fifobuf)const{
	Field_Node	*fieldNode,*tNode;
	uint32		vaildAreaLength;
	
	if (cgType != FNT_LENGTH)
		return 0;
	if (vaildAreaH == nullptr)
		return(GetValueCalc(&fifobuf));
	
	tNode = vaildAreaT;
	if (tNode == nullptr)
		tNode = vaildAreaH;
	
	fieldNode = vaildAreaH;
	vaildAreaLength = fieldNode->fnlength;
	while((fieldNode != nullptr) && (fieldNode != tNode)){
		vaildAreaLength += fieldNode->fnlength;
		fieldNode = (Field_Node*)GetNextBrother_nolock(fieldNode);
	};

	return(GetValueCalc(&fifobuf) - vaildAreaLength);
}
//------------------------------------------------------------------------------------------//
enum{
	RET_OK = 1,
	RET_AbandonFirstData = 0,
	RET_NoEnoughDataInFIFO = -1,
	RET_NotMessage = -2,
	RET_NoFirstChild = -3,
};
//------------------------------------------------------------------------------------------//
uint32 Field_Node::TryGetFrameAgain_if_NoEnoughDataInFIFO(FIFO_UINT8 *fifoIn){
	int32	retTry;
	uint32	fifoInOffset;
	
	fifoInOffset = 0;
	do{
		retTry = AnalysisFrameR0(*fifoIn,fifoInOffset);
		if (retTry == RET_OK){
			cgDefFifo->Empty();
			cgDefFifo->Put(*fifoIn,fnlength,fnOffset);
			fifoIn->Out(fnOffset + fnlength);
			AnalysisFrameR0(*cgDefFifo);
			return(fnlength);
		}
		else if ((retTry == RET_AbandonFirstData) || (retTry == RET_NoEnoughDataInFIFO)){
			++ fifoInOffset;
		}
		else{
			break;
		}
	}while(fifoInOffset < fifoIn->Used());
	fnOffset = 0;
	fnlength = 0;
	return 0;
}
//------------------------------------------------------------------------------------------//
uint32 Field_Node::TryGetFrameAgain_if_NoEnoughDataInFIFO(void){
	cgDefFifo->Out(fnOffset + fnlength);
	return(AnalysisFrameTryAgain(*cgDefFifo));
}
//------------------------------------------------------------------------------------------//
uint32 Field_Node::TryGetFrameAgain_if_NoEnoughDataInFIFO(const FIFO_UINT8 &fifoIn){
	cgDefFifo->Out(fnOffset + fnlength);
	cgDefFifo->Put(fifoIn, -1);
	return(AnalysisFrameTryAgain(*cgDefFifo));
}
//------------------------------------------------------------------------------------------//
uint32 Field_Node::TryGetFrameAgain_if_NoEnoughDataInFIFO(const uint8 *data,uint32 num){
	cgDefFifo->Out(fnOffset + fnlength);
	cgDefFifo->Put(data, num);
	return(AnalysisFrameTryAgain(*cgDefFifo));
}
//------------------------------------------------------------------------------------------//
uint32 Field_Node::TryGetFrame(const FIFO_UINT8 &fifoIn){
	cgDefFifo->Out(fnOffset + fnlength);
	cgDefFifo->Put(fifoIn,-1);
	return(AnalysisFrame(*cgDefFifo));
}
//------------------------------------------------------------------------------------------//
int32 Field_Node::TryGetFrame(const uint8 *data,uint32 num){
	cgDefFifo->Out(fnOffset + fnlength);
	cgDefFifo->Put(data, num);
	return(AnalysisFrame(*cgDefFifo));
}
//------------------------------------------------------------------------------------------//
int32 Field_Node::TryGetFrame(const std::string &strInput){
	cgDefFifo->Out(fnOffset + fnlength);
	cgDefFifo->PutInASCII(strInput, G_ESCAPE_OFF);
	return(AnalysisFrame(*cgDefFifo));
}
//------------------------------------------------------------------------------------------//
uint32 Field_Node::TryGetFrame(void){
	cgDefFifo->Out(fnOffset + fnlength);
	return(AnalysisFrame(*cgDefFifo));
}
//------------------------------------------------------------------------------------------//
uint32 Field_Node::AnalysisFrameTryAgain(const FIFO_UINT8 &fifoIn){
	int32	retTry;
	uint32	fifoInOffset;
	
	fifoInOffset = 0;
	do{
		retTry = AnalysisFrameR0(fifoIn,fifoInOffset);
		if (retTry == RET_OK){
			return(fnlength);
		}
		else if ((retTry == RET_AbandonFirstData) || (retTry == RET_NoEnoughDataInFIFO)){
			++ fifoInOffset;
		}
		else{
			break;
		}
	}while(fifoInOffset < fifoIn.Used());
	fnOffset = 0;
	fnlength = 0;
	return 0;
}
//------------------------------------------------------------------------------------------//
uint32 Field_Node::AnalysisFrame(const FIFO_UINT8 &fifoIn,uint32 fifoInOffset){
	if (AnalysisFrameR0(fifoIn,fifoInOffset) == RET_OK)
		return(fnlength);
	fnOffset = 0;
	fnlength = 0;
	return 0;
}
//------------------------------------------------------------------------------------------//
int32 Field_Node::AnalysisFrameR0(const FIFO_UINT8 &fifoIn,uint32 fifoInOffset){
	int32 retTry;
	
	fnOffset = fifoInOffset;
	fnlength = 0;
	while(fnOffset < fifoIn.Used()){
		retTry = AnalysisFrameR1(fifoIn);
		if (retTry != RET_AbandonFirstData)
			return(retTry);
		if (fifoInOffset > 0)//only analysis once,
			return RET_AbandonFirstData;
		++ fnOffset;//abandon first byte data.
	}
	return RET_NoEnoughDataInFIFO;
}
//------------------------------------------------------------------------------------------//
int32 Field_Node::AnalysisFrameR1(const FIFO_UINT8 &fifoIn){
	Field_Node	*fieldNode;
	uint32		getNum;
	int32		retTry;

	if (cgType != FNT_MESSAGE)
		return RET_NotMessage;
	
	fieldNode = (Field_Node*)GetFirstChild_nolock(this); //read Head;
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
		fieldNode = (Field_Node*)GetNextBrother_nolock(fieldNode);//read next node;
	}while(fieldNode != nullptr);
	if (ChecksumResult(fifoIn) > 0){
		fnlength = getNum;
		return RET_OK;
	}
	return RET_AbandonFirstData;
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
