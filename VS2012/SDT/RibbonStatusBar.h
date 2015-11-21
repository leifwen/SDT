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
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
class CMyMFCRibbonStatusBar : public CMFCRibbonStatusBar{
	public:
				 CMyMFCRibbonStatusBar(void);
		virtual ~CMyMFCRibbonStatusBar(void);
	public:
		void	SetPB(BOOL blStart);
		void	DoPB(void);
	protected:
		BOOL		m_blPBStart;
		CMFCRibbonProgressBar	*m_ProgressBar;
	protected:
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
		DECLARE_MESSAGE_MAP()
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
