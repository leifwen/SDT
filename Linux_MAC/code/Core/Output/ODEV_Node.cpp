/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: OutODEV.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2013.7.5
 *			: 2014.1.13 remove support BCB2012
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "ODEV_Node.h"
#include "SYS_Time.h"
#include "Comm_Convert.h"
//------------------------------------------------------------------------------------------//
#ifndef DIVIDING_LINE
#define	DIVIDING_LINE	"----------------------------------------------------------------------"
#endif
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
std::string	ODEV_NODE::MakeDividingLine(void){
	std::string	strSendData;
	
	strSendData = DIVIDING_LINE;
	strSendData += "\r\n";
	return(strSendData);
}
//------------------------------------------------------------------------------------------//
std::string	ODEV_NODE::MakeByteNum(uint32 ByteNum, uint64 tTotal){
	std::string	strSendData;
	
	strSendData = Str_IntToString(ByteNum);
	strSendData += "/";
	strSendData += Str_UInt64ToString(tTotal);
	
	if (strSendData.length() < 12)
		strSendData.insert(0,12 - strSendData.length(),' ');
	
	strSendData += " Bytes: ";
	return(strSendData);
}
//------------------------------------------------------------------------------------------//
std::string	ODEV_NODE::MakeTimeNow(void){
	SYS_DateTime	dtime;
	
	dtime.Now();
	return(dtime.FormatDateTime(DEFAULT_TIMEFORMATE));
}
//------------------------------------------------------------------------------------------//
std::string	ODEV_NODE::MakeTitle(const std::string &strTitle, uint32 ByteNum, uint64 tTotal){
	std::string	strSendData;
	
	strSendData = MakeTimeNow();
	strSendData += strTitle;
	strSendData += MakeByteNum(ByteNum, tTotal);
	return(strSendData);
}
//------------------------------------------------------------------------------------------//
std::string	ODEV_NODE::MakeTxTitle(const std::string &strM, const std::string &strS, uint32 ByteNum, uint64 tTotal){
	std::string	strSendData;
	
	strSendData = MakeTimeNow();
	if (strS.length() == 0){
		strSendData += " SDT ->> ";
		strSendData += strM;
	}
	else{
		strSendData += ' ';
		strSendData += strM;
		strSendData += " ->> ";
		strSendData += strS;
	}
	strSendData += ' ';
	strSendData += MakeByteNum(ByteNum, tTotal);
	return(strSendData);
}
//------------------------------------------------------------------------------------------//
std::string	ODEV_NODE::MakeRxTitle(const std::string &strM, const std::string &strS, uint32 ByteNum, uint64 tTotal){
	std::string	strSendData;
	
	strSendData = MakeTimeNow();
	if (strS.length() == 0){
		strSendData += " SDT <<- ";
		strSendData += strM;
	}
	else{
		strSendData += ' ';
		strSendData += strM;
		strSendData += " <<- ";
		strSendData += strS;
	}
	strSendData += ' ';
	strSendData += MakeByteNum(ByteNum, tTotal);
	return(strSendData);
}
//------------------------------------------------------------------------------------------//
std::string	ODEV_NODE::MakeEchoTitle(const std::string &strM, uint32 ByteNum, uint64 tTotal){
	std::string	strSendData;
	
	strSendData = MakeTimeNow();
	strSendData += " SDT <-> ";
	strSendData += strM;
	strSendData += ' ';
	strSendData += MakeByteNum(ByteNum, tTotal);
	return(strSendData);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
void ODEV_NODE::WriteChar(uint8 charData,const std::string &rColor,COLEnforcePrint blEP,G_LOCK_VAILD blLock){
	std::string	strTemp;
	
	strTemp = charData;
	WriteToStr(strTemp,rColor,blEP,blLock);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//

