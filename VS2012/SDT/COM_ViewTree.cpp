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
/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: COM_ViewTree.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2013.12.23
*/
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "COM_ViewTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define ID_CLASS_MYTREE_EDIT             (0)
//------------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CMyCTreeShowCEdit, CEdit)
	ON_WM_CONTEXTMENU()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()
//------------------------------------------------------------------------------------------//
void CMyCTreeShowCEdit::OnKillFocus(CWnd* pNewWnd){
	CEdit::OnKillFocus(pNewWnd);
	((CMyCTreeCtrl*)GetParent())->DoDestroyEdit();
}
//------------------------------------------------------------------------------------------//
CMyCTreeCtrl::CMyCTreeCtrl(void){
	m_SelectItem = NULL;
	m_HotItem = NULL;
	m_blMouseTracking = FALSE;
	m_blEditShown = FALSE;
	m_EditItem = NULL;
	m_EditFont.CreateFont
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
		,_T(""));//Courier New
	m_DrawFont.CreateFont
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
		,_T(""));//Courier New
}
//------------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CMyCTreeCtrl,CTreeCtrl)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEHOVER()
	ON_WM_SETFOCUS()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_PAINT()
	//ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CMyCTreeCtrl::OnNMCustomdraw)
END_MESSAGE_MAP()
//------------------------------------------------------------------------------------------//
CMyCTreeCtrl::~CMyCTreeCtrl(void){
}
//------------------------------------------------------------------------------------------//
BOOL CMyCTreeCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult){
	BOOL bRes = CTreeCtrl::OnNotify(wParam, lParam, pResult);

	NMHDR* pNMHDR = (NMHDR*)lParam;
	ASSERT(pNMHDR != NULL);

	if (pNMHDR && pNMHDR->code == TTN_SHOW && GetToolTips() != NULL)
		GetToolTips()->SetWindowPos(&wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
	return bRes;
}
//------------------------------------------------------------------------------------------//
void CMyCTreeCtrl::OnPaint(){
	CRect		clientRect;
	CPaintDC	dc(this); // device context for painting
	CBitmap		bitmap;
	CDC			MemeDc;
	int			maxpos,curpos;

	maxpos = GetScrollLimit(SB_HORZ);
	curpos = GetScrollPos(SB_HORZ);

	GetClientRect(&clientRect);
	clientRect.right += maxpos;

	MemeDc.CreateCompatibleDC(&dc);
	bitmap.CreateCompatibleBitmap(&dc, clientRect.Width(),clientRect.Height());
	CBitmap *pOldBitmap = MemeDc.SelectObject(&bitmap);
	MemeDc.FillSolidRect(clientRect,RGB(255,255,255));
	DrawAllItem(MemeDc);
	
	dc.BitBlt(clientRect.left - curpos,clientRect.top,clientRect.Width(),clientRect.Height(),&MemeDc,0,0,SRCCOPY);
	MemeDc.SelectObject(pOldBitmap);
	MemeDc.DeleteDC();
}
//------------------------------------------------------------------------------------------//
void CMyCTreeCtrl::DrawAllItem(CDC &pDc){
	HTREEITEM	currentItem;
	pDc.SelectObject(&m_DrawFont);
	currentItem = GetFirstVisibleItem();
	while (currentItem != NULL){
		DrawOneItem(pDc,currentItem);
		currentItem = GetNextVisibleItem(currentItem);
	}
}
//------------------------------------------------------------------------------------------//
void CMyCTreeCtrl::DrawOneItem(CDC &pDc,HTREEITEM ItemNode){
	CRect	clientRect,textRect,BGRect;
	HTHEME	hTheme;
	int		maxpos,curpos;
	CString	cText;

	maxpos = GetScrollLimit(SB_HORZ);
	curpos = GetScrollPos(SB_HORZ);
	GetClientRect(&clientRect);

	GetItemRect(ItemNode,&BGRect,0);
	if (BGRect.top > clientRect.bottom)
		return;

	BGRect.right += maxpos;
	textRect = BGRect;
	
	++ BGRect.left;
	-- BGRect.right;
	
	hTheme = OpenThemeData(GetSafeHwnd(),L"LISTVIEW");
	if (ItemNode == m_SelectItem){
		DrawThemeBackground(hTheme,pDc,LVP_GROUPHEADER,LVGH_CLOSESELECTED,&BGRect,NULL);
	}
	else if (ItemNode == m_HotItem){
		DrawThemeBackground(hTheme,pDc,LVP_GROUPHEADER,LVGH_OPENSELECTEDNOTFOCUSEDHOT,&BGRect,NULL);
	}else{
		pDc.FillSolidRect(BGRect,RGB(255,255,255));
	}
	CloseThemeData(hTheme);

	textRect.left += CaclRectLeft(ItemNode);

	if(ItemHasChildren(ItemNode)){//has child
		DrawGroupExpand(pDc,textRect,GetItemState(ItemNode,TVIS_EXPANDED) & TVIS_EXPANDED);
	}
	else if (GetParentItem(ItemNode) != NULL){
		DrawCheckbox(pDc,textRect,GetCheck(ItemNode));
	}

	textRect.top += 2;
	textRect.left += 20;
	cText = GetItemText(ItemNode);
	pDc.SetBkMode(TRANSPARENT);
	pDc.DrawText(cText,cText.GetLength(),&textRect,DT_LEFT | DT_NOPREFIX);
}
//------------------------------------------------------------------------------------------//
/*void CMyCTreeCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult){
	NMTVCUSTOMDRAW *ptvTreeCtrl=(NMTVCUSTOMDRAW*)pNMHDR;
	if(ptvTreeCtrl->nmcd.dwDrawStage == CDDS_PREPAINT){//Before the painting cycle begins.
		*pResult = CDRF_NOTIFYITEMDRAW;
		return;
	}
	if(ptvTreeCtrl->nmcd.dwDrawStage == CDDS_ITEMPREPAINT){//Before an item is drawn.
		CDC			dc;
		CRect		textRect(ptvTreeCtrl->nmcd.rc);
		CRect		BGRect = textRect;
		CPoint		ptItem(textRect.left + 1,textRect.top + 1);
		HTREEITEM	hTreeItem = HitTest(ptItem);
		CString		strItem = GetItemText(hTreeItem);
		HTHEME		hTheme;

		dc.Attach(ptvTreeCtrl->nmcd.hdc);
		++ BGRect.left;
		-- BGRect.right;
		if(ptvTreeCtrl->nmcd.uItemState & CDIS_SELECTED){
			hTheme = OpenThemeData(GetSafeHwnd(),L"LISTVIEW");
			DrawThemeBackground(hTheme,dc,LVP_GROUPHEADER,LVGH_CLOSESELECTED,&BGRect,NULL);
			CloseThemeData(hTheme);
		}
		else{
			if (m_SelectItem == hTreeItem){
				hTheme = OpenThemeData(GetSafeHwnd(),L"LISTVIEW");
				DrawThemeBackground(hTheme,dc,LVP_GROUPHEADER,LVGH_CLOSESELECTED,&BGRect,NULL);
				CloseThemeData(hTheme);
			}
			else{
				dc.FillSolidRect(BGRect,RGB(255,255,255));
			}
		}

		textRect.left += CaclRectLeft(hTreeItem);
		if(ItemHasChildren(hTreeItem)){//has child
			if (hTreeItem != NULL){
				DrawGroupExpand(dc,textRect,GetItemState(hTreeItem,TVIS_EXPANDED) & TVIS_EXPANDED);
				textRect.left += 20;
			}
		}
		else{//no  child
			if (hTreeItem != NULL){
				DrawCheckbox(dc,textRect,GetCheck(hTreeItem));
				textRect.left += 20;
			}
		}
		hTheme = OpenThemeData(GetSafeHwnd(),L"TEXTSTYLE");
		DrawThemeText(hTheme,dc,TEXT_LABEL,0,strItem,-1,DT_LEFT | DT_VCENTER | DT_SINGLELINE,0,textRect);
		CloseThemeData(hTheme);
		dc.Detach();
		*pResult = CDRF_SKIPDEFAULT;//CDRF_DODEFAULT;			
		return;
	}
}*/
//------------------------------------------------------------------------------------------//
void CMyCTreeCtrl::DrawCheckbox(CDC &dc,CRect DrawRect,BOOL blChecked){
	HTHEME	hTheme;

	hTheme = OpenThemeData(GetSafeHwnd(),L"Button");

	DrawRect.right = DrawRect.left + 16;
	if (blChecked == 0){
		DrawThemeBackground(hTheme,dc,BP_CHECKBOX,CBS_UNCHECKEDNORMAL,&DrawRect,NULL);
	}
	else{
		DrawThemeBackground(hTheme,dc,BP_CHECKBOX,CBS_CHECKEDNORMAL,&DrawRect,NULL);
	}
	CloseThemeData(hTheme);
}
//------------------------------------------------------------------------------------------//
void CMyCTreeCtrl::DrawGroupExpand(CDC &dc,CRect DrawRect,BOOL blExpand){
	HTHEME	hTheme;

	hTheme = OpenThemeData(GetSafeHwnd(),L"EXPLORERBAR");

	DrawRect.right = DrawRect.left + 16;
	if (blExpand == 0){
		DrawThemeBackground(hTheme,dc,EBP_IEBARMENU, EBM_NORMAL,&DrawRect,NULL);
	}
	else{
		DrawThemeBackground(hTheme,dc,EBP_SPECIALGROUPEXPAND,EBSGE_NORMAL,&DrawRect,NULL);
	}
	CloseThemeData(hTheme);
}
//------------------------------------------------------------------------------------------//
INT32 CMyCTreeCtrl::CaclRectLeft(HTREEITEM treeItem){
	HTREEITEM	treeItemT;
	treeItemT = GetParentItem(treeItem);
	if (treeItemT == NULL)
		return(4);
	return(CaclRectLeft(treeItemT) + 16);
}
//------------------------------------------------------------------------------------------//
void CMyCTreeCtrl::OnMouseMove(UINT nFlags, CPoint point){
	if (m_blEditShown == FALSE){
		if (!m_blMouseTracking){
			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(tme);
			tme.hwndTrack = GetSafeHwnd();
			tme.dwFlags = TME_LEAVE;
			tme.dwHoverTime = 2000;
			m_blMouseTracking = _TrackMouseEvent(&tme);
			{
				CRect rc;
				GetItemRect(m_SelectItem,&rc,0);
				InvalidateRect(rc);
			}
		}
		Select(m_HotItem,TVGN_DROPHILITE);
		m_HotItem = HitTest(point);
		if (m_HotItem != m_SelectItem){
			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(tme);
			tme.hwndTrack = GetSafeHwnd();
			tme.dwFlags = TME_CANCEL | TME_HOVER;
			tme.dwHoverTime = 300;
			_TrackMouseEvent(&tme);
		}
		SetFocus();
	}
	CTreeCtrl::OnMouseMove(nFlags,point);
}
//------------------------------------------------------------------------------------------//
void CMyCTreeCtrl::OnMouseHover(UINT nFlags, CPoint point){
	if (HitTest(point) == m_SelectItem)
		DoShowEdit(m_SelectItem);
}
//------------------------------------------------------------------------------------------//
void CMyCTreeCtrl::OnMouseLeave(void){
	CRect rc;
	GetItemRect(m_HotItem,&rc,0);
	InvalidateRect(rc);
	m_HotItem = NULL;
	m_blMouseTracking = FALSE;
	CTreeCtrl::OnMouseLeave();
}
//------------------------------------------------------------------------------------------//
void CMyCTreeCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar){
	DoSaveEdit(FALSE);
	CTreeCtrl::OnVScroll(nSBCode,nPos,pScrollBar);
}
//------------------------------------------------------------------------------------------//
void CMyCTreeCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar){
	DoSaveEdit(FALSE);
	CTreeCtrl::OnHScroll(nSBCode,nPos,pScrollBar);
}
//------------------------------------------------------------------------------------------//
void CMyCTreeCtrl::OnLButtonDblClk(UINT nFlags, CPoint point){
	if (m_SelectItem != NULL)
		Expand(m_SelectItem,TVE_TOGGLE);
	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(tme);
	tme.hwndTrack = GetSafeHwnd();
	tme.dwFlags = TME_CANCEL | TME_HOVER;
	tme.dwHoverTime = 300;
	_TrackMouseEvent(&tme);
}
//------------------------------------------------------------------------------------------//
void CMyCTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point){
	HTREEITEM hItem;
	
	hItem = m_SelectItem;
	m_SelectItem = HitTest(point);
	m_HotItem = m_SelectItem;
	SelectItem(m_SelectItem);
	SetFocus();

	if (DoECClick(m_SelectItem,point) == FALSE){
		if (hItem == m_SelectItem){
			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(tme);
			tme.hwndTrack = GetSafeHwnd();
			tme.dwFlags = TME_HOVER;
			tme.dwHoverTime = 300;
			_TrackMouseEvent(&tme);
		}
	}
	DoSaveEdit(FALSE);
}
//------------------------------------------------------------------------------------------//
BOOL CMyCTreeCtrl::DoShowEdit(HTREEITEM hItem){
	if (m_blEditShown == FALSE){
		if (hItem != NULL){
			CRect editRect;

			m_EditItem = hItem;
			GetItemRect(m_EditItem,&editRect,0);
			editRect.left += CaclRectLeft(m_EditItem);
			editRect.left += 20;
			m_Edit.Create(ES_LEFT|ES_AUTOHSCROLL|WS_BORDER|WS_CHILD,editRect,this,ID_CLASS_MYTREE_EDIT);
			m_Edit.SetFont(&m_EditFont);
			m_Edit.SetWindowTextW(GetItemText(m_EditItem));
			m_Edit.SetSel(-1);
			m_Edit.ShowWindow(SW_SHOW);
			m_Edit.SetFocus();
			m_blEditShown = TRUE;
		}
	}
	return(m_blEditShown);
}
//------------------------------------------------------------------------------------------//
BOOL CMyCTreeCtrl::DoSaveEdit(BOOL blSave){
	if (m_blEditShown == TRUE){
		if (blSave == TRUE){
			CString cstrText;
			m_Edit.GetWindowText(cstrText); 
			SetItemText(m_EditItem,cstrText.Trim());
			OnEditSave(m_EditItem);
		}
		m_Edit.DestroyWindow();
		m_blEditShown = FALSE;
		return TRUE;
	}
	return FALSE;
}
//------------------------------------------------------------------------------------------//
void CMyCTreeCtrl::DoDestroyEdit(void){
	if (m_blEditShown == TRUE){
		m_Edit.DestroyWindow();
		m_blEditShown = FALSE;
	}
}
//------------------------------------------------------------------------------------------//
void CMyCTreeCtrl::OnSetFocus(CWnd* pOldWnd){
	if (m_blEditShown == TRUE)
		m_Edit.SetFocus();
	CTreeCtrl::OnSetFocus(pOldWnd);
}
//------------------------------------------------------------------------------------------//
BOOL CMyCTreeCtrl::PreTranslateMessage(MSG *pMsg){
	if (pMsg->message == WM_KEYDOWN){
		UINT  nCode = pMsg->wParam;
		if ((nCode == _T('A') || nCode == _T('C') || nCode == _T('X') || nCode == _T('V') || nCode == _T('Z'))
			&&(::GetKeyState(VK_CONTROL) & 0x8000)){
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return TRUE;
		}
		if (pMsg->wParam == VK_RETURN)
			return(DoSaveEdit());
		if (pMsg->wParam == VK_ESCAPE)
			return(DoSaveEdit(FALSE));
		if ((pMsg->wParam == VK_PRIOR) || (pMsg->wParam == VK_NEXT)
			|| (pMsg->wParam == VK_HOME) || (pMsg->wParam == VK_END))
			return TRUE;
		if (pMsg->wParam == VK_UP){
			if (m_blEditShown == TRUE)
				return FALSE;
			if (m_SelectItem != NULL){
				HTREEITEM hItem;
				hItem = GetNextItem(m_SelectItem,TVGN_PREVIOUS);
				if (hItem == NULL)
					hItem = GetNextItem(m_SelectItem,TVGN_PARENT);
				if (hItem != NULL)
					SelectItem(hItem);
			}
			return TRUE;
		}
		if (pMsg->wParam == VK_DOWN){
			if (m_blEditShown == TRUE)
				return FALSE;
			if (m_SelectItem != NULL){
				HTREEITEM hItem;
				if (ItemHasChildren(m_SelectItem)){
					hItem = GetNextItem(m_SelectItem,TVGN_CHILD);
				}
				else{
					hItem = GetNextItem(m_SelectItem,TVGN_NEXT);
					if (hItem == NULL){
						hItem = GetNextItem(m_SelectItem,TVGN_PARENT);
						hItem = GetNextItem(hItem,TVGN_NEXT);
					}
				}
				if (hItem != NULL)
					SelectItem(hItem);
			}
			return TRUE;
		}
		if (pMsg->wParam == VK_LEFT){
			if (m_blEditShown == TRUE)
				return FALSE;
			if (m_SelectItem != NULL){
				if (ItemHasChildren(m_SelectItem)){
					Expand(m_SelectItem,TVE_COLLAPSE);
				}
				else{
					HTREEITEM hItem;
					hItem = GetNextItem(m_SelectItem,TVGN_PREVIOUS);
					if (hItem == NULL)
						hItem = GetNextItem(m_SelectItem,TVGN_PARENT);
					if (hItem != NULL)
						SelectItem(hItem);
				}
			}
			return TRUE;
		}
		if (pMsg->wParam == VK_RIGHT){
			if (m_blEditShown == TRUE)
				return FALSE;
			if (m_SelectItem != NULL){
				if (ItemHasChildren(m_SelectItem)){
					if (TVIS_EXPANDED & GetItemState(m_SelectItem,TVIS_EXPANDED)){
						SelectItem(GetNextItem(m_SelectItem,TVGN_CHILD));
					}
					else{
						Expand(m_SelectItem,TVE_EXPAND);
					}
				}
				else{
					HTREEITEM hItem;
					hItem = GetNextItem(m_SelectItem,TVGN_NEXT);
					if (hItem != NULL)
						SelectItem(hItem);
				}
			}
			return TRUE;
		}
	}
	if (pMsg->message == WM_MOUSEWHEEL){
		if (m_blEditShown == TRUE)
			return TRUE;
	}
	return(CTreeCtrl::PreTranslateMessage(pMsg));
}
//------------------------------------------------------------------------------------------//
BOOL CMyCTreeCtrl::DoECClick(HTREEITEM hItem, CPoint point){
	INT32	offsize;
	CRect	textRect;
	if (hItem != NULL){
		GetItemRect(hItem,&textRect,0);
		offsize = CaclRectLeft(hItem);
		textRect.left += offsize;
		textRect.right = textRect.left + 16;
		if ((point.x >= textRect.left) && (point.x <= textRect.right)
			&& (point.y >= textRect.top) && (point.y <= textRect.bottom)){
			if(ItemHasChildren(hItem)){
				Expand(hItem,TVE_TOGGLE);
				return TRUE;
			}
			else{
				SetCheck(hItem,!GetCheck(hItem));
				OnEditCheckSave(hItem);
				return TRUE;
			}
		}
	}
	return FALSE;
}
//------------------------------------------------------------------------------------------//
void CMyCTreeCtrl::OnRButtonDown(UINT nFlags, CPoint point){
	m_SelectItem = HitTest(point);
	m_HotItem = m_SelectItem;
	SelectItem(m_SelectItem);
	SetFocus();
	DoSaveEdit(FALSE);

	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(tme);
	tme.hwndTrack = GetSafeHwnd();
	tme.dwFlags = TME_CANCEL | TME_HOVER;
	tme.dwHoverTime = 300;
	_TrackMouseEvent(&tme);
}
//------------------------------------------------------------------------------------------//
HTREEITEM CMyCTreeCtrl::AddNode(LPCTSTR lpszItem, BOOL blCheck, HTREEITEM hParent, HTREEITEM hInsertAfter){
	HTREEITEM	ret;

	ret = InsertItem(lpszItem, hParent, hInsertAfter);
	if (ret != NULL){
		if (blCheck){
			SetCheck(ret,TRUE);
		}
		else{
			SetCheck(ret,FALSE);
		}
	}
	return (ret);
}
//------------------------------------------------------------------------------------------//
BOOL CMyCTreeCtrl::SelectItem(HTREEITEM hItem){
	m_SelectItem = hItem;
	return(CTreeCtrl::SelectItem(hItem));
}
//------------------------------------------------------------------------------------------//
HTREEITEM CMyCTreeCtrl::GetSelectedNode(void){
	return(m_SelectItem);
}
//------------------------------------------------------------------------------------------//
void CMyCTreeCtrl::ExpandAllRoot(HTREEITEM rootItem){
	HTREEITEM	hItem;

	hItem = GetParentItem(rootItem);
	if (hItem == NULL){
		hItem = GetRootItem();
	}
	else{
		hItem = GetNextItem(hItem,TVGN_CHILD);
	}
	while(hItem != NULL){
		Expand(hItem,TVE_EXPAND);
		hItem = GetNextItem(hItem,TVGN_NEXT);
	}
	CTreeCtrl::SelectItem(NULL);
	SelectItem(m_SelectItem);
}
//------------------------------------------------------------------------------------------//
void CMyCTreeCtrl::CollapseAllRoot(HTREEITEM rootItem){
	HTREEITEM	hItem;

	hItem = GetParentItem(rootItem);
	if (hItem == NULL){
		hItem = GetRootItem();
	}
	else{
		hItem = GetNextItem(hItem,TVGN_CHILD);
	}
	while(hItem != NULL){
		Expand(hItem,TVE_COLLAPSE);
		hItem = GetNextItem(hItem,TVGN_NEXT);
	}
}
//------------------------------------------------------------------------------------------//
INT32 CMyCTreeCtrl::CheckNodeLevel(HTREEITEM hItem){
	if (hItem == NULL)
		return 0;
	return(CheckNodeLevel(GetParentItem(hItem)) + 1);
}
//------------------------------------------------------------------------------------------//
void CMyCTreeCtrl::DelGroupSubNode(HTREEITEM rootItem){
	if (rootItem == NULL)
		return;
	if (ItemHasChildren(rootItem)){
		HTREEITEM		hItem;
		hItem = GetNextItem(rootItem,TVGN_CHILD);
		while(hItem != NULL){
			DeleteItem(hItem);
			hItem = GetNextItem(rootItem,TVGN_CHILD);
		}
	}
}
//------------------------------------------------------------------------------------------//
