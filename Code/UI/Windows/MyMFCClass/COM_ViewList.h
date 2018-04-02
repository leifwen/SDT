/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: COM_ViewList.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2013.12.23
*/
#pragma once
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
class CMyCListShowCEdit : public CEdit{
	public:
				 CMyCListShowCEdit(void) : CEdit(){ ; };
		virtual ~CMyCListShowCEdit(void){ ; };
	protected:// Overrides
		virtual BOOL PreTranslateMessage(MSG* pMsg);
		afx_msg void OnContextMenu		(CWnd* pWnd, CPoint point){;};
		DECLARE_MESSAGE_MAP()
};
//------------------------------------------------------------------------------------------//
class CMyCListCtrl : public CListCtrl{
	public:
				 CMyCListCtrl(void);
		virtual ~CMyCListCtrl(void){;};
	protected:
				int		m_EditItem;
				int		m_EditSubItem;
				CFont	m_EditFont;
				CFont	m_ComboBoxFont;
		virtual BOOL	DoShowExItem		(int hItem,int hSubItem,int Type = 0);
	protected:
		CMyCListShowCEdit	m_Edit;
				BOOL		m_blEditShown;
		virtual BOOL		DoShowEdit		(int hItem,int hSubItem);
		virtual BOOL		DoSaveEdit		(BOOL blSave = TRUE);
		virtual void		OnEditSave		(int hItem,int hSubItem){;};
	protected:
		CComboBox		m_ComboBox;
				BOOL	m_blComboBoxShown;
		virtual BOOL	DoShowComboBox		(int hItem,int hSubItem);
		virtual BOOL	DoSaveComboBox		(BOOL blSave = TRUE);
		virtual void	OnComboBoxSave		(int hItem,int hSubItem){;};
		virtual void	OnComboBoxShow		(int hItem,int hSubItem);
	protected:// Overrides
		virtual BOOL 	PreTranslateMessage	(MSG* pMsg);
	protected:
				int		m_SelectItem;
				int		m_SelectSubItem;
				void	ReCaclRect			(int col,CRect* editRect);
	protected:
		afx_msg int		OnCreate			(LPCREATESTRUCT lpCreateStruct);
		afx_msg void	OnMouseMove			(UINT nFlags, CPoint point);
		afx_msg void	OnLButtonDblClk		(UINT nFlags, CPoint point);
		afx_msg void	OnLButtonDown		(UINT nFlags, CPoint point);
		afx_msg void	OnRButtonDown		(UINT nFlags, CPoint point);
		afx_msg void	OnVScroll			(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
		afx_msg void	OnHScroll			(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
		DECLARE_MESSAGE_MAP()
};
//------------------------------------------------------------------------------------------//