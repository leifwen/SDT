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

#pragma once
//------------------------------------------------------------------------------------------//
#include "COM_ViewList.h"

//------------------------------------------------------------------------------------------//
class CSocketListToolBar : public CMFCToolBar{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler){
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}
	virtual BOOL AllowShowOnList() const { return FALSE; }
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
class CSocketListCtrl : public CMyCListCtrl{
	public:
				 CSocketListCtrl(void);
		virtual ~CSocketListCtrl(void){;};
	protected:// Overrides
		virtual BOOL	DoShowExItem(int hItem,int hSubItem,int Type = 0){return FALSE;};
		void			OnItemchangedListFifoGroup(NMHDR* pNMHDR, LRESULT* pResult);
	protected:
		CFont			m_Font;
		ASOCKETSERVER	*m_Server;
	public:
		void	LoadData		(ASOCKETSERVER *server);
		int		GetSelectedNode	(void){return(m_SelectItem);};
		void	Disconnect		(int disItem);
		void	CreateScoketView(ASOCKET *aSocket);
	protected:
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
		afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
		DECLARE_MESSAGE_MAP()
};
extern CSocketListCtrl	g_wndSocketList;
//------------------------------------------------------------------------------------------//
class CSocketListViewDP : public CDockablePane{
	public:
				 CSocketListViewDP(void);
		virtual ~CSocketListViewDP(void);
	protected:
		CSocketListToolBar		m_wndToolBar;
	protected:
		int		CreateScoketList(void);
		int		CreateToolBar(void);
	public:
		void	AdjustLayout(void);
		void	OnChangeVisualStyle(void);
	protected:
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
		afx_msg void OnPaint(void);
		afx_msg void OnSetFocus(CWnd* pOldWnd);
		afx_msg void OnDisconnect(void);
		DECLARE_MESSAGE_MAP()
};
//------------------------------------------------------------------------------------------//
