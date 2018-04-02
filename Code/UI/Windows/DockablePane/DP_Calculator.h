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
class CPaneCEdit : public CEdit{
	public:
				 CPaneCEdit(void) : CEdit(){ ; };
		virtual ~CPaneCEdit(void){ ; };
	protected:// Overrides
		virtual BOOL 	PreTranslateMessage(MSG *pMsg);
		afx_msg void	OnLButtonDblClk(UINT nFlags, CPoint point);
		DECLARE_MESSAGE_MAP()
};
//------------------------------------------------------------------------------------------//
class CCaclPaneView : public CView{
	public:
		DECLARE_DYNCREATE(CCaclPaneView)
				 CCaclPaneView(void);
		virtual ~CCaclPaneView(void){;};
	public:
		virtual void	OnDraw			(CDC* pDC);  // overridden to draw this view
		virtual BOOL	PreCreateWindow	(CREATESTRUCT& cs);
				void	AdjustLayout	(void);
	public:
	protected:
		COMMAND_NODE	m_CommandNode;
		CFont			m_ButtonFont;
		CPaneCEdit		m_cEdit;
		CButton			m_GroupSetting;
		CButton			m_CheckBoxHex;
		CButton			m_CheckBoxEscape;
		CButton			m_RadioLength;
		CButton			m_RadioH2A;
		CButton			m_RadioA2H;
		CButton			m_RadioXor;
		CButton			m_RadioOSP;
		CButton			m_ButtonCacl;
		int			CreateEdit			(void);
		std::string	CaclNMEAChecksum	(const std::string& strInput);
		std::string	CaclSiRfChecksum	(const std::string& strInput);
		uint32		CaclHEXLength		(const std::string& strInput);
		uint32		CaclASCIILength		(const std::string& strInput);
	protected:
		void		PostNcDestroy		(void);
		BOOL		PreTranslateMessage	(MSG* pMsg);
	protected:
		afx_msg int		OnMouseActivate	(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
		afx_msg int		OnCreate		(LPCREATESTRUCT lpCreateStruct);
		afx_msg void	OnSize			(UINT nType, int cx, int cy);
		afx_msg void	OnHex			(void);
		afx_msg void	OnEscape		(void);
		afx_msg void	OnLength		(void);
		afx_msg void	OnH2A			(void);
		afx_msg void	OnA2H			(void);
		afx_msg void	OnXor			(void);
		afx_msg void	OnOsp			(void);
		afx_msg void	OnCacl			(void);
		DECLARE_MESSAGE_MAP()
		#ifdef _DEBUG
			virtual void AssertValid	(void) const;
			virtual void Dump			(CDumpContext& dc) const;
		#endif
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
class CCaclViewDP : public CDockablePane{
	public:// Construction
				 CCaclViewDP(void) : CDockablePane(){;};
		virtual ~CCaclViewDP(void){;};
	protected:// Attributes
		CCaclPaneView		m_cView;
		int		CreateCView		(void);
	public:
		void	AdjustLayout	(void);
	protected:
		afx_msg int  OnCreate		(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnSize			(UINT nType, int cx, int cy);
		afx_msg void OnContextMenu	(CWnd* pWnd, CPoint point);
		afx_msg void OnPaint		(void);
		afx_msg void OnSetFocus		(CWnd* pOldWnd);
		DECLARE_MESSAGE_MAP()
};
//------------------------------------------------------------------------------------------//
