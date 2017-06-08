// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// http://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// SDTView.cpp : implementation of the CSDTView class
//
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "Global.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "SDT.h"
#endif

#include "resource.h"
#include "RichView.h"
#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//------------------------------------------------------------------------------------------//
IMPLEMENT_DYNCREATE(CMyRichView, CRichEditView)
//------------------------------------------------------------------------------------------//
CMyRichView::CMyRichView(void){
	m_ViewFont.CreateFont
		(14							// nHeight
		,0							// nWidth
		,0							// nEscapement
		,0							// nOrientation
		,FW_NORMAL					// nWeight
		,FALSE						// bItalic
		,FALSE						// bUnderline
		,0							// cStrikeOut
		,ANSI_CHARSET				// nCharSet
		,OUT_DEFAULT_PRECIS			// nOutPrecision
		,CLIP_DEFAULT_PRECIS		// nClipPrecision
		,DEFAULT_QUALITY			// nQuality
		,DEFAULT_PITCH | FF_SWISS	// nPitchAndFamily
		,_T("宋体"));				//Courier New
	m_Console = nullptr;
	SetExPar(&cgCF, COL_RGB_clBlack);
}
//------------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CMyRichView, CRichEditView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_DROPFILES()
	ON_WM_CREATE()
	ON_COMMAND(ID_RE_SAVE, &CMyRichView::OnSave)
	ON_COMMAND(ID_EDIT_PASTE, &CMyRichView::OnPaste)
