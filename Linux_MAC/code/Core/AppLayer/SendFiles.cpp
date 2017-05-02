/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: SendFiles.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2012.12.12
 *
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "SendFiles.h"
#include "Comm_Convert.h"
#include "Comm_File.h"
#include "SMC.h"
#ifdef SendFilesH
//------------------------------------------------------------------------------------------//
TFileSend::TFileSend(void):DEVICE_EXE_FRAME(){
	sendThread.ThreadInit(this,&TFileSend::SendThreadFun,"sendThread");
	cgfileName = "";
}
//------------------------------------------------------------------------------------------//
int32 TFileSend::Execute(const DEVICE *tDevice,const STDSTR &tfileName){
	if ((CFS_CheckFile(tfileName) == 0) || CheckExecute(tDevice) == 0)
		return 0;
	
	cgfileName = tfileName;
	return(DEVICE_EXE_FRAME::Execute(tDevice,&sendThread));
}
//------------------------------------------------------------------------------------------//
int32 TFileSend::SendThreadFun(void *p){
	std::fstream 	fileStream;
	uint32			fileLength,sendLength,i,j,dwBytesWr,TxBytes;
	uint8			*tempBuffer;
	STDSTR			strPrintdata,strTi,strTj;
	TIME			startTime,endTime;
	
	tempBuffer = cgSBUF.cgFIFO.GetPointer(0);
	
	startTime.Now();
	fileStream.open(cgfileName.c_str(),std::ios::in|std::ios::binary);
	fileStream.seekg(0,std::ios::end);
	fileLength = (uint32)fileStream.tellg();
	fileStream.seekg(0,std::ios::beg);
	
	DeviceLock();
	*GetDevOut() << Start << NL
	<< COL_DivLine_Maroon << COL_Time << SYS_MakeTime(startTime)
	<< COL_NormalMessage << "   Start to send file, " << Str_ToString(fileLength) << " Bytes:\n"
	<< COL_clBlue << cgfileName << "\n"
	<< COL_DivLine_Maroon
	<< Endl;
	DeviceUnlock();
	
	i = 0;
	TxBytes = 0;
	do{
		sendLength = cgDevice->SRxSUFMaxSize() - cgDevice->SUnsentBytes();
		if (sendLength > PACKAGE_MAX_SIZE)
			sendLength = PACKAGE_MAX_SIZE;
		fileStream.read((char*)tempBuffer,sendLength);
		sendLength = (uint32)fileStream.gcount();
		if (sendLength > 0){
			j = 0;
			while((j < sendLength) && (IsStop() == 0)){
				dwBytesWr = cgDevice->SSend(&tempBuffer[j],sendLength - j);
				j += dwBytesWr;
			}
			++ i;
			TxBytes += j;
			strTi = Str_ToString(i);
			AddSpaceInFront(&strTi,4);
			strTj = Str_ToString(j);
			AddSpaceInFront(&strTj,6);
			
			DeviceLock();
			*GetDevOut() << Start << NL << COL_clBlue << "Package " << strTi << ": " << strTj << " Bytes\n" << Endl;
			DeviceUnlock();
		}
		if (IsStop() != 0){
			DeviceLock();
			*GetDevOut() << Start << NL
			<< COL_DivLine_Maroon << COL_Time << SYS_MakeTime(startTime)
			<< COL_NormalMessage << "   The file transfer is stopped.\n"
			<< Endl;
			DeviceUnlock();
			break;
		}
	}while(!fileStream.eof());
	fileStream.close();
	endTime.Now();
	startTime.MinusDateTime(endTime,startTime);
	double KBps;
	KBps = (double)TxBytes / startTime.GetSec() / 1000.0;
	
	DeviceLock();
	*GetDevOut() << Start << NL
	<< COL_DivLine_Maroon
	<< COL_Time << SYS_MakeTime(endTime)
	<< COL_NormalMessage << "   Completed the transfer.\n"
	<< "Transfer Time :  " << SYS_FormatTimeABS(DEFAULT_TIMEFORMATE,startTime) << "\n"
	<< "Transfer Bytes:  " << Str_ToString(TxBytes) << "\n"
	<< "Transfer KBps :  " << Str_FloatToString(KBps) << "\n"
	<< COL_DivLine_Maroon
	<< Endl;
	DeviceUnlock();
	return 1;
}
//------------------------------------------------------------------------------------------//
#endif
