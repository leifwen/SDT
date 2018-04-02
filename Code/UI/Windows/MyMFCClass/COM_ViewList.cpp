/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: COM_ViewList.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2013.12.23
*/
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "COM_ViewList.h"
//------------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CMyCListShowCEdit, CEdit)
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()
//------------------------------------------------------------------------------------------//
BOOL CMyCListShowCEdit::PreTranslateMessage(MSG* pMsg){
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











//------------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CMyCListCtrl, CListCtrl)
	ON_WM_CREATE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
END_MESSAGE_MAP()
//------------------------------------------------------------------------------------------//
CMyCListCtrl::CMyCListCtrl(void) : CListCtrl(){
	m_SelectItem = -1;
	m_SelectSubItem = -1;
	m_blEditShown = FALSE;
	m_blComboBoxShown = FALSE;
	m_EditFont.CreateFont
		(15							// nHeight
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
	m_ComboBoxFont.CreateFont
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
		,_T(""));//Courier New
}
//------------------------------------------------------------------------------------------//
int CMyCListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct){
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	return 0;
}
//------------------------------------------------------------------------------------------//
BOOL CMyCListCtrl::PreTranslateMessage(MSG* pMsg){
	if (pMsg->message == WM_KEYDOWN){
		UINT  nCode = pMsg->wParam;
		if ((nCode == _T('A') || nCode == _T('C') || nCode == _T('X') || nCode == _T('V') || nCode == _T('Z'))
			&&(::GetKeyState(VK_CONTROL) & 0x8000)){
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return TRUE;
		}
		switch(pMsg->wParam){
			case VK_RETURN:
				return(DoSaveEdit());
			case VK_ESCAPE:
				return(DoSaveEdit(FALSE));
			case VK_PRIOR:
			case VK_NEXT:
			case VK_HOME:
			case VK_END:
			case VK_UP:
			case VK_DOWN:
			case VK_LEFT:
			case VK_RIGHT:
				break;
		}
	}
	if (pMsg->message == WM_MOUSEWHEEL){
		if ((m_blEditShown == TRUE) || (m_blComboBoxShown == TRUE))
			return(TRUE);
	}
	return(CListCtrl::PreTranslateMessage(pMsg));
}
//------------------------------------------------------------------------------------------//
void CMyCListCtrl::OnMouseMove(UINT nFlags, CPoint point){
	DoSaveComboBox(TRUE);
	CListCtrl::OnMouseMove(nFlags,point);
}
//------------------------------------------------------------------------------------------//
void CMyCListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point){
	LVHITTESTINFO lvinfo;
	lvinfo.pt = point;
	lvinfo.flags = LVHT_ABOVE;
     
	if (SubItemHitTest(&lvinfo) != -1){
		if ((lvinfo.iItem == m_SelectItem) && (lvinfo.iSubItem == m_SelectSubItem))
			DoShowExItem(m_SelectItem,m_SelectSubItem);
	}
}
//------------------------------------------------------------------------------------------//
void CMyCListCtrl::OnLButtonDown(UINT nFlags, CPoint point){
	OnRButtonDown(nFlags,point);
}
//------------------------------------------------------------------------------------------//
void CMyCListCtrl::OnRButtonDown(UINT nFlags, CPoint point){
	CListCtrl::OnLButtonDown(nFlags,point);
	LVHITTESTINFO lvinfo;
	lvinfo.pt = point;
	lvinfo.flags = LVHT_ABOVE;
    
	if (SubItemHitTest(&lvinfo) != -1){
		SetItemState(m_SelectItem, 0, LVIS_SELECTED|LVIS_FOCUSED);
		m_SelectItem = lvinfo.iItem;
		m_SelectSubItem = lvinfo.iSubItem;
		SetItemState(m_SelectItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	}

	DoSaveEdit(FALSE);
	DoSaveComboBox(FALSE);
}
//------------------------------------------------------------------------------------------//
void CMyCListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar){
	DoSaveEdit(FALSE);
	DoSaveComboBox(FALSE);
	CListCtrl::OnVScroll(nSBCode,nPos,pScrollBar);
}
//------------------------------------------------------------------------------------------//
void CMyCListCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar){
	DoSaveEdit(FALSE);
	DoSaveComboBox(FALSE);
	CListCtrl::OnHScroll(nSBCode,nPos,pScrollBar);
}
//------------------------------------------------------------------------------------------//
BOOL CMyCListCtrl::DoShowExItem(int hItem,int hSubItem,int Type){
	if (Type == 0)
		return(DoShowEdit(hItem,hSubItem));
	if (Type == 1)
		return(DoShowComboBox(hItem,hSubItem));
	return FALSE;
}
//------------------------------------------------------------------------------------------//
BOOL CMyCListCtrl::DoShowEdit(int hItem,int hSubItem){
	if ((m_blEditShown == FALSE) && (m_blComboBoxShown == FALSE)){
		if (hItem != -1){
			CRect editRect;
			m_EditItem = hItem;
			m_EditSubItem = hSubItem;
			GetItemRect(m_EditItem,&editRect,0);
			ReCaclRect(m_EditSubItem,&editRect);
			m_Edit.Create(ES_LEFT|ES_AUTOHSCROLL|WS_BORDER|WS_CHILD,editRect,this,0);
			m_Edit.SetFont(&m_EditFont);
			m_Edit.SetWindowTextW(GetItemText(m_EditItem,m_EditSubItem));
			m_Edit.SetSel(-1);
			m_Edit.ShowWindow(SW_SHOW);
			m_Edit.SetFocus();
			m_blEditShown = TRUE;
		}
	}
	return(m_blEditShown);
}
//------------------------------------------------------------------------------------------//
BOOL CMyCListCtrl::DoSaveEdit(BOOL blSave){
	if (m_blEditShown == TRUE){
		if (blSave == TRUE){
			CString cstrText;
			m_Edit.GetWindowText(cstrText); 
			SetItemText(m_EditItem,m_EditSubItem,cstrText.Trim());
			OnEditSave(m_EditItem,m_EditSubItem);
		}
		m_Edit.DestroyWindow();
		m_blEditShown = FALSE;
		return TRUE;
	}
	return FALSE;
}
//------------------------------------------------------------------------------------------//
BOOL CMyCListCtrl::DoShowComboBox(int hItem,int hSubItem){
	if ((m_blEditShown == FALSE) && (m_blComboBoxShown == FALSE)){
		if (hItem != -1){
			CRect editRect;
			m_EditItem = hItem;
			m_EditSubItem = hSubItem;
			GetItemRect(m_EditItem,&editRect,0);
			ReCaclRect(m_EditSubItem,&editRect);
			m_ComboBox.Create(CBS_DROPDOWNLIST|WS_CHILD,editRect,this,0);
			m_ComboBox.SetFont(&m_ComboBoxFont);
			OnComboBoxShow(m_EditItem,m_EditSubItem);
			m_ComboBox.ShowWindow(SW_SHOW);
			m_ComboBox.ShowDropDown();
			m_ComboBox.SetFocus();
			m_blComboBoxShown = TRUE;
		}
	}
	return(m_blComboBoxShown);
}
//------------------------------------------------------------------------------------------//
void CMyCListCtrl::OnComboBoxShow(int hItem,int hSubItem){
	CString	cText;
	cText = GetItemText(m_EditItem,m_EditSubItem);
	m_ComboBox.InsertString(0,cText);
	m_ComboBox.SetCurSel(0);
	m_ComboBox.SetWindowTextW(GetItemText(m_EditItem,hSubItem));
}
//------------------------------------------------------------------------------------------//
BOOL CMyCListCtrl::DoSaveComboBox(BOOL blSave){
	if (m_blComboBoxShown == TRUE){
		if (blSave == TRUE){
			CString cstrText;
			m_ComboBox.GetWindowTextW(cstrText);
			SetItemText(m_EditItem,m_EditSubItem,cstrText.Trim());
			OnComboBoxSave(m_EditItem,m_EditSubItem);
		}
		m_ComboBox.DestroyWindow();
		m_blComboBoxShown = FALSE;
		return TRUE;
	}
	return FALSE;
}
//------------------------------------------------------------------------------------------//
void CMyCListCtrl::ReCaclRect(int col,CRect *editRect){
	int i;
	i = col;
	while(i-- > 0){
		editRect->left += GetColumnWidth(i);
	}
	editRect->right = editRect->left + GetColumnWidth(col);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
