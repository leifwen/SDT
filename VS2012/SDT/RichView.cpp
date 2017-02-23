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
#include "Global.h"
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
	m_blEnableConsole = FALSE;
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
void CMyRichView::OnDropFiles(HDROP hDropInfo){
	int				nFileCount;
	std::string::size_type		pos;
	TCHAR			szFileName[MAX_PATH];
	CString			cFileName;
	std::wstring	wstrFileName;
	std::string		strFileName,strExtName,strT;

	nFileCount = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, MAX_PATH);
	//((CMDIChildWndEx*)GSDTApp.m_Device.cDevOutList->cODevRichMemo->cgRichEdit->GetParentFrame())->MDIActivate();
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
					GSDTApp.m_ReadInline.Paste((uint8*)strT.c_str(), strT.length());
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
static DWORD CALLBACK MyStreamOutCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb){
	CFile* pFile = (CFile*) dwCookie;
	pFile->Write(pbBuff, cb);
	*pcb = cb;
	return 0;
}
//------------------------------------------------------------------------------------------//
BOOL CMyRichView::PreTranslateMessage(MSG *pMsg){
	if (m_blEnableConsole == TRUE){
		if (GSDTApp.m_ReadInline.AKey(pMsg) != 0)
			return TRUE;
	}
	return(CRichEditView::PreTranslateMessage(pMsg));
}
//------------------------------------------------------------------------------------------//
void CMyRichView::OnPaste(void){
	if (m_blEnableConsole == FALSE){
		GetRichEditCtrl().Paste();
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
	GSDTApp.m_ReadInline.Paste((uint8*)ClipBoardText.c_str(), ClipBoardText.length());
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
	GetRichEditCtrl().StreamOut(SF_RTF, es);
	cFile.Close();
}
//------------------------------------------------------------------------------------------//
void CMyRichView::PrintRtfFile(std::string &tFileName){
	if (CFS_CheckFile(tFileName) == 0)
		return;

	GetRichEditCtrl().SetSel(0,-1);
	GetRichEditCtrl().Clear();

	CFile cFile;
	EDITSTREAM es;
	cFile.Open(Str_ANSIToUnicode(tFileName).c_str(),CFile::modeRead);
	es.dwCookie = (DWORD) &cFile;
	es.pfnCallback = MyStreamInCallback;
	GetRichEditCtrl().StreamIn(SF_RTF, es);
	cFile.Close();
}
//------------------------------------------------------------------------------------------//
void CMyRichView::PrintTxtFile(std::string &tFileName){
	if (CFS_CheckFile(tFileName) == 0)
		return;

	GetRichEditCtrl().SetSel(0,-1);
	//GetRichEditCtrl().Clear();

	CFile cFile;
	EDITSTREAM es;
	cFile.Open(Str_ANSIToUnicode(tFileName).c_str(),CFile::modeRead);
	es.dwCookie = (DWORD) &cFile;
	es.pfnCallback = MyStreamInCallback;

	SetFont(&m_ViewFont);
	GetRichEditCtrl().StreamIn(SF_TEXT, es);
	//GetRichEditCtrl().SetSel(-1, -1);
	//GetRichEditCtrl().SetSelectionCharFormat(&m_ViewFont);
	cFile.Close();
}
//------------------------------------------------------------------------------------------//
void CMyRichView::PrintHexFile(std::string &tFileName){
	std::string		strResult,str0;
	uint64			fileSize;
	uint8			buffer[8192];
	std::fstream	fileStream;
	uint32			num;
	ODEV_NODE_SDOUT	oDevNode;

	oDevNode.Init(this, NULL);

	if (CFS_CheckFile(tFileName) == 0)
		return;

	GetRichEditCtrl().SetSel(0,-1);
	GetRichEditCtrl().Clear();

	fileSize = CFS_CheckFileSize(tFileName);

	fileStream.open(tFileName.c_str(),std::ios::in|std::ios::binary);
	strResult = "Read in Hexadecimal mode, total ";
	strResult += Str_UInt64ToString(fileSize);
	strResult += " Bytes:\r\n";
	oDevNode.WriteToStr(strResult,RICH_CF_clMaroon);
	oDevNode.WriteDividingLine(RICH_CF_clPurple);

	if (fileSize < COLSTRING::BUF_MAX_SIZE / 3){
		do{
			fileStream.read((char*)buffer,sizeof(buffer));
			num = (uint32)fileStream.gcount();
			strResult = Str_CharToHEXStr(buffer,num,G_SPACE_ON);
			oDevNode.WriteToStr(strResult,RICH_CF_clBlack);
			oDevNode.Print();
		}while(!fileStream.eof());
		fileStream.close();
	}
	else{
		strResult = "File size is more then ";
		strResult += Str_IntToString(COLSTRING::BUF_MAX_SIZE / 3);
		strResult += " Bytes, do not show the detail.";
		oDevNode.WriteToStr(strResult,RICH_CF_clBlue);
	}


	oDevNode.WriteDividingLine(RICH_CF_clPurple);
	oDevNode.WriteToStr("End",RICH_CF_clMaroon);
	oDevNode.Print();
	GetRichEditCtrl().SetSel(0,0);
}
//------------------------------------------------------------------------------------------//
void CMyRichView::PrintFileName(HDROP hDropInfo){
	CString			cResult;
	std::wstring	wstrFileName;
	std::string		strPrint;
	TCHAR			szFileName[MAX_PATH];
	int				nFileCount;
	ODEV_NODE_SDOUT	oDevNode;

	oDevNode.Init(this,NULL);
	GetRichEditCtrl().SetSel(0,-1);
	GetRichEditCtrl().Clear();

	nFileCount = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, MAX_PATH);

	strPrint = "Total ";
	strPrint += Str_IntToString(nFileCount);
	strPrint += " files selected, pls drop one file and try again.";
	oDevNode.WriteToStr(strPrint,RICH_CF_clMaroon);
	oDevNode.WriteDividingLine(RICH_CF_clPurple);
	for (int i = 0; i < nFileCount; ++ i){
		ZeroMemory(szFileName,MAX_PATH);
		DragQueryFile(hDropInfo,i,szFileName,MAX_PATH);
		cResult += szFileName;
		cResult += _T("\r\n");
	}
	wstrFileName = cResult;
	oDevNode.WriteToStr(Str_UnicodeToANSI(wstrFileName),RICH_CF_clBlue);
	oDevNode.WriteDividingLine(RICH_CF_clPurple);
	oDevNode.WriteToStr("End",RICH_CF_clMaroon);
	oDevNode.Print();
	GetRichEditCtrl().SetSel(0,0);
}
//------------------------------------------------------------------------------------------//
BOOL CMyRichView::PrintDirList(std::string &tDirName){
	std::wstring	wstrFileName;
	std::string		strPrint;
	int				nFileCount;
	ODEV_NODE_SDOUT	oDevNode;

	CFileFind	fileFinder;
	CString		filePath,fileName,fileDir;
	BOOL		blFinished;

	oDevNode.Init(this, NULL);

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

	GetRichEditCtrl().SetSel(0,-1);
	GetRichEditCtrl().Clear();


	strPrint = "Total ";
	strPrint += Str_IntToString(nFileCount);
	strPrint += " files.\r\n";

	oDevNode.WriteToStr(strPrint,RICH_CF_clMaroon);
	oDevNode.WriteDividingLine(RICH_CF_clPurple);
	oDevNode.WriteToStr(tDirName + "\\\r\n",RICH_CF_clBlack);

	wstrFileName = fileDir;
	oDevNode.WriteToStr(Str_UnicodeToANSI(wstrFileName),RICH_CF_clRed);
	wstrFileName = fileName;
	oDevNode.WriteToStr(Str_UnicodeToANSI(wstrFileName),RICH_CF_clBlue);
	oDevNode.WriteDividingLine(RICH_CF_clPurple);
	oDevNode.WriteToStr("End",RICH_CF_clMaroon);
	oDevNode.Print();
	GetRichEditCtrl().SetSel(0,0);
	return TRUE;
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//