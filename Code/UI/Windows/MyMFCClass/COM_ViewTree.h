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
 * File Name: COM_ViewTree.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2013.12.23
*/
#pragma once
//------------------------------------------------------------------------------------------//
class CMyCTreeShowCEdit : public CEdit{
	public:
				 CMyCTreeShowCEdit(void) : CEdit(){;};
		virtual ~CMyCTreeShowCEdit(void){;};
	protected:// Overrides
		virtual BOOL PreTranslateMessage(MSG* pMsg);
		afx_msg void OnContextMenu		(CWnd* pWnd, CPoint point){;};
		afx_msg void OnKillFocus		(CWnd* pNewWnd);
		DECLARE_MESSAGE_MAP()
};
//------------------------------------------------------------------------------------------//
// CViewTree window
class CMyCTreeCtrl : public CTreeCtrl{
	public:
				 CMyCTreeCtrl(void);
		virtual ~CMyCTreeCtrl(void){;};
	protected:
				void	DrawCheckbox		(CDC& dc,CRect DrawRect,BOOL blChecked);
				void	DrawGroupExpand		(CDC& dc,CRect DrawRect,BOOL blExpand);
				void	DrawAllItem			(CDC& pDc);
				void	DrawOneItem			(CDC& pDc,HTREEITEM ItemNode);
	protected:// Overrides
		virtual BOOL	PreTranslateMessage	(MSG* pMsg);
		virtual BOOL	OnNotify			(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
		//virtual void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	protected:
		CFont				m_DrawFont;
		CMyCTreeShowCEdit	m_Edit;
		BOOL				m_blEditShown;
		HTREEITEM			m_EditItem;
		CFont				m_EditFont;
		virtual BOOL	DoShowEdit			(HTREEITEM hItem);
		virtual BOOL	DoSaveEdit			(BOOL blSave = TRUE);
		virtual void	OnEditSave			(HTREEITEM hItem){;};
		virtual void	OnEditCheckSave		(HTREEITEM hItem){OnEditSave(hItem);};
	protected:
		HTREEITEM		m_SelectItem;
		HTREEITEM		m_HotItem;
		BOOL			m_blMouseTracking;
		INT32			CaclRectLeft		(HTREEITEM treeItem);
		BOOL			DoECClick			(HTREEITEM hItem,CPoint point);
		virtual BOOL	SelectItem			(HTREEITEM hItem);
	// Implementation
	public:
		INT32			CheckNodeLevel		(HTREEITEM hItem);
		HTREEITEM		AddNode				(LPCTSTR lpszItem, BOOL blCheck, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST);
		HTREEITEM		GetSelectedNode		(void);
		void			DelGroupSubNode		(HTREEITEM rootItem);
		void			ExpandAllRoot		(HTREEITEM rootItem = NULL);
		void			CollapseAllRoot		(HTREEITEM rootItem = NULL);
		void			DoDestroyEdit		(void);
	protected:
		afx_msg void	OnPaint				(void);
		afx_msg void	OnLButtonDblClk		(UINT nFlags, CPoint point);
		afx_msg void	OnLButtonDown		(UINT nFlags, CPoint point);
		afx_msg void	OnRButtonDown		(UINT nFlags, CPoint point);
		afx_msg void	OnMouseMove			(UINT nFlags, CPoint point);
		afx_msg void	OnMouseHover		(UINT nFlags, CPoint point);
		afx_msg void	OnMouseLeave		(void);
		afx_msg void	OnSetFocus			(CWnd* pOldWnd);
		afx_msg void	OnVScroll			(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
		afx_msg void	OnHScroll			(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
		DECLARE_MESSAGE_MAP()
};
//------------------------------------------------------------------------------------------//