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
class CMyRichView : public CRichEditView{
	protected: // create from serialization only
		DECLARE_DYNCREATE(CMyRichView)
				CMyRichView(void);
		virtual ~CMyRichView(void);
	public:// Overrides
				BOOL m_blEnableConsole;
		virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
		virtual BOOL PreTranslateMessage(MSG *pMsg);
	protected:
		CFont		m_ViewFont;
		virtual void OnInitialUpdate(void); // called first time after construct
		void	PrintFileName(HDROP hDropInfo);
		void	PrintTxtFile(std::string &tFileName);
		void	PrintHexFile(std::string &tFileName);
		void	PrintRtfFile(std::string &tFileName);
		BOOL	PrintDirList(std::string &tFileName);
	public:// Implementation
		#ifdef _DEBUG
			virtual void AssertValid(void) const;
			virtual void Dump(CDumpContext& dc) const;
		#endif
		void SaveToRtfFile(std::string &tFileName);
	protected:// Generated message map functions
		afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
		afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
		afx_msg void OnDropFiles(HDROP hDropInfo);
		afx_msg	void OnSave(void);
		afx_msg	void OnPaste(void);
		DECLARE_MESSAGE_MAP()
};
//------------------------------------------------------------------------------------------//
