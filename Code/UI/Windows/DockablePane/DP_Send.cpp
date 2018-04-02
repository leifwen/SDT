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
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "Global.h"
#include "SDT.h"
#include "DP_Send.h"
#include "Global.h"
#include "RichView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//------------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CSendCEdit, CEdit)
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()
//------------------------------------------------------------------------------------------//
BOOL CSendCEdit::PreTranslateMessage(MSG* pMsg){
	UINT  nCode = pMsg->wParam;
	if (pMsg->message == WM_KEYDOWN){
		if (nCode == _T('A') && (::GetKeyState(VK_CONTROL) & 0x8000)){
			SetSel(0,-1);
			return TRUE;
		}
	}
	return(CEdit::PreTranslateMessage(pMsg));
}
//------------------------------------------------------------------------------------------//
void CSendCEdit::OnLButtonDblClk(UINT nFlags, CPoint point){
	SetSel(0,-1);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CSendTimeCEdit, CEdit)
	ON_WM_CONTEXTMENU()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()
//------------------------------------------------------------------------------------------//
void CSendTimeCEdit::OnKillFocus(CWnd* pNewWnd){
	CString	cValue;
	std::string		strTemp;
	double doubleValue;
	GetWindowText(cValue);
	strTemp = Str_UnicodeToANSI(cValue.GetBuffer(0));
	doubleValue = atof(strTemp.c_str());
	strTemp = Str_FloatToStr(doubleValue);
	SetWindowText(Str_ANSIToUnicode(strTemp).c_str());
	CEdit::OnKillFocus(pNewWnd);
}
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CSendViewDP, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_WM_SETFOCUS()
	ON_WM_PAINT()
