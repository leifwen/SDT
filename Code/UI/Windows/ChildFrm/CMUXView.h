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

// SDTView.h : interface of the CSDTView class
//

#pragma once
//------------------------------------------------------------------------------------------//
class CMyCStatic : public CStatic{
    DECLARE_DYNAMIC(CMyCStatic)
	public:
				 CMyCStatic(void) : CStatic(){;};
		virtual ~CMyCStatic(void){;};
	public:
				void	SetValue	(std::string& tText);
	protected:
		afx_msg void	OnPaint		(void);
		DECLARE_MESSAGE_MAP()
};
//------------------------------------------------------------------------------------------//
class CCMUXCOMCtrl : public CView{
	public:
		DECLARE_DYNCREATE(CCMUXCOMCtrl)
				 CCMUXCOMCtrl(void);
		virtual ~CCMUXCOMCtrl(void){;};
	public:
		CString		m_Title;
		int32		m_ComNumber;
	public:
		void		AdjustLayout(void);
		void		RefreshCOM	(void);
		#ifdef _DEBUG
			virtual void AssertValid(void) const;
			virtual void Dump(CDumpContext& dc) const;
		#endif
	protected:
		CFont		m_ViewFont;
		CButton		m_GroupSetting;
		CButton		m_CheckDSR;
		CButton		m_CheckCTS;
		CButton		m_CheckDCD;
		CButton		m_CheckRING;
		CButton		m_CheckDTR;
		CButton		m_CheckRTS;
		CButton		m_CheckHEX;
		CButton		m_CheckEscape;
		CComboBox	m_ComboCOM;
		CComboBox	m_ComboBR;
		CButton		m_ButtonOpen;
		CMyCStatic	m_StaticTx;
		CMyCStatic	m_StaticRx;
		IPCOMLIST	m_ValidAuxComList;
		BOOL		m_blConnect;
#ifdef	SWVERSION_CMUX
		VCOM*		m_CMUXCOM;
#endif
				int		CreateEdit				(void);
		virtual void 	OnDraw					(CDC* pDC);  // overridden to draw this view
		virtual void 	PostNcDestroy			(void){;};
				void	InitBR					(void);
				void	InitComboBox_COM		(void);
				void	SetSelectComboBox_COM	(void);
				void	CreateComboBox_COMList	(void);
		
	protected:
		afx_msg int		OnMouseActivate	(CWnd* pDesktopWnd, UINT nHitTest, UINT message){return 0;};
		afx_msg int		OnCreate		(LPCREATESTRUCT lpCreateStruct);
		afx_msg void	OnContextMenu	(CWnd* pWnd, CPoint point);
		afx_msg void	OnSize			(UINT nType, int cx, int cy);
		afx_msg void	OnCOMBO_COM		(void);
		afx_msg void	OnOpen			(void);
		afx_msg void	OnDTR			(void);
		afx_msg void	OnRTS			(void);
		afx_msg void	OnHEX			(void);
		afx_msg void	OnEscape		(void);
		DECLARE_MESSAGE_MAP()
};
//------------------------------------------------------------------------------------------//



//------------------------------------------------------------------------------------------//
class CCMUXView : public CView{
	protected: // create from serialization only
		DECLARE_DYNCREATE(CCMUXView)
				 CCMUXView(void);
		virtual ~CCMUXView(void){;};
	protected:
		CFont			m_ViewFont;
		CCMUXCOMCtrl	m_CMUXCOMCtrl1;
		CCMUXCOMCtrl	m_CMUXCOMCtrl2;
		CCMUXCOMCtrl	m_CMUXCOMCtrl3;
		CCMUXCOMCtrl	m_CMUXCOMCtrl4;
	protected:
		virtual void 	OnDraw		(CDC* pDC);  // overridden to draw this view
	public:// Implementation
				int		CreateEdit	(void);
				void	AdjustLayout(void);
				void	RefreshCOM	(void);
		#ifdef _DEBUG
			virtual void AssertValid(void) const;
			virtual void Dump		(CDumpContext& dc) const;
		#endif
	protected:// Generated message map functions
		afx_msg int		OnCreate		(LPCREATESTRUCT lpCreateStruct);
		afx_msg void	OnContextMenu	(CWnd* pWnd, CPoint point);
		afx_msg void	OnSize			(UINT nType, int cx, int cy);
		DECLARE_MESSAGE_MAP()
};
//------------------------------------------------------------------------------------------//
