//
//  SendFile.cpp
//  SDT
//
//  Created by Leif Wen on 22/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "SendFile.h"
#ifdef SendFile_h
#include "SYS_File.h"
#include "DS_STRING.h"
//------------------------------------------------------------------------------------------//
TFileSend::TFileSend(void) : DEVICE_EXE_FRAME(PACKAGE_MAX_SIZE){
	sendThread.ThreadInit(this,&TFileSend::SendThreadFun,"sendThread");
	cgFileName = "";
	cgPackSize = PACKAGE_MAX_SIZE;
	cgRateBps = FS_BPS_DEF;
	extSDTOUT = nullptr;
};
//------------------------------------------------------------------------------------------//
bool32 TFileSend::Execute(const DEVICE* dev,const STDSTR& fileName,OUTPUT_NODE* sdtout,uint32 rateBps,uint32 packSize){
	if ((CFS_CheckFile(fileName) == G_FALSE) || IsExecuting(dev) == G_FALSE)
		return G_FALSE;
	
	cgFileName = fileName;
	cgPackSize = packSize < PACKAGE_MAX_SIZE ? packSize : PACKAGE_MAX_SIZE;
	cgRateBps = rateBps;
	extSDTOUT = sdtout;
	return(DEVICE_EXE_FRAME::Execute(dev,&sendThread));
};
//------------------------------------------------------------------------------------------//
static void _Print(OUTPUT_NODE* sdtout,const uint32& bps,const uint64& fileSize,const uint64& wrSize){
	uint64	p;
	double	rt;
	STDSTR	strPer,strRT;

	p = wrSize * 100 / fileSize;
	strPer = Str_ToStr(p);
	Str_AddSpaceInFront(&strPer, 3);
	
	rt = (fileSize - wrSize) / (double)bps;
	if (rt > 1){
		strRT = Str_ToStr((uint64)rt);
	}
	else{
		strRT = Str_FloatToStr(rt);
	}

	if (wrSize < fileSize){
		OUTPUT_NODE::CleanLastLine(sdtout);
		OUTPUT_NODE::PrintWithTime_noNL(sdtout, "Sending", COL_clBlue, strPer, COL_NormalMessage, "%,"
										,"expected time remaining", COL_clBlue, strRT, COL_NormalMessage,"s.");
	}
	else{
		OUTPUT_NODE::CleanLastLine(sdtout);
		OUTPUT_NODE::PrintWithTime_noNL(sdtout, "Sending", COL_clBlue, strPer, COL_NormalMessage, "%.");
	}
};
//------------------------------------------------------------------------------------------//
bool32 TFileSend::SendThreadFun(void* p){
	std::fstream 	fileStream;
	uint32			sendLength,i,j;
	uint64			txBytes,fnSize;
	uint8			*tempBuffer;
	STDSTR			strTi,strTj;
	DTIME			startTime,endTime;
	SYS_TIME_S		dTm;
	double			delayMS;
	
	tempBuffer = cgSBUF.array.GetPointer(0);
		
	startTime.Now();
	
	fnSize = CFS_CheckFileSize(cgFileName);
	*cgDevice->GetSelSTDOUT() << Begin() << NL()
	<< COL_DivLine_Maroon << COL_NormalMessage
	<< " File size Bytes: " << Str_ToStr(fnSize) << NL()
	<< " Prediction time: " << Str_FloatToStr((double)fnSize / (double)cgRateBps) << "s" << NL()
	<< " File name:       " << COL_clBlue << cgFileName << NL()
	<< COL_DivLine_Maroon
	<< COL_Time << SYS_MakeTime(startTime) << COL_NormalMessage << " Start transmission:" << NL()
	<< Endl();
	
	SYS_SleepMS(1);
	
	fileStream.open(cgFileName.c_str(),std::ios::in|std::ios::binary);
	fileStream.seekg(0,std::ios::beg);

	i = 0;
	txBytes = 0;
	do{
		delayMS = 2;
		sendLength = cgDevice->TxMaxSize() - cgDevice->UnsentBytes();
		if (sendLength > cgPackSize)
			sendLength = cgPackSize;
		if (sendLength > 0){
			fileStream.read((char*)tempBuffer,sendLength);
			sendLength = (uint32)fileStream.gcount();
			
			SYS_StopWatch_Start(&dTm);
			j = cgDevice->Send(IUD(tempBuffer,sendLength));
			SYS_StopWatch_Stop(&dTm);
			
			delayMS = j * 1000 / (double)cgRateBps;
			delayMS = delayMS < dTm.timeMS ? 0 : (delayMS - dTm.timeMS);

			++ i;
			txBytes += j;
			strTi = Str_ToStr(i);
			Str_AddSpaceInFront(&strTi,4);
			strTj = Str_ToStr(j);
			Str_AddSpaceInFront(&strTj,6);
			
			_Print(extSDTOUT,cgRateBps,fnSize,txBytes);
		}
		if (IsTerminated()){
			*cgDevice->GetSelSTDOUT() << Begin() << NL()
			<< COL_DivLine_Maroon << COL_Time << SYS_MakeTimeNow()
			<< COL_NormalMessage << " The file transmission stopped.\n"
			<<Endl();
			break;
		}
		SYS_Delay_SetTS(&dTm,delayMS);
		while (!(IsTerminated() || SYS_Delay_IsTimeout(&dTm)))
			SYS_SleepMS(1);
	}while(!fileStream.eof());
	fileStream.close();
	endTime.Now();
	startTime = endTime - startTime;
	double KBps;
	KBps = (double)txBytes / startTime.GetSec() / 1000.0;
	
	SYS_SleepMS(10);
	*cgDevice->GetSelSTDOUT() << Begin() << NL()
	<< COL_Time << SYS_MakeTime(endTime)
	<< COL_NormalMessage << " Completed the transmission.\n"
	<< "transmission Time :  " << SYS_FormatTimeABS("[hh:mm:ss.zzz]",startTime) << NL()
	<< "transmission Bytes:  " << Str_ToStr(txBytes) << NL()
	<< "transmission KBps :  " << Str_FloatToStr(KBps) << NL()
	<< COL_DivLine_Maroon
	<< Endl();
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
#endif /* SendFile_h */