END_MESSAGE_MAP()
//------------------------------------------------------------------------------------------//
int CSendViewDP::OnCreate(LPCREATESTRUCT lpCreateStruct){
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();
	if (!m_cView.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rectDummy,this,0)){
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}
	AdjustLayout();
	return 0;
}
//------------------------------------------------------------------------------------------//
void CSendViewDP::AdjustLayout(void){
	if (GetSafeHwnd() == NULL)
		return;

	CRect rectT;
	GetClientRect(rectT);
	m_cView.SetWindowPos(NULL, rectT.left + 1, rectT.top + 1, rectT.Width() - 2, rectT.Height() - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}
//------------------------------------------------------------------------------------------//
void CSendViewDP::OnSize(UINT nType, int cx, int cy){
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}
//------------------------------------------------------------------------------------------//
void CSendViewDP::OnSetFocus(CWnd* pOldWnd){
	CDockablePane::OnSetFocus(pOldWnd);
}
//------------------------------------------------------------------------------------------//
void CSendViewDP::OnPaint(void){
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	m_cView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}
//------------------------------------------------------------------------------------------//
void CSendViewDP::OnContextMenu(CWnd* pWnd, CPoint point){
}
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------/
IMPLEMENT_DYNCREATE(CSendPaneView, CView)
//------------------------------------------------------------------------------------------/
BEGIN_MESSAGE_MAP(CSendPaneView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_MOUSEACTIVATE()
	ON_COMMAND(ID_SENDBOX_HEX, &CSendPaneView::OnHex)
	ON_COMMAND(ID_SENDBOX_ESCAPE, &CSendPaneView::OnEscape)
	ON_COMMAND(ID_SENDBOX_CR, &CSendPaneView::OnCR)
	ON_COMMAND(ID_SENDBOX_ENABLE, &CSendPaneView::OnEnable)
	ON_COMMAND(ID_SENDBOX_SEND, &CSendPaneView::OnSend)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()
//------------------------------------------------------------------------------------------//
CSendPaneView::CSendPaneView(void) : CView(){
	m_ButtonFont.CreateFont
		(13							// nHeight
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
		,_T("Tahoma"));//Courier New
}
//------------------------------------------------------------------------------------------//
void CSendPaneView::PostNcDestroy(void){   
// CView::PostNcDestroy();   
}
//------------------------------------------------------------------------------------------//
int CSendPaneView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message){
	return 0;
}
//------------------------------------------------------------------------------------------//
BOOL CSendPaneView::PreCreateWindow(CREATESTRUCT& cs){
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	return CView::PreCreateWindow(cs);
}
//------------------------------------------------------------------------------------------//
#ifdef _DEBUG
void CSendPaneView::AssertValid() const{
	CView::AssertValid();
}
void CSendPaneView::Dump(CDumpContext& dc) const{
	CView::Dump(dc);
}
#endif //_DEBUG
//------------------------------------------------------------------------------------------//
BOOL CSendPaneView::PreTranslateMessage(MSG* pMsg){
	if (pMsg->message == WM_KEYDOWN){
		UINT  nCode = pMsg->wParam;
		if ((nCode == _T('A') || nCode == _T('C') || nCode == _T('X') || nCode == _T('V') || nCode == _T('Z'))
			&&(::GetKeyState(VK_CONTROL) & 0x8000)){
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return TRUE;
		}
		if ((pMsg->wParam == VK_RETURN)
			&& !(::GetKeyState(VK_CONTROL) & 0x8000)
			&& !(::GetKeyState(VK_SHIFT) & 0x8000)
			&& !(::GetKeyState(VK_MENU) & 0x8000)){
			OnSend();
			return TRUE;
		}
	}
	return(CView::PreTranslateMessage(pMsg));
}
//------------------------------------------------------------------------------------------//
void CSendPaneView::OnDraw(CDC* pDC){
	CRect rectTree;

	m_cEdit.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	pDC->Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));


	m_TimeEdit.GetWindowRect(rectTree);
	ScreenToClient(rectTree);
	rectTree.InflateRect(1, 1);
	pDC->Draw3dRect(rectTree, ::GetSysColor(COLOR_ACTIVECAPTION), ::GetSysColor(COLOR_ACTIVECAPTION));
}
//------------------------------------------------------------------------------------------//
void CSendPaneView::OnSize(UINT nType, int cx, int cy){
	CView::OnSize(nType, cx, cy);
	AdjustLayout();
}
//------------------------------------------------------------------------------------------//
int CSendPaneView::OnCreate(LPCREATESTRUCT lpCreateStruct){
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	if (CreateEdit() == -1)
		return -1;
	DragAcceptFiles(TRUE);
	AdjustLayout();
	return 0;
}
//------------------------------------------------------------------------------------------//
int CSendPaneView::CreateEdit(void){
	CRect rectDummy;
	rectDummy.SetRectEmpty();
	if (!m_cEdit.Create(ES_MULTILINE|ES_AUTOVSCROLL|WS_CHILD|WS_VISIBLE|WS_VSCROLL, rectDummy, this, 0)){
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}

	if (!m_GroupSetting.Create(_T("Setting"),WS_CHILD|WS_VISIBLE|BS_GROUPBOX, rectDummy, this, 0)){
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}
	if (!m_CheckBoxHex.Create(_T("Hex"),WS_CHILD|WS_VISIBLE|BS_CHECKBOX, rectDummy, this, ID_SENDBOX_HEX)){
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}
	if (!m_CheckBoxEscape.Create(_T("EnEscape"),WS_CHILD|WS_VISIBLE|BS_CHECKBOX, rectDummy, this, ID_SENDBOX_ESCAPE)){
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}
	if (!m_CheckBoxCR.Create(_T("Send<CR>"),WS_CHILD|WS_VISIBLE|BS_CHECKBOX, rectDummy, this, ID_SENDBOX_CR)){
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}
	if (!m_CheckBoxSendEnable.Create(_T("Enable"),WS_CHILD|WS_VISIBLE|BS_CHECKBOX, rectDummy, this, ID_SENDBOX_ENABLE)){
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}
	if (!m_ButtonSend.Create(_T("Send"),WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, rectDummy, this, ID_SENDBOX_SEND)){
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}
	if (!m_GroupAutoSend.Create(_T("Time(S)"),WS_CHILD|WS_VISIBLE|BS_GROUPBOX, rectDummy, this, 0)){
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}
	if (!m_TimeEdit.Create(WS_CHILD|WS_VISIBLE, rectDummy, this, 0)){
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}
	m_TimeEdit.SetWindowText(_T("1"));
	m_CheckBoxHex.SetFont(&m_ButtonFont);
	m_CheckBoxEscape.SetFont(&m_ButtonFont);
	m_CheckBoxCR.SetFont(&m_ButtonFont);
	m_CheckBoxSendEnable.SetFont(&m_ButtonFont);
	m_GroupAutoSend.SetFont(&m_ButtonFont);
	m_TimeEdit.SetFont(&m_ButtonFont);
	m_ButtonSend.SetFont(&m_ButtonFont);
	m_cEdit.SetFont(&m_ButtonFont);
	return 0;
}
//------------------------------------------------------------------------------------------//
void CSendPaneView::AdjustLayout(void){
	CRect rectClient,rectGroupS,rectGroupT;
	GetClientRect(rectClient);

	rectClient.InflateRect(-5, -5);

	rectGroupS = rectClient;
	rectGroupS.top = rectGroupS.bottom - 90;
	m_GroupSetting.SetWindowPos(NULL, rectGroupS.left, rectGroupS.top, rectGroupS.Width(), rectGroupS.Height(), SWP_NOACTIVATE | SWP_NOZORDER);

	rectGroupT = rectClient;
	rectGroupT.bottom = rectGroupS.top - 5;
	m_cEdit.SetWindowPos(NULL, rectGroupT.left, rectGroupT.top, rectGroupT.Width(), rectGroupT.Height(), SWP_NOACTIVATE | SWP_NOZORDER);

	rectGroupT = rectGroupS;
	rectGroupT.left += 10;
	rectGroupT.right = rectGroupT.left + m_CheckBoxHex.GetWindowTextLengthW() * 12;
	rectGroupT.top += 20;
	rectGroupT.bottom = rectGroupT.top + 20;
	m_CheckBoxHex.SetWindowPos(NULL, rectGroupT.left, rectGroupT.top, rectGroupT.Width(), rectGroupT.Height(), SWP_NOACTIVATE | SWP_NOZORDER);

	rectGroupT.right = rectGroupT.left + m_CheckBoxEscape.GetWindowTextLengthW() * 10;
	rectGroupT.top += 22;
	rectGroupT.bottom = rectGroupT.top + 20;
	m_CheckBoxEscape.SetWindowPos(NULL, rectGroupT.left, rectGroupT.top, rectGroupT.Width(), rectGroupT.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
	
	rectGroupT.right = rectGroupT.left + m_CheckBoxCR.GetWindowTextLengthW() * 10;
	rectGroupT.top += 22;
	rectGroupT.bottom = rectGroupT.top + 20;
	m_CheckBoxCR.SetWindowPos(NULL, rectGroupT.left, rectGroupT.top, rectGroupT.Width(), rectGroupT.Height(), SWP_NOACTIVATE | SWP_NOZORDER);

	rectGroupT = rectGroupS;
	rectGroupT.left += 100;
	rectGroupT.right = rectGroupT.left + 100;
	rectGroupT.top += 15;
	rectGroupT.bottom = rectGroupT.bottom - 5;
	m_GroupAutoSend.SetWindowPos(NULL, rectGroupT.left, rectGroupT.top, rectGroupT.Width(), rectGroupT.Height(), SWP_NOACTIVATE | SWP_NOZORDER);

	rectGroupT.right = rectGroupS.right - 10;
	rectGroupT.bottom = rectGroupS.bottom - 5;
	m_ButtonSend.SetWindowPos(NULL, rectGroupT.left + 100 + 10, rectGroupT.top, rectGroupT.Width() - 100 - 10, rectGroupT.Height(), SWP_NOACTIVATE | SWP_NOZORDER);

	rectGroupT.left += 10;
	rectGroupT.right = rectGroupT.left + 80;
	rectGroupT.top += 20;
	rectGroupT.bottom = rectGroupT.top + 16;
	m_TimeEdit.SetWindowPos(NULL, rectGroupT.left, rectGroupT.top, rectGroupT.Width(), rectGroupT.Height(), SWP_NOACTIVATE | SWP_NOZORDER);

	rectGroupT.right = rectGroupT.left + m_CheckBoxSendEnable.GetWindowTextLengthW() * 10;
	rectGroupT.top += 25;
	rectGroupT.bottom = rectGroupT.top + 20;
	m_CheckBoxSendEnable.SetWindowPos(NULL, rectGroupT.left, rectGroupT.top, rectGroupT.Width(), rectGroupT.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
}
//------------------------------------------------------------------------------------------//
void CSendPaneView::OnHex(void){
	m_CheckBoxHex.SetCheck(!m_CheckBoxHex.GetCheck());
	if (m_CheckBoxHex.GetCheck()){
		m_CheckBoxEscape.SetCheck(FALSE);
		m_CheckBoxCR.SetCheck(FALSE);
	}
}
//------------------------------------------------------------------------------------------//
void CSendPaneView::OnEscape(void){
	m_CheckBoxEscape.SetCheck(!m_CheckBoxEscape.GetCheck());
	if (m_CheckBoxEscape.GetCheck())
		m_CheckBoxHex.SetCheck(FALSE);
}
//------------------------------------------------------------------------------------------//
void CSendPaneView::OnCR(void){
	m_CheckBoxCR.SetCheck(!m_CheckBoxCR.GetCheck());
	if (m_CheckBoxCR.GetCheck())
		m_CheckBoxHex.SetCheck(FALSE);
}
//------------------------------------------------------------------------------------------//
void CSendPaneView::OnEnable(void){
	m_CheckBoxSendEnable.SetCheck(!m_CheckBoxSendEnable.GetCheck());
}
//------------------------------------------------------------------------------------------//
void CSendPaneView::OnSend(void){
	std::wstring	wstrCommand;
	CString			cstrText;
	STDSTR			forPrint,forSend;

	m_CommandNode.Init();

	m_cEdit.GetWindowText(cstrText);
	wstrCommand = cstrText;
	m_CommandNode.StrCommand = Str_UnicodeToANSI(wstrCommand);

	m_TimeEdit.GetWindowText(cstrText);
	wstrCommand = cstrText;
	m_CommandNode.StrTimeout = Str_UnicodeToANSI(wstrCommand);
	m_CommandNode.StrCycle = "0";
	m_CommandNode.cmdTail = (CMD_TAIL)(m_CheckBoxCR.GetCheck());

	if (m_CheckBoxSendEnable.GetCheck()){
		if (theApp.GSDTApp.m_Script.Execute(&theApp.GSDTApp.m_Device1,&m_CommandNode) == 0)
			AfxMessageBox(_T("Script is running!"));
	}
	else{
		if (m_CheckBoxHex.GetCheck()){
			forPrint = Str_HEXToHEXs(m_CommandNode.StrCommand);
			forSend = Str_HEXToASCII(m_CommandNode.StrCommand);
			theApp.GSDTApp.m_Device1.PrintSendCommand(nullptr, forPrint, CMD_NONE, G_ESCAPE_OFF);
			theApp.GSDTApp.m_Device1.SendCommand(forSend);
		}
		else{
			theApp.GSDTApp.m_Device1.SendCommandWithPrint(m_CommandNode.StrCommand, m_CommandNode.cmdTail, (G_ESCAPE)m_CheckBoxEscape.GetCheck());
		}
	}
}
//------------------------------------------------------------------------------------------//
void CSendPaneView::OnDropFiles(HDROP hDropInfo){
	int				nFileCount;
	TCHAR			szFileName[MAX_PATH];
	CString			cFileName;
	std::wstring	wstrFileName;
	std::string		strFileName, strExtName;

	nFileCount = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, MAX_PATH);
	((CMDIChildWndEx*)theApp.GSDTApp.m_Cache.GetG1_STDOUT()->cgRichEdit->GetParentFrame())->MDIActivate();
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
			theApp.GSDTApp.m_FileSend.Execute(&theApp.GSDTApp.m_Device1,strFileName.c_str());
		}
	}
	DragFinish (hDropInfo);
}
//------------------------------------------------------------------------------------------//
void CSendPaneView::PrintFileName(HDROP hDropInfo){
	CString			cResult;
	std::wstring	wstrFileName;
	std::string		strPrint;
	TCHAR			szFileName[MAX_PATH];
	int				nFileCount;

	theApp.GSDTApp.m_Cache.GetG1_STDOUT()->Clean();

	nFileCount = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, MAX_PATH);

	strPrint = "Total ";
	strPrint += Str_ToStr(nFileCount);
	strPrint += " files selected, pls drop one file and try again.";
	theApp.GSDTApp.m_Cache.GetG1_STDOUT()->Write(nullptr,COL_clMaroon,strPrint);
	theApp.GSDTApp.m_Cache.GetG1_STDOUT()->WriteNL(nullptr,COL_DivLine,IUD(DIVIDING_LINE));
	for (int i = 0; i < nFileCount; ++ i){
		ZeroMemory(szFileName,MAX_PATH);
		DragQueryFile(hDropInfo,i,szFileName,MAX_PATH);
		cResult += szFileName;
		cResult += _T("\r\n");
	}
	wstrFileName = cResult;
	theApp.GSDTApp.m_Cache.GetG1_STDOUT()->WriteNL(nullptr,COL_clBlue,Str_UnicodeToANSI(wstrFileName));
	theApp.GSDTApp.m_Cache.GetG1_STDOUT()->WriteNL(nullptr,COL_DivLine, IUD(DIVIDING_LINE));
	theApp.GSDTApp.m_Cache.GetG1_STDOUT()->WriteNL(nullptr,COL_clMaroon, IUD("End"));
	theApp.GSDTApp.m_Cache.GetG1_STDOUT()->ToHome();
}
//------------------------------------------------------------------------------------------//