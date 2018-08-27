//
//  Console.cpp
//  SDT
//
//  Created by Leif Wen on 05/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
#include "Console.h"
//------------------------------------------------------------------------------------------//
#ifdef Console_h
#include "DS_STRING.h"
#include "SYS_File.h"
#include "BIC_Define.h"
#include "ODEV_STDOUT.h"
//------------------------------------------------------------------------------------------//
#ifdef  CommonDefH_Unix
#include <fcntl.h>
#include <termios.h>
#endif
//------------------------------------------------------------------------------------------//
Line_Edit::Line_Edit(void){
	Init();
};
//------------------------------------------------------------------------------------------//
void Line_Edit::Init(void){
	cgEditContent = "";
	cgEditCur = 0;
}
//------------------------------------------------------------------------------------------//
uint32 Line_Edit::KeyLeft(void){
	if (cgEditCur > 0){
		-- cgEditCur;
		return 1;
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
uint32 Line_Edit::KeyRight(void){
	if (cgEditCur < cgEditContent.length()){
		++ cgEditCur;
		return 1;
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
uint32 Line_Edit::KeyHome(void){
	uint32	ret;
	ret = cgEditCur;
	cgEditCur = 0;
	return(ret);
}
//------------------------------------------------------------------------------------------//
uint32 Line_Edit::KeyEnd(void){
	uint32	ret;
	ret = (uint32)cgEditContent.length() - (uint32)cgEditCur;
	cgEditCur = (uint32)cgEditContent.length();
	return(ret);
}
//------------------------------------------------------------------------------------------//
uint32 Line_Edit::KeyBK(void){
	if (cgEditCur > 0){
		-- cgEditCur;
		cgEditContent.erase(cgEditCur,1);
		return 1;
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
uint32 Line_Edit::KeyDel(void){
	if (cgEditCur < cgEditContent.length()){
		cgEditContent.erase(cgEditCur,1);
		return 1;
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
uint32 Line_Edit::Insert(uint8 key){
	cgEditContent.insert(cgEditCur,(char*)&key,1);
	KeyRight();
	return(cgEditCur - 1);
}
//------------------------------------------------------------------------------------------//
uint32 Line_Edit::Insert(const uint8* key,uint32 length){
	cgEditContent.insert(cgEditCur, (char*)key, length);
	cgEditCur += length;
	return(cgEditCur - 1);
}
//------------------------------------------------------------------------------------------//
const STDSTR& Line_Edit::SetData(const STDSTR& newInput){
	cgEditContent = newInput;
	cgEditCur = (uint32)newInput.length();
	return(newInput);
}
//------------------------------------------------------------------------------------------//
const STDSTR& Line_Edit::GetData(void)const{
	return(cgEditContent);
};
//------------------------------------------------------------------------------------------//
uint32 Line_Edit::GetLength(void)const{
	return((uint32)cgEditContent.length());
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
Line_History::Line_History(void){
	Init();
};
//------------------------------------------------------------------------------------------//
void Line_History::ClearHistory(void){
	Init();
};
//------------------------------------------------------------------------------------------//
const STDSTR& Line_History::GetData(void)const{
	return(cgEditHistory[cgHistoryCur]);
};
//------------------------------------------------------------------------------------------//
void Line_History::Init(void){
	cgCount = 0;
	cgTop = 0;
	cgHead = 0;
	cgHistoryCur = 0;
}
//------------------------------------------------------------------------------------------//
bool32 Line_History::SetPrior(void){
	if (cgHistoryCur == cgHead)
		return G_FALSE;
	if (cgCount < MaxRecord){
		-- cgHistoryCur;
		return G_TRUE;
	}
	if (cgCount == MaxRecord){
		if (cgHistoryCur == 0){
			cgHistoryCur = MaxRecord;
		}
		else{
			-- cgHistoryCur;
		}
	}
	return G_TRUE;
}
//------------------------------------------------------------------------------------------//
bool32 Line_History::SetNext(void){
	if (cgHistoryCur == cgTop)
		return G_FALSE;
	if (cgCount < MaxRecord){
		++ cgHistoryCur;
		return G_TRUE;
	}
	if (cgCount == MaxRecord){
		if (cgHistoryCur == MaxRecord){
			cgHistoryCur = 0;
		}
		else{
			++ cgHistoryCur;
		}
	}
	return G_TRUE;
}
//------------------------------------------------------------------------------------------//
void Line_History::Add(const STDSTR& newInput){
	STDSTR	strTemp;
	int32	last;
	
	Str_Trim(&strTemp,newInput);
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
STDSTR Line_History::GetHistory(void){
	STDSTR		strRet;
	int32		cur,i;
	
	cur = cgHead;
	i = 0;
	while(cur != cgTop){
		++ i;
		if (i < 100){
			strRet += ' ';
			if (i < 10)
				strRet += ' ';
		}
		strRet += Str_ToStr(i);
		strRet += ".   ";
		strRet += cgEditHistory[cur];
		strRet += "\n";
		++ cur;
		cur %= MaxRecord;
	}
	return(strRet);
}
//------------------------------------------------------------------------------------------//
STDSTR Line_History::GetSimilar(const STDSTR& inputStr){
	STDSTR		strT,strS,strR;
	int32		cur;
	uint32		lenS,lenR,lenH;
	
	cur = cgHead;
	Str_Trim(&strS,inputStr);
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
STDSTR Line_History::GetSimilarGroup(const STDSTR& inputStr){
	STDSTR		strRet,strT,strS;
	int32		cur;
	uint32		lenS,lenH;
	
	cur = cgHead;
	strRet = "";
	Str_Trim(&strS,inputStr);
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
				strRet += "\n";
			}
		}
		++ cur;
		cur %= MaxRecord;
	}
	return(strRet);
}
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
CONSOLE::CONSOLE(uint32 size) : BASE_FLAG(){
	cgCInput.Init();
	cgHistoryMain.Init();
	cgHistoryAux.Init();

	cgOdevSTDOUT = nullptr;
	cgBICENV = nullptr;
	
	cgLastKey.time.Clear();
	cgLastKey.message = 0;
	cgLastKey.lParam = 0;
	cgLastKey.wParam = 0;
	
	cgCMDSBUF.InitSize(size);
	
	SwitchToMain();
	bicThread.ThreadInit(this, &CONSOLE::BICThreadFun, "CONSOLE_BIC");
	SetblDisplayAuto();
};
//------------------------------------------------------------------------------------------//
CONSOLE::~CONSOLE(void){
	SetExit();
	bicThread.ThreadStop();
};
//------------------------------------------------------------------------------------------//
CONSOLE& CONSOLE::Init(const ODEV_STDOUT* oDevSTDOUT){
	cgOdevSTDOUT = (ODEV_STDOUT*)oDevSTDOUT;
	return(*this);
};
//------------------------------------------------------------------------------------------//
void	CONSOLE::SetInPressAnyKeyMode	(void)		{SetSFlag(CON_blInPressAnyKeyMode);};
void	CONSOLE::ClrInPressAnyKeyMode	(void)		{ClrSFlag(CON_blInPressAnyKeyMode);};
void	CONSOLE::SetInOnlineMode		(void)		{SetSFlag(CON_blInOnlineMode);};
void	CONSOLE::ClrInOnlineMode		(void)		{ClrSFlag(CON_blInOnlineMode);};
void	CONSOLE::SetblDisplayAuto		(void)		{SetSFlag(CON_blDisplayAuto);};
void	CONSOLE::ClrblDisplayAuto		(void)		{ClrSFlag(CON_blDisplayAuto);};
bool32	CONSOLE::IsDisplayAuto			(void)const	{return(CheckSFlag(CON_blDisplayAuto));};
void	CONSOLE::SetExit				(void)		{SetSFlag(CON_blExit);};
//------------------------------------------------------------------------------------------//
bool32 CONSOLE::LoadDefault(const STDSTR& filename){
	STDSTR			strResult;
	std::fstream	fileStream;
	
	cgCInput.Init();
	cgHistoryMain.Init();
	cgHistoryAux.Init();
	
	if (CFS_CheckFile(filename) < 1)
		return G_FALSE;
	
	fileStream.open(filename.c_str(),std::ios::in|std::ios::binary);
	do{
		strResult = "";
		std::getline(fileStream, strResult, '\n');
		if ((strResult.length() > 0) && (strResult[0] != '#'))
			AddToMain(strResult);
	}while(!fileStream.eof());
	fileStream.close();
	return G_TRUE;
}
//------------------------------------------------------------------------------------------//
void CONSOLE::AddToMain(const STDSTR& newInput){
	cgHistoryMain.Add(newInput);
};
//------------------------------------------------------------------------------------------//
void CONSOLE::SwitchToMain(void){
	cgHistory = &cgHistoryMain;
};
//------------------------------------------------------------------------------------------//
void CONSOLE::SwitchToAux(void){
	cgHistory = &cgHistoryAux;
};
//------------------------------------------------------------------------------------------//
bool32 CONSOLE::Paste(uint8* key, uint32 length){
	return(InsertChar(key, length));
};
//------------------------------------------------------------------------------------------//
void CONSOLE::KeyLeft(void){
	int32	offset;
	offset = cgCInput.KeyLeft();
#ifdef ODEV_STDOUT_h
	if (cgOdevSTDOUT != nullptr){
#ifdef CommonDefH_VC
		cgOdevSTDOUT->SetCurFromEnd(cgCInput.GetData().length() - cgCInput.cgEditCur);
#endif
#ifdef CommonDefH_Unix
		cgOdevSTDOUT->SetCurLeft(offset);
#endif
	}
#endif
}
//------------------------------------------------------------------------------------------//
void CONSOLE::KeyRight(void){
	int32	offset;
	offset = cgCInput.KeyRight();
#ifdef ODEV_STDOUT_h
	if (cgOdevSTDOUT != nullptr){
#ifdef CommonDefH_VC
		cgOdevSTDOUT->SetCurFromEnd(cgCInput.GetData().length() - cgCInput.cgEditCur);
#endif
#ifdef CommonDefH_Unix
		cgOdevSTDOUT->SetCurRight(offset);
#endif
	}
#endif
}
//------------------------------------------------------------------------------------------//
void CONSOLE::KeyHome(void){
	int32	offset;
	offset = cgCInput.KeyHome();
#ifdef ODEV_STDOUT_h
	if (cgOdevSTDOUT != nullptr){
#ifdef CommonDefH_VC
		cgOdevSTDOUT->SetCurFromEnd(cgCInput.GetData().length() - cgCInput.cgEditCur);
#endif
#ifdef CommonDefH_Unix
		cgOdevSTDOUT->SetCurLeft(offset);
#endif
	}
#endif
}
//------------------------------------------------------------------------------------------//
void CONSOLE::KeyEnd(void){
	int32	offset;
	offset = cgCInput.KeyEnd();
#ifdef ODEV_STDOUT_h
	if (cgOdevSTDOUT != nullptr){
#ifdef CommonDefH_VC
		cgOdevSTDOUT->SetCurFromEnd(cgCInput.GetData().length() - cgCInput.cgEditCur);
#endif
#ifdef CommonDefH_Unix
		cgOdevSTDOUT->SetCurRight(offset);
#endif
	}
#endif
}
//------------------------------------------------------------------------------------------//
void CONSOLE::KeyBK(void){
	if (cgCInput.KeyBK() != 0){
#ifdef ODEV_STDOUT_h
		if (cgOdevSTDOUT != nullptr){
#ifdef CommonDefH_VC
			cgOdevSTDOUT->DelCharFromEnd(cgCInput.GetData().length() - cgCInput.cgEditCur + 1,1);
#endif
#ifdef CommonDefH_Unix
			cgOdevSTDOUT->BKChar();
#endif
		}
#endif
	}
}
//------------------------------------------------------------------------------------------//
void CONSOLE::KeyDel(void){
	if (cgCInput.KeyDel() != 0){
#ifdef ODEV_STDOUT_h
		if (cgOdevSTDOUT != nullptr){
#ifdef CommonDefH_VC
			cgOdevSTDOUT->DelCharFromEnd(cgCInput.GetData().length() - cgCInput.cgEditCur + 1,1);
#endif
#ifdef CommonDefH_Unix
			cgOdevSTDOUT->DelChar();
#endif
		}
#endif
	}
}
//------------------------------------------------------------------------------------------//
void CONSOLE::KeyUp(void){
	if (cgHistory->SetPrior()){
#ifdef ODEV_STDOUT_h
		if (cgOdevSTDOUT != nullptr){
#ifdef CommonDefH_VC
			uint32 offset;
			offset = cgCInput.GetData().length();
			cgOdevSTDOUT->RewriteCharFromEnd(offset,COL_clDefault,cgCInput.SetData(cgHistory->GetData()));
#endif
#ifdef CommonDefH_Unix
			uint32 offset;
			offset = cgCInput.cgEditCur;
			cgOdevSTDOUT->RewriteChar(offset,COL_clDefault,cgCInput.SetData(cgHistory->GetData()));
#endif
		}
#endif
	}
}
//------------------------------------------------------------------------------------------//
void CONSOLE::KeyDown(void){
	if (cgHistory->SetNext()){
#ifdef ODEV_STDOUT_h
		if (cgOdevSTDOUT != nullptr){
#ifdef CommonDefH_VC
			uint32 offset;
			offset = cgCInput.GetData().length();
			cgOdevSTDOUT->RewriteCharFromEnd(offset, COL_clDefault, cgCInput.SetData(cgHistory->GetData()));
#endif
#ifdef CommonDefH_Unix
			uint32 offset;
			offset = cgCInput.cgEditCur;
			cgOdevSTDOUT->RewriteChar(offset,COL_clDefault,cgCInput.SetData(cgHistory->GetData()));
#endif
		}
#endif
	}
}
//------------------------------------------------------------------------------------------//
void CONSOLE::KeyReturn(void){
	if (cgCInput.GetData() == "LETQUIT")
		SetExit();
	
	cgCMDSBUF.Put(nullptr,cgCInput.GetData() + "\n");
#ifdef ODEV_STDOUT_h
	OUTPUT_NODE::PrintStr(cgOdevSTDOUT, "\n");
#endif
	if (CheckSFlag(CON_blInOnlineMode) || (cgCInput.GetData().length() > 1))
		cgHistory->Add(cgCInput.GetData());
	
	cgCInput.Init();
}
//------------------------------------------------------------------------------------------//
void CONSOLE::KeyEscape(void){
	cgCMDSBUF.Put(nullptr,IUD('\x1b'));
}
//------------------------------------------------------------------------------------------//
void CONSOLE::KeyTab(void){
#ifdef ODEV_STDOUT_h
	uint32 offset;

	if (cgOdevSTDOUT != nullptr){
#ifdef CommonDefH_VC
		offset = cgCInput.GetData().length();
		cgOdevSTDOUT->RewriteCharFromEnd(offset, COL_clDefault, cgCInput.SetData(cgHistory->GetSimilar(cgCInput.GetData())));
#endif
#ifdef CommonDefH_Unix
		offset = cgCInput.cgEditCur;
		cgOdevSTDOUT->RewriteChar(offset,COL_clDefault,cgCInput.SetData(cgHistory->GetSimilar(cgCInput.GetData())));
#endif
	}
#endif
}
//------------------------------------------------------------------------------------------//
void CONSOLE::KeyDBTab(void){
	STDSTR	newData;
	newData = cgHistory->GetSimilarGroup(cgCInput.GetData());
	if ((newData.length() == 0) || (newData == "\n"))
		return;
#ifdef ODEV_STDOUT_h
	if (cgOdevSTDOUT != nullptr){
		*cgOdevSTDOUT<< Begin() << NL()
		<< COL_clPurple << newData
		<< COL_clDefault << ">" << cgCInput.GetData()
		<< Endl();
	}
#endif
}
//------------------------------------------------------------------------------------------//
bool32 CONSOLE::InsertChar(uint8 data){
	if (CheckSFlag(CON_blInPressAnyKeyMode)){
		cgCMDSBUF.Put(nullptr,IUD(data));
		return G_TRUE;
	}
	if ((data >= 32) && (data <= 126)){
		cgCInput.Insert(data);
#ifdef ODEV_STDOUT_h
		if (cgOdevSTDOUT != nullptr){
			if (cgCInput.GetData().length() == cgCInput.cgEditCur){
				cgOdevSTDOUT->Write(nullptr,COL_clDefault,IUD(data));
			}
			else{
			#ifdef CommonDefH_VC
				cgOdevSTDOUT->InsterCharFromEnd(cgCInput.GetData().length() - cgCInput.cgEditCur, COL_clDefault, Str_CharToASCII(&data, 1, G_ESCAPE_OFF));
			#endif
			#ifdef CommonDefH_Unix
				cgOdevSTDOUT->InsterChar(COL_clDefault,&data,1);
			#endif
			}
		}
#endif
		return G_TRUE;
	}
	return G_FALSE;
}
//------------------------------------------------------------------------------------------//
bool32 CONSOLE::InsertChar(const uint8* data,uint32 length){
	cgCInput.Insert(data,length);
	if (cgOdevSTDOUT != nullptr){

		if (cgCInput.GetData().length() == cgCInput.cgEditCur){
#ifdef ODEV_STDOUT_h
			cgOdevSTDOUT->Write(nullptr,COL_clDefault,IUD(data,length));
		}
		else{
		#ifdef CommonDefH_VC
			cgOdevSTDOUT->InsterCharFromEnd(cgCInput.GetData().length() - cgCInput.cgEditCur, COL_clDefault, Str_CharToASCII(data, length, G_ESCAPE_OFF));
		#endif
		#ifdef CommonDefH_Unix
			cgOdevSTDOUT->InsterChar(COL_clDefault,data,length);
		#endif
#endif
		}
		return G_TRUE;
	}
	return G_FALSE;
}
//------------------------------------------------------------------------------------------//
bool32 CONSOLE::DoControl(const KEYMSG* keyMsg){
	uint8	key;
	
	key = keyMsg->wParam;
	
	if (CheckSFlag(CON_blInPressAnyKeyMode)){
		if (key == VK_ESCAPE){
			key = 0x1b;
		}
		else if (key != ' '){
			key = '\n';
		}
		cgCMDSBUF.Put(nullptr,IUD(key));
		return G_TRUE;
	}
	if (CheckSFlag(CON_blInDBEscMode) && (key == VK_RETURN) && (cgOdevSTDOUT != nullptr)){//change display mode
		if (CheckSFlag(CON_blDisplayAuto)){
			ClrSFlag(CON_blDisplayAuto);
#ifdef ODEV_STDOUT_h
			cgOdevSTDOUT->PrintEnable();
#endif
		}
		else{
			SetSFlag(CON_blDisplayAuto);
		}
		ClrSFlag(CON_blInDBEscMode);
		return G_TRUE;
	}
	if (CheckSFlag(CON_blInDBEscMode) && CheckSFlag(CON_blInOnlineMode)){
		#ifdef CommonDefH_VC
		switch (key) {
			case VK_OEM_4://[
				key = 0x80;
				break;
			case VK_OEM_5://'\'
				key = 0x1c;
				break;
			case VK_OEM_6://]
				key = 0x1d;
				break;
			case VK_OEM_3://~,quit onlinemode
			case VK_ESCAPE:
				key = 0x1b;
				break;
			default:
				if ((key >= 0x41) && (key <= 0x5A)){
					key -= 0x40;
				}
				else{
					ClrSFlag(CON_blInDBEscMode);
					return G_TRUE;
				}
				break;
		}
		#endif
		#ifdef CommonDefH_Unix
		switch (key) {
			case 0x5b://[
				key = 0x80;
				break;
			case 0x60://~
			case VK_ESCAPE:
				key = 0x1b;
				break;
			default:
				if ((key >= 0x40) && (key <= 0x5F)){
					key -= 0x40;
				}
				else{
					ClrSFlag(CON_blInDBEscMode);
					return G_TRUE;
				}
				break;
		}
		#endif
		cgCMDSBUF.Put(nullptr,IUD(key));
		if (key != 0x1b){
			STDSTR strT;
			if (key == 0x80)
				key = 0x1b;
			key += 0x40;
			strT = '^';
			strT += key;
#ifdef ODEV_STDOUT_h
			if (cgOdevSTDOUT != nullptr)
				cgOdevSTDOUT->Write(nullptr,COL_clDefault,strT);
#endif
		}
		ClrSFlag(CON_blInDBEscMode);
		return G_TRUE;
	}
	
	ClrSFlag(CON_blInDBEscMode);
	
	switch(key){
		case VK_UP:
			KeyUp();
			break;
		case VK_DOWN:
			KeyDown();
			break;
		case VK_LEFT:
			if (B_ChkFLAG32(keyMsg->lParam,VK_COCLK)){
				KeyHome();
			}
			else{
				KeyLeft();
			}
			break;
		case VK_RIGHT:
			if (B_ChkFLAG32(keyMsg->lParam,VK_COCLK)){
				KeyEnd();
			}
			else{
				KeyRight();
			}
			break;
		case VK_HOME:
			KeyHome();
			break;
		case VK_END:
			KeyEnd();
			break;
		case VK_PRIOR:
		case VK_NEXT:
			break;
		case VK_TAB:
			if (B_ChkFLAG32(keyMsg->lParam,VK_DBCLK)){
				KeyDBTab();
			}
			else{
				KeyTab();
			}
			break;
		case VK_DELETE:
			KeyDel();
			break;
		case VK_BACK:
			KeyBK();
			break;
		case VK_RETURN:
			KeyReturn();
			return 2;
		case VK_CONTROL:
			if (B_ChkFLAG32(keyMsg->lParam,VK_DBCLK)){
				SetSFlag(CON_blInDBEscMode);
				break;
			}
			return 2;
		case VK_ESCAPE:
			if (B_ChkFLAG32(keyMsg->lParam,VK_DBCLK))
				SetSFlag(CON_blInDBEscMode);
			break;
		default:;
			return G_FALSE;
	}
	return G_TRUE;
}
//------------------------------------------------------------------------------------------//
KEYMSG* CONSOLE::AnalyzeKey(const MSG* keyMsg){
	DTIME	dT,newKeyTime;
	double	dTsec;
	
#ifdef CommonDefH_Unix
	MSG		*key = (MSG*)keyMsg;
	
	if ((keyMsg->message == WM_CHAR) && (keyMsg->wParam >= 0x20) && (keyMsg->wParam <= 0x7e)){
		if (CheckSFlag(CON_blInOnlineMode) && CheckSFlag(CON_blInDBEscMode)){
			key->message = WM_KEYDOWN;
			if((keyMsg->wParam >= 0x61) && (keyMsg->wParam <= 0x7a))
				key->wParam -= 0x20;
		}
	}
#endif
	
	if ((keyMsg->message == WM_KEYDOWN) || (keyMsg->message == WM_CHAR)){
		newKeyTime.Now();
		dT = newKeyTime - cgLastKey.time;
		dTsec = dT.GetSec();
		if (dTsec > 2.0)
			ClrSFlag(CON_blInDBEscMode);
		
		if ((keyMsg->message == WM_KEYDOWN) && (cgLastKey.message == WM_KEYDOWN) && (cgLastKey.wParam == keyMsg->wParam)){
			if (B_ChkFLAG32(cgLastKey.lParam, VK_COCLK | VK_DBCLK) == 0){
				if (dTsec < 0.1){
					cgLastKey.lParam = VK_COCLK;
				}
				else if (dTsec < 0.3){
					cgLastKey.lParam = VK_DBCLK;
				}
			}
			else{
				cgLastKey.lParam = 0;
			}
		}
		else{
			cgLastKey.lParam = 0;
		}
		
		cgLastKey.time		= newKeyTime;
		cgLastKey.message	= keyMsg->message;
		cgLastKey.wParam	= keyMsg->wParam;
		return(&cgLastKey);
	}
	return(nullptr);
}
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_Unix
const MSG* CONSOLE::GetKey(MSG* keyMsg){
	char key;
	
	key = getchar();
	do{
		keyMsg->message = WM_KEYDOWN;
		keyMsg->lParam = 0;
		keyMsg->wParam = VK_ERR;

		switch (key){
			case 9:;
				keyMsg->wParam = VK_TAB;
				break;
			case 10:;
				keyMsg->wParam = VK_RETURN;
				break;
			case 127:;
				keyMsg->wParam = VK_BACK;
				break;
			case 27:{
				key = getchar();
				switch (key){
					case EOF:;
						keyMsg->wParam = VK_ESCAPE;
						break;
					case 79:{
						key = getchar();
						if (key == 70){
							keyMsg->wParam = VK_END;
						}
						else if (key == 72){
							keyMsg->wParam = VK_HOME;
						}
						break;
					}
					case 91:{
						key = getchar();
						switch(key){
							case 65:;
								keyMsg->wParam = VK_UP;
								break;
							case 66:;
								keyMsg->wParam = VK_DOWN;
								break;
							case 67:;
								keyMsg->wParam = VK_RIGHT;
								break;
							case 68:;
								keyMsg->wParam = VK_LEFT;
								break;
							case 50:;
								key = getchar();
								if (key == 126)
									keyMsg->wParam = VK_INSERT;
								break;
							case 51:;
								key = getchar();
								if (key == 126)
									keyMsg->wParam = VK_DELETE;//del
								break;
							case 53:;
								key = getchar();
								if (key == 126)
									keyMsg->wParam = VK_PRIOR;//PgUp
								break;
							case 54:;
								key = getchar();
								if (key == 126)
									keyMsg->wParam = VK_NEXT;//PgDn
								break;
							default:;
								break;
						};
						break;
					}
					default:;
						break;
				};
				break;
			}
			default:{
				if ((key >= 0x20) && (key <= 0x7e)){
					keyMsg->message = WM_CHAR;
					keyMsg->wParam = key;
				}
				else{
					keyMsg->message = WM_NOKEY;
					keyMsg->wParam = VK_NOKEY;
				}
				break;
			}
		};
	}while(keyMsg->wParam == VK_ERR);
	return(keyMsg);
}
#endif
//------------------------------------------------------------------------------------------//
bool32 CONSOLE::ReceiveKey(const MSG* keyMsg){
	KEYMSG	*key;
	bool32	err;
	
	if (CheckSFlag(CON_blExit))
		return -1;

	key = AnalyzeKey(keyMsg);
	if (key == nullptr)
		return G_FALSE;
	
	err = G_FALSE;
	if (key->message == WM_KEYDOWN){
		err = DoControl(key);
	}
	else if (key->message == WM_CHAR){
		err = InsertChar(key->wParam);
	}
#ifdef ODEV_STDOUT_h
	if ((err == G_TRUE) && (cgOdevSTDOUT != nullptr)){
		if (CheckSFlag(CON_blDisplayAuto))
			cgOdevSTDOUT->PrintDisable();
	}
#endif
	return err;
}
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_Unix
bool32 CONSOLE::ReceiveKey(void){
	MSG				keyMsg;
	int32			oldf;
	struct termios	oldt, newt;
	
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
	
	
	while (ReceiveKey(GetKey(&keyMsg)) != -1)
		SYS_SleepMS(10);

	
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);
	
	return -1;
}
#endif
//------------------------------------------------------------------------------------------//
bool32 CONSOLE::BICThreadFun(void* p){
#ifdef BIC_Define_h
	BIC_BASE_S* bic = static_cast<BIC_BASE_S*>(p);

	CMD_BASE::ClrblExit(cgBICENV);
	bic->Command(cgBICENV, "", nullptr);
	
	SetExit();
#endif
	return G_TRUE;
}
//------------------------------------------------------------------------------------------//
void CONSOLE::StartWithBIC(CMD_ENV* env,BIC_BASE_S* bic){
#ifdef BIC_Define_h
	ClearAllFlag();
	BIC_ENV::SetConsole(env,this);
	BIC_ENV::SetSTDIN(env,&cgCMDSBUF);
	
	cgBICENV = env;
	bicThread.ThreadRun(bic);
#ifdef CommonDefH_Unix
	ReceiveKey();
	CMD_BASE::SetblExit(env);
#endif
#endif
}
//------------------------------------------------------------------------------------------//
void CONSOLE::ExecBIC(const STDSTR& cmd){
	cgCMDSBUF.Put(nullptr, cmd + "\n");
}
//------------------------------------------------------------------------------------------//
void CONSOLE::Stop(void){
#ifdef CMD_h
	CMD_BASE::SetblExit(cgBICENV);
#endif
	SetExit();
	bicThread.ThreadStop();
	cgBICENV = nullptr;
}
//------------------------------------------------------------------------------------------//
#endif /* Console_h */
