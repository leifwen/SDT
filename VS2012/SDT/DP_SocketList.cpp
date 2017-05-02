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
#include "DP_SocketList.h"
#include "ChildFrm.h"
#include "RichView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//------------------------------------------------------------------------------------------//
CSocketListCtrl	g_wndSocketList;
//------------------------------------------------------------------------------------------//
CSocketListCtrl::CSocketListCtrl(void) : CMyCListCtrl(){
	m_Font.CreateFont
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
		,_T(""));//Courier New Calibri
}
//------------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CSocketListCtrl,CMyCListCtrl)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()
//------------------------------------------------------------------------------------------//
int CSocketListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct){
	if (CMyCListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	SetFont(&m_Font);
	SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
	InsertColumn(0, _T("IP"), LVCFMT_LEFT, 120);
	InsertColumn(1, _T("Port"), LVCFMT_LEFT, 60);
	return 0;
}
//------------------------------------------------------------------------------------------//
void CSocketListCtrl::LoadData(ASOCKETSERVER *server){
	int32 i;

	m_Server = server;
	if (m_Server == NULL)
		return;

	if (m_Server->CheckUpdate() == 0)
		return;

	DeleteAllItems();
	m_SelectItem = -1;
	m_SelectSubItem = -1;

	i = 0;
	TREE_LChildRChain_Traversal_LINE(ASOCKET, m_Server,
		operateNode_t->Spin_InUse_set();
		if (operateNode_t->IsConnected() != 0){
			InsertItem(i, Str_ANSIToUnicode(operateNode_t->GetBufName()).c_str());
			SetItemText(i, 1, Str_ANSIToUnicode(Str_ToString(operateNode_t->GetBufPar())).c_str());
			SetItemData(i, TREE_NODE::GetdRNodeID(operateNode_t));
			SetCheck(i, operateNode_t->CheckSelected());
			++i;
		}
		operateNode_t->Spin_InUse_clr();
	);
	m_Server->ClrblUpdate();
}
//------------------------------------------------------------------------------------------//
void CSocketListCtrl::Disconnect(int disItem){
	ASOCKET		*nextSocket;

	if (m_Server == NULL)
		return;
	if (disItem < 0)
		return;

	nextSocket = (ASOCKET*)TREE_NODE::FindInLChildRChainByDRNodeID(m_Server, GetItemData(disItem));
	m_Server->ChildClose(nextSocket);
}
//------------------------------------------------------------------------------------------//
void CSocketListCtrl::OnLButtonDown(UINT nFlags, CPoint point){
	CMyCListCtrl::OnLButtonDown(nFlags,point);
	LVHITTESTINFO lvinfo;

	ASOCKET		*socketSelected;
	int32		i;

	lvinfo.pt = point;
	lvinfo.flags = LVHT_ABOVE;
     
	if (SubItemHitTest(&lvinfo) != -1){
		if (GetCheck(lvinfo.iItem)){
			i = 0;
			while(i ++ < GetItemCount())
				SetCheck(i - 1,FALSE);
			SetCheck(lvinfo.iItem);
			if (m_Server == NULL)
				return;
			socketSelected = (ASOCKET*)TREE_NODE::FindInLChildRChainByDRNodeID(m_Server, GetItemData(lvinfo.iItem));
			if (m_Server->GetSelDB() != socketSelected)
				m_Server->ChildSetSel(socketSelected);
		}
		else{
			SetCheck(lvinfo.iItem,FALSE);
			if (m_Server != NULL){
				socketSelected = (ASOCKET*)TREE_NODE::FindInLChildRChainByDRNodeID(m_Server, GetItemData(lvinfo.iItem));
				if (m_Server->GetSelDB() == socketSelected)
					m_Server->ChildClrSel(socketSelected);
			}
		}
	}
}
//------------------------------------------------------------------------------------------//
void CSocketListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point){
	CMyCListCtrl::OnLButtonDblClk(nFlags,point);
	ASOCKET		*aSocket;

	if (m_SelectItem < 0)
		return;
	if (m_Server == NULL)
		return;
	aSocket = (ASOCKET*)TREE_NODE::FindInLChildRChainByDRNodeID(m_Server, GetItemData(m_SelectItem));
	if (aSocket != nullptr){
		if (aSocket->GetG3DefSTDOUT() == nullptr){
			CreateScoketView(aSocket);
		}
		else{
			((CChildFrame*)(aSocket->GetG3DefSTDOUT()->cgRichEdit->GetParentFrame()))->MDIActivate();
		}
	}
}
//------------------------------------------------------------------------------------------//
void CSocketListCtrl::CreateScoketView(ASOCKET *aSocket){
	CChildFrame		*childpFrm;
	CCreateContext	context;
	CString			cTitle;

	context.m_pNewViewClass = RUNTIME_CLASS(CMyRichView);
	childpFrm = new CChildFrame();
	childpFrm->LoadFrame(IDI_SOCKETFRM,WS_CHILD | WS_OVERLAPPEDWINDOW,theApp.m_pMainWnd,&context);
	childpFrm->ShowWindow(SW_SHOW);
	childpFrm->InitialUpdateFrame(NULL,true);
	((CMyRichView*)childpFrm->GetActiveView())->DragAcceptFiles(FALSE);
	childpFrm->m_bCanClose = TRUE;
	childpFrm->m_selfName = "Socket";
	cTitle = Str_ANSIToUnicode(aSocket->GetDevName()).c_str();
	childpFrm->SetWindowTextW(cTitle);

	childpFrm->cstdout.Init((CRichEditView*)childpFrm->GetActiveView(), nullptr);
	aSocket->AddG3D_STDOUT(&childpFrm->cstdout);
	childpFrm->m_bCanClose = TRUE;
}
//------------------------------------------------------------------------------------------//




