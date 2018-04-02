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
#include "COM_ViewTree.h"
#include "COM_ToolBar.h"
class	SC_LIST;
//------------------------------------------------------------------------------------------//
class CSCTree : public CMyCTreeCtrl{
	public:
				 CSCTree(void) : CMyCTreeCtrl(){m_SCList = NULL;};
		virtual ~CSCTree(void){;};
	protected:
		SC_LIST*	m_SCList;
		BOOL		DoShowEdit		(HTREEITEM hItem);
		void		OnEditSave		(HTREEITEM hItem);
		HTREEITEM	CreateNodeL1	(HTREEITEM hItem = NULL);
		HTREEITEM	CreateNodeL2	(HTREEITEM hItem = NULL);
		HTREEITEM	DownNodeL1		(HTREEITEM moveItem);
		HTREEITEM	DownNodeL2		(HTREEITEM moveItem);
		HTREEITEM	UpNodeL1		(HTREEITEM moveItem);
		HTREEITEM	UpNodeL2		(HTREEITEM moveItem);
		HTREEITEM	DelNodeL1		(HTREEITEM delItem);
		HTREEITEM	DelNodeL2		(HTREEITEM delItem);
	public:
		void		LoadData		(SC_LIST* tSCList);
		HTREEITEM	CreateNode		(HTREEITEM hItem = NULL);

		HTREEITEM	DelNode			(HTREEITEM delItem);
		HTREEITEM	UpNode			(HTREEITEM moveItem);
		HTREEITEM	DownNode		(HTREEITEM moveItem);
	protected:
		afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
		DECLARE_MESSAGE_MAP()
};
//------------------------------------------------------------------------------------------//
class CSCViewDP : public CDockablePane{
	public:// Construction
				 CSCViewDP(void) : CDockablePane(){;};
		virtual ~CSCViewDP(void){;};
	public:
		CSCTree			m_wndSCTree;
	protected:// Attributes
		CMyToolBar		m_wndToolBar;
	protected:
				int		CreateSCTree			(void);
				int		CreateSCToolBar			(void);
	public:
				void	AdjustLayout			(void);
				void	OnChangeVisualStyle		(void);
	protected:
		afx_msg int		OnCreate				(LPCREATESTRUCT lpCreateStruct);
		afx_msg void	OnSize					(UINT nType, int cx, int cy);
		afx_msg void	OnContextMenu			(CWnd* pWnd, CPoint point);
		afx_msg void	OnPaint					(void);
		afx_msg void	OnSetFocus				(CWnd* pOldWnd);

		afx_msg void	OnSC_Add				(void);
		afx_msg void	OnSC_Del				(void);
		afx_msg void	OnSC_Up					(void);
		afx_msg void	OnSC_Down				(void);
		afx_msg void	OnSC_Expand				(void);
		afx_msg void	OnSC_Collapse			(void);

		afx_msg void	OnButtonClickSC_LOAD	(void);
		afx_msg void	OnButtonClickSC_IMPORT	(void);
		afx_msg void	OnButtonClickSC_SAVE	(void);
		DECLARE_MESSAGE_MAP()
};
//------------------------------------------------------------------------------------------//
