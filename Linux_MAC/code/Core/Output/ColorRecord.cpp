/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: ColorRecord.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2013.7.5
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "ColorRecord.h"
#include "Comm_Convert.h"
//------------------------------------------------------------------------------------------//
void COLSTRING::ChageType(COLType tCOLType,G_LOCK_VAILD blLock){
	Spin_InUse_set(blLock);
	cgCOLType = tCOLType;
	COLSTRING::Clean(G_LOCK_OFF);
	Spin_InUse_clr(blLock);
};
//------------------------------------------------------------------------------------------//
uint64 COLSTRING::GetUnreadLength(G_LOCK_VAILD blLock){
	uint64	ret;
	Spin_InUse_set(blLock);
	ret = UnreadLength;
	Spin_InUse_clr(blLock);
	return(ret);
}
//------------------------------------------------------------------------------------------//
void COLSTRING::Clean(G_LOCK_VAILD blLock){
	Spin_InUse_set(blLock);
	LastChar = "";
	unreadContent = "";
	UnreadLength = 0;
	Spin_InUse_clr(blLock);
}
//------------------------------------------------------------------------------------------//
void COLSTRING::WriteToStr(const std::string &strContent,const std::string &rColor,COLEnforcePrint blEP,G_LOCK_VAILD blLock){
	Spin_InUse_set(blLock);
	if ((CheckblEnabled() != 0) || (blEP != COL_EP_NO)){
		if (UnreadLength < BUF_MAX_SIZE){
			UnreadLength += strContent.length();
			UpdateLastChar(strContent);
			if (cgCOLType == COLType_COL){
				unreadContent += ",";
				unreadContent += rColor;
				unreadContent += ",";
				unreadContent += Str_ASCIIToHEX(strContent,G_ESCAPE_OFF);
			}
			else if(cgCOLType == COLType_TXT){
				unreadContent += strContent;
			}
		}
	}
	Spin_InUse_clr(blLock);
}
//------------------------------------------------------------------------------------------//
void COLSTRING::WriteToStrN(const std::string &strContent,const std::string &rColor,COLEnforcePrint blEP,G_LOCK_VAILD blLock){
	std::string strLastChar;
	if ((CheckblEnabled() != 0) || (blEP != COL_EP_NO)){
		Spin_InUse_set(blLock);
		strLastChar = GetLastChar();
		if (UnreadLength < BUF_MAX_SIZE){
			UnreadLength += (strLastChar.length() + strContent.length());
			if (strContent.length() > 0)
				UpdateLastChar(strContent);
			if (cgCOLType == COLType_COL){
				unreadContent += ",";
				unreadContent += rColor;
				unreadContent += ",";
				unreadContent += Str_ASCIIToHEX(strLastChar + strContent,G_ESCAPE_OFF);
			}
			else if(cgCOLType == COLType_TXT){
				unreadContent += strLastChar;
				unreadContent += strContent;
			}
		}
		Spin_InUse_clr(blLock);
	}
}
//------------------------------------------------------------------------------------------//
const std::string& COLSTRING::ReadStr(std::string *retStr,G_LOCK_VAILD blLock){
	Spin_InUse_set(blLock);
	*retStr = unreadContent;
	unreadContent = "";
	UnreadLength = 0;
	Spin_InUse_clr(blLock);
	return(*retStr);
}
//------------------------------------------------------------------------------------------//
void COLSTRING::UpdateLastChar(const std::string &inputStr){
	std::string		strT;
	
	if (inputStr.length() == 0)
		return;
	if (inputStr.length() == 1){
		if (inputStr == "\r" || inputStr == "\n"){
			LastChar = "";
		}
		else{
			LastChar = "\r\n";
		}
	}
	if (inputStr.length() > 1){
		strT = inputStr.substr(inputStr.length() - 2,2);
		strT = Str_Replace(strT,"\n","\r");
		if (strT[1] == '\r'){
			LastChar = "";
		}
		else{
			LastChar = "\r\n";
		}
	}
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
