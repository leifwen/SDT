//
//  SendFile.cpp
//  SDT
//
//  Created by Leif Wen on 22/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
#include "SendFile.h"
#ifdef SendFile_h
#include "SYS_File.h"
#include "DS_STRING.h"
//------------------------------------------------------------------------------------------//
TFileSend::TFileSend(void) : DEVICE_EXE_FRAME(PACKAGE_MAX_SIZE){
	sendThread.ThreadInit(this,&TFileSend::SendThreadFun,"sendThread");
	cgFileName = "";
}
//------------------------------------------------------------------------------------------//
bool32 TFileSend::Execute(const DEVICE* dev,const STDSTR& fileName){
	if ((CFS_CheckFile(fileName) == G_FALSE) || IsExecuting(dev) == G_FALSE)
		return G_FALSE;
	
	cgFileName = fileName;
	return(DEVICE_EXE_FRAME::Execute(dev,&sendThread));
}
//------------------------------------------------------------------------------------------//
bool32 TFileSend::SendThreadFun(void* p){
	std::fstream 	fileStream;
	uint32			sendLength,i,j,dwBytesWr,TxBytes;
	uint8			*tempBuffer;
	STDSTR			strPrintdata,strTi,strTj;
	DTIME			startTime,endTime;
	
	tempBuffer = cgSBUF.cgArray.GetPointer(0);
	
	startTime.Now();
	
	*cgDevice->GetSelSTDOUT() << Begin() << NL()
	<< COL_DivLine_Maroon << COL_Time << SYS_MakeTime(startTime)
	<< COL_NormalMessage << " Start to send file, " << Str_ToStr(CFS_CheckFileSize(cgFileName)) << " Bytes:\n"
	<< COL_clBlue << cgFileName << "\n"
	<< COL_DivLine_Maroon
	<< Endl();
	
	fileStream.open(cgFileName.c_str(),std::ios::in|std::ios::binary);
	fileStream.seekg(0,std::ios::beg);

	i = 0;
	TxBytes = 0;
	do{
		sendLength = cgDevice->SRxSBUFMaxSize() - cgDevice->SUnsentBytes();
		if (sendLength > PACKAGE_MAX_SIZE)
			sendLength = PACKAGE_MAX_SIZE;
		fileStream.read((char*)tempBuffer,sendLength);
		sendLength = (uint32)fileStream.gcount();
		if (sendLength > 0){
			j = 0;
			while((j < sendLength) && (IsStop() == 0)){
				dwBytesWr = cgDevice->SSend(IUD(&tempBuffer[j],sendLength - j));
				j += dwBytesWr;
			}
			++ i;
			TxBytes += j;
			strTi = Str_ToStr(i);
			Str_AddSpaceInFront(&strTi,4);
			strTj = Str_ToStr(j);
			Str_AddSpaceInFront(&strTj,6);
			
			*cgDevice->GetSelSTDOUT() << Begin() << NL() << COL_clBlue << "Package " << strTi << ": " << strTj << " Bytes\n" << Endl();
		}
		if (IsStop()){
			*cgDevice->GetSelSTDOUT() << Begin() << NL()
			<< COL_DivLine_Maroon << COL_Time << SYS_MakeTimeNow()
			<< COL_NormalMessage << " The file transfer is stopped.\n"
			<<Endl();
			break;
		}
	}while(!fileStream.eof());
	fileStream.close();
	endTime.Now();
	startTime = endTime - startTime;
	double KBps;
	KBps = (double)TxBytes / startTime.GetSec() / 1000.0;
	
	*cgDevice->GetSelSTDOUT() << Begin() << NL()
	<< COL_DivLine_Maroon
	<< COL_Time << SYS_MakeTime(endTime)
	<< COL_NormalMessage << " Completed the transfer.\n"
	<< "Transfer Time :  " << SYS_FormatTimeABS("[hh:mm:ss.zzz]",startTime) << "\n"
	<< "Transfer Bytes:  " << Str_ToStr(TxBytes) << "\n"
	<< "Transfer KBps :  " << Str_FloatToStr(KBps) << "\n"
	<< COL_DivLine_Maroon
	<< Endl();
	return G_TRUE;
}
//------------------------------------------------------------------------------------------//
#endif /* SendFile_h */