//------------------------------------------------------------------------------------------//
CSocketListViewDP::CSocketListViewDP(void){
}
//------------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CSocketListViewDP, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_WM_SETFOCUS()
	ON_WM_PAINT()
	ON_COMMAND(ID_TOOLBAR_SOCKET_DISCONNECT, &CSocketListViewDP::OnDisconnect)
END_MESSAGE_MAP()
//------------------------------------------------------------------------------------------//
CSocketListViewDP::~CSocketListViewDP(void){
}
//------------------------------------------------------------------------------------------//
int CSocketListViewDP::OnCreate(LPCREATESTRUCT lpCreateStruct){
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (CreateScoketList() == -1)
		return -1;
	if (CreateToolBar() == -1)
		return -1;
	return 0;
}
//------------------------------------------------------------------------------------------//
void CSocketListViewDP::AdjustLayout(){
	if (GetSafeHwnd() == NULL)
		return;

	CRect rectClient;
	GetClientRect(rectClient);
	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	g_wndSocketList.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}
//------------------------------------------------------------------------------------------//
void CSocketListViewDP::OnSize(UINT nType, int cx, int cy){
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}
//------------------------------------------------------------------------------------------//
void CSocketListViewDP::OnSetFocus(CWnd* pOldWnd){
	CDockablePane::OnSetFocus(pOldWnd);
	g_wndSocketList.SetFocus();
}
//------------------------------------------------------------------------------------------//
void CSocketListViewDP::OnPaint(void){
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	g_wndSocketList.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}
//------------------------------------------------------------------------------------------//
void CSocketListViewDP::OnContextMenu(CWnd* pWnd, CPoint point){
	CTreeCtrl* pWndTree = (CTreeCtrl*)&g_wndSocketList;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree){
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}
}
//------------------------------------------------------------------------------------------//
int CSocketListViewDP::CreateToolBar(void){
	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_TOOLBAR_SOCKET);
	m_wndToolBar.LoadToolBar(IDR_TOOLBAR_SOCKET, 0, 0, TRUE /* Is locked */);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);
	
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);// All commands will be routed via this control , not via the parent frame:

	return 0;
}
//------------------------------------------------------------------------------------------//
void CSocketListViewDP::OnChangeVisualStyle(void){
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_SOCKET_HC : IDR_TOOLBAR_SOCKET, 0, 0, TRUE /* Locked */);
}
//------------------------------------------------------------------------------------------//
int CSocketListViewDP::CreateScoketList(void){
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	if (!g_wndSocketList.Create(WS_CHILD | WS_VISIBLE | LVS_REPORT, rectDummy, this, 0)){
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}
	g_wndSocketList.LoadData(NULL);
	return 0;
}
//------------------------------------------------------------------------------------------//
void CSocketListViewDP::OnDisconnect(void){
	g_wndSocketList.Disconnect(g_wndSocketList.GetSelectedNode());
}
//------------------------------------------------------------------------------------------//
