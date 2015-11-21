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
#include "Device.h"
#include "SMC.h"
//------------------------------------------------------------------------------------------//
TFileSend::TFileSend(void):RTREE_NODE(){
	exThread.ThreadInit(this,&TFileSend::SendFile);
	cDevice = nullptr;
	cfileName = "";
}
//------------------------------------------------------------------------------------------//
TFileSend::~TFileSend(void){
	StopSend();
}
//------------------------------------------------------------------------------------------//
int32 TFileSend::Execute(const DEVICE *tDevice,const std::string &tfileName){
	SMC_EncryptI(0)
	if ((tDevice == nullptr) ||(tfileName.length() == 0))
		return 0;
	if (InDoing_try() == 0)
		return 0;
	cfileName = "";
	SMC_EncryptS(0)
	StopSend();
	cfileName = tfileName;
	cDevice = (DEVICE*)tDevice;
	SMC_EncryptE(0)
	exThread.ThreadRun();
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 TFileSend::SendFile(void){
	std::fstream 	fileStream;
	uint32			fileLength,sendLength,i,j,dwBytesWr,TxBytes;
	uint8			*tempBuffer;
	std::string		strPrintdata,strTemp;
	SYS_DateTime	startTime,endTime;
	
	do{
		if (CFS_CheckFile(cfileName) == 0)
			break;
		
		tempBuffer = new uint8[PACKAGE_MAX_SIZE];
		if (tempBuffer == nullptr)
			break;
		
		SetInRun();
		startTime.Now();
		fileStream.open(cfileName.c_str(),std::ios::in|std::ios::binary);
		fileStream.seekg(0,std::ios::end);
		fileLength = (uint32)fileStream.tellg();
		
		strPrintdata = SYS_MakeTime(startTime);
		strPrintdata += "   Start to send file ,";
		strPrintdata += Str_UInt64ToString(fileLength);
		strPrintdata += " Bytes:\r\n";
		if (cDevice->cgODevList.cgOutput != nullptr){
			cDevice->cgODevList.cgOutput->Spin_InUse_set();
			cDevice->cgODevList.cgOutput->WriteDividingLine(RICH_CF_clMaroon);
			cDevice->cgODevList.cgOutput->WriteStrN(strPrintdata,RICH_CF_clPurple);
			strPrintdata = cfileName;
			strPrintdata += "\r\n";
			cDevice->cgODevList.cgOutput->WriteStr(strPrintdata,RICH_CF_clBlue);
			cDevice->cgODevList.cgOutput->WriteDividingLine(RICH_CF_clMaroon);
			cDevice->cgODevList.cgOutput->Spin_InUse_clr();
		}
		fileStream.seekg(0,std::ios::beg);
		i = 0;
		TxBytes = 0;
		do{
			sendLength = cDevice->BufferTxMaxSize() - cDevice->UnsentBytes();
			if (sendLength > PACKAGE_MAX_SIZE)
				sendLength = PACKAGE_MAX_SIZE;
			fileStream.read((char*)tempBuffer,sendLength);
			sendLength = (uint32)fileStream.gcount();
			if (sendLength > 0){
				j = 0;
				while(j < sendLength){
					dwBytesWr = cDevice->Write(&tempBuffer[j],sendLength - j);
					j += dwBytesWr;
					if (CheckblInRun() == 0)
						break;
				}
				++ i;
				TxBytes += j;
				strPrintdata = "Package ";
				strTemp = Str_IntToString(i);
				if (strTemp.length() < 4)
					strTemp.insert(0,4 - strTemp.length(),' ');
				strPrintdata += strTemp;
				strPrintdata += ": ";
				strTemp = Str_IntToString(j);
				if (strTemp.length() < 6)
					strTemp.insert(0,6 - strTemp.length(),' ');
				strPrintdata += strTemp;
				strPrintdata += " Bytes\r\n";
				if (cDevice->cgODevList.cgOutput != nullptr)
					cDevice->cgODevList.cgOutput->WriteStr(strPrintdata,RICH_CF_clBlue);
			}
			if (CheckblInRun() == 0){
				strPrintdata = SYS_MakeTimeNow();
				strPrintdata += "   The file transfer is stopped.\r\n";
				if (cDevice->cgODevList.cgOutput != nullptr){
					cDevice->cgODevList.cgOutput->Spin_InUse_set();
					cDevice->cgODevList.cgOutput->WriteDividingLine(RICH_CF_clMaroon);
					cDevice->cgODevList.cgOutput->WriteStrN(strPrintdata,RICH_CF_clPurple);
					cDevice->cgODevList.cgOutput->Spin_InUse_clr();
				}
				break;
			}
		}while(!fileStream.eof());
		fileStream.close();
		endTime.Now();
		startTime.MinusDateTime(endTime,startTime);
		strPrintdata = SYS_MakeTime(endTime);
		strPrintdata += "   Completed the transfer.\r\n";
		strPrintdata += "Transfer Time :  ";
		strPrintdata += SYS_FormatTimeABS(DEFAULT_TIMEFORMATE,startTime);
		strPrintdata += "\r\n";
		strPrintdata += "Transfer Bytes:  ";
		strPrintdata += Str_IntToString(TxBytes);
		strPrintdata += "\r\n";
		strPrintdata += "Transfer KBps :  ";
		double KBps;
		KBps = (double)TxBytes / startTime.GetSec() / 1000.0;
		strPrintdata += Str_FloatToString(KBps);
		strPrintdata += "\r\n";
		if (cDevice->cgODevList.cgOutput != nullptr){
			cDevice->cgODevList.cgOutput ->Spin_InUse_set();
			cDevice->cgODevList.cgOutput ->WriteDividingLine(RICH_CF_clMaroon);
			cDevice->cgODevList.cgOutput ->WriteStrN(strPrintdata,RICH_CF_clPurple);
			cDevice->cgODevList.cgOutput ->WriteDividingLine(RICH_CF_clMaroon);
			cDevice->cgODevList.cgOutput ->Spin_InUse_clr();
		}
		try{
			delete []tempBuffer;
		}
		catch(...){}
	}while(0);
	ClrInRun();
	InDoing_clr();
	return 1;
}
//------------------------------------------------------------------------------------------//
void TFileSend::StopSend(void){
	ClrInRun();
	exThread.ThreadStop();
}
//------------------------------------------------------------------------------------------//