END_MESSAGE_MAP()
//------------------------------------------------------------------------------------------//
CMyRichView::~CMyRichView(void){
}
//------------------------------------------------------------------------------------------//
int CMyRichView::OnCreate(LPCREATESTRUCT lpCreateStruct){
	if (CRichEditView::OnCreate(lpCreateStruct) == -1)
		return -1;
	RevokeDragDrop(this->GetSafeHwnd());
	SetFont(&m_ViewFont);
	return 0;
}
//------------------------------------------------------------------------------------------//
BOOL CMyRichView::PreCreateWindow(CREATESTRUCT& cs){
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	return CRichEditView::PreCreateWindow(cs);
}
//------------------------------------------------------------------------------------------//
void CMyRichView::OnInitialUpdate(void){
	CRichEditView::OnInitialUpdate();
}
//------------------------------------------------------------------------------------------//
void CMyRichView::OnRButtonUp(UINT /* nFlags */, CPoint point){
	ClientToScreen(&point);
	OnContextMenu(this, point);
}
//------------------------------------------------------------------------------------------//
void CMyRichView::OnContextMenu(CWnd* /* pWnd */, CPoint point){
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#ifdef _DEBUG
void CMyRichView::AssertValid() const{
	CRichEditView::AssertValid();
}
//------------------------------------------------------------------------------------------//
void CMyRichView::Dump(CDumpContext& dc) const{
	CRichEditView::Dump(dc);
}
#endif //_DEBUG

//------------------------------------------------------------------------------------------//
CHARFORMAT2& CMyRichView::SetExPar(CHARFORMAT2 *tcf, const COLORREF &col){
	memset(tcf, 0, sizeof(CHARFORMAT2));
	tcf->cbSize = sizeof(CHARFORMAT2);
	tcf->dwMask = CFM_BOLD | CFM_CHARSET | CFM_COLOR | CFM_FACE | CFM_OFFSET | CFM_SIZE;
	tcf->dwEffects = 0;
	tcf->yHeight = 14 * 14;
	tcf->yOffset = 0;
	tcf->bCharSet = GB2312_CHARSET;
	tcf->bPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	lstrcpy(tcf->szFaceName, _T("宋体"));//Arial,Courier New
	tcf->crTextColor = col;
	return(*tcf);
}
//------------------------------------------------------------------------------------------//
void CMyRichView::SetCurFromEnd(int32 offset){
	long	nStartChar, nEndChar;
	Spin_Print_Lock();
	GetRichEditCtrl().SetSel(-1, -1);
	GetRichEditCtrl().GetSel(nStartChar, nEndChar);
	nStartChar -= offset;
	GetRichEditCtrl().SetSel(nStartChar, nStartChar);
	Spin_Print_Unlock();
}
//------------------------------------------------------------------------------------------//
void CMyRichView::DelCharFromEnd(int32 offset, int32 length){
	long	nStartChar, nEndChar;
	Spin_Print_Lock();
	GetRichEditCtrl().HideSelection(TRUE, FALSE);
	GetRichEditCtrl().SetSel(-1, -1);
	GetRichEditCtrl().GetSel(nStartChar, nEndChar);
	nStartChar -= offset;
	nEndChar = nStartChar + length;
	GetRichEditCtrl().SetSel(nStartChar, nEndChar);
	GetRichEditCtrl().ReplaceSel(_T(""));
	GetRichEditCtrl().HideSelection(FALSE, FALSE);
	Spin_Print_Unlock();
}
//------------------------------------------------------------------------------------------//
void CMyRichView::RewriteCharFromEnd(int32 offset, const COLORREF &col,const STDSTR &strIn){
	long	nStartChar, nEndChar;

	Spin_Print_Lock();
	GetRichEditCtrl().HideSelection(TRUE, FALSE);
	GetRichEditCtrl().SetSel(-1, -1);
	GetRichEditCtrl().GetSel(nStartChar, nEndChar);
	nStartChar -= offset;

	GetRichEditCtrl().SetSel(nStartChar, nEndChar);
	GetRichEditCtrl().SetSelectionCharFormat(SetExPar(&cgCF, col));
	GetRichEditCtrl().ReplaceSel(Str_ANSIToUnicode(strIn).c_str());
	if (strIn.length() == 1){
		GetRichEditCtrl().SetSel(nStartChar, nStartChar + 1);
		GetRichEditCtrl().SetSelectionCharFormat(SetExPar(&cgCF, col));
	}

	GetRichEditCtrl().SetSel(-1, -1);
	GetRichEditCtrl().HideSelection(FALSE, FALSE);
	Spin_Print_Unlock();
}
//------------------------------------------------------------------------------------------//
void CMyRichView::InsterCharFromEnd(int32 offset, const COLORREF &col,const STDSTR &strIn){
	long	nStartChar, nEndChar;

	Spin_Print_Lock();
	GetRichEditCtrl().HideSelection(TRUE, FALSE);
	GetRichEditCtrl().SetSel(-1, -1);
	GetRichEditCtrl().GetSel(nStartChar, nEndChar);
	nStartChar -= offset;

	GetRichEditCtrl().SetSel(nStartChar, nStartChar);
	GetRichEditCtrl().SetSelectionCharFormat(SetExPar(&cgCF, col));
	GetRichEditCtrl().ReplaceSel(Str_ANSIToUnicode(strIn).c_str());
	if (strIn.length() == 1){
		GetRichEditCtrl().SetSel(nStartChar, nStartChar + 1);
		GetRichEditCtrl().SetSelectionCharFormat(SetExPar(&cgCF, col));
	}
	GetRichEditCtrl().SetSel(nStartChar + strIn.length(), nStartChar + strIn.length());
	GetRichEditCtrl().HideSelection(FALSE, FALSE);
	Spin_Print_Unlock();
}
//------------------------------------------------------------------------------------------//
void CMyRichView::AppendChar(const COLORREF &col, const STDSTR &strIn, G_LOCK blLock){
	long	nStartChar, nEndChar;
	Spin_Print_Lock(blLock);
	if (GetRichEditCtrl().GetTextLength() > ODEV_STDOUT::BUF_MAX_SIZE)
		Clean(G_LOCK_OFF);
	if (strIn.length() == 1){
		GetRichEditCtrl().HideSelection(TRUE, FALSE);
		GetRichEditCtrl().SetSel(-1, -1);
		GetRichEditCtrl().GetSel(nStartChar, nEndChar);
		GetRichEditCtrl().ReplaceSel(Str_ANSIToUnicode(strIn).c_str());
		GetRichEditCtrl().SetSel(nStartChar, -1);
		
		GetRichEditCtrl().SetSelectionCharFormat(SetExPar(&cgCF, col));
		GetRichEditCtrl().SetSel(-1, -1);
		GetRichEditCtrl().HideSelection(FALSE, FALSE);
	}
	else{
		GetRichEditCtrl().SetSel(-1, -1);
		GetRichEditCtrl().SetSelectionCharFormat(SetExPar(&cgCF, col));
		GetRichEditCtrl().ReplaceSel(Str_ANSIToUnicode(strIn).c_str());
	}
	Spin_Print_Unlock(blLock);
}
//------------------------------------------------------------------------------------------//
void CMyRichView::ToHome(void){
	Spin_Print_Lock();
	GetRichEditCtrl().SetSel(0, 0);
	Spin_Print_Unlock();
}
//------------------------------------------------------------------------------------------//
void CMyRichView::Clean(G_LOCK blLock){
	Spin_Print_Lock(blLock);
	GetRichEditCtrl().SetSel(0, -1);
	GetRichEditCtrl().Clear();
	Spin_Print_Unlock(blLock);
}
//------------------------------------------------------------------------------------------//
void CMyRichView::OnDropFiles(HDROP hDropInfo){
	int				nFileCount;
	std::string::size_type		pos;
	TCHAR			szFileName[MAX_PATH];
	CString			cFileName;
	std::wstring	wstrFileName;
	std::string		strFileName, strExtName, strT;

	nFileCount = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, MAX_PATH);
	if (nFileCount > 1){
		PrintFileName(hDropInfo);
	}
	else{
		ZeroMemory(szFileName,MAX_PATH);
		DragQueryFile(hDropInfo,0,szFileName,MAX_PATH);
		cFileName = szFileName;
		wstrFileName = cFileName;
		strFileName = Str_UnicodeToANSI(wstrFileName);
		if (CFS_CheckFile(strFileName) != 0){
			if (PrintDirList(strFileName) == FALSE){
				if (::GetKeyState(VK_MENU) & 0x8000){
					strT = Str_Replace(strFileName, " ", "\\ ");
					GSDTApp.m_Console.Paste((uint8*)strT.c_str(), strT.length());
				}
				else{
					pos = strFileName.find_last_of('.');
					strExtName = "";
					if (pos != std::string::npos)
						strExtName = strFileName.substr(pos);
					if ((Str_UpperCase(strExtName) == ".TXT")
						|| (Str_UpperCase(strExtName) == ".INI")){
						if (::GetKeyState(VK_CONTROL) & 0x8000){
							PrintHexFile(strFileName);
						}
						else{
							PrintTxtFile(strFileName);

						}
					}
					else if (Str_UpperCase(strExtName) == ".RTF"){
						if (::GetKeyState(VK_CONTROL) & 0x8000){
							PrintHexFile(strFileName);
						}
						else{
							PrintRtfFile(strFileName);
						}
					}
					else{
						if (::GetKeyState(VK_CONTROL) & 0x8000){
							PrintTxtFile(strFileName);
						}
						else{
							PrintHexFile(strFileName);
						}
					}
				}
			}
		}
	}
	DragFinish (hDropInfo);
}
//------------------------------------------------------------------------------------------//
static DWORD CALLBACK MyStreamInCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb){
	CFile* pFile = (CFile*) dwCookie;
	*pcb = pFile->Read(pbBuff, cb);
	return 0;
}
//------------------------------------------------------------------------------------------//
static DWORD CALLBACK MyStreamInCallback_HEX(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb){
	STDSTR  strT;
	LPBYTE	tempSave;
	LONG	num;
	uint8	charData;
	CFile* pFile = (CFile*)dwCookie;

	tempSave = pbBuff + ((cb / 3) << 1);

	num = pFile->Read(tempSave, cb / 3);
	*pcb = 0;

	while(num > 0){
		charData = (*tempSave >> 4) & (0x0f);
		if (charData < 0x0a){
			charData += '0';
		}
		else{
			charData += ('A' - 0x0a);
		}
		*pbBuff = charData;

		++pbBuff;
		charData = (*tempSave) & (0x0f);
		if (charData < 0x0a){
			charData += '0';
		}
		else{
			charData += ('A' - 0x0a);
		}
		*pbBuff = charData;

		++pbBuff;
		*pbBuff = ' ';

		++pbBuff;
		++tempSave;
		--num;
		*pcb += 3;
	};
	return 0;
}
//------------------------------------------------------------------------------------------//
static DWORD CALLBACK MyStreamOutCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb){
	CFile* pFile = (CFile*) dwCookie;
	pFile->Write(pbBuff, cb);
	*pcb = cb;
	return 0;
}
//------------------------------------------------------------------------------------------//
BOOL CMyRichView::PreTranslateMessage(MSG *pMsg){
	if (m_Console != nullptr){
		if (m_Console->ReceiveKey(pMsg) != 0)
			return TRUE;
	}
	return(CRichEditView::PreTranslateMessage(pMsg));
}
//------------------------------------------------------------------------------------------//
void CMyRichView::OnPaste(void){
	if (m_Console == nullptr){
		Spin_Print_Lock();
		GetRichEditCtrl().Paste();
		Spin_Print_Unlock();
		return;
	}

	std::string ClipBoardText;
	char* charP;

	if (!IsClipboardFormatAvailable(CF_TEXT))
		return;
        
	if (!::OpenClipboard(GetSafeHwnd()))
		return;

	HGLOBAL hMem = ::GetClipboardData(CF_TEXT);
	if (hMem != NULL){
		charP = (char*)GlobalLock(hMem);
		if (charP != NULL){
			ClipBoardText = charP;
			GlobalUnlock(hMem);
		}
	}
	CloseClipboard();
	m_Console->Paste((uint8*)ClipBoardText.c_str(), ClipBoardText.length());
	return;
}
//------------------------------------------------------------------------------------------//
void CMyRichView::OnSave(void){
	CString	cFileName;
	TCHAR	path[MAX_PATH];  
	std::wstring	strwText;

	GetCurrentDirectory(MAX_PATH,path);
	CFileDialog hFileDlg(FALSE,_T("rtf"),_T("*.rtf")
		,OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_READONLY|OFN_NOCHANGEDIR
		,_T("rtf file(*.rtf)|*.rtf|all(*.*)|*.*||"),NULL);
	hFileDlg.m_ofn.nFilterIndex = 1;
	hFileDlg.m_ofn.hwndOwner = GetSafeHwnd();
	hFileDlg.m_ofn.lStructSize = sizeof(OPENFILENAME);
	hFileDlg.m_ofn.lpstrTitle = _T("Save");
	hFileDlg.m_ofn.nMaxFile = MAX_PATH;
	hFileDlg.m_ofn.lpstrInitialDir = path;
	SetCurrentDirectory(path);

	if(hFileDlg.DoModal() == IDOK){
		cFileName = hFileDlg.GetPathName();
		strwText = cFileName.GetBuffer(0);
		SaveToRtfFile(Str_UnicodeToANSI(strwText));
	}
}
//------------------------------------------------------------------------------------------//
void CMyRichView::SaveToRtfFile(std::string &tFileName){
	CFile cFile;
	EDITSTREAM es;
	cFile.Open(Str_ANSIToUnicode(tFileName).c_str(),CFile::modeCreate|CFile::modeWrite);
	es.dwCookie = (DWORD) &cFile;
	es.pfnCallback = MyStreamOutCallback; 
	Spin_Print_Lock();
	GetRichEditCtrl().StreamOut(SF_RTF, es);
	Spin_Print_Unlock();
	cFile.Close();
}
//------------------------------------------------------------------------------------------//
void CMyRichView::PrintRtfFile(std::string &tFileName){
	if (CFS_CheckFile(tFileName) == 0)
		return;
	Clean();

	CFile cFile;
	EDITSTREAM es;
	cFile.Open(Str_ANSIToUnicode(tFileName).c_str(),CFile::modeRead);
	es.dwCookie = (DWORD) &cFile;
	es.pfnCallback = MyStreamInCallback;

	Spin_Print_Lock();
	GetRichEditCtrl().StreamIn(SF_RTF, es);
	Spin_Print_Unlock();
	cFile.Close();
}
//------------------------------------------------------------------------------------------//
void CMyRichView::PrintTxtFile(std::string &tFileName){
	if (CFS_CheckFile(tFileName) == 0)
		return;
	Clean();

	CFile cFile;
	EDITSTREAM es;
	cFile.Open(Str_ANSIToUnicode(tFileName).c_str(),CFile::modeRead);
	es.dwCookie = (DWORD) &cFile;
	es.pfnCallback = MyStreamInCallback;

	SetFont(&m_ViewFont);
	Spin_Print_Lock();
	GetRichEditCtrl().StreamIn(SF_TEXT, es);
	GetRichEditCtrl().SetSel(0, 0);
	Spin_Print_Unlock();
	cFile.Close();
}
//------------------------------------------------------------------------------------------//
void CMyRichView::PrintHexFile(std::string &tFileName){
	if (CFS_CheckFile(tFileName) == 0)
		return;
	Clean();

	CFile cFile;
	EDITSTREAM es;
	cFile.Open(Str_ANSIToUnicode(tFileName).c_str(), CFile::modeRead);
	es.dwCookie = (DWORD)&cFile;
	es.pfnCallback = MyStreamInCallback_HEX;

	SetFont(&m_ViewFont);
	Spin_Print_Lock();
	GetRichEditCtrl().StreamIn(SF_TEXT, es);
	GetRichEditCtrl().SetSel(0, 0);
	Spin_Print_Unlock();
	cFile.Close();
}
//------------------------------------------------------------------------------------------//
void CMyRichView::PrintFileName(HDROP hDropInfo){
	CString			cResult;
	std::wstring	wstrFileName;
	std::string		strPrint;
	TCHAR			szFileName[MAX_PATH];
	int				nFileCount;

	Spin_Print_Lock();
	Clean(G_LOCK_OFF);

	nFileCount = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, MAX_PATH);

	strPrint = "Total ";
	strPrint += Str_ToString(nFileCount);
	strPrint += " files selected, pls drop one file and try again.\n";
	AppendChar(COL_RGB_clMaroon,strPrint, G_LOCK_OFF);
	AppendChar(COL_RGB_clPurple,DIVIDING_LINE, G_LOCK_OFF);

	for (int i = 0; i < nFileCount; ++ i){
		ZeroMemory(szFileName,MAX_PATH);
		DragQueryFile(hDropInfo,i,szFileName,MAX_PATH);
		cResult += szFileName;
		cResult += _T("\r\n");
	}
	wstrFileName = cResult;
	AppendChar(COL_RGB_clBlue,Str_UnicodeToANSI(wstrFileName), G_LOCK_OFF);
	AppendChar(COL_RGB_clPurple,DIVIDING_LINE, G_LOCK_OFF);
	AppendChar(COL_RGB_clMaroon,"End", G_LOCK_OFF);

	GetRichEditCtrl().SetSel(0,0);
	Spin_Print_Unlock();
}
//------------------------------------------------------------------------------------------//
BOOL CMyRichView::PrintDirList(std::string &tDirName){
	std::wstring	wstrFileName;
	std::string		strPrint;
	int				nFileCount;

	CFileFind	fileFinder;
	CString		filePath,fileName,fileDir;
	BOOL		blFinished;

	filePath = Str_ANSIToUnicode(tDirName).c_str();
	blFinished = fileFinder.FindFile(filePath);
	if (blFinished == TRUE){
		fileFinder.FindNextFile();
		if (fileFinder.IsDirectory() == FALSE)
			return FALSE;
	}

	filePath += _T("\\*.*");
	blFinished = fileFinder.FindFile(filePath);
	fileName = "";
	fileDir = "";
	nFileCount = 0;
	while(blFinished){
		blFinished = fileFinder.FindNextFile();
		if(!fileFinder.IsDots()){
			++ nFileCount;
			if (fileFinder.IsDirectory() ==  FALSE){
				fileName += "  ";
				fileName += fileFinder.GetFileName();
				fileName += _T("\r\n");
			}
			else{
				fileDir += "  ";
				fileDir += fileFinder.GetFileName();
				fileDir += _T("\r\n");
			}
		}
	}

	Spin_Print_Lock();
	Clean(G_LOCK_OFF);

	strPrint = "Total ";
	strPrint += Str_ToString(nFileCount);
	strPrint += " files.\r\n";

	AppendChar(COL_RGB_clMaroon,strPrint);
	AppendChar(COL_RGB_clPurple,DIVIDING_LINE, G_LOCK_OFF);
	AppendChar(COL_RGB_clBlack,tDirName + "\\\r\n", G_LOCK_OFF);

	wstrFileName = fileDir;
	AppendChar(COL_RGB_clRed,Str_UnicodeToANSI(wstrFileName));
	wstrFileName = fileName;
	AppendChar(COL_RGB_clBlue,Str_UnicodeToANSI(wstrFileName));
	AppendChar(COL_RGB_clPurple,DIVIDING_LINE, G_LOCK_OFF);
	AppendChar(COL_RGB_clMaroon,"End");

	GetRichEditCtrl().SetSel(0,0);
	Spin_Print_Unlock();
	return TRUE;
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//