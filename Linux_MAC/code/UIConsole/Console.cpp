/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: Console.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.01.24
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "Console.h"
#include "Comm_Convert.h"
#include "ODEV_SDOUT.h"
#include "BIC.h"
//------------------------------------------------------------------------------------------//
void CON_LineEdit::Init(void){
	cgEditContent = "";
	cgEditCur = 0;
}
//------------------------------------------------------------------------------------------//
uint32 CON_LineEdit::KeyLeft(void){
	if (cgEditCur > 0){
		-- cgEditCur;
		return 1;
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
uint32 CON_LineEdit::KeyRight(void){
	if (cgEditCur < cgEditContent.length()){
		++ cgEditCur;
		return 1;
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
uint32 CON_LineEdit::KeyHome(void){
	uint32	ret;
	ret = cgEditCur;
	cgEditCur = 0;
	return(ret);
}
//------------------------------------------------------------------------------------------//
uint32 CON_LineEdit::KeyEnd(void){
	uint32	ret;
	ret = (uint32)cgEditContent.length() - (uint32)cgEditCur;
	cgEditCur = (uint32)cgEditContent.length();
	return(ret);
}
//------------------------------------------------------------------------------------------//
uint32 CON_LineEdit::KeyBK(void){
	if (cgEditCur > 0){
		-- cgEditCur;
		cgEditContent.erase(cgEditCur,1);
		return 1;
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
uint32 CON_LineEdit::KeyDel(void){
	if (cgEditCur < cgEditContent.length()){
		cgEditContent.erase(cgEditCur,1);
		return 1;
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
uint32 CON_LineEdit::Insert(uint8 key){
	cgEditContent.insert(cgEditCur,(char*)&key,1);
	KeyRight();
	return(cgEditCur - 1);
}
//------------------------------------------------------------------------------------------//
uint32 CON_LineEdit::Insert(const uint8 *key,uint32 length){
	cgEditContent.insert(cgEditCur, (char*)key, length);
	cgEditCur += length;
	return(cgEditCur - 1);
}
//------------------------------------------------------------------------------------------//
void CON_LineEdit::SetData(std::string const &newData,uint32 cur){
	cgEditContent = newData;
	cgEditCur = cur;
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
void CON_LineHistory::Init(void){
	cgCount = 0;
	cgTop = 0;
	cgHead = 0;
	cgHistoryCur = 0;
}
//------------------------------------------------------------------------------------------//
int32 CON_LineHistory::SetPrior(void){
	if (cgHistoryCur == cgHead)
		return 0;
	if (cgCount < MaxRecord){
		-- cgHistoryCur;
		return 1;
	}
	if (cgCount == MaxRecord){
		if (cgHistoryCur == 0){
			cgHistoryCur = MaxRecord;
		}
		else{
			-- cgHistoryCur;
		}
	}
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 CON_LineHistory::SetNext(void){
	if (cgHistoryCur == cgTop)
		return 0;
	if (cgCount < MaxRecord){
		++ cgHistoryCur;
		return 1;
	}
	if (cgCount == MaxRecord){
		if (cgHistoryCur == MaxRecord){
			cgHistoryCur = 0;
		}
		else{
			++ cgHistoryCur;
		}
	}
	return 1;
}
//------------------------------------------------------------------------------------------//
void CON_LineHistory::Add(const std::string &newData){
	std::string	strTemp;
	int32		last;
	
	strTemp = Str_Trim(newData);
	if (strTemp.length() == 0){
		cgHistoryCur = cgTop;
		return;
	}
	if (cgCount > 0){
		if (cgTop == 0){
			last = MaxRecord;
		}
		else{
			last = cgTop - 1;
		}
		if (cgEditHistory[last] == strTemp){
			cgHistoryCur = cgTop;
			return;
		}
	}
	
	cgEditHistory[cgTop] = strTemp;
	
	if (cgCount < MaxRecord){
		++ cgCount;
		++ cgTop;
	}
	
	if (cgCount == MaxRecord){
		++ cgTop;
		++ cgHead;
		if (cgTop > MaxRecord)
			cgTop = 0;
		if (cgHead > MaxRecord)
			cgHead = 0;
	}
	cgHistoryCur = cgTop;
}
//------------------------------------------------------------------------------------------//
std::string	CON_LineHistory::GetHistory(void){
	std::string		strRet;
	int32			cur,i;
	
	cur = cgHead;
	i = 0;
	while(cur != cgTop){
		++ i;
		if (i < 100){
			strRet += ' ';
			if (i < 10)
				strRet += ' ';
		}
		strRet += Str_IntToString(i);
		strRet += ".   ";
		strRet += cgEditHistory[cur];
		strRet += "\r\n";
		++ cur;
		cur %= MaxRecord;
	}
	return(strRet);
}
//------------------------------------------------------------------------------------------//
std::string	CON_LineHistory::GetSimilar(const std::string &inputStr){
	std::string		strT,strS,strR;
	int32			cur;
	uint32          lenS,lenR,lenH;
	
	cur = cgHead;
	strS = Str_Trim(inputStr);
	strR = "";
	lenS = (uint32)strS.length();
	lenR = 0;
	if (lenS == 0)
		return("");
	while(cur != cgTop){
		lenH = (uint32)cgEditHistory[cur].length();
		if (lenS <= lenH){
			if (lenS == lenH){
				strT = cgEditHistory[cur];
			}
			else{
				strT = cgEditHistory[cur].substr(0,lenS);
			}
			if (strT == strS){
				if (lenR == 0){
					strR = cgEditHistory[cur];
					lenR = (uint32)strR.length();
				}
				else{
					if (lenR > lenH){
						strR = strR.substr(0,lenH);
						lenR = (uint32)strR.length();
					}
					if (lenR == lenH){
						strT = cgEditHistory[cur];
					}
					else {
						strT = cgEditHistory[cur].substr(0,lenR);
					}
					while(strR != strT){
						if (lenS == lenR)
							return(strR);
						strR = strR.substr(0,--lenR);
						strT = cgEditHistory[cur].substr(0,lenR);
					}
				}
			}
		}
		++ cur;
		cur %= MaxRecord;
	}
	if (strR == "")
		strR = inputStr;
	return(strR);
}
//------------------------------------------------------------------------------------------//
std::string	CON_LineHistory::GetSimilarGroup(const std::string &inputStr){
	std::string		strRet,strT,strS;
	int32			cur;
	uint32			lenS,lenH;
	
	cur = cgHead;
	strRet = "";
	strS = Str_Trim(inputStr);
	lenS = (uint32)strS.length();
	while(cur != cgTop){
		lenH = (uint32)cgEditHistory[cur].length();
		if (lenS <= lenH){
			if (lenS == lenH){
				strT = cgEditHistory[cur];
			}
			else{
				strT = cgEditHistory[cur].substr(0,lenS);
			}
			if (strS == strT){
				strRet += cgEditHistory[cur];
				strRet += "\r\n";
			}
		}
		++ cur;
		cur %= MaxRecord;
	}
	return(strRet);
}
//------------------------------------------------------------------------------------------//


//------------------------------------------------------------------------------------------//
CON_ReadInLine::CON_ReadInLine(void){
	cgCInput.Init();
	cgMainLH.Init();
	cgSecondLH.Init();
	cODevOut = nullptr;
	
	cgblDBEscMode = 0;
	
	cgNewKey.keyTime.Clear();
	cgNewKey.message = 0;
	cgNewKey.lParam = 0;
	cgNewKey.wParam = 0;
	
	cgLastKey.keyTime.Clear();
	cgLastKey.message = 0;
	cgLastKey.lParam = 0;
	cgLastKey.wParam = 0;
};
//------------------------------------------------------------------------------------------//


//------------------------------------------------------------------------------------------//
void CON_ReadInLine::KeyLeft(void){
	int32	offset;
	offset = cgCInput.KeyLeft();
	if (cODevOut != nullptr){
#ifdef CommonDefH_Unix
		cODevOut->SetCurLeft(offset);
#endif
#ifdef CommonDefH_VC
		cODevOut->SetCurFromEnd(cgCInput.GetData().length() - cgCInput.cgEditCur);
#endif
	}
}
//------------------------------------------------------------------------------------------//
void CON_ReadInLine::KeyRight(void){
	int32	offset;
	offset = cgCInput.KeyRight();
	if (cODevOut != nullptr){
#ifdef CommonDefH_Unix
		cODevOut->SetCurRight(offset);
#endif
#ifdef CommonDefH_VC
		cODevOut->SetCurFromEnd(cgCInput.GetData().length() - cgCInput.cgEditCur);
#endif
	}
}
//------------------------------------------------------------------------------------------//
void CON_ReadInLine::KeyHome(void){
	int32	offset;
	offset = cgCInput.KeyHome();
	if (cODevOut != nullptr){
#ifdef CommonDefH_Unix
		cODevOut->SetCurLeft(offset);
#endif
#ifdef CommonDefH_VC
		cODevOut->SetCurFromEnd(cgCInput.GetData().length() - cgCInput.cgEditCur);
#endif
	}
}
//------------------------------------------------------------------------------------------//
void CON_ReadInLine::KeyEnd(void){
	int32	offset;
	offset = cgCInput.KeyEnd();
	if (cODevOut != nullptr){
#ifdef CommonDefH_Unix
		cODevOut->SetCurRight(offset);
#endif
#ifdef CommonDefH_VC
		cODevOut->SetCurFromEnd(cgCInput.GetData().length() - cgCInput.cgEditCur);
#endif
	}
}
//------------------------------------------------------------------------------------------//
void CON_ReadInLine::KeyBK(void){
	if (cgCInput.KeyBK() != 0){
		if (cODevOut != nullptr){
#ifdef CommonDefH_VC
			cODevOut->DelCharFromEnd(cgCInput.GetData().length() - cgCInput.cgEditCur,1);
#endif
#ifdef CommonDefH_Unix
			cODevOut->Spin_InUse_set();
			cODevOut->SetCurLeft(cgCInput.cgEditCur + 1,G_LOCK_OFF);
			cODevOut->OutputStr(cgCInput.GetData() + " ",RICH_LIN_clDefault,G_LOCK_OFF);
			cODevOut->SetCurLeft(cgCInput.GetLength() - cgCInput.cgEditCur + 1,G_LOCK_OFF);
			cODevOut->Spin_InUse_clr();
#endif
		}
	}
}
//------------------------------------------------------------------------------------------//
void CON_ReadInLine::KeyDel(void){
	if (cgCInput.KeyDel() != 0){
		if (cODevOut != nullptr){
#ifdef CommonDefH_VC
			cODevOut->DelCharFromEnd(cgCInput.GetData().length() - cgCInput.cgEditCur,1);
#endif
#ifdef CommonDefH_Unix
			cODevOut->Spin_InUse_set();
			cODevOut->SetCurLeft(cgCInput.cgEditCur,G_LOCK_OFF);
			cODevOut->OutputStr(cgCInput.GetData() + " ",RICH_LIN_clDefault,G_LOCK_OFF);
			cODevOut->SetCurLeft(cgCInput.GetLength() - cgCInput.cgEditCur + 1,G_LOCK_OFF);
			cODevOut->Spin_InUse_clr();
#endif
		}
	}
}
//------------------------------------------------------------------------------------------//
void CON_ReadInLine::KeyUp(void){
	if (cgCLH->SetPrior() != 0){
		if (cODevOut != nullptr){
#ifdef CommonDefH_VC
			cODevOut->Spin_InUse_set();
			cODevOut->DelCharFromEnd(0,cgCInput.GetData().length(),G_LOCK_OFF);
			cgCInput.SetData(cgCLH->GetData(),(uint32)cgCLH->GetData().length());
			cODevOut->OutputStr(cgCInput.GetData(),RICH_CF_clBlack,G_LOCK_OFF);
			cODevOut->Spin_InUse_clr();
#endif
#ifdef CommonDefH_Unix
			std::string		strTemp;
			strTemp = "";
			strTemp.insert(0,cgCInput.GetLength(),' ');
			cODevOut->Spin_InUse_set();
			cODevOut->SetCurLeft(cgCInput.cgEditCur,G_LOCK_OFF);
			cODevOut->OutputStr(strTemp,RICH_LIN_clDefault,G_LOCK_OFF);
			cODevOut->SetCurLeft(cgCInput.GetLength(),G_LOCK_OFF);
			cgCInput.SetData(cgCLH->GetData(),(uint32)cgCLH->GetData().length());
			cODevOut->OutputStr(cgCInput.GetData(),RICH_LIN_clDefault,G_LOCK_OFF);
			cODevOut->Spin_InUse_clr();
#endif
		}
	}
}
//------------------------------------------------------------------------------------------//
void CON_ReadInLine::KeyDown(void){
	if (cgCLH->SetNext() != 0){
		if (cODevOut != nullptr){
#ifdef CommonDefH_VC
			cODevOut->Spin_InUse_set();
			cODevOut->DelCharFromEnd(0,cgCInput.GetData().length(),G_LOCK_OFF);
			cgCInput.SetData(cgCLH->GetData(),(uint32)cgCLH->GetData().length());
			cODevOut->OutputStr(cgCInput.GetData(),RICH_CF_clBlack,G_LOCK_OFF);
			cODevOut->Spin_InUse_clr();
#endif
#ifdef CommonDefH_Unix
			std::string		strTemp;
			strTemp = "";
			strTemp.insert(0,cgCInput.GetLength(),' ');
			cODevOut->Spin_InUse_set();
			cODevOut->SetCurLeft(cgCInput.cgEditCur,G_LOCK_OFF);
			cODevOut->OutputStr(strTemp,RICH_LIN_clDefault,G_LOCK_OFF);
			cODevOut->SetCurLeft(cgCInput.GetLength(),G_LOCK_OFF);
			cgCInput.SetData(cgCLH->GetData(),(uint32)cgCLH->GetData().length());
			cODevOut->OutputStr(cgCInput.GetData(),RICH_LIN_clDefault,G_LOCK_OFF);
			cODevOut->Spin_InUse_clr();
#endif
		}
	}
}
//------------------------------------------------------------------------------------------//
void CON_ReadInLine::KeyReturn(void){
	cgBICPAR->charSBUF->Clean();
	cgBICPAR->retCommand = "";
	cgBICPAR->charSBUF->PutInASCII(cgCInput.GetData() + "\r",G_ESCAPE_OFF);
	
	if ((cgBICPAR->blInOnlineMode != 0) || (cgCInput.GetData().length() > 1))
		cgCLH->Add(cgCInput.GetData());
	
	cgCInput.Init();
	if (cODevOut != nullptr)
		cODevOut->OutputStr("\r\n",RICH_LIN_clDefault);
}
//------------------------------------------------------------------------------------------//
void CON_ReadInLine::KeyEscape(void){
	cgBICPAR->charSBUF->PutInHEX("1b");
}
//------------------------------------------------------------------------------------------//
void CON_ReadInLine::KeyTab(void){
	std::string		newData;
	if (cODevOut != nullptr){
#ifdef CommonDefH_VC
		cODevOut->Spin_InUse_set();
		cODevOut->DelCharFromEnd(0,cgCInput.GetData().length(),G_LOCK_OFF);
		newData = cgCLH->GetSimilar(cgCInput.GetData());
		cgCInput.SetData(newData,(uint32)newData.length());
		cODevOut->OutputStr(cgCInput.GetData(),RICH_CF_clBlack,G_LOCK_OFF);
		cODevOut->Spin_InUse_clr();
#endif
#ifdef CommonDefH_Unix
		std::string		strTemp;
		strTemp = "";
		strTemp.insert(0,cgCInput.GetLength(),' ');
		cODevOut->Spin_InUse_set();
		cODevOut->SetCurLeft(cgCInput.cgEditCur,G_LOCK_OFF);
		cODevOut->OutputStr(strTemp,RICH_LIN_clDefault,G_LOCK_OFF);
		cODevOut->SetCurLeft(cgCInput.GetLength(),G_LOCK_OFF);
		newData = cgCLH->GetSimilar(cgCInput.GetData());
		cgCInput.SetData(newData,(uint32)newData.length());
		cODevOut->OutputStr(cgCInput.GetData(),RICH_LIN_clDefault,G_LOCK_OFF);
		cODevOut->Spin_InUse_clr();
#endif
	}
}
//------------------------------------------------------------------------------------------//
void CON_ReadInLine::KeyDBTab(void){
	std::string		newData;
	newData = "\r\n";
	newData += cgCLH->GetSimilarGroup(cgCInput.GetData());
	if (newData == "\r\n")
		return;
	if (cODevOut != nullptr){
#ifdef CommonDefH_VC
		cODevOut->Spin_InUse_set();
		cODevOut->DelCharFromEnd(0,cgCInput.GetData().length(),G_LOCK_OFF);
		cODevOut->OutputStr(newData,RICH_CF_clPurple,G_LOCK_OFF);
		cODevOut->OutputStrN(">" + cgCInput.GetData(),RICH_CF_clBlack,G_LOCK_OFF);
		cODevOut->Spin_InUse_clr();
#endif
#ifdef CommonDefH_Unix
		std::string		strTemp;
		strTemp = "";
		strTemp.insert(0,cgCInput.GetLength(),' ');
		cODevOut->Spin_InUse_set();
		cODevOut->SetCurLeft(cgCInput.cgEditCur,G_LOCK_OFF);
		cODevOut->OutputStr(strTemp,RICH_LIN_clCyan,G_LOCK_OFF);
		cODevOut->SetCurLeft(cgCInput.GetLength(),G_LOCK_OFF);
		cODevOut->OutputStr(newData,RICH_LIN_clCyan,G_LOCK_OFF);
		cODevOut->OutputStrN(">" + cgCInput.GetData(),RICH_LIN_clDefault,G_LOCK_OFF);
		cODevOut->Spin_InUse_clr();
#endif
	}
}
//------------------------------------------------------------------------------------------//
int32 CON_ReadInLine::InsertChar(uint8 key){
	if (cgBICPAR->blInPressKeyMode != 0){
		cgBICPAR->charSBUF->Put(&key,1);
		return 1;
	}
	if ((key >= 32) && (key <= 126)){
		cgCInput.Insert(key);
		if (cODevOut != nullptr){
#ifdef CommonDefH_VC
			std::string	text;
			text = key;
			cODevOut->InsterCharFromEnd(cgCInput.GetData().length() - cgCInput.cgEditCur, text);
#endif
#ifdef CommonDefH_Unix
			cODevOut->Spin_InUse_set();
			cODevOut->SetCurLeft(cgCInput.cgEditCur - 1,G_LOCK_OFF);
			cODevOut->OutputStr(cgCInput.GetData(),RICH_LIN_clDefault,G_LOCK_OFF);
			cODevOut->SetCurLeft(cgCInput.GetLength() - cgCInput.cgEditCur,G_LOCK_OFF);
			cODevOut->Spin_InUse_clr();
#endif
		}
		return 1;
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
int32 CON_ReadInLine::InsertChar(const uint8 *key,uint32 length){
	cgCInput.Insert(key,length);
	if (cODevOut != nullptr){
#ifdef CommonDefH_VC
		std::string	text;
		text = Str_CharToASCIIStr(key,length,G_ESCAPE_OFF);
		cODevOut->InsterCharFromEnd(cgCInput.GetData().length() - cgCInput.cgEditCur, text);
#endif
#ifdef CommonDefH_Unix
		cODevOut->Spin_InUse_set();
		cODevOut->SetCurLeft(cgCInput.cgEditCur - 1,G_LOCK_OFF);
		cODevOut->OutputStr(cgCInput.GetData(), RICH_LIN_clDefault,G_LOCK_OFF);
		cODevOut->SetCurLeft(cgCInput.GetLength() - cgCInput.cgEditCur,G_LOCK_OFF);
		cODevOut->Spin_InUse_clr();
#endif
		return 1;
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
int32 CON_ReadInLine::DoContrl(void){
	uint8	key;
	
	key = cgNewKey.wParam;
	
	if (cgBICPAR->blInPressKeyMode != 0){
		if (key == VK_ESCAPE){
			key = 0x1b;
		}
		else if (key != ' '){
			key = '\r';
		}
		cgBICPAR->charSBUF->Put(&key,1);
		return 1;
	}
	if ((cgblDBEscMode != 0) && (key == VK_RETURN) && (cODevOut != nullptr)){//change display mode
		cODevOut->Spin_InUse_set();
		if (cgBICPAR->blDisplayAuto != 0){
			cgBICPAR->blDisplayAuto = 0;
			cODevOut->Enable();
		}
		else{
			cgBICPAR->blDisplayAuto = 1;
		}
		cODevOut->Spin_InUse_clr();
		cgblDBEscMode = 0;
		return 1;
	}
	if (cgBICPAR->blInOnlineMode != 0){
		if (cgblDBEscMode != 0){
#ifdef CommonDefH_VC
			if ((key >= 0x41) && (key <= 0x5A)){
				key -= 0x40;
			}
			else if (key == VK_OEM_4){//[
				key = 0x80;
			}
			else if (key == VK_OEM_5){//'\'
				key = 0x1c;
			}
			else if (key == VK_OEM_6){//]
				key = 0x1d;
			}
			else if (key == VK_ESCAPE){
				key = 0x1b;
			}
			else if (key == VK_OEM_3){//~`,quit onlinemode
				key = 0x1b;
			}
			else{
				cgblDBEscMode = 0;
				return 1;
			}
#endif
#ifdef CommonDefH_Unix
			if (key == 0x5b){//[
				key = 0x80;
			}
			else if ((key >= 0x40) && (key <= 0x5F)){
				key -= 0x40;
			}
			else if ((key == 0x60) || (key == VK_ESCAPE)){//~`,quit onlinemode
				key = 0x1b;
			}
			else{
				cgblDBEscMode = 0;
				return 1;
			}
#endif
			cgBICPAR->charSBUF->Put(&key,1);
			if (key != 0x1b){
				std::string strT;
				if (key == 0x80){
					key = 0x1b;
				}
				key += 0x40;
				strT = '^';
				strT += key;
				if (cODevOut != nullptr)
					cODevOut->OutputStr(strT,RICH_CF_clBlack);
			}
			cgblDBEscMode = 0;
			return 1;
		}
	}
	cgblDBEscMode = 0;
	
	switch(key){
		case VK_UP:
			KeyUp();
			return 1;
		case VK_DOWN:
			KeyDown();
			return 1;
		case VK_LEFT:
			if ((cgNewKey.lParam & VK_COCLK) == 0){
				KeyLeft();
			}
			else{
				KeyHome();
			}
			return 1;
		case VK_RIGHT:
			if ((cgNewKey.lParam & VK_COCLK) == 0){
				KeyRight();
			}
			else{
				KeyEnd();
			}
			return 1;
		case VK_HOME:
			KeyHome();
			return 1;
		case VK_END:
			KeyEnd();
			return 1;
		case VK_PRIOR:
		case VK_NEXT:
			return 1;
		case VK_TAB:
			if ((cgNewKey.lParam & VK_DBCLK) > 0){
				KeyDBTab();
			}
			else{
				KeyTab();
			}
			return 1;
		case VK_DELETE:
			KeyDel();
			return 1;
		case VK_BACK:
			KeyBK();
			return 1;
		case VK_RETURN:
			KeyReturn();
			return 2;
		case VK_CONTROL:
			if ((cgNewKey.lParam & VK_DBCLK) > 0){
				cgblDBEscMode = 1;
				return 1;
			}
			if ((cgNewKey.lParam & VK_COCLK) > 0){
				cgNewKey.keyTime.Clear();
				cgNewKey.message = 0;
				cgNewKey.lParam = 0;
				cgNewKey.wParam = 0;
			}
			return 2;
		case VK_ESCAPE:
			if ((cgNewKey.lParam & VK_DBCLK) > 0)
				cgblDBEscMode = 1;
			if ((cgNewKey.lParam & VK_COCLK) > 0){
				KeyEscape();
				cgNewKey.keyTime.Clear();
				cgNewKey.message = 0;
				cgNewKey.lParam = 0;
				cgNewKey.wParam = 0;
			}
			return 1;
		default:;
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
int32 CON_ReadInLine::AKey(const MSG *pMsg){
	int32	renKeyType,ret;
	if (cgBICPAR->blUseSecondLH != 0){
		SwitchSecondLH();
	}
	else{
		SwitchMainLH();
	}
	renKeyType = AnalyzeKeyStream(pMsg);
	CheckPressStatus();
	ret = 0;
	if (renKeyType == WM_KEYDOWN){
		ret = DoContrl();
	}
	else if (renKeyType == WM_CHAR){
		ret = InsertChar(cgNewKey.wParam);
	}
	if ((ret == 1) && (cODevOut != nullptr)){
		cODevOut->Spin_InUse_set();
		if (cgBICPAR->blDisplayAuto != 0)
			cODevOut->Disable();
		cODevOut->Spin_InUse_clr();
	}
	return(ret);
}
//------------------------------------------------------------------------------------------//
void CON_ReadInLine::CheckPressStatus(void){
	double			tTime;
	SYS_DateTime	dT;
	
	if ((cgNewKey.message == WM_KEYDOWN) && (cgLastKey.message == WM_KEYDOWN) && (cgLastKey.wParam == cgNewKey.wParam)){
		dT = cgNewKey.keyTime - cgLastKey.keyTime;
		tTime = dT.GetSec();
		if ((cgLastKey.lParam & (VK_DBCLK | VK_COCLK)) == 0){
			if (tTime < 0.1){
				cgNewKey.lParam |= VK_COCLK;
			}
			else if (tTime < 0.2){
				cgNewKey.lParam |= VK_DBCLK;
			}
		}
	}
	if ((cgNewKey.message == WM_KEYDOWN) || (cgNewKey.message == WM_CHAR)){
		cgLastKey.keyTime = cgNewKey.keyTime;
		cgLastKey.message = cgNewKey.message;
		cgLastKey.lParam = cgNewKey.lParam;
		cgLastKey.wParam = cgNewKey.wParam;
	}
}
//------------------------------------------------------------------------------------------//
int32 CON_ReadInLine::AnalyzeKeyStream(const MSG *pMsg){
	SYS_DateTime	dT;
	double			dTsec;
#ifdef CommonDefH_VC
	cgNewKey.keyTime.Now();
	cgNewKey.message = pMsg->message;
	cgNewKey.lParam = 0;
	cgNewKey.wParam = pMsg->wParam;
	
	dT = cgNewKey.keyTime - cgLastKey.keyTime;
	dTsec = dT.GetSec();
	if (dTsec > 2.0)
		cgblDBEscMode = 0;
	
	return(cgNewKey.message);
#endif
#ifdef CommonDefH_Unix
	char key1,key2,key3,key4;
	
	cgNewKey.keyTime.Now();
	cgNewKey.message = WM_NOKEY;
	cgNewKey.lParam = 0;
	cgNewKey.wParam = 0;
	
	dT = cgNewKey.keyTime - cgLastKey.keyTime;
	dTsec = dT.GetSec();
	if ((cgblDBEscMode != 0) && (dTsec > 0.8))
		cgblDBEscMode = 0;
	
	key1 = getchar();
	
	if (key1 == EOF)
		return(cgNewKey.message);
	
	if ((key1 >= 0x20) && (key1 <= 0x7e)){
		cgNewKey.message = WM_CHAR;
		if ((cgBICPAR->blInOnlineMode != 0) && (cgblDBEscMode != 0)){
			cgNewKey.message = WM_KEYDOWN;
			if((key1 >= 0x61) && (key1 <= 0x7a))
				key1 -= 0x20;
		}
		cgNewKey.wParam = key1;
		return(cgNewKey.message);
	}
	
	cgNewKey.message = WM_KEYDOWN;
	if (key1 == 9){
		cgNewKey.wParam = VK_TAB;
		return(cgNewKey.message);
	}
	if (key1 == 10){
		cgNewKey.wParam = VK_RETURN;
		return(cgNewKey.message);
	}
	if (key1 == 127){
		cgNewKey.wParam = VK_BACK;
		return(cgNewKey.message);
	}
	if (key1 == 27){
		key2 = getchar();
		key3 = getchar();
		if (key2 == EOF){
			cgNewKey.wParam = VK_ESCAPE;
			return(cgNewKey.message);
		}
		if (key2 == 91){
			if (key3 == 65){
				cgNewKey.wParam = VK_UP;
				return(cgNewKey.message);
			}
			if (key3 == 66){
				cgNewKey.wParam = VK_DOWN;
				return(cgNewKey.message);
			}
			if (key3 == 67){
				cgNewKey.wParam = VK_RIGHT;
				return(cgNewKey.message);
			}
			if (key3 == 68){
				cgNewKey.wParam = VK_LEFT;
				return(cgNewKey.message);
			}
			if (key3 == 50){
				key4 = getchar();
				if (key4 == 126){
					cgNewKey.wParam = VK_INSERT;
				}
				else{
					cgNewKey.wParam = 0;
				}
				return(cgNewKey.message);
			}
			if (key3 == 51){//del
				key4 = getchar();
				if (key4 == 126){
					cgNewKey.wParam = VK_DELETE;
				}
				else{
					cgNewKey.wParam = 0;
				}
				return(cgNewKey.message);
			}
			if (key3 == 53){//PgUp
				key4 = getchar();
				if (key4 == 126){
					cgNewKey.wParam = VK_PRIOR;
				}
				else{
					cgNewKey.wParam = 0;
				}
				return(cgNewKey.message);
			}
			if (key3 == 54){//PgDn
				key4 = getchar();
				if (key4 == 126){
					cgNewKey.wParam = VK_NEXT;
				}
				else{
					cgNewKey.wParam = 0;
				}
				return(cgNewKey.message);
			}
		}
		if (key2 == 79){
			if (key3 == 72){//home
				cgNewKey.wParam = VK_HOME;
				return(cgNewKey.message);
			}
			if (key3 == 70){
				cgNewKey.wParam = VK_END;
				return(cgNewKey.message);
			}
		}
	}
	cgNewKey.wParam = 0;
	return(cgNewKey.message);
#endif
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
