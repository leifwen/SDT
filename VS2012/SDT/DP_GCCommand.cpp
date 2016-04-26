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
#include "DP_GCCommand.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//------------------------------------------------------------------------------------------//
CGCCommandListCtrl	g_wndGCCommandList;
//------------------------------------------------------------------------------------------//
CGCCommandListCtrl::CGCCommandListCtrl(void) : CMyCListCtrl(){
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
BEGIN_MESSAGE_MAP(CGCCommandListCtrl,CMyCListCtrl)
	ON_WM_CREATE()
END_MESSAGE_MAP()
//------------------------------------------------------------------------------------------//
int CGCCommandListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct){
	if (CMyCListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	SetFont(&m_Font);
	SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
	InsertColumn(0, _T("Command"), LVCFMT_LEFT, 250);
	InsertColumn(1, _T("T"), LVCFMT_LEFT, 40);
	InsertColumn(2, _T("C"), LVCFMT_LEFT, 30);
	InsertColumn(3, _T("Continue"), LVCFMT_LEFT, 120);
	InsertColumn(4, _T("Resend"), LVCFMT_LEFT, 120);
	InsertColumn(5, _T("Stop"), LVCFMT_LEFT, 120);
	InsertColumn(6, _T("Catch"), LVCFMT_LEFT, 120);
	return 0;
}
//------------------------------------------------------------------------------------------//
void CGCCommandListCtrl::LoadData(COMMAND_GROUP *group,int32 blenforce){
	int32			i;
	if (group == NULL){
		DeleteAllItems();
		m_SelectItem = -1;
		m_SelectSubItem = -1;
		m_Group = NULL;
		return;
	}
	if ((m_Group != group) || (blenforce != 0)){
		DeleteAllItems();
		m_SelectItem = -1;
		m_SelectSubItem = -1;
		m_Group = group;

		m_Group->Spin_InUse_set();
		i = 0;
		RTREE_LChildRChain_Traversal_LINE(COMMAND_NODE,m_Group,
			InsertItem(i, Str_ANSIToUnicode(operateNode_t->StrCommand).c_str());
			SetCheck(i, (operateNode_t->blEnableSend != 0));
			SetItemText(i, 1, Str_ANSIToUnicode(operateNode_t->StrTimeout).c_str());
			if (operateNode_t->StrCycle != ""){
				SetItemText(i, 2, Str_ANSIToUnicode(operateNode_t->StrCycle).c_str());
			}
			else{
				SetItemText(i, 2, _T("1"));
			}
			SetItemText(i, 3, Str_ANSIToUnicode(operateNode_t->StrContinue).c_str());
			SetItemText(i, 4, Str_ANSIToUnicode(operateNode_t->StrResend).c_str());
			SetItemText(i, 5, Str_ANSIToUnicode(operateNode_t->StrStop).c_str());
			SetItemText(i, 6, Str_ANSIToUnicode(operateNode_t->StrCatch).c_str());
			SetItemData(i, RTREE_NODE::GetdRNodeID(operateNode_t));
			++i;
		);
		m_Group->Spin_InUse_clr();
	}
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CGCCommandViewDP::CGCCommandViewDP(void){
}
//------------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CGCCommandViewDP, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_WM_SETFOCUS()
	ON_WM_PAINT()
END_MESSAGE_MAP()
//------------------------------------------------------------------------------------------//
CGCCommandViewDP::~CGCCommandViewDP(void){
}
//------------------------------------------------------------------------------------------//
int CGCCommandViewDP::OnCreate(LPCREATESTRUCT lpCreateStruct){
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;
	if (CreateGCList() == -1)
		return -1;
	return 0;
}
//------------------------------------------------------------------------------------------//
void CGCCommandViewDP::AdjustLayout(){
	if (GetSafeHwnd() == NULL)
		return;

	CRect rectClient;
	GetClientRect(rectClient);

	g_wndGCCommandList.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + 1, rectClient.Width() - 2, rectClient.Height() - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}
//------------------------------------------------------------------------------------------//
void CGCCommandViewDP::OnSize(UINT nType, int cx, int cy){
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}
//------------------------------------------------------------------------------------------//
void CGCCommandViewDP::OnSetFocus(CWnd* pOldWnd){
	CDockablePane::OnSetFocus(pOldWnd);
	g_wndGCCommandList.SetFocus();
}
//------------------------------------------------------------------------------------------//
void CGCCommandViewDP::OnPaint(void){
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	g_wndGCCommandList.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}
//------------------------------------------------------------------------------------------//
void CGCCommandViewDP::OnContextMenu(CWnd* pWnd, CPoint point){
	CTreeCtrl* pWndTree = (CTreeCtrl*)&g_wndGCCommandList;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree){
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}
}
//------------------------------------------------------------------------------------------//
int CGCCommandViewDP::CreateGCList(void){
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	if (!g_wndGCCommandList.Create(WS_CHILD | WS_VISIBLE | LVS_REPORT, rectDummy, this, 0)){
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}
	g_wndGCCommandList.LoadData(NULL);
	return 0;
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
