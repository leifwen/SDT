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
#include "CMUXView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//------------------------------------------------------------------------------------------//
IMPLEMENT_DYNAMIC(CMyCStatic, CStatic)
BEGIN_MESSAGE_MAP(CMyCStatic, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()
//------------------------------------------------------------------------------------------//
void CMyCStatic::OnPaint(void){
	CPaintDC dc(this);
	CRect client_rect;
	GetClientRect(client_rect);

	CString szText;
	GetWindowText(szText);

	CFont *pFont, *pOldFont;
	pFont = GetFont();
	pOldFont = dc.SelectObject(pFont);

	dc.SetBkMode(TRANSPARENT);
	dc.DrawText(szText, client_rect, 0);
	dc.SelectObject(pOldFont);
}
//------------------------------------------------------------------------------------------//
void CMyCStatic::SetValue(std::string& tText){
	CString	cText,cText1;

	cText = Str_ANSIToUnicode(tText).c_str();
	GetWindowText(cText1);
	if (cText != cText1){
		ShowWindow(SW_HIDE);
		SetWindowText(cText);
		ShowWindow(SW_SHOW);
	}
}
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
IMPLEMENT_DYNCREATE(CCMUXView, CView)
//------------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CCMUXView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()
//------------------------------------------------------------------------------------------//
CCMUXView::CCMUXView(void) : CView(){
	m_ViewFont.CreateFont
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
		,_T("宋体"));				//Courier New
}
//------------------------------------------------------------------------------------------//
int CCMUXView::OnCreate(LPCREATESTRUCT lpCreateStruct){
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	RevokeDragDrop(this->GetSafeHwnd());
	DragAcceptFiles(FALSE);
	SetFont(&m_ViewFont);
	if (CreateEdit() == -1)
		return -1;
	AdjustLayout();
	return 0;
}
//------------------------------------------------------------------------------------------//
void CCMUXView::OnSize(UINT nType, int cx, int cy){
	CView::OnSize(nType, cx, cy);
	AdjustLayout();
}
//------------------------------------------------------------------------------------------//
void CCMUXView::OnDraw(CDC* pDC){
}
//------------------------------------------------------------------------------------------//
void CCMUXView::OnContextMenu(CWnd* /* pWnd */, CPoint point){
}
//------------------------------------------------------------------------------------------//
void  CCMUXView::RefreshCOM(void){
	m_CMUXCOMCtrl1.RefreshCOM();
	m_CMUXCOMCtrl2.RefreshCOM();
	m_CMUXCOMCtrl3.RefreshCOM();
	m_CMUXCOMCtrl4.RefreshCOM();
}
//------------------------------------------------------------------------------------------//
#ifdef _DEBUG
void CCMUXView::AssertValid() const{
	CView::AssertValid();
}
//------------------------------------------------------------------------------------------//
void CCMUXView::Dump(CDumpContext& dc) const{
	CView::Dump(dc);
}
#endif //_DEBUG
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int CCMUXView::CreateEdit(void){
	CRect rectDummy;
	rectDummy.SetRectEmpty();
	m_CMUXCOMCtrl1.m_ComNumber = 1;
	m_CMUXCOMCtrl1.m_Title = _T("Virtual Port #1");
	m_CMUXCOMCtrl2.m_ComNumber = 2;
	m_CMUXCOMCtrl2.m_Title = _T("Virtual Port #2");
	m_CMUXCOMCtrl3.m_ComNumber = 3;
	m_CMUXCOMCtrl3.m_Title = _T("Virtual Port #3");
	m_CMUXCOMCtrl4.m_ComNumber = 4;
	m_CMUXCOMCtrl4.m_Title = _T("Virtual Port #4");
	if (!m_CMUXCOMCtrl1.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rectDummy,this,0)){
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}
	if (!m_CMUXCOMCtrl2.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rectDummy,this,0)){
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}
	if (!m_CMUXCOMCtrl3.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rectDummy,this,0)){
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}
	if (!m_CMUXCOMCtrl4.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rectDummy,this,0)){
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
void CCMUXView::AdjustLayout(void){
	if (GetSafeHwnd() == NULL)
		return;

	CRect rectS,rectT;
	GetClientRect(rectS);

	rectS = rectT;
	rectT.left += 5;
	rectT.right = rectT.left + 180;
	rectT.bottom = rectT.top + 200;
	m_CMUXCOMCtrl1.SetWindowPos(NULL, rectT.left, rectT.top, rectT.Width(), rectT.Height(), SWP_NOACTIVATE | SWP_NOZORDER);

	rectT.left = rectT.right;
	rectT.right = rectT.left + 180;
	m_CMUXCOMCtrl2.SetWindowPos(NULL, rectT.left, rectT.top, rectT.Width(), rectT.Height(), SWP_NOACTIVATE | SWP_NOZORDER);

	rectT.left = rectS.left + 5;
	rectT.right = rectT.left + 180;
	rectT.top = rectT.bottom;
	rectT.bottom = rectT.top + 200;
	m_CMUXCOMCtrl3.SetWindowPos(NULL, rectT.left, rectT.top, rectT.Width(), rectT.Height(), SWP_NOACTIVATE | SWP_NOZORDER);

	rectT.left = rectT.right;
	rectT.right = rectT.left + 180;
	m_CMUXCOMCtrl4.SetWindowPos(NULL, rectT.left, rectT.top, rectT.Width(), rectT.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
}
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
IMPLEMENT_DYNCREATE(CCMUXCOMCtrl, CView)
//------------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CCMUXCOMCtrl, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_MOUSEACTIVATE()
	ON_CBN_SELCHANGE(ID_CMUXCTRL_COM, &CCMUXCOMCtrl::OnCOMBO_COM)
	ON_CBN_DROPDOWN(ID_CMUXCTRL_COM, &CCMUXCOMCtrl::CreateComboBox_COMList)
	ON_COMMAND(ID_CMUXCTRL_OPEN, &CCMUXCOMCtrl::OnOpen)
	ON_COMMAND(ID_CMUXCTRL_DTR, &CCMUXCOMCtrl::OnDTR)
	ON_COMMAND(ID_CMUXCTRL_RTS, &CCMUXCOMCtrl::OnRTS)
	ON_COMMAND(ID_CMUXCTRL_HEX, &CCMUXCOMCtrl::OnHEX)
	ON_COMMAND(ID_CMUXCTRL_ESCAPE, &CCMUXCOMCtrl::OnEscape)
END_MESSAGE_MAP()
//------------------------------------------------------------------------------------------//
CCMUXCOMCtrl::CCMUXCOMCtrl(void){
	m_ViewFont.CreateFont
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
		,_T("Tahoma"));				//Courier New
	m_Title = "";
	m_ComNumber = 0;
	m_blConnect = FALSE;
#ifdef	SWVERSION_CMUX
	m_CMUXCOM = NULL;
#endif
}
//------------------------------------------------------------------------------------------//
int CCMUXCOMCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct){
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	RevokeDragDrop(this->GetSafeHwnd());
	DragAcceptFiles(FALSE);
	SetFont(&m_ViewFont);

	if (CreateEdit() == -1)
		return -1;
	InitBR();
	InitComboBox_COM();
	m_ComboCOM.SetCurSel(0);
	OnCOMBO_COM();
	AdjustLayout();
	return 0;
}
//------------------------------------------------------------------------------------------//
void CCMUXCOMCtrl::OnSize(UINT nType, int cx, int cy){
	CView::OnSize(nType, cx, cy);
	AdjustLayout();
}
//------------------------------------------------------------------------------------------//
void CCMUXCOMCtrl::OnDraw(CDC* pDC){
}
//------------------------------------------------------------------------------------------//
void CCMUXCOMCtrl::OnContextMenu(CWnd* /* pWnd */, CPoint point){
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#ifdef _DEBUG
void CCMUXCOMCtrl::AssertValid() const{
	CView::AssertValid();
}
//------------------------------------------------------------------------------------------//
void CCMUXCOMCtrl::Dump(CDumpContext& dc) const{
	CView::Dump(dc);
}
#endif //_DEBUG
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int CCMUXCOMCtrl::CreateEdit(void){
	CRect rectDummy;
	rectDummy.SetRectEmpty();
	if (!m_GroupSetting.Create(m_Title,WS_CHILD|WS_VISIBLE|BS_GROUPBOX, rectDummy, this, 0)){
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}
	if (!m_CheckDSR.Create(_T("DSR"),WS_CHILD|WS_VISIBLE|BS_CHECKBOX, rectDummy, this, ID_CMUXCTRL_DSR)){
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}
	if (!m_CheckCTS.Create(_T("CTS"),WS_CHILD|WS_VISIBLE|BS_CHECKBOX, rectDummy, this, ID_CMUXCTRL_CTS)){
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}
	if (!m_CheckDCD.Create(_T("DCD"),WS_CHILD|WS_VISIBLE|BS_CHECKBOX, rectDummy, this, ID_CMUXCTRL_DCD)){
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}
	if (!m_CheckRING.Create(_T("RING"),WS_CHILD|WS_VISIBLE|BS_CHECKBOX, rectDummy, this, ID_CMUXCTRL_RING)){
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}
	if (!m_CheckDTR.Create(_T("DTR"),WS_CHILD|WS_VISIBLE|BS_CHECKBOX, rectDummy, this, ID_CMUXCTRL_DTR)){
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}
	if (!m_CheckRTS.Create(_T("RTS"),WS_CHILD|WS_VISIBLE|BS_CHECKBOX, rectDummy, this, ID_CMUXCTRL_RTS)){
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}
	if (!m_CheckHEX.Create(_T("HEX"),WS_CHILD|WS_VISIBLE|BS_CHECKBOX, rectDummy, this, ID_CMUXCTRL_HEX)){
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}
	if (!m_CheckEscape.Create(_T("Escape"),WS_CHILD|WS_VISIBLE|BS_CHECKBOX, rectDummy, this, ID_CMUXCTRL_ESCAPE)){
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}
	if (!m_ComboCOM.Create(WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST, rectDummy, this, ID_CMUXCTRL_COM)){
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}
	if (!m_ComboBR.Create(WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST, rectDummy, this, ID_CMUXCTRL_BR)){
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}
	if (!m_ButtonOpen.Create(_T("Open"),WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, rectDummy, this, ID_CMUXCTRL_OPEN)){
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}
	if (!m_StaticTx.Create(_T("Tx:"),WS_CHILD|WS_VISIBLE|SS_NOPREFIX, rectDummy, this, 0)){
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}
	if (!m_StaticRx.Create(_T("Rx:"),WS_CHILD|WS_VISIBLE|SS_NOPREFIX, rectDummy, this, 0)){
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}
	m_GroupSetting.SetFont(&m_ViewFont);
	m_CheckDSR.SetFont(&m_ViewFont);
	m_CheckCTS.SetFont(&m_ViewFont);
	m_CheckDCD.SetFont(&m_ViewFont);
	m_CheckRING.SetFont(&m_ViewFont);
	m_CheckDTR.SetFont(&m_ViewFont);
	m_CheckRTS.SetFont(&m_ViewFont);
	m_CheckHEX.SetFont(&m_ViewFont);
	m_CheckEscape.SetFont(&m_ViewFont);
	m_ComboCOM.SetFont(&m_ViewFont);
	m_ComboBR.SetFont(&m_ViewFont);
	m_ButtonOpen.SetFont(&m_ViewFont);
	m_StaticTx.SetFont(&m_ViewFont);
	m_StaticRx.SetFont(&m_ViewFont);

	m_CheckDSR.EnableWindow(FALSE);
	m_CheckCTS.EnableWindow(FALSE);
	m_CheckDCD.EnableWindow(FALSE);
	m_CheckRING.EnableWindow(FALSE);
	m_CheckDTR.EnableWindow(FALSE);
	m_CheckRTS.EnableWindow(FALSE);
	m_CheckEscape.SetCheck(TRUE);
	return 0;
}
//------------------------------------------------------------------------------------------//
void CCMUXCOMCtrl::AdjustLayout(void){
	CRect rectGroupS,rectGroupT;
	GetClientRect(rectGroupS);

	rectGroupS.InflateRect(-5, -5);
	rectGroupS.right = rectGroupS.left + 170;
	rectGroupS.bottom = rectGroupS.top + 190;
	m_GroupSetting.SetWindowPos(NULL, rectGroupS.left, rectGroupS.top, rectGroupS.Width(), rectGroupS.Height(), SWP_NOACTIVATE | SWP_NOZORDER);

	rectGroupT = rectGroupS;
	rectGroupT.left = rectGroupS.left + 10;
	rectGroupT.right = rectGroupT.left + 50;
	rectGroupT.top += 20;
	rectGroupT.bottom = rectGroupT.top + 20;
	m_StaticTx.SetWindowPos(NULL, rectGroupT.left, rectGroupT.top, rectGroupT.Width(), rectGroupT.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
	
	rectGroupT.left = rectGroupS.left + 90;
	rectGroupT.right = rectGroupT.left + 50;
	m_StaticRx.SetWindowPos(NULL, rectGroupT.left, rectGroupT.top, rectGroupT.Width(), rectGroupT.Height(), SWP_NOACTIVATE | SWP_NOZORDER);

	rectGroupT.left = rectGroupS.left + 10;
	rectGroupT.right = rectGroupT.left + 50;
	rectGroupT.top += 20;
	rectGroupT.bottom = rectGroupT.top + 20;
	m_CheckDSR.SetWindowPos(NULL, rectGroupT.left, rectGroupT.top, rectGroupT.Width(), rectGroupT.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
	
	rectGroupT.left = rectGroupS.left + 90;
	rectGroupT.right = rectGroupT.left + 50;
	m_CheckDTR.SetWindowPos(NULL, rectGroupT.left, rectGroupT.top, rectGroupT.Width(), rectGroupT.Height(), SWP_NOACTIVATE | SWP_NOZORDER);

	rectGroupT.left = rectGroupS.left + 10;
	rectGroupT.right = rectGroupT.left + 50;
	rectGroupT.top += 20;
	rectGroupT.bottom = rectGroupT.top + 20;
	m_CheckCTS.SetWindowPos(NULL, rectGroupT.left, rectGroupT.top, rectGroupT.Width(), rectGroupT.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
	
	rectGroupT.left = rectGroupS.left + 90;
	rectGroupT.right = rectGroupT.left + 50;
	m_CheckRTS.SetWindowPos(NULL, rectGroupT.left, rectGroupT.top, rectGroupT.Width(), rectGroupT.Height(), SWP_NOACTIVATE | SWP_NOZORDER);

	rectGroupT.left = rectGroupS.left + 10;
	rectGroupT.right = rectGroupT.left + 50;
	rectGroupT.top += 20;
	rectGroupT.bottom = rectGroupT.top + 20;
	m_CheckDCD.SetWindowPos(NULL, rectGroupT.left, rectGroupT.top, rectGroupT.Width(), rectGroupT.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
	
	rectGroupT.left = rectGroupS.left + 90;
	rectGroupT.right = rectGroupT.left + 50;
	m_CheckHEX.SetWindowPos(NULL, rectGroupT.left, rectGroupT.top, rectGroupT.Width(), rectGroupT.Height(), SWP_NOACTIVATE | SWP_NOZORDER);

	rectGroupT.left = rectGroupS.left + 10;
	rectGroupT.right = rectGroupT.left + 50;
	rectGroupT.top += 20;
	rectGroupT.bottom = rectGroupT.top + 20;
	m_CheckRING.SetWindowPos(NULL, rectGroupT.left, rectGroupT.top, rectGroupT.Width(), rectGroupT.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
	
	rectGroupT.left = rectGroupS.left + 90;
	rectGroupT.right = rectGroupT.left + 50;
	m_CheckEscape.SetWindowPos(NULL, rectGroupT.left, rectGroupT.top, rectGroupT.Width(), rectGroupT.Height(), SWP_NOACTIVATE | SWP_NOZORDER);

	rectGroupT.left = rectGroupS.left + 10;
	rectGroupT.right = rectGroupT.left + 70;
	rectGroupT.top += 25;
	rectGroupT.bottom = rectGroupT.top + 20;
	m_ComboCOM.SetWindowPos(NULL, rectGroupT.left, rectGroupT.top, rectGroupT.Width(), rectGroupT.Height(), SWP_NOACTIVATE | SWP_NOZORDER);

	rectGroupT.left = rectGroupS.left + 100;
	rectGroupT.right = rectGroupT.left + 60;
	rectGroupT.bottom = rectGroupT.top + 50;
	m_ButtonOpen.SetWindowPos(NULL, rectGroupT.left, rectGroupT.top, rectGroupT.Width(), rectGroupT.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
	
	rectGroupT.left = rectGroupS.left + 10;
	rectGroupT.right = rectGroupT.left + 70;
	rectGroupT.top += 30;
	rectGroupT.bottom = rectGroupT.top + 20;
	m_ComboBR.SetWindowPos(NULL, rectGroupT.left, rectGroupT.top, rectGroupT.Width(), rectGroupT.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
}
//------------------------------------------------------------------------------------------//
void CCMUXCOMCtrl::InitBR(void){
	m_ComboBR.InsertString(0,_T("1200"));
	m_ComboBR.InsertString(1,_T("2400"));
	m_ComboBR.InsertString(2,_T("4800"));
	m_ComboBR.InsertString(3,_T("9600"));
	m_ComboBR.InsertString(4,_T("14400"));
	m_ComboBR.InsertString(5,_T("19200"));
	m_ComboBR.InsertString(6,_T("38400"));
	m_ComboBR.InsertString(7,_T("56000"));
	m_ComboBR.InsertString(8,_T("57600"));
	m_ComboBR.InsertString(9,_T("115200"));
	m_ComboBR.InsertString(10,_T("128000"));
	m_ComboBR.InsertString(10,_T("230400"));
	m_ComboBR.InsertString(12,_T("256000"));
	m_ComboBR.InsertString(13,_T("460800"));
	m_ComboBR.InsertString(14,_T("912600"));
	m_ComboBR.SetCurSel(9);
}
//------------------------------------------------------------------------------------------//
void CCMUXCOMCtrl::InitComboBox_COM(void){
	CreateComboBox_COMList();
	m_ValidAuxComList.selectedNode = (IPCOMNAME*)TNF::GetTail(TNF::GetDown(&m_ValidAuxComList));
	SetSelectComboBox_COM();
}
//------------------------------------------------------------------------------------------//
void CCMUXCOMCtrl::SetSelectComboBox_COM(void){
	CString		sstrPortName,sstrShowName,sstrBR;
	IPCOMNAME	*listItem;
	int			id;

	if (m_ValidAuxComList.selectedNode == NULL)
		return;

	listItem = m_ValidAuxComList.selectedNode;
	
	if (listItem->typeID == COMENUM_DEVID_APICOM)
		sstrPortName = listItem->strIPComName.c_str();
	if (listItem->typeID == COMENUM_DEVID_TCPClient){
		sstrPortName = "TCP:";
		sstrPortName += listItem->strIPComName.c_str();
	}
	if (listItem->typeID == COMENUM_DEVID_UDPClient){
		sstrPortName = "UDP:";
		sstrPortName += listItem->strIPComName.c_str();
	}
	if (listItem->typeID == COMENUM_DEVID_TCPServer)
		sstrPortName = "TCP SERVER";
	if (listItem->typeID == COMENUM_DEVID_UDPServer)
		sstrPortName = "UDP SERVER";
	sstrShowName = listItem->strShowName.c_str();
	m_ComboCOM.SetWindowText(sstrPortName);
	sstrBR = Str_ToStr(listItem->portBaudrate).c_str();
	id = m_ComboBR.FindString(0,sstrBR);
	m_ComboBR.SetCurSel(id);
	m_ComboCOM.SetDroppedWidth(300);
}
//---------------------------------------------------------------------------
void CCMUXCOMCtrl::CreateComboBox_COMList(void){
	CString		sstrPortName,sstrShowName,sstrBackup;
	int32		i;

	m_ComboCOM.GetWindowText(sstrBackup);
	m_ComboCOM.ResetContent();

	theApp.GSDTApp.m_IPComList.Refresh();
	theApp.GSDTApp.m_IPComList.ExportCOM(&m_ValidAuxComList);

	i = 0;
	TREE_DownChain_Traversal_LINE(IPCOMNAME, (&m_ValidAuxComList),
		if (_opNode->typeID == COMENUM_DEVID_APICOM)
			sstrPortName = _opNode->strIPComName.c_str();
		if (_opNode->typeID == COMENUM_DEVID_TCPClient){
			sstrPortName = "TCP:";
			sstrPortName += _opNode->strIPComName.c_str();
		}
		if (_opNode->typeID == COMENUM_DEVID_UDPClient){
			sstrPortName = "UDP:";
			sstrPortName += _opNode->strIPComName.c_str();
		}
		if (_opNode->typeID == COMENUM_DEVID_TCPServer)
			sstrPortName = "TCP SERVER";
		if (_opNode->typeID == COMENUM_DEVID_UDPServer)
			sstrPortName = "UDP SERVER";
		sstrShowName = _opNode->strShowName.c_str();
		m_ComboCOM.InsertString(i,sstrShowName);
		m_ComboCOM.SetItemData(i, TNF::GetDRNodeID(_opNode));
		++ i;
		if (sstrBackup == sstrShowName)
			m_ValidAuxComList.selectedNode = _opNode;
	);

	if (m_ValidAuxComList.selectedNode != NULL){
		sstrShowName = m_ValidAuxComList.selectedNode->strShowName.c_str();
		m_ComboCOM.SetCurSel(m_ComboCOM.FindString(0,sstrShowName));
	}
	m_ComboCOM.SetWindowText(sstrBackup);
}
//------------------------------------------------------------------------------------------//
void CCMUXCOMCtrl::OnCOMBO_COM(void){
	int	id;
	CString		cText;

	m_ComboCOM.GetWindowText(cText);
	id = m_ComboCOM.FindString(0,cText);
	if (id >= 0){
		id = m_ComboCOM.GetItemData(id);
		m_ValidAuxComList.selectedNode = (IPCOMNAME*)m_ValidAuxComList.FindInDownChainByDRNodeID(&m_ValidAuxComList,id);
		SetSelectComboBox_COM();
	}
	else{
		m_ValidAuxComList.selectedNode = NULL;
	}
}
//------------------------------------------------------------------------------------------//
void CCMUXCOMCtrl::OnOpen(void){
#ifdef	SWVERSION_CMUX
	CString			cText;
	std::wstring	wsText;
	std::string		sText;
	int32			br;

	if (theApp.GSDTApp.m_CMUXDriver.IsOpened() == 0){
		m_CMUXCOM = NULL;
		m_blConnect = FALSE;
		m_ButtonOpen.SetWindowText(_T("Open"));
		m_CheckDTR.EnableWindow(FALSE);
		m_CheckRTS.EnableWindow(FALSE);
		m_ComboCOM.EnableWindow();
		m_ComboBR.EnableWindow();
		return;
	}
	m_ComboBR.GetWindowText(cText);
	wsText = cText;
	sText = Str_UnicodeToANSI(wsText);
	br = atoi(sText.c_str());

	m_ComboCOM.GetItemData(m_ComboCOM.GetCurSel());
	wsText = cText;
	sText = Str_UnicodeToANSI(wsText);
	if (m_blConnect == FALSE){
		m_CMUXCOM = theApp.GSDTApp.m_CMUXDriver.OpenVCOM(m_ComNumber,m_ValidAuxComList.selectedNode->strIPComName,m_ValidAuxComList.selectedNode->portBaudrate);
		if (m_CMUXCOM != NULL){
			m_blConnect = TRUE;
			m_ButtonOpen.SetWindowText(_T("Close"));
			m_CheckDTR.EnableWindow();
			m_CheckRTS.EnableWindow(); 
			m_ComboCOM.EnableWindow(FALSE);
			m_ComboBR.EnableWindow(FALSE);
		}
	}
	else{
		theApp.GSDTApp.m_CMUXDriver.CloseVCOM(m_ComNumber);
		m_CMUXCOM = NULL;
		m_blConnect = FALSE;
		m_ButtonOpen.SetWindowText(_T("Open"));
		m_CheckDTR.EnableWindow(FALSE);
		m_CheckRTS.EnableWindow(FALSE);
		m_ComboCOM.EnableWindow();
		m_ComboBR.EnableWindow();
	}
#endif
}
//------------------------------------------------------------------------------------------//
void CCMUXCOMCtrl::OnDTR(void){
#ifdef	SWVERSION_CMUX
	if (theApp.GSDTApp.m_CMUXDriver.IsOpened() == 0){
		m_CMUXCOM = NULL;
		m_CheckDTR.EnableWindow(FALSE);
		return;
	}
	if (m_CMUXCOM == NULL)
		return;
	CORE_VCOM::Update_DTR(m_CMUXCOM,CORE_VCOM::Check_DTR(m_CMUXCOM) == 0);
	theApp.GSDTApp.m_CMUXDriver.SendMSC(m_ComNumber,CORE_VCOM::Check_DTR(m_CMUXCOM),CORE_VCOM::Check_RTS(m_CMUXCOM));
#endif
}
//------------------------------------------------------------------------------------------//
void CCMUXCOMCtrl::OnRTS(void){
#ifdef	SWVERSION_CMUX
	if (theApp.GSDTApp.m_CMUXDriver.IsOpened() == 0){
		m_CMUXCOM = NULL;
		m_CheckRTS.EnableWindow(FALSE);
		return;
	}
	if (m_CMUXCOM == NULL)
		return;
	CORE_VCOM::Update_RTS(m_CMUXCOM,CORE_VCOM::Check_RTS(m_CMUXCOM) == 0);
	theApp.GSDTApp.m_CMUXDriver.SendMSC(m_ComNumber,CORE_VCOM::Check_DTR(m_CMUXCOM),CORE_VCOM::Check_RTS(m_CMUXCOM));
#endif
}
//------------------------------------------------------------------------------------------//
void CCMUXCOMCtrl::OnHEX(void){
#ifdef	SWVERSION_CMUX
	if (theApp.GSDTApp.m_CMUXDriver.IsOpened() == 0){
		m_CMUXCOM = NULL;
		return;
	}
	if (m_CMUXCOM == NULL)
		return;
	CORE_VCOM::Update_HEX(m_CMUXCOM,CORE_VCOM::Check_HEX(m_CMUXCOM) == 0);
	if (CORE_VCOM::Check_HEX(m_CMUXCOM) != 0)
		CORE_VCOM::Update_Escape(m_CMUXCOM,0);
#endif
}
//------------------------------------------------------------------------------------------//
void CCMUXCOMCtrl::OnEscape(void){
#ifdef	SWVERSION_CMUX
	if (theApp.GSDTApp.m_CMUXDriver.IsOpened() == 0){
		m_CMUXCOM = NULL;
		return;
	}
	if (m_CMUXCOM == NULL)
		return;
	CORE_VCOM::Update_Escape(m_CMUXCOM,CORE_VCOM::Check_Escape(m_CMUXCOM) == 0);
	if (CORE_VCOM::Check_Escape(m_CMUXCOM) != 0)
		CORE_VCOM::Update_HEX(m_CMUXCOM,0);
#endif
}
//------------------------------------------------------------------------------------------//
void  CCMUXCOMCtrl::RefreshCOM(void){
#ifdef	SWVERSION_CMUX
	if (theApp.GSDTApp.m_CMUXDriver.IsOpened() != 0){
		if (m_CMUXCOM != NULL){
			if (m_CMUXCOM->IsOpened() != 0){
				std::string	strTemp;
				m_blConnect = TRUE;
				m_CheckDSR.SetCheck(CORE_VCOM::Check_DSR(m_CMUXCOM) != 0);
				m_CheckCTS.SetCheck(CORE_VCOM::Check_CTS(m_CMUXCOM) != 0);
				m_CheckDCD.SetCheck(CORE_VCOM::Check_DCD(m_CMUXCOM) != 0);
				m_CheckRING.SetCheck(CORE_VCOM::Check_RING(m_CMUXCOM) != 0);
					
				m_CheckDTR.SetCheck(CORE_VCOM::Check_DTR(m_CMUXCOM) != 0);
				m_CheckRTS.SetCheck(CORE_VCOM::Check_RTS(m_CMUXCOM) != 0);

				m_CheckEscape.SetCheck(CORE_VCOM::Check_Escape(m_CMUXCOM) != 0);
				m_CheckHEX.SetCheck(CORE_VCOM::Check_HEX(m_CMUXCOM) != 0);

				strTemp = "Rx:";
				strTemp += Str_ToStr(m_CMUXCOM->RxBytes());

				m_StaticRx.SetValue(strTemp);

				strTemp = "Tx:";
				strTemp += Str_ToStr(m_CMUXCOM->TxBytes());
				m_StaticTx.SetValue(strTemp);
				return;
			}
			else{
				theApp.GSDTApp.m_CMUXDriver.CloseVCOM(m_ComNumber);
			}
		}
	}
	if (m_blConnect == TRUE){
		m_CMUXCOM = NULL;
		m_blConnect = FALSE;
		m_ButtonOpen.SetWindowText(_T("Open"));
		m_CheckDTR.EnableWindow(FALSE);
		m_CheckRTS.EnableWindow(FALSE);
		m_ComboCOM.EnableWindow();
		m_ComboBR.EnableWindow();
	}
#endif
}
//------------------------------------------------------------------------------------------//