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
//------------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CSocketListCtrl,CMyCListCtrl)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()
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
int	CSocketListCtrl::GetSelectedNode(void){
	return(m_SelectItem);
};
//------------------------------------------------------------------------------------------//
void CSocketListCtrl::LoadData(ASOCKETSERVER* server){
	int32 i;

	m_Server = server;
	if (m_Server == NULL)
		return;

	if (TNF::IsUpdate(m_Server) == 0)
		return;

	DeleteAllItems();
	m_SelectItem = -1;
	m_SelectSubItem = -1;

	i = 0;
	TREE_DownChain_Traversal_LINE(ASOCKET, m_Server,
		if (_opNode->IsOpened() != 0){
			InsertItem(i, Str_ANSIToUnicode(_opNode->Core()->GetOpenPar().name).c_str());
			SetItemText(i, 1, Str_ANSIToUnicode(Str_ToStr(_opNode->Core()->GetOpenPar().port)).c_str());
			SetItemData(i, TNF::GetDRNodeID(_opNode));
			SetCheck(i, COMMU_NODE::IsSelected(_opNode));
			++i;
		}
	);
	TNF::ClrblUpdate(m_Server);
}
//------------------------------------------------------------------------------------------//
void CSocketListCtrl::Disconnect(int disItem){
	ASOCKET		*nextSocket;

	if (m_Server == NULL)
		return;
	if (disItem < 0)
		return;

	nextSocket = (ASOCKET*)TNF::FindInDownChainByDRNodeID(m_Server, GetItemData(disItem));
	m_Server->CloseChild(nextSocket);
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
			socketSelected = (ASOCKET*)TNF::FindInDownChainByDRNodeID(m_Server, GetItemData(lvinfo.iItem));
			if (COMMU_NODE::IsSelected(socketSelected) == G_FALSE)
				m_Server->SetChildSelected(socketSelected);
		}
		else{
			SetCheck(lvinfo.iItem,FALSE);
			if (m_Server != NULL){
				socketSelected = (ASOCKET*)TNF::FindInDownChainByDRNodeID(m_Server, GetItemData(lvinfo.iItem));
				if (COMMU_NODE::IsSelected(socketSelected))
					m_Server->ClrChildSelected(socketSelected);
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
	aSocket = (ASOCKET*)TNF::FindInDownChainByDRNodeID(m_Server, GetItemData(m_SelectItem));
	if (aSocket != nullptr){
		if (COMMU_LOGSYS::GetG3DefSTDOUT(aSocket->unitTeam.logSys) == nullptr){
			CreateScoketView(aSocket);
		}
		else{
			((CChildFrame*)(COMMU_LOGSYS::GetG3DefSTDOUT(aSocket->unitTeam.logSys)->cgRichEdit->GetParentFrame()))->MDIActivate();
		}
	}
}
//------------------------------------------------------------------------------------------//
void CSocketListCtrl::CreateScoketView(ASOCKET *aSocket){
	CChildFrame		*childpFrm;
	CCreateContext	context;
	CString			cTitle;

	cTitle = Str_ANSIToUnicode(aSocket->Core()->GetDevName()).c_str();

	context.m_pNewViewClass = RUNTIME_CLASS(CMyRichView);
	childpFrm = new CChildFrame();
	childpFrm->LoadFrame(IDI_SOCKETFRM,WS_CHILD | WS_OVERLAPPEDWINDOW,theApp.m_pMainWnd,&context);
	childpFrm->ShowWindow(SW_SHOW);
	childpFrm->InitialUpdateFrame(NULL,true);

	childpFrm->SetWindowTextW(cTitle);
	childpFrm->GetActiveView()->DragAcceptFiles(FALSE);
	
	childpFrm->InitSTDOUT("Socket");
	childpFrm->m_bCanClose = TRUE;
	
	COMMU_LOGSYS::AddG3D_STDOUT(aSocket->unitTeam.logSys,&childpFrm->m_stdout);
}
//------------------------------------------------------------------------------------------//




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
	GetSocketList().SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}
//------------------------------------------------------------------------------------------//
void CSocketListViewDP::OnSize(UINT nType, int cx, int cy){
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}
//------------------------------------------------------------------------------------------//
void CSocketListViewDP::OnSetFocus(CWnd* pOldWnd){
	CDockablePane::OnSetFocus(pOldWnd);
	GetSocketList().SetFocus();
}
//------------------------------------------------------------------------------------------//
void CSocketListViewDP::OnPaint(void){
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	GetSocketList().GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}
//------------------------------------------------------------------------------------------//
void CSocketListViewDP::OnContextMenu(CWnd* pWnd, CPoint point){
	CTreeCtrl* pWndTree = (CTreeCtrl*)&GetSocketList();
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

	if (!GetSocketList().Create(WS_CHILD | WS_VISIBLE | LVS_REPORT, rectDummy, this, 0)){
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}
	GetSocketList().LoadData(NULL);
	return 0;
}
//------------------------------------------------------------------------------------------//
void CSocketListViewDP::OnDisconnect(void){
	GetSocketList().Disconnect(GetSocketList().GetSelectedNode());
}
//------------------------------------------------------------------------------------------//
CSocketListCtrl& CSocketListViewDP::GetSocketList(void){
	static CSocketListCtrl	sg_wndSocketList;
	return(sg_wndSocketList);
}
//------------------------------------------------------------------------------------------//
