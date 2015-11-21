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
class CSendCEdit : public CEdit{
	protected:// Overrides
		virtual BOOL PreTranslateMessage(MSG *pMsg){
			UINT  nCode = pMsg->wParam;
			if (pMsg->message == WM_KEYDOWN){
				if (nCode == _T('A') && (::GetKeyState(VK_CONTROL) & 0x8000)){
					SetSel(0,-1);
					return TRUE;
				}
			}
			return(CEdit::PreTranslateMessage(pMsg));
		}
		afx_msg void	OnLButtonDblClk(UINT nFlags, CPoint point){SetSel(0,-1);};
		DECLARE_MESSAGE_MAP()
};
//------------------------------------------------------------------------------------------//
class CSendTimeCEdit : public CEdit{
	protected:// Overrides
		afx_msg void OnContextMenu(CWnd* pWnd, CPoint point){;};
		afx_msg void OnKillFocus(CWnd* pNewWnd);
		DECLARE_MESSAGE_MAP()
};
//------------------------------------------------------------------------------------------//
class CSendPaneView : public CView{
	public:
		DECLARE_DYNCREATE(CSendPaneView)
				 CSendPaneView(void);
		virtual ~CSendPaneView(void);
	public:
		virtual void OnDraw(CDC* pDC);  // overridden to draw this view
		virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
		void	AdjustLayout(void);
	public:
	protected:
		COMMAND_NODE	m_CommandNode;
		CFont		m_ButtonFont;
		CSendCEdit	m_cEdit;
		CSendTimeCEdit	m_TimeEdit;
		CButton		m_GroupSetting;
		CButton		m_GroupAutoSend;
		CButton		m_CheckBoxHex;
		CButton		m_CheckBoxEscape;
		CButton		m_CheckBoxCR;
		CButton		m_CheckBoxSendEnable;
		CButton		m_ButtonSend;
		int			CreateEdit(void);
	protected:
		void		PostNcDestroy();
		BOOL		PreTranslateMessage(MSG *pMsg);
		void		PrintFileName(HDROP hDropInfo);
	protected:
		afx_msg int		OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
		afx_msg int		OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void	OnSize(UINT nType, int cx, int cy);
		afx_msg void	OnHex		(void);
		afx_msg void	OnEscape	(void);
		afx_msg void	OnCR		(void);
		afx_msg void	OnEnable	(void);
		afx_msg void	OnSend		(void);
		afx_msg void	OnDropFiles	(HDROP hDropInfo);
		DECLARE_MESSAGE_MAP()
		#ifdef _DEBUG
			virtual void AssertValid() const;
			virtual void Dump(CDumpContext& dc) const;
		#endif
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
class CSendViewDP : public CDockablePane{
	public:// Construction
				 CSendViewDP(void);
		virtual ~CSendViewDP(void);
	protected:// Attributes
		CSendPaneView		m_cView;
		int		CreateCView(void);
	public:
		void	AdjustLayout(void);
	protected:
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
		afx_msg void OnPaint(void);
		afx_msg void OnSetFocus(CWnd* pOldWnd);
		DECLARE_MESSAGE_MAP()
};
//------------------------------------------------------------------------------------------//
