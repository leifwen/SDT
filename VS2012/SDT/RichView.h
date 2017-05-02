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
#include "SYS_Lock.h"  
//------------------------------------------------------------------------------------------//
class CON_ReadInLine;
class CMyRichView : public CRichEditView{
	protected: // create from serialization only
		DECLARE_DYNCREATE(CMyRichView)
				CMyRichView(void);
		virtual ~CMyRichView(void);
	private:
		SYS_Lock		cgPrintLock;
		CHARFORMAT2		cgCF;
		CFont			m_ViewFont;
	public:
		CON_ReadInLine	*m_Console;
	public:
		inline	void	Spin_Print_Lock(G_LOCK blVaild = G_LOCK_ON){ cgPrintLock.Lock(blVaild); };
		inline	void	Spin_Print_Unlock(G_LOCK blVaild = G_LOCK_ON){ cgPrintLock.Unlock(blVaild); };
		inline	int32	Spin_Print_Try(G_LOCK blVaild = G_LOCK_ON){ return(cgPrintLock.TryLock(blVaild)); };
	public:
		CHARFORMAT2& SetExPar		(CHARFORMAT2 *tcf,const COLORREF &col);
		void	SetCurFromEnd		(int32 offset);
		void	DelCharFromEnd		(int32 offset, int32 length);
		void	RewriteCharFromEnd	(int32 offset, const COLORREF &col,const STDSTR &strIn);
		void	InsterCharFromEnd	(int32 offset, const COLORREF &col,const STDSTR &strIn);
		void	AppendChar			(const COLORREF &col,const STDSTR &strIn, G_LOCK blLock = G_LOCK_ON);
		void	ToHome				(void);
		void	Clean				(G_LOCK blLock = G_LOCK_ON);

		void	PrintFileName(HDROP hDropInfo);
		void	PrintTxtFile(std::string &tFileName);
		void	PrintHexFile(std::string &tFileName);
		void	PrintRtfFile(std::string &tFileName);
		BOOL	PrintDirList(std::string &tFileName);
		void SaveToRtfFile(std::string &tFileName);
	public:// Overrides
		virtual BOOL PreCreateWindow	(CREATESTRUCT& cs);
		virtual BOOL PreTranslateMessage(MSG *pMsg);
	protected:
		virtual void OnInitialUpdate(void); // called first time after construct
	public:// Implementation
		#ifdef _DEBUG
			virtual void AssertValid(void) const;
			virtual void Dump(CDumpContext& dc) const;
		#endif
	protected:// Generated message map functions
		afx_msg int  OnCreate		(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnRButtonUp	(UINT nFlags, CPoint point);
		afx_msg void OnContextMenu	(CWnd* pWnd, CPoint point);
		afx_msg void OnDropFiles	(HDROP hDropInfo);
		afx_msg	void OnSave			(void);
		afx_msg	void OnPaste		(void);
		DECLARE_MESSAGE_MAP()
};
//------------------------------------------------------------------------------------------//
