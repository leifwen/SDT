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

// ChildFrm.h : interface of the CChildFrame class
//

#pragma once
#include "Global.h"
//------------------------------------------------------------------------------------------//
class CChildFrame : public CMDIChildWndEx{
		DECLARE_DYNCREATE(CChildFrame)
	public:
				 CChildFrame(void);
		virtual ~CChildFrame(void);
	public:// Overrides
		std::string		m_selfName;
		ODEV_VG3D		m_VG3D;
		ODEV_STDOUT		cstdout;
		BOOL			m_bCanClose;
		virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	public:// Implementation
		#ifdef _DEBUG
			virtual void AssertValid() const;
			virtual void Dump(CDumpContext& dc) const;
		#endif
	// Generated message map functions
	protected:
		afx_msg void OnClose(void);
		DECLARE_MESSAGE_MAP()
};
//------------------------------------------------------------------------------------------//