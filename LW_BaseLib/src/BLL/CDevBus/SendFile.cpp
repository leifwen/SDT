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
TFileSend::TFileSend(void) : CDEVBUS_APP(PACKAGE_MAX_SIZE){
	sendThread.ThreadInit(this,&TFileSend::SendThreadFunc,"sendThread");
	cgFileName = "";
	cgPackSize = PACKAGE_MAX_SIZE;
	cgRateBps = FS_BPS_DEF;
	extSDTOUT = nullptr;
};
//------------------------------------------------------------------------------------------//
bool32 TFileSend::Execute(const CDEVBUS* cDevBus,const STDSTR& fileName,OUTPUT_NODE* sdtout,uint32 rateBps,uint32 packSize){
	if ((CFS_CheckFile(fileName) == G_FALSE) || IsExecuting(cDevBus) == G_FALSE)
		return G_FALSE;
	
	cgFileName = fileName;
	cgPackSize = packSize < PACKAGE_MAX_SIZE ? packSize : PACKAGE_MAX_SIZE;
	cgRateBps = rateBps;
	extSDTOUT = sdtout;
	return(CDEVBUS_APP::Execute(cDevBus,&sendThread));
};
//------------------------------------------------------------------------------------------//
static uint64 _Print(OUTPUT_NODE* sdtout,const uint32& bps,const uint64& fileSize,const uint64& wrSize,const uint64& percentage,bool32 blforce){
	uint64	newPercentage;
	double	rt;
	STDSTR	strPer,strRT;

	newPercentage = wrSize * 100 / fileSize;
	if (blforce == G_FALSE){
		if (newPercentage == percentage)
			return(newPercentage);
	}
	
	strPer = Str_ToStr(newPercentage);
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
		SYS_SleepMS(1);
		OUTPUT_NODE::PrintWithTime_noNL(sdtout, "Sending", COL_clBlue, strPer, COL_NormalMessage, "%,"
										,"expected time remaining", COL_clBlue, strRT, COL_NormalMessage,"s.");
	}
	else{
		OUTPUT_NODE::CleanLastLine(sdtout);
		SYS_SleepMS(1);
		OUTPUT_NODE::PrintWithTime_noNL(sdtout, "Sending", COL_clBlue, strPer, COL_NormalMessage, "%.");
	}
	return(newPercentage);
};
//------------------------------------------------------------------------------------------//
bool32 TFileSend::SendThreadFunc(void* p){
	std::fstream 	fileStream;
	uint32			sendLength,sendCount;
	uint64			txBytes,fnSize,percentage,newPercentage;
	uint8			*tempBuffer;
	DTIME			startTime,endTime;
	SYS_TIME_S		dTm,tm1S,calcBpsTm;
	bool32 			blforce;
	double			delayMS,dBps;
	
	tempBuffer = cgSBUF.array.GetPointer(0);
		
	startTime.Now();
	
	fnSize = CFS_CheckFileSize(cgFileName);
	*cgCDevBus->GetSelSTDOUT() << Begin() << NL()
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

	txBytes = 0;
	percentage = 0;
	newPercentage = 0;
	blforce = G_TRUE;
	SYS_StopWatch_Start(&tm1S);
	SYS_StopWatch_Start(&calcBpsTm);
	do{
		delayMS = 2;
		sendLength = cgCDevBus->TxMaxSize() - cgCDevBus->UnsentBytes();
		if (sendLength > cgPackSize)
			sendLength = cgPackSize;
		if (sendLength > 0){
			fileStream.read((char*)tempBuffer,sendLength);
			sendLength = (uint32)fileStream.gcount();
			
			SYS_StopWatch_Start(&dTm);
			sendCount = cgCDevBus->Send(IUD(tempBuffer,sendLength),100);
			SYS_StopWatch_Stop(&dTm);
			
			delayMS = sendCount * 1000 / (double)cgRateBps;
			delayMS = delayMS < dTm.timeMS ? 0 : (delayMS - dTm.timeMS);

			txBytes += sendCount;
		}
		{
			SYS_StopWatch_Stop(&tm1S);
			if (tm1S.timeMS > 100)
				blforce = G_TRUE;
			SYS_StopWatch_Stop(&calcBpsTm);
			dBps = (double)txBytes / calcBpsTm.timeMS * 1000.0 + 1;
			if (dBps > cgRateBps)
				dBps = cgRateBps;
			newPercentage = _Print(extSDTOUT,dBps,fnSize,txBytes,percentage,blforce);
			if ((newPercentage != percentage) || (blforce != G_FALSE)){
				blforce = G_FALSE;
				SYS_StopWatch_Start(&tm1S);
			}
			percentage = newPercentage;
		}
		if (IsTerminated()){
			*cgCDevBus->GetSelSTDOUT() << Begin() << NL()
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
	*cgCDevBus->GetSelSTDOUT() << Begin() << NL()
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
