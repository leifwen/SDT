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
//------------------------------------------------------------------------------------------//
uint32 PUB_SBUF::Put(const uint8 *dataBuf,uint32 num){
	uint32	dataNum;
	dataNum = 0;
	Spin_InUse_set();
	if (CheckblEnabled() != 0){
		if (num > cgBufFIFO.Unused())
			cgBufFIFO.Init(cgBufFIFO.MaxSize() + num - cgBufFIFO.Unused());
		dataNum = cgBufFIFO.Put(dataBuf,num);
	}
	Spin_InUse_clr();
	return(dataNum);
}
//------------------------------------------------------------------------------------------//
uint32 PUB_SBUF::Get(uint8 *dataBuf,uint32 num){
	uint32	dataNum;
	dataNum = 0;
	Spin_InUse_set();
	if (CheckblEnabled() != 0)
		dataNum = cgBufFIFO.Get(dataBuf,num);
	Spin_InUse_clr();
	return(dataNum);
}
//------------------------------------------------------------------------------------------//
uint32 PUB_SBUF::Put(const FIFO_UINT8 &dataFIFO,uint32 num,uint32 offset){
	uint32	dataNum;
	dataNum = 0;
	Spin_InUse_set();
	if (CheckblEnabled() != 0){
		if (num > cgBufFIFO.Unused())
			cgBufFIFO.Init(cgBufFIFO.MaxSize() + num - cgBufFIFO.Unused());
		dataNum = cgBufFIFO.Put(dataFIFO,num,offset);
	}
	Spin_InUse_clr();
	return(dataNum);
}
//------------------------------------------------------------------------------------------//
uint32 PUB_SBUF::Get(FIFO_UINT8 *dataFIFO,uint32 num){
	uint32	dataNum;
	dataNum = 0;
	Spin_InUse_set();
	if (CheckblEnabled() != 0)
		dataNum = cgBufFIFO.Get(dataFIFO,num);
	Spin_InUse_clr();
	return(dataNum);
}
//------------------------------------------------------------------------------------------//
uint32 PUB_SBUF::Out(uint32 num,G_LOCK_VAILD blLock){
	uint32	dataNum;
	dataNum = 0;
	Spin_InUse_set(blLock);
	if (CheckblEnabled() != 0)
		dataNum = cgBufFIFO.Out(num);
	Spin_InUse_clr(blLock);
	return(dataNum);
}
//------------------------------------------------------------------------------------------//
uint32 PUB_SBUF::GetInHEX(std::string *strNewData,G_SPACE_VAILD blSpace){
	uint32	dataNum;
	dataNum = 0;
	Spin_InUse_set();
	if (CheckblEnabled() != 0){
		if (blSpace == G_SPACE_OFF){
			dataNum = cgBufFIFO.GetInHEX(strNewData,cgBufFIFO.Used());
		}
		else{
			dataNum = cgBufFIFO.GetInHEX_S(strNewData,cgBufFIFO.Used());
		}
	}
	Spin_InUse_clr();
	return(dataNum);
}
//------------------------------------------------------------------------------------------//
uint32 PUB_SBUF::GetInASCII(std::string *strNewData,G_ESCAPE_VAILD blEscape){
	uint32	dataNum;
	dataNum = 0;
	Spin_InUse_set();
	if (CheckblEnabled() != 0){
		if (blEscape == G_ESCAPE_OFF){
			dataNum = cgBufFIFO.GetInASCII(strNewData,cgBufFIFO.Used());
		}
		else{
			dataNum = cgBufFIFO.GetInASCII_E(strNewData,cgBufFIFO.Used());
		}
	}
	Spin_InUse_clr();
	return(dataNum);
}
//------------------------------------------------------------------------------------------//
uint32 PUB_SBUF::PutInHEX(const std::string &strNewData){
	uint32	dataNum;
	dataNum = 0;
	Spin_InUse_set();
	if (CheckblEnabled() != 0){
		if (strNewData.length() / 2 > cgBufFIFO.Unused())
			cgBufFIFO.Init(cgBufFIFO.MaxSize() + (uint32)strNewData.length() / 2 - cgBufFIFO.Unused());
		dataNum = cgBufFIFO.PutInHEX(strNewData);
	}
	Spin_InUse_clr();
	return(dataNum);
}
//------------------------------------------------------------------------------------------//
uint32 PUB_SBUF::PutInASCII(const std::string &strNewData,G_ESCAPE_VAILD blEscape){
	int32	dataNum;
	dataNum = 0;
	Spin_InUse_set();
	if (CheckblEnabled() != 0){
		if (strNewData.length() > cgBufFIFO.Unused())
			cgBufFIFO.Init(cgBufFIFO.MaxSize() + (uint32)strNewData.length() - cgBufFIFO.Unused());
		dataNum = cgBufFIFO.PutInASCII(strNewData,blEscape);
	}
	Spin_InUse_clr();
	return(dataNum);
}
//------------------------------------------------------------------------------------------//









//------------------------------------------------------------------------------------------//
int32 PUB_DBUF::Init(uint32 tSize){
	cgRxBuffer.Init(tSize);
	cgTxBuffer.Init(tSize);
	cgRxBytes = 0;
	cgTxBytes = 0;
	cgFwBytes = 0;
	return 1;
}
//------------------------------------------------------------------------------------------//
void PUB_DBUF::Clean(void){
	Spin_InUse_set();
	cgRxBytes = 0;
	cgTxBytes = 0;
	cgFwBytes = 0;
	cgRxBuffer.Clean();
	cgTxBuffer.Clean();
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
